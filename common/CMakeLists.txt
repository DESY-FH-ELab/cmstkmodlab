add_library(Common SHARED
        nqlogger.cc
        SingletonApplication.cc
        ApplicationConfig.cc
        ApplicationConfigReader.cc
        ApplicationConfigWriter.cc
        SlackBot.cc
        MattermostBot.cc
        # RohdeSchwarzNGE103BModel.cc
        # RohdeSchwarzNGE103BWidget.cc
        # npoint2D.cc
        # npoint3D.cc
        # nvector2D.cc
        # nvector3D.cc
        # ndirection2D.cc
        # ndirection3D.cc
        # nline2D.cc
        # nline3D.cc
        # nplane3D.cc
        # nspline2D.cc
)

set(DEVICE_LIBRARIES
        #TkModLabKeithley
        #TkModLabGreisinger
        #TkModLabJulabo
        #TkModLabHameg
        #TkModLabPfeiffer
        #TkModLabHuber
        #TkModLabIota
        TkModLabArduino
        #TkModLabCori
        #TkModLabNanotec
        #TkModLabConrad
        #TkModLabVelleman
        #TkModLabLeybold
        TkModLabRohdeSchwarz
        TkModLabAgilent
        #TkModLabMarta
)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOMOC ON)

include_directories(${PROJECT_SOURCE_DIR})
include_directories(${Qt5Core_INCLUDE_DIRS})
include_directories(${Qt5Widgts_INCLUDE_DIRS})

add_definitions(${Qt5Core_DEFINITIONS})
add_definitions(${Qt5Widgts_DEFINITIONS})

set(CMAKE_CXX_FLAGS "${Qt5Core_EXECUTABLE_COMPILE_FLAGS}")
set(CMAKE_CXX_FLAGS "${Qt5Widgts_EXECUTABLE_COMPILE_FLAGS}")

set_target_properties(Common PROPERTIES VERSION ${PROJECT_VERSION})

target_compile_definitions(Common PRIVATE QT_NO_JAVA_STYLE_ITERATORS QT_NO_URL_CAST_FROM_STRING QT_NO_CAST_TO_ASCII)
target_compile_features(Common PUBLIC cxx_std_17)

target_link_libraries(Common Qt5::Core Qt5::Widgets ${CURL_LIBRARIES} ${DEVICE_LIBRARIES})