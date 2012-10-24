#ifndef _DEFOMEASUREMENTLISTMODEL_H
#define _DEFOMEASUREMENTLISTMODEL_H

#include <map>
#include <QObject>
#include <QDateTime>
#include <QMutex>
#include <QMutexLocker>
#include <QDir>

#include "DefoSurface.h"
#include "DefoMeasurement.h"

class DefoMeasurementListModel : public QObject
{
    Q_OBJECT

public:
  explicit DefoMeasurementListModel(QObject *parent = 0);

  // Deformation measurements
  int getMeasurementCount() const;
  const DefoMeasurementBase* getMeasurement(int index) const;

  void addMeasurement(const DefoMeasurementBase* measurement);
  const DefoPointCollection* getMeasurementPoints(
      const DefoMeasurementBase* measurement
  );
  void setMeasurementPoints(
      const DefoMeasurementBase* measurement
    , const DefoPointCollection* points
  );
  void appendMeasurementPoints(
      const DefoMeasurementBase* measurement
    , const DefoPointCollection* points
  );

  void write(const QDir& path);

protected:
  std::vector<const DefoMeasurementBase*> measurementList_;

  typedef std::map<const DefoMeasurementBase*, const DefoPointCollection*> PointMap;
  PointMap points_;

  // For thread safety
  QMutex mutex_;

signals:
  void measurementCountChanged(int count);
  void pointsUpdated(const DefoMeasurementBase* measurement);

};

#endif // _DEFOMEASUREMENTLISTMODEL_H
