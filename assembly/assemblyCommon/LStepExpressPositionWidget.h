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

#ifndef LSTEPEXPRESSPOSITIONWIDGET_H
#define LSTEPEXPRESSPOSITIONWIDGET_H

#include <vector>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QWidget>
#include <QFormLayout>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QString>

#include "LStepExpressModel.h"
#include "LStepExpressMotionManager.h"

class LStepExpressPositionAxisWidget : public QWidget
{
    Q_OBJECT

 public:
//explicit LStepExpressPositionAxisWidget(LStepExpressModel* model, unsigned int axis, QWidget * parent = 0);
    explicit LStepExpressPositionAxisWidget(LStepExpressMotionManager* manager, LStepExpressModel* model, unsigned int axis, QWidget * parent = 0);

 protected:
    LStepExpressModel* model_;
    LStepExpressMotionManager* manager_;
    unsigned int axis_;
    QHBoxLayout* layout_;
    QString axisDimensionName_;
    double position_;
    QLineEdit* Edit_;
    QLabel* Unit_;

 public slots:
        void updateWidgets();

 signals:
        void positionChanged(double value, unsigned int axis);
};

class LStepExpressPositionWidget : public QWidget
{
    Q_OBJECT

 public:
//    explicit LStepExpressPositionWidget(LStepExpressModel* model, QWidget *parent = 0);
    explicit LStepExpressPositionWidget(LStepExpressMotionManager* manager, LStepExpressModel* model, QWidget *parent = 0);

 protected:
    LStepExpressModel* model_;
    LStepExpressMotionManager* manager_;
    QVBoxLayout* layout_;
    QPushButton* moveAbsoluteButton_;
    QPushButton* moveRelativeButton_;
    std::vector<double> position_;
    LStepExpressPositionAxisWidget* xpos_;
    LStepExpressPositionAxisWidget* ypos_;
    LStepExpressPositionAxisWidget* zpos_;
    LStepExpressPositionAxisWidget* apos_;

 public slots:
     void lStepStateChanged( State newState);
     void moveAbsoluteButtonClicked();
     void moveRelativeButtonClicked();
     void positionChanged(double value, unsigned int axis);
};

#endif //LSTEPEXPRESSPOSITIONWIDGET_H
