#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H

#include <memory>

class Environment;

using EnvironmentPtr = std::shared_ptr<Environment>;

class Environment
{
public:
  Environment();

private:
  static size_t id_counter_;
  size_t id_;

};

#endif // ENVIRONMENT_H
