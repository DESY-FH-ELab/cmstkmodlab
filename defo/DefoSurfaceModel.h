#ifndef _DEFOSURFACEMODEL_H
#define _DEFOSURFACEMODEL_H

#include <map>
#include <QObject>
#include <QDateTime>
#include "DefoSurface.h"
#include "DefoMeasurement.h"

// TODO Place reference_ in measurementList_ and provide a good way to access
// or change it.

class DefoSurfaceModel : public QObject
{
    Q_OBJECT

public:
  explicit DefoSurfaceModel(QObject *parent = 0);
  ~DefoSurfaceModel();

  // Reference surface
  bool hasReferenceMeasurement() const;
  const DefoMeasurement* getReferenceMeasurement() const;
  void setReferenceMeasurement(DefoMeasurement* reference);

  // Deformation measurements
  int getMeasurementCount() const;
  const DefoMeasurement& getMeasurement(int index) const;
  void addMeasurement(const DefoMeasurement& measurement);
  void removeMeasurement(int index);
  // int indexOf(const DefoSurface& surface) const;

protected:
  DefoMeasurement* reference_;
//  std::map<QDateTime,DefoSurface> deformedList_;
  std::vector<DefoMeasurement> measurementList_;

signals:
  void referenceChanged();
  void measurementCountChanged();

public slots:

};

#endif // _DEFOSURFACEMODEL_H
