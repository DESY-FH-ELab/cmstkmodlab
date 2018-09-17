QMAKE = qmake

macx {
  CONFIG+=x86_64
  QMAKE_CXXFLAGS += -stdlib=libc++
  QMAKE_MACOSX_DEPLOYMENT_TARGET = 10.11
}

CONFIG += link_pkgconfig
PKGCONFIG += 

QMAKE_CXXFLAGS += -std=c++11

QT += core gui network script svg
greaterThan(QT_MAJOR_VERSION, 4) {
  QT += widgets
} else {
  QT += 
}

TARGET = CMSPumpStationControl
TEMPLATE = app

INCLUDEPATH += .

greaterThan(QT_MAJOR_VERSION, 4) {
  cache()
}

# Input
HEADERS += PumpStationSingletonApplication.h \
           PumpStationHTTPModel.h \
		   PumpStationMainWindow.h \
		   PumpStationSVG.h \
		   PumpStationSVGWidget.h \
		   PumpStationDialogs.h

SOURCES += pumpstation.cc \
		   PumpStationSingletonApplication.cc \
           PumpStationHTTPModel.cc \
           PumpStationMainWindow.cc \
           PumpStationSVGWidget.cc \
		   PumpStationDialogs.cc
