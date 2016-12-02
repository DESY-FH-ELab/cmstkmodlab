#include <boost/python.hpp>
#include <string>

#ifdef USE_FAKEIO
#include "devices/Leybold/LeyboldGraphixThreeFake.h"
typedef LeyboldGraphixThreeFake LeyboldGraphixThree_t;
#else
#include "devices/Leybold/LeyboldGraphixThree.h"
typedef LeyboldGraphixThree LeyboldGraphixThree_t;
#endif

using namespace boost::python;

BOOST_PYTHON_MODULE(PyTkModLabLeybold)
{
    class_<LeyboldGraphixThree_t>("LeyboldGraphixThree", init<const ioport_t>())
      .def("DeviceAvailable", &LeyboldGraphixThree_t::DeviceAvailable)
      .def("GetVersion", &LeyboldGraphixThree_t::GetVersion)
      .def("GetSerialNumber", &LeyboldGraphixThree_t::GetSerialNumber)
      .def("GetItemNumber", &LeyboldGraphixThree_t::GetItemNumber)
      .def("GetNumberOfChannels", &LeyboldGraphixThree_t::GetNumberOfChannels)
      .def("GetSensorDetectionMode", &LeyboldGraphixThree_t::GetSensorDetectionMode)
      .def("SetSensorDetectionMode", &LeyboldGraphixThree_t::SetSensorDetectionMode)
      .def("GetSensorType", &LeyboldGraphixThree_t::GetSensorType)
      .def("SetSensorType", &LeyboldGraphixThree_t::SetSensorType)
      .def("GetSensorName", &LeyboldGraphixThree_t::GetSensorName)
      .def("SetSensorName", &LeyboldGraphixThree_t::SetSensorName)
      .def("GetSensorStatus", &LeyboldGraphixThree_t::GetSensorStatus)
      .def("GetSensorStatusText", &LeyboldGraphixThree_t::GetSensorStatusText)
      .def("GetPressure", &LeyboldGraphixThree_t::GetPressure)
      .def("GetDisplayUnit", &LeyboldGraphixThree_t::GetDisplayUnit)
      .def("SetDisplayUnit", &LeyboldGraphixThree_t::SetDisplayUnit)
      .def("GetDisplayUnitName", &LeyboldGraphixThree_t::GetDisplayUnitName)
      .def("GetSetPointChannelAssignment", &LeyboldGraphixThree_t::GetSetPointChannelAssignment)
      .def("SetSetPointChannelAssignment", &LeyboldGraphixThree_t::SetSetPointChannelAssignment)
      .def("GetSetPointOnPressure", &LeyboldGraphixThree_t::GetSetPointOnPressure)
      .def("SetSetPointOnPressure", &LeyboldGraphixThree_t::SetSetPointOnPressure)
      .def("GetSetPointOffPressure", &LeyboldGraphixThree_t::GetSetPointOffPressure)
      .def("SetSetPointOffPressure", &LeyboldGraphixThree_t::SetSetPointOffPressure)
      .def("GetSetPointStatus", &LeyboldGraphixThree_t::GetSetPointStatus)
    ;
};
