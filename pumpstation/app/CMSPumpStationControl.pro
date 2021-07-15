QMAKE = qmake

macx {
  CONFIG+=x86_64
  QMAKE_CXXFLAGS += -stdlib=libc++
  QMAKE_MACOSX_DEPLOYMENT_TARGET = 11.3
}

win32 {
  RC_ICONS += CMSPumpStationControl.ico
}

CONFIG += link_pkgconfig
PKGCONFIG += 

QMAKE_CXXFLAGS += -std=c++17

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

isEmpty(TARGET_EXT) {
  win32 {
    TARGET_CUSTOM_EXT = .exe
  }
  macx {
    TARGET_CUSTOM_EXT = .app
  }
} else {
  TARGET_CUSTOM_EXT = $${TARGET_EXT}
}

win32 {
  DEPLOY_COMMAND = windeployqt
}
macx {
  DEPLOY_COMMAND = macdeployqt
}

CONFIG( debug, debug|release ) {
  # debug
  win32 {
    DEPLOY_TARGET = $$shell_quote($$shell_path($${OUT_PWD}/debug/$${TARGET}$${TARGET_CUSTOM_EXT}))
  }
  macx {
    DEPLOY_TARGET = $$shell_quote($$shell_path($${OUT_PWD}/$${TARGET}$${TARGET_CUSTOM_EXT}))
  }
} else {
  # release
  win32 {
    DEPLOY_TARGET = $$shell_quote($$shell_path($${OUT_PWD}/release/$${TARGET}$${TARGET_CUSTOM_EXT}))
  }
  macx {
    DEPLOY_TARGET = $$shell_quote($$shell_path($${OUT_PWD}/$${TARGET}$${TARGET_CUSTOM_EXT}))
  }
}

QMAKE_POST_LINK = $${DEPLOY_COMMAND} $${DEPLOY_TARGET}

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
