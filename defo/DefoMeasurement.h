#ifndef _DEFOMEASUREMENT_H
#define _DEFOMEASUREMENT_H

#include <QDateTime>
#include <QImage>
#include <QColor>

//#include "DefoSurface.h"
#include "DefoSquare.h"
#include "DefoPoint.h"

class DefoMeasurement {
public:
  DefoMeasurement(const QString& imageLocation);

  const QDateTime& getTimeStamp() const;
  QImage getImage() const;

  const DefoPointCollection* findPoints(
      const QRect* searchArea
    , int step1Threshold
    , int step2Threshold
    , int step3Threshold
    , int halfSquareWidth
  ) const;

protected:
  /// (Local) date and time of measurement.
  const QDateTime timestamp_;
  /// Image of the actual 'raw' measurement.
  const QString imageLocation_;
  /// Information about the temperature (gradient)
  // TODO implement TemperatureSensorStatus
  // TODO implement ChillerStatus
  // TODO implement CameraStatus: EXIF data in picture file
  // TODO implement DevicePowerStatus or LedPanelPowerStatus

  /*
    Analysis depentent information, does not belong in 'measurement' class.
    Processing can happen here, but storing it is not the purpose of this
    object.
    */
  DefoPoint getCenterOfGravity(
      const QImage& image
    , const QRect &area
    , int threshold
  ) const;

  void determinePointColors(
      const QImage& image
    , DefoPointCollection* points
    , int halfSquareWidth
    , int threshold
  ) const;

  const QColor getAverageColor(
      const QImage& image
    , const QRect& area
    , int threshold
  ) const;

};

#endif // _DEFOMEASUREMENT_H
