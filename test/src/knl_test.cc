///
/// \copyright Copyright 2012-2013 TOTAL S.A. All rights reserved.
/// This file is part of \b hicl.
///
/// \b hicl is free software: you can redistribute it and/or modify
/// it under the terms of the GNU General Public License as published by
/// the Free Software Foundation, either version 3 of the License, or
/// (at your option) any later version.
///
/// \b hicl is distributed in the hope that it will be useful,
/// but WITHOUT ANY WARRANTY; without even the implied warranty of
/// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
/// GNU General Public License for more details.
///
/// You should have received a copy of the GNU General Public License
/// along with \b hicl.  If not, see <http://www.gnu.org/licenses/>.
///
/// \author Issam Said
/// \file kernel_test.cc
/// \version $Id$
/// \brief Defines a unit test for the hicl_Kernel.
///
#include <hiCL/knl.h>
#include <hiCL/dev.h>
#include <hiCL/mem.h>
#include <__api/config/mem.h>
#include <__api/util-inl.h>
#include <__api/mem-inl.h>
#include <hiCL/timer.h>
#include <hiCL/base.h>
#include <gtest/gtest.h>
#include <math.h>

namespace {

    class KnlTest : public ::testing::Test {
    protected:
        dev d;
        size_t N, g, l;
        unsigned int DIM = 256, ITER = 100;
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

    TEST_F(KnlTest, build_by_name) {
        hicl_load("data/foo.cl", "-DSTENCIL=9");
        hicl_load("data/bar.cl", "-DSTENCIL=19");
        knl k = hicl_knl_find("test_hicl_1");
        hicl_knl_build(k, "-DSTENCIL=29");
        ASSERT_EQ(hicl_count(KERNEL), static_cast<unsigned int>(6));
    }

    TEST_F(KnlTest, build) {
        hicl_load("data/foo.cl", "-DSTENCIL=9");
        hicl_load("data/bar.cl", "-DSTENCIL=19");
        hicl_build("test_hicl_1", "-DSTENCIL=29");
        hicl_build("test_hicl_2", "-DSTENCIL=29");
        hicl_build("test_hicl_6", "-DSTENCIL=29");
        ASSERT_EQ(hicl_count(KERNEL), static_cast<unsigned int>(6));
    }

    void populate(float* buffer, size_t size) {
        unsigned int i;
        for (i = 0; i < size; ++i) buffer[i] = i;
    }

    TEST_F(KnlTest, shared_mem_objects) {
        unsigned int i;
        knl k1;
        knl k2;
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
        mem src  = hicl_mem_wrap(d, hsrc,  N, HWA | READ_ONLY);
        mem dst1 = hicl_mem_wrap(d, hdst1, N, HWA | WRITE_ONLY);
        mem dst2 = hicl_mem_wrap(d, hdst2, N, HWA | WRITE_ONLY);
        mem a    = hicl_mem_wrap(d, ha,    4, HWA | WRITE_ONLY);
        mem b    = hicl_mem_wrap(d, hb,    4, HWA | WRITE_ONLY);
        mem c    = hicl_mem_wrap(d, hc,    4, HWA | WRITE_ONLY);
        hicl_mem_update(hsrc, WRITE_ONLY);
        populate(hsrc, N);
        hicl_load("data/foo.cl", "-DSTENCIL=9");
        k1 = hicl_knl_find("test_hicl_1");
        k2 = hicl_knl_find("test_hicl_2");

        // ASSERT_EQ(0, src->ref);
        // ASSERT_EQ(0, dst1->ref);
        // ASSERT_EQ(0, dst2->ref);
        // ASSERT_EQ(0, a->ref);
        // ASSERT_EQ(0, b->ref);
        // ASSERT_EQ(0, c->ref);

        hicl_knl_set_wrk(k1, 1, &g, &l);
        hicl_knl_set_wrk(k2, 1, &g, &l);
        
        hicl_knl_set_mem(k1, 0, hsrc);
        hicl_knl_set_mem(k1, 1, hdst1);
        hicl_knl_set_int32(k1, 2, N);

        hicl_knl_set_mem(k2, 0, hsrc);
        hicl_knl_set_mem(k2, 1, hdst2);
        hicl_knl_set_mem(k2, 2, ha);
        hicl_knl_set_mem(k2, 3, hb);
        hicl_knl_set_mem(k2, 4, hc);
        hicl_knl_set_int32(k2, 5, N);
        hicl_knl_set_int32(k2, 6, N);
        hicl_knl_set_int32(k2, 7, N);

        // ASSERT_EQ(2, src->ref);
        // ASSERT_EQ(1, dst1->ref);
        // ASSERT_EQ(1, dst2->ref);
        // ASSERT_EQ(1, a->ref);
        // ASSERT_EQ(1, b->ref);
        // ASSERT_EQ(1, c->ref);

        hicl_knl_srun(k1, d);
        hicl_knl_srun(k2, d);
        hicl_mem_update(hdst1, READ_ONLY);
        hicl_mem_update(hdst2, READ_ONLY);
        for (i = 0; i < dst1->size; ++i) ASSERT_FLOAT_EQ(hdst1[i], i);
        for (i = 0; i < dst2->size; ++i) ASSERT_FLOAT_EQ(hdst2[i], i);
        
        ASSERT_EQ(2, k1->mems.size);
        ASSERT_EQ(5, k2->mems.size);
        ASSERT_EQ(6, hicl_count(MEMORY));

        hicl_mem_release(hsrc);
        hicl_mem_release(hdst2);
        hicl_mem_release(ha);
        hicl_mem_release(hb);
        hicl_mem_release(hc);

        ASSERT_EQ(1, k1->mems.size);
        ASSERT_EQ(0, k2->mems.size);
        ASSERT_EQ(1, hicl_count(MEMORY));
        hicl_knl_release(k2);
        ASSERT_EQ(1, hicl_count(MEMORY));
        
        hicl_knl_release(k1);
        ASSERT_EQ(0, hicl_count(MEMORY));
        
        free(hsrc);
        free(hdst1);
        free(hdst2);
        free(ha);
        free(hb);
        free(hc);
    }

    TEST_F(KnlTest, swap_mem_objects) {
        unsigned int i;
        knl k1;
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
        
        hicl_load("data/foo.cl", "-DSTENCIL=9");
        k1 = hicl_knl_find("test_hicl_1");
        hicl_knl_set_wrk(k1, 1, &g, &l);
        hicl_knl_set_mem(k1, 0, hsrc);
        hicl_knl_set_mem(k1, 1, hdst1);
        hicl_knl_set_int32(k1, 2, N);
        hicl_knl_srun(k1, d);
        hicl_mem_update(hdst1, READ_ONLY);
        for (i = 0; i < N; ++i) ASSERT_FLOAT_EQ(hdst1[i], i);
        ASSERT_EQ(k1->mems.size, 2);
        ASSERT_EQ(hicl_count(MEMORY), 3);
        hicl_knl_set_mem(k1, 0, hsrc);
        hicl_knl_set_mem(k1, 1, hdst2);
        hicl_knl_set_int32(k1, 2, N);
        hicl_knl_srun(k1, d);
        hicl_mem_update(hdst2, READ_ONLY);
        for (i = 0; i <N; ++i) ASSERT_FLOAT_EQ(hdst2[i], i);
        ASSERT_EQ(k1->mems.size, 2);
        ASSERT_EQ(hicl_count(MEMORY), 3);
        free(hsrc);
        free(hdst1);
        free(hdst2);
        free(ha);
        free(hb);
        free(hc);
    }

    TEST_F(KnlTest, srun_by_name) {
        unsigned int i;
        float *hsrc;
        posix_memalign((void**)(&hsrc), __API_MEM_ALIGN_SIZE, N*sizeof(float));
        float *hdst;
        posix_memalign((void**)(&hdst), __API_MEM_ALIGN_SIZE, N*sizeof(float));
        hicl_mem_wrap(d, hsrc, N, HWA | READ_ONLY);
        hicl_mem_wrap(d, hdst, N, HWA | WRITE_ONLY);
        hicl_mem_update(hsrc, WRITE_ONLY);
        populate(hsrc, N);
        hicl_load("data/foo.cl", "-DSTENCIL=9");
        hicl_set_wrk("test_hicl_1", 1, &g, &l);
        hicl_timer_tick();
        for (i=0; i<ITER; ++i)
            hicl_set_and_srun("test_hicl_1", d, hsrc, hdst, N);
        CL_PRINT("===> time: %f %s", hicl_timer_read()/ITER, hicl_timer_uget());
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
        hicl_load("data/foo.cl", "-DSTENCIL=9");
        hicl_set_wrk("test_hicl_1", 1, &g, &l);
        hicl_set_mem("test_hicl_1", 0, hsrc);
        hicl_set_mem("test_hicl_1", 1, hdst);
        hicl_set_int32("test_hicl_1", 2, N);
        hicl_timer_tick();
        for (i=0; i<ITER; ++i)
            hicl_run("test_hicl_1", d);
        hicl_dev_wait(d);
        CL_PRINT("===> time: %f %s", hicl_timer_read()/ITER, hicl_timer_uget());
        hicl_mem_update(hdst, READ_ONLY);
        for (i = 0; i < N; ++i) ASSERT_FLOAT_EQ(hdst[i], i);
        free(hsrc);
        free(hdst);
    }

    TEST_F(KnlTest, run) {
        unsigned int i;
        knl k;
        float *hsrc;
        posix_memalign((void**)(&hsrc), __API_MEM_ALIGN_SIZE, N*sizeof(float));
        float *hdst;
        posix_memalign((void**)(&hdst), __API_MEM_ALIGN_SIZE, N*sizeof(float));
        hicl_mem_wrap(d, hsrc, N, HWA | READ_ONLY);
        hicl_mem_wrap(d, hdst, N, HWA | WRITE_ONLY);
        hicl_mem_update(hsrc, WRITE_ONLY);
        populate(hsrc, N);
        hicl_load("data/foo.cl", "-DSTENCIL=9");
        k = hicl_knl_find("test_hicl_1");
        hicl_knl_set_wrk(k, 1, &g, &l);
        hicl_knl_set_mem(k, 0, hsrc);
        hicl_knl_set_mem(k, 1, hdst);
        hicl_knl_set_int32(k, 2, N);
        hicl_timer_tick();
        for (i=0; i<ITER; ++i)
            hicl_knl_run(k, d);
        hicl_dev_wait(d);
        CL_PRINT("===> time: %f %s", hicl_timer_read()/ITER, hicl_timer_uget());
        hicl_mem_update(hdst, READ_ONLY);
        for (i = 0; i < N; ++i) ASSERT_FLOAT_EQ(hdst[i], i);
        free(hsrc);
        free(hdst);
    }
    
    TEST_F(KnlTest, dry_srun) {
        unsigned int i;
        float *hsrc;
        float *hdst;

        hsrc = (float*)hicl_mem_allocate(d, N, HWA | READ_ONLY);
        hdst = (float*)hicl_mem_allocate(d, N, HWA | WRITE_ONLY);
        hicl_mem_update(hsrc, WRITE_ONLY);
        populate(hsrc, N);
        hicl_load("data/foo.cl", "-DSTENCIL=9");
        hicl_set_wrk("test_hicl_1", 1, &g, &l);
        hicl_timer_tick();
        for (i=0; i<ITER; ++i)
            hicl_set_and_srun("test_hicl_1", d, hsrc, hdst, N);
        CL_PRINT("===> time: %f %s", hicl_timer_read()/ITER, hicl_timer_uget());
        hicl_mem_update(hdst, READ_ONLY);
    }

    ///
    /// Copy 1D
    ///

    /// stdalone

    TEST_F(KnlTest, copy_1d_stdalone_duplicated) {
        unsigned int i;
        knl k;
        float *hsrc;
        float *hdst;

        hsrc = (float*)hicl_mem_allocate(d, N, HWA | READ_ONLY);
        hdst = (float*)hicl_mem_allocate(d, N, HWA | WRITE_ONLY);
        hicl_mem_update(hsrc, WRITE_ONLY);
        populate(hsrc, N);
        hicl_load("data/foo.cl", "-DSTENCIL=9");
        k = hicl_knl_find("test_hicl_1");
        hicl_knl_set_wrk(k, 1, &g, &l);
        for (i=0; i<5; ++i)  
            hicl_knl_set_and_srun(k, d, hsrc, hdst, N);
        hicl_timer_tick();
        for (i=0; i<ITER; ++i)  
            hicl_knl_run(k, d);
        hicl_dev_wait(d);
        CL_PRINT("===> time: %f %s", hicl_timer_read()/ITER, hicl_timer_uget());
        hicl_mem_update(hdst, READ_ONLY);
        for (i = 0; i < N; ++i) ASSERT_FLOAT_EQ((hdst)[i], i);
    }
    
    TEST_F(KnlTest, copy_1d_stdalone_pinned) {
        unsigned int i;
        knl k;
        float *hsrc;
        float *hdst;

        hsrc = (float*)hicl_mem_allocate(d, N, HWA | PINNED | READ_ONLY);
        hdst = (float*)hicl_mem_allocate(d, N, HWA | PINNED | WRITE_ONLY);
        hicl_mem_update(hsrc, WRITE_ONLY);
        populate(hsrc, N);
        hicl_load("data/foo.cl", "-DSTENCIL=9");
        k = hicl_knl_find("test_hicl_1");
        hicl_knl_set_wrk(k, 1, &g, &l);
        hicl_timer_tick();
        for (i=0; i<5; ++i)  
            hicl_knl_set_and_srun(k, d, hsrc, hdst, N);
        hicl_timer_tick();
        for (i=0; i<ITER; ++i)  
            hicl_knl_run(k, d);
        hicl_dev_wait(d);
        CL_PRINT("===> time: %f %s", hicl_timer_read()/ITER, hicl_timer_uget());
        hicl_mem_update(hdst, READ_ONLY);
        for (i = 0; i < N; ++i) ASSERT_FLOAT_EQ((hdst)[i], i);
    }

    TEST_F(KnlTest, copy_1d_stdalone_host_zero_copy) {
        unsigned int i;
        knl k;
        float *hsrc;
        float *hdst;

        hsrc = (float*)hicl_mem_allocate(d, N, CPU | ZERO_COPY | READ_ONLY);
        hdst = (float*)hicl_mem_allocate(d, N, CPU | ZERO_COPY | WRITE_ONLY);
        hicl_mem_update(hsrc, WRITE_ONLY);
        populate(hsrc, N);
        hicl_load("data/foo.cl", "-DSTENCIL=9");
        k = hicl_knl_find("test_hicl_1");
        hicl_knl_set_wrk(k, 1, &g, &l);
        hicl_timer_tick();
        for (i=0; i<5; ++i)  
            hicl_knl_set_and_srun(k, d, hsrc, hdst, N);
        hicl_timer_tick();
        for (i=0; i<ITER; ++i)  
            hicl_knl_run(k, d);
        hicl_dev_wait(d);
        CL_PRINT("===> time: %f %s", hicl_timer_read()/ITER, hicl_timer_uget());
        hicl_mem_update(hdst, READ_ONLY);
        for (i = 0; i < N; ++i) ASSERT_FLOAT_EQ((hdst)[i], i);
    }

    TEST_F(KnlTest, copy_1d_stdalone_device_zero_copy) {
        unsigned int i;
        knl k;
        float *hsrc;
        float *hdst;

        hsrc = (float*)hicl_mem_allocate(d, N, HWA | ZERO_COPY | READ_ONLY);
        hdst = (float*)hicl_mem_allocate(d, N, HWA | ZERO_COPY | WRITE_ONLY);
        hicl_mem_update(hsrc, WRITE_ONLY);
        populate(hsrc, N);
        hicl_load("data/foo.cl", "-DSTENCIL=9");
        k = hicl_knl_find("test_hicl_1");
        hicl_knl_set_wrk(k, 1, &g, &l);
        for (i=0; i<5; ++i)  
            hicl_knl_set_and_srun(k, d, hsrc, hdst, N);
        hicl_timer_tick();
        for (i=0; i<ITER; ++i)  
            hicl_knl_run(k, d);
        hicl_dev_wait(d);
        CL_PRINT("===> time: %f %s", hicl_timer_read()/ITER, hicl_timer_uget());
        hicl_mem_update(hdst, READ_ONLY);
        for (i = 0; i < N; ++i) ASSERT_FLOAT_EQ((hdst)[i], i);
    }

    TEST_F(KnlTest, copy_1d_rw_stdalone_duplicated) {
        unsigned int i;
        knl k;
        float *hsrc;
        float *hdst;
        hsrc = (float*)hicl_mem_allocate(d, N, HWA);
        hdst = (float*)hicl_mem_allocate(d, N, HWA);
        hicl_mem_update(hsrc, WRITE_ONLY);
        populate(hsrc, N);
        hicl_load("data/foo.cl", "-DSTENCIL=9");
        k = hicl_knl_find("test_hicl_1");
        hicl_knl_set_wrk(k, 1, &g, &l);
        for (i=0; i<5; ++i)  
            hicl_knl_set_and_srun(k, d, hsrc, hdst, N);
        hicl_timer_tick();
        for (i=0; i<ITER; ++i)  
            hicl_knl_run(k, d);
        hicl_dev_wait(d);
        CL_PRINT("===> time: %f %s", hicl_timer_read()/ITER, hicl_timer_uget());
        hicl_mem_update(hdst, READ_ONLY);
        for (i = 0; i < N; ++i) ASSERT_FLOAT_EQ((hdst)[i], i);
    }

    TEST_F(KnlTest, copy_1d_rw_stdalone_pinned) {
        unsigned int i;
        knl k;
        float *hsrc;
        float *hdst;

        hsrc = (float*)hicl_mem_allocate(d, N, HWA | PINNED);
        hdst = (float*)hicl_mem_allocate(d, N, HWA | PINNED);
        hicl_mem_update(hsrc, WRITE_ONLY);
        populate(hsrc, N);
        hicl_load("data/foo.cl", "-DSTENCIL=9");
        k = hicl_knl_find("test_hicl_1");
        hicl_knl_set_wrk(k, 1, &g, &l);
        for (i=0; i<5; ++i)  
            hicl_knl_set_and_srun(k, d, hsrc, hdst, N);
        hicl_timer_tick();
        for (i=0; i<ITER; ++i)  
            hicl_knl_run(k, d);
        hicl_dev_wait(d);
        CL_PRINT("===> time: %f %s", hicl_timer_read()/ITER, hicl_timer_uget());
        hicl_mem_update(hdst, READ_ONLY);
        for (i = 0; i < N; ++i) ASSERT_FLOAT_EQ((hdst)[i], i);
    }

    TEST_F(KnlTest, copy_1d_stdalone_rw_host_zero_copy) {
        unsigned int i;
        knl k;
        float *hsrc;
        float *hdst;
        hsrc = (float*)hicl_mem_allocate(d, N, CPU | ZERO_COPY);
        hdst = (float*)hicl_mem_allocate(d, N, CPU | ZERO_COPY);
        hicl_mem_update(hsrc, WRITE_ONLY);
        populate(hsrc, N);
        hicl_load("data/foo.cl", "-DSTENCIL=9");
        k = hicl_knl_find("test_hicl_1");
        hicl_knl_set_wrk(k, 1, &g, &l);
        hicl_timer_tick();
        for (i=0; i<5; ++i)  
            hicl_knl_set_and_srun(k, d, hsrc, hdst, N);
        hicl_timer_tick();
        for (i=0; i<ITER; ++i)  
            hicl_knl_run(k, d);
        hicl_dev_wait(d);
        CL_PRINT("===> time: %f %s", hicl_timer_read()/ITER, hicl_timer_uget());
        hicl_mem_update(hdst, READ_ONLY);
        for (i = 0; i < N; ++i) ASSERT_FLOAT_EQ((hdst)[i], i);
    }

    TEST_F(KnlTest, copy_1d_stdalone_rw_device_zero_copy) {
        unsigned int i;
        knl k;
        float *hsrc;
        float *hdst;
        hsrc = (float*)hicl_mem_allocate(d, N, HWA | ZERO_COPY);
        hdst = (float*)hicl_mem_allocate(d, N, HWA | ZERO_COPY);
        hicl_mem_update(hsrc, WRITE_ONLY);
        populate(hsrc , N);
        hicl_load("data/foo.cl", "-DSTENCIL=9");
        k = hicl_knl_find("test_hicl_1");
        hicl_knl_set_wrk(k, 1, &g, &l);
        for (i=0; i<5; ++i)  
            hicl_knl_set_and_srun(k, d, hsrc, hdst, N);
        hicl_timer_tick();
        for (i=0; i<ITER; ++i)  
            hicl_knl_run(k, d);
        hicl_dev_wait(d);
        CL_PRINT("===> time: %f %s", hicl_timer_read()/ITER, hicl_timer_uget());
        hicl_mem_update(hdst, READ_ONLY);
        for (i = 0; i < N; ++i) ASSERT_FLOAT_EQ(hdst[i], i);
    }

    /// wrapped
    TEST_F(KnlTest, copy_1d_wrapped_duplicated) {
        unsigned int i;
        knl k;
        float *hsrc;
        posix_memalign((void**)(&hsrc), __API_MEM_ALIGN_SIZE, N*sizeof(float));
        float *hdst;
        posix_memalign((void**)(&hdst), __API_MEM_ALIGN_SIZE, N*sizeof(float));
        hicl_mem_wrap(d, hsrc, N, HWA | READ_ONLY);
        hicl_mem_wrap(d, hdst, N, HWA | WRITE_ONLY);
        hicl_mem_update(hsrc, READ_WRITE);
        populate(hsrc, N);
        hicl_load("data/foo.cl", "-DSTENCIL=9");
        k = hicl_knl_find("test_hicl_1");
        hicl_knl_set_wrk(k, 1, &g, &l);
        for (i=0; i<5; ++i)  
            hicl_knl_set_and_srun(k, d, hsrc, hdst, N);
        hicl_timer_tick();
        for (i=0; i<ITER; ++i)  
            hicl_knl_run(k, d);
        hicl_dev_wait(d);
        CL_PRINT("===> time: %f %s", hicl_timer_read()/ITER, hicl_timer_uget());
        hicl_mem_update(hdst, READ_ONLY);
        for (i = 0; i < N; ++i) ASSERT_FLOAT_EQ(hdst[i], i);
        free(hsrc);
        free(hdst);
    }

    TEST_F(KnlTest, copy_1d_wrapped_pinned) {
        unsigned int i;
        knl k;
        float *hsrc;
        posix_memalign((void**)(&hsrc), __API_MEM_ALIGN_SIZE, N*sizeof(float));
        float *hdst;
        posix_memalign((void**)(&hdst), __API_MEM_ALIGN_SIZE, N*sizeof(float));
        hicl_mem_wrap(d, hsrc, N, HWA | PINNED | READ_ONLY);
        hicl_mem_wrap(d, hdst, N, HWA | PINNED | WRITE_ONLY);
        hicl_mem_update(hsrc, WRITE_ONLY);
        populate(hsrc, N);
        hicl_load("data/foo.cl", "-DSTENCIL=9");
        k = hicl_knl_find("test_hicl_1");
        hicl_knl_set_wrk(k, 1, &g, &l);
        for (i=0; i<5; ++i)  
            hicl_knl_set_and_srun(k, d, hsrc, hdst, N);
        hicl_timer_tick();
        for (i=0; i<ITER; ++i)  
            hicl_knl_run(k, d);
        hicl_dev_wait(d);
        CL_PRINT("===> time: %f %s", hicl_timer_read()/ITER, hicl_timer_uget());
        hicl_mem_update(hdst, READ_ONLY);
        for (i = 0; i < N; ++i) ASSERT_FLOAT_EQ(hdst[i], i);
        free(hsrc);
        free(hdst);
    }

    TEST_F(KnlTest, copy_1d_wrapped_host_zero_copy) {
        unsigned int i;
        knl k;
        float *hsrc;
        posix_memalign((void**)(&hsrc), __API_MEM_ALIGN_SIZE, N*sizeof(float));
        float *hdst;
        posix_memalign((void**)(&hdst), __API_MEM_ALIGN_SIZE, N*sizeof(float));
        hicl_mem_wrap(d, hsrc, N, CPU | ZERO_COPY | READ_ONLY);
        hicl_mem_wrap(d, hdst, N, CPU | ZERO_COPY | WRITE_ONLY);
        populate(hsrc, N);

        hicl_mem_update(hsrc, WRITE_ONLY);
        
        for (i = 0; i < N; ++i) ASSERT_FLOAT_EQ(hsrc[i], i);
        hicl_load("data/foo.cl", "-DSTENCIL=9");
        k = hicl_knl_find("test_hicl_1");
        hicl_knl_set_wrk(k, 1, &g, &l);
        for (i=0; i<5; ++i)  
            hicl_knl_set_and_srun(k, d, hsrc, hdst, N);
        hicl_timer_tick();
        for (i=0; i<ITER; ++i)  
            hicl_knl_run(k, d);
        hicl_dev_wait(d);
        CL_PRINT("===> time: %f %s", hicl_timer_read()/ITER, hicl_timer_uget());
        hicl_mem_update(hdst, READ_ONLY);
        for (i = 0; i < N; ++i) ASSERT_FLOAT_EQ(hdst[i], i);
        free(hsrc);
        free(hdst);
    }

    TEST_F(KnlTest, copy_1d_rw_wrapped_duplicated) {
        unsigned int i;
        knl k;
        float *hsrc;
        posix_memalign((void**)(&hsrc), __API_MEM_ALIGN_SIZE, N*sizeof(float));
        float *hdst;
        posix_memalign((void**)(&hdst), __API_MEM_ALIGN_SIZE, N*sizeof(float));
        hicl_mem_wrap(d, hsrc, N, HWA);
        hicl_mem_wrap(d, hdst, N, HWA);
        hicl_mem_update(hsrc, READ_WRITE);
        populate(hsrc, N);
        hicl_load("data/foo.cl", "-DSTENCIL=9");
        k = hicl_knl_find("test_hicl_1");
        hicl_knl_set_wrk(k, 1, &g, &l);
        for (i=0; i<5; ++i)  
            hicl_knl_set_and_srun(k, d, hsrc, hdst, N);
        hicl_timer_tick();
        for (i=0; i<ITER; ++i)  
            hicl_knl_run(k, d);
        hicl_dev_wait(d);
        CL_PRINT("===> time: %f %s", hicl_timer_read()/ITER, hicl_timer_uget());
        hicl_mem_update(hdst, READ_ONLY);
        for (i = 0; i < N; ++i) ASSERT_FLOAT_EQ(hdst[i], i);
        free(hsrc);
        free(hdst);
    }

    TEST_F(KnlTest, copy_1d_rw_wrapped_pinned) {
        unsigned int i;
        knl k;
        float *hsrc;
        posix_memalign((void**)(&hsrc), __API_MEM_ALIGN_SIZE, N*sizeof(float));
        float *hdst;
        posix_memalign((void**)(&hdst), __API_MEM_ALIGN_SIZE, N*sizeof(float));
        hicl_mem_wrap(d, hsrc, N, HWA | PINNED);
        hicl_mem_wrap(d, hdst, N, HWA | PINNED);
        hicl_mem_update(hsrc, WRITE_ONLY);
        populate(hsrc, N);
        hicl_load("data/foo.cl", "-DSTENCIL=9");
        k = hicl_knl_find("test_hicl_1");
        hicl_knl_set_wrk(k, 1, &g, &l);
        for (i=0; i<5; ++i)  
            hicl_knl_set_and_srun(k, d, hsrc, hdst, N);
        hicl_timer_tick();
        for (i=0; i<ITER; ++i)  
            hicl_knl_run(k, d);
        hicl_dev_wait(d);
        CL_PRINT("===> time: %f %s", hicl_timer_read()/ITER, hicl_timer_uget());
        hicl_mem_update(hdst, READ_ONLY);
        for (i = 0; i < N; ++i) ASSERT_FLOAT_EQ(hdst[i], i);
        free(hsrc);
        free(hdst);
    }

    TEST_F(KnlTest, copy_1d_rw_wrapped_host_zero_copy) {
        unsigned int i;
        knl k;
        float *hsrc;
        posix_memalign((void**)(&hsrc), __API_MEM_ALIGN_SIZE, N*sizeof(float));
        float *hdst;
        posix_memalign((void**)(&hdst), __API_MEM_ALIGN_SIZE, N*sizeof(float));
        hicl_mem_wrap(d, hsrc, N, CPU | ZERO_COPY);
        hicl_mem_wrap(d, hdst, N, CPU | ZERO_COPY);
        populate(hsrc, N);

        hicl_mem_update(hsrc, WRITE_ONLY);
        
        for (i = 0; i < N; ++i) ASSERT_FLOAT_EQ(hsrc[i], i);
        hicl_load("data/foo.cl", "-DSTENCIL=9");
        k = hicl_knl_find("test_hicl_1");
        hicl_knl_set_wrk(k, 1, &g, &l);
        for (i=0; i<5; ++i)  
            hicl_knl_set_and_srun(k, d, hsrc, hdst, N);
        hicl_timer_tick();
        for (i=0; i<ITER; ++i)  
            hicl_knl_run(k, d);
        hicl_dev_wait(d);
        CL_PRINT("===> time: %f %s", hicl_timer_read()/ITER, hicl_timer_uget());
        hicl_mem_update(hdst, READ_ONLY);
        for (i = 0; i < N; ++i) ASSERT_FLOAT_EQ(hdst[i], i);
        free(hsrc);
        free(hdst);
    }

    ///
    /// Stencil 3D
    /// 
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
        size_t size = (2*s[0] + dim[0])*(2*s[1] + dim[1])*(2*s[2] + dim[2])*sizeof(float);
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

    /// stdalone
    
    TEST_F(KnlTest, stencil_3d_stdalone_duplicated) {
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
        
        sprintf(options, "-DSTENCIL=%d -DLX=%lu -DLY=%lu", s[0], l[0], l[1]);
                        
        hicl_load("data/stencil_lv_3d.cl", options);
        
        knl k     = hicl_knl_find("stencil_lv_3d");
        float* ui    = (float*)hicl_mem_allocate(d, size, HWA | READ_ONLY);
        float* uo    = (float*)hicl_mem_allocate(d, size, HWA | WRITE_ONLY);
        float* coefx = (float*)hicl_mem_allocate(d, s[0]+1, READ_ONLY);
        float* coefy = (float*)hicl_mem_allocate(d, s[1]+1, READ_ONLY);
        float* coefz = (float*)hicl_mem_allocate(d, s[2]+1, READ_ONLY);
        
        hicl_mem_update(ui,    WRITE_ONLY);
        hicl_mem_update(coefx, WRITE_ONLY);
        hicl_mem_update(coefy, WRITE_ONLY);
        hicl_mem_update(coefz, WRITE_ONLY); 
        
        randomize_buffer(ui, size);
        setup_stencil_coeffs(coefx);
        setup_stencil_coeffs(coefy);
        setup_stencil_coeffs(coefz);
        
        hicl_knl_set_wrk(k, 2, g, l);
        for (i = 0; i < 5; ++i)
            hicl_knl_set_and_srun(k, d, ui, uo, coefx, coefy, coefz,
                                    dim[0], dim[1], dim[2], s[0], s[1], s[2]);
        hicl_timer_tick();
        for (i=0; i<n; ++i)
            hicl_knl_run(k, d);
        hicl_dev_wait(d);
        time = hicl_timer_read();
        hicl_timer_tick();
        hicl_mem_update(uo, READ_ONLY);
        comm = hicl_timer_read();
        hicl_mem_update(ui, READ_ONLY);
        CL_PRINT("avertage time: %8.2f (%8.2f) %s, %8.2f (%8.2f) Gflop/s",
                 time/n, comm + time/n, hicl_timer_uget(),
                 flops*1.e-9*n*dim[0]*dim[1]*dim[2]/(time*hicl_timer_coef()),
                 flops*1.e-9*dim[0]*dim[1]*dim[2]/((comm+(time/n))*hicl_timer_coef()));
        check_stencil_3d(dim, s, 
                         coefx, coefy, coefz, 
                         ui, uo, epsilon);
    }
    /*
    TEST_F(KnlTest, stencil_3d_stdalone_pinned) {
        double comm, time, flops;
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
        
        sprintf(options, "-DSTENCIL=%d -DLX=%lu -DLY=%lu", s[0], l[0], l[1]);
                        
        hicl_load("data/stencil_lv_3d.cl", options);
        
        knl k      = hicl_knl_find("stencil_lv_3d");
        float* ui    = (float*)hicl_mem_allocate(d, size, PINNED | READ_ONLY);
        float* uo    = (float*)hicl_mem_allocate(d, size, PINNED | WRITE_ONLY);
        float* coefx = (float*)hicl_mem_allocate(d, s[0]+1, READ_ONLY);
        float* coefy = (float*)hicl_mem_allocate(d, s[1]+1, READ_ONLY);
        float* coefz = (float*)hicl_mem_allocate(d, s[2]+1, READ_ONLY);
        
        hicl_mem_update(ui,    WRITE_ONLY);
        hicl_mem_update(coefx, WRITE_ONLY);
        hicl_mem_update(coefy, WRITE_ONLY);
        hicl_mem_update(coefz, WRITE_ONLY); 
        
        randomize_buffer(ui, size);
        setup_stencil_coeffs(coefx);
        setup_stencil_coeffs(coefy);
        setup_stencil_coeffs(coefz);
        
        hicl_knl_set_wrk(k, 2, g, l);
        for (i = 0; i < 5; ++i)
            hicl_knl_set_and_srun(k, d, ui, uo, coefx, coefy, coefz,
                                   dim[0], dim[1], dim[2], s[0], s[1], s[2]);
        hicl_timer_tick();
        for (i=0; i<n; ++i)
            hicl_knl_run(k, d);
        hicl_dev_wait(d);
        time = hicl_timer_read();
        hicl_timer_tick();
        hicl_mem_update(uo, READ_ONLY);
        comm = hicl_timer_read();
        hicl_mem_update(ui, READ_ONLY);
        CL_PRINT("avertage time: %8.2f (%8.2f) %s, %8.2f (%8.2f) Gflop/s",
                 time/n, comm + time/n, hicl_timer_uget(),
                 flops*1.e-9*n*dim[0]*dim[1]*dim[2]/(time*hicl_timer_coef()),
                 flops*1.e-9*dim[0]*dim[1]*dim[2]/((comm+(time/n))*hicl_timer_coef()));
        check_stencil_3d(dim, s, 
                         coefx, coefy, coefz, 
                         ui, uo, epsilon);
    }

    TEST_F(KnlTest, stencil_3d_stdalone_host_zero_copy) {
        double comm, time, flops;
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
        
        sprintf(options, "-DSTENCIL=%d -DLX=%lu -DLY=%lu", s[0], l[0], l[1]);
                        
        hicl_load("data/stencil_lv_3d.cl", options);
        
        knl k      = hicl_knl_find("stencil_lv_3d");
        float* ui    = (float*)hicl_mem_allocate(d, size, CPU | ZERO_COPY | READ_ONLY);
        float* uo    = (float*)hicl_mem_allocate(d, size, CPU | ZERO_COPY | WRITE_ONLY);
        float* coefx = (float*)hicl_mem_allocate(d, s[0]+1, READ_ONLY);
        float* coefy = (float*)hicl_mem_allocate(d, s[1]+1, READ_ONLY);
        float* coefz = (float*)hicl_mem_allocate(d, s[2]+1, READ_ONLY);
        
        hicl_mem_update(ui,    WRITE_ONLY);
        hicl_mem_update(coefx, WRITE_ONLY);
        hicl_mem_update(coefy, WRITE_ONLY);
        hicl_mem_update(coefz, WRITE_ONLY); 
        
        randomize_buffer(ui, size);
        setup_stencil_coeffs(coefx);
        setup_stencil_coeffs(coefy);
        setup_stencil_coeffs(coefz);
        
        hicl_knl_set_wrk(k, 2, g, l);
        for (i = 0; i < 5; ++i)
            hicl_knl_set_and_srun(k, d, ui, uo, coefx, coefy, coefz,
                                   dim[0], dim[1], dim[2], s[0], s[1], s[2]);
        hicl_timer_tick();
        for (i=0; i<n; ++i)
            hicl_knl_run(k, d);
        hicl_dev_wait(d);
        time = hicl_timer_read();
        hicl_timer_tick();
        hicl_mem_update(uo, READ_ONLY);
        comm = hicl_timer_read();
        hicl_mem_update(ui, READ_ONLY);
        CL_PRINT("avertage time: %8.2f (%8.2f) %s, %8.2f (%8.2f) Gflop/s",
                 time/n, comm + time/n, hicl_timer_uget(),
                 flops*1.e-9*n*dim[0]*dim[1]*dim[2]/(time*hicl_timer_coef()),
                 flops*1.e-9*dim[0]*dim[1]*dim[2]/((comm+(time/n))*hicl_timer_coef()));
        check_stencil_3d(dim, s, 
                         coefx, coefy, coefz, 
                         ui, uo, epsilon);
    }

    TEST_F(KnlTest, stencil_3d_stdalone_device_zero_copy) {
        double comm, time, flops;
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
        
        sprintf(options, "-DSTENCIL=%d -DLX=%lu -DLY=%lu", s[0], l[0], l[1]);
                        
        hicl_load("data/stencil_lv_3d.cl", options);
        
        knl k      = hicl_knl_find("stencil_lv_3d");
        float* ui    = (float*)hicl_mem_allocate(d, size, HWA | ZERO_COPY | READ_ONLY);
        float* uo    = (float*)hicl_mem_allocate(d, size, HWA | ZERO_COPY | WRITE_ONLY);
        float* coefx = (float*)hicl_mem_allocate(d, s[0]+1, READ_ONLY);
        float* coefy = (float*)hicl_mem_allocate(d, s[1]+1, READ_ONLY);
        float* coefz = (float*)hicl_mem_allocate(d, s[2]+1, READ_ONLY);
        
        hicl_mem_update(ui,    WRITE_ONLY);
        hicl_mem_update(coefx, WRITE_ONLY);
        hicl_mem_update(coefy, WRITE_ONLY);
        hicl_mem_update(coefz, WRITE_ONLY); 
        
        randomize_buffer(ui, size);
        setup_stencil_coeffs(coefx);
        setup_stencil_coeffs(coefy);
        setup_stencil_coeffs(coefz);
        
        hicl_knl_set_wrk(k, 2, g, l);
        for (i = 0; i < 5; ++i)
            hicl_knl_set_and_srun(k, d, ui, uo, 
                                    coefx, coefy, coefz,
                                    dim[0], dim[1], dim[2], s[0], s[1], s[2]);
        hicl_timer_tick();
        for (i=0; i<n; ++i)
            hicl_knl_run(k, d);
        hicl_dev_wait(d);
        time = hicl_timer_read();
        hicl_timer_tick();
        hicl_mem_update(uo, READ_ONLY);
        comm = hicl_timer_read();
        hicl_mem_update(ui, READ_ONLY);
        CL_PRINT("avertage time: %8.2f (%8.2f) %s, %8.2f (%8.2f) Gflop/s",
                 time/n, comm + time/n, hicl_timer_uget(),
                 flops*1.e-9*n*dim[0]*dim[1]*dim[2]/(time*hicl_timer_coef()),
                 flops*1.e-9*dim[0]*dim[1]*dim[2]/((comm+(time/n))*hicl_timer_coef()));
        check_stencil_3d(dim, s, 
                         coefx, coefy, coefz, 
                         ui, uo, epsilon);
    }

    TEST_F(KnlTest, stencil_3d_rw_stdalone_duplicated) {
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
        
        sprintf(options, "-DSTENCIL=%d -DLX=%lu -DLY=%lu", s[0], l[0], l[1]);
                        
        hicl_load("data/stencil_lv_3d.cl", options);
        
        knl k      = hicl_knl_find("stencil_lv_3d");
        float* ui    = (float*)hicl_mem_allocate(d, size, HWA);
        float* uo    = (float*)hicl_mem_allocate(d, size, HWA);
        float* coefx = (float*)hicl_mem_allocate(d, s[0]+1, READ_ONLY);
        float* coefy = (float*)hicl_mem_allocate(d, s[1]+1, READ_ONLY);
        float* coefz = (float*)hicl_mem_allocate(d, s[2]+1, READ_ONLY);
        
        hicl_mem_update(ui,    WRITE_ONLY);
        hicl_mem_update(coefx, WRITE_ONLY);
        hicl_mem_update(coefy, WRITE_ONLY);
        hicl_mem_update(coefz, WRITE_ONLY); 
        
        randomize_buffer(ui, size);
        setup_stencil_coeffs(coefx);
        setup_stencil_coeffs(coefy);
        setup_stencil_coeffs(coefz);
        
        hicl_knl_set_wrk(k, 2, g, l);
        for (i = 0; i < 5; ++i)
            hicl_knl_set_and_srun(k, d, ui, uo, coefx, coefy, coefz,
                                   dim[0], dim[1], dim[2], s[0], s[1], s[2]);
        hicl_timer_tick();
        for (i=0; i<n; ++i)
            hicl_knl_run(k, d);
        hicl_dev_wait(d);
        time = hicl_timer_read();
        hicl_timer_tick();
        hicl_mem_update(uo, READ_ONLY);
        comm = hicl_timer_read();
        hicl_mem_update(ui, READ_ONLY);
        CL_PRINT("avertage time: %8.2f (%8.2f) %s, %8.2f (%8.2f) Gflop/s",
                 time/n, comm + time/n, hicl_timer_uget(),
                 flops*1.e-9*n*dim[0]*dim[1]*dim[2]/(time*hicl_timer_coef()),
                 flops*1.e-9*dim[0]*dim[1]*dim[2]/((comm+(time/n))*hicl_timer_coef()));
        check_stencil_3d(dim, s, 
                         coefx, coefy, coefz, 
                         ui, uo, epsilon);
    }

    TEST_F(KnlTest, stencil_3d_rw_stdalone_pinned) {
        double comm, time, flops;
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
        
        sprintf(options, "-DSTENCIL=%d -DLX=%lu -DLY=%lu", s[0], l[0], l[1]);
                        
        hicl_load("data/stencil_lv_3d.cl", options);
        
        knl k      = hicl_knl_find("stencil_lv_3d");
        float* ui    = (float*)hicl_mem_allocate(d, size,   PINNED);
        float* uo    = (float*)hicl_mem_allocate(d, size,   PINNED);
        float* coefx = (float*)hicl_mem_allocate(d, s[0]+1, READ_ONLY);
        float* coefy = (float*)hicl_mem_allocate(d, s[1]+1, READ_ONLY);
        float* coefz = (float*)hicl_mem_allocate(d, s[2]+1, READ_ONLY);
        
        hicl_mem_update(ui,    WRITE_ONLY);
        hicl_mem_update(coefx, WRITE_ONLY);
        hicl_mem_update(coefy, WRITE_ONLY);
        hicl_mem_update(coefz, WRITE_ONLY); 
        
        randomize_buffer(ui, size);
        setup_stencil_coeffs(coefx);
        setup_stencil_coeffs(coefy);
        setup_stencil_coeffs(coefz);
        
        hicl_knl_set_wrk(k, 2, g, l);
        for (i = 0; i < 5; ++i)
            hicl_knl_set_and_srun(k, d, ui, uo, coefx, coefy, coefz,
                                   dim[0], dim[1], dim[2], s[0], s[1], s[2]);
        hicl_timer_tick();
        for (i=0; i<n; ++i)
            hicl_knl_run(k, d);
        hicl_dev_wait(d);
        time = hicl_timer_read();
        hicl_timer_tick();
        hicl_mem_update(uo, READ_ONLY);
        comm = hicl_timer_read();
        hicl_mem_update(ui, READ_ONLY);
        CL_PRINT("avertage time: %8.2f (%8.2f) %s, %8.2f (%8.2f) Gflop/s",
                 time/n, comm + time/n, hicl_timer_uget(),
                 flops*1.e-9*n*dim[0]*dim[1]*dim[2]/(time*hicl_timer_coef()),
                 flops*1.e-9*dim[0]*dim[1]*dim[2]/((comm+(time/n))*hicl_timer_coef()));
        check_stencil_3d(dim, s, 
                         coefx, coefy, coefz, 
                         ui, uo, epsilon);
    }

    TEST_F(KnlTest, stencil_3d_rw_stdalone_host_zero_copy) {
        double comm, time, flops;
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
        
        sprintf(options, "-DSTENCIL=%d -DLX=%lu -DLY=%lu", s[0], l[0], l[1]);
                        
        hicl_load("data/stencil_lv_3d.cl", options);
        
        knl k      = hicl_knl_find("stencil_lv_3d");
        float* ui    = (float*)hicl_mem_allocate(d, size, CPU | ZERO_COPY);
        float* uo    = (float*)hicl_mem_allocate(d, size, CPU | ZERO_COPY);
        float* coefx = (float*)hicl_mem_allocate(d, s[0]+1, READ_ONLY);
        float* coefy = (float*)hicl_mem_allocate(d, s[1]+1, READ_ONLY);
        float* coefz = (float*)hicl_mem_allocate(d, s[2]+1, READ_ONLY);
        
        hicl_mem_update(ui,    WRITE_ONLY);
        hicl_mem_update(coefx, WRITE_ONLY);
        hicl_mem_update(coefy, WRITE_ONLY);
        hicl_mem_update(coefz, WRITE_ONLY); 
        
        randomize_buffer(ui, size);
        setup_stencil_coeffs(coefx);
        setup_stencil_coeffs(coefy);
        setup_stencil_coeffs(coefz);
        
        hicl_knl_set_wrk(k, 2, g, l);
        for (i = 0; i < 5; ++i)
            hicl_knl_set_and_srun(k, d, ui, uo, coefx, coefy, coefz,
                                   dim[0], dim[1], dim[2], s[0], s[1], s[2]);
        hicl_timer_tick();
        for (i=0; i<n; ++i)
            hicl_knl_run(k, d);
        hicl_dev_wait(d);
        time = hicl_timer_read();
        hicl_timer_tick();
        hicl_mem_update(uo, READ_ONLY);
        comm = hicl_timer_read();
        hicl_mem_update(ui, READ_ONLY);
        CL_PRINT("avertage time: %8.2f (%8.2f) %s, %8.2f (%8.2f) Gflop/s",
                 time/n, comm + time/n, hicl_timer_uget(),
                 flops*1.e-9*n*dim[0]*dim[1]*dim[2]/(time*hicl_timer_coef()),
                 flops*1.e-9*dim[0]*dim[1]*dim[2]/((comm+(time/n))*hicl_timer_coef()));
        check_stencil_3d(dim, s, 
                         coefx, coefy, coefz, 
                         ui, uo, epsilon);
    }

    TEST_F(KnlTest, stencil_3d_rw_stdalone_device_zero_copy) {
        double comm, time, flops;
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
        
        sprintf(options, "-DSTENCIL=%d -DLX=%lu -DLY=%lu", s[0], l[0], l[1]);
                        
        hicl_load("data/stencil_lv_3d.cl", options);
        
        knl k      = hicl_knl_find("stencil_lv_3d");
        float* ui    = (float*)hicl_mem_allocate(d, size, HWA | ZERO_COPY);
        float* uo    = (float*)hicl_mem_allocate(d, size, HWA | ZERO_COPY);
        float* coefx = (float*)hicl_mem_allocate(d, s[0]+1, READ_ONLY);
        float* coefy = (float*)hicl_mem_allocate(d, s[1]+1, READ_ONLY);
        float* coefz = (float*)hicl_mem_allocate(d, s[2]+1, READ_ONLY);
        
        hicl_mem_update(ui,    WRITE_ONLY);
        hicl_mem_update(coefx, WRITE_ONLY);
        hicl_mem_update(coefy, WRITE_ONLY);
        hicl_mem_update(coefz, WRITE_ONLY); 
                
        randomize_buffer(ui, size);
        setup_stencil_coeffs(coefx);
        setup_stencil_coeffs(coefy);
        setup_stencil_coeffs(coefz);
        
        hicl_knl_set_wrk(k, 2, g, l);
        for (i = 0; i < 5; ++i)
            hicl_knl_set_and_srun(k, d, ui, uo, coefx, coefy, coefz,
                                   dim[0], dim[1], dim[2], s[0], s[1], s[2]);
        hicl_timer_tick();
        for (i=0; i<n; ++i)
            hicl_knl_run(k, d);
        hicl_dev_wait(d);
        time = hicl_timer_read();
        hicl_timer_tick();
        hicl_mem_update(uo, READ_ONLY);
        comm = hicl_timer_read();
        hicl_mem_update(ui, READ_ONLY);
        CL_PRINT("avertage time: %8.2f (%8.2f) %s, %8.2f (%8.2f) Gflop/s",
                 time/n, comm + time/n, hicl_timer_uget(),
                 flops*1.e-9*n*dim[0]*dim[1]*dim[2]/(time*hicl_timer_coef()),
                 flops*1.e-9*dim[0]*dim[1]*dim[2]/((comm+(time/n))*hicl_timer_coef()));
        check_stencil_3d(dim, s, 
                         coefx, coefy, coefz, 
                         ui, uo, epsilon);
    } 
    */
    /// wrapped
    
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
        
        sprintf(options, "-DSTENCIL=%d -DLX=%lu -DLY=%lu", s[0], l[0], l[1]);
                        
        hicl_load("data/stencil_lv_3d.cl", options);
        
        knl k      = hicl_knl_find("stencil_lv_3d");
        
        float *ui;
        float *uo;
        float *coefx;
        float *coefy;
        float *coefz;

        posix_memalign((void**)(&ui),    __API_MEM_ALIGN_SIZE,     size*sizeof(float));
        posix_memalign((void**)(&uo),    __API_MEM_ALIGN_SIZE,     size*sizeof(float));
        posix_memalign((void**)(&coefx), __API_MEM_ALIGN_SIZE, (s[0]+1)*sizeof(float));
        posix_memalign((void**)(&coefy), __API_MEM_ALIGN_SIZE, (s[1]+1)*sizeof(float));
        posix_memalign((void**)(&coefz), __API_MEM_ALIGN_SIZE, (s[2]+1)*sizeof(float));

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
                
        hicl_knl_set_wrk(k, 2, g, l);
        for (i = 0; i < 5; ++i)
            hicl_knl_set_and_srun(k, d, ui, uo, coefx, coefy, coefz,
                                   dim[0], dim[1], dim[2], s[0], s[1], s[2]);
        hicl_timer_tick();
        for (i=0; i<n; ++i)
            hicl_knl_run(k, d);
        hicl_dev_wait(d);
        time = hicl_timer_read();
        hicl_timer_tick();
        hicl_mem_update(uo, READ_ONLY);
        comm = hicl_timer_read();
        CL_PRINT("avertage time: %8.2f (%8.2f) %s, %8.2f (%8.2f) Gflop/s",
                 time/n, comm + time/n, hicl_timer_uget(),
                 flops*1.e-9*n*dim[0]*dim[1]*dim[2]/(time*hicl_timer_coef()),
                 flops*1.e-9*dim[0]*dim[1]*dim[2]/((comm+(time/n))*hicl_timer_coef()));
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
        
        sprintf(options, "-DSTENCIL=%d -DLX=%lu -DLY=%lu", s[0], l[0], l[1]);
                        
        hicl_load("data/stencil_lv_3d.cl", options);
        
        knl k      = hicl_knl_find("stencil_lv_3d");
        
        float *ui;
        float *uo;
        float *coefx;
        float *coefy;
        float *coefz;

        posix_memalign((void**)(&ui),    __API_MEM_ALIGN_SIZE,     size*sizeof(float));
        posix_memalign((void**)(&uo),    __API_MEM_ALIGN_SIZE,     size*sizeof(float));
        posix_memalign((void**)(&coefx), __API_MEM_ALIGN_SIZE, (s[0]+1)*sizeof(float));
        posix_memalign((void**)(&coefy), __API_MEM_ALIGN_SIZE, (s[1]+1)*sizeof(float));
        posix_memalign((void**)(&coefz), __API_MEM_ALIGN_SIZE, (s[2]+1)*sizeof(float));

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
        
        hicl_knl_set_wrk(k, 2, g, l);
        for (i = 0; i < 5; ++i)
            hicl_knl_set_and_srun(k, d, ui, uo, coefx, coefy, coefz,
                                   dim[0], dim[1], dim[2], s[0], s[1], s[2]);
        hicl_timer_tick();
        for (i=0; i<n; ++i)
            hicl_knl_run(k, d);
        hicl_dev_wait(d);
        time = hicl_timer_read();
        hicl_timer_tick();
        hicl_mem_update(uo, READ_ONLY);
        comm = hicl_timer_read();
        CL_PRINT("avertage time: %8.2f (%8.2f) %s, %8.2f (%8.2f) Gflop/s",
                 time/n, comm + time/n, hicl_timer_uget(),
                 flops*1.e-9*n*dim[0]*dim[1]*dim[2]/(time*hicl_timer_coef()),
                 flops*1.e-9*dim[0]*dim[1]*dim[2]/((comm+(time/n))*hicl_timer_coef()));
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
        
        sprintf(options, "-DSTENCIL=%d -DLX=%lu -DLY=%lu", s[0], l[0], l[1]);
                        
        hicl_load("data/stencil_lv_3d.cl", options);
        
        knl k      = hicl_knl_find("stencil_lv_3d");
        
        float *ui;
        float *uo;
        float *coefx;
        float *coefy;
        float *coefz;

        posix_memalign((void**)(&ui),    __API_MEM_ALIGN_SIZE,     size*sizeof(float));
        posix_memalign((void**)(&uo),    __API_MEM_ALIGN_SIZE,     size*sizeof(float));
        posix_memalign((void**)(&coefx), __API_MEM_ALIGN_SIZE, (s[0]+1)*sizeof(float));
        posix_memalign((void**)(&coefy), __API_MEM_ALIGN_SIZE, (s[1]+1)*sizeof(float));
        posix_memalign((void**)(&coefz), __API_MEM_ALIGN_SIZE, (s[2]+1)*sizeof(float));

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
                        
        hicl_knl_set_wrk(k, 2, g, l);
        for (i = 0; i < 5; ++i)
            hicl_knl_set_and_srun(k, d, ui, uo, coefx, coefy, coefz,
                                   dim[0], dim[1], dim[2], s[0], s[1], s[2]);
        hicl_timer_tick();
        for (i=0; i<n; ++i)
            hicl_knl_run(k, d);
        hicl_dev_wait(d);
        time = hicl_timer_read();
        hicl_timer_tick();
        hicl_mem_update(uo, READ_ONLY);
        comm = hicl_timer_read();
        CL_PRINT("avertage time: %8.2f (%8.2f) %s, %8.2f (%8.2f) Gflop/s",
                 time/n, comm + (time/n), hicl_timer_uget(),
                 flops*1.e-9*n*dim[0]*dim[1]*dim[2]/(time*hicl_timer_coef()),
                 flops*1.e-9*dim[0]*dim[1]*dim[2]/((comm+(time/n))*hicl_timer_coef()));
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
        
        sprintf(options, "-DSTENCIL=%d -DLX=%lu -DLY=%lu", s[0], l[0], l[1]);
                        
        hicl_load("data/stencil_lv_3d.cl", options);
        
        knl k = hicl_knl_find("stencil_lv_3d");
        
        float *ui;
        float *uo;
        float *coefx;
        float *coefy;
        float *coefz;

        posix_memalign((void**)(&ui),    __API_MEM_ALIGN_SIZE,     size*sizeof(float));
        posix_memalign((void**)(&uo),    __API_MEM_ALIGN_SIZE,     size*sizeof(float));
        posix_memalign((void**)(&coefx), __API_MEM_ALIGN_SIZE, (s[0]+1)*sizeof(float));
        posix_memalign((void**)(&coefy), __API_MEM_ALIGN_SIZE, (s[1]+1)*sizeof(float));
        posix_memalign((void**)(&coefz), __API_MEM_ALIGN_SIZE, (s[2]+1)*sizeof(float));

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
                
        hicl_knl_set_wrk(k, 2, g, l);
        for (i = 0; i < 5; ++i)
            hicl_knl_set_and_srun(k, d, ui, uo, coefx, coefy, coefz,
                                   dim[0], dim[1], dim[2], s[0], s[1], s[2]);
        hicl_timer_tick();
        for (i=0; i<n; ++i)
            hicl_knl_run(k, d);
        hicl_dev_wait(d);
        time = hicl_timer_read();
        hicl_timer_tick();
        hicl_mem_update(uo, READ_ONLY);
        comm = hicl_timer_read();
        CL_PRINT("avertage time: %8.2f (%8.2f) %s, %8.2f (%8.2f) Gflop/s",
                 time/n, comm + time/n, hicl_timer_uget(),
                 flops*1.e-9*n*dim[0]*dim[1]*dim[2]/(time*hicl_timer_coef()),
                 flops*1.e-9*dim[0]*dim[1]*dim[2]/((comm+(time/n))*hicl_timer_coef()));
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
        
        sprintf(options, "-DSTENCIL=%d -DLX=%lu -DLY=%lu", s[0], l[0], l[1]);
                        
        hicl_load("data/stencil_lv_3d.cl", options);
        
        knl k = hicl_knl_find("stencil_lv_3d");
        
        float *ui;
        float *uo;
        float *coefx;
        float *coefy;
        float *coefz;

        posix_memalign((void**)(&ui),    __API_MEM_ALIGN_SIZE,     size*sizeof(float));
        posix_memalign((void**)(&uo),    __API_MEM_ALIGN_SIZE,     size*sizeof(float));
        posix_memalign((void**)(&coefx), __API_MEM_ALIGN_SIZE, (s[0]+1)*sizeof(float));
        posix_memalign((void**)(&coefy), __API_MEM_ALIGN_SIZE, (s[1]+1)*sizeof(float));
        posix_memalign((void**)(&coefz), __API_MEM_ALIGN_SIZE, (s[2]+1)*sizeof(float));

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
        
        hicl_knl_set_wrk(k, 2, g, l);
        for (i = 0; i < 5; ++i)
            hicl_knl_set_and_srun(k, d, ui, uo, coefx, coefy, coefz,
                                   dim[0], dim[1], dim[2], s[0], s[1], s[2]);
        hicl_timer_tick();
        for (i=0; i<n; ++i)
            hicl_knl_run(k, d);
        hicl_dev_wait(d);
        time = hicl_timer_read();
        hicl_timer_tick();
        hicl_mem_update(uo, READ_ONLY);
        comm = hicl_timer_read();
        CL_PRINT("avertage time: %8.2f (%8.2f) %s, %8.2f (%8.2f) Gflop/s",
                 time/n, comm + time/n, hicl_timer_uget(),
                 flops*1.e-9*n*dim[0]*dim[1]*dim[2]/(time*hicl_timer_coef()),
                 flops*1.e-9*dim[0]*dim[1]*dim[2]/((comm+(time/n))*hicl_timer_coef()));
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
        
        sprintf(options, "-DSTENCIL=%d -DLX=%lu -DLY=%lu", s[0], l[0], l[1]);
                        
        hicl_load("data/stencil_lv_3d.cl", options);
        
        knl k = hicl_knl_find("stencil_lv_3d");
        
        float *ui;
        float *uo;
        float *coefx;
        float *coefy;
        float *coefz;

        posix_memalign((void**)(&ui),    __API_MEM_ALIGN_SIZE,     size*sizeof(float));
        posix_memalign((void**)(&uo),    __API_MEM_ALIGN_SIZE,     size*sizeof(float));
        posix_memalign((void**)(&coefx), __API_MEM_ALIGN_SIZE, (s[0]+1)*sizeof(float));
        posix_memalign((void**)(&coefy), __API_MEM_ALIGN_SIZE, (s[1]+1)*sizeof(float));
        posix_memalign((void**)(&coefz), __API_MEM_ALIGN_SIZE, (s[2]+1)*sizeof(float));

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
                        
        hicl_knl_set_wrk(k, 2, g, l);
        for (i = 0; i < 5; ++i)
            hicl_knl_set_and_srun(k, d, ui, uo, coefx, coefy, coefz,
                                    dim[0], dim[1], dim[2], s[0], s[1], s[2]);
        hicl_timer_tick();
        for (i=0; i<n; ++i)
            hicl_knl_run(k, d);
        hicl_dev_wait(d);
        time = hicl_timer_read();
        hicl_timer_tick();
        hicl_mem_update(uo, READ_ONLY);
        comm = hicl_timer_read();
        CL_PRINT("avertage time: %8.2f (%8.2f) %s, %8.2f (%8.2f) Gflop/s",
                 time/n, comm + (time/n), hicl_timer_uget(),
                 flops*1.e-9*n*dim[0]*dim[1]*dim[2]/(time*hicl_timer_coef()),
                 flops*1.e-9*dim[0]*dim[1]*dim[2]/((comm+(time/n))*hicl_timer_coef()));
        check_stencil_3d(dim, s, coefx, coefy, coefz, ui, uo, epsilon);
    }

}  // namespace
