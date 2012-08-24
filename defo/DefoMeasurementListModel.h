#ifndef _DEFOSURFACEMODEL_H
#define _DEFOSURFACEMODEL_H

#include <map>
#include <QObject>
#include <QDateTime>
#include "DefoSurface.h"
#include "DefoMeasurement.h"

// TODO Place reference_ in measurementList_ and provide a good way to access
// or change it.

class DefoMeasurementListModel : public QObject
{
    Q_OBJECT

public:
  explicit DefoMeasurementListModel(QObject *parent = 0);
//  ~DefoMeasurementListModel();

  // Reference surface
//  bool hasReferenceMeasurement() const;
//  const DefoMeasurement* getReferenceMeasurement() const;
//  void setReferenceMeasurement(DefoMeasurement* reference);

  // Deformation measurements
  int getMeasurementCount() const;
  const DefoMeasurement& getMeasurement(int index) const;

  void addMeasurement(const DefoMeasurement& measurement, bool select = true);
//  void removeMeasurement(int index);
  // int indexOf(const DefoSurface& surface) const;

  const DefoMeasurement& getSelection() const;

public slots:
  void setSelectionIndex(int index);

protected:
//  DefoMeasurement* reference_;
//  std::map<QDateTime,DefoSurface> deformedList_;
  std::vector<DefoMeasurement> measurementList_;
  int selectedIndex_;

signals:
//  void referenceChanged();
  void measurementCountChanged(int count);
  void selectionChanged(int index);

};

#endif // _DEFOSURFACEMODEL_H
