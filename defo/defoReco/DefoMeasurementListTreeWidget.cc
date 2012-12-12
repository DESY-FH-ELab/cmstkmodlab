#include "DefoMeasurementListTreeWidget.h"

DefoMeasurementListTreeWidgetItem::DefoMeasurementListTreeWidgetItem(
      DefoMeasurement* measurement
    , QTreeWidget *parent
) :
    QTreeWidgetItem(parent)
  , measurement_(measurement)
{

}

DefoMeasurementListTreeWidget::DefoMeasurementListTreeWidget(
     DefoMeasurementListModel* listModel
   , DefoMeasurementSelectionModel* selectionModel
   , QWidget *parent
) :
    QTreeWidget(parent)
  , listModel_(listModel)
  , selectionModel_(selectionModel)
{
  QTreeWidgetItem * headerItem = new QTreeWidgetItem();
  headerItem->setText(0, QString("Idx"));
  headerItem->setText(1, QString("Timestamp"));
  setHeaderItem(headerItem);
  
  // Copy values
  connect(listModel_
	, SIGNAL(measurementCountChanged(int))
	, this
        , SLOT(fillTree(int))
         );

  // Respond to selection changes
  connect(
  	  selectionModel_
        , SIGNAL(selectionChanged(DefoMeasurement*))
        , this
        , SLOT(setSelection(DefoMeasurement*))
  	  );
  
  connect(this
  	, SIGNAL(itemSelectionChanged())
        , this
        , SLOT(selectionChanged())
  	  );

  // Fill box and set selection
  fillTree(listModel_->getMeasurementCount());
}

const QString DefoMeasurementListTreeWidget::TIME_FORMAT = "yyyy-MM-dd hh:mm:ss";

void DefoMeasurementListTreeWidget::fillTree(int count) {

  setEnabled(count > 0);

  // // FIXME this sets the selection to NULL
  clear();

  // // Empty map
  indexMap_.clear();

  DefoMeasurement* measurement;

  // Fill it up and add to view
  for (int i = 0; i < count; ++i) {
    measurement = listModel_->getMeasurement(i);

    DefoMeasurementListTreeWidgetItem * item = 
      new DefoMeasurementListTreeWidgetItem(measurement, this);
    indexMap_[measurement] = item;

    item->setText(0, QString().setNum(i+1));
    item->setText(1, measurement->getTimeStamp().toString(TIME_FORMAT));

    addTopLevelItem(item);
  }

  if (count>0)
    setSelection(listModel_->getMeasurement(0));
}

void DefoMeasurementListTreeWidget::setSelection(
    DefoMeasurement *selection
) {
  MeasurementMap::const_iterator it = indexMap_.find(selection);
  
  // assure it was actually found
  if (it != indexMap_.end())
    setCurrentItem( it->second );
  else
    setCurrentItem( 0 );
}

void DefoMeasurementListTreeWidget::selectionChanged() {

  QList<QTreeWidgetItem*> items = selectedItems();
  if (items.count()==1) {
    DefoMeasurementListTreeWidgetItem* item = dynamic_cast<DefoMeasurementListTreeWidgetItem*>(items.first());
    DefoMeasurement* measurement = item->getMeasurement();
    selectionModel_->setSelection(measurement);
  } else {
    selectionModel_->setSelection(NULL);
  }
}
