#pragma once
#include <string>		// for size_t
#include <omp.h>
#include <iostream>
#include <math.h>
using namespace std;
extern double minkowskiPower;

double euclidean(const double *v1, const double *v2, const size_t len);
double manhattan(const double *v1, const double *v2, const size_t len);
double chebyshev(const double *v1, const double *v2, const size_t len);
double minkowski(const double *v1, const double *v2, const size_t len);
double cosineSimilarity(const double *v1, const double *v2, const size_t len);

