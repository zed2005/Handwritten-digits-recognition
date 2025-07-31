/**
 * \file mathFun.h
 * These are the mathematical functions that are needed for the network
 */

#ifndef MATHFUN_H
#define MATHFUN_H

    #ifndef M_E
    #define M_E 2.7182818284590452354	

    /// @brief Sigmoid function
    /// @param input the input of the mathematical function
    /// @return a number between 0 and 1 based on input 
    float sigmoid(float input);
    #endif

/// @brief ReLu function
/// @param input the function input
/// @return a number between -inf and +inf: max(0, input)
float ReLU(float input);

/// @brief random weight generator
/// @param min the minimum value we want
/// @param max the maximum value we want
/// @return a random float between the two numbers
float randWeight(signed int min, signed int max);

#endif