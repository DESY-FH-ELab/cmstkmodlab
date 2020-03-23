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

#include "MicroMainWindow.h"

static const char* microDAQGUID = "{7D85ED7C-8AC6-4C39-A893-1D25E8C338E2}";
//#define SINGLETON 1
//#define AUTOSTARTmicroDISPLAY 1

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
    QString logfilename = logdir + "/microDAQ.log";

    NQLog("microDAQ") << "using " << logfilename << " for logging";

    QFile * logfile = new QFile(logfilename);
    if (logfile->open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text)) {
        NQLogger::instance()->addDestiniation(logfile, NQLog::Message);
    }

    qRegisterMetaType<State>("State");

#ifdef SINGLETON
    SingletonApplication app(argc, argv, microDAQGUID);
    if(!app.lock()){
        NQLog("microDAQ") << "Application instance already running!";
        exit(1);
    }
#else
    QApplication app( argc, argv );
#endif

  app.setStyle("cleanlooks");

#ifdef AUTOSTARTmicroDISPLAY
  std::string microDisplay = std::string(Config::CMSTkModLabBasePath) + "/thermo/thermo1/thermoDisplay/thermoDisplay";
  QProcess myProcess(&app);
  myProcess.start(microDisplay.c_str());
#endif

  ApplicationConfig::instance(std::string(Config::CMSTkModLabBasePath) + "/thermo/thermo1/thermo.cfg");

  MicroMainWindow mainWindow;

  mainWindow.show();

  return app.exec();

}
