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

#include <nqlogger.h>
#include <ApplicationConfig.h>

#include <AssemblyAssemblyView.h>
#include <AssemblyMotionWidgets.h>
#include <AssemblyUtilities.h>

#include <iostream>
#include <sstream>
#include <iomanip>
#include <vector>
#include <stdlib.h>

#include <QFormLayout>
#include <QFileDialog>
#include <QPushButton>
#include <QString>
#include <QStringList>
#include <QPixmap>
#include <QLabel>
#include <QApplication>
#include <QImageReader>
#include <QPainter>
#include <QScriptEngine>
#include <qnumeric.h>

#include <TGraph.h>
#include <TCanvas.h>

AssemblyAssemblyView::AssemblyAssemblyView(const LStepExpressMotionManager* motion_manager, QWidget* parent) :
  QWidget(parent),

  scrollArea_1_(0),
  scrollArea_2_(0),
  scrollArea_3_(0),
  scrollArea_4_(0),

  imageView_1_(0),
  imageView_2_(0),
  imageView_3_(0),
  imageView_4_(0),

  liedit_1_(0),
  liedit_2_(0),
  liedit_3_(0),
  liedit_4_(0),

  w_patrec_(0),
  w_mupiup_(0),

  objfinder_connected_(false)
{
  if(motion_manager == nullptr)
  {
    NQLog("AssemblyAssemblyView", NQLog::Critical)
       << "input error: null pointer to LStepExpressMotionManager object, exiting constructor";

    return;
  }

  QGridLayout* layout = new QGridLayout;
  this->setLayout(layout);

  // IMAGE VIEW(S) -------
  QGridLayout* g0 = new QGridLayout;
  layout->addLayout(g0, 0, 0);

  QPalette palette;
  palette.setColor(QPalette::Background, QColor(220, 220, 220));

  imageView_1_ = new AssemblyUEyeView(this);
  imageView_1_->setMinimumSize(300,300);
  imageView_1_->setPalette(palette);
  imageView_1_->setBackgroundRole(QPalette::Background);
  imageView_1_->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
  imageView_1_->setScaledContents(true);
  imageView_1_->setAlignment(Qt::AlignCenter);

  QApplication::processEvents();

  scrollArea_1_ = new QScrollArea(this);
  scrollArea_1_->setMinimumSize(200, 200);
  scrollArea_1_->setPalette(palette);
  scrollArea_1_->setBackgroundRole(QPalette::Background);
  scrollArea_1_->setAlignment(Qt::AlignCenter);
  scrollArea_1_->setWidget(imageView_1_);
  scrollArea_1_->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

  g0->addWidget(scrollArea_1_, 0, 0);
  liedit_1_ = new QLineEdit("Object Location = X,Z");
  g0->addWidget(liedit_1_, 1, 0);

  imageView_2_ = new AssemblyUEyeView(this);
  imageView_2_->setMinimumSize(200,200);
  imageView_2_->setPalette(palette);
  imageView_2_->setBackgroundRole(QPalette::Background);
  imageView_2_->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
  imageView_2_->setScaledContents(true);
  imageView_2_->setAlignment(Qt::AlignCenter);

  scrollArea_2_ = new QScrollArea(this);
  scrollArea_2_->setMinimumSize(200, 200);
  scrollArea_2_->setPalette(palette);
  scrollArea_2_->setBackgroundRole(QPalette::Background);
  scrollArea_2_->setAlignment(Qt::AlignCenter);
  scrollArea_2_->setWidget(imageView_2_);
  scrollArea_2_->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

  g0->addWidget(scrollArea_2_, 0, 1);
  liedit_2_ = new QLineEdit("Orientation = alpha");
  g0->addWidget(liedit_2_, 1, 1);

  imageView_3_ = new AssemblyUEyeView(this);
  imageView_3_->setMinimumSize(200, 200);
  imageView_3_->setPalette(palette);
  imageView_3_->setBackgroundRole(QPalette::Background);
  imageView_3_->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
  imageView_3_->setScaledContents(true);
  imageView_3_->setAlignment(Qt::AlignCenter);

  scrollArea_3_ = new QScrollArea(this);
  scrollArea_3_->setMinimumSize(200,200);
  scrollArea_3_->setPalette(palette);
  scrollArea_3_->setBackgroundRole(QPalette::Background);
  scrollArea_3_->setAlignment(Qt::AlignCenter);
  scrollArea_3_->setWidget(imageView_3_);
  scrollArea_3_->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

  g0->addWidget(scrollArea_3_, 2, 0);
//  liedit_3_ = new QLineEdit("Mntd pos. (cor. 2) = lE3,X");
//  g0->addWidget(liedit_3_, 1, 2);

  imageView_4_ = new AssemblyUEyeView(this);
  imageView_4_->setMinimumSize(200,200);
  imageView_4_->setPalette(palette);
  imageView_4_->setBackgroundRole(QPalette::Background);
  imageView_4_->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
  imageView_4_->setScaledContents(true);
  imageView_4_->setAlignment(Qt::AlignCenter);

  scrollArea_4_ = new QScrollArea(this);
  scrollArea_4_->setMinimumSize(200, 200);
  scrollArea_4_->setPalette(palette);
  scrollArea_4_->setBackgroundRole(QPalette::Background);
  scrollArea_4_->setAlignment(Qt::AlignCenter);
  scrollArea_4_->setWidget(imageView_4_);
  scrollArea_4_->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

  g0->addWidget(scrollArea_4_, 2, 1);
//  liedit_4_ = new QLineEdit("Dtchd pos. = ,X,X");
//  g0->addWidget(liedit_4_, 3, 0);
  // ---------------------

  // MULTI-PICKUP WIDGET -
  //   repeatitions of "pick-up + PatRec" procedure
  //   to measure module-misplacement introduced by the pick-up
  QFormLayout* f10 = new QFormLayout;
  layout->addLayout(f10, 1, 0);

  QGroupBox* box_mupiup = new QGroupBox(tr("\"Pickup + PatRec\" Iterations"));

    w_mupiup_ = new AssemblyMultiPickupTesterWidget("Run \"Pickup + PatRec\"", motion_manager);
    f10->addRow(w_mupiup_);

    connect(w_mupiup_, SIGNAL(multipickup_request(const AssemblyMultiPickupTester::Configuration&)),
            this     , SIGNAL(multipickup_request(const AssemblyMultiPickupTester::Configuration&)));

  box_mupiup->setLayout(w_mupiup_->layout());

  f10->addRow(box_mupiup);
  // ---------------------

  QFormLayout* f01 = new QFormLayout;
  layout->addLayout(f01, 0, 1);

  // PATREC  WIDGET ------
  QGroupBox* box_patrec = new QGroupBox(tr("Pattern Recognition"));

  w_patrec_ = new AssemblyPatRecWidget("Standalone PatRec", this);
  w_patrec_->setToolTip("(4) Runs Pattern Recognition routine to determine sensor (x,y,z,a) position");

  box_patrec->setLayout(w_patrec_->layout());

  f01->addRow(box_patrec);
  // ---------------------

  // ALIGNMENT WIDGET ----
  QFormLayout* f11 = new QFormLayout;
  layout->addLayout(f11, 1, 1);

  QGroupBox* box_alignm = new QGroupBox(tr("Alignment"));

  w_alignm_ = new AssemblyAlignmWidget(this);
  w_alignm_->setToolTip("(5) Aligns object to target angle (with respect to the x-axis of the motion stage)");

  box_alignm->setLayout(w_alignm_->layout());

  f11->addRow(box_alignm);
  // ---------------------
}

void AssemblyAssemblyView::connect_to_finder(const AssemblyObjectFinderPatRec* finder)
{
  if(finder == nullptr)
  {
    NQLog("AssemblyAssemblyView", NQLog::Critical) << "connect_to_finder"
       << ": null pointer to AssemblyObjectFinderPatRec object, no action taken";

    return;
  }

  if(objfinder_connected_ == false)
  {
    connect(this->PatRec_Widget(), SIGNAL(mode(int, int)), finder, SLOT(run_PatRec(int, int)));

    connect(finder, SIGNAL(PatRec_exitcode(int)), this->PatRec_Widget(), SLOT(change_label(int)));

    connect(this->PatRec_Widget()->widget_angrough(), SIGNAL(input_string(QString)), finder, SLOT(update_rough_angles      (QString)));
    connect(this->PatRec_Widget()->widget_angscanp(), SIGNAL(input_string(QString)), finder, SLOT(update_angscan_parameters(QString)));

    connect(this->PatRec_Widget(), SIGNAL(sendPosition(int, double, double, double)), this, SLOT(updateText(int, double, double, double)));

    connect(finder, SIGNAL(image_path          (int, QString))               , this, SLOT(updateImage(int, QString)));
    connect(finder, SIGNAL(image_mat           (int, cv::Mat))               , this, SLOT(updateImage(int, cv::Mat)));
    connect(finder, SIGNAL(reportObjectLocation(int, double, double, double)), this, SLOT(updateText (int, double, double, double)));

    NQLog("AssemblyAssemblyView", NQLog::Spam) << "connect_to_finder"
       << ": widget connected to object of type AssemblyObjectFinderPatRec";

    objfinder_connected_ = true;
  }
  else
  {
    NQLog("AssemblyAssemblyView", NQLog::Critical) << "connect_to_finder"
       << ": widget already connected to an object of type AssemblyObjectFinderPatRec, no action taken";

    return;
  }

  return;
}

void AssemblyAssemblyView::updateText(int stage, double x, double y, double a)
{
  NQLog("AssemblyAssemblyView", NQLog::Spam) << "updateText"
     << "(" << stage << ", " << x << ", " << y << ", " << a << ")";

  std::stringstream posi_strs;
  posi_strs << std::setprecision(3) << x << ", " << y;
  const QString posi_qstr = QString::fromStdString(posi_strs.str());

  std::stringstream orie_strs;
  orie_strs << std::setprecision(3) << a;
  const QString orie_qstr = QString::fromStdString(orie_strs.str());

  if(stage == 1)
  {
    liedit_1_->setText("Object location = "   +posi_qstr+" [pixel #, pixel #] (field of view)");
    liedit_2_->setText("Object orientation = "+orie_qstr+" degrees");
  }
}

void AssemblyAssemblyView::updateImage(const int stage, const QString& filename)
{
  NQLog("AssemblyAssemblyView", NQLog::Spam) << "updateImage(" << stage << ", file=" << filename << ")";

  const std::string filename_ss = filename.toUtf8().constData();

  const cv::Mat img = cv::imread(filename_ss, CV_LOAD_IMAGE_UNCHANGED);

  const QImageReader img_reader(QString::fromStdString(filename_ss));

  if(img_reader.format() == "png" && (img.channels() > 1))
  {
    cv::Mat img_1;
    cv::cvtColor(img, img_1, CV_RGB2BGR);

    this->updateImage(stage, img_1);
  }
  else
  {
    this->updateImage(stage, img);
  }
}

void AssemblyAssemblyView::updateImage(const int stage, const cv::Mat& img)
{
  NQLog("AssemblyAssemblyView", NQLog::Spam) << "updateImage(" << stage << ", image)";

  if(stage == 1)
  {
    imageView_1_->setImage(img);
//    imageView_1_->setZoomFactor(0.3);
  }
  else if(stage == 2)
  {
    imageView_2_->setImage(img);
    imageView_2_->setZoomFactor(0.5);
  }
  else if(stage == 3)
  {
    imageView_3_->setImage(img);
//    imageView_3_->setZoomFactor(0.5);
  }
  else if(stage == 4)
  {
    imageView_4_->setImage(img);
//    imageView_4_->setZoomFactor(0.5);
  }
}

void AssemblyAssemblyView::keyReleaseEvent(QKeyEvent* event)
{
  if(!(event->modifiers() & Qt::ShiftModifier))
  {
    switch(event->key())
    {
      case Qt::Key_0:
//        imageView_1->setZoomFactor(0.25);
        event->accept();
        break;

      case Qt::Key_1:
//        imageView_1->setZoomFactor(1.00);
        event->accept();
        break;

      case Qt::Key_Plus:
//        imageView_1->increaseZoomFactor();
        event->accept();
        break;

      case Qt::Key_Minus:
//        imageView_1->decreaseZoomFactor();
        event->accept();
        break;

      default:
        break;
    }
  }
}
// ===========================================================================

AssemblyStringWidget::AssemblyStringWidget(const QString& label, const QString& default_entry, QWidget* parent) : QWidget(parent)
{
  layout_ = new QFormLayout;
  this->setLayout(layout_);

  button_ = new QPushButton(label, this);
  button_->setStyleSheet(
    "Text-align: left;"
    "padding-left:   4px;"
    "padding-right:  4px;"
    "padding-top:    3px;"
    "padding-bottom: 3px;"
  );

  lineed_ = new QLineEdit(this);
  lineed_->setText(default_entry);

  layout_->addRow(button_, lineed_);

  connect(button_, SIGNAL(clicked()), this, SLOT(execute()));
}

QString AssemblyStringWidget::get_input_string() const
{
  return this->lineed_->text();
}

void AssemblyStringWidget::execute()
{
  const QString line_entry = this->get_input_string();

  NQLog("AssemblyStringWidget", NQLog::Spam) << "execute"
     << ": emitting signal \"input_string(" << line_entry << ")\"";

  emit input_string(line_entry);
}
// ===========================================================================

AssemblyPatRecWidget::AssemblyPatRecWidget(const QString& label, QWidget* parent) :
  QWidget(parent),

  layout_(0),
  button_(0),
  lineed_(0),
  label_ (0),

  groupBox1_(0),
  groupBox2_(0),

  radio1_(0),
  radio2_(0),
  radio3_(0),
  radio4_(0),
  radio5_(0),
  radio6_(0),

  vbox1_(0),
  vbox2_(0),

  sw_angrough_(0),
  sw_angscanp_(0)
{
  layout_ = new QFormLayout;
  this->setLayout(layout_);

  QGridLayout* layout_1 = new QGridLayout;
  layout_->addRow(layout_1);

  button_ = new QPushButton(label, this);
  layout_1->addWidget(button_, 0, 0);

  groupBox1_ = new QGroupBox(tr("Object"));
  groupBox2_ = new QGroupBox(tr("Mode"));

  radio1_ = new QRadioButton(tr("&Fiducial marker"), this);
  radio2_ = new QRadioButton(tr("&Positioning pin"), this);
  radio3_ = new QRadioButton(tr("&Sensor corner")  , this);
  radio4_ = new QRadioButton(tr("&Spacer corner")  , this);

  radio1_->setChecked(true);

  vbox1_ = new QVBoxLayout;
  vbox1_->addWidget(radio1_);
  vbox1_->addWidget(radio2_);
  vbox1_->addWidget(radio3_);
  vbox1_->addWidget(radio4_);
  vbox1_->addStretch(1);

  groupBox1_->setLayout(vbox1_);

  radio5_ = new QRadioButton(tr("&Demo"), this);
  radio6_ = new QRadioButton(tr("&Lab") , this);

  radio5_->setChecked(true);

  vbox2_ = new QVBoxLayout;
  vbox2_->addWidget(radio5_);
  vbox2_->addWidget(radio6_);
  vbox2_->addStretch(1);

  groupBox2_->setLayout(vbox2_);

  layout_1->addWidget(groupBox1_, 1, 0);
  layout_1->addWidget(groupBox2_, 1, 1);

  QPixmap pixmap(100, 100);
  pixmap.fill(QColor("transparent"));

  label_ = new QLabel("", this);
  label_->setPixmap(pixmap);
  label_->setText(" WAITING");
  label_->setStyleSheet("QLabel { background-color : orange; color : black; }");

  layout_1->addWidget(label_, 0, 1);

  // PatRec: angular analysis configuration
  QGridLayout* layout_2 = new QGridLayout;
  layout_->addRow(layout_2);

  // widget: PatRec rough angles
  sw_angrough_ = new AssemblyStringWidget("Pre-Scan Angles (list)", "0,180", this);
  layout_2->addWidget(sw_angrough_->button(), 0, 0);
  layout_2->addWidget(sw_angrough_->lineed(), 0, 1);

  // widget: PatRec angular-scan parameters
  sw_angscanp_ = new AssemblyStringWidget("Scan Params (fine-range, fine-step)", "5,0.125", this);
  layout_2->addWidget(sw_angscanp_->button(), 1, 0);
  layout_2->addWidget(sw_angscanp_->lineed(), 1, 1);
  // ---------------------

  connect(button_, SIGNAL(clicked()), this, SLOT(execute()));
}

void AssemblyPatRecWidget::execute()
{
  int mode_lab(0), mode_obj(0);

  if     (radio1_->isChecked()){ mode_obj = 0; }
  else if(radio2_->isChecked()){ mode_obj = 1; }
  else if(radio3_->isChecked()){ mode_obj = 2; }
  else if(radio4_->isChecked()){ mode_obj = 3; }

  if     (radio5_->isChecked()){ mode_lab = 0; }
  else if(radio6_->isChecked()){ mode_lab = 1; }

  NQLog("AssemblyPatRecWidget", NQLog::Spam) << "execute"
     << ": emitting signal \"mode(" << mode_lab << ", " << mode_obj << ")\"";

  emit mode(mode_lab, mode_obj);
}

void AssemblyPatRecWidget::change_label(const int state)
{
  NQLog("AssemblyPatRecWidget", NQLog::Spam) << "change_label(" << state << ")";

  if(state == 0)
  {
    label_->setText(" FOUND MARKER");
    label_->setStyleSheet("QLabel { background-color : green; color : black; }");
  }
  else if(state == 2)
  {
    label_->setText(" ERROR");
    label_->setStyleSheet("QLabel { background-color : red; color : black; }");
  }

  return;
}
// ===========================================================================

AssemblyAlignmWidget::AssemblyAlignmWidget(QWidget *parent) :
  QWidget(parent)
{
  QGridLayout* g_lay = new QGridLayout;
  this->setLayout(g_lay);

  measur_button_ = new QPushButton("Measure Angle", this);
  alignm_button_ = new QPushButton("Align Object" , this);

  g_lay->addWidget(measur_button_, 0, 0);
  g_lay->addWidget(alignm_button_, 0, 1);

  connect(alignm_button_, SIGNAL(clicked()), this, SLOT(align_object()));
  connect(measur_button_, SIGNAL(clicked()), this, SLOT(measure_angle()));

  obj_deltaX_label_  = new QLabel("Obj. Delta-X [mm]", this);
  obj_deltaX_lineed_ = new QLineEdit("", this);
  g_lay->addWidget(obj_deltaX_label_ , 1, 0);
  g_lay->addWidget(obj_deltaX_lineed_, 1, 1);

  obj_deltaY_label_  = new QLabel("Obj. Delta-Y [mm]", this);
  obj_deltaY_lineed_ = new QLineEdit("", this);
  g_lay->addWidget(obj_deltaY_label_ , 2, 0);
  g_lay->addWidget(obj_deltaY_lineed_, 2, 1);

  ang_target_label_  = new QLabel("Target Angle [deg]", this);
  ang_target_lineed_ = new QLineEdit("", this);
  g_lay->addWidget(ang_target_label_ , 3, 0);
  g_lay->addWidget(ang_target_lineed_, 3, 1);

  obj_angle_label_  = new QLabel("Object Angle [deg]", this);
  obj_angle_lineed_ = new QLineEdit("", this);
  g_lay->addWidget(obj_angle_label_ , 4, 0);
  g_lay->addWidget(obj_angle_lineed_, 4, 1);

  obj_angle_lineed_->setReadOnly(true);

  this->enable(false);
}

void AssemblyAlignmWidget::measure_angle()
{
  this->execute(true);
}

void AssemblyAlignmWidget::align_object()
{
  this->execute(false);
}

void AssemblyAlignmWidget::execute(const bool meas_only_mode)
{
  // Object Delta-X
  const QString obj_deltaX_qstr = this->obj_deltaX_lineed_->text();

  bool valid_obj_deltaX(false);

  const double obj_deltaX_val = obj_deltaX_qstr.toDouble(&valid_obj_deltaX);

  if(!valid_obj_deltaX)
  {
    NQLog("AssemblyAlignmWidget", NQLog::Warning) << "execute"
       << ": invalid format for object delta-X distance (" << obj_deltaX_qstr << "), no action taken";

    return;
  }
  // ---------------

  // Object Delta-Y
  const QString obj_deltaY_qstr = this->obj_deltaY_lineed_->text();

  bool valid_obj_deltaY(false);

  const double obj_deltaY_val = obj_deltaY_qstr.toDouble(&valid_obj_deltaY);

  if(!valid_obj_deltaY)
  {
    NQLog("AssemblyAlignmWidget", NQLog::Warning) << "execute"
       << ": invalid format for object delta-Y distance (" << obj_deltaY_qstr << "), no action taken";

    return;
  }
  // ---------------

  if(meas_only_mode)
  {
    // disable widget
    this->enable(false);

    // signal
    NQLog("AssemblyAlignmWidget", NQLog::Spam) << "execute"
      << ": emitting signal \"measure_angle_request("
      << obj_deltaX_val << ", "
      << obj_deltaY_val
      << ")\"";

    emit measure_angle_request(obj_deltaX_val, obj_deltaY_val);
    // ---------------
  }
  else
  {
    // Target Angle
    const QString ang_target_qstr = this->ang_target_lineed_->text();

    QScriptEngine qscr_engine;
    const double ang_target_val = qscr_engine.evaluate(ang_target_qstr).toNumber();

    const bool valid_ang_target = qIsFinite(ang_target_val);

    if(!valid_ang_target)
    {
      NQLog("AssemblyAlignmWidget", NQLog::Warning) << "execute"
         << ": invalid format for target angle (" << ang_target_qstr << "), no action taken";

      return;
    }
    // ---------------

    // disable widget
    this->enable(false);

    // signal
    NQLog("AssemblyAlignmWidget", NQLog::Spam) << "alignment_request"
      << ": emitting signal \"alignment_request("
      << obj_deltaX_val << ", "
      << obj_deltaY_val << ", "
      << ang_target_val
      << ")\"";

    emit alignment_request(obj_deltaX_val, obj_deltaY_val, ang_target_val);
    // ---------------
  }
}

void AssemblyAlignmWidget::enable(const bool b)
{
  measur_button_->setEnabled(b);
  alignm_button_->setEnabled(b);

  const QString label_color = b ? "black" : "gray";

  obj_deltaX_label_->setStyleSheet("QLabel { color : "+label_color+"; }");
  obj_deltaY_label_->setStyleSheet("QLabel { color : "+label_color+"; }");
  ang_target_label_->setStyleSheet("QLabel { color : "+label_color+"; }");
  obj_angle_label_ ->setStyleSheet("QLabel { color : "+label_color+"; }");

  obj_deltaX_lineed_->setEnabled(b);
  obj_deltaY_lineed_->setEnabled(b);
  ang_target_lineed_->setEnabled(b);
  obj_angle_lineed_ ->setEnabled(b);

  return;
}

void AssemblyAlignmWidget::show_object_angle(const double val)
{
  std::stringstream posi_strs;
  posi_strs << val;

  obj_angle_lineed_->setText(QString::fromStdString(posi_strs.str()));

  return;
}
// ===========================================================================

AssemblyMultiPickupTesterWidget::AssemblyMultiPickupTesterWidget(const QString& label, const LStepExpressMotionManager* motion_manager, QWidget* parent) :
  QWidget(parent),
  motion_manager_(motion_manager)
{
  if(!motion_manager_)
  {
    NQLog("AssemblyMultiPickupTesterWidget", NQLog::Critical)
       << "input error: null pointer to LStepExpressMotionManager object, exiting constructor";

    return;
  }

  layout_ = new QGridLayout;
  this->setLayout(layout_);

  exe_button_ = new QPushButton(label, this);
  layout_->addWidget(exe_button_, 0, 0);

  QFormLayout* fl1 = new QFormLayout;
  layout_->addLayout(fl1, 1, 0);

  QGridLayout* g2 = new QGridLayout;

  measur_label_  = new QLabel("Measurement Position (x,y,z)", this);
  measur_lineed_ = new QLineEdit("0,0,0", this);
  measur_button_ = new QPushButton("Update", this);
  g2->addWidget(measur_label_ , 0, 0);
  g2->addWidget(measur_lineed_, 0, 1);
  g2->addWidget(measur_button_, 0, 2);

  pickup_label_  = new QLabel("Pickup Position (x,y,z)", this);
  pickup_lineed_ = new QLineEdit("0,0,0", this);
  pickup_button_ = new QPushButton("Update", this);
  g2->addWidget(pickup_label_ , 1, 0);
  g2->addWidget(pickup_lineed_, 1, 1);
  g2->addWidget(pickup_button_, 1, 2);

  iteraN_label_  = new QLabel("N Iterations (int)", this);
  iteraN_lineed_ = new QLineEdit("10", this);
  g2->addWidget(iteraN_label_ , 2, 0);
  g2->addWidget(iteraN_lineed_, 2, 1);

  fl1->addRow(g2);

  connect(exe_button_   , SIGNAL(clicked()), this, SLOT(execute()));
  connect(measur_button_, SIGNAL(clicked()), this, SLOT(update_position_measurement()));
  connect(pickup_button_, SIGNAL(clicked()), this, SLOT(update_position_pickup()));
}

void AssemblyMultiPickupTesterWidget::update_position_measurement()
{
  const double x = motion_manager_->get_position_X();
  const double y = motion_manager_->get_position_Y();
  const double z = motion_manager_->get_position_Z();

  std::stringstream posi_strs;
  posi_strs << x << ", " << y << ", " << z;

  measur_lineed_->setText(QString::fromStdString(posi_strs.str()));

  return;
}

void AssemblyMultiPickupTesterWidget::update_position_pickup()
{
  const double x = motion_manager_->get_position_X();
  const double y = motion_manager_->get_position_Y();
  const double z = motion_manager_->get_position_Z();

  std::stringstream posi_strs;
  posi_strs << x << ", " << y << ", " << z;

  pickup_lineed_->setText(QString::fromStdString(posi_strs.str()));

  return;
}

void AssemblyMultiPickupTesterWidget::enable(const bool b)
{
  exe_button_   ->setEnabled(b);

  measur_button_->setEnabled(b);
  pickup_button_->setEnabled(b);

  measur_lineed_->setEnabled(b);
  pickup_lineed_->setEnabled(b);
  iteraN_lineed_->setEnabled(b);

  return;
}

void AssemblyMultiPickupTesterWidget::execute()
{
  // measurement position
  const QString measur_qstr = this->measur_lineed_->text().remove(" ");

  const QStringList measur_qsl = measur_qstr.split(",");

  if(measur_qsl.length() != 3)
  {
    NQLog("AssemblyMultiPickupTesterWidget", NQLog::Warning) << "execute"
       << ": invalid format for measurement position (" << measur_qstr << "), no action taken";

    this->enable(true);

    return;
  }

  bool valid_measur_x(false);
  bool valid_measur_y(false);
  bool valid_measur_z(false);

  const double measur_x = measur_qsl.at(0).toDouble(&valid_measur_x);
  const double measur_y = measur_qsl.at(1).toDouble(&valid_measur_y);
  const double measur_z = measur_qsl.at(2).toDouble(&valid_measur_z);

  if(!valid_measur_x)
  {
    NQLog("AssemblyMultiPickupTesterWidget", NQLog::Warning) << "execute"
       << ": invalid format for x-axis value of measurement position (" << measur_qsl.at(0) << "), no action taken";

    this->enable(true);

    return;
  }
  else if(!valid_measur_y)
  {
    NQLog("AssemblyMultiPickupTesterWidget", NQLog::Warning) << "execute"
       << ": invalid format for y-axis value of measurement position (" << measur_qsl.at(1) << "), no action taken";

    this->enable(true);

    return;
  }
  else if(!valid_measur_z)
  {
    NQLog("AssemblyMultiPickupTesterWidget", NQLog::Warning) << "execute"
       << ": invalid format for z-axis value of measurement position (" << measur_qsl.at(2) << "), no action taken";

    this->enable(true);

    return;
  }

  // pickup position
  const QString pickup_qstr = this->pickup_lineed_->text().remove(" ");

  const QStringList pickup_qsl = pickup_qstr.split(",");

  if(pickup_qsl.length() != 3)
  {
    NQLog("AssemblyMultiPickupTesterWidget", NQLog::Warning) << "execute"
       << ": invalid format for pick-up position (" << pickup_qstr << "), no action taken";

    this->enable(true);

    return;
  }

  bool valid_pickup_x(false);
  bool valid_pickup_y(false);
  bool valid_pickup_z(false);

  const double pickup_x = pickup_qsl.at(0).toDouble(&valid_pickup_x);
  const double pickup_y = pickup_qsl.at(1).toDouble(&valid_pickup_y);
  const double pickup_z = pickup_qsl.at(2).toDouble(&valid_pickup_z);

  if(!valid_pickup_x)
  {
    NQLog("AssemblyMultiPickupTesterWidget", NQLog::Warning) << "execute"
       << ": invalid format for x-axis value of pickup position (" << pickup_qsl.at(0) << "), no action taken";

    this->enable(true);

    return;
  }
  else if(!valid_pickup_y)
  {
    NQLog("AssemblyMultiPickupTesterWidget", NQLog::Warning) << "execute"
       << ": invalid format for y-axis value of pickup position (" << pickup_qsl.at(1) << "), no action taken";

    this->enable(true);

    return;
  }
  else if(!valid_pickup_z)
  {
    NQLog("AssemblyMultiPickupTesterWidget", NQLog::Warning) << "execute"
       << ": invalid format for z-axis value of pickup position (" << pickup_qsl.at(2) << "), no action taken";

    this->enable(true);

    return;
  }

  // number of iterations
  const QString iteraN_qstr = this->iteraN_lineed_->text();

  bool valid_iteraN(false);

  const int iteraN = iteraN_qstr.toInt(&valid_iteraN);

  if(!valid_iteraN)
  {
    NQLog("AssemblyMultiPickupTesterWidget", NQLog::Warning) << "execute"
       << ": invalid format for number of iterations (" << iteraN_qstr << "), no action taken";

    this->enable(true);

    return;
  }

  // output signal
  NQLog("AssemblyMultiPickupTesterWidget", NQLog::Spam) << "execute"
     << ": emitting signal \"multipickup_request"
     <<  "(m_x=" << measur_x <<  ", m_y=" << measur_y << ", m_z=" << measur_z
     << ", p_x=" << pickup_x <<  ", p_y=" << pickup_y << ", p_z=" << pickup_z
     << ", n=" << iteraN << ")\"";

  const AssemblyMultiPickupTester::Configuration mpt_conf(
    measur_x, measur_y, measur_z,
    pickup_x, pickup_y, pickup_z,
    iteraN
  );

  emit multipickup_request(mpt_conf);
}
// ===========================================================================
