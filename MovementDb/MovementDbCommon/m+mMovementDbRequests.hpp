//--------------------------------------------------------------------------------------------------
//
//  File:       m+mMovementDbRequests.hpp
//
//  Project:    m+m
//
//  Contains:   The common macro definitions for requests and responses for the movement database
//              service.
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
//  Created:    2014-09-02
//
//--------------------------------------------------------------------------------------------------

#if (! defined(MpMMovementDbRequests_HPP_))
# define MpMMovementDbRequests_HPP_ /* Header guard */

# include <m+m/m+mRequests.hpp>

# if defined(__APPLE__)
#  pragma clang diagnostic push
#  pragma clang diagnostic ignored "-Wunknown-pragmas"
#  pragma clang diagnostic ignored "-Wdocumentation-unknown-command"
# endif // defined(__APPLE__)
/*! @file
 @brief The common macro definitions for requests and responses for the movement database
 service. */

/*! @namespace MplusM::MovementDb
 @brief The classes that support communicating with the movement database. */
# if defined(__APPLE__)
#  pragma clang diagnostic pop
# endif // defined(__APPLE__)

/*! @brief The channel-independent name of the movement database service. */
# define MpM_MOVEMENTDB_CANONICAL_NAME_ "MovementDb"

/*! @brief The name for the 'addfile' request. */
# define MpM_ADDFILE_REQUEST_       "addfile"

/*! @brief The name for the 'setdatatrack' request. */
# define MpM_SETDATATRACK_REQUEST_  "setdatatrack"

/*! @brief The name for the 'setemail' request. */
# define MpM_SETEMAIL_REQUEST_      "setemail"

/*! @brief The name for the 'stopdb' request. */
# define MpM_STOPDB_REQUEST_        "stopdb"

/*! @brief The number of elements expected in the output of an 'addfile' request. */
# define MpM_EXPECTED_ADDFILE_RESPONSE_SIZE_      1

/*! @brief The number of elements expected in the output of a 'setdatatrack' request. */
# define MpM_EXPECTED_SETDATATRACK_RESPONSE_SIZE_ 1

/*! @brief The number of elements expected in the output of a 'setemail' request. */
# define MpM_EXPECTED_SETEMAIL_RESPONSE_SIZE_     1

/*! @brief The number of elements expected in the output of a 'stopdb' request. */
# define MpM_EXPECTED_STOPDB_RESPONSE_SIZE_       1

#endif // ! defined(MpMMovementDbRequests_HPP_)
