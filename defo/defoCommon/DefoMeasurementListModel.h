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

#ifndef _DEFOMEASUREMENTLISTMODEL_H
#define _DEFOMEASUREMENTLISTMODEL_H

#include <map>
#include <QObject>
#include <QDateTime>
#include <QMutex>
#include <QMutexLocker>
#include <QDir>

#include "DefoMeasurement.h"

class DefoMeasurementListModel : public QObject
{
    Q_OBJECT

public:
  explicit DefoMeasurementListModel(QObject *parent = 0);

  // Deformation measurements
  int getMeasurementCount() const;
  const DefoMeasurement* getMeasurement(int index) const;
  DefoMeasurement* getMeasurement(int index);

  void addMeasurement(DefoMeasurement* measurement);
  const DefoPointCollection* getMeasurementPoints(
      DefoMeasurement* measurement
  );
  void setMeasurementPoints(
      DefoMeasurement* measurement
    , const DefoPointCollection* points
  );

  void write(const QDir& path);
  void exportMeasurement(const QDir& path, const QString& filename);
  void writePoints(const QDir& path);
  void clear();
  void read(const QString& filename);
  void readPoints(const QDir& path);

public slots:

  void appendMeasurementPoints(
      DefoMeasurement* measurement
    , const DefoPointCollection* points
  );

protected:
  std::vector<DefoMeasurement*> measurementList_;

  typedef std::map<DefoMeasurement*, const DefoPointCollection*> PointMap;
  PointMap points_;

  void cleanupPoints(DefoPointCollection* points);

  // For thread safety
  QMutex mutex_;

signals:
  void measurementCountChanged(int count);
  void pointsUpdated(const DefoMeasurement* measurement);

};

class DefoMeasurementPairListModel : public QObject
{
    Q_OBJECT

public:
  explicit DefoMeasurementPairListModel(QObject *parent = 0);

  // Deformation measurements
  int getMeasurementPairCount() const;
  const DefoMeasurementPair* getMeasurementPair(int index) const;
  DefoMeasurementPair* getMeasurementPair(int index);
  DefoMeasurementPair* findMeasurementPair(DefoMeasurement* reference,
                                           DefoMeasurement* deformed);

  void addMeasurementPair(DefoMeasurement* reference,
                          DefoMeasurement* deformed);
  void addMeasurementPair(DefoMeasurementPair* measurementPair);

protected:
  std::vector<DefoMeasurementPair*> measurementPairList_;

  // For thread safety
  QMutex mutex_;

signals:
  void measurementPairCountChanged(int count);
};

#endif // _DEFOMEASUREMENTLISTMODEL_H
