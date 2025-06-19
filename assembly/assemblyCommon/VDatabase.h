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

#ifndef VDATABASE_H
#define VDATABASE_H

#include <unistd.h>
#include <QObject>

class VDatabase : public QObject
{
 Q_OBJECT

  public:
      VDatabase(QObject* parent);
      ~VDatabase();

      virtual bool check_module_name_exists(std::string) = 0;
      virtual bool register_module_name(std::string) = 0;

      virtual bool MaPSA_to_BP_(std::string, std::string, std::string) = 0;
      virtual bool PSs_to_spacers_(std::string, std::string) = 0;
      virtual bool PSs_to_MaPSA_(std::string) = 0;

  protected:

      std::string MaPSA_name_;
      unsigned int MaPSA_dbid_;

      std::string PSs_name_;
      unsigned int PSs_dbid_;

      std::string BP_name_;
      unsigned int BP_dbid_;

      unsigned int Glue1_dbid_;
      unsigned int Glue2_dbid_;
      unsigned int Glue3_dbid_;

};

#endif // VDATABASE_H
