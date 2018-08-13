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
#include <QFile>

#include <opencv2/opencv.hpp>

#ifdef SINGLETON
static const char* assemblyGUID = "{5F9DC7D7-54C2-4625-A7C6-2EBE4C37C8F5}";
#endif

int main(int argc, char** argv)
{
    // log output -----------
    ApplicationConfig* config = ApplicationConfig::instance(std::string(Config::CMSTkModLabBasePath)+"/assembly/assembly.cfg");

    const NQLog::LogLevel nqloglevel_stdout  = ((NQLog::LogLevel) config->getValue<int>("LogLevel_stdout" , 2));
    const NQLog::LogLevel nqloglevel_logfile = ((NQLog::LogLevel) config->getValue<int>("LogLevel_logfile", 2));

    NQLogger::instance()->addActiveModule("*");
    NQLogger::instance()->addDestiniation(stdout, nqloglevel_stdout);

    const QString logdir = assembly::QtCacheDirectory();
    assembly::QDir_mkpath(logdir);

    const QString logfilename = logdir+"/assembly.log";

    NQLog("assembly", NQLog::Message) << "version  : " << APPLICATIONVERSIONSTR;
    NQLog("assembly", NQLog::Message) << "log file : " << logfilename;

    QFile* logfile = new QFile(logfilename);
    if(logfile->open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text))
    {
      NQLogger::instance()->addDestiniation(logfile, nqloglevel_logfile);
    }
    // ----------------------

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

    AssemblyMainWindow mainWindow;

    mainWindow.setWindowTitle("Automated Pixel-Strip Module Assembly ["+QString(APPLICATIONVERSIONSTR)+"]");
    mainWindow.show();

    return app.exec();
    // ----------------------
}
