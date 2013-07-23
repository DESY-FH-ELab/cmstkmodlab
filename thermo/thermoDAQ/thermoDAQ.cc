#include <QApplication>
#include <QProcess>

#include "SingletonApplication.h"
#include "ApplicationConfig.h"

#include "ThermoMainWindow.h"
#include "TestWindow.h"

static const char* thermoDAQGUID = "{2F9BC7D7-44A2-4625-A7C6-2EBE3C27C7F5}";
#define SINGLETON 1

int main( int argc, char** argv )
{

#ifdef SINGLETON
    SingletonApplication app(argc, argv, thermoDAQGUID);
    if(!app.lock()){
        std::cout << "Application instance already running!" << std::endl;
        exit(1);
    }
#else
    QApplication app( argc, argv );
#endif

  app.setStyle("cleanlooks");

  QProcess myProcess(&app);
  myProcess.start("thermoDisplay");

  ApplicationConfig::instance(std::string(Config::CMSTkModLabBasePath) + "/thermo/thermo.cfg");

  ThermoMainWindow mainWindow;
  //TestWindow mainWindow;

  mainWindow.show();

  return app.exec();

}
