#ifndef CLA_CLALLOCATOR_H_
#define CLA_CLALLOCATOR_H_

// OpenCL C code
#ifdef __OPENCL_VERSION__
    typedef unsigned int cla_size_t;
    typedef __global uchar * cla_mem_t;

    typedef struct cla_t
    {
        cla_mem_t mem_start;
        cla_mem_t mem_end;
        __global cla_size_t * mem_offset;
    } cla_t;

     typedef struct cla_block_t
    {
        __global void * ptr;
        cla_size_t length;
    } cla_t;

    cla_t cla_create(cla_mem_t __start, cla_size_t __size)
    {
        cla_t __cla = {
            __start + sizeof(cla_size_t), // start
            __start + __size, // end
            (__global cla_size_t *)__start // offset
        };
        atomic_cmpxchg(__cla.mem_offset, 0, sizeof(cla_size_t));
        return __cla;
    }

    cla_block_t cla_malloc(cla_size_t __size, cl_size_t __alignment)
    {

    }

    void cla_destroy(cla_t * __cla)
    {
        *(__cla->mem_start) = 0;
        *(__cla->mem_end) = 0;
        *(__cla->mem_offset) = 0;
        return;
    }
#else
    typedef cl_uint cla_size_t;
#endif // __OPENCL_VERSION__

#endif // CLA_CLALLOCATOR_H_
