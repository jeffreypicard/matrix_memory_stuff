#include <stdlib.h>
#include <stdio.h>
#include <time.h>

struct matrix {
	int n, m;
	double **vals;
};

#define ACCESS(A, i, j) ((A)->vals[i * (A)->n + j])

struct matrix2 {
	int n, m;
	double *vals;
};

struct vector {
	int n;
	double *vals;
};

struct matrix2 *
alloc_matrix2(int n, int m)
{
	struct matrix2 *A = calloc(1, sizeof *A);

	A->vals = calloc(n * m, sizeof *A->vals);
	A->n = n;
	A->m = m;

	return A;
}

void
free_matrix2(struct matrix2 *A)
{
	free(A->vals);
	free(A);
}

void
fill_matrix2(struct matrix2 *A)
{
	int i, j;
	double x = 0.0;

	for (i = 0; i < A->n; i++) {
		for (j = 0; j < A->m; j++) {
			ACCESS(A, i, j) = x;
			x += 1;
		}
	}
}

void
print_matrix2(struct matrix2 *A)
{
	int i, j;

	for (i = 0; i < A->n; i++) {
		for (j = 0; j < A->m; j++) {
			printf("%g\t", ACCESS(A, i, j));
		}
		printf("\n");
	}
}

void
print_matrix_addrs2(struct matrix2 *A)
{
	int i, j;

	for (i = 0; i < A->n; i++) {
		printf("%p,\t", A->vals + A->n * i);
		for (j = 0; j < A->m; j++) {
			printf("%p\t", A->vals + A->n * i + j);
		}
		printf("\n");
	}
}

void
transpose_inplace2(struct matrix2 *A)
{
	int i, j;
	double temp;

	for (i = 0; i < A->n; i++) {
		for (j = i; j < A->m; j++) {
			temp = ACCESS(A, i, j);
			ACCESS(A, i, j) = ACCESS(A, j, i);
			ACCESS(A, j, i) = temp;
		}
	}
}

struct matrix *
alloc_matrix(int n, int m)
{
	int i;
	struct matrix *A = calloc(1, sizeof *A);
	
	if (!A) {
		fprintf(stderr, "Error: alloc failed\n");
		return 0;
	}

	A->vals = calloc(n, sizeof *A->vals);

	for (i = 0; i < n; i++) {
		A->vals[i] = calloc(m, sizeof **A->vals);
	}

	A->n = n;
	A->m = m;

	return A;
}

void
free_matrix(struct matrix *A)
{
	int i;

	for (i = 0; i < A->n; i++) {
		free(A->vals[i]);
	}
	free(A->vals);
	free(A);
}

void
fill_matrix(struct matrix *A)
{
	int i, j;
	double x = 0.0;

	for (i = 0; i < A->n; i++) {
		for (j = 0; j < A->m; j++) {
			A->vals[i][j] = x;
			x += 1.0;
		}
	}
}

void
print_matrix(struct matrix *A)
{
	int i, j;

	for (i = 0; i < A->n; i++) {
		for (j = 0; j < A->m; j++) {
			printf("%g\t", A->vals[i][j]);
		}
		printf("\n");
	}
}

void
print_matrix_addrs(struct matrix *A)
{
	int i, j;

	for (i = 0; i < A->n; i++) {
		printf("%p,\t", A->vals + i);
		for (j = 0; j < A->m; j++) {
			printf("%p\t", A->vals[i] + j);
		}
		printf("\n");
	}
}

struct matrix*
transpose(struct matrix *A)
{
	int i, j;
	/* FIXME should this alloc memory? */
	struct matrix *B = alloc_matrix(A->m, A->n);

	for (i = 0; i < A->n; i++) {
		for (j = 0; j < A->m; j++) {
			B->vals[j][i] = A->vals[i][j];
		}
	}

	return B;
}

void
transpose_inplace(struct matrix *A)
{
	int i, j;
	double temp;

	for (i = 0; i < A->n; i++) {
		for (j = i; j < A->m; j++) {
			temp = A->vals[i][j];
			A->vals[i][j] = A->vals[j][i];
			A->vals[j][i] = temp;
		}
	}
}

struct matrix2 *
multiply2(struct matrix2 *A, struct matrix2 *B)
{
	int i, j, k;
	struct matrix2 *C;

	if (A->n != B->m) return 0;

	C = alloc_matrix2(A->m, B->n);
	if (!C) return 0;

	for (i = 0; i < C->n; i++) {
		for (j = 0; j < C->m; j++) {
			double res = 0;
			for (k = 0; k < C->m; k++) {
				res += ACCESS(A, i, k) * ACCESS(B, k, j);
			}
			ACCESS(C, i, j) = res;
		}
	}

	return C;
}

struct matrix *
multiply(struct matrix *A, struct matrix *B)
{
	int i, j, k;
	struct matrix *C;

	if (A->n != B->m) return 0;

	C = alloc_matrix(A->m, B->n);
	if (!C) return 0;

	for (i = 0; i < C->n; i++) {
		for (j = 0; j < C->m; j++) {
			double res = 0;
			for (k = 0; k < C->m; k++) {
				res += A->vals[i][k] * B->vals[k][j];
			}
			C->vals[i][j] = res;
		}
	}

	return C;
}

int
main(int argc, char **argv)
{
	struct matrix *A, *B, *C;
	struct matrix2 *X, *Y, *Z;
	int n, t;

	if (argc < 2) {
		n = 100;
	} else {
		n = atoi(argv[1]);
	}

	if (argc < 3) {
		t = 0;
	} else {
		t = atoi(argv[2]);
	}

	if (t == 1) {

		X = alloc_matrix2(n, n);
		Y = alloc_matrix2(n, n);
		fill_matrix2(X);
		fill_matrix2(Y);

		clock_t begin = clock();

		//print_matrix2(X);
		//transpose_inplace2(X);
		//print_matrix2(X);
		Z = multiply2(X, Y);

		clock_t end = clock();
		double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;

		//print_matrix_addrs2(X);
		//print_matrix2(X);
		//print_matrix2(Y);
		//print_matrix2(Z);
		
		printf("%f\n", time_spent);
	} else if (t == 0) {

		A = alloc_matrix(n, n);
		B = alloc_matrix(n, n);
		fill_matrix(A);
		fill_matrix(B);

		clock_t begin = clock();

		//print_matrix(A);
		//transpose_inplace(A);
		//print_matrix(A);
		C = multiply(A, B);

		clock_t end = clock();
		double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
		/*
		print_matrix(A);
		puts("");
		print_matrix(B);
		puts("");
		print_matrix(C);
		*/

		//print_matrix_addrs(A);
		
		printf("%f\n", time_spent);
	}

	return 0;
}
