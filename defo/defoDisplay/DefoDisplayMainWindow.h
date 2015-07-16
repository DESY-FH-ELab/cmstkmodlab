#ifndef DEFODISPLAYMAINWINDOW_H
#define DEFODISPLAYMAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QVector>

#include "DefoDAQClient.h"
#include "DefoDAQNetworkReader.h"
#include "DefoDAQDisplayWidget.h"
#include "DefoDAQValueVector.h"

class DefoDisplayMainWindow : public QMainWindow
{
  Q_OBJECT
public:
  explicit DefoDisplayMainWindow(QWidget *parent = 0);

public slots:

    void requestData();
    void updateInfo();
    void exportPlot();
    void clearData();

protected:

    QTimer* timer_;

    DefoDAQClient* client_;
    DefoDAQNetworkReader* reader_;

    DefoDAQValueVector<double> bathTemperature_;
    DefoDAQDisplayPlotItem* bathTemperaturePlot_;
    DefoDAQValueVector<double> workingTemperature_;
    DefoDAQDisplayPlotItem* workingTemperaturePlot_;
    DefoDAQValueVector<double> temperature_[10];
    DefoDAQDisplayPlotItem* temperaturePlot_[10];

    DefoDAQTemperatureDisplayWidget* tempDisplay_;
};

#endif // DEFODISPLAYMAINWINDOW_H
