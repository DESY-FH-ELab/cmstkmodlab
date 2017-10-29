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

#include <MarkerFinderPatRec.h>
#include <ApplicationConfig.h>
#include <nqlogger.h>
#include <Util.h>

#include <iostream>

#include <TGraph.h>
#include <TCanvas.h>
#include <TH1F.h>

MarkerFinderPatRec::MarkerFinderPatRec(QObject* parent) :
  QObject(parent),
  generalThreshold_(200)
{
  const QString      cache_dir = Util::QtCacheDirectory()+"/assembly/markerfinder";
  Util::QDir_mkpath (cache_dir);
  cacheDirectory1_ = cache_dir.toStdString();

  NQLog("MarkerFinderPatRec", NQLog::Spam) << "created directory " << cacheDirectory1_;

  const QString      cache_subdir = cache_dir+"/rotations";
  Util::QDir_mkpath (cache_subdir);
  cacheDirectory2_ = cache_subdir.toStdString();

  NQLog("MarkerFinderPatRec", NQLog::Spam) << "created directory " << cacheDirectory2_;

  NQLog("MarkerFinderPatRec", NQLog::Debug) << "constructed";
}

MarkerFinderPatRec::~MarkerFinderPatRec()
{
  NQLog("MarkerFinderPatRec", NQLog::Debug) << "destructed";
}

void MarkerFinderPatRec::setNewGeneralThreshold(const int value, const cv::Mat& img)
{
  generalThreshold_ = value;

  NQLog("MarkerFinderPatRec", NQLog::Debug) << "setNewGeneralThreshold"
     << ": emitting signal \"sendCurrentGeneralThreshold(" << value << ")\"";

  emit sendCurrentGeneralThreshold(value);

  this->updateThresholdImage(img);

  NQLog("MarkerFinderPatRec", NQLog::Spam) << "setNewGeneralThreshold"
     << ": updated threshold value: " << value;
}

void MarkerFinderPatRec::getCurrentGeneralThreshold()
{ 
  NQLog("MarkerFinderPatRec", NQLog::Debug) << "getCurrentGeneralThreshold"
     << ": emitting signal \"sendCurrentGeneralThreshold(" << generalThreshold_ << ")\"";

  emit sendCurrentGeneralThreshold(generalThreshold_);
}

void MarkerFinderPatRec::updateThresholdImage(const cv::Mat& img)
{
  cv::Mat img_copy = img.clone();

  // greyscale image
  cv::Mat img_copy_gs(img_copy.size(), img_copy.type());    

  if(img.channels() > 1)
  {
    // convert color to GS
    cv::cvtColor(img_copy, img_copy_gs, CV_BGR2GRAY);
  }
  else
  {
    img_copy_gs = img_copy.clone();
  }

  // binary image (thresholding)
  cv::Mat img_copy_bin(img_copy_gs.size(), img_copy_gs.type());
  cv::threshold(img_copy_gs, img_copy_bin, generalThreshold_, 255, cv::THRESH_BINARY);

  NQLog("MarkerFinderPatRec", NQLog::Spam) << "updateThresholdImage"
     << ": created binary image with threshold=" << generalThreshold_;

  std::string filename_img_bin = cacheDirectory1_+"/Sensor_bin.png";
  cv::imwrite(filename_img_bin, img_copy_bin);

  NQLog("MarkerFinderPatRec", NQLog::Debug) << "updateThresholdImage"
     << ": emitting signal \"sendUpdatedThresholdImage(" << filename_img_bin << ")\"";

  emit sendUpdatedThresholdImage(QString(filename_img_bin.c_str()));
}

void MarkerFinderPatRec::runObjectDetection(int labmode, int objectmode)
{
  NQLog("MarkerFinderPatRec", NQLog::Debug) << "runObjectDetection"
     << ": labmode=" << labmode << ", objectmode=" << objectmode;

  labmode_g    = labmode;
  objectmode_g = objectmode;

  if(labmode == 1)
  {
    NQLog("MarkerFinderPatRec", NQLog::Debug) << "runObjectDetection"
       << ": emitting signal \"acquireImage\"";

    emit acquireImage();
  }
  else if(labmode == 0)
  {
    // standard signals to launch PatRec here
    if(objectmode == 0)
    {
//    img = cv::imread(Config::CMSTkModLabBasePath+"/share/assembly/RawSensor_4.png"  , CV_LOAD_IMAGE_COLOR);
      img = cv::imread(Config::CMSTkModLabBasePath+"/share/assembly/SensorPiece_1.png", CV_LOAD_IMAGE_COLOR);

//    img_clip_A = cv::imread(Config::CMSTkModLabBasePath+"/share/assembly/RawSensor_3_clipB_temp.png"    , CV_LOAD_IMAGE_COLOR);
      img_clip_A = cv::imread(Config::CMSTkModLabBasePath+"/share/assembly/SensorPiece_1_clipC.png"       , CV_LOAD_IMAGE_COLOR);
//    img_clip_A = cv::imread(Config::CMSTkModLabBasePath+"/share/assembly/spacer_corner_tempate_crop.png", CV_LOAD_IMAGE_COLOR);
    }
    else if(objectmode == 1)
    {
      NQLog("MarkerFinderPatRec", NQLog::Warning) << "runObjectDetection"
         << ": detection of positioning pin not implemented yet";
    }
    else if(objectmode == 2)
    {
      img        = cv::imread(Config::CMSTkModLabBasePath+"/share/assembly/glassslidecorneronbaseplate_sliverpaint_A.png"     , CV_LOAD_IMAGE_COLOR);
      img_clip_A = cv::imread(Config::CMSTkModLabBasePath+"/share/assembly/glassslidecorneronbaseplate_sliverpaint_A_clip.png", CV_LOAD_IMAGE_COLOR);
    }

    NQLog("MarkerFinderPatRec", NQLog::Debug) << "runObjectDetection"
       << ": emitting signal \"locatePickupCorner_templateMatching\"";

    emit locatePickupCorner_templateMatching(img, img_clip_A);
  }

  return;
}

void MarkerFinderPatRec::runObjectDetection_labmode(const cv::Mat& master_image)
{
    NQLog("MarkerFinderPatRec", NQLog::Debug) << "runObjectDetection_labmode";

    if(objectmode_g == 0)
    {
      NQLog("MarkerFinderPatRec", NQLog::Debug) << "runObjectDetection_labmode"
         << ": detecting fiducial marker";

      img = master_image;

//      img_clip_A = cv::imread(Config::CMSTkModLabBasePath+"/share/assembly/RawSensor_3_clipB.png", CV_LOAD_IMAGE_COLOR);
//      img_clip_A = cv::imread(Config::CMSTkModLabBasePath+"/share/assembly/RawSensor_3_clipB_temp.png", CV_LOAD_IMAGE_COLOR);
      img_clip_A = cv::imread(Config::CMSTkModLabBasePath+"/share/assembly/SensorPiece_1_clipC.png", CV_LOAD_IMAGE_COLOR);
    }
    else if(objectmode_g == 1)
    {
      NQLog("MarkerFinderPatRec", NQLog::Warning) << "runObjectDetection_labmode"
         << ": detection of positioning pin not implemented yet";
    }
    else if (objectmode_g == 2)
    {
      NQLog("MarkerFinderPatRec", NQLog::Debug) << "runObjectDetection_labmode"
         << ": detecting silver painter corner";

      img = master_image;   

//      img_clip_A = cv::imread(Config::CMSTkModLabBasePath+"/share/assembly/glassslidecorneronbaseplate_sliverpaint_A_clip.png",CV_LOAD_IMAGE_COLOR);
//      img_clip_A = cv::imread(Config::CMSTkModLabBasePath+"/share/assembly/glassslidecorner_sliverpaint_D_crop.png",CV_LOAD_IMAGE_COLOR);
      img_clip_A = cv::imread(Config::CMSTkModLabBasePath+"/share/assembly/simplecorner.png",CV_LOAD_IMAGE_COLOR);
    }
    else if (objectmode_g == 3)
    {
      NQLog("MarkerFinderPatRec", NQLog::Debug) << "runObjectDetection_labmode"
         << ": detecting spacer corner";

      img = master_image;   

      img_clip_A = cv::imread(Config::CMSTkModLabBasePath+"/share/assembly/spacer_corner_tempate_crop.png", CV_LOAD_IMAGE_COLOR);
    }

    NQLog("MarkerFinderPatRec", NQLog::Debug) << "runObjectDetection_labmode"
       << ": emitting signal \"locatePickupCorner_templateMatching\"";

    emit locatePickupCorner_templateMatching(img, img_clip_A);
}

void MarkerFinderPatRec::findMarker_templateMatching(cv::Mat img, cv::Mat img_clip_A)
{
    NQLog("MarkerFinderPatRec") << "Finding Marker (Template Matching) here";

    NQLog("MarkerFinderPatRec") << ", number of image channels =  "<<img_clip_A.channels() ;
      
    cv::Point matchLoc_1, matchLoc_2, matchLoc_final;
    cv::Mat result_1, result_2; 
    cv::Mat img_copy = img.clone();
   

    //Greyscale images
    cv::Mat img_copy_gs(img_copy.size(), img_copy.type());
    cv::Mat img_clip_A_gs(img_clip_A.size(), img_clip_A.type());
    //cv::Mat img_glass_marker_raw_gs(img_glass_marker_raw.size(), img_glass_marker_raw.type());

    if (img.channels()> 1){
    //convert color to GS
    cvtColor( img_copy,   img_copy_gs,   CV_BGR2GRAY );
    }else {
     img_copy_gs = img_copy.clone();
    }

    if (img_clip_A.channels()> 1){
    //convert color to GS
    cvtColor( img_clip_A, img_clip_A_gs, CV_BGR2GRAY );
    }else {
     img_clip_A_gs = img_clip_A.clone();
    }

    /*if (img_glass_marker_raw.channels()> 1){
    //convert color to GS
    cvtColor( img_glass_marker_raw, img_glass_marker_raw_gs, CV_BGR2GRAY );
    }else {
     img_glass_marker_raw_gs = img_glass_marker_raw.clone();
    }*/

    //Binary images
    cv::Mat img_copy_bin(img_copy_gs.size(), img_copy_gs.type());
    cv::Mat img_clip_A_bin(img_clip_A_gs.size(), img_clip_A_gs.type());
    //cv::Mat img_glass_marker_bin(img_glass_marker_raw_gs.size(), img_glass_marker_raw_gs.type());
    
    //Apply thresholding
    cv::threshold(img_copy_gs, img_copy_bin, generalThreshold_, 255, cv::THRESH_BINARY);
    cv::threshold(img_clip_A_gs, img_clip_A_bin, 85, 255, cv::THRESH_BINARY);    //90 for silicon marker, 88 for glass?
    //cv::threshold(img_glass_marker_raw_gs, img_glass_marker_bin, 88, 255, cv::THRESH_BINARY);
        
    // img_copy_bin = img_copy_gs.clone();
    // img_clip_A_bin = img_clip_A_gs.clone();
    
    const std::string filename_img_bin    = cacheDirectory1_+"/Sensor_bin.png";
    const std::string filename_clip_A_bin = cacheDirectory1_+"/clip_A_bin.png";
//    const std::string filename_img_glass_marker_bin = cacheDirectory1_+"/share/assembly/img_glass_marker_bin.png";
    
    cv::imwrite(filename_img_bin, img_copy_bin);
    cv::imwrite(filename_clip_A_bin, img_clip_A_bin);
    //cv::imwrite(filename_img_glass_marker_bin, img_glass_marker_bin);
    
    QString filename_img_bin_qs = QString::fromStdString(filename_img_bin);
    QString filename_clip_A_bin_qs = QString::fromStdString(filename_clip_A_bin);
    //QString filename_img_glass_marker_bin_qs = QString::fromStdString(filename_img_glass_marker_bin);
    
    //emit updateImage(4, filename_img_bin_qs);
    //emit updateImage(5, filename_clip_A_bin_qs);
    
    NQLog("MarkerFinderPatRec") << "Finding Marker (Template Matching) wrote images" ;
    
    /// Localizing the best match with minMaxLoc
    double FOM, FOM_inc = 1000.0, minVal, maxVal; cv::Point minLoc; cv::Point maxLoc;
    double thetas[10], FOMs[10], best_theta;
    
    //create result matrix to hold correlation values
    int result_cols =  img_copy_bin.cols - img_clip_A.cols + 1;
    int result_rows = img_copy_bin.rows - img_clip_A.rows + 1;
    

    NQLog("MarkerFinderPatRec") << ", Master cols = "<< img_copy_bin.cols << "  Master rows " << img_copy_bin.rows << " template cols << " << img_clip_A.cols << " template rows "<<img_clip_A.rows  ;


    // result_1.create( result_rows, result_cols, CV_32FC1 );
    result_1.create( result_rows, result_cols, CV_32FC1 );
    
    int match_method = 1;
    matchTemplate( img_copy_bin, img_clip_A_bin, result_1, match_method);
    
    NQLog("MarkerFinderPatRec") << "Finding Marker (Template Matching), pre- matching routine" ;

    
    minMaxLoc( result_1, &minVal, &maxVal, &minLoc, &maxLoc, cv::Mat() );
    
    /// For SQDIFF and SQDIFF_NORMED, the best matches are lower values. For all the other methods, the higher the better
    if( match_method  == CV_TM_SQDIFF || match_method == CV_TM_SQDIFF_NORMED ) {
        matchLoc_1 = minLoc;
        FOM = minVal;
    } else {
        matchLoc_1 = maxLoc;
        FOM = maxVal;
    }
    
    NQLog("MarkerFinderPatRec") << "Finding Marker (Template Matching), post- matching routine" ;

    
    //circle( img_gs, cv::Point( matchLoc_1.x + (img_clip_gs.cols/2.0) , matchLoc_1.y + (img_clip_gs.rows/2.0) ), 10, cv::Scalar(255,0,0) );
    
    //circle( img, cv::Point( matchLoc_1.x  , matchLoc_1.y ), 30, cv::Scalar(255,0,0) );
    //rectangle( img, matchLoc_1, cv::Point( matchLoc_1.x + img_clip_A_bin.cols , matchLoc_1.y + img_clip_A_bin.rows ), cv::Scalar(255,0,0), 2, 8, 0 );
    
    NQLog("MarkerFinderPatRec") <<  matchLoc_1.x<< "   "<< matchLoc_1.y;
    
    
    int i = 0;
    int color = 200;
    
    
    //    for (float theta = 20; theta < 24;  theta = theta + 0.2){
      //    for (float theta = -64.0; theta < 64.0;  theta = theta + 3.2){
    //    for (float theta = -180.0; theta < 180.0;  theta = theta + 9.0){
      
    //     for (float theta = -5.0; theta < 5.0;  theta = theta + 0.25){
    //  for (float theta = -10.0; theta <= 10.0;  theta = theta + 1.0){
  
    //        for (float theta = -3.2; theta < 3.2;  theta = theta + 0.16){

    std::string filename_rotated;

    NQLog("MarkerFinderPatRec") << "looping"<< "   ";
    float scan_start = -5.0, scan_end = 180.0, scan_step =  0.1;
    float FOM_near, FOM_far;
    float theta_coarse;

    //first check if we are at the "near" or "far" corner
   // for (float theta_coarse = 0.0; theta_coarse <= 180.0;  theta_coarse = theta_coarse + 180.0){
     for (int scan = 0; scan <= 2;  scan++){
         NQLog("MarkerFinderPatRec") << "*** Scan number "<< scan;
        
         if (scan == 0){
            theta_coarse = 0.0;
            scan_start = 0.001, scan_end = 0.001, scan_step =  1.0;
         } else if (scan == 1){
             theta_coarse = 180.0;
             scan_start = 0.001, scan_end = 0.001, scan_step =  1.0;
         }
         
         else{
             
             NQLog("MarkerFinderPatRec") << "*** Deciding corner: "<< " FOM_near  =  "<<FOM_near <<" FOM far =  "<< FOM_far ;

             //scan_start = -0.5, scan_end = 0.85, scan_step =  0.15;
             scan_start = -0.25, scan_end = 0.425, scan_step =  0.075;

             if (FOM_near < FOM_far){
                 NQLog("MarkerFinderPatRec") << " @ Near corner" ;
                 theta_coarse = 0.0;
             }else if(FOM_near > FOM_far){
                 NQLog("MarkerFinderPatRec") << " @ Far corner" ;
                 theta_coarse = 180.0;
             }
         }
        
    
	for (float theta_fine = scan_start; theta_fine <= scan_end;  theta_fine = theta_fine + scan_step){
        float theta = theta_coarse + theta_fine;

         cv::Point2f src_center(img_copy_bin.cols/2.0F, img_copy_bin.rows/2.0F);
	  // cv::Point2f src_center( matchLoc_1.x + (img_clip_A_bin.cols/2) , matchLoc_1.y + (img_clip_A_bin.rows/2) );
        
	 // Mat rot_mat = cv::getRotationMatrix2D(matchLoc_1, theta, 1.0);
        cv::Mat rot_mat = cv::getRotationMatrix2D(src_center, theta, 1.0);
        cv::Mat dst, dst_orig;
        cv::Scalar avgPixelIntensity = cv::mean( img_copy_bin );

        warpAffine(img_copy_bin, dst     , rot_mat, img_copy_bin.size(), cv::INTER_CUBIC, cv::BORDER_CONSTANT, avgPixelIntensity);
        warpAffine(img_copy_gs , dst_orig, rot_mat, img_copy_bin.size(), cv::INTER_CUBIC, cv::BORDER_CONSTANT, avgPixelIntensity);

        std::string filename_rotated_base = cacheDirectory2_+"/Rotation_result_";
        std::ostringstream ss;
        ss << theta;
        std::string theta_str = ss.str();
        filename_rotated = filename_rotated_base + theta_str+".png";
        cv::imwrite(filename_rotated, dst);
        
        //create result matrix to hold correlation values
        result_cols =  img_copy_bin.cols - img_clip_A_bin.cols + 1;
        result_rows = img_copy_bin.rows - img_clip_A_bin.rows + 1;
        
        result_2.create( result_rows, result_cols, CV_32FC1 );
        matchTemplate( dst, img_clip_A_bin, result_2, match_method );
        
        // normalize( result, result, 0, 1, NORM_MINMAX, -1, cv::Mat() );
        minMaxLoc( result_2, &minVal, &maxVal, &minLoc, &maxLoc, cv::Mat() );
        
        /// For SQDIFF and SQDIFF_NORMED, the best matches are lower values. For all the other methods, the higher the better
        if( match_method  == CV_TM_SQDIFF || match_method == CV_TM_SQDIFF_NORMED ) {
            matchLoc_2 = minLoc;
            FOM = minVal;
        } else {
            matchLoc_2 = maxLoc;
            FOM = maxVal;
        }

        std::cout << std::setprecision(10);     
        std::cout <<"theta = "<<  theta << ",  FOM = "<<  FOM<<  std::endl;
        

        if (scan == 0 ){
            FOM_near = FOM;
        }
        else if (scan == 1){
            FOM_far = FOM;
        } else {
            thetas[i] = theta;
            FOMs[i] = FOM;
            i++;

            if (FOM < FOM_inc ) {
                
                FOM_inc = FOM;
                matchLoc_final = matchLoc_2;
                best_theta = theta;
            }
        
        }
        
    
        
        color = 50 + 200*theta;
    }
         
         NQLog("MarkerFinderPatRec") << " Finished fine scan best theta = "<<  best_theta ;
    }

    cv::Mat img_raw = img.clone();
    //  rectangle( img, matchLoc_final, cv::Point( matchLoc_final.x + img_clip_A_bin.cols , matchLoc_final.y + img_clip_A_bin.rows ), cv::Scalar(255,0,255), 2, 8, 0 );

    cv::Rect rect_result = cv::Rect( matchLoc_final, cv::Point( matchLoc_final.x + img_clip_A_bin.cols, matchLoc_final.y + img_clip_A_bin.rows));

    line(img, cv::Point(img.cols/2.0, 0), cv::Point(img.cols/2.0, img.rows ), cv::Scalar(255,255,0), 2, 8, 0);
    line(img, cv::Point(0, img.rows/2.0), cv::Point(img.cols, img.rows/2.0 ), cv::Scalar(255,255,0), 2, 8, 0);
    
        if (best_theta > -5.0 && best_theta < 5.0){
            //the circle of radius 4 is meant to *roughly* represnt the x,y precision of the x-y motion stage so that the
            //use can see if the patrec results make sense (the top left corner of the marker should be within the cirle)
            line(img,cv::Point(matchLoc_final.x,matchLoc_final.y - 50),cv::Point(matchLoc_final.x + 240,matchLoc_final.y - 50 ),cv::Scalar(0,255,0),2,8,0);
            putText(img, "200 um", cv::Point(matchLoc_final.x, matchLoc_final.y - 100 ), cv::FONT_HERSHEY_SCRIPT_SIMPLEX, 2, cv::Scalar(0,255,0), 3, 8);
            circle(img, matchLoc_final, 4, cv::Scalar(255,0,255), 4, 8, 0 );
            circle(img, matchLoc_final, 4, cv::Scalar(255,0,255), 4, 8, 0 );
            rectangle(img, matchLoc_final, cv::Point( matchLoc_final.x + img_clip_A_bin.cols, matchLoc_final.y + img_clip_A_bin.rows ), cv::Scalar(255,0,255), 2, 8, 0);

     } else if (best_theta > 175.0 && best_theta < 185.0){
         line(img, cv::Point( img.cols - matchLoc_final.x - 240 ,  img.rows - matchLoc_final.y - 100 ), cv::Point( img.cols - matchLoc_final.x,  img.rows - matchLoc_final.y - 100 ), cv::Scalar(0,255,0), 2, 8, 0);
         putText(img, "200 um", cv::Point( img.cols - matchLoc_final.x - 240, img.rows - matchLoc_final.y - 50 ), cv::FONT_HERSHEY_SCRIPT_SIMPLEX, 2, cv::Scalar(0,255,0), 3, 8);
         circle( img,  cv::Point( img.cols - matchLoc_final.x ,  img.rows - matchLoc_final.y ) , 4, cv::Scalar(255,0,255), 4, 8, 0 );
         rectangle(img, cv::Point( img.cols - matchLoc_final.x,  img.rows - matchLoc_final.y ),  cv::Point( img.cols - matchLoc_final.x  - img_clip_A_bin.cols,  img.rows - matchLoc_final.y - img_clip_A_bin.rows ) , cv::Scalar(255,0,255), 2, 8, 0 );
     }

    //     circle( dst_orig, matchLoc_final, 10, cv::Scalar(0,0,255), 4, 8, 0 );
    // rectangle(dst_orig, matchLoc_final, cv::Point( matchLoc_final.x + img_clip_A_bin.cols , matchLoc_final.y + img_clip_A_bin.rows ), cv::Scalar(255,0,255), 2, 8, 0 );
  

    TCanvas *c1 = new TCanvas("c1","Rotation extraction",200,10,700,500);
    
    if (thetas[0] && FOMs[0]){
        TGraph *gr = new TGraph(10, thetas, FOMs);

        //gr->Fit("pol6");
        gr->Draw("AC*");
        gr->GetHistogram()->GetXaxis()->SetTitle("Rotational tranformation (degrees)");
        gr->GetHistogram()->GetYaxis()->SetTitle("Minimised metric value");
        gr->GetHistogram()->SetTitle("");

        TGraph *gr_bestmatch = new TGraph(1);
        gr_bestmatch->SetPoint(0, best_theta, FOM_inc);
        gr_bestmatch->SetMarkerColor(2);
        gr_bestmatch->SetMarkerStyle(22);
        gr_bestmatch->SetMarkerSize(3);

        gr_bestmatch->Draw("PSAME");

        std::string filename_canvas = cacheDirectory1_+"/RotationExtraction.png";
        c1->SaveAs(filename_canvas.c_str());
        QString filename_canvas_qs = QString::fromStdString(filename_canvas);
        c1->SaveAs((cacheDirectory1_+"test.png").c_str());

        emit updateImage(2, filename_canvas_qs);
    }
    
    std::string filename = cacheDirectory1_+"/PatRec_TM_result.png";
    cv::imwrite(filename, img);
    
 
    QString filename_qs = QString::fromStdString(filename);
    QString filename_template = QString::fromStdString(filename_clip_A_bin);
    QString filename_master = QString::fromStdString(filename_img_bin);
    QString filename_rotated_qs = QString::fromStdString(filename_rotated);
    
    emit updateImage(1, filename_qs);
    
    emit foundSensor(1);
    emit getImageBlur(img_raw, rect_result);

    emit updateImage(3, filename_template);
    emit updateImage(4, filename_master);
    
    
    //work out match location in field of view
    // the origin of the FOV coordinate system is the top left corner
    //the match loction (centre of the template) is calculated in mm
    //this should be enough for postion correction with moverealtive()
    

    //    matchLoc_x_lab = (matchLoc_final.x +  (img_clip_A_bin.cols/2) ) * (5.0/img.cols); // need to add the current X pos of the lang
    //matchLoc_y_lab = (matchLoc_final.y +  (img_clip_A_bin.rows/2) ) * (4.0/img.rows); // need to add the current Y pos of the lang
    
    //update line edits in view
    
    emit reportObjectLocation(1, matchLoc_final.x , matchLoc_final.y, best_theta);
}
