set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOMOC ON)

set(SOURCES
        pumpstation.cc
        PumpStationSingletonApplication.cc
        PumpStationHTTPModel.cc
        PumpStationMainWindow.cc
        PumpStationSVGWidget.cc
        PumpStationDialogs.cc
)

if (CMAKE_HOST_SYSTEM_NAME STREQUAL "Darwin")
        set(MACOSX_BUNDLE_BUNDLE_NAME CMSPumpStationControl)
        set(MACOSX_BUNDLE_GUI_IDENTIFIER "org.desy.cmspumpstationcontrol")
        set(MACOSX_BUNDLE_INFO_STRING "Application to control CMS pump station in DAF 25c")
        set(MACOSX_BUNDLE_ICON_FILE "CMSPumpStationControl.icns")
        set(MACOSX_BUNDLE_LONG_VERSION_STRING ${PROJECT_VERSION})
        set(MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION})
        set(MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION})
        add_executable(CMSPumpStationControl MACOSX_BUNDLE ${SOURCES} CMSPumpStationControl.icns)
else()
        add_executable(CMSPumpStationControl ${SOURCES})
endif()

if(CMAKE_HOST_SYSTEM_NAME STREQUAL "Windows")
        target_sources(CMSPumpStationControl PRIVATE
                CMSPumpStationControl.rc
        )
endif()

if(CMAKE_HOST_SYSTEM_NAME STREQUAL "Darwin")
        add_compile_definitions(APPLICATIONVERSIONSTR=\"unknown\")
else()
        execute_process(COMMAND bash -c "git describe --dirty --always --tags" OUTPUT_VARIABLE GIT_DESCRIBE)
	string(REGEX REPLACE "\n$" "" GIT_DESCRIBE "${GIT_DESCRIBE}")
        add_compile_definitions(APPLICATIONVERSIONSTR=\"${GIT_DESCRIBE}\")
endif()

add_compile_definitions(__${CMAKE_HOST_SYSTEM_NAME}__)

include_directories(${PROJECT_SOURCE_DIR})

include_directories(${Qt5Core_INCLUDE_DIRS})
include_directories(${Qt5Widgets_INCLUDE_DIRS})
include_directories(${Qt5Network_INCLUDE_DIRS})
include_directories(${Qt5Svg_INCLUDE_DIRS})

add_definitions(${Qt5Core_DEFINITIONS})
add_definitions(${Qt5Widgets_DEFINITIONS})
add_definitions(${Qt5Network_DEFINITIONS})
add_definitions(${Qt5Svg_DEFINITIONS})

set(CMAKE_CXX_FLAGS "${Qt5Core_EXECUTABLE_COMPILE_FLAGS}")
set(CMAKE_CXX_FLAGS "${Qt5Widgets_EXECUTABLE_COMPILE_FLAGS}")
set(CMAKE_CXX_FLAGS "${Qt5Network_EXECUTABLE_COMPILE_FLAGS}")
set(CMAKE_CXX_FLAGS "${Qt5Svg_EXECUTABLE_COMPILE_FLAGS}")

target_link_libraries(CMSPumpStationControl LINK_PUBLIC Qt5::Widgets Qt5::Network Qt5::Svg)

if(CMAKE_HOST_SYSTEM_NAME STREQUAL "Darwin" OR CMAKE_HOST_SYSTEM_NAME STREQUAL "Windows")
        get_target_property(_qmake_executable Qt5::qmake IMPORTED_LOCATION)
        get_filename_component(_qt_bin_dir "${_qmake_executable}" DIRECTORY)
        find_program(WINDEPLOYQT_EXECUTABLE windeployqt HINTS "${_qt_bin_dir}")
        find_program(MACDEPLOYQT_EXECUTABLE macdeployqt HINTS "${_qt_bin_dir}")
endif()

if(CMAKE_HOST_SYSTEM_NAME STREQUAL "Darwin")
        add_custom_command(TARGET CMSPumpStationControl POST_BUILD
                COMMAND ${MACDEPLOYQT_EXECUTABLE}
                        "$<TARGET_FILE_DIR:CMSPumpStationControl>/../.."
                        -always-overwrite
                COMMENT "Running macdeployqt..."
        )
elseif(CMAKE_HOST_SYSTEM_NAME STREQUAL "Windows")
        add_custom_command(TARGET CMSPumpStationControl POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E
                        env PATH="${_qt_bin_dir}" "${WINDEPLOYQT_EXECUTABLE}"
                        "$<TARGET_FILE:CMSPumpStationControl>"
                COMMENT "Running CMSPumpStationControl..."
        )
endif()

INSTALL(
    TARGETS CMSPumpStationControl
    EXPORT Pumpstation
    RUNTIME DESTINATION bin
    ARCHIVE DESTINATION lib
)
