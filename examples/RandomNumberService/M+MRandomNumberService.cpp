//--------------------------------------------------------------------------------------
//
//  File:       M+MRandomNumberService.cpp
//
//  Project:    M+M
//
//  Contains:   The class definition for a simple M+M service.
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
//  Created:    2014-02-06
//
//--------------------------------------------------------------------------------------

#include "M+MRandomNumberService.h"
#include "M+MRandomNumberRequests.h"
#include "M+MRandomRequestHandler.h"

//#include "ODEnableLogging.h"
#include "ODLogging.h"

#if defined(__APPLE__)
# pragma clang diagnostic push
# pragma clang diagnostic ignored "-Wdocumentation-unknown-command"
#endif // defined(__APPLE__)
/*! @file
 
 @brief The class definition for a simple M+M service. */
#if defined(__APPLE__)
# pragma clang diagnostic pop
#endif // defined(__APPLE__)

using namespace MplusM::Example;

#if defined(__APPLE__)
# pragma mark Private structures, constants and variables
#endif // defined(__APPLE__)

#if defined(__APPLE__)
# pragma mark Local functions
#endif // defined(__APPLE__)

#if defined(__APPLE__)
# pragma mark Class methods
#endif // defined(__APPLE__)

#if defined(__APPLE__)
# pragma mark Constructors and destructors
#endif // defined(__APPLE__)

RandomNumberService::RandomNumberService(const char *                  launchPath,
                                         const yarp::os::ConstString & serviceEndpointName,
                                         const yarp::os::ConstString & serviceHostName,
                                         const yarp::os::ConstString & servicePortNumber) :
        inherited(launchPath, true, MpM_RANDOM_CANONICAL_NAME, "An example random number service", serviceEndpointName,
                  serviceHostName, servicePortNumber), _randomHandler(NULL)
{
    OD_LOG_ENTER();//####
    OD_LOG_S4("launchPath = ", launchPath, "serviceEndpointName = ", serviceEndpointName.c_str(),//####
              "serviceHostName = ", serviceHostName.c_str(), "servicePortNumber = ", servicePortNumber.c_str());//####
    attachRequestHandlers();
    OD_LOG_EXIT_P(this);//####
} // RandomNumberService::RandomNumberService

RandomNumberService::~RandomNumberService(void)
{
    OD_LOG_OBJENTER();//####
    detachRequestHandlers();
    OD_LOG_OBJEXIT();//####
} // RandomNumberService::~RandomNumberService

#if defined(__APPLE__)
# pragma mark Actions
#endif // defined(__APPLE__)

void RandomNumberService::attachRequestHandlers(void)
{
    OD_LOG_OBJENTER();//####
    try
    {
        _randomHandler = new RandomRequestHandler;
        if (_randomHandler)
        {
            registerRequestHandler(_randomHandler);
        }
        else
        {
            OD_LOG("! (_randomHandler)");//####
        }
    }
    catch (...)
    {
        OD_LOG("Exception caught");//####
        throw;
    }
    OD_LOG_OBJEXIT();//####
} // RandomNumberService::attachRequestHandlers

void RandomNumberService::detachRequestHandlers(void)
{
    OD_LOG_OBJENTER();//####
    try
    {
        if (_randomHandler)
        {
            unregisterRequestHandler(_randomHandler);
            delete _randomHandler;
            _randomHandler = NULL;
        }
    }
    catch (...)
    {
        OD_LOG("Exception caught");//####
        throw;
    }
    OD_LOG_OBJEXIT();//####
} // RandomNumberService::detachRequestHandlers

bool RandomNumberService::start(void)
{
    OD_LOG_OBJENTER();//####
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
                OD_LOG("! (isStarted())");//####
            }
        }
    }
    catch (...)
    {
        OD_LOG("Exception caught");//####
        throw;
    }
    OD_LOG_OBJEXIT_B(isStarted());//####
    return isStarted();
} // RandomNumberService::start

bool RandomNumberService::stop(void)
{
    OD_LOG_OBJENTER();//####
    bool result;
    
    try
    {
        result = inherited::stop();
    }
    catch (...)
    {
        OD_LOG("Exception caught");//####
        throw;
    }
    OD_LOG_OBJEXIT_B(result);//####
    return result;
} // RandomNumberService::stop