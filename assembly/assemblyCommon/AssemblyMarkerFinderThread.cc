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

#include <QMutex>

#include <nqlogger.h>

#include "AssemblyMarkerFinderThread.h"

AssemblyMarkerFinderThread::AssemblyMarkerFinderThread(AssemblyVMarkerFinder* finder,
                                                   QObject *parent)
    : QThread(parent),
      finder_(finder)
{
    finder_->moveToThread(this);

    NQLog("AssemblyMarkerFinderThread", NQLog::Spam) << "construct";
}

AssemblyMarkerFinderThread::~AssemblyMarkerFinderThread()
{
    NQLog("AssemblyMarkerFinderThread", NQLog::Spam) << "delete";
}

void AssemblyMarkerFinderThread::run()
{
    NQLog("AssemblyMarkerFinderThread", NQLog::Spam) << "started";
    exec();
}
