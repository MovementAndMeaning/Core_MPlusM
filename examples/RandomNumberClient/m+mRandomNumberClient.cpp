//--------------------------------------------------------------------------------------------------
//
//  File:       m+mRandomNumberClient.cpp
//
//  Project:    m+m
//
//  Contains:   The class definition for the Random Number client.
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
//  Created:    2014-03-06
//
//--------------------------------------------------------------------------------------------------

#include "m+mRandomNumberClient.hpp"
#include "m+mRandomNumberRequests.hpp"

#include <m+m/m+mServiceResponse.hpp>

//#include <odlEnable.h>
#include <odlInclude.h>

#if defined(__APPLE__)
# pragma clang diagnostic push
# pragma clang diagnostic ignored "-Wunknown-pragmas"
# pragma clang diagnostic ignored "-Wdocumentation-unknown-command"
#endif // defined(__APPLE__)
/*! @file
 @brief The class definition for the Random Number client. */
#if defined(__APPLE__)
# pragma clang diagnostic pop
#endif // defined(__APPLE__)

#if defined(__APPLE__)
# pragma mark Namespace references
#endif // defined(__APPLE__)

using namespace MplusM;
using namespace MplusM::Common;
using namespace MplusM::Example;

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

RandomNumberClient::RandomNumberClient(void) :
    inherited("examples/randomnumber_")
{
    ODL_ENTER(); //####
    ODL_EXIT_P(this); //####
} // RandomNumberClient::RandomNumberClient

RandomNumberClient::~RandomNumberClient(void)
{
    ODL_OBJENTER(); //####
    ODL_OBJEXIT(); //####
} // RandomNumberClient::~RandomNumberClient

#if defined(__APPLE__)
# pragma mark Actions and Accessors
#endif // defined(__APPLE__)

bool
RandomNumberClient::getOneRandomNumber(double & result)
{
    ODL_OBJENTER(); //####
    ODL_P1("result = ", &result); //####
    bool okSoFar = false;

    try
    {
        yarp::os::Bottle parameters;
        ServiceResponse  response;

        reconnectIfDisconnected();
        if (send(MpM_RANDOM_REQUEST_, parameters, response))
        {
            if (1 == response.count())
            {
                yarp::os::Value retrieved(response.element(0));

                if (retrieved.isDouble())
                {
                    result = retrieved.asDouble();
                    okSoFar = true;
                }
                else
                {
                    ODL_LOG("! (retrieved.isDouble())"); //####
                }
            }
            else
            {
                ODL_LOG("! (1 == response.count())"); //####
                ODL_S1s("response = ", response.asString()); //####
            }
        }
        else
        {
            ODL_LOG("! (send(MpM_RANDOM_REQUEST_, parameters, response))"); //####
        }
    }
    catch (...)
    {
        ODL_LOG("Exception caught"); //####
        throw;
    }
    ODL_OBJEXIT_B(okSoFar); //####
    return okSoFar;
} // RandomNumberClient::getOneRandomNumber

bool
RandomNumberClient::getRandomNumbers(const int      howMany,
                                     DoubleVector & result)
{
    ODL_OBJENTER(); //####
    ODL_I1("howMany = ", howMany); //####
    ODL_P1("result = ", &result); //####
    bool okSoFar = false;

    try
    {
        if (0 < howMany)
        {
            yarp::os::Bottle parameters;
            ServiceResponse  response;

            parameters.addInt(howMany);
            reconnectIfDisconnected();
            if (send(MpM_RANDOM_REQUEST_, parameters, response))
            {
                if (howMany == response.count())
                {
                    result.clear();
                    okSoFar = true;
                    for (int ii = 0; ii < howMany; ++ii)
                    {
                        yarp::os::Value retrieved(response.element(ii));

                        if (retrieved.isDouble())
                        {
                            result.push_back(retrieved.asDouble());
                        }
                        else
                        {
                            ODL_LOG("! (retrieved.isDouble())"); //####
                            okSoFar = false;
                            break;
                        }

                    }
                }
                else
                {
                    ODL_LOG("! (howMany == response.count())"); //####
                    ODL_S1s("response = ", response.asString()); //####
                }
            }
            else
            {
                ODL_LOG("! (send(MpM_RANDOM_REQUEST_, parameters, response))"); //####
            }
        }
        else
        {
            ODL_LOG("! (0 < howMany)"); //####
        }
    }
    catch (...)
    {
        ODL_LOG("Exception caught"); //####
        throw;
    }
    ODL_OBJEXIT_B(okSoFar); //####
    return okSoFar;
} // RandomNumberClient::getRandomNumbers

#if defined(__APPLE__)
# pragma mark Global functions
#endif // defined(__APPLE__)
