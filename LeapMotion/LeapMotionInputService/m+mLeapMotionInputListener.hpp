//--------------------------------------------------------------------------------------------------
//
//  File:       m+mLeapMotionInputListener.hpp
//
//  Project:    m+m
//
//  Contains:   The class declaration for a Leap Motion listener.
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
//  Created:    2014-09-16
//
//--------------------------------------------------------------------------------------------------

#if (! defined(MpMLeapMotionInputListener_HPP_))
# define MpMLeapMotionInputListener_HPP_ /* Header guard */

# include "Leap.h"

# include <m+m/m+mGeneralChannel.hpp>

# if defined(__APPLE__)
#  pragma clang diagnostic push
#  pragma clang diagnostic ignored "-Wunknown-pragmas"
#  pragma clang diagnostic ignored "-Wdocumentation-unknown-command"
# endif // defined(__APPLE__)
/*! @file
 @brief The class declaration for a %Leap Motion listener. */
# if defined(__APPLE__)
#  pragma clang diagnostic pop
# endif // defined(__APPLE__)

namespace MplusM
{
    namespace LeapMotion
    {
        /*! @brief A listener for %Leap Motion devices. */
        class LeapMotionInputListener : public Leap::Listener
        {
        public :

        protected :

        private :

            /*! @brief The class that this class is derived from. */
            typedef Leap::Listener inherited;

        public :

            /*! @brief The constructor.
             @param[in] outChannel The channel to send motion data to. */
            explicit
            LeapMotionInputListener(Common::GeneralChannel * outChannel);

            /*! @brief The destructor. */
            virtual
            ~LeapMotionInputListener(void);

            /*! @brief Stop using the output channel. */
            void
            clearOutputChannel(void);

        protected :

        private :

            /*! @brief The copy constructor.
             @param[in] other The object to be copied. */
            LeapMotionInputListener(const LeapMotionInputListener & other);

            /*! @brief Called when the Controller object connects to the %Leap Motion software and
             the %Leap Motion hardware device is plugged in, or when this Listener object is added
             to a Controller that is already connected.
             @param[in] theController The Controller object invoking this callback function. */
            virtual void
            onConnect(const Leap::Controller & theController);

            /*! @brief Called when a %Leap Motion controller plugged in, unplugged, or the device
             changes state.

             State changes include changes in frame rate and entering or leaving "robust" mode.
             Note that there is currently no way to query whether a device is in robust mode.
             @param[in] theController The Controller object invoking this callback function. */
            virtual void
            onDeviceChange(const Leap::Controller & theController);

            /*! @brief Called when the Controller object disconnects from the %Leap Motion software
             or the %Leap Motion hardware is unplugged.

             The controller can disconnect when the %Leap Motion device is unplugged, the user shuts
             the %Leap Motion software down, or the %Leap Motion software encounters an
             unrecoverable error.
             @param[in] theController The Controller object invoking this callback function. */
            virtual void
            onDisconnect(const Leap::Controller & theController);

            /*! @brief Called when this Listener object is removed from the Controller or the
             Controller instance is destroyed.
             @param[in] theController The Controller object invoking this callback function. */
            virtual void
            onExit(const Leap::Controller & theController);

            /*! @brief Called when this application becomes the foreground application.
             @param[in] theController The Controller object invoking this callback function. */
            virtual void
            onFocusGained(const Leap::Controller & theController);

            /*! @brief Called when this application loses the foreground focus.
             @param[in] theController The Controller object invoking this callback function. */
            virtual void
            onFocusLost(const Leap::Controller & theController);

            /*! @brief Called when a new frame of hand and finger tracking data is available.
             @param[in] theController The Controller object invoking this callback function. */
            virtual void
            onFrame(const Leap::Controller & theController);

            /*! @brief Called once, when this Listener object is newly added to a Controller.
             @param[in] theController The Controller object invoking this callback function. */
            virtual void
            onInit(const Leap::Controller & theController);

            /*! @brief Called when the %Leap Motion daemon / service connects to your application
             Controller.

             In the normal course of events onServiceConnect will get called once after onInit.
             @param[in] theController The Controller object invoking this callback function. */
            virtual void
            onServiceConnect(const Leap::Controller & theController);

            /*! @brief Called if the %Leap Motion daemon / service disconnects from your application
             Controller.

             Normally, this callback is not invoked. It is only called if some external event or
             problem shuts down the service or otherwise interrupts the connection.
             @param[in] theController The Controller object invoking this callback function. */
            virtual void
            onServiceDisconnect(const Leap::Controller & theController);

            /*! @brief The assignment operator.
             @param[in] other The object to be copied.
             @return The updated object. */
            LeapMotionInputListener &
            operator =(const LeapMotionInputListener & other);

        public :

        protected :

        private :

            /*! @brief The channel to send motion data to. */
            Common::GeneralChannel * _outChannel;

        }; // LeapMotionInputListener

    } // LeapMotion

} // MplusM

#endif // ! defined(MpMLeapMotionInputListener_HPP_)
