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

#include <nqlogger.h>

#include <AssemblyToolboxView.h>

#include <QVBoxLayout>
#include <QToolBox>

AssemblyToolboxView::AssemblyToolboxView(const LStepExpressMotionManager* const motion_manager, QWidget* parent) :
  QWidget(parent),

  posreg_wid_(nullptr),
  mupite_wid_(nullptr)
{
  if(motion_manager == nullptr)
  {
    NQLog("AssemblyToolboxView", NQLog::Critical) << "initialization error"
       << ": null pointer to LStepExpressMotionManager object, exiting constructor";

    return;
  }

  QVBoxLayout* layout = new QVBoxLayout;
  this->setLayout(layout);

  QToolBox* toolbox = new QToolBox;
  layout->addWidget(toolbox);

  //
  // Positions' Registry
  //  * widget to read current position of the motion stage
  //  * used to save reference positions
  //  * can calculate relative distances between two given (X,Y,X,A) positions
  //
  posreg_wid_ = new AssemblyPositionsRegistryWidget(motion_manager);

  toolbox->addItem(posreg_wid_, tr("Positions Registry"));
  // ---------------------

  //
  // Multi-Pickup Tester Widget
  //  * multiple iterations of "PatRec + pick-up + put-down" sequence
  //  * used to measure module-misplacement introduced by the pick-up
  //
  mupite_wid_ = new AssemblyMultiPickupTesterWidget(motion_manager);

  connect(mupite_wid_, SIGNAL(multipickup_request(const AssemblyMultiPickupTester::Configuration&)),
          this       , SIGNAL(multipickup_request(const AssemblyMultiPickupTester::Configuration&)));

  toolbox->addItem(mupite_wid_, tr("Multi-Pickup Tester (PatRec + pick-up + put-down)"));
  // ---------------------
  layout->addStretch(1);
}
