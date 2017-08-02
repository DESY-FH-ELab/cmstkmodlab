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

#ifndef ASSEMBLYUEYECAMERATHREAD_H
#define ASSEMBLYUEYECAMERATHREAD_H

#include <QObject>
#include <QThread>

#ifdef NOUEYE
#include "AssemblyUEyeFakeModel.h"
typedef AssemblyUEyeFakeModel AssemblyUEyeModel_t;
#else
#include "AssemblyUEyeModel.h"
typedef AssemblyUEyeModel AssemblyUEyeModel_t;
#endif

class AssemblyUEyeCameraThread : public QThread
{
public:
    explicit AssemblyUEyeCameraThread(AssemblyUEyeModel_t* model,
                                      QObject *parent = 0);

    ~AssemblyUEyeCameraThread();

    void run();

protected:
    AssemblyUEyeModel_t* model_;
};

#endif // ASSEMBLYUEYECAMERATHREAD_H
