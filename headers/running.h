#ifndef RUNNING_H
#define RUNNING_H
#include "neural.h"

/// @brief sets the activation of a single neuron
/// @param currentLayer pointer to the current layer
/// @param prevLayer pointer to the previous layer
/// @param curIdx the current neuron's index in the layer
void setNeuronActivation(NeuralLayer* currentLayer, NeuralLayer* prevLayer, size_t curIdx);

/// @brief sets the activation for all neurons in a given layer
/// @param currentLayer pointer to the current layer
/// @param prevLayer pointer to the previous layer
void setLayerActivation(NeuralLayer* currentLayer, NeuralLayer* prevLayer);

/// @brief sets the first layer's activation based on the pixels strength
/// @param firstLayer pointer to the first layer
/// @param nextLayer pointer to the next layer 
/// @param startLayer pointer to the processed image
void setFirstLayerActivation(NeuralLayer* firstLayer, NeuralLayer* nextLayer, const imagePixels* startLayer);

#endif