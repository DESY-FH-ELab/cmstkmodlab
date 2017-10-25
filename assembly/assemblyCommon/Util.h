#ifndef UTIL_H
#define UTIL_H

#include <QString>

namespace Util {

  QString QtCacheDirectory();

  bool QDir_mkpath(const QString&);
  bool QDir_mkpath(const std::string&);

  bool DirectoryExists(const QString&);
  bool DirectoryExists(const std::string&);

}

#endif
