function(configure)
  cmake_parse_arguments(CONFIGURE "" "TARGET" "LIB;INCLUDE;MODULES;COPY;DEF" ${ARGN})
  
  message(STATUS "Configuring ${CONFIGURE_TARGET}-library")
  
  set(CONFIGURE_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/libraries/${CONFIGURE_TARGET})
  
  project(${CONFIGURE_TARGET})
  
  set(CMAKE_C_STANDARD 26)
  set(CMAKE_C_STANDARD_REQUIRED ON)
  
  set(CMAKE_CXX_STANDARD 26)
  set(CMAKE_CXX_STANDARD_REQUIRED ON)
  
  add_compile_options(-Ofast -fno-fast-math -march=native -fno-omit-frame-pointer)
  add_link_options(-static -pthread -lpthread)

  add_compile_definitions(${CONFIGURE_DEF})
  
  include_directories(${CONFIGURE_DIRECTORY})
  
  if (CONFIGURE_INCLUDE)
    include_directories(${CONFIGURE_DIRECTORY}/${CONFIGURE_INCLUDE})
  endif()
  
  file(GLOB_RECURSE SOURCES ${CONFIGURE_DIRECTORY}/**.cpp
                            ${CONFIGURE_DIRECTORY}/**.hpp
                            ${CONFIGURE_DIRECTORY}/**.c
                            ${CONFIGURE_DIRECTORY}/**.h)
  
  add_library(${CONFIGURE_TARGET} STATIC ${SOURCES})

  set_target_properties(${CONFIGURE_TARGET} PROPERTIES LINKER_LANGUAGE CXX)
  
  if (CONFIGURE_LIB)
    link_directories(${CONFIGURE_DIRECTORY}/lib)
    
    find_library(CONFIGURE_LIB_PATH ${CONFIGURE_LIB} PATHS ${CONFIGURE_DIRECTORY}/lib)
    
    target_link_libraries(${CONFIGURE_TARGET} ${CONFIGURE_LIB_PATH})
  endif()

  if (CONFIGURE_MODULES) 
    add_dependencies(${CONFIGURE_TARGET} ${CONFIGURE_MODULES})
    
    target_link_libraries(${CONFIGURE_TARGET} ${CONFIGURE_MODULES})
  endif()

  if (CONFIGURE_COPY)
    foreach(FILE IN LISTS CONFIGURE_COPY)
      configure_file("${CONFIGURE_DIRECTORY}/file/${FILE}" "${FILE}" COPYONLY)
    endforeach()
  endif()
endfunction()