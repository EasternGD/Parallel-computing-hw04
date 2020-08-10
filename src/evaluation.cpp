#include "../inc/evaluation.h"

void randomize(myDataFormat &data)
{
    // std::cout << "\nREMEBER TO IMPLEMENT randomize " << __LINE__ << "@" << __FILE__ << endl;
    srand(1);
    double temp;
    string Stemp;
    size_t index_i = 0;
    // cout << data.row << endl;
    for (size_t i = 0; i < data.row; i++)
    {
        index_i = rand() % data.row;
        for (size_t j = 0; j < (data.column + data.remainColumn); j++)
        {
            // cout<<"["<<i+1<<" , "<<j+1<<"]";
            if (j < data.column)
            {

                temp = data.vector[i * data.column + j];
                data.vector[i * data.column + j] = data.vector[index_i * data.column + j];
                data.vector[index_i * data.column + j] = temp;
            }
            else
            {
                Stemp = data.remain[i * data.remainColumn + (j - data.column)];
                data.remain[i * data.remainColumn + (j - data.column)] = data.remain[index_i * data.remainColumn + (j - data.column)];
                data.remain[index_i * data.remainColumn + (j - data.column)] = Stemp;
            }
        }
    }

    // for (size_t i = 0; i < 1000; i++)
    // {
    //     for (size_t j = 0; j < (data.column + data.remainColumn); j++)
    //     {
    //         if (j < data.column)
    //         {
    //             cout << data.vector[i * data.column + j] << "\t";
    //         }
    //         else
    //         {
    //             cout << data.remain[i * data.remainColumn + (j - data.column)] << "\t";
    //         }
    //     }
    //     cout << endl;
    // }
}

void prepareTrainingData(const myDataFormat &data, myDataFormat &trainingData, size_t n, size_t i)
{
    // std::cout << "\nREMEBER TO IMPLEMENT prepareTrainingData " << __LINE__ << "@" << __FILE__ << endl;

    trainingData.row = data.row - data.row / n;

    trainingData.column = data.column;
    trainingData.remainColumn = data.remainColumn;
    trainingData.vector = new double[trainingData.row * trainingData.column];
    trainingData.remain = new string[trainingData.row * trainingData.remainColumn];
    // cout << " i = " << i << endl;
    if (i != 0)
    {
        for (size_t it = 0; it < i * (data.row / n); it++)
        {
            // cout << "it = " << it << endl;
            for (size_t jt = 0; jt < (trainingData.column + trainingData.remainColumn); jt++)
            {
                if (jt < trainingData.column)
                {
                    trainingData.vector[it * trainingData.column + jt] = data.vector[it * data.column + jt];
                    // cout << trainingData.vector[it * trainingData.column + jt] << "\t";
                }
                else
                {
                    trainingData.remain[it * trainingData.remainColumn + (jt - trainingData.column)] = data.remain[it * data.remainColumn + (jt - data.column)];
                    // cout << trainingData.remain[it * trainingData.remainColumn + (jt - trainingData.column)] << "\t";
                }
            }
            // cout << endl;
        }
    }

    for (size_t it = (data.row / n) * i; it < trainingData.row; it++)
    {
        // cout << " it = " << it << endl;
        for (size_t jt = 0; jt < (trainingData.column + trainingData.remainColumn); jt++)
        {
            if (jt < trainingData.column)
            {

                trainingData.vector[it * trainingData.column + jt] = data.vector[(it + (data.row / n)) * data.column + jt];
                // cout << trainingData.vector[it * trainingData.column + jt] << "\t";
            }
            else
            {
                trainingData.remain[it * trainingData.remainColumn + (jt - trainingData.column)] = data.remain[(it + (data.row / n)) * data.remainColumn + (jt - data.column)];
                // cout << trainingData.remain[it * trainingData.remainColumn + (jt - trainingData.column)] << "\t";
            }
        }
        // cout << endl;
    }
}

void prepareValidationData(const myDataFormat &data, myDataFormat &vData, size_t n, size_t i)
{
    // std::cout << "\nREMEBER TO IMPLEMENT prepareValidationData " << __LINE__ << "@" << __FILE__ << endl;
    vData.row = data.row / n;
    // cout << vData.row << endl;
    vData.column = data.column;
    // cout << vData.column << endl;
    vData.remainColumn = data.remainColumn;
    // cout << vData.remainColumn << endl;
    vData.vector = new double[vData.row * vData.column];
    vData.remain = new string[vData.row * vData.remainColumn];
    for (size_t iv = 0; iv < vData.row; iv++)
    {
        for (size_t jv = 0; jv < (vData.column + vData.remainColumn); jv++)
        {
            if (jv < vData.column)
            {
                vData.vector[iv * vData.column + jv] = data.vector[(iv + i * vData.row) * data.column + jv];
                // cout << vData.vector[iv * vData.column + jv] << ", ";
            }
            else
            {
                vData.remain[iv * vData.remainColumn + (jv - vData.column)] = data.remain[(iv + i * vData.row) * data.remainColumn + (jv - data.column)];
                // cout << vData.remain[iv * vData.remainColumn + (jv - vData.column)] << ", ";
            }
        }
        // cout << endl;
    }
}
struct record
{
    double wrongClassification = 0;
    double sum = 0;
    double max_Regression_Error = 0;
};
record *result;
int dataNum = 0;

void evaluateError(myDataFormat &data, const std::string &decisionString, size_t n, size_t i)
{
    // std::cout << "\nREMEBER TO IMPLEMENT evaluateError " << __LINE__ << "@" << __FILE__ << endl;

    double wrong_Regression_Error = 0;
    // for (size_t i = 0; i < decisionString.size() * data.row; i++)
    // {
    //     cout << data.result[i] << "|" << data.remain[i] << "\t";
    //     if (i % decisionString.size() == 1)
    //     {
    //         cout << endl;
    //     }
    // }
    //cout << "i = " << i << endl;
    if (i == 0)
        result = new record[data.resultNum];
    dataNum += data.row;
    for (size_t ie = 0; ie < data.remainColumn; ie++)
    {
        if (decisionString[ie] == 'C')
        {
            for (size_t j = 0; j < data.row; j++)
            {
                if (data.result[j * data.resultNum + ie] != data.remain[j * data.remainColumn + ie])
                {
                    result[ie].wrongClassification++;
                }
                if (j == data.row - 1  && i == (n - 1))
                {
                    cout << "\tClassification error rate:\t" << 100 * result[ie].wrongClassification / dataNum << "%" << endl;
                }
            }
        }
        else
        {
            for (size_t j = 0; j < data.row; j++)
            {
                if (data.result[j * data.resultNum + ie] != data.remain[j * data.remainColumn + ie])
                {
                    wrong_Regression_Error = abs((atof(data.result[j * data.resultNum + ie].c_str()) -
                                                  atof(data.remain[j * data.remainColumn + ie].c_str()))) /
                                             atof(data.remain[j * data.remainColumn + ie].c_str());

                    result[ie].sum += wrong_Regression_Error;

                    if (wrong_Regression_Error > result[ie].max_Regression_Error)
                    {
                        result[ie].max_Regression_Error = wrong_Regression_Error;
                        // cout << data.result[j * data.resultNum + ie] << "|" << data.remain[j * data.remainColumn + ie] << endl;
                        // cout << wrong_Regression_Error << endl;
                    }
                    else if (j == data.row - 1 && i == n - 1)
                    {

                        cout << "\tRegression average error rate:\t" << result[ie].sum * 100 / dataNum << "%" << endl;
                        //cout << "\tRegression maximum error rate:\t" << result[ie].max_Regression_Error * 100 << "%" << endl;
                    }
                }
            }
        }
    }

    // delete[] data.vector;
    // delete[] data.remain;
    // delete[] data.distance;
    // delete[] data.result;
    if (i == n - 1)
    {
        delete[] result;
    }
}
