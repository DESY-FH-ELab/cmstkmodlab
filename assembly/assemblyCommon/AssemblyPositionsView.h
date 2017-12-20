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

#ifndef ASSEMBLYPOSITIONSVIEW_H
#define ASSEMBLYPOSITIONSVIEW_H

#include <AssemblyUEyeView.h>
#include <LStepExpressMotionManager.h>
#include <nqlogger.h>

#include <vector>

#include <QWidget>
#include <QScrollArea>
#include <QKeyEvent>
#include <QPushButton>
#include <QLineEdit>
#include <QLabel>

#include <opencv2/opencv.hpp>

class AssemblyPositionsWidget;

class AssemblyPositionsView : public QWidget
{
 Q_OBJECT

 public:

  explicit AssemblyPositionsView(const LStepExpressMotionManager*, QWidget* parent=0);
  virtual ~AssemblyPositionsView() {}

  void    connectImageProducer(const QObject* sender, const char* signal);
  void disconnectImageProducer(const QObject* sender, const char* signal);

 protected:

  const LStepExpressMotionManager* motion_manager_;

  void keyReleaseEvent(QKeyEvent*);

  AssemblyUEyeView* img_view_;
  QScrollArea*      img_scroll_;
  QPushButton*      img_button_;
  QPushButton*      cli_button_;

  cv::Mat image_;

  std::vector<AssemblyPositionsWidget*> v_wpos_;

 public slots:

  void update_image(const cv::Mat&);
  void save_image();
  void modify_image_centerlines();

 signals:

  void image_updated(const cv::Mat&);
};

class AssemblyPositionsWidget : public QWidget
{
 Q_OBJECT

 public:

  explicit AssemblyPositionsWidget(const LStepExpressMotionManager*, QWidget* parent=0);
  virtual ~AssemblyPositionsWidget() {}

 protected:

  const LStepExpressMotionManager* motion_manager_;

  QPushButton* pos_button_;
  QLineEdit*   pos_lineed_value_;
  QLineEdit*   pos_lineed_comme_;

 public slots:

  void update_position();
};

//!! #include <ConradModel.h>
//!! #include <LStepExpressMotionManager.h>
//!! #include <AssemblyVUEyeCamera.h>
//!! #include <AssemblyUEyeView.h>
//!! #include <AssemblyObjectFinderPatRec.h>
//!! #include <AssemblyMultiPickupTester.h>
//!! 
//!! #include <string>
//!! 
//!! #include <QWidget>
//!! #include <QString>
//!! #include <QFormLayout>
//!! #include <QScrollArea>
//!! #include <QKeyEvent>
//!! #include <QGridLayout>
//!! #include <QPushButton>
//!! #include <QLineEdit>
//!! #include <QLabel>
//!! #include <QGroupBox>
//!! #include <QRadioButton>
//!! #include <QVBoxLayout>
//!! 
//!! #include <opencv2/opencv.hpp>
//!! 
//!! class AssemblyPositionsView : public QWidget
//!! {
//!!  Q_OBJECT
//!! 
//!!  public:
//!! 
//!!   explicit AssemblyPositionsView(QWidget* parent=0);
//!!   virtual ~AssemblyPositionsView();
//!! 
//!!  protected:
//!! 
//!!   void keyReleaseEvent(QKeyEvent *event);
//!! 
//!!   QScrollArea* scrollArea_1_;
//!!   QScrollArea* scrollArea_2_;
//!!   QScrollArea* scrollArea_3_;
//!!   QScrollArea* scrollArea_4_;
//!! 
//!!   AssemblyUEyeView* imageView_1_;
//!!   AssemblyUEyeView* imageView_2_;
//!!   AssemblyUEyeView* imageView_3_;
//!!   AssemblyUEyeView* imageView_4_;
//!! 
//!!   QLineEdit* liedit_1_;
//!!   QLineEdit* liedit_2_;
//!!   QLineEdit* liedit_3_;
//!!   QLineEdit* liedit_4_;
//!! 
//!!   AssemblyVacuumWidget* w_vacuum_;
//!!   AssemblyPatRecWidget* w_patrec_;
//!! 
//!!   AssemblyMultiPickupTesterWidget* w_mupiup_;
//!! 
//!!   bool objfinder_connected_;
//!! 
//!!  public slots:
//!! 
//!!   void updateImage(const int, const QString&);
//!!   void updateImage(const int, const cv::Mat&);
//!! 
//!!   void updateText(const int, const double, const double, const double);
//!! 
//!!  signals:
//!! 
//!!   void multipickup_request(const AssemblyMultiPickupTester::Configuration&);
//!! 
//!!   void launchAlignment(int, double, double, double);
//!! 
//!!   void launchSandwichAssembly(double, double, double, double, double, double, double, double, double);
//!! };
//!! // ===========================================================================
//!! 
//!! class AssemblyStringWidget : public QWidget
//!! {
//!!  Q_OBJECT
//!! 
//!!  public:
//!! 
//!!   explicit AssemblyStringWidget(const QString&, const QString&, QWidget* parent=0);
//!!   virtual ~AssemblyStringWidget() {}
//!! 
//!!   QPushButton* button() const { return button_; }
//!!   QLineEdit*   lineed() const { return lineed_; }
//!! 
//!!   QString get_input_string() const;
//!! 
//!!  protected:
//!! 
//!!   QFormLayout* layout_;
//!!   QPushButton* button_;
//!!   QLineEdit*   lineed_;
//!! 
//!!  public slots:
//!! 
//!!   void execute();
//!! 
//!!  signals:
//!! 
//!!   void input_string(QString);
//!! };
//!! // ===========================================================================
//!! 
//!! class AssemblyMoveWidget : public QWidget
//!! {
//!!  Q_OBJECT
//!! 
//!!  public:
//!! 
//!!   explicit AssemblyMoveWidget(const QString&, const QString&, const bool move_relative=false, QWidget* parent=0);
//!!   virtual ~AssemblyMoveWidget() {}
//!! 
//!!   QPushButton* button() const { return button_; }
//!!   QLineEdit*   lineed() const { return lineed_; }
//!! 
//!!   void useMoveRelative(const bool b=false){ moveRelative_ = b; }
//!! 
//!!   QString get_input_string() const;
//!! 
//!!  protected:
//!! 
//!!   bool moveRelative_;
//!! 
//!!   QFormLayout* layout_;
//!!   QPushButton* button_;
//!!   QLineEdit*   lineed_;
//!! 
//!!  public slots:
//!! 
//!!   void execute();
//!!     
//!!   void enable(const bool b=true);
//!! 
//!!  signals:
//!! 
//!!   void moveAbsolute(const double, const double, const double, const double);
//!!   void moveRelative(const double, const double, const double, const double);
//!! };
//!! // ===========================================================================
//!!   QLineEdit*   pickup_lineed_;
//!!   QPushButton* pickup_button_;
//!! 
//!!   QLabel*      measur_label_;
//!!   QLineEdit*   measur_lineed_;
//!!   QPushButton* measur_button_;
//!! 
//!!   QLabel*      iteraN_label_;
//!!   QLineEdit*   iteraN_lineed_;
//!! 
//!!  public slots:
//!! 
//!!   void execute();
//!! 
//!!   void update_position_measurement();
//!!   void update_position_pickup();
//!! 
//!!   void enable(const bool);
//!! 
//!!  signals:
//!! 
//!!   void multipickup_request(const AssemblyMultiPickupTester::Configuration&);
//!! };
//!! // ===========================================================================

#endif // ASSEMBLYPOSITIONSVIEW_H
