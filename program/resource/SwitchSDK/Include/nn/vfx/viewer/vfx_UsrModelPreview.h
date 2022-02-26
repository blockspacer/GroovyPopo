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

#include <nn/util/util_Matrix.h>
#include <nn/vfx/viewer/vfx_Viewer.h>
#include <nn/vfx/viewer/vfx_Preview.h>
#include <nn/vfx/viewer/vfx_VwrString.h>

namespace nn {
namespace vfx {
namespace viewer {
namespace detail {

//---------------------------------------------------------------------------
//! @brief        ユーザーモデルプレビューです。
//---------------------------------------------------------------------------
class UserModelPreview : public Preview
{
    NN_DISALLOW_COPY( UserModelPreview );
public:
    //---------------------------------------------------------------------------
    //! @brief     UserModelPreview クラスのインスタンスを生成します。
    //! @param[in] pHeap            ヒープ
    //! @param[in] guid             グローバルID
    //! @param[in] modelName        モデル名
    //! @param[in] pModelEnumerator モデル情報取得クラス
    //! @return                     生成済みモデルプレビュー
    //---------------------------------------------------------------------------
    static UserModelPreview* CreateUserModelPreview( nn::vfx::Heap* pHeap, nn::vfx::viewer::detail::Guid guid, const char* modelName, ModelEnumerator* pModelEnumerator ) NN_NOEXCEPT
    {
        // クラスの生成を行います。
        void* buffer = pHeap->Alloc( sizeof( UserModelPreview ) );
        if( !buffer )
        {
            return NULL;
        }
        UserModelPreview* userModelPreview = new ( buffer )UserModelPreview( VfxPreviewType_UserModel, modelName, pModelEnumerator );

        // TODO: Initialize()の引数をチェックする必要がる
        // プレビューを初期化します。
        userModelPreview->Initialize( pHeap, VfxPreviewType_UserModel, guid );

        // 破棄コールバックをセット
        userModelPreview->SetDestroyPreviewCallback( UserModelPreview::DestroyUserModelPreview );

        return userModelPreview;
    }

    //---------------------------------------------------------------------------
    //! @brief        UsrModelPreviewクラスのインスタンスを破棄します。
    //! @param[in] pHeap            ヒープ
    //! @param[in] pPreview         破棄するモデルプレビュー
    //---------------------------------------------------------------------------
    static void DestroyUserModelPreview( nn::vfx::Heap* pHeap, Preview* pPreview ) NN_NOEXCEPT
    {

        UserModelPreview* pModelPreview = reinterpret_cast< UserModelPreview* >( pPreview );
        if( pModelPreview )
        {
            pHeap->Free( pModelPreview );
        }
    }

    //---------------------------------------------------------------------------
    //! @brief        プレビュー名を取得します。
    //! @return       プレビュー名
    //---------------------------------------------------------------------------
    detail::ViewerString& GetPreivewName(){ return m_PreviewName; }

private:
    //---------------------------------------------------------------------------
    //! @brief        コンストラクタです。
    //! @param[in] type             プレビュータイプ
    //! @param[in] modelName        モデル名
    //! @param[in] pModelEnumerator モデル情報取得クラス
    //---------------------------------------------------------------------------
    UserModelPreview( VfxPreviewType type, const char* modelName, ModelEnumerator* pModelEnumerator ) NN_NOEXCEPT
        : Preview()
    {
        NN_UNUSED( type );
        m_PreviewName.Copy(modelName);
        m_pModelEnumerator = pModelEnumerator;
    }

    //---------------------------------------------------------------------------
    //! @brief        マトリクスを取得します。
    //! @param[out] pOutMatrix   計算済みマトリクス
    //! @param[in]  boneIndex    ボーンID
    //---------------------------------------------------------------------------
    virtual void GetMatrix( nn::util::Matrix4x3fType* pOutMatrix, int boneIndex) NN_NOEXCEPT
    {
        nn::util::Matrix4x3fType rootMatrix;
        nn::util::Matrix4x3fType boneMatrix;
        m_pModelEnumerator->GetModelRootMatrix(&rootMatrix, m_PreviewName.CStr());
        m_pModelEnumerator->GetBoneMatrix(&boneMatrix, m_PreviewName.CStr(), boneIndex);
        nn::util::MatrixMultiply(pOutMatrix, rootMatrix, boneMatrix);
    }

private:
    detail::ViewerString    m_PreviewName;          //!< プレビュー名
    ModelEnumerator*        m_pModelEnumerator;     //!< TBD
};

} // namespace detail
} // namespace viewer
} // namespace vfx
} // namespace nn

