#ifndef LSTEPEXPRESSSETTINGS_H
#define LSTEPEXPRESSSETTINGS_H

#include <vector>

#include <QObject>
#include <QString>
#include <QVariant>
#include <QMap>
#include <QList>
#include <QMutex>
#include <QMutexLocker>

#include <LStepExpressModel.h>

class LStepExpressSettingsInstruction
{
public:

    enum Type {
        Bool           =  0,
        Integer        =  1,
        VectorInteger  =  3,
        Double         =  2
    };

    explicit LStepExpressSettingsInstruction(const QString& key,
                                             const QString& setter,
                                             const QString& getter,
                                             bool needsValidConfig,
                                             bool needsValidPar);

    const QString& key() const { return key_; }
    const QString setter() { return setter_; }
    const QString& getter() const { return getter_; }
    virtual bool setValue(const QString& value) = 0;
    virtual const QString valueAsString() = 0;

    virtual const QString value() = 0;
    virtual QVariant getValue() = 0;
    virtual bool setValue(QVariant value) = 0;

    virtual Type type() = 0;

    bool needsValidConfig() { return needsValidConfig_; }
    bool needsValidPar() { return needsValidPar_; }

protected:

    QString key_;
    QString setter_;
    QString getter_;
    bool needsValidConfig_;
    bool needsValidPar_;
};

class LStepExpressSettingsInstructionB : public LStepExpressSettingsInstruction
{
public:

    explicit LStepExpressSettingsInstructionB(const QString& key,
                                              const QString& setter,
                                              const QString& getter,
                                              bool needsValidConfig,
                                              bool needsValidPar);

    bool setValue(const QString& value);
    const QString valueAsString();

    const QString value();
    virtual QVariant getValue() { return QVariant(value_); }
    bool setValue(QVariant value);

    virtual Type type() { return Bool; }

protected:

    bool value_;
};

class LStepExpressSettingsInstructionI : public LStepExpressSettingsInstruction
{
public:

    explicit LStepExpressSettingsInstructionI(const QString& key,
                                              const QString& setter,
                                              const QString& getter,
                                              bool needsValidConfig,
                                              bool needsValidPar);

    bool setValue(const QString& value);
    const QString valueAsString();

    const QString value();
    virtual QVariant getValue() { return QVariant(value_); }
    bool setValue(QVariant value);

    virtual Type type() { return Integer; }

protected:

    int value_;
};

class LStepExpressSettingsInstructionVI : public LStepExpressSettingsInstruction
{
public:

    explicit LStepExpressSettingsInstructionVI(const QString& key,
                                               const QString& setter,
                                               const QString& getter,
                                               int size,
                                               bool needsValidConfig,
                                               bool needsValidPar);

    bool setValue(const QString& value);
    const QString valueAsString();

    const QString value();
    virtual QVariant getValue();
    bool setValue(QVariant value);

    virtual Type type() { return VectorInteger; }

protected:

    int size_;
    std::vector<int> value_;
};

class LStepExpressSettingsInstructionD : public LStepExpressSettingsInstruction
{
public:

    explicit LStepExpressSettingsInstructionD(const QString& key,
                                              const QString& setter,
                                              const QString& getter,
                                              bool needsValidConfig,
                                              bool needsValidPar);

    bool setValue(const QString& value);
    const QString valueAsString();

    const QString value();
    virtual QVariant getValue() { return QVariant(value_); }
    bool setValue(QVariant value);

    virtual Type type() { return Double; }

protected:

    double value_;
};

class LStepExpressSettings : public QObject
{
Q_OBJECT

public:

    explicit LStepExpressSettings(LStepExpressModel* model, QObject* parent = 0);

    bool getDeviceState() { return model_->getDeviceState(); }

    QVariant getValueForKey(const QString& key);
    void setValueForKey(const QString& key, QVariant& value);

public slots:

    void deviceControlStateChanged(bool);

    void valueChanged(QString key, bool value);
    void valueChanged(QString key, bool value0, bool value1, bool value2);
    void valueChanged(QString key, int value);
    void valueChanged(QString key, double value);

    void readSettingsFromDevice();
    void readSettingsFromFile(const QString& filename);
    void writeSettingsToDevice();
    void writeSettingsToFile(const QString& filename);
    void saveSettingsOnDevice();
    void resetSettings();

protected:

    LStepExpressModel* model_;

    void addBA(const QString& key, const QString& setter, const QString& getter,
               bool needsValidConfig,
               bool needsValidPar);
    void addIA(const QString& key, const QString& setter, const QString& getter,
               bool needsValidConfig,
               bool needsValidPar);
    void addVIA(const QString& key, const QString& setter, const QString& getter, int size,
               bool needsValidConfig,
               bool needsValidPar);
    void addDA(const QString& key, const QString& setter, const QString& getter,
               bool needsValidConfig,
               bool needsValidPar);

    void addB(const QString& key, const QString& setter, const QString& getter,
              bool needsValidConfig,
              bool needsValidPar);
    void addI(const QString& key, const QString& setter, const QString& getter,
              bool needsValidConfig,
              bool needsValidPar);
    void addVI(const QString& key, const QString& setter, const QString& getter, int size,
              bool needsValidConfig,
              bool needsValidPar);
    void addD(const QString& key, const QString& setter, const QString& getter,
              bool needsValidConfig,
              bool needsValidPar);

    QMutex mutex_;

    QMap<QString,LStepExpressSettingsInstruction*> parameterMap_;
    QList<LStepExpressSettingsInstruction*> parameters_;

protected slots:

signals:

    void controlStateChanged(bool);
    void settingChanged(QString, QVariant);
};

#endif // LSTEPEXPRESSSETTINGS_H
