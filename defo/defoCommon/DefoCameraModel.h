#ifndef DEFOCAMERAMODEL_H
#define DEFOCAMERAMODEL_H

#include <QObject>
#include <QImage>
#include <QFile>
#include <QTimer>
#include <QTextDocument>

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
  const QImage& getLastLiveViewPicture() const;
  // TODO getLastPictureExif()

  QTextDocument* commentDocument() const { return comment_; }
  int getCalibAmplitude() const { return calibAmplitude_; }

public slots:
  virtual void setDeviceEnabled( bool enabled );
  void setControlsEnabled(bool enabled);
  virtual void setLiveViewEnabled( bool enabled );
  void acquirePicture(bool keep);
  void acquireLiveViewPicture();
  void setComment(const QString&);
  void setCalibAmplitude(int amplitude);

protected:
  virtual void initialize();
  virtual void setDeviceState( State state );

  // parameter cache
  std::map<Option, int> parameters_;
  void resetParameterCache();

  // image cache
  QString location_;
  QImage image_;
  QImage liveViewImage_;
  QTimer liveViewTimer_;
  QTextDocument* comment_;
  int calibAmplitude_;


signals:
  void deviceStateChanged(State newState);
  void deviceOptionChanged(DefoCameraModel::Option option, int newValue);
  void newImage(QString location, bool keep);
  void liveViewModeChanged(bool state);
  void newLiveViewImage(QString location);
  void defoMessage(const QString & text);
  void controlStateChanged(bool);
};

#endif // DEFOCAMERAMODEL_H
