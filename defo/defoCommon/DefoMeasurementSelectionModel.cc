#include "DefoMeasurementSelectionModel.h"

DefoMeasurementSelectionModel::DefoMeasurementSelectionModel(QObject *parent) :
    QObject(parent)
  , selection_(NULL)
{

}

DefoMeasurement * DefoMeasurementSelectionModel::getSelection() const
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
