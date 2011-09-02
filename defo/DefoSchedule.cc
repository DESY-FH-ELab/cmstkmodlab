
#include "DefoSchedule.h"

///
///
///
DefoSchedule::DefoSchedule() {
  
  // fixed size table
  model_.setRowCount( _SCHEDULE_NROWS );
  model_.setColumnCount( _SCHEDULE_NCOLUMNS );

  // set headers
  QStringList headerStrings;
  headerStrings << "ACTION" << "VALUE" << "#";
  model_.setHorizontalHeaderLabels( headerStrings );

  // fill actions map
  int enumIndex = metaObject()->indexOfEnumerator( "actions" );
  QMetaEnum metaEnum = metaObject()->enumerator( enumIndex );
  for( int i = 0; i < metaEnum.keyCount(); ++i ) {
    actionItems_.insert( std::pair< std::string,int>( std::string( metaEnum.key( i ) ), i ) );
  }

  // which of these action require a parameter?
  actionItemsRequiringParameter_.push_back( DefoSchedule::FILE_REF );
  actionItemsRequiringParameter_.push_back( DefoSchedule::FILE_DEFO );
  actionItemsRequiringParameter_.push_back( DefoSchedule::TEMP );
  actionItemsRequiringParameter_.push_back( DefoSchedule::SLEEP );
  actionItemsRequiringParameter_.push_back( DefoSchedule::GOTO );

}



///
///
///
void DefoSchedule::pollAction( void ) {

  int currentAction = DefoSchedule::INVALID;

  // to catch endless loops
  int repeatCounter = 0;

  // loop for catching GOTO actions
  do {
    
    repeatCounter++;

    // check if we have something useful...
    if( DefoSchedule::GOOD_ROW != checkRowValidity() ) {
      emit unableToDeliverAction(); // we have an invalid row here or the end of the table
      return;
    }
    
    // get item with current index from table
    QStandardItem* item0 = model_.item( currentIndex_, 0 );
    
    // check if we have it in the list;
    // INVALID must be caught (just in case)
    std::map<std::string,int>::iterator it = actionItems_.find( item0->text().toStdString() );
    if( actionItems_.end() == it || DefoSchedule::INVALID == it->second ) {
      std::cerr << " [DefoSchedule::pollAction] ** ERROR: unsupported action: \"" << item0->text().toStdString() 
		<< "\" row:" << currentIndex_+1 << std::endl;
      emit unableToDeliverAction();
      return;
    }

    currentAction = it->second;

    // if it's a goto, we have to jump
    if( DefoSchedule::GOTO == currentAction ) {
      bool isOk = false;
      currentIndex_ = model_.item( currentIndex_, 1 )->text().toUInt( &isOk, 10 ) - 1; // offset since table starts at 1
      if( !isOk ) {
	std::cerr << " [DefoSchedule::pollAction] ** ERROR: GOTO has invalid line address: "
		  << model_.item( currentIndex_, 1 )->text().toStdString() << " row: " << currentIndex_ + 1 << std::endl;
	emit unableToDeliverAction();
	return;
      }
    }

    else break; // no GOTO

  } while( repeatCounter <= 10000 );

  if( 10000 <= repeatCounter ) {
    std::cerr << " [DefoSchedule::pollAction] ** ERROR: Bailing out of endless GOTO loop row: " << currentIndex_ + 1 << std::endl;
    emit unableToDeliverAction();
    return;
  }

  // emit and increment counter
  else {

    // with parameter?
    QString parameter( "" );
    if( model_.item( currentIndex_, 1 ) ) parameter = model_.item( currentIndex_, 1 )->text();

    std::cout << " [DefoSchedule::pollAction] =2= emitting: " << currentAction << ", \"" 
	      << model_.item( currentIndex_, 0 )->text().toStdString() << "\"" << std::endl;

    emit newRow( currentIndex_ );

    ++currentIndex_;

    emit newAction( scheduleItem( currentAction, parameter ) );

  }
  
}



///
/// check for invalid or empty items in row,
/// return one of: DefoSchedule::rowStates
///
int DefoSchedule::checkRowValidity( void ) {

  QStandardItem* item0 = model_.item( currentIndex_, 0 );
  QStandardItem* item1 = model_.item( currentIndex_, 1 );

  // if we have an action
  if( item0 ) {

    // get action value
    std::map<std::string,int>::iterator it = actionItems_.find( item0->text().toStdString() );
    if( actionItems_.end() == it ) {
      std::cerr << " [DefoSchedule::checkRowValidity] -- Return BAD_ROW since unknown action: \"" 
		<< item0->text().toStdString() << "\" row: " << currentIndex_ + 1 << std::endl;
      return DefoSchedule::BAD_ROW; // unknown item
    }

    // it's in the list, check if it requires a parameter
    std::vector<int>::const_iterator itP = std::find( actionItemsRequiringParameter_.begin(), 
						      actionItemsRequiringParameter_.end(),
						      it->second );

    if( !(actionItemsRequiringParameter_.end() == itP) ) { // needs a parameter
      if( !item1 ) {
	std::cerr << " [DefoSchedule::checkRowValidity] -- Return BAD_ROW since missing parameter for action: \"" 
		  << item0->text().toStdString() << "\" row: " << currentIndex_ + 1 << std::endl;
	return DefoSchedule::BAD_ROW; // no parameter given
      }	
    }

    // needs no parameter
    return DefoSchedule::GOOD_ROW;

  }

  else { // no action given
    if( item1 ) {
      std::cerr << " [DefoSchedule::checkRowValidity] -- Return BAD_ROW since no action but parameter: \"" 
		<< item1->text().toStdString() << "\" row: " << currentIndex_ + 1 << std::endl;
      return DefoSchedule::BAD_ROW; // if there's a parameter, obviously something's wrong
    }

    std::cerr << " [DefoSchedule::checkRowValidity] -- Return EMPTY_ROW for row: " << currentIndex_ + 1 << std::endl;
    return DefoSchedule::EMPTY_ROW; // otherwise it's simply an empty row

  }

  // if we get here, there's a quirk in this logic...
  std::cerr << " [DefoSchedule::checkRowValidity] ** ERROR: should not be here (row: " << currentIndex_ + 1 << ")" << std::endl;
  throw;
  
}



///
/// empty the table
///
void DefoSchedule::clear( void ) {

  for( unsigned int row = 0; row < _SCHEDULE_NROWS; ++row ) {
    for( unsigned int column = 0; column < _SCHEDULE_NCOLUMNS; ++column ) {

      QStandardItem* item = model_.item( row, column );
      if( item ) item->setText("");

    }
  }

}


///
///
///
void DefoSchedule::loadFromFile( void ) {

  QFileDialog::Options options;

  QString selectedFilter;
  QString fileName = QFileDialog::getOpenFileName( 0,
						   QString("Schedule file name"),
						   QString("schedules"),
						   QString("Schedule Files (*.defoschedule)"),
						   &selectedFilter,
						   options );

  if( fileName.isEmpty() ) return;

  std::ifstream file( fileName.toStdString().c_str() );

  if( file.bad() ) {
    std::cerr << " [DefoSchedule::loadFromFile] ** ERROR: cannot open file: " << fileName.toStdString() << std::endl;
    QMessageBox::critical( 0, tr("[DefoSchedule::loadFromFile]"), QString("File \'%1\' cannot be opened.").arg( fileName ), QMessageBox::Ok );
    return;
  }

  clear();
  std::string buf0, buf1;
  unsigned int rowCounter = 0;

  while( !file.eof() ) {

    file >> buf0 >> buf1;
    if( file.eof() ) break; // avoid last row double reading

    // we create a small leak here, but dont care..
    QStandardItem* item0 = new QStandardItem( QString( buf0.c_str() ) );
    QStandardItem* item1 = new QStandardItem( QString( buf1.c_str() ) );

    model_.setItem( rowCounter, 0, item0 );
    model_.setItem( rowCounter, 1, item1 );

    ++rowCounter;

  } 

  file.close();

}



///
///
///
void DefoSchedule::saveToFile( void ) {

  QFileDialog::Options options;

  QString selectedFilter;
  QString fileName = QFileDialog::getSaveFileName( 0,
						   QString("Schedule file name"),
						   QString("schedules"),
						   QString("Schedule Files (*.defoschedule)"),
						   &selectedFilter,
						   options );
  if( fileName.isEmpty() ) return;

  std::ofstream file( fileName.toStdString().c_str() );

  if( file.bad() ) {
    std::cerr << " [DefoSchedule::saveToFile] ** ERROR: cannot open file: " << fileName.toStdString() << std::endl;
    QMessageBox::critical( 0, tr("[DefoSchedule::saveToFile]"), QString("File \'%1\' cannot be opened.").arg( fileName ), QMessageBox::Ok );
    return;
  }


  for( int row = 0; row < model_.rowCount(); ++row ) {

    QStandardItem* item0 = model_.item( row, 0 );
    QStandardItem* item1 = model_.item( row, 1 );
    
    if( !item0 ) break; // if empty action then we should be done
    file << item0->text().toStdString() << "\t" << item1->text().toStdString();
    
    file << std::endl;

  }
  

  file.close();
  
}
