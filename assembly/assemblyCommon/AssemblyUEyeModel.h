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

#ifndef ASSEMBLYUEYEMODEL_H
#define ASSEMBLYUEYEMODEL_H

#include <ueye.h>

#include <QObject>
#include <QTimer>
#include <QThread>
#include <QMutex>
#include <QMutexLocker>

#include "AssemblyVUEyeModel.h"

class AssemblyUEyeModel : public AssemblyVUEyeModel
{
 Q_OBJECT

  public:

    explicit AssemblyUEyeModel(int updateInterval=60, QObject* parent=nullptr);
    ~AssemblyUEyeModel();

  public slots:

    void updateInformation();

  protected slots:

  protected:

  private:

    UEYE_CAMERA_LIST* uEyeCameraList_;
};

#endif // ASSEMBLYUEYEMODEL_H
