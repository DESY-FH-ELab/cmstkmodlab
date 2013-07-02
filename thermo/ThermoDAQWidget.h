#ifndef THERMODAQWIDGET_H
#define THERMODAQWIDGET_H

#include <QWidget>
#include <QPushButton>
#include <QCheckBox>
#include <QGridLayout>
#include <QVBoxLayout>
#include <QFileDialog>
#include <QPlainTextEdit>

#include <qwt_plot.h>

#include "ThermoDAQModel.h"

class ThermoDAQWidget : public QWidget
{
  Q_OBJECT
public:
  explicit ThermoDAQWidget(ThermoDAQModel* DAQModel, QWidget *parent = 0);

public slots:

protected:
  // Models
  ThermoDAQModel* DAQModel_;

  // Widgets
  QPushButton* openDAQButton_;
  QPushButton* saveDAQButton_;
  QPushButton* saveAsDAQButton_;
  QPushButton* executeDAQButton_;
  QPushButton* abortDAQButton_;
  QWidget* buttons_;

  QwtPlot* temperaturePlot_;
  QwtPlot* pressurePlot_;

protected slots:
  void openDAQButtonClicked();
  void saveDAQButtonClicked();
  void saveAsDAQButtonClicked();
  void executeDAQButtonClicked();
  void abortDAQButtonClicked();
};

#endif // THERMODAQWIDGET_H
