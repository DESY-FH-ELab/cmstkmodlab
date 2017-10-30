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

#ifndef ASSEMBLYUEYEFAKEMODEL_H
#define ASSEMBLYUEYEFAKEMODEL_H

#include "AssemblyVUEyeModel.h"

class AssemblyUEyeFakeModel : public AssemblyVUEyeModel
{
 Q_OBJECT
  public:
    explicit AssemblyUEyeFakeModel(int updateInterval=60, QObject* parent=0);
    ~AssemblyUEyeFakeModel();

  public slots:
    void updateInformation();

  protected slots:

  protected:
    bool infoUpdated_;
};

#endif // ASSEMBLYUEYEFAKEMODEL_H
