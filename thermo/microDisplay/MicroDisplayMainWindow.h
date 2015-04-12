#ifndef MICRODISPLAYMAINWINDOW_H
#define MICRODISPLAYMAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QVector>
#include <QTabWidget>

#include "MicroDAQClient.h"
#include "MicroDAQNetworkReader.h"
#include "MicroDAQDisplayWidget.h"
#include "MicroDAQValueVector.h"

class MicroDisplayMainWindow : public QMainWindow
{
  Q_OBJECT
public:
  explicit MicroDisplayMainWindow(QWidget *parent = 0);

public slots:

    void requestData();
    void updateInfo();
    void exportPlot();
    void clearData();

protected:

    QTimer* timer_;

    QTabWidget* tabWidget_;

    MicroDAQClient* client_;
    MicroDAQNetworkReader* reader_;

    MicroDAQValueVector<double> arduinoA_;
    MicroDAQDisplayPlotItem* arduinoAPlot_;
    MicroDAQValueVector<double> arduinoB_;
    MicroDAQDisplayPlotItem* arduinoBPlot_;
    MicroDAQValueVector<double> arduinoFlow_;
    MicroDAQDisplayPlotItem* arduinoFlowPlot_;
    MicroDAQValueVector<double> iotaActPressure_;
    MicroDAQDisplayPlotItem* iotaActPressurePlot_;
    MicroDAQValueVector<double> iotaSetPressure_;
    MicroDAQDisplayPlotItem* iotaSetPressurePlot_;
    MicroDAQValueVector<double> coriTemp_;
    MicroDAQDisplayPlotItem* coriTempPlot_;
    MicroDAQValueVector<double> coriPres_;
    MicroDAQDisplayPlotItem* coriPresPlot_;
    MicroDAQValueVector<double> coriMeasure_;
    MicroDAQDisplayPlotItem* coriMeasurePlot_;

    MicroDAQMicroPressureDisplayWidget* microPressureDisplay_;
};

#endif // MICRODISPLAYMAINWINDOW_H
