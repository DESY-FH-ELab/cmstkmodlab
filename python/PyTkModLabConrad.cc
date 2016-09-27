#include <boost/python.hpp>
#include <boost/python/suite/indexing/vector_indexing_suite.hpp>
#include <string>

#ifdef USE_FAKEIO
#include "devices/Conrad/ConradControllerFake.h"
typedef ConradControllerFake ConradController_t;
#else
#include "devices/Conrad/ConradController.h"
typedef ConradController ConradController_t;
#endif

using namespace boost::python;

BOOST_PYTHON_MODULE(PyTkModLabConrad)
{
    class_<std::vector<bool>>("Status")
          .def(vector_indexing_suite<std::vector<bool>>() );

    class_<ConradController_t>("Conrad", init<const ioport_t>())
      .def("queryStatus", &ConradController_t::queryStatus)
      .def("setChannel", &ConradController_t::setChannel)
      .def("setSingleChannel", &ConradController_t::setSingleChannel)
      ;
};
