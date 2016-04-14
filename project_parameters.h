#ifndef PROJECTPARAMETERS_H
#define PROJECTPARAMETERS_H

#include <string>
#include <fstream>

struct ProjectParameters {
  static void writeToStream (std::ofstream& out);

  static std::string project_name;
  static std::string result_filename;
  static size_t preferred_threads;
  static size_t max_iteration;
};

#endif // PROJECTPARAMETERS_H
