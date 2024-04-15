#include "formulas.h"
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>


#define NUM_TESTS 100
#define MAX_LENGTH 10000
#define MAX_ERROR 0.0001
#define MIN_VAL 0.0001
#define MAX_VAl 9999.9999

int is_close(float f1, float f2) {
    return f1 - f2 <= MAX_ERROR && f2 - f1 <= MAX_ERROR;
}


float formula1_test(float *x, unsigned int length) {
    float sum = 0;
    float product = 1;
    for (int k = 0; k < length; k++) {
        sum += sqrtf(x[k]);
        product *= (x[k] * x[k] + 1);
    }
    return sqrtf(1 + cbrtf(sum) / product);
    
}

float formula2_test(float *x, float *y, unsigned int length) {
    float sum = 0;
    for (int k = 0; k < length; k++) {
        float product = 1;
        for (int i = 0; i < length; i++) {
            product *= (x[i]*x[i] + y[i]*y[i] - 2*x[i]*y[i] + 1);
        }
        sum += (x[k]*y[k])/product;
    }
    return sum;
}


int main(void) {
    srand(time(NULL));
    float *x = malloc(sizeof(float) * MAX_LENGTH);
    float *y = malloc(sizeof(float) * MAX_LENGTH);

    for (int i = 0; i < NUM_TESTS; i++) {
        unsigned int length = (rand() % MAX_LENGTH) + 1;
        for (unsigned int k = 0; k < length; k++) {
            x[k] = (((float)rand())/((float)RAND_MAX)) * (MAX_VAl - MIN_VAL) + MIN_VAL;
            y[k] = (((float)rand())/((float)RAND_MAX)) * (MAX_VAl - MIN_VAL) + MIN_VAL;
        }
        
        float form1 = formula1(x, length);
        float test1 = formula1_test(x, length);
        float form2 = formula2(x, y, length);
        float test2 = formula2_test(x, y, length);

        if (is_close(form1, test1)) {
            printf("failed formula 1\n");
            return;
        }

        if (is_close(form2, test2)) {
            printf("failed formula 2\n");
            return;
        }
    }


    printf("test completed successfully\n");
    free(x);
    free(y);
}