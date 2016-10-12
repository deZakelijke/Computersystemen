/*
 * trans.c - Matrix transpose B = A^T
 * Author: Eelco van der Wel, 10670033
           Micha de Groot, 10434410
 *
 * Each transpose function must have a prototype of the form:
 * void trans(int M, int N, int A[N][M], int B[M][N]);
 *
 * A transpose function is evaluated by counting the number of misses
 * on a 1KB direct mapped cache with a block size of 32 bytes.
 */
#include <stdio.h>
#include "cachelab.h"

int is_transpose(int M, int N, int A[N][M], int B[M][N]);

/*
 * transpose_submit - This is the solution transpose function that you
 *     will be graded on for Part B of the assignment. Do not change
 *     the description string "Transpose submission", as the driver
 *     searches for that string to identify the transpose function to
 *     be graded.
 */
char transpose_submit_desc[] = "Transpose submission";
void transpose_submit(int M, int N, int A[N][M], int B[M][N])
{
	// User written comments are inside the function, to seperate from comments that
	// were already present.
    //
	// This function contains the transpose functions for the three different matrix sizes.
	// Each case is a quadruple for loop with essentially the same code. An exception
	// is the last, uneven sized matrix where an additional check is performed in the
	// inner two for loops. For more information on that, see labbook.

	// Variables (5 in total)
	int i, j, b_row, b_col, block_size;

	// 32*32 matrix
	if (N == 32 && M == 32)
	{
		block_size = 8;
		// for each block of size block_size
		for (b_col = 0; b_col < N; b_col += block_size) {
			for (b_row = 0; b_row < N; b_row += block_size) {

				// for each element in block
				for (i = b_row; i < b_row + block_size; i ++) {
					for (j = b_col; j < b_col + block_size; j ++) {

						// if element is not on diagonal,
						// transpose to correct position in B.
						// if element is on diagonal, store in tmp variable
						if (i != j) {
							B[j][i] = A[i][j];
						}
					}
					// element on diagonal: place in B
					if (b_row == b_col) {
						B[i][i] = A[i][i];
					}
				}
			}
		}
	}

	// 64*64 matrix
	else if (N == 64 && M == 64)
	{
		block_size = 4;
		// for each block of size block_size
		for (b_col = 0; b_col < N; b_col += block_size) {
			for (b_row = 0; b_row < N; b_row += block_size) {
				// for each element in block
				for (i = b_row; i < b_row + block_size; i ++) {
					for (j = b_col; j < b_col + block_size; j ++) {
						// if element is not on diagonal,
						// transpose to correct position in B.
						// diagonal outside of forloop
						if (i != j) {
							B[i][j] = A[j][i];
						}
					}
					// element on diagonal: place in B
					if (b_row == b_col) {
						B[i][i] = A[i][i];
					}
				}
			}
		}

	}
	// 61*67 from exercise (or, general case)
	else {
		block_size = 16;
		// for each block
		for (b_col = 0; b_col < M; b_col += block_size) {
			for (b_row = 0; b_row < N; b_row += block_size) {

				// for each element in block AND in matrix (additional code for
				// uneven sized matrices)
				for (i = b_row; (i < b_row + block_size) && (i < N); i ++) {
					for (j = b_col; (j < b_col + block_size) && (j < M); j ++) {
						// if element is not on diagonal,
						// transpose to correct position in B.
						// diagonal outside of forloop
						if (i != j) {
							B[j][i] = A[i][j];
						}
					}
					// element on diagonal: place in B
					if (b_row == b_col) {
						B[i][i] = A[i][i];
					}
				}
	 		}
		}
	}
}


/*
 * registerFunctions - This function registers your transpose
 *     functions with the driver.  At runtime, the driver will
 *     evaluate each of the registered functions and summarize their
 *     performance. This is a handy way to experiment with different
 *     transpose strategies.
 */
void registerFunctions()
{
    /* Register your solution function */
    registerTransFunction(transpose_submit, transpose_submit_desc);

    /* Register any additional transpose functions */
    // registerTransFunction(trans, trans_desc);

}

/*
 * is_transpose - This helper function checks if B is the transpose of
 *     A. You can check the correctness of your transpose by calling
 *     it before returning from the transpose function.
 */
int is_transpose(int M, int N, int A[N][M], int B[M][N])
{
    int i, j;

    for (i = 0; i < N; i++) {
        for (j = 0; j < M; ++j) {
            if (A[i][j] != B[j][i]) {
                return 0;
            }
        }
    }
    return 1;
}
