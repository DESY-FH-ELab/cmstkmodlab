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

#ifndef AlignmentCheck_h
#define AlignmentCheck_h

#include <LStepExpressMotionManager.h>
#include <AssemblyObjectFinderPatRec.h>

#include <QObject>

class AlignmentCheck : public QObject
{
 Q_OBJECT

 public:
  explicit AlignmentCheck(const LStepExpressMotionManager* const, QObject* parent=nullptr);
  virtual ~AlignmentCheck() {}

  void reset();

  class Configuration {

   public:
    explicit Configuration() { this->reset(); }
    virtual ~Configuration() {}

    void reset();

    bool is_valid() const;

    double object_deltaX;
    double object_deltaY;

    double target_angle;

    bool only_measure_angle;

    bool complete_at_position1;

    bool use_autofocusing;

    double angle_max_dontIter;
    double angle_max_complete;

    AssemblyObjectFinderPatRec::Configuration PatRecOne_configuration;
    AssemblyObjectFinderPatRec::Configuration PatRecTwo_configuration;
    AssemblyObjectFinderPatRec::Configuration PatRecThree_configuration;
    AssemblyObjectFinderPatRec::Configuration PatRecFour_configuration;

  };

  const Configuration& configuration() const { return configuration_; }

  protected:

    const LStepExpressMotionManager* const motion_manager_;

    Configuration configuration_;

    bool motion_manager_enabled_;

    void enable_motion_manager(const bool);

    void    connect_motion_manager() { this->enable_motion_manager(true) ; }
    void disconnect_motion_manager() { this->enable_motion_manager(false); }

    // parameters
    double angle_max_dontIter_;
    double angle_max_complete_;

    // transient data (values to be updated during alignment)
    int alignment_step_;

    double posi_x1_, posi_y1_;
    double posi_x2_, posi_y2_;
    double posi_x3_, posi_y3_;
    double posi_x4_, posi_y4_;
 
    double obj_angle_deg_;

  public slots:

    void update_configuration(const Configuration&);

    void redirect_image(const cv::Mat&);

    void execute();

    void run_alignment(const double, const double, const double);

    void launch_next_alignment_step();

    void move_relative(const double, const double, const double, const double);

    //    void move_absolute(const double, const double, const double, const double);


    void complete_motion();

    //void GoToSensorMarkerPreAlignment_start();
    //    void GoToSensorMarkerPreAlignment_finish();
    // void GoFromSensorMarkerToPSSLL_start();
    //void GoFromSensorMarkerToPSSLL_finish();
    
    //void GoFromPSSLLToPSSTL_start();
    //void GoFromPSSLLToPSSTL_finish();
      
    //    void GoFromPSSTLToPSPTL_start();
    //void GoFromPSSTLToPSPTL_finish();

    //void RegisterPSPToPSSPosition_start();
    //void RegisterPSPToPSSPosition_finish();

  // ---------        
               

  signals:

    void configuration_updated();

    void nextAlignmentStep(const double, const double, const double);

    void image_PatRecOne(const cv::Mat&);
    void image_PatRecTwo(const cv::Mat&);
    void image_PatRecThree(const cv::Mat&);
    void image_PatRecFour(const cv::Mat&);
    
    void image_request();
    void autofocused_image_request();

    void PatRec_request(const AssemblyObjectFinderPatRec::Configuration&);

    void move_relative_request(const double, const double, const double, const double);

    void motion_completed();

    void measured_angle(const double);

    void pspbl_pos(const double, const double);
    void pssbl_pos(const double, const double);
    void psstl_pos(const double, const double);
    void psptl_pos(const double, const double);
  
    void offs_pos(const double, const double);

    
    void execution_completed();
    void move_absolute_request(const double, const double, const double, const double);


    void GoToSensorMarkerPreAlignment_finished();
    //void GoFromSensorMarkerToPSSLL_finished();
    //void GoFromPSSLLToPSSTL_finished();
    //void GoFromPSSTLToPSPTL_finished();

    void RegisterPSPToPSSPosition_finished();
    void PSPToPSSPosition_Registered();



};




#endif // AssemblyObjectAligner_h


