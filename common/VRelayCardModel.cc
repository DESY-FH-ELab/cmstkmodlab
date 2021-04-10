/////////////////////////////////////////////////////////////////////////////////
//                                                                             //
//               Copyright (C) 2011-2021 - The DESY CMS Group                  //
//                           All rights reserved                               //
//                                                                             //
//      The CMStkModLab source code is licensed under the GNU GPL v3.0.        //
//      You have the right to modify and/or redistribute this source code      //
//      under the terms specified in the license, which may be found online    //
//      at http://www.gnu.org/licenses or at License.txt.                      //
//                                                                             //
/////////////////////////////////////////////////////////////////////////////////

#include <nqlogger.h>

#include "VRelayCardModel.h"

VRelayCardModel::VRelayCardModel(const std::string& port, QObject* /* parent */)
 : QObject()
 , port_(port.c_str())
{

}

/// Shuts down all the devices before closing down.
VRelayCardModel::~VRelayCardModel()
{

}
