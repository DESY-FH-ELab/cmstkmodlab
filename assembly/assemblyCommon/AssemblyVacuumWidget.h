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


/////////////////////////////////////////////////////////////////////////////////
//                                                                             //
//            Fourth Vacuum Line Capability Added by Elise Hinkle              //
//                       Last Modified October 7, 2019                         //
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

#include <map>

class AssemblyVacuumWidget : public QWidget
{
 Q_OBJECT

 public:

  explicit AssemblyVacuumWidget(const QString&, QWidget* parent=nullptr);
  virtual ~AssemblyVacuumWidget() {}

  QVBoxLayout* layout() const { return layout_; }

  class Entry
  {
   public:
    explicit Entry() : radioButton_(nullptr), label_(nullptr) {}
    virtual ~Entry() {}

    QRadioButton* radioButton_;
    QLabel* label_;
  };

  bool   has(const int) const;
  Entry& get(const int);

 protected:

  QVBoxLayout* layout_;
  QPushButton* button_;

  std::map<int, Entry> valuemap_;

 public slots:

  void toggleVacuum();
  void updateVacuumChannelState(const int, const bool);

  void updateVacuumChannelsStatus();

  void  enableVacuumButton();
  void disableVacuumButton();

 signals:

  void toggleVacuum(const int);

  void vacuumChannelState_request(const int);
};

#endif // ASSEMBLYVACUUMWIDGET_H
