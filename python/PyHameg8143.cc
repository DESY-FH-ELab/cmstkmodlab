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
};
