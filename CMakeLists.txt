cmake_minimum_required(VERSION 3.20)

project(FluidWorld VERSION 0.1.0 LANGUAGES CXX)

option(FLUIDWORLD_BUILD_APPS "Build FluidWorld example applications" ON)
option(FLUIDWORLD_BUILD_TESTS "Build FluidWorld tests" ON)

add_library(fluidworld STATIC
    src/core/ParticleSet.cpp
    src/core/Simulation.cpp
    src/neighbor/UniformGrid.cpp
    src/boundary/AxisAlignedBox.cpp
    src/solver/WCSPHSolver.cpp
)

add_library(FluidWorld::fluidworld ALIAS fluidworld)

target_include_directories(fluidworld
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

target_compile_features(fluidworld PUBLIC cxx_std_17)

if(MSVC)
    target_compile_options(fluidworld PRIVATE /W4 /permissive-)
else()
    target_compile_options(fluidworld PRIVATE -Wall -Wextra -Wpedantic)
endif()

if(FLUIDWORLD_BUILD_APPS)
    add_executable(fluidworld_simple_sph apps/simple_sph/main.cpp)
    target_link_libraries(fluidworld_simple_sph PRIVATE FluidWorld::fluidworld)
endif()

if(FLUIDWORLD_BUILD_TESTS)
    include(CTest)
    add_executable(fluidworld_tests tests/sph_tests.cpp)
    target_link_libraries(fluidworld_tests PRIVATE FluidWorld::fluidworld)
    add_test(NAME fluidworld_tests COMMAND fluidworld_tests)
endif()
