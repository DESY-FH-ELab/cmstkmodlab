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

#ifndef ASSEMBLYTOOLBOXVIEW_H
#define ASSEMBLYTOOLBOXVIEW_H

#include <LStepExpressMotionManager.h>
#include <AssemblyMultiPickupTesterWidget.h>
#include <AssemblyPositionsRegistryWidget.h>

#include <QWidget>
#include <QMessageBox>

class AssemblyToolboxView : public QWidget
{
 Q_OBJECT

 public:

  explicit AssemblyToolboxView(const LStepExpressMotionManager* const, QWidget* parent=nullptr);
  virtual ~AssemblyToolboxView() {}

  AssemblyPositionsRegistryWidget* PositionsRegistry_Widget() { return posreg_wid_; }
  AssemblyMultiPickupTesterWidget* MultiPickupTester_Widget() { return mupite_wid_; }

 protected:

  AssemblyPositionsRegistryWidget* posreg_wid_;
  AssemblyMultiPickupTesterWidget* mupite_wid_;

 public slots:

  void display_infoTab();

 signals:
  
  void multipickup_request(const AssemblyMultiPickupTester::Configuration&);

};
// ===========================================================================

#endif // ASSEMBLYTOOLBOXVIEW_H
