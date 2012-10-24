#ifndef _DEFOMEASUREMENT_H
#define _DEFOMEASUREMENT_H

#include <QDateTime>
#include <QImage>
#include <QColor>
#include <QDir>

//#include "DefoSurface.h"
#include "DefoSquare.h"
#include "DefoPoint.h"
#include "DefoState.h"

#include "DefoPointRecognitionModel.h"
#include "DefoConradModel.h"
#include "DefoJulaboModel.h"
#include "DefoKeithleyModel.h"

class DefoMeasurementBase {
public:
  DefoMeasurementBase(const QString& imageLocation);

  virtual bool isPreview() const = 0;

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
  QString imageLocation_;

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

class DefoPreviewMeasurement :
      public DefoMeasurementBase
{
public:
  DefoPreviewMeasurement(const QString& imageLocation);

  bool isPreview() const { return true; }
};

class DefoMeasurement :
      public DefoMeasurementBase
{
public:
  DefoMeasurement(const QString& imageLocation);

  bool isPreview() const { return false; }

  void setImageLocation(const QString& imageLocation);
  void readExifData();
  void acquireData(const DefoPointRecognitionModel* model);
  void acquireData(const DefoConradModel* model);
  void acquireData(const DefoJulaboModel* model);
  void acquireData(const DefoKeithleyModel* model);

  void write(const QDir& path);

protected:

  std::vector<int> pointRecognitionThresholds_;

  // Exif information
  float exifFocalLength_;
  float exifExposureTime_;
  QString exifExposureTimeString_;
  float exifAperture_;
  int exifISO_;

  // Conrad information
  State conradState_;
  std::vector<State> panelStates_;
  State ledState_;

  // Julabo information
  State julaboState_;
  bool circulatorState_;
  float bathTemperature_;

  // Keithley information
  State keithleyState_;
  std::vector<State> temperatureSensorStates_
  std::vector<float> temperatures_
};

#endif // _DEFOMEASUREMENT_H
