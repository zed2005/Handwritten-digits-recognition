#ifndef NEURAL_H
#define NEURAL_H

#include "imgProc.h"
#include <stdlib.h>

typedef struct Neuron {
    float activation;
    float bias;
    int weightLen;
    short* weights;
} Neuron;

typedef struct NeuralLayer {
    int len;
    Neuron* neurons;
    struct NeuralLayer* nextLayer;
    struct NeuralLayer* prevLayer;
} NeuralLayer;

NeuralLayer* setupNetwork(int layers, int width, int outputWidth, const imagePixels* startLayer);

float calculateCost(NeuralLayer* finalLayer, size_t correctNum);

void freeNetwork(NeuralLayer* first);

#endif