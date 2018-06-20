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

#include <AssemblyParameters.h>

class AssemblyAssembly : public QObject
{
 Q_OBJECT

 public:
  explicit AssemblyAssembly(const LStepExpressMotionManager* const, const ConradManager* const, QObject* parent=nullptr);
  virtual ~AssemblyAssembly() {}

  AssemblyParameters* parameters() const;

  const LStepExpressMotionManager* motion() const;
  const ConradManager*             vacuum() const;

 protected:

  const LStepExpressMotionManager* const motion_;
  const ConradManager*             const vacuum_;

 public slots:

  void GoToSensorMarkerPreAlignment_start();
  void GoToSensorMarkerPreAlignment_finish();

 signals:

  void move_absolute(const double, const double, const double, const double);
  void move_relative(const double, const double, const double, const double);
};

#endif // ASSEMBLYASSEMBLY_H
