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

#include <AssemblyObjectFinderPatRecView.h>

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

  radio1_(nullptr),
  radio2_(nullptr),
  radio3_(nullptr),
  radio4_(nullptr),
  radio5_(nullptr),
  radio6_(nullptr),

  w_patrec_(nullptr),

  patrec_res1_linee_(nullptr),
  patrec_res2_linee_(nullptr),

  objfinder_connected_(false)
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

  QGridLayout* layout_1 = new QGridLayout;

  patrec_exe_button_ = new QPushButton(tr("Standalone PatRec"), this);
  layout_1->addWidget(patrec_exe_button_, 0, 0);

  QPixmap pixmap(100, 100);
  pixmap.fill(QColor("transparent"));

  patrec_exe_label_ = new QLabel("", this);
  patrec_exe_label_->setPixmap(pixmap);
  patrec_exe_label_->setText(" WAITING");
  patrec_exe_label_->setStyleSheet("QLabel { background-color : orange; color : black; }");

  layout_1->addWidget(patrec_exe_label_, 0, 1);

  QGroupBox* groupBox1_ = new QGroupBox(tr("Object"));
  groupBox1_->setStyleSheet("QGroupBox { font-weight: bold; } ");

  radio1_ = new QRadioButton(tr("Fiducial marker"), this);
  radio2_ = new QRadioButton(tr("Positioning pin"), this);
  radio3_ = new QRadioButton(tr("Sensor corner")  , this);
  radio4_ = new QRadioButton(tr("Spacer corner")  , this);

  radio1_->setChecked(true);

  QVBoxLayout* vbox1_ = new QVBoxLayout;
  vbox1_->addWidget(radio1_);
  vbox1_->addWidget(radio2_);
  vbox1_->addWidget(radio3_);
  vbox1_->addWidget(radio4_);
  vbox1_->addStretch(1);

  groupBox1_->setLayout(vbox1_);

  QGroupBox* groupBox2_ = new QGroupBox(tr("Mode"));
  groupBox2_->setStyleSheet("QGroupBox { font-weight: bold; } ");

  radio5_ = new QRadioButton(tr("Demo"), this);
  radio6_ = new QRadioButton(tr("Lab") , this);

  radio5_->setChecked(true);

  QVBoxLayout* vbox2_ = new QVBoxLayout;
  vbox2_->addWidget(radio5_);
  vbox2_->addWidget(radio6_);
  vbox2_->addStretch(1);

  groupBox2_->setLayout(vbox2_);

  layout_1->addWidget(groupBox1_, 1, 0);
  layout_1->addWidget(groupBox2_, 1, 1);

  patrec_lay->addLayout(layout_1);
  // -----------

  // PatRec configuration
  QLabel* patrec_cfg_title = new QLabel(tr("Configuration"));
  patrec_cfg_title->setAlignment(Qt::AlignCenter);
  patrec_cfg_title->setStyleSheet("QLabel { font-weight: bold; color : blue; }");

  patrec_lay->addSpacing(20);
  patrec_lay->addWidget(patrec_cfg_title);

  w_patrec_ = new AssemblyObjectFinderPatRecWidget;
  w_patrec_->setToolTip("Pattern Recognition Configuration");

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

//!!    connect(this->PatRec_Widget()->widget_angrough(), SIGNAL(input_string(QString)), finder, SLOT(update_rough_angles      (QString)));
//!!    connect(this->PatRec_Widget()->widget_angscanp(), SIGNAL(input_string(QString)), finder, SLOT(update_angscan_parameters(QString)));

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
    patrec_res1_linee_->setText(posi_qstr+" [pixel #, pixel #] (field of view)");
    patrec_res2_linee_->setText(orie_qstr);
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
