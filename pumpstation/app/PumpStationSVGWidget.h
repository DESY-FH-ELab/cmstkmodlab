/////////////////////////////////////////////////////////////////////////////////
//                                                                             //
//               Copyright (C) 2011-2025 - The DESY CMS Group                  //
//                           All rights reserved                               //
//                                                                             //
//      The CMStkModLab source code is licensed under the GNU GPL v3.0.        //
//      You have the right to modify and/or redistribute this source code      //
//      under the terms specified in the license, which may be found online    //
//      at http://www.gnu.org/licenses or at License.txt.                      //
//                                                                             //
/////////////////////////////////////////////////////////////////////////////////

#ifndef PUMPSTATIONSVGWIDGET_H
#define PUMPSTATIONSVGWIDGET_H

#include <QWidget>
#include <QSvgWidget>
#include <QResizeEvent>
#include <QMouseEvent>
#include <QRect>

class PumpStationSVGWidget : public QSvgWidget
{
    Q_OBJECT
public:

    PumpStationSVGWidget(QWidget* parent = 0);
    virtual int heightForWidth(int) const;
    virtual int widthForHeight(int) const;

protected:

    virtual void resizeEvent(QResizeEvent *event);
    virtual void mouseDoubleClickEvent(QMouseEvent *event);

signals:

    void buttonDoubleClicked(int);

private:

    QRect button0_0_;
    QRect button0_1_;
    QRect button1_0_;
    QRect button1_1_;
    QRect button2_0_;
    QRect button3_0_;
    QRect button4_0_;
};

#endif // DEFOGEOMETRYWIDGET_H
