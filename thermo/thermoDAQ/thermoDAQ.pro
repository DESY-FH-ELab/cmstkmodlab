LIBS += -L/home/cmsdaf/PS_AutomatedAssembly/cmstkmodlab/devices/lib -lTkModLabKeithley
LIBS += -L/home/cmsdaf/PS_AutomatedAssembly/cmstkmodlab/devices/lib -lTkModLabGreisinger
LIBS += -L/home/cmsdaf/PS_AutomatedAssembly/cmstkmodlab/devices/lib -lTkModLabJulabo
LIBS += -L/home/cmsdaf/PS_AutomatedAssembly/cmstkmodlab/devices/lib -lTkModLabHameg
LIBS += -L/home/cmsdaf/PS_AutomatedAssembly/cmstkmodlab/devices/lib -lTkModLabPfeiffer
LIBS += -L/home/cmsdaf/PS_AutomatedAssembly/cmstkmodlab/devices/lib -lTkModLabHuber
LIBS += -L/home/cmsdaf/PS_AutomatedAssembly/cmstkmodlab/common -lCommon
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

TARGET = thermoDAQ
TEMPLATE = app

macx {
  QMAKE_POST_LINK = install_name_tool -change libCommon.1.dylib /home/cmsdaf/PS_AutomatedAssembly/cmstkmodlab/common/libCommon.1.dylib $(TARGET)
}

DEPENDPATH += /home/cmsdaf/PS_AutomatedAssembly/cmstkmodlab/common
INCLUDEPATH += .
INCLUDEPATH += ..
INCLUDEPATH += /home/cmsdaf/PS_AutomatedAssembly/cmstkmodlab
INCLUDEPATH += /home/cmsdaf/PS_AutomatedAssembly/cmstkmodlab/common

greaterThan(QT_MAJOR_VERSION, 4){
  cache()
}

HEADERS += TestWindow.h \
           ThermoMainWindow.h \
           ThermoScriptModel.h \
           ThermoScriptThread.h \
           ThermoScriptWidget.h \
           ThermoScriptableGlobals.h \
           ThermoDAQThread.h \
           ThermoDAQModel.h \
           ThermoDAQStreamer.h \
           ThermoDAQWebInfo.h \
           ThermoDAQWidget.h \
           ThermoDAQServer.h \
           ThermoPfeifferWidget.h
    
SOURCES += thermoDAQ.cc \
           TestWindow.cc \
           ThermoMainWindow.cc \
           ThermoScriptModel.cc \
           ThermoScriptThread.cc \
           ThermoScriptWidget.cc \
           ThermoScriptableGlobals.cc \
           ThermoDAQThread.cc \
           ThermoDAQModel.cc \
           ThermoDAQStreamer.cc \
           ThermoDAQWebInfo.cc \
           ThermoDAQWidget.cc \
           ThermoDAQServer.cc \
           ThermoPfeifferWidget.cc
