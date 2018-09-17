/////////////////////////////////////////////////////////////////////////////////
//                                                                             //
//               Copyright (C) 2011-2017 - The DESY CMS Group                  //
//                           All rights reserved                               //
//                                                                             //
//      The CMStkModLab source code is licensed under the GNU GPL v3.0.        //
//      You have the right to modify and/or redistribute this source code      //
//      under the terms specified in the license, which may be found online    //
//      at http://www.gnu.org/licenses or at License.txt.                      //
//                                                                             //
/////////////////////////////////////////////////////////////////////////////////

#include <stdlib.h>

#include <string>
#include <iostream>

#include <QDebug>
#include <QBoxLayout>
#include <QFileDialog>
#include <QApplication>
#include <QPalette>
#include <QLabel>

#include <nqlogger.h>
#include <ApplicationConfig.h>

#include "PumpStationMainWindow.h"
#include "PumpStationSVG.h"
#include "PumpStationDialogs.h"

PumpStationMainWindow::PumpStationMainWindow(PumpStationHTTPModel* model,
                                             QWidget *parent)
  : QMainWindow(parent),
    model_(model),
    pin_("81-ec-fd-43-83-a1-b3-f7-80-52-15-da-76-9e-4b-b7-e3-68-45-1e"),
    locked_(true)
{
  QPalette pal;
  pal.setColor(QPalette::Background, Qt::white);
  setPalette(pal);

  QWidget * w = new QWidget(this);
  w->setPalette(pal);

  QVBoxLayout * layout = new QVBoxLayout(w);
  layout->setContentsMargins(20, 20, 20, 20);
  layout->setSpacing(20);

  QLabel * label = new QLabel("CMS Pump Station Control", w);
  QFont font = label->font();
  font.setPointSize(36);
  font.setBold(true);
  label->setFont(font);
  label->setAlignment(Qt::AlignHCenter);
  layout->addWidget(label);

  sketch_ = new PumpStationSVGWidget(w);
  sketch_->setMinimumSize(800, sketch_->heightForWidth(800));
  sketch_->resize(800, sketch_->heightForWidth(800));
  layout->addWidget(sketch_);

  connect(sketch_, SIGNAL(buttonDoubleClicked(int)), this, SLOT(buttonDoubleClicked(int)));

  sketchSource_ = svgString;

  connect(model_, SIGNAL(valuesChanged()), this, SLOT(updateSketch()));
  connect(this, SIGNAL(toggleSwitch(int)), model_, SLOT(toggleSwitch(int)));
  connect(model_, SIGNAL(enableWidgets()), this, SLOT(enableWidgets()));

  setCentralWidget(w);

  updateGeometry();
}

void PumpStationMainWindow::updateSketch()
{
  qDebug() << "void PumpStationMainWindow::updateSketch()";

  QString tempSketch = sketchSource_;

  QString red("255,0,0");
  QString orange("255,200,0");
  QString green("120,200,0");
  QString grey("230,230,230");

  bool bvalue;
  bool blocked;
  double dvalue;
  QString s;

  dvalue = model_->getPump1Hours();
  s = QString::number(dvalue, 'f', 1);
  tempSketch.replace("@S00@", s + " h");

  bvalue = model_->getPumpState1();
  blocked = model_->getPumpBlocked1();
  if (blocked) {
    tempSketch.replace("@S00BG@", grey);
  } else {
    if (bvalue) {
      tempSketch.replace("@S00BG@", green);
    } else {
      tempSketch.replace("@S00BG@", red);
    }
  }

  dvalue = model_->getPump2Hours();
  s = QString::number(dvalue, 'f', 1);
  tempSketch.replace("@S01@", s + " h");

  bvalue = model_->getPumpState2();
  blocked = model_->getPumpBlocked2();
  if (blocked) {
    tempSketch.replace("@S01BG@", grey);
  } else {
    if (bvalue) {
      tempSketch.replace("@S01BG@", green);
    } else {
      tempSketch.replace("@S01BG@", red);
    }
  }

  bvalue = model_->getValveState1();
  blocked = model_->getValveBlocked1();
  if (bvalue) {
    tempSketch.replace("@S02@", "open");
  } else {
    tempSketch.replace("@S02@", "closed");
  }
  if (blocked) {
    tempSketch.replace("@S02BG@", grey);
  } else {
    if (bvalue) {
      tempSketch.replace("@S02BG@", green);
    } else {
      tempSketch.replace("@S02BG@", red);
    }
  }

  bvalue = model_->getValveState2();
  blocked = model_->getValveBlocked2();
  if (bvalue) {
    tempSketch.replace("@S03@", "open");
  } else {
    tempSketch.replace("@S03@", "closed");
  }
  if (blocked) {
    tempSketch.replace("@S03BG@", grey);
  } else {
    if (bvalue) {
      tempSketch.replace("@S03BG@", green);
    } else {
      tempSketch.replace("@S03BG@", red);
    }
  }

  bvalue = model_->getValveState3();
  blocked = model_->getValveBlocked3();
  if (bvalue) {
    tempSketch.replace("@S04@", "open");
  } else {
    tempSketch.replace("@S04@", "closed");
  }
  if (blocked) {
    tempSketch.replace("@S04BG@", grey);
  } else {
    if (bvalue) {
      tempSketch.replace("@S04BG@", green);
    } else {
      tempSketch.replace("@S04BG@", red);
    }
  }

  dvalue = model_->getPressure1();
  if (dvalue<1) {
    s = QString::number(dvalue, 'e', 2);
  } else if (dvalue<100) {
    s = QString::number(dvalue, 'f', 1);
  } else {
    s = QString::number(dvalue, 'f', 0);
  }
  tempSketch.replace("@P00@", s + " mbar");

  if (dvalue<50) {
    tempSketch.replace("@P00BG@", green);
  } else if (dvalue<180) {
    tempSketch.replace("@P00BG@", orange);
  } else {
    tempSketch.replace("@P00BG@", red);
  }

  dvalue = model_->getPressure2();
  if (dvalue<1) {
    s = QString::number(dvalue, 'e', 2);
  } else if (dvalue<100) {
    s = QString::number(dvalue, 'f', 1);
  } else {
    s = QString::number(dvalue, 'f', 0);
  }
  tempSketch.replace("@P01@", s + " mbar");

  if (dvalue<50) {
    tempSketch.replace("@P01BG@", green);
  } else if (dvalue<180) {
    tempSketch.replace("@P01BG@", orange);
  } else {
    tempSketch.replace("@P01BG@", red);
  }

  dvalue = model_->getPressure3();
  if (dvalue<1) {
    s = QString::number(dvalue, 'e', 2);
  } else if (dvalue<100) {
    s = QString::number(dvalue, 'f', 1);
  } else {
    s = QString::number(dvalue, 'f', 0);
  }
  tempSketch.replace("@P02@", s + " mbar");

  if (dvalue<50) {
    tempSketch.replace("@P02BG@", green);
  } else if (dvalue<180) {
    tempSketch.replace("@P02BG@", orange);
  } else {
    tempSketch.replace("@P02BG@", red);
  }

  sketch_->load(tempSketch.toLocal8Bit());
}

void PumpStationMainWindow::buttonDoubleClicked(int button)
{
  qDebug() << "void PumpStationMainWindow::buttonDoubleClicked(int button) " << button;

  QString message;

  if (button==0) {
    if (model_->getPumpBlocked1()) return;
    if (model_->getPumpState1()) {
      message = "Do you really want to stop pump 1?";
    } else {
      message = "Do you really want to start pump 1?";
    }
  }
  if (button==1) {
    if (model_->getPumpBlocked2()) return;
    if (model_->getPumpState2()) {
      message = "Do you really want to stop pump 2?";
    } else {
      message = "Do you really want to start pump 2?";
    }
  }
  if (button==2) {
    if (model_->getValveBlocked1()) return;
    if (model_->getValveState1()) {
      message = "Do you really want to close valve 1?";
    } else {
      message = "Do you really want to open valve 1?";
    }
  }
  if (button==3) {
    if (model_->getValveBlocked2()) return;
    if (model_->getValveState2()) {
      message = "Do you really want to close valve 2?";
    } else {
      message = "Do you really want to open valve 2?";
    }
  }
  if (button==4) {
    if (model_->getValveBlocked3()) return;
    if (model_->getValveState3()) {
      message = "Do you really want to close valve 3?";
    } else {
      message = "Do you really want to open valve 3?";
    }
  }

  int result;
  QString pin = "";
  if (locked_==false) {
    PumpStationDialog dialog(message, this);
    result = dialog.exec();
  } else {
    PumpStationPINDialog dialog(message, this);
    result = dialog.exec();
    pin = dialog.getPIN();
  }

  if (result==0) return;

  if (pin_==pin) unlock();

  if (locked_) return;

  qDebug() << "YES " << button << " " << pin;

  QApplication::setOverrideCursor(Qt::WaitCursor);
  sketch_->setEnabled(false);

  emit toggleSwitch(button);
}

void PumpStationMainWindow::lock()
{
  locked_ = true;
}

void PumpStationMainWindow::unlock()
{
  locked_ = false;
  QTimer::singleShot(20*1000, this, SLOT(lock()));
}

void PumpStationMainWindow::enableWidgets()
{
  sketch_->setEnabled(true);
  QApplication::restoreOverrideCursor();
}
