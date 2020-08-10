#include "inc/kNN.h"
#include "inc/evaluation.h"
#include "inc/distance.h"

void showInstruction(const char *exe)
{
    cout << "\n"
         << exe << " [instances filename] [n] [k] [# feature attributes] [# decision variables] [similarity] [decision string] [p]" << endl;
    cout << "\n[ninstances filename]: specify a file that contains instances of data with known attributes & decision variables.";
    cout << "\n\t\t Note that the instance file is in CSV (comma separated values) format.";
    cout << "\n[n]: specify the n parameter for n-fold cross validation.";
    cout << "\n[k]: a positive integer that defines the k in kNN algorithm.";
    cout << "\n[# feature attributes]: a positive integer indicating the number of feature attributes.";
    cout << "\n\t\t It should be noted feature attributes must be in the first few columns of the specified CSV files.";
    cout << "\n[# decision variables]: a positive integer indicating the number of decision variables.";
    cout << "\n[similarity]: one of the following integer: ";
    cout << "\n\t\t0: use Euclidean distance measure.";
    cout << "\n\t\t1: use Manhattan distance measure.";
    cout << "\n\t\t2: use Chebyshev distance measure.";
    cout << "\n\t\t3: use Minkowski distance measure.";
    cout << "\n\t\t4: use Cosine Similarity.";
    cout << "\n[decision string]: each character in the string is either 'C' or 'R' and corresponds to a decision variable type.";
    cout << "\n\t\tC: classification, the corresponding decision variable is treated as text labels.";
    cout << "\n\t\tR: regression, the corresponding decision variable is treated as real numbers.";
    cout << "\n[p]: the p parameter used for Minkowski distance measure.";
    cout << endl;
}

int main(int argc, char **argv)
{
    if (argc < 8)
    {
        showInstruction(argv[0]);
        return 255;
    }

    // step 0: get parameters from command line arguments
    const size_t n = atoi(argv[2]);
    const size_t k = atoi(argv[3]);
    const size_t noAttributes = atoi(argv[4]);
    const size_t noDecisionVariables = atoi(argv[5]);
    const size_t distanceMeasureToUse = atoi(argv[6]);
    const string decisionString = argv[7];

    // step 1: do some validations on obtained parameters
    if (decisionString.size() != noDecisionVariables)
    {
        cerr << "\nThe length of decision string(" << decisionString << ") should be equal to " << noDecisionVariables << "(# decision variables)." << endl;
        return 254;
    }
    if (distanceMeasureToUse < 0 || distanceMeasureToUse > 4)
    {
        cerr << "\nThe specified distance measure " << distanceMeasureToUse << " is invalid.";
        return 253;
    }

    // step 2: read instance data set
    myDataFormat data;
    readCSV(data, argv[1], noAttributes, decisionString);

    double t[6] = {0};
    // step 3: normalize the data set
    t[0] = omp_get_wtime();
    normalize(data);
    t[0] = omp_get_wtime() - t[0];

    // step 4: randomlize the data set
    t[1] = omp_get_wtime();
    randomize(data);
    t[1] = omp_get_wtime() - t[1];

    // step 5: repeat n-times (n = # folders)
    for (size_t i = 0; i < n; ++i)
    {
        myDataFormat trainingData, validationData;
        // step 5a: prepare i-th training data set
        double tmp = omp_get_wtime();
        prepareTrainingData(data, trainingData, n, i);

        // step 5b: prepare i-th validation data set
        prepareValidationData(data, validationData, n, i);
        t[2] += omp_get_wtime() - tmp;

        // step 5c: calculate the distance matrix beween the testing and the training data set.
        tmp = omp_get_wtime();
        calculateDistanceMatrix(trainingData, validationData, distanceMeasureToUse);
        t[3] += omp_get_wtime() - tmp;

        // step 5d: now make decisions for each testing data
        tmp = omp_get_wtime();
        doKNN(k, trainingData, validationData, decisionString);
        t[4] += omp_get_wtime() - tmp;

        // step 5e: calculate the error rate
        tmp = omp_get_wtime();
        evaluateError(validationData, decisionString, n, i);
        t[5] += omp_get_wtime() - tmp;
        freeAllocation(validationData);
        freeAllocation(trainingData);
    }
    cout << "\nT: ";
    t[0] *= n;
    t[1] *= n;
    for (int i = 0; i < 6; ++i)
    {
        cout << t[i] / n << ", ";
    }
    cout << endl;
    // step 6: finally, do some cleanup...
    freeAllocation(data);
    return 0;
}
