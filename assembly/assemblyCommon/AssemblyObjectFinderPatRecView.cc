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
#include <QMutexLocker>

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

  patrec_wid_(nullptr),

  patrec_res_dX_linee_(nullptr),
  patrec_res_dY_linee_(nullptr),
  patrec_res_ang_linee_(nullptr),

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

  patrec_wid_ = new AssemblyObjectFinderPatRecWidget;
  patrec_wid_->setToolTip("Pattern Recognition Configuration");

  connect(patrec_exe_button_, SIGNAL(clicked()), this->PatRec_Widget(), SLOT(transmit_configuration()));

  imageView_4_->connectImageProducer(this->PatRec_Widget(), SIGNAL(updated_image_template(cv::Mat)));

  ApplicationConfig* config = ApplicationConfig::instance();
  if(config != nullptr)
  {
    const std::string fpath = config->getValue<std::string>("AssemblyObjectFinderPatRecView_template_fpath", "");
    if(fpath != ""){ patrec_wid_->load_image_template_from_path(QString::fromStdString(Config::CMSTkModLabBasePath+"/"+fpath)); }

    assembly::QLineEdit_setText(patrec_wid_->threshold_lineEdit()        , config->getValue<int>("AssemblyObjectFinderPatRecView_threshold"        , 100));
    assembly::QLineEdit_setText(patrec_wid_->adaptiveThreshold_lineEdit(), config->getValue<int>("AssemblyObjectFinderPatRecView_adaptiveThreshold", 587));

    assembly::QLineEdit_setText(patrec_wid_->angles_prescan_lineEdit()   , config->getValue<double>("AssemblyObjectFinderPatRecView_angles_prescan" , 0));
    assembly::QLineEdit_setText(patrec_wid_->angles_finemax_lineEdit()   , config->getValue<double>("AssemblyObjectFinderPatRecView_angles_finemax" , 2));
    assembly::QLineEdit_setText(patrec_wid_->angles_finestep_lineEdit()  , config->getValue<double>("AssemblyObjectFinderPatRecView_angles_finestep", 0.2));
  }

  patrec_lay->addWidget(patrec_wid_);
  // -----------

  // PatRec results
  QLabel* patrec_res_title = new QLabel(tr("Results"));
  patrec_res_title->setAlignment(Qt::AlignCenter);
  patrec_res_title->setStyleSheet("QLabel { font-weight: bold; color : blue; }");

  patrec_lay->addSpacing(10);
  patrec_lay->addWidget(patrec_res_title);
  patrec_lay->addSpacing(10);

  QGridLayout* patrec_res_lay = new QGridLayout;

  QLabel* patrec_res_dX_label = new QLabel(tr("Delta-X to Best-Match Pos. [mm]"));
  patrec_res_dX_linee_ = new QLineEdit(tr(""));
  patrec_res_dX_linee_->setReadOnly(true);

  patrec_res_lay->addWidget(patrec_res_dX_label , 0, 0);
  patrec_res_lay->addWidget(patrec_res_dX_linee_, 0, 1);

  QLabel* patrec_res_dY_label = new QLabel(tr("Delta-Y to Best-Match Pos. [mm]"));
  patrec_res_dY_linee_ = new QLineEdit(tr(""));
  patrec_res_dY_linee_->setReadOnly(true);

  patrec_res_lay->addWidget(patrec_res_dY_label , 1, 0);
  patrec_res_lay->addWidget(patrec_res_dY_linee_, 1, 1);

  QLabel* patrec_res_ang_label = new QLabel(tr("Best-Match Template Orient. [deg]"));
  patrec_res_ang_linee_ = new QLineEdit(tr(""));
  patrec_res_ang_linee_->setReadOnly(true);

  patrec_res_lay->addWidget(patrec_res_ang_label , 2, 0);
  patrec_res_lay->addWidget(patrec_res_ang_linee_, 2, 1);

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
    connect(this->PatRec_Widget(), SIGNAL(configuration(AssemblyObjectFinderPatRec::Configuration)), finder, SLOT(launch_PatRec(AssemblyObjectFinderPatRec::Configuration)));

    connect(finder, SIGNAL(PatRec_exitcode(int)), this, SLOT(update_label(int)));

    connect(finder, SIGNAL(PatRec_res_image_master_edited(QString))  , this, SLOT(update_image_1(QString)));
    connect(finder, SIGNAL(PatRec_res_image_master_edited(cv::Mat))  , this, SLOT(update_image_1(cv::Mat)));

    connect(finder, SIGNAL(PatRec_res_image_angscan(QString))        , this, SLOT(update_image_2(QString)));
    connect(finder, SIGNAL(PatRec_res_image_angscan(cv::Mat))        , this, SLOT(update_image_2(cv::Mat)));

    connect(finder, SIGNAL(PatRec_res_image_master_PatRec(QString))  , this, SLOT(update_image_3(QString)));
    connect(finder, SIGNAL(PatRec_res_image_master_PatRec(cv::Mat))  , this, SLOT(update_image_3(cv::Mat)));

    connect(finder, SIGNAL(PatRec_res_image_template_PatRec(QString)), this, SLOT(update_image_4(QString)));
    connect(finder, SIGNAL(PatRec_res_image_template_PatRec(cv::Mat)), this, SLOT(update_image_4(cv::Mat)));

    connect(finder, SIGNAL(PatRec_results(double, double, double)), this, SLOT(update_text(double, double, double)));

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

void AssemblyObjectFinderPatRecView::update_text(const double dx, const double dy, const double ang)
{
  NQLog("AssemblyObjectFinderPatRecView", NQLog::Spam) << "update_text"
     << "(" << dx << ", " << dy << ", " << ang << ")";

  std::stringstream strs_dX;
  strs_dX << dx;

  patrec_res_dX_linee_->setText(QString::fromStdString(strs_dX.str()));

  std::stringstream strs_dY;
  strs_dY << dy;

  patrec_res_dY_linee_->setText(QString::fromStdString(strs_dY.str()));

  std::stringstream strs_ang;
  strs_ang << ang;

  patrec_res_ang_linee_->setText(QString::fromStdString(strs_ang.str()));

  return;
}

void AssemblyObjectFinderPatRecView::update_image_1(const QString& fpath){ this->update_image(1, fpath); }
void AssemblyObjectFinderPatRecView::update_image_1(const cv::Mat& image){ this->update_image(1, image); }

void AssemblyObjectFinderPatRecView::update_image_2(const QString& fpath){ this->update_image(2, fpath); }
void AssemblyObjectFinderPatRecView::update_image_2(const cv::Mat& image){ this->update_image(2, image); }

void AssemblyObjectFinderPatRecView::update_image_3(const QString& fpath){ this->update_image(3, fpath); }
void AssemblyObjectFinderPatRecView::update_image_3(const cv::Mat& image){ this->update_image(3, image); }

void AssemblyObjectFinderPatRecView::update_image_4(const QString& fpath){ this->update_image(4, fpath); }
void AssemblyObjectFinderPatRecView::update_image_4(const cv::Mat& image){ this->update_image(4, image); }

void AssemblyObjectFinderPatRecView::update_image(const int stage, const QString& filename)
{
  //
  // NOTE: noticed calling NQLog inside this function can lead to the application crashing
  //       if AssemblyObjectFinderPatRec and AssemblyObjectFinderPatRecView run on separate threads
  //       (possibly because AssemblyObjectFinderPatRec::template_matching calls
  //        the slot AssemblyObjectFinderPatRecView::update_image too frequently)
  //
  // Summary: refrain from using NQLog in this method
  //

  QMutexLocker ml(&mutex_);

  const cv::Mat img = assembly::cv_imread(filename, cv::IMREAD_UNCHANGED);

  ml.unlock();

  this->update_image(stage, img);

  return;
}

void AssemblyObjectFinderPatRecView::update_image(const int stage, const cv::Mat& img)
{
  //
  // NOTE: noticed calling NQLog inside this function can lead to the application crashing
  //       if AssemblyObjectFinderPatRec and AssemblyObjectFinderPatRecView run on separate threads
  //       (possibly because AssemblyObjectFinderPatRec::template_matching calls
  //        the slot AssemblyObjectFinderPatRecView::update_image too frequently)
  //
  // Summary: refrain from using NQLog in this method
  //

  QMutexLocker ml(&mutex_);

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

void AssemblyObjectFinderPatRecView::update_label(const int state)
{
  NQLog("AssemblyObjectFinderPatRecView", NQLog::Spam) << "update_label(" << state << ")";

  QMutexLocker ml(&mutex_);

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

//-- Information about this tab in GUI
//HTML markup (<xxx></xxx>): p paragraph, b bold, em emphasize, i italic, s small, section, summary, var variable, ...
//Ex: <p style="color:red">This is a red paragraph.</p>
void AssemblyObjectFinderPatRecView::display_infoTab()
{
    QMessageBox::information(this, tr("Information - Pattern Recognition"),
            tr("<p>There is no available information about the content of this tab yet.</p>"));

    return;
}
