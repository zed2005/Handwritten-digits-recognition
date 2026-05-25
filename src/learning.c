/**
 * \file learning.c
 * This is where the learning functions are defined.
 */

#include "learning.h"
#include "neural.h"
#include "mathFun.h"
#include <math.h>

float calculateSum(NeuralLayer* currentLayer, size_t currentNeuronIdx) {
    float sum = currentLayer->neurons[currentNeuronIdx].bias;

    for(size_t i = 0; i < currentLayer->prevLayer->len; i++) {
        sum += currentLayer->prevLayer->neurons[i].activation * currentLayer->prevLayer->neurons[i].weights[currentNeuronIdx].weight;
    }

    return sum;
}

float calculateDeltaCost(NeuralLayer* currentLayer, size_t currentNeuronIdx, size_t currentWeightIdx) {
    return currentLayer->neurons[currentNeuronIdx].deltaError * currentLayer->prevLayer->neurons[currentWeightIdx].activation;
}

float calculateDeltaError(NeuralLayer* currentLayer, size_t currentNeuronIdx) {
    float delta = 0;

    for(size_t i = 0; i < currentLayer->nextLayer->len; i++) {
        delta += currentLayer->nextLayer->neurons[i].weights[currentNeuronIdx].deltaCost;
    }

    delta *= dReLU(calculateSum(currentLayer, currentNeuronIdx));

    return delta;
}

float calculateLastLayerDeltaError(NeuralLayer* currentLayer, size_t currentNeuronIdx, float epsilon) {
    return -2 * epsilon * dReLU(calculateSum(currentLayer, currentNeuronIdx));
}

void calculateNeuronWeights(NeuralLayer* currentLayer, size_t currentNeuronIdx) {
    for(size_t i = 0; i < currentLayer->prevLayer->len; i++) {
        float act = currentLayer->prevLayer->neurons[i].activation;
        float error = currentLayer->neurons[currentNeuronIdx].deltaError;
        
        currentLayer->prevLayer->neurons[i].weights[currentNeuronIdx].deltaCost = act * error;
    } 
}

void calculateLastLayer(NeuralLayer* lastLayer, short desiredNumber) {
    for(size_t i = 0; i < lastLayer->len; i++) {
        short d = desiredNumber == i ? 1 : 0;
        lastLayer->neurons[i].deltaError = calculateLastLayerDeltaError(lastLayer, i, d - lastLayer->neurons[i].activation);
        calculateNeuronWeights(lastLayer->prevLayer, i);
    }
}

void calculateLayer(NeuralLayer* currentLayer) {
    NeuralLayer* prev = currentLayer->prevLayer;
    if (prev == NULL) return;

    for(size_t i = 0; i < prev->len; i++) {
        for(size_t j = 0; j < currentLayer->len; j++) {
            float act = prev->neurons[i].activation;
            float err = currentLayer->neurons[j].deltaError;
            
            prev->neurons[i].weights[j].deltaCost = act * err;
        }
    }
}

void backPropagation(NeuralLayer* firstLayer, short desiredNumber) {
    NeuralLayer* iter = firstLayer;
    while(iter->nextLayer != NULL) iter = iter->nextLayer;

    calculateLastLayer(iter, desiredNumber);
    iter = iter->prevLayer;

    while(iter->prevLayer != NULL) {
        calculateLayer(iter);
        iter = iter->prevLayer;
    }
}

void addBiasToNeuron(NeuralLayer* currentLayer, size_t currentNeuronIdx, float learningRate) {
    currentLayer->neurons[currentNeuronIdx].bias -= learningRate * currentLayer->neurons[currentNeuronIdx].deltaError;
    currentLayer->neurons[currentNeuronIdx].deltaError = 0;
}

void addDeltaToNeuron(NeuralLayer* currentLayer, size_t currentNeuronIdx, float learningRate) {
    addBiasToNeuron(currentLayer, currentNeuronIdx, learningRate);
    for(size_t i = 0; i < currentLayer->nextLayer->len; i++) {
        currentLayer->neurons[currentNeuronIdx].weights[i].weight -= learningRate * currentLayer->neurons[currentNeuronIdx].weights[i].deltaCost;
        currentLayer->neurons[currentNeuronIdx].weights[i].deltaCost = 0; 
    }
}

void addDeltaToLayer(NeuralLayer* currentLayer, float learningRate) {
    for(size_t i = 0; i < currentLayer->len; i++) {
        addDeltaToNeuron(currentLayer, i, learningRate);
    }
}

void addDeltaToNetwork(NeuralLayer* firstLayer, float learningRate) {
    NeuralLayer* iter = firstLayer;

    while(iter->nextLayer != NULL) {
        addDeltaToLayer(iter, learningRate);
        iter = iter->nextLayer;
    }

    for(size_t i = 0; i < iter->len; i++) addBiasToNeuron(iter, i, learningRate);
}