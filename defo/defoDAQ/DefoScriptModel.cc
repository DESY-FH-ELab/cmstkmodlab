#include <QFile>
#include <QTextStream>
#include <QPlainTextDocumentLayout>
#include <QFont>
#include <QDateTime>

#include "DefoScriptModel.h"

DefoScriptModel::DefoScriptModel(
    DefoConradModel* conradModel
  , DefoCameraModel* cameraModel
  , DefoJulaboModel* julaboModel
  , KeithleyModel* keithleyModel
  , QObject *parent
) :
    QObject(parent)
  , conradModel_(conradModel)
  , cameraModel_(cameraModel)
  , julaboModel_(julaboModel)
  , keithleyModel_(keithleyModel)
{
  script_ = new QTextDocument(this);
  script_->setDocumentLayout(new QPlainTextDocumentLayout(script_));
  script_->setDefaultFont(QFont("Courier New",10));

  currentScriptFilename_ = QString();

  scriptThread_ = new DefoScriptThread(
      this
    , conradModel_
    , cameraModel_
    , julaboModel_
    , keithleyModel_
    , this
  );
  connect(scriptThread_, SIGNAL(started()), this, SLOT(executionStarted()));
  connect(scriptThread_, SIGNAL(finished()), this, SLOT(executionFinished()));

  connect(conradModel_, SIGNAL(defoMessage(const QString &)),
	  this, SLOT(doAppendMessageText(const QString &)));

  connect(cameraModel_, SIGNAL(defoMessage(const QString &)),
	  this, SLOT(doAppendMessageText(const QString &)));

  connect(julaboModel_, SIGNAL(defoMessage(const QString &)),
	  this, SLOT(doAppendMessageText(const QString &)));

  connect(keithleyModel_, SIGNAL(message(const QString &)),
	  this, SLOT(doAppendMessageText(const QString &)));

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

  emit setControlsEnabled(false);
  emit clearMessageText();

  executionTime_ = 0;
  executionTimer_.start(1000);
}

void DefoScriptModel::executionFinished() {

  executionTimer_.stop();  

  emit setControlsEnabled(true);
}

void DefoScriptModel::doPrepareNewMeasurement() {

  emit prepareNewMeasurement();
}

void DefoScriptModel::message(int value) {

  QString message = QString("%1")
    .arg(value);
  this->doAppendMessageText(message);
}

void DefoScriptModel::message(uint value) {

  QString message = QString("%1")
    .arg(value);
  this->doAppendMessageText(message);
}

void DefoScriptModel::message(double value) {

  QString message = QString("%1")
    .arg(value);
  this->doAppendMessageText(message);
}

void DefoScriptModel::message(const QString & text) {

  QString message = QString("%1")
    .arg(text);
  this->doAppendMessageText(message);
}

void DefoScriptModel::doClearMessageText() {
  
  emit clearMessageText();
}

void DefoScriptModel::doAppendMessageText(const QString & text) {

  if (!executionTimer_.isActive()) return;

  static QString TIME_FORMAT = "yyyy-MM-dd hh:mm:ss";
  QDateTime dt = QDateTime::currentDateTime();
  QString message = QString("%1 - %2")
    .arg(dt.toString(TIME_FORMAT))
    .arg(text);
  emit appendMessageText(message);
}

void DefoScriptModel::executionHeartBeat() {

  executionTime_++;
}
