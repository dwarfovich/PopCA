#ifndef ANIMATEPROPERTIES_H
#define ANIMATEPROPERTIES_H

#include <vector>

struct RationEntry
{
  friend bool operator< (const RationEntry& lhs, const RationEntry& rhs) {
    return lhs.feed_ability < rhs.feed_ability;
  }

  friend bool operator==(const RationEntry& lhs, unsigned rhs) {
    return lhs.id == rhs;
  }

  unsigned id;
  double feed_priority; // priority in [0; 1]
  double feed_ability;  // ability in [0; 1]
};

using Ration = std::vector<RationEntry>;

struct AnimateProperties {

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
};

#endif // ANIMATEPROPERTIES_H
