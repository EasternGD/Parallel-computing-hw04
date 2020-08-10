#include "../inc/kNN.h"
#include "../inc/distance.h"

using namespace std;

void readCSV(myDataFormat &output, const char *filename, size_t noAttributes, const string &decisions)
{
    // cout << "\nDO MODIFY readCSV " << __LINE__ << "@" << __FILE__ << "\n";
    cout << endl;
    ifstream inp(filename);
    if (!inp)
    {
        cerr << "\nError opening file: " << filename;
    }

    // let's count # lines first.
    size_t noLines = 0;
    string buf;
    while (inp.good())
    {
        getline(inp, buf);
        noLines++;
    }
    noLines -= 1;
    // cout << noLines << endl;
    // now you should allocate memory for storing data
    output.column = noAttributes;
    output.row = noLines;
    output.vector = new double[output.column * output.row];
    if (decisions.size() > 0)
    {
        output.remainColumn = decisions.size();
        output.remain = new string[output.row * output.remainColumn];
    }
    // re-read the file and this time read the actual data into allocated memory
    inp.clear();
    inp.seekg(0, inp.beg);

    for (size_t i = 0; i < noLines; ++i)
    {
        getline(inp, buf);
        size_t head = 0;
        string tmp(buf);
        // now we are reading attributes one by one that are separated by comma ','

        // cout << endl;
        for (size_t j = 0; j < noAttributes; ++j)
        {
            auto tail = tmp.find(',', head);
            auto attribute = atof(tmp.c_str() + head);
            // now I read a attribute in real number, what should you do with it?
            output.vector[i * noAttributes + j] = attribute;
            head = tail + 1;

            // cout << output.vector[i * noAttributes + j] << "\t";
        }
        // now we are reading decision variables one by one that are separated by comma ','
        for (size_t j = 0; j < decisions.size(); ++j)
        {
            auto tail = tmp.find(',', head);
            auto variable = tmp.substr(head, tail - head);

            // now I read a decision variable in string, what should you do with it?

            output.remain[i * decisions.size() + j] = variable;
            head = tail + 1;

            // cout << output.remain[i * decisions.size() + j] << "\t";
        }
    }

    inp.close();
}

void saveCSV(const myDataFormat &output, const char *filename)
{
    cout << "\nDO IMPLEMENT saveCSV " << __LINE__ << "@" << __FILE__;
    ofstream fileOutput(filename, ios::out);
    if (!fileOutput)
    {
        cerr << "\nError opening file: " << filename;
    }
    else
    {
        for (size_t i = 0; i < output.row; i++)
        {
            for (size_t j = 0; j < (output.resultNum + output.column); j++)
            {

                if (j < output.column)
                {
                    fileOutput << output.vector[i * output.column + j] << ", ";
                }
                else
                {
                    fileOutput << output.result[i * output.resultNum + j - output.column] << ", ";
                }
            }
            fileOutput << endl;
        }
        fileOutput.close();
    }

    ifstream fileInput(filename, ios::in);
    if (!fileInput)
    {
        cerr << "\nError opening file: " << filename;
    }
    else
    {
        string line;
        while (fileInput.good())
        {
            getline(fileInput, line);
            cout << "\n"
                 << line << endl;
        }
    }
    fileInput.close();
}

void normalize(myDataFormat &data)
{
    // cout << "\nDO IMPLEMENT normalize " << __LINE__ << "@" << __FILE__ << endl;
    // for each feature
    // 		compute its average (e.g. avg) and standard deviation (dev) throughout all instances
    // 		replace each feature value v by (v - avg) / dev
    double avg = 0, dev = 0;

    for (size_t i = 0; i < data.row * data.column; i++)
    {
        avg += data.vector[i];
    }

    // #pragma omp single
    avg /= (data.column * data.row);
    // cout << "\n avg = " << avg << endl;
    // #pragma omp parallel for schedule(static, 1000) \
//     reduction(+                                 \
//               : dev)
    for (size_t i = 0; i < data.row * data.column; i++)
    {
        dev += pow(data.vector[i] - avg, 2);
    }
    // #pragma omp single
    dev = sqrt(dev / (data.column * data.row));
    // cout << "\n dev = " << dev << endl;
    // #pragma omp parallel for
    for (size_t i = 0; i < data.row * data.column; i++)
    {

        data.vector[i] = (data.vector[i] - avg) / dev;
        // cout << data.vector[i * data.column + j] << ", ";

        // for (size_t j = 0; j < data.remainColumn; j++)
        // {
        // cout << data.remain[i * data.remainColumn + j] << ", ";
        // }
        // cout << endl;
    }
}

void calculateDistanceMatrix(const myDataFormat &data, myDataFormat &decisions, size_t distanceMeasureToUse)
{
    decisions.distance = new double[decisions.row * data.row];

    switch (distanceMeasureToUse)
    {
    case 0:
#pragma omp parallel for if (decisions.row * data.row >= 3000) \
    schedule(dynamic, 1072)
        for (size_t i = 0; i < decisions.row * data.row; ++i)
        {
            decisions.distance[i] =
                euclidean((decisions.vector + (i / data.row * decisions.column)),
                          (data.vector + (i * decisions.column) % (data.row * data.column)),
                          decisions.column);
        }
        break;
    case 1:
#pragma omp parallel for if (decisions.row * data.row >= 3000) \
    schedule(dynamic, 1072)
        for (size_t i = 0; i < decisions.row * data.row; ++i)
        {

            decisions.distance[i] =
                manhattan((decisions.vector + (i / data.row * decisions.column)),
                          (data.vector + (i * decisions.column) % (data.row * data.column)),
                          decisions.column);
        }
        break;
    case 2:
#pragma omp parallel for if (decisions.row * data.row >= 3000) \
    schedule(dynamic, 1072)
        for (size_t i = 0; i < decisions.row * data.row; ++i)
        {

            decisions.distance[i] =
                chebyshev((decisions.vector + (i / data.row * decisions.column)),
                          (data.vector + (i * decisions.column) % (data.row * data.column)),
                          decisions.column);
        }
        break;
    case 3:
#pragma omp parallel for if (decisions.row * data.row >= 3000) \
    schedule(dynamic, 1072)
        for (size_t i = 0; i < decisions.row * data.row; ++i)
        {

            decisions.distance[i] =
                minkowski((decisions.vector + (i / data.row * decisions.column)),
                          (data.vector + (i * decisions.column) % (data.row * data.column)),
                          decisions.column);
        }
        break;

    case 4:
#pragma omp parallel for if (decisions.row * data.row >= 3000) \
    schedule(dynamic, 1072)
        for (size_t i = 0; i < decisions.row * data.row; ++i)
        {

            decisions.distance[i] =
                cosineSimilarity((decisions.vector + (i / data.row * decisions.column)),
                                 (data.vector + (i * decisions.column) % (data.row * data.column)),
                                 decisions.column);
        }
    }
}
void doKNN(const size_t k, const myDataFormat &instances, myDataFormat &decisions, const string &decisionString)
{
    // cout << "\nDO IMPLEMENT doKNN " << __LINE__ << "@" << __FILE__ << endl;

    decisions.resultNum = decisionString.size();
    decisions.result = new string[decisions.resultNum * decisions.row];
    int *index = new int[k];
    //for Q1
    //Find k smallest distances and their corresponding instances.

    for (size_t e = 0; e < decisions.row; e++)
    {
        for (size_t i = 0; i < k; i++)
        {
            index[i] = e * instances.row;
            for (size_t j = 0; j < instances.row; j++)
            {
                if (decisions.distance[e * instances.row + j] <= decisions.distance[index[i]])
                {
                    index[i] = e * instances.row + j;
                }
            }

            decisions.distance[index[i]] = INT64_MAX;
            // cout << index[e * k + i] << " ";
        }
        // cout << endl;
        size_t count = 0;
        size_t maxCount = 0;

        for (size_t j = 0; j < decisionString.size(); j++)
        {
            // cout << instances.remain[(index[e * k + i] % instances.row) * decisionString.size() + j];
            double sum = 0;

            //do classification
            if (decisionString[j] == 'C')
            {
                for (size_t i = 0; i < k; i++)
                {
                    for (size_t x = 0; x < k; x++)
                    {
                        if (instances.remain[(index[i] % instances.row) * decisionString.size() + j] == instances.remain[(index[i] % instances.row) * decisionString.size() + j])
                        {
                            count++;
                        }

                        if (count > maxCount)
                        {
                            maxCount = count;
                        }

                        if (maxCount >= k / 2)
                        {
                            decisions.result[e * decisions.resultNum + j] = instances.remain[(index[i] % instances.row) * decisionString.size() + j];
                        }
                    }
                }
            }
            //do regression
            else
            {
                for (size_t i = 0; i < k; i++)
                {
                    sum += stod(instances.remain[(index[i] % instances.row) * decisionString.size() + j]) / k;
                }
                decisions.result[e * decisions.resultNum + j] = to_string(sum);
            }
        }
        // cout << endl;
    }

    delete[] index;
}

void freeAllocation(myDataFormat &data)
{
    // cout << "\nDO IMPLEMENT freeAllocation " << __LINE__ << "@" << __FILE__<<endl;
    //for all
    delete[] data.vector;
    if (data.resultNum > 0)
    {
        delete[] data.distance;
        delete[] data.result;
    }
    if (data.remainColumn > 0)
    {
        delete[] data.remain;
    }
}
