#include <vector>

#include <QVBoxLayout>
#include <QFileDialog>
#include <QPushButton>

#include <QUrl>

#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsPixmapItem>
#include <QLabel>
#include <QTextEdit>

#include <numeric>


#include <nqlogger.h>

#include "AssemblyUEyeTracker.h"



#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <stdio.h>

using namespace cv;
using namespace std;



AssemblyUEyeTracker::AssemblyUEyeTracker(QWidget *parent)
    : QWidget(parent)
{
    QVBoxLayout *l = new QVBoxLayout(this);
    setLayout(l);

    QPushButton * button = new QPushButton("save", this);
    connect(button, SIGNAL(clicked(bool)),
            this, SLOT(snapShot()));
    
    QPushButton * button_2 = new QPushButton("stream", this);
    connect(button_2, SIGNAL(clicked(bool)),
            this, SLOT(stream()));

    QPushButton * button_3 = new QPushButton("track", this);
    connect(button_3, SIGNAL(clicked(bool)),
            this, SLOT(track()));
    

    l->addWidget(button);
    l->addWidget(button_2);
    l->addWidget(button_3);
    
    
    QPalette palette;
    palette.setColor(QPalette::Background, QColor(220, 220, 220));

    imageView_ = new AssemblyUEyeView();
    imageView_->setMinimumSize(500, 500);
    imageView_->setPalette(palette);
    imageView_->setBackgroundRole(QPalette::Background);
    imageView_->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    imageView_->setScaledContents(true);
    imageView_->setAlignment(Qt::AlignCenter);

    scrollArea_ = new QScrollArea(this);
    scrollArea_->setMinimumSize(500, 500);
    scrollArea_->setPalette(palette);
    scrollArea_->setBackgroundRole(QPalette::Background);
    scrollArea_->setAlignment(Qt::AlignCenter);
    scrollArea_->setWidget(imageView_);
    scrollArea_->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    l->addWidget(scrollArea_);

    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
}

//void AssemblyUEyeTracker::connectImageProducer(const QObject* sender,
//                                                   const char* signal)
//{
//    NQLog("AssemblyUEyeTracker") << ":connectImageProducer " <<  signal ;

//    imageView_->connectImageProducer(sender, signal);

//    connect(sender, signal,
//            this, SLOT(imageAcquired(const cv::Mat&)));
//}



void AssemblyUEyeTracker::connectImageProducer_tracker(const QObject* sender,
                                               const char* signal)
{
    NQLog("AssemblyUEyeTracker") << ":connectImageProducer_tracker " ;
    
    imageView_->connectImageProducer(sender, signal);
    
    connect(sender, signal,
            this, SLOT(imageAcquired(const cv::Mat&)));
}

void AssemblyUEyeTracker::disconnectImageProducer(const QObject* sender,
                                                      const char* signal)
{
    NQLog("AssemblyUEyeTracker") << ":disconnectImageProducer";

    imageView_->disconnectImageProducer(sender, signal);

    disconnect(sender, signal,
               this, SLOT(imageAcquired(const cv::Mat&)));
}

void AssemblyUEyeTracker::snapShot()
{
     NQLog("AssemblyUEyeTracker") << ":snapShot()";

    if (image_.rows==0) return;

    QString filename = QFileDialog::getSaveFileName(this, "save image", ".", "*.png");
    if (filename.isNull() || filename.isEmpty()) return;

    if (!filename.endsWith(".png")) filename += ".png";

    cv::imwrite(filename.toStdString(), image_);
}

void AssemblyUEyeTracker::track(){
    
    NQLog("AssemblyUEyeTracker") << ":track";
    cv::Mat local = image_;
    cv:Mat src;
    cv::Mat gray;
    cv::Mat blur;

    //cv::Mat circles;

    cv::Mat blurgray;
   // src = imread( "/Users/keaveney/Automation/cmstkmodlab//share/assembly/sensor_43MHz_186ms_2.png", 1 );
 
    src = imread( "/Users/keaveney/Automation/cmstkmodlab//share/assembly/sensor_24MHz_333ms_4.png", 1 );

    
    int DELAY_CAPTION = 1500;
    int DELAY_BLUR = 100;
    int MAX_KERNEL_LENGTH = 31;
    
    if (_step == 0 ){
    imageView_->setImage(src);
    }else if(_step == 1){
        

        VideoCapture cap = VideoCapture(0); // open the video file for reading
        
        if ( !cap.isOpened() )  // if not success, exit program
        {
            cout << "Cannot open the video file" << endl;
           
        }
        
        //cap.set(CV_CAP_PROP_POS_MSEC, 300); //start the video at 300ms
        
        double fps = cap.get(CV_CAP_PROP_FPS); //get the frames per seconds of the video
        
        cout << "Frame per seconds : " << fps << endl;
        
        namedWindow("MyVideo",CV_WINDOW_AUTOSIZE); //create a window called "MyVideo"
        imageView_->setZoomFactor(1.5);
        vector<Vec3f> circles;
        
        QTextEdit * text = new QTextEdit();
        
        text->show();

        while(1)
        {
            Mat frame;
            
            bool bSuccess = cap.read(frame); // read a new frame from video
            
            if (!bSuccess) //if not success, break loop
            {
                cout << "Cannot read the frame from video file" << endl;
                break;
            }
             cvtColor( frame, gray, CV_BGR2GRAY );
            GaussianBlur( gray, gray, Size(9, 9), 2, 2 );
            HoughCircles( gray, circles, CV_HOUGH_GRADIENT, 1, 1, 200, 40, 0, 0 );
        
            NQLog("AssemblyUEyeTracker") << " N Circles detected = " << circles.size();

            int radius,x,y  = -1;
            /// Draw the circles detected
          /*  for( size_t i = 0; i < circles.size(); i++ )
            {
                Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
                x = cvRound(circles[i][0]);
                y = cvRound(circles[i][1]);
                radius = cvRound(circles[i][2]);
                // circle center
                circle( frame, center, 3, Scalar(255,0,0), -1, 8, 0 );
                // circle outline
                circle( frame, center, radius, Scalar(255,0,0), 3, 8, 0 );
            }
           
           */
            
            //Get the mean circle

            Vec3f m_circle = mean(circles);
            x = cvRound(m_circle[0]);
            y = cvRound(m_circle[1]);
            radius = cvRound(m_circle[2]);
            
            
            QString rad = QString::number(radius);
            QString x_str = QString::number(x);
            QString y_str = QString::number(y);
            
             Point center(x, y);
            // circle center
            circle( frame, center, 3, Scalar(255,0,0), -1, 8, 0 );
            // circle outline
            circle( frame, center, radius, Scalar(255,0,0), 3, 8, 0 );

            

            QString final_string = "Circle found with radius = " + rad + ", x = " + x_str + ", y =  " + y_str ;
            
           text->setText(final_string);
           
            
            imageView_->setImage(frame);

           // imshow("MyVideo", frame); //show the frame in "MyVideo" window
            
            if(waitKey(30) == 27) //wait for 'esc' key press for 30 ms. If 'esc' key is pressed, break loop
            {
                cout << "esc key is pressed by user" << endl;
                break;
            }
        }
        
        
        NQLog("AssemblyUEyeTracker") << ":Camera captured";
        
      //  QList<QCameraInfo> cameras = QCameraInfo::availableCameras();
      //  foreach (const QCameraInfo &cameraInfo, cameras) {
         //   if (cameraInfo.deviceName() == "mycamera")
          //      camera = new QCamera(cameraInfo);
        //    NQLog("AssemblyUEyeTracker") << cameraInfo.deviceName();
       // }
        
        
        //cvtColor( src, gray, CV_BGR2GRAY );
        //GaussianBlur( gray, gray, Size(9, 9), 6, 6 );
       // threshold( gray, gray, 100, 50,1 );
       // imageView_->setImage(gray);

    }
    else if(_step == 2){
        
        cvtColor( src, gray, CV_BGR2GRAY );
        GaussianBlur( gray, gray, Size(9, 9), 6, 6 );

        vector<Vec3f> circles;
        
        /// Apply the Hough Transform to find the circles
       // HoughCircles( gray, circles, CV_HOUGH_GRADIENT, 1, gray.rows/8, 200, 100, 0, 0 );
        HoughCircles( gray, circles, CV_HOUGH_GRADIENT, 1, 1, 200, 35, 0, 0 );
        
        /// Draw the circles detected
        for( size_t i = 0; i < circles.size(); i++ )
        {
            Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
            int radius = cvRound(circles[i][2]);
            // circle center
            circle( src, center, 3, Scalar(0,255,0), -1, 8, 0 );
            // circle outline
            circle( src, center, radius, Scalar(0,0,255), 3, 8, 0 );
        }
        
        NQLog("AssemblyUEyeTracker") << " N Circles detected = " << circles.size();

        imageView_->setImage(src);
        

    }
    
    _step++;
    
    
    
    
   // cvtColor( local, gray, CV_BGR2GRAY );
    
    // Reduce the noise so we avoid false circle detection
    GaussianBlur( gray, gray, Size(1, 1), 1, 1 );
    
    
  //  vector<Vec3f> circles;
    
   // imageView_->setImage(gray);
    
    // Apply the Hough Transform to find the circles
    //HoughCircles( gray, circles, CV_HOUGH_GRADIENT, 1, 30, 200, 50, 0, 0 );
    
//   cv::Mat src;
 //   src = cv::imread(local, CV_LOAD_IMAGE_GRAYSCALE);
}



Vec3f AssemblyUEyeTracker::mean(vector<Vec3f> vec_circles){
    
    Vec3f mean_circle;
    
    vector<int> radii;
    vector<int> x;
    vector<int> y;
    
    for (int c=0; c < vec_circles.size(); c++){
        radii.push_back(vec_circles[c][2]);
        x.push_back(vec_circles[c][0]);
        y.push_back(vec_circles[c][1]);
    }
    
    double sum_radii = std::accumulate(std::begin(radii), std::end(radii), 0.0);
    double m_radii =  sum_radii / radii.size();
    
    double sum_x = std::accumulate(std::begin(x), std::end(x), 0.0);
    double m_x =  sum_x / x.size();
    
    double sum_y = std::accumulate(std::begin(y), std::end(y), 0.0);
    double m_y =  sum_y / y.size();
    
//    double accum = 0.0;
  //  std::for_each (std::begin(v), std::end(v), [&](const double d) {
   //     accum += (d - m) * (d - m);
   // });
    //double stdev = sqrt(accum / (v.size()-1));
    
    mean_circle = Vec3f(m_x,m_y,m_radii);
    
    return mean_circle;
    

}


void AssemblyUEyeTracker::stream()
{
//    QUrl  url = QUrl("http://10.12.12.170:8080");
 //   QWebView * view = new QWebView();
 //   view->load(url);
 //   view->show();
    
    NQLog("AssemblyUEyeTracker") << ":stream " <<  signal ;

    
   
   // QGraphicsScene scene;
   // QGraphicsView view(&scene);
   // QGraphicsPixmapItem item(QPixmap("~/Desktop/X.gif"));
   // scene.addItem(&item);
   // view.show();
//emit acquireImage();
    
    //QLabel *image = new QLabel();
   // image->setPixmap( QPixmap( "Users/keaveney/Desktop/X.png" ) );
   // image->show();
   // update();
    
    
    
    // NQLog("AssemblyUEyeTracker") << ":snapShot()";
    
  //  if (image_.rows==0) return;
    
   // QString filename = QFileDialog::getSaveFileName(this, "save image", ".", "*.png");
   // if (filename.isNull() || filename.isEmpty()) return;
   // if (!filename.endsWith(".png")) filename += ".png";
    
//    cv::imwrite(filename.toStdString(), image_);
}




void AssemblyUEyeTracker::imageAcquired(const cv::Mat& newImage)
{
    
    NQLog("AssemblyUEyeTracker") << ":In imageAcquired() test";

    newImage.copyTo(image_);
}

void AssemblyUEyeTracker::keyReleaseEvent(QKeyEvent * event)
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
