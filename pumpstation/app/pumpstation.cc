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

#include "PumpStationMainWindow.h"

static const char* pumpstationGUID = "{5F9DC7D7-54C2-4625-A7C6-2EBE4C37C8F5}";
//#define SINGLETON 1

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
    QString logfilename = logdir + "/pumpstation.log";

    NQLog("assembly") << "version " << APPLICATIONVERSIONSTR;

    NQLog("assembly") << "using " << logfilename << " for logging";

    QFile * logfile = new QFile(logfilename);
    if (logfile->open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text)) {
        NQLogger::instance()->addDestiniation(logfile, NQLog::Message);
    }

#ifdef SINGLETON
    SingletonApplication app(argc, argv, plasmaGUID);
    if(!app.lock()){
        NQLog("assembly") << "Application instance already running!";
        exit(1);
    }
#else
    QApplication app( argc, argv );
#endif

  app.setStyle("cleanlooks");

  ApplicationConfig::instance(std::string(Config::CMSTkModLabBasePath) + "/pumpstation/pumpstation.cfg");

  PumpStationMainWindow mainWindow(5);

  mainWindow.setWindowTitle(QString("pumpstation - ") + APPLICATIONVERSIONSTR);
  mainWindow.show();

  return app.exec();
}
