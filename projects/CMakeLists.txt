include("projects/configure.cmake")

configure(TARGET "apex" 
          PROCESS "r5apex.exe"
          MODULES "display" "input" "render" "menu" "imgui"  "stopper" "memory" "glm" "visuals"
          DEPENDENCIES "dwmapi" "d3d11" "dxgi" "d3dcompiler" "vmm")