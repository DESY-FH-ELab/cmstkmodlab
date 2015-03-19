#include <QApplication>

#include "SingletonApplication.h"

#include "DefoConfig.h"
#include "DefoMainWindow.h"
#include "TestWindow.h"

static const char* defoDAQGUID = "{94E678B7-AD8C-4027-971D-5F49E0FFC647}";
//#define SINGLETON 1

int main( int argc, char** argv ) {

  qRegisterMetaType<DefoConradModel::DeviceSwitch>("DefoConradModel::DeviceSwitch");

#ifdef SINGLETON
    SingletonApplication app(argc, argv, defoDAQGUID);
    if(!app.lock()){
        std::cout << "Application instance already running!" << std::endl;
        exit(1);
    }
#else
    QApplication app( argc, argv );
#endif

  app.setStyle("cleanlooks");

  DefoConfig::instance(std::string(Defo::CMSTkModLabBasePath) + "/defo/defo.cfg");

  DefoMainWindow mainWindow;
  //TestWindow mainWindow;
  //ScriptWindow mainWindow;

  mainWindow.show();

  return app.exec();

}
