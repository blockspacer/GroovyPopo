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

#include <nn/nn_Common.h>
#include <nn/gfx/gfx_Core.h>
#include <nn/gfx/gfx_Types.h>
#include <nn/g3d/g3d_ResCommon.h>

// nn::g3d::viewer のバージョン
#define NN_G3D_VIEWER_VERSION_MAJOR ( 3 ) // NOLINT(readability/define)
#define NN_G3D_VIEWER_VERSION_MINOR ( 9 ) // NOLINT(readability/define)
#define NN_G3D_VIEWER_VERSION_MICRO ( 0 ) // NOLINT(readability/define)

#if (defined NN_ENABLE_HTC) && (defined NN_DISABLE_HTC)
#error can not define NN_ENABLE_HTC and NN_DISABLE_HTC at the same time.
#endif

#if (!defined NN_SDK_BUILD_LIBRARY) && ((!defined NN_ENABLE_HTC) || (defined NN_DISABLE_HTC))
#define NN_DETAIL_G3D_VIEWER_CONFIG_IS_VIEWER_EMPTY (1)
#else
#define NN_DETAIL_G3D_VIEWER_CONFIG_IS_VIEWER_EMPTY (0)
#endif

#define NN_DETAIL_G3D_VIEWER_API NN_FORCEINLINE

namespace nn { namespace gfx {
    class ResTextureFile;
}}

namespace nn { namespace g3d { namespace viewer {
    //! @brief サーバーの処理結果を示す列挙体です。
    enum ViewerResult
    {
        ViewerResult_Success, //!< @brief 成功です。
        ViewerResult_UnknownError, //!< @brief 不明のエラーです。
        ViewerResult_MemoryAllocationFailed, //!< @brief メモリ確保に失敗しました。
        ViewerResult_ServerNotInitialized, //!< @brief サーバーが初期化されていません。
        ViewerResult_NotConnected, //!< @brief 3DEditor と接続されていません。
        ViewerResult_AlreadyAttached, //!< @brief 既にアタッチされています。
        ViewerResult_InvalidArgument, //!< @brief 引数が不正です。
        ViewerResult_CommandQueueFull, //!< @brief コマンドキューが一杯です。
        ViewerResult_NotAttached, //!< @brief デタッチ対象がアタッチされていません。
        ViewerResult_ViewerDisabled, //!< @brief ビューアが無効化されています。
        ViewerResult_SendRenderInfoNotRequested, //!< @brief 描画情報定義の送信が 3DEditor から要求されていません。
        ViewerResult_ServerClosing, //!< @brief サーバーがクローズ中です。
    };

    //! @brief g3d::viewer 内部におけるテクスチャバインド時に呼ばれるコールバックの型です。
    //!
    //! @param[in] name バインドするテクスチャ名です。
    //! @param[in] pResTextureFile 3DEditor から送信された nn::gfx::ResTextureFile へのポインタです。
    //! @param[in] pUserData 任意のデータへのポインタです。
    //!
    //! @return nn::gfx::TextureView へのポインタを返します。
    //!
    typedef nn::g3d::TextureRef (*ViewerTextureBindCallback)(
        const char* name, const nn::gfx::ResTextureFile* pResTextureFile, void* pUserData);

    namespace detail {
        //! @briefprivate 編集アニメーションの種類です。
        enum ViewerAnimKind
        {
            ViewerAnimKind_SkeletalAnim,
            ViewerAnimKind_ShaderParamAnim,
            ViewerAnimKind_ColorAnim,
            ViewerAnimKind_TextureSrtAnim,
            ViewerAnimKind_TexturePatternAnim,
            ViewerAnimKind_BoneVisibilityAnim,
            ViewerAnimKind_MaterialVisibilityAnim,
            ViewerAnimKind_ShapeAnim,
            ViewerAnimKind_MaterialAnim,
        };
    }
}}}
