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

#ifndef _SCRIPTABLELEYBOLDGRAPHIXONE_H_
#define _SCRIPTABLELEYBOLDGRAPHIXONE_H_

#include <QObject>
#include <QMutex>
#include <QScriptValue>

#include <VScriptableDevice.h>

#include <LeyboldGraphixOneModel.h>

class ScriptableLeyboldGraphixOne : public VScriptableDevice
{
  Q_OBJECT
public:
  explicit ScriptableLeyboldGraphixOne(LeyboldGraphixOneModel* model,
				       QObject *parent = 0);

  void abort();

public slots:

  QScriptValue getPressure();

  void waitForPressureBelow(float pressure,
			                int timeout);

protected:

  LeyboldGraphixOneModel *model_;

  bool abortRequested_;

  // For thread safety
  QMutex mutex_;
};

#endif // _SCRIPTABLELEYBOLDGRAPHIXONE_H_
