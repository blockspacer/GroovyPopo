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

#include <nn/vfx/vfx_EmitterSet.h>
#include <nn/vfx/vfx_Handle.h>
#include <nn/vfx/viewer/vfx_Preview.h>

namespace nn {
namespace vfx {
namespace viewer {
namespace detail {

//---------------------------------------------------------------------------
//! @brief エフェクトプレビューが利用するグループID
//---------------------------------------------------------------------------
static const int EffectPreviewGroupId = ( nn::vfx::SystemParameters_MaxGroupCount - 1 ); //!< TBD

//---------------------------------------------------------------------------
//! @brief  エフェクトプレビュークラスです。
//---------------------------------------------------------------------------
class EffectPreview : public Preview
{
    NN_DISALLOW_COPY( EffectPreview );

public:
    //----------------------------------------
    //! @name 生成/破棄 処理
    //@{

    //---------------------------------------------------------------------------
    //! @brief  EffectPreview クラスのインスタンスを生成します。
    //! @param[in] pHeap     プレビューを生成するヒープ
    //! @param[in] pSystem   エフェクトシステム
    //! @param[in] guid      プレビューのグローバルID
    //! @return              生成されたプレビュー
    //---------------------------------------------------------------------------
    static EffectPreview* CreateEffectPreview( nn::vfx::Heap* pHeap, nn::vfx::System* pSystem, nn::vfx::viewer::detail::Guid guid ) NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief  EffectPreview クラスのインスタンスを破棄します。
    //! @param[in] pHeap        プレビューを生成したヒープ
    //! @param[in] pPreview     破棄するプレビュー
    //---------------------------------------------------------------------------
    static void DestroyEffectPreview( nn::vfx::Heap* pHeap, Preview* pPreview ) NN_NOEXCEPT;

    //@}


    //----------------------------------------
    //! @name 取得/設定 処理
    //@{

    //---------------------------------------------------------------------------
    //! @brief        エフェクトプレビューリソースを設定します。
    //! @param[in]    pResource エフェクトプレビューリソース。
    //---------------------------------------------------------------------------
    void SetResEffectPreview( const ResEffectPreview* pResource ) NN_NOEXCEPT
    {
        m_ResEffectPreview = *pResource;
        m_IsSetResEffectPreview = true;
        ApplyEffectPreviewParameter();
    }

    //---------------------------------------------------------------------------
    //! @brief        エフェクトプレビューリソースを取得します。
    //! @return       エフェクトプレビューリソースを返します。
    //---------------------------------------------------------------------------
    ResEffectPreview* GetResEffectPreview() NN_NOEXCEPT
    {
        return &m_ResEffectPreview;
    }

    //@}


    //----------------------------------------
    //! @name 定期 処理
    //@{

    //---------------------------------------------------------------------------
    //! @brief        計算処理です。
    //! @param[in] bPause         再生ポーズ中かどうか
    //! @param[in] frameRate      再生フレームレート
    //! @param[in] centerMatrix   ビューア中心行列
    //! @param[in] viewMatrix     ビューマトリクス
    //---------------------------------------------------------------------------
    bool Calculate( bool bPause, float frameRate, const nn::util::Matrix4x3fType& centerMatrix, const nn::util::Matrix4x3fType& viewMatrix ) NN_NOEXCEPT;

    //@}

    //----------------------------------------
    //! @name エミッタセット操作
    //@{

    //---------------------------------------------------------------------------
    //! @brief  エミッタセットを生成します。
    //! @return 生成成功で true, 失敗で false
    //---------------------------------------------------------------------------
    bool CreateEmitterSet() NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief  エミッタセットを破棄します。
    //! @return 生成成功で true, 失敗で false
    //---------------------------------------------------------------------------
    bool DestroyEmitterSet() NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief  指定フレーム再生位を進めます。
    //! @param[in] frame     エフェクト再生開始フレーム
    //! @return 生成成功で true, 失敗で false
    //---------------------------------------------------------------------------
    bool ForceCalculate( int frame ) NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief  再生中のエミッタセットに対してフェードを呼び出します。
    //! @return 生成成功で true, 失敗で false
    //---------------------------------------------------------------------------
    bool Fade() NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief  プレビュー再生をリセットします。
    //! @param[in] doFade     再生中エフェクトをフェードさせてリセットするかどうか
    //---------------------------------------------------------------------------
    void ResetPreview( bool doFade ) NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief  マトリクスを取得します。
    //! @param[in] pOutMatrix     書き込み先マトリクス
    //! @param[in] index          取得するマトリクスのインデックス( エフェクトプレビューでは未使用 )
    //---------------------------------------------------------------------------
    void GetMatrix( nn::util::Matrix4x3fType* pOutMatrix, int index ) NN_NOEXCEPT
    {
        NN_UNUSED( index );
        *pOutMatrix = GetDrawMatrix();
    }

    //---------------------------------------------------------------------------
    //! @brief  エミッタセットハンドルを取得します。
    //! @return エミッタセットハンドル
    //---------------------------------------------------------------------------
    nn::vfx::Handle* GetEmitterSetHandle() NN_NOEXCEPT
    {
        return &m_CurrentHandle;
    }

    //---------------------------------------------------------------------------
    //! @brief        エフェクトリソースIDを設定します。
    //! @param[in] resourceId     リソースID
    //---------------------------------------------------------------------------
    void SetResourceId( int resourceId ) NN_NOEXCEPT
    {
        m_ResId = resourceId;
    }

    //---------------------------------------------------------------------------
    //! @brief  エフェクトリソースIDを取得します。
    //! @return リソースID
    //---------------------------------------------------------------------------
    int GetResourceId() const NN_NOEXCEPT
    {
        return m_ResId;
    }

    //---------------------------------------------------------------------------
    //! @brief  描画のOn/Offを設定します。
    //! @param[in] visibility   trueで描画、falseで描画しない
    //---------------------------------------------------------------------------
    void SetVisible( bool visibility ) NN_NOEXCEPT;

    //@}

private:
    //---------------------------------------------------------------------------
    //! @briefprivate  コンストラクタです。
    //---------------------------------------------------------------------------
    EffectPreview() NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @briefprivate  エフェクトプレビューを初期化します。
    //! @param[in] pSystem     エフェクトシステムオブジェクト
    //---------------------------------------------------------------------------
    void InitializeEffectPreview( nn::vfx::System* pSystem ) NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @briefprivate  エフェクトプレビューリソースを適用する。
    //---------------------------------------------------------------------------
    void ApplyEffectPreviewParameter() NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @briefprivate Fade処理中のエミッタセット最大保持数
    //---------------------------------------------------------------------------
    enum PreviewSettings
    {
        PreviewSettings_MaxFadingPreviewCount = 32,       //!< Fade処理中のエミッタセット最大保持数
    };

    nn::vfx::System*            m_pSystem;                                                                  //!< エフェクトシステム
    nn::vfx::Handle             m_CurrentHandle;                                                            //!< 再生中のエフェクトハンドル
    nn::vfx::EmitterSet*        m_pFadeEmitterSetList[ PreviewSettings_MaxFadingPreviewCount ];             //!< フェード中のエフェクトハンドル
    int                         m_ResId;                                                                    //!< リソースID
    bool                        m_IsEmitterSetVisible;                                                      //!< エミッタセットの表示、非表示
    bool                        m_IsEmitterVisible[ nn::vfx::SystemParameters_MaxEmitterInclusionCount ];   //!< エミッタの表示、非表示
    ResEffectPreview            m_ResEffectPreview;                                                         //!< エフェクトプレビューリソース
    bool                        m_IsSetResEffectPreview;                                                    //!< エフェクトプレビューリソースがセットされたか？
    nn::vfx::EmitReservationInfo*   m_pReservationList;                                                     //!< TBD
    bool                        m_IsManualEmitter;
};

} // namespace detail
} // namespace viewer
} // namespace vfx
} // namespace nn
