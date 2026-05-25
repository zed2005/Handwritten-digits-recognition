/**
 * \file neural.h
 * This is the core of the network
 */

#ifndef NEURAL_H
#define NEURAL_H

#include "imgProc.h"
#include <stdlib.h>

/// @brief A single weight
/// @param weight the value of the weight
/// @param deltaCost how much the Cost changes based on the change of this weight
typedef struct Weight {
    float weight;
    float deltaCost;
} Weight;


/// @brief A single neuron
/// @param activation How strong is the neuron (between 0 and 1)
/// @param bias an additional value that helps in fine tunng the network
/// @param weightLen the lenght of the weight array
/// @param weights the weights leading from this neuron to the next layer
typedef struct Neuron {
    float activation;
    float bias;
    float deltaError;
    size_t weightLen;
    Weight* weights;
} Neuron;


/// @brief a layer in the network (the whole thing is a doubly linked list)
/// @param len how mani neurons does the layer have
/// @param neurons the array of neurons
/// @param nextLayer pointer to the next layer
/// @param prevLayer pointer to the previous layer
typedef struct NeuralLayer {
    size_t len;
    Neuron* neurons;
    struct NeuralLayer* nextLayer;
    struct NeuralLayer* prevLayer;
} NeuralLayer;


/// @brief function for making a network
/// @param layers how mani layers it has (excluding first and last)
/// @param width how wide is one hidden layer
/// @param outputWidth how wide is the output layer 
/// @param startWidth how wide is the input layer
/// @return a pointer to the input layer of the network
NeuralLayer* setupNetwork(size_t layers, size_t width, size_t outputWidth, size_t startWidth);

/// @brief running the network
/// @param firstLayer the pointer to the input layer
/// @param pixels the processed image
void runNetwork(NeuralLayer* firstLayer, const imagePixels* pixels);

/// @brief calculating the cost function of an image
/// @param finalLayer the final layer of the network
/// @param correctNum the numbers which is on the image
/// @return the cost of the image (sum(finalLayer[i]-desired[i])^2)) 
float calculateCost(NeuralLayer* finalLayer, size_t correctNum);

/// @brief freeing the memory allocated for the network
/// @param first the pointer to the input layer
void freeNetwork(NeuralLayer* first);

#endif