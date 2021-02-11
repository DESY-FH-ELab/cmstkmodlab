#-------------------------------------------------
#
# Project created by QtCreator 2013-07-19T12:02:15
#
#-------------------------------------------------

QMAKE = qmake-qt4

CONFIG+=c++17
QMAKE_CXXFLAGS += -std=c++17
macx {
  QMAKE_CXXFLAGS += -DAPPLICATIONVERSIONSTR=\\\"unknown\\\"
} else {
  QMAKE_CXXFLAGS += -DAPPLICATIONVERSIONSTR=\\\"`git describe --dirty --always --tags`\\\"
}

QT       += core xml

QT       -= gui

TARGET = thermoDAQ2Log
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

macx {
  QMAKE_MACOSX_DEPLOYMENT_TARGET=10.9
}

greaterThan(QT_MAJOR_VERSION, 4) {
  cache()
}

HEADERS += ThermoDAQLogReader.h

SOURCES += main.cc \
           ThermoDAQLogReader.cc
