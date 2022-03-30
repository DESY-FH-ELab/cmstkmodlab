configure_file(ApplicationConfig.h.in ApplicationConfig.h)
configure_file(ApplicationConfig.cc.in ApplicationConfig.cc)

add_library(Common SHARED
        nqlogger.cc
        npoint2D.cc
        npoint3D.cc
        nvector2D.cc
        nvector3D.cc
        ndirection2D.cc
        ndirection3D.cc
        nline2D.cc
        nline3D.cc
        nplane3D.cc
        nspline2D.cc
        SingletonApplication.cc
        ApplicationConfigReader.cc
        ApplicationConfigWriter.cc
        ApplicationConfigItemModel.cc
        ApplicationConfigViewer.cc
        VScriptableDevice.cc
        SlackBot.cc
        MattermostBot.cc
        JulaboModel.cc
        ScriptableJulabo.cc
        JulaboWidget.cc
        HuberPetiteFleurModel.cc
        ScriptableHuberPetiteFleur.cc
        HuberPetiteFleurWidget.cc
        KeithleyModel.cc
        ScriptableKeithley.cc
        KeithleyWidget.cc
        HamegModel.cc
        ScriptableHameg.cc
        HamegWidget.cc
        PfeifferModel.cc
        ScriptablePfeiffer.cc
        PfeifferWidget.cc
        IotaModel.cc
        ScriptableIota.cc
        IotaWidget.cc
        ArduinoPresWidget.cc
        ScriptableArduinoPres.cc
        ArduinoPresModel.cc
        CoriFlowWidget.cc
        ScriptableCoriFlow.cc
        CoriFlowModel.cc
        IntegerComboBox.cc
        NanotecSMCI36Model.cc
        NanotecSMCI36Widget.cc
        VRelayCardModel.cc
        ConradModel.cc
        VellemanModel.cc
        LeyboldGraphixThreeModel.cc
        LeyboldGraphixOneModel.cc
        LeyboldGraphixOneWidget.cc
        ScriptableLeyboldGraphixOne.cc
        RohdeSchwarzNGE103BModel.cc
        RohdeSchwarzNGE103BWidget.cc
        ScriptableRohdeSchwarzNGE103B.cc
        KeithleyDAQ6510Model.cc
        KeithleyDAQ6510Widget.cc
        ScriptableKeithleyDAQ6510.cc
        HuberUnistat525wModel.cc
        HuberUnistat525wWidget.cc
        ScriptableHuberUnistat525w.cc
        AgilentTwisTorr304Model.cc
        AgilentTwisTorr304Widget.cc
        MartaModel.cc
        MartaWidget.cc
        ScriptableMarta.cc
        ${CMAKE_BINARY_DIR}/common/ApplicationConfig.cc
)

set(DEVICE_LIBRARIES
        TkModLabAgilent
        TkModLabArduino
        TkModLabConrad
        TkModLabCori
        TkModLabGreisinger
        TkModLabHameg
        TkModLabHuber
        TkModLabIota
        TkModLabJulabo
        TkModLabKeithley
        TkModLabKeyence
        TkModLabLang
        TkModLabLeybold
        TkModLabMarta
        TkModLabNanotec
        TkModLabPfeiffer
        TkModLabRohdeSchwarz
        TkModLabVelleman
)

if(CMSTKMODLAB_ASSEMBLY OR CMSTKMODLAB_DEFO)
	list(APPEND DEVICE_LIBRARIES
        TkModLabCanon
	)
endif()

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOMOC ON)

include_directories(${PROJECT_SOURCE_DIR})

include_directories(${Qt5Core_INCLUDE_DIRS})
include_directories(${Qt5Widgts_INCLUDE_DIRS})
include_directories(${Qt5Script_INCLUDE_DIRS})
include_directories(${Qt5Svg_INCLUDE_DIRS})

add_definitions(${Qt5Core_DEFINITIONS})
add_definitions(${Qt5Widgts_DEFINITIONS})
add_definitions(${Qt5Script_DEFINITIONS})
add_definitions(${Qt5Svg_DEFINITIONS})

set(CMAKE_CXX_FLAGS "${Qt5Core_EXECUTABLE_COMPILE_FLAGS}")
set(CMAKE_CXX_FLAGS "${Qt5Widgts_EXECUTABLE_COMPILE_FLAGS}")
set(CMAKE_CXX_FLAGS "${Qt5Script_EXECUTABLE_COMPILE_FLAGS}")
set(CMAKE_CXX_FLAGS "${Qt5Svg_EXECUTABLE_COMPILE_FLAGS}")

set_target_properties(Common PROPERTIES VERSION ${PROJECT_VERSION})

target_compile_definitions(Common PRIVATE QT_NO_JAVA_STYLE_ITERATORS QT_NO_URL_CAST_FROM_STRING QT_NO_CAST_TO_ASCII)

target_link_libraries(Common
        PRIVATE Qt5::Core
        PRIVATE Qt5::Widgets
        PRIVATE Qt5::Script
        PRIVATE Qt5::Svg
        PUBLIC ${CURL_LIBRARIES}
        PUBLIC ddierckx
        ${DEVICE_LIBRARIES}
)

INSTALL(
    TARGETS Common
    EXPORT Common
    RUNTIME DESTINATION bin
    ARCHIVE DESTINATION lib
)
