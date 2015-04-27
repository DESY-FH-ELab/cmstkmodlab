#ifndef DEFOCONRADPANELWIDGET_H
#define DEFOCONRADPANELWIDGET_H

#include <QWidget>
#include "DefoConradModel.h"
#include "DefoConradButton.h"

class DefoConradPanelWidget : public QWidget
{
    Q_OBJECT

public:

  explicit DefoConradPanelWidget(DefoConradModel* model,
				 QWidget *parent = 0);
};

#endif // DEFOCONRADPANELWIDGET_H
