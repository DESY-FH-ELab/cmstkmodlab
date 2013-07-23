#include <QApplication>

#include "DefoConfig.h"
#include "DefoMainWindow.h"
#include "TestWindow.h"

static constexpr std::string defoDAQGUID = "{94E678B7-AD8C-4027-971D-5F49E0FFC647}";

int main( int argc, char** argv ) {

  qRegisterMetaType<DefoConradModel::DeviceSwitch>("DefoConradModel::DeviceSwitch");

  QApplication app( argc, argv );
  app.setStyle("cleanlooks");

  DefoConfig::instance(std::string(Defo::CMSTkModLabBasePath) + "/defo/defo.cfg");

  DefoMainWindow mainWindow;
  //TestWindow mainWindow;
  //ScriptWindow mainWindow;

  mainWindow.show();

  return app.exec();

}
