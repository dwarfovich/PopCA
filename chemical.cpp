#include "chemical.h"

Chemical::Chemical (bool is_new)
  : Population(is_new)
{}

Chemical::Chemical (const Chemical& chem)
  : Population(chem)
  , max_size_(chem.max_size_)
  , reproduction_size_(chem.reproduction_size_)
{}

Chemical::~Chemical()
{}

Type Chemical::type() const
{
  return kChemical;
}

void Chemical::setMaxSize (double max_size)
{
  max_size_ = max_size;
}

void Chemical::setReproductionSize (double size)
{
  reproduction_size_ = size;
}

void Chemical::reproduction (PopList *)
{
  if (size_ < max_size_ - reproduction_size_)
    size_ += reproduction_size_;
  else
    size_ = max_size_;
}

void Chemical::acceptPopulation (Population *population)
{
  if (population->id() == id_) {
    if (size_ + population->size() < max_size_)
      size_ += population->size();
    else
      size_ = max_size_;
  } else {
    std::cerr << "Error in Chemical::acceptPopulation(): wrong id" << std::endl;
  }
  delete population;
}

void Chemical::print() const
{
  Population::print();
  std::cout << "max_size: " << max_size_ << std::endl;
  std::cout << "reproduction_size: " << reproduction_size_ << std::endl;
  std::cout << "-------" << std::endl;
}
