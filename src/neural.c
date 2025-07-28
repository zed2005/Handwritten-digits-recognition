#include "neural.h"
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

void allocWeights(NeuralLayer* currentLayer, size_t nextWidth, size_t curIdx) {
    currentLayer->neurons[curIdx].weightLen = nextWidth;
    currentLayer->neurons[curIdx].weights = (short*)malloc(nextWidth*sizeof(short));
}

void setNeuronWeights(NeuralLayer* currentLayer, size_t curIdx, size_t nextWidth) {
    allocWeights(currentLayer, nextWidth, curIdx);
    currentLayer->neurons[curIdx].bias = randWeight(-4, 4);
    for(size_t j = 0; j < nextWidth; j++) {
        currentLayer->neurons[curIdx].weights[j] = randWeight(-4, 4);
    }
}

void setLayerActivation(NeuralLayer* currentLayer, NeuralLayer* prevLayer, size_t curIdx) {
    float sum = 0;
    for(size_t j = 0; j < currentLayer->len; j++) {
        sum += prevLayer->neurons[j].weights[curIdx]*prevLayer->neurons[j].activation;
    }
    currentLayer->neurons[curIdx].activation = sigmoid(sum);
}

void setFirstLayerActivation(NeuralLayer* firstLayer, NeuralLayer* nextLayer, const imagePixels* startLayer) {
    for(size_t i = 0; i < startLayer->len; i++) {
        firstLayer->neurons[i].activation = sigmoid(startLayer->data[i]);
    }
}

void setLayerWeights(NeuralLayer* currentLayer, NeuralLayer* nextLayer) {
    for(size_t i = 0; i < currentLayer->len; i++) {
        setNeuronWeights(currentLayer, i, nextLayer->len);
    }
}


NeuralLayer* addLayer(NeuralLayer* previousLayer, int width) {
    NeuralLayer* newLayer = CreateLayer(width);
    newLayer->prevLayer = previousLayer;
    return newLayer;
}

NeuralLayer* createLayerStructure(int layers, int width, int outputWidth, const imagePixels* startLayer) {
    NeuralLayer* first = CreateLayer(startLayer->len);
    NeuralLayer* temp = first;
    for(size_t i = 0; i < 2; i++) {
        temp->nextLayer = addLayer(temp, width);
        temp = temp->nextLayer;
    }
    temp->nextLayer = addLayer(temp, outputWidth);
    return first;
}

void initializeWeights(NeuralLayer* firstLayer) {
    setLayerWeights(firstLayer, firstLayer->nextLayer);
    NeuralLayer* iterLayer = firstLayer->nextLayer;
    while(iterLayer->nextLayer != NULL) {
        setLayerWeights(iterLayer, iterLayer->nextLayer);
        iterLayer = iterLayer->nextLayer;
    }
}

NeuralLayer* setupNetwork(int layers, int width, int outputWidth, const imagePixels* startLayer) {
    NeuralLayer* firstLayer = createLayerStructure(layers, width, outputWidth, startLayer);
    initializeWeights(firstLayer);
    return firstLayer;
}

float calculateCost(NeuralLayer* firstLayer, size_t correctNum) {
    NeuralLayer* finalLayer = firstLayer;
    while(finalLayer->nextLayer != NULL) finalLayer = finalLayer->nextLayer;
    float sum = 0;
    for(size_t i = 0; i < finalLayer->len; i++) {
        if(i == correctNum) sum += ((finalLayer->neurons[i].activation - correctNum)*(finalLayer->neurons[i].activation - correctNum));
        else sum += finalLayer->neurons[i].activation*finalLayer->neurons[i].activation;
    }
    return sum;
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