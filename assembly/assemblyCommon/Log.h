#ifndef LOG_H
#define LOG_H

#include <string>

namespace Log {

  void KILL   (const std::string&);
  void WARNING(const std::string&);

  extern unsigned int verbosity;

}

#endif
