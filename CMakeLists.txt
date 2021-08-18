cmake_minimum_required(VERSION 3.1)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(CMAKE_VERSION VERSION_LESS "3.7.0")
        set(CMAKE_INCLUDE_CURRENT_DIR ON)
endif()

project(cmstkmodlab VERSION 1.0.0)

if(CMAKE_HOST_SYSTEM_NAME STREQUAL "Darwin")
        option(FAKEIO "Build with fake IO support" ON)
else()
        option(FAKEIO "Build with fake IO support" OFF)
endif()

if(FAKEIO)
        message("Fake IO support: ON")
        add_compile_definitions(USE_FAKEIO)
else()
        message("Fake IO support: OFF")
endif()

FIND_PACKAGE(CURL)
IF(CURL_FOUND)
        INCLUDE_DIRECTORIES(${CURL_INCLUDE_DIR})
ELSE(CURL_FOUND)
        MESSAGE(FATAL_ERROR "Could not find the CURL library and development files.")
ENDIF(CURL_FOUND)

set(CMAKE_AUTOMOC ON)

find_package(Qt5 COMPONENTS Core Gui Widgets OpenGL Script XML Svg REQUIRED)

if(CMAKE_HOST_SYSTEM_NAME STREQUAL "Darwin")
        set(ENV{ROOT_DIR} /opt/local/libexec/root6/share/root/cmake)
endif()
find_package(ROOT 6.16 CONFIG REQUIRED)

add_subdirectory(devices)
add_subdirectory(common)
