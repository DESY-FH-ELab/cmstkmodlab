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

//!!#include <AssemblyPositionsView.h>
//!!#include <LStepExpressWidget.h>
//!!#include <ApplicationConfig.h>
//!!#include <nqlogger.h>
//!!#include <Util.h>
//!!
//!!#include <iostream>
//!!#include <sstream>
//!!#include <iomanip>
//!!#include <vector>
//!!#include <stdlib.h>
//!!
//!!#include <QFormLayout>
//!!#include <QFileDialog>
//!!#include <QPushButton>
//!!#include <QString>
//!!#include <QStringList>
//!!#include <QPixmap>
//!!#include <QLabel>
//!!#include <QApplication>
//!!#include <QImageReader>
//!!#include <QPainter>
//!!
//!!#include <TGraph.h>
//!!#include <TCanvas.h>
//!!
//!!AssemblyPositionsView::AssemblyPositionsView(QWidget* parent) :
//!!  QWidget(parent),
//!!
//!!  scrollArea_1_(0),
//!!  scrollArea_2_(0),
//!!  scrollArea_3_(0),
//!!  scrollArea_4_(0),
//!!
//!!  imageView_1_(0),
//!!  imageView_2_(0),
//!!  imageView_3_(0),
//!!  imageView_4_(0),
//!!
//!!  liedit_1_(0),
//!!  liedit_2_(0),
//!!  liedit_3_(0),
//!!  liedit_4_(0),
//!!
//!!  w_vacuum_(0),
//!!  w_patrec_(0),
//!!  w_mupiup_(0),
//!!
//!!  objfinder_connected_(false)
//!!{
//!!  if(motion_manager == nullptr)
//!!  {
//!!    NQLog("AssemblyPositionsView", NQLog::Critical)
//!!       << "input error: null pointer to LStepExpressMotionManager object, exiting constructor";
//!!
//!!    return;
//!!  }
//!!
//!!  QGridLayout*    layout = new QGridLayout(this);
//!!  this->setLayout(layout);
//!!
//!!  // IMAGE VIEW(S) -------
//!!  QGridLayout* g0 = new QGridLayout();
//!!  layout->addLayout(g0, 0, 0);
//!!
//!!  QPalette palette;
//!!  palette.setColor(QPalette::Background, QColor(220, 220, 220));
//!!
//!!  imageView_1_ = new AssemblyUEyeView();
//!!  imageView_1_->setMinimumSize(300,300);
//!!  imageView_1_->setPalette(palette);
//!!  imageView_1_->setBackgroundRole(QPalette::Background);
//!!  imageView_1_->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
//!!  imageView_1_->setScaledContents(true);
//!!  imageView_1_->setAlignment(Qt::AlignCenter);
//!!
//!!  QApplication::processEvents();
//!!
//!!  scrollArea_1_ = new QScrollArea(this);
//!!  scrollArea_1_->setMinimumSize(200, 200);
//!!  scrollArea_1_->setPalette(palette);
//!!  scrollArea_1_->setBackgroundRole(QPalette::Background);
//!!  scrollArea_1_->setAlignment(Qt::AlignCenter);
//!!  scrollArea_1_->setWidget(imageView_1_);
//!!  scrollArea_1_->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
//!!
//!!  g0->addWidget(scrollArea_1_, 0, 0);
//!!  liedit_1_ = new QLineEdit("Object Location = X,Z");
//!!  g0->addWidget(liedit_1_, 1, 0);
//!!
//!!  imageView_2_ = new AssemblyUEyeView();
//!!  imageView_2_->setMinimumSize(200,200);
//!!  imageView_2_->setPalette(palette);
//!!  imageView_2_->setBackgroundRole(QPalette::Background);
//!!  imageView_2_->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
//!!  imageView_2_->setScaledContents(true);
//!!  imageView_2_->setAlignment(Qt::AlignCenter);
//!!
//!!  scrollArea_2_ = new QScrollArea();
//!!  scrollArea_2_->setMinimumSize(200, 200);
//!!  scrollArea_2_->setPalette(palette);
//!!  scrollArea_2_->setBackgroundRole(QPalette::Background);
//!!  scrollArea_2_->setAlignment(Qt::AlignCenter);
//!!  scrollArea_2_->setWidget(imageView_2_);
//!!  scrollArea_2_->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
//!!
//!!  g0->addWidget(scrollArea_2_, 0, 1);
//!!  liedit_2_ = new QLineEdit("Orientation = alpha");
//!!  g0->addWidget(liedit_2_, 1, 1);
//!!
//!!  imageView_3_ = new AssemblyUEyeView();
//!!  imageView_3_->setMinimumSize(200, 200);
//!!  imageView_3_->setPalette(palette);
//!!  imageView_3_->setBackgroundRole(QPalette::Background);
//!!  imageView_3_->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
//!!  imageView_3_->setScaledContents(true);
//!!  imageView_3_->setAlignment(Qt::AlignCenter);
//!!
//!!  scrollArea_3_ = new QScrollArea();
//!!  scrollArea_3_->setMinimumSize(200,200);
//!!  scrollArea_3_->setPalette(palette);
//!!  scrollArea_3_->setBackgroundRole(QPalette::Background);
//!!  scrollArea_3_->setAlignment(Qt::AlignCenter);
//!!  scrollArea_3_->setWidget(imageView_3_);
//!!  scrollArea_3_->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
//!!
//!!  g0->addWidget(scrollArea_3_, 2, 0);
//!!//  liedit_3_ = new QLineEdit("Mntd pos. (cor. 2) = lE3,X");
//!!//  g0->addWidget(liedit_3_, 1, 2);
//!!
//!!  imageView_4_ = new AssemblyUEyeView();
//!!  imageView_4_->setMinimumSize(200,200);
//!!  imageView_4_->setPalette(palette);
//!!  imageView_4_->setBackgroundRole(QPalette::Background);
//!!  imageView_4_->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
//!!  imageView_4_->setScaledContents(true);
//!!  imageView_4_->setAlignment(Qt::AlignCenter);
//!!
//!!  scrollArea_4_ = new QScrollArea();
//!!  scrollArea_4_->setMinimumSize(200, 200);
//!!  scrollArea_4_->setPalette(palette);
//!!  scrollArea_4_->setBackgroundRole(QPalette::Background);
//!!  scrollArea_4_->setAlignment(Qt::AlignCenter);
//!!  scrollArea_4_->setWidget(imageView_4_);
//!!  scrollArea_4_->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
//!!
//!!  g0->addWidget(scrollArea_4_, 2, 1);
//!!//  liedit_4_ = new QLineEdit("Dtchd pos. = ,X,X");
//!!//  g0->addWidget(liedit_4_, 3, 0);
//!!  // ---------------------
//!!
//!!  // MULTI-PICKUP WIDGET -
//!!  //   repeatitions of "pick-up + PatRec" procedure
//!!  //   to measure module-misplacement introduced by the pick-up
//!!  QFormLayout* f10 = new QFormLayout();
//!!  layout->addLayout(f10, 1, 0);
//!!
//!!  QGroupBox* box_mupiup = new QGroupBox(tr("\"Pickup + PatRec\" Iterations"));
//!!
//!!    w_mupiup_ = new AssemblyMultiPickupTesterWidget("Run \"Pickup + PatRec\"", motion_manager);
//!!    f10->addRow(w_mupiup_);
//!!
//!!    connect(w_mupiup_, SIGNAL(multipickup_request(const AssemblyMultiPickupTester::Configuration&)),
//!!            this     , SIGNAL(multipickup_request(const AssemblyMultiPickupTester::Configuration&)));
//!!
//!!  box_mupiup->setLayout(w_mupiup_->layout());
//!!
//!!  f10->addRow(box_mupiup);
//!!  // ---------------------
//!!
//!!  QFormLayout* f01 = new QFormLayout();
//!!  layout->addLayout(f01, 0, 1);
//!!
//!!  // MOVE WIDGETS --------
//!!  QGroupBox* box_move = new QGroupBox(tr("Motion Stage"));
//!!
//!!  QGridLayout* g_move = new QGridLayout();
//!!
//!!    // widget: move absolute
//!!    AssemblyMoveWidget* w_moveabs = new AssemblyMoveWidget("Move Absolute", "0,0,0,0");
//!!    g_move->addWidget(w_moveabs->button(), 0, 0);
//!!    g_move->addWidget(w_moveabs->lineed(), 0, 1);
//!!
//!!    w_moveabs->useMoveRelative(false);
//!!    w_moveabs->setToolTip("(1) Moves x,y,z,a stage using moveAbsolute routine (with respect to origin)");
//!!
//!!    connect(w_moveabs, SIGNAL(moveAbsolute(double, double, double, double)), motion_manager, SLOT(moveAbsolute(double, double, double, double)));
//!!    connect(motion_manager, SIGNAL(motion_finished()), w_moveabs, SLOT(enable()));
//!!    // ---------------------
//!!
//!!    // widget: move relative
//!!    AssemblyMoveWidget* w_moverel = new AssemblyMoveWidget("Move Relative", "0,0,0,0");
//!!    g_move->addWidget(w_moverel->button(), 1, 0);
//!!    g_move->addWidget(w_moverel->lineed(), 1, 1);
//!!
//!!    w_moverel->useMoveRelative(true);
//!!    w_moverel->setToolTip("(2) Moves x,y,z,a stage using moveRelative routine (with respect to current position)");
//!!
//!!    connect(w_moverel, SIGNAL(moveRelative(double, double, double, double)), motion_manager, SLOT(moveRelative(double, double, double, double)));
//!!    connect(motion_manager, SIGNAL(motion_finished()), w_moverel, SLOT(enable()));
//!!    // ---------------------
//!!
//!!  box_move->setLayout(g_move);
//!!
//!!  f01->addRow(box_move);
//!!  // ---------------------
//!!
//!!  // VACUUM WIDGET -------
//!!  QGroupBox* box_vacuum = new QGroupBox(tr("Vacuum"));
//!!
//!!  w_vacuum_ = new AssemblyVacuumWidget("Toggle Vacuum", this);
//!!  w_vacuum_->setToolTip("(3) Controls vacuum valves");
//!!
//!!  box_vacuum->setLayout(w_vacuum_->layout());
//!!
//!!  f01->addRow(box_vacuum);
//!!  // ---------------------
//!!
//!!  // PATREC  WIDGET ------
//!!  QGroupBox* box_patrec = new QGroupBox("Pattern Recognition");
//!!
//!!  w_patrec_ = new AssemblyPatRecWidget("Standalone PatRec");
//!!  w_patrec_->setToolTip("(4) Runs Pattern Recognition routine to determine sensor (x,y,z,a) position");
//!!
//!!  box_patrec->setLayout(w_patrec_->layout());
//!!
//!!  f01->addRow(box_patrec);
//!!  // ---------------------
//!!
//!!  // ALIGNMENT WIDGET ----
//!!  QFormLayout* f11 = new QFormLayout();
//!!  layout->addLayout(f11, 1, 1);
//!!
//!!  QGroupBox* box_alignm = new QGroupBox(tr("Alignment"));
//!!
//!!  AssemblySandwichAssembler* w_alignm = new AssemblySandwichAssembler(this, "Assemble Sandwich");
//!!  w_alignm->setToolTip("(5) Aligns object to target angle (with respect to the x-axis of the motion stage)");
//!!
//!!  connect(w_alignm, SIGNAL(launchSandwichAssembly(double, double, double, double, double, double, double, double, double)),
//!!          this    , SIGNAL(launchSandwichAssembly(double, double, double, double, double, double, double, double, double)));
//!!
//!!  connect(w_alignm, SIGNAL(launchAlignment(int, double, double, double)),
//!!          this    , SIGNAL(launchAlignment(int, double, double, double)));
//!!
//!!  box_alignm->setLayout(w_alignm->layout());
//!!
//!!  f11->addRow(box_alignm);
//!!  // ---------------------
//!!}
//!!
//!!void AssemblyPositionsView::connect_to_finder(const AssemblyObjectFinderPatRec* finder)
//!!{
//!!  if(finder == nullptr)
//!!  {
//!!    NQLog("AssemblyPositionsView", NQLog::Critical) << "connect_to_finder"
//!!       << ": null pointer to AssemblyObjectFinderPatRec object, no action taken";
//!!
//!!    return;
//!!  }
//!!
//!!  if(objfinder_connected_ == false)
//!!  {
//!!    connect(this->PatRec_Widget(), SIGNAL(mode(int, int)), finder, SLOT(run_PatRec(int, int)));
//!!
//!!    connect(finder, SIGNAL(PatRec_exitcode(int)), this->PatRec_Widget(), SLOT(change_label(int)));
//!!
//!!    connect(this->PatRec_Widget()->widget_angrough(), SIGNAL(input_string(QString)), finder, SLOT(update_rough_angles      (QString)));
//!!    connect(this->PatRec_Widget()->widget_angscanp(), SIGNAL(input_string(QString)), finder, SLOT(update_angscan_parameters(QString)));
//!!
//!!    connect(this->PatRec_Widget(), SIGNAL(sendPosition(int, double, double, double)), this, SLOT(updateText (int, double, double, double)));
//!!
//!!    connect(finder, SIGNAL(image_path          (int, QString))               , this, SLOT(updateImage(int, QString)));
//!!    connect(finder, SIGNAL(image_mat           (int, cv::Mat))               , this, SLOT(updateImage(int, cv::Mat)));
//!!    connect(finder, SIGNAL(reportObjectLocation(int, double, double, double)), this, SLOT(updateText (int, double, double, double)));
//!!
//!!    NQLog("AssemblyPositionsView", NQLog::Spam) << "connect_to_finder"
//!!       << ": widget connected to object of type AssemblyObjectFinderPatRec";
//!!
//!!    objfinder_connected_ = true;
//!!  }
//!!  else
//!!  {
//!!    NQLog("AssemblyPositionsView", NQLog::Critical) << "connect_to_finder"
//!!       << ": widget already connected to an object of type AssemblyObjectFinderPatRec, no action taken";
//!!
//!!    return;
//!!  }
//!!
//!!  return;
//!!}
//!!
//!!void AssemblyPositionsView::updateText(int stage, double x, double y, double a)
//!!{
//!!  NQLog("AssemblyPositionsView", NQLog::Spam) << "updateText"
//!!     << "(" << stage << ", " << x << ", " << y << ", " << a << ")";
//!!
//!!  std::stringstream posi_strs;
//!!  posi_strs << std::setprecision(3) << x << ", " << y;
//!!  const QString posi_qstr = QString::fromStdString(posi_strs.str());
//!!
//!!  std::stringstream orie_strs;
//!!  orie_strs << std::setprecision(3) << a;
//!!  const QString orie_qstr = QString::fromStdString(orie_strs.str());
//!!
//!!  if(stage == 1)
//!!  {
//!!    liedit_1_->setText("Object location = "   +posi_qstr+" [pixel #, pixel #] (field of view)");
//!!    liedit_2_->setText("Object orientation = "+orie_qstr+" degrees");
//!!  }
//!!}
//!!
//!!void AssemblyPositionsView::updateImage(const int stage, const QString& filename)
//!!{
//!!  NQLog("AssemblyPositionsView", NQLog::Spam) << "updateImage(" << stage << ", file=" << filename << ")";
//!!
//!!  const std::string filename_ss = filename.toUtf8().constData();
//!!
//!!  const cv::Mat img = cv::imread(filename_ss, CV_LOAD_IMAGE_UNCHANGED);
//!!
//!!  const QImageReader img_reader(QString::fromStdString(filename_ss));
//!!
//!!  if(img_reader.format() == "png" && (img.channels() > 1))
//!!  {
//!!    cv::Mat img_1;
//!!    cv::cvtColor(img, img_1, CV_RGB2BGR);
//!!
//!!    this->updateImage(stage, img_1);
//!!  }
//!!  else
//!!  {
//!!    this->updateImage(stage, img);
//!!  }
//!!}
//!!
//!!void AssemblyPositionsView::updateImage(const int stage, const cv::Mat& img)
//!!{
//!!  NQLog("AssemblyPositionsView", NQLog::Spam) << "updateImage(" << stage << ", image)";
//!!
//!!  if(stage == 1)
//!!  {
//!!    imageView_1_->setImage(img);
//!!//    imageView_1_->setZoomFactor(0.3);
//!!  }
//!!  else if(stage == 2)
//!!  {
//!!    imageView_2_->setImage(img);
//!!    imageView_2_->setZoomFactor(0.5);
//!!  }
//!!  else if(stage == 3)
//!!  {
//!!    imageView_3_->setImage(img);
//!!//    imageView_3_->setZoomFactor(0.5);
//!!  }
//!!  else if(stage == 4)
//!!  {
//!!    imageView_4_->setImage(img);
//!!//    imageView_4_->setZoomFactor(0.5);
//!!  }
//!!}
//!!
//!!void AssemblyPositionsView::keyReleaseEvent(QKeyEvent * event)
//!!{
//!!  if(!(event->modifiers() & Qt::ShiftModifier))
//!!  {
//!!    switch(event->key())
//!!    {
//!!      case Qt::Key_0:
//!!//        imageView_1->setZoomFactor(0.25);
//!!        event->accept();
//!!        break;
//!!
//!!      case Qt::Key_1:
//!!//        imageView_1->setZoomFactor(1.00);
//!!        event->accept();
//!!        break;
//!!
//!!      case Qt::Key_Plus:
//!!//        imageView_1->increaseZoomFactor();
//!!        event->accept();
//!!        break;
//!!
//!!      case Qt::Key_Minus:
//!!//        imageView_1->decreaseZoomFactor();
//!!        event->accept();
//!!        break;
//!!
//!!      default:
//!!        break;
//!!    }
//!!  }
//!!}
//!!// ===========================================================================
//!!
//!!AssemblyMultiPickupTesterWidget::AssemblyMultiPickupTesterWidget(const QString& label, const LStepExpressMotionManager* motion_manager, QWidget* parent) :
//!!  QWidget(parent),
//!!  motion_manager_(motion_manager)
//!!{
//!!  if(!motion_manager_)
//!!  {
//!!    NQLog("AssemblyMultiPickupTesterWidget", NQLog::Critical)
//!!       << "input error: null pointer to LStepExpressMotionManager object, exiting constructor";
//!!
//!!    return;
//!!  }
//!!
//!!  layout_ = new QGridLayout(this);
//!!  this->setLayout(layout_);
//!!
//!!  exe_button_ = new QPushButton(label);
//!!  layout_->addWidget(exe_button_, 0, 0);
//!!
//!!  QFormLayout* fl1 = new QFormLayout();
//!!  layout_->addLayout(fl1, 1, 0);
//!!
//!!  QGridLayout* g2 = new QGridLayout();
//!!
//!!  measur_label_  = new QLabel("Measurement Position (x,y,z)");
//!!  measur_lineed_ = new QLineEdit("0,0,0");
//!!  measur_button_ = new QPushButton("Update");
//!!  g2->addWidget(measur_label_ , 0, 0);
//!!  g2->addWidget(measur_lineed_, 0, 1);
//!!  g2->addWidget(measur_button_, 0, 2);
//!!
//!!  pickup_label_  = new QLabel("Pickup Position (x,y,z)");
//!!  pickup_lineed_ = new QLineEdit("0,0,0");
//!!  pickup_button_ = new QPushButton("Update");
//!!  g2->addWidget(pickup_label_ , 1, 0);
//!!  g2->addWidget(pickup_lineed_, 1, 1);
//!!  g2->addWidget(pickup_button_, 1, 2);
//!!
//!!  iteraN_label_  = new QLabel("N Iterations (int)");
//!!  iteraN_lineed_ = new QLineEdit("10");
//!!  g2->addWidget(iteraN_label_ , 2, 0);
//!!  g2->addWidget(iteraN_lineed_, 2, 1);
//!!
//!!  fl1->addRow(g2);
//!!
//!!  connect(exe_button_   , SIGNAL(clicked()), this, SLOT(execute()));
//!!  connect(measur_button_, SIGNAL(clicked()), this, SLOT(update_position_measurement()));
//!!  connect(pickup_button_, SIGNAL(clicked()), this, SLOT(update_position_pickup()));
//!!}
//!!
//!!void AssemblyMultiPickupTesterWidget::update_position_measurement()
//!!{
//!!  const double x = motion_manager_->get_position_X();
//!!  const double y = motion_manager_->get_position_Y();
//!!  const double z = motion_manager_->get_position_Z();
//!!
//!!  std::stringstream posi_strs;
//!!  posi_strs << x << ", " << y << ", " << z;
//!!
//!!  measur_lineed_->setText(QString::fromStdString(posi_strs.str()));
//!!
//!!  return;
//!!}
//!!
//!!void AssemblyMultiPickupTesterWidget::update_position_pickup()
//!!{
//!!  const double x = motion_manager_->get_position_X();
//!!  const double y = motion_manager_->get_position_Y();
//!!  const double z = motion_manager_->get_position_Z();
//!!
//!!  std::stringstream posi_strs;
//!!  posi_strs << x << ", " << y << ", " << z;
//!!
//!!  pickup_lineed_->setText(QString::fromStdString(posi_strs.str()));
//!!
//!!  return;
//!!}
//!!
//!!void AssemblyMultiPickupTesterWidget::enable(const bool b)
//!!{
//!!  exe_button_   ->setEnabled(b);
//!!
//!!  measur_button_->setEnabled(b);
//!!  pickup_button_->setEnabled(b);
//!!
//!!  measur_lineed_->setEnabled(b);
//!!  pickup_lineed_->setEnabled(b);
//!!  iteraN_lineed_->setEnabled(b);
//!!
//!!  return;
//!!}
//!!
//!!void AssemblyMultiPickupTesterWidget::execute()
//!!{
//!!  // measurement position
//!!  const QString measur_qstr = this->measur_lineed_->text().remove(" ");
//!!
//!!  const QStringList measur_qsl = measur_qstr.split(",");
//!!
//!!  if(measur_qsl.length() != 3)
//!!  {
//!!    NQLog("AssemblyMultiPickupTesterWidget", NQLog::Warning) << "execute"
//!!       << ": invalid format for measurement position (" << measur_qstr << "), no action taken";
//!!
//!!    this->enable(true);
//!!
//!!    return;
//!!  }
//!!
//!!  const double measur_x = measur_qsl.at(0).toDouble();
//!!  const double measur_y = measur_qsl.at(1).toDouble();
//!!  const double measur_z = measur_qsl.at(2).toDouble();
//!!
//!!  // pickup position
//!!  const QString pickup_qstr = this->pickup_lineed_->text().remove(" ");
//!!
//!!  const QStringList pickup_qsl = pickup_qstr.split(",");
//!!
//!!  if(pickup_qsl.length() != 3)
//!!  {
//!!    NQLog("AssemblyMultiPickupTesterWidget", NQLog::Warning) << "execute"
//!!       << ": invalid format for pick-up position (" << pickup_qstr << "), no action taken";
//!!
//!!    this->enable(true);
//!!
//!!    return;
//!!  }
//!!
//!!  const double pickup_x = pickup_qsl.at(0).toDouble();
//!!  const double pickup_y = pickup_qsl.at(1).toDouble();
//!!  const double pickup_z = pickup_qsl.at(2).toDouble();
//!!
//!!  // number of iterations
//!!  const QString iteraN_qstr = this->iteraN_lineed_->text();
//!!
//!!  const int iteraN = iteraN_qstr.toInt();
//!!
//!!  // output signal
//!!  NQLog("AssemblyMultiPickupTesterWidget", NQLog::Spam) << "execute"
//!!     << ": emitting signal \"multipickup_request"
//!!     <<  "(m_x=" << measur_x <<  ", m_y=" << measur_y << ", m_z=" << measur_z
//!!     << ", p_x=" << pickup_x <<  ", p_y=" << pickup_y << ", p_z=" << pickup_z
//!!     << ", n=" << iteraN << ")\"";
//!!
//!!  const AssemblyMultiPickupTester::Configuration mpt_conf
//!!  (
//!!    measur_x, measur_y, measur_z,
//!!    pickup_x, pickup_y, pickup_z,
//!!    iteraN
//!!  );
//!!
//!!  emit multipickup_request(mpt_conf);
//!!}
//!!// ===========================================================================
//!!
//!!AssemblyMoveWidget::AssemblyMoveWidget(const QString& label, const QString& default_entry, const bool move_relative, QWidget* parent) :
//!!  QWidget(parent),
//!!  moveRelative_(move_relative)
//!!{
//!!  layout_ = new QFormLayout(this);
//!!  this->setLayout(layout_);
//!!
//!!  button_ = new QPushButton(label);
//!!  button_->setStyleSheet(
//!!    "Text-align: left;"
//!!    "padding-left:   4px;"
//!!    "padding-right:  4px;"
//!!    "padding-top:    3px;"
//!!    "padding-bottom: 3px;"
//!!  );
//!!
//!!  lineed_ = new QLineEdit();
//!!  lineed_->setText(default_entry);
//!!
//!!  layout_->addRow(button_, lineed_);
//!!
//!!  connect(button_, SIGNAL(clicked()), this, SLOT(execute()));
//!!}
//!!
//!!QString AssemblyMoveWidget::get_input_string() const
//!!{
//!!  return this->lineed_->text();
//!!}
//!!
//!!void AssemblyMoveWidget::enable(const bool b)
//!!{
//!!  button_->setEnabled(b);
//!!  lineed_->setEnabled(b);
//!!
//!!  return;
//!!}
//!!
//!!void AssemblyMoveWidget::execute()
//!!{
//!!  QString line_entry = this->get_input_string();
//!!
//!!  // parse lineEdit text to get target coordinates
//!!  const QStringList entries = line_entry.remove(" ").split(",");
//!!
//!!  if((entries.length() == 4) || (moveRelative_ && (entries.length() == 3)))
//!!  {
//!!    const double x_d = entries.value(0).toDouble();
//!!    const double y_d = entries.value(1).toDouble();
//!!    const double z_d = entries.value(2).toDouble();
//!!
//!!    const double a_d = (entries.length() == 4) ? entries.value(3).toDouble() : 0.0;
//!!
//!!    if(moveRelative_)
//!!    {
//!!      NQLog("AssemblyMoveWidget", NQLog::Message) << "execute"
//!!         << ": emitting signal \"moveRelative("
//!!         << x_d << ", " << y_d << ", " << z_d << ", " << a_d << ")\"";
//!!
//!!      this->enable(false);
//!!
//!!      emit moveRelative(x_d, y_d, z_d, a_d);
//!!    }
//!!    else
//!!    {
//!!      NQLog("AssemblyMoveWidget", NQLog::Message) << "execute"
//!!         << ": emitting signal \"moveAbsolute("
//!!         << x_d << ", " << y_d << ", " << z_d << ", " << a_d << ")\"";
//!!
//!!      this->enable(false);
//!!
//!!      emit moveAbsolute(x_d, y_d, z_d, a_d);
//!!    }
//!!  }
//!!  else
//!!  {
//!!    NQLog("AssemblyMoveWidget", NQLog::Warning) << "execute"
//!!       << ": [moveRelative=" << moveRelative_ << "] invalid input string format (" << line_entry << "), no action taken";
//!!
//!!    return;
//!!  }
//!!
//!!  return;
//!!}
//!!// ===========================================================================
//!!
//!!AssemblyStringWidget::AssemblyStringWidget(const QString& label, const QString& default_entry, QWidget* parent) : QWidget(parent)
//!!{
//!!  layout_ = new QFormLayout(this);
//!!  this->setLayout(layout_);
//!!
//!!  button_ = new QPushButton(label, this);
//!!  button_->setStyleSheet(
//!!    "Text-align: left;"
//!!    "padding-left:   4px;"
//!!    "padding-right:  4px;"
//!!    "padding-top:    3px;"
//!!    "padding-bottom: 3px;"
//!!  );
//!!
//!!  lineed_ = new QLineEdit();
//!!  lineed_->setText(default_entry);
//!!
//!!  layout_->addRow(button_, lineed_);
//!!
//!!  connect(button_, SIGNAL(clicked()), this, SLOT(execute()));
//!!}
//!!
//!!QString AssemblyStringWidget::get_input_string() const
//!!{
//!!  return this->lineed_->text();
//!!}
//!!
//!!void AssemblyStringWidget::execute()
//!!{
//!!  const QString line_entry = this->get_input_string();
//!!
//!!  NQLog("AssemblyStringWidget", NQLog::Spam) << "execute"
//!!     << ": emitting signal \"input_string(" << line_entry << ")\"";
//!!
//!!  emit input_string(line_entry);
//!!}
//!!
// ===========================================================================

#include <AssemblyPositionsView.h>
#include <LStepExpressWidget.h>
#include <ApplicationConfig.h>
#include <nqlogger.h>
#include <Util.h>

#include <QFileDialog>
#include <QGridLayout>
#include <QFormLayout>
#include <QGroupBox>

AssemblyPositionsView::AssemblyPositionsView(const LStepExpressMotionManager* motion_manager, QWidget* parent) :
  QWidget(parent),
  motion_manager_(motion_manager)
{
    //// INITIALIZATION
    if(motion_manager_ == nullptr)
    {
      NQLog("AssemblyPositionsView", NQLog::Critical) << "initialization error"
         << ": null pointer to LStepExpressMotionManager object, exiting constructor";

      return;
    }
    //// --------------

    QFormLayout* l = new QFormLayout();
    this->setLayout(l);

    //// IMAGE
    QGroupBox* box_image = new QGroupBox(tr("Image"));

    QGridLayout* g1 = new QGridLayout();
    box_image->setLayout(g1);

    QPalette palette;
    palette.setColor(QPalette::Background, QColor(220, 220, 220));

    img_view_ = new AssemblyUEyeView();
    img_view_->setMinimumSize(500, 300);
    img_view_->setPalette(palette);
    img_view_->setBackgroundRole(QPalette::Background);
    img_view_->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    img_view_->setScaledContents(true);
    img_view_->setAlignment(Qt::AlignCenter);

    img_scroll_ = new QScrollArea(this);
    img_scroll_->setMinimumSize(500, 300);
    img_scroll_->setPalette(palette);
    img_scroll_->setBackgroundRole(QPalette::Background);
    img_scroll_->setAlignment(Qt::AlignCenter);
    img_scroll_->setWidget(img_view_);
    img_scroll_->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    g1->addWidget(img_scroll_, 0, 0); 

    this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    QFormLayout*  lImg = new QFormLayout();
    g1->addLayout(lImg, 0, 1);

    img_button_ = new QPushButton("Save Image");
    lImg->addRow(img_button_);

    cli_button_ = new QPushButton("Draw Center Lines");
    lImg->addRow(cli_button_);

    l->addRow(box_image);
    //// ---------------

    //// POSITION(S)
    QGroupBox* box_posit = new QGroupBox(tr("Positions"));

    QFormLayout* l2 = new QFormLayout();
    box_posit->setLayout(l2);

//    measur_label_ = new QLabel("Position (x,y,z)");

    w_pos1_ = new AssemblyPositionsWidget(motion_manager_); l2->addRow(w_pos1_);
    w_pos2_ = new AssemblyPositionsWidget(motion_manager_); l2->addRow(w_pos2_);
    w_pos3_ = new AssemblyPositionsWidget(motion_manager_); l2->addRow(w_pos3_);
    w_pos4_ = new AssemblyPositionsWidget(motion_manager_); l2->addRow(w_pos4_);
    w_pos5_ = new AssemblyPositionsWidget(motion_manager_); l2->addRow(w_pos5_);
    w_pos6_ = new AssemblyPositionsWidget(motion_manager_); l2->addRow(w_pos6_);
    w_pos7_ = new AssemblyPositionsWidget(motion_manager_); l2->addRow(w_pos7_);

    l->addRow(box_posit);
    //// ---------------

    //// connection(s)
    connect(img_button_, SIGNAL(clicked()), this, SLOT(save_image()));
    connect(cli_button_, SIGNAL(clicked()), this, SLOT(modify_image_centerlines()));

    this->connectImageProducer(this, SIGNAL(image_updated(cv::Mat)));
    //// ---------------
}

void AssemblyPositionsView::update_image(const cv::Mat& img)
{
  if(img.channels() == 1)
  {
    cv::Mat img_color;
    cv::cvtColor(img, img_color, cv::COLOR_GRAY2BGR);

    image_ = img_color.clone();
  }
  else
  {
    image_ = img.clone();
  }

  NQLog("AssemblyPositionsView", NQLog::Spam) << "update_image"
     << ": emitting signal \"image_updated\"";

  emit image_updated(image_);
}

void AssemblyPositionsView::save_image()
{
    if(image_.rows == 0)
    {
      NQLog("AssemblyPositionsView", NQLog::Warning) << "save_image"
         << ": input cv::Mat object has zero rows, no image saved";

      return;
    }

    QString filename = QFileDialog::getSaveFileName(this, "save image", ".", "*.png");
    if(filename.isNull() || filename.isEmpty()){ return; }

    if(!filename.endsWith(".png")){ filename += ".png"; }

    cv::imwrite(filename.toStdString(), image_);

    return;
}

void AssemblyPositionsView::modify_image_centerlines()
{
  if(image_.rows == 0)
  {
    NQLog("AssemblyPositionsView", NQLog::Warning) << "modify_image_centerlines"
       << ": input cv::Mat object has zero rows, no image saved";

    return;
  }

  cv::Mat img = image_.clone();

  line(img, cv::Point(   img.cols/2.0, 0), cv::Point(img.cols/2.0, img.rows    ), cv::Scalar(255,0,0), 2, 8, 0);
  line(img, cv::Point(0, img.rows/2.0   ), cv::Point(img.cols    , img.rows/2.0), cv::Scalar(255,0,0), 2, 8, 0);

  NQLog("AssemblyPositionsView", NQLog::Spam) << "modify_image_centerlines"
     << ": updating image";

  this->update_image(img);

  return;
}

void AssemblyPositionsView::connectImageProducer(const QObject* sender, const char* signal)
{
    NQLog("AssemblyPositionsView", NQLog::Debug) << "connectImageProducer";

    img_view_->connectImageProducer(sender, signal);
}

void AssemblyPositionsView::disconnectImageProducer(const QObject* sender, const char* signal)
{
    NQLog("AssemblyPositionsView", NQLog::Debug) << "disconnectImageProducer";

    img_view_->disconnectImageProducer(sender, signal);
}

void AssemblyPositionsView::keyReleaseEvent(QKeyEvent* event)
{
    if(!(event->modifiers() & Qt::ShiftModifier))
    {
      switch(event->key())
      {
        case Qt::Key_0:
//          img_view_->setZoomFactor(0.25);
            event->accept();
            break;

        case Qt::Key_1:
//            img_view_->setZoomFactor(1.00);
            event->accept();
            break;

        case Qt::Key_Plus:
//            img_view_->increaseZoomFactor();
            event->accept();
            break;

        case Qt::Key_Minus:
//            img_view_->decreaseZoomFactor();
            event->accept();
            break;

        default:
            break;
      }
    }
}

AssemblyPositionsWidget::AssemblyPositionsWidget(const LStepExpressMotionManager* motion_manager, QWidget* parent) :
  QWidget(parent),
  motion_manager_(motion_manager)
{
  //// INITIALIZATION
  if(motion_manager_ == nullptr)
  {
    NQLog("AssemblyPositionsWidget", NQLog::Critical) << "initialization error"
       << ": null pointer to LStepExpressMotionManager object, exiting constructor";

    return;
  }
  //// --------------

  QGridLayout* l = new QGridLayout();
  this->setLayout(l);

  pos_button_ = new QPushButton("Read");
  pos_lineed_value_ = new QLineEdit("");
  pos_lineed_comme_ = new QLineEdit("Comment here");
  l->addWidget(pos_button_      , 0, 0);
  l->addWidget(pos_lineed_value_, 0, 1);
  l->addWidget(pos_lineed_comme_, 0, 2);

  connect(pos_button_, SIGNAL(clicked()), this, SLOT(update_position()));

  NQLog("AssemblyPositionsWidget", NQLog::Debug) << "constructed";
}

void AssemblyPositionsWidget::update_position()
{
  const double x = motion_manager_->get_position_X();
  const double y = motion_manager_->get_position_Y();
  const double z = motion_manager_->get_position_Z();

  std::stringstream posi_strs;
  posi_strs << x << ", " << y << ", " << z;

  pos_lineed_value_->setText(QString::fromStdString(posi_strs.str()));

  return;
}
