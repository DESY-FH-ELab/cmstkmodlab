#include <QFile>
#include <QTextStream>
#include <QPlainTextDocumentLayout>
#include <QFont>
#include <QDateTime>
#include <QXmlStreamWriter>

#include "MicroScriptModel.h"

MicroScriptModel::MicroScriptModel(MicroDAQModel* daqModel,
                                   IotaModel* iotaModel,
                                   ArduinoPresModel* arduinoPresModel,
                                   QObject *parent) :
    QObject(parent),
    daqModel_(daqModel),
    iotaModel_(iotaModel),
    arduinoPresModel_(arduinoPresModel)
{
    script_ = new QTextDocument(this);
    script_->setDocumentLayout(new QPlainTextDocumentLayout(script_));
    script_->setDefaultFont(QFont("Courier New",10));

    currentScriptFilename_ = QString();

    scriptThread_ = new MicroScriptThread(this,
                                          iotaModel_,
                                          arduinoPresModel_,
                                          this);
    connect(scriptThread_, SIGNAL(started()), this, SLOT(executionStarted()));
    connect(scriptThread_, SIGNAL(finished()), this, SLOT(executionFinished()));

    connect(iotaModel_, SIGNAL(message(const QString &)),
            this, SLOT(doAppendMessageText(const QString &)));

    connect(arduinoPresModel_, SIGNAL(message(const QString &)),
            this, SLOT(doAppendMessageText(const QString &)));

    connect(&executionTimer_, SIGNAL(timeout()), this, SLOT(executionHeartBeat()));
}

void MicroScriptModel::openScript(const QString filename) {

    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return;

    script_->setPlainText(file.readAll());

    currentScriptFilename_ = filename;
}

void MicroScriptModel::saveScript(const QString filename) {

    QFile file(filename);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate))
        return;

    QTextStream out(&file);
    out << script_->toPlainText();

    currentScriptFilename_ = filename;
}

void MicroScriptModel::executeScript() {

    scriptThread_->executeScript(script_->toPlainText());
}

void MicroScriptModel::abortScript() {

    scriptThread_->abortScript();
}

void MicroScriptModel::executionStarted() {

    emit setControlsEnabled(false);
    emit clearMessageText();

    QDateTime& utime = daqModel_->currentTime();

    QString buffer;
    QXmlStreamWriter xml(&buffer);

    xml.writeStartElement("Log");
    xml.writeAttribute("time", utime.toString());
    xml.writeCharacters("script execution started");
    xml.writeEndElement();

    daqModel_->customDAQMessage(buffer);

    executionTime_ = 0;
    executionTimer_.start(1000);
}

void MicroScriptModel::executionFinished() {

    executionTimer_.stop();

    QDateTime& utime = daqModel_->currentTime();

    QString buffer;
    QXmlStreamWriter xml(&buffer);

    xml.writeStartElement("Log");
    xml.writeAttribute("time", utime.toString());
    xml.writeCharacters("script execution finished");
    xml.writeEndElement();

    daqModel_->customDAQMessage(buffer);


    emit setControlsEnabled(true);
}
void MicroScriptModel::startMeasurement() {

    if (daqModel_->daqState()==false) {
        this->message("start Measurement");
        daqModel_->startMeasurement();
    }
}

void MicroScriptModel::stopMeasurement() {

    if (daqModel_->daqState()==true) {
        this->message("stop Measurement");
        daqModel_->stopMeasurement();
    }
}

void MicroScriptModel::message(int value) {

    QString message = QString("%1")
            .arg(value);
    this->doAppendMessageText(message);
}

void MicroScriptModel::message(uint value) {

    QString message = QString("%1")
            .arg(value);
    this->doAppendMessageText(message);
}

void MicroScriptModel::message(double value) {

    QString message = QString("%1")
            .arg(value);
    this->doAppendMessageText(message);
}

void MicroScriptModel::message(const QString & text) {

    QString message = QString("%1")
            .arg(text);
    this->doAppendMessageText(message);
}

void MicroScriptModel::log(const QString & text) {

    QDateTime& utime = daqModel_->currentTime();

    QString buffer;
    QXmlStreamWriter xml(&buffer);

    xml.writeStartElement("Log");
    xml.writeAttribute("time", utime.toString());
    xml.writeCharacters(text);
    xml.writeEndElement();

    daqModel_->customDAQMessage(buffer);
}

void MicroScriptModel::doClearMessageText() {

    emit clearMessageText();
}

void MicroScriptModel::doAppendMessageText(const QString & text) {

    if (!executionTimer_.isActive()) return;

    static QString TIME_FORMAT = "yyyy-MM-dd hh:mm:ss";
    QDateTime dt = QDateTime::currentDateTime();
    QString message = QString("%1 - %2")
            .arg(dt.toString(TIME_FORMAT))
            .arg(text);
    emit appendMessageText(message);
}

void MicroScriptModel::executionHeartBeat() {

    executionTime_++;
}
