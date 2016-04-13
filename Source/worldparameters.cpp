#include "worldparameters.h"

size_t WorldParameters::sizeI = 0;
size_t WorldParameters::sizeJ = 0;
double WorldParameters::fatal_size   = 0.001;
double WorldParameters::fatal_energy = 0.001;
double WorldParameters::feeding_efficacy_ = 0.01;
double WorldParameters::max_variation_amplitude_ = 0.1;
double WorldParameters::feed_ability_increase_ = 0.1;

void WorldParameters::writeToStream (std::ofstream& out)
{
  size_t s_size = sizeof(size_t);
  size_t d_size = sizeof(double);
  out.write(reinterpret_cast<const char*>( &WorldParameters::sizeI ), s_size );
  out.write(reinterpret_cast<const char*>( &WorldParameters::sizeJ ), s_size );
  out.write(reinterpret_cast<const char*>( &WorldParameters::fatal_size ),   d_size );
  out.write(reinterpret_cast<const char*>( &WorldParameters::fatal_energy ), d_size );
  out.write(reinterpret_cast<const char*>( &WorldParameters::feeding_efficacy_ ), d_size );
  out.write(reinterpret_cast<const char*>( &WorldParameters::max_variation_amplitude_ ), d_size );
  out.write(reinterpret_cast<const char*>( &WorldParameters::feed_ability_increase_ ), d_size );
}
