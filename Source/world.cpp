#include "world.h"

World::World ()
  : statistics_(nullptr)
{}

void World::initializeWorld ()
{
  world_.resize(WorldParameters::sizeI, CellVector(WorldParameters::sizeJ));
  for (size_t i = 0; i < WorldParameters::sizeI; ++i)
    for (size_t j = 0; j < WorldParameters::sizeJ; ++j) {
      world_[i][j].posI = i;
      world_[i][j].posJ = j;
    }
}

World::~World()
{}

size_t World::sizeI () const
{
  return world_.size();
}

size_t World::sizeJ () const
{
  return (sizeI() > 0 ? world_[0].size() : 0);
}

void World::addPopulation(size_t posI, size_t posJ, Population* population)
{
  if (posI < WorldParameters::sizeI && posJ < WorldParameters::sizeJ)
    addPopulation( cell(posI, posJ), std::move(population) );
  else
    std::cerr << "Error in addPopulation: wrong position" << endl;
}

void World::addPopulation (Cell* cell, Population *population)
{
  auto pop = cell->populations.find( population->id() );
  if (pop != cell->populations.end()) // Population found
    pop->second->acceptPopulation( population );
  else { // Population not found
    population->setCurrentCell(cell);
    cell->populations[population->id()] = population;
  }
}

PopMap &World::populations(size_t posI, size_t posJ)
{
  return world_[posI][posJ].populations;
}

void World::printWorld () const
{
  for(size_t i = 0; i < sizeI(); ++i)
  {
    for (size_t j = 0; j < sizeJ(); ++j)
      cout << world_[i][j].populations.size() << " ";
    cout << endl;
  }
  cout << "----------------" << endl << endl;
}

Cell* World::giveNeighborCell (Cell * const cell, Direction direction)
{
  size_t curI = 0, curJ = 0;
  /*
  // TODO: Improve cell searching
  bool is_found = false;
  for (size_t i = 0; i < sizeI() && !is_found; ++i)
    for (size_t j = 0; j < sizeJ() && !is_found; ++j)
      if (cell == &world_[i][j]) {
        curI = i, curJ = j;
        is_found = true;
        i = sizeI(), j = sizeJ();
      }
  if (!is_found)
    std::cerr << "Error in neighborCell(): cell not found" << endl;
    */
  curI = cell->posI;
  curJ = cell->posJ;

  size_t destI, destJ;
  switch (direction) {
  case North:
    destI = (curI >= 1 ? curI - 1 : sizeI() - 1);
    destJ = curJ;
    break;
  case NorthEast:
    destI = (curI >= 1 ? curI - 1 : sizeI() - 1);
    destJ = (curJ + 1 < sizeJ() ? curJ + 1 : 0);
    break;
  case East:
    destI = curI;
    destJ = (curJ + 1 < sizeJ() ? curJ + 1 : 0);
    break;
  case SouthEast:
    destI = (curI + 1 < sizeI() ? curI + 1 : 0);
    destJ = (curJ + 1 < sizeJ() ? curJ + 1 : 0);
    break;
  case South:
    destI = (curI + 1 < sizeI() ? curI + 1 : 0);
    destJ = curJ;
    break;
  case SouthWest:
    destI = (curI + 1 < sizeI() ? curI + 1 : 0);
    destJ = (curJ >= 1 ? curJ - 1 : sizeJ() - 1);
    break;
  case West:
    destI = curI;
    destJ = (curJ >= 1 ? curJ - 1 : sizeJ() - 1);
    break;
  case NorthWest:
    destI = (curI >= 1 ? curI - 1 : sizeI() - 1);
    destJ = (curJ >= 1 ? curJ - 1 : sizeJ() - 1);
    break;
  default:
    std::cerr << "default direction" << std::endl;
    break;
  } // end of switch

  return &world_[destI][destJ];
}

void World::saveStatistics (const std::string& filename, bool as_text)
{
  as_text ? statistics_->saveAsText(filename)
          : statistics_->saveGlobalStatistics(filename);
}

void World::processMutants ()
{
  while ( !mutants_.empty() ) {
    auto mutant = mutants_.back();
    mutants_.pop_back();
    addPopulation( mutant->currentCell(), mutant );
  }
}

void World::processMigrants ()
{
  while (!migrants_.empty()) {
    auto migrant = migrants_.back();
    migrants_.pop_back();
    auto destination_cell = giveNeighborCell(migrant.population->currentCell(), migrant.direction);
    addPopulation( destination_cell, migrant.population );
  }
}

BoundVector World::getBounds (size_t rows, size_t n_threads)
{
  BoundVector b;
  if (rows < n_threads) {
    b.resize(rows);
    for (size_t i = 0; i < rows; ++i)
    {
      b[i].a1 = i;
      b[i].a2 = i + 1;
    }
  } else {
    size_t rows_per_thread = rows / n_threads;
    b.resize(n_threads);
    int counter = rows % n_threads;
    size_t t [n_threads];
    for (size_t i = 0; i < n_threads; ++i)
      t[i] = rows_per_thread + size_t(counter-- > 0);

    b[0].a1 = 0, b[0].a2 = t[0];
    for (size_t i = 1; i < n_threads; ++i) {
      b[i].a1 = b[i - 1].a2;
      b[i].a2 = b[i].a1 + t[i];
    }
  }

  return b;
}

size_t World::findNumberOfThreads(size_t desired_number, size_t rows) const
{
  return 1;
  //return (size_t) aux::min(std::thread::hardware_concurrency(), desired_number, rows);
}

void World::clear()
{
}

Cell* World::cell (unsigned posI, unsigned posJ)
{
  return &world_[posI][posJ];
}

void World::setCollectStatistics (bool b)
{
  collect_statistics_ = b;
}

void World::printIteration (size_t current_iteration, size_t max_iteration, bool print_memory) const
{
  size_t rarefaction;
  if (max_iteration < 100)
    rarefaction = 10;
  else if (max_iteration < 500)
    rarefaction = 50;
  else if (max_iteration < 1000)
    rarefaction = 100;
  else
    rarefaction = 200;
  if (current_iteration % rarefaction == 0) {
    std::cout << "Iteration: " << current_iteration << std::endl;
    if(print_memory)
      printMemory();
  }
}

void World::play (size_t iterations, size_t desired_threads)
{
  if (collect_statistics_)
    statistics_ = std::make_shared<GlobalStatistics>(this, size_t(iterations));

  size_t n_threads = std::min(desired_threads, size_t(omp_get_max_threads()));
  omp_set_num_threads(n_threads);
  cout << "Using " << n_threads << " threads" << endl;

#pragma omp parallel
  {
    for (size_t iteration = 0; iteration < iterations; ++iteration)
    {
#pragma omp single
      printIteration(iteration, iterations, false);
      //{
#pragma omp for
        for (size_t i = 0; i < WorldParameters::sizeI; ++i)
          for (size_t j = 0; j < WorldParameters::sizeJ; ++j)
            processCell(i, j, &mutants_, &migrants_);
      //}
#pragma omp single
      {
        processMutants();
        processMigrants();
      }

#pragma omp single
      {
      // Collecting statistics
        if (collect_statistics_)
          statistics_->collectGlobalStatistics();
        if (!mutants_.empty())
          std::cerr << "Error in play(): non empty mutant list" << std::endl;
        if (!migrants_.empty())
          std::cerr << "Error in play(): non empty migrant list" << std::endl;
      }
    } // end of main loop
  } // end of omp parallel
}

void World::processCell (size_t posI, size_t posJ, PopList* mut_list, MigrationList* mig_list)
{
  // Reduction
  auto it = world_[posI][posJ].populations.begin();
  while (it != world_[posI][posJ].populations.end())
  {
    it->second->reduction();
    if (it->second->type() == kAnimate) {
      double energy = static_cast<Animate*>(it->second)->energy();
      if (it->second->size() < WorldParameters::fatal_size || energy < WorldParameters::fatal_energy) {
        delete it->second;
        it = world_[posI][posJ].populations.erase(it);
      }
      else
        ++it;
    } else
      ++it;
  } // end of reduction

  // Feeding
  for (auto& pop : world_[posI][posJ].populations)
    pop.second->feeding();

  // Reproduction
  for (auto& pop : world_[posI][posJ].populations)
    pop.second->reproduction(mut_list);

  // Migration
  for (auto& pop : world_[posI][posJ].populations)
    pop.second->migration(mig_list);
}

void World::printMemory () const
{
  struct sysinfo sys_info;

  if(sysinfo(&sys_info) != 0)
    perror("sysinfo");

  unsigned mb_size { 1024 * 1024 };
  cout << "freeram: " << sys_info.freeram / (double(sys_info.mem_unit)*mb_size) << std::endl;
  cout << "totalram: " << sys_info.totalram / (double(sys_info.mem_unit)*mb_size) << std::endl;
}

void World::setEnvironment(size_t posI, size_t posJ, size_t type)
{
  world_[posI][posJ].environment = type;
}
