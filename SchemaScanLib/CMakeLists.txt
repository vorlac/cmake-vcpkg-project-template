project(SchemaScanLib
  LANGUAGES
    C CXX
  VERSION
    0.0.1
)

find_package(podofo CONFIG REQUIRED)

add_library(${PROJECT_NAME} "")

file(GLOB_RECURSE
  schema_scan_lib_sources
    CONFIGURE_DEPENDS
      "${CMAKE_CURRENT_SOURCE_DIR}/SchemaScanLib.h"
      "${CMAKE_CURRENT_SOURCE_DIR}/Schematic.hpp"
      "${CMAKE_CURRENT_SOURCE_DIR}/Schematic.cpp"
      "${CMAKE_CURRENT_SOURCE_DIR}/deps/hashlib2plus/src/*.[hc]pp"
)

target_sources(${PROJECT_NAME}
  PRIVATE
    ${schema_scan_lib_sources}
)

target_include_directories(${PROJECT_NAME}
  PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}/deps/hashlib2plus/src/"
    "${PODOFO_INCLUDE_DIR}"
    "${CMAKE_CURRENT_SOURCE_DIR}/"
)

target_link_libraries(${PROJECT_NAME}
  PUBLIC $<IF:$<TARGET_EXISTS:podofo_shared>,podofo_shared,podofo_static>
)
