/**
 * \file learning.h
 * This is the learning procedure of the network
 */

#ifndef LEARNING_H
#define LEARNING_H

#include "neural.h"
#include <stdio.h>

/// @brief calculates the sum of the neuron before squishing (sum(wijk*y(i-1)k))
/// @param currentLayer the current layer the neuron is in
/// @param currentNeuronIdx the currrent neuron whos 
/// @return the sum
float calculateSum(NeuralLayer* currentLayer, size_t currentNeuronIdx);

/// @brief calculates deltaCost for a specific neurons specific weight
/// @param currentLayer the current layer the neuron is in
/// @param currentNeuronIdx the currrent neuron whos weight we are adjusting
/// @param currentWeightIdx the weight whichs deltaCost we are calculating
/// @return the specified deltaCost
float calculateDeltaCost(NeuralLayer* currentLayer, size_t currentNeuronIdx, size_t currentWeightIdx);

/// @brief calculates deltaError for a specific neuron not in the last layer
/// @param currentLayer the current layer the neuron is in
/// @param currentNeuronIdx the neuron whichs deltaError we are calculating
/// @return the deltaError of the specified neuron
float calculateDeltaError(NeuralLayer* currentLayer, size_t currentNeuronIdx);

/// @brief calculates deltaError for a specific neuron in the last layer
/// @param currentLayer the current layer the neuron is in
/// @param currentNeuronIdx the neuron whichs deltaError we are calculating
/// @param epsilon part of the cost, calculates as d - y, where d is the expected outptut, and y is the actual
/// @return the deltaError of the specified neuron
float calculateLastLayerDeltaError(NeuralLayer* currentLayer, size_t currentNeuronIdx, float epsilon);

/// @brief sets deltaCost and deltaError for the last layer
/// @param lastLayer the last layer of the network
void calculateLastLayer(NeuralLayer* lastLayer, short desiredNumber);

/// @brief sets deltaCost and deltaError for a layer
/// @param currentLayer the current layer of the network
void calculateLayer(NeuralLayer* currentLayer);

/// @brief calculates the deltas for the whole network
/// @param firstLayer the first layer of the network
void backPropagation(NeuralLayer* firstLayer, short desiredNumber);

/// @brief adds deltas tto a neuron and its weights
/// @param currentLayer the current layer
/// @param currentNeuronIdx index of current neuron
void addDeltaToNeuron(NeuralLayer* currentLayer, size_t currentNeuronIdx, float learningRate);

/// @brief adds the deltas to all neurons and weights in the layer
/// @param currentLayer the current layer
void addDeltaToLayer(NeuralLayer* currentLayer, float learningRate);

/// @brief adds the deltas to all neurons and weights in the network
/// @param firstLayer the first layer of the network
void addDeltaToNetwork(NeuralLayer* firstLayer, float learningRate);

void batching(NeuralLayer* network, size_t batchCount, size_t batchSize);

void testing(NeuralLayer* network);

#endif