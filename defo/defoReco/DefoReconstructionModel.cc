#include <iostream>
#include <fstream>
#include <cmath>

#include "ApplicationConfig.h"
#include "nqlogger.h"

#include "DefoReconstructionModel.h"
#include "DefoPointSaver.h"

DefoReconstructionModel::DefoReconstructionModel(DefoMeasurementListModel * listModel,
                                                 DefoMeasurementSelectionModel* refSelectionModel,
                                                 DefoMeasurementSelectionModel* defoSelectionModel,
                                                 DefoAlignmentModel* alignmentModel,
                                                 DefoPointIndexerModel* pointIndexerModel,
                                                 DefoColorSelectionModel* refColorModel,
                                                 DefoColorSelectionModel* defoColorModel,
                                                 DefoMeasurementPairListModel* pairListModel,
                                                 DefoMeasurementPairSelectionModel* pairSelectionModel,
                                                 DefoGeometryModel* geometryModel,
                                                 QObject *parent) :
    QObject(parent),
    listModel_(listModel),
    pairListModel_(pairListModel),
    pairSelectionModel_(pairSelectionModel),
    geometryModel_(geometryModel)
{
  angle_ = 0.0;
  refMeasurement_ = 0;
  defoMeasurement_ = 0;
  pointIndexer_ = 0;

  connect(refSelectionModel, SIGNAL(selectionChanged(DefoMeasurement*)),
          this, SLOT(refSelectionChanged(DefoMeasurement*)));

  connect(defoSelectionModel, SIGNAL(selectionChanged(DefoMeasurement*)),
          this, SLOT(defoSelectionChanged(DefoMeasurement*)));

  connect(listModel_, SIGNAL(pointsUpdated(const DefoMeasurement*)),
          this, SLOT(pointsUpdated(const DefoMeasurement*)));

  connect(alignmentModel, SIGNAL(alignmentChanged(double)),
          this, SLOT(alignmentChanged(double)));

  connect(pointIndexerModel, SIGNAL(pointIndexerChanged(DefoVPointIndexer*)),
          this, SLOT(pointIndexerChanged(DefoVPointIndexer*)));

  connect(refColorModel, SIGNAL(colorChanged(float,float)),
          this, SLOT(refColorChanged(float,float)));

  connect(defoColorModel, SIGNAL(colorChanged(float,float)),
          this, SLOT(defoColorChanged(float,float)));

  connect(geometryModel_, SIGNAL(geometryChanged()),
          this, SLOT(geometryChanged()));

  reco_ = new DefoRecoSurface(this);
  
  reco_->setPitchX(ApplicationConfig::instance()->getValue<double>("PIXEL_PITCH_X"));
  reco_->setPitchY(ApplicationConfig::instance()->getValue<double>("PIXEL_PITCH_Y"));

  connect(reco_, SIGNAL(incrementRecoProgress()),
          this, SLOT(incrementRecoProgress()));
}

void DefoReconstructionModel::setCurrentDir(QDir& dir) {
    currentDir_ = dir;
}

void DefoReconstructionModel::refSelectionChanged(DefoMeasurement* measurement) {
  std::cout << "void DefoReconstructionModel::referenceSelectionChanged(DefoMeasurement* measurement)" << std::endl;
  refMeasurement_ = measurement;
  emit setupChanged();
}

void DefoReconstructionModel::defoSelectionChanged(DefoMeasurement* measurement) {
  std::cout << "void DefoReconstructionModel::defoSelectionChanged(DefoMeasurement* measurement)" << std::endl;
  defoMeasurement_ = measurement;
  emit setupChanged();
}

void DefoReconstructionModel::pointsUpdated(const DefoMeasurement* measurement) {
  std::cout << "void DefoReconstructionModel::pointsUpdated(const DefoMeasurement* measurement)" << std::endl;
  if (measurement==refMeasurement_) emit setupChanged();
  if (measurement==defoMeasurement_) emit setupChanged();
}

void DefoReconstructionModel::alignmentChanged(double angle) {
  std::cout << "void DefoReconstructionModel::alignmentChanged(double angle)" << std::endl;
  angle_ = angle;
  emit setupChanged();
}

void DefoReconstructionModel::pointIndexerChanged(DefoVPointIndexer * indexer) {
  std::cout << "void DefoReconstructionModel::pointIndexerChanged(DefoVPointIndexer * indexer)" << std::endl;
  pointIndexer_ = indexer;
  emit setupChanged();
}

void DefoReconstructionModel::refColorChanged(float hue, float saturation) {
  std::cout << "void DefoReconstructionModel::refColorChanged(float hue, float saturation)" << std::endl;
  refColor_.setHsvF(hue, saturation, 1.0, 1.0);
  emit setupChanged();
}

void DefoReconstructionModel::defoColorChanged(float hue, float saturation) {
  std::cout << "void DefoReconstructionModel::defoColorChanged(float hue, float saturation)" << std::endl;
  defoColor_.setHsvF(hue, saturation, 1.0, 1.0);
  emit setupChanged();
}

void DefoReconstructionModel::geometryChanged()
{
  NQLog("DefoReconstructionModel::geometryChanged()", NQLog::Message) << "start";

  std::cout << "void DefoReconstructionModel::geometryChanged()" << std::endl;

  double angle1 = geometryModel_->getAngle1();
  double angle1Rad = angle1 * M_PI / 180.;
  double angle2 = geometryModel_->getAngle2();
  double angle2Rad = angle2 * M_PI / 180.;
  double distance = geometryModel_->getDistance();
  double height1 = geometryModel_->getHeight1();
  double height2 = geometryModel_->getHeight2();

  NQLog("DefoReconstructionModel", NQLog::Message) << "angle1 [deg]  = " << angle1;
  NQLog("DefoReconstructionModel", NQLog::Message) << "angle1 [rad]  = " << angle1Rad;
  NQLog("DefoReconstructionModel", NQLog::Message) << "angle2 [deg]  = " << angle2;
  NQLog("DefoReconstructionModel", NQLog::Message) << "angle2 [rad]  = " << angle2Rad;
  NQLog("DefoReconstructionModel", NQLog::Message) << "distance [mm] = " << distance;
  NQLog("DefoReconstructionModel", NQLog::Message) << "height1 [mm]   = " << height1;
  NQLog("DefoReconstructionModel", NQLog::Message) << "height2 [mm]   = " << height2;

  // height of camera rotation point over surface
  double heightCameraToSurface = height1 - height2 - distance * std::sin(angle2Rad);
  NQLog("DefoReconstructionModel", NQLog::Message) << "heightCameraToSurface [mm] = " << heightCameraToSurface;

  // viewing distance from camera to surface
  // (assumption: camera is mounted perpendicular to frame)
  double distanceCamera = heightCameraToSurface / std::cos(angle2Rad);
  NQLog("DefoReconstructionModel", NQLog::Message) << "distanceCamera [mm] = " << distanceCamera;

  reco_->setNominalCameraDistance(distanceCamera);

  // distance from grid to surface calculated as the shortest distance
  // between grid and the point on the surface under the grid roation axis
  double distanceGrid = (height1 - height2) * std::cos(angle1Rad);
  NQLog("DefoReconstructionModel", NQLog::Message) << "distanceGrid [mm] = " << distanceGrid;

  reco_->setNominalGridDistance(distanceGrid);

  // not really sure about this one...
  reco_->setNominalViewingAngle(angle2Rad);
  NQLog("DefoReconstructionModel", NQLog::Message) << "viewing angle [rad] = " << angle2Rad;

  reco_->calculateHelpers();

  NQLog("DefoReconstructionModel::geometryChanged()", NQLog::Message) << "end";

  emit setupChanged();
}

void DefoReconstructionModel::incrementRecoProgress() {
  emit incrementProgress();
}

void DefoReconstructionModel::reconstruct()
{
  std::cout << "void DefoReconstructionModel::reconstruct()" << std::endl;

  emit recoProgressChanged(0);

  if (refMeasurement_==0 || defoMeasurement_==0) {
    std::cout << "reco: reference and deformed measurements not selected" << std::endl;
    return;
  }

  if (pointIndexer_==0) {
    std::cout << "reco: point indexer not available" << std::endl;
    return;
  }

  const DefoPointCollection* refPoints = listModel_->getMeasurementPoints(refMeasurement_);
  if (!refPoints || refPoints->size()==0) {
    std::cout << "reco: reference measurement does not contain points" << std::endl;
    return;
  }

  const DefoPointCollection* defoPoints = listModel_->getMeasurementPoints(defoMeasurement_);
  if (!defoPoints || defoPoints->size()==0) {
    std::cout << "reco: deformed measurement does not contain points" << std::endl;
    return;
  }

  reco_->setImageSize(std::pair<double,double>(refMeasurement_->getWidth(),
                                               refMeasurement_->getHeight()));

  if (!alignPoints(refPoints, refCollection_)) {
    std::cout << "reco: reference points could not be aligned" << std::endl;
    return;
  }
  emit incrementProgress();

  if (!alignPoints(defoPoints, defoCollection_)) {
    std::cout << "reco: deformed points could not be aligned" << std::endl;
    return;
  }
  emit incrementProgress();


  QString basename = "offlinePoints_%1.xml";
  QString fileLocation;

  pointIndexer_->indexPoints(&refCollection_, refColor_);
  emit incrementProgress();

  fileLocation = currentDir_.absoluteFilePath(basename.arg(refMeasurement_->getTimeStamp().toString("yyyyMMddhhmmss")));
  DefoPointSaver refSaver(fileLocation);
  refSaver.writeXMLPoints(refCollection_);
  emit incrementProgress();

  pointIndexer_->indexPoints(&defoCollection_, defoColor_);
  emit incrementProgress();

  fileLocation = currentDir_.absoluteFilePath(basename.arg(defoMeasurement_->getTimeStamp().toString("yyyyMMddhhmmss")));
  DefoPointSaver defoSaver(fileLocation);
  defoSaver.writeXMLPoints(defoCollection_);

  reco_->setFocalLength(refMeasurement_->getFocalLength());
  NQLog("DefoReconstructionModel::reconstruct()", NQLog::Message) << "focalLength [mm]   = " << refMeasurement_->getFocalLength();

  emit incrementProgress();

  // reco_->dump();
  DefoSurface surface = reco_->reconstruct(defoCollection_, refCollection_);

  QString filename = "defoReco_";
  filename += refMeasurement_->getTimeStamp().toString("yyyyMMddhhmmss");
  filename += "_";
  filename += defoMeasurement_->getTimeStamp().toString("yyyyMMddhhmmss");
  filename += ".txt";
  surface.dumpSplineField(currentDir_.absoluteFilePath(filename).toStdString());

  bool newPair = false;
  DefoMeasurementPair * measurementPair = pairListModel_->findMeasurementPair(refMeasurement_,
                                                                              defoMeasurement_);
  if (measurementPair==0) {
    newPair = true;
    measurementPair = new DefoMeasurementPair(refMeasurement_, defoMeasurement_);
  }

  measurementPair->setSurface(surface);

  if (newPair) {
    pairListModel_->addMeasurementPair(measurementPair);
  }
}

bool DefoReconstructionModel::alignPoints(const DefoPointCollection* original,
                                          DefoPointCollection& aligned) {

  aligned.clear();

  if (angle_==0.0) {
    for (DefoPointCollection::const_iterator it = original->begin();
         it!=original->end();
         ++it) {
      aligned.push_back(*it);
    }
    return true;
  } else {

    double c = std::cos(angle_);
    double s = std::sin(angle_);

    for (DefoPointCollection::const_iterator it = original->begin();
         it!=original->end();
         ++it) {
      const DefoPoint& p = *it;
      DefoPoint np(p);
      np.setX(p.getX()*c - p.getY()*s);
      np.setY(p.getX()*s + p.getY()*c);

      aligned.push_back(np);
    }
  }

  return true;
}
