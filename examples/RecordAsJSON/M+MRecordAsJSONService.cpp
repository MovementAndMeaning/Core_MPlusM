//--------------------------------------------------------------------------------------------------
//
//  File:       M+MRecordAsJSONService.cpp
//
//  Project:    M+M
//
//  Contains:   The class definition for the Record As JSON output service.
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
//  Created:    2014-11-26
//
//--------------------------------------------------------------------------------------------------

#include "M+MRecordAsJSONService.h"
#include "M+MRecordAsJSONInputHandler.h"
#include "M+MRecordAsJSONRequests.h"

#include <mpm/M+MEndpoint.h>
#include <mpm/M+MGeneralChannel.h>

//#include <odl/ODEnableLogging.h>
#include <odl/ODLogging.h>

#if defined(__APPLE__)
# pragma clang diagnostic push
# pragma clang diagnostic ignored "-Wunknown-pragmas"
# pragma clang diagnostic ignored "-Wdocumentation-unknown-command"
#endif // defined(__APPLE__)
/*! @file
 @brief The class definition for the Record As JSON output service. */
#if defined(__APPLE__)
# pragma clang diagnostic pop
#endif // defined(__APPLE__)

#if defined(__APPLE__)
# pragma mark Namespace references
#endif // defined(__APPLE__)

using namespace MplusM;
using namespace MplusM::Common;
using namespace MplusM::RecordAsJSON;

#if defined(__APPLE__)
# pragma mark Private structures, constants and variables
#endif // defined(__APPLE__)

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

RecordAsJSONService::RecordAsJSONService(const YarpString & launchPath,
                                         const int          argc,
                                         char * *           argv,
                                         const YarpString & tag,
                                         const YarpString & serviceEndpointName,
                                         const YarpString & servicePortNumber) :
    inherited(launchPath, argc, argv, tag, true, MpM_RECORDASJSON_CANONICAL_NAME,
              RECORDASJSONOUTPUT_SERVICE_DESCRIPTION, "", serviceEndpointName, servicePortNumber),
    _outFile(NULL), _inHandler(new RecordAsJSONInputHandler)
{
    OD_LOG_ENTER(); //####
    OD_LOG_S4s("launchPath = ", launchPath, "tag = ", tag, "serviceEndpointName = ", //####
               serviceEndpointName, "servicePortNumber = ", servicePortNumber); //####
    OD_LOG_LL1("argc = ", argc); //####
    OD_LOG_P1("argv = ", argv); //####
    OD_LOG_EXIT_P(this); //####
} // RecordAsJSONService::RecordAsJSONService

RecordAsJSONService::~RecordAsJSONService(void)
{
    OD_LOG_OBJENTER(); //####
    stopStreams();
    delete _inHandler;
    OD_LOG_OBJEXIT(); //####
} // RecordAsJSONService::~RecordAsJSONService

#if defined(__APPLE__)
# pragma mark Actions and Accessors
#endif // defined(__APPLE__)

bool RecordAsJSONService::configure(const yarp::os::Bottle & details)
{
    OD_LOG_OBJENTER(); //####
    OD_LOG_P1("details = ", &details); //####
    bool result = false;
    
    try
    {
        if (! isActive())
        {
            if (1 == details.size())
            {
                yarp::os::Value firstValue(details.get(0));
                
                if (firstValue.isString())
                {
                    _outPath = firstValue.asString();
                    OD_LOG_S1s("_outPath <- ", _outPath); //####
                    result = true;
                }
            }
        }
    }
    catch (...)
    {
        OD_LOG("Exception caught"); //####
        throw;
    }
    OD_LOG_OBJEXIT_B(result); //####
    return result;
} // RecordAsJSONService::configure

void RecordAsJSONService::restartStreams(void)
{
    OD_LOG_OBJENTER(); //####
    try
    {
        // No special processing needed.
        stopStreams();
        startStreams();
    }
    catch (...)
    {
        OD_LOG("Exception caught"); //####
        throw;
    }
    OD_LOG_OBJEXIT(); //####
} // RecordAsJSONService::restartStreams

bool RecordAsJSONService::setUpStreamDescriptions(void)
{
    OD_LOG_OBJENTER(); //####
    bool               result = true;
    ChannelDescription description;
    YarpString         rootName(getEndpoint().getName() + "/");
    
    _inDescriptions.clear();
    description._portName = rootName + "input";
    description._portProtocol = "*";
    description._protocolDescription = "Arbitrary YARP messages";
    _inDescriptions.push_back(description);
    OD_LOG_OBJEXIT_B(result); //####
    return result;
} // RecordAsJSONService::setUpStreamDescriptions

bool RecordAsJSONService::start(void)
{
    OD_LOG_OBJENTER(); //####
    try
    {
        if (! isStarted())
        {
            inherited::start();
            if (isStarted())
            {
            
            }
            else
            {
                OD_LOG("! (isStarted())"); //####
            }
        }
    }
    catch (...)
    {
        OD_LOG("Exception caught"); //####
        throw;
    }
    OD_LOG_OBJEXIT_B(isStarted()); //####
    return isStarted();
} // RecordAsJSONService::start

void RecordAsJSONService::startStreams(void)
{
    OD_LOG_OBJENTER(); //####
    try
    {
        if (! isActive())
        {
#if MAC_OR_LINUX_
            _outFile = fopen(_outPath.c_str(), "w");
#else // ! MAC_OR_LINUX_
            if (! fopen_s(&_outFile, _outPath.c_str(), "w"))
            {
                _outFile = NULL;
            }
#endif // ! MAC_OR_LINUX_
            if (_outFile)
            {
                if (_inHandler)
                {
                    _inHandler->setFile(_outFile);
                    _inStreams.at(0)->setReader(*_inHandler);
                    setActive();
                }
                else
                {
                    fclose(_outFile);
                    _outFile = NULL;
                }
            }
        }
    }
    catch (...)
    {
        OD_LOG("Exception caught"); //####
        throw;
    }
    OD_LOG_OBJEXIT(); //####
} // RecordAsJSONService::startStreams

bool RecordAsJSONService::stop(void)
{
    OD_LOG_OBJENTER(); //####
    bool result;
    
    try
    {
        result = inherited::stop();
    }
    catch (...)
    {
        OD_LOG("Exception caught"); //####
        throw;
    }
    OD_LOG_OBJEXIT_B(result); //####
    return result;
} // RecordAsJSONService::stop

void RecordAsJSONService::stopStreams(void)
{
    OD_LOG_OBJENTER(); //####
    try
    {
        if (isActive())
        {
            if (_inHandler)
            {
                _inHandler->setFile(NULL);
            }
            fclose(_outFile);
            _outFile = NULL;
            clearActive();
        }
    }
    catch (...)
    {
        OD_LOG("Exception caught"); //####
        throw;
    }
    OD_LOG_OBJEXIT(); //####
} // RecordAsJSONService::stopStreams

#if defined(__APPLE__)
# pragma mark Global functions
#endif // defined(__APPLE__)