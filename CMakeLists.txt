cmake_minimum_required (VERSION 3.0 FATAL_ERROR)

project (QtRCP)

########################################  Setup CMake Settings #########################################################
# Point CMake at any custom modules we may ship
set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake")

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE RelWithDebInfo)
endif()
########################################  Setup Output Settings ########################################################
if(APPLE)
    set(APPLICATION_BUNDLE_DIR ${CMAKE_BINARY_DIR}/${PROJECT_NAME}.app/Contents)
    set(APPLICATION_BIN_DIR ${APPLICATION_BUNDLE_DIR}/MacOS)
    set(APPLICATION_PLUGINS_DIR ${APPLICATION_BUNDLE_DIR}/PlugIns)
    set(APPLICATION_DATA_DIR ${APPLICATION_BUNDLE_DIR}/Resources)
else()
    set(APPLICATION_BIN_DIR ${CMAKE_BINARY_DIR}/bin)
    set(APPLICATION_PLUGINS_DIR ${CMAKE_BINARY_DIR}/plugins)
endif()

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${APPLICATION_BIN_DIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${APPLICATION_PLUGINS_DIR})
# Prefer our bundled versions of dependencies.
set(DEPS_DIR "${CMAKE_CURRENT_SOURCE_DIR}/.deps")
set(DEPS_BUILD_DIR "${DEPS_DIR}/build")
set(DEPS_INSTALL_DIR "${DEPS_DIR}/usr")
set(DEPS_BIN_DIR "${DEPS_INSTALL_DIR}/bin")

list(APPEND CMAKE_PREFIX_PATH ${DEPS_INSTALL_DIR})

########################################  Setup Application Settings ###################################################
set(APPLICATION_VERSION_MAJOR 1)
set(APPLICATION_VERSION_MINOR 0)
set(APPLICATION_VERSION_MICRO 0)
set(APPLICATION_VERSION_PATCH "20140912")

set(APPLICATION_ORGNAME "dailypips.org")
set(APPLICATION_DOMAIN  "www.dailypips.org")
set(APPLICATION_AUTHOR "Xiaojun Gao")
set(APPLICATION_NAME ${PROJECT_NAME})

set(APPLICATION_VERSION "${APPLICATION_VERSION_MAJOR}.${APPLICATION_VERSION_MINOR}.${APPLICATION_VERSION_MICRO}")
set(APPLICATION_COMPACT_VERSION "${APPLICATION_VERSION_MAJOR}.${APPLICATION_VERSION_MINOR}")
set(G_PROJECT_LIBRARY_VERSION ${APPLICATION_VERSION})
set(G_PROJECT_LIBRARY_SOVERSION ${APPLICATION_VERSION_PATCH})

########################################  Test Compiler support C++11  #################################################
if(CMAKE_COMPILER_IS_GNUCXX)
   execute_process(COMMAND ${CMAKE_C_COMPILER} -dumpversion OUTPUT_VARIABLE GCC_VERSION)
   if(GCC_VERSION VERSION_GREATER 4.7 OR GCC_VERSION VERSION_EQUAL 4.7)
        add_definitions("-std=gnu++11")
   else()
        message(FATAL_ERROR "C++11 needed. Therefore a gcc compiler with a version higher than 4.3 is needed.")
   endif()
elseif("${CMAKE_CXX_COMPILER_ID}" MATCHES "Clang")
    execute_process(COMMAND ${CMAKE_C_COMPILER} -dumpversion OUTPUT_VARIABLE CLANG_VERSION)
    if (CLANG_VERSION VERSION_GREATER 4.2 OR CLANG_VERSION VERSION_EQUAL 4.2)
        add_definitions("-std=c++11 -stdlib=libc++")
    else()
        message(FATAL_ERROR "C++11 needed. Therefore a clang compiler with a version higher than 4.3 is needed.")
    endif()
else()
    message(FATAL_ERROR "Unknown CXX Compiler")
endif(CMAKE_COMPILER_IS_GNUCXX)
########################################  Setup QT5  ###################################################################
# Modules used by platform auto-detection
include(CheckLibraryExists)
if(NOT QT_QMAKE_EXECUTABLE)
  find_program(QT_QMAKE_EXECUTABLE_FINDQT NAMES qmake qmake5 qmake-qt5
               PATHS "${QT_SEARCH_PATH}/bin" "$ENV{QTDIR}/bin")
  set(QT_QMAKE_EXECUTABLE ${QT_QMAKE_EXECUTABLE_FINDQT} CACHE PATH "Qt qmake program.")
endif(NOT QT_QMAKE_EXECUTABLE)

if(NOT QT_XMLPATTERNS_EXECUTABLE)
  find_program(QT_XMLPATTERNS_EXECUTABLE_FINDQT NAMES xmlpatterns
               PATHS "${QT_SEARCH_PATH}/bin" "$ENV{QTDIR}/bin")
  set(QT_XMLPATTERNS_EXECUTABLE ${QT_XMLPATTERNS_EXECUTABLE_FINDQT} CACHE PATH "Qt xmlpatterns program.")
endif(NOT QT_XMLPATTERNS_EXECUTABLE)

exec_program(${QT_QMAKE_EXECUTABLE} ARGS "-query QT_VERSION" OUTPUT_VARIABLE QTVERSION)
if(NOT QTVERSION MATCHES "5.*")
  set(QT_FOUND FALSE)
  set(QT5_FOUND FALSE)
  if(Qt5_FIND_REQUIRED)
    message(FATAL_ERROR "CMake was unable to find Qt5, put qmake in your path or set QTDIR/QT_QMAKE_EXECUTABLE.")
  endif(Qt5_FIND_REQUIRED)
  return()
endif(NOT QTVERSION MATCHES "5.*")

exec_program(${QT_QMAKE_EXECUTABLE} ARGS "-query QT_INSTALL_PREFIX" OUTPUT_VARIABLE QT_INSTALL_PREFIX)
set(CMAKE_PREFIX_PATH ${QT_INSTALL_PREFIX} ${CMAKE_PREFIX_PATH})
message(STATUS "Found Qt ${QTVERSION} at ${QT_INSTALL_PREFIX}")
# Find Qt5
find_package(Qt5Core 5.3 REQUIRED)
find_package(Qt5Gui 5.3 REQUIRED)
find_package(Qt5Widgets 5.3 REQUIRED)
find_package(Qt5Network 5.3 REQUIRED)
find_package(Qt5XmlPatterns 5.3 REQUIRED)
find_package(Qt5Concurrent 5.3 REQUIRED)
find_package(Qt5PrintSupport 5.3 REQUIRED)
find_package(Qt5Xml 5.3 REQUIRED)
find_package(Qt5Script 5.3 REQUIRED)
find_package(Qt5Sql 5.3 REQUIRED)
find_package(Qt5Help 5.3 REQUIRED)
find_package(Qt5WebKit 5.3 REQUIRED)
find_package(Qt5WebKitWidgets 5.3 REQUIRED)
find_package(Qt5Quick 5.3 REQUIRED)
find_package(Qt5LinguistTools 5.3 REQUIRED)
find_package(Qt5Svg 5.3 REQUIRED)
if(APPLE)
    find_package(Qt5MacExtras 5.3 REQUIRED)
endif()
if (Qt5_POSITION_INDEPENDENT_CODE)
  set(CMAKE_POSITION_INDEPENDENT_CODE ON)
  message(STATUS "Enable Position Independent Code")
endif()
########################################  Setup Common Settings  #######################################################
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)


########################################################################################################################
add_subdirectory(src)
#add_subdirectory(res)
