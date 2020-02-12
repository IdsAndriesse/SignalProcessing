#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "util.h"
#include <complex.h>


void *safeMalloc(int sz)
{
	void *p = calloc(sz, 1);
	if (p == NULL)
	{
		fprintf(stderr, "Fatal error: safeMalloc(%d) failed.\n", sz);
		exit(EXIT_FAILURE);
	}
	return p;
}

void *safeRealloc(void *p, int sz){
    p = realloc(p, sz);
    if (p == NULL)
	{
		fprintf(stderr, "Fatal error: safeRealloc(%d) failed.\n", sz);
		exit(EXIT_FAILURE);
	}
	return p;
}

int *makeIntArray(int n)
{
	/* allocates dynamic int array of size/length n */
	return safeMalloc(n * sizeof(int));
}

double *makeDoubleArray(int n)
{
	/* allocates dynamic double array of size/length n */
	return safeMalloc(n * sizeof(double));
}

complex double *makeComplexArray(int n){
    return safeMalloc(n * sizeof(complex double));
}


void destroyArray(void *p)
{
	free(p);
}

void printIntArray(int length, int *arr)
{
	printf("[");
	if (length > 0)
	{
		printf("%d", arr[0]);
		for (int i = 1; i < length; i++)
		{
			printf(",%d", arr[i]);
		}
	}
	printf("]\n");
}

void printDoubleArray(int length, double *arr)
{
	printf("[");
	if (length > 0)
	{
		printf("%.2lf", arr[0]);
		for (int i = 1; i < length; i++)
		{
			printf(",%.2lf", arr[i]);
		}
	}
	printf("]\n");
}


int *readSignal(int *len) {
  int *x;
  char c;
  scanf("%d:", len);
  x = calloc(*len, sizeof(int));
  do c = getchar(); while (c != '[');
  if (*len > 0) {
    scanf("%d", &x[0]);
    for (int i=1; i < *len; i++) scanf(",%d", &x[i]);
  }
  do c = getchar(); while (c != ']');
  return x;
}

double *readSignalDouble(int *len){
  double *x;
  char c;
  scanf("%d:", len);
  x = makeDoubleArray(*len);
  do c = getchar(); while (c != '[');
  if (*len > 0) {
    scanf("%lf", &x[0]);
    for (int i=1; i < *len; i++) scanf(",%lf", &x[i]);
  }
  do c = getchar(); while (c != ']');
  return x;
}

void printSignal(int len, int *x) {
  printf("%d: [", len);
  if (len > 0) {
    printf("%d", x[0]);
    for (int i=1; i < len; i++) printf(",%d", x[i]);
  }
  printf("]\n");
}

void printSignalDouble(int len, double *x){
    printf("%d: [", len);
    if (len > 0) {
        printf("%.5lf", x[0]);
    for (int i=1; i < len; i++) printf(",%.5lf", x[i]);
    }
    printf("]\n");
}

int* doubleToInt(double* arr, int len){
    int *new = makeIntArray(len);
    for (int i = 0; i < len; i++){
        new[i] = (int)arr[i];
    }
    return new;
}

double *intToDouble(int *arr, int len){
    double *new = makeDoubleArray(len);
    for (int i = 0 ; i < len; i++){
        new[i] = (double)arr[i];
    }
    return new;
}

complex double *doubleToComplex(double *arr, int len){
    complex double *new = makeComplexArray(len);
    for (int i = 0; i < len; i++){
        new[i] = arr[i] + 0*I;
    }
    return new;
}

complex double *intToComplex(int *arr, int len){
    complex double *new = makeComplexArray(len);
    for (int i = 0; i < len; i++){
        new[i] = (double)arr[i];
    }
    return new;
}

int *complexToInt(complex double *arr, int len){
    int *new = makeIntArray(len);
    for (int i = 0; i < len; i++)
    {
        new[i] = creal(arr[i]);
    }
    return new;
    
    
}

int dequal(double a, double b, double epsilon)
{
	return fabs(a - b) < epsilon;
} 

int powerOfTwo(int n) {
    int p2;
    p2 = 1;
    while (p2 < n) {
        p2 *= 2;
    }
    return p2;
}

complex double *padArray(complex double *arr, int new_len) {
    
    return safeRealloc(arr, new_len*sizeof(complex double));
}





double average(double *arr, int len){
    double sum = 0;
    for (int i = 0; i < len; i++){
        sum += arr[i];
    }
    return sum / (double) len;
}

double getXBar(double *x, int len_x, int len_h, int d){
    double *overlap = makeDoubleArray(len_h);
    for (int i = 0; i < len_h; i++){
        overlap[i] = (double)x[i + d]; 
    }
    return average(overlap, len_h);
}
