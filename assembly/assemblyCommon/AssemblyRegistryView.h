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

#ifndef ASSEMBLYREGISTRYVIEW_H
#define ASSEMBLYREGISTRYVIEW_H

#include <AssemblyUEyeView.h>
#include <LStepExpressMotionManager.h>
#include <nqlogger.h>

#include <vector>

#include <QWidget>
#include <QScrollArea>
#include <QKeyEvent>
#include <QPushButton>
#include <QLineEdit>
#include <QLabel>

#include <opencv2/opencv.hpp>

class AssemblyRegistryPositionWidget;

class AssemblyRegistryView : public QWidget
{
 Q_OBJECT

 public:

  explicit AssemblyRegistryView(const LStepExpressMotionManager*, QWidget* parent=nullptr);
  virtual ~AssemblyRegistryView() {}

 protected:

  const LStepExpressMotionManager* motion_manager_;

  std::vector<AssemblyRegistryPositionWidget*> v_wpos_;

  QLineEdit*   pos_calc_lineed_from_;
  QLineEdit*   pos_calc_lineed_to_;
  QLineEdit*   pos_calc_lineed_res_;
  QPushButton* pos_calc_button_;

  bool load_position_4vector(std::vector<double>&, QString) const;

 public slots:

  void calculate_relative_distance();
};
// ==================================================

class AssemblyRegistryPositionWidget : public QWidget
{
 Q_OBJECT

 public:

  explicit AssemblyRegistryPositionWidget(const QString&, const LStepExpressMotionManager*, QWidget* parent=nullptr);
  virtual ~AssemblyRegistryPositionWidget() {}

  QString position_qstring() const { return pos_lineed_value_->text(); }

 protected:

  const LStepExpressMotionManager* motion_manager_;

  QPushButton* pos_button_;
  QLineEdit*   pos_lineed_value_;
  QLineEdit*   pos_lineed_comme_;

 public slots:

  void update_position();
};
// ==================================================

#endif // ASSEMBLYREGISTRYVIEW_H
