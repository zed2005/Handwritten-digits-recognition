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

    NeuralLayer* test = setupNetwork(2, 10, 10, 784);

    imagePixels pixels = processCSV("/home/zedded2005/Code/Handwritten-digits-recognition/mnist_train.csv");
    short desiredNumber = getDesiredNumber(&pixels);
    //printPixels(&pixels);
   
    runNetwork(test, &pixels);
    printf("result : %lf\n", calculateCost(test, desiredNumber));
    
    backPropagation(test, desiredNumber);
    addDeltaToNetwork(test, 0.01);
    runNetwork(test, &pixels);
    printf("result : %lf\n", calculateCost(test, desiredNumber));

    freeNetwork(test);
    free(pixels.data);
    printf("done!\n");
}