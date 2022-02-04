/////////////////////////////////////////////////////////////////////////////////
//                                                                             //
//               Copyright (C) 2011-2022 - The DESY CMS Group                  //
//                           All rights reserved                               //
//                                                                             //
//      The CMStkModLab source code is licensed under the GNU GPL v3.0.        //
//      You have the right to modify and/or redistribute this source code      //
//      under the terms specified in the license, which may be found online    //
//      at http://www.gnu.org/licenses or at License.txt.                      //
//                                                                             //
/////////////////////////////////////////////////////////////////////////////////

#include <iostream>

#include <QCoreApplication>
#include <QCommandLineParser>
#include <QTimer>

#include "ThermoDAQStreamReader.h"
#include "ThermoDAQ2StreamReader.h"

int main(int argc, char *argv[])
{
  QCoreApplication app(argc, argv);
  QCoreApplication::setApplicationName("thermoDAQ2ROOT");
  QCoreApplication::setApplicationVersion("1.0");

  QCommandLineParser parser;

  parser.setOptionsAfterPositionalArgumentsMode(QCommandLineParser::ParseAsOptions);

  parser.setApplicationDescription("Converts thermalDAQ XML data files to root files.");
  parser.addHelpOption();

  parser.addOption(QCommandLineOption("v",
      "File version to process. Must be either 1 or 2.\n"
      "Version 2 is the default",
      "version", "2"));
  parser.addPositionalArgument("filename", "XML data file to process");

  parser.parse(app.arguments());

  const QString version = parser.value("v");

  if (version=="2") {
    parser.addOption(QCommandLineOption("Keithley",
        "multimeter data triggers filling of root tree (default)"));
    parser.addOption(QCommandLineOption("Huber",
        "chiller data triggers filling of root tree"));
  }

  parser.process(app.arguments());

  if (parser.positionalArguments().count()!=1) {
      parser.showHelp(0);
  }

  QStringList parameters;
  if (parser.isSet("Keithley")) parameters << "Keithley";
  if (parser.isSet("Huber")) parameters << "Huber";

  const QString filename = parser.positionalArguments().last();

  std::cout << "file version: " << version.toStdString() << std::endl;
  std::cout << "parameters:   ";
  for (auto p : parameters) {
    std::cout << p.toStdString() << " ";
  }
  std::cout << std::endl;
  std::cout << "filename:     " << filename.toStdString() << std::endl;

  if (version=="1") {
    ThermoDAQStreamReader *reader = new ThermoDAQStreamReader(parameters, filename, &app);
    QObject::connect(reader, SIGNAL(finished()), &app, SLOT(quit()));
    QTimer::singleShot(0, reader, SLOT(run()));

  } else if (version=="2") {
    ThermoDAQ2StreamReader *reader = new ThermoDAQ2StreamReader(parameters, filename, &app);
    QObject::connect(reader, SIGNAL(finished()), &app, SLOT(quit()));
    QTimer::singleShot(0, reader, SLOT(run()));

  } else {
    parser.showHelp(0);
  }

  return app.exec();
}
