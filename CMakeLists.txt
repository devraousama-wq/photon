cmake_minimum_required(VERSION 3.25)

project(photon VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

option(PHOTON_BUILD_TESTS "Build unit tests" ON)

add_library(photon_core
    src/config.cpp
    src/accel/bvh.cpp
    src/scene/json_loader.cpp
)
target_include_directories(photon_core PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

add_executable(photon apps/photon_cli/main.cpp)
target_link_libraries(photon PRIVATE photon_core)

if(PHOTON_BUILD_TESTS)
    include(FetchContent)
    FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
    )
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
    FetchContent_MakeAvailable(googletest)
    enable_testing()
    add_subdirectory(tests)
endif()

install(TARGETS photon RUNTIME DESTINATION bin)
