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

#ifndef ASSEMBLYPOSITIONSREGISTRYWIDGET_H
#define ASSEMBLYPOSITIONSREGISTRYWIDGET_H

#include <LStepExpressMotionManager.h>

#include <vector>

#include <QWidget>
#include <QPushButton>
#include <QLineEdit>
#include <QString>

class AssemblyPositionWidget;

class AssemblyPositionsRegistryWidget : public QWidget
{
 Q_OBJECT

 public:

  explicit AssemblyPositionsRegistryWidget(const LStepExpressMotionManager* const, QWidget* parent=nullptr);
  virtual ~AssemblyPositionsRegistryWidget() {}

 protected:

  const LStepExpressMotionManager* const motion_manager_;

  std::vector<AssemblyPositionWidget*> v_wpos_;

  QLineEdit*   pos_calc_lineed_from_;
  QLineEdit*   pos_calc_lineed_to_;
  QLineEdit*   pos_calc_lineed_res_;
  QPushButton* pos_calc_button_;

  bool load_position_4vector(std::vector<double>&, QString) const;

 public slots:

  void calculate_relative_distance();
};
// ==================================================

class AssemblyPositionWidget : public QWidget
{
 Q_OBJECT

 public:

  explicit AssemblyPositionWidget(const QString&, const LStepExpressMotionManager* const, QWidget* parent=nullptr);
  virtual ~AssemblyPositionWidget() {}

  QString position_qstring() const { return pos_lineed_value_->text(); }

 protected:

  const LStepExpressMotionManager* const motion_manager_;

  QPushButton* pos_button_;
  QLineEdit*   pos_lineed_value_;
  QLineEdit*   pos_lineed_comme_;

 public slots:

  void update_position();
};
// ==================================================

#endif // ASSEMBLYPOSITIONSREGISTRYWIDGET_H
