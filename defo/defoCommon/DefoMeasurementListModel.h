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
  void appendMeasurementPoints(
      DefoMeasurement* measurement
    , const DefoPointCollection* points
  );

  void write(const QDir& path);
  void writePoints(const QDir& path);
  void clear();
  void read(const QString& filename);
  void readPoints(const QDir& path);

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

#endif // _DEFOMEASUREMENTLISTMODEL_H
