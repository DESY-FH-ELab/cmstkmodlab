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
}
