#include "formulas.h"
#include <immintrin.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

float formula1(float *x, unsigned int length) {

    unsigned int simdLength = length / 16;
    unsigned int remainder = length % 16;

    __m512 upSum;
    __m512 downSum = _mm512_set1_ps(1.0f);
    __m512 iterX, upIterSqrt, xSquaredPlusOne;

    for (unsigned int i = 0; i < simdLength; ++i) {
        iterX = _mm512_loadu_ps(&x[i * 16]);

        upIterSqrt = _mm512_sqrt_ps(iterX); // sqrt(Xi)
        upSum = _mm512_add_ps(upSum, upIterSqrt); // Add all elements.

        xSquaredPlusOne = _mm512_fmadd_ps(iterX, iterX, _mm512_set1_ps(1.0f));
        downSum = _mm512_mul_ps(downSum, xSquaredPlusOne);
    }

    if (remainder > 0) {
        __mmask16 mask = (1 << remainder) - 1;

        iterX = _mm512_maskz_loadu_ps(mask, &x[simdLength * 16]);

        upIterSqrt = _mm512_sqrt_ps(iterX);
        upSum = _mm512_add_ps(upSum, upIterSqrt);

        xSquaredPlusOne = _mm512_fmadd_ps(iterX, iterX, _mm512_set1_ps(1.0f));
        downSum = _mm512_mul_ps(downSum, xSquaredPlusOne);
    }

    // Horizontal sum1 reduction.
    float upSumResult = _mm512_reduce_add_ps(upSum);
    // The third root of sum1.
    upSumResult = cbrtf(upSumResult);

    float downSumResult = _mm512_reduce_mul_ps(downSum);


    return sqrtf(1 + (upSumResult / downSumResult));
}

