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

#include <boost/python.hpp>
#include <string>

#ifdef USE_FAKEIO
#include "devices/Hameg/Hameg8143Fake.h"
typedef Hameg8143Fake Hameg8143_t;
#else
#include "devices/Hameg/Hameg8143.h"
typedef Hameg8143 Hameg8143_t;
#endif

using namespace boost::python;

BOOST_PYTHON_MODULE(PyTkModLabHameg)
{
    class_<Hameg8143_t>("Hameg8143", init<const ioport_t>())
      .def("DeviceAvailable", &Hameg8143_t::DeviceAvailable)
      .def("GetStatus", &Hameg8143_t::GetStatus)
      .def("SetRemoteMode", &Hameg8143_t::SetRemoteMode)
      .def("SetMixedMode", &Hameg8143_t::SetMixedMode)

      .def("SetVoltage", &Hameg8143_t::SetVoltage)
      .def("SetTrackingVoltage", &Hameg8143_t::SetTrackingVoltage)
      .def("GetSetVoltage", &Hameg8143_t::GetSetVoltage)
      .def("GetVoltage", &Hameg8143_t::GetVoltage)

      .def("SetCurrent", &Hameg8143_t::SetCurrent)
      .def("SetTrackingCurrent", &Hameg8143_t::SetTrackingCurrent)
      .def("GetSetCurrent", &Hameg8143_t::GetSetCurrent)
      .def("GetCurrent", &Hameg8143_t::GetCurrent)

      .def("SwitchOutputOn", &Hameg8143_t::SwitchOutputOn)
      .def("SwitchOutputOff", &Hameg8143_t::SwitchOutputOff)
      ;
};
