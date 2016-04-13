#ifndef LABELLEDANIMATE_H
#define LABELLEDANIMATE_H

#include "auxiliary.h"
#include "animate.h"

#include <memory>
#include <list>

/*
class LabelledAnimate;

//using LPopulation* = std::shared_ptr<LabelledAnimate>;

class LabelledAnimate : public Animate
{
public:
  LabelledAnimate(bool is_new);

  // Population actions
  virtual void reduction () {
    Animate::reduction();
    size_stat_.push_back(size_);
    energy_stat_.push_back(energy_);
  }

  virtual void feeding () { Animate::feeding(); }
  virtual Population* reproduction () {
    double s = size_;
    Population* p =  Animate::reproduction();
    borns_stat_.push_back(size_ - s);
    return p;
  }
  virtual Population* migration () { return Animate::migration(); }
  virtual void acceptPopulation (const Population* object)
  { Animate::acceptPopulation(object);}

  // Animate properties
  virtual Type type() const { return kAnimate; }

  const DList& sizeStatistics () const { return size_stat_; }
  const DList& energyStatistics () const { return energy_stat_; }
  const DList& bornsStatistics () const { return borns_stat_; }

private:
  DList size_stat_;
  DList energy_stat_;
  DList borns_stat_;

};
*/

#endif // LABELLEDANIMATE_H
