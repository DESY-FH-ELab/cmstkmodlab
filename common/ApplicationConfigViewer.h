/////////////////////////////////////////////////////////////////////////////////
//                                                                             //
//               Copyright (C) 2011-2022 - The DESY CMS Group                  //
//                           All rights reserved                               //
//                                                                             //
//      The CMStkModLab source code is licensed under the GNU GPL v3.0.        //
//      You have the right to modify and/or redistribute this source code      //
//      under the terms specified in the license, which may be found online    //
//      at http://www.gnu.org/licenses or at License.txt.                      //
//                                                                             //
/////////////////////////////////////////////////////////////////////////////////

#ifndef APPLICATIONCONFIGVIEWER_H
#define APPLICATIONCONFIGVIEWER_H

#include <QTableView>

#include "ApplicationConfigItemModel.h"

class ApplicationConfigViewer : public QTableView
{
  Q_OBJECT
public:

  ApplicationConfigViewer(bool readOnly, QWidget *parent = 0);

public slots:

  void setReadOnly(bool readOnly);

protected slots:

  void resizeColumns();

protected:

  ApplicationConfigItemModel* model_;
};

#endif // APPLICATIONCONFIGVIEWER_H
