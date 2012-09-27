#include <QFile>
#include <QTextStream>
#include <QPlainTextDocumentLayout>
#include <QFont>

#include "DefoScriptModel.h"

DefoScriptModel::DefoScriptModel(
    DefoConradModel* conradModel
  , DefoCameraModel* cameraModel
  , QObject *parent
) :
    QObject(parent)
  , conradModel_(conradModel)
  , cameraModel_(cameraModel)
{
  script_ = new QTextDocument(this);
  script_->setDocumentLayout(new QPlainTextDocumentLayout(script_));
  script_->setDefaultFont(QFont("Courier New",10));

  currentScriptFilename_ = QString();

  scriptThread_ = new DefoScriptThread(
      this
    , conradModel_
    , cameraModel_
    , this
  );
  connect(scriptThread_, SIGNAL(started()), this, SLOT(executionStarted()));
  connect(scriptThread_, SIGNAL(finished()), this, SLOT(executionFinished()));

  connect(&executionTimer_, SIGNAL(timeout()), this, SLOT(executionHeartBeat()));
}

void DefoScriptModel::openScript(const QString filename) {

  QFile file(filename);
  if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    return;

  script_->setPlainText(file.readAll());

  currentScriptFilename_ = filename;
}

void DefoScriptModel::saveScript(const QString filename) {

  QFile file(filename);
  if (!file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate))
    return;
  
  QTextStream out(&file);
  out << script_->toPlainText();

  currentScriptFilename_ = filename;  
}

void DefoScriptModel::executeScript() {

  scriptThread_->executeScript(script_->toPlainText());
}

void DefoScriptModel::abortScript() {

  scriptThread_->abortScript();
}

void DefoScriptModel::executionStarted() {

  executionTime_ = 0;
  executionTimer_.start(1000);
}

void DefoScriptModel::executionFinished() {

  executionTimer_.stop();  
}

void DefoScriptModel::executionHeartBeat() {

  executionTime_++;
}
