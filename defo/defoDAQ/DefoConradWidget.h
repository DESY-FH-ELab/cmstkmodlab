#ifndef DEFOCONRADWIDGET_H
#define DEFOCONRADWIDGET_H

#include <QWidget>
#include "DefoConradModel.h"
#include "DefoConradCheckbox.h"
#include "DefoConradButton.h"

class DefoConradWidget : public QWidget
{
    Q_OBJECT
public:
  explicit DefoConradWidget(DefoConradModel* model, QWidget *parent = 0);

//protected:
//  DefoConradModel model_;

signals:

public slots:
};

#endif // DEFOCONRADWIDGET_H
