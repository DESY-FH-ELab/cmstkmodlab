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

#include <boost/python.hpp>
#include <string>

#ifdef USE_FAKEIO
#include "devices/Nanotec/NanotecSMCI36Fake.h"
typedef NanotecSMCI36Fake NanotecSMCI36_t;
#else
#include "devices/Nanotec/NanotecSMCI36.h"
typedef NanotecSMCI36 NanotecSMCI36_t;
#endif

using namespace boost::python;

BOOST_PYTHON_MODULE(PyTkModLabNanotec)
{
    class_<NanotecSMCI36_t>("NanotecSMCI36", init<const ioport_t>())
      .def("DeviceAvailable", &NanotecSMCI36_t::DeviceAvailable)
      .def("GetFirmwareVersion", &NanotecSMCI36_t::GetFirmwareVersion)
      .def("GetStatus", &NanotecSMCI36_t::GetStatus)
      .def("SetMotorType", &NanotecSMCI36_t::SetMotorType)
      .def("GetMotorType", &NanotecSMCI36_t::GetMotorType)
      .def("SetDriveAddress", &NanotecSMCI36_t::SetDriveAddress)
      .def("GetDriveAddress", &NanotecSMCI36_t::GetDriveAddress)
      .def("SetMotorID", &NanotecSMCI36_t::SetMotorID)
      .def("GetMotorID", &NanotecSMCI36_t::GetMotorID)
      .def("SetPhaseCurrent", &NanotecSMCI36_t::SetPhaseCurrent)
      .def("GetPhaseCurrent", &NanotecSMCI36_t::GetPhaseCurrent)
      .def("SetStandStillPhaseCurrent", &NanotecSMCI36_t::SetStandStillPhaseCurrent)
      .def("GetStandStillPhaseCurrent", &NanotecSMCI36_t::GetStandStillPhaseCurrent)
      .def("SetStepMode", &NanotecSMCI36_t::SetStepMode)
      .def("GetStepMode", &NanotecSMCI36_t::GetStepMode)
      .def("SetRampMode", &NanotecSMCI36_t::SetRampMode)
      .def("GetRampMode", &NanotecSMCI36_t::GetRampMode)
      .def("SetErrorCorrectionMode", &NanotecSMCI36_t::SetErrorCorrectionMode)
      .def("GetErrorCorrectionMode", &NanotecSMCI36_t::GetErrorCorrectionMode)
      .def("SetMaxEncoderDeviation", &NanotecSMCI36_t::SetMaxEncoderDeviation)
      .def("GetMaxEncoderDeviation", &NanotecSMCI36_t::GetMaxEncoderDeviation)
      .def("ResetPositionError", &NanotecSMCI36_t::ResetPositionError)
      .def("SetPositioningMode", &NanotecSMCI36_t::SetPositioningMode)
      .def("GetPositioningMode", &NanotecSMCI36_t::GetPositioningMode)
      .def("GetPosition", &NanotecSMCI36_t::GetPosition)
      .def("GetEncoderPosition", &NanotecSMCI36_t::GetEncoderPosition)
      .def("SetTravelDistance", &NanotecSMCI36_t::SetTravelDistance)
      .def("GetTravelDistance", &NanotecSMCI36_t::GetTravelDistance)
      .def("SetDirection", &NanotecSMCI36_t::SetDirection)
      .def("GetDirection", &NanotecSMCI36_t::GetDirection)
      .def("SetEncoderDirection", &NanotecSMCI36_t::SetEncoderDirection)
      .def("GetEncoderDirection", &NanotecSMCI36_t::GetEncoderDirection)
      .def("SetMinimumFrequency", &NanotecSMCI36_t::SetMinimumFrequency)
      .def("GetMinimumFrequency", &NanotecSMCI36_t::GetMinimumFrequency)
      .def("SetMaximumFrequency", &NanotecSMCI36_t::SetMaximumFrequency)
      .def("GetMaximumFrequency", &NanotecSMCI36_t::GetMaximumFrequency)
      .def("SetMaximumFrequency2", &NanotecSMCI36_t::SetMaximumFrequency2)
      .def("GetMaximumFrequency2", &NanotecSMCI36_t::GetMaximumFrequency2)
      .def("SetQuickstopRampHzPerSecond", &NanotecSMCI36_t::SetQuickstopRampHzPerSecond)
      .def("GetQuickstopRampHzPerSecond", &NanotecSMCI36_t::GetQuickstopRampHzPerSecond)
      .def("SetAccelerationRampHzPerSecond", &NanotecSMCI36_t::SetAccelerationRampHzPerSecond)
      .def("GetAccelerationRampHzPerSecond", &NanotecSMCI36_t::GetAccelerationRampHzPerSecond)
      .def("SetDecelerationRampHzPerSecond", &NanotecSMCI36_t::SetDecelerationRampHzPerSecond)
      .def("GetDecelerationRampHzPerSecond", &NanotecSMCI36_t::GetDecelerationRampHzPerSecond)
      .def("Start", &NanotecSMCI36_t::Start)
      .def("Stop", &NanotecSMCI36_t::Stop)
      .def("SetInputPinFunction", &NanotecSMCI36_t::SetInputPinFunction)
      .def("GetInputPinFunction", &NanotecSMCI36_t::GetInputPinFunction)
      .def("SetOutputPinFunction", &NanotecSMCI36_t::SetOutputPinFunction)
      .def("GetOutputPinFunction", &NanotecSMCI36_t::GetOutputPinFunction)
      .def("SetIOMask", &NanotecSMCI36_t::SetIOMask)
      .def("GetIOMask", &NanotecSMCI36_t::GetIOMask)
      .def("SetReversePolarityMask", &NanotecSMCI36_t::SetReversePolarityMask)
      .def("GetReversePolarityMask", &NanotecSMCI36_t::GetReversePolarityMask)
      .def("SetIO", &NanotecSMCI36_t::SetIO)
      .def("GetIO", &NanotecSMCI36_t::GetIO)
    ;
};
