/////////////////////////////////////////////////////////////////////////////////
//                                                                             //
//               Copyright (C) 2011-2025 - The DESY CMS Group                  //
//                           All rights reserved                               //
//                                                                             //
//      The CMStkModLab source code is licensed under the GNU GPL v3.0.        //
//      You have the right to modify and/or redistribute this source code      //
//      under the terms specified in the license, which may be found online    //
//      at http://www.gnu.org/licenses or at License.txt.                      //
//                                                                             //
/////////////////////////////////////////////////////////////////////////////////

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

class DefoMeasurementPairListComboBox : public QComboBox
{
    Q_OBJECT
public:
  explicit DefoMeasurementPairListComboBox(
      DefoMeasurementPairListModel* listModel
    , DefoMeasurementPairSelectionModel* selectionModel
    , QWidget *parent = 0
  );

protected:
  static const QString LABEL_FORMAT;
  static const QString TIME_FORMAT;

  DefoMeasurementPairListModel* listModel_;
  DefoMeasurementPairSelectionModel* selectionModel_;

  /**
    Mapping of the measurements in the list to the indices in the combo box.
    Mapping should be one to one, i.e. index in the combobox is the same in the
    list model.
    */
  typedef std::map<const DefoMeasurementPair*, int> MeasurementPairMap;
  MeasurementPairMap indexMap_;

protected slots:
  void fillOptions(int count);
  void setSelection(DefoMeasurementPair* selection);
  void selectionChanged(int index);

signals:

};

#endif // DEFOMEASUREMENTLISTCOMBOBOX_H
