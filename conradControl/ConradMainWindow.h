
#ifndef CONRADCONTROL_H
#define CONRADCONTROL_H

#include <vector>
#include <iostream>

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QCheckBox>
#include <QtGui/QComboBox>
#include <QtGui/QGroupBox>
#include <QtGui/QMainWindow>
#include <QtGui/QMenuBar>
#include <QtGui/QPushButton>
#include <QtGui/QStatusBar>
#include <QtGui/QWidget>
#include <QDir>
#include <QTimer>
#include <QMessageBox>

#include "devices/Conrad/ConradController.h"



///
/// QPushButton which remembers a boolean state
/// and has some color functionality
///
/// if the button is "controlling" it
/// emits stateChanged() when clickedAction() is received;
/// such that we can make other buttons just "followers"
///
class ConradPushButton : public QPushButton {

 Q_OBJECT

 public:

  enum ColorScheme { Black_White, Gray_Green };
  
  ConradPushButton( QWidget* parent = 0 );
  void setColorScheme( ConradPushButton::ColorScheme scheme ) { colorScheme_ = scheme; updateColor(); }
  bool isActive( void ) { return isActive_; }
  void setActive( bool a ) { if( a != isActive_ ) click(); }
  bool isControlling( void ) { return isControlling_; }
  void setControlling( bool c ) { isControlling_ = c; }

 public slots:
  void clickedAction( void );

 signals:
  void stateChanged( void );

 private:
  void updateColor( void );

  bool isControlling_;
  bool isActive_;
  ConradPushButton::ColorScheme colorScheme_;

};




///
///
///
class ConradMainWindow : public QWidget {

Q_OBJECT

public:
  ConradMainWindow( QWidget* parent = 0 );
  
public slots:
  void handleState( void );
  void allChannelsOn( void );
  void allChannelsOff( void );
  void allPanelsOn( void );
  void allPanelsOff( void );

private slots:
  void updateCommBox( void );
  void setupCardCommunication( void );

private:
  void setupSignalsAndSlots( void );
  void disableAllItems( void );
  void setupUi( void );
  void retranslateUi( void );

  ConradController* conradController_;
  bool isCommunication_;

  QGroupBox *panelsGroupBox_;
  std::vector<ConradPushButton*> lightPanelButtons_;

  QGroupBox *allPanelsGroupBox_;
  QPushButton *allPanelsOnButton_;
  QPushButton *allPanelsOffButton_;

  QGroupBox *otherGroupBox_;
  ConradPushButton *ledButton_;
  ConradPushButton *cameraButton_;

  QGroupBox *channelsGroupBox_;
  std::vector<ConradPushButton*> channelButtons_;
  QGroupBox *allChannelsGroupBox_;
  QPushButton *allChannelsOffButton_;
  QPushButton *allChannelsOnButton_;

  QGroupBox *portGroupBox_;
  QComboBox *portComboBox_;
  QCheckBox *connectCheckBox_;
  QPushButton *updateButton_;

};



#endif 
