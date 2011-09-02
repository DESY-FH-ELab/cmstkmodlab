
#include <QApplication>

#include "DefoMainWindow.h"


int main( int argc, char** argv ) {

  QApplication app( argc, argv );
  
  DefoMainWindow mainWindow;

  mainWindow.show();

  return app.exec();

}
