#ifndef THERMODISPLAYMAINWINDOW_H
#define THERMODISPLAYMAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QVector>
#include <QTabWidget>

#include "ThermoDAQClient.h"
#include "ThermoDAQNetworkReader.h"
#include "ThermoDAQDisplayWidget.h"
#include "ThermoDAQValueVector.h"

class ThermoDisplayMainWindow : public QMainWindow
{
  Q_OBJECT
public:
  explicit ThermoDisplayMainWindow(QWidget *parent = 0);

public slots:

    void requestData();
    void updateInfo();

protected:

    QTimer* timer_;

    QTabWidget* tabWidget_;

    ThermoDAQClient* client_;
    ThermoDAQNetworkReader* reader_;

    ThermoDAQValueVector<double> bathTemperature_;
    ThermoDAQDisplayPlotItem<double>* bathTemperaturePlot_;
    ThermoDAQValueVector<double> workingTemperature_;
    ThermoDAQDisplayPlotItem<double>* workingTemperaturePlot_;
    ThermoDAQValueVector<double> temperature_[10];
    ThermoDAQDisplayPlotItem<double>* temperaturePlot_[10];

    ThermoDAQValueVector<double> pressure1_;
    ThermoDAQDisplayPlotItem<double>* pressure1Plot_;
    ThermoDAQValueVector<double> pressure2_;
    ThermoDAQDisplayPlotItem<double>* pressure2Plot_;

    ThermoDAQTemperatureDisplayWidget* tempDisplay_;
    ThermoDAQPressureDisplayWidget* pDisplay_;
};

#endif // THERMODISPLAYMAINWINDOW_H
