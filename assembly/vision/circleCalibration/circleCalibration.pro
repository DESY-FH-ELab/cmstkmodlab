ARCHITECTURE=Linux
USEFAKEDEVICES="X0"
NOUEYE="X0"

LIBS += -L/home/brownhep/PS_AUTO/cmstkmodlab/devices/lib -lTkModLabLang
LIBS += -L/home/brownhep/PS_AUTO/cmstkmodlab/common -lCommon
LIBS += -L/home/brownhep/PS_AUTO/cmstkmodlab/assembly/assemblyCommon -lAssemblyCommon

equals(USEFAKEDEVICES,"X1") {
  NOUEYE="X1"
}

macx {
  NOUEYE="X1"
  CONFIG+=x86_64
  QMAKE_CXXFLAGS += -stdlib=libc++
  #QMAKE_MAC_SDK = macosx10.11
  QMAKE_MACOSX_DEPLOYMENT_TARGET = 10.11
  #LIBS += -framework AppKit
  #LIBS += -framework QuartzCore
  #LIBS += -framework QTKit
  #LIBS += -framework Cocoa
}

equals(NOUEYE,"X0") {
  LIBS += -lueye_api
}

CONFIG += link_pkgconfig
PKGCONFIG += opencv

QMAKE = qmake-qt4

QMAKE_CXXFLAGS += -std=c++17
macx {
  QMAKE_CXXFLAGS += -DAPPLICATIONVERSIONSTR=\\\"unknown\\\"
}
else {
  QMAKE_CXXFLAGS += -DAPPLICATIONVERSIONSTR=\\\"`git describe --dirty --always --tags`\\\"
}

DEFINES += 
equals(NOUEYE,"X1") {
  DEFINES += NOUEYE
}

QT += core gui xml network script svg
greaterThan(QT_MAJOR_VERSION, 4) {
  QT += widgets
} 

CONFIG   += console
CONFIG   -= app_bundle

TARGET = circleCalibration
TEMPLATE = app

DEPENDPATH += /home/brownhep/PS_AUTO/cmstkmodlab/common /home/brownhep/PS_AUTO/cmstkmodlab/assembly/assemblyCommon
INCLUDEPATH += .
INCLUDEPATH += ..
INCLUDEPATH += /home/brownhep/PS_AUTO/cmstkmodlab
INCLUDEPATH += /home/brownhep/PS_AUTO/cmstkmodlab/common
INCLUDEPATH += /home/brownhep/PS_AUTO/cmstkmodlab/assembly/assemblyCommon

# Input
HEADERS += 

equals(NOUEYE,"X0") {
HEADERS += 
} else {
HEADERS += 
}

SOURCES += circleCalibration.cc

equals(NOUEYE,"X0") {
SOURCES += 
} else {
SOURCES += 
}
