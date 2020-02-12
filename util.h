#include <complex.h>

#ifndef UTIL_H
#define UTIL_H


void *safeMalloc(int sz);

void *safeRealloc(void *p, int sz);

int *makeIntArray(int n);

double *makeDoubleArray(int n);

complex double *makeComplexArray(int n);

void destroyArray(void *p);

void printIntArray(int length, int *arr);

void printDoubleArray(int length, double *arr);

int *readSignal(int *len);

double *readSignalDouble(int *len);

void printSignal(int len, int *x);

void printSignalDouble(int len, double *x);

int* doubleToInt(double* arr, int len);

double *intToDouble(int *arr, int len);

complex double *doubleToComplex(double *arr, int len);

complex double *intToComplex(int *arr, int len);

int *complexToInt(complex double *arr, int len);

int dequal(double a, double b, double epsilon);

int powerOfTwo(int n);

complex double *padArray(complex double *arr, int new_len);

double average(double *arr, int len);

double getXBar(double *x, int len_x, int len_h, int d);

#endif