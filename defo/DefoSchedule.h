
#ifndef __DEFOSCHEDULE_H
#define __DEFOSCHEDULE_H

#include <iostream>
#include <fstream>
#include <map>
#include <utility>
#include <vector>
#include <algorithm>

#include <QObject>
#include <QStandardItemModel>
#include <QStringList>
#include <QMetaEnum>
#include <QFileDialog>
#include <QMessageBox>

#include "DefoConfigReader.h"



#define _SCHEDULE_NROWS 20
#define _SCHEDULE_NCOLUMNS 3

///
///
///
class DefoSchedule : public QObject {

  Q_OBJECT
  Q_ENUMS( actions )

 public:

  typedef std::pair<int,QString> scheduleItem;

  // those action which require a parameter must also be pushed on the
  // actionItemsRequiringParameter_
  // has to be done in a more convenient way later
  enum actions { INVALID, SET, REF, DEFO, FILE_SET, FILE_REF, FILE_DEFO, TEMP, SLEEP, GOTO, END };

  enum rowStates { GOOD_ROW, EMPTY_ROW, BAD_ROW };

  DefoSchedule();
  QStandardItemModel* getModel( void ) { return &model_; }
  unsigned int getCurrentIndex( void ) { return currentIndex_; }
  void setCurrentIndex( unsigned int index ) { currentIndex_ = index; }

 public slots:

  void pollAction( void );
  void setIndex( unsigned int index ) { currentIndex_ = index; }
  void clear( void );
  void loadFromFile( void );
  void saveToFile( void );

 signals:

  void newAction( DefoSchedule::scheduleItem item );
  void unableToDeliverAction( void );
  void newRow( int item );

 private:

  int checkRowValidity( void );

  QStandardItemModel model_;
  std::map<std::string,int> actionItems_;
  std::vector<int> actionItemsRequiringParameter_;
  unsigned int currentIndex_; // points to current row
  unsigned int debugLevel_;

};

#endif
