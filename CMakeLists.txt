cmake_minimum_required(VERSION 3.16)

# proj

set(vProjName "ChalGameOfLife")

project(${vProjName}
  VERSION 0.0.0
  DESCRIPTION "challenge: the game of life"
  ) # project

# lang

enable_language(CXX)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

# file

set(vProjPath "${CMAKE_SOURCE_DIR}")
file(GLOB vSrcPathArray "src/*.cpp")

# make

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# exec

set(vExeName p${vProjName}.exe)
set(vExePath ${CMAKE_BINARY_DIR}/${vExeName}.exe)

add_executable(${vExeName})
target_sources(${vExeName} PRIVATE ${vSrcPathArray})
target_compile_definitions(${vExeName} PRIVATE
  dProjName="${vProjName}"
  dProjPath="${vProjPath}"
  QT_DISABLE_DEPRECATED_UP_TO=0x050f00
  ) # target_compile_definitions
target_precompile_headers(${vExeName} PRIVATE ./src/top.hpp)
target_compile_options(${vExeName} PRIVATE
  "-fpermissive"
  ) # target_compile_options

## libs

find_package(fmt REQUIRED)
target_link_libraries(${vExeName} PRIVATE fmt::fmt)

find_package(Qt6 REQUIRED COMPONENTS Widgets)
target_link_libraries(${vExeName} PRIVATE Qt6::Widgets)
