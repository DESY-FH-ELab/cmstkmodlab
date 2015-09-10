#ifndef DEFOROICOPYTOWIDGET_H
#define DEFOROICOPYTOWIDGET_H

#include <QWidget>
#include <QPushButton>

#include "DefoRecoROIModel.h"
#include "DefoMeasurementSelectionModel.h"
#include "DefoRecoMeasurementListModel.h"

class DefoROICopyToWidget : public QWidget
{
  Q_OBJECT
public:
  explicit DefoROICopyToWidget(DefoRecoMeasurementListModel *listModel,
                               DefoRecoROIModel *roiModel,
                               QWidget *parent = 0);

protected:

  DefoRecoMeasurementListModel* listModel_;
  DefoMeasurementSelectionModel * copyToSelectionModel_;
  DefoRecoROIModel *roiModel_;
  QPushButton *copyToButton_;

  protected slots:

  void copyToButtonClicked();
};

#endif // DEFOROICOPYTOWIDGET_H
