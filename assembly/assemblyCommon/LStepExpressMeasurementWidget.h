#ifndef LSTEPEXPRESSMEASUREMENTWIDGET_H
#define LSTEPEXPRESSMEASUREMENTWIDGET_H

#include <vector>

#include <Qt>
#include <QCheckBox>
#include <QFormLayout>
#include <QGridLayout>
#include <QLCDNumber>
#include <QVBoxLayout>
#include <QComboBox>
#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QLineEdit>
#include <QMessageBox>
#include <QTableWidget>
#include <QStringList>
#include <QTableView>
#include <QModelIndex>
#include <QFileDialog>
#include <QLCDNumber>
#include <QMutex>
#include <QMutexLocker>

#include "LStepExpressModel.h"
#include "LStepExpressMeasurementTable.h"
#include "LStepExpressMotionManager.h"
#include "LaserModel.h"
#include "LaserWidget.h"
#include "LaserThread.h"

class LStepExpressMeasurementWidget : public QWidget
{
    Q_OBJECT

public:
    explicit LStepExpressMeasurementWidget(LStepExpressModel* model, LStepExpressMotionManager* manager, LaserModel* laserModel, QWidget *parent = 0);

protected:
    LStepExpressModel* model_;
    LStepExpressMotionManager* manager_;
    LaserModel* laserModel_;
    LaserWidget* laserWidget_;
    //    LaserThread* laserThread_;
    QCheckBox* averageMeasCheckBox_;
    QPushButton* buttonGeneratePos_;
    QPushButton *buttonStartMeasurement_;
    QPushButton *buttonStopMeasurement_;
    QPushButton *buttonStoreMeasurement_;
    QCheckBox *checkBoxEnableLaser_;
    QCheckBox *zigzagCheckBox_;
    //    QLineEdit* nstepsx_;
    //QLineEdit* nstepsy_;
    QLineEdit* x_min_;
    QLineEdit* x_max_;
    QLineEdit* y_min_;
    QLineEdit* y_max_;
    QLineEdit* x_stepsize_;
    QLineEdit* y_stepsize_;

    QMutex mutex_;

public slots:
    void generatePositions();
    void setAverageMeasEnabled(bool);
    void laserStateChanged(State newState);
    void setZigZag(bool);

private:
    bool averageMeasEnabled_;
    int nstepsx;
    int nstepsy;
    double x_stepsize;
    double y_stepsize;
    double rangex = 300; //full range of table
    double rangey = 300; //full range of table
    double z_init;
    double y_min;
    double x_min;
    double y_max;
    double x_max;
    bool isZigZag_;
    std::vector<float> circle_x;
    std::vector<float> circle_y;
    void generateCirclePositions();

    LStepExpressMeasurementTable *table_model;
    QTableView *table_view;
    bool clearedForMotion_;

private slots:
    void performMeasurement();
    void stopMeasurement();
    void storeResults();

};

#endif // LSTEPEXPRESSMEASUREMENTWIDGET_H
