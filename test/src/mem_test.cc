///
/// @copyright Copyright (c) 2013-2016, Université Pierre et Marie Curie
/// All rights reserved.
/// 
/// <b>hiCL</b> is owned by Université Pierre et Marie Curie, 
/// funded by TOTAL, and written by Issam SAID <said.issam@gmail.com>.
/// 
/// Redistribution and use in source and binary forms, with or without 
/// modification,
/// are permitted provided that the following conditions are met:
/// 
/// 1. Redistributions of source code must retain the above copyright notice, 
///    this list of conditions and the following disclaimer.
/// 2. Redistributions in binary form must reproduce the above copyright notice,
///    this list of conditions and the following disclaimer in the documentation
///    and/or other materials provided with the distribution.
/// 3. Neither the name of the UPMC nor the names of its contributors
///    may be used to endorse or promote products derived from this software
///    without specific prior written permission.
/// 
/// THIS SOFTWARE IS PROVIDED ''AS IS'' AND ANY
/// EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
/// WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
/// DISCLAIMED. IN NO EVENT SHALL THE UPMC OR ITS CONTRIBUTORS BE LIABLE FOR ANY
/// DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
/// (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
/// LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND 
/// ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
/// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF 
/// THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
/// 
/// @file mem_test.cc
/// @author Issam SAID
/// @brief Unit testing file for the hiCL memory objects manipulation routines.
///
#include <hiCL/base.h>
#include <hiCL/knl.h>
#include <hiCL/mem.h>
#include <hiCL/timer.h>
#include <hiCL/dev.h>
#include <gtest/gtest.h>
#include <__api/mem-inl.h>
#include <__api/util-inl.h>

#define MEM_ALIGN 128

namespace {

    class MemTest : public ::testing::Test {
    protected:
        hidev_t d;
        size_t N;
        char test_options[64];
        virtual void SetUp() {
            sprintf(test_options, "%s", "-cl-kernel-arg-info");
            hicl_init(DEFAULT);
            d = hicl_dev_find(DEFAULT);
            N = 128;
        }
        virtual void TearDown() { hicl_release(); }
    };

    TEST_F(MemTest, wrap_default_buffer) {
        float *h;
        int ret_val; 
        himem_t m;
        ret_val = posix_memalign((void**)(&h), MEM_ALIGN, N*sizeof(float));
        if (ret_val) {fprintf(stderr, "Failed to allocate memory\n"); FAIL();}
        m       = hicl_mem_wrap(d, h, N, DEFAULT);
        ASSERT_EQ(h, m->h);
        ASSERT_EQ(m,  __api_mem_find(h));
        ASSERT_EQ(m->size*m->unit_size, N*sizeof(float));
        ASSERT_TRUE(__API_MEM_HWA(m->flags));
        ASSERT_TRUE(__API_MEM_FLOAT(m->flags));
        hicl_mem_info(h);
        free(h);
    }

    TEST_F(MemTest, wrap_float_hwa_buffer) {
        float *h;
        int ret_val;
        himem_t m;
        ret_val = posix_memalign((void**)(&h), MEM_ALIGN, N*sizeof(float));
        if (ret_val) {fprintf(stderr, "Failed to allocate memory\n"); FAIL();}
        m = hicl_mem_wrap(d, h, N, HWA);
        ASSERT_EQ(h, m->h);
        ASSERT_EQ(m,  __api_mem_find(h));
        ASSERT_EQ(m->size*m->unit_size, N*sizeof(float));
        ASSERT_TRUE(__API_MEM_HWA(m->flags));
        ASSERT_TRUE(__API_MEM_FLOAT(m->flags));
        hicl_mem_info(h);
        free(h);
    }

    TEST_F(MemTest, wrap_int_pinned_buffer) {
        int *h ;
        int ret_val;
        himem_t m;
        ret_val = posix_memalign((void**)(&h), MEM_ALIGN, N*sizeof(int));    
        if (ret_val) {fprintf(stderr, "Failed to allocate memory\n"); FAIL();}
        m   = hicl_mem_wrap(d, h, N, HWA | PINNED | INT);
        ASSERT_EQ(m, find_rbn_address_t_himem_t(&hicl->mems, h)->value);
        ASSERT_EQ(m->size*m->unit_size, N*sizeof(int));
        ASSERT_TRUE(__API_MEM_HWA(m->flags));
        ASSERT_TRUE(__API_MEM_PINNED(m->flags));
        ASSERT_TRUE(__API_MEM_INT(m->flags));
        hicl_mem_info(h);
        free(h);
    }

    TEST_F(MemTest, wrap_float_read_only_buffer) {
        float *h ; 
        int ret_val;
        himem_t m;
        ret_val = posix_memalign((void**)(&h), MEM_ALIGN, N*sizeof(float));
        if (ret_val) {fprintf(stderr, "Failed to allocate memory\n"); FAIL();}
        m    = hicl_mem_wrap(d, h, N, READ_ONLY);
        ASSERT_TRUE(__API_MEM_READ_ONLY(m->flags));
        ASSERT_EQ(m, find_rbn_address_t_himem_t(&hicl->mems, h)->value);
        hicl_mem_info(h);
        free(h);
    }

    TEST_F(MemTest, wrap_float_write_only_buffer) {
        float *h ;
        int ret_val;
        himem_t m;
        ret_val = posix_memalign((void**)(&h), MEM_ALIGN, N*sizeof(float));
        if (ret_val) {fprintf(stderr, "Failed to allocate memory\n"); FAIL();}
        m = hicl_mem_wrap(d, h, N, WRITE_ONLY);
        ASSERT_EQ(m, find_rbn_address_t_himem_t(&hicl->mems, h)->value);
        ASSERT_TRUE(__API_MEM_WRITE_ONLY(m->flags));
        hicl_mem_info(h);
        free(h);
    }

    TEST_F(MemTest, wrap_float_host_zero_copy_buffer) {
        float *h;
        int ret_val;
        himem_t m;
        ret_val = posix_memalign((void**)(&h), MEM_ALIGN, N*sizeof(float));
        if (ret_val) {fprintf(stderr, "Failed to allocate memory\n"); FAIL();}
        m = hicl_mem_wrap(d, h, N, CPU | ZERO_COPY);
        ASSERT_EQ(m, find_rbn_address_t_himem_t(&hicl->mems, h)->value);
        ASSERT_TRUE(__API_MEM_CPU(m->flags));
        ASSERT_TRUE(__API_MEM_ZERO_COPY(m->flags));
        //hicl_mem_info(h);
        free(h);
    }

    TEST_F(MemTest, default_buffer_access_operator) {
        unsigned int i;
        float *h;
        int ret_val;
        himem_t m;
        ret_val = posix_memalign((void**)(&h), MEM_ALIGN, N*sizeof(float));
        if (ret_val) {fprintf(stderr, "Failed to allocate memory\n"); FAIL();}
        m = hicl_mem_wrap(d, h, N, READ_WRITE);
        ASSERT_EQ(m->size, N);
        ASSERT_EQ(m->h, h);
        hicl_mem_update(h, WRITE_ONLY);
        for (i = 0; i < N; ++i) h[i] = i;
        hicl_mem_update(h, READ_ONLY);
        for (i = 0; i < N; ++i) ASSERT_FLOAT_EQ(((float*)m->h)[i], i*1.f);
        hicl_mem_info(h);
        free(h);
    }

    TEST_F(MemTest, pack_unpack_halos_using_rect) {
        unsigned int x, y, z, nw, nh, nd;
        float *buf;
        float *cpy;
        int ret_val;
        size_t g[2], l[2];

        nw = 128;
        nh = 128;
        nd = 128;

        l[0]=16;
        l[1]=16;
        g[0]=nw;
        g[1]=nh;
        ret_val = posix_memalign((void**)(&buf), 
                       MEM_ALIGN, (nw+8)*(nh+8)*(nd+8)*sizeof(float));
        if (ret_val) {fprintf(stderr, "Failed to allocate memory\n"); FAIL();}
        ret_val = posix_memalign((void**)(&cpy), 
                       MEM_ALIGN, (nw+8)*(nh+8)*(nd+8)*sizeof(float));
        if (ret_val) {fprintf(stderr, "Failed to allocate memory\n"); FAIL();}
        
        himem_t m = hicl_mem_wrap(d, buf, (nw+8)*(nh+8)*(nd+8), READ_WRITE);
        hicl_mem_wrap(d, cpy, (nw+8)*(nh+8)*(nd+8), READ_WRITE);

        ASSERT_EQ(m->size, (nw+8)*(nh+8)*(nd+8));
        ASSERT_EQ(m->h, buf);
        hicl_load("data/select.cl", test_options);
        hicl_knl_set_wrk("kselect", 2, g, l);
        hicl_mem_update(buf, WRITE_ONLY);
        for (z=0; z < nd+8; ++z)
            for (y=0; y < nh+8; ++y)
                for (x=0; x < nw+8; ++x)
                    buf[(nw+8)*(z*(nh+8) + y) + x] = -1;                         
        for (z=4; z < 8; ++z) {
            for (y=4; y < nh+4; ++y) {
                for (x=4; x < nw+4; ++x) {
                    ASSERT_FLOAT_EQ(buf[(nw+8)*(z*(nh+8) + y) + x],-1*1.f);
                }
            }
        }
        hicl_knl_sync_run("kselect", d, buf, nw, nh, nd);

        // pack
        hicl_timer_tick();
        hicl_mem_pop(buf,  4, nw+3,  4, nh+3,  4,    7, nw+8, nh+8, CL_FALSE);
        hicl_mem_pop(buf,  4, nw+3,  4, nh+3, nd, nd+3, nw+8, nh+8, CL_FALSE);
        hicl_mem_pop(buf,  4,    7,  4, nh+3,  4, nd+3, nw+8, nh+8, CL_FALSE);
        hicl_mem_pop(buf, nw, nw+3,  4, nh+3,  4, nd+3, nw+8, nh+8, CL_FALSE);
        hicl_mem_pop(buf,  4, nw+3,  4,    7,  4, nd+3, nw+8, nh+8, CL_FALSE);
        hicl_mem_pop(buf,  4, nw+3, nh, nh+3,  4, nd+3, nw+8, nh+8, CL_TRUE);
        fprintf(stdout, "... time pack       : %f %s\n", 
                hicl_timer_read(), hicl_timer_uget());
        // Z
        for (z=4; z < 8; ++z) {
            for (y=4; y < nh+4; ++y) {
                for (x=4; x < nw+4; ++x) {
                    ASSERT_FLOAT_EQ(buf[(nw+8)*(z*(nh+8) + y) + x], 
                                  ((nw+8)*(z*(nh+8) + y) + x)*1.f);
                }
            }
        }
        for (z=nd; z < nd+4; ++z) {
            for (y=4; y < nh+4; ++y) {
                for (x=4; x < nw+4; ++x) {
                    ASSERT_FLOAT_EQ(buf[(nw+8)*(z*(nh+8) + y) + x], 
                                   ((nw+8)*(z*(nh+8) + y) + x)*1.f);
                }
            }
        }
        // X
        for (z=4; z < nd+4; ++z) {
            for (y=4; y < nh+4; ++y) {
                for (x=4; x < 7; ++x) {
                    ASSERT_FLOAT_EQ(buf[(nw+8)*(z*(nh+8) + y) + x], 
                                  ((nw+8)*(z*(nh+8) + y) + x)*1.f);
                }
            }
        }
        for (z=4; z < nd+4; ++z) {
            for (y=4; y < nh+4; ++y) {
                for (x=nw; x < nw+4; ++x) {
                    //printf("%d %d %d\n", z,y,x);
                    ASSERT_FLOAT_EQ(buf[(nw+8)*(z*(nh+8) + y) + x], 
                                   ((nw+8)*(z*(nh+8) + y) + x)*1.f);
                }
            }
        }
        // Y
        for (z=4; z < nd+4; ++z) {
            for (y=4; y < 8; ++y) {
                for (x=4; x < nw+4; ++x) {
                    //printf("%d %d %d\n", z,y,x);
                    ASSERT_FLOAT_EQ(buf[(nw+8)*(z*(nh+8) + y) + x], 
                                  ((nw+8)*(z*(nh+8) + y) + x)*1.f);
                }
            }
        }
        for (z=4; z < nd+4; ++z) {
            for (y=nh; y < nh+4; ++y) {
                for (x=4; x < nw+4; ++x) {
                    //printf("%d %d %d\n", z,y,x);
                    ASSERT_FLOAT_EQ(buf[(nw+8)*(z*(nh+8) + y) + x], 
                                   ((nw+8)*(z*(nh+8) + y) + x)*1.f);
                }
            }
        }
        for (z=8; z < nd; ++z) {
            for (y=8; y < nh; ++y) {
                for (x=8; x < nw; ++x) {
                    ASSERT_FLOAT_EQ(buf[(nw+8)*(z*(nh+8) + y) + x], -1*1.f);
                }
            }
        }
        hicl_knl_sync_run("kselect", d, buf, nw, nh, nd);
        hicl_timer_tick();
        hicl_mem_update(buf, READ_ONLY);
        fprintf(stdout, "... time full update: %f %s\n", 
                hicl_timer_read(), hicl_timer_uget());

        for (z=0; z < nd+8; ++z) {
            for (y=0; y < nh+8; ++y) {
                for (x=0; x < nw+8; ++x) {
                    cpy[(nw+8)*(z*(nh+8) + y) + x] = 
                        buf[(nw+8)*(z*(nh+8) + y) + x];
                }
            }
        }
        
        // unpack
        hicl_timer_tick();
        hicl_mem_push(cpy,  4, nw+3,  4, nh+3,  4,    7, nw+8, nh+8, CL_FALSE);
        hicl_mem_push(cpy,  4, nw+3,  4, nh+3, nd, nd+3, nw+8, nh+8, CL_FALSE);
        hicl_mem_push(cpy,  4,    7,  4, nh+3,  4, nd+3, nw+8, nh+8, CL_FALSE);
        hicl_mem_push(cpy, nw, nw+3,  4, nh+3,  4, nd+3, nw+8, nh+8, CL_FALSE);
        hicl_mem_push(cpy,  4, nw+3,  4,    7,  4, nd+3, nw+8, nh+8, CL_FALSE);
        hicl_mem_push(cpy,  4, nw+3, nh, nh+3,  4, nd+3, nw+8, nh+8, CL_TRUE);
        fprintf(stdout, "... time unpack     : %f %s\n", 
                hicl_timer_read(), hicl_timer_uget());

        for (z=0; z < nd+8; ++z) {
            for (y=0; y < nh+8; ++y) {
                for (x=0; x < nw+8; ++x) {
                    cpy[(nw+8)*(z*(nh+8) + y) + x] = -1;
                }
            }
        }

        hicl_mem_update(cpy, READ_ONLY);
        
        // Z
        for (z=4; z < 8; ++z) {
            for (y=4; y < nh+4; ++y) {
                for (x=4; x < nw+4; ++x) {
                    printf("%d %d %d\n", z,y,x);
                    ASSERT_FLOAT_EQ(cpy[(nw+8)*(z*(nh+8) + y) + x], 
                                  ((nw+8)*(z*(nh+8) + y) + x)*1.f);
                }
            }
        }
        for (z=nd; z < nd+4; ++z) {
            for (y=4; y < nh+4; ++y) {
                for (x=4; x < nw+4; ++x) {
                    ASSERT_FLOAT_EQ(cpy[(nw+8)*(z*(nh+8) + y) + x], 
                                   ((nw+8)*(z*(nh+8) + y) + x)*1.f);
                }
            }
        }
        // X
        for (z=4; z < nd+4; ++z) {
            for (y=4; y < nh+4; ++y) {
                for (x=4; x < 7; ++x) {
                    ASSERT_FLOAT_EQ(cpy[(nw+8)*(z*(nh+8) + y) + x], 
                                  ((nw+8)*(z*(nh+8) + y) + x)*1.f);
                }
            }
        }
        for (z=4; z < nd+4; ++z) {
            for (y=4; y < nh+4; ++y) {
                for (x=nw; x < nw+4; ++x) {
                    //printf("%d %d %d\n", z,y,x);
                    ASSERT_FLOAT_EQ(cpy[(nw+8)*(z*(nh+8) + y) + x], 
                                   ((nw+8)*(z*(nh+8) + y) + x)*1.f);
                }
            }
        }
        // Y
        for (z=4; z < nd+4; ++z) {
            for (y=4; y < 8; ++y) {
                for (x=4; x < nw+4; ++x) {
                    //printf("%d %d %d\n", z,y,x);
                    ASSERT_FLOAT_EQ(cpy[(nw+8)*(z*(nh+8) + y) + x], 
                                  ((nw+8)*(z*(nh+8) + y) + x)*1.f);
                }
            }
        }
        for (z=4; z < nd+4; ++z) {
            for (y=nh; y < nh+4; ++y) {
                for (x=4; x < nw+4; ++x) {
                    //printf("%d %d %d\n", z,y,x);
                    ASSERT_FLOAT_EQ(cpy[(nw+8)*(z*(nh+8) + y) + x], 
                                   ((nw+8)*(z*(nh+8) + y) + x)*1.f);
                }
            }
        }

        free(buf);
        free(cpy);
    }

    TEST_F(MemTest, pack_unpack_halos_using_kernels) {
        unsigned int x, y, z, nw, nh, nd;
        float *buf, *cpy;
        float *tab_north;
        float *tab_south;
        float *tab_west;
        float *tab_east;
        float *tab_front;
        float *tab_back;
        int ret_val;
        size_t g[2], l[2];

        nw = 128;
        nh = 128;
        nd = 128;

        l[0]=16;
        l[1]=16;
        g[0]=nw;
        g[1]=nh;
        ret_val = posix_memalign((void**)(&buf), 
                       MEM_ALIGN, (nw+8)*(nh+8)*(nd+8)*sizeof(float));
        if (ret_val) {fprintf(stderr, "Failed to allocate memory\n"); FAIL();}
        ret_val = posix_memalign((void**)(&cpy), 
                       MEM_ALIGN, (nw+8)*(nh+8)*(nd+8)*sizeof(float));
        if (ret_val) {fprintf(stderr, "Failed to allocate memory\n"); FAIL();}
        ret_val = posix_memalign((void**)(&tab_north), 
                       MEM_ALIGN, (nw)*(nh)*(4)*sizeof(float));
        if (ret_val) {fprintf(stderr, "Failed to allocate memory\n"); FAIL();}
        ret_val = posix_memalign((void**)(&tab_south), 
                       MEM_ALIGN, (nw)*(nh)*(4)*sizeof(float));
        if (ret_val) {fprintf(stderr, "Failed to allocate memory\n"); FAIL();}
        ret_val = posix_memalign((void**)(&tab_west), 
                       MEM_ALIGN, (nd)*(nh)*(4)*sizeof(float));
        if (ret_val) {fprintf(stderr, "Failed to allocate memory\n"); FAIL();}
        ret_val = posix_memalign((void**)(&tab_east), 
                       MEM_ALIGN, (nd)*(nh)*(4)*sizeof(float));
        if (ret_val) {fprintf(stderr, "Failed to allocate memory\n"); FAIL();}
        ret_val = posix_memalign((void**)(&tab_front), 
                       MEM_ALIGN, (nd)*(nw)*(4)*sizeof(float));
        if (ret_val) {fprintf(stderr, "Failed to allocate memory\n"); FAIL();}
        ret_val = posix_memalign((void**)(&tab_back), 
                       MEM_ALIGN, (nd)*(nw)*(4)*sizeof(float));
        if (ret_val) {fprintf(stderr, "Failed to allocate memory\n"); FAIL();}

        hicl_mem_wrap(d, buf, (nw+8)*(nh+8)*(nd+8), READ_WRITE);
        hicl_mem_wrap(d, cpy, (nw+8)*(nh+8)*(nd+8), READ_WRITE);
        hicl_mem_wrap(d, tab_north, (nw)*(nh)*(4),  READ_WRITE);
        hicl_mem_wrap(d, tab_south, (nw)*(nh)*(4),  READ_WRITE);
        hicl_mem_wrap(d, tab_west,  (nd)*(nh)*(4),  READ_WRITE);
        hicl_mem_wrap(d, tab_east,  (nd)*(nh)*(4),  READ_WRITE);
        hicl_mem_wrap(d, tab_front, (nd)*(nw)*(4),  READ_WRITE);
        hicl_mem_wrap(d, tab_back,  (nd)*(nw)*(4),  READ_WRITE);
        
        hicl_load("data/select.cl", test_options);
        hicl_knl_set_wrk("kselect", 2, g, l);
        hicl_mem_update(buf, WRITE_ONLY);
        for (z=0; z < nd+8; ++z)
            for (y=0; y < nh+8; ++y)
                for (x=0; x < nw+8; ++x)
                    buf[(nw+8)*(z*(nh+8) + y) + x] = -1;                         
        for (z=4; z < 8; ++z) {
            for (y=4; y < nh+4; ++y) {
                for (x=4; x < nw+4; ++x) {
                    ASSERT_FLOAT_EQ(buf[(nw+8)*(z*(nh+8) + y) + x],-1*1.f);
                }
            }
        }
        hicl_knl_sync_run("kselect", d, buf, nw, nh, nd);

        // pack
        g[0] = nw;
        g[1] = nh;
        hicl_knl_set_wrk("pack_north", 2, g, l);
        hicl_knl_set_wrk("pack_south", 2, g, l);
        g[0] = 4;
        g[1] = nh;
        l[0] = 4;
        l[1] = 64;
        hicl_knl_set_wrk("pack_west", 2, g, l);
        hicl_knl_set_wrk("pack_east", 2, g, l);
        g[0] = nw;
        g[1] = 4;
        l[0] = 64;
        l[1] = 4;
        hicl_knl_set_wrk("pack_front", 2, g, l);
        hicl_knl_set_wrk("pack_back",  2, g, l);

        hicl_knl_set_args("pack_north", nw, nh, nd, buf, tab_north);
        hicl_knl_set_args("pack_south", nw, nh, nd, buf, tab_south);
        hicl_knl_set_args("pack_east",  nw, nh, nd, buf, tab_east);
        hicl_knl_set_args("pack_west",  nw, nh, nd, buf, tab_west);
        hicl_knl_set_args("pack_front", nw, nh, nd, buf, tab_front);
        hicl_knl_set_args("pack_back",  nw, nh, nd, buf, tab_back);

        hicl_timer_tick();
        hicl_knl_exec("pack_north", d);
        hicl_knl_exec("pack_south", d);
        hicl_knl_exec("pack_east",  d);
        hicl_knl_exec("pack_west",  d);
        hicl_knl_exec("pack_front", d);
        hicl_knl_exec("pack_back",  d);
        hicl_mem_update(tab_north, READ_ONLY);
        hicl_mem_update(tab_south, READ_ONLY);
        hicl_mem_update(tab_west,  READ_ONLY);
        hicl_mem_update(tab_east,  READ_ONLY);
        hicl_mem_update(tab_front, READ_ONLY);
        hicl_mem_update(tab_back,  READ_ONLY);
        fprintf(stdout, "... time pack       : %f %s\n", 
                hicl_timer_read(), hicl_timer_uget());
        // Z
        for (z=4; z < 8; ++z) {
            for (y=4; y < nh+4; ++y) {
                for (x=4; x < nw+4; ++x) {
                    ASSERT_FLOAT_EQ(tab_south[(nw)*((z-4)*(nh) + (y-4)) + x-4], 
                                  ((nw+8)*(z*(nh+8) + y) + x)*1.f);
                }
            }
        }       
        for (z=nd; z < nd+4; ++z) {
            for (y=4; y < nh+4; ++y) {
                for (x=4; x < nw+4; ++x) {
                    ASSERT_FLOAT_EQ(tab_north[(nw)*((z-nd)*(nh) + (y-4)) + x-4], 
                                   ((nw+8)*(z*(nh+8) + y) + x)*1.f);
                }
            }
        }
        // X
        for (z=4; z < nd+4; ++z) {
            for (y=4; y < nh+4; ++y) {
                for (x=4; x < 8; ++x) {
                    //printf("%d %d %d\n", z,y,x);
                    ASSERT_FLOAT_EQ(tab_west[(4)*((z-4)*(nh) + (y-4)) + (x-4)], 
                                  ((nw+8)*(z*(nh+8) + y) + x)*1.f);
                }
            }
        }
        for (z=4; z < nd+4; ++z) {
            for (y=4; y < nh+4; ++y) {
                for (x=nw; x < nw+4; ++x) {
                    //printf("%d %d %d\n", z,y,x);
                    ASSERT_FLOAT_EQ(tab_east[4*((z-4)*(nh) + (y-4)) + (x-nw)],  
                                   ((nw+8)*(z*(nh+8) + y) + x)*1.f);
                }
            }
        }

        // Y
        for (z=4; z < nd+4; ++z) {
            for (y=4; y < 8; ++y) {
                for (x=4; x < nw+4; ++x) {
                    //printf("%d %d %d\n", z,y,x);
                    ASSERT_FLOAT_EQ(tab_back[nw*((z-4)*(4) + (y-4)) + x-4],  
                                    ((nw+8)*(z*(nh+8) + y) + x)*1.f);
                }
            }
        }
        for (z=4; z < nd+4; ++z) {
            for (y=nh; y < nh+4; ++y) {
                for (x=4; x < nw+4; ++x) {
                    //printf("%d %d %d\n", z,y,x);
                    ASSERT_FLOAT_EQ(tab_front[nw*((z-4)*(4) + (y-nh)) + x-4],  
                                    ((nw+8)*(z*(nh+8) + y) + x)*1.f);
                }
            }
        }
        hicl_knl_sync_run("kselect", d, buf, nw, nh, nd);
        hicl_timer_tick();
        hicl_mem_update(buf, READ_ONLY);
        fprintf(stdout, "... time full update: %f %s\n", 
                hicl_timer_read(), hicl_timer_uget());
        
        // Unpack
        g[0] = nw;
        g[1] = nh;
        hicl_knl_set_wrk("unpack_north", 2, g, l);
        hicl_knl_set_wrk("unpack_south", 2, g, l);
        g[0] = 4;
        g[1] = nh;
        l[0] = 4;
        l[1] = 64;
        hicl_knl_set_wrk("unpack_west", 2, g, l);
        hicl_knl_set_wrk("unpack_east", 2, g, l);
        g[0] = nw;
        g[1] = 4;
        l[0] = 64;
        l[1] = 4;
        hicl_knl_set_wrk("unpack_front", 2, g, l);
        hicl_knl_set_wrk("unpack_back",  2, g, l);

        hicl_knl_set_args("unpack_north", nw, nh, nd, cpy, tab_north);
        hicl_knl_set_args("unpack_south", nw, nh, nd, cpy, tab_south);
        hicl_knl_set_args("unpack_east",  nw, nh, nd, cpy, tab_east);
        hicl_knl_set_args("unpack_west",  nw, nh, nd, cpy, tab_west);
        hicl_knl_set_args("unpack_front", nw, nh, nd, cpy, tab_front);
        hicl_knl_set_args("unpack_back",  nw, nh, nd, cpy, tab_back);

        hicl_timer_tick();
        hicl_knl_exec("unpack_north", d);
        hicl_knl_exec("unpack_south", d);
        hicl_knl_exec("unpack_east",  d);
        hicl_knl_exec("unpack_west",  d);
        hicl_knl_exec("unpack_front", d);
        hicl_knl_exec("unpack_back",  d);
        hicl_dev_wait(d);
        fprintf(stdout, "... time unpack     : %f %s\n", 
                hicl_timer_read(), hicl_timer_uget());
        hicl_mem_update(cpy, READ_ONLY);
        // Z
        for (z=4; z < 8; ++z) {
            for (y=4; y < nh+4; ++y) {
                for (x=4; x < nw+4; ++x) {
                    ASSERT_FLOAT_EQ(cpy[(nw+8)*(z*(nh+8) + y) + x], 
                                  ((nw+8)*(z*(nh+8) + y) + x)*1.f);
                }
            }
        }
        for (z=nd; z < nd+4; ++z) {
            for (y=4; y < nh+4; ++y) {
                for (x=4; x < nw+4; ++x) {
                    ASSERT_FLOAT_EQ(cpy[(nw+8)*(z*(nh+8) + y) + x], 
                                   ((nw+8)*(z*(nh+8) + y) + x)*1.f);
                }
            }
        }
        // X
        for (z=4; z < nd+4; ++z) {
            for (y=4; y < nh+4; ++y) {
                for (x=4; x < 7; ++x) {
                    ASSERT_FLOAT_EQ(cpy[(nw+8)*(z*(nh+8) + y) + x], 
                                  ((nw+8)*(z*(nh+8) + y) + x)*1.f);
                }
            }
        }
        for (z=4; z < nd+4; ++z) {
            for (y=4; y < nh+4; ++y) {
                for (x=nw; x < nw+4; ++x) {
                    //printf("%d %d %d\n", z,y,x);
                    ASSERT_FLOAT_EQ(cpy[(nw+8)*(z*(nh+8) + y) + x], 
                                   ((nw+8)*(z*(nh+8) + y) + x)*1.f);
                }
            }
        }
        // Y
        for (z=4; z < nd+4; ++z) {
            for (y=4; y < 8; ++y) {
                for (x=4; x < nw+4; ++x) {
                    //printf("%d %d %d\n", z,y,x);
                    ASSERT_FLOAT_EQ(cpy[(nw+8)*(z*(nh+8) + y) + x], 
                                  ((nw+8)*(z*(nh+8) + y) + x)*1.f);
                }
            }
        }
        for (z=4; z < nd+4; ++z) {
            for (y=nh; y < nh+4; ++y) {
                for (x=4; x < nw+4; ++x) {
                    //printf("%d %d %d\n", z,y,x);
                    ASSERT_FLOAT_EQ(cpy[(nw+8)*(z*(nh+8) + y) + x], 
                                   ((nw+8)*(z*(nh+8) + y) + x)*1.f);
                }
            }
        }
        
        free(buf);
        free(cpy);
        free(tab_north);
        free(tab_south);
        free(tab_east);
        free(tab_west);
        free(tab_front);
        free(tab_back);
    }

    TEST_F(MemTest, pack_unpack_halos_using_kernels_zc) {
        unsigned int x, y, z, nw, nh, nd;
        float *buf, *cpy;
        float *tab_north;
        float *tab_south;
        float *tab_west;
        float *tab_east;
        float *tab_front;
        float *tab_back;
        int ret_val;
        size_t g[2], l[2];

        nw = 1024;
        nh = 1024;
        nd = 128;

        l[0]=16;
        l[1]=16;
        g[0]=nw;
        g[1]=nh;
        ret_val = posix_memalign((void**)(&buf), 
                       MEM_ALIGN, (nw+8)*(nh+8)*(nd+8)*sizeof(float));
        if (ret_val) {fprintf(stderr, "Failed to allocate memory\n"); FAIL();}
        ret_val = posix_memalign((void**)(&cpy), 
                       MEM_ALIGN, (nw+8)*(nh+8)*(nd+8)*sizeof(float));
        if (ret_val) {fprintf(stderr, "Failed to allocate memory\n"); FAIL();}
        ret_val = posix_memalign((void**)(&tab_north), 
                       MEM_ALIGN, (nw)*(nh)*(4)*sizeof(float));
        if (ret_val) {fprintf(stderr, "Failed to allocate memory\n"); FAIL();}
        ret_val = posix_memalign((void**)(&tab_south), 
                       MEM_ALIGN, (nw)*(nh)*(4)*sizeof(float));
        if (ret_val) {fprintf(stderr, "Failed to allocate memory\n"); FAIL();}
        ret_val = posix_memalign((void**)(&tab_west), 
                       MEM_ALIGN, (nd)*(nh)*(4)*sizeof(float));
        if (ret_val) {fprintf(stderr, "Failed to allocate memory\n"); FAIL();}
        ret_val = posix_memalign((void**)(&tab_east), 
                       MEM_ALIGN, (nd)*(nh)*(4)*sizeof(float));
        if (ret_val) {fprintf(stderr, "Failed to allocate memory\n"); FAIL();}
        ret_val = posix_memalign((void**)(&tab_front), 
                       MEM_ALIGN, (nd)*(nw)*(4)*sizeof(float));
        if (ret_val) {fprintf(stderr, "Failed to allocate memory\n"); FAIL();}
        ret_val = posix_memalign((void**)(&tab_back), 
                       MEM_ALIGN, (nd)*(nw)*(4)*sizeof(float));
        if (ret_val) {fprintf(stderr, "Failed to allocate memory\n"); FAIL();}

        hicl_mem_wrap(d, buf, (nw+8)*(nh+8)*(nd+8), 
                      CPU | ZERO_COPY | READ_WRITE);
        hicl_mem_wrap(d, cpy, (nw+8)*(nh+8)*(nd+8), 
                      CPU | ZERO_COPY | READ_WRITE);
        hicl_mem_wrap(d, tab_north, (nw)*(nh)*(4),  
                      CPU | ZERO_COPY | READ_WRITE);
        hicl_mem_wrap(d, tab_south, (nw)*(nh)*(4),  
                      CPU | ZERO_COPY | READ_WRITE);
        hicl_mem_wrap(d, tab_west,  (nd)*(nh)*(4),  
                      CPU | ZERO_COPY | READ_WRITE);
        hicl_mem_wrap(d, tab_east,  (nd)*(nh)*(4),  
                      CPU | ZERO_COPY | READ_WRITE);
        hicl_mem_wrap(d, tab_front, (nd)*(nw)*(4),  
                      CPU | ZERO_COPY | READ_WRITE);
        hicl_mem_wrap(d, tab_back,  (nd)*(nw)*(4),  
                      CPU | ZERO_COPY | READ_WRITE);
        
        hicl_load("data/select.cl", test_options);
        hicl_knl_set_wrk("kselect", 2, g, l);
        hicl_mem_update(buf, WRITE_ONLY);
        for (z=0; z < nd+8; ++z)
            for (y=0; y < nh+8; ++y)
                for (x=0; x < nw+8; ++x)
                    buf[(nw+8)*(z*(nh+8) + y) + x] = -1;                         
        for (z=4; z < 8; ++z) {
            for (y=4; y < nh+4; ++y) {
                for (x=4; x < nw+4; ++x) {
                    ASSERT_FLOAT_EQ(buf[(nw+8)*(z*(nh+8) + y) + x],-1*1.f);
                }
            }
        }
        hicl_knl_sync_run("kselect", d, buf, nw, nh, nd);

        // pack
        g[0] = nw;
        g[1] = nh;
        hicl_knl_set_wrk("pack_north", 2, g, l);
        hicl_knl_set_wrk("pack_south", 2, g, l);
        g[0] = 4;
        g[1] = nh;
        l[0] = 4;
        l[1] = 64;
        hicl_knl_set_wrk("pack_west", 2, g, l);
        hicl_knl_set_wrk("pack_east", 2, g, l);
        g[0] = nw;
        g[1] = 4;
        l[0] = 64;
        l[1] = 4;
        hicl_knl_set_wrk("pack_front", 2, g, l);
        hicl_knl_set_wrk("pack_back",  2, g, l);

        hicl_knl_set_args("pack_north", nw, nh, nd, buf, tab_north);
        hicl_knl_set_args("pack_south", nw, nh, nd, buf, tab_south);
        hicl_knl_set_args("pack_east",  nw, nh, nd, buf, tab_east);
        hicl_knl_set_args("pack_west",  nw, nh, nd, buf, tab_west);
        hicl_knl_set_args("pack_front", nw, nh, nd, buf, tab_front);
        hicl_knl_set_args("pack_back",  nw, nh, nd, buf, tab_back);

        hicl_timer_tick();
        hicl_knl_exec("pack_north", d);
        hicl_knl_exec("pack_south", d);
        hicl_knl_exec("pack_east",  d);
        hicl_knl_exec("pack_west",  d);
        hicl_knl_exec("pack_front", d);
        hicl_knl_exec("pack_back",  d);
        hicl_mem_update(tab_north, READ_ONLY);
        hicl_mem_update(tab_south, READ_ONLY);
        hicl_mem_update(tab_west,  READ_ONLY);
        hicl_mem_update(tab_east,  READ_ONLY);
        hicl_mem_update(tab_front, READ_ONLY);
        hicl_mem_update(tab_back,  READ_ONLY);
        fprintf(stdout, "... time pack       : %f %s\n", 
                hicl_timer_read(), hicl_timer_uget());
        // Z
        for (z=4; z < 8; ++z) {
            for (y=4; y < nh+4; ++y) {
                for (x=4; x < nw+4; ++x) {
                    ASSERT_FLOAT_EQ(tab_south[(nw)*((z-4)*(nh) + (y-4)) + x-4], 
                                  ((nw+8)*(z*(nh+8) + y) + x)*1.f);
                }
            }
        }       
        for (z=nd; z < nd+4; ++z) {
            for (y=4; y < nh+4; ++y) {
                for (x=4; x < nw+4; ++x) {
                    ASSERT_FLOAT_EQ(tab_north[(nw)*((z-nd)*(nh) + (y-4)) + x-4], 
                                   ((nw+8)*(z*(nh+8) + y) + x)*1.f);
                }
            }
        }
        // X
        for (z=4; z < nd+4; ++z) {
            for (y=4; y < nh+4; ++y) {
                for (x=4; x < 8; ++x) {
                    //printf("%d %d %d\n", z,y,x);
                    ASSERT_FLOAT_EQ(tab_west[(4)*((z-4)*(nh) + (y-4)) + (x-4)], 
                                  ((nw+8)*(z*(nh+8) + y) + x)*1.f);
                }
            }
        }
        for (z=4; z < nd+4; ++z) {
            for (y=4; y < nh+4; ++y) {
                for (x=nw; x < nw+4; ++x) {
                    //printf("%d %d %d\n", z,y,x);
                    ASSERT_FLOAT_EQ(tab_east[4*((z-4)*(nh) + (y-4)) + (x-nw)],  
                                   ((nw+8)*(z*(nh+8) + y) + x)*1.f);
                }
            }
        }

        // Y
        for (z=4; z < nd+4; ++z) {
            for (y=4; y < 8; ++y) {
                for (x=4; x < nw+4; ++x) {
                    //printf("%d %d %d\n", z,y,x);
                    ASSERT_FLOAT_EQ(tab_back[nw*((z-4)*(4) + (y-4)) + x-4],  
                                    ((nw+8)*(z*(nh+8) + y) + x)*1.f);
                }
            }
        }
        for (z=4; z < nd+4; ++z) {
            for (y=nh; y < nh+4; ++y) {
                for (x=4; x < nw+4; ++x) {
                    //printf("%d %d %d\n", z,y,x);
                    ASSERT_FLOAT_EQ(tab_front[nw*((z-4)*(4) + (y-nh)) + x-4],  
                                    ((nw+8)*(z*(nh+8) + y) + x)*1.f);
                }
            }
        }
        hicl_knl_sync_run("kselect", d, buf, nw, nh, nd);
        hicl_timer_tick();
        hicl_mem_update(buf, READ_ONLY);
        fprintf(stdout, "... time full update: %f %s\n", 
                hicl_timer_read(), hicl_timer_uget());
        
        // Unpack
        g[0] = nw;
        g[1] = nh;
        hicl_knl_set_wrk("unpack_north", 2, g, l);
        hicl_knl_set_wrk("unpack_south", 2, g, l);
        g[0] = 4;
        g[1] = nh;
        l[0] = 4;
        l[1] = 64;
        hicl_knl_set_wrk("unpack_west", 2, g, l);
        hicl_knl_set_wrk("unpack_east", 2, g, l);
        g[0] = nw;
        g[1] = 4;
        l[0] = 64;
        l[1] = 4;
        hicl_knl_set_wrk("unpack_front", 2, g, l);
        hicl_knl_set_wrk("unpack_back",  2, g, l);

        hicl_knl_set_args("unpack_north", nw, nh, nd, cpy, tab_north);
        hicl_knl_set_args("unpack_south", nw, nh, nd, cpy, tab_south);
        hicl_knl_set_args("unpack_east",  nw, nh, nd, cpy, tab_east);
        hicl_knl_set_args("unpack_west",  nw, nh, nd, cpy, tab_west);
        hicl_knl_set_args("unpack_front", nw, nh, nd, cpy, tab_front);
        hicl_knl_set_args("unpack_back",  nw, nh, nd, cpy, tab_back);

        hicl_timer_tick();
        hicl_knl_exec("unpack_north", d);
        hicl_knl_exec("unpack_south", d);
        hicl_knl_exec("unpack_east",  d);
        hicl_knl_exec("unpack_west",  d);
        hicl_knl_exec("unpack_front", d);
        hicl_knl_exec("unpack_back",  d);
        hicl_dev_wait(d);
        fprintf(stdout, "... time unpack     : %f %s\n", 
                hicl_timer_read(), hicl_timer_uget());
        hicl_mem_update(cpy, READ_ONLY);
        // Z
        for (z=4; z < 8; ++z) {
            for (y=4; y < nh+4; ++y) {
                for (x=4; x < nw+4; ++x) {
                    ASSERT_FLOAT_EQ(cpy[(nw+8)*(z*(nh+8) + y) + x], 
                                  ((nw+8)*(z*(nh+8) + y) + x)*1.f);
                }
            }
        }
        for (z=nd; z < nd+4; ++z) {
            for (y=4; y < nh+4; ++y) {
                for (x=4; x < nw+4; ++x) {
                    ASSERT_FLOAT_EQ(cpy[(nw+8)*(z*(nh+8) + y) + x], 
                                   ((nw+8)*(z*(nh+8) + y) + x)*1.f);
                }
            }
        }
        // X
        for (z=4; z < nd+4; ++z) {
            for (y=4; y < nh+4; ++y) {
                for (x=4; x < 7; ++x) {
                    ASSERT_FLOAT_EQ(cpy[(nw+8)*(z*(nh+8) + y) + x], 
                                  ((nw+8)*(z*(nh+8) + y) + x)*1.f);
                }
            }
        }
        for (z=4; z < nd+4; ++z) {
            for (y=4; y < nh+4; ++y) {
                for (x=nw; x < nw+4; ++x) {
                    //printf("%d %d %d\n", z,y,x);
                    ASSERT_FLOAT_EQ(cpy[(nw+8)*(z*(nh+8) + y) + x], 
                                   ((nw+8)*(z*(nh+8) + y) + x)*1.f);
                }
            }
        }
        // Y
        for (z=4; z < nd+4; ++z) {
            for (y=4; y < 8; ++y) {
                for (x=4; x < nw+4; ++x) {
                    //printf("%d %d %d\n", z,y,x);
                    ASSERT_FLOAT_EQ(cpy[(nw+8)*(z*(nh+8) + y) + x], 
                                  ((nw+8)*(z*(nh+8) + y) + x)*1.f);
                }
            }
        }
        for (z=4; z < nd+4; ++z) {
            for (y=nh; y < nh+4; ++y) {
                for (x=4; x < nw+4; ++x) {
                    //printf("%d %d %d\n", z,y,x);
                    ASSERT_FLOAT_EQ(cpy[(nw+8)*(z*(nh+8) + y) + x], 
                                   ((nw+8)*(z*(nh+8) + y) + x)*1.f);
                }
            }
        }
        
        free(buf);
        free(cpy);
        free(tab_north);
        free(tab_south);
        free(tab_east);
        free(tab_west);
        free(tab_front);
        free(tab_back);
    }

}  // namespace