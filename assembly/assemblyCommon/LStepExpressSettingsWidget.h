#ifndef LSTEPEXPRESSSETTINGSWIDGET_H
#define LSTEPEXPRESSSETTINGSWIDGET_H

#include <vector>

#include <QCheckBox>
#include <QDoubleSpinBox>
#include <QFormLayout>
#include <QGridLayout>
#include <QPushButton>
#include <QVBoxLayout>
#include <QComboBox>
#include <QWidget>
#include <QLabel>
#include <QToolBox>
#include <QWheelEvent>

#include "LStepExpressSettings.h"

class LStepExpressSettingsCheckBox : public QCheckBox
{
      Q_OBJECT

public:

    LStepExpressSettingsCheckBox(LStepExpressSettings* settings, const QString& key,
                                 QWidget * parent);

protected slots:

    void settingChanged(QString key, QVariant value);
    void statusChanged(int /* state */);

protected:

    LStepExpressSettings* settings_;
    QString key_;

signals:

    void valueChanged(QString, bool);
};

class LStepExpressSettingsTripleCheckBox : public QWidget
{
      Q_OBJECT

public:

    LStepExpressSettingsTripleCheckBox(LStepExpressSettings* settings, const QString& key,
                                       QWidget * parent);

protected slots:

    void settingChanged(QString key, QVariant value);
    void statusChanged(int /* state */);

protected:

    LStepExpressSettings* settings_;
    QString key_;
    QCheckBox* box_[3];

signals:

    void valueChanged(QString, bool, bool, bool);
};

class LStepExpressSettingsIntSpinBox : public QSpinBox
{
      Q_OBJECT

public:

    LStepExpressSettingsIntSpinBox(LStepExpressSettings* settings, const QString& key,
                                   int min, int max,
                                   QWidget * parent);

protected slots:

    void settingChanged(QString key, QVariant value);
    void handleValueChanged(int value);

protected:

    void wheelEvent(QWheelEvent * event);

    LStepExpressSettings* settings_;
    QString key_;

signals:

    void valueChanged(QString, int);
};

class LStepExpressSettingsDoubleSpinBox : public QDoubleSpinBox
{
    Q_OBJECT

public:

    LStepExpressSettingsDoubleSpinBox(LStepExpressSettings* settings, const QString& key,
                                      double min, double max,
                                      QWidget * parent);
protected slots:

    void settingChanged(QString key, QVariant value);
    void handleValueChanged(double value);

protected:

    void wheelEvent(QWheelEvent * event);

    LStepExpressSettings* settings_;
    QString key_;

signals:

    void valueChanged(QString, double);
};

class LStepExpressSettingsWidget : public QWidget
{
    Q_OBJECT

public:
    explicit LStepExpressSettingsWidget(LStepExpressSettings* settings, QWidget *parent = 0);

protected:

    LStepExpressSettings* settings_;

    QToolBox * mainToolBox_;
    QWidget * generalToolBox_;
    QWidget * axisToolBox_;
    QWidget * motorToolBox_;
    QWidget * encoderToolBox_;
    QWidget * joystickToolBox_;

    QPushButton * readFromDeviceButton_;
    QPushButton * readFromFileButton_;
    QPushButton * writeToDeviceButton_;
    QPushButton * writeToFileButton_;

    void fillGeneralToolBox();
    void fillAxisToolBox();
    void fillMotorToolBox();
    void fillEncoderToolBox();
    void fillJoystickToolBox();

public slots:

protected slots:

    void controlStateChanged(bool);
    void readFromDeviceClicked();
    void readFromFileClicked();
    void writeToDeviceClicked();
    void writeToFileClicked();
};

#endif // LSTEPEXPRESSSETTINGSWIDGET_H
