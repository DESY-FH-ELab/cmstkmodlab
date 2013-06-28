#include <QApplication>

#include "ApplicationConfig.h"
#include "ThermoMainWindow.h"

int main( int argc, char** argv ) {

  QApplication app( argc, argv );
  app.setStyle("cleanlooks");

  ApplicationConfig::instance(std::string(Config::CMSTkModLabBasePath) + "/thermo/thermo.cfg");

  ThermoMainWindow mainWindow;

  mainWindow.show();

  return app.exec();

}
