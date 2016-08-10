
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
    //General Section
    addI("BaudRate", "!baud", "baud", false, false); //9600, 19200, 38400, 57600, 115200
    addI("AxisCount", "!configmaxaxis", "configmaxaxis", false, false);
    //Axis Section
    //addBA("EnableAxes", "!axis", "axis", false, false);
    addBA("AxisDirection", "!axisdir", "axisdir", true, false);
    addIA("GearDenominator", "!geardenominator", "geardenominator", true, false); // 1 - INF
    addIA("GearNumerator", "!gearnumerator", "gearnumerator", true, false); // 1 - INF
    addDA("SpindlePitch", "!pitch", "pitch", true, false); // 0.0001 - 68
    addBA("SwapLimitSwitch", "!swchange", "swchange", true, false);
    addVIA("LimitSwitchPolarity", "!swpol", "swpol", 3, true, false);//order: E0 REF EE
    addDA("Acceleration", "!accel", "accel", false, false); // 0.01 - 20.0 [m/s^2]
    addIA("AccelerationJerk", "!acceljerk", "acceljerk", false, false); // 1 - INF [m/s^3]
    addDA("Deceleration", "!decel", "decel", false, false); // 0.01 - 20.0 [m/s^2]
    addIA("DecelerationJerk", "!deceljerk", "deceljerk", false, false); // 1 - INF [m/s^3]
    addDA("Velocity", "!vel", "vel", false, false); // 0.0 - MAX [R/s]
    addIA("Dimension", "!dim", "dim", false, false); //0 = microsteps, 1 = micrometer, 2 = millimeter, 3 = 360 degrees, 4 = number of revolutions

    //Motor Section
    addIA("MotorCurrentReduction", "!reduction", "reduction", false, false); // 0 - 100%
    addIA("MotorCurrentReductionDelay", "!curdelay", "curdelay", false, false); // 0 - 10000 [ms]
    addIA("MotorType", "!motortype", "motortype", true, false); // 0 = rotary 2-phase stepper motor, 1 = linear 2-phase stepper motor
    addIA("MotorPolePairs", "!motorpolepairs", "motorpolepairs", true, false);
    addIA("MotorPolePairResolution", "!motorpolepairres", "motorpolepairres", true, false); // 500, 1000, 2000, 32768
    addBA("MotorFieldDirection", "!motorfielddir", "motorfielddir", true, false);
    addDA("MotorMaxVelocity", "!motormaxvel", "motormaxvel", true, false);
    addIA("MotorBrake", "!motorbrake", "motorbrake", true, false); // 0 = to GND, 1 = dependent on power amplifier, 2 = to +12V
    addIA("MotorBrakeSwitchOnDelay", "!motorbrakeswitchondelay", "motorbrakeswitchondelay", true, false); // -5000 - 5000 [ms]
    addIA("MotorBrakeSwitchOffDelay", "!motorbrakeswitchoffdelay", "motorbrakeswitchoffdelay", true, false); // -5000 - 5000 [ms]
    addDA("MotorCurrent", "!motorcurrent", "motorcurrent", true, false); // 0.5 - 5.0 [A]
    addB("StopInputPolarity", "!stoppol", "stoppol", false, false); //0 = low active, 1 = high active
    addDA("StopDeceleration", "!stopdecel", "stopdecel", false, false); // 0.01 - 20.00 [m/s^2]
    addIA("StopDecelerationJerk", "!stopdeceljerk", "stopdeceljerk", false, false); // 1 - INF [m/s^3]
    addB("SpeedByPotentioMeter", "!pot", "pot", false, false); //0 = travelling at preset speed (vel), 1 = speed depends on settings potentiometer

    //Joystick Section
    addDA("JoystickSpeed", "!speed", "speed", false, false); //not sure if immediate, - MAX - MAX [R/s^2], !speed 0 is joystick mode OFF
    addBA("JoystickDirection", "!joydir", "joydir", false, false); //0 = normal, 1 = reverse
    addDA("JoystickVelocity", "!joyvel", "joyvel", false, false); // 0 - 70 [R/s] maximum velocity is set
    addDA("JoystickFilterTime", "!joyoutpass", "joyoutpass", false, false); // 0 - 500 000 microsec to prevent jerky changes in speed
    //addBA("JoystickAxesEnabled", "!joyenable", "joyenable", false, false); //enable connected joystick axes
    addBA("JoystickCurrentReduction", "!joyredcur", "joyredcur", false, false);
    //    addB("JoystickOn", "!joy", "joy", false, false); //switch joystick ON/OFF
    addDA("JoystickWindow", "!joywindow", "joywindow", false, false); // 0 - 100 -> joystick only moves if excursion is larger than this set value
    addIA("JoystickAxisAssignment", "!joytoaxis", "joytoaxis", false, false);

    //Calibration Section
    addDA("CalibrationOffset", "!caliboffset", "caliboffset", false, false); // 0 - 32*50000 (32*spindle pitch) depends on dimensions set for axes eg: 1 = 1 mm in Dim 2, zero position will be shifted
    addBA("CalibrationDirection", "!caldir", "caldir", false, false); //0 = calibrate towards positive software limit
    addDA("CalibrationSpeedAfterTrip", "!calibrmbspeed", "calibrmbspeed", false, false); // 0.0 - MAX [R/s]
    addDA("CalibrationAcceleration", "!calibrmaccel", "calibrmaccel", false, false); // 0.01 - 20.00 [m/s^2]
    addIA("CalibrationAdjustJerk", "!calibrmjerk", "calibrmjerk", false, false); // 1 - INF [m/s^3]
    addDA("CalibrationVelocity", "!calibrmvel", "calibrmvel", false, false); // 0 - 70 [R/s]
    addDA("RMOffset", "!rmoffset", "rmoffset", false, false); // 0 - 32*50000 (32*spindle pitch) depends on dimensions set for axes eg: 1 = 1 mm in Dim 2, limit position will be shifted

    //Encoder Section
    addBA("EncoderDirection", "!encdir", "encdir", true, false);
    addDA("EncoderPeriod", "!encperiod", "encperiod", true, false); // period length of the set dimension
    addIA("EncoderPolePairs", "!encpolepairs", "encpolepairs", true, false); // 1 - 50000
    addIA("EncoderToAxis", "!enctoaxis", "enctoaxis", false, false); // 1 - 6
    addIA("EncoderType", "!enctype", "enctype", true, false); //rotary 1 = 1Vss, 2 =11microA, 3 = MR, 4 = TTL, linear 5 = 1Vss, 6 = 11microA, 7 = MR, 8 = TTL
    addBA("EncoderPosition", "!encpos", "encpos", false, false);
    addBA("EncoderReference", "!encref", "encref", false, false);
    addBA("EncoderReferencePolarity", "!encrefpol", "encrefpol", false, false); // 0 = negative, 1 = positive
    addDA("TargetWindow", "!twi", "twi", true, true); //dependent on dimensions
    addDA("PositionControlKPBand", "!posconkp", "posconkp", true, true); // 0 - 4000 % 
    addIA("PositionControlTimeConstant", "!posconoutpass", "posconoutpass", true, true); // 0 - 100 000 microsec 
    addBA("PositionControlAxisEnable", "!posconenable", "posconenable", true, true);
    addB("PositionControlOn", "!poscon", "poscon", false, false);
    addDA("DeviationRange", "!deviationsrange", "deviationsrange", true, true); //depends on dimension
    addIA("DeviationTime", "!deviationtime", "deviationtime", true, true); // 0 - 10000 millisec
    addBA("DeviationCheck", "!deviationcheck", "deviationcheck", true, true); 

    //Limit switch
    addVIA("Limit", "!lim", "lim", 2, false, false); // - MAX - + MAX, dependent on dimensions, always enter in pairs
    addBA("RangeMonitoring", "!limctr", "limctr", false, false);
    addBA("NoSoftwareLimit", "!nosetlimit", "nosetlimit", false, false);
    addVIA("LimitSwitchOn", "!swact", "swact", 3, true, true); //order: E0 REF EE

    //Clock pulse
    addIA("ClockAxisAssignment", "!tvrtoaxis", "tvrtoaxis", false, false); // 0 = no allocation, 1 = QEP1, 2 = QEP2
    addBA("ClockPulseOn", "!tvr", "tvr", false, false);
    addIA("ClockPulseFactor", "!tvrf", "tvrf", false, false); // -10000 - 10000, one clock pulse = x motor increments (MI)
    addIA("ClockMode", "!tvrm", "tvrm", false, false); // 0 = Clock pulse OFF, 1 = normal (1 clock pulse = 1 MI), 2 = clock pulse with factor, 3 = clock pulse requires external enabling, 4 = combination of 2 and 3

    //Trigger Output
    addB("TriggerOn", "!trig", "trig", false, false);
    addI("TriggerAxis", "!triga", "triga", false, false); // check how is done
    addI("TriggerMode", "!trigm", "trigm", false, false); // 0 - 17 or 99
    addI("TriggerSignal", "!trigs", "trigs", false, false); // 3 - 120 microsec
    addD("TriggerDistance", "!trigd", "trigd", false, false); //depends on dimensions
    addB("TriggerCount", "!trigcount", "trigcount", false, false); //reads trigger count

    //Extra: Trackball, handwheel or jog mode
    addDA("JogModeVelocity", "!tippvel", "tippvel", false, false); // 0 - 70 [R/s] 
    addDA("JogModeFilterTime", "!tippoutpass", "tippoutpass", false, false); // 0 - 500 000 microsec to prevent jerky changes in speed
    addBA("JogModeDirection", "!tippdir", "tippdir", false, false);    
    addBA("JogModeAxesEnabled", "!tippenable", "tippenable", false, false); //enable connected axes
    addBA("JogModeCurrentReduction", "!tippredcur", "tippredcur", false, false);
    addB("JogModeOn", "!tipp", "tipp", false, false);
    addBA("TrackBallAxesEnable", "!tbenable", "tbenable", false, false);    
    addIA("TrackBallAxisAssignment", "!tbtoaxis", "tbtoaxis", false, false);
    addBA("TrackBallDirection", "!tbdir", "tbdir", false, false);
    addBA("TrackBallCurrentReduction", "!tbredcur", "tbredcur", false, false);
    addDA("TrackBallVelocity", "!tbvel", "tbvel", false, false);
    addDA("TrackBallFilterTime", "!tboutpass", "tboutpass", false, false); // 0 - 500 000 microsec
    addB("TrackBallOn", "!tb", "tb", false, false);

    //not in English manual, maybe in the German one?
    //addI("EQEP", "!eqepconfig", "eqepconfig", true, false);
    //addBA("MotorTempSensor", "!motortempsensor", "motortempsensor", true, false);
    //addIA("MotorTempSensorLowerLimit", "!motortempsensorminimum", "motortempsensorminimum", true, false);
    //addIA("MotorTempSensorUpperLimit", "!motortempsensormaximum", "motortempsensormaximum", true, false);
    //addDA("MotorMomentConstant", "!motormomentconstant", "motormomentconstant", true, true); // 0 - 50.000 [Nm/A]
    //addDA("MotorMomentOfInertia", "!motormomentofinertia", "motormomentofinertia", true, true); // 0 - 100000 [kg cm^2]
    //    addBA("SpeedFeedForward", "!speedfeedforward", "speedfeedforward", );//check
    //addBA("ModuloAxis", "!modulo", "modulo", true, false);
    //addIA("ModuloAxisMode", "!modulomode", "modulomode", false, false);
    //    addB("TTLOut", "!ttloutconfig", "ttloutconfig", true, false);
    //    addBA("Configured", "!configurated", "configurated", true, false);
    //addIA("MotorPoleScale", "!motorpolescale", "motorpolescale", true, false);    

    connect(model_, SIGNAL(controlStateChanged(bool)),
            this, SLOT(deviceControlStateChanged(bool)));


    connect(this, SIGNAL(controlStateChanged(bool)),
	this, SLOT(printSpyInformation()));
    connect(this, SIGNAL(settingChanged(QString, QVariant)),
	this, SLOT(printSpyInformation()));

    spyControlStateChanged = new QSignalSpy(this, SIGNAL(controlStateChanged(bool)));
    spySettingChanged = new QSignalSpy(this, SIGNAL(settingChanged(QString, QVariant)));
}

void LStepExpressSettings::printSpyInformation()
{

    for(int i = 0; i < spyControlStateChanged->size(); i++){
        std::cout<<"SPY LStepExpressSettings " << "this_, signal controlStateChanged()"    <<std::endl;
    }
    spyControlStateChanged->clear();
    for(int i = 0; i < spySettingChanged->size(); i++){
        std::cout<<"SPY LStepExpressSettings " << "this_, signal settingChanged()"    <<std::endl;
    }
    spySettingChanged->clear();
}

void LStepExpressSettings::deviceControlStateChanged(bool enabled)
{
    emit controlStateChanged(enabled);
}

void LStepExpressSettings::valueChanged(QString key, bool value)
{
    std::cout<<"LStepExpressSettings " << "valueChanged " << key.toStdString() << " " << value    <<std::endl;
}

void LStepExpressSettings::valueChanged(QString key, bool value0, bool value1, bool value2)
{
    std::cout<<"LStepExpressSettings " << "valueChanged " << key.toStdString() << " " << value0 << " " << value1 << " " << value2    <<std::endl;
}

void LStepExpressSettings::valueChanged(QString key, int value)
{
    std::cout<<"LStepExpressSettings " << "valueChanged " << key.toStdString() << " " << value    <<std::endl;
}

void LStepExpressSettings::valueChanged(QString key, double value)
{
    std::cout<<"LStepExpressSettings " << "valueChanged " << key.toStdString() << " " << value    <<std::endl;
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

    while (model_->isUpdating()) usleep(100);

    model_->pauseUpdate();

    for (QList<LStepExpressSettingsInstruction*>::iterator it = parameters_.begin();
         it!=parameters_.end();
         ++it) {
        LStepExpressSettingsInstruction* setting = *it;

        QString value;
        model_->getValue(setting->getter(), value);

        std::cout<<"LStepExpressSettings " << setting->getter().toStdString() << " -> " << value.toStdString()    <<std::endl;

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
        std::cout<<"LStepExpressSettings " << setting->setter().toStdString() << " <- " << setting->value().toStdString()    <<std::endl;
    }

    model_->validConfig();
    model_->validParameter();
}

void LStepExpressSettings::saveSettingsOnDevice()
{
    QMutexLocker locker(&mutex_);
    model_->saveConfig();

    std::cout<<"LStepExpressSettings " << "settings saved on controller"    <<std::endl;
}

void LStepExpressSettings::resetSettings()
{
    QMutexLocker locker(&mutex_);
    model_->reset();

    std::cout<<"LStepExpressSettings " << "reset settings to startup conditions"    <<std::endl;
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
