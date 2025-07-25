#include "neural.h"
#include "mathFun.h"
#include <stdlib.h>
#include <stdbool.h>

NeuralLayer* CreateLayer(int width) {
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

void setLayerWeights(NeuralLayer* currentLayer, NeuralLayer* prevLayer, size_t curIdx) {
    currentLayer->neurons[curIdx].bias = randWeight(-4, 4);
    float sum = 0;
    for(size_t j = 0; j < currentLayer->len; j++) {
        sum += prevLayer->neurons[j].weights[curIdx]*prevLayer->neurons[j].activation;
    }
    currentLayer->neurons[curIdx].activation = sigmoid(sum);
}

void setFirstLayer(NeuralLayer* firstLayer, NeuralLayer* nextLayer, const imagePixels* startLayer) {
    for(size_t i = 0; i < startLayer->len; i++) {
        firstLayer->neurons[i].activation = sigmoid(startLayer->data[i]);
        firstLayer->neurons[i].weightLen = nextLayer->len;
        allocWeights(firstLayer, nextLayer->len, i);
        for(size_t j = 0; j < nextLayer->len; j++) {
            firstLayer->neurons[i].weights[j] = randWeight(-4, 4);
        }
    }
}

void setHiddenLayer(NeuralLayer* currentLayer, NeuralLayer* prevLayer, NeuralLayer* nextLayer) {
    for(size_t i = 0; i < currentLayer->len; i++) {
        allocWeights(currentLayer, nextLayer->len, i);
        setLayerWeights(currentLayer, prevLayer, i);
        for(size_t j = 0; j < nextLayer->len; j++) {
            currentLayer->neurons[i].weights[j] = randWeight(-4, 4);
        }
    }
}

void setFinalLayer(NeuralLayer* currentLayer, NeuralLayer* prevLayer) {
    for(size_t i = 0; i < currentLayer->len; i++) {
        setLayerWeights(currentLayer, prevLayer, i);
    }
}

NeuralLayer* hiddenLayers(NeuralLayer* previousLayer, int width) {
    NeuralLayer* newLayer = CreateLayer(width);
    newLayer->prevLayer = previousLayer;
    return newLayer;
}

NeuralLayer* createNodes(int layers, int width, int outputWidth, const imagePixels* startLayer) {
    NeuralLayer* first = CreateLayer(startLayer->len);
    NeuralLayer* temp = first;
    for(int i = 0; i < 2; i++) {
        temp->nextLayer = hiddenLayers(temp, width);
        temp = temp->nextLayer;
    }
    temp->nextLayer = hiddenLayers(temp, outputWidth);
    return first;
}

void createWeights(NeuralLayer* firstLayer, const imagePixels* pixels) {
    setFirstLayer(firstLayer, firstLayer->nextLayer, pixels);
    NeuralLayer* iterLayer = firstLayer->nextLayer;
    while(iterLayer->nextLayer != NULL) {
        setHiddenLayer(iterLayer, iterLayer->prevLayer, iterLayer->nextLayer);
        iterLayer = iterLayer->nextLayer;
    }
    setFinalLayer(iterLayer, iterLayer->prevLayer);
}

NeuralLayer* setupNetwork(int layers, int width, int outputWidth, const imagePixels* startLayer) {
    NeuralLayer* firstLayer = createNodes(layers, width, outputWidth, startLayer);
    createWeights(firstLayer, startLayer);
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