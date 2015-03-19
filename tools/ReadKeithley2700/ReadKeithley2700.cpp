
#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include <sstream>
#include <vector>
#include <cstdio>

#include <boost/iostreams/tee.hpp>
#include <boost/iostreams/stream.hpp>

#include "Keithley2700.h"

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

  stringstream outputFileName;
  outputFileName << "readTemp_output_" << startingTime << ".dat";

  // dual stream, writing to cout and outputFile simultaneously
  ofstream outFile( outputFileName.str().c_str() );
  boost::iostreams::tee_device<std::ostream, std::ofstream> teeDevice( cout, outFile );
  boost::iostreams::stream<boost::iostreams::tee_device<ostream, ofstream> > dualOut( teeDevice );
  

  // init device anf give it some time
  Keithley2700 keithley(argv[1]);
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
      dualOut << setw( 8 ) << setfill( ' ' ) << left << "";
      dualOut << setw( 8 ) << setfill( ' ' ) << right << "seconds";
      dualOut << setw( 2 ) << setfill( ' ' ) << right << "";
      for( reading_t::const_iterator it = theReading.begin(); it < theReading.end(); ++it ) {
	dualOut << setw( 12 ) << right <<  setfill( '.' ) << it->first;
      }
      dualOut << endl << flush;
    }
    
    // print readings
    time_t currentTime = time( NULL );
    dualOut << setw( 8 ) << setfill( ' ' ) << left << " <DATA>";
    dualOut << setw( 8 ) << setfill( ' ' ) << right << currentTime - startingTime;
    dualOut << setw( 2 ) << setfill( ' ' ) << right << "";
    for( reading_t::const_iterator it = theReading.begin(); it < theReading.end(); ++it ) {
      dualOut << setw( 12 ) << right << setfill( ' ' ) << it->second;
    }
    dualOut << endl;
    
    // need to explicitly flush stream
    dualOut.flush();

    ++readingCounter;

    sleep(5);
  }
   

 
}
    
