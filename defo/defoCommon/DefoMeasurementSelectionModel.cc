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

#include "DefoMeasurementSelectionModel.h"

DefoMeasurementSelectionModel::DefoMeasurementSelectionModel(QObject *parent)
: QObject(parent),
  selection_(NULL)
{

}

DefoMeasurement * DefoMeasurementSelectionModel::getSelection() const
{
  return selection_;
}

DefoMeasurement * DefoMeasurementSelectionModel::getSelection()
{
  return selection_;
}

void DefoMeasurementSelectionModel::setSelection(DefoMeasurement *selection)
{
  if (selection_ != selection) {
    selection_ = selection;
    emit selectionChanged(selection_);
  }

}

DefoMeasurementPairSelectionModel::DefoMeasurementPairSelectionModel(QObject *parent)
  : QObject(parent),
    selection_(NULL)
{

}

DefoMeasurementPair * DefoMeasurementPairSelectionModel::getSelection() const
{
  return selection_;
}

void DefoMeasurementPairSelectionModel::setSelection(
    DefoMeasurementPair *selection
) {

  if (selection_ != selection) {
    selection_ = selection;
    emit selectionChanged(selection_);
  }

}
