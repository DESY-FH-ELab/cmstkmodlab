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

#include <AssemblyAutoFocusView.h>
#include <nqlogger.h>
#include <Util.h>

#include <sstream>

#include <QApplication>
#include <QFormLayout>
#include <QLabel>
#include <QString>
#include <QStringList>

AssemblyAutoFocusView::AssemblyAutoFocusView(QWidget* parent) :
  QWidget(parent),

  imageView_1_(0),
  imageView_2_(0),

  scrollArea_1_(0),
  scrollArea_2_(0),

  button_1_(0),
  button_2_(0),

  lineed_1_(0),
  lineed_2_(0),

  checkbox_(0)
{
  QGridLayout* l = new QGridLayout;
  this->setLayout(l);

  QGridLayout* g0 = new QGridLayout;
  l->addLayout(g0, 0, 0);

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
  imageView_2_->setZoomFactor(1.0);

  QApplication::processEvents();    

  imageView_2_->connectImageProducer(this, SIGNAL(graph_found(const cv::Mat&)));

  scrollArea_2_ = new QScrollArea(this);
  scrollArea_2_->setMinimumSize(200, 200);
  scrollArea_2_->setPalette(palette);
  scrollArea_2_->setBackgroundRole(QPalette::Background);
  scrollArea_2_->setAlignment(Qt::AlignCenter);
  scrollArea_2_->setWidget(imageView_2_);
  scrollArea_2_->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

  g0->addWidget(scrollArea_2_, 0, 1);

  // left-hand side widgets
  QGridLayout*  g1 = new QGridLayout;
  g0->addLayout(g1, 1, 0);

  button_1_ = new QPushButton("Update Auto-Focus Parameters", this);
  g1->addWidget(button_1_, 1, 0);

  QFormLayout*  fl1 = new QFormLayout;
  g1->addLayout(fl1, 2, 0);

  QLabel* lt1 = new QLabel("Max Delta-Z [mm], # Steps", this);
  lineed_1_ = new QLineEdit("", this);
  fl1->addRow(lt1, lineed_1_);

  QLabel* lt2 = new QLabel("Z (best focus) [mm]", this);
  lineed_2_ = new QLineEdit("", this);
  fl1->addRow(lt2, lineed_2_);

  // -----------------------

  // right-hand side widgets
  QFormLayout*  f2 = new QFormLayout;
  g0->addLayout(f2, 1, 1);

  button_2_ = new QPushButton("Go to focal point", this);
  f2->addRow(button_2_);

  checkbox_ = new QCheckBox("Track marker", this);
  f2->addRow(checkbox_);

  // -----------------------

  // connection(s)
  connect(button_1_, SIGNAL(clicked()), this, SLOT(configure_scan()));
  connect(button_2_, SIGNAL(clicked()), this, SLOT(go_to_focal_point()));
  // -----------------------

  NQLog("AssemblyAutoFocusView", NQLog::Debug) << "constructed";
}

AssemblyAutoFocusView::~AssemblyAutoFocusView()
{
}

void AssemblyAutoFocusView::update_scan_config(const double zrange, const int points)
{
  const std::string str = std::to_string(zrange)+","+std::to_string(points);

  std::stringstream scan_strs;
  scan_strs << zrange << ", " << points;

  lineed_1_->setText(QString::fromStdString(scan_strs.str()));

  return;
}

void AssemblyAutoFocusView::configure_scan()
{
  // parse lineEdit text to get target coordinates
  const QString parent_string = lineed_1_->text();

  const QStringList pieces = parent_string.split(",");

  if(pieces.length() == 2)
  {
    const QString x = pieces.value(0);
    const QString y = pieces.value(1);

    bool valid_x_d(false);
    bool valid_y_d(false);

    const double x_d = x.toDouble(&valid_x_d);
    const double y_d = y.toInt   (&valid_y_d);

    if(!valid_x_d)
    {
      NQLog("AssemblyAutoFocusView", NQLog::Warning) << "configure_scan"
         << ": invalid format for scan-config parameter #1 (" << x << "), no action taken";

      return;
    }
    else if(!valid_y_d)
    {
      NQLog("AssemblyAutoFocusView", NQLog::Warning) << "configure_scan"
         << ": invalid format for scan-config parameter #2 (" << y << "), no action taken";

      return;
    }

    NQLog("AssemblyAutoFocusView", NQLog::Spam) << "configure_scan"
       << ": emitting signal \"scan_config(" << x_d << ", " << y_d << "\")";

    emit scan_config(x_d, y_d);
  }
  else
  {
    NQLog("AssemblyAutoFocusView", NQLog::Warning) << "configure_scan"
       << ": invalid input string format, no action taken";

    return;
  }
}

void AssemblyAutoFocusView::read_graph(const QString& img_path)
{
  if(Util::IsFile(img_path))
  {
    const cv::Mat img = cv::imread(img_path.toStdString(), CV_LOAD_IMAGE_COLOR);

    NQLog("AssemblyAutoFocusView", NQLog::Spam) << "read_graph"
       << ": emitting signal \"graph_found\"";

    emit graph_found(img);
  }
  else
  {
    NQLog("AssemblyAutoFocusView", NQLog::Warning) << "read_graph"
       << ": invalid path to input file, no action taken (file=" << img_path << ")";

    return;
  }
}

void AssemblyAutoFocusView::updateText(const double z)
{
  lineed_2_->setText(QString::fromStdString(std::to_string(z)));

  NQLog("AssemblyAutoFocusView", NQLog::Spam) << "updateText"
     << ": displayed value of best z-position (focal point)";

  return;
}

void AssemblyAutoFocusView::go_to_focal_point()
{
  NQLog("AssemblyAutoFocusView", NQLog::Warning) << "go_to_focal_point"
     << ": to be implemented, no action taken";

  return;    
}

void AssemblyAutoFocusView::connectImageProducer(const QObject* sender, const char* signal)
{
  NQLog("AssemblyAutoFocusView", NQLog::Debug) << "connectImageProducer";

  imageView_1_->connectImageProducer(sender, signal);

  return;
}

void AssemblyAutoFocusView::disconnectImageProducer(const QObject* sender, const char* signal)
{
  NQLog("AssemblyAutoFocusView", NQLog::Debug) << "disconnectImageProducer";

  imageView_1_->disconnectImageProducer(sender, signal);

  return;
}

void AssemblyAutoFocusView::keyReleaseEvent(QKeyEvent* event)
{
  NQLog("AssemblyAutoFocusView", NQLog::Debug) << "keyReleaseEvent";

  if(!(event->modifiers() & Qt::ShiftModifier))
  {
    switch(event->key())
    {
      case Qt::Key_0:
        imageView_1_->setZoomFactor(0.25);
        event->accept();
        break;

      case Qt::Key_1:
        imageView_1_->setZoomFactor(1.00);
        event->accept();
        break;

      case Qt::Key_Plus:
        imageView_1_->increaseZoomFactor();
        event->accept();
        break;

      case Qt::Key_Minus:
        imageView_1_->decreaseZoomFactor();
        event->accept();
        break;

      default:
        break;
    }
  }

  return;
}
