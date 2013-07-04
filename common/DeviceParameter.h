#ifndef DEVICEPARAMETER_H
#define DEVICEPARAMETER_H

/// A numerical parameter value class bounded by (inclusive) limits.
template <typename T> class DeviceParameter {
public:
  // TODO Include units
    DeviceParameter(T min, T max, unsigned int precision) :
    max_(max), min_(min), precision_(precision) {
    // Start with lowest value allowed.
    value_ = min;
  }
  /// Returns the smallest value the parameter can have.
  const T& getMinimum() const { return min_; }
  /// Returns the greatest value the parameter can have.
  const T& getMaximum() const { return max_; }
  /**
    Returns the precision of the parameter. To be interpreted as 1e-{precision}.
    */
  unsigned int getPrecision() const { return precision_; }
  /// Returns the current value.
  const T& getValue() const { return value_; }
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
      double factor = std::pow(10., (int)precision);
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
typedef DeviceParameter<double> DeviceParameterDouble;
typedef DeviceParameter<float> DeviceParameterFloat;
typedef DeviceParameter<int> DeviceParameterInt;
typedef DeviceParameter<unsigned int> DeviceParameterUInt;

#endif // DEVICEPARAMETER_H
