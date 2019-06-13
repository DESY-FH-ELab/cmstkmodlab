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
#include <iomanip>

#include <QMessageBox>

#include "ApplicationConfigReader.h"

ApplicationConfigReader::ApplicationConfigReader( const std::string & inputFileName )
{
  inputFileName_ = std::string( inputFileName );
}

ApplicationConfigReader::~ApplicationConfigReader()
{
}

void ApplicationConfigReader::fill(std::multimap<std::string,std::string> &keyvalueMap)
{
  std::ifstream file(inputFileName_.c_str(), std::ios::in);

  if(not file.good())
  {
    std::cerr << " [ApplicationConfigReader::openAndCheckFile] ** ERROR: failed to open file: " << inputFileName_ << "." << std::endl;

    QMessageBox::critical(0,
      tr("[ApplicationConfigReader::fill]"),
      QString("Failed to open configuration file: \"%1\". No chance!").arg(QString(inputFileName_.c_str())),
      QMessageBox::Abort
    );

    throw; // must abort
  }

  std::string Key;
  std::string Value;
  std::string buffer;

//  bool inside_str = false;

  while(std::getline(file, buffer))
  {
    // ignore all characters after (and including) hashtag
    if(buffer.find("#") != std::string::npos)
    {
      buffer = buffer.substr(0, buffer.find("#"));
    }

    if(buffer.empty()){ continue; }

    std::istringstream iss(buffer.c_str(), std::istringstream::in);
    iss >> Key;
    while(iss >> std::quoted(Value))
    {
      std::cout << "!!!!!!!!!!!!!!!!!!! " << Key << " " << Value << std::endl;

      keyvalueMap.insert(std::make_pair(Key, Value));
    }
  }

  file.close();
}
