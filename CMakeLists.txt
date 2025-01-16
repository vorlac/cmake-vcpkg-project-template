cmake_minimum_required(VERSION 3.22)
set(CMAKE_MESSAGE_LOG_LEVEL STATUS)

list(APPEND CMAKE_MODULE_PATH
    "${CMAKE_SOURCE_DIR}/cmake"
    "${CMAKE_SOURCE_DIR}/extern"
    "${CMAKE_SOURCE_DIR}/extern/hunter/cmake/modules"
    "${CMAKE_SOURCE_DIR}/extern/hunter/cmake/find"
)

include(submodule-init)
include(vcpkg-init)

set(HUNTER_CONFIGURATION_TYPES Release CACHE STRING "Hunter configuration types")
set(HUNTER_JOBS_NUMBER 4 CACHE STRING "Hunter jobs number")
set(HUNTER_CACHE_SERVERS "https://github.com/ethereum/hunter-cache" CACHE STRING "Hunter cache servers")
set(HUNTER_PASSWORDS_PATH ${CMAKE_SOURCE_DIR}/extern/hunter/passwords.cmake CACHE STRING "Hunter password path")

include(etherium-cable/bootstrap)
include(etherium-cable/CableBuildInfo)
include(etherium-cable/CableBuildType)
include(etherium-cable/CableToolchains)

include(hunter_add_package)

include(GNUInstallDirs)

# include(EthCcache)
# include(EthOptions)
# include(EthCompilerSettings)
# include(EthExecutableHelper)
# include(EthDependencies)
# include(EthUtils)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_EXTENSIONS ON)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_COLOR_DIAGNOSTICS ON)
set(CMAKE_DIRECTORY_LABELS ON)

set(PROJECT_VERSION 0.0.1)
set(PROJECT_VERSION_MAJOR 0)
set(PROJECT_VERSION_MINOR 0)
set(PROJECT_VERSION_PATCH 1)

project(aleth
  LANGUAGES C CXX
  VERSION "${PROJECT_VERSION}"
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

cable_configure_toolchain(DEFAULT cxx14-pic)
cable_add_buildinfo_library(PROJECT_NAME aleth)

find_package(PkgConfig REQUIRED)
find_package(fmt CONFIG REQUIRED)

add_subdirectory(${CMAKE_SOURCE_DIR}/extern/etherium-ethash)

#hunter_add_package(Boost COMPONENTS program_options filesystem system thread context fiber log)
find_package(Boost CONFIG REQUIRED COMPONENTS program_options filesystem system thread context fiber log)

#hunter_add_package(JsonCpp)
find_package(JsonCpp CONFIG REQUIRED) #

#hunter_add_package(Snappy)
find_package(Snappy CONFIG REQUIRED) #

#hunter_add_package(cryptopp)
set(CRYPTOPP_BUILD_SHARED OFF)
find_package(CryptoPP CONFIG REQUIRED) #
pkg_check_modules(CryptoPP REQUIRED)

#hunter_add_package(unofficial-json-rpc-cxx)
find_package(unofficial-json-rpc-cxx CONFIG REQUIRED)

#hunter_add_package(libethash)

file(GLOB_RECURSE
  project_sources
    CONFIGURE_DEPENDS
      "${CMAKE_CURRENT_SOURCE_DIR}/src/*.[hc]"
      "${CMAKE_CURRENT_SOURCE_DIR}/src/*.[hc]pp"
      "${CMAKE_CURRENT_SOURCE_DIR}/extern/libscrypt/*.[hc]pp"
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
)

target_include_directories(${PROJECT_NAME} PRIVATE
  "${CMAKE_SOURCE_DIR}/src"
  "${CMAKE_SOURCE_DIR}/extern/libscrypt"
  "${Boost_INCLUDE_DIR}"
)

target_link_libraries(${PROJECT_NAME} PRIVATE
  fmt::fmt
  cryptopp::cryptopp
  unofficial::json-rpc-cxx::json-rpc-cxx
  JsonCpp::JsonCpp
  Snappy::snappy
  Boost::boost
  Boost::system
  Boost::filesystem
  ${Boost_Libraries}
)

if (AUTOFORMAT_SRC_ON_CONFIGURE MATCHES ON)
  # Formatting rules defined by .clangformat
  # Scoped to src/*.[h]pp and src/*.[c]pp
  include(clang-format)
endif()

include(cmake-utils)
