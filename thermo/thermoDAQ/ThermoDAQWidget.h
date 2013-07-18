#ifndef THERMODAQWIDGET_H
#define THERMODAQWIDGET_H

#include <QWidget>
#include <QPushButton>
#include <QCheckBox>
#include <QGridLayout>
#include <QVBoxLayout>
#include <QFileDialog>
#include <QPlainTextEdit>

#include "ThermoDAQModel.h"

class ThermoDAQWidget : public QWidget
{
  Q_OBJECT
public:
  explicit ThermoDAQWidget(ThermoDAQModel* DAQModel, QWidget *parent = 0);

protected:
  // Models
  ThermoDAQModel* daqModel_;

  // Widgets
  QPushButton* startMeasurementButton_;
  QPushButton* stopMeasurementButton_;
  QPushButton* clearHistoryButton_;
  QWidget* buttons_;

  QPlainTextEdit* logEdit_;
  QPushButton* logButton_;

protected slots:
  void daqStateChanged(bool running);
  void logButtonClicked();
};

#endif // THERMODAQWIDGET_H
