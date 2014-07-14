//--------------------------------------------------------------------------------------------------
//
//  File:       RegistryServiceMain.cpp
//
//  Project:    M+M
//
//  Contains:   The main application for the Service Registry M+M service.
//
//  Written by: Norman Jaffe
//
//  Copyright:  (c) 2014 by HPlus Technologies Ltd. and Simon Fraser University.
//
//              All rights reserved. Redistribution and use in source and binary forms, with or
//              without modification, are permitted provided that the following conditions are met:
//                * Redistributions of source code must retain the above copyright notice, this list
//                  of conditions and the following disclaimer.
//                * Redistributions in binary form must reproduce the above copyright notice, this
//                  list of conditions and the following disclaimer in the documentation and/or
//                  other materials provided with the distribution.
//                * Neither the name of the copyright holders nor the names of its contributors may
//                  be used to endorse or promote products derived from this software without
//                  specific prior written permission.
//
//              THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY
//              EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
//              OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT
//              SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
//              INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
//              TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
//              BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
//              CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
//              ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH
//              DAMAGE.
//
//  Created:    2014-02-06
//
//--------------------------------------------------------------------------------------------------

#include "M+MRegistryService.h"
#include "M+MRequests.h"

//#include "ODEnableLogging.h"
#include "ODLogging.h"

#if defined(__APPLE__)
# pragma clang diagnostic push
# pragma clang diagnostic ignored "-Wdocumentation-unknown-command"
#endif // defined(__APPLE__)
/*! @file
 
 @brief The main application for the Service Registry M+M service. */

/*! @dir RegistryService
 @brief The set of files that implement the Service Registry application. */
#if defined(__APPLE__)
# pragma clang diagnostic pop
#endif // defined(__APPLE__)

#if defined(__APPLE__)
# pragma mark Private structures, constants and variables
#endif // defined(__APPLE__)

/*! @brief Set to @c true to use an in-memory database and @c false to use a disk-based database. */
#if defined(MpM_UseDiskDatabase)
# define USE_INMEMORY false
#else // ! defined(MpM_UseDiskDatabase)
# define USE_INMEMORY true
#endif // ! defined(MpM_UseDiskDatabase)

#if defined(__APPLE__)
# pragma mark Local functions
#endif // defined(__APPLE__)

#if defined(__APPLE__)
# pragma mark Global functions
#endif // defined(__APPLE__)

/*! @brief The entry point for running the Service Registry service.
 
 The second, optional, argument is the port number to be used and the first, optional, argument is
 the name of the channel to be used. There is no output.
 @param argc The number of arguments in 'argv'.
 @param argv The arguments to be used with the Service Registry service.
 @returns @c 0 on a successful test and @c 1 on failure. */
int main(int     argc,
         char ** argv)
{
#if defined(MpM_ServicesLogToStandardError)
    OD_LOG_INIT(*argv, kODLoggingOptionIncludeProcessID | kODLoggingOptionIncludeThreadID | //####
                kODLoggingOptionWriteToStderr | kODLoggingOptionEnableThreadSupport); //####
#else // ! defined(MpM_ServicesLogToStandardError)
    OD_LOG_INIT(*argv, kODLoggingOptionIncludeProcessID | kODLoggingOptionIncludeThreadID | //####
                kODLoggingOptionEnableThreadSupport); //####
#endif // ! defined(MpM_ServicesLogToStandardError)
    OD_LOG_ENTER(); //####
    MplusM::Common::SetUpLogger(*argv);
    try
    {
#if CheckNetworkWorks_
        if (yarp::os::Network::checkNetwork())
#endif // CheckNetworkWorks_
        {
            yarp::os::Network                   yarp; // This is necessary to establish any
                                                      // connection to the YARP infrastructure
            MplusM::Registry::RegistryService * stuff = NULL;
            
            MplusM::Common::Initialize(*argv);
            if (1 <= argc)
            {
                switch (argc)
                {
                        // Argument order for tests = program name [, port]
                    case 1 :
                        stuff = new MplusM::Registry::RegistryService(*argv, USE_INMEMORY);
                        break;
                        
                    case 2 :
                        stuff = new MplusM::Registry::RegistryService(*argv, USE_INMEMORY, argv[1]);
                        break;
                        
                    default :
                        break;
                        
                }
            }
            if (stuff)
            {
                if (stuff->start())
                {
                    // Note that the Registry Service is self-registering... so we don't need to
                    // call RegisterLocalService() _or_ start a 'pinger'.
                    MplusM::StartRunning();
                    MplusM::Common::SetSignalHandlers(MplusM::SignalRunningStop);
                    stuff->startChecker();
                    for ( ; MplusM::IsRunning() && stuff; )
                    {
#if defined(MpM_MainDoesDelayNotYield)
                        yarp::os::Time::delay(ONE_SECOND_DELAY);
#else // ! defined(MpM_MainDoesDelayNotYield)
                        yarp::os::Time::yield();
#endif // ! defined(MpM_MainDoesDelayNotYield)
                    }
                    stuff->stop();
                }
                else
                {
                    OD_LOG("! (stuff->start())"); //####
                }
                delete stuff;
            }
            else
            {
                OD_LOG("! (stuff)"); //####
            }
        }
#if CheckNetworkWorks_
        else
        {
            OD_LOG("! (yarp::os::Network::checkNetwork())"); //####
            MplusM::Common::GetLogger().fail("YARP network not running.");
        }
#endif // CheckNetworkWorks_
    }
    catch (...)
    {
        OD_LOG("Exception caught"); //####
    }
    yarp::os::Network::fini();
    OD_LOG_EXIT_L(0); //####
    return 0;
} // main