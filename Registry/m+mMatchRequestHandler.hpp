//--------------------------------------------------------------------------------------------------
//
//  File:       m+mMatchRequestHandler.hpp
//
//  Project:    m+m
//
//  Contains:   The class declaration for the request handler for the 'match' request.
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
//  Created:    2014-03-10
//
//--------------------------------------------------------------------------------------------------

#if (! defined(MpMMatchRequestHandler_HPP_))
# define MpMMatchRequestHandler_HPP_ /* Header guard */

# include <m+m/m+mBaseNameValidator.hpp>
# include <m+m/m+mBaseRequestHandler.hpp>

# if defined(__APPLE__)
#  pragma clang diagnostic push
#  pragma clang diagnostic ignored "-Wunknown-pragmas"
#  pragma clang diagnostic ignored "-Wdocumentation-unknown-command"
# endif // defined(__APPLE__)
/*! @file
 @brief The class declaration for the request handler for the 'match' request. */
# if defined(__APPLE__)
#  pragma clang diagnostic pop
# endif // defined(__APPLE__)

namespace MplusM
{
    namespace Registry
    {
        class RegistryService;

        /*! @brief The 'match' request handler.

         The input is in the form of a pattern to be matched and the output is either 'OK', followed
         by a list of matching service channels, which indicates success, or 'FAILED' followed with
         a description of the reason for failure. Note that it is possible to have a successful
         request with no matching service channels.

         The matching pattern consists of a sequence of AND clauses, separated with commas.

         If a service matches any of the AND clauses, it will appear in the output of the pattern.
         Each AND clause is a sequence of field/value pairs, separated by ampersands.

         If a service matches all of the field/value pairs in the AND clause, it will be in the
         output of the AND clause. Field/value pairs consist of a field name followed by either a
         single value or a list of values, separated by either white space or a colon character. If
         a list of values is used, it must be surrounded by round brackets and, in this case, the
         white space or colon character between the field name and the list of values are optional.

         If the value in the pair is a single value, a service is in the output of the field/value
         pair if the corresponding field in the service matches the specified value. If value in the
         pair is a list, as service is in the output of the field/value pair if the corresponding
         field in the service matches at least one of the values in the list.

         A value consists of a sequence of non-blank, printable characters, or a sequence of
         printable characters surrounded by matching quote characters - either single quote or
         double quote. Note that a single quote character may appear within double quotes and vice
         versa. Three special characters are also used - an asterisk indicates a match for one or
         more characters, a question mark indicates a match for any character and a backslash is
         used to change the meaning of the next character in the value. The backslash provides an
         'escape' mechanism so that quote characters, asterisks, question marks and backslashes may
         appear within the value as literal characters rather than being interpreted as special
         characters.

         Note that matching is case-sensitive. */
        class MatchRequestHandler : public Common::BaseRequestHandler
        {
        public :

        protected :

        private :

            /*! @brief The class that this class is derived from. */
            typedef BaseRequestHandler inherited;

        public :

            /*! @brief The constructor.
             @param[in] service The service that has registered this request.
             @param[in] validator The field validator to use. */
            explicit
            MatchRequestHandler(RegistryService &           service,
                                Parser::BaseNameValidator * validator = NULL);

            /*! @brief The destructor. */
            virtual
            ~MatchRequestHandler(void);

        protected :

        private :

            /*! @brief The copy constructor.
             @param[in] other The object to be copied. */
            MatchRequestHandler(const MatchRequestHandler & other);

            /*! @brief Fill in a set of aliases for the request.
             @param[in,out] alternateNames Aliases for the request. */
            virtual void
            fillInAliases(YarpStringVector & alternateNames);

            /*! @brief Fill in a description dictionary for the request.
             @param[in] request The actual request name.
             @param[in,out] info The dictionary to be filled in. */
            virtual void
            fillInDescription(const YarpString &   request,
                              yarp::os::Property & info);

            /*! @brief The assignment operator.
             @param[in] other The object to be copied.
             @return The updated object. */
            MatchRequestHandler &
            operator =(const MatchRequestHandler & other);

            /*! @brief Process a request.
             @param[in] request The actual request name.
             @param[in] restOfInput The arguments to the operation.
             @param[in] senderChannel The name of the channel used to send the input data.
             @param[in] replyMechanism non-@c NULL if a reply is expected and @c NULL otherwise. */
            virtual bool
            processRequest(const YarpString &           request,
                           const yarp::os::Bottle &     restOfInput,
                           const YarpString &           senderChannel,
                           yarp::os::ConnectionWriter * replyMechanism);

        public :

        protected :

        private :

            /*! @brief The field name validator to be used. */
            Parser::BaseNameValidator * _validator;

        }; // MatchRequestHandler

    } // Registry

} // MplusM

#endif // ! defined(MpMMatchRequestHandler_HPP_)
