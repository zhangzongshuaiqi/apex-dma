#pragma once

#include <pch.hpp>

namespace backend {
class LOGGER(Memory) {
public:
  Memory() {
    char const* fpgaArguments[] = {"", "-device", "fpga"};

    logger->debug("Loading MemProcFs");

    if (_handle = VMMDLL_Initialize(sizeof(fpgaArguments) / sizeof(LPSTR), fpgaArguments);
        !_handle) {
      logger->critical("Failed to initialize MemProcFs");

      return;
    }

    logger->debug("Loading leechcore");

    LC_CONFIG config = {.dwVersion = LC_CONFIG_VERSION, .szDevice = "existing"};

    if (HANDLE lcHandle = LcCreate(&config)) {
      LcCommand(lcHandle, LC_CMD_FPGA_CFGREGPCIE_MARKWR | 0x2, 4, (BYTE[4]){0x10, 0x0, 0x10, 0x0},
                NULL, NULL);

      LcClose(lcHandle);
    } else {
      logger->error("Failed to initialize leechcore");

      return;
    }

    logger->debug("Getting PID from {}", PROCESS_NAME);

    if (!VMMDLL_PidGetFromName(_handle, PROCESS_NAME, &_pid)) {
      logger->error("Failed to get PID");

      return;
    }

    logger->debug("{}->PID = {}", PROCESS_NAME, _pid);

    logger->debug("Loading plugins");

    if (!VMMDLL_InitializePlugins(_handle)) {
      logger->critical("Failed to initialize plugins!");

      return;
    }

    logger->debug("Waiting for plugin routines");

    auto memoryPage = reinterpret_cast<PBYTE>(malloc(0x4));
    DWORD memoryRead = {};

    memset(memoryPage, '\0', 0x4);

    while (!VMMDLL_VfsReadU(_handle, "\\misc\\procinfo\\progress_percent.txt", memoryPage, 0x4,
                            &memoryRead, 0)) {
      if (std::string_view(reinterpret_cast<char*>(memoryPage), memoryRead).contains("100"))
        break;

      std::this_thread::sleep_for(std::chrono::seconds(static_cast<int32_t>(1)));
    }

    free(memoryPage);

    logger->debug("Fixing DTB");

    fix_dtb();
  }

  ~Memory() {
    if (_handle)
      VMMDLL_Close(_handle);
  }

  template <typename T = uintptr_t>
  std::optional<T> read(const uintptr_t& address) {
    std::unique_lock _{_mutex};

    T buf{};

    if (!VMMDLL_MemReadEx(_handle, _pid, address, reinterpret_cast<PBYTE>(&buf), sizeof(T), nullptr,
                          VMMDLL_FLAG_NOCACHE))
      return std::nullopt;

    return buf;
  }

  template <typename T = uintptr_t, size_t L>
  std::optional<std::array<T, L>> read(const uintptr_t& address) {
    std::unique_lock _{_mutex};

    std::array<T, L> buf{};

    if (!VMMDLL_MemReadEx(_handle, _pid, address, reinterpret_cast<PBYTE>(buf.data()),
                          sizeof(T) * L, nullptr, VMMDLL_FLAG_NOCACHE))
      return std::nullopt;

    return buf;
  }

  void fix_dtb();

  std::optional<uintptr_t> base_address(const std::string_view& module);

  const std::string& fpga_version() const {
    static std::string version{};

    if (!version.empty())
      return version;

    uint64_t major{}, minor{};

    VMMDLL_ConfigGet(_handle, LC_OPT_FPGA_VERSION_MAJOR, &major);
    VMMDLL_ConfigGet(_handle, LC_OPT_FPGA_VERSION_MINOR, &minor);

    version = std::format("{}.{}", major, minor);

    return version;
  }

  const std::string& fpga_id() const {
    static std::string fpga{};

    if (!fpga.empty())
      return fpga;

    uint64_t fpgaId{};

    VMMDLL_ConfigGet(_handle, LC_OPT_FPGA_FPGA_ID, &fpgaId);

    fpga = std::format("0x{:X}", fpgaId);

    return fpga;
  }

  const std::string& windows_version() const {
    static std::string version{};

    if (!version.empty())
      return version;

    uint64_t _build{};

    VMMDLL_ConfigGet(_handle, VMMDLL_OPT_WIN_VERSION_BUILD, &_build);

    version = std::format("{}", _build);

    return version;
  }

private:
  DWORD _pid{};

  VMM_HANDLE _handle{};
  PVMMDLL_MAP_MODULEENTRY _moduleEntry{};

  std::shared_mutex _mutex{};
};
}  // namespace backend