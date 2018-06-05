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

#include <AssemblyUEyeView.h>
#include <AssemblyObjectAligner.h>
#include <AssemblyObjectFinderPatRecWidget.h>

#include <QWidget>
#include <QString>
#include <QScrollArea>
#include <QKeyEvent>
#include <QCheckBox>
#include <QRadioButton>
#include <QPushButton>
#include <QLineEdit>

#include <opencv2/opencv.hpp>

class AssemblyAssemblyView : public QWidget
{
 Q_OBJECT

 public:

  explicit AssemblyAssemblyView(QWidget* parent=nullptr);
  virtual ~AssemblyAssemblyView() {}
};

#endif // ASSEMBLYASSEMBLYVIEW_H
