#ifndef DEFOSCRIPTMODEL_H
#define DEFOSCRIPTMODEL_H

#include <QObject>
#include <QTimer>
#include <QTextDocument>

#include "DefoConradModel.h"
#include "DefoCameraModel.h"
#include "DefoJulaboModel.h"
#include "DefoKeithleyModel.h"
#include "DefoScriptThread.h"

class DefoScriptModel :
      public QObject
{
    Q_OBJECT
public:

  explicit DefoScriptModel(
      DefoConradModel* conradModel
    , DefoCameraModel* cameraModel
    , DefoJulaboModel* julaboModel
    , DefoKeithleyModel* keithleyModel
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
  void doPrepareNewMeasurement();
  void doClearMessageText();
  void doAppendMessageText(const QString & text);

protected slots:
  void executionHeartBeat();

protected:

  QTextDocument* script_;
  QString currentScriptFilename_;

  QTimer executionTimer_;
  int executionTime_;
  DefoConradModel* conradModel_;
  DefoCameraModel* cameraModel_;
  DefoJulaboModel* julaboModel_;
  DefoKeithleyModel* keithleyModel_;
  DefoScriptThread* scriptThread_;

signals:
  void prepareNewMeasurement();
  void scriptLoaded();
  void setControlsEnabled(bool enabled);
  void clearMessageText();
  void appendMessageText(const QString & text);
};

#endif // DEFOSCRIPTMODEL_H
