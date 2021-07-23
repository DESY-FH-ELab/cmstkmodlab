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
	std::map<std::string,std::string> keyMap;
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

	int count;
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
		std::map<std::string,std::string>::iterator itFind = keyMap.find(Key);
		std::map<std::string,std::string>::iterator it = tmap.find(Key);
		if (it!=tmap.end()) {
			if (itFind==keyMap.end()) {
				ostream.fill(' ');
				ostream.width(25);
				ostream << std::left << it->first;

				count = 0;
				auto range = tmap.equal_range(it->first);
				for (auto i = range.first; i != range.second; ++i) {
					if (count>0) ostream << " ";
					ostream << i->second;
					count++;
				}
				ostream << "\n";

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
			ostream.width(25);
			ostream << std::left << kv.first;

			count = 0;
			auto range = tmap.equal_range(kv.first);
			for (auto i = range.first; i != range.second; ++i) {
				if (count>0) ostream << " ";
				ostream << i->second;
				count++;
			}
			ostream << "\n";

			keyMap.insert(std::make_pair(kv.first, kv.first));
		}
	}

	file.close();

	std::ofstream ofile(outputFileName_.c_str(), std::ios::trunc);
	ofile << ostream.str() << std::endl;
	ofile.close();
}

void ApplicationConfigWriter::writeNew(std::multimap<std::string,std::string> &keyvalueMap)
{
	std::map<std::string,std::string> keyMap;
  std::ofstream file(outputFileName_.c_str(), std::ios::out);
  for (auto & kv : keyvalueMap) {

  	std::map<std::string,std::string>::iterator itFind = keyMap.find(kv.first);
  	if (itFind==keyMap.end()) {
  		file.fill(' ');
  		file.width(25);
  		file << std::left << kv.first;

  		auto range = keyvalueMap.equal_range(kv.first);
  		for (auto i = range.first; i != range.second; ++i) {
  			file << i->second << " ";
  		}

  		keyMap.insert(std::make_pair(kv.first, kv.first));
  	}
  }
}
