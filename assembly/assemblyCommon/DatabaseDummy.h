/////////////////////////////////////////////////////////////////////////////////
//                                                                             //
//               Copyright (C) 2011-2021 - The DESY CMS Group                  //
//                           All rights reserved                               //
//                                                                             //
//      The CMStkModLab source code is licensed under the GNU GPL v3.0.        //
//      You have the right to modify and/or redistribute this source code      //
//      under the terms specified in the license, which may be found online    //
//      at http://www.gnu.org/licenses or at License.txt.                      //
//                                                                             //
/////////////////////////////////////////////////////////////////////////////////

#ifndef DATABASEDUMMY_H
#define DATABASEDUMMY_H

#include <VDatabase.h>

class DatabaseDummy : public VDatabase
{
 Q_OBJECT

  public:
      explicit DatabaseDummy(QObject* parent);
      ~DatabaseDummy();

      bool register_module_name(QString, QString);
      bool MaPSA_to_BP(QString, QString, QString);
      bool PSs_to_spacers(QString, QString);
      bool PSs_to_MaPSA(QString);

  protected:


};

#endif // DATABASEDUMMY_H
