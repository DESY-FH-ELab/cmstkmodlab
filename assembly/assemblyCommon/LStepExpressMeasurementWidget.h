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
#include <QSignalSpy>

#include "LStepExpressModel.h"
#include "LStepExpressMeasurementTable.h"
#include "LStepExpressMotionManager.h"
#include "LStepExpressMeasurement.h"
#include "LaserModel.h"
#include "LaserWidget.h"

class LStepExpressMeasurementWidget : public QWidget
{
    Q_OBJECT

public:
    explicit LStepExpressMeasurementWidget(LStepExpressModel* model, LaserModel* laserModel, LStepExpressMeasurement* measurement, LStepExpressMeasurementTable* table, QWidget *parent = 0);
    ~LStepExpressMeasurementWidget();

protected:
    LStepExpressModel* model_;
    LaserModel* laserModel_;
    LStepExpressMeasurement *measurement_model_;
    LStepExpressMeasurementTable *table_;
    LaserWidget* laserWidget_;
    QCheckBox* averageMeasCheckBox_;
    QPushButton* buttonGeneratePos_;
    QPushButton *buttonStartMeasurement_;
    QPushButton *buttonStopMeasurement_;
    QPushButton *buttonStoreMeasurement_;
    QCheckBox *checkBoxEnableLaser_;
    QCheckBox *zigzagCheckBox_;
    QLineEdit* x_min_;
    QLineEdit* x_max_;
    QLineEdit* y_min_;
    QLineEdit* y_max_;
    QLineEdit* x_stepsize_;
    QLineEdit* y_stepsize_;

    QMutex mutex_;

    QSignalSpy* spyAverageMeasCheckBox_;
    QSignalSpy* spyButtonGeneratePos_;
    QSignalSpy* spyButtonStartMeasurement_;
    QSignalSpy* spyButtonStopMeasurement_;
    QSignalSpy* spyButtonStoreMeasurement_;
    QSignalSpy* spyCheckBoxEnableLaser_;
    QSignalSpy* spyZigzagCheckBox_;

    void FakeMotion();

public slots:
    void laserStateChanged(State newState);
    void lstepStateChanged(State newState);
    void setInit();

private:
    QTableView *table_view;

private slots:
    void storeResults();
    void printSpyInformation();
    void updateWidget();

 signals:
};

#endif // LSTEPEXPRESSMEASUREMENTWIDGET_H
