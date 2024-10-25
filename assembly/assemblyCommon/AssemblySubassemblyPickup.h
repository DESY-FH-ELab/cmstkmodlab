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
#include <RelayCardManager.h>

#include <AssemblySmartMotionManager.h>
#include <ApplicationConfig.h>

class AssemblySubassemblyPickup : public QObject
{
 Q_OBJECT

 public:

  explicit AssemblySubassemblyPickup(const LStepExpressMotionManager* const, const RelayCardManager* const, const AssemblySmartMotionManager* const smart_motion=nullptr, QObject* parent=nullptr);
  virtual ~AssemblySubassemblyPickup();

  const LStepExpressMotionManager* motion() const;
  const RelayCardManager* vacuum() const;

  const AssemblySmartMotionManager* smart_motion() const;

 protected:
  const LStepExpressMotionManager* const motion_;
  const RelayCardManager* const vacuum_;

  const AssemblySmartMotionManager* const smart_motion_;

  const ApplicationConfig* config_;

  int vacuum_pickup_;
  int vacuum_spacer_;
  int vacuum_basepl_;
  int vacuum_sub_;

  bool use_smartMove_;
  bool in_action_;

  bool alreadyClicked_LowerPickupToolOntoSubassembly;

  double pickup1_Z_;

  void reset();


 public slots:

  void use_smartMove(const int);

  void EnableVacuumSpacers_start();
  void EnableVacuumSpacers_finish();

  void GoToSensorMarkerPreAlignment_start();
  void GoToSensorMarkerPreAlignment_finish();

  void EnableVacuumSubassembly_start();
  void EnableVacuumSubassembly_finish();

  void switchToAlignmentTab_PSS();

  void GoFromSensorMarkerToPickupXY_start();
  void GoFromSensorMarkerToPickupXY_finish();

  void EnableVacuumPickupTool_start();
  void EnableVacuumPickupTool_finish();

  void LowerPickupToolOntoSubassembly_start();
  void LowerPickupToolOntoSubassembly_finish();

  void DisableVacuumSubassembly_start();
  void DisableVacuumSubassembly_finish();

  void PickupSubassembly_start();
  void PickupSubassembly_finish();

  void DisableVacuumSpacers_start();
  void DisableVacuumSpacers_finish();

 signals:

  // motion
  void move_absolute_request(const double, const double, const double, const double);
  void move_relative_request(const double, const double, const double, const double);

  void GoToSensorMarkerPreAlignment_finished();
  void GoFromSensorMarkerToPickupXY_finished();
  void LowerPickupToolOntoSubassembly_finished();
  void PickupSubassembly_finished();

  void vacuum_ON_request(const int);
  void vacuum_OFF_request(const int);

  void EnableVacuumPickupTool_finished();
  void DisableVacuumPickupTool_finished();

  void EnableVacuumSpacers_finished();
  void DisableVacuumSpacers_finished();

  void EnableVacuumSubassembly_finished();
  void DisableVacuumSubassembly_finished();

  void switchToAlignmentTab_PSS_request();

  void DBLogMessage(const QString);

};

#endif // ASSEMBLYSUBASSEMBLYPICKUP_H
