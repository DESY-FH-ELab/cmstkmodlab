/********************************************************************************
 ** Form generated from reading ui file 'defoYg7928.ui'
 **
 ** Created: Tue Jul 5 11:58:11 2011
 **      by: Qt User Interface Compiler version 4.4.3
 **
 ** WARNING! All changes made in this file will be lost when recompiling ui file!
 ********************************************************************************/

#ifndef DEFOYG7928_H
#define DEFOYG7928_H

#include <iostream>
#include <string>
#include <fstream>

//#include <boost/archive/text_oarchive.hpp>
//#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/binary_oarchive.hpp>
#include <boost/archive/binary_iarchive.hpp>

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QComboBox>
#include <QtGui/QDoubleSpinBox>
#include <QtGui/QGroupBox>
#include <QtGui/QLCDNumber>
#include <QtGui/QLabel>
#include <QtGui/QMainWindow>
#include <QtGui/QPlainTextEdit>
#include <QtGui/QPushButton>
#include <QtGui/QRadioButton>
#include <QtGui/QSpinBox>
#include <QtGui/QStatusBar>
#include <QtGui/QTabWidget>
#include <QtGui/QTableView>
#include <QtGui/QToolButton>
#include <QtGui/QWidget>
#include <QDateTime>
#include <QFileInfo>
#include <QTimer>
#include <QMessageBox>
#include <QInputDialog>
#include <QCheckBox>
//#include <QMetaEnum>

#include "DefoSurfacePlot.h"
#include "DefoSchedule.h"
#include "DefoRawImage.h"
#include "DefoPoint.h"
#include "DefoRecoImage.h"
#include "DefoRecoSurface.h"
#include "DefoImageLabel.h"
#include "DefoConfigReader.h"
#include "DefoCamHandler.h"

#include "qwt_plot.h"


QT_BEGIN_NAMESPACE


///
///
///
class DefoMainWindow : public QWidget {

  Q_OBJECT

 public:
  DefoMainWindow( QWidget *parent = 0 );
  ~DefoMainWindow() {}

  void setupUi( void );
  void retranslateUi( void );
  void setupSignalsAndSlots( void );

  // use the same enums as EOS550D class
  //  #include "../devices/Canon/eosParameters.h"

  // !!!!!!!!!
  // ABOVE INCLUDE DOESNT WORK, since moc doesnt expand includes in this version!!!
  // so we have to include them in plain text... shit
  // !!!!!!!!!

  // all possible speeds, correct order, so we can call them by index;
  // last entry must be SINVALID!
  enum ShutterSpeed { 
    BULB, L30, L25, L20, L15, L13, L10, L8, L6, L5, L4, L3_2, L2_5, L2, 
    L1_6, L1_3, L1, L0_8, L0_6, L0_5, L0_4, L0_3, S4, S5, S6, S8, S10, 
    S13, S15, S20, S25, S30, S40, S50, S60, S80, S100, S125, S160, S200, 
    S250, S320, S400, S500, S640, S800, S1000, S1250, S1600, S2000, S2500, 
    S3200, S4000, SINVALID 
  };
  
  
  // all possible apertures (for current lens);
  // correct order, so we can call them by index;
  // last entry must be FINVALID!
  enum Aperture { 
    F5_6, F6_3, F7_1, F8, F9, F10, F11, F13, F14, F16, F18, F20, F22, F25,
    F29, F32, F36, FINVALID
  };
  
  
  // all possible iso values, correct order, so we can call them by index;
  // last entry must be ISOINVALID!
  enum Iso {
    ISOAUTO, ISO100, ISO200, ISO400, ISO800, ISO1600, ISO3200, ISO6400,
    ISOINVALID
  };

  Q_ENUMS( ShutterSpeed )
  Q_ENUMS( Aperture )
  Q_ENUMS( Iso )

 public slots:
  void handleAction( DefoSchedule::scheduleItem item );
  void startPolling( void );
  void stopPolling( void );
  void pausePolling( void );
  void manualFileRef( void );
  void manualFileDefo( void );
  void editMeasurementId( void );
  void defaultMeasurementId( void );
  void editBaseFolder( void );
  void receiveArea( DefoArea area );
  void deleteCurrentArea( void );
  void toggleAreaDisplay( bool isChecked );
  void toggleIndicesDisplay( bool isChecked );
  void togglePointSquareDisplay( bool isChecked );
  void loadImageFromCamera( void );
  void handleCameraAction( DefoCamHandler::Action );
  void loadImageFromFile( void );
  void cameraEnabledButtonToggled( bool );
  void writeParameters( void );

 signals:
  void pollAction( void );
  void imagelabelRefreshAreas( std::vector<DefoArea> );
  void imagelabelDisplayAreas( bool );
  void imagelabelRefreshIndices( std::vector<DefoPoint> );
  void imagelabelDisplayIndices( bool );
  void imagelabelRefreshPointSquares( std::vector<DefoSquare> );
  void imagelabelDisplayPointSquares( bool );

 private:

  void fillComboBoxes( void );
  void readCameraParametersFromCfgFile( void );

  unsigned int debugLevel_;

  DefoCamHandler camHandler_; // camera steering
  bool isCameraEnabled_; // flag for camera operation on/off

  DefoSchedule* schedule_; // the measurement schedule
  QTimer* pollingDelay_; // timer to allow for GUI update when polling

  DefoRecoImage defoRecoImage_; // image reconstruction instance
  DefoRecoSurface defoRecoSurface_; // surface reconstruction instance

  std::pair<DefoPointCollection,DefoRawImage> referenceImageOutput_; 
  std::pair<DefoPointCollection,DefoRawImage> defoImageOutput_;

  DefoSurfacePlot* surfacePlot_; // surface output plot on "offline" tab

  bool isManual_; // used for switching handleAction when manual buttons pressed
  bool isPolling_; // volatile switch indicating if schedule is being processed
  bool isRefImage_; // ref image has been taken?

  QString measurementId_;
  QString baseFolderName_;
  QString currentFolderName_;

  std::string filepathPlaced_; // where the camHandler puts the file

  std::vector<DefoArea> areas_;

  QWidget *centralwidget;
  QTabWidget *mainTabWidget_;
  QWidget *online_tab;
  QGroupBox *rawimageGroupBox_;
  DefoImageLabel *rawimageLabel_;
  QGroupBox *areaGroupBox_;
  QPushButton *areaNewButton_;
  QPushButton *areaDeleteButton_;
  QComboBox *areaComboBox_;
  QLabel *areaLabel_;
  QGroupBox *displayGroupBox_;
  QRadioButton *displayAreasButton_;
  QRadioButton *displaySplinesButton_;
  QRadioButton *displayRecoitemButton_;
  QRadioButton *displayIndicesButton_;
  QRadioButton *displayCoordsButton_;

  QGroupBox *refreshGroupBox_;
  QPushButton *refreshCameraButton_;
  QPushButton *refreshFileButton_;
  QGroupBox *manualGroupBox_;
  QPushButton *manualREFButton_;
  QPushButton *manualDEFOButton_;
/*   QPushButton *manualFILEREFButton_; */
/*   QPushButton *manualFILEDEFOButton_; */
  QPushButton *manualTEMPButton_;
  QToolButton *rawimageZoomButton_;
  QToolButton *rawimageHistoButton_;
  QGroupBox *statusGroupBox_;
  QCheckBox *refCheckBox_;

  QGroupBox *scheduleGroupBox_;
  QPushButton *scheduleClearButton_;
  QPushButton *scheduleVerifyButton_;
  QPushButton *scheduleLoadButton_;
  QPushButton *scheduleSaveButton_;
  QTableView *scheduleTableview_;
  QPushButton *scheduleStopButton_;
  QPushButton *scheduleStartButton_;
  QPushButton *schedulePauseButton_;
  QGroupBox *measurementidGroupBox_;
  QPushButton *measurementidEditButton_;
  QPushButton *measurementidDefaultButton_;
  QPlainTextEdit *measurementidTextedit_;
  QPushButton *quitButton_;
  QGroupBox *imageinfoGroupBox_;
  QPlainTextEdit *imageinfoTextedit_;
  QWidget *tab_2;
  QGroupBox *historyGroupBox_;
  QwtPlot *historyQwtPlot_;
  QToolButton *historyZoomButton_;
  QGroupBox *historyDisplayGroupBox_;
  QRadioButton *historyDisplay0Button_;
  QRadioButton *historyDisplay1Button_;
  QRadioButton *historyDisplay2Button_;
  QRadioButton *historyDisplay3Button_;
  QRadioButton *historyDisplay4Button_;
  QRadioButton *historyDisplay5Button_;
  QRadioButton *historyDisplay6Button_;
  QRadioButton *historyDisplay7Button_;
  QRadioButton *historyDisplay8Button_;
  QRadioButton *historyDisplay9Button_;
  QRadioButton *historyDisplayRefButton_;
  QRadioButton *historyDisplayBathButton_;
  QPushButton *historyClearButton_;
  QPushButton *historySaveButton_;
  QGroupBox *chillerGroupBox_;
  QLCDNumber *chillerTargetLcd_;
  QLabel *chillerTargetLabel_;
  QLCDNumber *chillerPowerLcd_;
  QLabel *chillerPowerLabel_;
  QLCDNumber *chillerBathLcd_;
  QLabel *chillerBathLabel_;
  QLabel *chillerBathMaxLabel_;
  QLCDNumber *chillerBathMaxLcd_;
  QLCDNumber *chillerBathMinLcd_;
  QLabel *chillerBathMinLabel_;
  QPushButton *chillerClearButton_;
  QGroupBox *sensorsGroupBox_;
  QLCDNumber *sensorsLcd5_;
  QLCDNumber *sensorsLcd8_;
  QLCDNumber *sensorsLcd4_;
  QLCDNumber *sensorsLcd1_;
  QLCDNumber *sensorsLcd3_;
  QLCDNumber *sensorsLcd6_;
  QLCDNumber *sensorsLcd7_;
  QLCDNumber *sensorsLcd0_;
  QLCDNumber *sensorsLcd2_;
  QLCDNumber *sensorsLcd9_;
  QLabel *sensorsSensor0Label_;
  QLabel *sensorsSensor1Label_;
  QLabel *sensorsSensor2Label_;
  QLabel *sensorsSensor3Label_;
  QLabel *sensorsSensor4Label_;
  QLabel *sensorsSensor6Label_;
  QLabel *sensorsSensor7Label_;
  QLabel *sensorsSensor5Label_;
  QLabel *sensorsSensor9Label_;
  QLabel *sensorsSensor8Label_;
  QLCDNumber *sensorsLcdMin5_;
  QLCDNumber *sensorsLcdMax5_;
  QLabel *sensorsSensor5MinLabel_;
  QLabel *sensorsSensor5MaxLabel_;
  QLabel *sensorsSensor6MinLabel_;
  QLCDNumber *sensorsLcdMax6_;
  QLabel *sensorsSensor6MaxLabel_;
  QLCDNumber *sensorsLcdMin6_;
  QLCDNumber *sensorsLcdMin7_;
  QLabel *sensorsSensor7MinLabel_;
  QLCDNumber *sensorsLcdMax7_;
  QLabel *sensorsSensor7MaxLabel_;
  QLCDNumber *sensorsLcdMin8_;
  QLabel *sensorsSensor8MinLabel_;
  QLCDNumber *sensorsLcdMax8_;
  QLabel *sensorsSensor8MaxLabel_;
  QLCDNumber *sensorsLcdMin9_;
  QLabel *sensorsSensor9MinLabel_;
  QLCDNumber *sensorsLcdMax9_;
  QLabel *sensorsSensor9MaxLabel_;
  QLCDNumber *sensorsLcdMin3_;
  QLCDNumber *sensorsLcdMin2_;
  QLabel *sensorsSensor2MaxLabel_;
  QLCDNumber *sensorsLcdMin1_;
  QLabel *sensorsSensor4MinLabel_;
  QLabel *sensorsSensor0MaxLabel_;
  QLabel *sensorsSensor1MaxLabel_;
  QLCDNumber *sensorsLcdMin4_;
  QLabel *sensorsSensor3MaxLabel_;
  QLabel *sensorsSensor2MinLabel_;
  QLabel *sensorsSensor0MinLabel_;
  QLCDNumber *sensorsLcdMax4_;
  QLabel *sensorsSensor4MaxLabel_;
  QLCDNumber *sensorsLcdMax2_;
  QLabel *sensorsSensor3MinLabel_;
  QLCDNumber *sensorsLcdMax0_;
  QLCDNumber *sensorsLcdMin0_;
  QLCDNumber *sensorsLcdMax1_;
  QLabel *sensorsSensor1MinLabel_;
  QLCDNumber *sensorsLcdMax3_;
  QPushButton *sensorsClearButton_;
  QWidget *offline_tab;
  QGroupBox *surfaceGroupBox_;
  QTabWidget *surfacesTabWidget_;
  QWidget *tab;
  //  QPushButton *surfaceResetButton_;

  QGroupBox *displaytypeGroupbox_;
  QRadioButton *displaytype2DButton_;
  QRadioButton *displaytype3DButton_;
  QGroupBox *displayitemsGroupbox_;
  QCheckBox *displayitemsAxesButton_;
  QCheckBox *displayitemsMeshButton_;
  QCheckBox *displayitemsShadeButton_;
  QCheckBox *displayitemsLegendButton_;
  QCheckBox *displayitemsIsolinesButton_;
  QSpinBox *displayitemsIsolinesSpinbox_;

  QGroupBox *zscaleGroupbox_;
  QPushButton *zscalePlusButton_;
  QPushButton *zscaleMinusButton_;

  QWidget *advanced_tab;
  QGroupBox *basefolderGroupBox_;
  QPlainTextEdit *basefolderTextedit_;
  QPushButton *basefolderEditButton_;
  QGroupBox *chillerparametersGroupBox_;
  QDoubleSpinBox *chillerParametersSpinbox1_;
  QSpinBox *chillerParametersSpinbox2_;
  QSpinBox *chillerParametersSpinbox3_;
  QLabel *chillerParametersLabel1_;
  QLabel *chillerParametersLabel2_;
  QLabel *chillerParametersLabel3_;
  QGroupBox *rawimagerecoGroupBox_;
  QGroupBox *seedingthresholdsGroupBox_;
  QDoubleSpinBox *seedingThresholdsStep1Spinbox_;
  QDoubleSpinBox *seedingThresholdsStep2Spinbox_;
  QDoubleSpinBox *seedingThresholdsStep3Spinbox_;
  QLabel *seedingThresholdsStep1Label_;
  QLabel *seedingThresholdsStep2Label_;
  QLabel *seedingThresholdsStep3Label_;
  QGroupBox *blueishnessGroupBox_;
  QDoubleSpinBox *blueishnessSpinBox_;
  QGroupBox *hswGroupBox_;
  QDoubleSpinBox *hswSpinBox_;
  QGroupBox *advancedActionGroupBox_;
  QPushButton *advancedApplyButton_;
  QPushButton *advancedReadButton_;
  QPushButton *advancedLoadButton_;
  QPushButton *advancedSaveButton_;
  QPushButton *advancedDefaultButton_;
  QGroupBox *commentGroupBox_;
  QPlainTextEdit *commentTextEdit_;
  QGroupBox *surfaceRecoGroupBox_;
  QGroupBox *surfaceRecoSpacingGroupBox_;
  QDoubleSpinBox *surfaceRecoSpacingSpinbox_;
  QGroupBox *surfaceRecoSearchpathGroupBox_;
  QDoubleSpinBox *surfaceRecoSearchpathSpinbox_;
  QGroupBox *groupBox_20;
  QGroupBox *geometryGroupBox_;
  QSpinBox *geometryFSpinbox_;
  QSpinBox *geometryLgSpinbox_;
  QSpinBox *geometryLcSpinbox_;
  QDoubleSpinBox *geometryDeltaSpinbox_;
  QLabel *geometryFLabel_;
  QLabel *geometryLgLabel_;
  QLabel *geometryLcLabel_;
  QLabel *geometryDeltaLabel_;
  QGroupBox *geometryPitchGroupBox_;
  QLabel *geometryPitchLabel1_;
  QLabel *geometryPitchLabel2_;
  QDoubleSpinBox *geometryPitchSpinbox1_;
  QDoubleSpinBox *geometryPitchSpinbox2_;
  QStatusBar *statusbar;

  QGroupBox *cameraOptionsGroupBox_;
  QComboBox *apertureComboBox_;
  QLabel *apertureLabel_;
  QLabel *exptimeLabel_;
  QComboBox *exptimeComboBox_;
  QLabel *isoLabel_;
  QComboBox *isoComboBox_;
  QLabel *wbalanceLabel_;
  QComboBox *wbalanceComboBox_;
  QGroupBox *cameraConnectionGroupBox_;
  QCheckBox *cameraConnectionCheckBox_;
  QPushButton *cameraConnectionTestButton_;
  QPushButton *cameraConnectionResetButton_;

};


namespace Defo {
  class MainWindow: public DefoMainWindow {};
}



QT_END_NAMESPACE

#endif // DEFOYG7928_H
