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

#ifndef ASSEMBLYASSEMBLYACTIONWIDGET_H
#define ASSEMBLYASSEMBLYACTIONWIDGET_H

#include <QWidget>
#include <QHBoxLayout>
#include <QPushButton>
#include <QCheckBox>
#include <QLabel>

class AssemblyAssemblyActionWidget : public QWidget
{
 Q_OBJECT

 public:
  explicit AssemblyAssemblyActionWidget(QWidget* parent=nullptr);
  virtual ~AssemblyAssemblyActionWidget() {}

  QHBoxLayout* layout() const { return layout_; }

  QLabel* label() const { return label_; }
  QPushButton* button() const { return button_; }
  QCheckBox* checkbox() const { return checkbox_; }

  void connect_action(const QObject*, const char*, const char*);

 protected:
  QHBoxLayout* layout_;

  QLabel* label_;
  QPushButton* button_;
  QCheckBox* checkbox_;

  const QObject* qobject_;
  const char* start_slot_;
  const char* stop_signal_;

  bool inhibit_dialogue_;

 public slots:
  void disable(const bool b=true);
  void disable(const int);

  void reset_action();
  void start_action();
  void disable_action();

 signals:
  void action_request();
};

#endif // ASSEMBLYASSEMBLYACTIONWIDGET_H
