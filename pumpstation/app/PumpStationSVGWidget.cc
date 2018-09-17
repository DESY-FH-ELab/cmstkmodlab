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

#include "PumpStationSVGWidget.h"

PumpStationSVGWidget::PumpStationSVGWidget(QWidget* parent)
  : QSvgWidget(parent)
{
  button0_0_ = QRect(550,  44, 110, 32);
  button0_1_ = QRect(620, 168,  90, 90);

  button1_0_ = QRect(550,   4, 110, 32);
  button1_1_ = QRect(620, 280,  90, 90);

  button2_0_ = QRect(220,  10,  70, 50);
  button2_1_ = QRect(496, 155,  50, 80);

  button3_0_ = QRect(130,  10,  70, 50);
  button3_1_ = QRect(496, 270,  50, 80);

  button4_0_ = QRect( 40,  10,  70, 50);
  button4_1_ = QRect( 91, 215,  50, 80);

  setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding));
  sizePolicy().setHeightForWidth(true);
}

int PumpStationSVGWidget::heightForWidth(int w) const
{
  return w*370/800;
}

int PumpStationSVGWidget::widthForHeight(int h) const
{
  return h*800/370;
}

void PumpStationSVGWidget::resizeEvent(QResizeEvent *event)
{
  QSize s = event->size();
  int h = heightForWidth(s.width());
  if (h<=s.height()) {
    resize(s.width(), h);
  } else {
    int w = widthForHeight(s.height());
    resize(w, s.height());
  }
}

void PumpStationSVGWidget::mouseDoubleClickEvent(QMouseEvent *event)
{
  double scale = 800./width();
  int x = scale * event->x();
  int y = scale * event->y();

  if (!isEnabled()) return;

  if (button0_0_.contains(x, y)) {
    emit buttonDoubleClicked(0);

  } else if (button0_1_.contains(x, y)) {
    emit buttonDoubleClicked(0);

  } else if (button1_0_.contains(x, y)) {
    emit buttonDoubleClicked(1);

  } else if (button1_1_.contains(x, y)) {
    emit buttonDoubleClicked(1);

  } else if (button2_0_.contains(x, y)) {
    emit buttonDoubleClicked(2);

  } else if (button2_1_.contains(x, y)) {
    emit buttonDoubleClicked(2);

  } else if (button3_0_.contains(x, y)) {
    emit buttonDoubleClicked(3);

  } else if (button3_1_.contains(x, y)) {
    emit buttonDoubleClicked(3);

  } else if (button4_0_.contains(x, y)) {
    emit buttonDoubleClicked(4);

  } else if (button4_1_.contains(x, y)) {
    emit buttonDoubleClicked(4);

  }
}

