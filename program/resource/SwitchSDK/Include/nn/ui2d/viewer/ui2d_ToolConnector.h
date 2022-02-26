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

#include <nn/TargetConfigs/build_Base.h>
#include <nn/nn_SdkAssert.h>
#include <nn/os/os_Config.h>
#include <nn/perf/perf_LoadMeter.h>
#include <nn/ui2d/viewer/ui2d_TaskBase.h>
#include <nn/ui2d/viewer/ui2d_Config.h>
#if defined(NN_UI2D_VIEWER_ENABLED)

namespace nn {
namespace ui2d {
namespace viewer {

//------------------------------------------------------------------------------
//! @brief  InGameEditing ToolConnector クラス
//------------------------------------------------------------------------------
class ToolConnector
{
    NN_DISALLOW_COPY( ToolConnector );
public:
    //------------------------------------------------------------------------------
    //! @brief TBD
    //! @param[in] pAllocateFunction     TBD
    //! @param[in] pFreeFunction         TBD
    //! @param[in] pUserDataForAllocator TBD
    //! @param[in] bufferSize            TBD
    //! @param[in] firstReadSize         TBD
    //! @param[in] noUseInternalThread   TBD
    //! @param[in] priority              TBD
    //! @return TBD
    //------------------------------------------------------------------------------
    static bool Initialize(
        nn::AlignedAllocateFunctionWithUserData pAllocateFunction,
        nn::FreeFunctionWithUserData            pFreeFunction,
        void*                                   pUserDataForAllocator,
        size_t                                  bufferSize,
        size_t                                  firstReadSize,
        bool                                    noUseInternalThread,
        int                                     priority ) NN_NOEXCEPT;

    //------------------------------------------------------------------------------
    //! @brief TBD
    //------------------------------------------------------------------------------
    static void Finalize() NN_NOEXCEPT;

    //------------------------------------------------------------------------------
    //! @brief TBD
    //! @return TBD
    //------------------------------------------------------------------------------
    static bool IsConnected() NN_NOEXCEPT
    {
        return g_IsConnected;
    }

    //------------------------------------------------------------------------------
    //! @brief TBD
    //! @param[out] pReceiveSize  TBD
    //! @return TBD
    //------------------------------------------------------------------------------
    static uint8_t* GetPacket( size_t* pReceiveSize = NULL ) NN_NOEXCEPT;

    //------------------------------------------------------------------------------
    //! @brief TBD
    //! @param[in] pSendBuffer  TBD
    //! @param[in] sendSize     TBD
    //! @return TBD
    //------------------------------------------------------------------------------
    static bool SendPacket( void *pSendBuffer, size_t sendSize ) NN_NOEXCEPT;

    //------------------------------------------------------------------------------
    //! @brief TBD
    //! @param[in] intArg     TBD
    //! @param[in] ptrArg     TBD
    //! @return TBD
    //------------------------------------------------------------------------------
    static int ThreadToolReceiver( int intArg, void* ptrArg ) NN_NOEXCEPT;

private:
    //------------------------------------------------------------------------------
    //! @brief TBD
    //------------------------------------------------------------------------------
    ToolConnector() NN_NOEXCEPT
    {
    }

    //------------------------------------------------------------------------------
    //! @brief TBD
    //! @param[in] param     TBD
    //! @return TBD
    //------------------------------------------------------------------------------
    static bool ReadPacket( int param ) NN_NOEXCEPT;

    //------------------------------------------------------------------------------
    //! @brief TBD
    //! @param[in] handle           TBD
    //! @param[in] pReceiveBuffer   TBD
    //! @param[in] receiveSize      TBD
    //! @return TBD
    //------------------------------------------------------------------------------
    static uint32_t ReadPacketInternal( int handle, uint8_t* pReceiveBuffer, size_t receiveSize ) NN_NOEXCEPT;

    //------------------------------------------------------------------------------
    //! @brief TBD
    //! @param[in] param        TBD
    //! @param[in] pSendBuffer  TBD
    //! @param[in] sendSize     TBD
    //! @return TBD
    //------------------------------------------------------------------------------
    static bool SendPacketInternal( int param, void* pSendBuffer, size_t sendSize ) NN_NOEXCEPT;

private:
    static volatile bool g_IsConnected;         //!< ツールとの接続状態をあわすフラグ
};

} // namespace viewer
} // namespace ui2d
} // namespace nn

#endif // NN_UI2D_VIEWER_ENABLED
