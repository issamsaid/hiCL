#define   W(z, x) u[(2*lx+dimx)*(z+lz) + x+lx]

kernel void add_source_2d(int ix, int iz,
                          int ix_p, int iz_p, 
                          float rxt, float rzt, 
                          int dimx,                         
                          int lx, int lz, 
                          global float* source,
                          int it, global float* u) {
    float src = source[it];
    W(iz, ix)     +=  src*(1.f-rxt)*(1.f-rzt);
    W(iz_p, ix)   +=  src*rzt*(1.f-rxt);
    W(iz,   ix_p) +=  src*(1.f-rzt)*rxt;
    W(iz_p, ix_p) +=  src*rzt*rxt;
}
