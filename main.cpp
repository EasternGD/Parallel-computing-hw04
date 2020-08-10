#include <iostream>
#include "inc/kNN.h"
#include "inc/distance.h"

using namespace std;

void showInstruction(const char *exe) {
	cout << "\n" << exe << "[instances filename] [questions filename] [k] [# feature attributes] [# decision variables] [similarity] [decision string] [p]" << endl;
	cout << "\[ninstances filename]: specify a file that contains instances of data with known attributes & decision variables.";
	cout << "\n[questions filenmae]: specify a file that contains questions to be answered."; 
	cout << "\n\t\t Note that both files are in CSV (comma separated values) format.";
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

int main(int argc, char **argv) {
	if (argc < 8) {
		showInstruction(argv[0]);
		return 255;
	}

	// step 0: get parameters from command line arguments
	size_t k = atoi(argv[3]);
	size_t noAttributes = atoi(argv[4]);
	size_t noDecisionVariables = atoi(argv[5]);
	size_t distanceMeasureToUse = atoi(argv[6]);
	string decisionString = argv[7];

	// step 1: do validations on obtained parameters
	if (decisionString.size() != noDecisionVariables) {
		cerr << "\nThe length of decision string(" << decisionString << ") should be equal to " << noDecisionVariables << "(# decision variables)." << endl;
		return 254;
	}
	if (distanceMeasureToUse < 0 || distanceMeasureToUse > 4) {
		cerr << "\nThe specified distance measure " << distanceMeasureToUse << " is invalid.";
		return 253;
	}
	if (distanceMeasureToUse == 3) {
		if( argc != 9 ) {
			cerr << "\np must be given if Minkowski distance measure is used!";
			return 252;
		}
		minkowskiPower = atof( argv[8] );	
	}

	// step 2: read instance data set 
	myDataFormat data;
	readCSV(data, argv[1], noAttributes, decisionString);

	// step 3: read questions to be answered
	myDataFormat decisions;
	readCSV(decisions, argv[2], noAttributes);

	// step 4: calculate the distance matrix
	calculateDistanceMatrix(data, decisions, distanceMeasureToUse);

	// step 5: now make decisions for each question
	doKNN(k, data, decisions, decisionString);

	// step 6: save results.
	// saveCSV(decisions, argv[2]);

	// step 7: finally, do some cleanup...
	freeAllocation(data);
	freeAllocation(decisions);

	return 0;
}
