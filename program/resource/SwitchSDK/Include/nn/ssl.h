/*--------------------------------------------------------------------------------*
  Copyright (C)Nintendo All rights reserved.

  These coded instructions, statements, and computer programs contain proprietary
  information of Nintendo and/or its licensed developers and are protected by
  national and international copyright laws. They may not be disclosed to third
  parties or copied or duplicated in any form, in whole or in part, without the
  prior written consent of Nintendo.

  The content herein is highly confidential and should be handled accordingly.
 *--------------------------------------------------------------------------------*/

/**
 *  @file
 *  @brief  Public header for the SSL library
 */

#pragma once

/**
 *  @namespace  nn::ssl
 *  @brief      Namespace for the SSL library
 *
 *              ### Overview
 *              The SSL library provides secure connection with the server on TCP/IP socket
 *              based connection along with server authentication and client authorization.
 *              The SSL library is provided for SSL client side applications, server side socket
 *              connections are not supported.
 *
 *              ### Precondition
 *              The SSL library needs to be initialized by nn::ssl::Initialize before using any
 *              API provided by the SSL library.
 *
 *              ### Setup network
 *              Prior to using the SSL library (and the socket library), it is necessary to
 *              activate the system network interface. Please refer to NIFM library documentation
 *              for details.
 *
 *              ### Working with the Socket library
 *              The SSL library is in charge of handling SSL/TLS protocols though it doesn't deal
 *              with TCP/IP connection establishment. The user of the SSL library is required to
 *              create socket, establish TCP connection with the server and pass it to the SSL
 *              library. Please refer the documentation of nn::ssl::Connection for details.
 *
 *              ### Certificate store
 *              The SSL library provides a build-in certificate store with common commercial CA
 *              certificates. These certificates are used automatically when the SSL library needs
 *              to verify the server.
 *              The SSL library provides the way to import CA certificates and client certificate/key
 *              too. Please refer the documentation of nn::ssl::Context for details.
 */

#include <nn/ssl/ssl_Types.h>
#include <nn/ssl/ssl_Api.h>
#include <nn/ssl/ssl_BuiltInManager.h>
#include <nn/ssl/ssl_CaCertificateId.h>
#include <nn/ssl/ssl_Context.h>
#include <nn/ssl/ssl_Connection.h>
#include <nn/ssl/ssl_Result.h>
#include <nn/ssl/ssl_SessionCache.h>
