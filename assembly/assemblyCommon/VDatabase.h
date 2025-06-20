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

      virtual bool register_module_name(QString, QString) = 0;
      virtual bool MaPSA_to_BP(QString, QString, QString, QString) = 0;
      virtual bool PSs_to_spacers(QString, QString, QString) = 0;
      virtual bool PSs_to_MaPSA(QString, QString) = 0;

  protected:

      QString module_name_;
      int module_dbid_;

      QString MaPSA_name_;
      int MaPSA_dbid_;

      QString PSs_name_;
      int PSs_dbid_;

      QString BP_name_;
      int BP_dbid_;

      int Glue1_dbid_;
      int Glue2_dbid_;
      int Glue3_dbid_;

};

#endif // VDATABASE_H
