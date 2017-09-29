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

#ifndef DEFOLENSCOMBOBOX_H
#define DEFOLENSCOMBOBOX_H

#include <QComboBox>

#include "DefoLensModel.h"

class DefoLensComboBox : public QComboBox
{
    Q_OBJECT
public:
    explicit DefoLensComboBox(DefoLensModel* model,
			      QWidget *parent = 0);

protected:
    
  DefoLensModel * model_;
  
protected slots:
  
  void lensChanged();

signals:

};

#endif // DEFOLENSCOMBOBOX_H
