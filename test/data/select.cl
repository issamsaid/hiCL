#define S(m, z, y, x) m[(8+dimx)*((8+dimy)*(z+4) + y+4) + x+4]

kernel void kselect(global float   *input, 
                   int              dimx, 
                   int              dimy, 
                   int              dimz) {
    int xgid = get_global_id(0);
    int ygid = get_global_id(1);
    int zgid;
    // Z
    if( (xgid<dimx) && (ygid<dimy) ) { 
        for(zgid = 0; zgid < 4; ++zgid) {
            S(input, zgid, ygid, xgid) = (8+dimx)*((8+dimy)*(zgid+4) + (ygid+4)) + xgid+4;
        }
        for(zgid = dimz-4; zgid < dimz; ++zgid) {
            S(input, zgid, ygid, xgid) = (8+dimx)*((8+dimy)*(zgid+4) + (ygid+4)) + xgid+4;
        }
    }
    // X
    if(( (xgid >= 0) && (xgid<4) ) || ( (xgid >= (dimx-4)) && (xgid<dimx) )) { 
        for(zgid = 0; zgid < dimz; ++zgid) {
            S(input, zgid, ygid, xgid) = (8+dimx)*((8+dimy)*(zgid+4) + (ygid+4)) + xgid+4;
        }
    }
    // Y
    if(( (ygid >= 0) && (ygid<4) ) || ( (ygid >= (dimy-4)) && (ygid<dimy) )) { 
        for(zgid = 0; zgid < dimz; ++zgid) {
            S(input, zgid, ygid, xgid) = (8+dimx)*((8+dimy)*(zgid+4) + (ygid+4)) + xgid+4;
        }
    }

}

#define STENCIL 4
#define ZRO     4
#define ONE     5
#define TWO     6
#define TRE     7
#define FOR     8
#define   U0(z, y, x)    u0[(2*STENCIL+dimx)*((2*STENCIL+dimy)*(z+STENCIL) + (y+STENCIL)) + x+STENCIL]
#define   U1(z, y, x)    u1[(2*STENCIL+dimx)*((2*STENCIL+dimy)*(z+STENCIL) + y+STENCIL) + x+STENCIL]
#define ROC2(z, y, x)  roc2[dimx*(dimy*(z) + y ) + x] 
#define  ETA(z, y, x)   eta[(dimx+2)*((dimy+2)*(z+1) + (y+1) ) + (x+1)]
#define  PHI(z, y, x)   phi[dimx*(dimy*(z) + y ) + x]
#define    L(y, x)        l[y+STENCIL][x+STENCIL]

//
// global size should be dimx x dimy
// pack dimx * dimy * lz entries
// tab[STENCIL][dimy][dimx]
//
kernel void pack_south(int         dimx, 
                       int         dimy, 
                       int         dimz,
                       global float *u0,
                       global float *tab) {
    int xgid = get_global_id(0);
    int ygid = get_global_id(1);
    int zgid;
    
    if ((xgid<dimx) && (ygid<dimy)) { 
        for(zgid = 0; zgid < STENCIL; ++zgid) {
            tab[xgid + dimx*(ygid+(zgid*dimy))] = U0(zgid, ygid, xgid);
        }
    }
}

//
// global size should be dimx x dimy
// unpack dimx * dimy * lz entries
// tab[STENCIL][dimy][dimx]
//
kernel void unpack_south(int         dimx, 
                         int         dimy, 
                         int         dimz,
                         global float *u0,
                         global float *tab) {
    int xgid = get_global_id(0);
    int ygid = get_global_id(1);
    int zgid;
    
    if ((xgid<dimx) && (ygid<dimy)) { 
        for(zgid = 0; zgid < STENCIL; ++zgid) {
            U0(zgid, ygid, xgid) = tab[xgid + dimx*(ygid+(zgid*dimy))];
        }
    }
}

//
// global size should be dimx x dimy
// pack dimx * dimy * lz entries
// tab[STENCIL][dimy][dimx]
//
kernel void pack_north(int         dimx, 
                       int         dimy, 
                       int         dimz,
                       global float *u0,
                       global float *tab) {
    int xgid = get_global_id(0);
    int ygid = get_global_id(1);
    int zgid;
    
    if ((xgid<dimx) && (ygid<dimy)) { 
        for(zgid = dimz-STENCIL; zgid < dimz; ++zgid) {
            tab[xgid + dimx*(ygid+((zgid-dimz+STENCIL)*dimy))] = U0(zgid, ygid, xgid);
        }
    }
}

//
// global size should be dimx x dimy
// unpack dimx * dimy * lz entries
// tab[STENCIL][dimy][dimx]
//
kernel void unpack_north(int         dimx, 
                       int         dimy, 
                       int         dimz,
                       global float *u0,
                       global float *tab) {
    int xgid = get_global_id(0);
    int ygid = get_global_id(1);
    int zgid;
    
    if ((xgid<dimx) && (ygid<dimy)) { 
        for(zgid = dimz-STENCIL; zgid < dimz; ++zgid) {
            U0(zgid, ygid, xgid) = tab[xgid + dimx*(ygid+((zgid-dimz+STENCIL)*dimy))];
        }
    }
}

//
// global size should be STENCIL x dimy
// pack STENCIL * dimy * dimz entries
// tab[dimz][dimy][STENCIL]
//
kernel void pack_west(int         dimx, 
                      int         dimy, 
                      int         dimz,
                      global float *u0,
                      global float *tab) {
    int xgid = get_global_id(0);
    int ygid = get_global_id(1);
    int zgid;
    if((xgid >= 0) && (xgid<STENCIL)) { 
        for(zgid = 0; zgid < dimz; ++zgid) {
            tab[xgid + STENCIL*(ygid+(zgid*dimy))] = U0(zgid, ygid, xgid);
        }
    }
}

//
// global size should be STENCIL x dimy
// unpack STENCIL * dimy * dimz entries
// tab[dimz][dimy][STENCIL]
//
kernel void unpack_west(int         dimx, 
                        int         dimy, 
                        int         dimz,
                        global float *u0,
                        global float *tab) {
    int xgid = get_global_id(0);
    int ygid = get_global_id(1);
    int zgid;
    if((xgid >= 0) && (xgid<STENCIL)) { 
        for(zgid = 0; zgid < dimz; ++zgid) {
            U0(zgid, ygid, xgid) = tab[xgid + STENCIL*(ygid+(zgid*dimy))];
        }
    }
}

//
// global size should be STENCIL x dimy
// pack STENCIL * dimy * dimz entries
// tab[dimz][dimy][STENCIL]
//
kernel void pack_east(int         dimx, 
                      int         dimy, 
                      int         dimz,
                      global float *u0,
                      global float *tab) {
    int xgid = get_global_id(0);
    int ygid = get_global_id(1);
    int zgid;
    if((xgid>=0) && (xgid<4)) { 
        for(zgid = 0; zgid < dimz; ++zgid) {
            tab[xgid + STENCIL*(ygid+(zgid*dimy))] = U0(zgid, ygid, xgid+dimx-STENCIL);
        }
    }
}

//
// global size should be STENCIL x dimy
// unpack STENCIL * dimy * dimz entries
// tab[dimz][dimy][STENCIL]
//
kernel void unpack_east(int         dimx, 
                        int         dimy, 
                        int         dimz,
                        global float *u0,
                        global float *tab) {
    int xgid = get_global_id(0);
    int ygid = get_global_id(1);
    int zgid;
    if((xgid>=0) && (xgid<4)) { 
        for(zgid = 0; zgid < dimz; ++zgid) {
            U0(zgid, ygid, xgid+dimx-STENCIL) = tab[xgid + STENCIL*(ygid+(zgid*dimy))];
        }
    }
}

//
// global size should be dimx x STENCIL
// pack dimx * ly * dimz entries
// tab[dimz][STENCIL][dimx]
//
kernel void pack_front(int         dimx, 
                       int         dimy, 
                       int         dimz,
                       global float *u0,
                       global float *tab) {
    int xgid = get_global_id(0);
    int ygid = get_global_id(1);
    int zgid;
    if((ygid >= 0) && (ygid<STENCIL)) { 
        for(zgid = 0; zgid < dimz; ++zgid) {
            tab[xgid + dimx*(ygid+(zgid*STENCIL))] = U0(zgid, ygid+dimy-STENCIL, xgid);
        }
    }
}

//
// global size should be dimx x STENCIL
// unpack dimx * ly * dimz entries
// tab[dimz][STENCIL][dimx]
//
kernel void unpack_front(int         dimx, 
                         int         dimy, 
                         int         dimz,
                         global float *u0,
                         global float *tab) {
    int xgid = get_global_id(0);
    int ygid = get_global_id(1);
    int zgid;
    if((ygid >= 0) && (ygid<STENCIL)) { 
        for(zgid = 0; zgid < dimz; ++zgid) {
            U0(zgid, ygid+dimy-STENCIL, xgid) = tab[xgid + dimx*(ygid+(zgid*STENCIL))];
        }
    }
}

//
// global size should be dimx x STENCIL
// pack dimx * ly * dimz entries
// tab[dimz][STENCIL][dimx]
//
kernel void pack_back(int         dimx, 
                      int         dimy, 
                      int         dimz,
                      global float *u0,
                      global float *tab) {
    int xgid = get_global_id(0);
    int ygid = get_global_id(1);
    int zgid;
    if((ygid >= 0) && (ygid<STENCIL)) { 
        for(zgid = 0; zgid < dimz; ++zgid) {
            tab[xgid + dimx*(ygid+(zgid*STENCIL))] = U0(zgid, ygid, xgid);
        }
    }
}

//
// global size should be dimx x STENCIL
// unpack dimx * ly * dimz entries
// tab[dimz][STENCIL][dimx]
//
kernel void unpack_back(int         dimx, 
                        int         dimy, 
                        int         dimz,
                        global float *u0,
                        global float *tab) {
    int xgid = get_global_id(0);
    int ygid = get_global_id(1);
    int zgid;
    if((ygid >= 0) && (ygid<STENCIL)) { 
        for(zgid = 0; zgid < dimz; ++zgid) {
            U0(zgid, ygid, xgid) = tab[xgid + dimx*(ygid+(zgid*STENCIL))];
        }
    }
}
