#include "animate_genealogy.h"

AnimateGenealogy::AnimateGenealogy()
{}

void AnimateGenealogy::addAnimate (const Animate& animate, size_t ancestor_)
{
  animates_.emplace(animate.id(), Animate(animate));
  auto descendant = descendants_.find(ancestor_);
  if (descendant == descendants_.end()) {
    descendants_[ancestor_].insert(animate.id());
  } else {
    descendants_[ancestor_].insert(animate.id());
  }

}
