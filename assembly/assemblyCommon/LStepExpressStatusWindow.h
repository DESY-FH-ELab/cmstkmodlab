#ifndef LSTEPEXPRESSSTATUSWINDOW_H
#define LSTEPEXPRESSSTATUSWINDOW_H

#include <vector>

#include <QCheckBox>
#include <QFormLayout>
#include <QGridLayout>
#include <QLCDNumber>
#include <QVBoxLayout>
#include <QComboBox>
#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QSignalSpy>
#include <QTimer>
#include <QPlainTextEdit>
#include <QScrollBar>

#include "LStepExpressModel.h"

class LStepExpressStatusWindow : public QPlainTextEdit
{
    Q_OBJECT

public:
    explicit LStepExpressStatusWindow(LStepExpressModel* model, QWidget *parent = 0);
    ~LStepExpressStatusWindow();

protected:
    LStepExpressModel* model_;

public slots:
    void updateStatus();
    void startTimer(State);

 protected:
    QTimer* timer_;

};

#endif // LSTEPEXPRESSWIDGET_H
