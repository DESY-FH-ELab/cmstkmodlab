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
#include <ApplicationConfig.h>

#include <AssemblyAssemblyView.h>
#include <AssemblyUtilities.h>

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QToolBox>
#include <QLabel>

AssemblyAssemblyView::AssemblyAssemblyView(QWidget* parent)
 :
   QWidget(parent)

 , CalibRotStage_wid_  (nullptr)
 , PSPToBasep_wid_     (nullptr)
 , PSSToSpacers_wid_   (nullptr)
 , PSSSpacToPSPBas_wid_(nullptr)
{
  QVBoxLayout* layout = new QVBoxLayout;
  this->setLayout(layout);

  QToolBox* toolbox = new QToolBox;
  layout->addWidget(toolbox);

  // ---------------------

  // Calibration of Rotation Stage
  CalibRotStage_wid_ = new QWidget;

  toolbox->addItem(CalibRotStage_wid_, tr("[0] Calibration of Rotation Stage"));

  QVBoxLayout* CalibRotStage_lay = new QVBoxLayout;
  CalibRotStage_wid_->setLayout(CalibRotStage_lay);

  CalibRotStage_lay->addWidget(new QLabel);
  // ---------------------

  // PSP To Baseplate ----
  PSPToBasep_wid_ = new QWidget;

  toolbox->addItem(PSPToBasep_wid_, tr("[1] PSP + Baseplate"));

  QVBoxLayout* PSPToBasep_lay = new QVBoxLayout;
  PSPToBasep_wid_->setLayout(PSPToBasep_lay);

  PSPToBasep_lay->addWidget(new QLabel);
  // ---------------------

  // PSS To Spacers ------
  PSSToSpacers_wid_ = new QWidget;

  toolbox->addItem(PSSToSpacers_wid_, tr("[2] PSS + Spacers"));

  QVBoxLayout* PSSToSpacers_lay = new QVBoxLayout;
  PSSToSpacers_wid_->setLayout(PSSToSpacers_lay);

  PSSToSpacers_lay->addWidget(new QLabel);
  // ---------------------

  // PSS+Spacers To PSP+Baseplate
  PSSSpacToPSPBas_wid_ = new QWidget;

  toolbox->addItem(PSSSpacToPSPBas_wid_, tr("[4] Assembly: PSS+Spacers To PSP+Baseplate"));

  QVBoxLayout* PSSSpacToPSPBas_lay = new QVBoxLayout;
  PSSSpacToPSPBas_wid_->setLayout(PSSSpacToPSPBas_lay);

  PSSSpacToPSPBas_lay->addWidget(new QLabel);
  // ---------------------

  // go to   ref-point on rotation stage

  // measure ref-point on rotation stage

  // -----------------------------------------------
  // -----------------------------------------------

  // go to measurement position on PSP

  // align PSP to motion-stage X-axis

  // pick up PSP

  // put  baseplate on platform

  // lock baseplate on platform (vacuum)

  // put down PSP on to baseplate

  // wait for glue to cure

  // release PSP from pickup tool

  // release baseplate from platform

  // -----------------------------------------------
  // -----------------------------------------------

  // go to measurement position on PSS

  // align PSS to motion-stage X-axis

  // pick up PSS

  // put  spacers on platform

  // lock spacers on platform (vacuum)

  // put down PSS on to spacers (including shift)

  // wait for glue to cure

  // pick up PSS+spacers

  // put  PSP+baseplate on platform

  // lock PSP+baseplate on platform (vacuum)

  // go to measurement position on PSP+baseplate

  // align PSP+baseplate to motion-stage X-axis

  // move PSS+spacers on to gluing platform

  // apply additional movements to dispense glue on the spacers

  // move PSS+spacers on top of PSP+baseplate in XY (arbitrary height)

  // put down PSS onto PSP+baseplate (including shift)

  // wait for glue to cure

  // -----------------------------------------------
  // -----------------------------------------------

}
