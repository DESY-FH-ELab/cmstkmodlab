#ifndef DEFOMEASUREMENTSELECTIONMODEL_H
#define DEFOMEASUREMENTSELECTIONMODEL_H

#include <QObject>
#include "DefoMeasurement.h"

class DefoMeasurementSelectionModel : public QObject
{
    Q_OBJECT
public:
  explicit DefoMeasurementSelectionModel(QObject *parent = 0);
  const DefoMeasurementBase* getSelection() const;

public slots:
  void setSelection(const DefoMeasurementBase* selection);

protected:
  const DefoMeasurementBase* selection_;

signals:
  void selectionChanged(const DefoMeasurementBase* selection);

};

#endif // DEFOMEASUREMENTSELECTIONMODEL_H
