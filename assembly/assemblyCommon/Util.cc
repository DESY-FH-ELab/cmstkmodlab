#include <Util.h>

#if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
#include <QDesktopServices>
#else
#include <QStandardPaths>
#endif
#include <QDir>

QString Util::QtCacheDirectory()
{
 #if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
   return QDesktopServices::storageLocation(QDesktopServices::CacheLocation);
 #else
   return QStandardPaths::writableLocation(QStandardPaths::CacheLocation);
 #endif
}

bool Util::QDir_mkpath(const QString& path)
{
  const QDir dir(path);
  if(dir.exists() == false){ return dir.mkpath("."); }

  return true;
}
