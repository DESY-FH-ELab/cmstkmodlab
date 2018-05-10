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

#include <AssemblyObjectFinderPatRecView.h>
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

AssemblyObjectFinderPatRecView::AssemblyObjectFinderPatRecView(const LStepExpressMotionManager* motion_manager, QWidget* parent) :
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

  objfinder_connected_(false)
{
  if(motion_manager == nullptr)
  {
    NQLog("AssemblyObjectFinderPatRecView", NQLog::Critical)
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

  QFormLayout* f01 = new QFormLayout;
  layout->addLayout(f01, 0, 1);

  // PATREC  WIDGET ------
  QGroupBox* box_patrec = new QGroupBox(tr("Pattern Recognition"));

  w_patrec_ = new AssemblyPatRecWidget_NEW("Standalone PatRec", this);
  w_patrec_->setToolTip("(4) Runs Pattern Recognition routine to determine sensor (x,y,z,a) position");

  box_patrec->setLayout(w_patrec_->layout());

  f01->addRow(box_patrec);
  // ---------------------
}

void AssemblyObjectFinderPatRecView::connect_to_finder(const AssemblyObjectFinderPatRec* finder)
{
  if(finder == nullptr)
  {
    NQLog("AssemblyObjectFinderPatRecView", NQLog::Critical) << "connect_to_finder"
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

    NQLog("AssemblyObjectFinderPatRecView", NQLog::Spam) << "connect_to_finder"
       << ": widget connected to object of type AssemblyObjectFinderPatRec";

    objfinder_connected_ = true;
  }
  else
  {
    NQLog("AssemblyObjectFinderPatRecView", NQLog::Critical) << "connect_to_finder"
       << ": widget already connected to an object of type AssemblyObjectFinderPatRec, no action taken";

    return;
  }

  return;
}

void AssemblyObjectFinderPatRecView::updateText(int stage, double x, double y, double a)
{
  NQLog("AssemblyObjectFinderPatRecView", NQLog::Spam) << "updateText"
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

void AssemblyObjectFinderPatRecView::updateImage(const int stage, const QString& filename)
{
  NQLog("AssemblyObjectFinderPatRecView", NQLog::Spam) << "updateImage(" << stage << ", file=" << filename << ")";

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

void AssemblyObjectFinderPatRecView::updateImage(const int stage, const cv::Mat& img)
{
  NQLog("AssemblyObjectFinderPatRecView", NQLog::Spam) << "updateImage(" << stage << ", image)";

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

void AssemblyObjectFinderPatRecView::keyReleaseEvent(QKeyEvent* event)
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

AssemblyStringWidget_NEW::AssemblyStringWidget_NEW(const QString& label, const QString& default_entry, QWidget* parent) : QWidget(parent)
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

QString AssemblyStringWidget_NEW::get_input_string() const
{
  return this->lineed_->text();
}

void AssemblyStringWidget_NEW::execute()
{
  const QString line_entry = this->get_input_string();

  NQLog("AssemblyStringWidget_NEW", NQLog::Spam) << "execute"
     << ": emitting signal \"input_string(" << line_entry << ")\"";

  emit input_string(line_entry);
}
// ===========================================================================

AssemblyPatRecWidget_NEW::AssemblyPatRecWidget_NEW(const QString& label, QWidget* parent) :
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
  sw_angrough_ = new AssemblyStringWidget_NEW("Pre-Scan Angles (list)", "0,180", this);
  layout_2->addWidget(sw_angrough_->button(), 0, 0);
  layout_2->addWidget(sw_angrough_->lineed(), 0, 1);

  // widget: PatRec angular-scan parameters
  sw_angscanp_ = new AssemblyStringWidget_NEW("Scan Params (fine-range, fine-step)", "5,0.125", this);
  layout_2->addWidget(sw_angscanp_->button(), 1, 0);
  layout_2->addWidget(sw_angscanp_->lineed(), 1, 1);
  // ---------------------

  connect(button_, SIGNAL(clicked()), this, SLOT(execute()));
}

void AssemblyPatRecWidget_NEW::execute()
{
  int mode_lab(0), mode_obj(0);

  if     (radio1_->isChecked()){ mode_obj = 0; }
  else if(radio2_->isChecked()){ mode_obj = 1; }
  else if(radio3_->isChecked()){ mode_obj = 2; }
  else if(radio4_->isChecked()){ mode_obj = 3; }

  if     (radio5_->isChecked()){ mode_lab = 0; }
  else if(radio6_->isChecked()){ mode_lab = 1; }

  NQLog("AssemblyPatRecWidget_NEW", NQLog::Spam) << "execute"
     << ": emitting signal \"mode(" << mode_lab << ", " << mode_obj << ")\"";

  emit mode(mode_lab, mode_obj);
}

void AssemblyPatRecWidget_NEW::change_label(const int state)
{
  NQLog("AssemblyPatRecWidget_NEW", NQLog::Spam) << "change_label(" << state << ")";

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
