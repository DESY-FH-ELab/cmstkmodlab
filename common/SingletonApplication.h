#ifndef SINGLETONAPPLICATION_H
#define SINGLETONAPPLICATION_H

#include <QApplication>
#include <QSharedMemory>

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

#endif // SINGLETONAPPLICATION_H
