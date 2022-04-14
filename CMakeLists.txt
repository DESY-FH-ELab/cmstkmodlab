cmake_minimum_required(VERSION 3.1)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(CMAKE_VERSION VERSION_LESS "3.7.0")
        set(CMAKE_INCLUDE_CURRENT_DIR ON)
endif()

project(cmstkmodlab VERSION 2.3.0)

execute_process(COMMAND bash -c "git describe --dirty --always --tags" OUTPUT_VARIABLE GIT_DESCRIBE)
string(REGEX REPLACE "\n$" "" GIT_DESCRIBE "${GIT_DESCRIBE}")

message("")
message("############################################")
message("")
message("     cmstkmodlab")
message("")
message(" version: ${GIT_DESCRIBE}")
message("")
message("############################################")
message("")

set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)

set(basepath ${PROJECT_SOURCE_DIR})

# Configure the installation prefix to allow both local as system-wide installation
IF(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
    SET(CMAKE_INSTALL_PREFIX
        "${PROJECT_SOURCE_DIR}"
        CACHE PATH "Prefix prepended to install directories" FORCE)
ENDIF()

# Set up the RPATH so executables find the libraries even when installed in non-default location
SET(CMAKE_MACOSX_RPATH TRUE)
SET(CMAKE_SKIP_BUILD_RPATH FALSE)
SET(CMAKE_SKIP_INSTALL_RPATH FALSE)
SET(CMAKE_BUILD_WITH_INSTALL_RPATH FALSE)
SET(CMAKE_INSTALL_RPATH ${CMAKE_INSTALL_FULL_LIBDIR})

# Add the automatically determined parts of the RPATH which point to directories outside the build tree to the install RPATH
SET(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)

# The RPATH to be used when installing, but only if it's not a system directory
LIST(FIND CMAKE_PLATFORM_IMPLICIT_LINK_DIRECTORIES "${CMAKE_INSTALL_PREFIX}/lib" IsSystemDir)
IF("${IsSystemDir}" STREQUAL "-1")
    SET(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib:${CMAKE_SOURCE_DIR}/devices/lib:${CMAKE_SOURCE_DIR}/external/lib")
ENDIF("${IsSystemDir}" STREQUAL "-1")

find_package(PkgConfig REQUIRED)

add_compile_definitions(__${CMAKE_HOST_SYSTEM_NAME}__)

if(CMAKE_HOST_SYSTEM_NAME STREQUAL "Darwin")
        add_compile_definitions(DARWIN)
        message(STATUS "Building on MacOS system")
        option(CMSTKMODLAB_FAKEIO "Build with fake IO support" ON)
        option(CMSTKMODLAB_FAKEUEYE "Build with fake uEYE support" ON)
else()
        option(CMSTKMODLAB_FAKEIO "Build with fake IO support" OFF)
        option(CMSTKMODLAB_FAKEUEYE "Build with fake uEYE support" OFF)
endif()

if(CMSTKMODLAB_FAKEIO)
        message(STATUS "Fake IO support: ON")
        add_compile_definitions(USE_FAKEIO)
else()
        message(STATUS "Fake IO support: OFF")
endif()

if(CMSTKMODLAB_FAKEUEYE)
        message(STATUS "Fake uEYE support: ON")
        add_compile_definitions(NOUEYE)
else()
        message(STATUS "Fake uEYE support: OFF")
endif()

find_package(CURL)
if(CURL_FOUND)
        INCLUDE_DIRECTORIES(${CURL_INCLUDE_DIR})
        message(STATUS "Found libcurl")
else(CURL_FOUND)
        MESSAGE(FATAL_ERROR "Could not find the CURL library and development files.")
endif(CURL_FOUND)

set(CMAKE_AUTOMOC ON)

find_package(Qt5 COMPONENTS Core Gui Widgets OpenGL Script Xml Svg Network Charts REQUIRED)

set(ENV{ROOT_DIR} ENV{ROOTSYS}/cmake)
find_package(ROOT 6.16 REQUIRED)
message(STATUS "Found ROOT (${ROOT_VERSION})")

option(CMSTKMODLAB_MARTA "Build with support for MARTA CO2 chiller" OFF)

if(CMSTKMODLAB_MARTA)
        message(STATUS "Support for MARTA CO2 chiller: ON")
        pkg_check_modules(MODBUS REQUIRED IMPORTED_TARGET libmodbus)
else()
        message(STATUS "Support for MARTA CO2 chiller: OFF")
        add_compile_definitions(NOMARTA)
endif()

option(CMSTKMODLAB_THERMO1 "Build with support for version 1 of thermal test setup" OFF)
if(CMSTKMODLAB_THERMO1)
        message(STATUS "Support for version 1 of thermal test setup: ON")
        include(FindQWT)
else()
        message(STATUS "Support for version 1 of thermal test setup: OFF")
endif()

option(CMSTKMODLAB_THERMO2 "Build with support for version 2 of thermal test setup" OFF)
if(CMSTKMODLAB_THERMO2)
        message(STATUS "Support for version 2 of thermal test setup: ON")
else()
        message(STATUS "Support for version 2 of thermal test setup: OFF")
endif()

if(CMSTKMODLAB_THERMO1 OR CMSTKMODLAB_THERMO2)
        pkg_check_modules(GSL REQUIRED IMPORTED_TARGET gsl)
        add_subdirectory(thermo)
endif()

option(CMSTKMODLAB_PUMPSTATION "Build with support for pump station" OFF)
if(CMSTKMODLAB_PUMPSTATION)
        message(STATUS "Support for pump station: ON")
        add_subdirectory(pumpstation)
else()
        message(STATUS "Support for pump station: OFF")
endif()

option(CMSTKMODLAB_ASSEMBLY "Build with support for PS automated assembly" OFF)
if(CMSTKMODLAB_ASSEMBLY)
        message(STATUS "Support for PS automated assembly: ON")
        add_subdirectory(assembly)
else()
        message(STATUS "Support for PS automated assembly: OFF")
endif()

option(CMSTKMODLAB_PLASMA "Build with support for plasma cleaner test setup" OFF)
if(CMSTKMODLAB_PLASMA)
        message(STATUS "Support for plasma cleaner test setup: ON")
        add_subdirectory(plasma)
else()
        message(STATUS "Support for plasma cleaner test setup: OFF")
endif()

option(CMSTKMODLAB_DEFO "Build with support for defo setup" OFF)
if(CMSTKMODLAB_DEFO)
        message(STATUS "Support for defo setup: ON")
        add_subdirectory(defo)
else()
        message(STATUS "Support for defo setup: OFF")
endif()

option(CMSTKMODLAB_PYTHON "Build python device modules" OFF)
if(CMSTKMODLAB_PYTHON)
        message(STATUS "Python device modules: ON")
        find_package(Python3 3.8 COMPONENTS Development REQUIRED)
        find_package(Boost COMPONENTS python38 REQUIRED)
        add_subdirectory(python)
else()
        message(STATUS "Python device modules: OFF")
endif()

if(CMSTKMODLAB_ASSEMBLY OR CMSTKMODLAB_DEFO)
        pkg_check_modules(LIBGPHOTO2 REQUIRED IMPORTED_TARGET libgphoto2)
        if(CMAKE_HOST_SYSTEM_NAME STREQUAL "Darwin")
                find_package(OpenCV REQUIRED)
        else()
                pkg_check_modules(OPENCV4 REQUIRED IMPORTED_TARGET opencv4)
        endif()
        pkg_check_modules(EXIV2 REQUIRED IMPORTED_TARGET exiv2)
endif()

if(CMSTKMODLAB_ASSEMBLY AND NOT CMSTKMODLAB_FAKEUEYE)
        include(Findueyeapi)
endif()

configure_file(tkmodlabenv.sh.in tkmodlabenv.sh)
configure_file(tkmodlabenv.csh.in tkmodlabenv.csh)

add_subdirectory(external)
add_subdirectory(devices)
add_subdirectory(tools)
add_subdirectory(common)
add_subdirectory(common_test)

enable_testing()
add_test(NAME testApplicationConfig COMMAND testApplicationConfig)
add_test(NAME testRingbuffer COMMAND testRingbuffer)
add_test(NAME testFifo COMMAND testFifo)
add_test(NAME testHistoryFifo COMMAND testHistoryFifo)
