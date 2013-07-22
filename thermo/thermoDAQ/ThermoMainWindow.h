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

#include "ThermoScriptModel.h"
#include "ThermoScriptWidget.h"

#include "ThermoDAQModel.h"
#include "ThermoDAQStreamer.h"
#include "ThermoDAQServer.h"

class ThermoMainWindow : public QMainWindow
{
  Q_OBJECT
public:
  explicit ThermoMainWindow(QWidget *parent = 0);

public slots:

protected:

  QDir currentDir_;

  QTabWidget* tabWidget_;

  HuberPetiteFleurModel* huberModel_;
  HamegModel* hamegModel_;
  KeithleyModel* keithleyModel_;
  PfeifferModel* pfeifferModel_;

  ThermoScriptModel* scriptModel_;

  ThermoDAQModel* daqModel_;
  ThermoDAQStreamer* daqStreamer_;
  ThermoDAQServer* daqServer_;
};

#endif // THERMOMAINWINDOW_H
