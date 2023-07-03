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

#ifndef ASSEMBLYSUBASSEMBLYPICKUP_H
#define ASSEMBLYSUBASSEMBLYPICKUP_H

#include <LStepExpressMotionManager.h>

class AssemblySubassemblyPickup : public QObject
{
 Q_OBJECT

 public:

  explicit AssemblySubassemblyPickup(const LStepExpressMotionManager*, QObject* parent=nullptr);
  virtual ~AssemblySubassemblyPickup();

  const LStepExpressMotionManager* motion_manager() const { return motion_manager_; }

 private:
  Q_DISABLE_COPY(AssemblySubassemblyPickup)

 protected:

  mutable QMutex mutex_;

  const LStepExpressMotionManager* motion_manager_;

  bool motion_manager_enabled_;

  void enable_motion_manager(const bool);

  void    connect_motion_manager() { this->enable_motion_manager(true) ; }
  void disconnect_motion_manager() { this->enable_motion_manager(false); }

  void reset();


 public slots:


 signals:

};

#endif // ASSEMBLYSUBASSEMBLYPICKUP_H
