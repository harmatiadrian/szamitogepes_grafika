#include "matrix.h"

int main(int argc, char* argv[])
{
	float a[3][3] = {
        { 1.0f, -2.0f,  3.0f},
        { 5.0f, -3.0f,  0.0f},
        {-2.0f,  1.0f, -4.0f}
    };
	float b[3][3];
	float c[3][3];

    init_zero_matrix(b);
    b[1][1] =  8.0f;
    b[2][0] = -3.0f;
    b[2][2] =  5.0f;

    print_matrix(a);
    print_matrix(b);
    
    add_matrices(a, b, c);
    
    print_matrix(c);
    /* //for teszt
    float d[3][3];
    init_identity_matrix(3,3,d);
    print_matrix(d);
    
    float e[3][3] = 
    {
        {-3.1,2.2,4.0},
        {2.0,8.0,-10.1},
        {12.2,2.0,6.3}
    };
    matrix_scalar_szorzas(3,3,e,10);
    print_matrix(e);
    float f[3][3];
    multiply_matrices(3,3,a,3,3,e,f);
    print_matrix(f);
    */

    float point[3][1] = {3,3,1};
    float transzform_maratrix[3][3] =
    {
        {1,0,2},
        {0,1,3},
        {0,0,1}
    };
    transform_point(point,transzform_maratrix);
    //printf("teszt: \n");
    print_anySizeMatrix(1,3,(void*)point,1);
	return 0;
}

