#include <cmath>

#include <nqlogger.h>

#include "PlasmaStepDoubleSpinBox.h"

PlasmaStepDoubleSpinBox::PlasmaStepDoubleSpinBox(QWidget *parent)
  : QDoubleSpinBox(parent)
{

}

QString PlasmaStepDoubleSpinBox::textFromValue(double value) const
{
  double steps = value / singleStep();

  QString text = QWidget::locale().toString(std::round(steps)*singleStep(), 'f', decimals());

  return text;
}

double PlasmaStepDoubleSpinBox::valueFromText(const QString & text) const
{
  double value = QDoubleSpinBox::valueFromText(text);

  value = std::round(value / singleStep())*singleStep();

  return value;
}
