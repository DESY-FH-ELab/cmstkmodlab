#ifndef _DEFOMEASUREMENTLISTMODEL_H
#define _DEFOMEASUREMENTLISTMODEL_H

#include <map>
#include <QObject>
#include <QDateTime>
#include <QMutex>
#include <QMutexLocker>
#include "DefoSurface.h"
#include "DefoMeasurement.h"

class DefoMeasurementListModel : public QObject
{
    Q_OBJECT

public:
  explicit DefoMeasurementListModel(QObject *parent = 0);

  // Deformation measurements
  int getMeasurementCount() const;
  const DefoMeasurement* getMeasurement(int index) const;

  void addMeasurement(const DefoMeasurement* measurement);
  const DefoPointCollection* getMeasurementPoints(
      const DefoMeasurement* measurement
  );
  void setMeasurementPoints(
      const DefoMeasurement* measurement
    , const DefoPointCollection* points
  );
  void appendMeasurementPoints(
      const DefoMeasurement* measurement
    , const DefoPointCollection* points
  );

protected:
  std::vector<const DefoMeasurement*> measurementList_;

  typedef std::map<const DefoMeasurement*, const DefoPointCollection*> PointMap;
  PointMap points_;

  // For thread safety
  QMutex mutex_;

signals:
  void measurementCountChanged(int count);
  void pointsUpdated(const DefoMeasurement* measurement);

};

#endif // _DEFOMEASUREMENTLISTMODEL_H
