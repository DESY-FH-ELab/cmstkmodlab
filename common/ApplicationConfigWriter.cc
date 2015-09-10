#include <fstream>
#include <sstream>

#include <QMessageBox>

#include "ApplicationConfigWriter.h"

ApplicationConfigWriter::ApplicationConfigWriter( const std::string & outputFileName )
{
  outputFileName_ = std::string( outputFileName );
}

ApplicationConfigWriter::~ApplicationConfigWriter()
{

}

void ApplicationConfigWriter::write(std::multimap<std::string,std::string> &keyvalueMap)
{
  std::ifstream file( outputFileName_.c_str(), std::ios::in );
  if (file.good()) {
    file.close();
    writeMerge(keyvalueMap);
  } else {
    writeNew(keyvalueMap);
  }
}

void ApplicationConfigWriter::writeMerge(std::multimap<std::string,std::string> &keyvalueMap)
{
  std::multimap<std::string,std::string> tmap = keyvalueMap;
  std::ostringstream ostream;
  
  std::ifstream file( outputFileName_.c_str(), std::ios::in );
  if( !file.good() ) {
    std::cerr << " [ApplicationConfigWriter::openAndCheckFile] ** ERROR: failed to open file: "
	      << outputFileName_ << "." << std::endl;
    QMessageBox::critical( 0, tr("[ApplicationConfigWriter::fill]"),
			   QString("Failed to open configuration file: \"%1\". No chance!").arg(QString(outputFileName_.c_str())),
			   QMessageBox::Abort );
    throw; // must abort
  }

  std::string Key;
  std::string Value;
  std::string buffer;
  
  while (std::getline(file, buffer)) {
    while (buffer[0]==' ') buffer = buffer.substr(1, buffer.length());
    if (buffer[0]=='\0' || buffer[0]=='\n' || buffer[0]=='#') {
      ostream << buffer << std::endl;
      continue;
    }

    std::istringstream iss(buffer.c_str(), std::istringstream::in);
    iss >> Key;
    iss >> Value;

    std::map<std::string,std::string>::iterator it = tmap.find(Key);
    if (it!=tmap.end()) {
      Value = it->second;
      tmap.erase(it);
    }
    ostream.fill(' ');
    ostream.width(25);
    ostream << std::left << Key;
    ostream << Value << std::endl;
  }
  
  for (auto v : tmap) {
     ostream.fill(' ');
     ostream.width(25);
     ostream << std::left << v.first;
     ostream << v.second << std::endl;
  }  
  
  file.close();
  
  std::ofstream ofile(outputFileName_.c_str(), std::ios::trunc);
  ofile << ostream.str() << std::endl;
  ofile.close();
}

void ApplicationConfigWriter::writeNew(std::multimap<std::string,std::string> &keyvalueMap)
{
  std::ofstream file(outputFileName_.c_str(), std::ios::out);
  for (auto v : keyvalueMap) {
     file.fill(' ');
     file.width(25);
     file << std::left << v.first;
     file << v.second << std::endl;
  }
}
