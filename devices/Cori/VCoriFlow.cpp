#include "VCoriFlow.h"

///
///
///
VCoriFlow::VCoriFlow( const ioport_t ioPort )
{

}

float VCoriFlow::ToFloat(const char* buffer) const
{
  std::string temp = buffer;
  //   std::cout << " > " << buffer << std::endl;
  temp.erase(0,11);
  temp.insert(0, "0x");
  const char *t = temp.c_str();
  
  unsigned int num;
  float f;
  sscanf(t, "%x", &num);  // assuming you checked input
  f = *((float*)&num);
  //   printf("the hexadecimal 0x%08x becomes %.3f as a float\n", num, f);
  
  return f;
}

float VCoriFlow::ToInt(const char* buffer) const
{
  std::string temp = buffer;
  //   std::cout << " > " << buffer << std::endl;
  temp.erase(0,11);
  temp.insert(0, "0x");
  const char *t = temp.c_str();
  
  unsigned int num;
  int f;
  sscanf(t, "%x", &num);  // assuming you checked input
  f = *((int*)&num);
  //   printf("the hexadecimal 0x%08x becomes %.3f as a float\n", num, f);
  
  return f;
}

std::string VCoriFlow::ToUnit(const char* buffer) const
{
  std::string temp = buffer;
  //   std::cout << " > " << buffer << std::endl;
  temp.erase(0,13);
  temp.insert(0, "0x");
  const char *t = temp.c_str();
  
  std::string newString;
  for(int i=0; i< 9; i+=2)
  {
    std::string byte = temp.substr(i,2);
    char chr = (char) (int)strtol(byte.c_str(), nullptr, 16);
    newString.push_back(chr);
  }
  //   std::cout << " temp > " << temp << std::endl;
  
  return newString;
}