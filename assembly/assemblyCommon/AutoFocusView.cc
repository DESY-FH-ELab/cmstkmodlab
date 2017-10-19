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

#include <AutoFocusView.h>
#include <nqlogger.h>

//!!#include <vector>
//!!#include <iostream>
//!!#include <iomanip>

#include <QApplication>
#include <QFormLayout>
#include <QFileDialog>
#include <QPushButton>
#include <QString>
#include <QStringList>
#include <QPixmap>
#include <QLabel>

using namespace cv;//!!

AutoFocusView::AutoFocusView(AssemblyScanner* /* cmdr_zscan */, QWidget *parent) : QWidget(parent)
{
//  NQLog("AutoFocusView::AutoFocusView");

  QGridLayout* l = new QGridLayout(this);
  setLayout(l);

  QGridLayout* g0 = new QGridLayout();
  l->addLayout(g0, 0, 0);

  QPalette palette;
  palette.setColor(QPalette::Background, QColor(220, 220, 220));

  imageView_1 = new AssemblyUEyeView();
  imageView_1->setMinimumSize(200, 200);
  imageView_1->setPalette(palette);
  imageView_1->setBackgroundRole(QPalette::Background);
  imageView_1->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
  imageView_1->setScaledContents(true);
  imageView_1->setAlignment(Qt::AlignCenter);
  QApplication::processEvents();
//!!  imageView_1->connectImageProducer(cmdr_zscan, SIGNAL(updateScanImage(cv::Mat)));

  scrollArea_1 = new QScrollArea(this);
  scrollArea_1->setMinimumSize(200, 200);
  scrollArea_1->setPalette(palette);
  scrollArea_1->setBackgroundRole(QPalette::Background);
  scrollArea_1->setAlignment(Qt::AlignCenter);
  scrollArea_1->setWidget(imageView_1);
  scrollArea_1->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

  g0->addWidget(scrollArea_1,0,0);

  imageView_2 = new AssemblyUEyeView();
  imageView_2->setMinimumSize(200, 200);
  imageView_2->setPalette(palette);
  imageView_2->setBackgroundRole(QPalette::Background);
  imageView_2->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
  imageView_2->setScaledContents(true);
  imageView_2->setAlignment(Qt::AlignCenter);
  imageView_2->setZoomFactor(1.0);

  QApplication::processEvents();    
  imageView_2->connectImageProducer(this, SIGNAL(graph_made(const cv::Mat&)));

  scrollArea_2 = new QScrollArea(this);
  scrollArea_2->setMinimumSize(200, 200);
  scrollArea_2->setPalette(palette);
  scrollArea_2->setBackgroundRole(QPalette::Background);
  scrollArea_2->setAlignment(Qt::AlignCenter);
  scrollArea_2->setWidget(imageView_2);
  scrollArea_2->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

  g0->addWidget(scrollArea_2, 0, 1);

  lE1 = new QLineEdit("0.1, 10");
  g0->addWidget(lE1,1,0);

  button1 = new QPushButton("AutoFocus (step distance (mm), number of steps)", this);
  g0->addWidget(button1, 2, 0);

  checkbox = new QCheckBox("Track marker", this);
  g0->addWidget(checkbox, 3, 0);

  QFormLayout *g1 = new QFormLayout();
  g0->addLayout(g1, 1, 1);

  button2 = new QPushButton("Go to focal point", this);
  g1->addRow(button2);

  lE2 = new QLineEdit("Absolute focal point = ");
  g1->addRow(lE2);

  //make all the neccessary connections
  connect(button1, SIGNAL(clicked())            , this      , SLOT(configure_scan()));
//!!  connect(this   , SIGNAL(run_scan(double, int)), cmdr_zscan, SLOT(run_scan(double, int)));
  connect(button2, SIGNAL(clicked())            , this      , SLOT(go_to_focal_point()));

  return;    
}

void AutoFocusView::update_autofocus_parameters()
{
  //parse lineEdit text to get target coordinates
  QString parent_string = lE1->text();

  QStringList pieces = parent_string.split( "," );
  QString x = pieces.value(pieces.length() - 2);
  QString y = pieces.value(pieces.length() - 1);

  double x_d = x.toDouble();
  double y_d = y.toInt();

  NQLog("AutoFocusView::configure_scan(),  Scan range  = ") << x_d << ",   "<< y_d ;

  emit autofocus_parameters(x_d, y_d);

  return;
}

void AutoFocusView::make_graph(const std::string img_name)
{
  cv::Mat img = cv::imread(img_name, CV_LOAD_IMAGE_COLOR);
  emit graph_made(img);

  return;
}

void AutoFocusView::updateText(double z)
{
  NQLog("AutoFocusView::updateText");

  const QString qstr = "Absolute focal point (measured)  = "+QString::fromStdString(std::to_string(z));

  lE2->setText(qstr);

  return;
}

void AutoFocusView::go_to_focal_point(){

  NQLog("AutoFocusView::go_to_focal_point()");

  return;    
}

void AutoFocusView::updateImage(int stage, std::string filename)
{
  NQLog("AutoFocusView") << ":updateImage()  " + filename;

  cv::Mat img_gs = cv::imread(filename, CV_LOAD_IMAGE_COLOR);

  if(stage == 1){

    imageView_1->setZoomFactor(0.2);
    imageView_1->setImage(img_gs);    
  }

  return;
}

void AutoFocusView::connectImageProducer(const QObject* sender, const char* signal)
{
  NQLog("AutoFocusView") << ":connectImageProducer";

  imageView_1->connectImageProducer(sender, signal);

  connect(sender, signal, this, SLOT(imageAcquired(const cv::Mat&)));

  return;
}

void AutoFocusView::disconnectImageProducer(const QObject* sender, const char* signal)
{
  NQLog("AutoFocusView") << ":disconnectImageProducer";

  imageView_1->disconnectImageProducer(sender, signal);

  disconnect(sender, signal, this, SLOT(imageAcquired(const cv::Mat&)));

  return;
}

void AutoFocusView::imageAcquired(const cv::Mat& newImage)
{
  NQLog("AutoFocusView") << ":imageAcquired";

  newImage.copyTo(image_);

  return;
}

void AutoFocusView::keyReleaseEvent(QKeyEvent * event)
{

  if(!(event->modifiers() & Qt::ShiftModifier)){

    switch(event->key()){

      case Qt::Key_0:
        imageView_1->setZoomFactor(0.25);
        event->accept();
        break;

      case Qt::Key_1:
        imageView_1->setZoomFactor(1.00);
        event->accept();
        break;

      case Qt::Key_Plus:
        imageView_1->increaseZoomFactor();
        event->accept();
        break;

      case Qt::Key_Minus:
        imageView_1->decreaseZoomFactor();
        event->accept();
        break;

      default:
        break;
    }
  }

  return;
}
