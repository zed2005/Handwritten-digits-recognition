#ifndef STRUCTURE_H
#define STRUCTURE_H

#include "neural.h"

NeuralLayer* CreateLayer(size_t width);

NeuralLayer* addLayer(NeuralLayer* previousLayer, size_t width);

NeuralLayer* createLayerStructure(size_t layers, size_t width, size_t outputWidth, size_t startWidth);

void allocWeights(NeuralLayer* currentLayer, size_t nextWidth, size_t curIdx);

void setNeuronWeights(NeuralLayer* currentLayer, size_t curIdx, size_t nextWidth);

void setLayerWeights(NeuralLayer* currentLayer, NeuralLayer* nextLayer);

void initializeWeights(NeuralLayer* firstLayer);

void freeNeurons(NeuralLayer* currentLayer, bool isLast);

#endif