#ifndef SCRIPTABLEHUBERPETITEFLEUR_H
#define SCRIPTABLEHUBERPETITEFLEUR_H

#include <QObject>
#include <QMutex>
#include <QScriptValue>

#include <HuberPetiteFleurModel.h>

class ScriptableHuberPetiteFleur : public QObject
{
  Q_OBJECT
public:
  explicit ScriptableHuberPetiteFleur(HuberPetiteFleurModel* petiteFleurModel,
                                      QObject *parent = 0);

public slots:

//  QScriptValue state(unsigned int channel);
//  QScriptValue temperature(unsigned int channel);

protected:
  HuberPetiteFleurModel* huberPetiteFleurModel_;

  // For thread safety
  QMutex mutex_;
};

#endif // SCRIPTABLEHUBERPETITEFLEUR_H
