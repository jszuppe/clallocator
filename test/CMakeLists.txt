# MIT License
#
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

include_directories(../include)

if(${BUILD_OPENCL_TESTS})
  set(BOOST_COMPONENTS ${BOOST_COMPONENTS} unit_test_framework)
endif()
find_package(Boost 1.54 REQUIRED COMPONENTS ${BOOST_COMPONENTS})

# Add path to clalloc headers
add_definitions(-DCLA_INCLUDE_DIR="${CMAKE_CURRENT_SOURCE_DIR}/../include/")

function(add_opencl_test TEST_NAME TEST_SOURCE)
  get_filename_component(TEST_TARGET ${TEST_SOURCE} NAME_WE)
  add_executable(${TEST_TARGET} ${TEST_SOURCE})
  target_link_libraries(${TEST_TARGET}
    ${OpenCL_LIBRARIES}
    ${Boost_LIBRARIES}
  )
  add_test(${TEST_NAME} ${TEST_TARGET})
endfunction()

add_opencl_test("clalloc.basic" test_basic.cpp)