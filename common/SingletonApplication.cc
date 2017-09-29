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

#include "SingletonApplication.h"

SingletonApplication::SingletonApplication(int &argc, char **argv,
                                           const char *uniqueKey) :
    QApplication(argc, argv, true)
{
    singular_ = new QSharedMemory(this);
    singular_->setKey(uniqueKey);
}

SingletonApplication::~SingletonApplication()
{
    if (singular_->isAttached())
        singular_->detach();
}

bool SingletonApplication::lock()
{
    if (singular_->attach(QSharedMemory::ReadOnly)) {
        singular_->detach();
        return false;
    }

    if (singular_->create(1))
        return true;

    return false;
}
