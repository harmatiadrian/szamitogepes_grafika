#ifndef MATRIX_H
#define MATRIX_H

/**
 * Initializes all elements of the matrix to zero.
 */
void init_zero_matrix(float matrix[3][3]);

/**
 * Print the elements of the matrix.
 */
void print_matrix(const float matrix[3][3]);

/**
 * Add matrices.
 */
void add_matrices(const float a[3][3], const float b[3][3], float c[3][3]);

//feladatmegoldas:

/**
 * Initializes the identity matrix 
 */
void init_identity_matrix(int height, int width, float identity_matrix[height][width]);

/**
 * mátrix skalárral való szorzásához
 */
void matrix_scalar_szorzas(int height, int width, float matrix[height][width],int num);

/**
 * multiplying matrices with requirements test returns 0 if successsful -1 if not
 */
int multiply_matrices(int heigth1, int width1, float matrix1[heigth1][width1],int heigth2, int width2, float matrix2[heigth2][width2], float return_martix[heigth1][width2]);

/**
 * függvény, amelyik egy síkbeli, homogén koordinátákkal adott pontra végzi el a paraméterben kapott mátrix által leírt transzformációt
 */
void transform_point(float point[3][1], float transformMartrix[3][3]);

/**
 * type is 0 if int; 1 if float.
 */
void print_anySizeMatrix(int heigth, int width, void* matrix[heigth][width], int type);
#endif // MATRIX_H

