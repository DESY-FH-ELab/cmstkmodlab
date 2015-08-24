#ifndef ASSEMBLYUEYEFAKEPIXELCLOCK_H
#define ASSEMBLYUEYEFAKEPIXELCLOCK_H

#include "AssemblyVUEyePixelClock.h"

class AssemblyUEyeFakePixelClock : public AssemblyVUEyePixelClock
{
    Q_OBJECT
public:
    explicit AssemblyUEyeFakePixelClock(QObject *parent);
    ~AssemblyUEyeFakePixelClock();

public slots:

    virtual void updatePixelClocks(unsigned int, bool);

protected slots:

protected:

signals:

private:
};

#endif // ASSEMBLYUEYEFAKEPIXELCLOCK_H
