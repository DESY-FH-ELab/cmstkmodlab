#include <iostream>
#include <cmath>

#include <QFile>
#include <QXmlStreamWriter>

#include "DefoPointIndexerModel.h"
#include "DefoPointIndexer.h"

DefoPointIndexerModel::DefoPointIndexerModel(QObject *parent) :
    QObject(parent)
{
  DefoVPointIndexer *indexer;
  indexer = new DefoPointIndexer(this);
  indexers_.push_back(indexer);
  indexerNames_[indexer] = "Sorting Indexer";
}

const std::string DefoPointIndexerModel::getIndexerName(const DefoVPointIndexer* indexer) const {
  NameMap::const_iterator it = indexerNames_.find(indexer);
  if (it!=indexerNames_.end()) return it->second;
  return std::string();
}

DefoVPointIndexer* DefoPointIndexerModel::getSelectedIndexer() const {
  return selectedIndexer_;
}

void DefoPointIndexerModel::setSelectedIndexer(DefoVPointIndexer* indexer) {
  selectedIndexer_ = indexer;
}
