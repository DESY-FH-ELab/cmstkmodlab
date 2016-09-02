ARCHITECTURE=Linux
USEFAKEDEVICES="X0"

LIBS += -L/home/ksbeerna/cmstkmodlab/devices/lib -lTkModLabKeyence
LIBS += -L/home/ksbeerna/cmstkmodlab/common -lCommon
LIBS += -L/home/ksbeerna/cmstkmodlab/assembly/assemblyCommon -lAssemblyCommon

equals(USEFAKEDEVICES,"X0") {
LIBS += -lueye_api
}

QMAKE = qmake-qt4

macx {
  CONFIG+=x86_64
  QMAKE_CXXFLAGS += -stdlib=libc++
  #QMAKE_MAC_SDK = macosx10.11
  QMAKE_MACOSX_DEPLOYMENT_TARGET = 10.11
  #LIBS += -framework AppKit
  #LIBS += -framework QuartzCore
  #LIBS += -framework QTKit
  #LIBS += -framework Cocoa
}

CONFIG += link_pkgconfig
PKGCONFIG += opencv

QMAKE_CXXFLAGS += -std=c++11
macx {
  QMAKE_CXXFLAGS += -DAPPLICATIONVERSIONSTR=\\\"unknown\\\"
}
else {
  QMAKE_CXXFLAGS += -DAPPLICATIONVERSIONSTR=\\\"`git describe --dirty --always --tags`\\\"
}

DEFINES +=

QT += core gui xml network script svg
greaterThan(QT_MAJOR_VERSION, 4) {
  QT += widgets
}

TARGET = Testing
TEMPLATE = app

macx {
  QMAKE_POST_LINK = install_name_tool -change libCommon.1.dylib /home/ksbeerna/cmstkmodlab/common/libCommon.1.dylib $(TARGET)
  QMAKE_POST_LINK += && install_name_tool -change libAssemblyCommon.1.dylib /home/ksbeerna/cmstkmodlab/assembly/assemblyCommon/libAssemblyCommon.1.dylib $(TARGET)
}

DEPENDPATH += /home/ksbeerna/cmstkmodlab/common /home/ksbeerna/cmstkmodlab/assembly/assemblyCommon
INCLUDEPATH += .
INCLUDEPATH += ..
INCLUDEPATH += /home/ksbeerna/cmstkmodlab
INCLUDEPATH += /home/ksbeerna/cmstkmodlab/common
INCLUDEPATH += /home/ksbeerna/cmstkmodlab/assembly/assemblyCommon

greaterThan(QT_MAJOR_VERSION, 4) {
  cache()
}

# Input
HEADERS += LaserCommanderMainWindow.h

equals(USEFAKEDEVICES,"X0") {
HEADERS +=
} else {
HEADERS +=
}

SOURCES += Testing.cc \
           LaserCommanderMainWindow.cc

equals(USEFAKEDEVICES,"X0") {
SOURCES +=
} else {
SOURCES +=
}

