//--------------------------------------------------------------------------------------------------
//
//  File:       m+mAbsorberFilterService.cpp
//
//  Project:    m+m
//
//  Contains:   The class definition for the Absorber filter service.
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
//  Created:    2014-09-30
//
//--------------------------------------------------------------------------------------------------

#include "m+mAbsorberFilterService.hpp"
#include "m+mAbsorberFilterInputHandler.hpp"
#include "m+mAbsorberFilterRequests.hpp"
#include "m+mAbsorberFilterThread.hpp"

#include <m+m/m+mEndpoint.hpp>
#include <m+m/m+mGeneralChannel.hpp>

//#include <odlEnable.h>
#include <odlInclude.h>

#if defined(__APPLE__)
# pragma clang diagnostic push
# pragma clang diagnostic ignored "-Wunknown-pragmas"
# pragma clang diagnostic ignored "-Wdocumentation-unknown-command"
#endif // defined(__APPLE__)
/*! @file
 @brief The class definition for the Absorber filter service. */
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

/*! @brief Convert a positive numeric value into a comma-separated string.
 @param[in] aNumber The number to convert.
 @return A string representation of the number with commas between each triple digit group. */
static YarpString
convertToCommaSplitNumber(const size_t aNumber)
{
    // Note that the function 'convertToCommaSplitNumber' was used because the locale
    // mechanism in C++ for OS X is very, very broken - so we can't use 'std::imbue'!
    YarpString result;
    size_t     work = aNumber;

    do
    {
        size_t            bottom = (work % 1000);
        std::stringstream buff;

        if (1000 <= work)
        {
            buff.width(3);
            buff.fill('0');
        }
        buff << bottom;
        result = YarpString(buff.str()) + result;
        work /= 1000;
        if (0 < work)
        {
            result = YarpString(",") + result;
        }
    }
    while (0 < work);
    return result;
} // convertToCommaSplitNumber

#if defined(__APPLE__)
# pragma mark Class methods
#endif // defined(__APPLE__)

#if defined(__APPLE__)
# pragma mark Constructors and Destructors
#endif // defined(__APPLE__)

AbsorberFilterService::AbsorberFilterService(const Utilities::DescriptorVector & argumentList,
                                             const YarpString &                  launchPath,
                                             const int                           argc,
                                             char * *                            argv,
                                             const YarpString &                  tag,
                                             const YarpString &
                                                                             serviceEndpointName,
                                             const YarpString &
                                                                             servicePortNumber) :
    inherited(argumentList, launchPath, argc, argv, tag, true, MpM_ABSORBERFILTER_CANONICAL_NAME_,
              ABSORBERFILTER_SERVICE_DESCRIPTION_, "", serviceEndpointName, servicePortNumber),
    _inHandler(new AbsorberFilterInputHandler(*this)), _generator(NULL), _count(0), _lastCount(-1),
    _lastBytes(0), _totalBytes(0), _sampleInterval(0)
{
    ODL_ENTER(); //####
    ODL_P2("argumentList = ", &argumentList, "argv = ", argv); //####
    ODL_S4s("launchPath = ", launchPath, "tag = ", tag, "serviceEndpointName = ", //####
            serviceEndpointName, "servicePortNumber = ", servicePortNumber); //####
    ODL_I1("argc = ", argc); //####
    ODL_EXIT_P(this); //####
} // AbsorberFilterService::AbsorberFilterService

AbsorberFilterService::~AbsorberFilterService(void)
{
    ODL_OBJENTER(); //####
    stopStreams();
    delete _inHandler;
    ODL_OBJEXIT(); //####
} // AbsorberFilterService::~AbsorberFilterService

#if defined(__APPLE__)
# pragma mark Actions and Accessors
#endif // defined(__APPLE__)

bool
AbsorberFilterService::configure(const yarp::os::Bottle & details)
{
    ODL_OBJENTER(); //####
    ODL_P1("details = ", &details); //####
    bool result = false;

    try
    {
        if (1 <= details.size())
        {
            yarp::os::Value firstValue(details.get(0));

            if (firstValue.isInt())
            {
                int firstNumber = firstValue.asInt();

                if (0 <= firstNumber)
                {
                    std::stringstream buff;

                    _sampleInterval = firstNumber;
                    ODL_I1("_sampleInterval <- ", _sampleInterval); //####
                    buff << "Sample interval is " << _sampleInterval;
                    setExtraInformation(buff.str());
                    result = true;
                }
                else
                {
                    cerr << "One or more inputs are out of range." << endl;
                }
            }
            else
            {
                cerr << "One or more inputs have the wrong type." << endl;
            }
        }
        else
        {
            cerr << "Missing input(s)." << endl;
        }
    }
    catch (...)
    {
        ODL_LOG("Exception caught"); //####
        throw;
    }
    ODL_OBJEXIT_B(result); //####
    return result;
} // AbsorberFilterService::configure

void
AbsorberFilterService::disableMetrics(void)
{
    ODL_OBJENTER(); //####
    inherited::disableMetrics();
    if (_inHandler)
    {
        _inHandler->disableMetrics();
    }
    ODL_OBJEXIT(); //####
} // AbsorberFilterService::disableMetrics

void
AbsorberFilterService::enableMetrics(void)
{
    ODL_OBJENTER(); //####
    inherited::enableMetrics();
    if (_inHandler)
    {
        _inHandler->enableMetrics();
    }
    ODL_OBJEXIT(); //####
} // AbsorberFilterService::enableMetrics

bool
AbsorberFilterService::getConfiguration(yarp::os::Bottle & details)
{
    ODL_OBJENTER(); //####
    ODL_P1("details = ", &details); //####
    bool result = true;

    details.clear();
    details.addInt(_sampleInterval);
    ODL_OBJEXIT_B(result); //####
    return result;
} // AbsorberFilterService::getConfiguration

bool
AbsorberFilterService::setUpStreamDescriptions(void)
{
    ODL_OBJENTER(); //####
    bool               result = true;
    ChannelDescription description;
    YarpString         rootName(getEndpoint().getName() + "/");

    _inDescriptions.clear();
    description._portName = rootName + "input";
    description._portProtocol = ""; // Empty, so everything accepted
    description._protocolDescription = "";
    _inDescriptions.push_back(description);
    _outDescriptions.clear();
    description._portName = rootName + "stats";
    description._portProtocol = "dd";
    description._protocolDescription = "Two floating-point values";
    _outDescriptions.push_back(description);
    ODL_OBJEXIT_B(result); //####
    return result;
} // AbsorberFilterService::setUpStreamDescriptions

void
AbsorberFilterService::startStreams(void)
{
    ODL_OBJENTER(); //####
    try
    {
        if (! isActive())
        {
            if (_inHandler)
            {
                _inHandler->setChannel(getInletStream(0));
                getInletStream(0)->setReader(*_inHandler);
                if (0 < _sampleInterval)
                {
                    _generator = new AbsorberFilterThread(*this, _sampleInterval);
                    if (_generator->start())
                    {
                        setActive();
                    }
                    else
                    {
                        ODL_LOG("! (generator->start())"); //####
                        cerr << "Could not start auxiliary thread." << endl;
                        delete _generator;
                        _generator = NULL;
                    }
                }
                else
                {
                    setActive();
                }
            }
        }
    }
    catch (...)
    {
        ODL_LOG("Exception caught"); //####
        throw;
    }
    ODL_OBJEXIT(); //####
} // AbsorberFilterService::startStreams

void
AbsorberFilterService::stopStreams(void)
{
    ODL_OBJENTER(); //####
    try
    {
        if (isActive())
        {
            if (_generator)
            {
                _generator->stop();
                for ( ; _generator->isRunning(); )
                {
                    ConsumeSomeTime(IO_SERVICE_DELAY_FACTOR_);
                }
                delete _generator;
                _generator = NULL;
            }
            clearActive();
        }
    }
    catch (...)
    {
        ODL_LOG("Exception caught"); //####
        throw;
    }
    ODL_OBJEXIT(); //####
} // AbsorberFilterService::stopStreams

void
AbsorberFilterService::reportMessageRate(void)
{
    ODL_OBJENTER(); //####
    if (0 <= _lastCount)
    {
        double           byteDelta = static_cast<double>(_totalBytes - _lastBytes);
        double           countDelta = _count - _lastCount;
        double           messageRate = (countDelta / _sampleInterval);
        double           byteRate = (byteDelta / _sampleInterval);
        GeneralChannel * outStream = getOutletStream(0);

        cout << "rate = " << messageRate << " messages/s, " << byteRate << " bytes/s" << endl;
        if (outStream)
        {
            yarp::os::Bottle outBottle;

            outBottle.addDouble(messageRate);
            outBottle.addDouble(byteRate);
            if (! outStream->write(outBottle))
            {
                ODL_LOG("(! outStream->write(message))"); //####
#if defined(MpM_StallOnSendProblem)
                Stall();
#endif // defined(MpM_StallOnSendProblem)
            }
        }
    }
    _lastCount = _count;
    _lastBytes = _totalBytes;
    ODL_OBJEXIT(); //####
} // AbsorberFilterService::reportMessageRate

void
AbsorberFilterService::updateCount(const size_t numBytes)
{
    ODL_OBJENTER(); //####
    ODL_I1("numBytes = ", numBytes); //####
    try
    {
        if (isActive())
        {
            ++_count;
            _totalBytes += numBytes;
            if (0 >= _sampleInterval)
            {
                // Note that the function 'convertToCommaSplitNumber' was used because the locale
                // mechanism in C++ for OS X is very, very broken.
                cout << "messages = " << convertToCommaSplitNumber(_count).c_str() <<
                        "; bytes = " << convertToCommaSplitNumber(numBytes).c_str() <<
                        "; total = " << convertToCommaSplitNumber(_totalBytes).c_str() << endl;
            }
        }
    }
    catch (...)
    {
        ODL_LOG("Exception caught"); //####
        throw;
    }
    ODL_OBJEXIT(); //####
} // AbsorberFilterService::updateCount

#if defined(__APPLE__)
# pragma mark Global functions
#endif // defined(__APPLE__)
