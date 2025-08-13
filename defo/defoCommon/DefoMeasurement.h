/////////////////////////////////////////////////////////////////////////////////
//                                                                             //
//               Copyright (C) 2011-2025 - The DESY CMS Group                  //
//                           All rights reserved                               //
//                                                                             //
//      The CMStkModLab source code is licensed under the GNU GPL v3.0.        //
//      You have the right to modify and/or redistribute this source code      //
//      under the terms specified in the license, which may be found online    //
//      at http://www.gnu.org/licenses or at License.txt.                      //
//                                                                             //
/////////////////////////////////////////////////////////////////////////////////

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

class DefoMeasurement
{
public:

  DefoMeasurement(const QDateTime& timestamp);
  DefoMeasurement(const QString& imageLocation, bool preview);
  DefoMeasurement(const QStringList& imageLocations);

  bool isPreview() const { return previewImage_; }

  void setTimeStamp(const QDateTime& dt);
  const QDateTime& getTimeStamp() const;
  QImage getImage() const;

  float getFocalLength() const { return exifFocalLength_; }
  float getExposureTime() const { return exifExposureTime_; }
  const QString& getExposureTimeString() const { return exifExposureTimeString_; }
  float getAperture() const { return exifAperture_; }
  int getISO() const { return exifISO_; }

  int getWidth() const;
  int getHeight() const;

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

  void setImageLocation(const QString& imageLocation);
  void setImageLocations(const QStringList& imageLocations);
  void readExifData();
  void acquireData(const DefoCameraModel* model);
  void acquireData(const DefoPointRecognitionModel* model);
  void acquireData(const DefoConradModel* model);
  void acquireData(const DefoJulaboModel* model);
  void acquireData(const KeithleyModel* model);

  virtual void write(const QDir& path);
  virtual void read(const QDir&path);

  void readImages();

protected:

  /// (Local) date and time of measurement.
  QDateTime timestamp_;
  /// Image of the actual 'raw' measurement.
  QStringList imageLocations_;

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
