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

#ifndef ASSEMBLYPARAMETERSVIEW_H
#define ASSEMBLYPARAMETERSVIEW_H

#include <LStepExpressMotionManager.h>
#include <AssemblyMultiPickupTesterWidget.h>
#include <AssemblyPositionsRegistryWidget.h>
#include <ApplicationConfig.h>

#include <string>
#include <map>

#include <QWidget>
#include <QPushButton>
#include <QLineEdit>

class AssemblyParametersView : public QWidget
{
 Q_OBJECT

 public:

  explicit AssemblyParametersView(QWidget* parent=nullptr);
  // virtual ~AssemblyParametersView() {};
  ~AssemblyParametersView();

  bool       has(const std::string&) const;
  QLineEdit* get(const std::string&) const;

  void setText(const std::string&, const double);
  void setText(const std::string&, const std::string);

  std::map<std::string, std::string> entries_map() const;

 protected:

  QWidget* dime_wid_;
  QWidget* posi_wid_;
  QWidget* dist_wid_;
  QWidget* imag_wid_;

  QPushButton* paramIO_button_read_;
  QPushButton* paramIO_button_write_;

  QPushButton *button_moveAbsRefPos1_, *button_moveAbsRefPos2_, *button_moveAbsRefPos4_, *button_moveAbsRefPos5_;
  QPushButton *button_moveRelRefDist1_, *button_moveRelRefDist2_, *button_moveRelRefDist3_, *button_moveRelRefDist4_, *button_moveRelRefDist5_, *button_moveRelRefDist6_, *button_moveRelRefDist7_, *button_moveRelRefDist8_, *button_moveRelRefDist9_, *button_moveRelRefDist10_, *button_moveRelRefDist11_, *button_moveRelRefDist12_, *button_moveRelRefDist13_;

  std::map<std::string, QLineEdit*> map_lineEdit_;

  ApplicationConfig* config_;

 public slots:

  void  read_parameters();
  void write_parameters();

  void copy_values();

  void transmit_entries();

  void display_infoTab();

  void Dump_UserValues_toDBlogfile(const QString&);

  void overwriteParameter(const QString&);

  void moveToAbsRefPos1() {emit click_moveToAbsRefPos(1);};
  void moveToAbsRefPos2() {emit click_moveToAbsRefPos(2);};
  void moveToAbsRefPos4() {emit click_moveToAbsRefPos(4);};
  void moveToAbsRefPos5() {emit click_moveToAbsRefPos(5);};
  void askConfirmMoveToAbsRefPoint(int);

  void moveByRelRefDist1() {emit click_moveByRelRefDist(1);};
  void moveByRelRefDist2() {emit click_moveByRelRefDist(2);};
  void moveByRelRefDist3() {emit click_moveByRelRefDist(3);};
  void moveByRelRefDist4() {emit click_moveByRelRefDist(4);};
  void moveByRelRefDist5() {emit click_moveByRelRefDist(5);};
  void moveByRelRefDist6() {emit click_moveByRelRefDist(6);};
  void moveByRelRefDist7() {emit click_moveByRelRefDist(7);};
  void moveByRelRefDist8() {emit click_moveByRelRefDist(8);};
  void moveByRelRefDist9() {emit click_moveByRelRefDist(9);};
  void moveByRelRefDist10() {emit click_moveByRelRefDist(10);};
  void moveByRelRefDist11() {emit click_moveByRelRefDist(11);};
  void moveByRelRefDist12() {emit click_moveByRelRefDist(12);};
  void moveByRelRefDist13() {emit click_moveByRelRefDist(13);};
  void askConfirmMoveByRelRefDist(int);

 signals:

  void read_from_file_request(const QString&);
  void  write_to_file_request(const QString&);
  void append_to_file_request(const QString&);

  void entries(const std::map<std::string, std::string>&);

  void click_moveToAbsRefPos(int);
  void request_moveToAbsRefPosition(double,double,double,double);
  void click_moveByRelRefDist(int);
  void request_moveByRelRefDistance(double,double,double,double);
};
// ===========================================================================

#endif // ASSEMBLYPARAMETERSVIEW_H
