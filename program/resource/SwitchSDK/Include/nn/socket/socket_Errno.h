/*--------------------------------------------------------------------------------*
  Copyright (C)Nintendo All rights reserved.

  These coded instructions, statements, and computer programs contain proprietary
  information of Nintendo and/or its licensed developers and are protected by
  national and international copyright laws. They may not be disclosed to third
  parties or copied or duplicated in any form, in whole or in part, without the
  prior written consent of Nintendo.

  The content herein is highly confidential and should be handled accordingly.
 *--------------------------------------------------------------------------------*/

#pragma once

namespace nn {
namespace socket {

/**
    Global variable @a errno values, also returned by @ref nn::socket::GetLastError().
*/
enum class Errno : uint32_t
{
    ESuccess        =   0, ///< 0
    EPerm           =   1, ///< 1
    ENoEnt          =   2, ///< 2
    ESrch           =   3, ///< 3
    EIntr           =   4, ///< 4
    EIo             =   5, ///< 5
    ENxIo           =   6, ///< 6
    E2Big           =   7, ///< 7
    ENoExec         =   8, ///< 8
    EBadf           =   9, ///< 9
    EChild          =  10, ///< 10
    EAgain          =  11, ///< 11
    ENoMem          =  12, ///< 12
    EAcces          =  13, ///< 13
    EFault          =  14, ///< 14
    ENotBlk         =  15, ///< 15
    EBusy           =  16, ///< 16
    EExist          =  17, ///< 17
    EXDev           =  18, ///< 18
    ENoDev          =  19, ///< 19
    ENotDir         =  20, ///< 20
    EIsDir          =  21, ///< 21
    EInval          =  22, ///< 22
    ENFile          =  23, ///< 23
    EMFile          =  24, ///< 24
    ENotTy          =  25, ///< 25
    ETxtBsy         =  26, ///< 26
    EFBig           =  27, ///< 27
    ENoSpc          =  28, ///< 28
    ESPipe          =  29, ///< 29
    ERofs           =  30, ///< 30
    EMLink          =  31, ///< 31
    EPipe           =  32, ///< 32
    EDom            =  33, ///< 33
    ERange          =  34, ///< 34
    EDeadLk         =  35, ///< 35
    ENameTooLong    =  36, ///< 36
    ENoLck          =  37, ///< 37
    ENoSys          =  38, ///< 38
    ENotEmpty       =  39, ///< 39
    ELoop           =  40, ///< 40
    EWouldBlock     =  EAgain, ///< 11
    ENoMsg          =  42, ///< 42
    EIdrm           =  43, ///< 43
    EChrng          =  44, ///< 44
    EL2NSync        =  45, ///< 45
    EL3Hlt          =  46, ///< 46
    EL3Rst          =  47, ///< 47
    ELnrng          =  48, ///< 48
    EUnatch         =  49, ///< 49
    ENoCsi          =  50, ///< 50
    EL2Hlt          =  51, ///< 51
    EBade           =  52, ///< 52
    EBadr           =  53, ///< 53
    EXFull          =  54, ///< 54
    ENoAno          =  55, ///< 55
    EBadRqc         =  56, ///< 56
    EBadSsl         =  57, ///< 57
    EDeadLock       =  EDeadLk, ///< 35
    EBFont          =  59, ///< 59
    ENoStr          =  60, ///< 60
    ENoData         =  61, ///< 61
    ETime           =  62, ///< 62
    ENoSr           =  63, ///< 63
    ENoNet          =  64, ///< 64
    ENoPkg          =  65, ///< 65
    ERemote         =  66, ///< 66
    ENoLink         =  67, ///< 67
    EAdv            =  68, ///< 68
    ESrmnt          =  69, ///< 69
    EComm           =  70, ///< 70
    EProto          =  71, ///< 71
    EMultiHop       =  72, ///< 72
    EDotDot         =  73, ///< 73
    EBadMsg         =  74, ///< 74
    EOverflow       =  75, ///< 75
    ENotUnuq        =  76, ///< 76
    EBadFd          =  77, ///< 77
    ERemChg         =  78, ///< 78
    ELibAcc         =  79, ///< 79
    ELibBad         =  80, ///< 80
    ELibScn         =  81, ///< 81
    ELibMax         =  82, ///< 82
    ELibExec        =  83, ///< 83
    EIlSeq          =  84, ///< 84
    ERestart        =  85, ///< 85
    EStrPipe        =  86, ///< 86
    EUsers          =  87, ///< 87
    ENotSock        =  88, ///< 88
    EDestAddrReq    =  89, ///< 89
    EMsgSize        =  90, ///< 90
    EPrototype      =  91, ///< 91
    ENoProtoOpt     =  92, ///< 92
    EProtoNoSupport =  93, ///< 93
    ESocktNoSupport =  94, ///< 94
    EOpNotSupp      =  95, ///< 95
    ENotSup         =  EOpNotSupp, ///< 95
    EPfNoSupport    =  96, ///< 96
    EAfNoSupport    =  97, ///< 97
    EAddrInUse      =  98, ///< 98
    EAddrNotAvail   =  99, ///< 99
    ENetDown        = 100, ///< 100
    ENetUnreach     = 101, ///< 101
    ENetReset       = 102, ///< 102
    EConnAborted    = 103, ///< 103
    EConnReset      = 104, ///< 104
    ENoBufs         = 105, ///< 105
    EIsConn         = 106, ///< 106
    ENotConn        = 107, ///< 107
    EShutDown       = 108, ///< 108
    ETooManyRefs    = 109, ///< 109
    ETimedOut       = 110, ///< 110
    EConnRefused    = 111, ///< 111
    EHostDown       = 112, ///< 112
    EHostUnreach    = 113, ///< 113
    EAlready        = 114, ///< 114
    EInProgress     = 115, ///< 115
    EStale          = 116, ///< 116
    EUClean         = 117, ///< 117
    ENotNam         = 118, ///< 118
    ENAvail         = 119, ///< 119
    EIsNam          = 120, ///< 120
    ERemoteIo       = 121, ///< 121
    EDQuot          = 122, ///< 122
    ENoMedium       = 123, ///< 123
    EMediumType     = 124, ///< 124
    ECanceled       = 125, ///< 125
    ENoKey          = 126, ///< 126
    EKeyExpired     = 127, ///< 127
    EKeyRevoked     = 128, ///< 128
    EKeyRejected    = 129, ///< 129
    EOwnerDead      = 130, ///< 130
    ENotRecoverable = 131, ///< 131
    ERfKill         = 132, ///< 132
    EHwPoison       = 133, ///< 133
    EProcLim        = 156  ///< 156
};

/**
    Error return codes from @ref nn::socket::GetHostEntByName() and @ref nn::socket::GetHostEntByAddr()
*/
enum class HErrno : int32_t
{
    Netdb_Internal  = -1,       ///< See @ref GetLastError().
    Netdb_Success   = 0,        ///< No problem.
    Host_Not_Found  = 1,        ///< Authoritative Answer Host not found.
    Try_Again       = 2,        ///< Non-Authoritive Host not found, or SERVERFAIL.
    No_Recovery     = 3,        ///< Non recoverable errors, FORMERR, REFUSED, NOTIMP.
    No_Data         = 4,        ///< Valid name, no data record of requested type.
    No_Address      = No_Data   ///< No address, look for MX record.
};

/**
    Error return codes from @ref nn::socket::GetAddrInfo() and @ref nn::socket::GetNameInfo()
*/
enum class AiErrno : uint32_t
{
    EAi_Success     =  0,   ///< No problem.
    EAi_AddrFamily  =  1,   ///< Address family for hostname not supported.
    EAi_Again       =  2,   ///< Temporary failure in name resolution.
    EAi_BadFlags    =  3,   ///< Invalid value for ai_flags.
    EAi_Fail        =  4,   ///< Non-recoverable failure in name resolution.
    EAi_Family      =  5,   ///< Family not supported.
    EAi_Memory      =  6,   ///< Memory allocation failure.
    EAi_NoData      =  7,   ///< No address associated with hostname.
    EAi_NoName      =  8,   ///< Hostname or servname not provided, or not known.
    EAi_Service     =  9,   ///< Servname not supported for ai_socktype.
    EAi_SockType    = 10,   ///< Socket @ref Type not supported.
    EAi_System      = 11,   ///< System error returned in errno.
    EAi_BadHints    = 12,   ///< Invalid value for hints.
    EAi_Protocol    = 13,   ///< Resolved protocol is unknown.
    EAi_Overflow    = 14,   ///< Argument buffer overflow.
    EAi_Max         = 15    ///< Used for bounds checking.
};

inline bool operator ! (Errno a) NN_NOEXCEPT
{
    return (Errno::ESuccess == a);
}

inline bool operator ! (HErrno a) NN_NOEXCEPT
{
    return (HErrno::Netdb_Success == a);
}

inline bool operator ! (AiErrno a) NN_NOEXCEPT
{
    return (AiErrno::EAi_Success == a);
}

}} /* nn::socket */
