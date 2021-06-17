/////////////////////////////////////////////////////////////////////////////////
//                                                                             //
//                                                                             //
//                                                                             //
//                   Written by Elise Hinkle (Brown CMS Group)                 //
//               Modeled on cmstkmodlab/python/PyTkModLabConrad.cc             //
//                          Last Updated August 27, 2019                       //
//                                                                             //
//                                                                             //
//                                                                             //
/////////////////////////////////////////////////////////////////////////////////

#include <boost/python.hpp>
#include <boost/python/suite/indexing/vector_indexing_suite.hpp>
#include <string>

#ifdef USEFAKEIO
#include "devices/Velleman/VellemanControllerFake.h"
typedef VellemanControllerFake VellemanController_t;
#else
#include "devices/Velleman/VellemanController.h"
typedef VellemanController VellemanController_t;
#endif

using namespace boost::python;

BOOST_PYTHON_MODULE(PyTkModLabVelleman)
{
 
  class_<std::vector<bool>>("Status")
    .def(vector_indexing_suite<std::vector<bool>>() );

  class_<VellemanController_t>("Velleman", init<const std::string&>())
    .def("queryChannelStatus", &VellemanController_t::queryRelayStatus)
    .def("setChannel", &VellemanController_t::setRelay)
    .def("setSingleChannel", &VellemanController_t::setSingleRelay)
    .def("setMultiChannels", &VellemanController_t::setMultiRelays)
    ;
};
