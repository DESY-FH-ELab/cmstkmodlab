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

#include <AssemblyModuleAssembler.h>
#include <LStepExpressWidget.h>
#include <ApplicationConfig.h>
#include <nqlogger.h>
#include <Util.h>

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

#include <TGraph.h>
#include <TCanvas.h>

using namespace cv;

AssemblyModuleAssembler::AssemblyModuleAssembler(LStepExpressMotionManager* motion_manager, MarkerFinderPatRec* finder, QWidget* parent) :
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

  w_vacuum_(0)
{
  if(!motion_manager)
  {
    NQLog("AssemblyModuleAssembler", NQLog::Critical)
       << "input error: null pointer to LStepExpressMotionManager object, initialization stopped";

    return;
  }

  if(!finder)
  {
    NQLog("AssemblyModuleAssembler", NQLog::Critical)
       << "input error: null pointer to MarkerFinderPatRec object, initialization stopped";

    return;
  }

  QGridLayout *l = new QGridLayout();
  setLayout(l);

  QGridLayout *g0 = new QGridLayout();
  l->addLayout(g0, 0, 0);

  QPalette palette;
  palette.setColor(QPalette::Background, QColor(220, 220, 220));

  imageView_1_ = new AssemblyUEyeView();
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

  imageView_2_ = new AssemblyUEyeView();
  imageView_2_->setMinimumSize(200,200);
  imageView_2_->setPalette(palette);
  imageView_2_->setBackgroundRole(QPalette::Background);
  imageView_2_->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
  imageView_2_->setScaledContents(true);
  imageView_2_->setAlignment(Qt::AlignCenter);

  scrollArea_2_ = new QScrollArea();
  scrollArea_2_->setMinimumSize(200, 200);
  scrollArea_2_->setPalette(palette);
  scrollArea_2_->setBackgroundRole(QPalette::Background);
  scrollArea_2_->setAlignment(Qt::AlignCenter);
  scrollArea_2_->setWidget(imageView_2_);
  scrollArea_2_->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

  g0->addWidget(scrollArea_2_, 0, 1);
  liedit_2_ = new QLineEdit("Orientation = alpha");
  g0->addWidget(liedit_2_, 1, 1);

  imageView_3_ = new AssemblyUEyeView();
  imageView_3_->setMinimumSize(200, 200);
  imageView_3_->setPalette(palette);
  imageView_3_->setBackgroundRole(QPalette::Background);
  imageView_3_->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
  imageView_3_->setScaledContents(true);
  imageView_3_->setAlignment(Qt::AlignCenter);

  scrollArea_3_ = new QScrollArea();
  scrollArea_3_->setMinimumSize(200,200);
  scrollArea_3_->setPalette(palette);
  scrollArea_3_->setBackgroundRole(QPalette::Background);
  scrollArea_3_->setAlignment(Qt::AlignCenter);
  scrollArea_3_->setWidget(imageView_3_);
  scrollArea_3_->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

  g0->addWidget(scrollArea_3_, 2, 0);
//  liedit_3_ = new QLineEdit("Mntd pos. (cor. 2) = lE3,X");
//  g0->addWidget(liedit_3_, 1, 2);

  imageView_4_ = new AssemblyUEyeView();
  imageView_4_->setMinimumSize(200,200);
  imageView_4_->setPalette(palette);
  imageView_4_->setBackgroundRole(QPalette::Background);
  imageView_4_->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
  imageView_4_->setScaledContents(true);
  imageView_4_->setAlignment(Qt::AlignCenter);

  scrollArea_4_ = new QScrollArea();
  scrollArea_4_->setMinimumSize(200, 200);
  scrollArea_4_->setPalette(palette);
  scrollArea_4_->setBackgroundRole(QPalette::Background);
  scrollArea_4_->setAlignment(Qt::AlignCenter);
  scrollArea_4_->setWidget(imageView_4_);
  scrollArea_4_->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

  g0->addWidget(scrollArea_4_, 2, 1);
//  liedit_4_ = new QLineEdit("Dtchd pos. = ,X,X");
//  g0->addWidget(liedit_4_, 3, 0);

  QGridLayout* g1 = new QGridLayout();
  l->addLayout(g1, 0, 1);

  // widget: move absolute
  MoveWidget*   w_moveabs = new MoveWidget("Move Absolute", "0,0,0");
  g1->addWidget(w_moveabs, 0, 0);

  w_moveabs->useMoveRelative(false);
  w_moveabs->setToolTip("(1) Moves x,y,z,a stage using moveAbsolute routine (with respect to origin)");

  connect(w_moveabs, SIGNAL(moveAbsolute(double, double, double, double)), motion_manager, SLOT(moveAbsolute(double, double, double, double)));
  // ---------------------

  // widget: move relative
  MoveWidget*   w_moverel = new MoveWidget("Move Relative", "0,0,0");
  g1->addWidget(w_moverel, 1, 0);

  w_moverel->useMoveRelative(true);
  w_moverel->setToolTip("(2) Moves x,y,z,a stage using moveRelative routine (with respect to current position)");

  connect(w_moverel, SIGNAL(moveRelative(double, double, double, double)), motion_manager, SLOT(moveRelative(double, double, double, double)));
  // ---------------------

  // widget: Pattern Recognition
  PatRecWidget* w_patrec = new PatRecWidget("Standalone Pattern Recognition", this);
  w_patrec->setToolTip("(3) Runs PatRec routine to deduce and report sensor (x,y,z,phi) position");
  g1->addWidget(w_patrec, 2, 0);

  connect(w_patrec, SIGNAL(mode(int, int))       , finder  , SLOT(run_PatRec(int, int)));
  connect(finder  , SIGNAL(PatRec_exitcode(int)) , w_patrec, SLOT(change_label(int)));

  finder->update_rough_angles      ( w_patrec->widget_angrough()->get_input_string() );
  finder->update_angscan_parameters( w_patrec->widget_angscanp()->get_input_string() );

  connect(w_patrec->widget_angrough(), SIGNAL(input_string(QString)), finder, SLOT(update_rough_angles      (QString)));
  connect(w_patrec->widget_angscanp(), SIGNAL(input_string(QString)), finder, SLOT(update_angscan_parameters(QString)));

  connect(w_patrec, SIGNAL(sendPosition        (int, double, double, double)), this, SLOT(updateText (int, double, double, double)));
  connect(finder  , SIGNAL(image_path          (int, QString))               , this, SLOT(updateImage(int, QString)));
  connect(finder  , SIGNAL(image_mat           (int, cv::Mat))               , this, SLOT(updateImage(int, cv::Mat)));
  connect(finder  , SIGNAL(reportObjectLocation(int, double, double, double)), this, SLOT(updateText (int, double, double, double)));
  // ---------------------

  // widget: vacuum
  w_vacuum_ = new VacuumWidget("Toggle Vacuum", this);
  g1->addWidget(w_vacuum_, 3, 0);
  // ---------------------

//!!  AssemblyPrecisionEstimator * precision1 = new AssemblyPrecisionEstimator(this, "Estimate Assembly Precision", "-200.0,0.0,0.0", "0.0,0.0,0.0", 1 , cnrd1);
//!!//  g1->addWidget(precision1,4,0);
//!!
//!!  connect(precision1, SIGNAL(launchPrecisionEstimation(double, double, double, double, double, double, int)),
//!!          this      , SLOT  (startMacro               (double, double, double, double, double, double, int)));

  // SandwichAssembler
  QGridLayout* g2 = new QGridLayout();
  l->addLayout(g2, 1, 1);

  AssemblySandwichAssembler* sandwich1 = new AssemblySandwichAssembler(this, "Assemble Sandwich");
  g2->addWidget(sandwich1, 0, 0);

  connect(sandwich1, SIGNAL(launchSandwichAssembly(double, double, double, double, double, double, double, double, double)),
          this     , SIGNAL(launchSandwichAssembly(double, double, double, double, double, double, double, double, double)));

  connect(sandwich1, SIGNAL(launchAlignment(int, double, double, double)),
          this     , SIGNAL(launchAlignment(int, double, double, double)));
  // ---------------------
}

void AssemblyModuleAssembler::updateText(int stage, double x, double y, double a)
{
  NQLog("AssemblyModuleAssembler", NQLog::Debug) << "updateText"
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

void AssemblyModuleAssembler::startMacro(double x_meas, double y_meas, double z_meas, double x_pickup, double y_pickup, double z_pickup, int iterations)
{
    NQLog("AssemblyModuleAssembler::startMacro");
    
    emit launchPrecisionEstimation(x_meas,  y_meas,  z_meas,  x_pickup,  y_pickup, z_pickup, iterations);
}

void AssemblyModuleAssembler::updateImage(const int stage, const QString& filename)
{
  NQLog("AssemblyModuleAssembler", NQLog::Debug) << "updateImage(" << stage << ", file=" << filename << ")";

  const std::string filename_ss = filename.toUtf8().constData();

  const cv::Mat img_gs = cv::imread(filename_ss, CV_LOAD_IMAGE_UNCHANGED);

  const QImageReader img_reader(QString::fromStdString(filename_ss));

  if(img_reader.format() == "png")
  {
    cv::Mat img_1;
    cv::cvtColor(img_gs, img_1, CV_RGB2BGR);

    this->updateImage(stage, img_1);
  }
  else
  {
    this->updateImage(stage, img_gs);
  }
}

void AssemblyModuleAssembler::updateImage(const int stage, const cv::Mat& img)
{
  NQLog("AssemblyModuleAssembler", NQLog::Debug) << "updateImage(" << stage << ", image)";

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

void AssemblyModuleAssembler::keyReleaseEvent(QKeyEvent * event)
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

AssemblyPrecisionEstimator::AssemblyPrecisionEstimator(QWidget *parent, std::string text, std::string measurement_position, std::string pickup_position, int /* iterations */, ConradModel* /* cnrd1 */)
: QWidget(parent)
{
    QGridLayout *l = new QGridLayout(this);
    setLayout(l);
    
    QString q_pu_pos = QString::fromStdString(pickup_position);
    QString q_meas_pos = QString::fromStdString(measurement_position);
    QString qname = QString::fromStdString(text);

    button1 = new QPushButton(qname, this);
    l->addWidget(button1,0,0);
    
    QFormLayout *fl1 = new QFormLayout();
    l->addLayout(fl1,1,0);

    label1 = new QLabel();
    lineEdit1 = new QLineEdit();
    fl1->addRow(label1,lineEdit1);

    label2 = new QLabel();
    lineEdit2 = new QLineEdit();
    fl1->addRow(label2,lineEdit2);
    
    label3 = new QLabel();
    lineEdit3 = new QLineEdit();
    fl1->addRow(label3,lineEdit3);
    

    label1->setText("Measurement position (x,y,z)");
    label2->setText("Pickup position (x,y,z)");
    label3->setText("N iterations");
    
    
    lineEdit1->setText("48.9792,58.0699,-88.1536");
    lineEdit2->setText("-39.6346,17.4785,-119.1284,0.0");
    lineEdit3->setText("1");

    connect(button1, SIGNAL(clicked()), this, SLOT(run()));

    return;
}

void AssemblyPrecisionEstimator::recordPosition(double /* x */, double /* y */, double /* theta */)
{
  NQLog("AssemblyPrecisionEstimator::recordPosition");

  return;
}

void AssemblyPrecisionEstimator::run()
{
    
    NQLog("AssemblyPrecisionEstimator") << ":run";
    
    
    
    //parse lineEdit text to get target coordinates
    QString  parent_string_meas = this->lineEdit1->text();
    
    QStringList pieces_meas = parent_string_meas.split( "," );
    QString x_meas_s = pieces_meas.value( pieces_meas.length() - 3);
    QString y_meas_s = pieces_meas.value( pieces_meas.length() - 2);
    QString z_meas_s = pieces_meas.value( pieces_meas.length() -1);
    
    double x_meas_d = x_meas_s.toDouble();
    double y_meas_d = y_meas_s.toDouble();
    double z_meas_d = z_meas_s.toDouble();
    
    
    QString  parent_string_pickup = this->lineEdit2->text();
    
    QStringList pieces_pickup = parent_string_pickup.split( "," );
    QString x_pickup_s = pieces_pickup.value( pieces_pickup.length() - 3);
    QString y_pickup_s = pieces_pickup.value( pieces_pickup.length() - 2);
    QString z_pickup_s = pieces_pickup.value( pieces_pickup.length() - 1);
    
    double x_pickup_d = x_pickup_s.toDouble();
    double y_pickup_d = y_pickup_s.toDouble();
    double z_pickup_d = z_pickup_s.toDouble();
    
    
    QString  parent_string_iterations = this->lineEdit3->text();
    
    int iterations_i = parent_string_iterations.toInt();
    
    NQLog("AssemblyPrecisionEstimator::run")<<  " x_m = " << x_meas_d <<  " y_m = " << y_meas_d << " z_m = " << z_meas_d << " x_p = " << x_pickup_d  << " y_p = " << y_pickup_d  <<" z_p = " << z_pickup_d <<  " iterations = " << iterations_i;


    emit launchPrecisionEstimation(x_meas_d, y_meas_d, z_meas_d, x_pickup_d, y_pickup_d, z_pickup_d, iterations_i );
    
}
// ===========================================================================

AssemblySandwichAssembler::AssemblySandwichAssembler(QWidget *parent, std::string text, std::string /* assembly_position */, std::string /* bottom_part_position */, std::string /* top_part_position */)
: QWidget(parent)
{
    QGridLayout *l = new QGridLayout(this);
    setLayout(l);
    
    //QString q_assembly_pos = QString::fromStdString(assembly_pickup);
    //QString q_bottom_pos = QString::fromStdString(bottom_part_position);
    //QString q_top_pos = QString::fromStdString(bottom_part_position);
    QString qname = QString::fromStdString(text);

    button0 = new QPushButton("Align object", this);
    l->addWidget(button0,0,0);
    
    button1 = new QPushButton(qname, this);
    l->addWidget(button1,1,0);
    
    QFormLayout *fl1 = new QFormLayout();
    l->addLayout(fl1,2,0);

    label1 = new QLabel();
    lineEdit1 = new QLineEdit();
    fl1->addRow(label1,lineEdit1);

    label2 = new QLabel();
    lineEdit2 = new QLineEdit();
    fl1->addRow(label2,lineEdit2);
    
    label3 = new QLabel();
    lineEdit3 = new QLineEdit();
    fl1->addRow(label3,lineEdit3);
    

    label1->setText("Assembly position (x,y,z)");
    label2->setText("Bottom part position (x,y,z)");
    label3->setText("Top part position (x,y,z)");
    

    
    lineEdit1->setText("0.0,55.0,-108.0");
    lineEdit2->setText("50.0,-111.90,-127.7184");
    lineEdit3->setText("-80.0,-111.90,-125.70");
    
    connect(button1, SIGNAL(clicked()),
            this, SLOT(run()));
    
    connect(button0, SIGNAL(clicked()),
            this, SLOT(run_alignment()));
}


void AssemblySandwichAssembler::run_alignment(){

    NQLog("AssemblySandwichAssembler") << ":run_alignment()";

    emit launchAlignment(1,0.0,0.0,0.0);

}

void AssemblySandwichAssembler::run()
{
    
    NQLog("AssemblySandwichAssembler") << ":run";
    
    
    
    //parse lineEdit text to get target coordinates
    QString  parent_string_assembly = this->lineEdit1->text();
    
    QStringList pieces_assembly = parent_string_assembly.split( "," );
    QString x_assembly_s = pieces_assembly.value( pieces_assembly.length() - 3);
    QString y_assembly_s = pieces_assembly.value( pieces_assembly.length() - 2);
    QString z_assembly_s = pieces_assembly.value( pieces_assembly.length() -1);
    
    double x_assembly_d = x_assembly_s.toDouble();
    double y_assembly_d = y_assembly_s.toDouble();
    double z_assembly_d = z_assembly_s.toDouble();
    
    
    QString  parent_string_bottom = this->lineEdit2->text();
    
    QStringList pieces_bottom = parent_string_bottom.split( "," );
    QString x_bottom_s = pieces_bottom.value( pieces_bottom.length() - 3);
    QString y_bottom_s = pieces_bottom.value( pieces_bottom.length() - 2);
    QString z_bottom_s = pieces_bottom.value( pieces_bottom.length() - 1);
    
    double x_bottom_d = x_bottom_s.toDouble();
    double y_bottom_d = y_bottom_s.toDouble();
    double z_bottom_d = z_bottom_s.toDouble();
    
    
    QString  parent_string_top = this->lineEdit3->text();
    
    QStringList pieces_top = parent_string_top.split( "," );
    QString x_top_s = pieces_top.value( pieces_top.length() - 3);
    QString y_top_s = pieces_top.value( pieces_top.length() - 2);
    QString z_top_s = pieces_top.value( pieces_top.length() - 1);
    
    double x_top_d = x_top_s.toDouble();
    double y_top_d = y_top_s.toDouble();
    double z_top_d = z_top_s.toDouble();
    
    NQLog("AssemblySandwichAssembler::run")<<  " x_a = " << x_assembly_d <<  " y_a = " << y_assembly_d << " z_a = " << z_assembly_d;
    NQLog("AssemblySandwichAssembler::run")<<  " x_b = " << x_bottom_d <<  " y_b = " << y_bottom_d << " z_b = " << z_bottom_d;
    NQLog("AssemblySandwichAssembler::run")<<  " x_t = " << x_top_d <<  " y_t = " << y_top_d << " z_t = " << z_top_d;


    emit launchSandwichAssembly(x_assembly_d, y_assembly_d, z_assembly_d, x_bottom_d, y_bottom_d, z_bottom_d, x_top_d, y_top_d, z_top_d);
    
}
// ===========================================================================

MoveWidget::MoveWidget(const QString& label, const QString& default_entry, const bool move_relative, QWidget* parent) :
  QWidget(parent),
  moveRelative_(move_relative)
{
  layout_ = new QFormLayout(this);
  this->setLayout(layout_);

  button_ = new QPushButton(label, this);

  lineed_ = new QLineEdit();
  lineed_->setText(default_entry);
  layout_->addRow(button_, lineed_);

  connect(button_, SIGNAL(clicked()), this, SLOT(execute()));
}

void MoveWidget::execute()
{
  QString line_entry = this->lineed_->text();

  // parse lineEdit text to get target coordinates
  const QStringList entries = line_entry.remove(" ").split(",");

  if((entries.length() == 3) || (entries.length() == 4))
  {
    const double x_d = entries.value(0).toDouble();
    const double y_d = entries.value(1).toDouble();
    const double z_d = entries.value(2).toDouble();

    const double a_d = (entries.length() == 4) ? entries.value(3).toDouble() : 0.0;

    if(moveRelative_)
    {
      NQLog("MoveWidget", NQLog::Message) << "execute"
         << ": emitting signal \"moveRelative("
         << x_d << ", " << y_d << ", " << z_d << ", " << a_d << ")\"";

      emit moveRelative(x_d, y_d, z_d, a_d);
    }
    else
    {
      NQLog("MoveWidget", NQLog::Message) << "execute"
         << ": emitting signal \"moveAbsolute("
         << x_d << ", " << y_d << ", " << z_d << ", " << a_d << ")\"";

      emit moveAbsolute(x_d, y_d, z_d, a_d);
    }
  }
  else
  {
    NQLog("MoveWidget", NQLog::Warning) << "execute"
       << ": invalid input string format (" << line_entry << "), no action taken";

    return;
  }

  return;
}
// ===========================================================================

StringWidget::StringWidget(const QString& label, const QString& default_entry, QWidget* parent) : QWidget(parent)
{
  layout_ = new QFormLayout(this);
  this->setLayout(layout_);

  button_ = new QPushButton(label, this);

  lineed_ = new QLineEdit();
  lineed_->setText(default_entry);
  layout_->addRow(button_, lineed_);

  connect(button_, SIGNAL(clicked()), this, SLOT(execute()));
}

QString StringWidget::get_input_string() const
{
  return this->lineed_->text();
}

void StringWidget::execute()
{
  const QString line_entry = this->get_input_string();

  NQLog("StringWidget", NQLog::Debug) << "execute"
     << ": emitting signal \"input_string(" << line_entry << ")\"";

  emit input_string(line_entry);
}
// ===========================================================================

PatRecWidget::PatRecWidget(const QString& label, QWidget* parent) :
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
  layout_ = new QFormLayout(this);
  this->setLayout(layout_);

  QGridLayout*    layout_1 = new QGridLayout();
  layout_->addRow(layout_1);

  button_ = new QPushButton(label, this);
  button_->setEnabled(true);
  layout_1->addWidget(button_, 0, 0);

  groupBox1_ = new QGroupBox(tr("Object"));
  groupBox2_ = new QGroupBox(tr("Mode"));

  radio1_ = new QRadioButton(tr("&Fiducial marker"));
  radio2_ = new QRadioButton(tr("&Positioning pin"));
  radio3_ = new QRadioButton(tr("&Sensor corner"));
  radio4_ = new QRadioButton(tr("&Spacer corner"));

  radio1_->setChecked(true);

  vbox1_ = new QVBoxLayout;
  vbox1_->addWidget(radio1_);
  vbox1_->addWidget(radio2_);
  vbox1_->addWidget(radio3_);
  vbox1_->addWidget(radio4_);
  vbox1_->addStretch(1);

  groupBox1_->setLayout(vbox1_);

  radio5_ = new QRadioButton(tr("&Demo"));
  radio5_->setChecked(true);

  radio6_ = new QRadioButton(tr("&Lab"));

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
  QFormLayout*    layout_2 = new QFormLayout();
  layout_->addRow(layout_2);

  // widget: PatRec rough angles
  sw_angrough_ = new StringWidget("Rough Angles (list)", "0,180");
  layout_2->addRow(sw_angrough_);

  // widget: PatRec angular-scan parameters
  sw_angscanp_ = new StringWidget("Ang-Scan parameters (fine-range, fine-step)", "5,0.25");
  layout_2->addRow(sw_angscanp_);
  // ---------------------

  connect(button_, SIGNAL(clicked()), this, SLOT(execute()));
}

void PatRecWidget::execute()
{
  int mode_lab(0), mode_obj(0);

  if     (radio1_->isChecked()){ mode_obj = 0; }
  else if(radio2_->isChecked()){ mode_obj = 1; }
  else if(radio3_->isChecked()){ mode_obj = 2; }
  else if(radio4_->isChecked()){ mode_obj = 3; }

  if     (radio5_->isChecked()){ mode_lab = 0; }
  else if(radio6_->isChecked()){ mode_lab = 1; }

  NQLog("PatRecWidget", NQLog::Debug) << "execute"
     << ": emitting signal \"mode(" << mode_lab << ", " << mode_obj << ")\"";

  emit mode(mode_lab, mode_obj);
}

void PatRecWidget::change_label(const int state)
{
  NQLog("PatRecWidget", NQLog::Debug) << "change_label(" << state << ")";

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

VacuumWidget::VacuumWidget(const QString& label, QWidget* parent) :
  QWidget(parent),
  button_(0)
{
  QGridLayout* layout = new QGridLayout(this);
  this->setLayout(layout);

  button_ = new QPushButton(label, this);
  layout->addWidget(button_, 0, 0);

  QRadioButton* radio_1 = new QRadioButton(tr("&Pickup"));
  QRadioButton* radio_2 = new QRadioButton(tr("&Spacers"));
  QRadioButton* radio_3 = new QRadioButton(tr("&Baseplate"));
//  QRadioButton* radio_4 = new QRadioButton(tr("&Channel 4"));

  layout->addWidget(radio_1, 1, 0);
  layout->addWidget(radio_2, 3, 0);
  layout->addWidget(radio_3, 5, 0);
//  layout->addWidget(radio_4, 7, 0);

  valves_.clear();
  valves_.push_back(radio_1);
  valves_.push_back(radio_2);
  valves_.push_back(radio_3);
//  valves_.push_back(radio4);

  QPixmap pixmap(100, 100);
  pixmap.fill(QColor("transparent"));

  QPainter painter(&pixmap);
  painter.setBrush(QBrush(Qt::red));
  painter.drawEllipse(0, 0, 30, 30);

  QLabel* label_1 = new QLabel("", this);
  label_1->setPixmap(pixmap);
  label_1->setText(" VACUUM OFF");
  label_1->setStyleSheet("QLabel { background-color : green; color : black; }");

  QLabel* label_2 = new QLabel("", this);
  label_2->setPixmap(pixmap);
  label_2->setText(" VACUUM OFF");
  label_2->setStyleSheet("QLabel { background-color : green; color : black; }");

  QLabel* label_3 = new QLabel("", this);
  label_3->setPixmap(pixmap);
  label_3->setText(" VACUUM OFF");
  label_3->setStyleSheet("QLabel { background-color : green; color : black; }");

//  QLabel* label_4 = new QLabel("", this);
//  label_4->setPixmap(pixmap);
//  label_4->setText(" VACUUM OFF");
//  label_4->setStyleSheet("QLabel { background-color : green; color : black; }");

  layout->addWidget(label_1, 1, 1);
  layout->addWidget(label_2, 3, 1);
  layout->addWidget(label_3, 5, 1);
//  layout->addWidget(label_4, 7, 1);

  labels_.clear();
  labels_.push_back(label_1);
  labels_.push_back(label_2);
  labels_.push_back(label_3);
//  labels_.push_back(label_4);

  connect(button_, SIGNAL(clicked()), this, SLOT(toggleVacuum()));

  NQLog("VacuumWidget", NQLog::Debug) << "constructed";
}

void VacuumWidget::toggleVacuum()
{
  NQLog("VacuumWidget") << ": toggling vacuum voltage";

  for(unsigned int i=0; i<valves_.size(); ++i)
  {
    if(valves_.at(i)->isChecked())
    {
      NQLog("VacuumWidget") << ": emit signal to channel " << (i+1);

      button_->setEnabled(false);

      emit toggleVacuum(i+1);

      return;
    }
  }

  NQLog("VacuumWidget") << ": None channel selected! Vacuum is not toggled.";
}

void VacuumWidget::enableVacuumButton()
{
  button_->setEnabled(true);
}

void VacuumWidget::disableVacuumButton()
{
  button_->setEnabled(false);
}

void VacuumWidget::updateVacuumChannelState(const int channelNumber, const bool channelState)
{
  if(channelNumber >= int(labels_.size()))
  {
    NQLog("VacuumWidget", NQLog::Warning) << "updateVacuumChannelState"
       << "(" << channelNumber << ", " << channelState << ")"
       << ": out-of-range input channel number (" << channelNumber << "), no action taken";

    return;
  }

  emit enableVacuumButton();

  if(channelState)
  {
    labels_.at(channelNumber)->setText(" VACUUM ON");
    labels_.at(channelNumber)->setStyleSheet("QLabel { background-color : red; color : black; }");
  }
  else
  {
    labels_.at(channelNumber)->setText(" VACUUM OFF");
    labels_.at(channelNumber)->setStyleSheet("QLabel { background-color : green; color : black; }");    
  }
}
// ===========================================================================

//!!AssemblyMountChecker::AssemblyMountChecker(QWidget *parent, std::string string,
//!!                                           double x ,double y, double z,
//!!                                           double a, int mode)
//!!: QWidget(parent), local_x(x), local_y(y),local_z(z),local_a(a)
//!!{
//!!  QFormLayout *l = new QFormLayout(this);
//!!  setLayout(l);
//!!
//!!  NQLog("AssemblyMountChecker") << ": in mode" << mode;
//!!
//!!  std::ostringstream strs;
//!!  strs.clear();
//!!  strs << x;
//!!  strs << ",";
//!!  strs << y;
//!!  strs << ",";
//!!  strs << z;
//!!  std::string str = strs.str();
//!!  QString qstr = QString::fromStdString(str);
//!!  QString qname = QString::fromStdString(string);
//!!
//!!  this->local_x = x;
//!!  this->local_y = y;
//!!  this->local_z = z;
//!!  this->local_a = a;
//!!
//!!  button1 = new QPushButton(qname, this);
//!!
//!!  lineEdit1 = new QLineEdit();
//!!  lineEdit1->setText(qstr);
//!!  l->addRow(button1,lineEdit1);
//!!
//!!  connect(button1, SIGNAL(clicked()),
//!!          this, SLOT(checkMount()));
//!!}
//!!
//!!void AssemblyMountChecker::checkMount()
//!!{
//!!  //parse lineEdit text to get target coordinates
//!!  QString  parent_string = this->lineEdit1->text();
//!!
//!!  QStringList pieces = parent_string.split( "," );
//!!  QString x = pieces.value( pieces.length() - 3);
//!!  QString y = pieces.value( pieces.length() - 2);
//!!  QString z = pieces.value( pieces.length() - 1);
//!!  QString a = pieces.value( pieces.length() - 1);
//!!
//!!  double x_d = x.toDouble();
//!!  double y_d = y.toDouble();
//!!  double z_d = z.toDouble();
//!!  double a_d = a.toDouble();
//!!
//!!  NQLog("AssemblyMountChecker") << ": going to target (parsed) "<< x_d<<" "<< y_d<<"  "<< z_d;
//!!
//!!  emit moveAbsolute(x_d, y_d, z_d, a_d);// this should bring you to the first corner
//!!  emit locateCorner(2);
//!!  emit reportCornerLocation(1);
//!!  emit moveRelative(0.0, 10.0, 0.0, 0.0);//move to next corner
//!!  emit locateCorner(3);
//!!  emit reportCornerLocation(2);
//!!  emit moveRelative(5.0, 0.0, 0.0, 0.0);//move to next corner
//!!  emit locateCorner(4);
//!!  emit reportCornerLocation(3);
//!!  emit moveRelative(0.0, -10.0, 0.0, 0.0);//move to next corner
//!!  emit locateCorner(5);
//!!  emit reportCornerLocation(4);
//!!}
// ----------

//!!AssemblyAligner::AssemblyAligner(QWidget *parent, std::string string,
//!!                                 double a)
//!!  : QWidget(parent),local_a(a)
//!!{
//!!  QFormLayout *l = new QFormLayout(this);
//!!  setLayout(l);
//!!
//!!  std::ostringstream strs;
//!!  strs.clear();
//!!  strs << a;
//!!  std::string str = strs.str();
//!!  QString qstr = QString::fromStdString(str);
//!!  QString qname = QString::fromStdString(string);
//!!
//!!  button1 = new QPushButton(qname, this);
//!!
//!!  lineEdit1 = new QLineEdit();
//!!  lineEdit1->setText(qstr);
//!!  l->addRow(button1,lineEdit1);
//!!
//!!  connect(button1, SIGNAL(clicked()),
//!!          this, SLOT(setDown()));
//!!}
//!!
//!!void AssemblyAligner::setDown()
//!!{
//!!  emit locateSetdowncorner(6);
//!!
//!!}
//!!
//!!void AssemblyAligner::align()
//!!{
//!!  //     emit locate(6);
//!!
//!!  //parse lineEdit text to get target coordinates
//!!  QString  parent_string = this->lineEdit1->text();
//!!
//!!  //  QStringList pieces = parent_string.split( "," );
//!!  //  QString x = pieces.value( pieces.length() - 3);
//!!  //  QString y = pieces.value( pieces.length() - 2);
//!!  //  QString z = pieces.value( pieces.length() - 1);
//!!  //  QString a = pieces.value( pieces.length() - 1);
//!!
//!!  // double x_d = x.toDouble();
//!!  // double y_d = y.toDouble();
//!!  // double z_d = z.toDouble();
//!!  double a_d = parent_string.toDouble();
//!!
//!!  NQLog("AssemblyAligner:align") << ": going to target alignment (parsed) "<< a_d<<" ";
//!!
//!!  emit moveRelative(0.0, 0.0, 0.0, a_d);
//!!}
// ----------
