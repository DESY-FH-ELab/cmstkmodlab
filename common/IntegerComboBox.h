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

#ifndef INTEGERCOMBOBOX_H
#define INTEGERCOMBOBOX_H

#include <QComboBox>

class IntegerComboBox : public QComboBox
{
    Q_OBJECT
public:
    explicit IntegerComboBox(QWidget* parent=0);

    void addItem(int value);

public slots:

protected:

protected slots:

    void currentItemChanged(int index);

signals:

    void valueChanged(int value);
};

#endif // INTEGERCOMBOBOX_H
