#define INDEX_OPTIM
#ifdef INDEX_OPTIM
#define G(m, z, y, x) m[(2+dimx)*((2*STENCIL+dimy)*(z) + y) + x]
#define L(m, y, x)    m[y][x]
#else
#define G(m, z, y, x) m[(2+dimx)*((2*sy+dimy)*(z+sz) + y+sy) + x+1]
#define L(m, y, x)    m[y+sy][x+1]
#endif

kernel void stencil_lv_3d(global   float4 *input, 
                          global   float4 *output,
                          constant float *coefx,
                          constant float *coefy,
                          constant float *coefz,
                          int dimx, int dimy, int dimz,
                          int   sx,   int sy,   int sz) {
#ifdef INDEX_OPTIM
    int xgid     = get_global_id(0)+1;
    int ygid     = get_global_id(1)+STENCIL;
    
    float coef0 = coefx[0] + coefy[0] + coefz[0];

    dimx = dimx>>2;	

    if( (xgid<(dimx+1)) && (ygid<(dimy+STENCIL)) ) { 
        int zgid;   
        int xlid     = get_local_id(0)+1;
        int ylid     = get_local_id(1)+STENCIL;
        float4 v, a, r;

        float4 f0, f1, f2, f3;
        float4 b0, b1, b2, b3; 
        float4 current;
        local float4 l[LY + (2*STENCIL)][LX + 2]; 

        b3      = 0.;
        b2      = G(input, 0, ygid, xgid);
        b1      = G(input, 1, ygid, xgid);
        b0      = G(input, 2, ygid, xgid);
        current = G(input, 3, ygid, xgid);
        f0      = G(input, 4, ygid, xgid);
        f1      = G(input, 5, ygid, xgid);
        f2      = G(input, 6, ygid, xgid);
        f3      = G(input, 7, ygid, xgid);
        
        for(zgid=STENCIL ;zgid < (dimz+STENCIL); ++zgid) {
            b3 = b2;
            b2 = b1;
            b1 = b0;
            b0 = current;
            current= f0;
            f0 = f1;
            f1 = f2;
            f2 = f3;
            f3 = G(input, zgid+STENCIL, ygid, xgid);
            
            if(xlid < 2) {
                L(l, ylid, xlid-1) = G(input, zgid, ygid, xgid-1);
            }
            if((xlid >= LX) || (xgid >= dimx)) {
                L(l, ylid, xlid+1) = G(input, zgid, ygid, xgid+1);
            }
            if(ylid < 2*STENCIL) {
                L(l, ylid-sy, xlid) = G(input, zgid, ygid-sy, xgid);
            }
            if((ylid >= LY) || (ygid >= dimy)) {
                L(l, ylid+sy, xlid) = G(input, zgid, ygid+sy, xgid);
            }
            L(l, ylid, xlid) = current;
            
            barrier(CLK_LOCAL_MEM_FENCE); 
            
            r = L(l, ylid, xlid+1);
            a = L(l, ylid, xlid-1);	

            v  = coef0 * current;
            v += coefx[1] * ( (float4)(current.yzw, r.x)   + (float4)(a.w,   current.xyz ));
            v += coefx[2] * ( (float4)(current.zw,  r.xy)  + (float4)(a.zw,  current.xy  ));
            v += coefx[3] * ( (float4)(current.w,   r.xyz) + (float4)(a.yzw, current.x   ));
            v += coefx[4] * ( r                            +                             a);
            
            v += coefy[1] * ( L(l, ylid+1, xlid) + L(l, ylid-1, xlid) );
            v += coefy[2] * ( L(l, ylid+2, xlid) + L(l, ylid-2, xlid) );
            v += coefy[3] * ( L(l, ylid+3, xlid) + L(l, ylid-3, xlid) );
            v += coefy[4] * ( L(l, ylid+4, xlid) + L(l, ylid-4, xlid) );
            
            v += coefz[1] * ( f0 + b0 );
            v += coefz[2] * ( f1 + b1 );
            v += coefz[3] * ( f2 + b2 );
            v += coefz[4] * ( f3 + b3 );
	    
            G(output, zgid, ygid, xgid) = current + v;
        }
    }
#else
    int xgid     = get_global_id(0);
    int ygid     = get_global_id(1);
    dimx = dimx>>2;

    if( (xgid<dimx) && (ygid<dimy) ) {
        int zgid;        
        int xlid     = get_local_id(0);
        int ylid     = get_local_id(1);
        float4 v, a, r;
        
        float4 f0, f1, f2, f3;
        float4 b0, b1, b2, b3; 
        float4 current;
        local float4 l[LY + (2*STENCIL)][LX + 2]; 

        b3      = 0.;
        b2      = G(input, -4, ygid, xgid);
        b1      = G(input, -3, ygid, xgid);
        b0      = G(input, -2, ygid, xgid);
        current = G(input, -1, ygid, xgid);
        f0      = G(input,  0, ygid, xgid);
        f1      = G(input,  1, ygid, xgid);
        f2      = G(input,  2, ygid, xgid);
        f3      = G(input,  3, ygid, xgid);
        
        for(zgid=0 ;zgid < dimz; ++zgid) {
            b3 = b2;
            b2 = b1;
            b1 = b0;
            b0 = current;
            current= f0;
            f0 = f1;
            f1 = f2;
            f2 = f3;
            f3 = G(input, zgid+sz, ygid, xgid);
            
            if(xlid < 1)
                L(l, ylid, xlid-1) = G(input, zgid, ygid, xgid-1);
            
            if((xlid >= (LX-1)) || (xgid >= (dimx-1)))
                L(l, ylid, xlid+1) = G(input, zgid, ygid, xgid+1);
            
            if(ylid < sy)
                L(l, ylid-sy, xlid) = G(input, zgid, ygid-sy, xgid);
            
            if((ylid >= (LY-sy)) || (ygid >= (dimy-sy)))
                L(l, ylid+sy, xlid) = G(input, zgid, ygid+sy, xgid);
            
            L(l, ylid, xlid) = current;
            
            barrier(CLK_LOCAL_MEM_FENCE); 
            
            r = L(l, ylid, xlid+1);
            a = L(l, ylid, xlid-1);	

            v  = coef0 * current;
            v += coef1 * ( (float4)(current.yzw, r.x)   + (float4)(a.w,   current.xyz ));
            v += coef2 * ( (float4)(current.zw,  r.xy)  + (float4)(a.zw,  current.xy  ));
            v += coef3 * ( (float4)(current.w,   r.xyz) + (float4)(a.yzw, current.x   ));
            v += coef4 * ( r                            +                             a);
            
            v += coef1 * ( L(l, ylid+1, xlid) + L(l, ylid-1, xlid) );
            v += coef2 * ( L(l, ylid+2, xlid) + L(l, ylid-2, xlid) );
            v += coef3 * ( L(l, ylid+3, xlid) + L(l, ylid-3, xlid) );
            v += coef4 * ( L(l, ylid+4, xlid) + L(l, ylid-4, xlid) );
            
            v += coef1 * ( f0 + b0 );
            v += coef2 * ( f1 + b1 );
            v += coef3 * ( f2 + b2 );
            v += coef4 * ( f3 + b3 );
	    
            G(output, zgid, ygid, xgid) = current + v;
        }
    }
#endif
}
