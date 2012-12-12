#ifndef DEFOMEASUREMENTLISTCOMBOBOX_H
#define DEFOMEASUREMENTLISTCOMBOBOX_H

#include <QComboBox>
#include "DefoMeasurementListModel.h"
#include "DefoMeasurementSelectionModel.h"

class DefoMeasurementListComboBox : public QComboBox
{
    Q_OBJECT
public:
  explicit DefoMeasurementListComboBox(
      DefoMeasurementListModel* listModel
    , DefoMeasurementSelectionModel* selectionModel
    , QWidget *parent = 0
  );

protected:
  static const QString LABEL_FORMAT;
  static const QString TIME_FORMAT;

  DefoMeasurementListModel* listModel_;
  DefoMeasurementSelectionModel* selectionModel_;

  /**
    Mapping of the measurements in the list to the indices in the combo box.
    Mapping should be one to one, i.e. index in the combobox is the same in the
    list model.
    */
  typedef std::map<const DefoMeasurement*, int> MeasurementMap;
  MeasurementMap indexMap_;

protected slots:
  void fillOptions(int count);
  void setSelection(DefoMeasurement* selection);
  void selectionChanged(int index);

signals:

};

#endif // DEFOMEASUREMENTLISTCOMBOBOX_H
