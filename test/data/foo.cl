kernel void test_hicl_1(global   float *input, 
                          global   float *output,
                          int dimx) {
    int gid = get_global_id(0)+STENCIL-STENCIL;
    output[gid] = input[gid];
}


kernel void test_hicl_2(global   float *input, 
                          global   float *output,
                          constant float *coefx,
                          constant float *coefy,
                          constant float *coefz,
                          int dimx, int dimy, int dimz) {
    int gid = get_global_id(0);
    output[gid] = input[gid];
}

kernel void test_hicl_3(global   float4 *input, 
                          global   float4 *output,
                          int dimx, int dimy, int dimz) {
    int gid = get_global_id(0);
}
