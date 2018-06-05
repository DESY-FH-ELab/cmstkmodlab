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

class AssemblyAssemblyView : public QWidget
{
 Q_OBJECT

 public:

  explicit AssemblyAssemblyView(QWidget* parent=nullptr);
  virtual ~AssemblyAssemblyView() {}

 protected:

  QWidget* CalibRotStage_wid_;
  QWidget* PSPToBasep_wid_;
  QWidget* PSSToSpacers_wid_;
  QWidget* PSSSpacToPSPBas_wid_;
};

#endif // ASSEMBLYASSEMBLYVIEW_H
