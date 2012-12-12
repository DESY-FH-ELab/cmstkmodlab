#ifndef DEFOMEASUREMENTLISTTREEWIDGET_H
#define DEFOMEASUREMENTLISTTREEWIDGET_H

#include <QTreeWidget>
#include <QTreeWidgetItem>

#include "DefoMeasurementListModel.h"
#include "DefoMeasurementSelectionModel.h"

class DefoMeasurementListTreeWidgetItem : public QTreeWidgetItem
{
public:
  explicit DefoMeasurementListTreeWidgetItem(
      DefoMeasurement* measurement
    , QTreeWidget *parent = 0
  );

  DefoMeasurement* getMeasurement() { return measurement_; }

protected:
  DefoMeasurement* measurement_;
};

class DefoMeasurementListTreeWidget : public QTreeWidget
{
    Q_OBJECT
public:
  explicit DefoMeasurementListTreeWidget(
      DefoMeasurementListModel* listModel
    , DefoMeasurementSelectionModel* selectionModel
    , QWidget *parent = 0
  );

protected:
  static const QString TIME_FORMAT;

  DefoMeasurementListModel* listModel_;
  DefoMeasurementSelectionModel* selectionModel_;

  /**
     Mapping of the measurements in the list to the indices in the combo box.
     Mapping should be one to one, i.e. index in the combobox is the same in the
     list model.
  */
  typedef std::map<const DefoMeasurement*,
                   DefoMeasurementListTreeWidgetItem*> MeasurementMap;
  MeasurementMap indexMap_;

protected slots:
  void fillTree(int count);
  void setSelection(DefoMeasurement* selection);
  void selectionChanged();

signals:

};

#endif // DEFOMEASUREMENTLISTTREEWIDGET_H
