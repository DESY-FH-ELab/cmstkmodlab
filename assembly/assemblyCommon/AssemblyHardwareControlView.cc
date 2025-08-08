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

#include <AssemblyHardwareControlView.h>

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGroupBox>

AssemblyHardwareControlView::AssemblyHardwareControlView(const LStepExpressMotionManager* const manager, QWidget* parent)
 : QWidget(parent)
 , manager_(manager)

 , w_move_  (nullptr)
 , w_vacuum_(nullptr)

 , cb_lockMotionSettings_(nullptr)

 , w_lStep_        (nullptr)
{
  if(manager_ == nullptr)
  {
    NQLog("AssemblyHardwareControlView", NQLog::Critical) << "initialization error"
      << ": null pointer to LStepExpressMotionManager object, exiting constructor";

    return;
  }

  if(manager_->model() == nullptr)
  {
    NQLog("AssemblyHardwareControlView", NQLog::Critical) << "initialization error"
      << ": null pointer to LStepExpressModel object from LStepExpressMotionManager::model(), exiting constructor";

    return;
  }

  //// Main Layout -----------------------------
  QVBoxLayout* layout = new QVBoxLayout;
  this->setLayout(layout);

  //// Motion and Vacuum Widgets
  QHBoxLayout* g1 = new QHBoxLayout;

  /// Motion Widgets
  QGroupBox* box_move = new QGroupBox(tr("Motion Stage"));
  box_move->setStyleSheet("QGroupBox { font-weight: bold; } ");

  QVBoxLayout* l_move = new QVBoxLayout;
  box_move->setLayout(l_move);

  w_move_ = new AssemblyMoveWidget(manager_);
  w_move_->setToolTip("Single-Motion interface");
  w_move_->setEnabled(false);

  l_move->addWidget(w_move_);
  // -----

  this->stateChanged(manager_->model()->getDeviceState());

  connect(manager_->model(), SIGNAL(deviceStateChanged(State)), this, SLOT(stateChanged(State)));

  connect(manager_->model(), SIGNAL(motionStarted ()), this, SLOT(disableMotionTools()));
  connect(manager_, SIGNAL(motion_finished()), this, SLOT( enableMotionTools()));

  g1->addWidget(box_move, 60);
  /// -------------

  /// Vacuum Widget
  QGroupBox* box_vacuum = new QGroupBox(tr("Vacuum"));
  box_vacuum->setStyleSheet("QGroupBox { font-weight: bold; } ");

  w_vacuum_ = new AssemblyVacuumWidget("Toggle Vacuum");
  w_vacuum_->setToolTip("(3) Controls vacuum valves");

  box_vacuum->setLayout(w_vacuum_->layout());

  g1->addWidget(box_vacuum, 40);
  /// -------------

  layout->addLayout(g1);

  //// ------------------

  cb_lockMotionSettings_ = new QCheckBox("Lock Motion Stage Settings");
  layout->addWidget(cb_lockMotionSettings_);

  //// LStepExpressWidget
  w_lStep_ = new LStepExpressWidget(manager_->model());
  layout->addWidget(w_lStep_);

  connect(w_lStep_, SIGNAL(clearQueue_request()), manager_, SLOT(clear_motion_queue()));

  connect(w_lStep_, SIGNAL(moveToOrigin_request()), manager_, SLOT(moveAbsolute())); //New signal/slot to connect the Origin button with the manager (where XYA/Z priorities are implemented) rather than directly via the model
  //// ------------------

  connect(cb_lockMotionSettings_, SIGNAL(toggled(bool)), w_lStep_, SLOT(lockMotionSettings(bool)));
  cb_lockMotionSettings_->setChecked(true);

  // -------------------------------------------

  NQLog("AssemblyHardwareControlView", NQLog::Debug) << "constructed";
}

AssemblyHardwareControlView::~AssemblyHardwareControlView()
{
  if(w_lStep_){ delete w_lStep_; w_lStep_ = nullptr; }

  if(w_move_){ delete w_move_; w_move_ = nullptr; }
}

void AssemblyHardwareControlView::stateChanged(const State& newState)
{
  if(w_move_){ w_move_->setEnabled(newState == READY); }
}

void AssemblyHardwareControlView::enableMotionTools(const bool enable)
{
  if(w_move_){ w_move_->setEnabled(enable); }

  if(w_lStep_        ){ w_lStep_        ->enableMotionTools(enable); }
}

void AssemblyHardwareControlView::disableMotionTools(const bool disable)
{
  this->enableMotionTools(!disable);
}

//-- Information about this tab in GUI
//HTML markup (<xxx></xxx>): p paragraph, b bold, em emphasize, i italic, s small, section, summary, var variable, ...
//Ex: <p style="color:red">This is a red paragraph.</p>
void AssemblyHardwareControlView::display_infoTab()
{
    QMessageBox messageBox;
    messageBox.setWindowTitle(tr("Information - HW Controllers"));

    messageBox.setText(tr("<p>This tab allows for manual control of the Motion Stage (MS) and vacuum lines.</p>"

    "<p style=color:orange>When starting the program, always click 'Calibrate' first to make the MS aware of its physical boundaries. Click 'Origin' anytime to go back to the (0,0,0,0) absolute position.</p>"

    "<ul>"
    "<li>[<b>Motion Stage</b>]: apply any relative/absolute movement to the MS (within boundaries). <i>NB: always click the 'Clear XYZA' button afterwards to avoid mistakes !</i></li>"
    "<li>[<b>Toggle Vacuum</b>]: (dis)activate the vacuum lines for pickup (pickup tool), spacers (spacer slots on platform) or baseplate (baseplate slot on platform).</li>"
    "<li>[<b>Arrows (at bottom)</b>]: apply predefined relative movements along desired axis. This is to be prefered for small movements (limited to +-10mm).</li>"
    "</ul>"
    ));

    QSpacerItem* horizontalSpacer = new QSpacerItem(3000, 0, QSizePolicy::Minimum, QSizePolicy::Expanding); //Use this to enlarge box width
    QGridLayout* layout = (QGridLayout*) messageBox.layout();
    layout->addItem(horizontalSpacer, layout->rowCount(), 0, 1, layout->columnCount());
    messageBox.exec();

    return;
}
