#include "SingletonApplication.h"

SingletonApplication::SingletonApplication(int &argc, char **argv,
                                           const char *uniqueKey) :
    QApplication(argc, argv, true)
{
    singular_ = new QSharedMemory(this);
    singular_->setKey(uniqueKey);
}

SingletonApplication::~SingletonApplication()
{
    if (singular_->isAttached())
        singular_->detach();
}

bool SingletonApplication::lock()
{
    if (singular_->attach(QSharedMemory::ReadOnly)) {
        singular_->detach();
        return false;
    }

    if (singular_->create(1))
        return true;

    return false;
}
