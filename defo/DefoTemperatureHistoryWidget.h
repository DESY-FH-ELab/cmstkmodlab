#ifndef DEFOTEMPERATUREHISTORYWIDGET_H
#define DEFOTEMPERATUREHISTORYWIDGET_H

#include <QGridLayout>
#include <QLabel>
#include <QLCDNumber>
#include <QString>
#include <QWidget>
#include "DefoTemperatureHistoryModel.h"

class DefoTemperatureHistoryWidget : public QWidget
{
    Q_OBJECT
public:
  explicit DefoTemperatureHistoryWidget(
      DefoTemperatureHistoryModel* model
//    , const QString& label
    , QWidget *parent = 0
  );

//  DefoTemperatureHistoryModel* getModel();

protected:
  static const unsigned int LCD_SIZE;

  DefoTemperatureHistoryModel* model_;

  QLCDNumber* currentDisplay_;
  QLCDNumber* minDisplay_;
  QLCDNumber* maxDisplay_;

protected slots:
  void temperatureChanged( double value );
  void maximumChanged( double value );
  void minimumChanged( double value );

};

#endif // DEFOTEMPERATUREHISTORYWIDGET_H
