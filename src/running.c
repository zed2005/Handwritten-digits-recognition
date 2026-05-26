/**
 * \file running.c
 * this is where the functions related to the running of the network are defined
 */

#include <stdlib.h>
#include "running.h"
#include "mathFun.h"

void setNeuronActivation(NeuralLayer* currentLayer, NeuralLayer* prevLayer, size_t curIdx) {
    float sum = currentLayer->neurons[curIdx].bias;
    for(size_t j = 0; j < prevLayer->len; j++) {
        sum += prevLayer->neurons[j].weights[curIdx].weight*prevLayer->neurons[j].activation;
    }
    
    if (currentLayer->nextLayer == NULL) {
        currentLayer->neurons[curIdx].activation = sigmoid(sum);
    } else {
        currentLayer->neurons[curIdx].activation = ReLU(sum);
    }
}

void setLayerActivation(NeuralLayer* currentLayer, NeuralLayer* prevLayer) {
    for(size_t i = 0; i < currentLayer->len; i++) {
        setNeuronActivation(currentLayer, prevLayer, i);
    }
}

void setFirstLayerActivation(NeuralLayer* firstLayer, NeuralLayer* nextLayer, const imagePixels* startLayer) {
    for(size_t i = 0; i < startLayer->len; i++) {
        firstLayer->neurons[i].activation = ReLU(startLayer->data[i]/255.0f);
    }
}

void runNetwork(NeuralLayer* firstLayer, const imagePixels* pixels) {
    setFirstLayerActivation(firstLayer, firstLayer->nextLayer, pixels);
    NeuralLayer* iterLayer = firstLayer->nextLayer;
    while(iterLayer != NULL) {
        setLayerActivation(iterLayer, iterLayer->prevLayer);
        iterLayer = iterLayer->nextLayer;
    }
}

float calculateCost(NeuralLayer* firstLayer, size_t correctNum) {
    NeuralLayer* finalLayer = firstLayer;
    while(finalLayer->nextLayer != NULL) finalLayer = finalLayer->nextLayer;
    float sum = 0;
    for(size_t i = 0; i < finalLayer->len; i++) {
        if(i == correctNum) sum += ((finalLayer->neurons[i].activation - 1)*(finalLayer->neurons[i].activation - 1));
        else sum += finalLayer->neurons[i].activation*finalLayer->neurons[i].activation;
    }
    return sum;
}