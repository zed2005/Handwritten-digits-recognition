#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "imgProc.h"

void lineSkipper(FILE* fptr, int destLine) {
    int c = 0;
    for(int curLines = 0; (c = fgetc(fptr)) != EOF;) {
        if(c == '\n') curLines++;
        if(curLines == destLine) return;
    }
}

/*char* getLine(FILE* fptr, signed int len) {
    char* line = malloc(2*len*sizeof(char)+1);
    fscanf(fptr, "%[^\n]", line);
    fgetc(fptr);
    return line;
}*/

char* getLine(FILE* fptr, int len) {
    char* line = malloc(2 * len + 1);

    if (fgets(line, 2 * len + 1, fptr)) {
        printf("%s\n", line);
        return line;
    } else {
        free(line);
        return NULL;
    }
}

void processLine(short* pixels, char* line) {
    char* tokens = strtok(line, ",");
    //printf("%s\n", tokens);
    for(int i = 0; tokens; i++) { 
        //printf("%s ", tokens);
        pixels[i] = atoi(tokens); 
        printf("%d ", pixels[i]);
        tokens = strtok(NULL, ",");
    }
    printf("\n");
}

imagePixels setLayer(int times, FILE* fptr) {
    imagePixels pixels;
    pixels.data = (short*)malloc(sizeof(short)*28*28);
    pixels.len = 28*28+1;
    char* line = getLine(fptr, pixels.len);
    processLine(pixels.data, line);
    //free(line);
    return pixels;
}

imagePixels processCSV(const char* fName) {
    static int linesProcesssed = 1;
    imagePixels pixels;
    pixels.data = NULL;
    pixels.len = 0;
    FILE* fptr = fopen(fName, "r");
    if(fptr == NULL) {
        printf("file not opened\n");
        fclose(fptr);
        return pixels;
    }
    lineSkipper(fptr, linesProcesssed);
    pixels = setLayer(linesProcesssed, fptr);
    linesProcesssed++;
    fclose(fptr);
    return pixels;
}

void printPixels(imagePixels* pixels) {
    for(int i = 0; i < pixels->len; i++) {
        printf("%d ", pixels->data[i]);
    }
}

short getDesiredNumber(imagePixels* pixels) {
    short current = pixels->data[0];
    for(size_t i = 1; i < pixels->len; i++) {
        pixels->data[i-1] = pixels->data[i];
    }
    return current;
}