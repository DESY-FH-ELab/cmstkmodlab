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

#ifndef ASSEMBLYUEYEWIDGET_H
#define ASSEMBLYUEYEWIDGET_H

#include <QTabWidget>

#include <DeviceState.h>

#include "AssemblyVUEyeModel.h"
#include "AssemblyVUEyeCamera.h"

class AssemblyUEyeWidget : public QTabWidget
{
    Q_OBJECT
public:
  explicit AssemblyUEyeWidget(AssemblyVUEyeModel *model,
                              QWidget *parent = 0);

protected:

  AssemblyVUEyeModel* model_;

public slots:

  void cameraCountChanged(unsigned int);
};

#endif // ASSEMBLYUEYEWIDGET_H
