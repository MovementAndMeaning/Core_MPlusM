//--------------------------------------------------------------------------------------------------
//
//  File:       m+m/m+mMatchExpression.cpp
//
//  Project:    m+m
//
//  Contains:   The class definition for a pattern matcher for expressions.
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

#include "m+mMatchExpression.hpp"

#include <m+m/m+mMatchConstraint.hpp>

//#include <odlEnable.h>
#include <odlInclude.h>

#if defined(__APPLE__)
# pragma clang diagnostic push
# pragma clang diagnostic ignored "-Wunknown-pragmas"
# pragma clang diagnostic ignored "-Wdocumentation-unknown-command"
#endif // defined(__APPLE__)
/*! @file
 @brief The class definition for a pattern matcher for expressions. */
#if defined(__APPLE__)
# pragma clang diagnostic pop
#endif // defined(__APPLE__)

#if defined(__APPLE__)
# pragma mark Namespace references
#endif // defined(__APPLE__)

using namespace MplusM;
using namespace MplusM::Common;
using namespace MplusM::Parser;

#if defined(__APPLE__)
# pragma mark Private structures, constants and variables
#endif // defined(__APPLE__)

/*! @brief The character used to separate expression list elements. */
static const char kComma = ',';

#if defined(__APPLE__)
# pragma mark Global constants and variables
#endif // defined(__APPLE__)

#if defined(__APPLE__)
# pragma mark Local functions
#endif // defined(__APPLE__)

#if defined(__APPLE__)
# pragma mark Class methods
#endif // defined(__APPLE__)

MatchExpression *
MatchExpression::CreateMatcher(const YarpString &  inString,
                               const size_t        inLength,
                               const size_t        startPos,
                               size_t &            endPos,
                               BaseNameValidator * validator)
{
    ODL_ENTER(); //####
    ODL_S1s("inString = ", inString); //####
    ODL_I2("inLength = ", inLength, "startPos = ", startPos);
    MatchExpression * result = NULL;

    try
    {
        size_t workPos = SkipWhitespace(inString, inLength, startPos);

        if (workPos < inLength)
        {
            // We potentially have a constraint list.
            bool done = false;
            bool okSoFar = true;

            result = new MatchExpression;
            for ( ; okSoFar && (! done); )
            {
                size_t            nextElementPos;
                MatchConstraint * element = MatchConstraint::CreateMatcher(inString, inLength,
                                                                           workPos, nextElementPos,
                                                                           validator);

                if (element)
                {
                    // Skip over any trailing whitespace, to find if the constraint list is complete
                    // or more coming.
                    workPos = SkipWhitespace(inString, inLength, nextElementPos);
                    if (workPos < inLength)
                    {
                        char scanChar = inString[workPos];

                        if (kComma == scanChar)
                        {
                            // We've got more elements to go.
                            result->_constraints.push_back(element);
                            ++workPos;
                        }
                        else
                        {
                            ODL_LOG("! (kComma == scanChar)"); //####
                            // Something unexpected has appeared.
                            okSoFar = false;
                        }
                    }
                    else
                    {
                        // This is the last element.
                        result->_constraints.push_back(element);
                        done = true;
                    }
                }
                else
                {
                    ODL_LOG("! (element)"); //####
                    // We have a malformed constraint list.
                    okSoFar = false;
                }
            }
            if (okSoFar)
            {
                endPos = workPos;
            }
            else
            {
                delete result;
                result = NULL;
            }
        }
        else
        {
            ODL_LOG("! (workPos < inLength)"); //####
        }
    }
    catch (...)
    {
        ODL_LOG("Exception caught"); //####
        throw;
    }
    ODL_EXIT_P(result); //####
    return result;
} // MatchExpression::CreateMatcher

char
MatchExpression::ExpressionSeparatorCharacter(void)
{
    return kComma;
} // MatchExpression::ExpressionSeparatorCharacter

#if defined(__APPLE__)
# pragma mark Constructors and Destructors
#endif // defined(__APPLE__)

MatchExpression::MatchExpression(void) :
    inherited(), _constraints()
{
    ODL_ENTER(); //####
    ODL_EXIT_P(this); //####
} // MatchExpression::MatchExpression

MatchExpression::~MatchExpression(void)
{
    ODL_OBJENTER(); //####
    empty();
    ODL_OBJEXIT(); //####
} // MatchExpression::~MatchExpression

#if defined(__APPLE__)
# pragma mark Actions and Accessors
#endif // defined(__APPLE__)

YarpString
MatchExpression::asSQLString(const char * prefixString,
                             const char * suffixString)
const
{
    ODL_OBJENTER(); //####
    ODL_S2("prefixString = ", prefixString, "suffixString = ", suffixString); //####
    YarpString result;

    try
    {
        for (MatchExpressionListSize ii = 0, maxI = _constraints.size(); ii < maxI; ++ii)
        {
            MatchConstraint * element = _constraints[ii];

            if (ii)
            {
                if (suffixString)
                {
                    result += suffixString;
                }
                result += " UNION ";
            }
            if (prefixString)
            {
                result += prefixString;
            }
            result += element->asSQLString();
        }
        if ((0 < result.length()) && suffixString)
        {
            result += suffixString;
        }
    }
    catch (...)
    {
        ODL_LOG("Exception caught"); //####
        throw;
    }
    ODL_OBJEXIT_S(result.c_str()); //####
    return result;
} // MatchExpression::asSQLString

YarpString
MatchExpression::asString(void)
const
{
    YarpString result;

    try
    {
        for (MatchExpressionListSize ii = 0, maxI = _constraints.size(); ii < maxI; ++ii)
        {
            MatchConstraint * element = _constraints[ii];

            if (ii)
            {
                result += kComma;
                result += " ";
            }
            result += element->asString();
        }
    }
    catch (...)
    {
        ODL_LOG("Exception caught"); //####
        throw;
    }
    return result;
} // MatchExpression::asString

int
MatchExpression::count(void)
const
{
    return static_cast<int>(_constraints.size());
} // MatchExpression::count

const MatchConstraint *
MatchExpression::element(const int index)
const
{
    MatchConstraint * result = NULL;

    try
    {
        if ((index >= 0) && (index < static_cast<int>(_constraints.size())))
        {
            result = _constraints[static_cast<MatchExpressionListSize>(index)];
        }
        else
        {
            ODL_LOG("! ((index >= 0) && (index < static_cast<int>(_constraints.size())))"); //####
            result = NULL;
        }
    }
    catch (...)
    {
        ODL_LOG("Exception caught"); //####
        throw;
    }
    return result;
} // MatchExpression::element

void
MatchExpression::empty(void)
{
    ODL_OBJENTER(); //####
    try
    {
        for (MatchExpressionListSize ii = 0, maxI = _constraints.size(); ii < maxI; ++ii)
        {
            MatchConstraint * element = _constraints[ii];

            delete element;
        }
        _constraints.clear();
    }
    catch (...)
    {
        ODL_LOG("Exception caught"); //####
        throw;
    }
    ODL_OBJEXIT(); //####
} // MatchExpression::empty

#if defined(__APPLE__)
# pragma mark Global functions
#endif // defined(__APPLE__)
