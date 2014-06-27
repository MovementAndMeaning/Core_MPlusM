//--------------------------------------------------------------------------------------
//
//  File:       M+MCommon.cpp
//
//  Project:    M+M
//
//  Contains:   The function and variable declarations for common entities for M+M
//              clients and services.
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
//  Created:    2014-03-19
//
//--------------------------------------------------------------------------------------

//#include "M+MCommon.h"
#include "M+MBailOut.h"
#include "M+MRequests.h"

//#include "ODEnableLogging.h"
#include "ODLogging.h"

#include <ace/Version.h>
#include <cmath>
#include <cstdio>
#include <ctime>
#if defined(__APPLE__)
# pragma clang diagnostic push
# pragma clang diagnostic ignored "-Wc++11-extensions"
# pragma clang diagnostic ignored "-Wdocumentation"
# pragma clang diagnostic ignored "-Wdocumentation-unknown-command"
# pragma clang diagnostic ignored "-Wpadded"
# pragma clang diagnostic ignored "-Wshadow"
# pragma clang diagnostic ignored "-Wunused-parameter"
# pragma clang diagnostic ignored "-Wweak-vtables"
#endif // defined(__APPLE__)
#include <yarp/conf/version.h>
#if defined(__APPLE__)
# pragma clang diagnostic pop
#endif // defined(__APPLE__)

#if defined(__APPLE__)
# pragma clang diagnostic push
# pragma clang diagnostic ignored "-Wdocumentation-unknown-command"
#endif // defined(__APPLE__)
/*! @file
 
 @brief The function and variable definitions for common entities for M+M clients and services. */
#if defined(__APPLE__)
# pragma clang diagnostic pop
#endif // defined(__APPLE__)

using namespace MplusM;
using namespace MplusM::Common;
using std::cerr;
using std::cout;
using std::endl;

#if defined(__APPLE__)
# pragma mark Private structures, constants and variables
#endif // defined(__APPLE__)

/*! @brief The maximum integer that we wish to use for generated random values. */
static const int kMaxRandom = 123456789;

#if defined(__APPLE__)
# pragma mark Local functions
#endif // defined(__APPLE__)

#if MAC_OR_LINUX_
/*! @brief The signal handler to catch requests to stop the service.
 @param signal The signal being handled. */
static void localCatcher(int signal)
{
    OD_LOG_ENTER();//####
    OD_LOG_LL1("signal = ", signal);//####
    cerr << "Exiting due to signal " << signal << " = " << MplusM::NameOfSignal(signal) << endl;
    OD_LOG_EXIT_EXIT(1);//####
    yarp::os::exit(1);
} // localCatcher
#endif // MAC_OR_LINUX_

/*! @brief Returns a printable string, even for null strings.
 @param aString The string to be checked.
 @returns The input string, if non-@c NULL, or a fixed string if it is @c NULL. */
static const char * nullOrString(const char * aString)
{
    const char * result;
    
    if (aString)
    {
        result = aString;
    }
    else
    {
        result = "<>";
    }
    return result;
} // nullOrString

#if defined(__APPLE__)
# pragma mark Global functions
#endif // defined(__APPLE__)

void MplusM::Common::DumpContactToLog(const char *              tag,
                                      const yarp::os::Contact & aContact)
{
    OD_LOG_S4("tag = ", tag, "contact.name = ", aContact.getName().c_str(),//####
              "contact.host = ", aContact.getHost().c_str(), "contact.carrier = ", aContact.getCarrier().c_str());//####
    OD_LOG_LL1("contact.port = ", aContact.getPort());//####
    OD_LOG_S1("contact.toString = ", aContact.toString().c_str());//####
    OD_LOG_B1("contact.isValid = ", aContact.isValid());//####
} // MplusM::Common::DumpContactToLog

yarp::os::ConstString MplusM::Common::GetRandomChannelName(const char * channelRoot)
{
    OD_LOG_ENTER();//####
    OD_LOG_S1("channelRoot = ", channelRoot);//####
    yarp::os::ConstString result;

    try
    {
        bool         hasLeadingSlash = false;
        const char * stringToUse;
        size_t       buffLen;
        
        if (channelRoot)
        {
            stringToUse = channelRoot;
            buffLen = strlen(channelRoot);
            if ('/' == *channelRoot)
            {
                hasLeadingSlash = true;
            }
            else
            {
                ++buffLen;
            }
        }
        else
        {
            stringToUse = "_";
            buffLen = 1;
        }
        buffLen += 32; // allow for a big number...
        char * buff = new char[buffLen];
        int    randNumb = yarp::os::Random::uniform(0, kMaxRandom);
        
#  if MAC_OR_LINUX_
        if (hasLeadingSlash)
        {
            snprintf(buff, buffLen, "%s%x", stringToUse, randNumb);
        }
        else
        {
            snprintf(buff, buffLen, "/%s%x", stringToUse, randNumb);
        }
#  else // ! MAC_OR_LINUX_
        if (hasLeadingSlash)
        {
            sprintf(buff, "%s%x", stringToUse, randNumb);
        }
        else
        {
            sprintf(buff, "/%s%x", stringToUse, randNumb);
        }
#  endif // ! MAC_OR_LINUX_
        result = buff;
        delete[] buff;
    }
    catch (...)
    {
        OD_LOG("Exception caught");//####
        throw;
    }
    OD_LOG_EXIT_S(result.c_str());//####
    return result;
} // MplusM::Common::GetRandomChannelName

void MplusM::Common::Initialize(const char * progName)
{
#if (! defined(MpM_ChattyStart))
# if MAC_OR_LINUX_
#  pragma unused(progName)
# endif // MAC_OR_LINUX_
#endif // ! defined(MpM_ChattyStart)
    OD_LOG_ENTER();//####
    try
    {
#if (defined(OD_ENABLE_LOGGING) && defined(MpM_LogIncludesYarpTrace))
        yarp::os::Network::setVerbosity(1);
#else // ! (defined(OD_ENABLE_LOGGING) && defined(MpM_LogIncludesYarpTrace))
        yarp::os::Network::setVerbosity(-1);
#endif // ! (defined(OD_ENABLE_LOGGING) && defined(MpM_LogIncludesYarpTrace))
        double intPart;
        double now = yarp::os::Time::now();
        double fraction = modf(now, &intPart);
        int    seed = static_cast<int>(ceil(fraction * kMaxRandom));
        
#if defined(MpM_ChattyStart)
        cerr << "Program " << progName << endl;
        cerr << "Movement And Meaning Version: " << MpM_VERSION << ", YARP Version: " << YARP_VERSION_STRING <<
                ", ACE Version: " << ACE_VERSION << endl;
#endif // defined(MpM_ChattyStart)
        OD_LOG_D2("time = ", now, "fraction = ", fraction);//####
        OD_LOG_LL1("seed = ", seed);//####
        yarp::os::Random::seed(seed);
    }
    catch (...)
    {
        OD_LOG("Exception caught");//####
        throw;
    }
    OD_LOG_EXIT();//####
} // MplusM::Common::Initialize

bool MplusM::Common::NetworkConnectWithRetries(const yarp::os::ConstString & sourceName,
                                               const yarp::os::ConstString & destinationName,
                                               const double                  timeToWait,
                                               const bool                    isUDP)
{
#if ((! RETRY_LOOPS_USE_TIMEOUTS) && (! defined(OD_ENABLE_LOGGING)))
# if MAC_OR_LINUX_
#  pragma unused(timeToWait)
# endif // MAC_OR_LINUX_
#endif // (! RETRY_LOOPS_USE_TIMEOUTS) && (! defined(OD_ENABLE_LOGGING))
    OD_LOG_ENTER();//####
    OD_LOG_S2("sourceName = ", sourceName.c_str(), "destinationName = ", destinationName.c_str());//####
    OD_LOG_D1("timeToWait = ", timeToWait);//####
    OD_LOG_B1("isUDP = ", isUDP);//####
    bool result = false;
    
    if (yarp::os::Network::exists(sourceName) && yarp::os::Network::exists(destinationName))
    {
        double retryTime = INITIAL_RETRY_INTERVAL;
        int    retriesLeft = MAX_RETRIES;
        
#if RETRY_LOOPS_USE_TIMEOUTS
        SetUpCatcher();
#endif // RETRY_LOOPS_USE_TIMEOUTS
        try
        {
#if RETRY_LOOPS_USE_TIMEOUTS
            BailOut      bailer(timeToWait);
#endif // RETRY_LOOPS_USE_TIMEOUTS
            const char * carrier;
            
            if (isUDP)
            {
                carrier = "udp";
            }
            else
            {
                carrier = "tcp";
            }
            do
            {
                OD_LOG("about to connect");//####
#if (defined(OD_ENABLE_LOGGING) && defined(MpM_LogIncludesYarpTrace))
                result = yarp::os::Network::connect(sourceName, destinationName, carrier, false);
#else // ! (defined(OD_ENABLE_LOGGING) && defined(MpM_LogIncludesYarpTrace))
                result = yarp::os::Network::connect(sourceName, destinationName, carrier, true);
#endif // ! (defined(OD_ENABLE_LOGGING) && defined(MpM_LogIncludesYarpTrace))
                OD_LOG("connected?");//####
                if (! result)
                {
                    if (0 < --retriesLeft)
                    {
                        OD_LOG("%%retry%%");//####
                        yarp::os::Time::delay(retryTime);
                        retryTime *= RETRY_MULTIPLIER;
                    }
                }
            }
            while ((! result) && (0 < retriesLeft));
        }
        catch (...)
        {
            OD_LOG("Exception caught");//####
            throw;
        }
#if RETRY_LOOPS_USE_TIMEOUTS
        ShutDownCatcher();
#endif // RETRY_LOOPS_USE_TIMEOUTS
    }
    else
    {
        OD_LOG("! (yarp::os::Network::exists(sourceName) && yarp::os::Network::exists(destinationName))");//####
    }
    OD_LOG_EXIT_B(result);//####
    return result;
} // MplusM::Common::NetworkConnectWithRetries

bool MplusM::Common::NetworkDisconnectWithRetries(const yarp::os::ConstString & sourceName,
                                                  const yarp::os::ConstString & destinationName,
                                                  const double                  timeToWait)
{
#if ((! RETRY_LOOPS_USE_TIMEOUTS) && (! defined(OD_ENABLE_LOGGING)))
# if MAC_OR_LINUX_
#  pragma unused(timeToWait)
# endif // MAC_OR_LINUX_
#endif // (! RETRY_LOOPS_USE_TIMEOUTS) && (! defined(OD_ENABLE_LOGGING))
    OD_LOG_ENTER();//####
    OD_LOG_S2("sourceName = ", sourceName.c_str(), "destinationName = ", destinationName.c_str());//####
    OD_LOG_D1("timeToWait = ", timeToWait);//####
    bool result = false;
    
    if (yarp::os::Network::exists(sourceName) && yarp::os::Network::exists(destinationName))
    {
        double retryTime = INITIAL_RETRY_INTERVAL;
        int    retriesLeft = MAX_RETRIES;
        
#if RETRY_LOOPS_USE_TIMEOUTS
        SetUpCatcher();
#endif // RETRY_LOOPS_USE_TIMEOUTS
        try
        {
#if RETRY_LOOPS_USE_TIMEOUTS
            BailOut bailer(timeToWait);
#endif // RETRY_LOOPS_USE_TIMEOUTS

            do
            {
                OD_LOG("about to disconnect");//####
#if (defined(OD_ENABLE_LOGGING) && defined(MpM_LogIncludesYarpTrace))
                result = yarp::os::Network::disconnect(sourceName, destinationName, false);
#else // ! (defined(OD_ENABLE_LOGGING) && defined(MpM_LogIncludesYarpTrace))
                result = yarp::os::Network::disconnect(sourceName, destinationName, true);
#endif // ! (defined(OD_ENABLE_LOGGING) && defined(MpM_LogIncludesYarpTrace))
                OD_LOG("disconnected?");//####
                if (! result)
                {
                    if (0 < --retriesLeft)
                    {
                        OD_LOG("%%retry%%");//####
                        yarp::os::Time::delay(retryTime);
                        retryTime *= RETRY_MULTIPLIER;
                    }
                }
            }
            while ((! result) && (0 < retriesLeft));
        }
        catch (...)
        {
            OD_LOG("Exception caught");//####
            throw;
        }
#if RETRY_LOOPS_USE_TIMEOUTS
        ShutDownCatcher();
#endif // RETRY_LOOPS_USE_TIMEOUTS
    }
    else
    {
        OD_LOG("! (yarp::os::Network::exists(sourceName) && yarp::os::exists(destinationName))");//####
    }
    OD_LOG_EXIT_B(result);//####
    return result;
} // MplusM::Common::NetworkDisconnectWithRetries

void MplusM::Common::SetSignalHandlers(yarp::os::YarpSignalHandler theHandler)
{
    OD_LOG_ENTER();//####
#if MAC_OR_LINUX_
    sigset_t         blocking;
    struct sigaction act;
#endif // MAC_OR_LINUX_
    
#if MAC_OR_LINUX_
    act.sa_handler = theHandler;
    sigemptyset(&act.sa_mask);
    act.sa_flags = 0;
# if (defined(SIGABRT) && (SIGABRT != STANDARD_SIGNAL_TO_USE))
    sigaction(SIGABRT, &act, NULL);
# endif // defined(SIGABRT) && (SIGABRT != STANDARD_SIGNAL_TO_USE)
# if (defined(SIGHUP) && (SIGHUP != STANDARD_SIGNAL_TO_USE))
    sigaction(SIGHUP, &act, NULL);
# endif // defined(SIGHUP) && (SIGABRT != STANDARD_SIGNAL_TO_USE)
# if (defined(SIGINT) && (SIGINT != STANDARD_SIGNAL_TO_USE))
    sigaction(SIGINT, &act, NULL);
# endif // defined(SIGINT) && (SIGABRT != STANDARD_SIGNAL_TO_USE)
# if (defined(SIGQUIT) && (SIGQUIT != STANDARD_SIGNAL_TO_USE))
    sigaction(SIGQUIT, &act, NULL);
# endif // defined(SIGQUIT) && (SIGABRT != STANDARD_SIGNAL_TO_USE)
# if (defined(SIGUSR1) && (SIGUSR1 != STANDARD_SIGNAL_TO_USE))
    sigaction(SIGUSR1, &act, NULL);
# endif // defined(SIGUSR1) && (SIGABRT != STANDARD_SIGNAL_TO_USE)
# if (defined(SIGUSR2) && (SIGUSR2 != STANDARD_SIGNAL_TO_USE))
    sigaction(SIGUSR2, &act, NULL);
# endif // defined(SIGUSR2) && (SIGABRT != STANDARD_SIGNAL_TO_USE)
    sigemptyset(&blocking);
    sigaddset(&blocking, STANDARD_SIGNAL_TO_USE);
    pthread_sigmask(SIG_BLOCK, &blocking, NULL);
#else // ! MAC_OR_LINUX_
# if (defined(SIGABRT) && (SIGABRT != STANDARD_SIGNAL_TO_USE))
    yarp::os::signal(SIGABRT, theHandler);
# endif // defined(SIGABRT) && (SIGABRT != STANDARD_SIGNAL_TO_USE)
# if (defined(SIGHUP) && (SIGHUP != STANDARD_SIGNAL_TO_USE))
    yarp::os::signal(SIGHUP, theHandler);
# endif // defined(SIGHUP) && (SIGABRT != STANDARD_SIGNAL_TO_USE)
# if (defined(SIGINT) && (SIGINT != STANDARD_SIGNAL_TO_USE))
    yarp::os::signal(SIGINT, theHandler);
# endif // defined(SIGINT) && (SIGABRT != STANDARD_SIGNAL_TO_USE)
# if (defined(SIGQUIT) && (SIGQUIT != STANDARD_SIGNAL_TO_USE))
    yarp::os::signal(SIGQUIT, theHandler);
# endif // defined(SIGQUIT) && (SIGABRT != STANDARD_SIGNAL_TO_USE)
# if (defined(SIGUSR1) && (SIGUSR1 != STANDARD_SIGNAL_TO_USE))
    yarp::os::signal(SIGUSR1, theHandler);
# endif // defined(SIGUSR1) && (SIGABRT != STANDARD_SIGNAL_TO_USE)
# if (defined(SIGUSR2) && (SIGUSR2 != STANDARD_SIGNAL_TO_USE))
    yarp::os::signal(SIGUSR2, theHandler);
# endif // defined(SIGUSR2) && (SIGABRT != STANDARD_SIGNAL_TO_USE)
    yarp::os::signal(SIGTERM, theHandler);
#endif // ! MAC_OR_LINUX_
    OD_LOG_EXIT();//####
} // MplusM::Common::SetSignalHandlers

void MplusM::Common::SetUpCatcher(void)
{
    OD_LOG_ENTER();//####
#if MAC_OR_LINUX_
    sigset_t         unblocking;
    struct sigaction act;
#endif // MAC_OR_LINUX_
    
#if MAC_OR_LINUX_
    sigemptyset(&unblocking);
    sigaddset(&unblocking, STANDARD_SIGNAL_TO_USE);
    pthread_sigmask(SIG_UNBLOCK, &unblocking, NULL);
    act.sa_handler = localCatcher;
    sigemptyset(&act.sa_mask);
    act.sa_flags = 0;
    sigaction(STANDARD_SIGNAL_TO_USE, &act, NULL);
#else // ! MAC_OR_LINUX_
#endif // ! MAC_OR_LINUX_
    OD_LOG_EXIT();//####
} // MplusM::Common::SetUpCatcher

void MplusM::Common::ShutDownCatcher(void)
{
    OD_LOG_ENTER();//####
#if MAC_OR_LINUX_
    sigset_t         blocking;
    struct sigaction act;
#endif // MAC_OR_LINUX_

#if MAC_OR_LINUX_
    sigemptyset(&blocking);
    sigaddset(&blocking, STANDARD_SIGNAL_TO_USE);
    pthread_sigmask(SIG_BLOCK, &blocking, NULL);
    act.sa_handler = SIG_DFL;
    sigemptyset(&act.sa_mask);
    act.sa_flags = 0;
    sigaction(STANDARD_SIGNAL_TO_USE, &act, NULL);
#else // ! MAC_OR_LINUX_
#endif // ! MAC_OR_LINUX_
    OD_LOG_EXIT();//####
} // MplusM::Common::ShutDownCatcher

void MplusM::Common::Stall(void)
{
    for ( ; ; )
    {
        yarp::os::Time::yield();
    }
} // MplusM::Common::Stall

bool MplusM::CanReadFromStandardInput(void)
{
    OD_LOG_ENTER();//####
#if MAC_OR_LINUX_
    pid_t fg = tcgetpgrp(STDIN_FILENO);
#endif // MAC_OR_LINUX_
    bool  result = false;
    
#if MAC_OR_LINUX_
    if (-1 == fg)
    {
        // Piped
        result = true;
    }
    else if (getpgrp() == fg)
    {
        // Foreground
        result = true;
    }
    else
    {
        // Background
        result = false;
    }
#else // ! MAC_OR_LINUX_
      // How do we check on Windows??
#endif // ! MAC_OR_LINUX_
    OD_LOG_EXIT_B(result);//####
    return result;
} // MplusM::CanReadFromStandardInput

const char * MplusM::NameOfSignal(const int theSignal)
{
    const char * result;

#if MAC_OR_LINUX_
    switch (theSignal)
    {
        case SIGHUP:
            result = "SIGHUP[hangup]";
            break;
            
        case SIGINT:
            result = "SIGINT[interrupt]";
            break;
            
        case SIGQUIT:
            result = "SIGQUIT[quit]";
            break;
            
        case SIGILL:
            result = "SIGILL[illegal instruction]";
            break;
            
        case SIGTRAP:
            result = "SIGTRAP[trace trap]";
            break;
            
        case SIGABRT:
            result = "SIGABRT[abort()]";
            break;
            
# if (defined(_POSIX_C_SOURCE) && (! defined(_DARWIN_C_SOURCE)))
        case SIGPOLL:
            result = "SIGPOLL[pollable evebt]";
            break;
# else // (! defined(_POSIX_C_SOURCE)) || defined(_DARWIN_C_SOURCE)
        case SIGEMT:
            result = "SIGEMT[EMT instruction]";
            break;
# endif // (! defined(_POSIX_C_SOURCE)) || defined(_DARWIN_C_SOURCE)
            
        case SIGFPE:
            result = "SIGFPE[floating point exception]";
            break;
            
        case SIGKILL:
            result = "SIGKILL[kill]";
            break;
            
        case SIGBUS:
            result = "SIGBUS[bus error]";
            break;
            
        case SIGSEGV:
            result = "SIGSEGV[segmentation violation]";
            break;
            
        case SIGSYS:
            result = "SIGSYS[bad argument to system call]";
            break;
            
        case SIGPIPE:
            result = "SIGPIPE[write on a pipe with no one to read it]";
            break;
            
        case SIGALRM:
            result = "SIGALRM[alarm clock]";
            break;
            
        case SIGTERM:
            result = "SIGTERM[software termination signal from kill]";
            break;
            
        case SIGURG:
            result = "SIGURG[urgent condition on IO channel]";
            break;
            
        case SIGSTOP:
            result = "SIGSTOP[sendable stop signal not from tty]";
            break;
            
        case SIGTSTP:
            result = "SIGTSTP[stop signal from tty]";
            break;
            
        case SIGCONT:
            result = "SIGCONT[continue a stopped process]";
            break;
            
        case SIGCHLD:
            result = "SIGCHLD[to parent on child stop or exit]";
            break;
            
        case SIGTTIN:
            result = "SIGTTIN[to readers pgrp upon background tty read]";
            break;
            
        case SIGTTOU:
            result = "SIGTTOU[like TTIN for output if (tp->t_local&LTOSTOP)]";
            break;
            
# if ((! defined(_POSIX_C_SOURCE)) || defined(_DARWIN_C_SOURCE))
        case SIGIO:
            result = "SIGIO[input/output possible signal]";
            break;
# endif // (! defined(_POSIX_C_SOURCE)) || defined(_DARWIN_C_SOURCE)
            
        case SIGXCPU:
            result = "SIGXCPU[exceeded CPU time limit]";
            break;
            
        case SIGXFSZ:
            result = "SIGXFSZ[exceeded file size limit]";
            break;
            
        case SIGVTALRM:
            result = "SIGVTALRM[virtual time alarm]";
            break;
            
        case SIGPROF:
            result = "SIGPROF[profiling time alarm]";
            break;
            
# if ((! defined(_POSIX_C_SOURCE)) || defined(_DARWIN_C_SOURCE))
        case SIGWINCH:
            result = "SIGWINCH[window size changes]";
            break;
# endif // (! defined(_POSIX_C_SOURCE)) || defined(_DARWIN_C_SOURCE)
            
# if ((! defined(_POSIX_C_SOURCE)) || defined(_DARWIN_C_SOURCE))
        case SIGINFO:
            result = "SIGINFO[information request]";
            break;
# endif // (! defined(_POSIX_C_SOURCE)) || defined(_DARWIN_C_SOURCE)
            
        case SIGUSR1:
            result = "SIGUSR1[user defined signal 1]";
            break;
            
        case SIGUSR2:
            result = "SIGUSR2[user defined signal 2]";
            break;
            
        default:
            result = "unknown";
            break;
            
    }
#else // ! MAC_OR_LINUX_
	//ASSUME WINDOWS
	switch (theSignal)
	{
		case SIGINT:
			result = "SIGINT[interrupt]";
			break;
            
		case SIGABRT:
			result = "SIGABRT[abort()]";
            break;
            
		default:
			result = "unknown";
            break;

	}
#endif // ! MAC_OR_LINUX_
    return result;
} // MplusM::NameOfSignal

void MplusM::OutputDescription(std::ostream &                outStream,
                               const char *                  heading,
                               const yarp::os::ConstString & description)
{
    size_t                descriptionLength = description.size();
    size_t                indentSize = strlen(heading);
    size_t                pieceStart = 0;
    yarp::os::ConstString blanks(indentSize, ' ');
    yarp::os::ConstString indent(heading);
    
    for (size_t ii = 0; ii < descriptionLength; ++ii)
    {
        if ('\n' == description[ii])
        {
            yarp::os::ConstString piece(description.substr(pieceStart, ii - pieceStart));
            
            outStream << indent << piece.c_str() << endl;
            pieceStart = ii + 1;
            indent = blanks;
        }
    }
    yarp::os::ConstString piece(description.substr(pieceStart, descriptionLength - pieceStart));
    
    outStream << indent << piece.c_str() << endl;
} // MplusM::OutputDescription

yarp::os::ConstString MplusM::SanitizeString(const yarp::os::ConstString & inString,
                                             const bool                    allowDoubleQuotes)
{
    OD_LOG_ENTER();//####
    OD_LOG_S1("channelRoot = ", inString.c_str());//####
    OD_LOG_B1("allowDoubleQuotes = ", allowDoubleQuotes);//####
    yarp::os::ConstString outString;
    
    try
    {
        for (int ii = 0, mm = inString.size(); mm > ii; )
        {
            char cc = inString[ii++];
            
            switch (cc)
            {
                case '\t':
                    outString += '\\';
                    cc = 't';
                    break;
                    
                case '\n':
                    outString += '\\';
                    cc = 'n';
                    break;
                    
                case '\\':
                    outString += '\\';
                    break;
                    
                case '"':
                    if (! allowDoubleQuotes)
                    {
                        outString += '\\';
                    }
                    break;
                    
                default:
                    break;
                    
            }
            outString += cc;
        }
    }
    catch (...)
    {
        OD_LOG("Exception caught");//####
        throw;
    }
    OD_LOG_EXIT_S(outString.c_str());//####
    return outString;
} // MplusM::SanitizeString
