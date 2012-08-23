
#include <QApplication>

//#include "DefoMainWindow.h"
#include "TestWindow.h"


int main( int argc, char** argv ) {

  QApplication app( argc, argv );
  app.setStyle("cleanlooks");

  TestWindow mainWindow;

  mainWindow.show();

  return app.exec();

}
