#ifndef AUXILIARY_H
#define AUXILIARY_H

#include <stdlib.h>
#include <algorithm>
#include <vector>
#include <list>
#include <float.h>
#include <mutex>

static std::mutex mmm;

using posint = size_t;
using PVector = std::vector<posint>;
using DVector = std::vector<double>;
using DList   = std::list<double>;

constexpr const double min_double = -DBL_MAX;
constexpr const double max_double =  DBL_MAX;

namespace aux {

bool doesEventOccur (double probability);
/*
int randInt (int min, int max);

double randDouble (double min, double max);

size_t randPosint (size_t min, size_t max);
*/

template <typename T>
T randomInRange (const T& min, const T& max)
{
  if (min < max )
    return min + (rand() / T(RAND_MAX)) * (max - min);
  else if (min > max)
    return max + (rand() / T(RAND_MAX)) * (min - max);
  else
    return min;
}

double min(double n1, double n2, double n3);

double sigmoid(double value, double a, double c);

}

#endif // AUXILIARY_H
