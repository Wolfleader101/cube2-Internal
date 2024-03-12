cmake_minimum_required (VERSION 3.20)

SET(PROJECT_NAME CubeInteralTrainer)

project(${PROJECT_NAME})

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Compiler-specific flags
if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang" OR
    "${CMAKE_CXX_COMPILER_ID}" STREQUAL "AppleClang")
    add_compile_options(
        -Weverything -fcolor-diagnostics
        -Wno-c++98-compat -Wno-c++98-compat-pedantic -Wno-padded
        -Wno-deprecated-declarations -Wno-exit-time-destructors
        -Wno-switch-enum -Wno-weak-vtables -Wno-global-constructors
        -Wno-newline-eof
    )
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    add_compile_options(-Wall -Wextra -Wpedantic -fdiagnostics-color=always)
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
    add_compile_options(/W4)
endif()

add_definitions(-D_UNICODE -DUNICODE) # Use Unicode


add_subdirectory(src)

add_library(${PROJECT_NAME} SHARED ${SOURCES})

target_include_directories(${PROJECT_NAME} PRIVATE ${GLOBAL_INCLUDE_DIRS})

add_subdirectory(libs)

target_link_libraries(${PROJECT_NAME} PUBLIC
    imgui
    # spdlog
    glm
)

# add_library(${PROJECT_NAME}SHARED SHARED src/dllmain.cpp)
# target_include_directories(${PROJECT_NAME}SHARED PRIVATE ${GLOBAL_INCLUDE_DIRS})



add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
    "$<TARGET_FILE:${PROJECT_NAME}>" "E:/hacks/sauerbraten/hack.dll"
    COMMENT "Copying and renaming the DLL to hack.dll"
)