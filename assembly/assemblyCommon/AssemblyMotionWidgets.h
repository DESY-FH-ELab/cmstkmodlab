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

#include <QWidget>
#include <QString>
#include <QFormLayout>
#include <QPushButton>
#include <QLineEdit>

class AssemblyMoveWidget : public QWidget
{
 Q_OBJECT

 public:

  explicit AssemblyMoveWidget(const QString&, const QString&, const bool move_relative=false, QWidget* parent=nullptr);
  virtual ~AssemblyMoveWidget() {}

  QPushButton* button() const { return button_; }
  QLineEdit*   lineed() const { return lineed_; }

  void useMoveRelative(const bool b=false){ moveRelative_ = b; }

  QString get_input_string() const;

 protected:

  bool moveRelative_;

  QFormLayout* layout_;
  QPushButton* button_;
  QLineEdit*   lineed_;

 public slots:

  void execute();
    
  void enable(const bool b=true);

 signals:

  void moveAbsolute(const double, const double, const double, const double);
  void moveRelative(const double, const double, const double, const double);
};
// ===========================================================================

#endif // ASSEMBLYMOTIONWIDGETS_H
