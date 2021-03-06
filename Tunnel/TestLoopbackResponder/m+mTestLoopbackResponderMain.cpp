//--------------------------------------------------------------------------------------------------
//
//  File:       m+mTestLoopbackResponderMain.cpp
//
//  Project:    m+m
//
//  Contains:   The main application for the Test Loopback Responder utility.
//
//  Written by: Norman Jaffe
//
//  Copyright:  (c) 2015 by H Plus Technologies Ltd. and Simon Fraser University.
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
//  Created:    2015-02-26
//
//--------------------------------------------------------------------------------------------------

#include <m+m/m+mPortArgumentDescriptor.hpp>
#include <m+m/m+mUtilities.hpp>

//#include <odlEnable.h>
#include <odlInclude.h>

#if defined(__APPLE__)
# pragma clang diagnostic push
# pragma clang diagnostic ignored "-Wunknown-pragmas"
# pragma clang diagnostic ignored "-Wdocumentation-unknown-command"
#endif // defined(__APPLE__)
/*! @file
 @brief The main application for the Test Loopback Responder utility. */

/*! @dir TestLoopbackResponder
 @brief The set of files that implement the Test Loopback Responder utility. */
#if defined(__APPLE__)
# pragma clang diagnostic pop
#endif // defined(__APPLE__)

#if defined(__APPLE__)
# pragma mark Namespace references
#endif // defined(__APPLE__)

using namespace MplusM;
using namespace MplusM::Common;
using std::cout;
using std::endl;

#if defined(__APPLE__)
# pragma mark Private structures, constants and variables
#endif // defined(__APPLE__)

/*! @brief The size of the receive / send buffer. */
#define BUFFER_SIZE_ 1024

//#define CHATTY_OUTPUT_ /* Write out information on activity. */

#if defined(__APPLE__)
# pragma mark Global constants and variables
#endif // defined(__APPLE__)

#if defined(__APPLE__)
# pragma mark Local functions
#endif // defined(__APPLE__)

#if defined(__APPLE__)
# pragma mark Global functions
#endif // defined(__APPLE__)

#if (! MAC_OR_LINUX_)
# pragma warning(push)
# pragma warning(disable: 4100)
#endif // ! MAC_OR_LINUX_
/*! @brief The entry point for communicating with the Test Loopback Control utility.
 @param[in] argc The number of arguments in 'argv'.
 @param[in] argv The arguments to be used with the Test Loopback Responder utility.
 @return @c 0 on a successful test and @c 1 on failure. */
int
main(int      argc,
     char * * argv)
{
#if MAC_OR_LINUX_
# pragma unused(argc)
#endif // MAC_OR_LINUX_
    ODL_INIT(*argv, kODLoggingOptionIncludeProcessID | kODLoggingOptionIncludeThreadID | //####
             kODLoggingOptionEnableThreadSupport | kODLoggingOptionWriteToStderr); //####
    ODL_ENTER(); //####
    try
    {
        Utilities::PortArgumentDescriptor firstArg("port", T_("The outgoing port"),
                                                   Utilities::kArgModeRequired, 12345, false);
        Utilities::DescriptorVector       argumentList;
        OutputFlavour                     flavour; // ignored

        argumentList.push_back(&firstArg);
        if (Utilities::ProcessStandardUtilitiesOptions(argc, argv, argumentList,
                                                       T_("Communicates with the Test Loopback "
                                                          "Control application"), 2015,
                                                       STANDARD_COPYRIGHT_NAME_, flavour, true))
        {
            Utilities::SetUpGlobalStatusReporter();
            try
            {
                int     listenPort = firstArg.getCurrentValue();
                SOCKET  listenSocket;
#if ! MAC_OR_LINUX_
                WORD    wVersionRequested = MAKEWORD(2, 2);
                WSADATA ww;
#endif // ! MAC_OR_LINUX_

#if MAC_OR_LINUX_
                listenSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
                if (INVALID_SOCKET != listenSocket)
                {
                    struct sockaddr_in addr;

                    memset(&addr, 0, sizeof(addr));
                    addr.sin_family = AF_INET;
                    addr.sin_port = htons(listenPort);
                    addr.sin_addr.s_addr = htonl(INADDR_ANY);
                    if (bind(listenSocket, reinterpret_cast<struct sockaddr *>(&addr),
                             sizeof(addr)))
                    {
                        close(listenSocket);
                        listenSocket = INVALID_SOCKET;
                    }
                    else
                    {
                        listen(listenSocket, SOMAXCONN);
                    }
                }
#else // ! MAC_OR_LINUX_
                if (WSAStartup(wVersionRequested, &ww))
                {
                }
                else if ((2 == LOBYTE(ww.wVersion)) && (2 == HIBYTE(ww.wVersion)))
                {
                    listenSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
                    if (INVALID_SOCKET != listenSocket)
                    {
                        SOCKADDR_IN addr;

                        addr.sin_family = AF_INET;
                        addr.sin_port = htons(listenPort);
                        addr.sin_addr.s_addr = htonl(INADDR_ANY);
                        int res = bind(listenSocket, reinterpret_cast<LPSOCKADDR>(&addr),
                                       sizeof(addr));

                        if (SOCKET_ERROR == res)
                        {
                            ODL_LOG("(SOCKET_ERROR == res)"); //####
                            closesocket(listenSocket);
                            listenSocket = INVALID_SOCKET;
                        }
                        else
                        {
                            listen(listenSocket, SOMAXCONN);
                        }
                    }
                }
#endif // ! MAC_OR_LINUX_
                if (INVALID_SOCKET != listenSocket)
                {
                    ODL_LOG("(INVALID_SOCKET != listenSocket)"); //####
                    bool   keepGoing = true;
                    char   theBuffer[BUFFER_SIZE_];
                    ODL_LOG("waiting for a connection"); //####
                    SOCKET loopSocket = accept(listenSocket, NULL, NULL);

                    ODL_I1("loopSocket = ", loopSocket); //####
                    for ( ; keepGoing; )
                    {
#if MAC_OR_LINUX_
                        ssize_t inSize = recv(loopSocket, theBuffer, sizeof(theBuffer), 0);
#else // ! MAC_OR_LINUX_
                        int     inSize = recv(loopSocket, theBuffer, sizeof(theBuffer), 0);
#endif // ! MAC_OR_LINUX_

                        if (0 < inSize)
                        {
#if defined(CHATTY_OUTPUT_)
                            cout << "received " << inSize << " bytes." << endl;
#endif // defined(CHATTY_OUTPUT_)
                            if (send(loopSocket, theBuffer, inSize, 0) != inSize)
                            {
                                ODL_LOG("(send(loopSocket, theBuffer, inSize, 0) != " //####
                                        "inSize)"); //####
#if defined(CHATTY_OUTPUT_)
                                cout << "sent " << inSize << " bytes." << endl;
#endif // defined(CHATTY_OUTPUT_)
                                keepGoing = false;
                            }
                        }
                        else
                        {
                            ODL_LOG("! (0 < inSize)"); //####
                            keepGoing = false;
                        }
                    }
#if MAC_OR_LINUX_
                    shutdown(listenSocket, SHUT_RDWR);
                    shutdown(loopSocket, SHUT_RDWR);
                    close(listenSocket);
                    close(loopSocket);
#else // ! MAC_OR_LINUX_
                    shutdown(listenSocket, SD_BOTH);
                    shutdown(loopSocket, SD_BOTH);
                    closesocket(listenSocket);
                    closesocket(loopSocket);
#endif // ! MAC_OR_LINUX_
                }
            }
            catch (...)
            {
                ODL_LOG("Exception caught"); //####
            }
            Utilities::ShutDownGlobalStatusReporter();
        }
    }
    catch (...)
    {
        ODL_LOG("Exception caught"); //####
    }
    ODL_EXIT_I(0); //####
    return 0;
} // main
#if (! MAC_OR_LINUX_)
# pragma warning(pop)
#endif // ! MAC_OR_LINUX_
