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

#ifndef ASSEMBLYMULTIPICKUPTESTER_H
#define ASSEMBLYMULTIPICKUPTESTER_H

#include <LStepExpressMotionManager.h>

class AssemblyMultiPickupTester : public QObject
{
 Q_OBJECT

 public:

  explicit AssemblyMultiPickupTester(const LStepExpressMotionManager*, QObject* parent=0);
  virtual ~AssemblyMultiPickupTester();

  const LStepExpressMotionManager* motion_manager() const { return motion_manager_; }

  class Configuration
  {
   public:

    explicit Configuration
    (
      const double measur_x=0., const double measur_y=0., const double measur_z=0.,
      const double pickup_x=0., const double pickup_y=0., const double pickup_z=0.,
      const int    iteraN=0
    ) :
      measur_x_(measur_x), measur_y_(measur_y), measur_z_(measur_z),
      pickup_x_(pickup_x), pickup_y_(pickup_y), pickup_z_(pickup_z),
      iteraN_  (iteraN)
    {}

    virtual ~Configuration() {}

    double measurement_X() const { return measur_x_; }
    double measurement_Y() const { return measur_y_; }
    double measurement_Z() const { return measur_z_; }

    double pickup_X() const { return pickup_x_; }
    double pickup_Y() const { return pickup_y_; }
    double pickup_Z() const { return pickup_z_; }

    int  iterations() const { return iteraN_; }

   protected:

    double measur_x_;
    double measur_y_;
    double measur_z_;
    double pickup_x_;
    double pickup_y_;
    double pickup_z_;
    int    iteraN_;
  };

  void set_configuration(const Configuration& conf){ conf_ = conf; }

 protected:

  const LStepExpressMotionManager* motion_manager_;

  bool motion_manager_enabled_;

  void enable_motion_manager(const bool);

  void    connect_motion_manager() { this->enable_motion_manager(true) ; }
  void disconnect_motion_manager() { this->enable_motion_manager(false); }

  void reset();

  int    pickup_vacuum_;
  int    pickup_basepl_;
  double pickup_deltaZ_;

  enum Mode { Mode_None, Mode_measurement, Mode_pickup };
  enum Movement { Movement_None, Movement_XY, Movement_Z };

  int itera_counter_;

  Mode          mode_;
  Movement      move_;
  Configuration conf_;

  bool use_vacuumBP_;

  bool vacuum_on_;
  bool vacuumBP_on_;
  bool pickup_done_;
  bool picked_up_;

 public slots:

  void start_measurement();
  void finish_measurement(const int exit_code=0);
  void start_pickup();
  void setup_next_step();

 signals:

  void measurement_finished();
  void      pickup_finished();
  void        test_finished();

  void measurement_request();

  void move_relative(const double, const double, const double, const double);

  void vacuum_toggle(const int);
};

#endif // ASSEMBLYMULTIPICKUPTESTER_H
