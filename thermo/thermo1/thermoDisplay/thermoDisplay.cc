/////////////////////////////////////////////////////////////////////////////////
//                                                                             //
//               Copyright (C) 2011-2017 - The DESY CMS Group                  //
//                           All rights reserved                               //
//                                                                             //
//      The CMStkModLab source code is licensed under the GNU GPL v3.0.        //
//      You have the right to modify and/or redistribute this source code      //
//      under the terms specified in the license, which may be found online    //
//      at http://www.gnu.org/licenses or at License.txt.                      //
//                                                                             //
/////////////////////////////////////////////////////////////////////////////////

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

#include "ThermoDisplayMainWindow.h"
#include "ThermoDAQNetworkReader.h"
#include "ThermoDAQClient.h"

static const char* thermoDisplayGUID = "{413A4B5E-EEAD-44E5-8195-4106D8C1A2DD}";
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
    QString logfilename = logdir + "/thermoDisplay.log";

    NQLog("thermoDisplay") << "version " << APPLICATIONVERSIONSTR;

    NQLog("thermoDisplay") << "using " << logfilename << " for logging";

    QFile * logfile = new QFile(logfilename);
    if (logfile->open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text)) {
        NQLogger::instance()->addDestiniation(logfile, NQLog::Message);
    }

#ifdef SINGLETON
    SingletonApplication app(argc, argv, thermoDisplayGUID);
    if(!app.lock()){
        NQLog("thermoDisplay") << "Application instance already running!";
        exit(1);
    }
#else
    QApplication app( argc, argv );
#endif

    app.setStyle("cleanlooks");

    ApplicationConfig::instance(std::string(Config::CMSTkModLabBasePath) + "/thermo/thermo1/thermo.cfg");

    if (app.arguments().size()==2 &&
        app.arguments().at(1)=="--nogui") {

        ThermoDAQClient * client = new ThermoDAQClient(55555);
        ThermoDAQNetworkReader * reader = new ThermoDAQNetworkReader(&app);

        QObject::connect(client, SIGNAL(handleMessage(QString&)),
                         reader, SLOT(run(QString&)));
        QObject::connect(reader, SIGNAL(finished()),
                         &app, SLOT(quit()));

        client->readDAQStatus();

    } else {
        ThermoDisplayMainWindow * mainWindow = new ThermoDisplayMainWindow();
        mainWindow->show();
    }

    return app.exec();
}
