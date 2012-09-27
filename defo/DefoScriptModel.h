#ifndef DEFOSCRIPTMODEL_H
#define DEFOSCRIPTMODEL_H

#include <QObject>
#include <QTimer>
#include <QTextDocument>

#include "DefoConradModel.h"
#include "DefoCameraModel.h"
#include "DefoScriptThread.h"

class DefoScriptModel :
      public QObject
{
    Q_OBJECT
public:

  explicit DefoScriptModel(
      DefoConradModel* conradModel
    , DefoCameraModel* cameraModel
    , QObject *parent = 0
  );

  QTextDocument* scriptDocument() { return script_; }
  const QString& currentScriptFilename() const { return currentScriptFilename_; }

  void openScript(const QString filename);
  void saveScript(const QString filename);
  void executeScript();
  void abortScript();
  int executionTime() const { return executionTime_; }

public slots:
  void executionStarted();
  void executionFinished();

protected slots:
  void executionHeartBeat();

protected:

  QTextDocument* script_;
  QString currentScriptFilename_;

  QTimer executionTimer_;
  int executionTime_;
  DefoConradModel* conradModel_;
  DefoCameraModel* cameraModel_;
  DefoScriptThread* scriptThread_;

signals:
  void scriptLoaded();
};

#endif // DEFOSCRIPTMODEL_H
