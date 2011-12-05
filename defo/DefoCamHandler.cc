

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
    break;

  case DefoCamHandler::SETCFG:
    // nothing yet
    break;

  case DefoCamHandler::GETIMAGE:
    camera_.getAndSaveImage( filePath_ );
    actionFinished( DefoCamHandler::GETIMAGE );
    break;

  default:
    std::cerr << " [DefoCamHandler::run] ** ERROR: switch reached default" << std::endl;
    throw;

  }

}
