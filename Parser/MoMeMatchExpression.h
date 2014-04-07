//--------------------------------------------------------------------------------------
//
//  File:       MoMeMatchExpression.h
//
//  Project:    MoAndMe
//
//  Contains:   The class declaration for a pattern matcher for expressions.
//
//  Written by: Norman Jaffe
//
//  Copyright:  (c) 2014 by HPlus Technologies Ltd. and Simon Fraser University.
//
//              All rights reserved. Redistribution and use in source and binary forms,
//              with or without modification, are permitted provided that the following
//              conditions are met:
//                * Redistributions of source code must retain the above copyright
//                  notice, this list of conditions and the following disclaimer.
//                * Redistributions in binary form must reproduce the above copyright
//                  notice, this list of conditions and the following disclaimer in the
//                  documentation and/or other materials provided with the
//                  distribution.
//                * Neither the name of the copyright holders nor the names of its
//                  contributors may be used to endorse or promote products derived
//                  from this software without specific prior written permission.
//
//              THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
//              "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
//              LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
//              PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
//              OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
//              SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
//              LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
//              DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
//              THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
//              (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
//              OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//
//  Created:    2014-03-10
//
//--------------------------------------------------------------------------------------

#if (! defined(MOMEMATCHEXPRESSION_H_))
/*! @brief Header guard. */
# define MOMEMATCHEXPRESSION_H_ /* */

# include "MoMeBaseMatcher.h"

# if defined(__APPLE__)
#  pragma clang diagnostic push
#  pragma clang diagnostic ignored "-Wdocumentation-unknown-command"
# endif // defined(__APPLE__)
/*! @file
 
 @brief The class declaration for a pattern matcher for expressions. */
# if defined(__APPLE__)
#  pragma clang diagnostic pop
# endif // defined(__APPLE__)

namespace MoAndMe
{
    namespace Parser
    {
        class BaseNameValidator;
        class MatchConstraint;
        
        /*! @brief A pattern matcher for expressions. */
        class MatchExpression : public BaseMatcher
        {
        public:
            
            /*! @brief The destructor. */
            virtual ~MatchExpression(void);
            
            /*! @brief Generate a proper SQL string value corresponding to this match value.
             @param prefixString The SELECT prefix to be applied before each expression.
             @returns A string representing the value as a string suitable for use with SQL. */
            yarp::os::ConstString asSQLString(const char * prefixString)
            const;
            
            /*! @brief Return the match value as a printable string.
             @returns The matching substring as a printable string. */
            yarp::os::ConstString asString(void)
            const;
            
            /*! @brief Return the number of elements in the expression list.
             @returns The number of elements in the expression list. */
            int count(void)
            const;
            
            /*! @brief Return an element from the expression list.
             @param index The zero-origin index of the element.
             @returns An element of the expression list or @c NULL if the index is outside the range of the expression
             list. */
            const MatchConstraint * element(const int index)
            const;
            
            /*! @brief Create a pattern matcher if the next substring would be a valid expression.
             @param inString The string being scanned.
             @param inLength The length of the string being scanned.
             @param startPos Where in the string to start scanning.
             @param endPos Where the scan terminated, if successful.
             @param validator A function that returns @c true if the field name is valid and @c false otherwise.
             @returns A non-null matcher if the string would be a valid value and @c NULL otherwise. */
            static MatchExpression * CreateMatcher(const yarp::os::ConstString & inString,
                                                   const int                     inLength,
                                                   const int                     startPos,
                                                   int &                         endPos,
                                                   BaseNameValidator *           validator = NULL);
            
            /*! @brief The character used between expression list elements.
             @returns The character that separates expression list elements. */
            static char ExpressionSeparatorCharacter(void);
            
        protected:
            
        private:
            
            /*! @brief The class that this class is derived from. */
            typedef BaseMatcher inherited;
            
            /*! @brief A sequence of AND clauses. */
            typedef std::vector<MatchConstraint *>     MatchExpressionListData;
            
            /*! @brief The size-type for sequence data. */
            typedef MatchExpressionListData::size_type MatchExpressionListSize;
            
            /*! @brief The constructor. */
            MatchExpression(void);
            
            /*! @brief Copy constructor.
             
             Note - not implemented and private, to prevent unexpected copying.
             @param other Another object to construct from. */
            MatchExpression(const MatchExpression & other);
            
            /*! @brief Assignment operator.
             
             Note - not implemented and private, to prevent unexpected copying.
             @param other Another object to construct from. */
            MatchExpression & operator=(const MatchExpression & other);
            
            /*! @brief Remove all the list elements. */
            void empty(void);
            
            /*! @brief The elements of the list. */
            MatchExpressionListData _constraints;
            
        }; // MatchExpression
        
    } // Parser
    
} // MoAndMe

#endif // ! defined(MOMEMATCHEXPRESSION_H_)