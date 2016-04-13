#include "auxiliary.h"

bool aux::doesEventOccur (double probability)
{
  return rand() / double(RAND_MAX) <= probability;
}

/*
int aux::randInt (int min, int max)
{
  return min + (rand() % (max - min));
}

double aux::randDouble (double min, double max)
{
  return min + (rand() / double(RAND_MAX)) * (max - min);
}

size_t aux::randSize (size_t min, size_t max)
{
  return min + (rand() % (max - min + 1));
}
*/

double aux::min(double n1, double n2, double n3)
{
  return std::min(n1, std::min(n2, n3));
}

double aux::sigmoid(double value, double a, double c)
{
  return 1. / (1. + std::exp(-a * (value - c)));
}

/*
template <typename T>
T aux::randomInRange (const T& min, const T& max)
{

}
*/
