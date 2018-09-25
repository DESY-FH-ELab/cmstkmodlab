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

#include <nqlogger.h>
#include <ApplicationConfig.h>
#include <DeviceState.h>
#include <SingletonApplication.h>

#include <AssemblyMainWindow.h>
#include <AssemblyUtilities.h>

#include <string>

#include <QApplication>
#include <QDateTime>
#include <QFile>

#include <opencv2/opencv.hpp>

#ifdef SINGLETON
static const char* assemblyGUID = "{5F9DC7D7-54C2-4625-A7C6-2EBE4C37C8F5}";
#endif

int main(int argc, char** argv)
{
    // Qt application -------
    qRegisterMetaType<State>("State");
    qRegisterMetaType<cv::Mat>("cv::Mat");

#ifdef SINGLETON
    SingletonApplication app(argc, argv, assemblyGUID);
    if(!app.lock())
    {
      NQLog("assembly", NQLog::Message) << "Application instance already running!";
      exit(1);
    }
#else
    QApplication app(argc, argv);
#endif

    app.setStyle("cleanlooks");

    // log output -----------
    ApplicationConfig* config = ApplicationConfig::instance(std::string(Config::CMSTkModLabBasePath)+"/assembly/assembly.cfg");

    const NQLog::LogLevel nqloglevel_stdout  = ((NQLog::LogLevel) config->getValue<int>("LogLevel_stdout" , 2));
    const NQLog::LogLevel nqloglevel_logfile = ((NQLog::LogLevel) config->getValue<int>("LogLevel_logfile", 2));

    NQLogger::instance()->addActiveModule("*");
    NQLogger::instance()->addDestiniation(stdout, nqloglevel_stdout);

    const QString timestamp = QDateTime::currentDateTime().toString("yyyyMMdd_hhmmss");

    const QString outputdir_path = assembly::QtCacheDirectory()+"/"+timestamp;

    if(assembly::DirectoryExists(outputdir_path))
    {
      NQLog("assembly", NQLog::Fatal) << "target output directory already exists: " << outputdir_path;

      return 1;
    }
    else
    {
      assembly::QDir_mkpath(outputdir_path);
    }

    const QString logfile_path = outputdir_path+"/assembly.log";

    NQLog("assembly", NQLog::Message) << "VERSION  : " << APPLICATIONVERSIONSTR;
    NQLog("assembly", NQLog::Message) << "SESSION  : " << timestamp;
    NQLog("assembly", NQLog::Message) << "LOG FILE : " << logfile_path;

    QFile* logfile = new QFile(logfile_path);
    if(logfile->open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text))
    {
      NQLogger::instance()->addDestiniation(logfile, nqloglevel_logfile);
    }
    // ----------------------

    AssemblyMainWindow mainWindow(outputdir_path, logfile_path);

    mainWindow.setWindowTitle("Automated Pixel-Strip Module Assembly ["+QString(APPLICATIONVERSIONSTR)+"]");
    mainWindow.setWindowState(Qt::WindowMaximized);

    mainWindow.show();

    return app.exec();
    // ----------------------
}
