

#include "DefoCamHandler.h"


///
/// when calling this, make sure that
/// action_ and - if necessary - cfg_ have
/// been set properly beforehand
///
void DefoCamHandler::run( void ) {

  switch( action_ ) {

  case DefoCamHandler::GETCFG:
    // nothing yet
    emit actionFinished( DefoCamHandler::GETCFG );
    break;

  case DefoCamHandler::SETCFG:
    camera_.writeConfig( cfg_ );
    emit actionFinished( DefoCamHandler::SETCFG );
    break;

  case DefoCamHandler::GETIMAGE:
    camera_.getAndSaveImage( filePath_ );
    emit actionFinished( DefoCamHandler::GETIMAGE );
    break;

  default:
    std::cerr << " [DefoCamHandler::run] ** ERROR: switch reached default" << std::endl;
    QMessageBox::critical( 0, tr("[DefoCamHandler::run]"),
	 QString("Switch statement reached default, this should not happen.\nCheck for illegal arguments in calls to DefoCamHandler::setAction."),
	 QMessageBox::Ok );

  }

}
