#include "DefoTemperatureHistoryWidget.h"

const unsigned int DefoTemperatureHistoryWidget::LCD_SIZE = 5;

DefoTemperatureHistoryWidget::DefoTemperatureHistoryWidget(
    DefoTemperatureHistoryModel* model
//  , const QString& label
  , QWidget *parent
) :
    QWidget(parent)
  , model_(model)
{

  QGridLayout* layout = new QGridLayout(this);

  currentDisplay_ = new QLCDNumber(LCD_SIZE, this);
  currentDisplay_->setSegmentStyle(QLCDNumber::Flat);
  currentDisplay_->setSmallDecimalPoint(true);

  minDisplay_ = new QLCDNumber(LCD_SIZE, this);
  minDisplay_->setSegmentStyle(QLCDNumber::Flat);
  minDisplay_->setSmallDecimalPoint(true);

  maxDisplay_ = new QLCDNumber(LCD_SIZE, this);
  maxDisplay_->setSegmentStyle(QLCDNumber::Flat);
  maxDisplay_->setSmallDecimalPoint(true);

  // Current value
//  layout->addWidget(new QLabel(label), 0, 0, 1, 2);
  layout->addWidget(currentDisplay_, 0, 0, 2, 2);

  // Minimum value
  layout->addWidget(minDisplay_, 2, 0);
  layout->addWidget(new QLabel("min"), 3, 0);

  // Maximum value
  layout->addWidget(maxDisplay_, 2, 1);
  layout->addWidget(new QLabel("max"), 3, 1);

  connect(
          model_
        , SIGNAL(valueAdded(double))
        , this
        , SLOT(temperatureChanged(double))
  );

  connect(
          model_
        , SIGNAL(mininumChanged(double))
        , this
        , SLOT(minimumChanged(double))
  );

  connect(
          model_
        , SIGNAL(maximumChanged(double))
        , this
        , SLOT(maximumChanged(double))
  );

  if ( model_->getSize() > 0 ) {
    currentDisplay_->display(model->getLatest());
    minDisplay_->display(model_->getMinimum());
    maxDisplay_->display(model_->getMaximum());
  }

}

void DefoTemperatureHistoryWidget::temperatureChanged(double value)
{
  currentDisplay_->display(value);
}

void DefoTemperatureHistoryWidget::maximumChanged(double value)
{
  maxDisplay_->display(value);
}

void DefoTemperatureHistoryWidget::minimumChanged(double value)
{
  minDisplay_->display(value);
}
