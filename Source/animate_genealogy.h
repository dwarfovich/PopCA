#ifndef ANIMATEGENEALOGY_H
#define ANIMATEGENEALOGY_H

#include "animate.h"

#include <unordered_map>
#include <unordered_set>

class AnimateGenealogy
{
public:
  AnimateGenealogy();

  void addAnimate (const Animate& animate, size_t ancestor_);

private:
  std::unordered_map<size_t, Animate> animates_;
  std::unordered_map<size_t, std::unordered_set<size_t>> descendants_;
};

#endif // ANIMATEGENEALOGY_H
