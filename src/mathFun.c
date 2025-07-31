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

float randWeight(signed int min, signed int max) {
    int magicNum = 128;
    int buffMin = min * magicNum; int buffMax = max * magicNum; 
    float randNum = rand()%buffMax;
    randNum += buffMin;
    randNum /= 128;
    return randNum*((min+max)/2);
}