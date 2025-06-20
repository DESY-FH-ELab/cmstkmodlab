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
#include <AssemblyUtilities.h>

#include <DatabaseDESY.h>
#include <DatabaseDummy.h>

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

  double makespace_Y_;
  double makespace_Z_;
  double position_y_before_makespace_;
  double position_z_before_makespace_;
  bool position_before_makespace_stored_;

  bool use_smartMove_;
  bool in_action_;

  bool PSSPlusSpacersToMaPSAPosition_isRegistered_;
  double PSSPlusSpacersToMaPSAPosition_X_;
  double PSSPlusSpacersToMaPSAPosition_Y_;
  double PSSPlusSpacersToMaPSAPosition_Z_;
  double PSSPlusSpacersToMaPSAPosition_A_;

  double original_Z_velocity_;

  QString Baseplate_ID_, MaPSA_ID_, PSS_ID_, Glue1_ID_, Glue2_ID_, Glue3_ID_, Module_ID_;

  VDatabase* database_;

  bool alreadyClicked_LowerPickupToolOntoMaPSA, alreadyClicked_LowerPickupToolOntoPSS, alreadyClicked_LowerMaPSAOntoBaseplate, alreadyClicked_LowerPSSOntoSpacers, alreadyClicked_LowerPSSPlusSpacersOntoGluingStage, alreadyClicked_LowerPSSPlusSpacersOntoMaPSA;

  assembly::Center assembly_center_;

 public:
  assembly::Center GetAssemblyCenter() const {return assembly_center_;};

 public slots:

  void use_smartMove(const int);

  // motion
  void GoToPSPSensorMarkerPreAlignment_start() {GoToSensorMarkerPreAlignment_start(true );};
  void GoToPSSSensorMarkerPreAlignment_start() {GoToSensorMarkerPreAlignment_start(false);};

  void GoToSensorMarkerPreAlignment_start(const bool isMapsa);
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

  void MakeSpaceOnPlatform_start();
  void MakeSpaceOnPlatform_finish();

  void ReturnToPlatform_start();
  void ReturnToPlatform_finish();

  void GoFromPSSPlusSpacersToMaPSAPositionToGluingStageRefPointXY_start();
  void GoFromPSSPlusSpacersToMaPSAPositionToGluingStageRefPointXY_finish();

  void LowerPSSPlusSpacersOntoGluingStage_start();
  void LowerPSSPlusSpacersOntoGluingStage_finish();

  void ReturnToPSSPlusSpacersToMaPSAPosition_start();
  void ReturnToPSSPlusSpacersToMaPSAPosition_finish();

  void SlowlyLiftFromGluingStage_start();
  void SlowlyLiftFromGluingStage_finish();

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

  void ScanMaPSAID_start();
  void ScanPSSID_start();
  void ScanBaseplateID_start();
  void ScanGlue1ID_start();
  void ScanGlue2ID_start();
  void ScanGlue3ID_start();
  void ScanModuleID_start();

  void RegisterModuleID_start();
  void PushAllToDB_start();
  void PushStep1ToDB_start();
  void PushStep2ToDB_start();
  void PushStep3ToDB_start();

  void Update_Baseplate_ID(QString ID) {Baseplate_ID_ = ID;};
  void Update_MaPSA_ID(QString ID) {MaPSA_ID_ = ID;};
  void Update_PSS_ID(QString ID) {PSS_ID_ = ID;};
  void Update_Glue1_ID(QString ID) {Glue1_ID_ = ID;};
  void Update_Glue2_ID(QString ID) {Glue2_ID_ = ID;};
  void Update_Glue3_ID(QString ID) {Glue3_ID_ = ID;};
  void Update_Module_ID(QString ID) {Module_ID_ = ID;};

  void RegisterPSSPlusSpacersToMaPSAPosition_start();
  void RegisterPSSPlusSpacersToMaPSAPosition_finish();

  void switchToAlignmentTab_PSP();
  void switchToAlignmentTab_PSS();
  // ---------

 signals:

  // motion
  void move_absolute_request(const double, const double, const double, const double);
  void move_relative_request(const double, const double, const double, const double);

  void ScanMaPSAID_finished();
  void ScanPSSID_finished();
  void ScanBaseplateID_finished();
  void ScanGlue1ID_finished();
  void ScanGlue2ID_finished();
  void ScanGlue3ID_finished();
  void ScanModuleID_finished();

  void RegisterModuleID_finished();
  void PushAllToDB_finished();
  void PushStep1ToDB_finished();
  void PushStep2ToDB_finished();
  void PushStep3ToDB_finished();

  void ScanMaPSAID_aborted();
  void ScanBaseplateID_aborted();
  void ScanPSSID_aborted();
  void ScanGlue1ID_aborted();
  void ScanGlue2ID_aborted();
  void ScanGlue3ID_aborted();
  void ScanModuleID_aborted();

  void RegisterModuleID_aborted();
  void PushAllToDB_aborted();
  void PushStep1ToDB_aborted();
  void PushStep2ToDB_aborted();
  void PushStep3ToDB_aborted();

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
  void MakeSpaceOnPlatform_finished();
  void ReturnToPlatform_finished();
  void PSSPlusSpacersToMaPSAPosition_registered();

  void GoFromPSSPlusSpacersToMaPSAPositionToGluingStageRefPointXY_finished();
  void LowerPSSPlusSpacersOntoGluingStage_finished();
  void ReturnToPSSPlusSpacersToMaPSAPosition_finished();

  void SlowlyLiftFromGluingStage_finished();
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

  void MaPSA_ID_updated(const QString);
  void PSS_ID_updated(const QString);
  void Baseplate_ID_updated(const QString);
  void Glue1_ID_updated(const QString);
  void Glue2_ID_updated(const QString);
  void Glue3_ID_updated(const QString);
  void Module_ID_updated(const QString);
  // ------


  void DBLogMessage(const QString);
};

#endif // ASSEMBLYASSEMBLYV2_H
