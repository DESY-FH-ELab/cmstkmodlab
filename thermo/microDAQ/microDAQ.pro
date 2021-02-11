LIBS += -L/home/brownhep/PS_AUTO/cmstkmodlab/devices/lib -lTkModLabKeithley
LIBS += -L/home/brownhep/PS_AUTO/cmstkmodlab/devices/lib -lTkModLabGreisinger
LIBS += -L/home/brownhep/PS_AUTO/cmstkmodlab/devices/lib -lTkModLabJulabo
LIBS += -L/home/brownhep/PS_AUTO/cmstkmodlab/devices/lib -lTkModLabHameg
LIBS += -L/home/brownhep/PS_AUTO/cmstkmodlab/devices/lib -lTkModLabPfeiffer
LIBS += -L/home/brownhep/PS_AUTO/cmstkmodlab/devices/lib -lTkModLabHuber
LIBS += -L/home/brownhep/PS_AUTO/cmstkmodlab/devices/lib -lTkModLabArduino
LIBS += -L/home/brownhep/PS_AUTO/cmstkmodlab/devices/lib -lTkModLabCori
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

QT += core gui xml network script svg
greaterThan(QT_MAJOR_VERSION, 4) {
  QT += widgets
} 

TARGET = microDAQ
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
HEADERS += MicroMainWindow.h \
           MicroScriptModel.h \
           MicroScriptThread.h \
           MicroScriptWidget.h \
           MicroScriptableGlobals.h \
           MicroDAQThread.h \
           MicroDAQModel.h \
           MicroDAQStreamer.h \
           MicroDAQWidget.h \
           MicroDAQServer.h

SOURCES += microDAQ.cc \
           MicroMainWindow.cc \
           MicroScriptModel.cc \
           MicroScriptThread.cc \
           MicroScriptWidget.cc \
           MicroScriptableGlobals.cc \
           MicroDAQThread.cc \
           MicroDAQModel.cc \
           MicroDAQStreamer.cc \
           MicroDAQWidget.cc \
           MicroDAQServer.cc
