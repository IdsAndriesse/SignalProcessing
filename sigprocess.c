/*
file    : sigprocess.c    
author  : Ids Andriesse (ids.andriesse@gmail.com)
date    : 12-02-2020
version : 1.0
*/

/* Fast Fourier Transforms and applications*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "util.h"
#include "sigprocess.h"
#include <complex.h>

#define MAX(a,b) ((a)>(b) ? (a) : (b))
#define MIN(a,b) ((a)<(b) ? (a) : (b))

const double pi = 3.141592653589793238462643383279502884;

/*Fast Fourier Transform using Cooley Tukey algorithm
 *The sign of omega (root of unity) gives the direction (see convolveFFT for example) */
complex double *FFT(complex double *a, int n, complex double omega){
    if (n == 1){
        return a;
    }
    //Update root of unity
    complex double x = cpow(omega, 0);
    //Split arrays in even and odd
    complex double *a_even = makeComplexArray(n/2);
    complex double *a_odd = makeComplexArray(n/2);
    for (int i = 0; 2 * i < n; i++){
        a_even[i] = a[2*i];
        a_odd[i] = a[2*i + 1];
    }
    //Recursive calls
    complex double *y_even = FFT(a_even, n/2, cpow(omega, 2));
    complex double *y_odd = FFT(a_odd, n/2, cpow(omega, 2));

    complex double *y = makeComplexArray(n);
    for (int i = 0; i < n/2; i++){
        y[i] = y_even[i] + x*y_odd[i];
        y[i + n/2] = y_even[i] - x*y_odd[i];
        x = x * omega;
    }
    return y;
}

/*2D FFT given a matrix*/
complex double **FFT2D(complex double **matrix, int width, int height, complex double omega){
    /* width and height must both be powers of two ! */
    if (width != powerOfTwo(width) || height != powerOfTwo(height)){
		printf("FFT2D: No power of two\n width = %d\n heigth = %d", width, height);
	}

    //First FFT over rows
    complex double *temp = makeComplexArray(width);
    for (int row = 0; row < height; row++){
		for (int col = 0; col < width; col++){
			temp[col] = matrix[row][col];
		}
		temp = FFT(temp, width, omega);
		for (int col = 0; col < width; col++){
			matrix[row][col] = temp[col];
		}
	}
    free(temp);

    //Then FFT over columns
    temp = makeComplexArray(height);
    for (int col = 0; col < width; col++){
		for (int row = 0; row < height; row++){
			temp[row] = matrix[row][col];
		}
		temp = FFT(temp, width, omega);
		for (int row = 0; row < height; row++){
			matrix[row][col] = temp[row];
		}
	}
    free(temp);
    return matrix;

}

/*Convolution of filter h with signal x */
double *convolveFFT(complex double *h, int len_h, complex double *x, int len_x){
    //Lenght of the convolution
    int len_y = len_h + len_x - 1;

    //Pad arrays to twice the length and the next power of 2
    int new_len = len_h > len_x ? powerOfTwo(len_h) : powerOfTwo(len_x);
    new_len *= 2;
    h = padArray(h, new_len);
    x = padArray(x, new_len);


    //Give root of primitve root of unity
    double angle = 2*pi / new_len;
    complex double omega = cos(angle) + sin(angle)*I;

    h = FFT(h, new_len, omega);
    x = FFT(x, new_len, omega);
    

    //pointWise multiplication 
    complex double *cy = makeComplexArray(new_len);
    for (int i = 0; i < new_len; i++){
        cy[i] = h[i] * x[i];
    }

    //Change sign of angle root of unity for inverse FFT
    angle *= -1;
    omega = cos(angle) + sin(angle)*I;
    //IFFT
    cy = FFT(cy, new_len, omega);
    
    double *y = makeDoubleArray(len_y);
    for (int i = 0; i < len_y; i++){
        y[i] = (cy[i] /  new_len);
    }
    return y;
}

/*Correlation using FFT see convolveFFt for details*/
double *correlateFFT(complex double *h, int len_h, complex double *x, int len_x){
    int len_y = len_h + len_x - 1;

    int new_len = len_h > len_x ? powerOfTwo(len_h) : powerOfTwo(len_x);
    new_len *= 2;
    h = padArray(h, new_len);
    x = padArray(x, new_len);


    //Give root of primitve root of unity
    double angle = 2*pi / new_len;
    complex double omega = cos(angle) + sin(angle)*I;

    h = FFT(h, new_len, omega);
    x = FFT(x, new_len, omega);
    

    //pointWise multiplication using conjugate of h (flipped filter)
    complex double *cy = makeComplexArray(new_len);
    for (int i = 0; i < new_len; i++){
        cy[i] = conj(h[i]) * x[i];
    }
    //Change sign of angle root of unity for inverse FFT
    angle *= -1;
    omega = cos(angle) + sin(angle)*I;
    //IFFT
    cy = FFT(cy, new_len, omega);

    double *y = makeDoubleArray(len_y);
    for (int i = 0; i < len_y; i++){
        y[i] = (cy[i] /  new_len);
    }
    return y;
}

/*This function performs a correlation using FFT and then corrects it
 * such that the final values are in range [-1, 1] (pearson correlation coefficient) */
double *pearsonCorrelator(double *h, int len_h, double *x, int len_x){
    int len_y =  len_x - len_h + 1;
    double *cor = correlateFFT(h, len_h, x, len_x);
    double *y = makeDoubleArray(len_y);
    double h_bar = average(h, len_h);
    for (int d = 0; d < len_y; d++){
        double sum_x = 0;
        double sum_h = 0;
        double sum_num = 0;
        double x_bar = getXBar(x, len_x, len_h, d);

        for (int i = 0; i < len_h; i++){
            sum_x += pow(x[i + d] - x_bar, 2);
            sum_h += pow(h[i] - h_bar, 2);
            sum_num += ((x[i + d]*h_bar) + (x_bar*h[i]) - (x_bar*h_bar));
        }
        sum_x = sqrt(sum_x);
        sum_h = sqrt(sum_h);
        double denom = sum_h * sum_x;
        double num = cor[d] - sum_num;
        y[d] = num / denom; 
    }
    return y;
}