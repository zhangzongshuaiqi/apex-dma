include("frontend/configure.cmake")

configure(TARGET "menu" 
          MODULES "imgui" "stopper" "config" "memory")

configure(TARGET "visuals"
          MODULES "render")