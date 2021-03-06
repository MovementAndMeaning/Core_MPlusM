#--------------------------------------------------------------------------------------------------
#
#  File:       ViconDataStreamInputService/CMakeLists.txt
#
#  Project:    m+m
#
#  Contains:   The CMAKE definitions for the Vicon DataStream input service application.
#
#  Written by: Norman Jaffe
#
#  Copyright:  (c) 2014 by H Plus Technologies Ltd. and Simon Fraser University.
#
#              All rights reserved. Redistribution and use in source and binary forms, with or
#              without modification, are permitted provided that the following conditions are met:
#                * Redistributions of source code must retain the above copyright notice, this list
#                  of conditions and the following disclaimer.
#                * Redistributions in binary form must reproduce the above copyright notice, this
#                  list of conditions and the following disclaimer in the documentation and / or
#                  other materials provided with the distribution.
#                * Neither the name of the copyright holders nor the names of its contributors may
#                  be used to endorse or promote products derived from this software without
#                  specific prior written permission.
#
#              THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY
#              EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
#              OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT
#              SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
#              INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
#              TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
#              BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
#              CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
#              ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH
#              DAMAGE.
#
#  Created:    2014-11-07
#
#--------------------------------------------------------------------------------------------------

set(THIS_TARGET m+mViconDataStreamInputService)

if(WIN32)
    set(VERS_RESOURCE ${THIS_TARGET}.rc)
else()
    set(VERS_RESOURCE "")
endif()

configure_file(${THIS_TARGET}.rc.in ${THIS_TARGET}.rc)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include_directories(${MpMVICONDATASTREAM_SOURCE_DIR}/ViconDataStreamInputService/ViconLib/inc)

# Set up our program
add_executable(${THIS_TARGET}
               m+mViconDataStreamInputServiceMain.cpp
               m+mViconDataStreamEventThread.cpp
               m+mViconDataStreamInputService.cpp
               ${VERS_RESOURCE})

if(WIN32)
    add_library(ViconDataStreamSDK SHARED IMPORTED)
    if(WIN64)
        set(MpMVICON_LIB_DIR
            "${MpMVICONDATASTREAM_SOURCE_DIR}/ViconDataStreamInputService/ViconLib/Lib/x64")
        set(MpMVICON_BOOST1_DLL "${MpMVICON_LIB_DIR}/boost_thread-vc110-mt-1_55.dll")
        set(MpMVICON_BOOST2_DLL "${MpMVICON_LIB_DIR}/boost_chrono-vc110-mt-1_55.dll")
        set(MpMVICON_BOOST3_DLL "${MpMVICON_LIB_DIR}/boost_python-vc110-mt-1_55.dll")
        set(MpMVICON_BOOST4_DLL "${MpMVICON_LIB_DIR}/boost_system-vc110-mt-1_55.dll")
    else()
        set(MpMVICON_LIB_DIR
            "${MpMVICONDATASTREAM_SOURCE_DIR}/ViconDataStreamInputService/ViconLib/Lib/x86")
        set(MpMVICON_BOOST1_DLL "${MpMVICON_LIB_DIR}/boost_thread-vc90-mt-1_48.dll")
        set(MpMVICON_BOOST2_DLL "")
        set(MpMVICON_BOOST3_DLL "")
        set(MpMVICON_BOOST4_DLL "")
    endif()
    set_property(TARGET ViconDataStreamSDK PROPERTY IMPORTED_LOCATION
                 "${MpMVICON_LIB_DIR}/ViconDataStreamSDK_CPP.dll")
    set_property(TARGET ViconDataStreamSDK PROPERTY IMPORTED_IMPLIB
                 "${MpMVICON_LIB_DIR}/ViconDataStreamSDK_CPP.lib")
    install(FILES
            "${MpMVICON_LIB_DIR}/ViconDataStreamSDK_CPP.dll"
            DESTINATION bin
            COMPONENT applications)
    add_library(BoostThread SHARED IMPORTED)
    set_property(TARGET BoostThread PROPERTY IMPORTED_LOCATION "${MpMVICON_BOOST1_DLL}")
    if(WIN64)
        add_library(BoostChrono SHARED IMPORTED)
        set_property(TARGET BoostChrono PROPERTY IMPORTED_LOCATION "${MpMVICON_BOOST2_DLL}")
        add_library(BoostPython SHARED IMPORTED)
        set_property(TARGET BoostPython PROPERTY IMPORTED_LOCATION "${MpMVICON_BOOST3_DLL}")
        add_library(BoostSystem SHARED IMPORTED)
        set_property(TARGET BoostSystem PROPERTY IMPORTED_LOCATION "${MpMVICON_BOOST4_DLL}")
        install(FILES
                "${MpMVICON_BOOST1_DLL}"
                "${MpMVICON_BOOST2_DLL}"
                "${MpMVICON_BOOST3_DLL}"
                "${MpMVICON_BOOST4_DLL}"
                DESTINATION bin
                COMPONENT applications)
    else()
        install(FILES
                "${MpMVICON_BOOST1_DLL}"
                DESTINATION bin
                COMPONENT applications)
    endif()
    add_custom_command(TARGET ${THIS_TARGET} POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory
                        "${MpMVICON_LIB_DIR}" $<TARGET_FILE_DIR:m+mViconDataStreamInputService>)
endif()

# Note that the order of inclusion of libraries is critical in Linux, as they appear to only be
# processed once.
if(MpM_BuildDummyServices)
    target_link_libraries(${THIS_TARGET} ${MpM_LINK_LIBRARIES})
else()
    target_link_libraries(${THIS_TARGET} ViconDataStreamSDK ${MpM_LINK_LIBRARIES})
endif()

fix_dynamic_libs(${THIS_TARGET})

install(TARGETS ${THIS_TARGET}
        DESTINATION bin
        COMPONENT applications)

set(THIS_TARGET m+mViconBlobInputService)

if(WIN32)
    set(VERS_RESOURCE ${THIS_TARGET}.rc)
else()
    set(VERS_RESOURCE "")
endif()

configure_file(${THIS_TARGET}.rc.in ${THIS_TARGET}.rc)

include_directories(${MpMVICONDATASTREAM_SOURCE_DIR}/ViconDataStreamInputService/ViconLib/inc)

# Set up our program
add_executable(${THIS_TARGET}
                m+mViconBlobInputServiceMain.cpp
                m+mViconBlobEventThread.cpp
                m+mViconBlobInputService.cpp
                ${VERS_RESOURCE})

# Note that the order of inclusion of libraries is critical in Linux, as they appear to only be
# processed once.
if(MpM_BuildDummyServices)
    target_link_libraries(${THIS_TARGET} ${MpM_LINK_LIBRARIES})
else()
    target_link_libraries(${THIS_TARGET} ViconDataStreamSDK ${MpM_LINK_LIBRARIES})
endif()

fix_dynamic_libs(${THIS_TARGET})

install(TARGETS ${THIS_TARGET}
        DESTINATION bin
        COMPONENT applications)

enable_testing()
