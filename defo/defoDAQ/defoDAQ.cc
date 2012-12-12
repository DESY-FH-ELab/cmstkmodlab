
#include <QApplication>

#include "DefoMainWindow.h"
#include "TestWindow.h"

int main( int argc, char** argv ) {

  qRegisterMetaType<DefoConradModel::DeviceSwitch>("DefoConradModel::DeviceSwitch");

  QApplication app( argc, argv );
  app.setStyle("cleanlooks");

  DefoMainWindow mainWindow;
  //TestWindow mainWindow;
  //ScriptWindow mainWindow;

  mainWindow.show();

  return app.exec();

}
