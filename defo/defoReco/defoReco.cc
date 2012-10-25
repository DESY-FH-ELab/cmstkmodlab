
#include <QApplication>

#include "DefoRecoMainWindow.h"

int main( int argc, char** argv ) {

  QApplication app( argc, argv );
  app.setStyle("cleanlooks");

  DefoRecoMainWindow mainWindow;

  mainWindow.show();

  return app.exec();

}
