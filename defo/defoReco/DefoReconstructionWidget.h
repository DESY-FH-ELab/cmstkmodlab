#ifndef DEFORECONSTRUCTIONWIDGET_H
#define DEFORECONSTRUCTIONWIDGET_H

#include <QWidget>

#include "DefoReconstructionModel.h"

class DefoReconstructionWidget : public QWidget
{
    Q_OBJECT
public:
  explicit DefoReconstructionWidget(
      DefoReconstructionModel* reconstructionModel
    , QWidget *parent = 0
  );

protected:

  DefoReconstructionModel* reconstructionModel_;

protected slots:

  void setupChanged();
  void reconstructButtonClicked();
};

#endif // DEFORECONSTRUCTIONWIDGET_H
