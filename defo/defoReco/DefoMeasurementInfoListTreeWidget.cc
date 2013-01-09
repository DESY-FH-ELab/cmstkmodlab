#include "DefoMeasurementInfoListTreeWidget.h"

DefoMeasurementInfoListTreeWidget::DefoMeasurementInfoListTreeWidget(
     DefoMeasurementSelectionModel* selectionModel
   , QWidget *parent
) :
    QTreeWidget(parent)
  , selectionModel_(selectionModel)
{
  setMinimumSize(QSize(300, 300));

  QTreeWidgetItem * headerItem = new QTreeWidgetItem();
  headerItem->setText(0, QString("Key"));
  headerItem->setText(1, QString("Value"));
  setHeaderItem(headerItem);
  
  // Respond to selection changes
  connect(
  	  selectionModel_
        , SIGNAL(selectionChanged(DefoMeasurement*))
        , this
        , SLOT(setSelection(DefoMeasurement*))
  	  );
}

const QString DefoMeasurementInfoListTreeWidget::TIME_FORMAT = "yyyy-MM-dd hh:mm:ss";

void DefoMeasurementInfoListTreeWidget::setSelection(
    DefoMeasurement *measurement
) {

  if (!measurement) return;

  clear();

  QTreeWidgetItem * item;

  item = new QTreeWidgetItem(this);
  item->setText(0, "timestamp");
  item->setText(1, measurement->getTimeStamp().toString(TIME_FORMAT));
  addTopLevelItem(item);

  item = new QTreeWidgetItem(this);
  item->setText(0, "focal length");
  item->setText(1, QString("%1 mm").arg(measurement->getFocalLength()));
  addTopLevelItem(item);

  item = new QTreeWidgetItem(this);
  item->setText(0, "exposure time");
  item->setText(1, QString("%1 s").arg(measurement->getExposureTime()));
  addTopLevelItem(item);

  item = new QTreeWidgetItem(this);
  item->setText(0, "exposure time string");
  item->setText(1, measurement->getExposureTimeString());
  addTopLevelItem(item);

  item = new QTreeWidgetItem(this);
  item->setText(0, "aperture");
  item->setText(1, QString("%1").arg(measurement->getAperture()));
  addTopLevelItem(item);

  item = new QTreeWidgetItem(this);
  item->setText(0, "ISO");
  item->setText(1, QString("%1").arg(measurement->getISO()));
  addTopLevelItem(item);

  QImage image = measurement->getImage();
  item = new QTreeWidgetItem(this);
  item->setText(0, "width");
  item->setText(1, QString("%1 pixel").arg(image.width()));
  addTopLevelItem(item);
  item = new QTreeWidgetItem(this);
  item->setText(0, "height");
  item->setText(1, QString("%1 pixel").arg(image.height()));
  addTopLevelItem(item);

  QTreeWidgetItem * conrad = new QTreeWidgetItem(this);
  conrad->setText(0, "conrad");
  conrad->setText(1, measurement->getConradState()==READY ? "On" : "Off");
  addTopLevelItem(conrad);

  QString panelState = "";
  for (int p=0;p<5;++p) {
      panelState += measurement->getPanelState(p)==READY ? "1" : "0";
  }
  item = new QTreeWidgetItem(conrad);
  item->setText(0, "panels");
  item->setText(1, panelState);
  conrad->addChild(item);

  item = new QTreeWidgetItem(conrad);
  item->setText(0, "LEDs");
  item->setText(1, measurement->getLEDState()==READY ? "On" : "Off");
  conrad->addChild(item);

  if (measurement->getConradState()==READY) {
      expandItem(conrad);
  } else {
      collapseItem(conrad);
  }

  QTreeWidgetItem *julabo = new QTreeWidgetItem(this);
  julabo->setText(0, "julabo");
  julabo->setText(1, measurement->getJulaboState()==READY ? "On" : "Off");
  addTopLevelItem(julabo);

  item = new QTreeWidgetItem(julabo);
  item->setText(0, "circulator");
  item->setText(1, measurement->getCirculatorState()==true ? "On" : "Off");
  julabo->addChild(item);

  item = new QTreeWidgetItem(julabo);
  item->setText(0, "bath");
  item->setText(1, measurement->getJulaboState()==READY ? QString("%1 °C").arg(measurement->getBathTemperature()) : "Off");
  julabo->addChild(item);

  if (measurement->getJulaboState()==READY) {
      expandItem(julabo);
  } else {
      collapseItem(julabo);
  }

  QTreeWidgetItem * keithley = new QTreeWidgetItem(this);
  keithley->setText(0, "keithley");
  keithley->setText(1, measurement->getKeithleyState()==READY ? "On" : "Off");
  addTopLevelItem(keithley);

  QString sensorState = "";
  for (int s=0;s<10;++s) {
      sensorState += measurement->getTemperatureSensorState(s)==READY ? "1" : "0";
  }
  item = new QTreeWidgetItem(keithley);
  item->setText(0, "sensors");
  item->setText(1, sensorState);
  keithley->addChild(item);

  for (int s=0;s<10;++s) {
      item = new QTreeWidgetItem(keithley);
      item->setText(0, QString("T%1").arg(s+1));
      item->setText(1, measurement->getKeithleyState()==READY && measurement->getTemperatureSensorState(s)==READY ?
                        QString("%1 °C").arg(measurement->getTemperature(s)) : "Off");
      keithley->addChild(item);
  }

  if (measurement->getKeithleyState()==READY) {
      expandItem(keithley);
  } else {
      collapseItem(keithley);
  }

}
