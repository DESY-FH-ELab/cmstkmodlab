#include "DefoMeasurementSelectionModel.h"

DefoMeasurementSelectionModel::DefoMeasurementSelectionModel(QObject *parent) :
    QObject(parent)
  , selection_(NULL)
{}

const DefoMeasurementBase * DefoMeasurementSelectionModel::getSelection() const {
  return selection_;
}

void DefoMeasurementSelectionModel::setSelection(
    const DefoMeasurementBase *selection
) {

  if (selection_ != selection) {
    selection_ = selection;
    emit selectionChanged(selection_);
  }

}
