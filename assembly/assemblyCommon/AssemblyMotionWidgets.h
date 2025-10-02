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

#ifndef ASSEMBLYMOTIONWIDGETS_H
#define ASSEMBLYMOTIONWIDGETS_H

#include <LStepExpressMotionManager.h>

#include <QWidget>
#include <QString>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLineEdit>

class AssemblyMoveWidget : public QWidget
{
 Q_OBJECT

 public:

  explicit AssemblyMoveWidget(const LStepExpressMotionManager* const, QWidget* parent=nullptr);
  virtual ~AssemblyMoveWidget() {}

  const LStepExpressMotionManager* manager() const { return manager_; }

  QVBoxLayout* layout() const { return layout_; }

  QHBoxLayout* layout_XYZA() const { return lay_XYZA_; }

 protected:

  const LStepExpressMotionManager* const manager_;

  bool in_execution_;

  QVBoxLayout* layout_;

  QHBoxLayout* lay_XYZA_;

  QLineEdit* X_lineed_;
  QLineEdit* Y_lineed_;
  QLineEdit* Z_lineed_;
  QLineEdit* A_lineed_;

  QHBoxLayout* lay_cmds_;

  QPushButton* button_moveabs_;
  QPushButton* button_moverel_;
  QPushButton* button_readpos_;
  QPushButton* button_clear_;

 public slots:

  void execute(const bool);

  void move_absolute(){ this->execute(false); }
  void move_relative(){ this->execute(true); }

  void  read_positions();
  void clear_positions();

  void reactivate();
  void deactivate();

 signals:

  void move_absolute(const double, const double, const double, const double);
  void move_relative(const double, const double, const double, const double);
};

#endif // ASSEMBLYMOTIONWIDGETS_H
