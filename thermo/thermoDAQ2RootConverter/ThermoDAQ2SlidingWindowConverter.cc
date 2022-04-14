/////////////////////////////////////////////////////////////////////////////////
//                                                                             //
//               Copyright (C) 2011-2022 - The DESY CMS Group                  //
//                           All rights reserved                               //
//                                                                             //
//      The CMStkModLab source code is licensed under the GNU GPL v3.0.        //
//      You have the right to modify and/or redistribute this source code      //
//      under the terms specified in the license, which may be found online    //
//      at http://www.gnu.org/licenses or at License.txt.                      //
//                                                                             //
/////////////////////////////////////////////////////////////////////////////////

#include <iostream>

#include <QDateTime>
#include <QTextStream>
#include <QFile>
#include <QDir>
#include <QFileInfo>

#include <TDatime.h>

#include "ThermoDAQ2SlidingWindowConverter.h"

ThermoDAQ2SlidingWindowConverter::ThermoDAQ2SlidingWindowConverter(int windowSize,
    const QString& ifilename,
    const QString& ofilename,
    QObject* parent)
: QObject(parent),
  windowSize_(windowSize),
  ifilename_(ifilename),
  ofilename_(ofilename)
{

}

void ThermoDAQ2SlidingWindowConverter::run()
{
  process();
  emit finished();
}

void ThermoDAQ2SlidingWindowConverter::process()
{
  char branchName[40];
  char branchLeafList[40];
  Long64_t nentries;

  ifile_ = new TFile(ifilename_.toStdString().c_str(), "READ");

  itree_ = ifile_->Get<TTree>("thermoDAQ2");

  itree_->SetBranchAddress("uTime", &omeasurement_.uTime);
  TDatime *dt = &omeasurement_.datime;
  itree_->SetBranchAddress("datime", &dt);

  itree_->SetBranchAddress("u525wState", &omeasurement_.u525wState_);
  itree_->SetBranchAddress("u525wTemperatureSetPoint", &omeasurement_.u525wTemperatureSetPoint_);
  itree_->SetBranchAddress("u525wTemperatureControlMode", &omeasurement_.u525wTemperatureControlMode_);
  itree_->SetBranchAddress("u525wTemperatureControlEnabled", &omeasurement_.u525wTemperatureControlEnabled_);
  itree_->SetBranchAddress("u525wCirculatorEnabled", &omeasurement_.u525wCirculatorEnabled_);
  itree_->SetBranchAddress("u525wInternalTemperature", &imeasurement_.u525wInternalTemperature_);
  itree_->SetBranchAddress("u525wProcessTemperature", &imeasurement_.u525wProcessTemperature_);
  itree_->SetBranchAddress("u525wReturnTemperature", &omeasurement_.u525wReturnTemperature_);
  itree_->SetBranchAddress("u525wPumpPressure", &omeasurement_.u525wPumpPressure_);
  itree_->SetBranchAddress("u525wPower", &omeasurement_.u525wPower_);
  itree_->SetBranchAddress("u525wCWInletTemperature", &omeasurement_.u525wCWInletTemperature_);
  itree_->SetBranchAddress("u525wCWOutletTemperature", &omeasurement_.u525wCWOutletTemperature_);
  itree_->SetBranchAddress("u525wAutoPID", &omeasurement_.u525wAutoPID_);
  itree_->SetBranchAddress("u525wKpInternal", &omeasurement_.u525wKpInternal_);
  itree_->SetBranchAddress("u525wTnInternal", &omeasurement_.u525wTnInternal_);
  itree_->SetBranchAddress("u525wTvInternal", &omeasurement_.u525wTvInternal_);
  itree_->SetBranchAddress("u525wKpJacket", &omeasurement_.u525wKpJacket_);
  itree_->SetBranchAddress("u525wTnJacket", &omeasurement_.u525wTnJacket_);
  itree_->SetBranchAddress("u525wTvJacket", &omeasurement_.u525wTvJacket_);
  itree_->SetBranchAddress("u525wKpProcess", &omeasurement_.u525wKpProcess_);
  itree_->SetBranchAddress("u525wTnProcess", &omeasurement_.u525wTnProcess_);
  itree_->SetBranchAddress("u525wTvProcess", &omeasurement_.u525wTvProcess_);

  itree_->SetBranchAddress("martaState", &omeasurement_.martaState_);
  itree_->SetBranchAddress("martaPT03", &omeasurement_.martaPT03_);
  itree_->SetBranchAddress("martaPT05", &omeasurement_.martaPT05_);
  itree_->SetBranchAddress("martaPT01CO2", &omeasurement_.martaPT01CO2_);
  itree_->SetBranchAddress("martaPT02CO2", &omeasurement_.martaPT02CO2_);
  itree_->SetBranchAddress("martaPT03CO2", &omeasurement_.martaPT03CO2_);
  itree_->SetBranchAddress("martaPT04CO2", &omeasurement_.martaPT04CO2_);
  itree_->SetBranchAddress("martaPT05CO2", &omeasurement_.martaPT05CO2_);
  itree_->SetBranchAddress("martaPT06CO2", &omeasurement_.martaPT06CO2_);
  itree_->SetBranchAddress("martaTT02", &omeasurement_.martaTT02_);
  itree_->SetBranchAddress("martaTT01CO2", &omeasurement_.martaTT01CO2_);
  itree_->SetBranchAddress("martaTT02CO2", &omeasurement_.martaTT02CO2_);
  itree_->SetBranchAddress("martaTT03CO2", &omeasurement_.martaTT03CO2_);
  itree_->SetBranchAddress("martaTT04CO2", &omeasurement_.martaTT04CO2_);
  itree_->SetBranchAddress("martaTT05CO2", &omeasurement_.martaTT05CO2_);
  itree_->SetBranchAddress("martaTT06CO2", &omeasurement_.martaTT06CO2_);
  itree_->SetBranchAddress("martaTT07CO2", &omeasurement_.martaTT07CO2_);
  itree_->SetBranchAddress("martaSH05", &omeasurement_.martaSH05_);
  itree_->SetBranchAddress("martaSC01CO2", &omeasurement_.martaSC01CO2_);
  itree_->SetBranchAddress("martaSC02CO2", &omeasurement_.martaSC02CO2_);
  itree_->SetBranchAddress("martaSC03CO2", &omeasurement_.martaSC03CO2_);
  itree_->SetBranchAddress("martaSC05CO2", &omeasurement_.martaSC05CO2_);
  itree_->SetBranchAddress("martaSC06CO2", &omeasurement_.martaSC06CO2_);
  itree_->SetBranchAddress("martaDP01CO2", &omeasurement_.martaDP01CO2_);
  itree_->SetBranchAddress("martaDP02CO2", &omeasurement_.martaDP02CO2_);
  itree_->SetBranchAddress("martaDP03CO2", &omeasurement_.martaDP03CO2_);
  itree_->SetBranchAddress("martaDP04CO2", &omeasurement_.martaDP04CO2_);
  itree_->SetBranchAddress("martaDT02CO2", &omeasurement_.martaDT02CO2_);
  itree_->SetBranchAddress("martaDT03CO2", &omeasurement_.martaDT03CO2_);
  itree_->SetBranchAddress("martaST01CO2", &omeasurement_.martaST01CO2_);
  itree_->SetBranchAddress("martaST02CO2", &omeasurement_.martaST02CO2_);
  itree_->SetBranchAddress("martaST03CO2", &omeasurement_.martaST03CO2_);
  itree_->SetBranchAddress("martaST04CO2", &omeasurement_.martaST04CO2_);
  itree_->SetBranchAddress("martaFT01CO2", &omeasurement_.martaFT01CO2_);
  itree_->SetBranchAddress("martaSpeedSetpoint", &omeasurement_.martaSpeedSetpoint_);
  itree_->SetBranchAddress("martaFlowSetpoint", &omeasurement_.martaFlowSetpoint_);
  itree_->SetBranchAddress("martaTemperatureSetpoint", &omeasurement_.martaTemperatureSetpoint_);
  itree_->SetBranchAddress("martaStatus", &omeasurement_.martaStatus_);
  itree_->SetBranchAddress("martaSpeedSetpoint2", &omeasurement_.martaSpeedSetpoint2_);
  itree_->SetBranchAddress("martaFlowSetpoint2", &omeasurement_.martaFlowSetpoint2_);
  itree_->SetBranchAddress("martaTemperatureSetpoint2", &omeasurement_.martaTemperatureSetpoint2_);
  itree_->SetBranchAddress("martaAlarms0", &omeasurement_.martaAlarms_[0]);
  itree_->SetBranchAddress("martaAlarms1", &omeasurement_.martaAlarms_[1]);
  itree_->SetBranchAddress("martaAlarms2", &omeasurement_.martaAlarms_[2]);
  itree_->SetBranchAddress("martaAlarms3", &omeasurement_.martaAlarms_[3]);

  itree_->SetBranchAddress("agilentState", &omeasurement_.agilentState_);
  itree_->SetBranchAddress("agilentPumpState", &omeasurement_.agilentPumpState_);
  itree_->SetBranchAddress("agilentPumpStatus", &omeasurement_.agilentPumpStatus_);
  itree_->SetBranchAddress("agilentErrorCode", &omeasurement_.agilentErrorCode_);

  itree_->SetBranchAddress("leyboldState", &omeasurement_.leyboldState_);
  itree_->SetBranchAddress("leyboldPressure", &omeasurement_.leyboldPressure_);

  for (int i=0;i<3;++i) {
    sprintf(branchName, "nge103BState_%d", i+1);
    itree_->SetBranchAddress(branchName, &omeasurement_.nge103BState[i]);

    sprintf(branchName, "nge103BVoltage_%d", i+1);
    itree_->SetBranchAddress(branchName, &omeasurement_.nge103BVoltage[i]);

    sprintf(branchName, "nge103BCurrent_%d", i+1);
    itree_->SetBranchAddress(branchName, &omeasurement_.nge103BCurrent[i]);
  }

  for (int i=0;i<2;++i) {
    for (int j=0;j<10;++j) {
      sprintf(branchName, "KeithleyDAQ6510State_%d%02d", i+1, j+1);
      itree_->SetBranchAddress(branchName, &omeasurement_.keithleyState[i][j]);

      sprintf(branchName, "KeithleyDAQ6510Temperature_%d%02d", i+1, j+1);
      itree_->SetBranchAddress(branchName, &imeasurement_.keithleyTemperature[i][j]);
    }
  }
  
  ilogtree_ = ifile_->Get<TTree>("thermoLog");
  ilogtree_->SetBranchAddress("uTime", &log_.uTime);
  TDatime *dtlog = &log_.datime;
  ilogtree_->SetBranchAddress("datime", &dtlog);
  std::string *message = &log_.message;
  ilogtree_->SetBranchAddress("message", &message);

  ofile_ = new TFile(ofilename_.toStdString().c_str(), "RECREATE");

  otree_ = new TTree("thermoDAQ2", "thermoDAQ2");

  otree_->Branch("uTime", &omeasurement_.uTime, "uTime/i");
  otree_->Branch("datime", "TDatime", &omeasurement_.datime);

  otree_->Branch("u525wState", &omeasurement_.u525wState_, "u525wState/O");
  otree_->Branch("u525wTemperatureSetPoint", &omeasurement_.u525wTemperatureSetPoint_, "u525wTemperatureSetPoint/F");
  otree_->Branch("u525wTemperatureControlMode", &omeasurement_.u525wTemperatureControlMode_, "u525wTemperatureControlMode/O");
  otree_->Branch("u525wTemperatureControlEnabled", &omeasurement_.u525wTemperatureControlEnabled_, "u525wTemperatureControlEnabled/O");
  otree_->Branch("u525wCirculatorEnabled", &omeasurement_.u525wCirculatorEnabled_, "u525wCirculatorEnabled/O");
  otree_->Branch("u525wBathTemperature", &omeasurement_.u525wInternalTemperature_, "u525wBathTemperature/F");
  otree_->Branch("u525wInternalTemperature", &omeasurement_.u525wInternalTemperature_, "u525wInternalTemperature/F");
  otree_->Branch("u525wSigmaInternalTemperature", &omeasurement_.u525wSigmaInternalTemperature_, "u525wSigmaInternalTemperature/F");
  otree_->Branch("u525wProcessTemperature", &omeasurement_.u525wProcessTemperature_, "u525wProcessTemperature/F");
  otree_->Branch("u525wSigmaProcessTemperature", &omeasurement_.u525wSigmaProcessTemperature_, "u525wSigmaProcessTemperature/F");
  otree_->Branch("u525wReturnTemperature", &omeasurement_.u525wReturnTemperature_, "u525wReturnTemperature/F");
  otree_->Branch("u525wPumpPressure", &omeasurement_.u525wPumpPressure_, "u525wPumpPressure/F");
  otree_->Branch("u525wPower", &omeasurement_.u525wPower_, "u525wPower/I");
  otree_->Branch("u525wCWInletTemperature", &omeasurement_.u525wCWInletTemperature_, "u525wCWInletTemperature/F");
  otree_->Branch("u525wCWOutletTemperature", &omeasurement_.u525wCWOutletTemperature_, "u525wCWOutletTemperature/F");
  otree_->Branch("u525wAutoPID", &omeasurement_.u525wAutoPID_, "u525wAutoPID/O");
  otree_->Branch("u525wKpInternal", &omeasurement_.u525wKpInternal_, "u525wKpInternal/I");
  otree_->Branch("u525wTnInternal", &omeasurement_.u525wTnInternal_, "u525wTnInternal/F");
  otree_->Branch("u525wTvInternal", &omeasurement_.u525wTvInternal_, "u525wTvInternal/F");
  otree_->Branch("u525wKpJacket", &omeasurement_.u525wKpJacket_, "u525wKpJacket/I");
  otree_->Branch("u525wTnJacket", &omeasurement_.u525wTnJacket_, "u525wTnJacket/F");
  otree_->Branch("u525wTvJacket", &omeasurement_.u525wTvJacket_, "u525wTvJacket/F");
  otree_->Branch("u525wKpProcess", &omeasurement_.u525wKpProcess_, "u525wKpProcess/I");
  otree_->Branch("u525wTnProcess", &omeasurement_.u525wTnProcess_, "u525wTnProcess/F");
  otree_->Branch("u525wTvProcess", &omeasurement_.u525wTvProcess_, "u525wTvProcess/F");

  otree_->Branch("martaState", &omeasurement_.martaState_, "martaState/O");
  otree_->Branch("martaPT03", &omeasurement_.martaPT03_, "martaPT03/F");
  otree_->Branch("martaPT05", &omeasurement_.martaPT05_, "martaPT05/F");
  otree_->Branch("martaPT01CO2", &omeasurement_.martaPT01CO2_, "martaPT01CO2/F");
  otree_->Branch("martaPT02CO2", &omeasurement_.martaPT02CO2_, "martaPT02CO2/F");
  otree_->Branch("martaPT03CO2", &omeasurement_.martaPT03CO2_, "martaPT03CO2/F");
  otree_->Branch("martaPT04CO2", &omeasurement_.martaPT04CO2_, "martaPT04CO2/F");
  otree_->Branch("martaPT05CO2", &omeasurement_.martaPT05CO2_, "martaPT05CO2/F");
  otree_->Branch("martaPT06CO2", &omeasurement_.martaPT06CO2_, "martaPT06CO2/F");
  otree_->Branch("martaTT02", &omeasurement_.martaTT02_, "martaTT02/F");
  otree_->Branch("martaTT01CO2", &omeasurement_.martaTT01CO2_, "martaTT01CO2/F");
  otree_->Branch("martaTT02CO2", &omeasurement_.martaTT02CO2_, "martaTT02CO2/F");
  otree_->Branch("martaTT03CO2", &omeasurement_.martaTT03CO2_, "martaTT03CO2/F");
  otree_->Branch("martaTT04CO2", &omeasurement_.martaTT04CO2_, "martaTT04CO2/F");
  otree_->Branch("martaTT05CO2", &omeasurement_.martaTT05CO2_, "martaTT05CO2/F");
  otree_->Branch("martaTT06CO2", &omeasurement_.martaTT06CO2_, "martaTT06CO2/F");
  otree_->Branch("martaTT07CO2", &omeasurement_.martaTT07CO2_, "martaTT07CO2/F");
  otree_->Branch("martaSH05", &omeasurement_.martaSH05_, "martaSH05/F");
  otree_->Branch("martaSC01CO2", &omeasurement_.martaSC01CO2_, "martaSC01CO2/F");
  otree_->Branch("martaSC02CO2", &omeasurement_.martaSC02CO2_, "martaSC02CO2/F");
  otree_->Branch("martaSC03CO2", &omeasurement_.martaSC03CO2_, "martaSC03CO2/F");
  otree_->Branch("martaSC05CO2", &omeasurement_.martaSC05CO2_, "martaSC05CO2/F");
  otree_->Branch("martaSC06CO2", &omeasurement_.martaSC06CO2_, "martaSC06CO2/F");
  otree_->Branch("martaDP01CO2", &omeasurement_.martaDP01CO2_, "martaDP01CO2/F");
  otree_->Branch("martaDP02CO2", &omeasurement_.martaDP02CO2_, "martaDP02CO2/F");
  otree_->Branch("martaDP03CO2", &omeasurement_.martaDP03CO2_, "martaDP03CO2/F");
  otree_->Branch("martaDP04CO2", &omeasurement_.martaDP04CO2_, "martaDP04CO2/F");
  otree_->Branch("martaDT02CO2", &omeasurement_.martaDT02CO2_, "martaDT02CO2/F");
  otree_->Branch("martaDT03CO2", &omeasurement_.martaDT03CO2_, "martaDT03CO2/F");
  otree_->Branch("martaST01CO2", &omeasurement_.martaST01CO2_, "martaST01CO2/F");
  otree_->Branch("martaST02CO2", &omeasurement_.martaST02CO2_, "martaST02CO2/F");
  otree_->Branch("martaST03CO2", &omeasurement_.martaST03CO2_, "martaST03CO2/F");
  otree_->Branch("martaST04CO2", &omeasurement_.martaST04CO2_, "martaST04CO2/F");
  otree_->Branch("martaFT01CO2", &omeasurement_.martaFT01CO2_, "martaFT01CO2/F");
  otree_->Branch("martaSpeedSetpoint", &omeasurement_.martaSpeedSetpoint_, "martaSpeedSetpoint/F");
  otree_->Branch("martaFlowSetpoint", &omeasurement_.martaFlowSetpoint_, "martaFlowSetpoint/F");
  otree_->Branch("martaTemperatureSetpoint", &omeasurement_.martaTemperatureSetpoint_, "martaTemperatureSetpoint/F");
  otree_->Branch("martaStatus", &omeasurement_.martaStatus_, "martaStatus/s");
  otree_->Branch("martaSpeedSetpoint2", &omeasurement_.martaSpeedSetpoint2_, "martaSpeedSetpoint2/F");
  otree_->Branch("martaFlowSetpoint2", &omeasurement_.martaFlowSetpoint2_, "martaFlowSetpoint2/F");
  otree_->Branch("martaTemperatureSetpoint2", &omeasurement_.martaTemperatureSetpoint2_, "martaTemperatureSetpoint2/F");
  otree_->Branch("martaAlarms0", &omeasurement_.martaAlarms_[0], "martaAlarms0/s");
  otree_->Branch("martaAlarms1", &omeasurement_.martaAlarms_[1], "martaAlarms1/s");
  otree_->Branch("martaAlarms2", &omeasurement_.martaAlarms_[2], "martaAlarms2/s");
  otree_->Branch("martaAlarms3", &omeasurement_.martaAlarms_[3], "martaAlarms3/s");

  otree_->Branch("agilentState", &omeasurement_.agilentState_, "agilentState/O");
  otree_->Branch("agilentPumpState", &omeasurement_.agilentPumpState_, "agilentPumpState/O");
  otree_->Branch("agilentPumpStatus", &omeasurement_.agilentPumpStatus_, "agilentPumpStatus/i");
  otree_->Branch("agilentErrorCode", &omeasurement_.agilentErrorCode_, "agilentErrorCode/i");

  otree_->Branch("leyboldState", &omeasurement_.leyboldState_, "leyboldState/O");
  otree_->Branch("leyboldPressure", &omeasurement_.leyboldPressure_, "leyboldPressure/F");

  for (int i=0;i<3;++i) {
    sprintf(branchName, "nge103BState_%d", i+1);
    sprintf(branchLeafList, "nge103BState_%d/O", i+1);
    otree_->Branch(branchName, &omeasurement_.nge103BState[i], branchLeafList);

    sprintf(branchName, "nge103BVoltage_%d", i+1);
    sprintf(branchLeafList, "nge103BVoltage_%d/F", i+1);
    otree_->Branch(branchName, &omeasurement_.nge103BVoltage[i], branchLeafList);

    sprintf(branchName, "nge103BCurrent_%d", i+1);
    sprintf(branchLeafList, "nge103BCurrent_%d/F", i+1);
    otree_->Branch(branchName, &omeasurement_.nge103BCurrent[i], branchLeafList);
  }

  for (int i=0;i<2;++i) {
    for (int j=0;j<10;++j) {
      sprintf(branchName, "KeithleyDAQ6510State_%d%02d", i+1, j+1);
      sprintf(branchLeafList, "KeithleyDAQ6510State_%d%02d/O", i+1, j+1);
      otree_->Branch(branchName, &omeasurement_.keithleyState[i][j], branchLeafList);

      sprintf(branchName, "KeithleyDAQ6510Temperature_%d%02d", i+1, j+1);
      sprintf(branchLeafList, "KeithleyDAQ6510Temperature_%d%02d/F", i+1, j+1);
      otree_->Branch(branchName, &omeasurement_.keithleyTemperature[i][j], branchLeafList);

      sprintf(branchName, "KeithleyDAQ6510SigmaTemperature_%d%02d", i+1, j+1);
      sprintf(branchLeafList, "KeithleyDAQ6510SigmaTemperature_%d%02d/F", i+1, j+1);
      otree_->Branch(branchName, &omeasurement_.keithleySigmaTemperature[i][j], branchLeafList);
    }
  }

  ologtree_ = new TTree("thermoLog", "thermoLog");
  ologtree_->Branch("uTime", &log_.uTime, "uTime/i");
  ologtree_->Branch("datime", "TDatime", &log_.datime);
  ologtree_->Branch("message", &log_.message);

  nentries = ilogtree_->GetEntries();
  for (Long64_t ientry=0; ientry<nentries ; ientry++) {
    ilogtree_->GetEntry(ientry);
    ologtree_->Fill();
  }

  itree_->GetEntry(0);
  Long64_t previousUTime = omeasurement_.uTime;
  int windowSteps = 0;
  Long64_t firstEntry;

  nentries = itree_->GetEntries();
  for (Long64_t ientry=1; ientry<nentries ; ientry++) {
    itree_->GetEntry(ientry);

    Long64_t currentUTime = omeasurement_.uTime;

    windowSteps += currentUTime - previousUTime;

    if (windowSteps>=windowSize_) {
        firstEntry = ientry;
        break;
    }

    previousUTime = currentUTime;
  }

  // std::cout << "first entry: " << firstEntry << std::endl;

  for (Long64_t ientry=firstEntry; ientry<nentries ; ientry++) {
    itree_->GetEntry(ientry);

    if ((ientry%10)==0) {
      std::cout << "entry " << ientry << "/" << nentries << std::endl;
    }

    Long64_t entryUTime = omeasurement_.uTime;
    windowSteps = 0;
    Long64_t lastEntry;
    for (Long64_t jentry=ientry-1; jentry>=0 ; jentry--) {
        itree_->GetEntry(jentry);

        Long64_t currentUTime = omeasurement_.uTime;

        if (entryUTime-currentUTime>=windowSize_) {
            lastEntry = jentry;
            break;
        }
    }

    omeasurement_.u525wInternalTemperature_ = 0;
    omeasurement_.u525wSigmaInternalTemperature_ = 0;
    omeasurement_.u525wProcessTemperature_ = 0;
    omeasurement_.u525wSigmaProcessTemperature_ = 0;
    for (int i=0;i<2;++i) {
      for (int j=0;j<10;++j) {
        omeasurement_.keithleyTemperature[i][j] = 0;
        omeasurement_.keithleySigmaTemperature[i][j] = 0;
      }
    }

    Double_t steps = 0;
    for (Long64_t jentry=lastEntry; jentry<=ientry ; jentry++) {
      itree_->GetEntry(jentry);

      Long64_t currentUTime = omeasurement_.uTime;

      omeasurement_.u525wInternalTemperature_ += imeasurement_.u525wInternalTemperature_;
      omeasurement_.u525wProcessTemperature_ += imeasurement_.u525wProcessTemperature_;

      for (int i=0;i<2;++i) {
        for (int j=0;j<10;++j) {
          omeasurement_.keithleyTemperature[i][j] += imeasurement_.keithleyTemperature[i][j];
        }
      }
      steps++;
    }

    omeasurement_.u525wInternalTemperature_ /= steps;
    omeasurement_.u525wProcessTemperature_ /= steps;
    for (int i=0;i<2;++i) {
      for (int j=0;j<10;++j) {
          omeasurement_.keithleyTemperature[i][j] /= steps;
      }
    }

    steps = 0;
    for (Long64_t jentry=lastEntry; jentry<=ientry ; jentry++) {
      itree_->GetEntry(jentry);

      omeasurement_.u525wSigmaInternalTemperature_ += std::pow(imeasurement_.u525wInternalTemperature_ - omeasurement_.u525wInternalTemperature_, 2);
      omeasurement_.u525wSigmaProcessTemperature_ += std::pow(imeasurement_.u525wProcessTemperature_ - omeasurement_.u525wProcessTemperature_, 2);

      for (int i=0;i<2;++i) {
        for (int j=0;j<10;++j) {
          omeasurement_.keithleySigmaTemperature[i][j] += std::pow(imeasurement_.keithleyTemperature[i][j] - omeasurement_.keithleyTemperature[i][j], 2);
        }
      }
      steps++;
    }

    otree_->Fill();
  }

  ofile_->Write();
  delete ofile_;

  delete ifile_;
}
