if(NOT EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/extern/hunter/scripts")
    message(NOTICE "Hunter submodule sources not found")
    message(NOTICE "initializing/updating the extern/huner submodule...")
    execute_process(
        COMMAND git submodule update --init --recursive extern/hunter
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        COMMAND_ERROR_IS_FATAL ANY
    )
endif()


if(NOT EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/extern/etherium-cable/toolchains")
    message(NOTICE "Etherium:cable submodule sources not found")
    message(NOTICE "initializing/updating the extern/etherium-cable submodule...")
    execute_process(
        COMMAND git submodule update --init --recursive extern/etherium-cable
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        COMMAND_ERROR_IS_FATAL ANY
    )
endif()

if(NOT EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/extern/etherium-evmc/include")
    message(NOTICE "Etherium:evmc submodule sources not found")
    message(NOTICE "initializing/updating the extern/etherium-evmc submodule...")
    execute_process(
        COMMAND git submodule update --init --recursive extern/etherium-evmc
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        COMMAND_ERROR_IS_FATAL ANY
    )
endif()

if(NOT EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/extern/etherium-ethash/src")
    message(NOTICE "Etherium:ethash submodule sources not found")
    message(NOTICE "initializing/updating the extern/etherium-ethash submodule...")
    execute_process(
        COMMAND git submodule update --init --recursive extern/etherium-ethash
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        COMMAND_ERROR_IS_FATAL ANY
    )
endif()

if(NOT EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/extern/libscrypt/libscrypt.h")
    message(NOTICE "libscrypt submodule sources not found")
    message(NOTICE "initializing/updating the extern/libscrypt submodule...")
    execute_process(
        COMMAND git submodule update --init --recursive extern/libscrypt
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        COMMAND_ERROR_IS_FATAL ANY
    )
endif()
