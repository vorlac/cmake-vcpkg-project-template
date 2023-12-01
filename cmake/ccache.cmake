
find_program(CCACHE_PROGRAM NAMES ccache)

if (CCACHE_PROGRAM)
    execute_process(
        COMMAND "${CCACHE_PROGRAM}" --version
        OUTPUT_VARIABLE CCACHE_VERSION
    )

    string(REGEX MATCH "[^\r\n]*" CCACHE_VERSION ${CCACHE_VERSION})
    message(STATUS "Using ccache: ${CCACHE_PROGRAM} (${CCACHE_VERSION})")
    set(CMAKE_CXX_COMPILER_LAUNCHER "${CCACHE_PROGRAM}")
    set(CMAKE_C_COMPILER_LAUNCHER "${CCACHE_PROGRAM}")

    unset(CCACHE_VERSION)
endif()
