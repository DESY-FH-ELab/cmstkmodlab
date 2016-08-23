#include <QApplication>
#include <QProcess>
#include <QFile>
#include <QDir>
#if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
#include <QDesktopServices>
#else
#include <QStandardPaths>
#endif

#include <iostream>
#include <nqlogger.h>
#include <DeviceState.h>

#include "SingletonApplication.h"
#include "ApplicationConfig.h"

#include "MCommanderMainWindow.h"

static const char* assemblyGUID = "{5F9DC7D7-54C2-4625-A7C6-2EBE4C37C8F5}";
//#define SINGLETON 1

int main( int argc, char** argv )
{
    NQLogger::instance()->addActiveModule("*");
    NQLogger::instance()->addDestiniation(stdout, NQLog::Debug);

#if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
    QString logdir = QDesktopServices::storageLocation(QDesktopServices::CacheLocation);
#else
    QString logdir = QStandardPaths::writableLocation(QStandardPaths::CacheLocation);
#endif
    QDir dir(logdir);
    if (!dir.exists()) dir.mkpath(".");
    QString logfilename = logdir + "/motionCommander.log";
    //QString logfilename = "./Logfile_motionCommander.log";

    //NQLog("motionCommander") << "version " << APPLICATIONVERSIONSTR;

    //NQLog("motionCommander") << "using " << logfilename << " for logging";

    QFile * logfile = new QFile(logfilename);
    //if (logfile->open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text)) {
    if (logfile->open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text)) {
      NQLogger::instance()->addDestiniation(logfile, NQLog::Debug);
    }

    qRegisterMetaType<State>("State");

#ifdef SINGLETON
    SingletonApplication app(argc, argv, assemblyGUID);
    if(!app.lock()){
    //  NQLog("motionCommander") << "Application instance already running!";
      exit(1);
    }
#else
    QApplication app( argc, argv );
#endif

    app.setStyle("cleanlooks");

    ApplicationConfig::instance(std::string(Config::CMSTkModLabBasePath) + "/assembly/assembly.cfg");

    MCommanderMainWindow mainWindow;

    mainWindow.setWindowTitle(QString("motionCommander - ") + APPLICATIONVERSIONSTR);
    mainWindow.show();

    return app.exec();
}
