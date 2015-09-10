#ifndef DEFOLENSCOMBOBOX_H
#define DEFOLENSCOMBOBOX_H

#include <QComboBox>

#include "DefoLensModel.h"

class DefoLensComboBox : public QComboBox
{
    Q_OBJECT
public:
    explicit DefoLensComboBox(DefoLensModel* model,
			      QWidget *parent = 0);

protected:
    
  DefoLensModel * model_;
  
protected slots:
  
  void lensChanged();

signals:

};

#endif // DEFOLENSCOMBOBOX_H
