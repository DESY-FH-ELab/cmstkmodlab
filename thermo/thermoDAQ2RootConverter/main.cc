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

#include "ThermoDAQ2SlidingWindowConverter.h"

int main(int argc, char *argv[])
{
  QCoreApplication app(argc, argv);
  QCoreApplication::setApplicationName("thermoDAQ2ROOTConverter");
  QCoreApplication::setApplicationVersion("1.0");

  QCommandLineParser parser;

  parser.setOptionsAfterPositionalArgumentsMode(QCommandLineParser::ParseAsOptions);

  parser.setApplicationDescription("Converts thermalDAQ root files.");
  parser.addHelpOption();

  QString algorithm = "One of the following conversion algorithms to apply:\n"
      "   slw - Sliding window averager";
  parser.addPositionalArgument("algorithm", algorithm);

  parser.addPositionalArgument("input", "Filename of input.");
  parser.addPositionalArgument("output", "Filename of output.");

  parser.parse(app.arguments());

  const QStringList args = parser.positionalArguments();

  if (args.count()<1) {
    parser.showHelp(0);
  }

  const QString command = args.first();

  if (command == "slw") {
    parser.clearPositionalArguments();
    parser.addPositionalArgument("slw", "Sliding window averager.", "slw [slw_options]");
    parser.addOption(QCommandLineOption({"s", "size"} , "Window size in seconds.", "size"));
    parser.addPositionalArgument("input", "Filename of input.", "<input>");
    parser.addPositionalArgument("output", "Filename of output.", "<output>");

  } else {
    parser.showHelp(0);
  }

  parser.process(app);

  if (command == "slw") {

    int size = parser.value("s").toInt();

    const QString ifile = args[args.count()-2];
    const QString ofile = args[args.count()-1];

    ThermoDAQ2SlidingWindowConverter *converter = new ThermoDAQ2SlidingWindowConverter(size, ifile, ofile, &app);
    QObject::connect(converter, SIGNAL(finished()), &app, SLOT(quit()));
    QTimer::singleShot(0, converter, SLOT(run()));
  }

  return app.exec();
}
