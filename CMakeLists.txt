cmake_minimum_required(VERSION 3.22)
project(ORC)
set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

option(ENABLE_DEBUG "Enable debug mode" OFF)

# enabling debug mode
if (ENABLE_DEBUG)
    add_definitions(-DDEBUG)
endif()

# import and set up libraries necessary for testing =============================
include(FetchContent)
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/refs/tags/release-1.11.0.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)
#================================================================================
# register all source files =====================================================
file(GLOB_RECURSE SOURCE
        src/*
        cem/*
)
# Exclude files in the 'cem/test' directory
file(GLOB_RECURSE CEM_TEST_FILES
        cem/test/*
)
list(REMOVE_ITEM SOURCE ${CEM_TEST_FILES})


file(GLOB_RECURSE TEST
        test/*
)
#================================================================================
# create ORC_run executable =====================================================
add_executable(${CMAKE_PROJECT_NAME}_run ${SOURCE})
# create "configurations" folder next to ORC_run binary file
add_custom_command(TARGET ${CMAKE_PROJECT_NAME}_run POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E make_directory $<TARGET_FILE_DIR:${CMAKE_PROJECT_NAME}_run>/configurations
)

# register header files lib, cem and src subdirectories
include_directories(lib
        lib/nlohmann
        cem
        cem/Enums
        cem/Enums/Communication
        cem/Messages
        cem/Messages/ToJRU
        cem/Exceptions
        cem/Exceptions/InternalStateMessageExceptions
        cem/Other
        cem/Other/CommunicationData
        cem/Other/CommunicationData/Dmi
        cem/Packets
        cem/Packets/Factory
        src
        src/Core
        src/ControllingOpenRails/
        src/Core/TopicWorker
        src/Services
        src/Services/Configuration
        src/Services/MessageHandlersService
        src/Services/MqttListener
        src/Services/MqttPublisher
        src/Services/JRULogger
        src/Services/Time
        src/MessageHandlers
        src/Enums
        src/Configurations
)

if (WIN32)
    # build on 3 cores (only works with cl compiler)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MP3")
    # set WINDOWS macro
    add_definitions(-DWINDOWS)
    set(ENV{MOSQUITTODIR} ${PROJECT_SOURCE_DIR}/noCompileLibs/Mosquitto)
    find_package(MOSQUITTO REQUIRED)
    include_directories(${PROJECT_SOURCE_DIR}/noCompileLibs/Mosquitto)
    target_link_libraries(${CMAKE_PROJECT_NAME}_run ${MOSQUITTO_LIBRARIES})
    # Find all DLLs in project
    file(GLOB_RECURSE DLLS "${PROJECT_SOURCE_DIR}/noCompileLibs/*.dll")
    # Copy DLLs next to the EXE file
    add_custom_command(TARGET ${CMAKE_PROJECT_NAME}_run POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            ${DLLS}
            $<TARGET_FILE_DIR:${CMAKE_PROJECT_NAME}_run>)
else ()
    find_package(PkgConfig REQUIRED)
    find_package(Threads REQUIRED)
    pkg_check_modules(Mosquitto REQUIRED IMPORTED_TARGET libmosquitto)
    target_link_libraries(${CMAKE_PROJECT_NAME}_run PRIVATE Threads::Threads)
    target_link_libraries(${CMAKE_PROJECT_NAME}_run PRIVATE PkgConfig::Mosquitto)
endif ()
#================================================================================
# testing part ==================================================================
enable_testing()
add_executable(
        ${CMAKE_PROJECT_NAME}_test
        ${TEST}
)
# create "configurations" folder next to ORC_test binary file
add_custom_command(TARGET ${CMAKE_PROJECT_NAME}_test POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E make_directory $<TARGET_FILE_DIR:${CMAKE_PROJECT_NAME}_test>/configurations
)
if (WIN32)
    target_link_libraries(
            ${CMAKE_PROJECT_NAME}_test
            gtest_main
            ${MOSQUITTO_LIBRARIES}
    )
else ()
    target_link_libraries(
            ${CMAKE_PROJECT_NAME}_test
            gtest_main
            Threads::Threads
            PkgConfig::Mosquitto
    )
endif ()

include(GoogleTest)
gtest_discover_tests(${CMAKE_PROJECT_NAME}_test)
