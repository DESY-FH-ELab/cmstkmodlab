#include "DefoPointBin.h"

DefoPointBin::DefoPointBin(
    const DefoPoint::Axis& axis
  , DefoPoint* initialPoint
) :
    axis_(axis)
  , hasReferencePoint_(false)
{

  double value = initialPoint->getPosition(axis_);

  minValue_ = value;
  maxValue_ = value;
  totalValue_ = value;

  points_.push_back(initialPoint);

}

DefoPointBin & DefoPointBin::operator =(const DefoPointBin &pointBin) {
  DefoPointBin* copy = new DefoPointBin(pointBin.axis_, *(pointBin.begin()));
  return *copy;
}

DefoPointBin::const_iterator DefoPointBin::begin() const {
  return points_.begin();
}

DefoPointBin::const_iterator DefoPointBin::end() const {
  return points_.end();
}

void DefoPointBin::addPoint(DefoPoint *point) {

  // Store point
  points_.push_back(point);

  // Update statistics
  double pos = point->getPosition(axis_);

  if ( pos < minValue_ )
    minValue_ = pos;
  else if ( pos > maxValue_ )
    maxValue_ = pos;

  totalValue_ += pos;

}

double DefoPointBin::getMin() const {
  return minValue_;
}

double DefoPointBin::getMax() const {
  return maxValue_;
}

double DefoPointBin::getMean() const {
  return (totalValue_ / points_.size());
}

/// Set the index of all points in this bin for the current axis.
void DefoPointBin::setIndex(int index) {

  for ( std::vector<DefoPoint*>::iterator it = points_.begin()
      ; it < points_.end()
      ; ++it
  ) {
    // TODO setAxis(axis, value)
    if (axis_ == DefoPoint::X)
      (*it)->setIndexX(index);
    else
      (*it)->setIndexY(index);
  }

}

void DefoPointBin::setHasReferencePoint(bool hasReference) {
  hasReferencePoint_ = hasReference;
}

bool DefoPointBin::hasReferencePoint() const {
  return hasReferencePoint_;
}
