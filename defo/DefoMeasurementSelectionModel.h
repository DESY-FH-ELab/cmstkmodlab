#ifndef DEFOMEASUREMENTSELECTIONMODEL_H
#define DEFOMEASUREMENTSELECTIONMODEL_H

#include <QObject>
#include "DefoMeasurement.h"

class DefoMeasurementSelectionModel : public QObject
{
    Q_OBJECT
public:
  explicit DefoMeasurementSelectionModel(QObject *parent = 0);
  const DefoMeasurement* getSelection() const;

public slots:
  void setSelection(const DefoMeasurement* selection);

protected:
  const DefoMeasurement* selection_;

signals:
  void selectionChanged(const DefoMeasurement* selection);

};

#endif // DEFOMEASUREMENTSELECTIONMODEL_H
