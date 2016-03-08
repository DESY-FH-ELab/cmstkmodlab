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

#include "LStepExpressModel.h"
#include "LStepExpressMeasurementTable.h"
#include "LStepExpressMotionManager.h"

class LStepExpressMeasurementWidget : public QWidget
{
    Q_OBJECT

public:
    explicit LStepExpressMeasurementWidget(LStepExpressModel* model, LStepExpressMotionManager* manager, QWidget *parent = 0);

protected:
    LStepExpressModel* model_;
    LStepExpressMotionManager* manager_;
    QCheckBox* averageMeasCheckBox_;
    QPushButton* buttonGeneratePos_;
    QPushButton *buttonStartMeasurement_;
    QPushButton *buttonStoreMeasurement_;
    QLineEdit* nstepsx_;
    QLineEdit* nstepsy_;

public slots:
    void generatePositions();
    void setAverageMeasEnabled(bool);

private:
    bool averageMeasEnabled_;
    int nstepsx;
    int nstepsy;
    double rangex = 100; //FIX ME! take full range of table
    double rangey = 100; //FIX ME! take full range of table
    double z_init;
    double y_init;
    double x_init;
    std::vector<float> circle_x;
    std::vector<float> circle_y;
    void generateCirclePositions();

    LStepExpressMeasurementTable *table_model;
    QTableView *table_view;

private slots:
    void performMeasurement();
    void storeResults();

};

#endif // LSTEPEXPRESSMEASUREMENTWIDGET_H
