#ifndef SIGPROCESS_H
#define SIGPROCESS_H

complex double *FFT(complex double *a, int n, complex double omega);

complex double **FFT2D(complex double **matrix, int width, int height, complex double omega);

double *convolveFFT(complex double *h, int len_h, complex double *x, int len_x);

double *correlateFFT(complex double *h, int len_h, complex double *x, int len_x);

double *pearsonCorrelator(double *h, int len_h, double *x, int len_x);

#endif