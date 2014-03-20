//--------------------------------------------------------------------------------------
//
//  File:       RequestCounterClient/main.cpp
//
//  Project:    YarpPlusPlus
//
//  Contains:   The main application for the client of the request counter service.
//
//  Written by: Norman Jaffe
//
//  Copyright:  (c) 2014 by HPlus Technologies Ltd. and Simon Fraser University.
//
//              All rights reserved. Redistribution and use in source and binary forms,
//              with or without modification, are permitted provided that the following
//              conditions are met:
//                * Redistributions of source code must retain the above copyright
//                  notice, this list of conditions and the following disclaimer.
//                * Redistributions in binary form must reproduce the above copyright
//                  notice, this list of conditions and the following disclaimer in the
//                  documentation and/or other materials provided with the
//                  distribution.
//                * Neither the name of the copyright holders nor the names of its
//                  contributors may be used to endorse or promote products derived
//                  from this software without specific prior written permission.
//
//              THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
//              "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
//              LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
//              PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
//              OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
//              SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
//              LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
//              DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
//              THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
//              (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
//              OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//
//  Created:    2014-03-14
//
//--------------------------------------------------------------------------------------

//#define ENABLE_OD_SYSLOG /* */
#include "ODSyslog.h"
#include "YPPRequestCounterClient.h"
#include <iostream>
#include <yarp/os/all.h>

using namespace YarpPlusPlus;
using std::cin;
using std::cout;
using std::cerr;
using std::endl;

#if defined(__APPLE__)
# pragma mark Private structures, constants and variables
#endif // defined(__APPLE__)

/*! @brief Run loop control; @c true if the service is to keep going and @c false otherwise. */
static bool lKeepRunning;

#if defined(__APPLE__)
# pragma mark Local functions
#endif // defined(__APPLE__)

#if (defined(__APPLE__) || defined(__linux__))
/*! @brief The signal handler to catch requests to stop the service.
 @param signal The signal being handled. */
static void stopRunning(int signal)
{
# pragma unused(signal)
    lKeepRunning = false;
} // stopRunning
#endif // defined(__APPLE__) || defined(__linux__)

#if defined(__APPLE__)
# pragma mark Global functions
#endif // defined(__APPLE__)

/*! @brief The entry point for creating an example client.
 @param argc The number of arguments in 'argv'.
 @param argv The arguments to be used with the example client.
 @returns @c 0 on a successful test and @c 1 on failure. */
int main(int     argc,
         char ** argv)
{
#if defined(ENABLE_OD_SYSLOG)
# pragma unused(argc)
#else // ! defined(ENABLE_OD_SYSLOG)
# pragma unused(argc,argv)
#endif // ! defined(ENABLE_OD_SYSLOG)
    OD_SYSLOG_INIT(*argv, kODSyslogOptionIncludeProcessID | kODSyslogOptionIncludeThreadID |//####
                   kODSyslogOptionEnableThreadSupport | kODSyslogOptionWriteToStderr);//####
    OD_SYSLOG_ENTER();//####
    try
    {
        if (yarp::os::Network::checkNetwork())
        {
#if defined(ENABLE_OD_SYSLOG)
            yarp::os::Network::setVerbosity(1);
#else // ! defined(ENABLE_OD_SYSLOG)
            yarp::os::Network::setVerbosity(-1);
#endif // ! defined(ENABLE_OD_SYSLOG)
            yarp::os::Network yarp; // This is necessary to establish any connection to the YARP infrastructure
            
            YarpPlusPlus::Initialize();
            RequestCounterClient * stuff = new RequestCounterClient;
            
            if (stuff)
            {
                lKeepRunning = true;
#if (defined(__APPLE__) || defined(__linux__))
                signal(SIGHUP, stopRunning);
                signal(SIGINT, stopRunning);
                signal(SIGINT, stopRunning);
                signal(SIGUSR1, stopRunning);
#endif // defined(__APPLE__) || defined(__linux__)
                for ( ; lKeepRunning; )
                {
                    int count;
                    
                    cout << "How many requests? ";
                    cin >> count;
                    if (0 >= count)
                    {
                        break;
                    }
                    
                    if (stuff->findService("name:RequestCounter"))
                    {
                        if (stuff->resetServiceCounters())
                        {
                            for (int ii = 0; ii < count; ++ii)
                            {
                                if (! stuff->pokeService())
                                {
                                    cerr << "Problem poking the service." << endl;
                                    break;
                                }
                                
                            }
                            long   counter;
                            double elapsedTime;
                            
                            if (stuff->getServiceStatistics(counter, elapsedTime))
                            {
                                if (0 < counter)
                                {
                                    cout << "count = " << counter << ", elapsed time = " << elapsedTime <<
                                            ", average time = " << (elapsedTime / counter) << "." << endl;
                                }
                                else
                                {
                                    cout << "Service reports zero requests." << endl;
                                }
                            }
                            else
                            {
                                OD_SYSLOG("! (stuff->getServiceStatistics(counter, elapsedTime))");//####
                                cerr << "Problem getting statistics from the service." << endl;
                            }
                        }
                        else
                        {
                            OD_SYSLOG("! (stuff->resetServiceCounters())");//####
                            cerr << "Problem resetting the service counters." << endl;
                        }
                    }
                    else
                    {
                        OD_SYSLOG("! (stuff->findService(\"name:RequestCounter\"))");//####
                        cerr << "Problem locating the service." << endl;
                    }
                }
                delete stuff;
            }
            else
            {
                OD_SYSLOG("! (stuff)");//####
            }
        }
        else
        {
            OD_SYSLOG("! (yarp::os::Network::checkNetwork())");//####
            cerr << "YARP network not running." << endl;
        }
    }
    catch (...)
    {
        OD_SYSLOG("Exception caught");//####
    }
    yarp::os::Network::fini();
    OD_SYSLOG_EXIT_L(0);//####
    return 0;
} // main
