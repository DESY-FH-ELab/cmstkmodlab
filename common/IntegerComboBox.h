#ifndef INTEGERCOMBOBOX_H
#define INTEGERCOMBOBOX_H

#include <QComboBox>

class IntegerComboBox : public QComboBox
{
    Q_OBJECT
public:
    explicit IntegerComboBox(QWidget* parent=0);

    void addItem(int value);

public slots:

protected:

protected slots:

    void currentItemChanged(int index);

signals:

    void valueChanged(int value);
};

#endif // INTEGERCOMBOBOX_H
