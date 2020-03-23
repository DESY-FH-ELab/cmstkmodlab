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

#ifndef SINGLETONAPPLICATION_H
#define SINGLETONAPPLICATION_H

#include <QApplication>
#include <QSharedMemory>

/** @addtogroup common
 *  @{
 */

class SingletonApplication : public QApplication
{
    Q_OBJECT

public:
    SingletonApplication(int &argc, char **argv,
                         const char * uniqueKey = 0);
    ~SingletonApplication();

    bool lock();

private:
    QSharedMemory* singular_;
};

/** @} */

#endif // SINGLETONAPPLICATION_H
