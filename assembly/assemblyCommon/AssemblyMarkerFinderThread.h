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
