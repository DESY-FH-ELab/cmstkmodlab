#include <QApplication>
#include <QProcess>
#include <QFile>
#include <QDir>
#if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
#include <QDesktopServices>
#else
#include <QStandardPaths>
#endif

#include <nqlogger.h>

#include "SingletonApplication.h"
#include "ApplicationConfig.h"
#include "ApplicationVersion.h"

#include "DefoRecoMainWindow.h"

static const char* defoRecoGUID = "{94E678B7-AD8C-4027-971D-5F49E0FFC647}";

int main( int argc, char** argv )
{
  NQLogger::instance()->addActiveModule("*");
  NQLogger::instance()->addDestiniation(stdout, NQLog::Spam);

#if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
  QString logdir = QDesktopServices::storageLocation(QDesktopServices::CacheLocation);
#else
  QString logdir = QStandardPaths::writableLocation(QStandardPaths::CacheLocation);
#endif
  QDir dir(logdir);
  if (!dir.exists()) dir.mkpath(".");
  QString logfilename = logdir + "/defoReco.log";

  NQLog("defoReco") << "version " << APPLICATIONVERSIONSTR;

  NQLog("defoReco") << "using " << logfilename << " for logging";

  QFile * logfile = new QFile(logfilename);
  if (logfile->open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text)) {
      NQLogger::instance()->addDestiniation(logfile, NQLog::Message);
  }

  QApplication app( argc, argv );

  app.setStyle("cleanlooks");

  ApplicationConfig::instance(std::string(Config::CMSTkModLabBasePath) + "/defo/defo.cfg");

  DefoRecoMainWindow mainWindow;

  mainWindow.show();

  return app.exec();

}
