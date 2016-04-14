#include "population.h"
#include "cell.h"

//std::atomic<size_t> Population::id_counter_ {0};
size_t Population::id_counter_ {0};

Population::Population(bool is_new)
  : mass_(1.)
{
  if (is_new){
#pragma omp critical
    {
    id_ = ++id_counter_;
    }
    current_cell_ = nullptr;
  }
}

Population::Population(const Population& pop)
  : current_cell_(pop.current_cell_)
  , id_(pop.id_)
  , size_(pop.size_)
  , mass_(pop.mass_)
{}

Population::~Population()
{}

void Population::setCurrentCell (Cell* cell)
{
  current_cell_ = cell;
}

size_t Population::id() const
{
  return id_;
}

double Population::size () const
{
  return size_;
}

double Population::mass () const
{
  return mass_;
}

void Population::setSize(double new_size)
{
  size_ = new_size;
}

void Population::setMass(double new_mass)
{
  mass_ = new_mass;
}

void Population::print () const
{
  std::cout << "id: " << id_ << std::endl;
  std::cout << "size: " << size_ << std::endl;
  std::cout << "mass: " << mass_ << std::endl;
}

void Population::decreaseSizeBy (double size)
{
  size_ -= size;
}

size_t Population::idCounter ()
{
  return id_counter_;
}

void Population::acceptPopulation (Population* population)
{
  if (id_ != population->id())
    std::cerr << "Error in Population::acceptPopulation(): wrong id" << std::endl;
}
