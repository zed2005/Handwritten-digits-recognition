#ifndef NEURAL_H
#define NEURAL_H

#include "imgProc.h"
#include <stdlib.h>

typedef struct Neuron {
    float activation;
    float bias;
    size_t weightLen;
    short* weights;
} Neuron;

typedef struct NeuralLayer {
    size_t len;
    Neuron* neurons;
    struct NeuralLayer* nextLayer;
    struct NeuralLayer* prevLayer;
} NeuralLayer;

NeuralLayer* setupNetwork(size_t layers, size_t width, size_t outputWidth, size_t startWidth);

void runNetwork(NeuralLayer* firstLayer, const imagePixels* pixels);

float calculateCost(NeuralLayer* finalLayer, size_t correctNum);

void freeNetwork(NeuralLayer* first);

#endif