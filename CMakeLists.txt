# Copyright (C) 2015 CODYCO Project
# Author: Serena Ivaldi <serena.ivaldi@inria.fr>
#
# Copyright (C) 2012 MACSi Project - www.macsi.isir.upmc.fr
# Author: Serena Ivaldi <serena.ivaldi@isir.upmc.fr>
#
# Permission is granted to copy, distribute, and/or modify this program
# under the terms of the GNU General Public License, version 2 or any
# later version published by the Free Software Foundation.
#
# This program is distributed in the hope that it will be useful, but
# WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details
#

cmake_minimum_required(VERSION 2.6)

SET(PROJECTNAME taskRecorderGUI)
PROJECT(${PROJECTNAME})

# updated list of commands due to decoupling from the macsi cmakelist
find_package(YARP)
find_package(ICUB)

list(APPEND CMAKE_MODULE_PATH ${YARP_MODULE_PATH})
list(APPEND CMAKE_MODULE_PATH ${ICUB_MODULE_PATH})
include(YarpInstallationHelpers)
include(iCubHelpers)
#INCLUDE(iCubFindDependencies)


find_package(GtkPlus)
#find_package(Gtk2)

set(CMAKE_INSTALL_PREFIX "${CMAKE_CURRENT_SOURCE_DIR}" CACHE PATH "Installation directory" FORCE)
#end of new part

FILE(GLOB folder_header *.h)
FILE(GLOB folder_source *.cpp)
SOURCE_GROUP("Header Files" FILES ${folder_header})
SOURCE_GROUP("Source Files" FILES ${folder_source})

ADD_DEFINITIONS(${GTKPLUS_C_FLAGS})
#ADD_DEFINITIONS(${GTK2_DEFINITIONS})
#link_libraries(${GTKPLUS_LINK_FLAGS})

INCLUDE_DIRECTORIES(${iCubDev_INCLUDE_DIRS}
                    #${ctrlLib_INCLUDE_DIRS}
                    ${GtkPlus_INCLUDE_DIRS}
		    #${GTK2_INCLUDE_DIRS}
		    ${GTKPLUS_INCLUDE_DIR}
                    #${modHelp_INCLUDE_DIRS}
                    #${objects_INCLUDE_DIRS}
                    ${YARP_INCLUDE_DIRS})

ADD_EXECUTABLE(${PROJECTNAME} ${folder_header} ${folder_source})



TARGET_LINK_LIBRARIES(${PROJECTNAME} ctrlLib
				     icubmod
                     			#modHelp
                     			#objects
${GTKPLUS_LINK_FLAGS}
                                     #${GTK2_LIBRARIES}
				     ${GtkPlus_LIBRARIES}
				     ${YARP_LIBRARIES})

INSTALL(TARGETS ${PROJECTNAME} DESTINATION bin)


