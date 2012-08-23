#ifndef DEFOJULABOMODEL_H
#define DEFOJULABOMODEL_H

#include <math.h>

#include <QString>
#include <QTimer>

#include "DefoState.h"

#ifdef USE_FAKEIO
#include "devices/Julabo/JulaboFP50Fake.h"
typedef JulaboFP50Fake JulaboFP50_t;
#else
#include "devices/Julabo/JulaboFP50.h"
typedef JulaboFP50 JulaboFP50_t;
#endif


/// A numerical parameter value class bounded by (inclusive) limits.
template <class T> class Parameter {
public:
  // TODO Include units
  Parameter(T min, T max, unsigned int precision) :
    max_(max), min_(min), precision_(precision)
  {
    // Start with lowest value allowed.
    value_ = min;
  }
  /// Returns the smallest value the parameter can have.
  const T& getMinimum() const {return min_;}
  /// Returns the greatest value the parameter can have.
  const T& getMaximum() const {return max_;}
  /**
    Returns the precision of the parameter. To be interpreted as 1e-{precision}.
    */
  unsigned int getPrecision() const {return precision_;}
  /// Returns the current value.
  const T& getValue() const {return value_;}
  /**
    \brief Attempts to set the value.
    This validates it within the range and automatically
    rounds it.
    \return Retursn false when out of range, true otherwise.
    */
  bool setValue(T value) {
    if ( value >= min_ && value <= max_ ) {
      // TODO implement rounding
      value_ = round( value, precision_ );
      return true;
    }
    else
      return false;
  }

protected:
  /// Rounds a number type T with fixed precision.
  static T round(const T& value, unsigned int precision) {
    T result;
    // try not to perform unnescesary calculations
    if ( precision > 0 ) {
      double factor = pow10(precision);
      T fixed = floor( value*factor + 0.5 );
      result = fixed / factor;
    }
    else
      result = floor( value + 0.5 );

    return result;
  }

  const T max_; ///< Maximal value of the parameter.
  const T min_; ///< Minimal value of the paramter.
  const unsigned int precision_; ///< Resolution of the parameter, i.e. 1e-RES.
  T value_; ///< Current value of the parameter.
};
typedef Parameter<float> ParameterFloat;
typedef Parameter<unsigned int> ParameterUInt;


/**
  Command and control model of the Julabo chiller.
  */
class DefoJulaboModel : public QObject
{
    Q_OBJECT
public:
  explicit DefoJulaboModel(float updateInterval = 5, QObject *parent = 0);
  ~DefoJulaboModel();

  const State& getDeviceState() const;

  const ParameterFloat& getProportionalParameter() const;
  const ParameterUInt& getIntegralParameter() const;
  const ParameterUInt& getDifferentialParameter() const;

  bool isCirculatorEnabled() const;

  const ParameterUInt& getPumpPressureParameter() const;
  const ParameterFloat& getWorkingTemperatureParameter() const;
  float getBathTemperature() const;
  float getSafetySensorTemperature() const;
  unsigned int getPower() const;

public slots:
  void setDeviceEnabled( bool enabled );

  void setProportionalValue( double value );
  void setIntegralValue( int value );
  void setDifferentialValue( int value );

  void setCirculatorEnabled( bool enabled );

  void setPumpPressureValue( int pump );

  void setWorkingTemperatureValue( double temperature );


protected:
  static const QString JULABO_PORT;

  VJulaboFP50* controller_;
  void renewController( const QString& port );
  void initialize();
  void close();

  /// Time interval between cache refreshes; in seconds.
  const float updateInterval_;
  QTimer* timer_;

  // cached config information
  State state_;
  void setDeviceState( State state );

  template <class T> void updateParameterCache(
      Parameter<T>& parameter
    , const T& value
  );

  ParameterFloat proportional_; ///< Proportional range >Xp<; units K
  ParameterUInt integral_; ///< Resetting time >Tn<; units s
  ParameterUInt differential_; ///< Lead time >Tv<; units s

  bool circulatorEnabled_;
  ParameterUInt pumpPressure_; ///< Pump pressure stage; no units
  ParameterFloat workingTemperature_; ///< Working temperature; units °C

  float bathTemperature_; ///< Current cooling bath temperature; units °C
  float safetySensorTemperature_;
  unsigned int power_; ///< Current fraction of maximum power; units %

protected slots:
  void updateInformation();

signals:
  void deviceStateChanged( State newState );
  void informationChanged();

};

#endif // DEFOJULABOMODEL_H
