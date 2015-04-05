#ifndef THERMOMAINWINDOW_H
#define THERMOMAINWINDOW_H

#include <QMainWindow>
#include <QTabWidget>
#include <QPushButton>
#include <QFileDialog>
#include <QComboBox>
#include <QImage>
#include <QTimer>

#include "HuberPetiteFleurModel.h"
#include "HuberPetiteFleurWidget.h"

#include "HamegModel.h"
#include "HamegWidget.h"

#include "KeithleyModel.h"
#include "KeithleyWidget.h"

#include "PfeifferModel.h"
#include "PfeifferWidget.h"
#include "ThermoPfeifferWidget.h"

#include "IotaModel.h"
#include "IotaWidget.h"

#include "ArduinoPresModel.h"
#include "ArduinoPresWidget.h"

#include "ThermoScriptModel.h"
#include "ThermoScriptWidget.h"

#include "ThermoDAQModel.h"
#include "ThermoDAQThread.h"
#include "ThermoDAQStreamer.h"
#include "ThermoDAQWebInfo.h"
#include "ThermoDAQServer.h"

class ThermoMainWindow : public QMainWindow
{
  Q_OBJECT
public:
  explicit ThermoMainWindow(QWidget *parent = 0);

public slots:

  void quit();

protected:

  QDir currentDir_;

  QTabWidget* tabWidget_;

  HuberPetiteFleurModel* huberModel_;
  HamegModel* hamegModel_;
  KeithleyModel* keithleyModel_;
  PfeifferModel* pfeifferModel_;
  IotaModel* iotaModel_;
  ArduinoPresModel* arduinoPresModel_;

  ThermoScriptModel* scriptModel_;

  ThermoDAQModel* daqModel_;
  ThermoDAQThread* daqThread_;
  ThermoDAQStreamer* daqStreamer_;
  ThermoDAQWebInfo* daqWebInfo_;
  ThermoDAQServer* daqServer_;
};

#endif // THERMOMAINWINDOW_H
