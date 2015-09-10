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
