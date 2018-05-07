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

#ifndef ASSEMBLYOBJECTFINDERPATRECVIEW_H
#define ASSEMBLYOBJECTFINDERPATRECVIEW_H

#include <LStepExpressMotionManager.h>
#include <AssemblyVUEyeCamera.h>
#include <AssemblyUEyeView.h>
#include <AssemblyObjectFinderPatRec.h>

#include <string>

#include <QWidget>
#include <QString>
#include <QFormLayout>
#include <QScrollArea>
#include <QKeyEvent>
#include <QGridLayout>
#include <QPushButton>
#include <QLineEdit>
#include <QLabel>
#include <QGroupBox>
#include <QRadioButton>
#include <QVBoxLayout>

#include <opencv2/opencv.hpp>

class AssemblyPatRecWidget_NEW;

class AssemblyObjectFinderPatRecView : public QWidget
{
 Q_OBJECT

 public:

  explicit AssemblyObjectFinderPatRecView(const LStepExpressMotionManager*, QWidget* parent=nullptr);
  virtual ~AssemblyObjectFinderPatRecView() {}

  AssemblyPatRecWidget_NEW* PatRec_Widget() const { return w_patrec_; }

  void connect_to_finder(const AssemblyObjectFinderPatRec*);

 protected:

  void keyReleaseEvent(QKeyEvent *event);

  QScrollArea* scrollArea_1_;
  QScrollArea* scrollArea_2_;
  QScrollArea* scrollArea_3_;
  QScrollArea* scrollArea_4_;

  AssemblyUEyeView* imageView_1_;
  AssemblyUEyeView* imageView_2_;
  AssemblyUEyeView* imageView_3_;
  AssemblyUEyeView* imageView_4_;

  QLineEdit* liedit_1_;
  QLineEdit* liedit_2_;
  QLineEdit* liedit_3_;
  QLineEdit* liedit_4_;

  AssemblyPatRecWidget_NEW* w_patrec_;

  bool objfinder_connected_;

 public slots:

  void updateImage(const int, const QString&);
  void updateImage(const int, const cv::Mat&);

  void updateText(const int, const double, const double, const double);

 signals:

};
// ===========================================================================

class AssemblyStringWidget_NEW : public QWidget
{
 Q_OBJECT

 public:

  explicit AssemblyStringWidget_NEW(const QString&, const QString&, QWidget* parent=nullptr);
  virtual ~AssemblyStringWidget_NEW() {}

  QPushButton* button() const { return button_; }
  QLineEdit*   lineed() const { return lineed_; }

  QString get_input_string() const;

 protected:

  QFormLayout* layout_;
  QPushButton* button_;
  QLineEdit*   lineed_;

 public slots:

  void execute();

 signals:

  void input_string(QString);
};
// ===========================================================================

class AssemblyPatRecWidget_NEW : public QWidget
{
 Q_OBJECT

 public:

  explicit AssemblyPatRecWidget_NEW(const QString&, QWidget* parent=nullptr);
  virtual ~AssemblyPatRecWidget_NEW() {}

  QFormLayout* layout() const { return layout_; }

  const AssemblyStringWidget_NEW* widget_angrough() { return sw_angrough_; }
  const AssemblyStringWidget_NEW* widget_angscanp() { return sw_angscanp_; }

 protected:

  QFormLayout* layout_;
  QPushButton* button_;
  QLineEdit*   lineed_;
  QLabel*      label_;
  QGroupBox*   groupBox1_;
  QGroupBox*   groupBox2_;

  QRadioButton* radio1_;
  QRadioButton* radio2_;
  QRadioButton* radio3_;
  QRadioButton* radio4_;
  QRadioButton* radio5_;
  QRadioButton* radio6_;

  QVBoxLayout* vbox1_;
  QVBoxLayout* vbox2_;

  AssemblyStringWidget_NEW* sw_angrough_;
  AssemblyStringWidget_NEW* sw_angscanp_;

 public slots:

  void execute();

  void change_label(const int);

 signals:

  void mode(int, int);

  void sendPosition(int, double, double, double);
};
// ===========================================================================

#endif // ASSEMBLYOBJECTFINDERPATRECVIEW_H
