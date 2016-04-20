
#include <QStringList>
#include <QApplication>
#include <QFile>
#include <QTextStream>

#include <nqlogger.h>

#include "LStepExpressSettings.h"

LStepExpressSettingsInstruction::LStepExpressSettingsInstruction(const QString& key,
                                                                 const QString& setter,
                                                                 const QString& getter,
                                                                 bool needsValidConfig,
                                                                 bool needsValidPar)
    : key_(key),
      setter_(setter),
      getter_(getter),
      needsValidConfig_(needsValidConfig),
      needsValidPar_(needsValidPar)
{

}

LStepExpressSettingsInstructionB::LStepExpressSettingsInstructionB(const QString& key,
                                                                   const QString& setter,
                                                                   const QString& getter,
                                                                   bool needsValidConfig,
                                                                   bool needsValidPar)
    : LStepExpressSettingsInstruction(key, setter, getter, needsValidConfig, needsValidPar),
      value_(false)
{

}

const QString LStepExpressSettingsInstructionB::value()
{
    return QString(value_==true?"1":"0");
}

bool LStepExpressSettingsInstructionB::setValue(const QString& value)
{
    bool newValue;
    bool ret = false;

    if (value=="1") {
        newValue = true;
    } else {
        newValue = false;
    }

    if (newValue!=value_) ret = true;
    value_ = newValue;

    return ret;
}

bool LStepExpressSettingsInstructionB::setValue(QVariant value)
{
    bool newValue;
    bool ret = false;

    newValue = value.toBool();

    if (newValue!=value_) ret = true;
    value_ = newValue;

    return ret;
}

const QString LStepExpressSettingsInstructionB::valueAsString()
{
    return QString(value_==true?"1":"0");
}

LStepExpressSettingsInstructionI::LStepExpressSettingsInstructionI(const QString& key,
                                                                   const QString& setter,
                                                                   const QString& getter,
                                                                   bool needsValidConfig,
                                                                   bool needsValidPar)
    : LStepExpressSettingsInstruction(key, setter, getter, needsValidConfig, needsValidPar),
      value_(0)
{

}

const QString LStepExpressSettingsInstructionI::value()
{
    return QString::number(value_);
}

bool LStepExpressSettingsInstructionI::setValue(const QString& value)
{
    int newValue;
    bool ret = false;

    newValue = value.toInt();

    if (newValue!=value_) ret = true;
    value_ = newValue;

    return ret;
}

bool LStepExpressSettingsInstructionI::setValue(QVariant value)
{
    int newValue;
    bool ret = false;

    newValue = value.toInt();

    if (newValue!=value_) ret = true;
    value_ = newValue;

    return ret;
}

const QString LStepExpressSettingsInstructionI::valueAsString()
{
    return QString::number(value_);
}

LStepExpressSettingsInstructionVI::LStepExpressSettingsInstructionVI(const QString& key,
                                                                   const QString& setter,
                                                                   const QString& getter,
                                                                     int size,
                                                                   bool needsValidConfig,
                                                                   bool needsValidPar)
    : LStepExpressSettingsInstruction(key, setter, getter, needsValidConfig, needsValidPar),
      size_(size)
{
    value_.resize(size_, 0);
}

const QString LStepExpressSettingsInstructionVI::value()
{
    QString temp;
    for (std::vector<int>::iterator it = value_.begin();
         it!=value_.end();
         ++it) {
        if (it!=value_.begin()) temp += " ";
        temp += QString::number(*it);
    }
    return temp;
}

bool LStepExpressSettingsInstructionVI::setValue(const QString& value)
{
    int newValue;
    bool ret = false;

    QStringList tokens = value.split(" ", QString::SkipEmptyParts);
    int i = 0;
    for (QStringList::iterator it = tokens.begin();
         it!=tokens.end();
         ++it) {
        newValue = (*it).toInt();
        if (newValue!=value_[i]) ret = true;
        value_[i] = newValue;
        i++;
    }

    return ret;
}

bool LStepExpressSettingsInstructionVI::setValue(QVariant value)
{
    int newValue;
    bool ret = false;

    QList<QVariant> list = value.toList();
    int i = 0;
    for (QList<QVariant>::Iterator it = list.begin();
         it!=list.end();
         ++it) {
        newValue = (*it).toInt();
        if (newValue!=value_[i]) ret = true;
        value_[i] = newValue;
        i++;
    }

    return ret;
}

const QString LStepExpressSettingsInstructionVI::valueAsString()
{
    QString temp;
    for (std::vector<int>::iterator it = value_.begin();
         it!=value_.end();
         ++it) {
        if (it!=value_.begin()) temp += " ";
        temp += QString::number(*it);
    }
    return temp;
}

QVariant LStepExpressSettingsInstructionVI::getValue()
{
    QList<QVariant> list;
    for (std::vector<int>::iterator it = value_.begin();
         it!=value_.end();
         ++it) {
        list.append(QVariant(*it));
    }
    return QVariant(list);
}

LStepExpressSettingsInstructionD::LStepExpressSettingsInstructionD(const QString& key,
                                                                   const QString& setter,
                                                                   const QString& getter,
                                                                   bool needsValidConfig,
                                                                   bool needsValidPar)
    : LStepExpressSettingsInstruction(key, setter, getter, needsValidConfig, needsValidPar),
      value_(0.0)
{

}

const QString LStepExpressSettingsInstructionD::value()
{
    return QString::number(value_);
}

bool LStepExpressSettingsInstructionD::setValue(const QString& value)
{
    double newValue;
    bool ret = false;

    newValue = value.toDouble();

    if (newValue!=value_) ret = true;
    value_ = newValue;

    return ret;
}

bool LStepExpressSettingsInstructionD::setValue(QVariant value)
{
    double newValue;
    bool ret = false;

    newValue = value.toDouble();

    if (newValue!=value_) ret = true;
    value_ = newValue;

    return ret;
}

const QString LStepExpressSettingsInstructionD::valueAsString()
{
    return QString::number(value_);
}

LStepExpressSettings::LStepExpressSettings(LStepExpressModel* model, QObject* parent)
    : QObject(parent),
      model_(model)
{
    addI("AxisCount", "!configmaxaxis", "configmaxaxis", false, false);

    addI("EQEP", "!eqepconfig", "eqepconfig", true, false);
    addB("TTLOut", "!ttloutconfig", "ttloutconfig", true, false);

    addBA("Configured", "!configurated", "configurated", true, false);

    addBA("AxisDirection", "!axisdir", "axisdir", true, false);
    addIA("GearDenominator", "!geardenominator", "geardenominator", true, false); // 1 - INF
    addIA("GearNumerator", "!gearnumerator", "gearnumerator", true, false); // 1 - INF
    addDA("SpindlePitch", "!pitch", "pitch", true, false); // 0.0001 - 68
    addBA("SwapLimitSwitch", "!swchange", "swchange", true, false);
    addVIA("LimitSwitchPolarity", "!swpol", "swpol", 3, true, false);
    addDA("Acceleration", "!accel", "accel", false, false); // 0.01 - 20.0 [m/s^2]
    addIA("AccelerationJerk", "!acceljerk", "acceljerk", false, false); // 1 - INF [m/s^3]
    addDA("Deceleration", "!decel", "decel", false, false); // 0.01 - 20.0 [m/s^2]
    addIA("DecelerationJerk", "!deceljerk", "deceljerk", false, false); // 1 - INF [m/s^3]
    addDA("Velocity", "!vel", "vel", false, false); // 0.0 - MAX [R/s]

    addIA("MotorCurrentReduction", "!reduction", "reduction", false, false); // 0 - 100%
    addIA("MotorCurrentReductionDelay", "!curdelay", "curdelay", false, false); // 0 - 10000 [ms]
    addBA("ModuloAxis", "!modulo", "modulo", true, false);
    addIA("ModuloAxisMode", "!modulomode", "modulomode", false, false);

    addIA("MotorType", "!motortype", "motortype", true, false);
    addIA("MotorPoleScale", "!motorpolescale", "motorpolescale", true, false);
    addIA("MotorPolePairs", "!motorpolepairs", "motorpolepairs", true, false);
    addIA("MotorPolePairResolution", "!motorpolepairres", "motorpolepairres", true, false); // 500, 1000, 2000, 32768
    addBA("MotorFieldDirection", "!motorfielddir", "motorfielddir", true, false);
    addDA("MotorMaxVelocity", "!motormaxvel", "motormaxvel", true, false);
    addIA("MotorBrake", "!motorbrake", "motorbrake", true, false);
    addIA("MotorBrakeSwitchOnDelay", "!motorbrakeswitchondelay", "motorbrakeswitchondelay", true, false); // -5000 - 5000 [ms]
    addIA("MotorBrakeSwitchOffDelay", "!motorbrakeswitchoffdelay", "motorbrakeswitchoffdelay", true, false); // -5000 - 5000 [ms]
    addBA("MotorTempSensor", "!motortempsensor", "motortempsensor", true, false);
    addIA("MotorTempSensorLowerLimit", "!motortempsensorminimum", "motortempsensorminimum", true, false);
    addIA("MotorTempSensorUpperLimit", "!motortempsensormaximum", "motortempsensormaximum", true, false);
    addDA("MotorMomentConstant", "!motormomentconstant", "motormomentconstant", true, true); // 0 - 50.000 [Nm/A]
    addDA("MotorMomentOfInertia", "!motormomentofinertia", "motormomentofinertia", true, true); // 0 - 100000 [kg cm^2]
    addDA("MotorCurrent", "!motorcurrent", "motorcurrent", true, false); // 0.5 - 5.0 [A]

    addBA("EncoderDirection", "!encdir", "encdir", true, false);
    addDA("EncoderPeriod", "!encperiod", "encperiod", true, false);
    addIA("EncoderPolePairs", "!encpolepairs", "encpolepairs", true, false);
    addIA("EncoderToAxis", "!enctoaxis", "enctoaxis", true, false);
    addIA("EncoderType", "!enctype", "enctype", true, false);
    addBA("EncoderPosition", "!encpos", "encpos", false, false);
    addBA("EncoderReference", "!encref", "encref", false, false);
    addBA("EncoderReferencePolarity", "!encrefpol", "encrefpol", false, false);


    connect(model_, SIGNAL(controlStateChanged(bool)),
            this, SLOT(deviceControlStateChanged(bool)));
}

void LStepExpressSettings::deviceControlStateChanged(bool enabled)
{
    emit controlStateChanged(enabled);
}

void LStepExpressSettings::valueChanged(QString key, bool value)
{
    NQLog("LStepExpressSettings", NQLog::Spam) << "valueChanged " << key << " " << value;
}

void LStepExpressSettings::valueChanged(QString key, bool value0, bool value1, bool value2)
{
    NQLog("LStepExpressSettings", NQLog::Spam) << "valueChanged " << key << " " << value0 << " " << value1 << " " << value2;
}

void LStepExpressSettings::valueChanged(QString key, int value)
{
    NQLog("LStepExpressSettings", NQLog::Spam) << "valueChanged " << key << " " << value;
}

void LStepExpressSettings::valueChanged(QString key, double value)
{
    NQLog("LStepExpressSettings", NQLog::Spam) << "valueChanged " << key << " " << value;
}

QVariant LStepExpressSettings::getValueForKey(const QString& key)
{
    QMutexLocker locker(&mutex_);

    QMap<QString,LStepExpressSettingsInstruction*>::iterator it = parameterMap_.find(key);
    if (it==parameterMap_.end()) return QVariant();

    LStepExpressSettingsInstruction* setting = it.value();

    return setting->getValue();
}

void LStepExpressSettings::setValueForKey(const QString& key, QVariant& value)
{
    QMutexLocker locker(&mutex_);

    QMap<QString,LStepExpressSettingsInstruction*>::iterator it = parameterMap_.find(key);
    if (it==parameterMap_.end()) return;

    LStepExpressSettingsInstruction* setting = it.value();

    if (setting->setValue(value)) {
        emit settingChanged(key, value);
    }
}

void LStepExpressSettings::readSettingsFromDevice()
{
    QMutexLocker locker(&mutex_);

    //this randomly staryted to cause compilation errors on mac...???
    //while (model_->isUpdating()) usleep(100);

    model_->pauseUpdate();

    for (QList<LStepExpressSettingsInstruction*>::iterator it = parameters_.begin();
         it!=parameters_.end();
         ++it) {
        LStepExpressSettingsInstruction* setting = *it;

        QString value;
        model_->getValue(setting->getter(), value);

        NQLog("LStepExpressSettings", NQLog::Spam) << setting->getter() << " -> " << value;

        if (setting->setValue(value)) {
            emit settingChanged(setting->key(),setting->getValue());
        }
    }

    model_->continueUpdate();
}

void LStepExpressSettings::readSettingsFromFile(const QString& filename)
{
    QFile data(filename);

    QString key, value;
    if (data.open(QIODevice::ReadOnly)) {
        QTextStream in(&data);

        while (!in.atEnd()) {
            in >> key;
            value = in.readLine().simplified();

            QMap<QString,LStepExpressSettingsInstruction*>::iterator it =  parameterMap_.find(key);
            if (it!=parameterMap_.end()) {
                LStepExpressSettingsInstruction* setting = *it;

                if (setting->setValue(value)) {
                    emit settingChanged(setting->key(), setting->getValue());
                }

            }
        }
    }
}

void LStepExpressSettings::writeSettingsToDevice()
{
    QMutexLocker locker(&mutex_);

    for (QList<LStepExpressSettingsInstruction*>::iterator it = parameters_.begin();
         it!=parameters_.end();
         ++it) {
        LStepExpressSettingsInstruction* setting = *it;

        model_->setValue(setting->setter(), setting->value());
        NQLog("LStepExpressSettings", NQLog::Spam) << setting->setter() << " <- " << setting->value();
    }

    model_->validConfig();
    model_->validParameter();
}

void LStepExpressSettings::writeSettingsToFile(const QString& filename)
{
    QFile data(filename);
    if (data.open(QFile::WriteOnly | QFile::Truncate)) {
        QTextStream out(&data);

        for (QList<LStepExpressSettingsInstruction*>::iterator it = parameters_.begin();
             it!=parameters_.end();
             ++it) {
            LStepExpressSettingsInstruction* setting = *it;

            out << qSetFieldWidth(40) << left << setting->key()
                << qSetFieldWidth(20) << right << setting->valueAsString() << "\n";
        }
    }
}

void LStepExpressSettings::addBA(const QString& key, const QString& setter, const QString& getter,
                                 bool needsValidConfig,
                                 bool needsValidPar)
{
    addB(QString("X-") + key, setter + " x", getter + " x", needsValidConfig, needsValidPar);
    addB(QString("Y-") + key, setter + " y", getter + " y", needsValidConfig, needsValidPar);
    addB(QString("Z-") + key, setter + " z", getter + " z", needsValidConfig, needsValidPar);
    addB(QString("A-") + key, setter + " a", getter + " a", needsValidConfig, needsValidPar);
}

void LStepExpressSettings::addIA(const QString& key, const QString& setter, const QString& getter,
           bool needsValidConfig,
           bool needsValidPar)
{
    addI(QString("X-") + key, setter + " x", getter + " x", needsValidConfig, needsValidPar);
    addI(QString("Y-") + key, setter + " y", getter + " y", needsValidConfig, needsValidPar);
    addI(QString("Z-") + key, setter + " z", getter + " z", needsValidConfig, needsValidPar);
    addI(QString("A-") + key, setter + " a", getter + " a", needsValidConfig, needsValidPar);
}

void LStepExpressSettings::addVIA(const QString& key, const QString& setter, const QString& getter,
                                  int size,
                                  bool needsValidConfig,
                                  bool needsValidPar)
{
    addVI(QString("X-") + key, setter + " x", getter + " x", size, needsValidConfig, needsValidPar);
    addVI(QString("Y-") + key, setter + " y", getter + " y", size, needsValidConfig, needsValidPar);
    addVI(QString("Z-") + key, setter + " z", getter + " z", size, needsValidConfig, needsValidPar);
    addVI(QString("A-") + key, setter + " a", getter + " a", size, needsValidConfig, needsValidPar);
}

void LStepExpressSettings::addDA(const QString& key, const QString& setter, const QString& getter,
           bool needsValidConfig,
           bool needsValidPar)
{
    addD(QString("X-") + key, setter + " x", getter + " x", needsValidConfig, needsValidPar);
    addD(QString("Y-") + key, setter + " y", getter + " y", needsValidConfig, needsValidPar);
    addD(QString("Z-") + key, setter + " z", getter + " z", needsValidConfig, needsValidPar);
    addD(QString("A-") + key, setter + " a", getter + " a", needsValidConfig, needsValidPar);
}

void LStepExpressSettings::addB(const QString& key, const QString& setter, const QString& getter,
                                bool needsValidConfig,
                                bool needsValidPar)
{
    LStepExpressSettingsInstruction* parameter = new LStepExpressSettingsInstructionB(key, setter, getter,
                                                                                      needsValidConfig,
                                                                                      needsValidPar);
    parameterMap_.insert(key, parameter);
    parameters_.append(parameter);
}

void LStepExpressSettings::addI(const QString& key, const QString& setter, const QString& getter,
                                bool needsValidConfig,
                                bool needsValidPar)
{
    LStepExpressSettingsInstruction* parameter = new LStepExpressSettingsInstructionI(key, setter, getter,
                                                                                      needsValidConfig,
                                                                                      needsValidPar);
    parameterMap_.insert(key, parameter);
    parameters_.append(parameter);
}

void LStepExpressSettings::addVI(const QString& key, const QString& setter, const QString& getter,
                                 int size,
                                 bool needsValidConfig,
                                 bool needsValidPar)
{
    LStepExpressSettingsInstruction* parameter = new LStepExpressSettingsInstructionVI(key, setter, getter, size,
                                                                                       needsValidConfig,
                                                                                       needsValidPar);
    parameterMap_.insert(key, parameter);
    parameters_.append(parameter);
}

void LStepExpressSettings::addD(const QString& key, const QString& setter, const QString& getter,
                                bool needsValidConfig,
                                bool needsValidPar)
{
    LStepExpressSettingsInstruction* parameter = new LStepExpressSettingsInstructionD(key, setter, getter,
                                                                                      needsValidConfig,
                                                                                      needsValidPar);
    parameterMap_.insert(key, parameter);
    parameters_.append(parameter);
}
