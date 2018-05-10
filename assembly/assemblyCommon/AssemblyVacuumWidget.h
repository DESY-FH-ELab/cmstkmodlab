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

#ifndef ASSEMBLYVACUUMWIDGET_H
#define ASSEMBLYVACUUMWIDGET_H

#include <QWidget>
#include <QString>
#include <QVBoxLayout>
#include <QPushButton>
#include <QRadioButton>
#include <QLabel>

class AssemblyVacuumWidget : public QWidget
{
 Q_OBJECT

 public:

  explicit AssemblyVacuumWidget(const QString&, QWidget* parent=nullptr);
  virtual ~AssemblyVacuumWidget() {}

  QVBoxLayout* layout() const { return layout_; }

 protected:

  QVBoxLayout* layout_;
  QPushButton* button_;

  std::vector<QRadioButton*> valves_;
  std::vector<QLabel*>       labels_;

 public slots:

  void toggleVacuum();
  void updateVacuumChannelState(const int, const bool);
  void enableVacuumButton();
  void disableVacuumButton();

 signals:

  void toggleVacuum(const int);
};

#endif // ASSEMBLYVACUUMWIDGET_H
