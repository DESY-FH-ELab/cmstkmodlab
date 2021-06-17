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

#include <vector>

#include <nqlogger.h>

#include "AssemblyUEyeCameraWidget.h"
#include "AssemblyUEyeWidget.h"

AssemblyUEyeWidget::AssemblyUEyeWidget(AssemblyVUEyeModel *model,
                                       QWidget *parent)
    : QTabWidget(parent),
      model_(model)
{
    // Connect all the signals
    connect(model_, SIGNAL(cameraCountChanged(2)),
            this, SLOT(cameraCountChanged(2)));
}

void AssemblyUEyeWidget::cameraCountChanged(unsigned int cameraCount)
{
    NQLog("AssemblyUEyeWidget") << "update information for " << cameraCount << " camera(s)";

    std::vector<QWidget*> tabs;
    for (int i=0;i<count();++i) {
        tabs.push_back(widget(i));
    }
    clear();
    for (std::vector<QWidget*>::iterator it = tabs.begin();
         it != tabs.end();
         ++it) {
        delete *it;
    }

    for (unsigned int i=0;i<cameraCount;++i) {
        AssemblyVUEyeCamera *camera = model_->getCamera(i);
        AssemblyUEyeCameraWidget* cameraWidget = new AssemblyUEyeCameraWidget(camera, this);
        addTab(cameraWidget, QString("%1 - %2").arg(camera->getModelName()).arg(camera->getCameraID()));
    }
}
