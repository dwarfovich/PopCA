#ifndef POPULATION_H
#define POPULATION_H

#include "auxiliary.h"

#include <omp.h>
#include <memory>
//#include <atomic>
#include <unordered_set>
#include <unordered_map>
#include <list>
#include <iostream>

struct Cell;
class Population;

using PopMap = std::unordered_map<unsigned, Population*>;
using PopList = std::list<Population*>;

enum Type { kBase = 0, kChemical = 1, kAnimate = 2 };
enum {kOldSpecie = 0, kNewSpecie = 1};
enum Direction
{
  NoDirection = 0,
  North, NorthEast, East, SouthEast,
  South, SouthWest, West, NorthWest
};

struct MigrationEvent
{
  Population* population;
  Direction direction;
};
using MigrationList = std::list<MigrationEvent>;
class Population
{
public:
  explicit Population (bool is_new);
  explicit Population (const Population& pop);
  virtual ~Population () = 0;

  // Population actions
  virtual void reduction () = 0;
  virtual void feeding () = 0;
  virtual void migration (MigrationList *) = 0;
  virtual void reproduction (PopList *) = 0;
  virtual void acceptPopulation (Population* population) = 0;

  // Population properties
  virtual Type type() const { return kBase; }
  size_t id   () const;
  double size () const;
  double mass () const;
  void setMass (double new_mass);
  void setSize (double new_size);
  void decreaseSizeBy (double size);
  void setCurrentCell (Cell* cell);
  Cell* currentCell () { return current_cell_; }

  // Utility
  static size_t idCounter ();
  virtual void print () const;
  virtual void printPersonal () {}

protected:
  static size_t id_counter_;
  Cell* current_cell_;
  size_t id_;
  double size_;
  double mass_;
};

#endif // POPULATION_H
