#ifndef DEFODAQTHREAD_H
#define DEFODAQTHREAD_H

#include <QObject>
#include <QThread>

#include "DefoDAQModel.h"

class DefoDAQThread : public QThread
{
public:
  explicit DefoDAQThread(DefoDAQModel* daqModel,
			 QObject *parent = 0);
public:
  void run();

protected:
  DefoDAQModel* daqModel_;
};

#endif // DEFODAQTHREAD_H
