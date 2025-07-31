#ifndef RUNNING_H
#define RUNNING_H
#include "neural.h"

void setNeuronActivation(NeuralLayer* currentLayer, NeuralLayer* prevLayer, size_t curIdx);

void setLayerActivation(NeuralLayer* currentLayer, NeuralLayer* prevLayer);

void setFirstLayerActivation(NeuralLayer* firstLayer, NeuralLayer* nextLayer, const imagePixels* startLayer);

#endif