/////////////////////////////////////////////////////////////////////////////////
//                                                                             //
//               Copyright (C) 2011-2020 - The DESY CMS Group                  //
//                           All rights reserved                               //
//                                                                             //
//      The CMStkModLab source code is licensed under the GNU GPL v3.0.        //
//      You have the right to modify and/or redistribute this source code      //
//      under the terms specified in the license, which may be found online    //
//      at http://www.gnu.org/licenses or at License.txt.                      //
//                                                                             //
/////////////////////////////////////////////////////////////////////////////////

#ifndef LEYBOLDGRAPHIXONEWIDGET_H
#define LEYBOLDGRAPHIXONEWIDGET_H

#include <QCheckBox>
#include <QDoubleSpinBox>
#include <QFormLayout>
#include <QHBoxLayout>
#include <QLCDNumber>
#include <QSpinBox>
#include <QVBoxLayout>
#include <QWidget>
#include <QGroupBox>
#include <QLabel>

#include "LeyboldGraphixOneModel.h"
#include "DeviceState.h"

class LeyboldGraphixOneWidget : public QWidget
{
    Q_OBJECT
public:
  explicit LeyboldGraphixOneWidget(LeyboldGraphixOneModel* model,
                                   QWidget *parent = 0);

protected:

  LeyboldGraphixOneModel* model_;

  QCheckBox* leyboldCheckBox_;

  QWidget* operationPanel_;

public slots:
  void updateDeviceState( State newState );
  void controlStateChanged(bool);
  void updateInfo();
};

class LeyboldGraphixOneGaugeWidget : public QGroupBox
{
  Q_OBJECT

public:
  explicit LeyboldGraphixOneGaugeWidget(LeyboldGraphixOneModel* model,
                                        unsigned int sensor,
                                        QWidget *parent = 0);

protected:

  static const unsigned int LCD_SIZE;
  static const QString LABEL_FORMAT;

  LeyboldGraphixOneModel* model_;
  unsigned int sensor_;

  QLabel* statusDisplay_;
  QLCDNumber* pressureDisplay_;

public slots:
  void updateDeviceState( State newState );
  void controlStateChanged(bool);
  void updateInfo();
};

#endif // LEYBOLDGRAPHIXONEWIDGET_H
