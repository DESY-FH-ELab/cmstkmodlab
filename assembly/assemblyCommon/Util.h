#ifndef UTIL_H
#define UTIL_H

#include <QString>

namespace Util {

  QString QtCacheDirectory();

  bool QDir_mkpath(const QString&);

}

#endif
