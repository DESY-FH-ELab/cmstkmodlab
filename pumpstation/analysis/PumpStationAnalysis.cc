/////////////////////////////////////////////////////////////////////////////////
//                                                                             //
//               Copyright (C) 2011-2017 - The DESY CMS Group                  //
//                           All rights reserved                               //
//                                                                             //
//      The CMStkModLab source code is licensed under the GNU GPL v3.0.        //
//      You have the right to modify and/or redistribute this source code      //
//      under the terms specified in the license, which may be found online    //
//      at http://www.gnu.org/licenses or at License.txt.                      //
//                                                                             //
/////////////////////////////////////////////////////////////////////////////////

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
	if (argc<3 || argc>5) {
		std::cout << "usage:" << std::endl;
		std::cout << argv[0] << " Output-File Input-Directory [Start Date YYYY-MM-DD] [End Date YYYY-MM-DD]" << std::endl;
		exit(0);
	}

	QCoreApplication app(argc, argv);

  QStringList arguments = QCoreApplication::arguments();
  arguments.removeAt(0);

  Analyser analyser(arguments);
  QTimer::singleShot(0, &analyser, SLOT(analyse()));

  return app.exec();
}
