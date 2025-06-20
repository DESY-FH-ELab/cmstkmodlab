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

#include <DatabaseDummy.h>

DatabaseDummy::DatabaseDummy(QObject *parent) : VDatabase(parent)
{
    // Initialise stuff!
}

DatabaseDummy::~DatabaseDummy()
{
    // Any connections to close?
}


bool DatabaseDummy::register_module_name(QString module_name, QString operator_name)
{
    return false;
}

bool DatabaseDummy::MaPSA_to_BP(QString, QString, QString)
{
    return false;
}

bool DatabaseDummy::PSs_to_spacers(QString, QString)
{
    return false;
}

bool DatabaseDummy::PSs_to_MaPSA(QString)
{
    return false;
}
