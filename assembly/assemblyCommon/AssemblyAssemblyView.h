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

#ifndef ASSEMBLYASSEMBLYVIEW_H
#define ASSEMBLYASSEMBLYVIEW_H

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QCheckBox>
#include <QLabel>

class AssemblyAssemblyView : public QWidget
{
 Q_OBJECT

 public:
  explicit AssemblyAssemblyView(const QObject* const, QWidget* parent=nullptr);
  virtual ~AssemblyAssemblyView() {}

 public slots:
  void display_infoTab();

 protected:
  QCheckBox* smartMove_checkbox_;

  QWidget* wid_PSSAlignm_;
  QWidget* wid_PSSToSpacers_;
  QWidget* wid_PSSToMaPSA_;
  QWidget* wid_PSToBasep_;
};

#endif // ASSEMBLYASSEMBLYVIEW_H
