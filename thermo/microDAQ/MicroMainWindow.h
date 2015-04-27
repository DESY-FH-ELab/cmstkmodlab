#ifndef MICROMAINWINDOW_H
#define MICROMAINWINDOW_H

#include <QMainWindow>
#include <QTabWidget>
#include <QPushButton>
#include <QFileDialog>
#include <QComboBox>
#include <QImage>
#include <QTimer>

#include "IotaModel.h"
#include "IotaWidget.h"

#include "ArduinoPresModel.h"
#include "ArduinoPresWidget.h"

#include "CoriFlowModel.h"
#include "CoriFlowWidget.h"

#include "MicroScriptModel.h"
#include "MicroScriptWidget.h"

#include "MicroDAQModel.h"
#include "MicroDAQThread.h"
#include "MicroDAQStreamer.h"
#include "MicroDAQServer.h"

class MicroMainWindow : public QMainWindow
{
  Q_OBJECT
public:
  explicit MicroMainWindow(QWidget *parent = 0);

public slots:

  void quit();

protected:

  QDir currentDir_;

  QTabWidget* tabWidget_;

  IotaModel* iotaModel_;
  ArduinoPresModel* arduinoPresModel_;
  CoriFlowModel* coriModel_;

  MicroScriptModel* scriptModel_;

  MicroDAQModel* daqModel_;
  MicroDAQThread* daqThread_;
  MicroDAQStreamer* daqStreamer_;
  MicroDAQServer* daqServer_;
};

#endif // MICROMAINWINDOW_H
