#ifndef DEFOMEASUREMENTSELECTIONMODEL_H
#define DEFOMEASUREMENTSELECTIONMODEL_H

#include <QObject>
#include "DefoMeasurement.h"

class DefoMeasurementSelectionModel : public QObject
{
  Q_OBJECT
public:

  explicit DefoMeasurementSelectionModel(QObject *parent = 0);
  DefoMeasurement* getSelection();
  DefoMeasurement* getSelection() const;

public slots:

  void setSelection(DefoMeasurement* selection);

protected:

  DefoMeasurement* selection_;

signals:

  void selectionChanged(DefoMeasurement* selection);
};

class DefoMeasurementPairSelectionModel : public QObject
{
    Q_OBJECT
public:

  explicit DefoMeasurementPairSelectionModel(QObject *parent = 0);
  DefoMeasurementPair* getSelection() const;

public slots:

  void setSelection(DefoMeasurementPair* selection);

protected:

  DefoMeasurementPair* selection_;

signals:

  void selectionChanged(DefoMeasurementPair* selection);

};

#endif // DEFOMEASUREMENTSELECTIONMODEL_H
