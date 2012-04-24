
#include <QApplication>

#include "ConradMainWindow.h"

int main( int argc, char** argv ) {

  QApplication app( argc, argv );
  app.setStyle("cleanlooks");

  ConradMainWindow conrad;
  conrad.show();

  return app.exec();

}


