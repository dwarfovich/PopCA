#ifndef CELL_H
#define CELL_H

#include "population.h"
#include "chemical.h"

#include <memory>
#include <vector>

struct Cell;

using CellVector = std::vector<Cell>;
using CellMatrix = std::vector<CellVector>;

struct Cell
{
  Cell() {}
  ~Cell();
  PopMap populations;
  void clear () {}

  size_t posI;
  size_t posJ;
  size_t environment = 0;
};

#endif // CELL_H
