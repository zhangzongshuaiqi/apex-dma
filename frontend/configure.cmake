function(configure)
  cmake_parse_arguments(CONFIGURE "" "TARGET" "DEPENDENCIES;MODULES" ${ARGN})
  
  message(STATUS "Configuring ${CONFIGURE_TARGET}-frontend")
  
  set(CONFIGURE_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/frontend/${CONFIGURE_TARGET})
  set(SHARED_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/shared)
  
  project(${CONFIGURE_TARGET})
  
  set(CMAKE_C_STANDARD 26)
  set(CMAKE_C_STANDARD_REQUIRED ON)
  
  set(CMAKE_CXX_STANDARD 26)
  set(CMAKE_CXX_STANDARD_REQUIRED ON)
  
  add_compile_options(-Ofast -fno-fast-math -march=native -fno-omit-frame-pointer)
  add_link_options(-static -pthread -lpthread)
    
  include_directories(${CONFIGURE_DIRECTORY}/include ${SHARED_DIRECTORY}/include)

  file(GLOB_RECURSE SOURCES ${CONFIGURE_DIRECTORY}/**.cpp
                            ${CONFIGURE_DIRECTORY}/**.hpp
                            ${CONFIGURE_DIRECTORY}/**.c
                            ${CONFIGURE_DIRECTORY}/**.h
                            ${SHARED_DIRECTORY}/**.cpp
                            ${SHARED_DIRECTORY}/**.hpp
                            ${SHARED_DIRECTORY}/**.c
                            ${SHARED_DIRECTORY}/**.h)
  
  add_library(${CONFIGURE_TARGET} STATIC ${SOURCES})
  
  target_precompile_headers(${CONFIGURE_TARGET} PRIVATE ${SHARED_DIRECTORY}/include/pch.hpp)
  
  if (CONFIGURE_MODULES) 
    add_dependencies(${CONFIGURE_TARGET} ${CONFIGURE_MODULES})
    
    target_link_libraries(${CONFIGURE_TARGET} ${CONFIGURE_MODULES})
  endif()
  
  if (CONFIGURE_DEPENDENCIES)
    target_link_libraries(${CONFIGURE_TARGET} ${CONFIGURE_DEPENDENCIES})
  endif()
endfunction()