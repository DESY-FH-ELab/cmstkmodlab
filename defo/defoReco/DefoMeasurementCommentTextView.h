#ifndef DEFOMEASUREMENTCOMMENTTEXTVIEW_H
#define DEFOMEASUREMENTCOMMENTTEXTVIEW_H

#include <QPlainTextEdit>

#include "DefoMeasurementSelectionModel.h"

class DefoMeasurementCommentTextView : public QPlainTextEdit
{
    Q_OBJECT
public:
  explicit DefoMeasurementCommentTextView(
      DefoMeasurementSelectionModel* selectionModel
    , QWidget *parent = 0
  );

protected:

  DefoMeasurementSelectionModel* selectionModel_;

protected slots:
  void setSelection(DefoMeasurement* selection);

};

#endif // DEFOMEASUREMENTCOMMENTTEXTVIEW_H
