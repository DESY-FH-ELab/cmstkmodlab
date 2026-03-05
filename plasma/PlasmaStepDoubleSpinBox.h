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

#ifndef PLASMASTEPDOUBLESPINBOX_H
#define PLASMASTEPDOUBLESPINBOX_H

#include <QDoubleSpinBox>

class PlasmaStepDoubleSpinBox : public QDoubleSpinBox
{
  Q_OBJECT
public:

  explicit PlasmaStepDoubleSpinBox(QWidget *parent = 0);

  QString textFromValue(double value) const;
  double valueFromText(const QString & text) const;
};

#endif // PLASMASTEPDOUBLESPINBOX_H
