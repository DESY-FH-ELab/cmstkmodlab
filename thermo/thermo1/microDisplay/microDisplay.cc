#include <QApplication>
#include <QTcpSocket>
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

#include "MicroDisplayMainWindow.h"
#include "MicroDAQNetworkReader.h"
#include "MicroDAQClient.h"

static const char* thermoDisplayGUID = "{311A1DA3-DA0F-414D-9F9B-C322F2DCA775}";
#define SINGLETON 1

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
    QString logfilename = logdir + "/microDisplay.log";

    NQLog("microDisplay") << "using " << logfilename << " for logging";

    QFile * logfile = new QFile(logfilename);
    if (logfile->open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text)) {
        NQLogger::instance()->addDestiniation(logfile, NQLog::Message);
    }

#ifdef SINGLETON
    SingletonApplication app(argc, argv, thermoDisplayGUID);
    if(!app.lock()){
        NQLog("microDisplay") << "Application instance already running!";
        exit(1);
    }
#else
    QApplication app( argc, argv );
#endif

    app.setStyle("cleanlooks");

    ApplicationConfig::instance(std::string(Config::CMSTkModLabBasePath) + "/thermo/thermo1/thermo.cfg");

    if (app.arguments().size()==2 &&
        app.arguments().at(1)=="--nogui") {

        MicroDAQClient * client = new MicroDAQClient(55556);
        MicroDAQNetworkReader * reader = new MicroDAQNetworkReader(&app);

        QObject::connect(client, SIGNAL(handleMessage(QString&)),
                         reader, SLOT(run(QString&)));
        QObject::connect(reader, SIGNAL(finished()),
                         &app, SLOT(quit()));

        client->readDAQStatus();

    } else {
        MicroDisplayMainWindow * mainWindow = new MicroDisplayMainWindow();
        mainWindow->show();
    }

    return app.exec();
}
