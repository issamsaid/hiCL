kernel void test_hicl_4(global   float4 *input, 
                          global   float4 *output,
                          constant float *coefx,
                          constant float *coefy,
                          constant float *coefz,
                          int dimx, int dimy, int dimz,
                          int   sx,   int sy,   int sz) {
    int s   = STENCIL;
    int gid = get_global_id(0) + s;
}


kernel void test_hicl_5(global   float4 *input, 
                          global   float4 *output,
                          constant float *coefx,
                          constant float *coefy,
                          constant float *coefz,
                          int dimx, int dimy, int dimz) {
    int gid = get_global_id(0);
}

kernel void test_hicl_6(global   float4 *input, 
                          global   float4 *output,
                          int dimx, int dimy, int dimz) {
    int gid = get_global_id(0);
}
