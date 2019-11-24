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

//template <typename value_type> class ThermoDAQValue
//{
//public:
//    ThermoDAQValue(unsigned int time, value_type value) {
//        time_ = time;
//        value_ = value;
//    }

//    unsigned int& time() { return time_; }
//    const unsigned int& time() const { return time_; }
//    value_type& value() { return value_; }
//    const value_type& value() const { return value_; }

//protected:

//    unsigned int time_;
//    value_type value_;
//};

typedef struct {
  QDateTime      dt;
  bool           daqState;

  float          bathTemperature;
  float          workingTemperature;
  int            circulator;

  int            channelActive[10];
  float          temperature[10];

  int            gaugeStatus1;
  float          gaugePressure1;
  int            gaugeStatus2;
  float          gaugePressure2;

  int            powerRemote;
  int            powerOn;
  int            cv1;
  int            cv2;
  float          setVoltage1;
  float          setCurrent1;
  float          setVoltage2;
  float          setCurrent2;
  float          voltage1;
  float          current1;
  float          voltage2;
  float          current2;

  bool           iotaPumpEnabled;
  float          iotaActPressure;
  float          iotaSetPressure;
  float          iotaActFlow;
  float          iotaSetFlow;

  float          arduinoPressureA;
  float          arduinoPressureB;
} Measurement_t;

template <typename value_type> class Thermo2DAQValueVector : public QVector<qint64>
{
public:
  Thermo2DAQValueVector() {

  }

  void clear() {
    QVector<qint64>::clear();
    values_.clear();
  }

  bool push(const QDateTime& time, value_type value) {
    bool ret = QVector<qint64>::size()>0 && lastValue()==value;
    QVector<qint64>::append(time.toMSecsSinceEpoch());
    values_.append(value);
    return ret;
  }

  bool pushIfChanged(const QDateTime& time, value_type value) {
    if (QVector<qint64>::size()>0 && lastValue()==value) return false;
    QVector<qint64>::append(time.toMSecsSinceEpoch());
    values_.append(value);
    return true;
  }

  qint64& lastTime() { return QVector<qint64>::last(); }
  const qint64& lastTime() const { return QVector<qint64>::last(); }
  value_type& lastValue() { return values_.last(); }

  const qint64* constTimes() const { return QVector<qint64>::constData(); }
  const value_type* constValues() const { return values_.constData(); }

protected:
  QVector<value_type> values_;
};

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

  const Measurement_t& getMeasurement();

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

  Measurement_t measurement_;

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
