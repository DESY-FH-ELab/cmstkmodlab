/////////////////////////////////////////////////////////////////////////////////
//                                                                             //
//               Copyright (C) 2011-2017 - The DESY CMS Group                  //
//                           All rights reserved                               //
//                                                                             //
//      The CMStkModLab source code is licensed under the GNU GPL v3.0.        //
//      You have the right to modify and/or redistribute this source code      //
//      under the terms specified in the license, which may be found online    //
//      at http://www.gnu.org/licenses or at License.txt.                      //
//                                                                             //
/////////////////////////////////////////////////////////////////////////////////

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
    void exportPlot();
    void clearData();

protected:

    QTimer* timer_;

    QTabWidget* tabWidget_;

    ThermoDAQClient* client_;
    ThermoDAQNetworkReader* reader_;

    ThermoDAQValueVector<double> bathTemperature_;
    ThermoDAQDisplayPlotItem* bathTemperaturePlot_;
    ThermoDAQValueVector<double> workingTemperature_;
    ThermoDAQDisplayPlotItem* workingTemperaturePlot_;
    ThermoDAQValueVector<double> temperature_[10];
    ThermoDAQDisplayPlotItem* temperaturePlot_[10];

    ThermoDAQValueVector<double> pressure1_;
    ThermoDAQDisplayPlotItem* pressure1Plot_;
    ThermoDAQValueVector<double> pressure2_;
    ThermoDAQDisplayPlotItem* pressure2Plot_;

    ThermoDAQValueVector<double> arduinoA_;
    ThermoDAQDisplayPlotItem* arduinoAPlot_;
    ThermoDAQValueVector<double> arduinoB_;
    ThermoDAQDisplayPlotItem* arduinoBPlot_;
    ThermoDAQValueVector<double> iotaActPressure_;
    ThermoDAQDisplayPlotItem* iotaActPressurePlot_;
    ThermoDAQValueVector<double> iotaSetPressure_;
    ThermoDAQDisplayPlotItem* iotaSetPressurePlot_;

    ThermoDAQTemperatureDisplayWidget* tempDisplay_;
    ThermoDAQPressureDisplayWidget* pDisplay_;
    ThermoDAQMicroPressureDisplayWidget* microPressureDisplay_;
};

#endif // THERMODISPLAYMAINWINDOW_H
