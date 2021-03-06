//--------------------------------------------------------------------------------------------------
//
//  File:       m+m/m+mBaseChannel.cpp
//
//  Project:    m+m
//
//  Contains:   The class definition for common resources for channels.
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
//  Created:    2014-10-08
//
//--------------------------------------------------------------------------------------------------

#include "m+mBaseChannel.hpp"

#include <m+m/m+mBailOut.hpp>

//#include <odlEnable.h>
#include <odlInclude.h>

#if defined(__APPLE__)
# pragma clang diagnostic push
# pragma clang diagnostic ignored "-Wunknown-pragmas"
# pragma clang diagnostic ignored "-Wdocumentation-unknown-command"
#endif // defined(__APPLE__)
/*! @file
 @brief The class definition for common resources for channels. */
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

/*! @brief Set the IP address for the endpoint.
 @param[in] workingContact The connection information that is to be filled in.
 @param[in] channelName The desired endpoint name.
 @return @c true if the connection information has been constructed and @c false otherwise. */
static bool
setChannelIPAddress(yarp::os::Contact & workingContact,
                    const YarpString &  channelName)
{
#if defined(MpM_ReportContactDetails)
    DumpContactToLog("enter setIPAddress", workingContact); //####
#endif // defined(MpM_ReportContactDetails)
    bool result = false;

    try
    {
        yarp::os::Contact aContact = yarp::os::Network::registerName(channelName);
        YarpString        ipAddress = aContact.getHost();

        ODL_S1s("ipAddress = ", ipAddress); //####
        yarp::os::Network::unregisterName(channelName);
        workingContact = workingContact.addSocket(CHANNEL_CARRIER_, ipAddress, 0);
#if defined(MpM_ReportContactDetails)
        DumpContactToLog("after addSocket", workingContact); //####
#endif // defined(MpM_ReportContactDetails)
        result = workingContact.isValid();
    }
    catch (...)
    {
        ODL_LOG("Exception caught"); //####
        throw;
    }
    return result;
} // setChannelIPAddress

#if defined(__APPLE__)
# pragma mark Class methods
#endif // defined(__APPLE__)

void
BaseChannel::RelinquishChannel(BaseChannel * theChannel)
{
    ODL_ENTER(); //####
    ODL_P1("theChannel = ", theChannel); //####
    if (theChannel)
    {
#if (! defined(MpM_DontUseTimeouts))
        SetUpCatcher();
#endif // ! defined(MpM_DontUseTimeouts)
        try
        {
#if (! defined(MpM_DontUseTimeouts))
            BailOut bailer(*theChannel, STANDARD_WAIT_TIME_);
#endif // ! defined(MpM_DontUseTimeouts)

            delete theChannel;
        }
        catch (...)
        {
            ODL_LOG("Exception caught"); //####
            throw;
        }
#if (! defined(MpM_DontUseTimeouts))
        ShutDownCatcher();
#endif // ! defined(MpM_DontUseTimeouts)
    }
    ODL_EXIT(); //####
} // BaseChannel::RelinquishChannel

#if defined(__APPLE__)
# pragma mark Constructors and Destructors
#endif // defined(__APPLE__)

BaseChannel::BaseChannel(void) :
    inherited(), _name(), _counters(), _metricsEnabled(false)
{
    ODL_ENTER(); //####
    ODL_EXIT_P(this); //####
} // BaseChannel::BaseChannel

BaseChannel::~BaseChannel(void)
{
    ODL_OBJENTER(); //####
    ODL_OBJEXIT(); //####
} // BaseChannel::~BaseChannel

#if defined(__APPLE__)
# pragma mark Actions and Accessors
#endif // defined(__APPLE__)

void
BaseChannel::close(void)
{
    ODL_OBJENTER(); //####
#if (! defined(MpM_DontUseTimeouts))
    SetUpCatcher();
#endif // ! defined(MpM_DontUseTimeouts)
    try
    {
#if (! defined(MpM_DontUseTimeouts))
        BailOut bailer(*this, STANDARD_WAIT_TIME_);
#endif // ! defined(MpM_DontUseTimeouts)

        inherited::interrupt();
        ODL_LOG("about to close"); //####
        inherited::close();
        ODL_LOG("close completed."); //####
    }
    catch (...)
    {
        ODL_LOG("Exception caught"); //####
        throw;
    }
#if (! defined(MpM_DontUseTimeouts))
    ShutDownCatcher();
#endif // ! defined(MpM_DontUseTimeouts)
    ODL_OBJEXIT(); //####
} // BaseChannel::close

void
BaseChannel::disableMetrics(void)
{
    ODL_OBJENTER(); //####
    _metricsEnabled = false;
    ODL_OBJEXIT(); //####
} // BaseChannel::disableMetrics

void
BaseChannel::enableMetrics(void)
{
    ODL_OBJENTER(); //####
    _metricsEnabled = true;
    ODL_OBJEXIT(); //####
} // BaseChannel::enableMetrics

void
BaseChannel::getSendReceiveCounters(SendReceiveCounters & counters)
{
    ODL_OBJENTER(); //####
    ODL_P1("counters = ", &counters); //####
    counters = _counters;
    ODL_OBJEXIT(); //####
} // BaseChannel::getSendReceiveCounters

#if (! MAC_OR_LINUX_)
# pragma warning(push)
# pragma warning(disable: 4100)
#endif // ! MAC_OR_LINUX_
bool
BaseChannel::openWithRetries(const YarpString & theChannelName,
                             const double       timeToWait)
{
#if ((! RETRY_LOOPS_USE_TIMEOUTS) && (! defined(ODL_ENABLE_LOGGING_)))
# if MAC_OR_LINUX_
#  pragma unused(timeToWait)
# endif // MAC_OR_LINUX_
#endif // (! RETRY_LOOPS_USE_TIMEOUTS) && (! defined(ODL_ENABLE_LOGGING_))
    ODL_OBJENTER(); //####
    ODL_S1s("theChannelName = ", theChannelName); //####
    ODL_D1("timeToWait = ", timeToWait); //####
    bool              result = false;
    yarp::os::Contact aContact = yarp::os::Contact::byName(theChannelName);

#if defined(MpM_ReportContactDetails)
    DumpContactToLog("after byName", aContact); //####
#endif // defined(MpM_ReportContactDetails)
    if (setChannelIPAddress(aContact, theChannelName))
    {
        result = openWithRetries(aContact, timeToWait);
    }
    ODL_OBJEXIT_B(result); //####
    return result;
} // BaseChannel::openWithRetries
#if (! MAC_OR_LINUX_)
# pragma warning(pop)
#endif // ! MAC_OR_LINUX_

#if (! MAC_OR_LINUX_)
# pragma warning(push)
# pragma warning(disable: 4100)
#endif // ! MAC_OR_LINUX_
bool
BaseChannel::openWithRetries(yarp::os::Contact & theContactInfo,
                             const double        timeToWait)
{
#if ((! RETRY_LOOPS_USE_TIMEOUTS) && (! defined(ODL_ENABLE_LOGGING_)))
# if MAC_OR_LINUX_
#  pragma unused(timeToWait)
# endif // MAC_OR_LINUX_
#endif // (! RETRY_LOOPS_USE_TIMEOUTS) && (! defined(ODL_ENABLE_LOGGING_))
    ODL_OBJENTER(); //####
    ODL_P1("theContactInfo = ", &theContactInfo); //####
    ODL_D1("timeToWait = ", timeToWait); //####
#if defined(MpM_ReportContactDetails)
    DumpContactToLog("theContactInfo = ", theContactInfo); //####
#endif // defined(MpM_ReportContactDetails)
    bool   result = false;
    double retryTime = INITIAL_RETRY_INTERVAL_;
    int    retriesLeft = MAX_RETRIES_;

#if (defined(ODL_ENABLE_LOGGING_) && defined(MpM_LogIncludesYarpTrace))
    inherited::setVerbosity(1);
#else // ! (defined(ODL_ENABLE_LOGGING_) && defined(MpM_LogIncludesYarpTrace))
    inherited::setVerbosity(-1);
#endif // ! (defined(ODL_ENABLE_LOGGING_) && defined(MpM_LogIncludesYarpTrace))
#if RETRY_LOOPS_USE_TIMEOUTS
    SetUpCatcher();
#endif // RETRY_LOOPS_USE_TIMEOUTS
    try
    {
#if RETRY_LOOPS_USE_TIMEOUTS
        BailOut bailer(*this, timeToWait);
#endif // RETRY_LOOPS_USE_TIMEOUTS

        do
        {
            ODL_LOG("about to open"); //####
            result = inherited::open(theContactInfo);
            if (! result)
            {
                if (0 < --retriesLeft)
                {
                    ODL_LOG("%%retry%%"); //####
                    yarp::os::Time::delay(retryTime);
                    retryTime *= RETRY_MULTIPLIER_;
                }
            }
            if (result)
            {
                _name = theContactInfo.getName();
                ODL_S1s("_name <- ", _name); //####
            }
        }
        while ((! result) && (0 < retriesLeft));
    }
    catch (...)
    {
        ODL_LOG("Exception caught"); //####
        throw;
    }
#if RETRY_LOOPS_USE_TIMEOUTS
    ShutDownCatcher();
#endif // RETRY_LOOPS_USE_TIMEOUTS
    ODL_OBJEXIT_B(result); //####
    return result;
} // BaseChannel::openWithRetries
#if (! MAC_OR_LINUX_)
# pragma warning(pop)
#endif // ! MAC_OR_LINUX_

void
BaseChannel::updateReceiveCounters(const size_t numBytes)
{
    ODL_OBJENTER(); //####
    ODL_I1("numBytes = ", numBytes); //####
    if (_metricsEnabled)
    {
        _counters.incrementInCounters(numBytes);
    }
    ODL_OBJEXIT(); //####
} // BaseChannel::updateReceiveCounters

void
BaseChannel::updateSendCounters(const size_t numBytes)
{
    ODL_OBJENTER(); //####
    ODL_I1("numBytes = ", numBytes); //####
    if (_metricsEnabled)
    {
        _counters.incrementOutCounters(numBytes);
    }
    ODL_OBJEXIT(); //####
} // BaseChannel::updateSendCounters

bool
BaseChannel::writeBottle(yarp::os::Bottle & message)
{
    ODL_OBJENTER(); //####
    ODL_S1s("message = ", message.toString()); //####
    size_t messageSize = 0;

    if (_metricsEnabled)
    {
        message.toBinary(&messageSize);
    }
    bool result = inherited::write(message);

    if (result && _metricsEnabled)
    {
        updateSendCounters(messageSize);
    }
    ODL_OBJEXIT_B(result); //####
    return result;
} // BaseChannel::writeBottle

bool
BaseChannel::writeBottle(yarp::os::Bottle & message,
                         yarp::os::Bottle & reply)
{
    ODL_OBJENTER(); //####
    ODL_S1s("message = ", message.toString()); //####
    ODL_P1("reply = ", &reply); //####
    size_t messageSize = 0;

    if (_metricsEnabled)
    {
        message.toBinary(&messageSize);
    }
    bool result = inherited::write(message, reply);

    if (result && _metricsEnabled)
    {
        size_t replySize = 0;

        reply.toBinary(&replySize);
        updateSendCounters(messageSize);
        updateReceiveCounters(replySize);
    }
    ODL_OBJEXIT_B(result); //####
    return result;
} // BaseChannel::writeBottle

#if defined(__APPLE__)
# pragma mark Global functions
#endif // defined(__APPLE__)
