#ifndef LASERTHREAD_H
#define LASERTHREAD_H

#include <QThread>

class LaserThread : public QThread
{
    Q_OBJECT

 public:

    explicit LaserThread(QObject *parent = 0);

    void run();
};

#endif // LASERTHREAD_H                          
