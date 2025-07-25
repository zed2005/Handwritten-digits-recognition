#include <stdio.h>
#include <stdlib.h>
#include "imgProc.h"
#include "neural.h"
#include "time.h"

int main() {
    srand(time(NULL));
    imagePixels pixels = processCSV("/home/zedded2005/Code/c/ML/mnist_train.csv");
    short desiredNumber = getDesiredNumber(&pixels);
    printPixels(&pixels);
    NeuralLayer* test = setupNetwork(2, 10, 10, &pixels);
    printf("result : %lf\n", calculateCost(test, desiredNumber));
    freeNetwork(test);
    free(pixels.data);
    printf("done!\n");
}