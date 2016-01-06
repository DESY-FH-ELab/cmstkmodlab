#ifndef LSTEPEXPRESSJOYSTICKWIDGET_H
#define LSTEPEXPRESSJOYSTICKWIDGET_H

#include <vector>

#include <QCheckBox>
#include <QFormLayout>
#include <QGridLayout>
#include <QPushButton>
#include <QVBoxLayout>
#include <QComboBox>
#include <QWidget>
#include <QLabel>

#include "LStepExpressModel.h"

class LStepExpressJoystickWidget : public QWidget
{
    Q_OBJECT

public:
    explicit LStepExpressJoystickWidget(LStepExpressModel* model, QWidget *parent = 0);

protected:
    LStepExpressModel* model_;
    QWidget* axisControlWidget_;

public slots:
    void lstepStateChanged(State newState);
    void controlStateChanged(bool);
};

class LStepExpressJoystickAxisWidget : public QWidget
{
    Q_OBJECT

public:
    explicit LStepExpressJoystickAxisWidget(LStepExpressModel* model_,
                                            unsigned int axis,
                                            QWidget *parent = 0);

protected:
    LStepExpressModel* model_;
    unsigned int axis_;

    QVBoxLayout* layout_;
    QPushButton *upButton_;
    QPushButton *downButton_;

public slots:
    void lStepStateChanged( State state );
    void controlStateChanged(bool);
    void updateWidgets();
    void upButtonClicked();
    void downButtonClicked();
};

#endif // LSTEPEXPRESSJOYSTICKWIDGET_H
