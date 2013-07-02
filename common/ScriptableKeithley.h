#ifndef SCRIPTABLEKEITHLEY_H
#define SCRIPTABLEKEITHLEY_H

#include <QObject>
#include <QMutex>
#include <QScriptValue>

#include <KeithleyModel.h>

class ScriptableKeithley : public QObject
{
  Q_OBJECT
public:
  explicit ScriptableKeithley(KeithleyModel* keithleyModel, QObject *parent = 0);

public slots:

  QScriptValue state(unsigned int channel);
  QScriptValue temperature(unsigned int channel);

protected:
  KeithleyModel* keithleyModel_;

  // For thread safety
  QMutex mutex_;
};

#endif // SCRIPTABLEKEITHLEY_H
