#include <boost/python.hpp>
#include <string>

#ifdef USE_FAKEIO
#include "devices/Hameg/Hameg8143Fake.h"
typedef Hameg8143Fake Hameg8143_t;
#else
#include "devices/Hameg/Hameg8143.h"
typedef Hameg8143 Hameg8143_t;
#endif

#ifdef USE_FAKEIO
#include "devices/Nanotec/NanotecSMCI36Fake.h"
typedef NanotecSMCI36Fake NanotecSMCI36_t;
#else
#include "devices/Nanotec/NanotecSMCI36.h"
typedef NanotecSMCI36 NanotecSMCI36_t;
#endif

using namespace boost::python;

BOOST_PYTHON_MODULE(PyTkModLab)
{
    class_<Hameg8143_t>("Hameg8143", init<const ioport_t>())
      .def("DeviceAvailable", &Hameg8143_t::DeviceAvailable)
      .def("GetStatus", &Hameg8143_t::GetStatus)
      .def("SetRemoteMode", &Hameg8143_t::SetRemoteMode)
      .def("SetVoltage", &Hameg8143_t::SetVoltage)
      .def("GetSetVoltage", &Hameg8143_t::GetSetVoltage)
      .def("GetVoltage", &Hameg8143_t::GetVoltage)
    ;

    class_<NanotecSMCI36_t>("NanotecSMCI36", init<const ioport_t>())
      .def("DeviceAvailable", &NanotecSMCI36_t::DeviceAvailable)
      .def("GetStatus", &NanotecSMCI36_t::GetStatus)
      .def("SetMotorType", &NanotecSMCI36_t::SetMotorType)
      .def("GetMotorType", &NanotecSMCI36_t::GetMotorType)
      .def("SetPhaseCurrent", &NanotecSMCI36_t::SetPhaseCurrent)
      .def("GetPhaseCurrent", &NanotecSMCI36_t::GetPhaseCurrent)
      .def("SetStandStillPhaseCurrent", &NanotecSMCI36_t::SetStandStillPhaseCurrent)
      .def("GetStandStillPhaseCurrent", &NanotecSMCI36_t::GetStandStillPhaseCurrent)
      .def("SetStepMode", &NanotecSMCI36_t::SetStepMode)
      .def("GetStepMode", &NanotecSMCI36_t::GetStepMode)
      .def("SetRampMode", &NanotecSMCI36_t::SetRampMode)
      .def("GetRampMode", &NanotecSMCI36_t::GetRampMode)
      .def("SetPositioningMode", &NanotecSMCI36_t::SetPositioningMode)
      .def("GetPositioningMode", &NanotecSMCI36_t::GetPositioningMode)
      .def("GetPosition", &NanotecSMCI36_t::GetPosition)
      .def("GetEncoderPosition", &NanotecSMCI36_t::GetEncoderPosition)
      .def("SetTravelDistance", &NanotecSMCI36_t::SetTravelDistance)
      .def("GetTravelDistance", &NanotecSMCI36_t::GetTravelDistance)
      .def("SetDirection", &NanotecSMCI36_t::SetDirection)
      .def("GetDirection", &NanotecSMCI36_t::GetDirection)
      .def("SetMinimumFrequency", &NanotecSMCI36_t::SetMinimumFrequency)
      .def("GetMinimumFrequency", &NanotecSMCI36_t::GetMinimumFrequency)
      .def("SetMaximumFrequency", &NanotecSMCI36_t::SetMaximumFrequency)
      .def("GetMaximumFrequency", &NanotecSMCI36_t::GetMaximumFrequency)
      .def("SetMaximumFrequency2", &NanotecSMCI36_t::SetMaximumFrequency2)
      .def("GetMaximumFrequency2", &NanotecSMCI36_t::GetMaximumFrequency2)
      .def("Start", &NanotecSMCI36_t::Start)
      .def("Stop", &NanotecSMCI36_t::Stop)
      .def("SetInputPinFunction", &NanotecSMCI36_t::SetInputPinFunction)
      .def("GetInputPinFunction", &NanotecSMCI36_t::GetInputPinFunction)
      .def("SetOutputPinFunction", &NanotecSMCI36_t::SetOutputPinFunction)
      .def("GetOutputPinFunction", &NanotecSMCI36_t::GetOutputPinFunction)
      .def("SetIOMask", &NanotecSMCI36_t::SetIOMask)
      .def("GetIOMask", &NanotecSMCI36_t::GetIOMask)
      .def("SetIO", &NanotecSMCI36_t::SetIO)
      .def("GetIO", &NanotecSMCI36_t::GetIO)
    ;
};
