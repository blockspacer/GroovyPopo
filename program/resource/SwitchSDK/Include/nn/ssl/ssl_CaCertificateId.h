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

namespace nn { namespace ssl {

/**
 * @brief  IDs for built-in Certificate Authorities
 */
enum CaCertificateId
{
    CaCertificateId_NintendoCAG3 = 0x00000001,                                        //!< Nintendo CA G3
    CaCertificateId_NintendoClass2CAG3 = 0x00000002,                                  //!< Nintendo Class 2 CA G3
    CaCertificateId_AmazonRootCA1 = 0x000003E8,                                       //!< Amazon Root CA 1
    CaCertificateId_StarfieldServicesRootCertificateAuthorityG2 = 0x000003E9,         //!< Starfield Services Root Certificate Authority G2
    CaCertificateId_AddTrustExternalCARoot = 0x000003EA,                              //!< AddTrust External CA Root
    CaCertificateId_COMODOCertificationAuthority = 0x000003EB,                        //!< COMODO Certification Authority
    CaCertificateId_UTNDATACorpSGC = 0x000003EC,                                      //!< UTN - DATACorp SGC
    CaCertificateId_UTNUSERFirstHardware = 0x000003ED,                                //!< UTN-USERFirst-Hardware
    CaCertificateId_BaltimoreCyberTrustRoot = 0x000003EE,                             //!< Baltimore CyberTrust Root
    CaCertificateId_CybertrustGlobalRoot = 0x000003EF,                                //!< Cybertrust Global Root
    CaCertificateId_VerizonGlobalRootCA = 0x000003F0,                                 //!< Verizon Global Root CA
    CaCertificateId_DigiCertAssuredIDRootCA = 0x000003F1,                             //!< DigiCert Assured ID Root CA
    CaCertificateId_DigiCertAssuredIDRootG2 = 0x000003F2,                             //!< DigiCert Assured ID Root G2
    CaCertificateId_DigiCertGlobalRootCA = 0x000003F3,                                //!< DigiCert Global Root CA
    CaCertificateId_DigiCertGlobalRootG2 = 0x000003F4,                                //!< DigiCert Global Root G2
    CaCertificateId_DigiCertHighAssuranceEVRootCA = 0x000003F5,                       //!< DigiCert High Assurance EV Root CA
    CaCertificateId_EntrustnetCertificationAuthority2048 = 0x000003F6,                //!< Entrustnet Certification Authority (2048)
    CaCertificateId_EntrustRootCertificationAuthority = 0x000003F7,                   //!< Entrustnet Root Certification Authority
    CaCertificateId_EntrustRootCertificationAuthorityG2 = 0x000003F8,                 //!< Entrustnet Root Certification Authority G2
    CaCertificateId_GeoTrustGlobalCA2 = 0x000003F9,                                   //!< GeoTrust Global CA 2
    CaCertificateId_GeoTrustGlobalCA = 0x000003FA,                                    //!< GeoTrust Global CA
    CaCertificateId_GeoTrustPrimaryCertificationAuthorityG3 = 0x000003FB,             //!< GeoTrust Primary Certification Authority G3
    CaCertificateId_GeoTrustPrimaryCertificationAuthority = 0x000003FC,               //!< GeoTrust Primary Certification Authority
    CaCertificateId_GlobalSignRootCA = 0x000003FD,                                    //!< GlobalSign Root CA
    CaCertificateId_GlobalSignRootCAR2 = 0x000003FE,                                  //!< GlobalSign Root CA R2
    CaCertificateId_GlobalSignRootCAR3 = 0x000003FF,                                  //!< GlobalSign Root CA R3
    CaCertificateId_GoDaddyClass2CertificationAuthority = 0x00000400,                 //!< Go Daddy Class 2 Certification Authority
    CaCertificateId_GoDaddyRootCertificateAuthorityG2 = 0x00000401,                   //!< Go Daddy Root Certificate Authority G2
    CaCertificateId_StarfieldClass2CertificationAuthority = 0x00000402,               //!< Starfield Class 2 Certification Authority
    CaCertificateId_StarfieldRootCertificateAuthorityG2 = 0x00000403,                 //!< Starfield Root Certificate Authority G2
    CaCertificateId_thawtePrimaryRootCAG3 = 0x00000404,                               //!< thawte Primary Root CA G3
    CaCertificateId_thawtePrimaryRootCA = 0x00000405,                                 //!< thawte Primary Root CA
    CaCertificateId_VeriSignClass3PublicPrimaryCertificationAuthorityG3 = 0x00000406, //!< VeriSign Class 3 Public Primary Certification Authority G3
    CaCertificateId_VeriSignClass3PublicPrimaryCertificationAuthorityG5 = 0x00000407, //!< VeriSign Class 3 Public Primary Certification Authority G5
    CaCertificateId_VeriSignUniversalRootCertificationAuthority = 0x00000408,         //!< VeriSign Universal Root Certification Authority
    CaCertificateId_DSTRootCAX3 = 0x00000409,                                         //!< DST Root CA X3
    CaCertificateId_All = 0xFFFFFFFF                                                  //!< All built-in CAs
};

} }    //  nn::ssl
