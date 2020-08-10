#include "../inc/distance.h"

double minkowskiPower = 2.0;

double euclidean(const double *v1, const double *v2, const size_t len)
{
    // std::cout << "\nREMEBER TO IMPLEMENT EUCLIDEAN DISTANCE MEASURE " << __LINE__ << "@" << __FILE__;
    double ans = 0;
    for (size_t i = 0; i < len; i++)
    {
        ans += pow((v1[i] - v2[i]), 2);
    }
    ans = sqrt(ans);
    return ans;
}

double manhattan(const double *v1, const double *v2, const size_t len)
{

    // std::cout << "\nREMEBER TO IMPLEMENT MANHATTAN DISTANCE MEASURE " << __LINE__ << "@" << __FILE__;
    double ans = 0;
    for (size_t i = 0; i < len; i++)
    {
        ans += abs(v1[i] - v2[i]);
    }
    return ans;
}

double chebyshev(const double *v1, const double *v2, const size_t len)
{
    // std::cout << "\nREMEBER TO IMPLEMENT CHEBYSHEV DISTANCE MEASURE " << __LINE__ << "@" << __FILE__;
    double ans = 0;
    for (size_t i = 0; i < len; i++)
    {
        if (abs(v1[i] - v2[i]) > ans)
        {
            ans = abs(v1[i] - v2[i]);
        }
    }
    return ans;
}

double minkowski(const double *v1, const double *v2, const size_t len)
{
    // std::cout << "\nREMEBER TO IMPLEMENT MINKOWSKI DISTANCE MEASURE " << __LINE__ << "@" << __FILE__;
    double ans = 0;

    for (size_t i = 0; i < len; i++)
    {
        ans += abs(v1[i] - v2[i]);
    }
    ans = pow(ans, minkowskiPower / 1 / minkowskiPower);
    return ans;
}

double cosineSimilarity(const double *v1, const double *v2, const size_t len)
{
    // std::cout << "\nREMEBER TO IMPLEMENT COSINE SIMILARITY  " << __LINE__ << "@" << __FILE__;
    double dot = 0;
    double length_v1 = 0;
    double length_v2 = 0;
    for (size_t i = 0; i < len; i++)
    {
        dot += v1[i] * v2[i];
        length_v1 += v1[i] * v1[i];
        length_v2 += v2[i] * v2[i];
    }
    double ans = acos(dot / (sqrt(length_v1 * length_v2))) /M_PI ;
    return ans;
}
