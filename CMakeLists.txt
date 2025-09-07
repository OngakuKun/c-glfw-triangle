cmake_minimum_required(VERSION 3.16)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_C_STANDARD 99)

set(RELEASE_BUILD OFF CACHE BOOL "RELEASE BUILD" FORCE)

if(RELEASE_BUILD)
    #link time optimization
    set(CMAKE_INTERPROCEDURAL_OPTIMIZATION TRUE)
    set(CMAKE_BUILD_TYPE Release)
else()
    set(CMAKE_INTERPROCEDURAL_OPTIMIZATION FALSE)
    set(CMAKE_BUILD_TYPE Debug)
endif()

project("hello-world")

file(GLOB_RECURSE MY_SOURCE CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/src/*.c")

add_executable("${CMAKE_PROJECT_NAME}")

set_property(TARGET "${CMAKE_PROJECT_NAME}" PROPERTY C_STANDARD 99)

file(CREATE_LINK
    "${CMAKE_BINARY_DIR}/compile_commands.json"
    "${CMAKE_SOURCE_DIR}/compile_commands.json"
    SYMBOLIC
)

if(RELEASE_BUILD)
    # setup the ASSETS_PATH macro to be in the root folder of your exe
    target_compile_definitions("${CMAKE_PROJECT_NAME}" PUBLIC ASSETS_PATH="./res/")
    target_compile_definitions("${CMAKE_PROJECT_NAME}" PUBLIC RELEASE=1)
    target_compile_definitions("${CMAKE_PROJECT_NAME}" PUBLIC DEBUG=0)
else()
    # This is useful to get an ASSETS_PATH in your IDE during development
    target_compile_definitions("${CMAKE_PROJECT_NAME}" PUBLIC ASSETS_PATH="${CMAKE_CURRENT_SOURCE_DIR}/res/")
    target_compile_definitions("${CMAKE_PROJECT_NAME}" PUBLIC RELEASE=0)
    target_compile_definitions("${CMAKE_PROJECT_NAME}" PUBLIC DEBUG=1)
endif()

target_sources("${CMAKE_PROJECT_NAME}" PRIVATE ${MY_SOURCE})

if(MSVC) # If using the VS compiler...
    target_compile_definitions("${CMAKE_PROJECT_NAME}" PUBLIC _CRT_SECURE_NO_WARNINGS)
    target_compile_options(${CMAKE_PROJECT_NAME} PRIVATE /W4 /WX)

    set_target_properties("${CMAKE_PROJECT_NAME}" PROPERTIES LINK_FLAGS "/SUBSYSTEM:WINDOWS /ENTRY:mainCRTStartup")

    set_property(TARGET "${CMAKE_PROJECT_NAME}" PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreadedDebug<$<CONFIG:Debug>:Debug>")
    set_property(TARGET "${CMAKE_PROJECT_NAME}" PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Release>:Release>")

    # make sure SIMD optimizations take place
    add_compile_options(/arch:AVX2)
else()
    target_compile_options(${CMAKE_PROJECT_NAME} PRIVATE -Wall -Wextra -Wpedantic -Werror)
endif()

#force remove unicode
if (WIN32)
    target_compile_options("${CMAKE_PROJECT_NAME}" PRIVATE /UUNICODE /U_UNICODE)
endif()

target_include_directories("${CMAKE_PROJECT_NAME}" PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include/")

