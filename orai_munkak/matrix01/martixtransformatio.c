#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

static double Imatrix3x3[3][3] = {{1,0,0},{0,1,0},{0,0,1}};
//void ellenorzott_matrix_bekeres(char* teszt[], double* matrix[][]);
void eltolas(double x, double y, double matrix_input[3][3],double matrix_output[3][3]);
//void matrix3x3_Cloneing(double* matrix1[3][3],double* matrix3[3][3]);
void matrix3x3_kiir(double matrix[3][3]);

int main()
{
    double matrix[3][3];
    eltolas(1,6,Imatrix3x3,matrix);
    matrix3x3_kiir(matrix);

}
void eltolas(double x, double y, double matrix_input[3][3],double matrix_output[3][3])
{
    
    for (size_t i = 0; i < 3; i++)
    {
        for (size_t j = 0; j < 3; j++)
        {
            if ((i == 0) && (j == 2))
            {
                matrix_output[i][j] = (Imatrix3x3[i][j] + x);
                //printf("1");

            }else if ((i == 1) && (j == 2))
            {
                matrix_output[i][j] = (Imatrix3x3[i][j] + y);
                //printf("2");
            }else
            {
                matrix_output[i][j] = matrix_input[i][j];
                //printf("3");
            }
        }
        
    }
    /*
    printf("\n");
    printf("A matrix: \n");
    for (size_t i = 0; i < 3; i++)
    {
        for (size_t j = 0; j < 3; j++)
        {
            printf(" %d ",matrix_output[i][j]);
        }
        printf("\n");
    }
    printf("\n");
    
    
    for (size_t i = 0; i < 3; i++)
    {
        for (size_t j = 0; j < 3; j++)
        {
            matrix_output[i][j] = matrix_input[i][j];
        }
        
    }

    matrix_output[0][2] += x;
    matrix_output[1][2] += y;
    */
}
/*void matrix3x3_Cloneing(double* masolando_matrix[3][3],double* eredmeny_martix[3][3])
{
    for (size_t i = 0; i < 3; i++)
    {
        for (size_t j = 0; j < 3; j++)
        {
            *masolando_matrix[i][j] = *eredmeny_martix[i][j];
        }
        
    }
    
}*/
void matrix3x3_kiir(double matrix[3][3])
{
    printf("A matrix: \n");
    for (size_t i = 0; i < 3; i++)
    {
        for (size_t j = 0; j < 3; j++)
        {
            printf(" %.3f ",matrix[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}