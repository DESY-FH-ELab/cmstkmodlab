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

#include <nqlogger.h>

#include "IntegerComboBox.h"

IntegerComboBox::IntegerComboBox(QWidget* parent)
    : QComboBox(parent)
{

}

void IntegerComboBox::addItem(int value)
{
    QComboBox::addItem(QString::number(value), QVariant(value));
}

void IntegerComboBox::currentItemChanged(int index)
{
    QVariant data = itemData(index);
    emit valueChanged(data.toInt());
}
