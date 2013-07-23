#include <QApplication>
#include <QTcpSocket>

#include "SingletonApplication.h"
#include "ApplicationConfig.h"

#include "ThermoDisplayMainWindow.h"
#include "ThermoDAQNetworkReader.h"
#include "ThermoDAQClient.h"

static const char* thermoDisplayGUID = "{413A4B5E-EEAD-44E5-8195-4106D8C1A2DD}";
#define SINGLETON 1

int main( int argc, char** argv ) {

#ifdef SINGLETON
    SingletonApplication app(argc, argv, thermoDisplayGUID);
    if(!app.lock()){
        std::cout << "Application instance already running!" << std::endl;
        exit(1);
    }
#else
    QApplication app( argc, argv );
#endif

    app.setStyle("cleanlooks");

    ApplicationConfig::instance(std::string(Config::CMSTkModLabBasePath) + "/thermo/thermo.cfg");

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
