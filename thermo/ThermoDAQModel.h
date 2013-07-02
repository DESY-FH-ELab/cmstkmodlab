#ifndef THERMODAQMODEL_H
#define THERMODAQMODEL_H

#include <QObject>

#include "JulaboModel.h"
#include "KeithleyModel.h"
#include "HamegModel.h"
#include "PfeifferModel.h"

template <typename value_type> class ThermoDAQValue
{
public:
    ThermoDAQValue(unsigned int time, value_type value) {
        time_ = time;
        value_ = value;
    }

    unsigned int& time() { return time_; }
    value_type& value() { return value_; }

protected:

    unsigned int time_;
    value_type value_;
};

template <typename value_type> class ThermoDAQValueVector : public std::vector<ThermoDAQValue<value_type> >
{
public:
    ThermoDAQValueVector() {

    }

    void push(unsigned time, value_type value) {
        std::vector<ThermoDAQValue<value_type> >::push_back(ThermoDAQValue<value_type>(time, value));
    }

    bool pushIfChanged(unsigned time, value_type value) {
        if (std::vector<ThermoDAQValue<value_type> >::size()>0 && lastValue()==value) return false;
        std::vector<ThermoDAQValue<value_type> >::push_back(ThermoDAQValue<value_type>(time, value));
        return true;
    }

    unsigned int& lastTime() { return std::vector<ThermoDAQValue<value_type> >::back().time(); }
    value_type& lastValue() { return std::vector<ThermoDAQValue<value_type> >::back().value(); }
};

class ThermoDAQModel : public QObject
{
    Q_OBJECT
public:
    explicit ThermoDAQModel(JulaboModel* julaboModel,
                            KeithleyModel* keithleyModel,
                            HamegModel* hamegModel,
                            PfeifferModel* pfeifferModel,
                            QObject *parent = 0);

protected slots:
    void julaboInfoChanged();
    void keithleySensorStateChanged(unsigned int sensor, State newState);
    void keithleyTemperatureChanged(unsigned int sensor, double temperature);

protected:
  JulaboModel* julaboModel_;
  KeithleyModel* keithleyModel_;
  HamegModel* hamegModel_;
  PfeifferModel* pfeifferModel_;

  unsigned int currentTime_;
  unsigned int currentTime();

  // JULABO DATA
  ThermoDAQValueVector<float> julaboWorkingTemperature_;
  ThermoDAQValueVector<float> julaboBathTemperature_;

  // KEITHLEY DATA
  ThermoDAQValueVector<State> keithleySensorState_[10];
  ThermoDAQValueVector<float> keithleyTemperature_[10];

signals:

  void daqMessage(const QString & message);
};

#endif // THERMODAQMODEL_H
