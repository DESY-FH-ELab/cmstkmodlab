#ifndef DEFORECONSTRUCTIONWIDGET_H
#define DEFORECONSTRUCTIONWIDGET_H

#include <QWidget>
#include <QProgressBar>

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
  QProgressBar *progressBar_;

public slots:

  void setupChanged();
  void reconstructButtonClicked();
  void recoProgressChanged(int);
  void incrementProgress();
};

#endif // DEFORECONSTRUCTIONWIDGET_H
