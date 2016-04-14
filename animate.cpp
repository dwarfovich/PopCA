#include "animate.h"

using std::cout;
using std::endl;

GenealogyMap Animate::genealogy_;

Animate::Animate (bool is_new)
  : Population(is_new)
  , lifetime_(0)
{
#pragma omp critical
  {
    genealogy_.emplace(this->id(), GenealogyEntry{this->properties()});
  }
}

Animate::Animate (const Animate& animate)
  : Population(animate)
  , lifetime_(0)
  , energy_(animate.energy_)
  , energy_decay_(animate.energy_decay_)
  , preferred_energy_(animate.preferred_energy_)
  , birth_rate_(animate.birth_rate_)
  , death_rate_(animate.death_rate_)
  , migration_probability_(animate.migration_probability_)
  , migration_strength_(animate.migration_strength_)
  , mutation_probability_(animate.mutation_probability_)
  , mutation_strength_(animate.mutation_strength_)
  , food_trial_probability_(animate.food_trial_probability_)
  , ration_(animate.ration_)
  , environment_fitness_(animate.environment_fitness_)
{}


Animate::~Animate ()
{}

Type Animate::type() const
{
  return kAnimate;
}

double Animate::energy () const
{
  return energy_;
}

void Animate::reduction ()
{
  energy_ = (energy_ >= energy_decay_ ? energy_ - energy_decay_ : 0.);
  //energy_ -= std::pow(mass_, three_quarter);
  size_ -= size_ * death_rate_ * aux::sigmoid(1. - energy_, 10., 0.5);
}

void Animate::feeding ()
{
  // TODO: Optimize prey searching in case ration.size < map.size etc
  for(size_t r_i = 0; r_i < ration_.size() && energy_ < preferred_energy_; ++r_i)
  {
    auto prey_it = current_cell_->populations.find( ration_[r_i].id );
    if (prey_it == current_cell_->populations.end() || prey_it->second->size() <= WorldParameters::fatal_size) {
      //cout << "id " << ration_[r_i].id << " not found" << endl;
      continue;
    }
    else {
      eatPopulation(r_i, prey_it->second);
    }
  }

  // if energy is still low - try new food
  if (energy_ < preferred_energy_) {
    if(aux::doesEventOccur(food_trial_probability_)) {
      eatNewFood();
    }
  }
}

void Animate::eatPopulation (size_t ration_number, Population *prey)
{
  double desired_preys = size_ * (preferred_energy_ - energy_) / (prey->mass() * WorldParameters::feeding_efficacy_);
  double eaten_preys = aux::min(prey->size(),
                                prey->size() * ration_[ration_number].feed_ability * environment_fitness_[current_cell_->environment],
                                desired_preys);
  double obtained_energy = (eaten_preys * prey->mass() * WorldParameters::feeding_efficacy_) / size_;
  prey->decreaseSizeBy(eaten_preys);
  changeRationAbility(ration_number, WorldParameters::feed_ability_increase_*(1. - (desired_preys - eaten_preys) / desired_preys));
  if (energy_ + obtained_energy > 1)
    energy_ = 1.;
  else
    energy_ += obtained_energy;
}

void Animate::eatNewFood ()
{
  auto pops = current_cell_->populations;
  for (auto& prey: pops) {
    unsigned id = prey.second->id();
    if (id == id_) {
      continue;
    }
    if (std::find(ration_.begin(), ration_.end(), id) != ration_.end()) {
      RationEntry r {id, 0.8, WorldParameters::feed_ability_increase_};
      ration_.push_back(r);
      std::sort(ration_.begin(), ration_.end());
      eatPopulation(ration_.size() - 1, prey.second);
    }
  }
}

void Animate::changeRationAbility (size_t number, double amplitude)
{
  // TODO: Remove ration entry if its feed_ability <= 0.
  if (ration_[number].feed_ability + amplitude > 1.)
    ration_[number].feed_ability = 1.;
  else if (ration_[number].feed_ability + amplitude <= 0.)
    ration_[number].feed_ability = 0.;
  else
    ration_[number].feed_ability += amplitude;
}

void Animate::migration (MigrationList *migrants_queue)
{
  if( !aux::doesEventOccur(migration_probability_) )
    return ;

  auto migrant = new Animate (*this);
  double migrant_size = size_ * migration_strength_;
  // TODO: Improve migrants size handling
  if (migrant_size <= WorldParameters::fatal_size) {
    //std::cerr << "Error in migration: n_migrants <= 0:" << std::endl;
    delete migrant;
    return;
  }
  if (size_ - migrant_size <= WorldParameters::fatal_size) {
    //std::cerr << "Error in migration: size_ <= 0:" << std::endl;
    delete migrant;
    return;
  }

  migrant->size_ = migrant_size;
  size_ -= migrant_size;

  MigrationEvent me {migrant, migrationDirection()};
#pragma omp critical
  {
    migrants_queue->push_back(me);
  }
}

void Animate::reproduction (PopList * mutants_queue)
{
  // TODO: Check if size < fatal_size
  ++lifetime_;
  //double new_borns = birth_rate_  * size_ * aux::sigmoid(energy_, 30., 0.8); // ordinal child
  double new_borns = birth_rate_ * size_;
  if( aux::doesEventOccur(mutation_probability_) ) { // mutant child born
    double mutant_size = new_borns * mutation_strength_;
    size_ += new_borns - mutant_size;
    auto mutant = createMutant(mutant_size);
#pragma omp critical
    {
      mutants_queue->push_back(mutant);
    }
  } else {
    size_ += new_borns;
  }
}

Animate *Animate::createMutant(double size)
{
  auto mutant = new Animate (kNewSpecie);
  mutant->size_ = size;
  mutant->current_cell_ = current_cell_;
  mutant->energy_ = energy_;
  mutant->mass_ = propertyVariation(mass_, 1.);
  mutant->energy_decay_          = unitPropertyVariation(energy_decay_);
  mutant->preferred_energy_      = unitPropertyVariation(preferred_energy_);
  mutant->birth_rate_            = unitPropertyVariation(birth_rate_);
  mutant->death_rate_            = unitPropertyVariation(death_rate_);
  mutant->migration_probability_ = unitPropertyVariation(migration_probability_);
  mutant->migration_strength_    = unitPropertyVariation(migration_strength_);
  mutant->mutation_probability_  = unitPropertyVariation(mutation_probability_);
  mutant->mutation_strength_     = unitPropertyVariation(mutation_strength_);
  // TODO: Realize ration changes, and food_trial variability, and environment fitness
  mutant->food_trial_probability_= food_trial_probability_;
  mutant->setRation(ration_);
  mutant->setEnvironmentFitness(environment_fitness_);

#pragma omp critical
  {
    genealogy_.emplace(mutant->id(), GenealogyEntry{mutant->properties()});
    auto descendant = genealogy_.find(this->id());

    if (descendant != genealogy_.end()) {
      genealogy_.at(id()).descendants.insert(mutant->id());
    }
  }

  return mutant;
}

Direction Animate::migrationDirection () const
{
  return Direction(aux::randPosint(1, 8));
}

void Animate::acceptPopulation (Population *population)
{
  if (population->id() != id_) {
    std::cerr << "Error in Animate::acceptPopulation(): wring id" << std::endl;
    return;
  }

  auto animate = static_cast<Animate*>(population);
#pragma omp critical
  {
    energy_ = (energy_ * size_ + animate->energy_ * animate->size_)
        / (size_ + animate->size_);
    size_ += animate->size_;
  }
  delete animate;
}



double Animate::propertyVariation (double value, double max_amplitude)
{
  double amplitude = aux::randDouble(-value * max_amplitude, value * max_amplitude);
  return (value + amplitude >= 0. ? value + amplitude : min_double);
}

double Animate::unitPropertyVariation (double value, double max_amplitude)
{
  double amplitude = aux::randDouble(-value * max_amplitude, value * max_amplitude);
  if (amplitude < 0.)
    return (value + amplitude >= 0. ? value + amplitude : min_double);
  else
    return (value + amplitude <= 1. ? value + amplitude : 1.);
}

void Animate::setRation (const Ration &ration)
{
  ration_ = ration;
}

void Animate::setProperties (const AnimateProperties& properties)
{
  energy_ = properties.energy_;
  energy_decay_ = properties.energy_decay_;
  preferred_energy_ = properties.preferred_energy_;
  birth_rate_ = properties.birth_rate_;
  death_rate_ = properties.death_rate_;
  migration_probability_ = properties.migration_probability_;
  migration_strength_ = properties.migration_strength_;
  mutation_probability_ = properties.mutation_probability_;
  mutation_strength_ = properties.mutation_strength_;
  food_trial_probability_ = properties.food_trial_probability_;
  ration_ = properties.ration_;
}

unsigned Animate::lifetime () const
{
  return lifetime_;
}

AnimateProperties Animate::properties () const
{
  AnimateProperties ap;
  ap.energy_ = energy_;
  ap.energy_decay_ = energy_decay_;
  ap.preferred_energy_ = preferred_energy_;
  ap.birth_rate_ = birth_rate_;
  ap.death_rate_ = death_rate_;
  ap.migration_probability_ = migration_probability_;
  ap.migration_strength_ = migration_strength_;
  ap.mutation_probability_ = mutation_probability_;
  ap.mutation_strength_ = mutation_strength_;
  ap.food_trial_probability_ = food_trial_probability_;
  ap.ration_ = ration_;
  return ap;
}

const AnimateProperties& Animate::animateProperties (size_t id)
{
  return genealogy_.at(id).animate;
}

void Animate::setEnvironmentFitness (const std::vector<double>& fitness)
{
  environment_fitness_ = fitness;
}
