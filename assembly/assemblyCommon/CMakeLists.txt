set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOMOC ON)

add_library(AssemblyCommon SHARED
        AssemblyVUEyeCamera.cc
        AssemblyVUEyeModel.cc
        AssemblyUEyeWidget.cc
        AssemblyUEyeCameraWidget.cc
        AssemblyUEyeCameraThread.cc
        AssemblyUEyeView.cc
        AssemblyUEyeSnapShooter.cc
        AssemblyZFocusFinder.cc
        AssemblyImageController.cc
        AssemblyImageView.cc
        AssemblyThresholder.cc
        AssemblyThresholderView.cc
        AssemblyObjectFinderPatRec.cc
        AssemblyObjectFinderPatRecWidget.cc
        AssemblyObjectFinderPatRecView.cc
        AssemblyObjectFinderPatRecThread.cc
        AssemblyObjectAligner.cc
        AssemblyObjectAlignerView.cc
        AssemblyAssembly.cc
        AssemblyAssemblyView.cc
        AssemblyAssemblyV2.cc
        AssemblyAssemblyV2View.cc
        AssemblyAssemblyActionWidget.cc
        AssemblyAssemblyTextWidget.cc
        AssemblyMultiPickupTester.cc
        AssemblyMultiPickupTesterWidget.cc
        AssemblyPositionsRegistryWidget.cc
        AssemblyToolboxView.cc
        AssemblySmartMotionManager.cc
        AssemblyParametersView.cc
        AssemblyVacuumWidget.cc
        AssemblyMotionWidgets.cc
        AssemblyHardwareControlView.cc
        AssemblyLogFileController.cc
        AssemblyLogFileView.cc
        RelayCardManager.cc
        LStepExpressModel.cc
        LStepExpressMotion.cc
        LStepExpressMotionManager.cc
        LStepExpressMotionView.cc
        LStepExpressMotionThread.cc
        LStepExpressWidget.cc
        LStepExpressJoystickWidget.cc
        LStepExpressSettings.cc
        LStepExpressSettingsWidget.cc
        LStepExpressMeasurement.cc
        LStepExpressMeasurementWidget.cc
        LStepExpressMeasurementTable.cc
        LStepExpressPositionWidget.cc
        LStepExpressStatusWindow.cc
        LaserModel.cc
        LaserWidget.cc
        LaserThread.cc
        LaserControlsWidget.cc
        AssemblyUtilities.cc
        AssemblyDBLoggerController.cc
        AssemblyDBLoggerModel.cc
        AssemblyDBLoggerView.cc
)

if(CMSTKMODLAB_FAKEUEYE)
        target_sources(AssemblyCommon PRIVATE
                AssemblyUEyeFakeCamera.cc
                AssemblyUEyeFakeModel.cc)
else()
target_sources(AssemblyCommon PRIVATE
        AssemblyUEyeCamera.cc
        AssemblyUEyeModel.cc)
endif()

include_directories(${PROJECT_SOURCE_DIR})

include_directories(${ROOT_INCLUDE_DIRS})

include_directories(${OPENCV4_INCLUDE_DIRS})

include_directories(${Qt5Core_INCLUDE_DIRS})
include_directories(${Qt5Widgts_INCLUDE_DIRS})
include_directories(${Qt5Script_INCLUDE_DIRS})
include_directories(${Qt5Svg_INCLUDE_DIRS})

include_directories(${UEYE_HEADER_DIR})

add_definitions(${Qt5Core_DEFINITIONS})
add_definitions(${Qt5Widgts_DEFINITIONS})
add_definitions(${Qt5Script_DEFINITIONS})
add_definitions(${Qt5Svg_DEFINITIONS})

set(CMAKE_CXX_FLAGS "${Qt5Core_EXECUTABLE_COMPILE_FLAGS}")
set(CMAKE_CXX_FLAGS "${Qt5Widgts_EXECUTABLE_COMPILE_FLAGS}")
set(CMAKE_CXX_FLAGS "${Qt5Script_EXECUTABLE_COMPILE_FLAGS}")
set(CMAKE_CXX_FLAGS "${Qt5Svg_EXECUTABLE_COMPILE_FLAGS}")

set_target_properties(AssemblyCommon PROPERTIES VERSION ${PROJECT_VERSION})

target_compile_definitions(AssemblyCommon PRIVATE QT_NO_JAVA_STYLE_ITERATORS QT_NO_URL_CAST_FROM_STRING QT_NO_CAST_TO_ASCII)
target_compile_features(AssemblyCommon PUBLIC cxx_std_17)

INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/common)
INCLUDE_DIRECTORIES(${CMAKE_BINARY_DIR}/common)

target_link_libraries(AssemblyCommon
        PRIVATE Qt5::Core
        PRIVATE Qt5::Widgets
        PRIVATE Qt5::Script
        PRIVATE Qt5::Svg
        PRIVATE Common
        PRIVATE ROOT::Gpad
	PUBLIC  ${UEYE_LIBRARY}
)

INSTALL(
    TARGETS AssemblyCommon
    EXPORT Assembly
    RUNTIME DESTINATION bin
    ARCHIVE DESTINATION lib
)
