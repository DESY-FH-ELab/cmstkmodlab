/////////////////////////////////////////////////////////////////////////////////
//                                                                             //
//               Copyright (C) 2011-2025 - The DESY CMS Group                  //
//                           All rights reserved                               //
//                                                                             //
//      The CMStkModLab source code is licensed under the GNU GPL v3.0.        //
//      You have the right to modify and/or redistribute this source code      //
//      under the terms specified in the license, which may be found online    //
//      at http://www.gnu.org/licenses or at License.txt.                      //
//                                                                             //
/////////////////////////////////////////////////////////////////////////////////

#ifndef DEFOCAMERAMODEL_H
#define DEFOCAMERAMODEL_H

#include <QObject>
#include <QImage>
#include <QFile>
#include <QTimer>
#include <QTextDocument>
#include <QStringList>

#include <map>

#ifdef USE_FAKEIO
#include "devices/Canon/EOS550DFake.h"
typedef EOS550DFake EOS550D_t;
#else
#include "devices/Canon/EOS550D.h"
typedef EOS550D EOS550D_t;
#endif

#include "DeviceState.h"

#include <QImage>

class DefoCameraModel :
      public QObject
    , public AbstractDeviceModel<EOS550D_t>
{
    Q_OBJECT
public:
  typedef std::vector<std::string> OptionList;
  /// Copy enum values from VEOS500D to not expose them directly.
  enum Option {
      APERTURE      = VEOS550D::APERTURE
    , SHUTTER_SPEED = VEOS550D::SHUTTER_SPEED
    , ISO           = VEOS550D::ISO
    , WHITE_BALANCE = VEOS550D::WHITE_BALANCE
  };

  explicit DefoCameraModel(QObject *parent = 0);

  std::vector<std::string> getOptions( const Option& option ) const;
  void setOptionSelection( const Option& option, int value );
  int getOptionValue( const Option& option) const;

  const QImage& getLastPicture() const;
  const QString& getLastPictureLocation() const;
  const QStringList& getLastPictureLocations() const;
  const QImage& getLastLiveViewPicture() const;
  // TODO getLastPictureExif()

  QTextDocument* commentDocument() const { return comment_; }
  int getCalibAmplitude() const { return calibAmplitude_; }
  int getNumberOfImages() const { return numberOfImages_; }

public slots:
  virtual void setDeviceEnabled( bool enabled );
  void setControlsEnabled(bool enabled);
  virtual void setLiveViewEnabled( bool enabled );
  void acquirePicture(bool keep);
  void acquirePictures(int count);
  void acquireLiveViewPicture();
  void setComment(const QString&);
  void setCalibAmplitude(int amplitude);
  void setNumberOfImages(int number);

protected:
  virtual void initialize();
  virtual void setDeviceState( State state );

  // parameter cache
  std::map<Option, int> parameters_;
  void resetParameterCache();

  // image cache
  QString location_;
  QStringList locations_;
  QImage image_;
  QImage liveViewImage_;
  QTimer liveViewTimer_;
  QTextDocument* comment_;
  int calibAmplitude_;
  int numberOfImages_;

signals:
  void deviceStateChanged(State newState);
  void deviceOptionChanged(DefoCameraModel::Option option, int newValue);
  void newImage(QString location, bool keep);
  void newImages(QStringList locations);
  void liveViewModeChanged(bool state);
  void newLiveViewImage(QString location);
  void defoMessage(const QString & text);
  void controlStateChanged(bool);
};

#endif // DEFOCAMERAMODEL_H
