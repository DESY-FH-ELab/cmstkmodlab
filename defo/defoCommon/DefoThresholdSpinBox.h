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

#ifndef DEFOTHRESHOLDSPINBOX_H
#define DEFOTHRESHOLDSPINBOX_H

#include <QSpinBox>
#include "DefoPointRecognitionModel.h"

class DefoThresholdSpinBox : public QSpinBox
{
    Q_OBJECT
public:
  explicit DefoThresholdSpinBox(
      DefoPointRecognitionModel* model
    , const DefoPointRecognitionModel::Threshold& threshold
    , QWidget *parent = 0
  );

protected:
  DefoPointRecognitionModel* model_;
  const DefoPointRecognitionModel::Threshold threshold_;

protected slots:
  void spinnerValueChanged(int value);
  void modelValueChanged(
      DefoPointRecognitionModel::Threshold threshold
    , int value
  );
  void dynamicMinimumChanged(
      DefoPointRecognitionModel::Threshold threshold
    , int minimum
  );
  void controlStateChanged(bool);

};

#endif // DEFOTHRESHOLDSPINBOX_H
