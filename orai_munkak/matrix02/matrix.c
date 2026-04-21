#include "matrix.h"

#include <stdio.h>

void init_zero_matrix(float matrix[3][3])
{
    int i;
    int j;

    for (i = 0; i < 3; ++i) {
        for (j = 0; j < 3; ++j) {
            matrix[i][j] = 0.0;
        }
    }
}

void print_matrix(const float matrix[3][3])
{
    int i;
    int j;

    for (i = 0; i < 3; ++i) {
        for (j = 0; j < 3; ++j) {
            printf("%4.4f ", matrix[i][j]);
        }
        printf("\n");
    }
}

void add_matrices(const float a[3][3], const float b[3][3], float c[3][3])
{
    int i;
    int j;

    for (i = 0; i < 3; ++i) {
        for (j = 0; j < 3; ++j) {
            c[i][j] = a[i][j] + b[i][j];
        }
    }
}

//feladatmegoldas:

void init_identity_matrix(int height,int width, float identity_matrix[height][width])
{
    int i;
    int j;

    for (i = 0; i < height; i++)
    {
        for (j = 0; j < width; j++)
        {
            if (i==j)
            {
                identity_matrix[i][j] = 1.0;
            }
            else
            {
                identity_matrix[i][j] = 0.0;
            }
        }
    }
    
}
void matrix_scalar_szorzas(int height, int width, float matrix[height][width],int num)
{
    int i;
    int j;

    for ( i = 0; i < height; i++)
    {
        for ( j = 0; j < width; j++)
        {
            matrix[i][j] *= num;
        }
        
    }
    
}
int multiply_matrices(int heigth1, int width1, float matrix1[heigth1][width1],int heigth2, int width2, float matrix2[heigth2][width2], float return_martix[heigth1][width2])
{
    int i;
    int j;
    int k;

    if (width1 != heigth2)
    {
        printf("Error matrix multiplying not possible!");
        return -1;
    }else
    {
        for ( i = 0; i < heigth1; i++)
        {
            for ( j = 0; j < width2; j++)
            {
                float seged = 0.0;
                for ( k = 0; k < width1; k++)
                {
                    
                    seged += matrix1[i][k]*matrix2[k][j];
                    //printf("teszt %f;%f = %f (matrix1[%d][%d]*matrix2[%d][%d]) \n",matrix1[i][k],matrix2[k][j],seged,i,k,k,j);
                }
                
                return_martix[i][j] = seged;
            }
            
        }
        
    }
    
    return 0;
}

void transform_point(float point[3][1], float transformMartrix[3][3])
{
    int i,j;

    multiply_matrices(3,3,transformMartrix,3,1,point,point);
    
}

void print_anySizeMatrix(int heigth, int width, void* matrix[heigth][width], int type)
{
    //printf("teszt: \n");
    int i;
    int j;


    if (type == 0)
    {
        for (i = 0; i < heigth; ++i) {
            for (j = 0; j < width; ++j) {
                printf("%d ",(int)matrix[i][j]);
            }
            printf("\n");
        }
    }
    else if (type == 1)
    {
        float **seged = (float**)&matrix;
        for (i = 0; i < heigth; ++i) {
            for (j = 0; j < width; ++j) {
                printf("%4.4f ",seged[i][j]);
            }
            printf("\n");
        }
    }
    else
    {
        printf("Error: type must be ether 0 or 1 (unknown matrix type)!");
    }
    
}

