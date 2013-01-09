#include "DefoMeasurementCommentTextView.h"

DefoMeasurementCommentTextView::DefoMeasurementCommentTextView(
     DefoMeasurementSelectionModel* selectionModel
   , QWidget *parent
) :
    QPlainTextEdit(parent)
  , selectionModel_(selectionModel)
{
  setMinimumSize(QSize(300, 100));
  setReadOnly(true);

  // Respond to selection changes
  connect(
  	  selectionModel_
        , SIGNAL(selectionChanged(DefoMeasurement*))
        , this
        , SLOT(setSelection(DefoMeasurement*))
  	  );
}

void DefoMeasurementCommentTextView::setSelection(
    DefoMeasurement *measurement
) {

  if (!measurement) return;

  clear();
  setPlainText(measurement->getComment());
}
