//--------------------------------------------------------------------------------------------------
//
//  File:       m+mRunningSumService.hpp
//
//  Project:    m+m
//
//  Contains:   The class declaration for the Running Sum service.
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
//  Created:    2014-03-18
//
//--------------------------------------------------------------------------------------------------

#if (! defined(MpMRunningSumService_HPP_))
# define MpMRunningSumService_HPP_ /* Header guard */

# include <m+m/m+mBaseService.hpp>

# if defined(__APPLE__)
#  pragma clang diagnostic push
#  pragma clang diagnostic ignored "-Wunknown-pragmas"
#  pragma clang diagnostic ignored "-Wdocumentation-unknown-command"
# endif // defined(__APPLE__)
/*! @file
 @brief The class declaration for the Running Sum service. */
# if defined(__APPLE__)
#  pragma clang diagnostic pop
# endif // defined(__APPLE__)

/*! @brief The channel name to use for the service if not provided. */
# define DEFAULT_RUNNINGSUM_SERVICE_NAME_ BUILD_NAME_(MpM_SERVICE_BASE_NAME_, \
                                                      BUILD_NAME_("examples", "runningsum"))

/*! @brief The description of the service. */
# define RUNNINGSUM_SERVICE_DESCRIPTION_ T_("Running Sum service")

namespace MplusM
{
    namespace Example
    {
        class AddToSumRequestHandler;
        class ResetSumRequestHandler;
        class StartSumRequestHandler;
        class StopSumRequestHandler;

        /*! @brief The Running Sum service. */
        class RunningSumService : public Common::BaseService
        {
        public :

        protected :

        private :

            /*! @brief The class that this class is derived from. */
            typedef BaseService inherited;

        public :

            /*! @brief The constructor.
             @param[in] launchPath The command-line name used to launch the service.
             @param[in] argc The number of arguments in 'argv'.
             @param[in] argv The arguments passed to the executable used to launch the service.
             @param[in] tag The modifier for the service name and port names.
             @param[in] serviceEndpointName The YARP name to be assigned to the new service.
             @param[in] servicePortNumber The port being used by the service. */
            RunningSumService(const YarpString & launchPath,
                              const int          argc,
                              char * *           argv,
                              const YarpString & tag,
                              const YarpString & serviceEndpointName,
                              const YarpString & servicePortNumber = "");

            /*! @brief The destructor. */
            virtual
            ~RunningSumService(void);

            /*! @brief Add to the running sum for the given client.
             @param[in] key The client-provided key.
             @param[in] value The value to be added to the running sum.
             @return The running sum, including the newly added value. */
            double
            addToSum(const YarpString & key,
                     const double       value);

            /*! @brief Reset the running sum for the given client.
             @param[in] key The client-provided key. */
            void
            resetSum(const YarpString & key);

            /*! @brief Start a running sum for the given client.
             @param[in] key The client-provided key. */
            void
            startSum(const YarpString & key);

        protected :

        private :

            /*! @brief The copy constructor.
             @param[in] other The object to be copied. */
            RunningSumService(const RunningSumService & other);

            /*! @brief Enable the standard request handlers. */
            void
            attachRequestHandlers(void);

            /*! @brief Disable the standard request handlers. */
            void
            detachRequestHandlers(void);

            /*! @brief The assignment operator.
             @param[in] other The object to be copied.
             @return The updated object. */
            RunningSumService &
            operator =(const RunningSumService & other);

        public :

        protected :

        private :

            /*! @brief The request handler for the 'addToSum' request. */
            AddToSumRequestHandler * _addToSumHandler;

            /*! @brief The request handler for the 'resetSum' request. */
            ResetSumRequestHandler * _resetSumHandler;

            /*! @brief The request handler for the 'startSum' request. */
            StartSumRequestHandler * _startSumHandler;

            /*! @brief The request handler for the 'stopSum' request. */
            StopSumRequestHandler * _stopSumHandler;

        }; // RunningSumService

    } // Example

} // MplusM

#endif // ! defined(MpMRunningSumService_HPP_)
