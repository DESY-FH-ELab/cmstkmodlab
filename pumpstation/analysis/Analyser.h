#ifndef ANALYSER_H
#define ANALYSER_H

#include <QStringList>
#include <QObject>

class Analyser : public QObject
{
  Q_OBJECT

public:

  Analyser(QStringList& arguments);

public slots:

  void analyse();

private:

  QStringList arguments_;
};

#endif // ANALYSER_H
