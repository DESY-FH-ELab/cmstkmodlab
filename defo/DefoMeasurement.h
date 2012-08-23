#ifndef _DEFOMEASUREMENT_H
#define _DEFOMEASUREMENT_H

#include <QDateTime>
#include <QImage>
#include <QColor>

//#include "DefoSurface.h"
//#include "DefoSquare.h"
//#include "DefoPoint.h"

class DefoMeasurement {
public:
  DefoMeasurement(const QImage& image);

  const QDateTime& getTimeStamp() const;
  const QImage& getImage() const;



//  const DefoSurface& getSurface() const;
//  const DefoPointCollection& getPoints() const;
//  void findPoints(const QRect* area);

protected:
  /// (Local) date and time of measurement.
  QDateTime timestamp_;
  /// Image of the actual 'raw' measurement.
  QImage image_;
  /// Information about the temperature (gradient)
  // TODO implement TemperatureSensorStatus
  // TODO implement ChillerStatus
  // TODO implement CameraStatus
  // TODO implement DevicePowerStatus or LedPanelPowerStatus

  /*
    ANALYSIS DEPENDENT INFORMATION. DOES NOT BELONG IN 'MEASUREMENT' CLASS.
    */
  /// Procesed information about the surface in the image.
//  DefoSurface surface_;

  /// Collection of the recognised points in the image.
//  DefoPointCollection points_;
//  void determinePointColors();

//  const QColor getAverageColor(const QRect& area) const;
//  const DefoPoint getCenterOfGravity(const QRect& area) const;


private:
//  int step1Threshold_;
//  int step2Threshold_;
//  int step3Threshold_;
//  unsigned int spacingAssumed_;
//  unsigned int halfSquareWidth_;
//  double blueishnessThreshold_;
//  unsigned int debugLevel_;

};

#endif // _DEFOMEASUREMENT_H
