cmake_minimum_required(VERSION 3.1)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(CMAKE_VERSION VERSION_LESS "3.7.0")
        set(CMAKE_INCLUDE_CURRENT_DIR ON)
endif()

project(cmstkmodlab VERSION 1.0.0)

find_package(PkgConfig REQUIRED)

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

find_package(CURL)
if(CURL_FOUND)
        INCLUDE_DIRECTORIES(${CURL_INCLUDE_DIR})
else(CURL_FOUND)
        MESSAGE(FATAL_ERROR "Could not find the CURL library and development files.")
endif(CURL_FOUND)

set(CMAKE_AUTOMOC ON)

find_package(Qt5 COMPONENTS Core Gui Widgets OpenGL Script Xml Svg REQUIRED)

#if(CMAKE_HOST_SYSTEM_NAME STREQUAL "Darwin")
#        set(ENV{ROOT_DIR} /opt/local/libexec/root6/share/root/cmake)
#else()
#	set(ENV{ROOT_DIR} /home/mussgill/development/root_bin/cmake)
#endif()
#find_package(ROOT 6.16 CONFIG REQUIRED)

pkg_check_modules(LIBGPHOTO2 REQUIRED IMPORTED_TARGET libgphoto2)
pkg_check_modules(OPENCV REQUIRED IMPORTED_TARGET opencv)
pkg_check_modules(EXIV2 REQUIRED IMPORTED_TARGET exiv2)

add_subdirectory(devices)
add_subdirectory(common)
