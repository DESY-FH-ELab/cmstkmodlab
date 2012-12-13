#include "DefoPointIndexerListComboBox.h"

DefoPointIndexerListComboBox::DefoPointIndexerListComboBox(
     DefoPointIndexerModel* model
   , QWidget *parent
) :
    QComboBox(parent)
  , model_(model)
{
  // No user editable text
  setEditable(false);

  // Respond to selection changes
  connect(
        model_
      , SIGNAL(indexerChanged(DefoVPointIndexer*))
      , this
      , SLOT(setSelection(DefoVPointIndexer*))
  );
  connect(
        this
      , SIGNAL(currentIndexChanged(int))
      , this
      , SLOT(selectionChanged(int))
  );

  fillOptions(model_->getIndexerCount());
}

const QString DefoPointIndexerListComboBox::LABEL_FORMAT = "%1. %2";

void DefoPointIndexerListComboBox::fillOptions(int count) {

  setEnabled(count > 0);

  // FIXME this sets the selection to NULL
  clear();

  const DefoVPointIndexer* indexer;

  // Fill it up and add to view
  for (int i = 0; i < count; ++i) {
    indexer = model_->getIndexer(i);
    indexMap_[indexer] = i;
    addItem(
        LABEL_FORMAT
          .arg(i+1)
          .arg(model_->getIndexerName(indexer).c_str())
    );
  }

  setSelection(model_->getSelectedIndexer());
}

void DefoPointIndexerListComboBox::setSelection(
     DefoVPointIndexer* indexer
) {

  IndexerMap::const_iterator it = indexMap_.find(indexer);

  // assure it was actually found
  if (it != indexMap_.end())
    setCurrentIndex( it->second );
  else
    setCurrentIndex( -1 );
}

void DefoPointIndexerListComboBox::selectionChanged(int index) {

  // Set NULL on no selection, or measurement on valid selection
  if ( index == -1 )
    model_->setSelectedIndexer(NULL);
  else
  model_->setSelectedIndexer(model_->getIndexer(index));
}
