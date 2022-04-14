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

#ifndef AssemblyObjectAligner_h
#define AssemblyObjectAligner_h

#include <LStepExpressMotionManager.h>
#include <AssemblyObjectFinderPatRec.h>

#include <QObject>

class AssemblyObjectAligner : public QObject
{
 Q_OBJECT

 public:
  explicit AssemblyObjectAligner(const LStepExpressMotionManager* const, QObject* parent=nullptr);
  virtual ~AssemblyObjectAligner() {}

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

    //Cfg instances which must be transmitted when running PatRec on markers 1 and 2
    AssemblyObjectFinderPatRec::Configuration PatRecOne_configuration;
    AssemblyObjectFinderPatRec::Configuration PatRecTwo_configuration;
  };

  const Configuration& configuration() const { return configuration_; }

  protected:

    const LStepExpressMotionManager* const motion_manager_;

    Configuration configuration_;
    const ApplicationConfig* config_;

    bool motion_manager_enabled_;

    void enable_motion_manager(const bool);

    void connect_motion_manager() { this->enable_motion_manager(true) ; }
    void disconnect_motion_manager() { this->enable_motion_manager(false); }

    // maximum number of allowed iterations in a single alignment execution
    int max_numOfRotations_;

    // transient data (values to be updated during alignment)
    int alignment_step_;
    int counter_numOfRotations_; //Count the number of rotations executed during the alignment routine

    double posi_x1_, posi_y1_;
    double posi_x2_, posi_y2_;

    double obj_angle_deg_;

    void reset();
    void reset_counter_numOfRotations();

  public slots:

    void update_configuration(const Configuration&);

    void redirect_image(const cv::Mat&);

    void execute();

    void run_alignment(const double, const double, const double);

    void launch_next_alignment_step();

    void move_relative(const double, const double, const double, const double);

    void complete_motion();

  signals:

    void configuration_updated();

    void nextAlignmentStep(const double, const double, const double);

    void image_PatRecOne(const cv::Mat&);
    void image_PatRecTwo(const cv::Mat&);

    void image_request();
    void autofocused_image_request();

    void PatRec_request(const AssemblyObjectFinderPatRec::Configuration&);

    void move_relative_request(const double, const double, const double, const double);

    void motion_completed();

    void measured_angle(const double);

    void execution_completed();

    void DBLogMessage(const QString);
};

#endif // AssemblyObjectAligner_h
