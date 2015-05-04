#ifndef DEFODAQMODEL_H
#define DEFODAQMODEL_H

#include <QObject>
#include <QVector>
#include <QDateTime>
#include <QThread>
#include <QMutex>
#include <QMutexLocker>

#include <qwt_date.h>

#include "DefoConradModel.h"
#include "DefoJulaboModel.h"
#include "KeithleyModel.h"

typedef struct {
    QDateTime      dt;
    bool           daqState;

    float          bathTemperature;
    float          workingTemperature;
    int            circulator;

    int            channelActive[10];
    float          temperature[10];

    int            conradState[8];
} Measurement_t;

template <typename value_type> class DefoDAQValueVector : public QVector<double>
{
public:
    DefoDAQValueVector() {

    }

    void clear() {
        QVector<double>::clear();
        values_.clear();
    }

    bool push(const QDateTime& time, value_type value) {
        bool ret = QVector<double>::size()>0 && lastValue()==value;
        QVector<double>::append(QwtDate::toDouble(time));
        values_.append(value);
        return ret;
    }

    bool pushIfChanged(const QDateTime& time, value_type value) {
        if (QVector<double>::size()>0 && lastValue()==value) return false;
        QVector<double>::append(QwtDate::toDouble(time));
        values_.append(value);
        return true;
    }

    double& lastTime() { return QVector<double>::last(); }
    const double& lastTime() const { return QVector<double>::last(); }
    value_type& lastValue() { return values_.last(); }

    const double* constTimes() const { return QVector<double>::constData(); }
    const value_type* constValues() const { return values_.constData(); }

protected:
    QVector<value_type> values_;
};

class DefoDAQModel : public QObject
{
    Q_OBJECT
public:
    explicit DefoDAQModel(DefoConradModel* conradModel,
			  DefoJulaboModel* julaboModel,
			  KeithleyModel* keithleyModel,
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

    void julaboInfoChanged();
    void keithleySensorStateChanged(unsigned int sensor, State newState);
    void keithleyTemperatureChanged(unsigned int sensor, double temperature);
    void conradSwitchStateChanged(DefoConradModel::DeviceSwitch device, State newState);

    void clearHistory();

protected:

  bool daqState_;

  DefoConradModel* conradModel_;
  DefoJulaboModel* julaboModel_;
  KeithleyModel* keithleyModel_;

  QMutex mutex_;

  QDateTime currentTime_;

  Measurement_t measurement_;

  template <typename T> bool updateIfChanged(T &variable, T newValue) {
      if (variable==newValue) return false;
      variable = newValue;
      return true;
  }

  // JULABO DATA
  State julaboState_;
  bool julaboCirculator_;
  float julaboWorkingTemperature_;
  float julaboBathTemperature_;

  // KEITHLEY DATA
  State keithleySensorState_[10];
  double keithleyTemperature_[10];
  
  // CONRAD DATA
  State conradSwitchState_[8];

signals:

  void daqMessage(const QString & message);
  void daqStateChanged(bool running);
  void newDataAvailable();
};

#endif // DEFODAQMODEL_H
