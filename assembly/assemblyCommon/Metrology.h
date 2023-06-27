////////////////////////////////////////////////////////////////////////////////
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

#ifndef Metrology_h
#define Metrology_h

#include <LStepExpressMotionManager.h>
#include <AssemblyObjectFinderPatRec.h>

#include <QObject>

class Metrology : public QObject
{
 Q_OBJECT

 public:
  explicit Metrology(const LStepExpressMotionManager* const, QObject* parent=nullptr);
  virtual ~Metrology() {}

  class Configuration {

   public:
    explicit Configuration() { this->reset(); }
    virtual ~Configuration() {}

    void reset();

    bool is_valid() const;

    bool complete_at_position1;
    bool use_autofocusing;

    //Cfg instances which must be transmitted when running PatRec on markers 1 and 2
    AssemblyObjectFinderPatRec::Configuration PatRec_PSP1_configuration;
    AssemblyObjectFinderPatRec::Configuration PatRec_PSP2_configuration;
    AssemblyObjectFinderPatRec::Configuration PatRec_PSS1_configuration;
    AssemblyObjectFinderPatRec::Configuration PatRec_PSS2_configuration;
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

    // transient data (values to be updated during alignment)
    int metrology_step_;

    double posi_PSp_x1_, posi_PSp_y1_, posi_PSp_z1_;
    double posi_PSp_x2_, posi_PSp_y2_, posi_PSp_z2_;
    double posi_PSs_x1_, posi_PSs_y1_, posi_PSs_z1_;
    double posi_PSs_x2_, posi_PSs_y2_, posi_PSs_z2_;

    double PSp_angle_deg_;
    double PSs_angle_deg_;

    bool moving_to_start_;

    void reset();

  public slots:

    void update_configuration(const Configuration&);

    void redirect_image(const cv::Mat&);

    void execute();

    void run_metrology(const double, const double, const double);

    void launch_next_metrology_step();

    void move_relative(const double, const double, const double, const double);

    void complete_motion();

    void move_to_start();

    void clear_results();

  signals:

    void configuration_updated();

    void nextMetrologyStep(const double, const double, const double);

    void image_PatRecOne(const cv::Mat&);
    void image_PatRecTwo(const cv::Mat&);
    void image_PatRecThree(const cv::Mat&);
    void image_PatRecFour(const cv::Mat&);

    void image_request();
    void autofocused_image_request();

    void PatRec_request(const AssemblyObjectFinderPatRec::Configuration&);

    void move_relative_request(const double, const double, const double, const double);

    void motion_completed();

    void measured_angle(const bool, const double);

    void measured_results(const double, const double, const double, const double, const double, const double, const double);

    void execution_completed();

    void DBLogMessage(const QString);
};

#endif // Metrology_h
