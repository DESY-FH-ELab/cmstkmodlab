#include <QFile>
#include <QTextStream>

#include "DefoScriptModel.h"

DefoScriptModel::DefoScriptModel(
    DefoCameraModel* cameraModel
  , QObject *parent
) :
    QObject(parent)
  , cameraModel_(cameraModel)
{
  script_ = new QTextDocument(this);
  currentScriptFilename_ = QString();

  scriptThread_ = new DefoScriptThread(
      this
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

  std::cout << "void DefoScriptModel::executeScript()" << std::endl;

  scriptThread_->executeScript(script_->toPlainText());

}

void DefoScriptModel::abortScript() {

  std::cout << "void DefoScriptModel::abortScript()" << std::endl;

  scriptThread_->abortScript();

}

void DefoScriptModel::executionStarted() {

  std::cout << "void DefoScriptModel::executionStarted()" << std::endl;
  executionTime_ = 0;
  executionTimer_.start(1000);
}

void DefoScriptModel::executionFinished() {

  std::cout << "void DefoScriptModel::executionFinished()" << std::endl;
  executionTimer_.stop();  
}

void DefoScriptModel::executionHeartBeat() {

  executionTime_++;

}
