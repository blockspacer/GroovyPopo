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

namespace nn     {
namespace socket {
/**
    @brief Default maximum size a tcp socket buffer can automatically resize to.
           This default value is the maximum advertised window size of stack.
*/
const int DefaultTcpAutoBufferSizeMax = (192 * 1024);

/**
    Minimum size of the memory for socket buffers that can be passed to @ref Initialize().
*/
const int MinTransferMemorySize       = (2 * DefaultTcpAutoBufferSizeMax + 128 * 1024);

/**
    Minimum size of the memory used for resolver that can be passed to @ref Initialize().
*/
const int MinSocketAllocatorSize      = (128 * 1024);

/**
    Total minimum size of the memory that can be passed to Initialize().
*/
const int MinSocketMemoryPoolSize     = (MinTransferMemorySize + MinSocketAllocatorSize);

/**
    Alignment requirement for socket library memory pools.
*/
const size_t MemoryPoolAlignment      = 4096;

/**
    Maximum value of concurrency count that can be passed to Initialize().
*/
const int ConcurrencyLimitMax         = 14;

/**
    Default size of memory allocated on behalf of the client when Initialize() is called.
*/
const int DefaultSocketMemoryPoolSize = (6  * 1024 * 1024);
const int DefaultSocketAllocatorSize  = (128 * 1024);
const int DefaultConcurrencyLimit     = ConcurrencyLimitMax;

/**
    Socket Errors within our namespace.
*/
const int InvalidSocket = -1;
const int SocketError   = -1;

/**
    Current version of the socket library.
*/
const int LibraryVersion = 5;

/**
    Longest possible address.
*/
NN_DEPRECATED const unsigned char SockMaxAddrLen  = 255;
const unsigned char Sock_MaxAddrLen  = 255;

/**
    Maximum number of pending connections.
*/
const unsigned int  SoMaxConn       = 128;

/**
    Maximum size required to convert an Af_Inet address, including null terminator.
*/
NN_DEPRECATED const unsigned int  InetAddrStrlen  = 16;
const unsigned int  Inet_AddrStrlen  = 16;

/**
    If poll timeout is InfTim (-1), the poll blocks indefinitely.
*/
const int           InfTim          = -1;

/**
    Max congestion control name length.
*/
NN_DEPRECATED const int           TcpCaNameMax    = 16;
const int           Tcp_Ca_Name_Max = 16;

}}
