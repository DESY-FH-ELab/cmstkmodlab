#ifndef LSTEPEXPRESSPOSITIONWIDGET_H
#define LSTEPEXPRESSPOSITIONWIDGET_H

#include <vector>
#include <QSignalSpy>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QWidget>
#include <QFormLayout>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QString>

#include "LStepExpressModel.h"
#include "LStepExpressMotionManager.h"

class LStepExpressPositionAxisWidget : public QWidget
{
    Q_OBJECT

 public:
//explicit LStepExpressPositionAxisWidget(LStepExpressModel* model, unsigned int axis, QWidget * parent = 0);
    explicit LStepExpressPositionAxisWidget(LStepExpressMotionManager* manager, LStepExpressModel* model, unsigned int axis, QWidget * parent = 0);

 protected:
    LStepExpressModel* model_;
    LStepExpressMotionManager* manager_;
    unsigned int axis_;
    QHBoxLayout* layout_;
    QString axisDimensionName_;
    double position_;
    QLineEdit* Edit_;
    QLabel* Unit_;
    QSignalSpy* spyEdit_;
    QSignalSpy* spyPositionChanged_;

 public slots:
        void updateWidgets();
        void printSpyInformation();

 signals:
        void positionChanged(double value, unsigned int axis);
};

class LStepExpressPositionWidget : public QWidget
{
    Q_OBJECT

 public:
//    explicit LStepExpressPositionWidget(LStepExpressModel* model, QWidget *parent = 0);
    explicit LStepExpressPositionWidget(LStepExpressMotionManager* manager, LStepExpressModel* model, QWidget *parent = 0);

 protected:
    LStepExpressModel* model_;
    LStepExpressMotionManager* manager_;
    QVBoxLayout* layout_;
    QPushButton* moveAbsoluteButton_;
    QPushButton* moveRelativeButton_;
    std::vector<double> position_;
    LStepExpressPositionAxisWidget* xpos_;
    LStepExpressPositionAxisWidget* ypos_;
    LStepExpressPositionAxisWidget* zpos_;
    LStepExpressPositionAxisWidget* apos_;
    QSignalSpy* spyMoveAbsoluteButton_;
    QSignalSpy* spyMoveRelativeButton_;

 public slots:
     void lStepStateChanged( State newState);
     void moveAbsoluteButtonClicked();
     void moveRelativeButtonClicked();
     void positionChanged(double value, unsigned int axis);
     void printSpyInformation();
};

#endif //LSTEPEXPRESSPOSITIONWIDGET_H
