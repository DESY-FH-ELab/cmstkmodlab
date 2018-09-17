ARCHITECTURE=Darwin
USEFAKEDEVICES="X1"

LIBS += -L/Users/mussgill/Documents/Physik/CMS/Labor/cmstkmodlab/common -lCommon

QMAKE = qmake

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
PKGCONFIG += 

QMAKE_CXXFLAGS += -std=c++11
macx {
  QMAKE_CXXFLAGS += -DAPPLICATIONVERSIONSTR=\\\"unknown\\\"
}
else {
  QMAKE_CXXFLAGS += -DAPPLICATIONVERSIONSTR=\\\"`git describe --dirty --always --tags`\\\"
}

DEFINES +=  USE_FAKEIO NOUEYE DARWIN

equals(USEFAKEDEVICES,"X1") {
DEFINES += USE_FAKEIO
}

QT += core gui xml network script svg
greaterThan(QT_MAJOR_VERSION, 4) {
  QT += widgets
} else {
  QT += 
}

TARGET = CMSPumpStationControl
TEMPLATE = app

macx {
  QMAKE_POST_LINK = install_name_tool -change libCommon.1.dylib /Users/mussgill/Documents/Physik/CMS/Labor/cmstkmodlab/common/libCommon.1.dylib $(TARGET)
}

DEPENDPATH += /Users/mussgill/Documents/Physik/CMS/Labor/cmstkmodlab/common
INCLUDEPATH += .
INCLUDEPATH += ..
INCLUDEPATH += /Users/mussgill/Documents/Physik/CMS/Labor/cmstkmodlab
INCLUDEPATH += /Users/mussgill/Documents/Physik/CMS/Labor/cmstkmodlab/common

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
           
equals(USEFAKEDEVICES,"X0") {
HEADERS += 
} else {
HEADERS += 
}

SOURCES += pumpstation.cc \
		   PumpStationSingletonApplication.cc \
           PumpStationHTTPModel.cc \
           PumpStationMainWindow.cc \
           PumpStationSVGWidget.cc \
		   PumpStationDialogs.cc

equals(USEFAKEDEVICES,"X0") {
SOURCES += 
} else {
SOURCES += 
}
