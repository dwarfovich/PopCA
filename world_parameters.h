#ifndef WORLDPARAMETERS_H
#define WORLDPARAMETERS_H

#include <stdlib.h>
#include <fstream>

struct WorldParameters {
  static void writeToStream (std::ofstream& out);

  static size_t sizeI;
  static size_t sizeJ;
  static double fatal_size;
  static double fatal_energy;
  static double feeding_efficacy_;
  static double max_variation_amplitude_;
  static double feed_ability_increase_;
};

#endif // WORLDPARAMETERS_H
