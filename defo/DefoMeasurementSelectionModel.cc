#include "DefoMeasurementSelectionModel.h"

DefoMeasurementSelectionModel::DefoMeasurementSelectionModel(QObject *parent) :
    QObject(parent)
  , selection_(NULL)
{}

const DefoMeasurement * DefoMeasurementSelectionModel::getSelection() const {
  return selection_;
}

void DefoMeasurementSelectionModel::setSelection(
    const DefoMeasurement *selection
) {

  if (selection_ != selection) {
    selection_ = selection;
    emit selectionChanged(selection_);
  }

}
