#include <QApplication>

#include "DefoConfig.h"
#include "DefoRecoMainWindow.h"

int main( int argc, char** argv ) {

  QApplication app( argc, argv );
  app.setStyle("cleanlooks");

  DefoConfig::instance(std::string(Defo::CMSTkModLabBasePath) + "/defo/defo.cfg");

  DefoRecoMainWindow mainWindow;

  mainWindow.show();

  return app.exec();

}
