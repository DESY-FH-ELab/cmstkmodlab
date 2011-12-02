
#include <QApplication>

#include "DefoMainWindow.h"


int main( int argc, char** argv ) {

  QApplication app( argc, argv );
  app.setStyle("cleanlooks");

  DefoMainWindow mainWindow;

  mainWindow.show();

  return app.exec();

}
