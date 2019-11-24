/////////////////////////////////////////////////////////////////////////////////
//                                                                             //
//               Copyright (C) 2011-2019 - The DESY CMS Group                  //
//                           All rights reserved                               //
//                                                                             //
//      The CMStkModLab source code is licensed under the GNU GPL v3.0.        //
//      You have the right to modify and/or redistribute this source code      //
//      under the terms specified in the license, which may be found online    //
//      at http://www.gnu.org/licenses or at License.txt.                      //
//                                                                             //
/////////////////////////////////////////////////////////////////////////////////

#ifndef THERMO2DAQMODEL_H
#define THERMO2DAQMODEL_H

#include <QObject>
#include <QVector>
#include <QDateTime>
#include <QThread>
#include <QMutex>
#include <QMutexLocker>

#include "RohdeSchwarzNGE103BModel.h"

class Thermo2DAQModel : public QObject
{
  Q_OBJECT
public:

  explicit Thermo2DAQModel(RohdeSchwarzNGE103BModel* nge103BModel,
                           QObject *parent = 0);

  QDateTime& currentTime();

  void customDAQMessage(const QString & message);
  void createDAQStatusMessage(QString & buffer);

  void myMoveToThread(QThread *thread);

  bool daqState() const { return daqState_; }

public slots:

  void startMeasurement();
  void stopMeasurement();

protected slots:

  void nge103BInfoChanged();

  void clearHistory();

protected:

  bool daqState_;

  RohdeSchwarzNGE103BModel* nge103BModel_;

  QMutex mutex_;

  QDateTime currentTime_;

  template <typename T> bool updateIfChanged(T &variable, T newValue) {
    if (variable==newValue) return false;
    variable = newValue;
    return true;
  }

  // Rohde & Schwarz NGE130B Data
  bool nge103BOutputState_[3];
  unsigned int nge103BOutputMode_[3];
  float nge103BVoltage_[3];
  float nge103BCurrent_[3];
  float nge103BMeasuredVoltage_[3];
  float nge103BMeasuredCurrent_[3];

signals:

  void daqMessage(const QString & message);
  void daqStateChanged(bool running);
  void newDataAvailable();
};

#endif // THERMO2DAQMODEL_H
