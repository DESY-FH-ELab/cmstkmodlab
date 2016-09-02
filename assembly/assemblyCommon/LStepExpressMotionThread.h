#ifndef LSTEPEXPRESSMOTIONTHREAD_H
#define LSTEPEXPRESSMOTIONTHREAD_H

#include <QThread>

class LStepExpressMotionThread : public QThread
{
    Q_OBJECT

public:

    explicit LStepExpressMotionThread(QObject *parent = 0);

    void run();
};

#endif // LSTEPEXPRESSMOTIONTHREAD_H
