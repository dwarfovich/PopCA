#ifndef CHEMICAL_H
#define CHEMICAL_H

#include "population.h"
#include "auxiliary.h"

#include <memory>
#include <iostream>

class Chemical;

class Chemical : public Population
{
public:
  explicit Chemical (bool is_new);
  explicit Chemical (const Chemical& chem);
  virtual ~Chemical ();

  // Population actions
  virtual void reduction () {}
  virtual void feeding () {}
  virtual void migration (MigrationList *) {}
  virtual void reproduction (PopList *);
  virtual void acceptPopulation (Population* population);

  // Chemical's properties
  virtual Type type() const;
  void setMaxSize (double max_size);
  void setReproductionSize (double size);

  // Utility
  virtual void print() const;

private:
  double max_size_;
  double reproduction_size_;
};



#endif // CHEMICAL_H
