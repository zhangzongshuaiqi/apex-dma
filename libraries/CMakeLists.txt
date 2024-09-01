include("libraries/configure.cmake")

configure(TARGET "imgui")

configure(TARGET "memprocfs"
          LIB "leechcore" "vmm" "FTD3XX"
          COPY "FTD3XX.dll" "dbghelp.dll" "leechcore.dll" "vmm.dll" "info.db" "symsrv.dll")

configure(TARGET "glm")

configure(TARGET "json")

configure(TARGET "nameof")

configure(TARGET "spdlog"
          DEF "SPDLOG_NO_TLS")