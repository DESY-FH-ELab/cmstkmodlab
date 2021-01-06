/////////////////////////////////////////////////////////////////////////////////
//                                                                             //
//               Copyright (C) 2011-2021 - The DESY CMS Group                  //
//                           All rights reserved                               //
//                                                                             //
//      The CMStkModLab source code is licensed under the GNU GPL v3.0.        //
//      You have the right to modify and/or redistribute this source code      //
//      under the terms specified in the license, which may be found online    //
//      at http://www.gnu.org/licenses or at License.txt.                      //
//                                                                             //
/////////////////////////////////////////////////////////////////////////////////

#ifndef THERMO2SCRIPTTHREAD_H
#define THERMO2SCRIPTTHREAD_H

#include <QObject>
#include <QThread>
#include <QScriptEngine>

class Thermo2ScriptModel;

#include "HuberUnistat525wModel.h"
#include "MartaModel.h"
#include "RohdeSchwarzNGE103BModel.h"
#include "KeithleyDAQ6510Model.h"

class Thermo2ScriptThread : public QThread
{
public:
  explicit Thermo2ScriptThread(Thermo2ScriptModel* scriptModel,
                               HuberUnistat525wModel* huberModel,
															 MartaModel* martaModel,
                               RohdeSchwarzNGE103BModel* nge103BModel,
                               KeithleyDAQ6510Model* keithleyModel,
                               QObject *parent = 0);

  void executeScript(const QString & script);
  void abortScript();

protected:
  void run();

  QString script_;
  QScriptEngine* engine_;

  Thermo2ScriptModel* scriptModel_;
  HuberUnistat525wModel* huberModel_;
  MartaModel* martaModel_;
  RohdeSchwarzNGE103BModel* nge103BModel_;
  KeithleyDAQ6510Model* keithleyModel_;
};

#endif // THERMO2SCRIPTTHREAD_H
