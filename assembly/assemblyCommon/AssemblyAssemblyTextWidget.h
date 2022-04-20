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

#ifndef ASSEMBLYASSEMBLYTEXTWIDGET_H
#define ASSEMBLYASSEMBLYTEXTWIDGET_H

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QCheckBox>
#include <QLabel>

class AssemblyAssemblyTextWidget : public QWidget
{
 Q_OBJECT

 public:
  explicit AssemblyAssemblyTextWidget(QWidget* parent=nullptr);
  virtual ~AssemblyAssemblyTextWidget() {}

  QHBoxLayout* layout() const { return layout_; }

  QLabel* label() const { return label_; }
  QLabel* text() const { return text_; }
  QCheckBox* checkbox() const { return checkbox_; }

  void connect_check_action(const QObject*, const char*, const char* = nullptr);

 protected:
  QHBoxLayout* layout_;

  QLabel* label_;
  QLabel* text_;
  QCheckBox* checkbox_;

  const QObject* qobject_;
  const char* check_slot_;
  const char* uncheck_slot_;

 public slots:
  void disable(const bool b=true);
  void disable(const int);

  void reset_action();
  void change_action(int);

 signals:
 void action_request();
};
// ====================================================================================================

#endif // ASSEMBLYASSEMBLYTEXTWIDGET_H
