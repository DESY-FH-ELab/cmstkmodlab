#ifndef DEFOCONRADWIDGET_H
#define DEFOCONRADWIDGET_H

#include <QWidget>
#include "DefoConradModel.h"
#include "DefoConradCheckbox.h"
#include "DefoConradPanelWidget.h"

class DefoConradWidget : public QWidget
{
    Q_OBJECT

public:

  explicit DefoConradWidget(DefoConradModel* model,
			    QWidget *parent = 0);
};

#endif // DEFOCONRADWIDGET_H
