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

#ifndef MULTIPICKUPTESTER_H
#define MULTIPICKUPTESTER_H

#include <LStepExpressMotionManager.h>

class MultiPickupTester : public QObject
{
 Q_OBJECT

 public:

  explicit MultiPickupTester(LStepExpressMotionManager*, QObject* parent=0);
  virtual ~MultiPickupTester();

  LStepExpressMotionManager* motion_manager() const { return motion_manager_; }

  enum Mode { None, measurement, pickup };

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

  LStepExpressMotionManager* motion_manager_;

  int itera_counter_;

  double pickup_deltaZ_;

  Mode          mode_;
  Configuration conf_;

  bool vacuum_on_   = false;
  bool pickup_done_ = false;
  bool picked_up_   = false;

 public slots:

  void execute(const Configuration&);

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

#endif // MULTIPICKUPTESTER_H
