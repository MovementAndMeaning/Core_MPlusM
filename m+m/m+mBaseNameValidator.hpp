//--------------------------------------------------------------------------------------------------
//
//  File:       m+m/m+mBaseNameValidator.hpp
//
//  Project:    m+m
//
//  Contains:   The class declaration for the minimal functionality required for an m+m field name
//              matcher.
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
//  Created:    2014-03-24
//
//--------------------------------------------------------------------------------------------------

#if (! defined(MpMBaseNameValidator_HPP_))
# define MpMBaseNameValidator_HPP_ /* Header guard */

# include <m+m/m+mCommon.hpp>

# if defined(__APPLE__)
#  pragma clang diagnostic push
#  pragma clang diagnostic ignored "-Wunknown-pragmas"
#  pragma clang diagnostic ignored "-Wdocumentation-unknown-command"
# endif // defined(__APPLE__)
/*! @file
 @brief The class declaration for the minimal functionality required for an m+m field name
 matcher. */
# if defined(__APPLE__)
#  pragma clang diagnostic pop
# endif // defined(__APPLE__)

namespace MplusM
{
    namespace Parser
    {
        /*! @brief A convenience class to provide function objects for field name handling. */
        class BaseNameValidator
        {
        public :

        protected :

        private :

        public :

            /*! @brief The destructor. */
            virtual
            ~BaseNameValidator(void);

            /*! @brief Check a field name for validity.
             @param[in] aString The string to be checked.
             @return @c true if the field name was valid or @c false if the field name was
             invalid. */
            virtual bool
            checkName(const char * aString) = 0;

            /*! @brief Get the 'true name' matching the name and its prefix and suffix strings.
             @param[in] aString The string to be checked.
             @param[in,out] prefixString The string to be used in the SQL prefix for this field.
             @param[in,out] suffixString The string to be used in the SQL suffix for this field.
             @return The actual field name to be used or @c NULL if the field name was
             unmatched. */
            virtual const char *
            getPrefixAndSuffix(const char *   aString,
                               const char * & prefixString,
                               const char * & suffixString) = 0;

        protected :

            /*! @brief The constructor. */
            BaseNameValidator(void);

        private :

            /*! @brief The copy constructor.
             @param[in] other The object to be copied. */
            BaseNameValidator(const BaseNameValidator & other);

            /*! @brief The assignment operator.
             @param[in] other The object to be copied.
             @return The updated object. */
            BaseNameValidator &
            operator =(const BaseNameValidator & other);

        public :

        protected :

        private :

        }; // BaseNameValidator

    } // Parser

} // MplusM

#endif // ! defined(MpMBaseNameValidator_HPP_)
