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

#ifndef ASSEMBLYASSEMBLY_H
#define ASSEMBLYASSEMBLY_H

#include <QObject>

#include <LStepExpressMotionManager.h>
#include <ConradManager.h>

#include <AssemblySmartMotionManager.h>
#include <AssemblyParameters.h>

#include <vector>

class AssemblyAssembly : public QObject
{
 Q_OBJECT

 public:
  explicit AssemblyAssembly(const LStepExpressMotionManager* const, const ConradManager* const, const AssemblySmartMotionManager* const smart_motion=nullptr, QObject* parent=nullptr);
  virtual ~AssemblyAssembly() {}

  AssemblyParameters* parameters() const;

  const LStepExpressMotionManager* motion() const;
  const ConradManager*             vacuum() const;

  const AssemblySmartMotionManager* smart_motion() const;

 protected:

  const LStepExpressMotionManager* const motion_;
  const ConradManager*             const vacuum_;

  const AssemblySmartMotionManager* const smart_motion_;

  int vacuum_pickup_;
  int vacuum_spacer_;
  int vacuum_basepl_;

  double pickup1_Z_;
  double pickup2_Z_;

  bool use_smartMove_;

 public slots:

  void use_smartMove(const int);

  // motion
  void GoToSensorMarkerPreAlignment_start();
  void GoToSensorMarkerPreAlignment_finish();

  void GoFromSensorMarkerToPickupXY_start();
  void GoFromSensorMarkerToPickupXY_finish();

  void LowerPickupToolOntoPSS_start();
  void LowerPickupToolOntoPSS_finish();

  void PickupPSS_start();
  void PickupPSS_finish();

  void GoToSpacerRefPoint_start();
  void GoToSpacerRefPoint_finish();

  void GoFromSpacerRefPointToSpacerGluingXYPosition_start();
  void GoFromSpacerRefPointToSpacerGluingXYPosition_finish();

  void LowerPSSOntoSpacers_start();
  void LowerPSSOntoSpacers_finish();

  void PickupSpacersAndPSS_start();
  void PickupSpacersAndPSS_finish();

  void ApplyPSPToPSSXYOffset_start();
  void ApplyPSPToPSSXYOffset_finish();

  void LiftUpPickupTool_start();
  void LiftUpPickupTool_finish();

  void LowerSpacersAndPSSOntoPSP_start();
  void LowerSpacersAndPSSOntoPSP_finish();

  void PickupPSPAndPSS_start();
  void PickupPSPAndPSS_finish();

  void GoToBaseplateRefPoint_start();
  void GoToBaseplateRefPoint_finish();

  void GoFromBaseplateRefPointToBaseplateGluingXYPosition_start();
  void GoFromBaseplateRefPointToBaseplateGluingXYPosition_finish();

  void LowerSensorAssemblyOntoBaseplate_start();
  void LowerSensorAssemblyOntoBaseplate_finish();
  // ---------

  // vacuum
  void  EnableVacuumPickupTool_start();
  void  EnableVacuumPickupTool_finish();

  void DisableVacuumPickupTool_start();
  void DisableVacuumPickupTool_finish();

  void  EnableVacuumSpacers_start();
  void  EnableVacuumSpacers_finish();

  void DisableVacuumSpacers_start();
  void DisableVacuumSpacers_finish();

  void  EnableVacuumBaseplate_start();
  void  EnableVacuumBaseplate_finish();

  void DisableVacuumBaseplate_start();
  void DisableVacuumBaseplate_finish();
  // ---------

 signals:

  // motion
  void move_absolute_request(const double, const double, const double, const double);
  void move_relative_request(const double, const double, const double, const double);

  void GoToSensorMarkerPreAlignment_finished();

  void GoFromSensorMarkerToPickupXY_finished();

  void LowerPickupToolOntoPSS_finished();

  void PickupPSS_finished();

  void GoToSpacerRefPoint_finished();

  void GoFromSpacerRefPointToSpacerGluingXYPosition_finished();

  void LowerPSSOntoSpacers_finished();

  void PickupSpacersAndPSS_finished();

  void ApplyPSPToPSSXYOffset_finished();

  void LiftUpPickupTool_finished();

  void LowerSpacersAndPSSOntoPSP_finished();

  void PickupPSPAndPSS_finished();

  void GoToBaseplateRefPoint_finished();

  void GoFromBaseplateRefPointToBaseplateGluingXYPosition_finished();

  void LowerSensorAssemblyOntoBaseplate_finished();
  // ------

  // vacuum
  void vacuum_ON_request (const int);
  void vacuum_OFF_request(const int);

  void  EnableVacuumPickupTool_finished();
  void DisableVacuumPickupTool_finished();

  void  EnableVacuumSpacers_finished();
  void DisableVacuumSpacers_finished();

  void  EnableVacuumBaseplate_finished();
  void DisableVacuumBaseplate_finished();
  // ------
};

#endif // ASSEMBLYASSEMBLY_H
