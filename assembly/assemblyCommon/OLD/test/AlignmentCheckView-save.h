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

#ifndef ALIGNMENTCHECKVIEW_H
#define ALIGNMENTCHECKVIEW_H

#include <AssemblyAssembly.h>
#include <AlignmentCheck.h>



#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QCheckBox>
#include <QLabel>

#include <AssemblyUEyeView.h>
#include <AssemblyObjectFinderPatRecWidget.h>

#include <QWidget>
#include <QString>
#include <QScrollArea>
#include <QKeyEvent>
#include <QCheckBox>
#include <QRadioButton>
#include <QPushButton>
#include <QLineEdit>

#include <opencv2/opencv.hpp>

class AlignmentCheckView : public QWidget
{
 Q_OBJECT

 public:

  explicit AlignmentCheckView(const AlignmentCheck* const, QWidget* parent=nullptr);
  virtual ~AlignmentCheckView() {}

 protected:

  QCheckBox* smartMove_checkbox_;

  QWidget* wid_CalibRotStage_;
  QWidget* wid_PSSAlignm_;
  QWidget* wid_PSSToSpacers_;
  QWidget* wid_PSSToPSP_;
  QWidget* wid_PSToBasep_;

  uint assembly_step_N_;
};
// ====================================================================================================

class AlignmentCheckActionWidget : public QWidget
{
 Q_OBJECT

 public:

  explicit AlignmentCheckActionWidget(QWidget* parent=nullptr);
  virtual ~AlignmentCheckActionWidget() {}

  QHBoxLayout* layout()   const { return layout_; }

  QLabel*      label()    const { return label_; }
  QPushButton* button()   const { return button_; }
  QCheckBox*   checkbox() const { return checkbox_; }

  void connect_action(const QObject*, const char*, const char*);

 protected:

  QHBoxLayout* layout_;

  QLabel*      label_;
  QPushButton* button_;
  QCheckBox*   checkbox_;

  const QObject* qobject_;
  const char* start_slot_;
  const char* stop_signal_;
  QPushButton*  alignm_exeac_pusbu_;
 public slots:

  void disable(const bool b=true);
  void disable(const int);

  void   reset_action();
  void   start_action();
  void disable_action();

 signals:

  void action_request();
};
// ====================================================================================================

class AlignmentCheckTextWidget : public QWidget
{
 Q_OBJECT

 public:

  explicit AlignmentCheckTextWidget(QWidget* parent=nullptr);
  virtual ~AlignmentCheckTextWidget() {}

  QHBoxLayout* layout()   const { return layout_; }

  QLabel*      label()    const { return label_; }
  QLabel*      text()     const { return text_; }
  QCheckBox*   checkbox() const { return checkbox_; }

 protected:

  QHBoxLayout* layout_;

  QLabel*    label_;
  QLabel*    text_;
  QCheckBox* checkbox_;

 public slots:

  void disable(const bool b=true);
  void disable(const int);

 signals:
};
// ====================================================================================================

#endif // ASSEMBLYASSEMBLYVIEW_H
