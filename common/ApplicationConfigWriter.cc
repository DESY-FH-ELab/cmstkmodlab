/////////////////////////////////////////////////////////////////////////////////
//                                                                             //
//               Copyright (C) 2011-2017 - The DESY CMS Group                  //
//                           All rights reserved                               //
//                                                                             //
//      The CMStkModLab source code is licensed under the GNU GPL v3.0.        //
//      You have the right to modify and/or redistribute this source code      //
//      under the terms specified in the license, which may be found online    //
//      at http://www.gnu.org/licenses or at License.txt.                      //
//                                                                             //
/////////////////////////////////////////////////////////////////////////////////

#include <fstream>
#include <sstream>
#include <algorithm>

#include <QMessageBox>

#include "ApplicationConfigWriter.h"

ApplicationConfigWriter::ApplicationConfigWriter( const std::string & outputFileName )
{
  outputFileName_ = std::string( outputFileName );
}

ApplicationConfigWriter::~ApplicationConfigWriter()
{

}

void ApplicationConfigWriter::write(ApplicationConfig::storage_t  &keyvalueMap)
{
  std::ifstream file( outputFileName_.c_str(), std::ios::in );
  if (file.good()) {
    file.close();
    writeMerge(keyvalueMap);
  } else {
    file.close();
    writeNew(keyvalueMap);
  }
}

void ApplicationConfigWriter::write(ApplicationConfig::storage_t  &keyvalueMap,
    ApplicationConfig::configfile_t &configFileKeyMap)
{
  ApplicationConfig::storage_t temp;

  auto range = configFileKeyMap.equal_range(outputFileName_);

  for (auto i = range.first; i != range.second; ++i) {
    auto search = keyvalueMap.find(i->second);
    if (search!=keyvalueMap.end()) {
      temp.insert({ search->first, search->second });
    }
  }

  std::ifstream file( outputFileName_.c_str(), std::ios::in );
  if (file.good()) {
    file.close();
    writeMerge(temp);
  } else {
    file.close();
    writeNew(temp);
  }
}

void ApplicationConfigWriter::writeMerge(ApplicationConfig::storage_t  &keyvalueMap)
{
  std::map<std::string,std::string> keyMap;
  ApplicationConfig::storage_t tmap = keyvalueMap;
  std::ostringstream ostream;

  unsigned long maxKeyLength = 0;
  for (auto & kv : keyvalueMap) {
    maxKeyLength = std::max(maxKeyLength, kv.first.size());
  }
  maxKeyLength = 5*(1+maxKeyLength/5)-1;

  std::ifstream file( outputFileName_.c_str(), std::ios::in );
  if( !file.good() ) {
    std::cerr << " [ApplicationConfigWriter::openAndCheckFile] ** ERROR: failed to open file: "
        << outputFileName_ << "." << std::endl;
    QMessageBox::critical( 0, tr("[ApplicationConfigWriter::fill]"),
        QString("Failed to open configuration file: \"%1\". No chance!").arg(QString(outputFileName_.c_str())),
        QMessageBox::Abort );
    throw; // must abort
  }

  int count;
  std::string Key;
  std::string Value;
  std::vector<std::string> Values;
  std::string buffer;
  std::string::size_type n;

  while (std::getline(file, buffer)) {

    while (buffer[0]==' ') buffer = buffer.substr(1, buffer.length());
    if (buffer[0]=='\0' || buffer[0]=='\n' || buffer[0]=='#') {
      ostream << buffer << std::endl;
      continue;
    }

    std::istringstream iss(buffer.c_str(), std::istringstream::in);
    iss >> Key;
    std::map<std::string,std::string>::iterator itFind = keyMap.find(Key);
    ApplicationConfig::storage_t::iterator it = tmap.find(Key);
    if (it!=tmap.end()) {
      if (itFind==keyMap.end()) {
        ostream.fill(' ');
        ostream.width(maxKeyLength);
        ostream << std::left << it->first << " ";

        count = 0;
        for (auto & v : it->second) {
          if (count>0) ostream << " ";
          ostream << v;
          count++;
        }

        n = buffer.find("#");
        if (n != std::string::npos) {
          ostream << " " << buffer.substr(n);
        }

        ostream << std::endl;

        keyMap.insert(std::make_pair(it->first, it->first));
      }
    }
  }

  for (auto & kv : keyMap) {
    tmap.erase(kv.first);
  }

  for (auto & kv : tmap) {

    std::map<std::string,std::string>::iterator itFind = keyMap.find(kv.first);
    if (itFind==keyMap.end()) {
      ostream.fill(' ');
      ostream.width(maxKeyLength);
      ostream << std::left << kv.first << " ";

      count = 0;
      for (auto & v : kv.second) {
        if (count>0) ostream << " ";
        ostream << v;
        count++;
      }
      ostream << std::endl;

      keyMap.insert(std::make_pair(kv.first, kv.first));
    }
  }

  file.close();

  std::ofstream ofile(outputFileName_.c_str(), std::ios::trunc);
  ofile << ostream.str() << std::endl;
  ofile.close();
}

void ApplicationConfigWriter::writeNew(ApplicationConfig::storage_t  &keyvalueMap)
{
  int count = 0;
  std::map<std::string,std::string> keyMap;
  std::ofstream file(outputFileName_.c_str(), std::ios::out);

  unsigned long maxKeyLength = 0;
  for (auto & kv : keyvalueMap) {
    maxKeyLength = std::max(maxKeyLength, kv.first.size());
  }
  maxKeyLength = 5*(1+maxKeyLength/5)-1;

  for (auto & kv : keyvalueMap) {
    std::map<std::string,std::string>::iterator itFind = keyMap.find(kv.first);
    if (itFind==keyMap.end()) {
      file.fill(' ');
      file.width(maxKeyLength);
      file << std::left << kv.first << " ";

      count = 0;
      for (auto & v : kv.second) {
        if (count>0) file << " ";
        file << v << " ";
        count++;
      }
      file << std::endl;

      keyMap.insert(std::make_pair(kv.first, kv.first));
    }
  }
}
