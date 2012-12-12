#ifndef DEFOMEASUREMENTINFOLISTTREEWIDGET_H
#define DEFOMEASUREMENTINFOLISTTREEWIDGET_H

#include <QTreeWidget>
#include <QTreeWidgetItem>

#include "DefoMeasurementSelectionModel.h"

class DefoMeasurementInfoListTreeWidget : public QTreeWidget
{
    Q_OBJECT
public:
  explicit DefoMeasurementInfoListTreeWidget(
      DefoMeasurementSelectionModel* selectionModel
    , QWidget *parent = 0
  );

protected:
  static const QString TIME_FORMAT;

  DefoMeasurementSelectionModel* selectionModel_;

protected slots:
  void setSelection(DefoMeasurement* selection);

};

#endif // DEFOMEASUREMENTINFOLISTTREEWIDGET_H
