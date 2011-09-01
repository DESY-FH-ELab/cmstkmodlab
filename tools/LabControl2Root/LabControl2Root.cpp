#include <iostream>
#include <fstream>
#include <sstream>

#include <TFile.h>
#include <TTree.h>
#include <TString.h>

#include <string>

typedef struct { 
  UInt_t   uTime; 
  UInt_t   daqTime;
  Int_t    channelActive[10];
  Float_t  temperature[10];
  Int_t    referenceActive;
  Float_t  reference;
  Float_t  pidPx;
  Int_t    pidTn;
  Int_t    pidTv;
  Float_t  bath;
  Float_t  safety;
  Float_t  workPoint;
  Int_t    heatingPower;
  Int_t    pressure;
  Int_t    circulator;
  Int_t    hamegStatus;
  Float_t  hamegU1set;
  Float_t  hamegI1limit;
  Float_t  hamegU2set;
  Float_t  hamegI2limit;
  Float_t  hamegU1;
  Float_t  hamegI1;
  Float_t  hamegU2;
  Float_t  hamegI2;
} Measurement_t;

typedef struct { 
  UInt_t      uTime; 
  UInt_t      daqTime;
  std::string message;
} Log_t; 

TFile * ofile;
TTree * tree;
TTree * logtree;

Int_t uTimeStart;
std::vector<int> activeChannels;
Measurement_t data;
Log_t log;

void makeTree(int fileformat, const std::string time)
{
  std::string filename = "LabControl_";
  filename += time;
  filename += ".root";

  std::cout << "LabControl2Root: writing to file " << filename << std::endl;

  ofile = new TFile(filename.c_str(), "RECREATE");

  tree = new TTree("LabControl", "LabControl");

  tree->Branch("uTime", &data.uTime, "uTime/i");
  tree->Branch("daqTime", &data.daqTime, "daqTime/i");
  tree->Branch("channelActive", data.channelActive, "channelActive[10]/I");
  tree->Branch("temperature", data.temperature ,"temperature[10]/F");
  for (Int_t i=0;i<10;i++) {
    tree->Branch(Form("channel%dActive", i), &data.channelActive[i],
		 Form("channel%dActive/I", i));
    tree->Branch(Form("temperature%d", i), &data.temperature[i],
		 Form("temperature%d/F", i));
  }
  tree->Branch("referenceActive", &data.referenceActive, "referenceActive/I");
  tree->Branch("reference", &data.reference ,"reference/F");

  tree->Branch("pidPx", &data.pidPx, "pidPx/F");
  tree->Branch("pidTn", &data.pidTn, "pidTn/I");
  tree->Branch("pidTv", &data.pidTv, "pidTv/I");

  tree->Branch("bath", &data.bath ,"bath/F");
  tree->Branch("safety", &data.safety ,"safety/F");
  tree->Branch("workPoint", &data.workPoint ,"workPoint/F");

  tree->Branch("heatingPower", &data.heatingPower, "heatingPower/I");
  tree->Branch("pressure", &data.pressure, "pressure/I");
  tree->Branch("circulator", &data.circulator, "circulator/I");

  tree->Branch("hamegStatus", &data.hamegStatus, "hamegStatus/I");
  tree->Branch("hamegU1set", &data.hamegU1set, "hamegU1set/F");
  tree->Branch("hamegI1limit", &data.hamegI1limit, "hamegI1limit/F");
  tree->Branch("hamegU2set", &data.hamegU2set, "hamegU2set/F");
  tree->Branch("hamegI2limit", &data.hamegI2limit, "hamegI2limit/F");
  tree->Branch("hamegU1", &data.hamegU1, "hamegU1/F");
  tree->Branch("hamegI1", &data.hamegI1, "hamegI1/F");
  tree->Branch("hamegU2", &data.hamegU2, "hamegU2/F");
  tree->Branch("hamegI2", &data.hamegI2, "hamegI2/F");

  logtree = new TTree("LabControlLog", "LabControl");

  logtree->Branch("uTime", &log.uTime, "uTime/i");
  logtree->Branch("daqTime", &log.daqTime, "daqTime/i");
  logtree->Branch("message", &log.message);
}

void closeTree()
{
  ofile->Write();
  delete ofile;
}

void parseStatusLine(std::ifstream & ifile)
{
  Int_t time;
  Float_t Px;
  Int_t Tn, Tv;
  Float_t work;
  Int_t pressure;
  Int_t circulator;
  
  ifile >> time >> Px >> Tn >> Tv >> work >> pressure >> circulator;
  
  data.uTime = uTimeStart + time;
  data.daqTime = time;

  data.pidPx = Px;
  data.pidTn = Tn;
  data.pidTv = Tv;
  data.workPoint = work;
  data.pressure = pressure;
  data.circulator = circulator;
}

void parseHamegStatusLine(std::ifstream & ifile)
{
  Int_t time;
  Int_t status;
  Float_t U1, I1, U2, I2;
  
  ifile >> time >> status >> U1 >> I1 >> U2 >> I2;
  
  data.uTime = uTimeStart + time;
  data.daqTime = time;

  data.hamegStatus = status;
  data.hamegU1set = U1;
  data.hamegI1limit = I1;
  data.hamegU2set = U2;
  data.hamegI2limit = I2;
}

void parseHamegLine(std::ifstream & ifile)
{
  Int_t time;
  Float_t U1, I1, U2, I2;
  
  ifile >> time >> U1 >> I1 >> U2 >> I2;
  
  data.uTime = uTimeStart + time;
  data.daqTime = time;

  data.hamegU1 = U1;
  data.hamegI1 = I1;
  data.hamegU2 = U2;
  data.hamegI2 = I2;
}

void parseDataLine(std::ifstream & ifile)
{
  Int_t time;
  Float_t temp;
  Float_t bath, safety;
  Int_t power;
  
  ifile >> time;
  data.uTime = uTimeStart + time;
  data.daqTime = time;

  for (UInt_t i=0;i<activeChannels.size();i++) {
    ifile >> temp;
    data.temperature[activeChannels[i]] = temp;
  }

  if (data.referenceActive) {
    ifile >> temp;
    data.reference = temp;
  }

  ifile >> bath >> safety >> power;

  data.bath = bath;
  data.safety = safety;
  data.heatingPower = power;

  tree->Fill();
}

void parseLogLine(std::ifstream & ifile)
{
  Int_t time;
  TString temp;
  
  ifile >> time;
  log.uTime = uTimeStart + time;
  log.daqTime = time;

  temp.ReadLine(ifile);
  log.message = temp.Data();

  logtree->Fill();
}

int main(int argc, char * argv[])
{
  if (argc!=2) {
    std::cout << "usage: LabControl2Root <file>" << std::endl;
  }

  std::ifstream ifile(argv[1]);
  if (!ifile.good()) {
    std::cout << "LabControl2Root: can not open file " << argv[1] << std::endl;
    return -1;
  }
  std::cout << "LabControl2Root: reading from file " << argv[1] << std::endl;
  
  for (Int_t i=0;i<10;i++) {
    data.channelActive[i] = 0;
    data.temperature[i] = -999;
  }
  data.referenceActive = 0;
  data.reference = -999;

  data.hamegStatus = 0;

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
      }
      if (cmd=="utime") {
	ifile >> uTimeStart;
      }
      if (cmd=="activeChannels") {
	int count, channel;
	ifile >> count;
	for (int i=0;i<count;i++) {
	  ifile >> channel;
	  data.channelActive[channel] = 1;
	  activeChannels.push_back(channel);
	}
      }
      if (cmd=="referenceActive") {
	int active;
	ifile >> active;
	data.referenceActive = active;
      }
    }

    if (token=="<DATALAYOUT>") makeTree(fileformat, time);

    if (token=="<STATUS>") parseStatusLine(ifile);
    if (token=="<HAMEG>") parseHamegStatusLine(ifile);
    if (token=="<HAMEGDATA>") parseHamegLine(ifile);
    if (token=="<DATA>") parseDataLine(ifile);
    if (token=="<LOG>") parseLogLine(ifile);
  }

  closeTree();

  return 0;
}
