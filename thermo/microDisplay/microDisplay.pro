LIBS += -L/home/brownhep/PS_AUTO/cmstkmodlab/devices/lib -lTkModLabKeithley
LIBS += -L/home/brownhep/PS_AUTO/cmstkmodlab/devices/lib -lTkModLabGreisinger
LIBS += -L/home/brownhep/PS_AUTO/cmstkmodlab/devices/lib -lTkModLabJulabo
LIBS += -L/home/brownhep/PS_AUTO/cmstkmodlab/devices/lib -lTkModLabHameg
LIBS += -L/home/brownhep/PS_AUTO/cmstkmodlab/devices/lib -lTkModLabPfeiffer
LIBS += -L/home/brownhep/PS_AUTO/cmstkmodlab/devices/lib -lTkModLabHuber
LIBS += -L/home/brownhep/PS_AUTO/cmstkmodlab/common -lCommon
LIBS += -lqwt -lqwtplot3d-qt4 -lGL -lGLU

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

CONFIG+=c++17
QMAKE_CXXFLAGS += -std=c++17
macx {
  QMAKE_CXXFLAGS += -DAPPLICATIONVERSIONSTR=\\\"unknown\\\"
} else {
  QMAKE_CXXFLAGS += -DAPPLICATIONVERSIONSTR=\\\"`git describe --dirty --always --tags`\\\"
}

INCLUDEPATH += /usr/include/qwt /usr/include/qwtplot3d-qt4
DEFINES += 

QT += core gui xml network
greaterThan(QT_MAJOR_VERSION, 4) {
  QT += widgets
} 

TARGET = microDisplay
TEMPLATE = app

macx {
  QMAKE_POST_LINK = install_name_tool -change libCommon.1.dylib /home/brownhep/PS_AUTO/cmstkmodlab/common/libCommon.1.dylib $(TARGET)
}

DEPENDPATH += /home/brownhep/PS_AUTO/cmstkmodlab/common
INCLUDEPATH += .
INCLUDEPATH += ..
INCLUDEPATH += /home/brownhep/PS_AUTO/cmstkmodlab
INCLUDEPATH += /home/brownhep/PS_AUTO/cmstkmodlab/common

greaterThan(QT_MAJOR_VERSION, 4) {
  cache()
}

# Input
HEADERS += MicroDAQClient.h \
           MicroDAQNetworkReader.h \
           MicroDisplayMainWindow.h \
           MicroDAQDisplayWidget.h \
           MicroDAQValueVector.h
    
SOURCES += microDisplay.cc \
           MicroDAQClient.cc \
           MicroDAQNetworkReader.cc \
           MicroDisplayMainWindow.cc \
           MicroDAQDisplayWidget.cc
