//--------------------------------------------------------------------------------------------------
//
//  File:       m+mWhereRequestHandler.cpp
//
//  Project:    m+m
//
//  Contains:   The class definition for the request handler for a 'where' request.
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
//  Created:    2015-02-11
//
//--------------------------------------------------------------------------------------------------

#include "m+mWhereRequestHandler.hpp"
#include "m+mTunnelRequests.hpp"
#include "m+mTunnelService.hpp"

//#include <odlEnable.h>
#include <odlInclude.h>

#if defined(__APPLE__)
# pragma clang diagnostic push
# pragma clang diagnostic ignored "-Wunknown-pragmas"
# pragma clang diagnostic ignored "-Wdocumentation-unknown-command"
#endif // defined(__APPLE__)
/*! @file
 @brief The class definition for the request handler for a 'where' request. */
#if defined(__APPLE__)
# pragma clang diagnostic pop
#endif // defined(__APPLE__)

#if defined(__APPLE__)
# pragma mark Namespace references
#endif // defined(__APPLE__)

using namespace MplusM;
using namespace MplusM::Tunnel;
using namespace MplusM::Common;

#if defined(__APPLE__)
# pragma mark Private structures, constants and variables
#endif // defined(__APPLE__)

/*! @brief The protocol version number for the 'where' request. */
#define WHERE_REQUEST_VERSION_NUMBER_ "1.0"

#if defined(__APPLE__)
# pragma mark Global constants and variables
#endif // defined(__APPLE__)

#if defined(__APPLE__)
# pragma mark Local functions
#endif // defined(__APPLE__)

#if defined(__APPLE__)
# pragma mark Class methods
#endif // defined(__APPLE__)

#if defined(__APPLE__)
# pragma mark Constructors and Destructors
#endif // defined(__APPLE__)

WhereRequestHandler::WhereRequestHandler(TunnelService & service) :
    inherited(MpM_WHERE_REQUEST_, service)
{
    ODL_ENTER(); //####
    ODL_P1("service = ", &service); //####
    ODL_EXIT_P(this); //####
} // WhereRequestHandler::WhereRequestHandler

WhereRequestHandler::~WhereRequestHandler(void)
{
    ODL_OBJENTER(); //####
    ODL_OBJEXIT(); //####
} // WhereRequestHandler::~WhereRequestHandler

#if defined(__APPLE__)
# pragma mark Actions and Accessors
#endif // defined(__APPLE__)

void
WhereRequestHandler::fillInDescription(const YarpString &   request,
                                       yarp::os::Property & info)
{
    ODL_OBJENTER(); //####
    ODL_S1s("request = ", request); //####
    ODL_P1("info = ", &info); //####
    try
    {
        info.put(MpM_REQREP_DICT_REQUEST_KEY_, request);
        info.put(MpM_REQREP_DICT_VERSION_KEY_, WHERE_REQUEST_VERSION_NUMBER_);
        info.put(MpM_REQREP_DICT_DETAILS_KEY_, T_("Return the remembered IP address and port\n"
                                                  "Input: nothing\n"
                                                  "Output: the remembered IP address and port"));
        yarp::os::Value    keywords;
        yarp::os::Bottle * asList = keywords.asList();

        asList->addString(request);
        info.put(MpM_REQREP_DICT_KEYWORDS_KEY_, keywords);
    }
    catch (...)
    {
        ODL_LOG("Exception caught"); //####
        throw;
    }
    ODL_OBJEXIT(); //####
} // WhereRequestHandler::fillInDescription

#if (! MAC_OR_LINUX_)
# pragma warning(push)
# pragma warning(disable: 4100)
#endif // ! MAC_OR_LINUX_
bool
WhereRequestHandler::processRequest(const YarpString &           request,
                                    const yarp::os::Bottle &     restOfInput,
                                    const YarpString &           senderChannel,
                                    yarp::os::ConnectionWriter * replyMechanism)
{
#if (! defined(ODL_ENABLE_LOGGING_))
# if MAC_OR_LINUX_
#  pragma unused(request,restOfInput,senderChannel)
# endif // MAC_OR_LINUX_
#endif // ! defined(ODL_ENABLE_LOGGING_)
    ODL_OBJENTER(); //####
    ODL_S3s("request = ", request, "restOfInput = ", restOfInput.toString(), //####
            "senderChannel = ", senderChannel); //####
    ODL_P1("replyMechanism = ", replyMechanism); //####
    bool result = true;

    try
    {
        YarpString address;
        int        port;

        static_cast<TunnelService &>(_service).getAddress(address, port);
        _response.clear();
        _response.addString(address);
        _response.addInt(port);
        sendResponse(replyMechanism);
    }
    catch (...)
    {
        ODL_LOG("Exception caught"); //####
        throw;
    }
    ODL_OBJEXIT_B(result); //####
    return result;
} // WhereRequestHandler::processRequest
#if (! MAC_OR_LINUX_)
# pragma warning(pop)
#endif // ! MAC_OR_LINUX_

#if defined(__APPLE__)
# pragma mark Global functions
#endif // defined(__APPLE__)
