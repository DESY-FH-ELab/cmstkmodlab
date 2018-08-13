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

#ifndef ASSEMBLYASSEMBLYVIEW_H
#define ASSEMBLYASSEMBLYVIEW_H

#include <ConradModel.h>
#include <LStepExpressMotionManager.h>
#include <AssemblyVUEyeCamera.h>
#include <AssemblyUEyeView.h>
#include <AssemblyObjectFinderPatRec.h>
#include <AssemblyMultiPickupTester.h>

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

class AssemblyPatRecWidget;
class AssemblyAlignmWidget;
class AssemblyMultiPickupTesterWidget;

class AssemblyAssemblyView : public QWidget
{
 Q_OBJECT

 public:

  explicit AssemblyAssemblyView(const LStepExpressMotionManager*, QWidget* parent=nullptr);

  AssemblyPatRecWidget* PatRec_Widget() const { return w_patrec_; }
  AssemblyAlignmWidget* Alignm_Widget() const { return w_alignm_; }

  AssemblyMultiPickupTesterWidget* MultiPickup_Widget() { return w_mupiup_; }

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

  AssemblyPatRecWidget* w_patrec_;
  AssemblyAlignmWidget* w_alignm_;

  AssemblyMultiPickupTesterWidget* w_mupiup_;

  bool objfinder_connected_;

 public slots:

  void updateImage(const int, const QString&);
  void updateImage(const int, const cv::Mat&);

  void updateText(const int, const double, const double, const double);

 signals:

  void multipickup_request(const AssemblyMultiPickupTester::Configuration&);

  void launchAlignment(int, double, double, double);

//  void launchSandwichAssembly(double, double, double, double, double, double, double, double, double);
};
// ===========================================================================

class AssemblyStringWidget : public QWidget
{
 Q_OBJECT

 public:

  explicit AssemblyStringWidget(const QString&, const QString&, QWidget* parent=nullptr);
  virtual ~AssemblyStringWidget() {}

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

class AssemblyPatRecWidget : public QWidget
{
 Q_OBJECT

 public:

  explicit AssemblyPatRecWidget(const QString&, QWidget* parent=nullptr);
  virtual ~AssemblyPatRecWidget() {}

  QFormLayout* layout() const { return layout_; }

  const AssemblyStringWidget* widget_angrough() { return sw_angrough_; }
  const AssemblyStringWidget* widget_angscanp() { return sw_angscanp_; }

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

  AssemblyStringWidget* sw_angrough_;
  AssemblyStringWidget* sw_angscanp_;

 public slots:

  void execute();

  void change_label(const int);

 signals:

  void mode(int, int);

  void sendPosition(int, double, double, double);
};
// ===========================================================================

class AssemblyAlignmWidget : public QWidget
{
 Q_OBJECT

 public:

  explicit AssemblyAlignmWidget(QWidget* parent=nullptr);
  virtual ~AssemblyAlignmWidget() {}

 protected:

  QPushButton* measur_button_;
  QPushButton* alignm_button_;

  QLabel*      obj_deltaX_label_;
  QLineEdit*   obj_deltaX_lineed_;

  QLabel*      obj_deltaY_label_;
  QLineEdit*   obj_deltaY_lineed_;

  QLabel*      ang_target_label_;
  QLineEdit*   ang_target_lineed_;

  QLabel*      obj_angle_label_;
  QLineEdit*   obj_angle_lineed_;

 public slots:

  void execute(const bool);

  void measure_angle();
  void align_object();

  void enable(const bool on=true);

  void show_object_angle(const double);

 signals:

  void measure_angle_request(const double, const double);
  void alignment_request    (const double, const double, const double);
};
// ===========================================================================

class AssemblyMultiPickupTesterWidget : public QWidget
{
 Q_OBJECT

 public:

  explicit AssemblyMultiPickupTesterWidget(const QString&, const LStepExpressMotionManager*, QWidget* parent=nullptr);
  virtual ~AssemblyMultiPickupTesterWidget() {}

  QGridLayout* layout() const { return layout_; }

 protected:

  const LStepExpressMotionManager* const motion_manager_;

  QGridLayout* layout_;

  QPushButton* exe_button_;

  QLabel *     pickup_label_;
  QLineEdit*   pickup_lineed_;
  QPushButton* pickup_button_;

  QLabel*      measur_label_;
  QLineEdit*   measur_lineed_;
  QPushButton* measur_button_;

  QLabel*      iteraN_label_;
  QLineEdit*   iteraN_lineed_;

 public slots:

  void execute();

  void update_position_measurement();
  void update_position_pickup();

  void enable(const bool);

 signals:

  void multipickup_request(const AssemblyMultiPickupTester::Configuration&);
};
// ===========================================================================

#endif // ASSEMBLYASSEMBLYVIEW_H
