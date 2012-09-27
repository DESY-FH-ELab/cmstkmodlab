#ifndef DEFOSCRIPTABLEGLOBALS_H
#define DEFOSCRIPTABLEGLOBALS_H

#include <QObject>
#include <QMutex>

#include "DefoScriptModel.h"

class DefoScriptableGlobals :
      public QObject
{
  Q_OBJECT
public:
  explicit DefoScriptableGlobals(
      DefoScriptModel* scriptModel
    , QObject *parent = 0
  );
public slots:
  void wait(int seconds);
  uint eTime();
  uint uTime();
  uint mkUTime(int year, int month, int day, int hour, int minute, int second);

protected:
  DefoScriptModel* scriptModel_;

  // For thread safety
  QMutex mutex_;
};

#endif // DEFOSCRIPTABLEGLOBALS_H
