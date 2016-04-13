#include "cell.h"

Cell::~Cell()
{
  for (auto& i : populations)
    delete i.second;
}
