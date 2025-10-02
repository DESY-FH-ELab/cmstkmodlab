/////////////////////////////////////////////////////////////////////////////////
//                                                                             //
//               Copyright (C) 2011-2025 - The DESY CMS Group                  //
//                           All rights reserved                               //
//                                                                             //
//      The CMStkModLab source code is licensed under the GNU GPL v3.0.        //
//      You have the right to modify and/or redistribute this source code      //
//      under the terms specified in the license, which may be found online    //
//      at http://www.gnu.org/licenses or at License.txt.                      //
//                                                                             //
/////////////////////////////////////////////////////////////////////////////////

#include <AssemblyVUEyeCamera.h>
#include <nqlogger.h>

#include <unistd.h> 

AssemblyVUEyeCamera::AssemblyVUEyeCamera(QObject* parent) :
  QObject(parent),
  cameraState_(State::OFF)
{
}

AssemblyVUEyeCamera::~AssemblyVUEyeCamera()
{
}

size_t AssemblyVUEyeCamera::getCurrentPixelClockIndex() const
{
    size_t idx = 0;
    for(std::vector<unsigned int>::const_iterator it=pixelClocks_.begin(); it != pixelClocks_.end(); ++it)
    {
      if(*it == currentPixelClock_){ return idx; }
      idx++;
    }

    return idx;
}
