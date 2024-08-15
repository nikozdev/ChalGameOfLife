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

include(FetchContent)

function(fAddPackage vPackName vCompName vLinkName vRepoUrl vRepoTag)
  find_package(${vPackName} COMPONENTS ${vCompName} QUIET)
  if(${vPackName}_FOUND)
    message(STATUS "Package has been found: \"${vPackName}\"; comp: \"${vCompName}\";")
  else()
    message(STATUS "Package is being fetched: \"${vPackName}\" comp: \"${vCompName}\";")
    FetchContent_Declare(${vPackName}
      GIT_REPOSITORY  ${vRepoUrl}
      GIT_TAG         ${vRepoTag}
      GIT_SHALLOW     TRUE
    )
    FetchContent_MakeAvailable(${vPackName})
    find_package(${vPackName} COMPONENTS ${vCompName} REQUIRED)
  endif()
  target_link_libraries(${vExeName} PRIVATE ${vLinkName})
endfunction(fAddPackage)

fAddPackage("fmt" "" "fmt::fmt" "https://github.com/fmtlib/fmt.git" "11.0.2")
fAddPackage("Qt6" "Core" "Qt6::Core" "https://code.qt.io/qt/qt5.git" "v6.7.2")
fAddPackage("Qt6" "Widgets" "Qt6::Widgets" "https://code.qt.io/qt/qt5.git" "v6.7.2")
