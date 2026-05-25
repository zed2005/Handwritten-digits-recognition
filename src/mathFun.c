/**
 * \file mathFun.c
 * this is where the mathematical functions are defined
 */

#include <math.h>
#include <stdlib.h>
#include "mathFun.h"

float sigmoid(float input) {
    float partial = pow(M_E, input*-1);
    partial += 1;
    return 1/partial;
}

float ReLU(float input) {
    return fmax(0, input);
}

float randWeight(float min, float max) {
    float scale = rand() / (float) RAND_MAX; 
    return min + scale * ( max - min ); 
}