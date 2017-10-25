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

bool Util::QDir_mkpath(const std::string& path)
{
  return Util::QDir_mkpath(QString(path.c_str()));
}

bool Util::DirectoryExists(const QString& path)
{
  const QDir dir(path);

  return dir.exists();
}

bool Util::DirectoryExists(const std::string& path)
{
  return Util::DirectoryExists(QString(path.c_str()));
}
