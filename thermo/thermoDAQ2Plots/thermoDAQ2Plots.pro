#-------------------------------------------------
#
# Project created by QtCreator 2013-07-19T12:02:15
#
#-------------------------------------------------

QMAKE_CXXFLAGS += -pthread -std=c++1z -m64 -I/home/brownhep/Packages/root/include
LIBS += -L/home/brownhep/Packages/root/lib -lCore -lImt -lRIO -lNet -lHist -lGraf -lGraf3d -lGpad -lROOTDataFrame -lROOTVecOps -lTree -lTreePlayer -lRint -lPostscript -lMatrix -lPhysics -lMathCore -lThread -lMultiProc -pthread -lm -ldl -rdynamic

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

QT       += core xml
QT       -= gui

TARGET = thermoDAQ2Plots
TEMPLATE = app

macx {
  QMAKE_POST_LINK = install_name_tool -change libCommon.1.dylib /home/brownhep/PS_AUTO/cmstkmodlab/common/libCommon.1.dylib $(TARGET)
}

CONFIG   += console
CONFIG   -= app_bundle


greaterThan(QT_MAJOR_VERSION, 4) {
  cache()
}

HEADERS += ThermoDAQStreamReader.h

SOURCES += main.cc \
           ThermoDAQStreamReader.cc
