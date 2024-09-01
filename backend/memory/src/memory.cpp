#include <memory.hpp>

std::optional<uintptr_t> backend::Memory::base_address(const std::string_view& module) {
  std::unique_lock _{_mutex};

  if (!VMMDLL_Map_GetModuleFromNameU(_handle, _pid, module.data(), &_moduleEntry, NULL))
    return std::nullopt;

  return _moduleEntry->vaBase;
}

void backend::Memory::fix_dtb() {
  if (VMMDLL_Map_GetModuleFromNameU(_handle, _pid, PROCESS_NAME, &_moduleEntry, NULL)) {
    logger->debug("Skipping DTB fix");

    return;
  }

  auto memoryPage = reinterpret_cast<PBYTE>(malloc(0x1000000));
  DWORD memoryRead = {};

  memset(memoryPage, '\0', 0x1000000);

  logger->debug("Reading DTB list");

  if (VMMDLL_VfsReadU(_handle, "\\misc\\procinfo\\dtb.txt", memoryPage, 0x1000000, &memoryRead,
                      0)) {
    logger->error("Failed to read DTB list");

    return;
  }

  std::string dtbList(reinterpret_cast<char*>(memoryPage), memoryRead);

  free(memoryPage);

  logger->debug("Finding new DTB");

  while (dtbList.find('\n') != std::string::npos) {
    const auto& endNewline = dtbList.find('\n');
    const auto& line = dtbList.substr(0, endNewline);

    dtbList = dtbList.substr(endNewline + 1, dtbList.size() - endNewline);

    uintptr_t dtb{};

    std::sscanf(line.c_str(), "%x %d %llx", &dtb, &dtb, &dtb);

    VMMDLL_ConfigSet(_handle, VMMDLL_OPT_PROCESS_DTB | _pid, dtb);

    if (VMMDLL_Map_GetModuleFromNameU(_handle, _pid, PROCESS_NAME, &_moduleEntry, NULL)) {
      logger->debug("Fixed DTB");
      logger->debug("DTB = 0x{:x}", dtb);
      logger->debug("{}->vaBase = 0x{:x}", PROCESS_NAME, _moduleEntry->vaBase);

      return;
    }
  }

  logger->error("Failed to fix DTB");
}