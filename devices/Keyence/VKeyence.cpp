#include <sstream>
#include <iostream>

#include "VKeyence.h"

VKeyence::VKeyence( const ioport_t ioPort )
{

}

VKeyence::~VKeyence()
{

}

std::string VKeyence::SetValue(const std::string & command)
{
    this->SendCommand(command);
    std::string buffer;
    this->ReceiveString(buffer);
    return buffer;
}

std::string VKeyence::SetValue(const std::string & command,
                             const std::string & value)
{
  std::ostringstream os;
  os << command << value;
  this->SendCommand(os.str());
  std::string buffer;
  this->ReceiveString(buffer);
  return buffer;
}

std::string VKeyence::SetValue(const std::string & command,
                             int value1)
{
  std::ostringstream os;
  os << command << value1;
  this->SendCommand(os.str());
  std::string buffer;
  this->ReceiveString(buffer);
  return buffer;
}

std::string VKeyence::SetValue(const std::string & command,
                             int value1, int value2)
{
  std::ostringstream os;
  os << command << value1 << "," << value2;
  this->SendCommand(os.str());
  std::string buffer;
  this->ReceiveString(buffer);
  return buffer;
}

std::string VKeyence::SetValue(const std::string & command, 
int value1, const std::string & value2, int value3)
{
  std::ostringstream os;
  os << command << value1 << "," << value2 << "," << value3;
  this->SendCommand(os.str());
  std::string buffer;
  this->ReceiveString(buffer);
  return buffer;
}

std::string VKeyence::SetValue(const std::string & command,
                             double value1)
{
  std::ostringstream os;
  os << command << value1;
  this->SendCommand(os.str());
  std::string buffer;
  this->ReceiveString(buffer);
  return buffer;
}

/*
void VKeyence::GetValue(const std::string & command,
                             std::string & value)
{
  this->SendCommand(command);
  this->ReceiveString(value);
}

void VKeyence::GetValue(const std::string & command,
                             int & value)
{
  this->SendCommand(command);

  std::string buffer;
  int temp;
  this->ReceiveString(buffer);
  std::istringstream is(buffer);
  is >> temp;
  value = temp;
}

void VKeyence::GetValue(const std::string & command,
			int & value1, std::string & value2)
{
  this->SetValue(command, value1);
  this->ReceiveString(value2);
}


void VKeyence::GetValue(const std::string & command,
			int & value1, double & value2)
{
  this->SetValue(command, value1);

  std::string buffer;
  double temp;
  this->ReceiveString(buffer);
  std::istringstream is(buffer);
  is >> temp;
  value2 = temp;
}

void VKeyence::GetValue(const std::string & command,
                             double & value)
{
  this->SendCommand(command);

  std::string buffer;
  double temp;
  this->ReceiveString(buffer);
  std::istringstream is(buffer);
  is >> temp;
  value = temp;
}

void VKeyence::GetValue(const std::string & command, 
			int & value1,
                        std::vector<double> & value2)
{
  this->SetValue(command, value1);

  std::string buffer;
  double temp;
  this->ReceiveString(buffer);

  value2.clear();
  std::istringstream is(buffer);
  while (is >> temp) {
    value2.push_back(temp);
  }
}
*/




