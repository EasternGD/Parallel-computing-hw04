#pragma once
#include "kNN.h"
#include <omp.h>
#include <string>
#include <iostream>
#include <random>
#include <time.h>
using namespace std;

// This function randomly order the original data set
void randomize(myDataFormat &data);

// This function prepares the training data for n-fold cross validation
void prepareTrainingData(const myDataFormat& data, myDataFormat& trainingData, size_t n, size_t i);

// This function prepares the validation data for n-fold cross validation
void prepareValidationData(const myDataFormat& data, myDataFormat& validationdata, size_t n, size_t i);

// This function evaluates classification error or regression error for n-fold cross validation
void evaluateError(myDataFormat& validationData, const std::string &decisionString, size_t n, size_t i);
