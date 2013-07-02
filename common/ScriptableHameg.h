#ifndef SCRIPTABLEHAMEG_H
#define SCRIPTABLEHAMEG_H

#include <QObject>
#include <QMutex>
#include <QScriptValue>

#include <HamegModel.h>

class ScriptableHameg : public QObject
{
  Q_OBJECT
public:
  explicit ScriptableHameg(HamegModel* hamegModel, QObject *parent = 0);

public slots:

//  QScriptValue state(unsigned int channel);
//  QScriptValue temperature(unsigned int channel);

protected:
  HamegModel* hamegModel_;

  // For thread safety
  QMutex mutex_;
};

#endif // SCRIPTABLEHAMEG_H
