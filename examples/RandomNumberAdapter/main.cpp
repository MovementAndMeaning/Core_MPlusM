//--------------------------------------------------------------------------------------
//
//  File:       RandomNumberClient/main.cpp
//
//  Project:    MoAndMe
//
//  Contains:   The main application for an adapter of a simple MoAndMe service.
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
//  Created:    2014-03-27
//
//--------------------------------------------------------------------------------------

#include "MoMeAdapterChannel.h"
#include "MoMeChannelStatusReporter.h"
#include "MoMeRandomNumberAdapterData.h"
#include "MoMeRandomNumberClient.h"
#include "MoMeRandomNumberDataInputHandler.h"

//#include "ODEnableLogging.h"
#include "ODLogging.h"

#include <iostream>
#if defined(__APPLE__)
# pragma clang diagnostic push
# pragma clang diagnostic ignored "-Wc++11-extensions"
# pragma clang diagnostic ignored "-Wdocumentation"
# pragma clang diagnostic ignored "-Wdocumentation-unknown-command"
# pragma clang diagnostic ignored "-Wpadded"
# pragma clang diagnostic ignored "-Wshadow"
# pragma clang diagnostic ignored "-Wunused-parameter"
# pragma clang diagnostic ignored "-Wweak-vtables"
#endif // defined(__APPLE__)
#include <yarp/os/all.h>
#if defined(__APPLE__)
# pragma clang diagnostic pop
#endif // defined(__APPLE__)

#if defined(__APPLE__)
# pragma clang diagnostic push
# pragma clang diagnostic ignored "-Wdocumentation-unknown-command"
#endif // defined(__APPLE__)
/*! @file
 
 @brief The main application for an adapter of a simple MoAndMe service. */

/*! @dir RandomNumberAdapter
 @brief The set of files that implement an adapter for a simple MoAndMe service. */
#if defined(__APPLE__)
# pragma clang diagnostic pop
#endif // defined(__APPLE__)

using namespace MoAndMe::Example;
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

/*! @brief The signal handler to catch requests to stop the service.
 @param signal The signal being handled. */
static void stopRunning(int signal)
{
#if (! defined(OD_ENABLE_LOGGING))
# pragma unused(signal)
#endif // ! defined(OD_ENABLE_LOGGING)
    OD_LOG_ENTER();//####
    OD_LOG_LL1("signal = ", signal);//####
    lKeepRunning = false;
    OD_LOG_EXIT();//####
} // stopRunning

#if defined(__APPLE__)
# pragma mark Global functions
#endif // defined(__APPLE__)

/*! @brief The entry point for creating an example client.
 @param argc The number of arguments in 'argv'.
 @param argv The arguments to be used with the example client.
 @returns @c 0 on a successful test and @c 1 on failure. */
int main(int      argc,
         char * * argv)
{
    OD_LOG_INIT(*argv, kODLoggingOptionIncludeProcessID | kODLoggingOptionIncludeThreadID |//####
                kODLoggingOptionEnableThreadSupport | kODLoggingOptionWriteToStderr);//####
    OD_LOG_ENTER();//####
    try
    {
        if (yarp::os::Network::checkNetwork())
        {
            yarp::os::Network yarp; // This is necessary to establish any connection to the YARP infrastructure
            
            MoAndMe::Common::Initialize(*argv);
            RandomNumberClient * stuff = new RandomNumberClient;
            
            if (stuff)
            {
                lKeepRunning = true;
                MoAndMe::Common::SetSignalHandlers(stopRunning);
                if (stuff->findService("keyword random"))
                {
#if defined(MAM_REPORT_ON_CONNECTIONS)
                    MoAndMe::Common::ChannelStatusReporter reporter;
#endif // defined(MAM_REPORT_ON_CONNECTIONS)
                    
#if defined(MAM_REPORT_ON_CONNECTIONS)
                    stuff->setReporter(reporter, true);
#endif // defined(MAM_REPORT_ON_CONNECTIONS)
                    if (stuff->connectToService())
                    {
                        MoAndMe::Common::AdapterChannel * dataChannel = new MoAndMe::Common::AdapterChannel;
                        MoAndMe::Common::AdapterChannel * outputChannel = new MoAndMe::Common::AdapterChannel;
                        RandomNumberAdapterData           sharedData(stuff, outputChannel);
                        RandomNumberDataInputHandler *    dataHandler = new RandomNumberDataInputHandler(sharedData);
                        
                        if (dataChannel && outputChannel && dataHandler)
                        {
                            yarp::os::ConstString dataName("/adapter/data/randomnumber");
                            yarp::os::ConstString outputName("/adapter/output/randomnumber");
                            
                            if (argc > 1)
                            {
                                dataName = argv[1];
                                if (argc > 2)
                                {
                                    outputName = argv[2];
                                }
                            }
#if defined(MAM_REPORT_ON_CONNECTIONS)
                            dataChannel->setReporter(reporter);
                            dataChannel->getReport(reporter);
                            outputChannel->setReporter(reporter);
                            outputChannel->getReport(reporter);
#endif // defined(MAM_REPORT_ON_CONNECTIONS)
                            if (dataChannel->openWithRetries(dataName) && outputChannel->openWithRetries(outputName))
                            {
                                sharedData.activate();
                                dataChannel->setReader(*dataHandler);
                                for ( ; lKeepRunning && sharedData.isActive(); )
                                {
#if defined(MAM_MAIN_DOES_DELAY_NOT_YIELD)
                                    yarp::os::Time::delay(ONE_SECOND_DELAY);
#else // ! defined(MAM_MAIN_DOES_DELAY_NOT_YIELD)
                                    yarp::os::Time::yield();
#endif // ! defined(MAM_MAIN_DOES_DELAY_NOT_YIELD)
                                    if (! lKeepRunning)
                                    {
                                        sharedData.deactivate();
                                    }
                                }
                            }
                            else
                            {
                                OD_LOG("! (dataChannel->openWithRetries(dataName) && "
                                       "outputChannel->openWithRetries(outputName))");//####
                                cerr << "Problem opening a channel." << endl;
                            }
#if defined(MAM_DO_EXPLICIT_CLOSE)
                            dataChannel->close();
                            outputChannel->close();
#endif // defined(MAM_DO_EXPLICIT_CLOSE)
                        }
                        else
                        {
                            OD_LOG("! (controlChannel && dataChannel && outputChannel && controlHandler && "//####
                                   "dataHandler)");//####
                            cerr << "Problem creating a channel." << endl;
                        }
                        MoAndMe::Common::AdapterChannel::RelinquishChannel(dataChannel);
                        MoAndMe::Common::AdapterChannel::RelinquishChannel(outputChannel);
                        if (! stuff->disconnectFromService())
                        {
                            OD_LOG("(! stuff->disconnectFromService())");//####
                            cerr << "Problem disconnecting from the service." << endl;
                        }
                    }
                    else
                    {
                        OD_LOG("! (stuff->connectToService())");//####
                        cerr << "Problem connecting to the service." << endl;
                    }
                }
                else
                {
                    OD_LOG("! (stuff->findService(\"Name RandomNumber\"))");//####
                    cerr << "Problem locating the service." << endl;
                }
                delete stuff;
            }
            else
            {
                OD_LOG("! (stuff)");//####
            }
        }
        else
        {
            OD_LOG("! (yarp::os::Network::checkNetwork())");//####
            cerr << "YARP network not running." << endl;
        }
    }
    catch (...)
    {
        OD_LOG("Exception caught");//####
    }
    yarp::os::Network::fini();
    OD_LOG_EXIT_L(0);//####
    return 0;
} // main
