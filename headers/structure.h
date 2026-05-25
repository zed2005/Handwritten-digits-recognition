/**
 * \file structure.h
 * this is the structural building when creating a network 
 */

#ifndef STRUCTURE_H
#define STRUCTURE_H

#include "neural.h"
#include <stdbool.h>

/// @brief creates a layer with malloced neurons
/// @param width the width of the layer
/// @return the pointer to the created layer
NeuralLayer* CreateLayer(size_t width);

/// @brief extend the network with a layer
/// @param previousLayer pointer to the previous layer
/// @param width the width of the new layer
/// @return a pointer to the new layer
NeuralLayer* addLayer(NeuralLayer* previousLayer, size_t width);

/// @brief creates all the layers of the network
/// @param layers number of hidden layers
/// @param width width of hidden layers
/// @param outputWidth output layer width
/// @param startWidth input layer with
/// @return pointer to the input layer
NeuralLayer* createLayerStructure(size_t layers, size_t width, size_t outputWidth, size_t startWidth);

/// @brief allocates memory for weigths for neuron weights
/// @param currentLayer the layer which the neuron is in
/// @param nextWidth the width of the next layer
/// @param curIdx the current neuron's index in the layer
void allocWeights(NeuralLayer* currentLayer, size_t nextWidth, size_t curIdx);

/// @brief sets up a weight
/// @param currentLayer the current layer we are in
/// @param currentNeuronIdx the index of current neuron whos weights we are setting
/// @param currentWeightIdx the index of the weight we are setting
void setWeight(NeuralLayer* currentlayer, size_t currentNeuronIdx, size_t currentWeightIdx);

/// @brief sets the weights of a given neuron
/// @param currentLayer pointer to the current layer
/// @param curIdx the current neuron's index in the layer
/// @param nextWidth the width of the next layer
void setNeuronWeights(NeuralLayer* currentLayer, size_t curIdx, size_t nextWidth);

/// @brief sets the weights for all the neurons in a given layer
/// @param currentLayer pointer to the current layer
/// @param nextLayer pointer to the next layer
void setLayerWeights(NeuralLayer* currentLayer, NeuralLayer* nextLayer);

/// @brief initializes weights for the whole network
/// @param firstLayer pointer to the input layer
void initializeWeights(NeuralLayer* firstLayer);

/// @brief frees a given layer's neurons
/// @param currentLayer pointer to the actual layer
/// @param isLast output layer doesnt have weights so they don't have to be deleted
void freeNeurons(NeuralLayer* currentLayer, bool isLast);

#endif