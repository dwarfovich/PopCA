#include "global_statistics.h"
#include "world.h"

GlobalStatistics::GlobalStatistics(World *world, size_t length)
  : world_(world)
  , length_(length)
  , current_index_(0)
{
  if (length > 0) {
    id_counter_.resize(length);
    pop_counter_.resize(length);
    max_mass_.resize(length);
    global_size_.resize(length);
    global_energy_.resize(length);
    total_mass_.resize(length);
    average_energy_.resize(length);
    average_mass_.resize(length);
    average_lifetime_.resize(length);
    max_lifetime_.resize(length);
  }
}

void GlobalStatistics::collectGlobalStatistics ()
{
  double total_energy = 0;
  double total_size = 0;
  double total_mass = 0;
  size_t n_pops = 0;
  double max_mass = 0;
  double average_lifetime = 0;
  size_t max_lifetime = 0;
  for (size_t i = 0; i < world_->sizeI(); ++i)
    for (size_t j = 0; j < world_->sizeJ(); ++j)
    {
      auto cell = world_->cell(i, j);
      auto pops = cell->populations;
      for (auto& pop: pops)
      {
        if (pop.second->type() == kAnimate) {
          auto a = static_cast<Animate*>(pop.second);
          if (a->mass() > max_mass)
            max_mass = a->mass();
          ++n_pops;
          total_energy += a->energy();
          total_mass += a->mass();
          total_size += a->size();
          average_lifetime += a->lifetime();
          if (a->lifetime() > max_lifetime)
            max_lifetime = a->lifetime();
        }
      }
    }

  id_counter_[current_index_] = Population::idCounter();
  pop_counter_[current_index_] = n_pops;
  max_mass_[current_index_] = max_mass;
  global_size_[current_index_] = total_size;
  global_energy_[current_index_] = total_energy;
  total_mass_[current_index_] = total_mass;
  average_energy_[current_index_] = total_energy / double(n_pops);
  average_mass_[current_index_] = total_mass / double(n_pops);
  average_lifetime_[current_index_] = average_lifetime / double(n_pops);
  max_lifetime_[current_index_] = max_lifetime;
  ++current_index_;
}

void GlobalStatistics::saveGlobalStatistics (const std::string& filename)
{
  std::ofstream stream (filename, std::ios::binary);
  if (stream.is_open()) {
    ProjectParameters::writeToStream(stream);
    WorldParameters::writeToStream(stream);

    cout << length_ << endl;
    size_t pvector_size = sizeof(size_t) * length_;
    size_t dvector_size = sizeof(double) * length_;
    stream.write(reinterpret_cast<char*>( &length_            ), sizeof(size_t));
    stream.write(reinterpret_cast<char*>( &id_counter_[0]     ), pvector_size);
    stream.write(reinterpret_cast<char*>( &pop_counter_[0]    ), pvector_size);
    stream.write(reinterpret_cast<char*>( &max_mass_[0]       ), dvector_size);
    stream.write(reinterpret_cast<char*>( &global_size_[0]    ), dvector_size);
    stream.write(reinterpret_cast<char*>( &global_energy_[0]  ), dvector_size);
    stream.write(reinterpret_cast<char*>( &total_mass_[0]     ), dvector_size);
    stream.write(reinterpret_cast<char*>( &average_energy_[0] ), dvector_size);
    stream.write(reinterpret_cast<char*>( &average_mass_[0]   ), dvector_size);
    stream.write(reinterpret_cast<char*>( &average_lifetime_[0] ), dvector_size);
    stream.write(reinterpret_cast<char*>( &max_lifetime_[0]   ), pvector_size);

    stream.close();
    saveAsText("");
  } else {
    std::cerr << "Error in saveGlobalStatistics(): can't open file" << endl;
  }
}

void GlobalStatistics::saveAsText (const std::string& filename)
{
  /*
  //QFile file (QString::fromStdString(filename));
  QFile file ("/home/boo/Results/res.txt");
  file.open(QIODevice::WriteOnly);
  QTextStream stream (&file);
  stream << unsigned(length_) << endl;

  stream << "id_counter: " << endl;
  for (size_t i = 0; i < length_; ++i)
    stream << id_counter_[i] << " ";
  stream << endl;

  stream << "pop_counter: " << endl;
  for (size_t i = 0; i < length_; ++i)
    stream << pop_counter_[i] << " ";
  stream << endl;

  stream << "max mass: " << endl;
  for (size_t i = 0; i < length_; ++i)
    stream << max_mass_[i] << " ";
  stream << endl;

  stream << "global_size: " << endl;
  for (size_t i = 0; i < length_; ++i)
    stream << global_size_[i] << " ";
  stream << endl;

  stream << "global_energy: " << endl;
  for (size_t i = 0; i < length_; ++i)
    stream << global_energy_[i] << " ";
  stream << endl;

  stream << "total_mass: " << endl;
  for (size_t i = 0; i < length_; ++i)
    stream << total_mass_[i] << " ";
  stream << endl;

  stream << "average_energy: " << endl;
  for (size_t i = 0; i < length_; ++i)
    stream << average_energy_[i] << " ";
  stream << endl;

  stream << "average_mass: " << endl;
  for (size_t i = 0; i < length_; ++i)
    stream << average_mass_[i] << " ";
  stream << endl;

  file.close();
  */
}
