#ifndef IOTAWIDGET_H
#define IOTAWIDGET_H

#include <QCheckBox>
#include <QDoubleSpinBox>
#include <QFormLayout>
#include <QHBoxLayout>
#include <QLCDNumber>
#include <QSpinBox>
#include <QVBoxLayout>
#include <QWidget>

#include "IotaModel.h"
#include "DeviceState.h"

/** @addtogroup common
 *  @{
 */

/** @addtogroup widgets
 *  @{
 */

class IotaWidget : public QWidget
{
  Q_OBJECT
public:
  explicit IotaWidget(IotaModel* model,
                      QWidget *parent = 0);

protected:
  const static int LCD_SIZE = 5;

  IotaModel* model_;

  QCheckBox* iotaCheckBox_;

  QWidget* operationPanel_;

  QCheckBox* pumpCheckBox_;
  QLCDNumber* aPressureLCD_;
  QLCDNumber* aFlowLCD_;
  QDoubleSpinBox* sPressureSpinner_;
  QDoubleSpinBox* sFlowSpinner_;

  public slots:
  void updateDeviceState( State newState );
  void controlStateChanged(bool);
  void updateInfo();
};

/** @} */

/** @} */

#endif // IOTAWIDGET_H
