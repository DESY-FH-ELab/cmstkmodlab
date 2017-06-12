#include <iostream>

#include <QCoreApplication>
#include <QtCore>
#include <QFile>
#if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
#include <QDesktopServices>
#else
#include <QStandardPaths>
#endif

#include "Analyser.h"

int main(int argc, char *argv[])
{
  QCoreApplication app(argc, argv);

  QStringList arguments = QCoreApplication::arguments();
  arguments.removeAt(0);

  Analyser analyser(arguments);
  QTimer::singleShot(0, &analyser, SLOT(analyse()));

  return app.exec();
}
