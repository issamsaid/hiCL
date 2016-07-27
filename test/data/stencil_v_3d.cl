#define G(m, z, y, x) m[(2+dimx)*((2*sy+dimy)*(z+sz) + y+sy) + x+1]

kernel void stencil_v_3d(global float4  *input,
                         global float4 *output,
                         constant float *coefx,
                         constant float *coefy,
                         constant float *coefz,
                         int              dimx, 
                         int              dimy, 
                         int              dimz,
                         int                sx,   
                         int                sy, 
                         int                sz) {
    int xgid  = get_global_id(0);
    int ygid  = get_global_id(1);
    int zgid;
    float4 v, l, r;
    dimx = dimx>>2;	
    
    float4 f0, f1, f2, f3;
    float4 b0, b1, b2, b3; 
    float4 current;
    float  coef0 = coefx[0] + coefy[0] + coefz[0];
     
    if( (xgid<dimx) && (ygid<dimy) ) {

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
	    
            r = G(input, zgid, ygid, xgid+1);
            l = G(input, zgid, ygid, xgid-1);		
	    
            v  = coef0 * current;
            v += coefx[1] * ( (float4)(current.yzw, r.x)   + 
                              (float4)(l.w,   current.xyz ));
            v += coefx[2] * ( (float4)(current.zw,  r.xy)  + 
                              (float4)(l.zw,  current.xy  ));
            v += coefx[3] * ( (float4)(current.w,   r.xyz) + 
                              (float4)(l.yzw, current.x   ));
            v += coefx[4] * ( r + l );
	    
            v += coefy[1] * ( G(input, zgid, ygid+1, xgid) + 
                              G(input, zgid, ygid-1, xgid) );
            v += coefy[2] * ( G(input, zgid, ygid+2, xgid) + 
                              G(input, zgid, ygid-2, xgid) );
            v += coefy[3] * ( G(input, zgid, ygid+3, xgid) + 
                              G(input, zgid, ygid-3, xgid) );
            v += coefy[4] * ( G(input, zgid, ygid+4, xgid) + 
                              G(input, zgid, ygid-4, xgid) );
	    
            v += coefz[1] * ( f0 + b0 );
            v += coefz[2] * ( f1 + b1 );
            v += coefz[3] * ( f2 + b2 );
            v += coefz[4] * ( f3 + b3 );
	    
            G(output, zgid, ygid, xgid) = current + v;
        }
    }
}