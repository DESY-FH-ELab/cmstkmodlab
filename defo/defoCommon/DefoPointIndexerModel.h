#ifndef DEFOPOINTINDEXERMODEL_H
#define DEFOPOINTINDEXERMODEL_H

#include <vector>
#include <map>

#include <QObject>

#include "DefoVPointIndexer.h"

class DefoPointIndexerModel : public QObject
{
    Q_OBJECT
public:
  explicit DefoPointIndexerModel(QObject *parent = 0);

  int getIndexerCount() const { return indexers_.size(); }
  DefoVPointIndexer* getIndexer(int i) const { return indexers_[i]; }
  const std::string getIndexerName(const DefoVPointIndexer*) const;
  DefoVPointIndexer* getSelectedIndexer() const;
  void setSelectedIndexer(DefoVPointIndexer*);

protected:

  std::vector<DefoVPointIndexer*> indexers_;
  typedef std::map<const DefoVPointIndexer *, std::string> NameMap;
  NameMap indexerNames_;

  DefoVPointIndexer* selectedIndexer_;

signals:
  void pointIndexerChanged(DefoVPointIndexer *);
};

#endif // DEFOALIGNMENTMODEL_H
