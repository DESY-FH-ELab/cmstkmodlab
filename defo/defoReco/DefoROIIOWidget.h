#ifndef DEFOROIIOWIDGET_H
#define DEFOROIIOWIDGET_H

#include <QWidget>
#include <QPushButton>

#include "DefoRecoROIModel.h"
#include "DefoMeasurementSelectionModel.h"
#include "DefoRecoMeasurementListModel.h"

class DefoROIIOWidget : public QWidget
{
  Q_OBJECT
public:
  explicit DefoROIIOWidget(DefoRecoMeasurementListModel *listModel,
                           DefoRecoROIModel *roiModel,
                           QWidget *parent = 0);

protected:

  DefoRecoMeasurementListModel* listModel_;
  DefoMeasurementSelectionModel * copyFromSelectionModel_;
  DefoRecoROIModel *roiModel_;

  QPushButton *exportButton_;
  QPushButton *importButton_;

protected slots:

  void exportButtonClicked();
  void importButtonClicked();
};

#endif // DEFOROIIOWIDGET_H
