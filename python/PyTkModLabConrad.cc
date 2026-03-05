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

    class_<ConradController_t>("Conrad", init<const std::string&>())
      .def("queryStatus", &ConradController_t::queryStatus)
      .def("setChannel", &ConradController_t::setChannel)
      .def("setSingleChannel", &ConradController_t::setSingleChannel)
      ;
};
