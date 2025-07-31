/**
 * \file imgProc.h
 * This part turns the data of a nxn CSV file into an array of length n*n
 */

#ifndef IMGPROC_H
#define IMGPROC_H

/// @brief this is the output array
/// @param len the length of the array
/// @param data the actual array 
typedef struct imagePixels {
    unsigned int len;
    short* data;
} imagePixels;

/// @brief main file processing function
/// @param fName the name of the file
/// @return the array of n*n numbers
imagePixels processCSV(const char* fName);

/// @brief displays the array
/// @param pixels the array
void printPixels(imagePixels* pixels);

/// @brief gets the first number in training images
/// @param pixels the processed file details
/// @return the first element (in training images its the actual number)
short getDesiredNumber(imagePixels* pixels);

#endif