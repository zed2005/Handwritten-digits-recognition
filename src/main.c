/**
 * \file main.c
 * this is the main file of the half baked neural network
 * it now sets up a network with random weights, gets one image, and runs the network on it,
 * then calculates the cost of said image
 */

#include <stdio.h>
#include <stdlib.h>
#include "imgProc.h"
#include "neural.h"
#include "learning.h"
#include "time.h"

int main() {
    srand(time(NULL));

    NeuralLayer* test = setupNetwork(2, 64, 10, 784);

    batching(test, 7000, 8);

    testing(test);
    
    freeNetwork(test);
    
    printf("done!\n");
}