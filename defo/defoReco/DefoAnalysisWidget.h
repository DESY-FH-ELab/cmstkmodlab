#ifndef DEFOANALYSISWIDGET_H
#define DEFOANALYSISWIDGET_H

#include <map>

#include <QWidget>
#include <QLabel>

#include "DefoMeasurementListModel.h"
#include "DefoMeasurementSelectionModel.h"
#include "DefoSurfacePlot.h"
#include "DefoSurfacePlotViewSettings.h"

class DefoAnalysisWidget : public QWidget
{
    Q_OBJECT
public:
  explicit DefoAnalysisWidget(DefoMeasurementPairListModel * listModel,
                              DefoMeasurementPairSelectionModel * selectionModel,
                              QWidget *parent = 0);

protected:

  DefoMeasurementPairListModel* listModel_;
  DefoMeasurementPairSelectionModel* selectionModel_;

  DefoSurfacePlot* plot3D_;
  std::map<DefoMeasurementPair*,DefoSurfacePlotViewSettings*> viewSettings_;

  QLabel* minZLabel_;
  QLabel* posXAtMinZLabel_;
  QLabel* posYAtMinZLabel_;
  QLabel* maxZLabel_;
  QLabel* posXAtMaxZLabel_;
  QLabel* posYAtMaxZLabel_;

protected slots:

  void selectionChanged(DefoMeasurementPair* selection);
};

#endif // DEFOANALYSISWIDGET_H
