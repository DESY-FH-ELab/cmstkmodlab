#ifndef LSTEPEXPRESSMEASUREMENT_H
#define LSTEPEXPRESSMEASUREMENT_H

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

class LStepExpressMeasurement : public QObject
{
    Q_OBJECT

public:
    explicit LStepExpressMeasurement(LStepExpressModel* model, LStepExpressMotionManager* manager, LaserModel* laserModel, LStepExpressMeasurementTable* table, QObject *parent = 0);
    ~LStepExpressMeasurement();
    void setInit(double x_min_, double x_max_, double y_min_, double y_max_, double x_stepsize_, double y_stepsize_);

protected:
    LStepExpressModel* model_;
    LStepExpressMotionManager* manager_;
    LaserModel* laserModel_;
    LStepExpressMeasurementTable* table_;

    QMutex mutex_;


public slots:
    void generatePositions();
    void setAverageMeasEnabled(bool);
    void setZigZag(bool);
    void takeMeasurement();
    void setLaserEnabled(State newState);

private:
    bool averageMeasEnabled_;
    int nstepsx;
    int nstepsy;
    double x_stepsize;
    double y_stepsize;
    //    double rangex = 300; //full range of table
    //double rangey = 300; //full range of table
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
    bool clearedForMotion_;
    int currentIndex_;
    int tableSize_;
    bool isLaserEnabled_;
    bool measurementInProgress_;

private slots:
    void performScan();
    void stopMeasurement();
    void doNextScanStep();

 signals:
    void nextScanStep();
    void FakeMotionFinished();
    void informationChanged();
};

#endif // LSTEPEXPRESSMEASUREMENT_H
