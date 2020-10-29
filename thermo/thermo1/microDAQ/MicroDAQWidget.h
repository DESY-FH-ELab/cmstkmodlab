#ifndef MicroDAQWIDGET_H
#define MicroDAQWIDGET_H

#include <QWidget>
#include <QPushButton>
#include <QCheckBox>
#include <QGridLayout>
#include <QVBoxLayout>
#include <QFileDialog>
#include <QPlainTextEdit>

#include "MicroDAQModel.h"

class MicroDAQWidget : public QWidget
{
  Q_OBJECT
public:
  explicit MicroDAQWidget(MicroDAQModel* DAQModel, QWidget *parent = 0);

protected:
  // Models
  MicroDAQModel* daqModel_;

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

#endif // MicroDAQWIDGET_H
