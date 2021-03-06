//--------------------------------------------------------------------------------------------------
//
//  File:       m+mCommonLispFilterService.hpp
//
//  Project:    m+m
//
//  Contains:   The class declaration for the CommonLisp filter service.
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
//  Created:    2015-08-05
//
//--------------------------------------------------------------------------------------------------

#if (! defined(MpMCommonLispFilterService_HPP_))
# define MpMCommonLispFilterService_HPP_ /* Header guard */

# include "m+mCommonLispFilterCommon.hpp"

# include <m+m/m+mBaseFilterService.hpp>

// The following is necessary to avoid a conflict in typedefs between the Windows header files and
// the ECL header files!
# if (! MAC_OR_LINUX_)
#  define int8_t int8_t_ecl
# endif // ! MAC_OR_LINUX_
# include <ecl/ecl.h>
# if (! MAC_OR_LINUX_)
#  undef int8_t
# endif // ! MAC_OR_LINUX_

# if defined(__APPLE__)
#  pragma clang diagnostic push
#  pragma clang diagnostic ignored "-Wunknown-pragmas"
#  pragma clang diagnostic ignored "-Wdocumentation-unknown-command"
# endif // defined(__APPLE__)
/*! @file
 @brief The class declaration for the %CommonLisp filter service. */

/*! @namespace MplusM::CommonLisp
 @brief A set of classes to support executing %CommonLisp in an m+m installation. */
# if defined(__APPLE__)
#  pragma clang diagnostic pop
# endif // defined(__APPLE__)

/*! @brief The base channel name to use for the service if not provided. */
# define DEFAULT_COMMONLISP_SERVICE_NAME_ BUILD_NAME_(MpM_SERVICE_BASE_NAME_, \
                                                      BUILD_NAME_("filter", "commonlisp"))

/*! @brief The description of the service. */
# define COMMONLISPFILTER_SERVICE_DESCRIPTION_ T_("Common Lisp filter service")

namespace MplusM
{
    namespace CommonLisp
    {
        class CommonLispFilterInputHandler;
        class CommonLispFilterThread;

        /*! @brief A sequence of object pointers. */
        typedef std::vector<cl_object> ObjectVector;

        /*! @brief The %CommonLisp filter service. */
        class CommonLispFilterService : public Common::BaseFilterService
        {
        public :

        protected :

        private :

            /*! @brief The class that this class is derived from. */
            typedef BaseFilterService inherited;

            /*! @brief A sequence of input handlers. */
            typedef std::vector<CommonLispFilterInputHandler *> HandlerVector;

        public :

            /*! @brief The constructor.
             @param[in] argumentList Descriptions of the arguments to the executable.
             @param[in] launchPath The command-line name used to launch the service.
             @param[in] argc The number of arguments in 'argv'.
             @param[in] argv The arguments passed to the executable used to launch the service.
             @param[in] tag The modifier for the service name and port names.
             @param[in] description The description from the active script.
             @param[in] loadedInletDescriptions The list of loaded inlet stream descriptions.
             @param[in] loadedOutletDescriptions The list of loaded outlet stream descriptions.
             @param[in] loadedInletHandlers The list of loaded inlet handlers.
             @param[in] loadedStartingFunction The function to execute on starting the service
             streams.
             @param[in] loadedStoppingFunction The function to execute on stopping the service
             streams.
             @param[in] sawThread @c true if a thread function was defined.
             @param[in] loadedThreadFunction The function to execute on an output-generating thread.
             @param[in] loadedInterval The interval (in seconds) between executions of the
             output-generating thread.
             @param[in] serviceEndpointName The YARP name to be assigned to the new service.
             @param[in] servicePortNumber The port being used by the service. */
            CommonLispFilterService(const Utilities::DescriptorVector & argumentList,
                                    const YarpString &                  launchPath,
                                    const int                           argc,
                                    char * *                            argv,
                                    const YarpString &                  tag,
                                    const YarpString &                  description,
                                    const Common::ChannelVector &       loadedInletDescriptions,
                                    const Common::ChannelVector &       loadedOutletDescriptions,
                                    const ObjectVector &                loadedInletHandlers,
                                    cl_object                           loadedStartingFunction,
                                    cl_object                           loadedStoppingFunction,
                                    const bool                          sawThread,
                                    cl_object                           loadedThreadFunction,
                                    const double                        loadedInterval,
                                    const YarpString &                  serviceEndpointName,
                                    const YarpString &                  servicePortNumber = "");

            /*! @brief The destructor. */
            virtual
            ~CommonLispFilterService(void);

            /*! @brief Configure the input/output streams.
             @param[in] details The configuration information for the input/output streams.
             @return @c true if the service was successfully configured and @c false otherwise. */
            virtual bool
            configure(const yarp::os::Bottle & details);

            /*! @brief Turn off the send / receive metrics collecting. */
            virtual void
            disableMetrics(void);

            /*! @brief Declare the doIdle method, which is executed repeatedly once the service has
             been set up. */
            virtual void
            doIdle(void);

            /*! @brief Turn on the send / receive metrics collecting. */
            virtual void
            enableMetrics(void);

            /*! @brief Send a value out a specified channel.
             @param[in] channelSlot The output channel to be used.
             @param[in] theData The value to be sent.
             @return @c true if the data was successfully sent and @c false otherwise. */
            bool
            sendToChannel(const cl_fixnum channelSlot,
                          cl_object       theData);

            /*! @brief Signal to the background process that the thread or handler function should
             be performed. */
            void signalRunFunction(void);

            /*! @brief Stall a thread until the main thread can process the request and then process
             the request.
             @param[in] slotNumber The slot number of the input handler making the request. */
            void
            stallUntilIdle(const size_t slotNumber);

            /*! @brief Start the input / output streams. */
            virtual void
            startStreams(void);

            /*! @brief Stop the input / output streams. */
            virtual void
            stopStreams(void);

        protected :

        private :

            /*! @brief The copy constructor.
             @param[in] other The object to be copied. */
            CommonLispFilterService(const CommonLispFilterService & other);

            /*! @brief The assignment operator.
             @param[in] other The object to be copied.
             @return The updated object. */
            CommonLispFilterService &
            operator =(const CommonLispFilterService & other);

            /*! @brief Release all the allocated handlers. */
            void
            releaseHandlers(void);

            /*! @brief Set up the descriptions that will be used to construct the input / output
             streams.
             @return @c true if the descriptions were set up and @c false otherwise. */
            virtual bool
            setUpStreamDescriptions(void);

        public :

        protected :

        private :

            /*! @brief The handler functions to use for input. */
            const ObjectVector & _inletHandlers;

            /*! @brief The set of input handlers. */
            HandlerVector _inHandlers;

            /*! @brief The output thread to use. */
            CommonLispFilterThread * _generator;

            /*! @brief The list of loaded inlet stream descriptions. */
            const Common::ChannelVector & _loadedInletDescriptions;

            /*! @brief The list of loaded outlet stream descriptions. */
            const Common::ChannelVector & _loadedOutletDescriptions;

            /*! @brief The communication signal for the thread or handlers. */
            yarp::os::Semaphore _goAhead;

            /*! @brief The communication signal for the handlers. */
            yarp::os::Semaphore _staller;

            /*! @brief The Common Lisp script starting function. */
            cl_object _scriptStartingFunc;

            /*! @brief The Common Lisp script stopping function. */
            cl_object _scriptStoppingFunc;

            /*! @brief The Common Lisp script thread function. */
            cl_object _scriptThreadFunc;

            /*! @brief The hash2assoc function. */
            cl_object _hash2assocFunc;

            /*! @brief The setHash function. */
            cl_object _setHashFunc;

            /*! @brief The thread interval. */
            double _threadInterval;

            /*! @brief The slot of the most recent input handler. */
            size_t _mostRecentSlot;

            /*! @brief @c true if a thread is being used. */
            bool _isThreaded;

# if defined(__APPLE__)
#  pragma clang diagnostic push
#  pragma clang diagnostic ignored "-Wunused-private-field"
# endif // defined(__APPLE__)
            /*! @brief Filler to pad to alignment boundary */
            char _filler1[7];
# if defined(__APPLE__)
#  pragma clang diagnostic pop
# endif // defined(__APPLE__)

        }; // CommonLispFilterService

        /*! @brief Create a new base-string.

         Note that there is a bug in the ecl_make_simple_base_string where, instead of freshly
         allocating storage, the input character pointer is shared, resulting in garbage data being
         retained.
         @param[in] inString The string contents to be used.
         @param[in] inLength The string length to be used.
         @return A new base-string with the given contents and length. */
        cl_object
        CreateBaseString(const char * inString,
                         const size_t inLength);

    } // CommonLisp

} // MplusM

#endif // ! defined(MpMCommonLispFilterService_HPP_)
