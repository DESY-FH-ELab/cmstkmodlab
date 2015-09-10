#include <nqlogger.h>

#include "IntegerComboBox.h"

IntegerComboBox::IntegerComboBox(QWidget* parent)
    : QComboBox(parent)
{

}

void IntegerComboBox::addItem(int value)
{
    QComboBox::addItem(QString::number(value), QVariant(value));
}

void IntegerComboBox::currentItemChanged(int index)
{
    QVariant data = itemData(index);
    emit valueChanged(data.toInt());
}
