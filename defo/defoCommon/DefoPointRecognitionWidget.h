/////////////////////////////////////////////////////////////////////////////////
//                                                                             //
//               Copyright (C) 2011-2025 - The DESY CMS Group                  //
//                           All rights reserved                               //
//                                                                             //
//      The CMStkModLab source code is licensed under the GNU GPL v3.0.        //
//      You have the right to modify and/or redistribute this source code      //
//      under the terms specified in the license, which may be found online    //
//      at http://www.gnu.org/licenses or at License.txt.                      //
//                                                                             //
/////////////////////////////////////////////////////////////////////////////////

#ifndef DEFOPOINTRECOGNICTIONWIDGET_H
#define DEFOPOINTRECOGNICTIONWIDGET_H

#include <QMutex>

#include <QTabWidget>
#include <QPushButton>
#include <QCheckBox>

#include "DefoMeasurementSelectionModel.h"
#include "DefoPointRecognitionModel.h"
#include "DefoImageWidget.h"

class DefoPointRecognitionWidget : public QTabWidget
{
  Q_OBJECT

public:

  explicit DefoPointRecognitionWidget(DefoMeasurementListModel* listModel,
                                      DefoMeasurementSelectionModel* selectionModel,
                                      DefoPointRecognitionModel* pointModel,
                                      QWidget *parent = 0);

protected:

  // Models
  DefoMeasurementListModel* listModel_;
  DefoMeasurementSelectionModel* selectionModel_;
  DefoPointRecognitionModel* pointModel_;

  // Widgets
  DefoImageThresholdsWidget* thresholdImage_;
  DefoImagePointsWidget * pointsImage_;
  QCheckBox* fitPoints_;
  QPushButton* findPoints_;
  QPushButton* savePoints_;

  QMutex mutex_;

protected slots:

  void findPointsButtonClicked();
  void savePointsButtonClicked();
  void controlStateChanged(bool);
};

#endif // DEFOPOINTRECOGNICTIONWIDGET_H
