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

#ifndef DATABASEBROWN_H
#define DATABASEBROWN_H

#include <VDatabase.h>

#include <QFileInfo>

class DatabaseBrown : public VDatabase
{
 Q_OBJECT

  public:
      explicit DatabaseBrown(QObject* parent, QFileInfo file_path);
      ~DatabaseBrown();

      bool register_module_name(QString, QString);
      bool MaPSA_to_BP(QString, QString, QString="", QString="");
      bool PSs_to_spacers(QString, QString="", QString="");
      bool PSs_to_MaPSA(QString, QString, QString, QString="", QString="");
      bool PSs_to_MaPSA(QString, QString) { return false; };

      bool is_component_available(QString, QString){ return true; };

  protected:

      void error_message(QString message);

      QFileInfo m_file_path;


};

#endif // DATABASEBROWN_H
