#include <iostream>
#include <fstream>
#include <sstream>

#include <string>
#include <map>
#include <vector>
#include <ctime>

typedef struct { 
  unsigned int   uTime;
  unsigned int   daqTime;
  float          pidPx;
  int            pidTn;
  int            pidTv;
  float          workPoint;
  int            pressure;
  int            circulator;
  int            hamegStatus;
  float          hamegU1set;
  float          hamegI1limit;
  float          hamegU2set;
  float          hamegI2limit;
} Status_t;

unsigned int uTimeStart;
std::map<unsigned int,std::vector<std::string> > messages;
Status_t status;
Status_t lastStatus1;
std::ofstream * ofile;

void parseStatusLine(std::ifstream & ifile)
{
  int time;
  float Px;
  int Tn, Tv;
  float work;
  int pressure;
  int circulator;

  ifile >> time >> Px >> Tn >> Tv >> work >> pressure >> circulator;

  status.uTime = uTimeStart + time;
  status.daqTime = time;

  status.pidPx = Px;
  status.pidTn = Tn;
  status.pidTv = Tv;
  status.workPoint = work;
  status.pressure = pressure;
  status.circulator = circulator;

  if (lastStatus1.pidPx!=status.pidPx) {
    std::stringstream msg;
    msg << "PID controller Px value set to ";
    msg << status.pidPx;
    messages[time].push_back(msg.str());
  }
  if (lastStatus1.pidTn!=status.pidTn) {
    std::stringstream msg;
    msg << "PID controller Tn value set to ";
    msg << status.pidTn;
    messages[time].push_back(msg.str());
  }
  if (lastStatus1.pidTv!=status.pidTv) {
    std::stringstream msg;
    msg << "PID controller Tv value set to ";
    msg << status.pidTv;
    messages[time].push_back(msg.str());
  }
  if (lastStatus1.workPoint!=status.workPoint) {
    std::stringstream msg;
    msg << "chiller work point set to ";
    msg << status.workPoint;
    messages[time].push_back(msg.str());
  }
  if (lastStatus1.pressure!=status.pressure) {
    std::stringstream msg;
    msg << "chiller pressure is ";
    msg << status.pressure;
    messages[time].push_back(msg.str());
  }
  if (lastStatus1.circulator!=status.circulator) {
    std::stringstream msg;
    msg << "chiller circulator is switched ";
    msg << (status.circulator==1 ? "on" : "off");
    messages[time].push_back(msg.str());
  }

  lastStatus1.pidPx = status.pidPx;
  lastStatus1.pidTn = status.pidTn;
  lastStatus1.pidTv = status.pidTv;
  lastStatus1.workPoint = status.workPoint;
  lastStatus1.pressure = status.pressure;
  lastStatus1.circulator = status.circulator;
}

void parseHamegStatusLine(std::ifstream & ifile)
{
  int time;
  int statusBits;
  float U1;
  float I1;
  float U2;
  float I2;

  ifile >> time >> statusBits >> U1 >> I1 >> U2 >> I2;

  status.uTime = uTimeStart + time;
  status.daqTime = time;

  status.hamegStatus = statusBits;
  status.hamegU1set = U1;
  status.hamegI1limit = I1;
  status.hamegU2set = U2;
  status.hamegI2limit = I2;

  if (lastStatus1.hamegStatus!=status.hamegStatus) {
    std::stringstream msg;
    msg << "Hameg HM8143 status changed to ";
    if (status.hamegStatus&0x01) msg << "OP0 ";
    if (status.hamegStatus&0x02) msg << "OP1 ";
    if (status.hamegStatus&0x04) msg << "CV1 ";
    if (status.hamegStatus&0x08) msg << "CC1 ";
    if (status.hamegStatus&0x10) msg << "CV2 ";
    if (status.hamegStatus&0x20) msg << "CC2 ";
    if (status.hamegStatus&0x40) msg << "RM1 ";
    if (status.hamegStatus&0x80) msg << "RM0 ";
    messages[time].push_back(msg.str());
  }
  if (lastStatus1.hamegU1set!=status.hamegU1set) {
    std::stringstream msg;
    msg << "Hameg HM8143 U1 set to ";
    msg << status.hamegU1set << "V";
    messages[time].push_back(msg.str());
  }
  if (lastStatus1.hamegI1limit!=status.hamegI1limit) {
    std::stringstream msg;
    msg << "Hameg HM8143 I1 limit set to ";
    msg << status.hamegI1limit << "A";
    messages[time].push_back(msg.str());
  }
  if (lastStatus1.hamegU2set!=status.hamegU2set) {
    std::stringstream msg;
    msg << "Hameg HM8143 U2 set to ";
    msg << status.hamegU2set << "V";
    messages[time].push_back(msg.str());
  }
  if (lastStatus1.hamegI2limit!=status.hamegI2limit) {
    std::stringstream msg;
    msg << "Hameg HM8143 I2 limit set to ";
    msg << status.hamegI2limit << "A";
    messages[time].push_back(msg.str());
  }

  lastStatus1.pidPx = status.pidPx;
  lastStatus1.pidTn = status.pidTn;
  lastStatus1.pidTv = status.pidTv;
  lastStatus1.workPoint = status.workPoint;
  lastStatus1.pressure = status.pressure;
  lastStatus1.circulator = status.circulator;
  lastStatus1.hamegStatus = status.hamegStatus;
  lastStatus1.hamegU1set = status.hamegU1set;
  lastStatus1.hamegI1limit = status.hamegI1limit;
  lastStatus1.hamegU2set = status.hamegU2set;
  lastStatus1.hamegI2limit = status.hamegI2limit;
}

void parseDataLine(std::ifstream & ifile)
{
  std::string line;
  std::getline(ifile, line);
}

void parseLogLine(std::ifstream & ifile)
{
  int time;
  std::string temp;

  ifile >> time;

  std::getline(ifile, temp);
  size_t found;
  found = temp.find_first_not_of(" ");
  temp.erase(0, found);

  found = temp.find_last_not_of(" ");
  temp.resize(found+1);

  messages[time].push_back(temp);
}

int main(int argc, char * argv[])
{
  if (argc!=2) {
    std::cout << "usage: LabControl2Report <file>" << std::endl;
  }

  std::ifstream ifile(argv[1]);
  if (!ifile.good()) {
    std::cout << "LabControl2Report: can not open file " << argv[1] << std::endl;
    return -1;
  }
  std::cout << "LabControl2Report: reading from file " << argv[1] << std::endl;

  lastStatus1.pidPx = -1;
  lastStatus1.pidTn = -1;
  lastStatus1.pidTv = -1;
  lastStatus1.workPoint = -999;
  lastStatus1.pressure = -999;
  lastStatus1.circulator = 9;

  int fileformat = 0;
  std::string token;
  std::string cmd;
  std::string time;

  while (ifile >> token) {

    if (token=="<HEADER>") {
      ifile >> cmd;

      if (cmd=="fileformat") {
	ifile >> fileformat;
      }
      if (cmd=="time") {
	ifile >> time;
	std::string filename = "LabControl_";
	filename += time;
	filename += ".log";
	ofile = new std::ofstream(filename.c_str());
	std::cout << "LabControl2Report: writing to file "
		  << filename << std::endl;
      }
      if (cmd=="utime") {
	ifile >> uTimeStart;
      }
      if (cmd=="activeChannels") {
	int count, channel;
	ifile >> count;
	std::stringstream msg;
	msg << "active channels: ";
	for (int i=0;i<count;i++) {
	  ifile >> channel;
	  if (i>0) msg << ", ";
	  msg << channel;
	}
	messages[0].push_back(msg.str());
      }
    }

    if (token=="<STATUS>") parseStatusLine(ifile);
    if (token=="<HAMEG>") parseHamegStatusLine(ifile);
    if (token=="<DATA>") parseDataLine(ifile);
    if (token=="<LOG>") parseLogLine(ifile);
  }

  for (std::map<unsigned int,std::vector<std::string> >::iterator itm = messages.begin();
       itm!=messages.end();
       ++itm) {

    time_t tm = (*itm).first + uTimeStart;
    struct tm *lt = localtime(&tm);
    char trep[80];
    strftime(trep, 80, "%d/%m/%Y %X", lt);
    *ofile << trep << "  -  @" << (*itm).first << "s" << std::endl;

    for (std::vector<std::string>::iterator its = (*itm).second.begin();
	 its!=(*itm).second.end();
	 ++its) {
      std::string msg = (*its);
      *ofile << "  " << msg << std::endl;
    }

    *ofile << std::endl;
  }

  ofile->close();
  delete ofile;

  return 0;
}
