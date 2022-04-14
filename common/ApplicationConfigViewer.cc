/////////////////////////////////////////////////////////////////////////////////
//                                                                             //
//               Copyright (C) 2011-2022 - The DESY CMS Group                  //
//                           All rights reserved                               //
//                                                                             //
//      The CMStkModLab source code is licensed under the GNU GPL v3.0.        //
//      You have the right to modify and/or redistribute this source code      //
//      under the terms specified in the license, which may be found online    //
//      at http://www.gnu.org/licenses or at License.txt.                      //
//                                                                             //
/////////////////////////////////////////////////////////////////////////////////

#include <QHeaderView>

#include <nqlogger.h>
#include <ApplicationConfig.h>

#include "ApplicationConfigViewer.h"

ApplicationConfigViewer::ApplicationConfigViewer(bool readOnly, QWidget *parent)
  : QTableView(parent)
{
  NQLog("ApplicationConfigViewer", NQLog::Debug) << "ApplicationConfigViewer(QWidget *parent)";

  model_ = new ApplicationConfigItemModel(readOnly, this);

  setModel(model_);

  horizontalHeader()->setStretchLastSection(true);

  connect(model_, SIGNAL(keysChanged()),
      this, SLOT(resizeColumns()));
}

void ApplicationConfigViewer::setReadOnly(bool readOnly)
{
  model_->setReadOnly(readOnly);
}

void ApplicationConfigViewer::resizeColumns()
{
  horizontalHeader()->resizeSections(QHeaderView::ResizeToContents);
}
