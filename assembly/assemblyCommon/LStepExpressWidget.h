#ifndef LSTEPEXPRESSWIDGET_H
#define LSTEPEXPRESSWIDGET_H

#include <vector>

#include <QCheckBox>
#include <QFormLayout>
#include <QGridLayout>
#include <QLCDNumber>
#include <QVBoxLayout>
#include <QComboBox>
#include <QWidget>
#include <QLabel>

#include "LStepExpressModel.h"

class LStepExpressWidget : public QWidget
{
    Q_OBJECT

public:
    explicit LStepExpressWidget(LStepExpressModel* model, QWidget *parent = 0);

protected:
    LStepExpressModel* model_;
    QCheckBox* lstepCheckBox_;
    QWidget* axisControlWidget_;

public slots:
    void lstepStateChanged(State newState);
    void controlStateChanged(bool);
};

class LStepExpressAxisWidget : public QWidget
{
    Q_OBJECT

public:
    explicit LStepExpressAxisWidget(LStepExpressModel* model_,
                                    unsigned int axis,
                                    QWidget *parent = 0);

protected:
    LStepExpressModel* model_;
    unsigned int axis_;

    QFormLayout* layout_;
    QCheckBox* enabledCheckBox_;
    QLabel* positionLabel_;

public slots:
    void lStepStateChanged( State state );
    void controlStateChanged(bool);
    void updateWidgets();
    void updateMotionWidgets();
    void enabledCheckBoxToggled(bool enabled);
};

#endif // LSTEPEXPRESSWIDGET_H
