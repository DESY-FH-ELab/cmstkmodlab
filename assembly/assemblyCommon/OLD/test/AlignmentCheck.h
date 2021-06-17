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

/////////////////////////////////////////////////////////////////////////////////
//                                                                             //
//            Fourth Vacuum Line Capability Added by Elise Hinkle              //
//                       Last Modified October 7, 2019                         //
//                                                                             //
/////////////////////////////////////////////////////////////////////////////////

#ifndef ALIGNMENTCHECK_H
#define ALIGNMENTCHECK_H

#include <QObject>

#include <LStepExpressMotionManager.h>
// #include <ConradManager.h> // CONRAD
#include <VellemanManager.h> // VELLEMAN

#include <AssemblySmartMotionManager.h>
#include <AssemblyParameters.h>

#include <vector>

class AlignmentCheck : public QObject
{
 Q_OBJECT

 public:
  // explicit AssemblyAssembly(const LStepExpressMotionManager* const, const ConradManager* const, const AssemblySmartMotionManager* const smart_motion=nullptr, QObject* parent=nullptr); // CONRAD 
  explicit AlignmentCheck(const LStepExpressMotionManager* const, const VellemanManager* const, const AssemblySmartMotionManager* const smart_motion=nullptr, QObject* parent=nullptr);  // VELLEMAN

  virtual ~AlignmentCheck() {}

  AssemblyParameters* parameters() const;

  const LStepExpressMotionManager* motion() const;
  // const ConradManager*             vacuum() const; // CONRAD
  const VellemanManager*           vacuum() const;    // VELLEMAN

  const AssemblySmartMotionManager* smart_motion() const;

 protected:

  const LStepExpressMotionManager* const motion_;
  // const ConradManager*             const vacuum_; // CONRAD
  const VellemanManager*           const vacuum_;    // VELLEMAN
  
  const AssemblySmartMotionManager* const smart_motion_;

  int vacuum_pickup_;
  int vacuum_spacer_;
  int vacuum_basepl_;
  int vacuum_stage_;

  double pickup1_Z_;
  double pickup2_Z_;

  bool use_smartMove_;

  bool   PSPToPSSPosition_isRegistered_;
  double PSPToPSSPosition_X_;
  double PSPToPSSPosition_Y_;
  double PSPToPSSPosition_Z_;
  double PSPToPSSPosition_A_;

 public slots:

  void use_smartMove(const int);

  // motion
  void GoToSensorMarkerPreAlignment_start();
  void GoToSensorMarkerPreAlignment_finish();

  //void GoFromSensorMarkerToPickupXY_start();
  //void GoFromSensorMarkerToPickupXY_finish();

  //void LowerPickupToolOntoPSS_start();
  //void LowerPickupToolOntoPSS_finish();

  //  void PickupPSS_start();
  //void PickupPSS_finish();

  void GoFromSensorMarkerToPSSLL_start();
  void GoFromSensorMarkerToPSSLL_finish();
  
  void GoFromPSSLLToPSSTL_start();
  void GoFromPSSLLToPSSTL_finish();

  void GoFromPSSTLToPSPTL_start();
  void GoFromPSSTLToPSPTL_finish();

  void RegisterPSPToPSSPosition_start();
  void RegisterPSPToPSSPosition_finish();
  
  // ---------


  // ---------

 signals:

  // motion
  void move_absolute_request(const double, const double, const double, const double);
  void move_relative_request(const double, const double, const double, const double);

  void GoToSensorMarkerPreAlignment_finished();
  void GoFromSensorMarkerToPSSLL_finished();
  void GoFromPSSLLToPSSTL_finished();
  void GoFromPSSTLToPSPTL_finished();

  void RegisterPSPToPSSPosition_finished();
  void PSPToPSSPosition_Registered();
  
  void GoFromSensorMarkerToPickupXY_finished();

    // ------
};

#endif // ALIGNMENTCHECK_H
