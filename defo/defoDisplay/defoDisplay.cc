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

#include "DefoDisplayMainWindow.h"
#include "DefoDAQNetworkReader.h"
#include "DefoDAQClient.h"

static const char* defoDisplayGUID = "{413A4B5E-EEAD-44E5-8195-4106D8C1A2DA}";
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
    QString logfilename = logdir + "/defoDisplay.log";

    NQLog("defoDisplay") << "version " << APPLICATIONVERSIONSTR;

    NQLog("defoDisplay") << "using " << logfilename << " for logging";

    QFile * logfile = new QFile(logfilename);
    if (logfile->open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text)) {
        NQLogger::instance()->addDestiniation(logfile, NQLog::Message);
    }

#ifdef SINGLETON
    SingletonApplication app(argc, argv, defoDisplayGUID);
    if(!app.lock()){
        NQLog("defoDisplay") << "Application instance already running!";
        exit(1);
    }
#else
    QApplication app( argc, argv );
#endif

    app.setStyle("cleanlooks");

    ApplicationConfig::instance(std::string(Config::CMSTkModLabBasePath) + "/defo/defo.cfg");

    if (app.arguments().size()==2 &&
        app.arguments().at(1)=="--nogui") {

        DefoDAQClient * client = new DefoDAQClient(55556);
        DefoDAQNetworkReader * reader = new DefoDAQNetworkReader(&app);

        QObject::connect(client, SIGNAL(handleMessage(QString&)),
                         reader, SLOT(run(QString&)));
        QObject::connect(reader, SIGNAL(finished()),
                         &app, SLOT(quit()));

        client->readDAQStatus();

    } else {
        DefoDisplayMainWindow * mainWindow = new DefoDisplayMainWindow();
        mainWindow->show();
    }

    return app.exec();
}
