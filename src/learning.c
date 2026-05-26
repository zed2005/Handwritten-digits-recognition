/**
 * \file learning.c
 * This is where the learning functions are defined.
 */

#include "learning.h"
#include "neural.h"
#include "mathFun.h"
#include <math.h>
#include <stdlib.h>
#include <stdio.h>

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
        float w = currentLayer->neurons[currentNeuronIdx].weights[i].weight;
        float nextError = currentLayer->nextLayer->neurons[i].deltaError;
        delta += w * nextError;
    }

    float act = currentLayer->neurons[currentNeuronIdx].activation;
    delta *= (act > 0) ? 1.0f : 0.0f;

    return delta;
}

float calculateLastLayerDeltaError(NeuralLayer* currentLayer, size_t currentNeuronIdx, float epsilon) {
    float act = currentLayer->neurons[currentNeuronIdx].activation;
    float sigmoidDerivative = act * (1.0f - act);
    return -2.0f * epsilon * sigmoidDerivative;
}


void calculateNeuronWeights(NeuralLayer* currentLayer, size_t currentNeuronIdx) {
    for(size_t i = 0; i < currentLayer->prevLayer->len; i++) {
        float act = currentLayer->prevLayer->neurons[i].activation;
        float error = currentLayer->neurons[currentNeuronIdx].deltaError;
        
        currentLayer->prevLayer->neurons[i].weights[currentNeuronIdx].deltaCost += act * error;
    } 
}

void calculateLayerWeights(NeuralLayer* currentLayer) {
    NeuralLayer* prev = currentLayer->prevLayer;

    if (prev == NULL) return;
    for(size_t i = 0; i < prev->len; i++) {
        for(size_t j = 0; j < currentLayer->len; j++) {
            float act = prev->neurons[i].activation;
            float err = currentLayer->neurons[j].deltaError;
            prev->neurons[i].weights[j].deltaCost += act * err;
        }
    }
}

void calculateLastLayer(NeuralLayer* lastLayer, short desiredNumber) {
    for(size_t i = 0; i < lastLayer->len; i++) {
        short d = desiredNumber == i ? 1 : 0;
        lastLayer->neurons[i].deltaError = calculateLastLayerDeltaError(lastLayer, i, d - lastLayer->neurons[i].activation);
        
        lastLayer->neurons[i].deltaBias += lastLayer->neurons[i].deltaError; 
    }
    calculateLayerWeights(lastLayer);
}

void calculateLayer(NeuralLayer* currentLayer) {
    for(size_t i = 0; i < currentLayer->len; i++) {
        currentLayer->neurons[i].deltaError = calculateDeltaError(currentLayer, i);
        
        currentLayer->neurons[i].deltaBias += currentLayer->neurons[i].deltaError;
    }
    calculateLayerWeights(currentLayer);
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
    currentLayer->neurons[currentNeuronIdx].bias -= learningRate * currentLayer->neurons[currentNeuronIdx].deltaBias;
    currentLayer->neurons[currentNeuronIdx].deltaBias = 0;
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

void batching(NeuralLayer* network, size_t batchCount, size_t batchSize) {
    FILE* fptr = fopen("/home/zedded2005/Code/Handwritten-digits-recognition/mnist_train.csv", "r");
    if(fptr == NULL) { printf("Error while opening file!\n"); return; }

    for(size_t b = 0; b < batchCount; b++) {
        for(size_t i = 0; i < batchSize; i++) {
            imagePixels pixels = setLayer(fptr); 
            short desiredNumber = getDesiredNumber(&pixels);

            runNetwork(network, &pixels);
            backPropagation(network, desiredNumber);
            free(pixels.data);
        }

        addDeltaToNetwork(network, 0.05f / batchSize);
        printf("Batch %d done.\n", (int)b);
    }
    
    fclose(fptr);
}

int getBestActivation(NeuralLayer* fLayer) {
    while(fLayer->nextLayer != NULL) fLayer = fLayer->nextLayer;

    int maxIdx = 0;
    float max = fLayer->neurons[0].activation;

    for(size_t i = 0; i < fLayer->len; i++) {
        if(fLayer->neurons[i].activation > max) {
            max = fLayer->neurons[i].activation;
            maxIdx = i; 
        }
    }

    return maxIdx;
}

void testing(NeuralLayer* network) {
    int correct = 0;

    for(size_t i = 0; i < 1000; i++) {
        imagePixels pixels = processCSV("/home/zedded2005/Code/Handwritten-digits-recognition/mnist_train.csv");
        short desiredNumber = getDesiredNumber(&pixels);

        runNetwork(network, &pixels);
        int guessed = getBestActivation(network);
        printf("Result: %lf, Guess: %d, Correct: %d\n", calculateCost(network, desiredNumber), guessed, desiredNumber);
        
        if(guessed == desiredNumber) correct++;

        free(pixels.data);
    }

    printf("Accuracy: %lf\n", (float)correct/1000);
}
