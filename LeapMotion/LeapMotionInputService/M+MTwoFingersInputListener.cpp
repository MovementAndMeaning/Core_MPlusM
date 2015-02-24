//--------------------------------------------------------------------------------------------------
//
//  File:       M+MTwoFingersInputListener.cpp
//
//  Project:    M+M
//
//  Contains:   The class definition for a Two Fingers listener.
//
//  Written by: Norman Jaffe
//
//  Copyright:  (c) 2015 by HPlus Technologies Ltd. and Simon Fraser University.
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
//  Created:    2015-02-23
//
//--------------------------------------------------------------------------------------------------

#include "M+MTwoFingersInputListener.h"

//#include <odl/ODEnableLogging.h>
#include <odl/ODLogging.h>

#if defined(__APPLE__)
# pragma clang diagnostic push
# pragma clang diagnostic ignored "-Wunknown-pragmas"
# pragma clang diagnostic ignored "-Wdocumentation-unknown-command"
#endif // defined(__APPLE__)
/*! @file
 @brief The class definition for a Two Fingers listener. */
#if defined(__APPLE__)
# pragma clang diagnostic pop
#endif // defined(__APPLE__)

using namespace MplusM;
using namespace MplusM::Common;
using namespace MplusM::TwoFingers;

#if defined(__APPLE__)
# pragma mark Private structures, constants and variables
#endif // defined(__APPLE__)

enum HandMask
{
    kNoHands   = 0x00,
    kLeftHand  = 0x01,
    kRightHand = 0x02
}; // HandMask

#if defined(__APPLE__)
# pragma mark Local functions
#endif // defined(__APPLE__)

/*! @brief Put the three coordinates of a %Vector in a dictionary with the given tag.
 @param dictionary The dictionary to be added to.
 @param tag The name to be associated with the value.
 @param vectorToUse The %Vector containing the data to be added. */
static void putVectorInDictionary(yarp::os::Property &          dictionary,
                                  const yarp::os::ConstString & tag,
                                  const Leap::Vector &          vectorToUse)
{
    yarp::os::Value    stuff;
    yarp::os::Bottle * stuffList = stuff.asList();
    
    if (stuffList)
    {
        stuffList->addDouble(vectorToUse.x);
        stuffList->addDouble(vectorToUse.y);
        stuffList->addDouble(vectorToUse.z);
        dictionary.put(tag, stuff);
    }
} // putVectorInDictionary

#if defined(__APPLE__)
# pragma mark Class methods
#endif // defined(__APPLE__)

#if defined(__APPLE__)
# pragma mark Constructors and Destructors
#endif // defined(__APPLE__)

TwoFingersInputListener::TwoFingersInputListener(GeneralChannel * outChannel) :
    inherited(), _outChannel(outChannel)
{
    OD_LOG_ENTER(); //####
    OD_LOG_P1("outChannel = ", outChannel); //####
    OD_LOG_EXIT_P(this); //####
} // TwoFingersInputListener::TwoFingersInputListener

TwoFingersInputListener::~TwoFingersInputListener(void)
{
    OD_LOG_OBJENTER(); //####
    OD_LOG_OBJEXIT(); //####
} // TwoFingersInputListener::~TwoFingersInputListener

#if defined(__APPLE__)
# pragma mark Actions and Accessors
#endif // defined(__APPLE__)

void TwoFingersInputListener::clearOutputChannel(void)
{
    OD_LOG_OBJENTER(); //####
    _outChannel = NULL;
    OD_LOG_OBJEXIT(); //####
} // TwoFingersInputListener::clearOutputChannel

void TwoFingersInputListener::onConnect(const Leap::Controller & theController)
{
    OD_LOG_OBJENTER(); //####
    OD_LOG_P1("theController = ", &theController); //####
    theController.setPolicyFlags(Leap::Controller::POLICY_DEFAULT);
    OD_LOG_OBJEXIT(); //####
} // TwoFingersInputListener::onConnect

void TwoFingersInputListener::onDeviceChange(const Leap::Controller & theController)
{
    OD_LOG_OBJENTER(); //####
    OD_LOG_P1("theController = ", &theController); //####
    OD_LOG_OBJEXIT(); //####
} // TwoFingersInputListener::onDeviceChange

void TwoFingersInputListener::onDisconnect(const Leap::Controller & theController)
{
    OD_LOG_OBJENTER(); //####
    OD_LOG_P1("theController = ", &theController); //####
    OD_LOG_OBJEXIT(); //####
} // TwoFingersInputListener::onDisconnect

void TwoFingersInputListener::onExit(const Leap::Controller & theController)
{
    OD_LOG_OBJENTER(); //####
    OD_LOG_P1("theController = ", &theController); //####
    OD_LOG_OBJEXIT(); //####
} // TwoFingersInputListener::onExit

void TwoFingersInputListener::onFocusGained(const Leap::Controller & theController)
{
    OD_LOG_OBJENTER(); //####
    OD_LOG_P1("theController = ", &theController); //####
    OD_LOG_OBJEXIT(); //####
} // TwoFingersInputListener::onFocusGained

void TwoFingersInputListener::onFocusLost(const Leap::Controller & theController)
{
    OD_LOG_OBJENTER(); //####
    OD_LOG_P1("theController = ", &theController); //####
    OD_LOG_OBJEXIT(); //####
} // TwoFingersInputListener::onFocusLost

void TwoFingersInputListener::onFrame(const Leap::Controller & theController)
{
    OD_LOG_OBJENTER(); //####
    OD_LOG_P1("theController = ", &theController); //####
    Leap::Frame latestFrame(theController.frame());
    
    if (latestFrame.isValid())
    {
        Leap::HandList hands(latestFrame.hands());
        int            handCount = hands.count();

        if (0 < handCount)
        {
            HandMask handsPresent = kNoHands;
            double   fingerPositions[6];

            memset(fingerPositions, 0, sizeof(fingerPositions));
            for (Leap::HandList::const_iterator handWalker(hands.begin());
                 hands.end() != handWalker; ++handWalker)
            {
                Leap::Hand aHand(*handWalker);
                
                if (aHand.isValid())
                {
                    int offset;

                    if (aHand.isLeft())
                    {
                        offset = 0;
                        handsPresent = static_cast<HandMask>(handsPresent | kLeftHand);
                    }
                    else if (aHand.isRight())
                    {
                        offset = 3;
                        handsPresent = static_cast<HandMask>(handsPresent | kRightHand);
                    }
                    else
                    {
                        offset = -1;
                    }
                    if (0 <= offset)
                    {
                        // fingers
                        Leap::FingerList fingers(aHand.fingers());
                        
                        for (Leap::FingerList::const_iterator fingerWalker(fingers.begin());
                             fingers.end() != fingerWalker; ++fingerWalker)
                        {
                            Leap::Finger aFinger(*fingerWalker);

                            if (aFinger.isValid())
                            {
                                const Leap::Vector & position = aFinger.tipPosition();

                                fingerPositions[offset] = position.x;
                                fingerPositions[offset + 1] = position.y;
                                fingerPositions[offset + 2] = position.z;
                            }
                        }
                    }
                }
            }
            if (kNoHands != handsPresent)
            {
                yarp::os::Bottle message;

                message.addInt(handsPresent);
                for (int ii = 0; ii < (sizeof(fingerPositions) / sizeof(*fingerPositions)); ++ii)
                {
                    message.addDouble(fingerPositions[ii]);
                }
                if (_outChannel)
                {
                    if (! _outChannel->write(message))
                    {
                        OD_LOG("(! _outChannel->write(message))"); //####
#if defined(MpM_StallOnSendProblem)
                        Stall();
#endif // defined(MpM_StallOnSendProblem)
                    }
                }
            }
        }
    }
    OD_LOG_OBJEXIT(); //####
} // TwoFingersInputListener::onFrame

void TwoFingersInputListener::onInit(const Leap::Controller & theController)
{
    OD_LOG_OBJENTER(); //####
    OD_LOG_P1("theController = ", &theController); //####
	theController.setPolicyFlags(Leap::Controller::POLICY_BACKGROUND_FRAMES);
    OD_LOG_OBJEXIT(); //####
} // TwoFingersInputListener::onInit

void TwoFingersInputListener::onServiceConnect(const Leap::Controller & theController)
{
    OD_LOG_OBJENTER(); //####
    OD_LOG_P1("theController = ", &theController); //####
    OD_LOG_OBJEXIT(); //####
} // TwoFingersInputListener::onServiceConnect

void TwoFingersInputListener::onServiceDisconnect(const Leap::Controller & theController)
{
    OD_LOG_OBJENTER(); //####
    OD_LOG_P1("theController = ", &theController); //####
    OD_LOG_OBJEXIT(); //####
} // TwoFingersInputListener::onServiceDisconnect

#if defined(__APPLE__)
# pragma mark Global functions
#endif // defined(__APPLE__)
