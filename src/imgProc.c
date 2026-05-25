/**
 * \file imgProc.c
 * this is where the file processing functions are defined
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "imgProc.h"

/// @brief skips x lines (x images in this case)
/// @param fptr pointer to the file
/// @param destLine number of lines we want to skip
void lineSkipper(FILE* fptr, int destLine) {
    int c = 0;
    for(int curLines = 0; (c = fgetc(fptr)) != EOF;) {
        if(c == '\n') curLines++;
        if(curLines == destLine) return;
    }
}

/// @brief gets characters from a file till the endline character and stores them in a string
/// @param fptr pointer to the file
/// @param len length of the line 
/// @return pointer to the string
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

/// @brief processes one line, and stores it in an array
/// @param pixels the array we want to store the numbers in
/// @param line the line we process
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

/// @brief processes a single image and converts it into an array
/// @param fptr pointer to the file
/// @return the array of numbers
imagePixels setLayer(FILE* fptr) {
    imagePixels pixels;
    pixels.data = (short*)malloc(sizeof(short)*28*28+1);
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
    pixels = setLayer(fptr);
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