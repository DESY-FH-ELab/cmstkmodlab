#include <QApplication>

#include "ApplicationConfig.h"

#include "ThermoMainWindow.h"
#include "TestWindow.h"

int main( int argc, char** argv ) {

  QApplication app( argc, argv );
  app.setStyle("cleanlooks");

  ApplicationConfig::instance(std::string(Config::CMSTkModLabBasePath) + "/thermo/thermo.cfg");

  ThermoMainWindow mainWindow;
  //TestWindow mainWindow;

  mainWindow.show();

  return app.exec();

}
