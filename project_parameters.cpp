#include "project_parameters.h"

std::string ProjectParameters::project_name    {};
std::string ProjectParameters::result_filename {};
size_t ProjectParameters::preferred_threads    {7};
size_t ProjectParameters::max_iteration        {200};

void ProjectParameters::writeToStream (std::ofstream& out)
{
  size_t s_size = sizeof(size_t);
  size_t length = project_name.length();
  out.write(reinterpret_cast<const char*>( &length ),                s_size );
  out.write(reinterpret_cast<const char*>( project_name.c_str()),    length );
  length = result_filename.length();
  out.write(reinterpret_cast<const char*>( &length ),                s_size );
  out.write(reinterpret_cast<const char*>( result_filename.c_str()), length );
  out.write(reinterpret_cast<const char*>( &preferred_threads ),     s_size );
  out.write(reinterpret_cast<const char*>( &max_iteration ),         s_size );
}
