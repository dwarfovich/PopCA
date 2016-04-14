#include "world.h"
#include "animate.h"
#include "chemical.h"
#include "project_parameters.h"

#include <set>
#include <map>
#include <memory>
#include <limits>
#include <time.h>
#include <iostream>
#include <string>
#include <chrono>

using namespace std;
using namespace std::chrono;

void initializeWorldFromFile (const std::string& filename, World& world);
void initializeEnvironment (World& world);

int main(int argc, char *argv[])
{
  srand(time(0));

  size_t size = 48;
  ProjectParameters::project_name = "Test project";
  ProjectParameters::result_filename = "res.bin";
  ProjectParameters::max_iteration = 5000;
  ProjectParameters::preferred_threads = 7;
  WorldParameters::sizeI = size;
  WorldParameters::sizeJ = size;
  WorldParameters::feeding_efficacy_ = 0.1;
  WorldParameters::feed_ability_increase_ = 0.1;
  WorldParameters::max_variation_amplitude_ = 0.1;
  WorldParameters::fatal_size = 0.1;
  WorldParameters::fatal_energy = 0.1;

  if (argc != 2) {
    std::cerr << "Wrong number of arguments: 1 needed" << std::endl;
    return -1;
  }
  std::string filename {argv[1]};
  World world;
  //initializeWorldFromFile(filename, world);
  world.initializeWorld();
  initializeEnvironment(world);

  // Adding chemicals
  Chemical* chem_prototype = new Chemical (true);
  chem_prototype->setMaxSize(200.);
  chem_prototype->setSize(100.);
  chem_prototype->setReproductionSize(100.);
  for (unsigned i = 0; i < WorldParameters::sizeI; ++i)
    for (unsigned j = 0; j < WorldParameters::sizeJ; ++j)
      world.addPopulation(i, j, new Chemical (*chem_prototype));
  delete chem_prototype;

  // Adding animates // n1
  auto ani_prototype = new Animate (true);
  ani_prototype->setSize(20.);
  ani_prototype->setMass(3.);
  Ration ration;
  ration.push_back(RationEntry{1, 1., 1.});

  AnimateProperties ap;
  ap.energy_ = 0.9;
  ap.energy_decay_ = 0.01;
  ap.preferred_energy_ = 1.;
  ap.birth_rate_ = 0.01;
  ap.death_rate_ = 0.01;
  ap.migration_probability_ = 0.1;
  ap.migration_strength_ = 0.01;
  ap.mutation_probability_ = 0.1;
  ap.mutation_strength_ = 0.01;
  ap.food_trial_probability_ = 0.1;
  ap.ration_ = ration;
  ani_prototype->setProperties(ap);
  DVector environment_fitness {0.7, 1., 0.7};
  ani_prototype->setEnvironmentFitness(environment_fitness);
  world.addPopulation(0, 0, ani_prototype);

  cout << "iterations: " << ProjectParameters::max_iteration << endl;
  world.setCollectStatistics(true);
  auto t1 = system_clock::now();

  world.play(ProjectParameters::max_iteration, ProjectParameters::preferred_threads);

  auto t2 = system_clock::now();
  auto timing = duration_cast<std::chrono::milliseconds>(t2 - t1).count();
  cout << "Duration: " << timing << " ms" << endl;
  //cout << "max size_t: " << std::numeric_limits<size_t>::max() << endl;
  cout << "pop counter: " << Animate::idCounter() << endl;

  world.saveStatistics(ProjectParameters::result_filename);
  //Animate::printGenealogy();

  // actually will be deleted in ~Cell
  //delete ani_prototype;

  return 0;
}

void initializeWorldFromFile (const std::string& filename, World &world)
{
  std::ifstream file (filename);
  if (file.is_open())
  {
    string line;
    getline(file, line);
    ProjectParameters::result_filename = line.substr(0, line.find(";", 0));
    getline(file, line);
    ProjectParameters::preferred_threads = stoul( line.substr(0, line.find(";", 0)) );
    getline(file, line);
    ProjectParameters::max_iteration = stoul( line.substr(0, line.find(";", 0)) );
    getline(file, line);
    WorldParameters::sizeI = stoul( line.substr(0, line.find(";", 0)) );
    getline(file, line);
    WorldParameters::sizeJ = stoul( line.substr(0, line.find(";", 0)) );
    getline(file, line);
    WorldParameters::fatal_size = stoul( line.substr(0, line.find(";", 0)) );
    getline(file, line);
    WorldParameters::fatal_energy = stoul( line.substr(0, line.find(";", 0)) );
    world.initializeWorld();
    
    // Add chemicals
    Chemical* chem_prototype = new Chemical (true);
    chem_prototype->setMaxSize(200.);
    chem_prototype->setSize(100.);
    chem_prototype->setReproductionSize(100.);
    for (unsigned i = 0; i < WorldParameters::sizeI; ++i)
      for (unsigned j = 0; j < WorldParameters::sizeJ; ++j)
        world.addPopulation(i, j, new Chemical (*chem_prototype));
    delete chem_prototype;
    
    // Add animates
    auto ani_prototype = new Animate (true);
    ani_prototype->setSize(20.);
    ani_prototype->setMass(2.);
    Ration ration;
    ration.push_back(RationEntry{1, 1., 1.});

    AnimateProperties ap;
    ap.energy_ = 0.9;
    ap.energy_decay_ = 0.01;
    ap.preferred_energy_ = 1.;
    ap.birth_rate_ = 0.01;
    ap.death_rate_ = 0.01;
    ap.migration_probability_ = 0.00;
    ap.migration_strength_ = 0.01;
    ap.mutation_probability_ = 0.1;
    ap.mutation_strength_ = 0.01;
    ap.food_trial_probability_ = 0.1;
    ap.ration_ = ration;
    ani_prototype->setProperties(ap);
    world.addPopulation(0, 0, ani_prototype);
    file.close();
  }
  else
    std::cerr << "Error: Unable to open config file" << std::endl;
}

void initializeEnvironment (World& world)
{
  size_t size_i = world.sizeI(), size_j = world.sizeJ();
  for(size_t i = 0; i < size_i; ++i)
    for(size_t j = 0; j < size_j; ++j)
      world.setEnvironment(i, j, 1);

  /*
  for(size_t i = 0; i < 3; ++i)
    for(size_t j = 0; j < size_j; ++j)
      world.setEnvironment(i, j, 0);

  for(size_t j = 0; j < size_j; ++j)
    world.setEnvironment(3, j, 1);

  for(size_t i = 4; i < size_i; ++i)
    for(size_t j = 0; j < size_j; ++j)
      world.setEnvironment(i, j, 2);
      */
}
