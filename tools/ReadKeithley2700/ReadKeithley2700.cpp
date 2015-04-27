
#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include <sstream>
#include <vector>
#include <cstdio>

#include <boost/iostreams/tee.hpp>
#include <boost/iostreams/stream.hpp>

#ifdef USE_FAKEIO
#include "Keithley2700Fake.h"
typedef Keithley2700Fake Keithley2700_t;
#else
#include "Keithley2700.h"
typedef Keithley2700 Keithley2700_t;
#endif

///
/// usage: ./readTemp port [channelString]
/// where channelString = e.g. "1,2,3-5,8"
///
int main( int argc, char** argv ) {

  if( argc < 2 || argc > 3  ) {
    std::cerr << " [" << argv[0] << "] ** ERROR: unrecognized command line options" << std::endl;
    std::cerr << "   Usage: readTemp port [channelString]" << std::endl;
    std::cerr << "   (where [channelString] e.g. = 1,2,3-5,8)" << std::endl;
    exit( -1 );
  }

  unsigned int readingCounter = 0;
  const time_t startingTime = time( NULL );

  std::stringstream outputFileName;
  outputFileName << "readTemp_output_" << startingTime << ".dat";

  // dual stream, writing to cout and outputFile simultaneously
  std::ofstream outFile( outputFileName.str().c_str() );
  boost::iostreams::tee_device<std::ostream, std::ofstream> teeDevice( std::cout, outFile );
  boost::iostreams::stream<boost::iostreams::tee_device<std::ostream, std::ofstream> > dualOut( teeDevice );
  

  // init device anf give it some time
  Keithley2700_t keithley(argv[1]);
  sleep( 1 );

  // no channels specified, read all
  if (argc == 2) keithley.SetActiveChannels("0-9");

  // pass selected channels to device
  if (argc == 3) keithley.SetActiveChannels( argv[1] );
  
  // log info
  dualOut << std::endl;
  dualOut << " Log starting: " << startingTime << " = " << asctime( localtime( &startingTime ) );
  dualOut << " Logging to: " << outputFileName.str() << std::endl;
  dualOut << std::endl;

  // loop forever
  while (true) {

    // scan and fill theReading
    reading_t theReading = keithley.Scan();
    if( ! keithley.IsScanOk() ) continue;

    // pretty print header, every ten lines
    if( 0 ==  readingCounter % 10 ) {
      dualOut << std::setw( 8 ) << std::setfill( ' ' ) << std::left << "";
      dualOut << std::setw( 8 ) << std::setfill( ' ' ) << std::right << "seconds";
      dualOut << std::setw( 2 ) << std::setfill( ' ' ) << std::right << "";
      for( reading_t::const_iterator it = theReading.begin(); it < theReading.end(); ++it ) {
        dualOut << std::setw( 12 ) << std::right <<  std::setfill( '.' ) << it->first;
      }
      dualOut << std::endl << std::flush;
    }
    
    // print readings
    time_t currentTime = time( NULL );
    dualOut << std::setw( 8 ) << std::setfill( ' ' ) << std::left << " <DATA>";
    dualOut << std::setw( 8 ) << std::setfill( ' ' ) << std::right << currentTime - startingTime;
    dualOut << std::setw( 2 ) << std::setfill( ' ' ) << std::right << "";
    for( reading_t::const_iterator it = theReading.begin(); it < theReading.end(); ++it ) {
      dualOut << std::setw( 12 ) << std::right << std::setfill( ' ' ) << it->second;
    }
    dualOut << std::endl;
    
    // need to explicitly flush stream
    dualOut.flush();

    ++readingCounter;

    sleep(5);
  }
}
    
