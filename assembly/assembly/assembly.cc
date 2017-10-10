/////////////////////////////////////////////////////////////////////////////////
//                                                                             //
//               Copyright (C) 2011-2017 - The DESY CMS Group                  //
//                           All rights reserved                               //
//                                                                             //
//      The CMStkModLab source code is licensed under the GNU GPL v3.0.        //
//      You have the right to modify and/or redistribute this source code      //
//      under the terms specified in the license, which may be found online    //
//      at http://www.gnu.org/licenses or at License.txt.                      //
//                                                                             //
/////////////////////////////////////////////////////////////////////////////////

#include <opencv2/opencv.hpp>

#include <QApplication>
#include <QProcess>
#include <QFile>
#include <QDir>
#if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
#include <QDesktopServices>
#else
#include <QStandardPaths>
#endif

#include <nqlogger.h>
#include <DeviceState.h>

#include "SingletonApplication.h"
#include "ApplicationConfig.h"

#include "AssemblyMainWindow.h"

static const char* assemblyGUID = "{5F9DC7D7-54C2-4625-A7C6-2EBE4C37C8F5}";
//#define SINGLETON 1

int main( int argc, char** argv )
{
    qRegisterMetaType<cv::Mat>("cv::Mat");

    NQLogger::instance()->addActiveModule("*");
    NQLogger::instance()->addDestiniation(stdout, NQLog::Spam);

#if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
    QString logdir = QDesktopServices::storageLocation(QDesktopServices::CacheLocation);
#else
    QString logdir = QStandardPaths::writableLocation(QStandardPaths::CacheLocation);
#endif
    QDir dir(logdir);
    if(!dir.exists()){ dir.mkpath("."); }
    QString logfilename = logdir + "/assembly.log";

    NQLog("assembly") << "version " << APPLICATIONVERSIONSTR;

    NQLog("assembly") << "using " << logfilename << " for logging";

    QFile * logfile = new QFile(logfilename);
    if(logfile->open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text)){

      NQLogger::instance()->addDestiniation(logfile, NQLog::Message);
    }

    qRegisterMetaType<State>("State");

#ifdef SINGLETON
    SingletonApplication app(argc, argv, assemblyGUID);
    if(!app.lock()){

      NQLog("assembly") << "Application instance already running!";
      exit(1);
    }
#else
    QApplication app( argc, argv );
#endif

    app.setStyle("cleanlooks");

    ApplicationConfig::instance(std::string(Config::CMSTkModLabBasePath)+"/assembly/assembly.cfg");

    AssemblyMainWindow mainWindow;

    mainWindow.setWindowTitle(QString("assembly - ") + APPLICATIONVERSIONSTR);
    mainWindow.show();

    return app.exec();
}
