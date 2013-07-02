#ifndef SCRIPTABLEPFEIFFER_H
#define SCRIPTABLEPFEIFFER_H

#include <QObject>
#include <QMutex>
#include <QScriptValue>

#include <PfeifferModel.h>

class ScriptablePfeiffer : public QObject
{
  Q_OBJECT
public:
  explicit ScriptablePfeiffer(PfeifferModel* pfeifferModel, QObject *parent = 0);

public slots:

//  QScriptValue state(unsigned int channel);
//  QScriptValue temperature(unsigned int channel);

protected:
  PfeifferModel* pfeifferModel_;

  // For thread safety
  QMutex mutex_;
};

#endif // SCRIPTABLEPFEIFFER_H
