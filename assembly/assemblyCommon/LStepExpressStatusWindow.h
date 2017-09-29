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

#ifndef LSTEPEXPRESSSTATUSWINDOW_H
#define LSTEPEXPRESSSTATUSWINDOW_H

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
#include <QPlainTextEdit>
#include <QScrollBar>

#include "LStepExpressModel.h"

class LStepExpressStatusWindow : public QPlainTextEdit
{
    Q_OBJECT

public:
    explicit LStepExpressStatusWindow(LStepExpressModel* model, QWidget *parent = 0);
    ~LStepExpressStatusWindow();

protected:
    LStepExpressModel* model_;

public slots:
    void updateStatus();
    void startTimer(State);

 protected:
    QTimer* timer_;

};

#endif // LSTEPEXPRESSWIDGET_H
