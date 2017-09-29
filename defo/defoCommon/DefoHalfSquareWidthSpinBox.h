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

#ifndef DEFOHALFSQUAREWIDTHSPINBOX_H
#define DEFOHALFSQUAREWIDTHSPINBOX_H

#include <QSpinBox>
#include "DefoPointRecognitionModel.h"

class DefoHalfSquareWidthSpinBox : public QSpinBox
{
    Q_OBJECT
public:
  explicit DefoHalfSquareWidthSpinBox(
      DefoPointRecognitionModel* model
    , QWidget *parent = 0
  );

protected:
  DefoPointRecognitionModel* model_;

protected slots:
  void spinnerValueChanged(int value);
  void modelValueChanged(int value);
  void controlStateChanged(bool);

};

#endif // DEFOHALFSQUAREWIDTHSPINBOX_H
