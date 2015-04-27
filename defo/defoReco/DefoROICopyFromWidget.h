#ifndef DEFOROICOPYFROMWIDGET_H
#define DEFOROICOPYFROMWIDGET_H

#include <QWidget>
#include <QPushButton>

#include "DefoRecoROIModel.h"
#include "DefoMeasurementSelectionModel.h"
#include "DefoRecoMeasurementListModel.h"

class DefoROICopyFromWidget : public QWidget
{
    Q_OBJECT
public:
  explicit DefoROICopyFromWidget(DefoRecoMeasurementListModel *listModel,
                                 DefoRecoROIModel *roiModel,
                                 QWidget *parent = 0);

protected:

  DefoRecoMeasurementListModel* listModel_;
  DefoMeasurementSelectionModel * copyFromSelectionModel_;
  DefoRecoROIModel *roiModel_;
  QPushButton *copyFromButton_;

protected slots:

  void copyFromButtonClicked();
};

#endif // DEFOROICOPYFROMWIDGET_H
