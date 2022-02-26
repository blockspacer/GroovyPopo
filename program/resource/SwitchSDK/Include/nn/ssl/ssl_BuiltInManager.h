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
 * @file
 * @brief    Public header file for SSL library.
 */

#pragma once

#include <nn/ssl/ssl_Types.h>
#include <nn/ssl/ssl_CaCertificateId.h>

namespace nn { namespace ssl {

/**
 * @brief    The class used to retrieve bundled CA certificates in DER format.
 *
 * @details  This class may be used by applications to retrieve CA certificates
 *           which are bundled with the platform.  The bundled CA certificates
 *           are each assigned IDs by Nintendo, which can be found in the
 *           header ssl_CaCertificateId.h.
 */
class BuiltInManager
{
public:

    /**
     * @brief  Information about a specific certificate, including the DER data.
     */
    struct BuiltInCertificateInfo
    {
        CaCertificateId         id;
        TrustedCertStatus       status;
        uint32_t                certificateSize;
        union
        {
            struct
            {
                uint8_t         *pCertificateDerData;
            } ptr;

            struct
            {
                uint32_t        reserved1;
                uint32_t        reserved2;
            } priv;
        } data;
    };


    /**
     * @brief       Get the built-in certificates from the nn::ssl layer.
     *
     * @param[out]  pOutCertInfo  Pointer to a pointer which will receive the
     *                            start of an array of BuiltInCertificateInfo
     *                            structures within pOutBuffer.  This array
     *                            may be accessed to retrieve the details of
     *                            each requested certificate.  If CaCertificateId_All
     *                            is requested, the last element of this array
     *                            will have its id field set to CaCertificateId_All.
     *                            Using this information, the caller can get all
     *                            built-in certificates without having to know the
     *                            IDs ahead of time.
     * @param[out]  pOutBuffer    Pointer to a buffer to place the certificates and
     *                            meta data. After this call, the contents of this
     *                            buffer should not be directly interpreted.  Use
     *                            pOutCertInfo to get the certificate details.
     * @param[in]   bufSize       The total size of the buffer at pOutBuffer.
     * @param[in]   pInIds        Pointer to buffer containing the certificate
     *                            IDs which are being retrieved.  If CaCertificateId_All
     *                            is specified in any element then all built-in CA
     *                            certificates will be retrieved (assuming the
     *                            provided output buffer is large enough.)
     * @param[in]   idCount       The total number of IDs in the buffer.
     *
     * @return      The result of the get operation is returned
     *
     * @retval      ResultSuccess                Success
     * @retval      ResultLibraryNotInitialized  The SSL library is not initialized yet.
     * @retval      ResultInvalidPointer         The provided buffer pointer is invalid.
     * @retval      ResultBufferTooShort         The provided buffer is not large enough.
     * @retval      ResultInvalidIdCount         A value of 0 was provided for the ID count.
     * @retval      ResultErrorLower             An internal error occurred while trying
     *                                           to retrieve the certificates.
     */
    static nn::Result GetBuiltInCertificates(BuiltInCertificateInfo  **pOutCertInfo,
                                             uint8_t                 *pOutBuffer,
                                             uint32_t                bufSize,
                                             CaCertificateId         *pInIds,
                                             uint32_t                idCount);

    /**
     * @brief       Get the size of the buffer needed to be provided to nn::ssl::BuiltInManager::GetBuiltInCertificates
     *              to retrieve all of the specified certificates.
     *
     * @param[out]  pOutBufSize   Pointer to a uint32_t where the total size
     *                            of the buffer required to retrieve the specified
     *                            certificate IDs will be placed.
     * @param[in]   pInIds        Pointer to buffer containing the certificate
     *                            IDs which are being retrieved.  If the ID
     *                            CaCertificateId_All is specified in any element this
     *                            API will return the buffer size needed to get all
     *                            built-in CA certificates, each having 1 entry.
     * @param[in]   idCount       The total number of IDs in the buffer.
     *
     * @return      The result of retrieving the certificate size information.
     *
     * @retval      ResultSuccess                Success
     * @retval      ResultLibraryNotInitialized  The SSL library is not initialized yet.
     * @retval      ResultInvalidPointer         The provided pointer is invalid.
     * @retval      ResultInvalidIdCount         A value of 0 was provided for the ID count.
     */
    static nn::Result GetBuiltInCertificateBufSize(uint32_t         *pOutBufSize,
                                                   CaCertificateId  *pInIds,
                                                   uint32_t         idCount);
};

} }    //  nn::ssl
