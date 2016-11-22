#include <vector>
#include <iostream>
#include <sstream>
#include <iomanip>

#include <QFormLayout>
#include <QFileDialog>
#include <QPushButton>
#include <QString>
#include <QStringList>
#include <QPixmap>
#include <QLabel>
#include <QApplication>

#include <TGraph.h>
#include <TCanvas.h>

#include <nqlogger.h>
#include <ApplicationConfig.h>

#include <QDir>
#if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
#include <QDesktopServices>
#else
#include <QStandardPaths>
#endif

#include "AssemblyModuleAssembler.h"

using namespace std;
using namespace cv;

AssemblyModuleAssembler::AssemblyModuleAssembler(AssemblyVUEyeModel *uEyeModel_,
                                                 LStepExpressModel* lStepExpressModel_,
                                                 LStepExpressMotionManager* manager_,
                                                 ConradModel * conradModel_,
                                                 QWidget *parent)
  : QWidget(parent)
{
  LStepExpressMotionManager* motionManager_ = new LStepExpressMotionManager(lStepExpressModel_);

  QGridLayout *l = new QGridLayout(this);
  setLayout(l);

  QGridLayout *g0 = new QGridLayout(this);
  l->addLayout(g0,0,0);

  QPalette palette;
  palette.setColor(QPalette::Background, QColor(220, 220, 220));
    
  finder_ = new AssemblySensorMarkerFinder();

  imageView_1 = new AssemblyUEyeView();
  imageView_1->setMinimumSize(200,200);
  imageView_1->setPalette(palette);
  imageView_1->setBackgroundRole(QPalette::Background);
  imageView_1->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
  imageView_1->setScaledContents(true);
  imageView_1->setAlignment(Qt::AlignCenter);
  QApplication::processEvents();
    
  imageView_1->connectImageProducer(finder_, SIGNAL(markerFound(const cv::Mat&)));

  connect(uEyeModel_, SIGNAL(imageAcquired(const cv::Mat&)),
            finder_, SLOT(findMarker(const cv::Mat&)));

  scrollArea_1 = new QScrollArea(this);
  scrollArea_1->setMinimumSize(200, 200);
  scrollArea_1->setPalette(palette);
  scrollArea_1->setBackgroundRole(QPalette::Background);
  scrollArea_1->setAlignment(Qt::AlignCenter);
  scrollArea_1->setWidget(imageView_1);
  scrollArea_1->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

  g0->addWidget(scrollArea_1,0,0);
  lE1 = new QLineEdit("Pickup pos. = X,X,X");
  g0->addWidget(lE1,1,0);

  imageView_2 = new AssemblyUEyeView();
  imageView_2->setMinimumSize(200,200);
  imageView_2->setPalette(palette);
  imageView_2->setBackgroundRole(QPalette::Background);
  imageView_2->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
  imageView_2->setScaledContents(true);
  imageView_2->setAlignment(Qt::AlignCenter);

  scrollArea_2 = new QScrollArea(this);
  scrollArea_2->setMinimumSize(200, 200);
  scrollArea_2->setPalette(palette);
  scrollArea_2->setBackgroundRole(QPalette::Background);
  scrollArea_2->setAlignment(Qt::AlignCenter);
  scrollArea_2->setWidget(imageView_2);
  scrollArea_2->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

  g0->addWidget(scrollArea_2,0,1);
  lE2 = new QLineEdit("Mntd pos. (cor. 1) = X,X,X");
  g0->addWidget(lE2,1,1);

  imageView_3 = new AssemblyUEyeView();
  imageView_3->setMinimumSize(200,200);
  imageView_3->setPalette(palette);
  imageView_3->setBackgroundRole(QPalette::Background);
  imageView_3->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
  imageView_3->setScaledContents(true);
  imageView_3->setAlignment(Qt::AlignCenter);

  scrollArea_3 = new QScrollArea(this);
  scrollArea_3->setMinimumSize(200, 200);
  scrollArea_3->setPalette(palette);
  scrollArea_3->setBackgroundRole(QPalette::Background);
  scrollArea_3->setAlignment(Qt::AlignCenter);
  scrollArea_3->setWidget(imageView_3);
  scrollArea_3->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

  g0->addWidget(scrollArea_3,0,2);
  lE3 = new QLineEdit("Mntd pos. (cor. 2) = X,X,X");
  g0->addWidget(lE3,1,2);

  imageView_4 = new AssemblyUEyeView();
  imageView_4->setMinimumSize(200,200);
  imageView_4->setPalette(palette);
  imageView_4->setBackgroundRole(QPalette::Background);
  imageView_4->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
  imageView_4->setScaledContents(true);
  imageView_4->setAlignment(Qt::AlignCenter);

  scrollArea_4 = new QScrollArea(this);
  scrollArea_4->setMinimumSize(200, 200);
  scrollArea_4->setPalette(palette);
  scrollArea_4->setBackgroundRole(QPalette::Background);
  scrollArea_4->setAlignment(Qt::AlignCenter);
  scrollArea_4->setWidget(imageView_4);
  scrollArea_4->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

  g0->addWidget(scrollArea_4,2,0);
  lE4 = new QLineEdit("Dtchd pos. = X,X,X");
  g0->addWidget(lE4,3,0);

  imageView_5 = new AssemblyUEyeView();
  imageView_5->setMinimumSize(200,200);
  imageView_5->setPalette(palette);
  imageView_5->setBackgroundRole(QPalette::Background);
  imageView_5->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
  imageView_5->setScaledContents(true);
  imageView_5->setAlignment(Qt::AlignCenter);

  scrollArea_5 = new QScrollArea(this);
  scrollArea_5->setMinimumSize(200, 200);
  scrollArea_5->setPalette(palette);
  scrollArea_5->setBackgroundRole(QPalette::Background);
  scrollArea_5->setAlignment(Qt::AlignCenter);
  scrollArea_5->setWidget(imageView_5);
  scrollArea_5->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

  g0->addWidget(scrollArea_5,2,1);
  lE5 = new QLineEdit("Mntd pos. (cor. 4) = X,X,X");
  g0->addWidget(lE5,3,1);

  imageView_6 = new AssemblyUEyeView();
  imageView_6->setMinimumSize(200,200);
  imageView_6->setPalette(palette);
  imageView_6->setBackgroundRole(QPalette::Background);
  imageView_6->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
  imageView_6->setScaledContents(true);
  imageView_6->setAlignment(Qt::AlignCenter);

  scrollArea_6 = new QScrollArea(this);
  scrollArea_6->setMinimumSize(200, 200);
  scrollArea_6->setPalette(palette);
  scrollArea_6->setBackgroundRole(QPalette::Background);
  scrollArea_6->setAlignment(Qt::AlignCenter);
  scrollArea_6->setWidget(imageView_6);
  scrollArea_6->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

  g0->addWidget(scrollArea_6,2,2);
  lE6 = new QLineEdit("Mntd pos. (cor. 3) = X,X,X");
  g0->addWidget(lE6,3,2);

  QGridLayout *g1 = new QGridLayout(this);
  l->addLayout(g1,0,1);

  QLabel* header_label = new QLabel("Hover mouse over button for info on each step", this);
  g1->addWidget(header_label,0,0);

  AssemblyCommander * cmdr0 = new AssemblyCommander(this, "Go to origin", 0.0,0.0,0.0,0.0);
  cmdr0->setToolTip("(1) Returns x,y,z stage to origin (default = (0,0,0) using moveAbsolute(x,y,z) routine)");
  g1->addWidget(cmdr0,1,0);

  AssemblyCommander * cmdr1 = new AssemblyCommander(this, "Go to pickup", 100.0,100.0,100.0,100.0);
  cmdr1->setToolTip("(2) Moves x,y,z stage to pickup position (default = (100,100,100) using moveAbsolute(x,y,z) routine)");
  g1->addWidget(cmdr1,2,0);

  AssemblySensorLocator * lctr1 = new AssemblySensorLocator(this, "Locate sensor", 0.0, finder_);
  lctr1->setToolTip("(3) Acquires image from mobile camera, runs PatRec routine to deduce and report sensor (x,y,z,phi) postion");
  g1->addWidget(lctr1,3,0);

  AssemblyCommander * cmdr2 = new AssemblyCommander(this, "Correct position", 100.0,100.0,100.0,100.0);
  cmdr2->setToolTip("(4) Corrects arm position using relative displacement using (eventually) pre-calculated displacment from PatRec");
  g1->addWidget(cmdr2,4,0);

  AssemblyAttacher * attacher1 = new AssemblyAttacher("Drop/Raise", 10.0);
  g1->addWidget(attacher1,5,0);

  AssemblyVacuumToggler * toggle1 = new AssemblyVacuumToggler(this, "Toggle vacuum", 0.0);
  g1->addWidget(toggle1,6,0);

  AssemblyCommander * cmdr4 = new AssemblyCommander(this, "Go to stat. camera", 100.0,100.0,100.0,100.0);
  g1->addWidget(cmdr4, 7, 0);

  AssemblyMountChecker * cmdr5 = new AssemblyMountChecker(this, "Check mount", 100.0,100.0,100.0,100.0, 0);
  g1->addWidget(cmdr5, 8, 0);

  AssemblyCommander * cmdr6 = new AssemblyCommander(this, "Go to rotation stage", 100.0,100.0,100.0,100.0);
  g1->addWidget(cmdr6, 9, 0);

  AssemblyCommander * cmdr7 = new AssemblyCommander(this, "Drop and detach", 100.0,100.0,100.0,100.0);
  g1->addWidget(cmdr7, 10, 0);

  AssemblyAligner * cmdr8 = new AssemblyAligner(this, "Align", 0.0);
  g1->addWidget(cmdr8, 11, 0);

  //make all the neccessary connections
  connect(attacher1, SIGNAL(moveRelative(double,double,double,double)),
          motionManager_, SLOT(moveRelative(double,double,double,double)));
  connect(cmdr0, SIGNAL(moveAbsolute(double,double,double,double)),
          motionManager_, SLOT(moveAbsolute(double,double,double,double)));
  connect(cmdr1, SIGNAL(moveAbsolute(double,double,double,double)),
          motionManager_, SLOT(moveAbsolute(double,double,double,double)));
  connect(cmdr2, SIGNAL(moveAbsolute(double,double,double,double)),
          motionManager_, SLOT(moveRelative(double,double,double,double)));
  connect(cmdr4, SIGNAL(moveAbsolute(double,double,double,double)),
          motionManager_, SLOT(moveAbsolute(double,double,double,double)));
  connect(cmdr5, SIGNAL(moveAbsolute(double,double,double,double)),
          motionManager_, SLOT(moveAbsolute(double,double,double,double)));
  connect(cmdr6, SIGNAL(moveAbsolute(double,double,double,double)),
          motionManager_, SLOT(moveAbsolute(double,double,double,double)));
  connect(cmdr7, SIGNAL(moveAbsolute(double,double,double,double)),
          motionManager_, SLOT(moveAbsolute(double,double,double,double)));
  connect(cmdr8, SIGNAL(moveRelative(double,double,double,double)),
          motionManager_, SLOT(moveRelative(double,double,double,double)));
  connect(cmdr8, SIGNAL(locateSetdowncorner(int)), lctr1, SLOT( locateSensor(int)));

  connect(lctr1, SIGNAL(sendPosition(int, double,double,double)), this, SLOT(updateText(int,double,double,double)));

  connect(lctr1, SIGNAL(updateImage(int, std::string)), this, SLOT( updateImage(int,std::string)));
  

  connect(lctr1, SIGNAL(foundSensor(int)), lctr1, SLOT( foundsensor(int)));

    //now patrec method in assemblymarkerfinder sends the processed image back to the view
    //should replace connection above
    connect(finder_, SIGNAL(updateImage(int, std::string)), this, SLOT( updateImage(int,std::string)));
    connect(finder_, SIGNAL(foundSensor(int)), lctr1, SLOT( foundsensor(int)));


}

void AssemblyModuleAssembler::updateText(int stage, double x, double y , double a){

  NQLog("AssemblyCommander::updateText");

  std::ostringstream strs;
  strs.clear();
  strs << x;
  strs << ",";
  strs << y;
  strs << ",";
  strs << a;
  std::string str = strs.str();
  QString qstr = "";
  qstr = QString::fromStdString(str);
  // QString qname = QString::fromStdString(string);

  if(stage == 1 ){
    qstr = "Pickup pos. = " + qstr;
    lE1->setText(qstr);
  } else if (stage==2){
    qstr = "Mntd. pos. (cor.1) = " + qstr;
    lE2->setText(qstr);
  }
  else if (stage==3){
    qstr = "Mntd. pos. (cor.2) = " + qstr;
    lE3->setText(qstr);
  }
  else if (stage==4){
    qstr = "Mntd. pos. (cor.3) = " + qstr;
    lE6->setText(qstr);
  }else if (stage==5){
    qstr = "Mntd. pos. (cor.4) = " + qstr;
    lE5->setText(qstr);
  }
  else if (stage==6){
    qstr = "Dtchd. pos. = " + qstr;
    lE4->setText(qstr);
  }
}

void AssemblyModuleAssembler::updateImage(int stage, std::string filename)
{
  NQLog("AssemblyModuleAssembler") << ":updateImage()  " + filename;

  //cv::Mat img_gs = cv::imread("/Users/keaveney/Desktop/calibration/PatRec_result.png", CV_LOAD_IMAGE_COLOR);

  cv::Mat img_gs = cv::imread(filename, CV_LOAD_IMAGE_COLOR);

  if (stage == 1 ){
    imageView_1->setZoomFactor(0.2);
    imageView_1->setImage(img_gs);
    
  }else if (stage ==2){
    imageView_2->setImage(img_gs);
    imageView_2->setZoomFactor(0.75);
  }
  else if (stage ==3){
    imageView_3->setImage(img_gs);
  }
  else if (stage ==4){
    imageView_6->setImage(img_gs);
  }
  else if (stage ==5){
    imageView_5->setImage(img_gs);
  }
  else if (stage ==6){
    imageView_4->setImage(img_gs);
  }
}

void AssemblyModuleAssembler::gotoPickup()
{
  NQLog("AssemblyModuleAssembler") << ":gotoPickup()";
  //connect(this, SIGNAL(moveAbsolute(double,double,double,double)), motionManager_, SLOT(moveAbsolute(double,double,double,double)));
}

void AssemblyModuleAssembler::connectImageProducer(const QObject* sender,
                                                   const char* signal)
{
  NQLog("AssemblyModuleAssembler") << ":connectImageProducer";

  imageView_1->connectImageProducer(sender, signal);

  connect(sender, signal,
          this, SLOT(imageAcquired(const cv::Mat&)));
}

void AssemblyModuleAssembler::disconnectImageProducer(const QObject* sender,
                                                      const char* signal)
{
  NQLog("AssemblyModuleAssembler") << ":disconnectImageProducer";

  imageView_1->disconnectImageProducer(sender, signal);

  disconnect(sender, signal,
             this, SLOT(imageAcquired(const cv::Mat&)));
}

void AssemblyModuleAssembler::snapShot()
{
   NQLog("AssemblyModuleAssembler") << ":snapShot()";

  if (image_.rows==0) return;

  QString filename = QFileDialog::getSaveFileName(this, "save image", ".", "*.png");
  if (filename.isNull() || filename.isEmpty()) return;

  if (!filename.endsWith(".png")) filename += ".png";

  cv::imwrite(filename.toStdString(), image_);
}

void AssemblyModuleAssembler::imageAcquired(const cv::Mat& newImage)
{
  NQLog("AssemblyModuleAssembler") << ":imageAcquired";

  newImage.copyTo(image_);
}

void AssemblyModuleAssembler::keyReleaseEvent(QKeyEvent * event)
{
  if (!(event->modifiers() & Qt::ShiftModifier)) {
    switch (event->key()) {
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
}

AssemblyVacuumToggler::AssemblyVacuumToggler(QWidget *parent, std::string string, double a)
: QWidget(parent),local_a(a)
{
  QGridLayout *l = new QGridLayout(this);
  setLayout(l);

  cnrd1 = new ConradModel(parent);

  std::ostringstream strs;
  strs.clear();
  strs << a;
  std::string str = strs.str();
  QString qstr = QString::fromStdString(str);
  QString qname = QString::fromStdString(string);

  state = false;

  button1 = new QPushButton(qname, this);
  l->addWidget(button1,0,0);

  ql = new QLabel("", this);
  l->addWidget(ql,0,1);

  QPixmap pixmap(100,100);
  pixmap.fill(QColor("transparent"));

  QPainter painter(&pixmap);
  painter.setBrush(QBrush(Qt::red));
  painter.drawEllipse(0, 0, 30, 30);

  ql->setPixmap(pixmap);
  ql->setText("VACUUM OFF");
  ql->setStyleSheet("QLabel { background-color : green; color : black; }");

  connect(button1, SIGNAL(clicked()),
          this, SLOT(toggleVacuum()));

}

void AssemblyVacuumToggler::toggleVacuum()
{
  NQLog("AssemblyVacuumToggler") << ": toggling vacuum voltage";

  if (!state){
    cnrd1->setSwitchEnabled(1, true);
    
    if (cnrd1->getSwitchState(1) == 1){
      ql->setText("VACUUM ON");
      ql->setStyleSheet("QLabel { background-color : red; color : black; }");
      state = true;
    }
  }else if (state){
    cnrd1->setSwitchEnabled(1, false);
    
    if (cnrd1->getSwitchState(1) == 0){
      ql->setText("VACUUM OFF");
      ql->setStyleSheet("QLabel { background-color : green; color : black; }");
      state = false;
    }
  }
}

AssemblyAttacher::AssemblyAttacher(std::string string, double drop)
: local_drop(drop)
{
  QFormLayout *ll = new QFormLayout(this);
  setLayout(ll);


  std::ostringstream strs;
  strs.clear();
  strs << drop;
  std::string str = strs.str();
  QString qstr = QString::fromStdString(str);
  QString qname = QString::fromStdString(string);


  button1 = new QPushButton(qname, this);

  lineEdit1 = new QLineEdit();
  lineEdit1->setText(qstr);
  ll->addRow(button1,lineEdit1);

  connect(button1, SIGNAL(clicked()),
          this, SLOT(dropAttach()));
}

void AssemblyAttacher::dropAttach(){

  //parse lineEdit text to get target coordinates
  QString  parent_string = this->lineEdit1->text();

  double d_d = parent_string.toDouble();

  NQLog("AssemblyUEyeCameraSettingsMotionInterface") <<" requesting move... ";

  emit moveRelative(0.0,0.0,-d_d, 0.0);

  NQLog("AssemblyUEyeCameraSettingsMotionInterface") <<"move requested ";
}

AssemblyMountChecker::AssemblyMountChecker(QWidget *parent, std::string string,
                                           double x ,double y, double z,
                                           double a, int mode)
: QWidget(parent), local_x(x), local_y(y),local_z(z),local_a(a)
{
  QFormLayout *l = new QFormLayout(this);
  setLayout(l);

  NQLog("AssemblyMountChecker") << ": in mode" << mode;

  std::ostringstream strs;
  strs.clear();
  strs << x;
  strs << ",";
  strs << y;
  strs << ",";
  strs << z;
  std::string str = strs.str();
  QString qstr = QString::fromStdString(str);
  QString qname = QString::fromStdString(string);

  this->local_x = x;
  this->local_y = y;
  this->local_z = z;
  this->local_a = a;

  button1 = new QPushButton(qname, this);

  lineEdit1 = new QLineEdit();
  lineEdit1->setText(qstr);
  l->addRow(button1,lineEdit1);

  connect(button1, SIGNAL(clicked()),
          this, SLOT(checkMount()));
}

void AssemblyMountChecker::checkMount()
{
  //parse lineEdit text to get target coordinates
  QString  parent_string = this->lineEdit1->text();

  QStringList pieces = parent_string.split( "," );
  QString x = pieces.value( pieces.length() - 3);
  QString y = pieces.value( pieces.length() - 2);
  QString z = pieces.value( pieces.length() - 1);
  QString a = pieces.value( pieces.length() - 1);

  double x_d = x.toDouble();
  double y_d = y.toDouble();
  double z_d = z.toDouble();
  double a_d = a.toDouble();

  NQLog("AssemblyMountChecker") << ": going to target (parsed) "<< x_d<<" "<< y_d<<"  "<< z_d;

  emit moveAbsolute(x_d, y_d, z_d, a_d);// this should bring you to the first corner
  emit locateCorner(2);
  emit reportCornerLocation(1);
  emit moveRelative(0.0, 10.0, 0.0, 0.0);//move to next corner
  emit locateCorner(3);
  emit reportCornerLocation(2);
  emit moveRelative(5.0, 0.0, 0.0, 0.0);//move to next corner
  emit locateCorner(4);
  emit reportCornerLocation(3);
  emit moveRelative(0.0, -10.0, 0.0, 0.0);//move to next corner
  emit locateCorner(5);
  emit reportCornerLocation(4);
}

AssemblyCommander::AssemblyCommander(QWidget *parent, std::string string,
                                     double x ,double y, double z,
                                     double a)
: QWidget(parent), local_x(x), local_y(y),local_z(z),local_a(a)
{
  QFormLayout *l = new QFormLayout(this);
  setLayout(l);

  std::ostringstream strs;
  strs.clear();
  strs << x;
  strs << ",";
  strs << y;
  strs << ",";
  strs << z;
  std::string str = strs.str();
  QString qstr = QString::fromStdString(str);
  QString qname = QString::fromStdString(string);

  this->local_x = x;
  this->local_y = y;
  this->local_z = z;
  this->local_a = a;

  button1 = new QPushButton(qname, this);

  lineEdit1 = new QLineEdit();
  lineEdit1->setText(qstr);
  l->addRow(button1,lineEdit1);

  connect(button1, SIGNAL(clicked()),
          this, SLOT(goToTarget()));
}

void AssemblyCommander::goToTarget()
{
  //parse lineEdit text to get target coordinates
  QString  parent_string = this->lineEdit1->text();

  QStringList pieces = parent_string.split( "," );
  QString x = pieces.value( pieces.length() - 3);
  QString y = pieces.value( pieces.length() - 2);
  QString z = pieces.value( pieces.length() - 1);
  QString a = pieces.value( pieces.length() - 1);

  double x_d = x.toDouble();
  double y_d = y.toDouble();
  double z_d = z.toDouble();
  double a_d = a.toDouble();

  NQLog("AssemblyCommander:goToTarget") << ": going to target (parsed) "<< x_d<<" "<< y_d<<"  "<< z_d;

  NQLog("AssemblyCommander:goToTarget") <<" requesting move...";

  emit moveAbsolute(x_d, y_d, z_d, a_d);

  NQLog("AssemblyCommander:goToTarget") <<"move requested...";
}

AssemblyAligner::AssemblyAligner(QWidget *parent, std::string string,
                                 double a)
  : QWidget(parent),local_a(a)
{
  QFormLayout *l = new QFormLayout(this);
  setLayout(l);

  std::ostringstream strs;
  strs.clear();
  strs << a;
  std::string str = strs.str();
  QString qstr = QString::fromStdString(str);
  QString qname = QString::fromStdString(string);

  button1 = new QPushButton(qname, this);

  lineEdit1 = new QLineEdit();
  lineEdit1->setText(qstr);
  l->addRow(button1,lineEdit1);

  connect(button1, SIGNAL(clicked()),
          this, SLOT(setDown()));
}

void AssemblyAligner::setDown()
{
  emit locateSetdowncorner(6);

}

void AssemblyAligner::align()
{
  //     emit locate(6);

  //parse lineEdit text to get target coordinates
  QString  parent_string = this->lineEdit1->text();

  //  QStringList pieces = parent_string.split( "," );
  //  QString x = pieces.value( pieces.length() - 3);
  //  QString y = pieces.value( pieces.length() - 2);
  //  QString z = pieces.value( pieces.length() - 1);
  //  QString a = pieces.value( pieces.length() - 1);

  // double x_d = x.toDouble();
  // double y_d = y.toDouble();
  // double z_d = z.toDouble();
  double a_d = parent_string.toDouble();

  NQLog("AssemblyAligner:align") << ": going to target alignment (parsed) "<< a_d<<" ";

  emit moveRelative(0.0, 0.0, 0.0, a_d);
}

AssemblySensorLocator::AssemblySensorLocator(QWidget *parent, std::string string,
                                             double a, AssemblySensorMarkerFinder * finder_ )
 : QWidget(parent), local_a(a)
{
  std::ostringstream strs;
  strs.clear();
  strs << a;
  std::string str = strs.str();
  QString qstr = QString::fromStdString(str);
  QString qname = QString::fromStdString(string);

#if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
  QString cachedirTemp = QDesktopServices::storageLocation(QDesktopServices::CacheLocation);
#else
  QString cachedirTemp = QStandardPaths::writableLocation(QStandardPaths::CacheLocation);
#endif
  cachedirTemp += "/assembly/calibration";
  QDir dir1(cachedirTemp);
  if (!dir1.exists()) dir1.mkpath(".");
  cacheDirectory1_ = cachedirTemp.toStdString();
  cachedirTemp += "/RotatedImages";
  QDir dir2(cachedirTemp);
  if (!dir2.exists()) dir2.mkpath(".");
  cacheDirectory2_ = cachedirTemp.toStdString();

  QGridLayout *l = new QGridLayout(this);
  setLayout(l);

  button1 = new QPushButton(qname, this);

  l->addWidget(button1,0,0);
    
  groupBox1 = new QGroupBox(tr("Pattern Recognition Method"));
  groupBox2 = new QGroupBox(tr("Mode"));
    
  radio1 = new QRadioButton(tr("&Circle Seed Algorithm"));
  radio2 = new QRadioButton(tr("T&emplate Matching"));
  radio2->setChecked(true);

  vbox1 = new QVBoxLayout;
  vbox1->addWidget(radio1);
  vbox1->addWidget(radio2);
  vbox1->addStretch(1);
  groupBox1->setLayout(vbox1);
    
  radio3 = new QRadioButton(tr("&Demo"));
  radio4 = new QRadioButton(tr("&Lab"));
  radio3->setChecked(true);
    
  vbox2 = new QVBoxLayout;
  vbox2->addWidget(radio3);
  vbox2->addWidget(radio4);
  vbox2->addStretch(1);
  groupBox2->setLayout(vbox2);

  l->addWidget(groupBox1,1,0);
  l->addWidget(groupBox2,1,1);

  ql = new QLabel("", this);
  l->addWidget(ql,0,1);

  QPixmap pixmap(100,100);
  pixmap.fill(QColor("transparent"));

  QPainter painter(&pixmap);
  painter.setBrush(QBrush(Qt::red));
  painter.drawEllipse(0, 0, 30, 30);

  ql->setPixmap(pixmap);
  ql->setText("WAITING");
  ql->setStyleSheet("QLabel { background-color : orange; color : black; }");

  connect(button1, SIGNAL(clicked()),
          this, SLOT(locatePickup()));
  connect(this, SIGNAL(locatePickupCorner_circleSeed(int)),
          this, SLOT(locateSensor_circleSeed(int)));

  connect(this, SIGNAL(locatePickupCorner_templateMatching(int)),
             finder_, SLOT(findMarker_templateMatching(int)));
    
  connect(this, SIGNAL(locatePickupCorner_circleSeed(int)),
            finder_, SLOT(findMarker_circleSeed(int)));

}

void AssemblySensorLocator::locatePickup()
{
  if (radio1->isChecked()) {
    NQLog("AssemblySensorLocator") << "Circle seed selected" ;
      if(radio3->isChecked()){
       emit locatePickupCorner_circleSeed(0);
      } else if (radio4->isChecked()){
          emit locatePickupCorner_circleSeed(1);
      }
  }
  
  else if (radio2->isChecked()) {
    NQLog("AssemblySensorLocator") << "Template matching selected" ;
      if(radio3->isChecked()){
          emit locatePickupCorner_templateMatching(0);
      } else if(radio4->isChecked()){
          emit locatePickupCorner_templateMatching(1);
      }
  }
}



void AssemblySensorLocator::foundsensor(int state)
{
  NQLog("AssemblySensorLocator::foundsensor");

  if (state == 0) {
    ql->setText("WAITING");
    NQLog("AssemblySensorLocator::foundsensor")<< " 0  ";

    ql->setStyleSheet("QLabel { background-color : orange; color : black; }");
    
  } else if (state == 1) {
    
    NQLog("AssemblySensorLocator::foundsensor")<< " 1 ";
    
    ql->setText("FOUND MARKER");
    ql->setStyleSheet("QLabel { background-color : green; color : black; }");
    
  } else if (state == 2) {
    NQLog("AssemblySensorLocator::foundsensor")<< " 2 ";
    ql->setText("ERROR");
    ql->setStyleSheet("QLabel { background-color : red; color : black; }");
  }
}
