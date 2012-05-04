
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
#include "devices/Julabo/JulaboFP50.h" // for temperature soft thresholds


#define _SCHEDULE_NROWS 20
#define _SCHEDULE_NCOLUMNS 3

///
/// class for storing and polling defo schedules
///
class DefoSchedule : public QObject {

  Q_OBJECT
  Q_ENUMS( actions )

 public:

  typedef std::pair<int,QString> scheduleItem;

  // those action which require a parameter must also be pushed on the
  // actionItemsRequiringParameter_
  // has to be done in a more convenient way later
  enum actions { INVALID, SET, REF, DEFO, FILE_SET, FILE_REF, FILE_DEFO, TEMP, SLEEP, CALIB, GOTO, END };

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
  void validate( void );


 signals:

  void newAction( DefoSchedule::scheduleItem item );
  void unableToDeliverAction( void );
  void newRow( int item );

 private:

  int checkRowValidity( unsigned int );
  void issueMissingFileError( int, QString, QString, QString );
  void issueGotoInvalidLineError( int, QString, int );
  void issueGotoPointsToItselfError( int, QString );
  void issueCameraNotEnabledError( int, QString, QString );
  void issueBogusTemperatureError( int, QString );
  void issueExcessTemperatureError( int, QString );
  QStandardItemModel model_;
  std::map<std::string,int> actionItems_;
  std::vector<int> actionItemsRequiringParameter_;
  unsigned int currentIndex_; // points to current row
  unsigned int debugLevel_;  

};

#endif
