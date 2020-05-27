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
  virtual ~AssemblyParametersView() {}

  bool       has(const std::string&) const;
  QLineEdit* get(const std::string&) const;

  void copy_values(const std::map<std::string, double>&);

  void setText(const std::string&, const double);

  std::map<std::string, std::string> entries_map() const;

 protected:

  QWidget* dime_wid_;
  QWidget* posi_wid_;
  QWidget* dist_wid_;
  QWidget* imag_wid_;

  QPushButton* paramIO_button_read_;
  QPushButton* paramIO_button_write_;

  std::map<std::string, QLineEdit*> map_lineEdit_;

 public slots:

  void  read_parameters();
  void write_parameters();

  void transmit_entries();

  void display_infoTab();

 signals:

  void read_from_file_request(const QString&);
  void  write_to_file_request(const QString&);

  void entries(const std::map<std::string, std::string>&);
};
// ===========================================================================

#endif // ASSEMBLYPARAMETERSVIEW_H
