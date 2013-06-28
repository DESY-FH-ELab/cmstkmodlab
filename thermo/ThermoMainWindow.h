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

#include "ThermoScriptModel.h"
// #include "ThermoScriptWidget.h"

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

  ThermoScriptModel* scriptModel_;
};

#endif // THERMOMAINWINDOW_H
