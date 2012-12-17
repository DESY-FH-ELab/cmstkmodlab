#ifndef DEFOANALYSISWIDGET_H
#define DEFOANALYSISWIDGET_H

#include <QWidget>

#include "DefoMeasurementListModel.h"
#include "DefoMeasurementSelectionModel.h"
#include "DefoSurfacePlot.h"

class DefoAnalysisWidget : public QWidget
{
    Q_OBJECT
public:
  explicit DefoAnalysisWidget(
      DefoMeasurementPairListModel * listModel
    , DefoMeasurementPairSelectionModel * selectionModel
    , QWidget *parent = 0
  );

protected:

  DefoMeasurementPairListModel* listModel_;
  DefoMeasurementPairSelectionModel* selectionModel_;

  DefoSurfacePlot* plot3D_;

protected slots:

};

#endif // DEFOANALYSISWIDGET_H
