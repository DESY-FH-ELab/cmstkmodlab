/////////////////////////////////////////////////////////////////////////////////
//                                                                             //
//               Copyright (C) 2011-2025 - The DESY CMS Group                  //
//                           All rights reserved                               //
//                                                                             //
//      The CMStkModLab source code is licensed under the GNU GPL v3.0.        //
//      You have the right to modify and/or redistribute this source code      //
//      under the terms specified in the license, which may be found online    //
//      at http://www.gnu.org/licenses or at License.txt.                      //
//                                                                             //
/////////////////////////////////////////////////////////////////////////////////

#ifndef ASSEMBLYASSEMBLYV2VIEW_H
#define ASSEMBLYASSEMBLYV2VIEW_H

#include <QObject>
#include <QWidget>
#include <QCheckBox>
#include <QMessageBox>
#include <QLineEdit>
#include <QPushButton>

#include <AssemblyAssemblyActionWidget.h>

class AssemblyAssemblyV2View : public QWidget
{
 Q_OBJECT

 public:
  explicit AssemblyAssemblyV2View(const AssemblyAssemblyV2* const, QWidget* parent=nullptr);
  virtual ~AssemblyAssemblyV2View() {}

 public slots:
  void display_infoTab();
  void disable_DB();

 protected:
  QCheckBox* smartMove_checkbox_;

  QWidget* wid_PSPToBasep_;
  QWidget* wid_PSSToSpacers_;
  QWidget* wid_PSSToMaPSA_;

  AssemblyAssemblyActionWidget* push_step1_to_db_wid_;
  AssemblyAssemblyActionWidget* push_step2_to_db_wid_;
  AssemblyAssemblyActionWidget* push_step3_to_db_wid_;

  QLineEdit* baseplate_id_lineed_;
  QLineEdit* mapsa_id_lineed_;
  QLineEdit* pss_id_lineed_;
  QLineEdit* glue1_id_lineed_;
  QLineEdit* glue2_id_lineed_;
  QLineEdit* glue3_id_lineed_;
  QLineEdit* module_id_lineed_;
  //QPushButton* push_to_db_button_;

};

#endif // ASSEMBLYASSEMBLYV2VIEW_H
