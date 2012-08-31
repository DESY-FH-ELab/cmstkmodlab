#include "DefoMeasurementListComboBox.h"

DefoMeasurementListComboBox::DefoMeasurementListComboBox(
     DefoMeasurementListModel* listModel
   , DefoMeasurementSelectionModel* selectionModel
   , QWidget *parent
) :
    QComboBox(parent)
  , listModel_(listModel)
  , selectionModel_(selectionModel)
{

  // No user editable text
  setEditable(false);

  // Copy values
  connect(
        listModel_
      , SIGNAL(measurementCountChanged(int))
      , this
      , SLOT(fillOptions(int))
  );

  // Respond to selection changes
  connect(
        selectionModel_
      , SIGNAL(selectionChanged(const DefoMeasurement*))
      , this
      , SLOT(setSelection(const DefoMeasurement*))
  );
  connect(
        this
      , SIGNAL(currentIndexChanged(int))
      , this
      , SLOT(selectionChanged(int))
  );

  // Fill box and set selection
  fillOptions(listModel_->getMeasurementCount());
//  setSelection(selectionModel_->getSelection());

}

const QString DefoMeasurementListComboBox::LABEL_FORMAT = "%1. %2";
const QString DefoMeasurementListComboBox::TIME_FORMAT = "yyyy-MM-dd hh:mm:ss";

void DefoMeasurementListComboBox::fillOptions(int count) {

  setEnabled(count > 0);

  // FIXME this sets the selection to NULL
  clear();

  // Empty map
  indexMap_.clear();

  const DefoMeasurement* measurement;

  // Fill it up and add to view
  for (int i = 0; i < count; ++i) {
    measurement = listModel_->getMeasurement(i);
    indexMap_[measurement] = i;
    addItem(
        LABEL_FORMAT
          .arg(i+1)
          .arg(measurement->getTimeStamp().toString(TIME_FORMAT))
    );
  }

  setSelection(selectionModel_->getSelection());

}

void DefoMeasurementListComboBox::setSelection(
    const DefoMeasurement *selection
) {

  MeasurementMap::const_iterator it = indexMap_.find(selection);

  // assure it was actually found
  if (it != indexMap_.end())
    setCurrentIndex( it->second );
  else
    setCurrentIndex( -1 );

}

void DefoMeasurementListComboBox::selectionChanged(int index) {

  // Set NULL on no selection, or measurement on valid selection
  if ( index == -1 )
    selectionModel_->setSelection(NULL);
  else
    selectionModel_->setSelection( listModel_->getMeasurement(index) );

}
