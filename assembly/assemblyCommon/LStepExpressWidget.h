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
#include <QPushButton>
#include <QTimer>

#include "LStepExpressModel.h"

class LStepExpressWidget : public QWidget
{
    Q_OBJECT

public:
    explicit LStepExpressWidget(LStepExpressModel* model, QWidget *parent = 0);
    ~LStepExpressWidget();

protected:
    LStepExpressModel* model_;
    QCheckBox* lstepCheckBox_;
    QCheckBox* joystickCheckBox_;
    QPushButton* buttonOrigin_;
    QWidget* axisControlWidget_;
    QPushButton* buttonCalibrate_;
    QPushButton* buttonEmergencyStop_;

public slots:
    void lstepStateChanged(State newState);
    void controlStateChanged(bool);
    void updateWidgets();
    void motionStarted();
    void motionFinished();
};

class LStepExpressAxisWidget : public QWidget
{
    Q_OBJECT

public:
    explicit LStepExpressAxisWidget(LStepExpressModel* model_,
                                    unsigned int axis,
                                    QWidget *parent = 0);

    ~LStepExpressAxisWidget();

protected:
    LStepExpressModel* model_;
    unsigned int axis_;

    QFormLayout* layout_;
    QCheckBox* enabledCheckBox_;
    QCheckBox* joystickCheckBox_;
    QLabel* statusLabel_;
    QLabel* positionLabel_;
    QString axisDimensionName_;

public slots:
    void lStepStateChanged( State state );
    void controlStateChanged(bool);
    void updateWidgets();
    void updateMotionWidgets();
    void enabledCheckBoxToggled(bool enabled);
    void joystickCheckBoxToggled(bool enabled);
    void motionStarted();
    void motionFinished();
};

#endif // LSTEPEXPRESSWIDGET_H
