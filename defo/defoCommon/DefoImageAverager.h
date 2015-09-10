#ifndef DEFOIMAGEAVERAGER_H
#define DEFOIMAGEAVERAGER_H

#include <QStringList>
#include <QImage>

class DefoImageAverager
{
public:

  explicit DefoImageAverager(const QStringList& filenames);
  ~DefoImageAverager();

  QImage getAveragedImage();

protected:

  QStringList filenames_;
};

#endif // DEFOIMAGEAVERAGER_H
