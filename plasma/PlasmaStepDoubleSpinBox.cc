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

#include <cmath>

#include <nqlogger.h>

#include "PlasmaStepDoubleSpinBox.h"

PlasmaStepDoubleSpinBox::PlasmaStepDoubleSpinBox(QWidget *parent)
  : QDoubleSpinBox(parent)
{

}

QString PlasmaStepDoubleSpinBox::textFromValue(double value) const
{
  double steps = value / singleStep();

  QString text = QWidget::locale().toString(std::round(steps)*singleStep(), 'f', decimals());

  return text;
}

double PlasmaStepDoubleSpinBox::valueFromText(const QString & text) const
{
  double value = QDoubleSpinBox::valueFromText(text);

  value = std::round(value / singleStep())*singleStep();

  return value;
}
