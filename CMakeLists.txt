cmake_minimum_required(VERSION 3.10)

project(clever
	VERSION 1.0
	DESCRIPTION "Coincident Low-Energy Vertex Reconstruction"
    LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
list(APPEND CMAKE_PREFIX_PATH ${ROOTSYS})

find_package (Eigen3 3.3 REQUIRED NO_MODULE)
find_package(ROOT CONFIG REQUIRED)
include(${ROOT_USE_FILE})

include_directories(${CMAKE_SOURCE_DIR}/libclever ${ROOT_INCLUDE_DIRS})

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/609281088cfefc76f9d0ce82e1ff6c30cc3591e5.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
#set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

add_library(libclever
	${CMAKE_SOURCE_DIR}/libclever/libconstants.hpp
	${CMAKE_SOURCE_DIR}/libclever/libgeometry.hpp
	${CMAKE_SOURCE_DIR}/libclever/libhitselect.hpp
	${CMAKE_SOURCE_DIR}/libclever/libtestpointcalc.hpp
	${CMAKE_SOURCE_DIR}/libclever/libfourhitcombos.hpp
	${CMAKE_SOURCE_DIR}/libclever/libhitinfo.hpp
	${CMAKE_SOURCE_DIR}/libclever/libtestpointcalc.cpp
	${CMAKE_SOURCE_DIR}/libclever/libfourhitcombos.cpp
	${CMAKE_SOURCE_DIR}/libclever/libhitselect.cpp
	${CMAKE_SOURCE_DIR}/libclever/libgeometry.cpp
)


target_include_directories(libclever PUBLIC libclever)

# Add the clever executable
add_executable(
	clever
	$<TARGET_OBJECTS:libclever>
	clever/clever.cpp
	)

target_link_libraries(
	clever libclever Eigen3::Eigen
	)


#Add the clever executable for rat-paci
add_executable(
		clever_rat
	$<TARGET_OBJECTS:libclever>
	clever/clever_rat.cpp
		)

target_link_libraries(
		clever_rat Eigen3::Eigen ${ROOT_LIBRARIES}
					)


# Add the unit tests #

add_executable(
    cleverTests
	$<TARGET_OBJECTS:libclever>
    libclever/libgeometry.test.cpp
	libclever/libhitselect.test.cpp
	#libclever/libtestpointcalc.test.cpp
	)

target_link_libraries(
    cleverTests
    gtest
	gtest_main
	libclever
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
