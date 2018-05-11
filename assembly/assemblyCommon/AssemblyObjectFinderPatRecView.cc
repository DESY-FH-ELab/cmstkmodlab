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

#include <sstream>
#include <iomanip>

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QGroupBox>
#include <QPixmap>
#include <QApplication>
#include <QImageReader>

AssemblyObjectFinderPatRecView::AssemblyObjectFinderPatRecView(QWidget* parent) :
  QWidget(parent),

  scrollArea_1_(0),
  scrollArea_2_(0),
  scrollArea_3_(0),
  scrollArea_4_(0),

  imageView_1_(0),
  imageView_2_(0),
  imageView_3_(0),
  imageView_4_(0),

  patrec_exe_button_(nullptr),
  patrec_exe_label_(nullptr),

  w_patrec_(nullptr),

  patrec_res1_linee_(nullptr),
  patrec_res2_linee_(nullptr),

  finder_connected_(false)
{
  QHBoxLayout* layout = new QHBoxLayout;
  this->setLayout(layout);

  // IMAGE VIEW(S) -------
  QGridLayout* g0 = new QGridLayout;
  layout->addLayout(g0);

  QPalette palette;
  palette.setColor(QPalette::Background, QColor(220, 220, 220));

  imageView_1_ = new AssemblyUEyeView(this);
  imageView_1_->setMinimumSize(200, 200);
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

  imageView_2_ = new AssemblyUEyeView(this);
  imageView_2_->setMinimumSize(200, 200);
  imageView_2_->setPalette(palette);
  imageView_2_->setBackgroundRole(QPalette::Background);
  imageView_2_->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
  imageView_2_->setScaledContents(true);
  imageView_2_->setAlignment(Qt::AlignCenter);
  imageView_2_->setZoomFactor(0.50);

  scrollArea_2_ = new QScrollArea(this);
  scrollArea_2_->setMinimumSize(200, 200);
  scrollArea_2_->setPalette(palette);
  scrollArea_2_->setBackgroundRole(QPalette::Background);
  scrollArea_2_->setAlignment(Qt::AlignCenter);
  scrollArea_2_->setWidget(imageView_2_);
  scrollArea_2_->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

  g0->addWidget(scrollArea_2_, 0, 1);

  imageView_3_ = new AssemblyUEyeView(this);
  imageView_3_->setMinimumSize(200, 200);
  imageView_3_->setPalette(palette);
  imageView_3_->setBackgroundRole(QPalette::Background);
  imageView_3_->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
  imageView_3_->setScaledContents(true);
  imageView_3_->setAlignment(Qt::AlignCenter);

  scrollArea_3_ = new QScrollArea(this);
  scrollArea_3_->setMinimumSize(200, 200);
  scrollArea_3_->setPalette(palette);
  scrollArea_3_->setBackgroundRole(QPalette::Background);
  scrollArea_3_->setAlignment(Qt::AlignCenter);
  scrollArea_3_->setWidget(imageView_3_);
  scrollArea_3_->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

  g0->addWidget(scrollArea_3_, 1, 0);

  imageView_4_ = new AssemblyUEyeView(this);
  imageView_4_->setMinimumSize(200, 200);
  imageView_4_->setPalette(palette);
  imageView_4_->setBackgroundRole(QPalette::Background);
  imageView_4_->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
  imageView_4_->setScaledContents(true);
  imageView_4_->setAlignment(Qt::AlignCenter);
  imageView_4_->setZoomFactor(0.35);

  scrollArea_4_ = new QScrollArea(this);
  scrollArea_4_->setMinimumSize(200, 200);
  scrollArea_4_->setPalette(palette);
  scrollArea_4_->setBackgroundRole(QPalette::Background);
  scrollArea_4_->setAlignment(Qt::AlignCenter);
  scrollArea_4_->setWidget(imageView_4_);
  scrollArea_4_->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

  g0->addWidget(scrollArea_4_, 1, 1);
  // ---------------------

  // PatRec execution
  QGroupBox* patrec_box = new QGroupBox(tr("Pattern Recognition"));
  patrec_box->setStyleSheet("QGroupBox { font-weight: bold; } ");

  QVBoxLayout* patrec_lay = new QVBoxLayout;

  QGridLayout* patrec_exe_lay = new QGridLayout;

  patrec_exe_button_ = new QPushButton(tr("Standalone PatRec"), this);
  patrec_exe_lay->addWidget(patrec_exe_button_, 0, 0);

  QPixmap pixmap(100, 100);
  pixmap.fill(QColor("transparent"));

  patrec_exe_label_ = new QLabel("", this);
  patrec_exe_label_->setPixmap(pixmap);
  patrec_exe_label_->setText(" WAITING");
  patrec_exe_label_->setStyleSheet("QLabel { background-color : orange; color : black; }");

  patrec_exe_lay->addWidget(patrec_exe_label_, 0, 1);

  patrec_lay->addLayout(patrec_exe_lay);
  // -----------

  // PatRec configuration
  QLabel* patrec_cfg_title = new QLabel(tr("Configuration"));
  patrec_cfg_title->setAlignment(Qt::AlignCenter);
  patrec_cfg_title->setStyleSheet("QLabel { font-weight: bold; color : blue; }");

  patrec_lay->addSpacing(20);
  patrec_lay->addWidget(patrec_cfg_title);

  w_patrec_ = new AssemblyObjectFinderPatRecWidget;
  w_patrec_->setToolTip("Pattern Recognition Configuration");

  connect(patrec_exe_button_, SIGNAL(clicked()), this->PatRec_Widget(), SLOT(update_configuration()));

  imageView_4_->connectImageProducer(this->PatRec_Widget(), SIGNAL(updated_image_template(cv::Mat)));

  ApplicationConfig* config = ApplicationConfig::instance();
  if(config != nullptr)
  {
    const std::string fpath = config->getValue<std::string>("AssemblyObjectFinderPatRecView_template_fpath", "");
    if(fpath != ""){ w_patrec_->load_image_template_from_path(QString::fromStdString(Config::CMSTkModLabBasePath+"/"+fpath)); }
  }

  patrec_lay->addWidget(w_patrec_);
  // -----------

  // PatRec results
  QLabel* patrec_res_title = new QLabel(tr("Results"));
  patrec_res_title->setAlignment(Qt::AlignCenter);
  patrec_res_title->setStyleSheet("QLabel { font-weight: bold; color : blue; }");

  patrec_lay->addSpacing(10);
  patrec_lay->addWidget(patrec_res_title);
  patrec_lay->addSpacing(10);

  QGridLayout* patrec_res_lay = new QGridLayout;

  QLabel* patrec_res1_label = new QLabel(tr("Best-Match XY position [mm]"));
  patrec_res1_linee_ = new QLineEdit(tr(""));
  patrec_res1_linee_->setReadOnly(true);

  patrec_res_lay->addWidget(patrec_res1_label , 0, 0);
  patrec_res_lay->addWidget(patrec_res1_linee_, 0, 1);

  QLabel* patrec_res2_label = new QLabel(tr("Best-Match Template Orientation [deg]"));
  patrec_res2_linee_ = new QLineEdit(tr(""));
  patrec_res2_linee_->setReadOnly(true);

  patrec_res_lay->addWidget(patrec_res2_label , 1, 0);
  patrec_res_lay->addWidget(patrec_res2_linee_, 1, 1);

  patrec_lay->addLayout(patrec_res_lay);
  // -----------

  patrec_lay->addStretch(1);

  patrec_box->setLayout(patrec_lay);

  layout->addWidget(patrec_box);

  layout->setStretch(0, 55);
  layout->setStretch(1, 45);
  // ---------------------
}

void AssemblyObjectFinderPatRecView::connect_to_finder(const AssemblyObjectFinderPatRec* const finder)
{
  if(finder == nullptr)
  {
    NQLog("AssemblyObjectFinderPatRecView", NQLog::Critical) << "connect_to_finder"
       << ": null pointer to AssemblyObjectFinderPatRec object, no action taken";

    return;
  }

  if(finder_connected_ == false)
  {
    connect(this->PatRec_Widget(), SIGNAL(configuration_updated(AssemblyObjectFinderPatRec::Configuration)), finder, SLOT(launch_PatRec(AssemblyObjectFinderPatRec::Configuration)));

    connect(finder, SIGNAL(PatRec_exitcode(int)), this, SLOT(change_label(int)));

    connect(finder, SIGNAL(image_path(int, QString)), this, SLOT(updateImage(int, QString)));
    connect(finder, SIGNAL(image_mat (int, cv::Mat)), this, SLOT(updateImage(int, cv::Mat)));

    connect(finder, SIGNAL(reportObjectLocation(int, double, double, double)), this, SLOT(updateText(int, double, double, double)));

    NQLog("AssemblyObjectFinderPatRecView", NQLog::Spam) << "connect_to_finder"
       << ": view connected to object of type AssemblyObjectFinderPatRec";

    finder_connected_ = true;
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
    patrec_res1_linee_->setText(posi_qstr+" [pixel #, pixel #] (field of view)");
    patrec_res2_linee_->setText(orie_qstr);
  }
}

void AssemblyObjectFinderPatRecView::updateImage(const int stage, const QString& filename)
{
  NQLog("AssemblyObjectFinderPatRecView", NQLog::Spam) << "updateImage(" << stage << ", file=" << filename << ")";

  const cv::Mat img = assembly::cv_imread(filename, CV_LOAD_IMAGE_UNCHANGED);

  this->updateImage(stage, img);

  return;
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
//    imageView_2_->setZoomFactor(0.5);
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

void AssemblyObjectFinderPatRecView::change_label(const int state)
{
  NQLog("AssemblyObjectFinderPatRecView", NQLog::Spam) << "change_label(" << state << ")";

  if(state == 0)
  {
    patrec_exe_label_->setText(" FOUND MARKER");
    patrec_exe_label_->setStyleSheet("QLabel { background-color : green; color : black; }");
  }
  else
  {
    patrec_exe_label_->setText(" ERROR");
    patrec_exe_label_->setStyleSheet("QLabel { background-color : red; color : black; }");
  }

  return;
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
