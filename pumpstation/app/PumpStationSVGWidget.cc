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
  button0_0_ = QRect( 45,  32, 110, 48);
  button0_1_ = QRect(125, 110,  80, 80);

  button1_0_ = QRect( 45, 232, 110, 48);
  button1_1_ = QRect(125, 310,  80, 80);

  button2_0_ = QRect(287,  98,  50, 70);

  button3_0_ = QRect(287, 300,  50, 70);

  button4_0_ = QRect(654, 198,  50, 70);

  setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding));
  sizePolicy().setHeightForWidth(true);
}

int PumpStationSVGWidget::heightForWidth(int w) const
{
  return w*400/800;
}

int PumpStationSVGWidget::widthForHeight(int h) const
{
  return h*400/370;
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

  } else if (button3_0_.contains(x, y)) {
    emit buttonDoubleClicked(3);

  } else if (button4_0_.contains(x, y)) {
    emit buttonDoubleClicked(4);

  }
}

