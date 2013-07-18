#include <QApplication>
#include <QTcpSocket>

#include "ApplicationConfig.h"

#include "ThermoDAQClient.h"

int main( int argc, char** argv ) {

  QApplication app( argc, argv );
  app.setStyle("cleanlooks");

  ApplicationConfig::instance(std::string(Config::CMSTkModLabBasePath) + "/thermo/thermo.cfg");

  //ThermoMainWindow mainWindow;
  //TestWindow mainWindow;

  //mainWindow.show();

  ThermoDAQClient client(55555);

  client.readDAQStatus();

  return app.exec();
}
