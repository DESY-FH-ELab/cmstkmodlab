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

      bool check_module_name_exists(std::string);
      bool register_module_name(std::string);

      bool MaPSA_to_BP_(std::string, std::string, std::string);
      bool PSs_to_spacers_(std::string, std::string);
      bool PSs_to_MaPSA_(std::string);

  protected:


};

#endif // DATABASEDUMMY_H
