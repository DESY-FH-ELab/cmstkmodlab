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

#ifndef ASSEMBLYMARKERFINDERTHREAD_H
#define ASSEMBLYMARKERFINDERTHREAD_H

#include <QObject>
#include <QThread>

#include "AssemblyVMarkerFinder.h"

class AssemblyMarkerFinderThread : public QThread
{
public:
    explicit AssemblyMarkerFinderThread(AssemblyVMarkerFinder* finder,
                                        QObject *parent = 0);

    ~AssemblyMarkerFinderThread();

    void run();

protected:

    AssemblyVMarkerFinder* finder_;
};

#endif // ASSEMBLYMARKERFINDERTHREAD_H
