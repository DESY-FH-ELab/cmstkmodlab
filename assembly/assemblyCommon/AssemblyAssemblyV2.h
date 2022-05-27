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

#ifndef ASSEMBLYASSEMBLYV2_H
#define ASSEMBLYASSEMBLYV2_H

#include <QObject>

#include <LStepExpressMotionManager.h>
#include <RelayCardManager.h>

#include <AssemblySmartMotionManager.h>
#include <ApplicationConfig.h>

class AssemblyAssemblyV2 : public QObject
{
 Q_OBJECT

 public:
  explicit AssemblyAssemblyV2(const LStepExpressMotionManager* const, const RelayCardManager* const, const AssemblySmartMotionManager* const smart_motion=nullptr, QObject* parent=nullptr);
  virtual ~AssemblyAssemblyV2() {}

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

  double pickup1_Z_;
  double pickup2_Z_;

  bool use_smartMove_;
  bool in_action_;

  bool PSSPlusSpacersToMaPSAPosition_isRegistered_;
  double PSSPlusSpacersToMaPSAPosition_X_;
  double PSSPlusSpacersToMaPSAPosition_Y_;
  double PSSPlusSpacersToMaPSAPosition_Z_;
  double PSSPlusSpacersToMaPSAPosition_A_;

  bool alreadyClicked_LowerPickupToolOntoMaPSA, alreadyClicked_LowerPickupToolOntoPSS, alreadyClicked_LowerMaPSAOntoBaseplate, alreadyClicked_LowerPSSOntoSpacers, alreadyClicked_LowerPSSPlusSpacersOntoGluingStage, alreadyClicked_LowerPSSPlusSpacersOntoMaPSA;

 public slots:

  void use_smartMove(const int);

  // motion
  void GoToPspSensorMarkerPreAlignment_start() {GoToSensorMarkerPreAlignment_start(true );};
  void GoToPssSensorMarkerPreAlignment_start() {GoToSensorMarkerPreAlignment_start(false);};
  
  void GoToSensorMarkerPreAlignment_start(bool isMapsa);
  void GoToSensorMarkerPreAlignment_finish();

  void GoFromSensorMarkerToPickupXY_start();
  void GoFromSensorMarkerToPickupXY_finish();

  void LowerPickupToolOntoMaPSA_start();
  void LowerPickupToolOntoMaPSA_finish();

  void LowerPickupToolOntoPSS_start();
  void LowerPickupToolOntoPSS_finish();

  void PickupPSS_start();
  void PickupPSS_finish();

  void GoToXYAPositionToGluePSSToSpacers_start();
  void GoToXYAPositionToGluePSSToSpacers_finish();

  void LowerPSSOntoSpacers_start();
  void LowerPSSOntoSpacers_finish();

  void PickupPSSPlusSpacers_start();
  void PickupPSSPlusSpacers_finish();

  void GoToPSPMarkerIdealPosition_start();
  void GoToPSPMarkerIdealPosition_finish();

  void ApplyPSPToPSSXYOffset_start();
  void ApplyPSPToPSSXYOffset_finish();

  void GoFromPSSPlusSpacersToMaPSAPositionToGluingStageRefPointXY_start();
  void GoFromPSSPlusSpacersToMaPSAPositionToGluingStageRefPointXY_finish();

  void LowerPSSPlusSpacersOntoGluingStage_start();
  void LowerPSSPlusSpacersOntoGluingStage_finish();

  void ReturnToPSSPlusSpacersToMaPSAPosition_start();
  void ReturnToPSSPlusSpacersToMaPSAPosition_finish();

  void LowerPSSPlusSpacersOntoMaPSA_start();
  void LowerPSSPlusSpacersOntoMaPSA_finish();

  void PickupMaPSA_start();
  void PickupMaPSA_finish();

  void GoToXYAPositionToGlueMaPSAToBaseplate_start();
  void GoToXYAPositionToGlueMaPSAToBaseplate_finish();

  void LowerMaPSAOntoBaseplate_start();
  void LowerMaPSAOntoBaseplate_finish();

  void LiftUpPickupTool_start();
  void LiftUpPickupTool_finish();
  // ---------

  // vacuum
  void EnableVacuumPickupTool_start();
  void EnableVacuumPickupTool_finish();

  void DisableVacuumPickupTool_start();
  void DisableVacuumPickupTool_finish();

  void EnableVacuumSpacers_start();
  void EnableVacuumSpacers_finish();

  void DisableVacuumSpacers_start();
  void DisableVacuumSpacers_finish();

  void EnableVacuumBaseplate_start();
  void EnableVacuumBaseplate_finish();

  void DisableVacuumBaseplate_start();
  void DisableVacuumBaseplate_finish();

  void AssemblyCompleted_start();
  // ---------

  // others

  void RegisterPSSPlusSpacersToMaPSAPosition_start();
  void RegisterPSSPlusSpacersToMaPSAPosition_finish();

  void switchToAlignmentTab_PSP();
  void switchToAlignmentTab_PSS();
  // ---------

 signals:

  // motion
  void move_absolute_request(const double, const double, const double, const double);
  void move_relative_request(const double, const double, const double, const double);

  void GoToSensorMarkerPreAlignment_finished();

  void GoFromSensorMarkerToPickupXY_finished();

  void LowerPickupToolOntoMaPSA_finished();
  void PickupMaPSA_finished();

  void GoToXYAPositionToGlueMaPSAToBaseplate_finished();
  void LowerMaPSAOntoBaseplate_finished();

  void LowerPickupToolOntoPSS_finished();
  void PickupPSS_finished();

  void GoToXYAPositionToGluePSSToSpacers_finished();
  void LowerPSSOntoSpacers_finished();
  void PickupPSSPlusSpacers_finished();

  void GoToPSPMarkerIdealPosition_finished();

  void ApplyPSPToPSSXYOffset_finished();
  void PSSPlusSpacersToMaPSAPosition_registered();

  void GoFromPSSPlusSpacersToMaPSAPositionToGluingStageRefPointXY_finished();
  void LowerPSSPlusSpacersOntoGluingStage_finished();
  void ReturnToPSSPlusSpacersToMaPSAPosition_finished();

  void LowerPSSPlusSpacersOntoMaPSA_finished();
  void LiftUpPickupTool_finished();
  // ------

  // vacuum
  void vacuum_ON_request(const int);
  void vacuum_OFF_request(const int);

  void EnableVacuumPickupTool_finished();
  void DisableVacuumPickupTool_finished();

  void EnableVacuumSpacers_finished();
  void DisableVacuumSpacers_finished();

  void EnableVacuumBaseplate_finished();
  void DisableVacuumBaseplate_finished();

  void AssemblyCompleted_finished();
  // ------

  // others
  void RegisterPSSPlusSpacersToMaPSAPosition_finished();
  void switchToAlignmentTab_PSP_request();
  void switchToAlignmentTab_PSS_request();
  // ------


  void DBLogMessage(const QString);
};

#endif // ASSEMBLYASSEMBLYV2_H
