//--------------------------------------------------------------------------------------------------
//
//  File:       m+mMovementDbService.hpp
//
//  Project:    m+m
//
//  Contains:   The class declaration for a service that collects statistic on requests.
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

#if (! defined(MpMMovementDbService_HPP_))
# define MpMMovementDbService_HPP_ /* Header guard */

# include <m+m/m+mBaseService.hpp>

# if defined(__APPLE__)
#  pragma clang diagnostic push
#  pragma clang diagnostic ignored "-Wunknown-pragmas"
#  pragma clang diagnostic ignored "-Wdocumentation-unknown-command"
# endif // defined(__APPLE__)
/*! @file
 @brief The class declaration for a service that collects statistic on requests. */
# if defined(__APPLE__)
#  pragma clang diagnostic pop
# endif // defined(__APPLE__)

/*! @brief The channel name to use for the service if not provided. */
# define DEFAULT_MOVEMENTDB_SERVICE_NAME_ BUILD_NAME_(MpM_SERVICE_BASE_NAME_, "movementdb")

/*! @brief The description of the service. */
# define MOVEMENTDB_SERVICE_DESCRIPTION_ T_("Movement database service")

namespace MplusM
{
    namespace MovementDb
    {
        class AddFileRequestHandler;
        class SetDataTrackRequestHandler;
        class SetEmailRequestHandler;
        class StopDbRequestHandler;

        /*! @brief The movement database service. */
        class MovementDbService : public Common::BaseService
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
             @param[in] databaseServerAddress The IP address of the database server.
             @param[in] serviceEndpointName The YARP name to be assigned to the new service.
             @param[in] servicePortNumber The port being used by the service. */
            MovementDbService(const YarpString & launchPath,
                              const int          argc,
                              char * *           argv,
                              const YarpString & tag,
                              const YarpString & databaseServerAddress,
                              const YarpString & serviceEndpointName,
                              const YarpString & servicePortNumber = "");

            /*! @brief The destructor. */
            virtual
            ~MovementDbService(void);

            /*! @brief Add a file entry to the backend database.
             @param[in] key The client-provided key.
             @param[in] filePath The filesystem path to the file.
             @return @c true if the file entry was added successfully and @c false otherwise. */
            bool
            addFileToDb(const YarpString & key,
                        const YarpString & filePath);

            /*! @brief Set the active data track.
             @param[in] key The client-provided key.
             @param[in] dataTrack The data track to use with subsequent files.
             @return @c true if the data track was successfully set and @c false otherwise. */
            bool
            setDataTrack(const YarpString & key,
                         const YarpString & dataTrack);

            /*! @brief Set the active e-mail address.
             @param[in] key The client-provided key.
             @param[in] emailAddress The e-mail address of the user that will own subsequent files.
             @return @c true if the e-mail address was successfully set and @c false otherwise. */
            bool
            setEmailAddress(const YarpString & key,
                            const YarpString & emailAddress);

        protected :

        private :

            /*! @brief The copy constructor.
             @param[in] other The object to be copied. */
            MovementDbService(const MovementDbService & other);

            /*! @brief Enable the standard request handlers. */
            void
            attachRequestHandlers(void);

            /*! @brief Disable the standard request handlers. */
            void
            detachRequestHandlers(void);

            /*! @brief The assignment operator.
             @param[in] other The object to be copied.
             @return The updated object. */
            MovementDbService &
            operator =(const MovementDbService & other);

        public :

        protected :

        private :

            /*! @brief The IP address of the backend database server. */
            YarpString _databaseAddress;

            /*! @brief The request handler for the 'addfile' request. */
            AddFileRequestHandler * _addFileHandler;

            /*! @brief The request handler for the 'setdatatrack' request. */
            SetDataTrackRequestHandler * _setDataTrackHandler;

            /*! @brief The request handler for the 'setemail' request. */
            SetEmailRequestHandler * _setEmailHandler;

            /*! @brief The request handler for the 'stop' request. */
            StopDbRequestHandler * _stopDbHandler;

        }; // MovementDbService

    } // MovementDb

} // MplusM

#endif // ! defined(MpMMovementDbService_HPP_)
