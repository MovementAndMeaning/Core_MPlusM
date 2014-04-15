//--------------------------------------------------------------------------------------
//
//  File:       MoMeRequestCounterService.cpp
//
//  Project:    MPlusM
//
//  Contains:   The class definition for a service that collects statistic on requests.
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

#include "MoMeRequestCounterService.h"
#include "MoMeRequestCounterContext.h"
#include "MoMeRequestCounterDefaultRequestHandler.h"
#include "MoMeRequestCounterRequests.h"
#include "MoMeResetRequestHandler.h"
#include "MoMeStatsRequestHandler.h"

//#include "ODEnableLogging.h"
#include "ODLogging.h"

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
#include <yarp/os/Time.h>
#if defined(__APPLE__)
# pragma clang diagnostic pop
#endif // defined(__APPLE__)

#if defined(__APPLE__)
# pragma clang diagnostic push
# pragma clang diagnostic ignored "-Wdocumentation-unknown-command"
#endif // defined(__APPLE__)
/*! @file
 
 @brief The class definition for a service that collects statistic on requests. */
#if defined(__APPLE__)
# pragma clang diagnostic pop
#endif // defined(__APPLE__)

using namespace MplusM::RequestCounter;

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

RequestCounterService::RequestCounterService(const yarp::os::ConstString & serviceEndpointName,
                                             const yarp::os::ConstString & serviceHostName,
                                             const yarp::os::ConstString & servicePortNumber) :
        inherited(true, MpM_REQUESTCOUNTER_CANONICAL_NAME, "The request counter service", serviceEndpointName,
                  serviceHostName, servicePortNumber), _defaultHandler(NULL), _resetHandler(NULL),
        _statsHandler(NULL)
#if (! defined(SERVICES_HAVE_CONTEXTS))
        , _counter(0), _lastReset(yarp::os::Time::now())
#endif // ! defined(SERVICES_HAVE_CONTEXTS)
{
    OD_LOG_ENTER();//####
    OD_LOG_S3("serviceEndpointName = ", serviceEndpointName.c_str(), "serviceHostName = ",//####
              serviceHostName.c_str(), "servicePortNumber = ", servicePortNumber.c_str());//####
    attachRequestHandlers();
    OD_LOG_EXIT_P(this);//####
} // RequestCounterService::RequestCounterService

RequestCounterService::~RequestCounterService(void)
{
    OD_LOG_OBJENTER();//####
    detachRequestHandlers();
    OD_LOG_OBJEXIT();//####
} // RequestCounterService::~RequestCounterService

#if defined(__APPLE__)
# pragma mark Actions
#endif // defined(__APPLE__)

void RequestCounterService::attachRequestHandlers(void)
{
    OD_LOG_OBJENTER();//####
    try
    {
        _defaultHandler = new RequestCounterDefaultRequestHandler(*this);
        _resetHandler = new ResetRequestHandler(*this);
        _statsHandler = new StatsRequestHandler(*this);
        if (_defaultHandler && _resetHandler && _statsHandler)
        {
            registerRequestHandler(_resetHandler);
            registerRequestHandler(_statsHandler);
            setDefaultRequestHandler(_defaultHandler);
        }
        else
        {
            OD_LOG("! (_defaultHandler && _resetHandler && _statsHandler)");//####
        }
    }
    catch (...)
    {
        OD_LOG("Exception caught");//####
        throw;
    }
    OD_LOG_OBJEXIT();//####
} // RequestCounterService::attachRequestHandlers

void RequestCounterService::countRequest(const yarp::os::ConstString & key)
{
#if (! defined(SERVICES_HAVE_CONTEXTS))
# pragma unused(key)
#endif // ! defined(SERVICES_HAVE_CONTEXTS)
    OD_LOG_OBJENTER();//####
    try
    {
#if defined(SERVICES_HAVE_CONTEXTS)
        RequestCounterContext * context = (RequestCounterContext *) findContext(key);
#endif // defined(SERVICES_HAVE_CONTEXTS)

#if defined(SERVICES_HAVE_CONTEXTS)
        if (! context)
        {
            context = new RequestCounterContext;
            addContext(key, context);
        }
        context->counter() += 1;
#else // ! defined(SERVICES_HAVE_CONTEXTS)
        ++_counter;
#endif // ! defined(SERVICES_HAVE_CONTEXTS)
    }
    catch (...)
    {
        OD_LOG("Exception caught");//####
        throw;
    }
    OD_LOG_OBJEXIT();//####
} // RequestCounterService::countRequest

void RequestCounterService::detachRequestHandlers(void)
{
    OD_LOG_OBJENTER();//####
    try
    {
        if (_defaultHandler)
        {
            setDefaultRequestHandler(NULL);
            delete _defaultHandler;
            _defaultHandler = NULL;
        }
        if (_resetHandler)
        {
            unregisterRequestHandler(_resetHandler);
            delete _resetHandler;
            _resetHandler = NULL;
        }
        if (_statsHandler)
        {
            unregisterRequestHandler(_statsHandler);
            delete _statsHandler;
            _statsHandler = NULL;
        }
    }
    catch (...)
    {
        OD_LOG("Exception caught");//####
        throw;
    }
    OD_LOG_OBJEXIT();//####
} // RequestCounterService::detachRequestHandlers

void RequestCounterService::getStatistics(const yarp::os::ConstString & key,
                                          long &                        counter,
                                          double &                      elapsedTime)
{
#if (! defined(SERVICES_HAVE_CONTEXTS))
# pragma unused(key)
#endif // ! defined(SERVICES_HAVE_CONTEXTS)
    OD_LOG_OBJENTER();//####
    try
    {
#if defined(SERVICES_HAVE_CONTEXTS)
        RequestCounterContext * context = (RequestCounterContext *) findContext(key);
#endif // defined(SERVICES_HAVE_CONTEXTS)

#if defined(SERVICES_HAVE_CONTEXTS)
        if (! context)
        {
            context = new RequestCounterContext;
            addContext(key, context);
        }
        counter = context->counter();
        elapsedTime = yarp::os::Time::now() - context->lastReset();
#else // ! defined(SERVICES_HAVE_CONTEXTS)
        counter = _counter;
        elapsedTime = yarp::os::Time::now() - _lastReset;
#endif // ! defined(SERVICES_HAVE_CONTEXTS)
    }
    catch (...)
    {
        OD_LOG("Exception caught");//####
        throw;
    }
    OD_LOG_OBJEXIT();//####
} // RequestCounterService::getStatistics

void RequestCounterService::resetCounters(const yarp::os::ConstString & key)
{
#if (! defined(SERVICES_HAVE_CONTEXTS))
# pragma unused(key)
#endif // ! defined(SERVICES_HAVE_CONTEXTS)
    OD_LOG_OBJENTER();//####
    try
    {
#if defined(SERVICES_HAVE_CONTEXTS)
        RequestCounterContext * context = (RequestCounterContext *) findContext(key);
#endif // defined(SERVICES_HAVE_CONTEXTS)

#if defined(SERVICES_HAVE_CONTEXTS)
        if (! context)
        {
            context = new RequestCounterContext;
            addContext(key, context);
        }
        context->counter() = 0;
        context->lastReset() = yarp::os::Time::now();
#else // ! defined(SERVICES_HAVE_CONTEXTS)
        _counter = 0;
        _lastReset = yarp::os::Time::now();
#endif // ! defined(SERVICES_HAVE_CONTEXTS)
    }
    catch (...)
    {
        OD_LOG("Exception caught");//####
        throw;
    }
    OD_LOG_OBJEXIT();//####
} // RequestCounterService::resetCounters

bool RequestCounterService::start(void)
{
    OD_LOG_OBJENTER();//####
    bool result = false;
    
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
        result = isStarted();
    }
    catch (...)
    {
        OD_LOG("Exception caught");//####
        throw;
    }
    OD_LOG_OBJEXIT_B(result);//####
    return result;
} // RequestCounterService::start

bool RequestCounterService::stop(void)
{
    OD_LOG_OBJENTER();//####
    bool result = false;
    
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
} // RequestCounterService::stop
