cmake_minimum_required(VERSION 3.20)
project("rasterizer")

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(program_name ${CMAKE_PROJECT_NAME})

file(GLOB_RECURSE src_files CONFIGURE_DEPENDS src/*.cpp)

set(gcc_like_cxx "$<COMPILE_LANG_AND_ID:CXX,ARMClang,AppleClang,Clang,GNU,LCC>")
set(msvc_cxx "$<COMPILE_LANG_AND_ID:CXX,MSVC>")
set(compile_options
  "$<${gcc_like_cxx}:-pedantic-errors;-Wall;-Weffc++;-Wextra;-Wsign-conversion;-Wconversion>"
  "$<${msvc_cxx}:/permissive-;/W4;/WX>"
)

include(FetchContent)

FetchContent_Declare(
  glfw
  GIT_REPOSITORY https://github.com/glfw/glfw.git
  GIT_TAG 3.4
  GIT_SHALLOW TRUE
)

FetchContent_MakeAvailable(glfw)

add_subdirectory(lib/glad)
add_subdirectory(lib/stb_image)

add_executable(${program_name} ${src_files})
target_include_directories(${program_name} PRIVATE src)
target_compile_options(${program_name} PRIVATE ${compile_options})
target_link_libraries(${program_name} PRIVATE glfw glad stb_image)