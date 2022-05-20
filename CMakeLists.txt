cmake_minimum_required(VERSION 3.10)

project(clever
	VERSION 1.0
	DESCRIPTION "Coincident Low-Energy Vertex Reconstruction"
    LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/609281088cfefc76f9d0ce82e1ff6c30cc3591e5.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

add_library(libgeometry 
	${CMAKE_SOURCE_DIR}/libclever/libhitselect.cpp
	${CMAKE_SOURCE_DIR}/libclever/libgeometry.cpp
	${CMAKE_SOURCE_DIR}/libclever/libconstants.hpp
	${CMAKE_SOURCE_DIR}/libclever/libgeometry.hpp
	${CMAKE_SOURCE_DIR}/libclever/libhitselect.hpp
)

target_include_directories(libgeometry PUBLIC libclever)

add_executable(clever
	   $<TARGET_OBJECTS:libgeometry>
	   clever/clever.cpp)

target_link_libraries(clever libgeometry)

# Add the unit tests #

add_executable(
    cleverTests
	$<TARGET_OBJECTS:libgeometry>
    libclever/libgeometry.test.cpp
	libclever/libhitselect.test.cpp
)

target_link_libraries(
    cleverTests
    gtest
	gtest_main
	libgeometry
)

include(GoogleTest)
gtest_discover_tests(cleverTests)

#########################################

if (CMAKE_SYSTEM_NAME STREQUAL "Windows")
    target_compile_options(clever PRIVATE /W4)
elseif (CMAKE_SYSTEM_NAME STREQUAL "Linux")
    target_compile_options(clever PRIVATE -Wall -Wextra -Wpedantic)
elseif (CMAKE_SYSTEM_NAME STREQUAL "Darwin")
    # other macOS-specific flags for Clang
endif()

########how do I add dependencies?
