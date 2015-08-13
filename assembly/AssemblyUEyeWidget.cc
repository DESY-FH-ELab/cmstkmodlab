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

        /*
        const UEYE_CAMERA_INFO* info = model_->getCameraInfo(i);

        NQLog("AssemblyUEyeWidget") << "dwCameraID      " << info->dwCameraID;
        NQLog("AssemblyUEyeWidget") << "dwDeviceID      " << info->dwDeviceID;
        NQLog("AssemblyUEyeWidget") << "dwSensorID      " << info->dwSensorID;
        NQLog("AssemblyUEyeWidget") << "SerNo           " << info->SerNo;
        NQLog("AssemblyUEyeWidget") << "Model           " << info->Model;
        NQLog("AssemblyUEyeWidget") << "FullModelName   " << info->FullModelName;
        NQLog("AssemblyUEyeWidget") << "dwStatus        " << info->dwStatus;

        //HIDS cameraHandle = info->dwDeviceID;
        //is_InitCamera (&cameraHandle, 0);

        */
    }
}
