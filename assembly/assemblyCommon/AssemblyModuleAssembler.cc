#include <vector>

#include <QFormLayout>
#include <QFileDialog>
#include <QPushButton>
#include <QString>
#include <QStringList>
#include <QPixmap>
#include <QLabel>
#include <QApplication>


#include <iostream>


#include <nqlogger.h>

#include "AssemblyModuleAssembler.h"

using namespace std;

AssemblyModuleAssembler::AssemblyModuleAssembler(QWidget *parent)
    : QWidget(parent)
{
    QGridLayout *l = new QGridLayout(this);
    setLayout(l);

    QPalette palette;
    palette.setColor(QPalette::Background, QColor(220, 220, 220));

    imageView_ = new AssemblyUEyeView();
    imageView_->setMinimumSize(400,400);
    imageView_->setPalette(palette);
    imageView_->setBackgroundRole(QPalette::Background);
    imageView_->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    imageView_->setScaledContents(true);
    imageView_->setAlignment(Qt::AlignCenter);

    scrollArea_ = new QScrollArea(this);
    scrollArea_->setMinimumSize(200, 200);
    scrollArea_->setPalette(palette);
    scrollArea_->setBackgroundRole(QPalette::Background);
    scrollArea_->setAlignment(Qt::AlignCenter);
    scrollArea_->setWidget(imageView_);
    scrollArea_->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    l->addWidget(scrollArea_,0,0);
    

    QGridLayout *g1 = new QGridLayout(this);
    l->addLayout(g1,0,1);
    
//    QPushButton * button1 = new QPushButton("Toggle Vacuum", this);
//    g1->addWidget(button1, 0, 0);

    AssemblyVacuumToggler * toggle1 = new AssemblyVacuumToggler(this, "Toggle vacuum", 0.0, 0);
    g1->addWidget(toggle1,0,0);

    
  /*  QGridLayout *g2 = new QGridLayout(this);
    g1->addLayout(g2,0,0);*/
    

    AssemblyCommander * cmdr1 = new AssemblyCommander(this, "Go to pickup station", 100.0,100.0,100.0,100.0, 0);
    g1->addWidget(cmdr1,1,0);
    AssemblySensorLocator * lctr1 = new AssemblySensorLocator(this, "Locate sensor", 0.0, 0);
    g1->addWidget(lctr1,2,0);
    
    connect(lctr1, SIGNAL(updateImage(std::string)), this, SLOT( updateImage(std::string)));
    connect(lctr1, SIGNAL(foundSensor(int)), lctr1, SLOT( foundsensor(int)));

    AssemblyCommander * cmdr2 = new AssemblyCommander(this, "Correct position", 100.0,100.0,100.0,100.0, 0);
    g1->addWidget(cmdr2,3,0);
    
    connect(lctr1, SIGNAL(sendPosition(double, double, double)), cmdr2, SLOT(updateText(double, double, double)));
    
    
    AssemblyAttacher * attacher1 = new AssemblyAttacher(this, "Drop and attach", 10.0,0);
    g1->addWidget(attacher1,4,0);
    

    AssemblyCommander * cmdr4 = new AssemblyCommander(this, "Go to rotation stage", 100.0,100.0,100.0,100.0, 0);
    g1->addWidget(cmdr4, 5, 0);
    AssemblyCommander * cmdr5 = new AssemblyCommander(this, "Drop and detach", 100.0,100.0,100.0,100.0, 0);
    g1->addWidget(cmdr5, 6, 0);
    AssemblyAlligner * cmdr6 = new AssemblyAlligner(this, "Align", 0.0,0);
    g1->addWidget(cmdr6, 7, 0);
    
    //AssemblySensorLocator
    
    
  /*  QPushButton * button1 = new QPushButton("Go to pickup station (pickup position)", this);
    QLineEdit *lineEdit1 = new QLineEdit();
    lineEdit1->setText("100.0,100.0,100.0");
    l->addRow(button1,lineEdit1);

    QPushButton * button2 = new QPushButton("Drop and attach (drop distance)", this);
    QLineEdit *lineEdit2 = new QLineEdit();
    lineEdit2->setText("10.0");
    l->addRow(button2,lineEdit2);
    
    QPushButton * button3 = new QPushButton("Go to rotation stage (RS position)", this);
    QLineEdit *lineEdit3 = new QLineEdit();
    lineEdit3->setText("0.0,0.0,0.0");
    l->addRow(button3,lineEdit3);
    
    QPushButton * button4 = new QPushButton("Drop and detach (drop distance)", this);
    QLineEdit *lineEdit4 = new QLineEdit();
    lineEdit4->setText("10.0");
    l->addRow(button4,lineEdit4);
    
    QPushButton * button5 = new QPushButton("Align (target orientation)", this);
    QLineEdit *lineEdit5 = new QLineEdit();
    lineEdit5->setText("10.0");
    l->addRow(button5,lineEdit5);
    
    connect(button1, SIGNAL(clicked()), this, SLOT(gotoPickup()));

   */
    
    cv::Mat img_gs = cv::imread("/Users/keaveney/Desktop/calibration/im_scan___Exp10___EdgeThr145___lt110.png", CV_LOAD_IMAGE_COLOR);

    //imageView_->setImage(img_gs);
    
    
    QPixmap pixmap(100,100);
    pixmap.fill(QColor("transparent"));
    
    QPainter painter(&pixmap);
    painter.setBrush(QBrush(Qt::red));
    painter.drawEllipse(0, 0, 30, 30);
    
    imageView_->setPixmap(pixmap);
    imageView_->setStyleSheet("QLabel { background-color : orange; color : black; }");
    
    imageView_->setText("No image loaded");

    QApplication::processEvents();
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
}



void AssemblyModuleAssembler::updateImage(std::string filename)
{

    NQLog("AssemblyModuleAssembler") << ":updateImage()  " + filename;

    
    //cv::Mat img_gs = cv::imread("/Users/keaveney/Desktop/calibration/PatRec_result.png", CV_LOAD_IMAGE_COLOR);
    
    cv::Mat img_gs = cv::imread(filename, CV_LOAD_IMAGE_COLOR);
    
    imageView_->setImage(img_gs);

    
}


void AssemblyModuleAssembler::gotoPickup()
{
    NQLog("AssemblyModuleAssembler") << ":gotoPickup()";
    //emit updateStatus("Moving to pickup area",20.0);
    ApplicationConfig* config = ApplicationConfig::instance();
    LStepExpressModel* lStepExpressModel_ = new LStepExpressModel(config->getValue<std::string>("LStepExpressDevice").c_str(),1000, 100);
    LStepExpressSettings* lStepExpressSettings_ = new LStepExpressSettings(lStepExpressModel_);
    LStepExpressMotionManager* motionManager_ = new LStepExpressMotionManager(lStepExpressModel_);
    connect(this, SIGNAL(moveAbsolute(double,double,double,double)), motionManager_, SLOT(moveAbsolute(double,double,double,double)));
    
    lStepExpressModel_->initialize();
    //emit moveAbsolute(0.0,0.0,0.0,0.0);
    

}




void AssemblyModuleAssembler::connectImageProducer(const QObject* sender,
                                                   const char* signal)
{
    NQLog("AssemblyModuleAssembler") << ":connectImageProducer";

    imageView_->connectImageProducer(sender, signal);

    connect(sender, signal,
            this, SLOT(imageAcquired(const cv::Mat&)));
}

void AssemblyModuleAssembler::disconnectImageProducer(const QObject* sender,
                                                      const char* signal)
{
    NQLog("AssemblyModuleAssembler") << ":disconnectImageProducer";

    imageView_->disconnectImageProducer(sender, signal);

    disconnect(sender, signal,
               this, SLOT(imageAcquired(const cv::Mat&)));
}

void AssemblyModuleAssembler::snapShot()
{
    // NQLog("AssemblyModuleAssembler") << ":snapShot()";

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
            imageView_->setZoomFactor(0.25);
            event->accept();
            break;
        case Qt::Key_1:
            imageView_->setZoomFactor(1.00);
            event->accept();
            break;
        case Qt::Key_Plus:
            imageView_->increaseZoomFactor();
            event->accept();
            break;
        case Qt::Key_Minus:
            imageView_->decreaseZoomFactor();
            event->accept();
            break;
        default:
            break;
        }
    }
}







AssemblyVacuumToggler::AssemblyVacuumToggler(QWidget *parent, std::string string, double a,  int mode)
: QWidget(parent),local_a(a)
{
    QGridLayout *l = new QGridLayout(this);
    setLayout(l);
    
    std::ostringstream strs;
    strs.clear();
    strs << a;
    std::string str = strs.str();
    QString qstr = QString::fromStdString(str);
    QString qname = QString::fromStdString(string);
    
    state = false;
    
    button1 = new QPushButton(qname, this);
    
    // lineEdit1 = new QLineEdit();
    // lineEdit1->setText(qstr);
    l->addWidget(button1,0,0);
    // l->addWidget(lineEdit1,0,1);
    
    
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
    
  //  ql->setPixmap(pixmap);
    
    if (!state){
    ql->setText("VACUUM ON");
    ql->setStyleSheet("QLabel { background-color : red; color : black; }");
        state = true;
    }else if (state){
    ql->setText("VACUUM OFF");
    ql->setStyleSheet("QLabel { background-color : green; color : black; }");
        state = false;

    }
    
        
        
}



AssemblyAttacher::AssemblyAttacher(QWidget *parent, std::string string, double drop,  int mode)
: QWidget(parent), local_drop(drop)
{
    QFormLayout *l = new QFormLayout(this);
    setLayout(l);
    
    std::ostringstream strs;
    strs.clear();
    strs << drop;
   // strs << ",";
    std::string str = strs.str();
    QString qstr = QString::fromStdString(str);
    QString qname = QString::fromStdString(string);
    
 //   this->local_x = x;
 //   this->local_y = y;
 //   this->local_z = z;
 //   this->local_a = a;
    
    button1 = new QPushButton(qname, this);
    
    lineEdit1 = new QLineEdit();
    lineEdit1->setText(qstr);
    l->addRow(button1,lineEdit1);
    
    connect(button1, SIGNAL(clicked()),
            this, SLOT(dropAttach()));
    
    
}



void AssemblyAttacher::dropAttach(){
    
    //parse lineEdit text to get target coordinates
    QString  parent_string = this->lineEdit1->text();

    
    double d_d = parent_string.toDouble();

    NQLog("AssemblyAttacher::dropAttach") << d_d;
    
    
    ApplicationConfig* config = ApplicationConfig::instance();
    LStepExpressModel* lStepExpressModel_ = new LStepExpressModel(config->getValue<std::string>("LStepExpressDevice").c_str(),1000, 100);
    LStepExpressSettings* lStepExpressSettings_ = new LStepExpressSettings(lStepExpressModel_);
    LStepExpressMotionManager* motionManager_ = new LStepExpressMotionManager(lStepExpressModel_);
    lStepExpressModel_->initialize();
    
    connect(this, SIGNAL(moveRelative(double,double,double,double)), motionManager_, SLOT(moveRelative(double,double,double,double)));
    
    
    NQLog("AssemblyUEyeCameraSettingsMotionInterface") <<" requesting move... ";
    
    //  if( lStepExpressModel_) lStepExpressModel_->moveRelative( 1.0, 1.0, 0.0, -10.0);
    
    emit moveRelative(0.0,0.0,-d_d, 0.0);
    
    NQLog("AssemblyUEyeCameraSettingsMotionInterface") <<"move requested ";
    
    
}







AssemblyCommander::AssemblyCommander(QWidget *parent, std::string string, double x ,double y, double z,double a,  int mode)
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



void AssemblyCommander::goToTarget(){
    

    
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

    
    NQLog("AssemblyUEyeCameraSettingsMotionInterface") << ": going to target (parsed) "<< x_d<<" "<< y_d<<"  "<< z_d;
    
    
    ApplicationConfig* config = ApplicationConfig::instance();
    LStepExpressModel* lStepExpressModel_ = new LStepExpressModel(config->getValue<std::string>("LStepExpressDevice").c_str(),1000, 100);
    LStepExpressSettings* lStepExpressSettings_ = new LStepExpressSettings(lStepExpressModel_);
    LStepExpressMotionManager* motionManager_ = new LStepExpressMotionManager(lStepExpressModel_);
    lStepExpressModel_->initialize();
    
    connect(this, SIGNAL(moveAbsolute(double,double,double,double)), motionManager_, SLOT(moveAbsolute(double,double,double,double)));
   

    NQLog("AssemblyUEyeCameraSettingsMotionInterface") <<" requesting move... ";
    
    //  if( lStepExpressModel_) lStepExpressModel_->moveRelative( 1.0, 1.0, 0.0, -10.0);

    emit moveAbsolute(x_d, y_d, z_d, a_d);
    
    NQLog("AssemblyUEyeCameraSettingsMotionInterface") <<"move requested ";

    
}


void AssemblyCommander::updateText(double x, double y , double a){
    
    NQLog("AssemblyCommander::updateText");


    std::ostringstream strs;
    strs.clear();
    strs << x;
    strs << ",";
    strs << y;
    strs << ",";
    strs << a;
    std::string str = strs.str();
    QString qstr = QString::fromStdString(str);
   // QString qname = QString::fromStdString(string);
    
    lineEdit1->setText(qstr);

}



AssemblyAlligner::AssemblyAlligner(QWidget *parent, std::string string, double a,  int mode)
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
            this, SLOT(allign()));
    
    
}



void AssemblyAlligner::allign(){
    
    
    
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
    
    
    NQLog("AssemblyUEyeCameraSettingsMotionInterface") << ": going to target allignment (parsed) "<< a_d<<" ";
    
    
    ApplicationConfig* config = ApplicationConfig::instance();
    LStepExpressModel* lStepExpressModel_ = new LStepExpressModel(config->getValue<std::string>("LStepExpressDevice").c_str(),1000, 100);
    LStepExpressSettings* lStepExpressSettings_ = new LStepExpressSettings(lStepExpressModel_);
    LStepExpressMotionManager* motionManager_ = new LStepExpressMotionManager(lStepExpressModel_);
    lStepExpressModel_->initialize();
    
    connect(this, SIGNAL(moveRelative(double,double,double,double)), motionManager_, SLOT(moveRelative(double,double,double,double)));
    
    
    NQLog("AssemblyUEyeCameraSettingsMotionInterface") <<" requesting move... ";
    
    //  if( lStepExpressModel_) lStepExpressModel_->moveRelative( 1.0, 1.0, 0.0, -10.0);
    
    emit moveRelative(0.0, 0.0, 0.0, a_d);
    
    NQLog("AssemblyUEyeCameraSettingsMotionInterface") <<"move requested ";
    
    
}






AssemblySensorLocator::AssemblySensorLocator(QWidget *parent, std::string string, double a,  int mode)
: QWidget(parent),local_a(a)
{
    QGridLayout *l = new QGridLayout(this);
    setLayout(l);
    
    std::ostringstream strs;
    strs.clear();
    strs << a;
    std::string str = strs.str();
    QString qstr = QString::fromStdString(str);
    QString qname = QString::fromStdString(string);
    
    button1 = new QPushButton(qname, this);
    
   // lineEdit1 = new QLineEdit();
   // lineEdit1->setText(qstr);
    l->addWidget(button1,0,0);
   // l->addWidget(lineEdit1,0,1);
    

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
            this, SLOT(locateSensor()));
    
}

void AssemblySensorLocator::locateSensor(){

    NQLog("AssemblySensorLocator") << "Finding Marker (Circle Seed Algorithm)" ;

    cv::Mat img_gs, img_rgb, img_edges;
    
    img_gs = cv::imread("/Users/keaveney/Desktop/calibration/im_scan___Exp10___EdgeThr145___lt110.png", CV_LOAD_IMAGE_COLOR);

    cvtColor(img_gs, img_gs , CV_RGB2GRAY);

    
    int radius;
    int  expectedCircleRadius_ = 89;
    double circleCenterDetectionThreshold_  = 35.0;
    double circleEdgeDetectionThreshold_  = 145.0;
    double linesCannyEdgeDetectionThreshold1_ =50;
    double linesCannyEdgeDetectionThreshold2_ =200;
    double linesCannyEdgeDetectionApertureSize_ =3;
    double linesCannyEdgeDetectionL2Gradient_ =1;
    double linesHoughDistanceResolution_ = 1.0;
    //    double linesHoughAngleResolution_*CV_PI/180,
    double linesHoughAngleResolution_ = 0.1;
    double linesHoughThreshold_ = 110.0;
    double linesHoughMinLineLength_=  150.0;
    double linesHoughMaxLineGap_ =25.0;
    bool  dist1_, dist2_, dist3_, doca_ ;
    double slope_final, ang_final;
    double distance,doca,x0,x1,x2, y0,y1,y2, x,y;
    
    
    std::vector<cv::Vec3f> circles;
    cv::Point2f circleCenter_;

    std::vector<cv::Vec4i> tempLines;
    std::vector<std::pair<cv::Point2f,cv::Point2f> > lines_;
    std::vector<std::pair<cv::Point2f,cv::Point2f> > goodLines_;
    std::vector<std::pair<cv::Point2f,cv::Point2f> > mergedLines_;
    
    std::vector<cv::Point2f> intersections_;
    std::vector<cv::Point2f> goodIntersections_;
    std::vector<cv::Point2f> finalIntersections_;
    std::vector<cv::Point2f> finalIntersectionsUp_;
    std::vector<cv::Point2f> finalIntersectionsDown_;
    
    lines_.clear();
    goodLines_.clear();
    intersections_.clear();
    goodIntersections_.clear();
    finalIntersections_.clear();
    
    //detect circles
    cv::HoughCircles(img_gs, circles, CV_HOUGH_GRADIENT, 1, 20,
                     circleEdgeDetectionThreshold_, circleCenterDetectionThreshold_,
                     expectedCircleRadius_ - 15., expectedCircleRadius_ + 15.);
    
    //get centre of first circle...problematic if this is not the right one.
    if (circles.size() != 0){
        circleCenter_.x = circles[0][0];
        circleCenter_.y = circles[0][1];
    }
    
//    std::ostringstream convert;   // stream used for the conversion
//    convert.str(" ");
//    convert.clear();
//    convert << p;
//    str_p = convert.str();
//    convert.str(" ");
//    convert.clear();
//    convert << lt;
//    str_lt = convert.str();
//
//    convert.str(" ");
//    convert.clear();
    cvtColor(img_gs, img_rgb , CV_GRAY2RGB);

    /// Draw the circles detected in blue
    for( size_t i = 0; i < circles.size(); i++ )
    {
        cv::Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
        x = cvRound(circles[i][0]);
        y = cvRound(circles[i][1]);
        radius = cvRound(circles[i][2]);
        // circle center
        circle( img_rgb, center, 3, cv::Scalar(0,0,255), -1, 8, 0 );
        //circle outline;
        circle( img_rgb, center, radius, cv::Scalar(0,0,255), 3, 8, 0 );
    }
    
    //detect edges
    cv::Canny(img_gs, img_edges,
              linesCannyEdgeDetectionThreshold1_,
              linesCannyEdgeDetectionThreshold2_,
              linesCannyEdgeDetectionApertureSize_,
              linesCannyEdgeDetectionL2Gradient_);
    
    
    NQLog("AssemblyVUEyeCamera") << "  running hough lines";
    
    
    //detect lines
    cv::HoughLinesP(img_edges, tempLines,
                    linesHoughDistanceResolution_,
                    linesHoughAngleResolution_*CV_PI/180,
                    linesHoughThreshold_,
                    linesHoughMinLineLength_,
                    linesHoughMaxLineGap_);
    
    
    for (size_t i = 0; i < tempLines.size(); i++ ) {
        lines_.push_back(std::pair<cv::Point2f,cv::Point2f>(cv::Point2f(tempLines[i][0],tempLines[i][1]), cv::Point2f(tempLines[i][2],tempLines[i][3])));
    }
    
    
    //draw the initial collection of lines in red (handy for debugging, monitoring of PatRec)
    for (size_t i = 0; i < lines_.size(); i++ ) {
        cv::line(img_rgb,
                 cv::Point(cvRound(lines_[i].first.x), cvRound(lines_[i].first.y)),
                 cv::Point(cvRound(lines_[i].second.x), cvRound(lines_[i].second.y)),
                 cv::Scalar(255, 0, 0), 2, CV_AA);
    }
    
    
    //select only the lines which have starting/ending points close to the circle centre and DOCA consistent with the L marker outline
    for (size_t i = 0; i < lines_.size(); i++ ) {
        dist1_  = dist2_ = dist3_ = doca_ = false;
        distance = std::sqrt(std::pow(circleCenter_.x - lines_[i].first.x, 2.0) +
                             std::pow(circleCenter_.y - lines_[i].first.y, 2.0));
        if (distance < expectedCircleRadius_*6.0) dist1_ = true;
        
        distance = std::sqrt(std::pow(circleCenter_.x - lines_[i].second.x, 2.0) +
                             std::pow(circleCenter_.y - lines_[i].second.y, 2.0));
        //  if (distance < expectedCircleRadius_*2.5) goodLines_.push_back(lines_[i]);
        if (distance < expectedCircleRadius_*6.0)  dist2_ = true  ;
        
        x0 = circleCenter_.x;
        y0 = circleCenter_.y;
        x1 =  lines_[i].first.x;
        y1 =  lines_[i].first.y;
        x2 =  lines_[i].second.x;
        y2 =  lines_[i].second.y;
        
        doca =  std::fabs( x0*(y2-y1)  -  y0*(x2-x1) + x2*y1 -y2*x1)/ std::sqrt(   std::pow(y2 - y1,2.0)   +    std::pow(x2 - x1,2.0)      );
        
        
        if ((doca <expectedCircleRadius_*5.8  && doca >expectedCircleRadius_*5.4)  || (doca <expectedCircleRadius_*1.3  && doca >expectedCircleRadius_*0.7)   ) doca_ = true;
        
        if (dist1_ && dist2_ && doca_ ) goodLines_.push_back(lines_[i]);
        // if ( doca_ == true) goodLines_.push_back(lines_[i]);
    }
    
    //draw the initial collection of "good" lines in pink (handy for debugging, monitoring of PatRec)

    for (size_t i = 0; i < goodLines_.size(); i++ ) {
        cv::line(img_rgb,
                 cv::Point(cvRound(goodLines_[i].first.x), cvRound(goodLines_[i].first.y)),
                 cv::Point(cvRound(goodLines_[i].second.x), cvRound(goodLines_[i].second.y)),
                 cv::Scalar(255, 0, 255), 4, CV_AA);
    }

    
    //find all intersections between goodlines that intersect @ ~ 90 degrees
    for (size_t i = 0; i < goodLines_.size(); i++) {
        for (size_t j = 0; j < goodLines_.size(); j++) {
            
            if (i == j ) continue;
            cv::Point2f theIntersection;
            cv::Point2f o1,o2,p1,p2,r;
            double distance;
            o1 = goodLines_[i].first;
            p1 = goodLines_[i].second;
            o2 = goodLines_[j].first;
            p2 = goodLines_[j].second;
            
            cv::Point2f x = o2 - o1;
            cv::Point2f d1 = p1 - o1;
            cv::Point2f d2 = p2 - o2;
            
            double cross = d1.x*d2.y - d1.y*d2.x;
            double slope1 = (p1.y - o1.y) / (p1.x - o1.x);
            double slope2 = (p2.y - o2.y) / (p2.x - o2.x);
            double ang1 = atan (slope1) * 180 / 3.14;
            double ang2 = atan (slope2) * 180 / 3.14;
            
            double ang = fabs(ang2) - fabs(ang1);
            
            //if (fabs(ang) > 89.0  && fabs(ang) < 91.0 ) ;
            
            // cout <<" angle between lines: ang1 =  "<<  ang1  <<" ang2 = "<< ang2 <<" ang = "<<  ang<<endl;
            
            
            //  if (cv::norm(r - o1) > distance &&
            //      cv::norm(r - p1) > distance) continue;
            
            //  if (cv::norm(r - o2) > distance &&
            //      cv::norm(r - p2) > distance) continue;
            //  std::cout << "ang   " << ang     <<std::endl;
            
            if ( fabs(ang) > 80.0  && fabs(ang) < 100.0   &&   abs(cross) > /*EPS*/1e-7){
                double t1 = (x.x * d2.y - x.y * d2.x)/cross;
                r = o1 + d1 * t1;
                intersections_.push_back(r);
                
                
                distance = std::sqrt(std::pow(circleCenter_.x - r.x, 2.0) +
                                     std::pow(circleCenter_.y - r.y, 2.0));
                //if (distance > 6.0*expectedCircleRadius_)continue;
            }
        }
    }
    
   
    cout <<"n intersections  = = "<< intersections_.size()<< endl;

    
    // Now select intersections that occur near the start or end point of the intersecting lines. This removes random intersections from sprurious "noise" lines.
    
    for(int  intsec = 0; intsec < intersections_.size(); intsec++ ){
        bool goodIntersection = false;
        for(int  gl = 0; gl < goodLines_.size(); gl++ ){
            
            double d_start = std::sqrt(std::pow(intersections_[intsec].x - goodLines_[gl].first.x, 2.0) + std::pow(intersections_[intsec].y - goodLines_[gl].first.y, 2.0));
            double d_end = std::sqrt(std::pow(intersections_[intsec].x - goodLines_[gl].second.x, 2.0) + std::pow(intersections_[intsec].y - goodLines_[gl].second.y, 2.0));
            if(d_start< 5.0 || d_end < 5.0) goodIntersection = true;
        }
        if(goodIntersection == true) goodIntersections_.push_back(intersections_[intsec]);
    }
    
    for (int goodint =0; goodint < goodIntersections_.size(); goodint++){
        // circle( img_rgb, goodIntersections_[goodint], 10, cv::Scalar(0,255,0), 3, 8, 0 );
        
        
        // Select the two intersections that are adjacent to the circle.
        if (std::sqrt(std::pow(circleCenter_.x - goodIntersections_[goodint].x, 2.0) + std::pow(circleCenter_.y - goodIntersections_[goodint].y, 2.0)) < expectedCircleRadius_*1.6) finalIntersections_.push_back(goodIntersections_[goodint]);

    }
    

    
    //one typically ends up with multiple intersections at each corner due extra, spurious lines
    cout <<"n final intersection  = = "<< finalIntersections_.size()<< endl;

    // I divide the collection of final intersections into those abpve the circle's centre "up"
    // and below "down". I use the average point of these collections to define to define the interesting intersections.
    double running_total_x  =0;
    double running_total_y  =0;
    double av_x_up;
    double av_x_down;
    double av_y_up;
    double av_y_down;
    
    finalIntersectionsDown_.clear();
    finalIntersectionsUp_.clear();
    
    if (finalIntersections_.size() >0){
        for (int finalint =0; finalint < finalIntersections_.size(); finalint++) {
            if ( y > finalIntersections_[finalint].y){
                finalIntersectionsDown_.push_back(finalIntersections_[finalint]);
            }
            else if ( y < finalIntersections_[finalint].y){
                finalIntersectionsUp_.push_back(finalIntersections_[finalint]);
            }
            
        }
        
        
        for (int finalintup =0; finalintup < finalIntersectionsUp_.size(); finalintup++) {
            running_total_x = running_total_x + finalIntersectionsUp_[finalintup].x;
            running_total_y = running_total_y + finalIntersectionsUp_[finalintup].y;
        }
        
        av_x_up = running_total_x/finalIntersectionsUp_.size();
        av_y_up = running_total_y/finalIntersectionsUp_.size();
        
        running_total_x  =0;
        running_total_y  =0;
        
        for (int finalintdown =0; finalintdown < finalIntersectionsDown_.size(); finalintdown++) {
            running_total_x = running_total_x + finalIntersectionsDown_[finalintdown].x;
            running_total_y = running_total_y + finalIntersectionsDown_[finalintdown].y;
        }
        
        av_x_down = running_total_x/finalIntersectionsDown_.size();
        av_y_down = running_total_y/finalIntersectionsDown_.size();
        
        
        
        //             if (finalIntersections_.size() >0){
        for (int finalintup =0; finalintup < finalIntersectionsUp_.size(); finalintup++) {
            circle( img_rgb, finalIntersectionsUp_[finalintup], 10, cv::Scalar(0,255,255), 3, 8, 0 );
        }
        
        for (int finalintdown =0; finalintdown < finalIntersectionsDown_.size(); finalintdown++) {
            circle( img_rgb, finalIntersectionsDown_[finalintdown], 10, cv::Scalar(0,255,0), 3, 8, 0 );
        }
        
        
        
        cv::line(img_rgb,
                 cv::Point(cvRound(av_x_down), cvRound(av_y_down)),
                 cv::Point(cvRound(av_x_up), cvRound(av_y_up)),
                 cv::Scalar(0, 255, 255), 4, CV_AA);
        
        
        circle( img_rgb, cv::Point(x,y), 10, cv::Scalar(255,255,0), 3, 8, 0 );
        
        
        slope_final = (av_y_up -  av_y_down) / (av_x_up -  av_x_down);
        ang_final = atan (slope_final) * 180 / 3.14;
        

        
    }
    
    
   std::ostringstream convert;   // stream used for the conversion
   convert.str(" ");
   convert.clear();
   convert << ang_final;
   std::string str_ang_final = convert.str();
   convert.str(" ");
   convert.clear();
   convert << circleCenter_.x ;
   std::string str_x = convert.str();
   convert.str(" ");
   convert.clear();
   convert << circleCenter_.y;
   std::string str_y = convert.str();
    
    std::string text_result = "Marker is at = (" +  str_x  +  ", " + str_y + "), with orientation = " + str_ang_final  + " degrees";
    
    
    putText(img_rgb, text_result, cvPoint(30,100),
            cv::FONT_HERSHEY_COMPLEX_SMALL, 2.8, cvScalar(200,200,250), 1, CV_AA);
    std::string filename = "/Users/keaveney/Desktop/calibration/PatRec_result.png";
    cv::imwrite(filename, img_rgb);
    
    emit updateImage(filename);
    emit foundSensor(1);
    emit sendPosition(circleCenter_.x,circleCenter_.y, ang_final);
    NQLog("AssemblyVUEyeCamera") << "  found marker";
}


void AssemblySensorLocator::foundsensor(int state){

    NQLog("AssemblySensorLocator::foundsensor");

    
    if (state == 0){
        ql->setText("WAITING");
        NQLog("AssemblySensorLocator::foundsensor")<< " 0  ";

        ql->setStyleSheet("QLabel { background-color : orange; color : black; }");
    
    } else if (state == 1){
        
        NQLog("AssemblySensorLocator::foundsensor")<< " 1 ";

        ql->setText("FOUND MARKER");
        ql->setStyleSheet("QLabel { background-color : green; color : black; }");
    
    } else if (state == 2){
        NQLog("AssemblySensorLocator::foundsensor")<< " 2 ";
        ql->setText("ERROR");
        ql->setStyleSheet("QLabel { background-color : red; color : black; }");

    }
    
    


}







