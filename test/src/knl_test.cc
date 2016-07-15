///
/// @copyright Copyright (c) 2013-2016, Univrsité Pierre et Marie Curie
/// All rights reserved.
///
/// <b>hiCL</b> is owned by Université Pierre et Marie Curie (UPMC),
/// funded by TOTAL, and written by Issam SAID <said.issam@gmail.com>.
///
/// Redistribution and use in source and binary forms, with or without
/// modification, are permetted provided that the following conditions
/// are met:
///
/// 1. Redistributions of source code must retain the above copyright
///    notice, this list of conditions and the following disclaimer.
/// 2. Redistributions in binary form must reproduce the above copyright
///    notice, this list of conditions and the following disclaimer in the
///    documentation and/or other materials provided with the distribution.
/// 3. Neither the name of the UPMC nor the names of its contributors
///    may be used to endorse or promote products derived from this software
///    without specific prior written permission.
///
/// THIS SOFTWARE IS PROVIDED "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
/// INCLUDING, BUT NOT LIMITED TO, WARRANTIES OF MERCHANTABILITY AND FITNESS
/// FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE UPMC OR
/// ITS CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
/// EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
/// PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
/// PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF 
/// LIABILITY, WETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
/// NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
/// SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
///
/// @file knl_test.cc
/// @author Issam SAID
/// @brief Unit testing file for the hiCL kernels manipulation routines.
///
#include <hiCL/knl.h>
#include <hiCL/dev.h>
#include <hiCL/mem.h>
#include <__api/config/mem.h>
#include <__api/util-inl.h>
#include <__api/mem-inl.h>
#include <__api/knl-inl.h>
#include <hiCL/timer.h>
#include <hiCL/base.h>
#include <gtest/gtest.h>
#include <math.h>

extern hienv_t hicl;

namespace {

inline void hicl_fknl_set_mem(hiknl_t k, int index, address_t h) {
    rbn_int_himem_t *n;
    himem_t m = find_rbn_address_t_himem_t(&hicl->mems, h)->value;
    if ((n = find_rbn_int_himem_t(&k->mems, index)) == k->mems.nil) {
        HICL_DEBUG("insert @ %p for kernel %s", m->id, __api_knl_name(k->id));
        insert_rbn_int_himem_t(&k->mems, index, m);
        insert_rbn_hiknl_t_int(&m->knls, k, index);
        __api_knl_set_arg_cl_mem(k->id, index, &m->id);
    } else {
        if (m != n->value) {
            HICL_DEBUG("modify @ %p for kernel %s", m->id, __api_knl_name(k->id));
            n->value = m; 
            __api_knl_set_arg_cl_mem(k->id, index, &m->id);
        }
    }
}

inline void hicl_fknl_set_args(hiknl_t k, ...) {
    va_list list;
    va_start(list, k);
    __api_knl_set_args_valist(k, list);
    va_end(list);
}

inline void hicl_fknl_set_wrk(hiknl_t k, cl_uint wrk, size_t *gws, size_t *lws) {
    cl_uint idx;
    k->wrk = wrk;
    for(idx=0; idx<3; ++idx) {
        if (idx < wrk) {
            k->gws[idx] = gws[idx];
            k->lws[idx] = lws[idx];
        } else {
            k->gws[idx] = 1;
            k->lws[idx] = 1;
        }
    }
    HICL_DEBUG("call hicl_fknl_set_wrk: global[%lux%lux%lu], local[%lux%lux%lu]",
         k->gws[0], k->gws[1], k->gws[2], k->lws[0], k->lws[1], k->lws[2]);
}

inline void hicl_fknl_set_ofs(hiknl_t k, size_t *ofs) {
    cl_uint idx;
    for(idx=0; idx<3; ++idx) k->ofs[idx] = ofs[idx];
    HICL_DEBUG("call hicl_fknl_set_ofs: ofs[%lux%lux%lu]", 
         k->ofs[0], k->ofs[1], k->ofs[2]);
}

inline void hicl_fknl_exec(hiknl_t k, hidev_t d) {
    HICL_DEBUG("run (async) kernel : %s", __api_knl_name(k->id));
    walk_value_rbt_int_himem_t(&k->mems, __api_mem_sync);
    __api_knl_async_run(k->id, d->queue, k->wrk, k->gws, k->lws, k->ofs);
    HICL_DEBUG("");
}

inline void hicl_fknl_sync_exec(hiknl_t k, hidev_t d) {
    HICL_DEBUG("run (sync) kernel  : %s", __api_knl_name(k->id));
    walk_value_rbt_int_himem_t(&k->mems, __api_mem_sync);
    __api_knl_sync_run(k->id, d->queue, k->wrk, k->gws, k->lws, k->ofs);
    HICL_DEBUG("");
}

inline double hicl_fknl_timed_exec(hiknl_t k, hidev_t d) {
    HICL_DEBUG("run (timed) kernel : %s", __api_knl_name(k->id));
    walk_value_rbt_int_himem_t(&k->mems, __api_mem_sync);
    hicl_timer_tick();
    __api_knl_sync_run(k->id, d->queue, k->wrk, k->gws, k->lws, k->ofs);
    HICL_DEBUG("");
    return hicl_timer_read();
}

inline void hicl_fknl_run(hiknl_t k, hidev_t d, ...) {
    HICL_DEBUG("call hicl_fknl_set_and_run");
    va_list list;
    va_start(list, d);
    __api_knl_set_args_valist(k, list);
    va_end(list);
    hicl_fknl_exec(k, d);
}

inline void hicl_fknl_sync_run(hiknl_t k, hidev_t d, ...) {
    HICL_DEBUG("call hicl_fknl_set_and_srun");
    va_list list;
    va_start(list, d);
    __api_knl_set_args_valist(k, list);
    va_end(list);
    hicl_fknl_sync_exec(k, d);
}

inline double hicl_fknl_timed_run(hiknl_t k, hidev_t d, ...) {
    HICL_DEBUG("call hicl_fknl_set_and_trun");
    va_list list;
    va_start(list, d);
    __api_knl_set_args_valist(k, list);
    va_end(list);
    return hicl_fknl_timed_exec(k, d);
}


    class KnlTest : public ::testing::Test {
    protected:
        hidev_t d;
        size_t N, g, l;
        const static unsigned int DIM = 256, ITER = 100;
        virtual void SetUp() {
            hicl_init(DEFAULT);
            hicl_timer_uset(MILLI_SECONDS);
            d = hicl_dev_find(DEFAULT);
            N = 128*1024*1024/4;
            g = N;
            l = 64;
        }
        virtual void TearDown() { hicl_release(); }
    };

    TEST_F(KnlTest, build) {
        hicl_load("data/foo.cl", "-DSTENCIL=9");
        hicl_load("data/bar.cl", "-DSTENCIL=19");
        hicl_knl_build("test_hicl_1", "-DSTENCIL=29");
        hicl_knl_build("test_hicl_2", "-DSTENCIL=29");
        hicl_knl_build("test_hicl_6", "-DSTENCIL=29");
        ASSERT_EQ(list_size_hiknl_t(&hicl->knls), static_cast<unsigned int>(6));
    }

    TEST_F(KnlTest, release) {
        hicl_load("data/foo.cl", "-DSTENCIL=9");
        hicl_knl_release("test_hicl_1");
    }

    void populate(float* buffer, size_t size) {
        unsigned int i;
        for (i = 0; i < size; ++i) buffer[i] = i;
    }

    TEST_F(KnlTest, shared_mem_objects) {
        unsigned int i;
        hiknl_t k1;
        hiknl_t k2;
        float *hsrc;
        posix_memalign((void**)(&hsrc), __API_MEM_ALIGN_SIZE, N*sizeof(float));
        float *hdst1;
        posix_memalign((void**)(&hdst1), __API_MEM_ALIGN_SIZE, N*sizeof(float));
        float *hdst2;
        posix_memalign((void**)(&hdst2), __API_MEM_ALIGN_SIZE, N*sizeof(float));
        float *ha;
        posix_memalign((void**)(&ha), __API_MEM_ALIGN_SIZE, 4*sizeof(float));
        float *hb;
        posix_memalign((void**)(&hb), __API_MEM_ALIGN_SIZE, 4*sizeof(float));
        float *hc;
        posix_memalign((void**)(&hc), __API_MEM_ALIGN_SIZE, 4*sizeof(float));
        himem_t src  = hicl_mem_wrap(d, hsrc,  N, HWA | READ_ONLY);
        himem_t dst1 = hicl_mem_wrap(d, hdst1, N, HWA | WRITE_ONLY);
        himem_t dst2 = hicl_mem_wrap(d, hdst2, N, HWA | WRITE_ONLY);
        himem_t a    = hicl_mem_wrap(d, ha,    4, HWA | WRITE_ONLY);
        himem_t b    = hicl_mem_wrap(d, hb,    4, HWA | WRITE_ONLY);
        himem_t c    = hicl_mem_wrap(d, hc,    4, HWA | WRITE_ONLY);
        hicl_mem_update(hsrc, WRITE_ONLY);
        populate(hsrc, N);
        hicl_load("data/foo.cl", "-DSTENCIL=9 -cl-kernel-arg-info");
        hicl_knl_set_wrk("test_hicl_1", 1, &g, &l);
        hicl_knl_set_wrk("test_hicl_2", 1, &g, &l);
        
        hicl_knl_set_mem("test_hicl_1", 0, hsrc);
        hicl_knl_set_mem("test_hicl_1", 1, hdst1);
        hicl_knl_set_int32("test_hicl_1", 2, N);

        hicl_knl_set_mem("test_hicl_2", 0, hsrc);
        hicl_knl_set_mem("test_hicl_2", 1, hdst2);
        hicl_knl_set_mem("test_hicl_2", 2, ha);
        hicl_knl_set_mem("test_hicl_2", 3, hb);
        hicl_knl_set_mem("test_hicl_2", 4, hc);
        hicl_knl_set_int32("test_hicl_2", 5, N);
        hicl_knl_set_int32("test_hicl_2", 6, N);
        hicl_knl_set_int32("test_hicl_2", 7, N);

        hicl_knl_sync_exec("test_hicl_1", d);
        hicl_knl_sync_exec("test_hicl_2", d);
        hicl_mem_update(hdst1, READ_ONLY);
        hicl_mem_update(hdst2, READ_ONLY);
        for (i = 0; i < dst1->size; ++i) ASSERT_FLOAT_EQ(hdst1[i], i);
        for (i = 0; i < dst2->size; ++i) ASSERT_FLOAT_EQ(hdst2[i], i);
        
        k1 = hicl_knl_find("test_hicl_1");
        k2 = hicl_knl_find("test_hicl_2");

        ASSERT_EQ(2, k1->mems.size);
        ASSERT_EQ(5, k2->mems.size);
        ASSERT_EQ(6, hicl->mems.size);

        hicl_mem_release(hsrc);
        hicl_mem_release(hdst2);
        hicl_mem_release(ha);
        hicl_mem_release(hb);
        hicl_mem_release(hc);

        ASSERT_EQ(1, k1->mems.size);
        ASSERT_EQ(0, k2->mems.size);
        ASSERT_EQ(1, hicl->mems.size);
        hicl_knl_release("test_hicl_2");
        ASSERT_EQ(1, hicl->mems.size);
        
        hicl_knl_release("test_hicl_1");
        ASSERT_EQ(0, hicl->mems.size);
        
        free(hsrc);
        free(hdst1);
        free(hdst2);
        free(ha);
        free(hb);
        free(hc);
    }

    TEST_F(KnlTest, swap_mem_objects) {
        unsigned int i;
        hiknl_t k1;
        float *hsrc;
        posix_memalign((void**)(&hsrc), __API_MEM_ALIGN_SIZE, N*sizeof(float));
        float *hdst1;
        posix_memalign((void**)(&hdst1), __API_MEM_ALIGN_SIZE, N*sizeof(float));
        float *hdst2;
        posix_memalign((void**)(&hdst2), __API_MEM_ALIGN_SIZE, N*sizeof(float));
        float *ha;
        posix_memalign((void**)(&ha), __API_MEM_ALIGN_SIZE, 4*sizeof(float));
        float *hb;
        posix_memalign((void**)(&hb), __API_MEM_ALIGN_SIZE, 4*sizeof(float));
        float *hc;
        posix_memalign((void**)(&hc), __API_MEM_ALIGN_SIZE, 4*sizeof(float));
        
        hicl_mem_wrap(d, hsrc,  N, HWA | READ_WRITE);
        hicl_mem_wrap(d, hdst1, N, HWA | READ_WRITE);
        hicl_mem_wrap(d, hdst2, N, HWA | READ_WRITE);
        hicl_mem_update(hsrc, WRITE_ONLY);
        
        populate(hsrc, N);
        
        hicl_load("data/foo.cl", "-DSTENCIL=9 -cl-kernel-arg-info");
        k1 = hicl_knl_find("test_hicl_1");
        hicl_knl_set_wrk("test_hicl_1", 1, &g, &l);
        hicl_knl_set_mem("test_hicl_1", 0, hsrc);
        hicl_knl_set_mem("test_hicl_1", 1, hdst1);
        hicl_knl_set_int32("test_hicl_1", 2, N);
        hicl_knl_sync_exec("test_hicl_1", d);
        hicl_mem_update(hdst1, READ_ONLY);
        for (i = 0; i < N; ++i) ASSERT_FLOAT_EQ(hdst1[i], i);
        ASSERT_EQ(k1->mems.size, 2);
        ASSERT_EQ(hicl->mems.size, 3);
        hicl_knl_set_mem("test_hicl_1", 0, hsrc);
        hicl_knl_set_mem("test_hicl_1", 1, hdst2);
        hicl_knl_set_int32("test_hicl_1", 2, N);
        hicl_knl_sync_exec("test_hicl_1", d);
        hicl_mem_update(hdst2, READ_ONLY);
        for (i = 0; i <N; ++i) ASSERT_FLOAT_EQ(hdst2[i], i);
        ASSERT_EQ(k1->mems.size, 2);
        ASSERT_EQ(hicl->mems.size, 3);
        free(hsrc);
        free(hdst1);
        free(hdst2);
        free(ha);
        free(hb);
        free(hc);
    }

    TEST_F(KnlTest, sync_run_by_name) {
        unsigned int i;
        float *hsrc;
        posix_memalign((void**)(&hsrc), __API_MEM_ALIGN_SIZE, N*sizeof(float));
        float *hdst;
        posix_memalign((void**)(&hdst), __API_MEM_ALIGN_SIZE, N*sizeof(float));
        hicl_mem_wrap(d, hsrc, N, HWA | READ_ONLY);
        hicl_mem_wrap(d, hdst, N, HWA | WRITE_ONLY);
        hicl_mem_update(hsrc, WRITE_ONLY);
        populate(hsrc, N);
        //
        // it look like that NVIDIA needs "-cl-kernel-arg-info"
        // during the build of the OpenCL kernel
        //
        hicl_load("data/foo.cl", "-DSTENCIL=9 -cl-kernel-arg-info");
        hicl_knl_set_wrk("test_hicl_1", 1, &g, &l);
        
        hicl_timer_tick();
        for (i=0; i<ITER; ++i)
            hicl_knl_sync_run("test_hicl_1", d, hsrc, hdst, N);
        HICL_PRINT("=> time: %f %s", hicl_timer_read()/ITER, hicl_timer_uget());
        hicl_mem_update(hdst, READ_ONLY);
        for (i = 0; i < N; ++i) ASSERT_FLOAT_EQ(hdst[i], i);
        free(hsrc);
        free(hdst);
    }

    TEST_F(KnlTest, run_by_name) {
        unsigned int i;
        float *hsrc;
        posix_memalign((void**)(&hsrc), __API_MEM_ALIGN_SIZE, N*sizeof(float));
        float *hdst;
        posix_memalign((void**)(&hdst), __API_MEM_ALIGN_SIZE, N*sizeof(float));
        hicl_mem_wrap(d, hsrc, N, HWA | READ_ONLY);
        hicl_mem_wrap(d, hdst, N, HWA | WRITE_ONLY);
        hicl_mem_update(hsrc, WRITE_ONLY);
        populate(hsrc, N);
        hicl_load("data/foo.cl", "-DSTENCIL=9 -cl-kernel-arg-info");
        hicl_knl_set_wrk("test_hicl_1", 1, &g, &l);
        hicl_knl_set_mem("test_hicl_1", 0, hsrc);
        hicl_knl_set_mem("test_hicl_1", 1, hdst);
        hicl_knl_set_int32("test_hicl_1", 2, N);
        hicl_timer_tick();
        for (i=0; i<ITER; ++i) hicl_knl_exec("test_hicl_1", d);
        hicl_dev_wait(d);
        HICL_PRINT("=> time: %f %s", hicl_timer_read()/ITER, hicl_timer_uget());
        hicl_mem_update(hdst, READ_ONLY);
        for (i = 0; i < N; ++i) ASSERT_FLOAT_EQ(hdst[i], i);
        free(hsrc);
        free(hdst);
    }

    //
    // Stencil 3D
    // 
    void randomize_buffer(float* buffer, size_t size) {
        unsigned int i;
        for (i=0; i<size; ++i) {
            buffer[i] = (float)(i % 100);
        }
    }

    void setup_stencil_coeffs(float *t) {
        t[0] = -2.6f;
        t[1] =  1.6f;
        t[2] = -0.2f;
        t[3] =  0.02f;
        t[4] = -0.001f;
    }

#define O(u,z,y,x) (u[(x+s[0]) + (2*s[0] + dim[0]) *            \
                      ((2*s[1] + dim[1]) * (z+s[2]) + (y+s[1]))])

    void check_stencil_3d(int *dim, int *s,
                          float *coefx, float *coefy, float *coefz,
                          float *i, float *res, float e) {
        int x, y, z;
        float laplacian, coef0;
        size_t size = (2*s[0] + dim[0])*(2*s[1] + 
                                dim[1])*(2*s[2] + dim[2])*sizeof(float);
        float *o = (float*)(malloc(size));
        coef0 = coefx[0] + coefy[0] + coefz[0];
        memset(o, 0, size);
        
#pragma omp parallel for private(laplacian, z, y, x)
        for(z = 0; z < dim[2] ; ++z) {
            for(y = 0; y < dim[1]; ++y) {
                for(x = 0; x < dim[0]; ++x) {
                    laplacian = coef0 * O(i, z, y, x)                
                        + coefx[1]*( O(i, z,   y,   x+1) + O(i, z,   y,   x-1))
                        + coefy[1]*( O(i, z,   y+1, x  ) + O(i, z,   y-1, x  ))
                        + coefz[1]*( O(i, z+1, y,   x  ) + O(i, z-1, y,   x  ))
                        
                        + coefx[2]*( O(i, z,   y,   x+2) + O(i, z,   y,   x-2))
                        + coefy[2]*( O(i, z,   y+2, x  ) + O(i, z,   y-2, x  ))
                        + coefz[2]*( O(i, z+2, y,   x  ) + O(i, z-2, y,   x  ))
                        
                        + coefx[3]*( O(i, z,   y,   x+3) + O(i, z,   y,   x-3))
                        + coefy[3]*( O(i, z,   y+3, x  ) + O(i, z,   y-3, x  ))
                        + coefz[3]*( O(i, z+3, y,   x  ) + O(i, z-3, y,   x  ))
                        
                        + coefx[4]*( O(i, z,   y,   x+4) + O(i, z,   y,   x-4))
                        + coefy[4]*( O(i, z,   y+4, x  ) + O(i, z,   y-4, x  ))
                        + coefz[4]*( O(i, z+4, y,   x  ) + O(i, z-4, y,   x  ));
                    O(o, z,y,x) = O(i, z,y,x) + laplacian;
                }
            }
        }
        
        for (z = 0; z < dim[2]; ++z)
            for (y = 0; y < dim[1]; ++y)
                for (x = 0; x < dim[0]; ++x)
                    ASSERT_NEAR(O(o, z,y,x), O(res, z,y,x), e);
                
        free(o);
    }
    
    TEST_F(KnlTest, stencil_3d_wrapped_duplicated) {
        double comm, time = 0., flops;
        unsigned int i, n=100;
        float epsilon = 1.e-1;
        size_t size;
        char options[128];
        int dim[3] = {DIM, DIM, DIM}, s[3] = {4, 4, 4};
        size_t g[3], l[3] = {16, 16, 1};

        g[0] = dim[0]/4;
        g[1] = dim[1];
        g[3] = 1;

        size   = (2*s[0] + dim[0])*(2*s[1] + dim[1])*(2*s[2] + dim[2]);
        flops  = 2+(3*s[0]+1)+(3*s[1]+1)+(3*s[2]+1);
        
        sprintf(options, 
                "-cl-kernel-arg-info -DSTENCIL=%d -DLX=%lu -DLY=%lu", 
                s[0], l[0], l[1]);
                        
        hicl_load("data/stencil_lv_3d.cl", options);
        
        float *ui;
        float *uo;
        float *coefx;
        float *coefy;
        float *coefz;

        posix_memalign((void**)(&ui),    
                        __API_MEM_ALIGN_SIZE,     size*sizeof(float));
        posix_memalign((void**)(&uo),    
                        __API_MEM_ALIGN_SIZE,     size*sizeof(float));
        posix_memalign((void**)(&coefx), 
                        __API_MEM_ALIGN_SIZE, (s[0]+1)*sizeof(float));
        posix_memalign((void**)(&coefy), 
                        __API_MEM_ALIGN_SIZE, (s[1]+1)*sizeof(float));
        posix_memalign((void**)(&coefz), 
                        __API_MEM_ALIGN_SIZE, (s[2]+1)*sizeof(float));

        memset(uo, 0, size*sizeof(float));
        randomize_buffer(ui, size);
        setup_stencil_coeffs(coefx);
        setup_stencil_coeffs(coefy);
        setup_stencil_coeffs(coefz);

        hicl_mem_wrap(d, ui, size,   READ_ONLY);
        hicl_mem_wrap(d, uo, size,   WRITE_ONLY);
        hicl_mem_wrap(d, coefx, s[0]+1, READ_ONLY);
        hicl_mem_wrap(d, coefy, s[1]+1, READ_ONLY);
        hicl_mem_wrap(d, coefz, s[2]+1, READ_ONLY);
                
        hicl_knl_set_wrk("stencil_lv_3d", 2, g, l);
        for (i = 0; i < 5; ++i)
            hicl_knl_sync_run("stencil_lv_3d", d, ui, uo, coefx, coefy, coefz,
                              dim[0], dim[1], dim[2], s[0], s[1], s[2]);
        hicl_timer_tick();
        for (i=0; i<n; ++i) hicl_knl_exec("stencil_lv_3d", d);
        hicl_dev_wait(d);
        time = hicl_timer_read();
        hicl_timer_tick();
        hicl_mem_update(uo, READ_ONLY);
        comm = hicl_timer_read();
        HICL_PRINT("avertage time: %8.2f (%8.2f) %s, %8.2f (%8.2f) Gflop/s",
                 time/n, comm + time/n, hicl_timer_uget(),
                 flops*1.e-9*n*dim[0]*dim[1]*dim[2]/(time*hicl_timer_coef()),
                 flops*1.e-9*dim[0]*dim[1]*dim[2]/
                 ((comm+(time/n))*hicl_timer_coef()));
        check_stencil_3d(dim, s, coefx, coefy, coefz, ui, uo, epsilon);
    }

    TEST_F(KnlTest, stencil_3d_wrapped_pinned) {
        double comm, time = 0., flops;
        unsigned int i, n=100;
        float epsilon = 1.e-1;
        size_t size;
        char options[128];
        int dim[3] = {DIM, DIM, DIM}, s[3] = {4, 4, 4};
        size_t g[3], l[3] = {16, 16, 1};

        g[0] = dim[0]/4;
        g[1] = dim[1];
        g[3] = 1;

        size   = (2*s[0] + dim[0])*(2*s[1] + dim[1])*(2*s[2] + dim[2]);
        flops  = 2+(3*s[0]+1)+(3*s[1]+1)+(3*s[2]+1);
        
        sprintf(options, 
                "-cl-kernel-arg-info -DSTENCIL=%d -DLX=%lu -DLY=%lu", 
                s[0], l[0], l[1]);
                        
        hicl_load("data/stencil_lv_3d.cl", options);
        
        hiknl_t k = hicl_knl_find("stencil_lv_3d");
        
        float *ui;
        float *uo;
        float *coefx;
        float *coefy;
        float *coefz;

        posix_memalign((void**)(&ui),    
                        __API_MEM_ALIGN_SIZE,     size*sizeof(float));
        posix_memalign((void**)(&uo),    
                        __API_MEM_ALIGN_SIZE,     size*sizeof(float));
        posix_memalign((void**)(&coefx), 
                        __API_MEM_ALIGN_SIZE, (s[0]+1)*sizeof(float));
        posix_memalign((void**)(&coefy), 
                        __API_MEM_ALIGN_SIZE, (s[1]+1)*sizeof(float));
        posix_memalign((void**)(&coefz), 
                        __API_MEM_ALIGN_SIZE, (s[2]+1)*sizeof(float));

        memset(uo, 0, size*sizeof(float));
        randomize_buffer(ui, size);
        setup_stencil_coeffs(coefx);
        setup_stencil_coeffs(coefy);
        setup_stencil_coeffs(coefz);

        hicl_mem_wrap(d, ui, size, HWA | PINNED | READ_ONLY);
        hicl_mem_wrap(d, uo, size, HWA | PINNED | WRITE_ONLY);
        hicl_mem_wrap(d, coefx, s[0]+1, READ_ONLY);
        hicl_mem_wrap(d, coefy, s[1]+1, READ_ONLY);
        hicl_mem_wrap(d, coefz, s[2]+1, READ_ONLY);        
        
        hicl_knl_set_wrk("stencil_lv_3d", 2, g, l);
        for (i = 0; i < 5; ++i)
            hicl_knl_sync_run("stencil_lv_3d", d, ui, uo, coefx, coefy, coefz,
                               dim[0], dim[1], dim[2], s[0], s[1], s[2]);
        hicl_timer_tick();
        for (i=0; i<n; ++i) hicl_knl_exec("stencil_lv_3d", d);
        hicl_dev_wait(d);
        time = hicl_timer_read();
        hicl_timer_tick();
        hicl_mem_update(uo, READ_ONLY);
        comm = hicl_timer_read();
        HICL_PRINT("avertage time: %8.2f (%8.2f) %s, %8.2f (%8.2f) Gflop/s",
                    time/n, comm + time/n, hicl_timer_uget(),
                    flops*1.e-9*n*dim[0]*dim[1]*dim[2]/(time*hicl_timer_coef()),
                    flops*1.e-9*dim[0]*dim[1]*dim[2]/
                    ((comm+(time/n))*hicl_timer_coef()));
        check_stencil_3d(dim, s, coefx, coefy, coefz, ui, uo, epsilon);
    }

    TEST_F(KnlTest, stencil_3d_wrapped_host_zero_copy) {
        double comm, time = 0., flops;
        unsigned int i, n=100;
        float epsilon = 1.e-1;
        size_t size;
        char options[128];
        int dim[3] = {DIM, DIM, DIM}, s[3] = {4, 4, 4};
        size_t g[3], l[3] = {16, 16, 1};

        g[0] = dim[0]/4;
        g[1] = dim[1];
        g[3] = 1;

        size   = (2*s[0] + dim[0])*(2*s[1] + dim[1])*(2*s[2] + dim[2]);
        flops  = 2+(3*s[0]+1)+(3*s[1]+1)+(3*s[2]+1);
        
        sprintf(options, 
                "-cl-kernel-arg-info -DSTENCIL=%d -DLX=%lu -DLY=%lu", 
                s[0], l[0], l[1]);
                        
        hicl_load("data/stencil_lv_3d.cl", options);
        
        hiknl_t k      = hicl_knl_find("stencil_lv_3d");
        
        float *ui;
        float *uo;
        float *coefx;
        float *coefy;
        float *coefz;

        posix_memalign((void**)(&ui),    
                        __API_MEM_ALIGN_SIZE,     size*sizeof(float));
        posix_memalign((void**)(&uo),    
                        __API_MEM_ALIGN_SIZE,     size*sizeof(float));
        posix_memalign((void**)(&coefx), 
                        __API_MEM_ALIGN_SIZE, (s[0]+1)*sizeof(float));
        posix_memalign((void**)(&coefy), 
                        __API_MEM_ALIGN_SIZE, (s[1]+1)*sizeof(float));
        posix_memalign((void**)(&coefz), 
                        __API_MEM_ALIGN_SIZE, (s[2]+1)*sizeof(float));

        memset(uo, 0, size*sizeof(float));
        randomize_buffer(ui, size);
        setup_stencil_coeffs(coefx);
        setup_stencil_coeffs(coefy);
        setup_stencil_coeffs(coefz);

        hicl_mem_wrap(d, ui, size,   CPU | ZERO_COPY | READ_ONLY);
        hicl_mem_wrap(d, uo, size,   CPU | ZERO_COPY | WRITE_ONLY);
        hicl_mem_wrap(d, coefx, s[0]+1, READ_ONLY);
        hicl_mem_wrap(d, coefy, s[1]+1, READ_ONLY);
        hicl_mem_wrap(d, coefz, s[2]+1, READ_ONLY);
                        
        hicl_knl_set_wrk("stencil_lv_3d", 2, g, l);
        for (i = 0; i < 5; ++i)
            hicl_knl_sync_run("stencil_lv_3d", d, ui, uo, coefx, coefy, coefz,
                              dim[0], dim[1], dim[2], s[0], s[1], s[2]);
        hicl_timer_tick();
        for (i=0; i<n; ++i) hicl_knl_exec("stencil_lv_3d", d);
        hicl_dev_wait(d);
        time = hicl_timer_read();
        hicl_timer_tick();
        hicl_mem_update(uo, READ_ONLY);
        comm = hicl_timer_read();
        HICL_PRINT("avertage time: %8.2f (%8.2f) %s, %8.2f (%8.2f) Gflop/s",
                 time/n, comm + (time/n), hicl_timer_uget(),
                 flops*1.e-9*n*dim[0]*dim[1]*dim[2]/(time*hicl_timer_coef()),
                 flops*1.e-9*dim[0]*dim[1]*dim[2]/
                 ((comm+(time/n))*hicl_timer_coef()));
        check_stencil_3d(dim, s, coefx, coefy, coefz, ui, uo, epsilon);
    }

    TEST_F(KnlTest, stencil_3d_rw_wrapped_duplicated) {
        double comm, time = 0., flops;
        unsigned int i, n=100;
        float epsilon = 1.e-1;
        size_t size;
        char options[128];
        int dim[3] = {DIM, DIM, DIM}, s[3] = {4, 4, 4};
        size_t g[3], l[3] = {16, 16, 1};

        g[0] = dim[0]/4;
        g[1] = dim[1];
        g[3] = 1;

        size   = (2*s[0] + dim[0])*(2*s[1] + dim[1])*(2*s[2] + dim[2]);
        flops  = 2+(3*s[0]+1)+(3*s[1]+1)+(3*s[2]+1);
        
        sprintf(options, 
                "-cl-kernel-arg-info -DSTENCIL=%d -DLX=%lu -DLY=%lu", 
                s[0], l[0], l[1]);
                        
        hicl_load("data/stencil_lv_3d.cl", options);
        
        hiknl_t k = hicl_knl_find("stencil_lv_3d");
        
        float *ui;
        float *uo;
        float *coefx;
        float *coefy;
        float *coefz;

        posix_memalign((void**)(&ui),    
                        __API_MEM_ALIGN_SIZE,     size*sizeof(float));
        posix_memalign((void**)(&uo),    
                        __API_MEM_ALIGN_SIZE,     size*sizeof(float));
        posix_memalign((void**)(&coefx), 
                        __API_MEM_ALIGN_SIZE, (s[0]+1)*sizeof(float));
        posix_memalign((void**)(&coefy), 
                        __API_MEM_ALIGN_SIZE, (s[1]+1)*sizeof(float));
        posix_memalign((void**)(&coefz), 
                        __API_MEM_ALIGN_SIZE, (s[2]+1)*sizeof(float));

        memset(uo, 0, size*sizeof(float));
        randomize_buffer(ui, size);
        setup_stencil_coeffs(coefx);
        setup_stencil_coeffs(coefy);
        setup_stencil_coeffs(coefz);

        hicl_mem_wrap(d, ui, size, HWA);
        hicl_mem_wrap(d, uo, size, HWA);
        hicl_mem_wrap(d, coefx, s[0]+1, READ_ONLY);
        hicl_mem_wrap(d, coefy, s[1]+1, READ_ONLY);
        hicl_mem_wrap(d, coefz, s[2]+1, READ_ONLY);
                
        hicl_knl_set_wrk("stencil_lv_3d", 2, g, l);
        for (i = 0; i < 5; ++i)
            hicl_knl_sync_run("stencil_lv_3d", d, ui, uo, coefx, coefy, coefz,
                              dim[0], dim[1], dim[2], s[0], s[1], s[2]);
        hicl_timer_tick();
        for (i=0; i<n; ++i) hicl_knl_exec("stencil_lv_3d", d);
        hicl_dev_wait(d);
        time = hicl_timer_read();
        hicl_timer_tick();
        hicl_mem_update(uo, READ_ONLY);
        comm = hicl_timer_read();
        HICL_PRINT("avertage time: %8.2f (%8.2f) %s, %8.2f (%8.2f) Gflop/s",
                 time/n, comm + time/n, hicl_timer_uget(),
                 flops*1.e-9*n*dim[0]*dim[1]*dim[2]/(time*hicl_timer_coef()),
                 flops*1.e-9*dim[0]*dim[1]*dim[2]/
                 ((comm+(time/n))*hicl_timer_coef()));
        check_stencil_3d(dim, s, coefx, coefy, coefz, ui, uo, epsilon);
    }

    TEST_F(KnlTest, stencil_3d_rw_wrapped_pinned) {
        double comm, time = 0., flops;
        unsigned int i, n=100;
        float epsilon = 1.e-1;
        size_t size;
        char options[128];
        int dim[3] = {DIM, DIM, DIM}, s[3] = {4, 4, 4};
        size_t g[3], l[3] = {16, 16, 1};

        g[0] = dim[0]/4;
        g[1] = dim[1];
        g[3] = 1;

        size   = (2*s[0] + dim[0])*(2*s[1] + dim[1])*(2*s[2] + dim[2]);
        flops  = 2+(3*s[0]+1)+(3*s[1]+1)+(3*s[2]+1);
        
        sprintf(options, 
                "-cl-kernel-arg-info -DSTENCIL=%d -DLX=%lu -DLY=%lu", 
                s[0], l[0], l[1]);
                        
        hicl_load("data/stencil_lv_3d.cl", options);
        
        hiknl_t k = hicl_knl_find("stencil_lv_3d");
        
        float *ui;
        float *uo;
        float *coefx;
        float *coefy;
        float *coefz;

        posix_memalign((void**)(&ui),    
                        __API_MEM_ALIGN_SIZE,     size*sizeof(float));
        posix_memalign((void**)(&uo),    
                        __API_MEM_ALIGN_SIZE,     size*sizeof(float));
        posix_memalign((void**)(&coefx), 
                        __API_MEM_ALIGN_SIZE, (s[0]+1)*sizeof(float));
        posix_memalign((void**)(&coefy), 
                        __API_MEM_ALIGN_SIZE, (s[1]+1)*sizeof(float));
        posix_memalign((void**)(&coefz), 
                        __API_MEM_ALIGN_SIZE, (s[2]+1)*sizeof(float));

        memset(uo, 0, size*sizeof(float));
        randomize_buffer(ui, size);
        setup_stencil_coeffs(coefx);
        setup_stencil_coeffs(coefy);
        setup_stencil_coeffs(coefz);

        hicl_mem_wrap(d, ui, size, HWA | PINNED);
        hicl_mem_wrap(d, uo, size, HWA | PINNED);
        hicl_mem_wrap(d, coefx, s[0]+1, READ_ONLY);
        hicl_mem_wrap(d, coefy, s[1]+1, READ_ONLY);
        hicl_mem_wrap(d, coefz, s[2]+1, READ_ONLY);        
        
        hicl_knl_set_wrk("stencil_lv_3d", 2, g, l);
        for (i = 0; i < 5; ++i)
            hicl_knl_sync_run("stencil_lv_3d", d, ui, uo, coefx, coefy, coefz,
                              dim[0], dim[1], dim[2], s[0], s[1], s[2]);
        hicl_timer_tick();
        for (i=0; i<n; ++i) hicl_knl_exec("stencil_lv_3d", d);
        hicl_dev_wait(d);
        time = hicl_timer_read();
        hicl_timer_tick();
        hicl_mem_update(uo, READ_ONLY);
        comm = hicl_timer_read();
        HICL_PRINT("avertage time: %8.2f (%8.2f) %s, %8.2f (%8.2f) Gflop/s",
                 time/n, comm + time/n, hicl_timer_uget(),
                 flops*1.e-9*n*dim[0]*dim[1]*dim[2]/(time*hicl_timer_coef()),
                 flops*1.e-9*dim[0]*dim[1]*dim[2]/
                 ((comm+(time/n))*hicl_timer_coef()));
        check_stencil_3d(dim, s, coefx, coefy, coefz, ui, uo, epsilon);
    }

    TEST_F(KnlTest, stencil_3d_rw_wrapped_host_zero_copy) {
        double comm, time = 0., flops;
        unsigned int i, n=100;
        float epsilon = 1.e-1;
        size_t size;
        char options[128];
        int dim[3] = {DIM, DIM, DIM}, s[3] = {4, 4, 4};
        size_t g[3], l[3] = {16, 16, 1};

        g[0] = dim[0]/4;
        g[1] = dim[1];
        g[3] = 1;

        size   = (2*s[0] + dim[0])*(2*s[1] + dim[1])*(2*s[2] + dim[2]);
        flops  = 2+(3*s[0]+1)+(3*s[1]+1)+(3*s[2]+1);
        
        sprintf(options, 
                "-cl-kernel-arg-info -DSTENCIL=%d -DLX=%lu -DLY=%lu", 
                s[0], l[0], l[1]);
                        
        hicl_load("data/stencil_lv_3d.cl", options);
        
        hiknl_t k = hicl_knl_find("stencil_lv_3d");
        
        float *ui;
        float *uo;
        float *coefx;
        float *coefy;
        float *coefz;

        posix_memalign((void**)(&ui),    
                        __API_MEM_ALIGN_SIZE,     size*sizeof(float));
        posix_memalign((void**)(&uo),    
                        __API_MEM_ALIGN_SIZE,     size*sizeof(float));
        posix_memalign((void**)(&coefx), 
                        __API_MEM_ALIGN_SIZE, (s[0]+1)*sizeof(float));
        posix_memalign((void**)(&coefy), 
                        __API_MEM_ALIGN_SIZE, (s[1]+1)*sizeof(float));
        posix_memalign((void**)(&coefz), 
                        __API_MEM_ALIGN_SIZE, (s[2]+1)*sizeof(float));

        memset(uo, 0, size*sizeof(float));
        randomize_buffer(ui, size);
        setup_stencil_coeffs(coefx);
        setup_stencil_coeffs(coefy);
        setup_stencil_coeffs(coefz);

        hicl_mem_wrap(d, ui, size,   CPU | ZERO_COPY);
        hicl_mem_wrap(d, uo, size,   CPU | ZERO_COPY);
        hicl_mem_wrap(d, coefx, s[0]+1, READ_ONLY);
        hicl_mem_wrap(d, coefy, s[1]+1, READ_ONLY);
        hicl_mem_wrap(d, coefz, s[2]+1, READ_ONLY);
                        
        hicl_knl_set_wrk("stencil_lv_3d", 2, g, l);
        for (i = 0; i < 5; ++i)
            hicl_knl_sync_run("stencil_lv_3d", d, ui, uo, coefx, coefy, coefz,
                              dim[0], dim[1], dim[2], s[0], s[1], s[2]);
        hicl_timer_tick();
        for (i=0; i<n; ++i) hicl_knl_exec("stencil_lv_3d", d);
        hicl_dev_wait(d);
        time = hicl_timer_read();
        hicl_timer_tick();
        hicl_mem_update(uo, READ_ONLY);
        comm = hicl_timer_read();
        HICL_PRINT("avertage time: %8.2f (%8.2f) %s, %8.2f (%8.2f) Gflop/s",
                   time/n, comm + (time/n), hicl_timer_uget(),
                   flops*1.e-9*n*dim[0]*dim[1]*dim[2]/(time*hicl_timer_coef()),
                   flops*1.e-9*dim[0]*dim[1]*dim[2]/
                   ((comm+(time/n))*hicl_timer_coef()));
        check_stencil_3d(dim, s, coefx, coefy, coefz, ui, uo, epsilon);
    }

    TEST_F(KnlTest, find_performance) {
        double comm, time = 0.;
        unsigned int i, n=1000;
        size_t size;
        char options[128];
        int dim[3] = {DIM, DIM, DIM}, s[3] = {4, 4, 4};
        size_t g[3], l[3] = {16, 16, 1};
        hiknl_t k;
        
        g[0]  = dim[0]/4;
        g[1]  = dim[1];
        g[3]  = 1;
        size  = (2*s[0] + dim[0])*(2*s[1] + dim[1])*(2*s[2] + dim[2]);
        
        sprintf(options, 
                "-cl-kernel-arg-info -DSTENCIL=%d -DLX=%lu -DLY=%lu", 
                s[0], l[0], l[1]);
              
        hicl_load("data/foo.cl", "-DSTENCIL=9");
        hicl_load("data/bar.cl", "-DSTENCIL=19");  
        hicl_load("data/stencil_lv_3d.cl", options);
        
        float *ui;
        float *uo;
        float *coefx;
        float *coefy;
        float *coefz;

        posix_memalign((void**)(&ui),    
                        __API_MEM_ALIGN_SIZE,     size*sizeof(float));
        posix_memalign((void**)(&uo),    
                        __API_MEM_ALIGN_SIZE,     size*sizeof(float));
        posix_memalign((void**)(&coefx), 
                        __API_MEM_ALIGN_SIZE, (s[0]+1)*sizeof(float));
        posix_memalign((void**)(&coefy), 
                        __API_MEM_ALIGN_SIZE, (s[1]+1)*sizeof(float));
        posix_memalign((void**)(&coefz), 
                        __API_MEM_ALIGN_SIZE, (s[2]+1)*sizeof(float));

        memset(uo, 0, size*sizeof(float));
        randomize_buffer(ui, size);
        setup_stencil_coeffs(coefx);
        setup_stencil_coeffs(coefy);
        setup_stencil_coeffs(coefz);

        hicl_mem_wrap(d, ui, size, HWA);
        hicl_mem_wrap(d, uo, size, HWA);
        hicl_mem_wrap(d, coefx, s[0]+1, READ_ONLY);
        hicl_mem_wrap(d, coefy, s[1]+1, READ_ONLY);
        hicl_mem_wrap(d, coefz, s[2]+1, READ_ONLY);
        
        hicl_timer_tick();
        k = hicl_knl_find("stencil_lv_3d");      
        hicl_fknl_set_wrk(k, 2, g, l);
        for (i = 0; i < 5; ++i)
            hicl_fknl_sync_run(k, d, ui, uo, coefx, coefy, coefz,
                               dim[0], dim[1], dim[2], s[0], s[1], s[2]);
        for (i=0; i<n; ++i) hicl_fknl_exec(k, d);
        hicl_dev_wait(d);
        time = hicl_timer_read();
        HICL_PRINT("direct time: %8.2f %s", time, hicl_timer_uget());

        hicl_timer_tick();     
        hicl_knl_set_wrk("stencil_lv_3d", 2, g, l);
        for (i = 0; i < 5; ++i)
            hicl_knl_sync_run("stencil_lv_3d", d, ui, uo, coefx, coefy, coefz,
                              dim[0], dim[1], dim[2], s[0], s[1], s[2]);
        for (i=0; i<n; ++i) hicl_knl_exec("stencil_lv_3d", d);
        hicl_dev_wait(d);
        time = hicl_timer_read();
        HICL_PRINT("loockup time: %8.2f %s", time, hicl_timer_uget());
    }    

}  // namespace