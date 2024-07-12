cmake_minimum_required(VERSION 3.22)
set(CMAKE_MESSAGE_LOG_LEVEL STATUS)

list(APPEND CMAKE_MODULE_PATH
  "${CMAKE_CURRENT_SOURCE_DIR}/cmake"
)

include(vcpkg-init)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_EXTENSIONS ON)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_COLOR_DIAGNOSTICS ON)
set(CMAKE_DIRECTORY_LABELS ON)

project(myprogram
  LANGUAGES
    C CXX
  VERSION
    0.0.1
)

option(
  AUTOFORMAT_SRC_ON_CONFIGURE
    "If enabled, clang-format will be used to format all sources in src/ during configuration"
      OFF # ON
)

string(TOLOWER "${CMAKE_SYSTEM_NAME}" host_os)
string(TOLOWER "${CMAKE_BUILD_TYPE}" build_type)
string(TOLOWER "${CMAKE_CXX_COMPILER_ARCHITECTURE_ID}" architecture)
set(CMAKE_DEBUG_POSTFIX ".${build_type}.${architecture}")

set(compiler_is_msvc "$<CXX_COMPILER_ID:MSVC>")
set(compiler_is_gnu "$<CXX_COMPILER_ID:GNU>")
set(compiler_is_clang "$<OR:$<CXX_COMPILER_ID:AppleClang>,$<CXX_COMPILER_ID:Clang>>")

set(SDL_STATIC ON)
set(SDL_SHARED OFF)
set(BUILD_SHARED_LIBS OFF)

set(SDL3IMAGE_DEPS_SHARED OFF)
set(SDL3MIXER_DEPS_SHARED OFF)
set(SDL3_DEPS_SHARED OFF)

set(SDL_LIBC OFF)
set(FORCE_STATIC_VCRT OFF)

set(SDL3TTF_VENDORED OFF)
set(SDL3IMAGE_VENDORED OFF)
set(SDL3MIXER_VENDORED OFF)

find_package(PkgConfig REQUIRED)
find_package(Freetype CONFIG REQUIRED)
find_package(harfbuzz CONFIG REQUIRED)
find_package(Ogg CONFIG REQUIRED)
find_package(PNG REQUIRED)
find_package(TIFF REQUIRED)
find_package(wavpack CONFIG REQUIRED)
find_package(WebP CONFIG REQUIRED)

pkg_check_modules(dav1d REQUIRED IMPORTED_TARGET dav1d)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/extern/sdl)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/extern/sdl_ttf)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/extern/sdl_image)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/extern/sdl_mixer)

file(GLOB_RECURSE
  project_sources
    CONFIGURE_DEPENDS
      "${CMAKE_CURRENT_SOURCE_DIR}/src/*.[hc]"
      "${CMAKE_CURRENT_SOURCE_DIR}/src/*.[hc]pp"
)

add_executable(${PROJECT_NAME}
  ${project_sources}
)

include(compiler-warnings)

target_compile_features(${PROJECT_NAME} PRIVATE
  PRIVATE
    cxx_std_23
)

target_compile_definitions(${PROJECT_NAME} PRIVATE
  NOMINMAX

  $<${compiler_is_msvc}:
      WIN32_LEAN_AND_MEAN
      VC_EXTRALEAN

      $<$<CONFIG:Debug>:
          # debug only msvc defs
      >
  >

  $<${compiler_is_gnu}:
      # gcc defs here
  >

  $<${compiler_is_clang}:
      # clang defs here
  >
)

target_compile_options(${PROJECT_NAME} PRIVATE
  $<${compiler_is_msvc}:
      /std:c++latest
      /Zc:preprocessor
      /utf-8
      /EHsc
      /nologo
      /UNICODE
  >

  $<$<CONFIG:Release>:
      /Ox
      /MD
  >

  $<$<CONFIG:Debug>:
      /MDd
  >
)

target_include_directories(${PROJECT_NAME}
  PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/src"
)

target_link_libraries(${PROJECT_NAME}
    PRIVATE SDL3::SDL3
    PRIVATE SDL3_ttf::SDL3_ttf
    PRIVATE SDL3_image::SDL3_image
    PRIVATE SDL3_mixer::SDL3_mixer
)

if (AUTOFORMAT_SRC_ON_CONFIGURE MATCHES ON)
  # Formatting rules defined by .clangformat
  # Scoped to src/*.[h]pp and src/*.[c]pp
  include(clang-format)
endif()

add_custom_command(TARGET ${PROJECT_NAME}
  PRE_BUILD COMMAND
    ${CMAKE_COMMAND} -E copy_directory_if_different
      ${CMAKE_CURRENT_SOURCE_DIR}/resources
      $<TARGET_FILE_DIR:${PROJECT_NAME}>/resources
)

include(cmake-utils)
