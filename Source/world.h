#ifndef WORLD_H
#define WORLD_H

#include "cell.h"
#include "environment.h"
#include "population.h"
#include "globalstatistics.h"
#include "animate.h"
#include "worldparameters.h"

#include <sys/sysinfo.h>
#include <omp.h>
#include <sstream>
#include <stdlib.h>
#include <vector>
#include <list>
#include <memory>
#include <chrono>
#include <string>

using StatPtr = std::shared_ptr<GlobalStatistics>;

struct Bound {
  size_t a1;
  size_t a2;
};

using BoundVector = std::vector<Bound>;

class World
{
public:
  explicit World ();
  ~World ();

  void initializeWorld ();

  size_t sizeI () const;
  size_t sizeJ () const;
  Cell* cell (unsigned posI, unsigned posJ);

  void addPopulation (size_t posI, size_t posJ, Population* population);
  void addPopulation (Cell* cell, Population* population);
  PopMap& populations (size_t posI, size_t posJ);
  Cell* giveNeighborCell (Cell* const cell, Direction direction);
  void setWorldParameters (const WorldParameters& parameters);
  // Statistics
  void setCollectStatistics (bool b);
  void saveStatistics (const std::string& filename, bool as_text = false);
  void setEnvironment(size_t posI, size_t posJ, size_t type);

  // Playing
  void play (size_t iterations, size_t desired_threads = 7);
  void processCell (size_t posI, size_t posJ, PopList* mut_list, MigrationList* mig_list);
  void processMutants ();
  void processMigrants ();

  void printWorld () const;
  BoundVector getBounds (size_t rows, size_t n_threads);
  size_t findNumberOfThreads(size_t desired_number, size_t rows) const;
  void clear();
  void printIteration (size_t current_iteration, size_t max_iteration, bool print_memory) const;
  void printMemory () const;

private:
  CellMatrix world_;
  PopList mutants_;
  MigrationList migrants_;
  StatPtr statistics_;
  bool collect_statistics_;
};

#endif // WORLD_H
