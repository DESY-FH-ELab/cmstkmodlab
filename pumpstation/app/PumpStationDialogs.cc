/////////////////////////////////////////////////////////////////////////////////
//                                                                             //
//               Copyright (C) 2011-2018 - The DESY CMS Group                  //
//                           All rights reserved                               //
//                                                                             //
//      The CMStkModLab source code is licensed under the GNU GPL v3.0.        //
//      You have the right to modify and/or redistribute this source code      //
//      under the terms specified in the license, which may be found online    //
//      at http://www.gnu.org/licenses or at License.txt.                      //
//                                                                             //
/////////////////////////////////////////////////////////////////////////////////

#include <QDebug>
#include <QBoxLayout>
#include <QGridLayout>
#include <QLabel>
#include <QPushButton>
#include <QSignalMapper>
#include <QCryptographicHash>
#include <QIntValidator>

#include "PumpStationDialogs.h"

PumpStationDialog::PumpStationDialog(const QString & message, QWidget* parent)
  : QDialog(parent)
{
  QVBoxLayout * vlayout = new QVBoxLayout(this);

  QLabel * label = new QLabel(message, this);
  vlayout->addWidget(label);

  QHBoxLayout * hlayout = new QHBoxLayout();

  QPushButton * ButtonOk = new QPushButton("Yes", this);
  connect(ButtonOk, SIGNAL(clicked()), this, SLOT(accept()));
  hlayout->addWidget(ButtonOk);

  QPushButton * ButtonCancel = new QPushButton("No", this);
  ButtonCancel->setDefault(true);
  connect(ButtonCancel, SIGNAL(clicked()), this, SLOT(reject()));
  hlayout->addWidget(ButtonCancel);

  vlayout->addLayout(hlayout);
}

PumpStationPINDialog::PumpStationPINDialog(const QString & message, QWidget* parent)
  : QDialog(parent)
{
  QVBoxLayout * vlayout = new QVBoxLayout(this);

  QLabel * label;

  label = new QLabel(message, this);
  vlayout->addWidget(label);

  QHBoxLayout * hlayout;

  hlayout = new QHBoxLayout();

  label = new QLabel("Please enter PIN:", this);
  hlayout->addWidget(label);

  pinLabel_ = new QLineEdit(this);
  pinLabel_->setEchoMode(QLineEdit::Password);
  // pinLabel_->setValidator(new QIntValidator(0, 999, this));
  pinLabel_->setText("");
  hlayout->addWidget(pinLabel_);

  vlayout->addLayout(hlayout);

  QSignalMapper* mapper = new QSignalMapper();
  connect(mapper, SIGNAL(mapped(int)), this, SLOT(addNumber(int)));

  QGridLayout * grid = new QGridLayout();
  grid->setHorizontalSpacing(4);
  grid->setVerticalSpacing(4);
  QPushButton * button;
  for (int i=0;i<3;++i) {
    button = new QPushButton(QString::number(i+1), this);
    connect(button, SIGNAL(clicked()), mapper, SLOT(map()));
    mapper->setMapping(button, i+1);
    grid->addWidget(button, 0, i);

    button = new QPushButton(QString::number(i+4), this);
    connect(button, SIGNAL(clicked()), mapper, SLOT(map()));
    mapper->setMapping(button, i+4);
    grid->addWidget(button, 1, i);

    button = new QPushButton(QString::number(i+7), this);
    connect(button, SIGNAL(clicked()), mapper, SLOT(map()));
    mapper->setMapping(button, i+7);
    grid->addWidget(button, 2, i);
  }

  button = new QPushButton(QString::number(0), this);
  connect(button, SIGNAL(clicked()), mapper, SLOT(map()));
  mapper->setMapping(button, 0);
  grid->addWidget(button, 3, 1);

  button = new QPushButton("<-", this);
  connect(button, SIGNAL(clicked()), this, SLOT(removeNumber()));
  grid->addWidget(button, 3, 2);

  vlayout->addLayout(grid);

  hlayout = new QHBoxLayout();

  QPushButton * ButtonOk = new QPushButton("Yes", this);
  connect(ButtonOk, SIGNAL(clicked()), this, SLOT(accept()));
  hlayout->addWidget(ButtonOk);

  QPushButton * ButtonCancel = new QPushButton("No", this);
  ButtonCancel->setDefault(true);
  connect(ButtonCancel, SIGNAL(clicked()), this, SLOT(reject()));
  hlayout->addWidget(ButtonCancel);

  vlayout->addLayout(hlayout);
}

void PumpStationPINDialog::addNumber(int n)
{
  qDebug() << "addNumber " << n;

  pinLabel_->insert(QString::number(n));

  qDebug() << pinLabel_->text();
}

void PumpStationPINDialog::removeNumber()
{
  if (pinLabel_->text().length()>0) {

    qDebug() << "removeNumber";

    pinLabel_->backspace();

    qDebug() << pinLabel_->text();
  }
}

const QString PumpStationPINDialog::getPIN()
{
  QString pin = pinLabel_->text();

  pin = QCryptographicHash::hash(pin.toUtf8(), QCryptographicHash::Sha1).toHex('-');

  return pin;
}
