#ifndef ASSEMBLYUEYEPIXELCLOCK_H
#define ASSEMBLYUEYEPIXELCLOCK_H

#include <uEye.h>

#include "AssemblyVUEyePixelClock.h"

class AssemblyUEyePixelClock : public AssemblyVUEyePixelClock
{
    Q_OBJECT
public:
    explicit AssemblyUEyePixelClock(QObject *parent);
    ~AssemblyUEyePixelClock();

public slots:

    virtual void updatePixelClocks(unsigned int, bool);

protected slots:

protected:

    unsigned int readPixelClock(unsigned int);

signals:

private:

};

#endif // ASSEMBLYUEYEPIXELCLOCK_H
