//--------------------------------------------------------------------------------------------------
//
//  File:       m+m/m+mBailOutThread.cpp
//
//  Project:    m+m
//
//  Contains:   The class definition for a timeout thread for m+m.
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
//  Created:    2014-04-01
//
//--------------------------------------------------------------------------------------------------

#include "m+mBailOutThread.hpp"

#include <m+m/m+mBaseChannel.hpp>

//#include <odlEnable.h>
#include <odlInclude.h>

#if defined(__APPLE__)
# pragma clang diagnostic push
# pragma clang diagnostic ignored "-Wunknown-pragmas"
# pragma clang diagnostic ignored "-Wdocumentation-unknown-command"
#endif // defined(__APPLE__)
/*! @file
 @brief The class definition for a timeout thread for m+m. */
#if defined(__APPLE__)
# pragma clang diagnostic pop
#endif // defined(__APPLE__)

#if defined(__APPLE__)
# pragma mark Namespace references
#endif // defined(__APPLE__)

using namespace MplusM;
using namespace MplusM::Common;

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

BailOutThread::BailOutThread(const double timeToWait) :
    inherited(), _baseChannel(NULL), _timeToWait(timeToWait)
{
    ODL_ENTER(); //####
    ODL_D1("timeToWait = ", timeToWait); //####
    ODL_EXIT_P(this); //####
} // BailOutThread::BailOutThread

BailOutThread::BailOutThread(BaseChannel & channelOfInterest,
                             const double  timeToWait) :
    inherited(), _baseChannel(&channelOfInterest), _timeToWait(timeToWait)
{
    ODL_ENTER(); //####
    ODL_P1("channelOfInterest = ", &channelOfInterest); //####
    ODL_D1("timeToWait = ", timeToWait); //####
    ODL_EXIT_P(this); //####
} // BailOutThread::BailOutThread

BailOutThread::~BailOutThread(void)
{
    ODL_OBJENTER(); //####
    ODL_OBJEXIT(); //####
} // BailOutThread::~BailOutThread

#if defined(__APPLE__)
# pragma mark Actions and Accessors
#endif // defined(__APPLE__)

void
BailOutThread::run(void)
{
    ODL_OBJENTER(); //####
    for ( ; ! isStopping(); )
    {
        if (_endTime <= yarp::os::Time::now())
        {
            ODL_LOG("(_endTime <= yarp::os::Time::now())"); //####
            if (_baseChannel)
            {
                _baseChannel->interrupt();
            }
            raise(STANDARD_SIGNAL_TO_USE_);
#if defined(__APPLE__)
# pragma clang diagnostic push
# pragma clang diagnostic ignored "-Wunreachable-code"
#endif // defined(__APPLE__)
            break;
#if defined(__APPLE__)
# pragma clang diagnostic pop
#endif // defined(__APPLE__)
        }
        ConsumeSomeTime();
    }
    ODL_OBJEXIT(); //####
} // BailOutThread::run

bool
BailOutThread::threadInit(void)
{
    ODL_OBJENTER(); //####
    bool result = true;

    _endTime = yarp::os::Time::now() + _timeToWait;
    ODL_OBJEXIT_B(result); //####
    return result;
} // BailOutThread::threadInit

#if defined(__APPLE__)
# pragma mark Global functions
#endif // defined(__APPLE__)
