//#include <uEye.h>

#include <nqlogger.h>

#include "AssemblyUEyeWidget.h"

AssemblyUEyeWidget::AssemblyUEyeWidget(AssemblyUEyeModel_t* model,
                                       QWidget *parent)
: QWidget(parent),
  model_(model)
{
  // Connect all the signals
  connect(model_, SIGNAL(cameraCountChanged(unsigned int)),
          this, SLOT(updateCameraInformation(unsigned int)));

  updateCameraInformation(0);
}

void AssemblyUEyeWidget::updateCameraInformation(unsigned int cameraCount)
{
    NQLog("AssemblyUEyeWidget") << "update information for " << cameraCount << " camera(s)";

    for (unsigned int i=0;i<cameraCount;++i) {

        //HIDS cameraHandle = info->dwDeviceID;
        //is_InitCamera (&cameraHandle, 0);

    }
}
