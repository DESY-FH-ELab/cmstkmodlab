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

#ifndef ASSEMBLYMODULEASSEMBLER_H
#define ASSEMBLYMODULEASSEMBLER_H

#include <AssemblyUEyeView.h>
#include <AssemblyVUEyeCamera.h>
#include <MarkerFinderPatRec.h>
#include <LStepExpressMotionManager.h>
#include <ConradModel.h>

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

class VacuumWidget;

class AssemblyModuleAssembler : public QWidget
{
 Q_OBJECT

 public:

  explicit AssemblyModuleAssembler(LStepExpressMotionManager*, MarkerFinderPatRec*, QWidget* parent=0);

  double pickup_position;

  // to connect vacuum signals in MainWindow
  VacuumWidget* VacuumToggler() const { return w_vacuum_; }

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

  VacuumWidget* w_vacuum_;

 public slots:

  void updateImage(const int, const QString&);
  void updateImage(const int, const cv::Mat&);

  void updateText(const int, const double, const double, const double);
  void startMacro(double, double, double, double, double, double, int);

 signals:

  void launchAlignment(int, double, double, double);

  void launchPrecisionEstimation(double, double, double, double, double, double, int);
  void launchSandwichAssembly(double, double, double, double, double, double, double, double, double);
};
// ===========================================================================

class AssemblyPrecisionEstimator : public QWidget
{
    Q_OBJECT
public:
    
    explicit AssemblyPrecisionEstimator(QWidget *parent = 0, std::string text ="Estimate Assembly Precision",
                                        std::string measurement_position = "0.0,0.0,0.0", std::string pickup_position = "0.0,0.0,0.0", int iterations = 1 , ConradModel * conradModel_ = 0);
    
    double local_x, local_y, local_z, local_a;
    QPushButton* button1;
    
    QLabel * label1;
    QLabel * label2;
    QLabel * label3;
    
    QLineEdit *lineEdit1;
    QLineEdit *lineEdit2;
    QLineEdit *lineEdit3;

    
    protected:
    
    public slots:
    void recordPosition(double,double,double);
    void run();
signals:
    void moveAbsolute(double,double,double,double);
    void locateMarker();
    void launchPrecisionEstimation(double, double, double, double, double, double, int);
};
// ===========================================================================

class AssemblySandwichAssembler : public QWidget
{
    Q_OBJECT
public:
    
    explicit AssemblySandwichAssembler(QWidget *parent = 0, std::string text ="Assemble sandwich",
                                        std::string assembly_position = "0.0,0.0,0.0", std::string bottom_part_position = "0.0,0.0,0.0", std::string top_part_position = "0.0,0.0,0.0");
    
    //double local_x, local_y, local_z, local_a;
    QPushButton* button0;
    QPushButton* button1;
    
    QLabel * label1;
    QLabel * label2;
    QLabel * label3;
    
    QLineEdit *lineEdit1;
    QLineEdit *lineEdit2;
    QLineEdit *lineEdit3;

    
    protected:
    
    public slots:
    void run();
    void run_alignment();

    signals:
    void launchSandwichAssembly(double, double, double, double, double, double, double, double, double);
    void launchAlignment(int, double, double, double );
};
// ===========================================================================

class StringWidget : public QWidget
{
 Q_OBJECT

 public:

  explicit StringWidget(const QString&, const QString&, QWidget* parent=0);
  virtual ~StringWidget() {}

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

class MoveWidget : public QWidget
{
 Q_OBJECT

 public:

  explicit MoveWidget(const QString&, const QString&, const bool move_relative=false, QWidget* parent=0);
  virtual ~MoveWidget() {}

  void useMoveRelative(const bool b=false){ moveRelative_ = b; }

 protected:

  bool moveRelative_;

  QFormLayout* layout_;
  QPushButton* button_;
  QLineEdit*   lineed_;

 public slots:

  void execute();
    
 signals:

  void moveAbsolute(const double, const double, const double, const double);
  void moveRelative(const double, const double, const double, const double);
};
// ===========================================================================

class PatRecWidget : public QWidget
{
 Q_OBJECT

 public:

  explicit PatRecWidget(const QString&, QWidget* parent=0);
  virtual ~PatRecWidget() {}

  const StringWidget* widget_angrough() { return sw_angrough_; }
  const StringWidget* widget_angscanp() { return sw_angscanp_; }

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

  StringWidget* sw_angrough_;
  StringWidget* sw_angscanp_;

 public slots:

  void execute();

  void change_label(const int);

 signals:

  void mode(int, int);

  void sendPosition(int, double, double, double);
};
// ===========================================================================

class VacuumWidget : public QWidget
{
 Q_OBJECT

 public:

  explicit VacuumWidget(const QString&, QWidget* parent=0);
  virtual ~VacuumWidget() {}

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
// ===========================================================================

//!!class AssemblyMountChecker : public QWidget
//!!{
//!!  Q_OBJECT
//!!
//!!public:
//!!
//!!  explicit AssemblyMountChecker(QWidget *parent = 0, std::string ="test",
//!!                                double x =0.0, double y =0.0, double z  =0.0,
//!!                                double a  =0.0, int =0);
//!!
//!!  double local_x, local_y, local_z, local_a;
//!!  QPushButton* button1;
//!!  QLineEdit *lineEdit1;
//!!
//!!protected:
//!!
//!!public slots:
//!!
//!!  void checkMount();
//!!
//!!signals:
//!!
//!!  void moveAbsolute(double,double,double,double);
//!!  void moveRelative(double,double,double,double);
//!!  void locateCorner(int);
//!!  void reportCornerLocation(int);
//!!};
//!!// ===========================================================================
//!!
//!!class AssemblyAligner : public QWidget
//!!{
//!!  Q_OBJECT      
//!!public:
//!!
//!!  explicit AssemblyAligner(QWidget *parent = 0, std::string ="test",
//!!                           double a  = 0.0);
//!!
//!!  double local_x, local_y, local_z, local_a;
//!!  QPushButton* button1;
//!!  QLineEdit *lineEdit1;
//!!
//!!protected:
//!!
//!!public slots:
//!!
//!!  void align();
//!!  void setDown();
//!!
//!!signals:
//!!
//!!  void moveRelative(double,double,double,double);
//!!  void locateSetdowncorner(int);
//!!};
//!!// ===========================================================================

#endif // ASSEMBLYMODULEASSEMBLER_H
