cmake_minimum_required(VERSION 3.22)
set(CMAKE_MESSAGE_LOG_LEVEL STATUS)

list(APPEND CMAKE_MODULE_PATH
  "${CMAKE_CURRENT_SOURCE_DIR}/cmake"
)

include(vcpkg-init)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_EXTENSIONS ON)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(EXPORT_COMPILE_COMMANDS ON)
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
      ON
)

string(TOLOWER "${CMAKE_SYSTEM_NAME}" host_os)
string(TOLOWER "${CMAKE_BUILD_TYPE}" build_type)
string(TOLOWER "${CMAKE_CXX_COMPILER_ARCHITECTURE_ID}" architecture)
set(CMAKE_DEBUG_POSTFIX ".${build_type}.${architecture}")

set(compiler_is_msvc "$<CXX_COMPILER_ID:MSVC>")
set(compiler_is_gnu "$<CXX_COMPILER_ID:GNU>")
set(compiler_is_clang "$<OR:$<CXX_COMPILER_ID:AppleClang>,$<CXX_COMPILER_ID:Clang>>")

set(FORCE_STATIC_VCRT OFF)
set(BUILD_SHARED_LIBS ON)

# SDL3 build options
set(SDL_LIBC OFF)
set(SDL_VULKAN ON)
set(SDL_SHARED ON)
set(SDL_STATIC ON)
set(SDL2IMAGE_BUILD_SHARED_LIBS ON)

set(SDL_TESTS OFF)
set(SDL_TEST_LIBRARY OFF)

set(SDL2IMAGE_AVIF ON)
set(SDL2IMAGE_BMP ON)
set(SDL2IMAGE_GIF ON)
set(SDL2IMAGE_JPG ON)
set(SDL2IMAGE_PNG ON)
set(SDL2IMAGE_TIF ON)
set(SDL2IMAGE_JPG_SAVE ON)
set(SDL2IMAGE_PNG_SAVE ON)
set(SDL2IMAGE_VENDORED OFF)

find_package(PkgConfig REQUIRED)

find_package(PNG REQUIRED)
find_package(fmt CONFIG REQUIRED)
find_package(SDL2 CONFIG REQUIRED)
find_package(SDL2_image CONFIG REQUIRED)
find_package(SDL2_ttf CONFIG REQUIRED)
find_package(SDL2_mixer CONFIG REQUIRED)
find_package(SDL2_net CONFIG REQUIRED)

file(GLOB_RECURSE
  project_sources
    CONFIGURE_DEPENDS
      "${CMAKE_CURRENT_SOURCE_DIR}/src/*.[hc]"
      "${CMAKE_CURRENT_SOURCE_DIR}/src/*.[hc]pp"
      "${CMAKE_CURRENT_SOURCE_DIR}/include/*.[hc]"
      "${CMAKE_CURRENT_SOURCE_DIR}/include/*.[hc]pp"
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
  >
)

target_compile_options(${PROJECT_NAME} PRIVATE
  $<${compiler_is_msvc}:
      /std:c++latest
      /Zc:preprocessor
      /utf-8
      /EHsc
      /nologo
  >
)

target_include_directories(${PROJECT_NAME}
  PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/src"
  PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/include/headers"
)

target_link_options(${PROJECT_NAME}
  PRIVATE
    $<${compiler_is_msvc}:
      /subsystem:console
    >
)

target_link_libraries(${PROJECT_NAME}
  PRIVATE fmt::fmt
  PRIVATE PNG::PNG
  PRIVATE $<TARGET_NAME_IF_EXISTS:SDL2::SDL2main>
  PRIVATE $<IF:$<TARGET_EXISTS:SDL2::SDL2>,SDL2::SDL2,SDL2::SDL2-static>
  PRIVATE $<IF:$<TARGET_EXISTS:SDL2_image::SDL2_image>,SDL2_image::SDL2_image,SDL2_image::SDL2_image-static>
  PRIVATE $<IF:$<TARGET_EXISTS:SDL2_ttf::SDL2_ttf>,SDL2_ttf::SDL2_ttf,SDL2_ttf::SDL2_ttf-static>
  PRIVATE $<IF:$<TARGET_EXISTS:SDL2_mixer::SDL2_mixer>,SDL2_mixer::SDL2_mixer,SDL2_mixer::SDL2_mixer-static>
  PRIVATE $<IF:$<TARGET_EXISTS:SDL2_net::SDL2_net>,SDL2_net::SDL2_net,SDL2_net::SDL2_net-static>
)

if (AUTOFORMAT_SRC_ON_CONFIGURE MATCHES ON)
    include(clang-format)
endif()

add_custom_command(TARGET ${PROJECT_NAME}
  PRE_BUILD COMMAND
    ${CMAKE_COMMAND} -E create_symlink
      ${CMAKE_CURRENT_SOURCE_DIR}/assets
      $<TARGET_FILE_DIR:${PROJECT_NAME}>/assets
)

include(cmake-utils)
