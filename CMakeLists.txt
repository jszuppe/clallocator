# MIT License
#
# Copyright (c) 2017 OpenCL.org (https://github.com/OpenCL/CL-compute)
# Copyright (c) 2017 Jakub Szuppe <j.szuppe@gmail.com>
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in all
# copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.

cmake_minimum_required(VERSION 3.1)

# Change project name
project(clallocator)

# find OpenCL
find_package(OpenCL REQUIRED)
include_directories(SYSTEM ${OpenCL_INCLUDE_DIRS})

# find Boost
find_package(Boost 1.54 REQUIRED)
include_directories(SYSTEM ${Boost_INCLUDE_DIRS})

# find Boost.Compute
if(${Boost_VERSION} LESS 106100)
    find_path(BOOST_COMPUTE_INCLUDE boost/compute.hpp)
    include_directories(SYSTEM ${BOOST_COMPUTE_INCLUDE})
endif()

# If no build type specified, configure for Release
if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Choose the type of build" FORCE)
endif()

# C++11
set(CMAKE_CXX_STANDARD 11)

if(CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX)
    # Disable "no-unused-result" warning
    SET(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -Wno-unused-result")
endif()

# Tests
option(CLA_BUILD_TESTS "Build tests" ON)

# Configure Boost.Compute
include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/BoostComputeConfig.cmake)
# Enable automatic kernel compilation error messages for tests
add_definitions(-DBOOST_COMPUTE_DEBUG_KERNEL_COMPILATION)
# Use only OpenCL 1.2
add_definitions(-DBOOST_COMPUTE_MAX_CL_VERSION=102)

if(CLA_BUILD_TESTS)
  enable_testing()
  add_subdirectory(test)
endif()
