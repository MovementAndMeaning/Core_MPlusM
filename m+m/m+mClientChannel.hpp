//--------------------------------------------------------------------------------------------------
//
//  File:       m+m/m+mClientChannel.hpp
//
//  Project:    m+m
//
//  Contains:   The class declaration for channels for responses from a service to a client.
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
//  Created:    2014-04-07
//
//--------------------------------------------------------------------------------------------------

#if (! defined(MpMClientChannel_HPP_))
# define MpMClientChannel_HPP_ /* Header guard */

# include <m+m/m+mBaseChannel.hpp>

# if defined(__APPLE__)
#  pragma clang diagnostic push
#  pragma clang diagnostic ignored "-Wunknown-pragmas"
#  pragma clang diagnostic ignored "-Wdocumentation-unknown-command"
# endif // defined(__APPLE__)
/*! @file
 @brief The class declaration for channels for responses from a service to a client. */
# if defined(__APPLE__)
#  pragma clang diagnostic pop
# endif // defined(__APPLE__)

namespace MplusM
{
    namespace Common
    {
        /*! @brief A convenience class to provide distinct channels for responses from a service to
         a client. */
        class ClientChannel : public BaseChannel
        {
        public :

        protected :

        private :

            /*! @brief The class that this class is derived from. */
            typedef BaseChannel inherited;

        public :

            /*! @brief The constructor. */
            ClientChannel(void);

            /*! @brief The destructor. */
            virtual
            ~ClientChannel(void);

            /*! @brief Add an output to the channel, using a backoff strategy with retries.
             @param[in] theChannelToBeAdded The output to be added to the channel.
             @param[in] timeToWait The number of seconds allowed before a failure is considered.
             @return @c true if the channel was opened and @c false if it could not be opened. */
            bool
            addOutputWithRetries(const YarpString & theChannelToBeAdded,
                                 const double       timeToWait);

        protected :

        private :

            /*! @brief The copy constructor.
             @param[in] other The object to be copied. */
            ClientChannel(const ClientChannel & other);

            /*! @brief The assignment operator.
             @param[in] other The object to be copied.
             @return The updated object. */
            ClientChannel &
            operator =(const ClientChannel & other);

        public :

        protected :

        private :

        }; // ClientChannel

    } // Common

} // MplusM

#endif // ! defined(MpMClientChannel_HPP_)
