#ifndef IMGPROC_H
#define IMGPROC_H

typedef struct imagePixels {
    unsigned int len;
    short* data;
} imagePixels;

imagePixels processCSV(const char* fName);
void printPixels(imagePixels* pixels);
short getDesiredNumber(imagePixels* pixels);

#endif