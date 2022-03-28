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

#include <nqlogger.h>
#include <ApplicationConfig.h>

#include "Controller.h"

int main(int argc, char *argv[])
{
  QCoreApplication app(argc, argv);
  QCoreApplication::setApplicationName("thermoControl2");
  QCoreApplication::setApplicationVersion("1.0");

  QCommandLineParser parser;
  parser.setApplicationDescription("Remote control for thermoDAQ2");
  parser.addHelpOption();

  parser.addOption(QCommandLineOption({"d", "debug"} , "Switch to debugging mode."));

  parser.addPositionalArgument("<command>",
      "One of the following commands:\n"
      " ThermoDAQ2\n"
      " -------------\n"
      "   start\n"
      "   stop\n"
      " Huber Unistat\n"
      " -------------\n"
      "   getKp\n"
      "   setKp\n"
      "   getTn\n"
      "   setTn\n"
      "   getTv\n"
      "   setTv\n"
      "   setPID\n"
      "   setSetPoint\n"
      "   setTemperatureControl\n"
      " Rohde & Schwartz NGE103B\n"
      " ------------------------\n"
      "   setOutputState\n"
      "   setVoltage\n"
      "   setCurrent");

  parser.parse(app.arguments());

  const QStringList args = parser.positionalArguments();

  if (args.count()<1) {
    parser.showHelp(0);
  }

  const QString command = args.first();

  if (command == "start") {
    parser.clearPositionalArguments();
    if (args.count()!=0) parser.showHelp(0);

  } else if (command == "stop") {
    parser.clearPositionalArguments();
    if (args.count()!=0) parser.showHelp(0);

  } else if (command == "getKp") {
    parser.clearPositionalArguments();
    parser.addPositionalArgument("getKp", "Get chiller Kp PID parameter");
    if (args.count()!=1) parser.showHelp(0);

  } else if (command == "setKp") {
    parser.clearPositionalArguments();
    parser.addPositionalArgument("setKp", "Set chiller Kp PID parameter");
    parser.addPositionalArgument("<Kp>", "Kp");
    if (args.count()!=2) parser.showHelp(0);

  } else if (command == "getTn") {
    parser.clearPositionalArguments();
    parser.addPositionalArgument("getTn", "Get chiller Tn PID parameter");
    if (args.count()!=1) parser.showHelp(0);

  } else if (command == "setTn") {
    parser.clearPositionalArguments();
    parser.addPositionalArgument("setTn", "Set chiller Tn PID parameter");
    parser.addPositionalArgument("<Tn>", "Tn");
    if (args.count()!=2) parser.showHelp(0);

  } else if (command == "getTv") {
    parser.clearPositionalArguments();
    parser.addPositionalArgument("getTv", "Get chiller Tv PID parameter");
    if (args.count()!=1) parser.showHelp(0);

  } else if (command == "setTv") {
    parser.clearPositionalArguments();
    parser.addPositionalArgument("setTv", "Set chiller Tv PID parameter");
    parser.addPositionalArgument("<Tv>", "Tv");
    if (args.count()!=2) parser.showHelp(0);

  } else if (command == "setPID") {
    parser.clearPositionalArguments();
    parser.addPositionalArgument("setPID", "Set chiller PID parameters");
    parser.addPositionalArgument("<Kp>", "Kp");
    parser.addPositionalArgument("<Tn>", "Tn");
    parser.addPositionalArgument("<Tv>", "Tv");
    if (args.count()!=4) parser.showHelp(0);

  } else if (command == "setSetPoint") {
    parser.clearPositionalArguments();
    parser.addPositionalArgument("setSetPoint", "Set chiller temperature set point");
    parser.addPositionalArgument("<temperature>", "temperature");
    if (args.count()!=2) parser.showHelp(0);

  } else if (command == "setTemperatureControl") {
    parser.clearPositionalArguments();
    parser.addPositionalArgument("setTemperatureControl", "Set state of chiller temperature control");
    parser.addPositionalArgument("<state>", "1 = on / 0 = off");
    if (args.count()!=2) parser.showHelp(0);

  } else if (command == "setTemperatureControl") {
    parser.clearPositionalArguments();
    parser.addPositionalArgument("setTemperatureControl", "Set state of chiller temperature control");
    parser.addPositionalArgument("<state>", "1 = on / 0 = off");
    if (args.count()!=2) parser.showHelp(0);

  } else if (command == "setOutputState") {
    parser.clearPositionalArguments();
    parser.addPositionalArgument("setOutputState", "Set power supply output state");
    parser.addPositionalArgument("<channel>", "channel 1-3");
    parser.addPositionalArgument("<state>", "1 = on / 0 = off");
    if (args.count()!=3) parser.showHelp(0);

  } else if (command == "setVoltage") {
    parser.clearPositionalArguments();
    parser.addPositionalArgument("setVoltage", "Set voltage");
    parser.addPositionalArgument("<channel>", "channel 1-3");
    parser.addPositionalArgument("<voltage>", "voltage");
    if (args.count()!=3) parser.showHelp(0);

  } else if (command == "setCurrent") {
    parser.clearPositionalArguments();
    parser.addPositionalArgument("setCurrent", "Set current");
    parser.addPositionalArgument("<channel>", "channel 1-3");
    parser.addPositionalArgument("<current>", "current");
    if (args.count()!=3) parser.showHelp(0);

  } else {
    parser.showHelp(0);
  }

  parser.process(app);

  if (parser.isSet("d")) {
    NQLogger::instance()->addActiveModule("*");
    NQLogger::instance()->addDestiniation(stdout, NQLog::Debug);
  }

  QStringList parameters;
    
  if (command == "setKp") {
    parameters << args.last();
  } else if (command == "setTn") {
    parameters << args.last();
  } else if (command == "setTv") {
    parameters << args.last();
  } else if (command == "setPID") {
    parameters << args[args.count()-3];
    parameters << args[args.count()-2];
    parameters << args[args.count()-1];
  } else if (command == "setSetPoint") {
    parameters << args.last();
  } else if (command == "setTemperatureControl") {
    parameters << args.last();
  } else if (command == "setOutputState") {
    parameters << args[args.count()-2];
    parameters << args[args.count()-1];
  } else if (command == "setVoltage") {
    parameters << args[args.count()-2];
    parameters << args[args.count()-1];
  } else if (command == "setCurrent") {
    parameters << args[args.count()-2];
    parameters << args[args.count()-1];
  }

  ApplicationConfig::instance();
  ApplicationConfig::instance()->append(std::string(Config::CMSTkModLabBasePath) + "/thermo/thermo2/thermo2.cfg", "main");

  Controller controller(command, parameters);
  QTimer::singleShot(0, &controller, SLOT(connectToServer()));

  return app.exec();
}
