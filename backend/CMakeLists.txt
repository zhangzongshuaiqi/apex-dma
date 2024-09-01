include("backend/configure.cmake")

configure(TARGET "stopper")

configure(TARGET "input" 
          MODULES "imgui" "stopper")

configure(TARGET "memory"
          MODULES "memprocfs" "spdlog")

configure(TARGET "display" 
          MODULES "input"
          DEPENDENCIES "dwmapi")

configure(TARGET "render" 
          MODULES "display" "imgui"
          DEPENDENCIES "d3d11" "dxgi" "d3dcompiler")

configure(TARGET "config"
          MODULES "json")