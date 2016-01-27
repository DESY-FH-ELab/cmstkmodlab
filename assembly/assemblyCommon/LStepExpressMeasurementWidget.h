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

#include "LStepExpressModel.h"
#include "LStepExpressMeasurementTable.h"

class LStepExpressMeasurementWidget : public QWidget
{
    Q_OBJECT

public:
    explicit LStepExpressMeasurementWidget(LStepExpressModel* model, QWidget *parent = 0);

protected:
    LStepExpressModel* model_;
    QCheckBox* averageMeasCheckBox_;
    QPushButton* buttonGeneratePos_;
    QPushButton *buttonStartMeasurement_;
    QLineEdit* nstepsx_;
    QLineEdit* nstepsy_;

public slots:
    void generatePositions();
    void setAverageMeasEnabled(bool);

private:
    bool averageMeasEnabled_;
    int nstepsx;
    int nstepsy;
    double rangex = 100;
    double rangey = 100;
    double z_init;
    double y_init;
    double x_init;
    std::vector<double> circle_x;
    std::vector<double> circle_y;
    void generateCirclePositions();
    void performMeasurement(std::vector<double> x, std::vector<double> y);

    LStepExpressMeasurementTable *table_model;
    QTableView *table_view;
};

#endif // LSTEPEXPRESSMEASUREMENTWIDGET_H
