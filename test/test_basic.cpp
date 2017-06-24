// MIT License

// Copyright (c) 2017 Jakub Szuppe <j.szuppe@gmail.com>

// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:

// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#define BOOST_TEST_MODULE TestBasicCLA
#include <boost/test/unit_test.hpp>

#include <boost/compute/core.hpp>
#include <boost/compute/system.hpp>
#include <boost/compute/command_queue.hpp>
#include <boost/compute/utility/source.hpp>
#include <boost/compute/container/vector.hpp>

#include "cla/clallocator.h"

struct OpenCLContext
{
    boost::compute::device        device;
    boost::compute::context       context;
    boost::compute::command_queue queue;

    OpenCLContext() :
        device (boost::compute::system::default_device()),
        context(boost::compute::system::default_context()),
        queue  (boost::compute::system::default_queue())
    {}
};

BOOST_FIXTURE_TEST_SUITE(basic_cla_test, OpenCLContext)

BOOST_AUTO_TEST_CASE(test_cla_create)
{
    namespace bc = boost::compute;
    const size_t cla_byte_size = 1024;
    boost::compute::buffer cla_buffer(context, cla_byte_size);

    // Init memory
    auto first = boost::compute::make_buffer_iterator<cla_size_t>(cla_buffer);
    first.write(cla_size_t(0), queue);
    queue.finish();

    std::string source = "#include \"cla/clallocator.h\"\n";
    source += BOOST_COMPUTE_STRINGIZE_SOURCE(
        __kernel void test_kernel(cla_mem_t cla_mem, const cla_size_t cla_byte_size)
        {
            cla_t cla = cla_create(cla_mem, cla_byte_size);
        }
    );

    std::string options = "-I" + std::string(CLA_INCLUDE_DIR);
    bc::program program = bc::program::build_with_source(
        source, context, options
    );
    bc::kernel k = program.create_kernel("test_kernel");

    k.set_args(
        cla_buffer,
        static_cast<cla_size_t>(cla_buffer.size())
    );

    queue
        .enqueue_1d_range_kernel(k, 0, 1024, 0)
        .wait();

    cla_size_t size = first.read(queue);
    BOOST_CHECK_EQUAL(size, 4);
}

BOOST_AUTO_TEST_SUITE_END()