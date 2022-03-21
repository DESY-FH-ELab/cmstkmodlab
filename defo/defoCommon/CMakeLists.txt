set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_library(defoCommon SHARED
        DefoExifReader.cc
        DefoJulaboModel.cc
        DefoKeithleyModel.cc
        DefoCameraModel.cc
        DefoConradModel.cc
        DefoMeasurement.cc
        DefoMeasurementListModel.cc
        DefoMeasurementSelectionModel.cc
        DefoMeasurementListComboBox.cc
        DefoImageAverager.cc
        DefoPointRecognitionModel.cc
        DefoPointRecognitionWidget.cc
        DefoThresholdSpinBox.cc
        DefoHalfSquareWidthSpinBox.cc
        DefoImageWidget.cc
        DefoImageZoomModel.cc
        DefoPoint.cc
        DefoPointBin.cc
        DefoSquare.cc
        DefoSpline.cc
        DefoRecoSurface.cc
        DefoSurface.cc
        DefoPointFinder.cc
        DefoPointSaver.cc
        DefoROI.cc
        DefoROIModel.cc
        DefoAlignmentModel.cc
        DefoGeometryModel.cc
        DefoCalibrationModel.cc
        Defo2DSplineInterpolationModel.cc
        DefoColorSelectionModel.cc
        DefoPointIndexerModel.cc
        DefoPointIndexerListComboBox.cc
        DefoVPointIndexer.cc
        DefoPointIndexer.cc
        DefoPropagationPointIndexer.cc
        DefoLensModel.cc
        DefoLensComboBox.cc
)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOMOC ON)

include_directories(${PROJECT_SOURCE_DIR})

include_directories(${OPENCV4_INCLUDE_DIRS})

include_directories(${Qt5Core_INCLUDE_DIRS})
include_directories(${Qt5Widgts_INCLUDE_DIRS})
include_directories(${Qt5Script_INCLUDE_DIRS})
include_directories(${Qt5Svg_INCLUDE_DIRS})

include_directories(${CMAKE_SOURCE_DIR}/common)
include_directories(${CMAKE_BINARY_DIR}/common)

add_definitions(${Qt5Core_DEFINITIONS})
add_definitions(${Qt5Widgts_DEFINITIONS})
add_definitions(${Qt5Script_DEFINITIONS})
add_definitions(${Qt5Svg_DEFINITIONS})

set(CMAKE_CXX_FLAGS "-fPIC -std=c++11")

set(CMAKE_CXX_FLAGS "${Qt5Core_EXECUTABLE_COMPILE_FLAGS}")
set(CMAKE_CXX_FLAGS "${Qt5Widgts_EXECUTABLE_COMPILE_FLAGS}")
set(CMAKE_CXX_FLAGS "${Qt5Script_EXECUTABLE_COMPILE_FLAGS}")
set(CMAKE_CXX_FLAGS "${Qt5Svg_EXECUTABLE_COMPILE_FLAGS}")

set_target_properties(defoCommon PROPERTIES VERSION ${PROJECT_VERSION})

target_compile_definitions(defoCommon PRIVATE QT_NO_JAVA_STYLE_ITERATORS QT_NO_URL_CAST_FROM_STRING QT_NO_CAST_TO_ASCII)
target_compile_features(defoCommon PRIVATE cxx_std_11)

target_link_libraries(defoCommon
        PRIVATE Qt5::Core
        PRIVATE Qt5::Widgets
        PRIVATE Qt5::Script
        PRIVATE Qt5::Svg
        PRIVATE Common
        PUBLIC PkgConfig::EXIV2
)

INSTALL(
    TARGETS defoCommon
    EXPORT Defo
    RUNTIME DESTINATION bin
    ARCHIVE DESTINATION lib
)
