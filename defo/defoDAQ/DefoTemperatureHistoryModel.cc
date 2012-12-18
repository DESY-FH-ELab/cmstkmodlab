#include "DefoTemperatureHistoryModel.h"

DefoTemperatureHistoryModel::DefoTemperatureHistoryModel(QObject *parent) :
    QObject(parent)
  , temperatures_(0)
  , minPos_(-1)
  , maxPos_(-1)
{
}

DefoTemperatureHistoryModel::const_iterator
    DefoTemperatureHistoryModel::begin() const
{
  return temperatures_.begin();
}

DefoTemperatureHistoryModel::const_iterator
    DefoTemperatureHistoryModel::end() const
{
  return temperatures_.end();
}

double DefoTemperatureHistoryModel::getSize() const
{
  return temperatures_.size();
}

void DefoTemperatureHistoryModel::addValue(double value)
{
  // Get current size, i.e. index of last value when adding one
  unsigned int newIndex = getSize();

  // ... which happens to be done right here
  temperatures_.push_back( value );
  emit valueAdded( value );

  // Check if it's a new minimum
  if ( minPos_ == -1 || temperatures_[minPos_] > value ) {
    minPos_ = newIndex;
    emit mininumChanged( value );
  }

  // Check if it's a new maximum
  if ( maxPos_ == -1 || temperatures_[maxPos_] < value ) {
    maxPos_ = newIndex;
    emit maximumChanged( value );
  }
}

double DefoTemperatureHistoryModel::getLatest() const
{
  return temperatures_.back();
}

double DefoTemperatureHistoryModel::getMaximum() const {

  if ( maxPos_ > -1 )
    return temperatures_[maxPos_];
  else
    return 0.0; // TODO return an exception

}

double DefoTemperatureHistoryModel::getMinimum() const {

  if ( minPos_ > -1 )
    return temperatures_[minPos_];
  else
    return 0.0; // TODO return an exception

}
