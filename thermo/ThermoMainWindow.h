#ifndef THERMOMAINWINDOW_H
#define THERMOMAINWINDOW_H

#include <QMainWindow>
#include <QTabWidget>
#include <QPushButton>
#include <QFileDialog>
#include <QComboBox>
#include <QImage>
#include <QTimer>

#include "JulaboModel.h"
#include "JulaboWidget.h"

#include "HamegModel.h"
#include "HamegWidget.h"

#include "KeithleyModel.h"
#include "KeithleyWidget.h"

#include "PfeifferModel.h"
#include "PfeifferWidget.h"

#include "ThermoScriptModel.h"
#include "ThermoScriptWidget.h"

#include "ThermoDAQModel.h"
#include "ThermoDAQStreamer.h"

class ThermoMainWindow : public QMainWindow
{
  Q_OBJECT
public:
  explicit ThermoMainWindow(QWidget *parent = 0);

public slots:

protected:

  QDir currentDir_;

  QTabWidget* tabWidget_;

  JulaboModel* julaboModel_;
  HamegModel* hamegModel_;
  KeithleyModel* keithleyModel_;
  PfeifferModel* pfeifferModel_;

  ThermoScriptModel* scriptModel_;

  ThermoDAQModel* daqModel_;
  ThermoDAQStreamer* daqStreamer_;
};

#endif // THERMOMAINWINDOW_H
