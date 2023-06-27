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

#ifndef AssemblySmartMotionManager_h
#define AssemblySmartMotionManager_h

#include <LStepExpressMotionManager.h>

#include <QObject>
#include <QQueue>

#include <vector>

class AssemblySmartMotionManager : public QObject
{
 Q_OBJECT

 public:

  explicit AssemblySmartMotionManager(const LStepExpressMotionManager* const, QObject* parent=nullptr);
  virtual ~AssemblySmartMotionManager() {}

  static QQueue<LStepExpressMotion> smartMotions_relative(const double, const double, const double, const double, const std::vector<double>&, int&, const double);

 protected:

  const LStepExpressMotionManager* const motion_manager_;

  bool motion_manager_enabled_;

  int motion_index_;

  int smartMotions_N_;

  std::vector<double> smartMove_steps_dZ_;

  double smartMove_initial_step_up_dZ_;

  QQueue<LStepExpressMotion> motions_;

  void smartMove_window(const LStepExpressMotion&);

 public slots:

  void enable_motion_manager(const bool);

  void    connect_motion_manager() { this->enable_motion_manager(true) ; }
  void disconnect_motion_manager() { this->enable_motion_manager(false); }

  void move_relative(const double, const double, const double, const double);
  void move_absolute(const double, const double, const double, const double);

  void next_step();

 signals:

  void move_relative_request(const double, const double, const double, const double);
  void move_absolute_request(const double, const double, const double, const double);

  void motion_request(const LStepExpressMotion&);

  void motion_completed();
};

#endif // AssemblySmartMotionManager_h
