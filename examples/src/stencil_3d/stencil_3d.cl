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
/// @file stencil_3d/stencil_3d.cl
/// @author Issam SAID
/// @brief he OpenCL kernel used for the 3D stencil computations based on hiCL.
///
#define G(m, z, y, x) m[(8+dimx)*((8+dimy)*(z+4) + y+4) + x+4]

kernel void stencil_3d(global float    *input, 
                       global float   *output,
                       constant float *coeffs,
                       int               dimx, 
                       int               dimy, 
                       int               dimz) {
    int xgid = get_global_id(0);
    int ygid = get_global_id(1);
    int zgid;
    
    // laplacian operator
    float laplacian;
    float current;	
    float b00;
    float b01;
    float b02;
    float b03;
    float f00;
    float f01;
    float f02;
    float f03;
    float coef = 3*coeffs[0];

       
    if( (xgid<dimx) && (ygid<dimy) ) { 
        
        b03 = 0.;
        b02 = G(input, -4, ygid, xgid);
        b01 = G(input, -3, ygid, xgid);
        b00 = G(input, -2, ygid, xgid);
        current  = G(input, -1, ygid, xgid);
        f00  = G(input,  0, ygid, xgid);
        f01  = G(input,  1, ygid, xgid);
        f02  = G(input,  2, ygid, xgid);
        f03  = G(input,  3, ygid, xgid);
     
        for(zgid = 0; zgid < dimz; zgid++) {
            b03 = b02;
            b02 = b01;
            b01 = b00;
            b00 = current;
            current  = f00;
            f00  = f01;
            f01  = f02;
            f02  = f03;
            f03  = G(input, (zgid+4), ygid, xgid);

            laplacian  = coef * current
                + coeffs[1] * ( G(input, zgid, ygid, xgid+1) +
                               G(input, zgid, ygid, xgid-1) )
                + coeffs[2] * ( G(input, zgid, ygid, xgid+2) +
                               G(input, zgid, ygid, xgid-2) )
                + coeffs[3] * ( G(input, zgid, ygid, xgid+3) +
                               G(input, zgid, ygid, xgid-3) )
                + coeffs[4] * ( G(input, zgid, ygid, xgid+4) +
                               G(input, zgid, ygid, xgid-4) )

                + coeffs[1] * ( G(input, zgid, ygid+1, xgid) +
                               G(input, zgid, ygid-1, xgid) )
                + coeffs[2] * ( G(input, zgid, ygid+2, xgid) +
                               G(input, zgid, ygid-2, xgid) )
                + coeffs[3] * ( G(input, zgid, ygid+3, xgid) +
                               G(input, zgid, ygid-3, xgid) )
                + coeffs[4] * ( G(input, zgid, ygid+4, xgid) +
                               G(input, zgid, ygid-4, xgid) )

                + coeffs[1] * ( f00 + b00 )
                + coeffs[2] * ( f01 + b01 )
                + coeffs[3] * ( f02 + b02 )
                + coeffs[4] * ( f03 + b03 );
            G(output, zgid, ygid, xgid) = current + laplacian;
        }
    }
}
