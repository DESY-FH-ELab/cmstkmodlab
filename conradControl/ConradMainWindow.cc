
#include "ConradMainWindow.h"



///
///
///
ConradMainWindow::ConradMainWindow( QWidget* parent ) : QWidget( parent ) {

  // DO NOT CHANGE THE ORDER HERE!
  setupUi();

  // this block must go before setupSignalsAndSlots,
  // otherwise we call setupCardCommunication twice
  updateCommBox();
  isCommunication_ = false;
  setupCardCommunication();

  setupSignalsAndSlots();

}



///
///
///
void ConradMainWindow::setupCardCommunication( void ) {

  if( 0 == portComboBox_->currentText().toStdString().size() ) {
    QMessageBox::critical( 0, tr("[ConradMainWindow::setupCardCommunication]"), 
			   QString("No devices."), QMessageBox::Ok );
    return;
  }

  std::cout << "CONNECT: " <<  portComboBox_->currentText().toStdString() << std::endl; /////////////////////////////////

  isCommunication_ = true; /////////////
  connectCheckBox_->setChecked( true ); ///////////////

}



///
///
///
void ConradMainWindow::setupSignalsAndSlots( void ) {
  
  // connect channel buttons;
  // only those are controlling, the rest (panels, leds, camera)
  /// just "follow" through interconnection (see below)
  for( std::vector<ConradPushButton*>::const_iterator it = channelButtons_.begin(); it < channelButtons_.end(); ++it ) {
    (*it)->setControlling( true );
    QObject::connect( *it, SIGNAL(stateChanged()), this, SLOT(handleState()) );
  }

  // all channels / all panels buttons
  QObject::connect( allChannelsOnButton_,  SIGNAL(clicked()), this, SLOT(allChannelsOn()) );
  QObject::connect( allChannelsOffButton_, SIGNAL(clicked()), this, SLOT(allChannelsOff()) );
  QObject::connect( allPanelsOnButton_,  SIGNAL(clicked()), this, SLOT(allPanelsOn()) );
  QObject::connect( allPanelsOffButton_, SIGNAL(clicked()), this, SLOT(allPanelsOff()) );

  // re-connect to card when new port selected
  QObject::connect( portComboBox_, SIGNAL(currentIndexChanged(int)), this, SLOT(setupCardCommunication()) );


  // INTERCONNECT BUTTONS:
  // here we define which panels are connected to which channels on the card!!!
  
  // channels <> panels: 1<>1, 2<>2, 3<>3, 4<>4, 5<>5 (1..8)
  for( unsigned int i = 0; i < 5; ++i ) { // both directions
    QObject::connect( channelButtons_.at( i ), SIGNAL(clicked()), lightPanelButtons_.at( i ), SLOT(clickedOn()) );
    QObject::connect( lightPanelButtons_.at( i ), SIGNAL(clicked()), channelButtons_.at( i ), SLOT(clickedOn()) );
  }

  // LEDs: channel 6 (1..8)
  QObject::connect( ledButton_, SIGNAL(clicked()), channelButtons_.at( 5 ), SLOT(clickedOn()) );
  QObject::connect( channelButtons_.at( 5 ), SIGNAL(clicked()), ledButton_, SLOT(clickedOn()) );
  
  // Camera: channel 7 (1..8)
  QObject::connect( cameraButton_, SIGNAL(clicked()), channelButtons_.at( 6 ), SLOT(clickedOn()) );
  QObject::connect( channelButtons_.at( 6 ), SIGNAL(clicked()), cameraButton_, SLOT(clickedOn()) );

}



///
///
///
void ConradMainWindow::handleState( void ) {
  
  std::cout << "ConradMainWindow::handleChannels" << std::endl;
  for( std::vector<ConradPushButton*>::const_iterator it = channelButtons_.begin(); it < channelButtons_.end(); ++it ) {
    std::cout << "BUTTON: " << it - channelButtons_.begin() << " " << ((*it)->isActive()?"ACTIVE":"*") << std::endl;
  }

}



///
///
///
void ConradMainWindow::allChannelsOn( void ) {

  for( std::vector<ConradPushButton*>::const_iterator it = channelButtons_.begin(); it < channelButtons_.end(); ++it ) {
    (*it)->setActive( true );
  }

}



///
///
///
void ConradMainWindow::allChannelsOff( void ) {

  for( std::vector<ConradPushButton*>::const_iterator it = channelButtons_.begin(); it < channelButtons_.end(); ++it ) {
    (*it)->setActive( false );
  }

}



///
///
///
void ConradMainWindow::allPanelsOn( void ) {

  for( std::vector<ConradPushButton*>::const_iterator it = lightPanelButtons_.begin(); it < lightPanelButtons_.end(); ++it ) {
    (*it)->setActive( true );
  }

}



///
///
///
void ConradMainWindow::allPanelsOff( void ) {
  
  for( std::vector<ConradPushButton*>::const_iterator it = lightPanelButtons_.begin(); it < lightPanelButtons_.end(); ++it ) {
    (*it)->setActive( false );
  }

}



///
///
///
void ConradMainWindow::updateCommBox( void ) {

  QDir devDir( "/dev" );
  QStringList filters;
  //filters << "ttyUSB*"; // /dev/ttyUSB* !!!!!!!!!!!!!!
  filters << "ttyS*"; // /dev/ttyUSB* !!!!!!!!!!!!!!
  QStringList list = devDir.entryList( filters, QDir::System ); 
  
  for( QStringList::const_iterator it = list.begin(); it < list.end(); ++it ) {
    portComboBox_->addItem( *it );
  }

}



///
///
///
//void ConradMainWindow::setupUi(QMainWindow *ConradMainWindow) {

void ConradMainWindow::setupUi() {

  // fixed size button vectors
  lightPanelButtons_.resize( 5 );
  channelButtons_.resize( 8 );

  if (objectName().isEmpty())
    setObjectName(QString::fromUtf8("ConradMainWindow"));
  resize(384, 600);
  QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
  sizePolicy.setHorizontalStretch(0);
  sizePolicy.setVerticalStretch(0);
  //sizePolicy.setHeightForWidth(sizePolicy().hasHeightForWidth());
  setSizePolicy(sizePolicy);
  setMinimumSize(QSize(384, 600));
  setMaximumSize(QSize(384, 600));
  setStyleSheet(QString::fromUtf8(""));
  //  centralwidget = new QWidget(this);
  //  centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
  panelsGroupBox_ = new QGroupBox(this);
  panelsGroupBox_->setObjectName(QString::fromUtf8("panelsGroupBox_"));
  panelsGroupBox_->setGeometry(QRect(10, 20, 251, 331));
  QFont font;
  font.setPointSize(12);
  panelsGroupBox_->setFont(font);
  panelsGroupBox_->setAlignment(Qt::AlignCenter);

  QFont font1;
  font1.setPointSize(14);
  font1.setBold(true);
  font1.setWeight(75);

  lightPanelButtons_.at( 0 ) = new ConradPushButton(panelsGroupBox_);
  lightPanelButtons_.at( 0 )->setColorScheme( ConradPushButton::Black_White );
  lightPanelButtons_.at( 0 )->setObjectName(QString::fromUtf8("lightPanelButton1_"));
  lightPanelButtons_.at( 0 )->setGeometry(QRect(90, 30, 71, 71));
  lightPanelButtons_.at( 0 )->setFont(font1);

  lightPanelButtons_.at( 1 ) = new ConradPushButton(panelsGroupBox_);
  lightPanelButtons_.at( 1 )->setColorScheme( ConradPushButton::Black_White );
  lightPanelButtons_.at( 1 )->setObjectName(QString::fromUtf8("lightPanelButton2_"));
  lightPanelButtons_.at( 1 )->setGeometry(QRect(90, 110, 71, 71));
  lightPanelButtons_.at( 1 )->setFont(font1);

  lightPanelButtons_.at( 2 ) = new ConradPushButton(panelsGroupBox_);
  lightPanelButtons_.at( 2 )->setColorScheme( ConradPushButton::Black_White );
  lightPanelButtons_.at( 2 )->setObjectName(QString::fromUtf8("lightPanelButton3_"));
  lightPanelButtons_.at( 2 )->setGeometry(QRect(20, 30, 61, 151));
  lightPanelButtons_.at( 2 )->setFont(font1);

  lightPanelButtons_.at( 3 ) = new ConradPushButton(panelsGroupBox_);
  lightPanelButtons_.at( 3 )->setColorScheme( ConradPushButton::Black_White );
  lightPanelButtons_.at( 3 )->setObjectName(QString::fromUtf8("lightPanelButton4_"));
  lightPanelButtons_.at( 3 )->setGeometry(QRect(170, 30, 61, 151));
  lightPanelButtons_.at( 3 )->setFont(font1);

  lightPanelButtons_.at( 4 ) = new ConradPushButton(panelsGroupBox_);
  lightPanelButtons_.at( 4 )->setColorScheme( ConradPushButton::Black_White );
  lightPanelButtons_.at( 4 )->setObjectName(QString::fromUtf8("lightPanelButton5_"));
  lightPanelButtons_.at( 4 )->setGeometry(QRect(20, 190, 211, 51));
  lightPanelButtons_.at( 4 )->setFont(font1);

  allPanelsGroupBox_ = new QGroupBox(panelsGroupBox_);
  allPanelsGroupBox_->setObjectName(QString::fromUtf8("allPanelsGroupBox_"));
  allPanelsGroupBox_->setGeometry(QRect(20, 250, 211, 71));

  QFont font2;
  font2.setPointSize(10);
  allPanelsGroupBox_->setFont(font2);
  allPanelsGroupBox_->setAlignment(Qt::AlignCenter);
  allPanelsOnButton_ = new QPushButton(allPanelsGroupBox_);
  allPanelsOnButton_->setObjectName(QString::fromUtf8("allPanelsOnButton_"));
  allPanelsOnButton_->setGeometry(QRect(20, 20, 80, 41));
  allPanelsOffButton_ = new QPushButton(allPanelsGroupBox_);
  allPanelsOffButton_->setObjectName(QString::fromUtf8("allPanelsOffButton_"));
  allPanelsOffButton_->setGeometry(QRect(110, 20, 80, 41));
  otherGroupBox_ = new QGroupBox(this);
  otherGroupBox_->setObjectName(QString::fromUtf8("otherGroupBox_"));
  otherGroupBox_->setGeometry(QRect(10, 360, 251, 101));
  otherGroupBox_->setFont(font);
  otherGroupBox_->setAlignment(Qt::AlignCenter);
  ledButton_ = new ConradPushButton(otherGroupBox_);
  ledButton_->setColorScheme( ConradPushButton::Gray_Green );
  ledButton_->setObjectName(QString::fromUtf8("ledButton_"));
  ledButton_->setGeometry(QRect(30, 30, 81, 51));
  QFont font3;
  font3.setBold(true);
  font3.setWeight(75);
  ledButton_->setFont(font3);
  cameraButton_ = new ConradPushButton(otherGroupBox_);
  cameraButton_->setColorScheme( ConradPushButton::Gray_Green );
  cameraButton_->setObjectName(QString::fromUtf8("cameraButton_"));
  cameraButton_->setGeometry(QRect(140, 30, 81, 51));
  cameraButton_->setFont(font3);
  channelsGroupBox_ = new QGroupBox(this);
  channelsGroupBox_->setObjectName(QString::fromUtf8("channelsGroupBox_"));
  channelsGroupBox_->setGeometry(QRect(270, 20, 101, 531));
  channelsGroupBox_->setFont(font);
  channelsGroupBox_->setAlignment(Qt::AlignCenter);

  channelButtons_.at( 0 ) = new ConradPushButton(channelsGroupBox_);
  channelButtons_.at( 0 )->setObjectName(QString::fromUtf8("channelButtons_1"));
  channelButtons_.at( 0 )->setGeometry(QRect(20, 30, 61, 31));
  channelButtons_.at( 0 )->setFont(font3);

  channelButtons_.at( 1 ) = new ConradPushButton(channelsGroupBox_);
  channelButtons_.at( 1 )->setObjectName(QString::fromUtf8("channelButtons_2"));
  channelButtons_.at( 1 )->setGeometry(QRect(20, 70, 61, 31));
  channelButtons_.at( 1 )->setFont(font3);

  channelButtons_.at( 2 ) = new ConradPushButton(channelsGroupBox_);
  channelButtons_.at( 2 )->setObjectName(QString::fromUtf8("channelButtons_3"));
  channelButtons_.at( 2 )->setGeometry(QRect(20, 110, 61, 31));
  channelButtons_.at( 2 )->setFont(font3);

  channelButtons_.at( 3 ) = new ConradPushButton(channelsGroupBox_);
  channelButtons_.at( 3 )->setObjectName(QString::fromUtf8("channelButtons_4"));
  channelButtons_.at( 3 )->setGeometry(QRect(20, 150, 61, 31));
  channelButtons_.at( 3 )->setFont(font3);

  channelButtons_.at( 4 ) = new ConradPushButton(channelsGroupBox_);
  channelButtons_.at( 4 )->setObjectName(QString::fromUtf8("channelButtons_5"));
  channelButtons_.at( 4 )->setGeometry(QRect(20, 190, 61, 31));
  channelButtons_.at( 4 )->setFont(font3);

  channelButtons_.at( 5 ) = new ConradPushButton(channelsGroupBox_);
  channelButtons_.at( 5 )->setObjectName(QString::fromUtf8("channelButtons_6"));
  channelButtons_.at( 5 )->setGeometry(QRect(20, 270, 61, 31));
  channelButtons_.at( 5 )->setFont(font3);

  channelButtons_.at( 6 ) = new ConradPushButton(channelsGroupBox_);
  channelButtons_.at( 6 )->setObjectName(QString::fromUtf8("channelButtons_7"));
  channelButtons_.at( 6 )->setGeometry(QRect(20, 230, 61, 31));
  channelButtons_.at( 6 )->setFont(font3);

  channelButtons_.at( 7 ) = new ConradPushButton(channelsGroupBox_);
  channelButtons_.at( 7 )->setObjectName(QString::fromUtf8("channelButtons_8"));
  channelButtons_.at( 7 )->setGeometry(QRect(20, 310, 61, 31));
  channelButtons_.at( 7 )->setFont(font3);

  allChannelsGroupBox_ = new QGroupBox(channelsGroupBox_);
  allChannelsGroupBox_->setObjectName(QString::fromUtf8("allChannelsGroupBox_"));
  allChannelsGroupBox_->setGeometry(QRect(10, 370, 81, 151));
  allChannelsGroupBox_->setFont(font2);
  allChannelsOffButton_ = new QPushButton(allChannelsGroupBox_);
  allChannelsOffButton_->setObjectName(QString::fromUtf8("allChannelsOffButton_"));
  allChannelsOffButton_->setGeometry(QRect(10, 90, 61, 41));
  allChannelsOnButton_ = new QPushButton(allChannelsGroupBox_);
  allChannelsOnButton_->setObjectName(QString::fromUtf8("allChannelsOnButton_"));
  allChannelsOnButton_->setGeometry(QRect(10, 30, 61, 41));
  portGroupBox_ = new QGroupBox(this);
  portGroupBox_->setObjectName(QString::fromUtf8("portGroupBox_"));
  portGroupBox_->setGeometry(QRect(10, 469, 251, 81));
  portGroupBox_->setFont(font);
  portGroupBox_->setAlignment(Qt::AlignCenter);
  portComboBox_ = new QComboBox(portGroupBox_);
  portComboBox_->setObjectName(QString::fromUtf8("portComboBox_"));
  portComboBox_->setGeometry(QRect(20, 30, 111, 31));
  connectCheckBox_ = new QCheckBox(portGroupBox_);
  connectCheckBox_->setObjectName(QString::fromUtf8("connectCheckBox_"));
  connectCheckBox_->setGeometry(QRect(160, 31, 81, 31));
  connectCheckBox_->setChecked( false );
  connectCheckBox_->setEnabled( false ); // display only
  QFont font4;
  font4.setPointSize(11);
  connectCheckBox_->setFont(font4);
  //  setCentralWidget(centralwidget);
  //  menubar = new QMenuBar(ConradMainWindow);
  //  menubar->setObjectName(QString::fromUtf8("menubar"));
  //  menubar->setGeometry(QRect(0, 0, 384, 24));
  //  setMenuBar(menubar);
  //  statusbar = new QStatusBar(ConradMainWindow);
  //  statusbar->setObjectName(QString::fromUtf8("statusbar"));
  //  setStatusBar(statusbar);

  retranslateUi();

  //  QMetaObject::connectSlotsByName(ConradMainWindow);

} // setupUi


///
///
///
//void ConradMainWindow::retranslateUi(QMainWindow *ConradMainWindow) {
void ConradMainWindow::retranslateUi() {

  setWindowTitle(QApplication::translate("ConradMainWindow", "ConradControl", 0, QApplication::UnicodeUTF8));
  panelsGroupBox_->setTitle(QApplication::translate("ConradMainWindow", "Panels", 0, QApplication::UnicodeUTF8));
  for( unsigned int i = 0; i < 5; ++i ) {
    lightPanelButtons_.at( i )->setText(QApplication::translate("MainWindow",
      (QString("%1").arg(i+1)).toStdString().c_str(), 0, QApplication::UnicodeUTF8));
  }

  allPanelsGroupBox_->setTitle(QApplication::translate("ConradMainWindow", "All panels", 0, QApplication::UnicodeUTF8));
  allPanelsOnButton_->setText(QApplication::translate("ConradMainWindow", "ON", 0, QApplication::UnicodeUTF8));
  allPanelsOffButton_->setText(QApplication::translate("ConradMainWindow", "OFF", 0, QApplication::UnicodeUTF8));
  otherGroupBox_->setTitle(QApplication::translate("ConradMainWindow", "Other", 0, QApplication::UnicodeUTF8));
  ledButton_->setText(QApplication::translate("ConradMainWindow", "LED", 0, QApplication::UnicodeUTF8));
  cameraButton_->setText(QApplication::translate("ConradMainWindow", "Camera", 0, QApplication::UnicodeUTF8));
  channelsGroupBox_->setTitle(QApplication::translate("ConradMainWindow", "Channels", 0, QApplication::UnicodeUTF8));
  for( unsigned int i = 0; i < 8; ++i ) {
    channelButtons_.at( i )->setText(QApplication::translate("MainWindow",
      (QString("%1").arg(i+1)).toStdString().c_str(), 0, QApplication::UnicodeUTF8));
  }

  allChannelsGroupBox_->setTitle(QApplication::translate("ConradMainWindow", "All channels", 0, QApplication::UnicodeUTF8));
  allChannelsOffButton_->setText(QApplication::translate("ConradMainWindow", "OFF", 0, QApplication::UnicodeUTF8));
  allChannelsOnButton_->setText(QApplication::translate("ConradMainWindow", "ON", 0, QApplication::UnicodeUTF8));
  portGroupBox_->setTitle(QApplication::translate("ConradMainWindow", "Comm", 0, QApplication::UnicodeUTF8));
  connectCheckBox_->setText(QApplication::translate("ConradMainWindow", "Connect", 0, QApplication::UnicodeUTF8));
  //  Q_UNUSED(ConradMainWindow);

} // retranslateUi






///
///
///
ConradPushButton::ConradPushButton( QWidget* parent ) : QPushButton( parent ) {

  // default color scheme is gray_green
  colorScheme_ = ConradPushButton::Gray_Green;
  
  isActive_ = false;
  updateColor();

  QObject::connect( this, SIGNAL(clicked()), this, SLOT(clickedOn()) );

}



///
///
///
void ConradPushButton::clickedOn( void ) {

  // toggle
  isActive_ = !isActive_;
  updateColor();
  if( isControlling_ ) emit stateChanged();

}



///
///
///
void ConradPushButton::updateColor( void ) {

  if( ConradPushButton::Gray_Green == colorScheme_ ) {
    if( isActive_ ) setStyleSheet("background-color: rgb(0, 255, 0); color: rgb(0, 0, 0)");
    else setStyleSheet("background-color: rgb(240, 240, 240); color: rgb(0, 0, 0)");
  }

  else {
    if( isActive_ ) setStyleSheet("background-color: rgb(252, 250, 210); color: rgb(0, 0, 0)");
    else setStyleSheet("background-color: rgb(0, 0, 0); color: rgb(255, 255, 255)");
  }

}
