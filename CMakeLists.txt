#############################################################################
#  This file is part of the Stride software.
#  It is free software: you can redistribute it and/or modify
#  it under the terms of the GNU General Public License as published by
#  the Free Software Foundation, either version 3 of the License, or any
#  later version.
#  The software is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#  GNU General Public License for more details.
#  You should have received a copy of the GNU General Public License,
#  along with the software. If not, see <http://www.gnu.org/licenses/>.
#  see http://www.gnu.org/licenses/.
#
#  Copyright 2017, Willem L, Kuylen E & Broeckhove J
#############################################################################
#
#  CMake config.
#
#############################################################################
cmake_minimum_required(VERSION 3.5)
include(CMakeConfig.cmake)
include(CMakeLocal.cmake OPTIONAL)
if(CMAKE_VERSION VERSION_LESS 3.8)
    set(STRIDE_FORCE_NO_PYTHON ON)
endif()

#============================================================================
# Configuration for CMake, for build & install.
#============================================================================
project(STRIDE)

#============================================================================
# Settings for Qt
#============================================================================
# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)
# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)
set(CMAKE_PREFIX_PATH "/opt/Qt/5.10.1/gcc_64")
# set_property(SOURCE cmake-build-release/main/python/stridePYTHON_wrap.cxx PROPERTY SKIP_AUTOMOC ON)

#============================================================================
# Configuration for Qt.
#============================================================================
find_package(Qt5 COMPONENTS Core Widgets Gui Charts REQUIRED)
include_directories(${Qt5Widgets_INCLUDE_DIRS})


#============================================================================
# INSTALL LOCATION for bin, doc etc.
#============================================================================
set(BIN_INSTALL_LOCATION     bin)
set(LIB_INSTALL_LOCATION     lib)
set(DATA_INSTALL_LOCATION    data)
set(CONFIG_INSTALL_LOCATION  config)
set(DOC_INSTALL_LOCATION     doc)
set(TESTS_INSTALL_LOCATION   tests)
set(UI_INSTALL_LOCATION      ui)

#============================================================================
# Configuration for Git.
#============================================================================
find_package(Git QUIET)
if(GIT_FOUND)
    GIT_WC_INFO(${CMAKE_SOURCE_DIR} STRIDE)
else()
    # This is done to eliminate blank output of undefined CMake variables.
    set(GIT_FOUND FALSE)
    set(STRIDE_WC_REVISION_HASH       "0000-0000-000")
    set(STRIDE_WC_LAST_CHANGED_DATE   "0000 0000 0000")
endif()

#============================================================================
# Configuration for CPP.
#============================================================================
include(CMakeCPP.cmake)

#============================================================================
# Configuration for Swig & Python.
#============================================================================
if(NOT STRIDE_FORCE_NO_PYTHON)
	find_package(SWIG)
	find_package(PythonInterp 3)
	find_package(PythonLibs 3)
endif()

#============================================================================
# Configuration for DOC: DOXYGEN
#============================================================================
if(STRIDE_INCLUDE_DOC)
    find_package( Doxygen)
endif()

#============================================================================
# Add subdirs
#============================================================================
add_subdirectory(main)
add_subdirectory(test)
if(STRIDE_INCLUDE_DOC)
	add_subdirectory(doc)
endif()

#============================================================================
# Overview report:
#============================================================================
include(CMakeReport.cmake)

#############################################################################
