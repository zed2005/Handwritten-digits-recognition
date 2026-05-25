/**
 * \file structure.c
 * this is where the structural functions are defined
 */

#include "structure.h"
#include "mathFun.h"
#include <stdlib.h>
#include <stdbool.h>

NeuralLayer* CreateLayer(size_t width) {
    NeuralLayer* first = (NeuralLayer*)malloc(sizeof(NeuralLayer));
    first->len = width;
    first->neurons = (Neuron*)malloc(first->len*sizeof(Neuron));
    first->prevLayer = NULL;
    first->nextLayer = NULL;
    return first;
}

NeuralLayer* addLayer(NeuralLayer* previousLayer, size_t width) {
    NeuralLayer* newLayer = CreateLayer(width);
    newLayer->prevLayer = previousLayer;
    return newLayer;
}

NeuralLayer* createLayerStructure(size_t layers, size_t width, size_t outputWidth, size_t startWidth) {
    NeuralLayer* first = CreateLayer(startWidth);
    NeuralLayer* temp = first;
    for(size_t i = 0; i < layers; i++) {
        temp->nextLayer = addLayer(temp, width);
        temp = temp->nextLayer;
    }
    temp->nextLayer = addLayer(temp, outputWidth);
    return first;
}


void allocWeights(NeuralLayer* currentLayer, size_t nextWidth, size_t curIdx) {
    currentLayer->neurons[curIdx].weightLen = nextWidth;
    currentLayer->neurons[curIdx].weights = (Weight*)malloc(nextWidth*sizeof(Weight));
}

void setWeight(NeuralLayer* currentlayer, size_t currentNeuronIdx, size_t currentWeightIdx) {
    currentlayer->neurons[currentNeuronIdx].weights[currentWeightIdx].weight = randWeight(-0.5, 0.5);
    currentlayer->neurons[currentNeuronIdx].weights[currentWeightIdx].deltaCost = 0;
}

void setNeuronWeights(NeuralLayer* currentLayer, size_t curIdx, size_t nextWidth) {
    allocWeights(currentLayer, nextWidth, curIdx);
    currentLayer->neurons[curIdx].bias = randWeight(-0.5f, 0.5f);
    currentLayer->neurons[curIdx].deltaError = 0;
    for(size_t j = 0; j < nextWidth; j++) {
        setWeight(currentLayer, curIdx, j);
    }
}

void setLayerWeights(NeuralLayer* currentLayer, NeuralLayer* nextLayer) {
    for(size_t i = 0; i < currentLayer->len; i++) {
        setNeuronWeights(currentLayer, i, nextLayer->len);
    }
}

void initializeWeights(NeuralLayer* firstLayer) {
    NeuralLayer* iterLayer = firstLayer;
    while(iterLayer->nextLayer != NULL) {
        setLayerWeights(iterLayer, iterLayer->nextLayer);
        iterLayer = iterLayer->nextLayer;
    }
}

NeuralLayer* setupNetwork(size_t layers, size_t width, size_t outputWidth, size_t startWidth) {
    NeuralLayer* firstLayer = createLayerStructure(layers, width, outputWidth, startWidth);
    initializeWeights(firstLayer);
    return firstLayer;
}

void freeNeurons(NeuralLayer* currentLayer, bool isLast) {
    if(!isLast) {
        for(size_t i = 0; i < currentLayer->len; i++) {
            free(currentLayer->neurons[i].weights);
        }
    }
    free(currentLayer->neurons);
}

void freeNetwork(NeuralLayer* first) {
    NeuralLayer* temp = first;
    while (temp->nextLayer != NULL) {
        temp = temp->nextLayer;
        freeNeurons(temp->prevLayer, false);
        free(temp->prevLayer);
    }
    freeNeurons(temp, true);
    free(temp);
}