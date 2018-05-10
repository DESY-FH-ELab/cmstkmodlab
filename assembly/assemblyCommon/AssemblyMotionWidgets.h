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

  explicit AssemblyMoveWidget(const LStepExpressMotionManager* const, const QString&, const bool move_relative=false, QWidget* parent=nullptr);
  virtual ~AssemblyMoveWidget() {}

  const LStepExpressMotionManager* manager() const { return manager_; }

  QVBoxLayout* layout() const { return layout_; }
  QPushButton* button() const { return button_; }

  QHBoxLayout* XYZA_layout() const { return XYZA_lay_; }

  void use_move_relative(const bool b=false){ move_relative_ = b; }

 protected:

  const LStepExpressMotionManager* const manager_;

  bool move_relative_;

  QVBoxLayout* layout_;
  QPushButton* button_;

  QHBoxLayout* XYZA_lay_;

  QLineEdit* X_lineed_;
  QLineEdit* Y_lineed_;
  QLineEdit* Z_lineed_;
  QLineEdit* A_lineed_;

 public slots:

  void execute();

  void  enable(){ this->setEnabled(true) ; }
  void disable(){ this->setEnabled(false); }

 signals:

  void move_absolute(const double, const double, const double, const double);
  void move_relative(const double, const double, const double, const double);
};

#endif // ASSEMBLYMOTIONWIDGETS_H
