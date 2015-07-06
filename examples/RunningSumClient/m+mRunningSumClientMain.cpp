//--------------------------------------------------------------------------------------------------
//
//  File:       m+mRunningSumClientMain.cpp
//
//  Project:    m+m
//
//  Contains:   The main application for the Running Sum client.
//
//  Written by: Norman Jaffe
//
//  Copyright:  (c) 2014 by H Plus Technologies Ltd. and Simon Fraser University.
//
//              All rights reserved. Redistribution and use in source and binary forms, with or
//              without modification, are permitted provided that the following conditions are met:
//                * Redistributions of source code must retain the above copyright notice, this list
//                  of conditions and the following disclaimer.
//                * Redistributions in binary form must reproduce the above copyright notice, this
//                  list of conditions and the following disclaimer in the documentation and / or
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
//  Created:    2014-03-18
//
//--------------------------------------------------------------------------------------------------

#include "m+mRunningSumClient.h"

#include <m+m/m+mUtilities.h>

//#include <odl/ODEnableLogging.h>
#include <odl/ODLogging.h>

#if defined(__APPLE__)
# pragma clang diagnostic push
# pragma clang diagnostic ignored "-Wunknown-pragmas"
# pragma clang diagnostic ignored "-Wdocumentation-unknown-command"
#endif // defined(__APPLE__)
/*! @file
 @brief The main application for the Running Sum client. */
/*! @dir RunningSumClient
 @brief The set of files that implement the Running Sum client. */
#if defined(__APPLE__)
# pragma clang diagnostic pop
#endif // defined(__APPLE__)

#if defined(__APPLE__)
# pragma mark Namespace references
#endif // defined(__APPLE__)

using namespace MplusM;
using namespace MplusM::Common;
using namespace MplusM::Example;
using std::cerr;
using std::cin;
using std::cout;
using std::endl;

#if defined(__APPLE__)
# pragma mark Private structures, constants and variables
#endif // defined(__APPLE__)

#if defined(__APPLE__)
# pragma mark Global constants and variables
#endif // defined(__APPLE__)

#if defined(__APPLE__)
# pragma mark Local functions
#endif // defined(__APPLE__)

/*! @brief Display the available commands. */
static void displayCommands(void)
{
    OD_LOG_ENTER(); //####
    cout << "Commands:" << endl;
    cout << "  ? - display this list" << endl;
    cout << "  + - read a number from the terminal and update the running sum" << endl;
    cout << "  q - quit the application" << endl;
    cout << "  r - reset the running sum" << endl;
    cout << "  s - start calculating the running sum" << endl;
    OD_LOG_EXIT(); //####
} // displayCommands

/*! @brief Set up the environment and perform the operation. */
#if defined(MpM_ReportOnConnections)
static void setUpAndGo(ChannelStatusReporter * reporter)
#else // ! defined(MpM_ReportOnConnections)
static void setUpAndGo(void)
#endif // ! defined(MpM_ReportOnConnections)
{
    OD_LOG_ENTER(); //####
#if defined(MpM_ReportOnConnections)
    OD_LOG_P1("reporter = ", reporter); //####
#endif // defined(MpM_ReportOnConnections)
    RunningSumClient * aClient = new RunningSumClient;
    
    if (aClient)
    {
        StartRunning();
        SetSignalHandlers(SignalRunningStop);
        if (aClient->findService("Name: RunningSum"))
        {
#if defined(MpM_ReportOnConnections)
            aClient->setReporter(*reporter, true);
#endif // defined(MpM_ReportOnConnections)
            if (aClient->connectToService())
            {
                for ( ; IsRunning(); )
                {
                    char   inChar;
                    double newSum;
                    double value;
                    
                    cout << "Operation: [? + q r s]? ";
                    cout.flush();
                    cin >> inChar;
                    switch (inChar)
                    {
                        case '?' :
                            // Help
                            displayCommands();
                            break;
                            
                        case '+' :
                            cout << "add: ";
                            cout.flush();
                            cin >> value;
                            cout << "adding " << value << endl;
                            if (aClient->addToSum(value, newSum))
                            {
                                cout << "running sum = " << newSum << endl;
                            }
                            else
                            {
                                OD_LOG("! (aClient->addToSum(value, newSum))"); //####
#if MAC_OR_LINUX_
                                GetLogger().fail("Problem adding to the sum.");
#endif // MAC_OR_LINUX_
                            }
                            break;
                            
                        case 'q' :
                        case 'Q' :
                            cout << "Exiting" << endl;
                            if (! aClient->stopSum())
                            {
                                OD_LOG("(! aClient->stopSum())"); //####
#if MAC_OR_LINUX_
                                GetLogger().fail("Problem stopping the sum.");
#endif // MAC_OR_LINUX_
                            }
                            StopRunning();
                            break;
                            
                        case 'r' :
                        case 'R' :
                            cout << "Resetting" << endl;
                            if (! aClient->resetSum())
                            {
                                OD_LOG("(! aClient->resetSum())"); //####
#if MAC_OR_LINUX_
                                GetLogger().fail("Problem resetting the sum.");
#endif // MAC_OR_LINUX_
                            }
                            break;
                            
                        case 's' :
                        case 'S' :
                            cout << "Starting" << endl;
                            if (! aClient->startSum())
                            {
                                OD_LOG("(! aClient->startSum())"); //####
#if MAC_OR_LINUX_
                                GetLogger().fail("Problem starting the sum.");
#endif // MAC_OR_LINUX_
                            }
                            break;
                            
                        default :
                            cout << "Unrecognized request '" << inChar << "'." << endl;
                            break;
                            
                    }
                }
                if (! aClient->disconnectFromService())
                {
                    OD_LOG("(! aClient->disconnectFromService())"); //####
#if MAC_OR_LINUX_
                    GetLogger().fail("Problem disconnecting from the service.");
#endif // MAC_OR_LINUX_
                }
            }
            else
            {
                OD_LOG("! (aClient->connectToService())"); //####
#if MAC_OR_LINUX_
                GetLogger().fail("Could not connect to the required service.");
#else // ! MAC_OR_LINUX_
                cerr << "Could not connect to the required service." << endl;
#endif // ! MAC_OR_LINUX_
            }
        }
        else
        {
            OD_LOG("! (aClient->findService(\"Name: RunningSum\")"); //####
#if MAC_OR_LINUX_
            GetLogger().fail("Could not find the required service.");
#else // ! MAC_OR_LINUX_
            cerr << "Could not find the required service." << endl;
#endif // ! MAC_OR_LINUX_
        }
        delete aClient;
    }
    else
    {
        OD_LOG("! (aClient)"); //####
    }
    OD_LOG_EXIT(); //####
} // setUpAndGo

#if defined(__APPLE__)
# pragma mark Global functions
#endif // defined(__APPLE__)

#if (! MAC_OR_LINUX_)
# pragma warning(push)
# pragma warning(disable: 4100)
#endif // ! MAC_OR_LINUX_
/*! @brief The entry point for communicating with the Running Sum service.
 
 Commands from standard input will be interpreted and will result in requests being sent to the
 service.
 Commands are not case-sensitive. The commands are:
 
 @c ? Display the list of commands.
 
 @c + Read a number from standard input and send it to the service.
 
 @c r Ask the service to reset its running sum.
 
 @c s Ask the service to start calculating the running sum.
 
 @c x Ask the service to stop calculating the running sum and exit from the program.
 
 @param argc The number of arguments in 'argv'.
 @param argv The arguments to be used with the application.
 @returns @c 0 on a successful test and @c 1 on failure. */
int main(int      argc,
         char * * argv)
{
#if MAC_OR_LINUX_
# pragma unused(argc)
#endif // MAC_OR_LINUX_
    YarpString progName(*argv);

    OD_LOG_INIT(progName.c_str(), kODLoggingOptionIncludeProcessID | //####
                kODLoggingOptionIncludeThreadID | kODLoggingOptionEnableThreadSupport | //####
                kODLoggingOptionWriteToStderr); //####
    OD_LOG_ENTER(); //####
#if MAC_OR_LINUX_
    SetUpLogger(progName);
#endif // MAC_OR_LINUX_
    try
    {
        Utilities::DescriptorVector argumentList;
        OutputFlavour               flavour;
        
        if (Utilities::ProcessStandardClientOptions(argc, argv, argumentList,
                                                    "The client for the Running Sum service",
                                                    2014, STANDARD_COPYRIGHT_NAME_, flavour, true))
        {
            if (CanReadFromStandardInput())
            {
                Utilities::SetUpGlobalStatusReporter();
                Utilities::CheckForNameServerReporter();
                if (Utilities::CheckForValidNetwork())
                {
#if defined(MpM_ReportOnConnections)
                    ChannelStatusReporter * reporter = Utilities::GetGlobalStatusReporter();
#endif // defined(MpM_ReportOnConnections)
                    yarp::os::Network       yarp; // This is necessary to establish any connections
                                                  // to the YARP infrastructure
                    
                    Initialize(progName);
                    if (Utilities::CheckForRegistryService())
                    {
#if defined(MpM_ReportOnConnections)
                        setUpAndGo(reporter);
#else // ! defined(MpM_ReportOnConnections)
                        setUpAndGo();
#endif // ! defined(MpM_ReportOnConnections)
                    }
                    else
                    {
                        OD_LOG("! (Utilities::CheckForRegistryService())"); //####
#if MAC_OR_LINUX_
                        GetLogger().fail("Registry Service not running.");
#else // ! MAC_OR_LINUX_
                        cerr << "Registry Service not running." << endl;
#endif // ! MAC_OR_LINUX_
                    }
                }
                else
                {
                    OD_LOG("! (Utilities::CheckForValidNetwork())"); //####
#if MAC_OR_LINUX_
                    GetLogger().fail("YARP network not running.");
#else // ! MAC_OR_LINUX_
                    cerr << "YARP network not running." << endl;
#endif // ! MAC_OR_LINUX_
                }
                Utilities::ShutDownGlobalStatusReporter();
            }
        }
    }
    catch (...)
    {
        OD_LOG("Exception caught"); //####
    }
    yarp::os::Network::fini();
    OD_LOG_EXIT_L(0); //####
    return 0;
} // main
#if (! MAC_OR_LINUX_)
# pragma warning(pop)
#endif // ! MAC_OR_LINUX_