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

//-- Information about this tab in GUI
//HTML markup (<xxx></xxx>): p paragraph, b bold, em emphasize, i italic, s small, section, summary, var variable, ...
//Ex: <p style="color:red">This is a red paragraph.</p>
void AssemblyToolboxView::display_infoTab()
{
    QMessageBox messageBox;
    messageBox.setWindowTitle(tr("Information - Toolbox"));

    messageBox.setText(tr("<p>This tab offers helper tools and functionalities.</p>"

    "<ul>"
    "<li> <u>Record positions</u>: Save the current absolute position of the Motion Stage, and comment it if desired. It is possible to determine the relative distance between 2 registered positions.</li>"
    "<br>"
    "<li> <u>Multi-Pickup Tester</u>: advanced functionality. Used to calibrate the relative movement required along the z-axis to pickup an object, when the camera is focused on that object. The basic procedure is: a) PatRec on sensor ref. marker; b) lower pickup tool; c) pickup object; d) lift up; e) put back down; f) rerun PatRec; g) repeat. At each repetition, can monitor in the PatRec tab whether the object was displaced in XY by the pickup, which indicates that excessive/insufficient pressure was applied to the object. Repeat this test until you are satisfied with the z-axis movement, and store its value in the cfg file.</li>"
    "<p style=color:orange><b>WARNING: you must activate the baseplate vacuum before starting !</p></b>"
    "</ul></p>"
    ));

    QSpacerItem* horizontalSpacer = new QSpacerItem(3000, 0, QSizePolicy::Minimum, QSizePolicy::Expanding); //Use this to enlarge box width
    QGridLayout* layout = (QGridLayout*) messageBox.layout();
    layout->addItem(horizontalSpacer, layout->rowCount(), 0, 1, layout->columnCount());
    messageBox.exec();

    return;
}
