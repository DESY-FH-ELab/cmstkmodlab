
#include "DefoSchedule.h"

///
///
///
DefoSchedule::DefoSchedule() {

  // read parameters
  DefoConfigReader cfgReader( "defo.cfg" );
  debugLevel_ = cfgReader.getValue<unsigned int>( "DEBUG_LEVEL" );

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
  actionItemsRequiringParameter_.push_back( DefoSchedule::FILE_SET );
  actionItemsRequiringParameter_.push_back( DefoSchedule::FILE_REF );
  actionItemsRequiringParameter_.push_back( DefoSchedule::FILE_DEFO );
  actionItemsRequiringParameter_.push_back( DefoSchedule::TEMP );
  actionItemsRequiringParameter_.push_back( DefoSchedule::SLEEP );
  actionItemsRequiringParameter_.push_back( DefoSchedule::GOTO );

}



///
/// get next action in table and send it to the caller
/// via an emit newAction()
///
void DefoSchedule::pollAction( void ) {

  int currentAction = DefoSchedule::INVALID;

  // to catch endless loops
  int repeatCounter = 0;

  // loop for catching GOTO actions
  do {
    
    repeatCounter++;

    // check if we have something useful...
    if( DefoSchedule::GOOD_ROW != checkRowValidity( currentIndex_ ) ) {
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

    if( debugLevel_ >= 2 ) std::cout << " [DefoSchedule::pollAction] =2= emitting: " << currentAction << ", \"" 
	      << model_.item( currentIndex_, 0 )->text().toStdString() << "\"" << std::endl;

    emit newRow( currentIndex_ );

    ++currentIndex_;

    emit newAction( scheduleItem( currentAction, parameter ) );

  }
  
}



///
/// check for invalid or empty items in a single row (index "row"),
/// return one of: DefoSchedule::rowStates
///
int DefoSchedule::checkRowValidity( unsigned int index ) {

  QStandardItem* item0 = model_.item( index, 0 );
  QStandardItem* item1 = model_.item( index, 1 );

  // if we have an action
  if( item0 ) {

    // get action value
    std::map<std::string,int>::iterator it = actionItems_.find( item0->text().toStdString() );
    if( actionItems_.end() == it ) {
      std::cerr << " [DefoSchedule::checkRowValidity] WARNING ** Return BAD_ROW since unknown action: \"" 
		<< item0->text().toStdString() << "\" row: " << index + 1 << ". Fix your schedule." << std::endl;
      return DefoSchedule::BAD_ROW; // unknown item
    }

    // it's in the list, check if it requires a parameter
    std::vector<int>::const_iterator itP = std::find( actionItemsRequiringParameter_.begin(), 
						      actionItemsRequiringParameter_.end(),
						      it->second );

    if( !(actionItemsRequiringParameter_.end() == itP) ) { // needs a parameter
      if( !item1 ) {
	std::cerr << " [DefoSchedule::checkRowValidity] WARNING ** Return BAD_ROW since missing parameter for action: \"" 
		  << item0->text().toStdString() << "\" row: " << index + 1 << ". Fix your schedule." << std::endl;
	return DefoSchedule::BAD_ROW; // no parameter given
      }	
    }

    // needs no parameter
    return DefoSchedule::GOOD_ROW;

  }

  else { // no action given
    if( item1 ) {
      std::cerr << " [DefoSchedule::checkRowValidity] WARNING ** Return BAD_ROW since no action but parameter: \"" 
		<< item1->text().toStdString() << "\" row: " << index + 1 << ". Fix your schedule." << std::endl;
      return DefoSchedule::BAD_ROW; // if there's a parameter, obviously something's wrong
    }

    if( debugLevel_ >= 3 ) std::cout << " [DefoSchedule::checkRowValidity] =3= Return EMPTY_ROW for row: " << index + 1 
	      << ". Fix your schedule." << std::endl;
    return DefoSchedule::EMPTY_ROW; // otherwise it's simply an empty row

  }

  // if we get here, there's a quirk in this logic...
  std::cerr << " [DefoSchedule::checkRowValidity] ** ERROR: Should not reach this point (row: " << index + 1 << ")" << std::endl;
  
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
/// load a schedule from a a text file
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

    QStandardItem* item1;
    if( !(std::string( "-" ) == buf1) ) item1 = new QStandardItem( QString( buf1.c_str() ) );
    else item1 = new QStandardItem( QString("") );

    model_.setItem( rowCounter, 0, item0 );
    model_.setItem( rowCounter, 1, item1 );

    ++rowCounter;

  } 

  file.close();

}



///
/// save schedule to a text file
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
    
    if( !item0 ) break; // if empty action then we're done
    file << item0->text().toStdString() << "\t";

    if( item1->text().size() ) file << item1->text().toStdString();
    else file << "-";
    
    file << std::endl;

  }
  

  file.close();
  
}



///
/// make sure (as good as possible) that the whole schedule is executable:
/// check validity, existence of input files, goto lines exist, camera enabled, etc.
///
void DefoSchedule::validate( void ) {

  // determine number of filled rows in model,
  // until an END action or an empty row appears
  unsigned int nFilledRows = 0;
  for( int row = 0; row < model_.rowCount(); ++row ) {

    if( DefoSchedule::EMPTY_ROW == checkRowValidity( row ) ) break;

    QStandardItem* item0 = model_.item( row, 0 );
    std::map<std::string,int>::iterator it = actionItems_.find( item0->text().toStdString() );
    if( !item0 ) break;

    nFilledRows++;

  }


  for( unsigned int row = 0; row < nFilledRows; ++row ) {

    // check basic syntax
    if( DefoSchedule::GOOD_ROW != checkRowValidity( row ) ) {
      QMessageBox::critical( 0, tr("[DefoSchedule::validate]"), 
			     QString( "ERROR ** syntax problem in row %1" ).arg( row + 1 ),
			     QMessageBox::Ok );
      std::cerr << " [DefoSchedule::validate] ** ERROR: schedule table: syntax problem in row " << row + 1 << std::endl;
      break;
    }

    // various tests for individual actions
    QStandardItem* item0 = model_.item( row, 0 );
    QStandardItem* item1 = model_.item( row, 1 );

    // get action value
    std::map<std::string,int>::iterator it = actionItems_.find( item0->text().toStdString() );

    switch( it->second ) {

    case DefoSchedule::GOTO:
      { // here we check for endless loops and if goto points to an invalid/empty row
	bool isOk = false;
	const unsigned int rowPointedTo = model_.item( row, 1 )->text().toUInt( &isOk, 10 ) - 1;
	if( rowPointedTo >= nFilledRows ) issueGotoInvalidLineError( row, QString( "DefoSchedule::validate" ), rowPointedTo );
	if( rowPointedTo == row ) issueGotoPointsToItselfError( row, QString( "DefoSchedule::validate" ) );
	
      } break;


    case DefoSchedule::FILE_SET:
    case DefoSchedule::FILE_REF:
    case DefoSchedule::FILE_DEFO:
      { // check if the files exists
	if( !QFile( item1->text() ).exists() ) issueMissingFileError( row, QString( "DefoSchedule::validate" ),
								      QString( it->first.c_str() ), item1->text() );
      } break;

    case DefoSchedule::SET:
    case DefoSchedule::REF:
    case DefoSchedule::DEFO:
      { // check if camera is enabled & powered
	// comes later (have to ask DefoMainWindow..)
      } break;

    case DefoSchedule::TEMP:
      {
	// check if the temperature value makes sense
	bool isOk = false;
	double temp = model_.item( row, 1 )->text().toDouble( &isOk );
	if( !isOk ) {
	  issueBogusTemperatureError( row, QString( "DefoSchedule::validate" ) );
	}
	if( temp < __FP50_LOWER_TEMP_LIMIT || temp > __FP50_UPPER_TEMP_LIMIT ) { // these from JulaboFP50.h
	  issueExcessTemperatureError( row, QString( "DefoSchedule::validate" ) );
	}

      } break;

    default:
      {} break;

    }
      
  }

}



///
/// row is real row number (starts at 0)
///
void DefoSchedule::issueMissingFileError( int row, QString issuer, QString action, QString filename ) {

  QMessageBox::critical( 0, issuer, 
			 QString( "Row %1 [" ).arg( row+1 ) + action + QString( "] ** ERROR: Cannot open file:\n \"%1\"" ).arg( filename ),
			 QMessageBox::Ok );

}



///
/// row is real row number (starts at 0)
///
void DefoSchedule::issueGotoInvalidLineError( int row, QString issuer, int pointToRow ) {
  
  QMessageBox::critical( 0, issuer, 
			 QString( "Row %1 " ).arg( row+1 ) + QString( "** ERROR: GOTO points to invalid line: %1" ).arg( pointToRow+1 ),
			 QMessageBox::Ok );
  
}



///
/// row is real row number (starts at 0)
///
void DefoSchedule::issueGotoPointsToItselfError( int row, QString issuer ) {

  QMessageBox::critical( 0, issuer, 
			 QString( "Row %1 " ).arg( row+1 ) + QString( "** ERROR: GOTO points to itself" ),
			 QMessageBox::Ok );

}



///
/// row is real row number (starts at 0)
///
void DefoSchedule::issueCameraNotEnabledError( int row, QString issuer, QString action ) {

  QMessageBox::critical( 0, issuer, 
			 QString( "Row %1 [" ).arg( row+1 ) + action + QString( "] ** ERROR: Camera is disabled." ),
			 QMessageBox::Ok );

}



///
/// row is real row number (starts at 0)
///
void DefoSchedule::issueBogusTemperatureError( int row, QString issuer ) {

  QMessageBox::critical( 0, issuer, 
			 QString( "Row %1 " ).arg( row+1 ) + QString( "** ERROR: bogus temperature value." ),
			 QMessageBox::Ok );

}



///
/// row is real row number (starts at 0)
///
void DefoSchedule::issueExcessTemperatureError( int row, QString issuer ) {

  QMessageBox::critical( 0, issuer, 
			 QString( "Row %1 " ).arg( row+1 ) + QString( "** ERROR: temperature exceeds Julabo soft thresholds." ),
			 QMessageBox::Ok );

}
