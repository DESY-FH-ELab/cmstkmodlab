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

#include <AssemblyParametersView.h>

#include <QVBoxLayout>
#include <QToolBox>

AssemblyParametersView::AssemblyParametersView(QWidget* parent) :
  QWidget(parent)
{
  QVBoxLayout* layout = new QVBoxLayout;
  this->setLayout(layout);

  QToolBox* toolbox = new QToolBox;
  layout->addWidget(toolbox);

  // ---------------------

  toolbox->addItem(new QWidget, tr("AAA"));
  toolbox->addItem(new QWidget, tr("BBB"));
  toolbox->addItem(new QWidget, tr("CCC"));

  // ---------------------

  layout->addStretch(1);
}
