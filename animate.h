#ifndef ANIMATE_H
#define ANIMATE_H

#include "auxiliary.h"
#include "population.h"
#include "animate_properties.h"
#include "cell.h"
#include "world_parameters.h"

#include <memory>
#include <vector>
#include <algorithm>
#include <map>
#include <set>
#include <iostream>

class Animate;

struct GenealogyEntry {
  explicit GenealogyEntry(const AnimateProperties& animate_properties)
    : animate(animate_properties)
  {}

  AnimateProperties animate;
  std::unordered_set<size_t> descendants;
};

using GenealogyMap = std::unordered_map<size_t, GenealogyEntry>;

class Animate : public Population
{
  friend class GlobalStatistics;

public:
  explicit Animate (bool is_new);
  explicit Animate (const Animate& animate);
  virtual ~Animate ();
  Animate* createMutant (double size);

  // Population actions
  virtual void reduction ();
  virtual void feeding ();
  virtual void migration (MigrationList * migrants_queue);
  virtual void reproduction (PopList * mutants_queue);
  virtual void acceptPopulation (Population* population);
  void eatPopulation (size_t ration_number, Population* prey);
  void eatNewFood ();
  void changeRationAbility (size_t number, double amplitude);

  // Animate properties
  virtual Type type() const;
  double energy () const;
  void setRation (const Ration& ration);
  void setEnvironmentFitness (const std::vector<double>& fitness);
  void setProperties (const AnimateProperties& properties);
  AnimateProperties properties () const;
  Direction migrationDirection() const;
  unsigned lifetime () const;
  static void setFatalValues (double fatal_energy, double fatal_size);
  static void setFeedingEfficacy (double value);

  // Utility
  static const AnimateProperties& animateProperties (size_t id);

  static void setWorldParameters (const WorldParameters& parameters);

private:
  static double propertyVariation (double doublevalue,
                                   double max_amplitude = WorldParameters::max_variation_amplitude_);
  static double unitPropertyVariation (double value,
                                       double max_amplitude = WorldParameters::max_variation_amplitude_);
protected:
  size_t lifetime_;
  double energy_;
  double energy_decay_;
  double preferred_energy_;
  double birth_rate_;
  double death_rate_;
  double migration_probability_;
  double migration_strength_;
  double mutation_probability_;
  double mutation_strength_;
  double food_trial_probability_;
  Ration ration_;
  std::vector<double> environment_fitness_;

  static GenealogyMap genealogy_;
};

#endif // ANIMATE_H
