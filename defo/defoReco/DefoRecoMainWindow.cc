#include <QGroupBox>
#include <QFileDialog>

#include "DefoImageWidget.h"
#include "DefoRecoImageWidget.h"

#include "DefoRecoMainWindow.h"

#include "DefoPointRecognitionModel.h"
#include "DefoRecoPointRecognitionWidget.h"
#include "DefoMeasurementListComboBox.h"
#include "DefoPointIndexerListComboBox.h"
#include "DefoRecoColorHistoWidget.h"

DefoRecoMainWindow::DefoRecoMainWindow(QWidget *parent) :
    QMainWindow(parent)
{
  currentDir_ = "/home/tkmodlab/Desktop/measurements";

  // MEASUREMENT MODEL
  listModel_ = new DefoMeasurementListModel(this);
  selectionModel_ = new DefoMeasurementSelectionModel(this);

  roiSelectionModel_ = new DefoMeasurementSelectionModel(this);
  roiModel_ = new DefoROIModel(this);

  alignmentSelectionModel_ = new DefoMeasurementSelectionModel(this);
  alignmentModel_ = new DefoAlignmentModel(this);

  // POINT MODEL
  refSelectionModel_ = new DefoMeasurementSelectionModel(this);
  refPointModel_ = new DefoPointRecognitionModel(this);
  defoSelectionModel_ = new DefoMeasurementSelectionModel(this);
  defoPointModel_ = new DefoPointRecognitionModel(this);

  pointIndexerModel_ = new DefoPointIndexerModel(this);

  refColorModel_ = new DefoColorSelectionModel(this);
  defoColorModel_ = new DefoColorSelectionModel(this);

  reconstructionModel_ = new DefoReconstructionModel(listModel_,
                                                     refSelectionModel_,
                                                     defoSelectionModel_,
                                                     alignmentModel_,
                                                     pointIndexerModel_,
                                                     refColorModel_,
                                                     defoColorModel_,
                                                     this);

  tabWidget_ = new QTabWidget(this);
  tabWidget_->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);

  QBoxLayout *layout = new QHBoxLayout();
  QWidget * measurementWidget = new QWidget(tabWidget_);
  measurementWidget->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
  measurementWidget->setLayout(layout);

  QBoxLayout *vbox = new QVBoxLayout();
  QWidget * measurementInfoWidget = new QWidget(measurementWidget);
  measurementInfoWidget->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
  measurementInfoWidget->setLayout(vbox);
  layout->addWidget(measurementInfoWidget);

  QBoxLayout *hbox = new QHBoxLayout();
  QWidget * measurementButtonWidget = new QWidget(measurementInfoWidget);
  measurementButtonWidget->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
  measurementButtonWidget->setLayout(hbox);
  vbox->addWidget(measurementButtonWidget);

  QPushButton * loadMeasurementButton = new QPushButton("&Load measurement", measurementButtonWidget);
  connect(loadMeasurementButton, SIGNAL(clicked()),
   	  this, SLOT(loadMeasurementButtonClicked()));
  hbox->addWidget(loadMeasurementButton);

  QPushButton * saveMeasurementButton = new QPushButton("&Save measurement", measurementButtonWidget);
  connect(saveMeasurementButton, SIGNAL(clicked()),
          this, SLOT(saveMeasurementButtonClicked()));
  hbox->addWidget(saveMeasurementButton);

  DefoMeasurementListTreeWidget * treeWidget = new DefoMeasurementListTreeWidget(listModel_,
										 selectionModel_,
										 measurementInfoWidget);
  vbox->addWidget(treeWidget);

  DefoMeasurementInfoListTreeWidget * infoTreeWidget = new DefoMeasurementInfoListTreeWidget(selectionModel_,
											     measurementInfoWidget);
  vbox->addWidget(infoTreeWidget);

  DefoRecoRawImageWidget *rawImage = new DefoRecoRawImageWidget(selectionModel_, measurementInfoWidget);
  layout->addWidget(rawImage);

  tabWidget_->addTab(measurementWidget, "Measurement");

  vbox = new QVBoxLayout();
  QWidget * roiWidget = new QWidget(tabWidget_);
  roiWidget->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
  roiWidget->setLayout(vbox);

  DefoMeasurementListComboBox *roiSelect = new DefoMeasurementListComboBox(listModel_, roiSelectionModel_, roiWidget);
  vbox->addWidget(roiSelect);

  DefoRecoROIImageWidget *roiImage = new DefoRecoROIImageWidget(roiSelectionModel_, roiModel_, roiWidget);
  vbox->addWidget(roiImage);

  tabWidget_->addTab(roiWidget, "ROI");

  // read default settings
  DefoConfigReader cfgReader( "/Users/mussgill/Documents/Physik/CMS/Labor/cmstkmodlab/trunk/defo/defo.cfg" );

  layout = new QHBoxLayout();
  QWidget * pointsWidget = new QWidget(tabWidget_);
  pointsWidget->setLayout(layout);

  vbox = new QVBoxLayout();
  QWidget * refPointWidget = new QWidget(pointsWidget);
  refPointWidget->setLayout(vbox);

  DefoMeasurementListComboBox *refSelect = new DefoMeasurementListComboBox(listModel_, refSelectionModel_, refPointWidget);
  vbox->addWidget(refSelect);

  DefoRecoPointRecognitionWidget * refPointRecognitionWidget =
    new DefoRecoPointRecognitionWidget(listModel_,
				       refSelectionModel_,
                                       refPointModel_,
                                       roiModel_,
				       refPointWidget);
  refPointModel_->setThresholdValue(
        DefoPointRecognitionModel::THRESHOLD_1
        , cfgReader.getValue<int>( "STEP1_THRESHOLD" )
        );
  refPointModel_->setThresholdValue(
        DefoPointRecognitionModel::THRESHOLD_2
        , cfgReader.getValue<int>( "STEP2_THRESHOLD" )
        );
  refPointModel_->setThresholdValue(
        DefoPointRecognitionModel::THRESHOLD_3
        , cfgReader.getValue<int>( "STEP3_THRESHOLD" )
        );
  refPointModel_->setHalfSquareWidth(
        cfgReader.getValue<int>( "HALF_SQUARE_WIDTH" )
        );
  vbox->addWidget(refPointRecognitionWidget);

  layout->addWidget(refPointWidget);

  vbox = new QVBoxLayout();
  QWidget * defoPointWidget = new QWidget(pointsWidget);
  defoPointWidget->setLayout(vbox);
  
  DefoMeasurementListComboBox *defoSelect = new DefoMeasurementListComboBox(listModel_, defoSelectionModel_, defoPointWidget);
  vbox->addWidget(defoSelect);

  DefoRecoPointRecognitionWidget * defoPointRecognitionWidget =
    new DefoRecoPointRecognitionWidget(listModel_,
				       defoSelectionModel_,
				       defoPointModel_,
                                       roiModel_,
                                       defoPointWidget);
  defoPointModel_->setThresholdValue(
        DefoPointRecognitionModel::THRESHOLD_1
        , cfgReader.getValue<int>( "STEP1_THRESHOLD" )
        );
  defoPointModel_->setThresholdValue(
        DefoPointRecognitionModel::THRESHOLD_2
        , cfgReader.getValue<int>( "STEP2_THRESHOLD" )
        );
  defoPointModel_->setThresholdValue(
        DefoPointRecognitionModel::THRESHOLD_3
        , cfgReader.getValue<int>( "STEP3_THRESHOLD" )
        );
  defoPointModel_->setHalfSquareWidth(
        cfgReader.getValue<int>( "HALF_SQUARE_WIDTH" )
        );
  vbox->addWidget(defoPointRecognitionWidget);

  layout->addWidget(defoPointWidget);

  tabWidget_->addTab(pointsWidget, "Points");

  vbox = new QVBoxLayout();
  QWidget * alignmentWidget = new QWidget(tabWidget_);
  alignmentWidget->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
  alignmentWidget->setLayout(vbox);

  DefoMeasurementListComboBox *alignmentSelect = new DefoMeasurementListComboBox(listModel_,
                                                                                 alignmentSelectionModel_,
                                                                                 alignmentWidget);
  vbox->addWidget(alignmentSelect);

  DefoRecoAlignmentImageWidget *alignmentImage = new DefoRecoAlignmentImageWidget(alignmentSelectionModel_,
                                                                                  alignmentModel_,
                                                                                  alignmentWidget);
  vbox->addWidget(alignmentImage);

  tabWidget_->addTab(alignmentWidget, "Alignment");

  vbox = new QVBoxLayout();
  QWidget * indexerWidget = new QWidget(tabWidget_);
  indexerWidget->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
  indexerWidget->setLayout(vbox);

  DefoPointIndexerListComboBox *indexerSelect = new DefoPointIndexerListComboBox(pointIndexerModel_,
                                                                                 indexerWidget);
  vbox->addWidget(indexerSelect);

  DefoRecoColorHistoWidget *refIndexerSelect = new DefoRecoColorHistoWidget(listModel_,
                                                                            refSelectionModel_,
                                                                            refColorModel_,
                                                                            indexerWidget);
  vbox->addWidget(refIndexerSelect);

  DefoRecoColorHistoWidget *defoIndexerSelect = new DefoRecoColorHistoWidget(listModel_,
                                                                             defoSelectionModel_,
                                                                             defoColorModel_,
                                                                             indexerWidget);
  vbox->addWidget(defoIndexerSelect);

  tabWidget_->addTab(indexerWidget, "Indexer");

  DefoReconstructionWidget *recoWidget = new DefoReconstructionWidget(reconstructionModel_,
                                                                      tabWidget_);
  tabWidget_->addTab(recoWidget, "Reconstruction");

  setCentralWidget(tabWidget_);
}

void DefoRecoMainWindow::loadMeasurementButtonClicked() {
  
  QString filename = QFileDialog::getOpenFileName(this,
						  QString("Load Measurement"),
                                                  currentDir_.absolutePath(),
						  QString("ODM Measurement Files (*.odmx)"));
  if (filename.isNull()) return;

  QFileInfo fi(filename);
  currentDir_ = fi.absolutePath();
  std::cout << currentDir_.absolutePath().toStdString() << std::endl;

  roiModel_->read(currentDir_.absoluteFilePath("roi.xml"));
  alignmentModel_->read(currentDir_.absoluteFilePath("alignment.xml"));

  listModel_->clear();
  listModel_->read(filename);
  listModel_->readPoints(currentDir_);
}

void DefoRecoMainWindow::saveMeasurementButtonClicked() {

  roiModel_->write(currentDir_.absolutePath());
  alignmentModel_->write(currentDir_.absolutePath());

  listModel_->writePoints(currentDir_);
}
