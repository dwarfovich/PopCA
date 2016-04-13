#ifndef GLOBALSTATISTICS_H
#define GLOBALSTATISTICS_H

#include "auxiliary.h"
#include "animate.h"
#include "project_parameters.h"

#include <QFile>
#include <QDataStream>
#include <QTextStream>

#include <fstream>
#include <list>
#include <map>

#include <iostream>
using std::cout;
using std::endl;

class World;

using AnimateList = std::list<Animate>;
using AnimateDump = std::map<size_t, AnimateList>;

class GlobalStatistics
{
  friend class World;
public:
  GlobalStatistics(World* world, size_t length = 0);

  size_t length () const { return length_; }

  void collectGlobalStatistics ();
  void saveGlobalStatistics (const std::string& filename);
  void saveAsText (const std::string& filename);

private:
  World* world_;
  size_t length_;
  size_t current_index_;
  PVector id_counter_;
  PVector pop_counter_;
  DVector max_mass_;
  DVector global_size_;
  DVector global_energy_;
  DVector total_mass_;
  DVector average_energy_;
  DVector average_mass_;
  DVector average_lifetime_;
  PVector max_lifetime_;
};

#endif // GLOBALSTATISTICS_H
