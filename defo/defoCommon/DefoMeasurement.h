#ifndef _DEFOMEASUREMENT_H
#define _DEFOMEASUREMENT_H

#include <utility>

#include <QDateTime>
#include <QImage>
#include <QPolygonF>
#include <QColor>
#include <QDir>

#include "DeviceState.h"

#include "DefoSurface.h"
#include "DefoSquare.h"
#include "DefoPoint.h"

#include "DefoCameraModel.h"
#include "DefoPointRecognitionModel.h"
#include "DefoConradModel.h"
#include "DefoJulaboModel.h"
#include "KeithleyModel.h"

class DefoMeasurement {
public:
  DefoMeasurement(const QString& imageLocation, bool preview);

  bool isPreview() const { return previewImage_; }

  void setTimeStamp(const QDateTime& dt);
  const QDateTime& getTimeStamp() const;
  QImage getImage() const;

  float getFocalLength() const { return exifFocalLength_; }
  float getExposureTime() const { return exifExposureTime_; }
  const QString& getExposureTimeString() const { return exifExposureTimeString_; }
  float getAperture() const { return exifAperture_; }
  int getISO() const { return exifISO_; }

  State getConradState() const { return conradState_; }
  State getPanelState(unsigned int panel) const { return panelStates_[panel]; }
  State getLEDState() const { return ledState_; }

  State getJulaboState() const { return julaboState_; }
  bool getCirculatorState() const { return circulatorState_; }
  float getBathTemperature() const { return bathTemperature_; }

  State getKeithleyState() const { return keithleyState_; }
  State getTemperatureSensorState(unsigned int sensor) const { return temperatureSensorStates_[sensor]; }
  float getTemperature(unsigned int sensor) const { return temperatures_[sensor]; }

  const QString& getComment() const { return comment_; }
  int getCalibAmplitude() const { return calibAmplitude_; }

  const DefoPointCollection* findPoints(
      const QRect* searchArea
    , const QPolygonF* roi
    , int step1Threshold
    , int step2Threshold
    , int step3Threshold
    , int halfSquareWidth
  ) const;

  void setImageLocation(const QString& imageLocation);
  void readExifData();
  void acquireData(const DefoCameraModel* model);
  void acquireData(const DefoPointRecognitionModel* model);
  void acquireData(const DefoConradModel* model);
  void acquireData(const DefoJulaboModel* model);
  void acquireData(const KeithleyModel* model);

  virtual void write(const QDir& path);
  virtual void read(const QDir&path);

protected:
  /// (Local) date and time of measurement.
  QDateTime timestamp_;
  /// Image of the actual 'raw' measurement.
  QString imageLocation_;

  QImage image_;

  bool previewImage_;

  std::vector<int> pointRecognitionThresholds_;
  int pointRecognitionHalfSquareWidth_;

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
  std::vector<State> temperatureSensorStates_;
  std::vector<float> temperatures_;

  QString comment_;
  int calibAmplitude_;

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

class DefoMeasurementPair : public std::pair<DefoMeasurement*,DefoMeasurement*> {

public:
  DefoMeasurementPair(DefoMeasurement* reference,
                      DefoMeasurement* deformed);

  void setSurface(const DefoSurface& surface);
  const DefoSurface& getSurface() const { return surface_; }

protected:
  DefoSurface surface_;
};

#endif // _DEFOMEASUREMENT_H
