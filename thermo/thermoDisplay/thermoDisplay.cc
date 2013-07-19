#include <QApplication>
#include <QTcpSocket>

#include "ApplicationConfig.h"

#include "ThermoDisplayMainWindow.h"
#include "ThermoDAQNetworkReader.h"
#include "ThermoDAQClient.h"

int main( int argc, char** argv ) {

  QApplication app( argc, argv );

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
