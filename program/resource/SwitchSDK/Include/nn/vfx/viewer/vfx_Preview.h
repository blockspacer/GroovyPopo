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

#include <nn/vfx/viewer/vfx_Guid.h>
#include <nn/vfx/vfx_Heap.h>
#include <nn/vfx/viewer/vfx_ViewerData.h>

namespace nn {
namespace vfx {
namespace viewer {
namespace detail {

//---------------------------------------------------
//! @brief  プレビュークラス破棄コールバック 型。
//---------------------------------------------------
class Preview;
typedef void( *DestroyPreviewCallback )( nn::vfx::Heap* pHeap, nn::vfx::viewer::detail::Preview* pPreview );

//---------------------------------------------------------------------------
//! @brief  プレビュークラスです。
//---------------------------------------------------------------------------
class Preview
{
    NN_DISALLOW_COPY( Preview );
public:
    //---------------------------------------------------------------------------
    //! @brief  プレビュータイプ
    //---------------------------------------------------------------------------
    enum VfxPreviewType
    {
        VfxPreviewType_TopPreview = 0,              //!< トッププレビュー
        VfxPreviewType_ModelPreview = 1,            //!< モデルプレビュー
        VfxPreviewType_EffectPreview = 2,           //!< エフェクトプレビュー
        VfxPreviewType_UserModel = 3,               //!< ユーザーモデルプレビュー
        VfxPreviewType_MaxPreviewType,              //!< TBD
    };

    //---------------------------------------------------------------------------
    //! @brief  プレビューフラグ
    //---------------------------------------------------------------------------
    enum VfxPreviewFlag
    {
        VfxPreviewFlag_Constrained = 0x1 << 1,     //!< コンストレインされている
        VfxPreviewFlag_Function = 0x1 << 2,     //!< ファンクションを利用しているか
    };

    //---------------------------------------------------------------------------
    //! @brief  マトリクス適用タイプ
    //---------------------------------------------------------------------------
    enum MatrixApplyType
    {
        MatrixApplyType_Srt      = 0,            //!< SRT追従
        MatrixApplyType_T        = 1,            //!< Tのみ追従
    };

    //----------------------------------------
    //! @name 初期化/終了 処理
    //@{

    //---------------------------------------------------------------------------
    //! @brief  コンストラクタです。
    //! @param[in] type     TBD
    //---------------------------------------------------------------------------
    explicit Preview( VfxPreviewType type ) NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief  コンストラクタです。
    //---------------------------------------------------------------------------
    Preview() NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief  デストラクタです。
    //---------------------------------------------------------------------------
    virtual ~Preview() NN_NOEXCEPT
    {
    }

    //---------------------------------------------------------------------------
    //! @brief  初期化処理です。
    //! @param[in] pHeap     TBD
    //! @param[in] type     TBD
    //! @param[in] guid     TBD
    //---------------------------------------------------------------------------
    void Initialize( nn::vfx::Heap* pHeap, VfxPreviewType type, nn::vfx::viewer::detail::Guid guid ) NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief  終了処理です。
    //---------------------------------------------------------------------------
    void Finalize() NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief  プレビューリソースを設定します。
    //! @param[in] pResPreview     TBD
    //---------------------------------------------------------------------------
    void SetResPreview( ResPreview* pResPreview ) NN_NOEXCEPT
    {
        m_ResPreview = *pResPreview;
        m_IsSetResPreview = true;
    }

    //---------------------------------------------------------------------------
    //! @brief  プレビューリソースを取得します。
    //! @return TBD
    //---------------------------------------------------------------------------
    ResPreview* GetResPreview() NN_NOEXCEPT
    {
        return &m_ResPreview;
    }

    //---------------------------------------------------------------------------
    //! @brief  リソースマトリクスを設定します。
    //! @param[in] matrix     TBD
    //---------------------------------------------------------------------------
    void SetResMatrix( nn::util::Matrix4x3fType& matrix ) NN_NOEXCEPT
    {
        m_ResMatrix = matrix;
    }

    //@}

    //----------------------------------------
    //! @name 情報取得
    //@{

    //---------------------------------------------------------------------------
    //! @brief  プレビューのタイプを取得します。
    //! @return TBD
    //---------------------------------------------------------------------------
    VfxPreviewType GetPreviewType() const NN_NOEXCEPT
    {
        return m_PreviewType;
    }

    //---------------------------------------------------------------------------
    //! @brief  guidを取得します。
    //! @return TBD
    //---------------------------------------------------------------------------
    nn::vfx::viewer::detail::Guid GetGuid() const NN_NOEXCEPT
    {
        return m_Guid;
    }

    //---------------------------------------------------------------------------
    //! @brief  マトリクスを取得します。
    //! @param[in] pOutMatrix   TBD
    //! @param[in] index        TBD
    //---------------------------------------------------------------------------
    virtual void GetMatrix( nn::util::Matrix4x3fType* pOutMatrix, int index ) NN_NOEXCEPT
    {
        NN_UNUSED( index );
        nn::util::MatrixIdentity( pOutMatrix );
    }

    //---------------------------------------------------------------------------
    //! @brief        描画のOn/Offを状態を取得します。
    //! @return TBD
    //---------------------------------------------------------------------------
    virtual bool IsVisible() const NN_NOEXCEPT
    {
        return true;
    }

    //@}

    //----------------------------------------
    //! @name 定期処理
    //@{

    //---------------------------------------------------------------------------
    //! @brief  計算処理です。
    //! @param[in] isPause      一時停止状態か
    //! @param[in] frameRate    再生レート
    //! @param[in] centerMatrix ビューア中心行列
    //! @param[in] viewMatrix   ビュー行列
    //---------------------------------------------------------------------------
    virtual bool Calculate( bool isPause, float frameRate, const nn::util::Matrix4x3fType& centerMatrix, const nn::util::Matrix4x3fType& viewMatrix ) NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief  プレビュー描画処理です。
    //---------------------------------------------------------------------------
    virtual void Draw( nn::gfx::CommandBuffer* pCommandBuffer ) NN_NOEXCEPT
    {
        NN_UNUSED( pCommandBuffer );
    }

    //@}

    //----------------------------------------
    //! @name プレビューリスト操作
    //@{

    //---------------------------------------------------------------------------
    //! @brief  プレビューをリストに追加します。
    //! @param[in] pPreview     TBD
    //---------------------------------------------------------------------------
    void AddPreview( Preview* pPreview ) NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief  このプレビューをリストから削除します。
    //---------------------------------------------------------------------------
    void RemovePreview() NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief  NextPreviewを取得します。
    //! @return TBD
    //---------------------------------------------------------------------------
    Preview* GetNextPreview() const NN_NOEXCEPT
    {
        return m_pNext;
    }

    //---------------------------------------------------------------------------
    //! @brief  Guidを指定してプレビューを取得します。
    //! @param[in] guid     TBD
    //! @return TBD
    //---------------------------------------------------------------------------
    Preview* GetPreview( nn::vfx::viewer::detail::Guid guid ) NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief  最後尾のPreviewを取得します。
    //! @return TBD
    //---------------------------------------------------------------------------
    Preview* GetTailPreview() NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief  先頭のPreviewを取得します。
    //! @return TBD
    //---------------------------------------------------------------------------
    Preview* GetHeadPreview() NN_NOEXCEPT;

    //@}

    //----------------------------------------
    //! @name プレビュー操作
    //@{

    //---------------------------------------------------------------------------
    //! @brief        プレビューをリセットします。
    //! @param[in] doFade     TBD
    //---------------------------------------------------------------------------
    virtual void ResetPreview( bool doFade ) NN_NOEXCEPT
    {
        NN_UNUSED( doFade );
    }

    //---------------------------------------------------------------------------
    //! @brief  描画のOn/Offを設定します。
    //! @param[in] visibility     TBD
    //---------------------------------------------------------------------------
    virtual void SetVisible( bool visibility ) NN_NOEXCEPT
    {
        NN_UNUSED( visibility );
    }

    //---------------------------------------------------------------------------
    //! @brief  描画マトリクスを取得します
    //! @return 描画マトリクス
    //---------------------------------------------------------------------------
    const nn::util::Matrix4x3fType& GetDrawMatrix() const NN_NOEXCEPT
    {
        return m_DrawMatrix;
    }

    //---------------------------------------------------------------------------
    //! @brief  プレビュータイムを取得します
    //! @return プレビュータイム
    //---------------------------------------------------------------------------
    float GetTime() const NN_NOEXCEPT
    {
        return m_Time;
    }

    //---------------------------------------------------------------------------
    //! @brief  リリースカウンタを設定します
    //! @param[in] counter     設定するリリースカウンタ
    //---------------------------------------------------------------------------
    void SetReleaseCounter( int counter ) NN_NOEXCEPT
    {
        m_ReleaseCounter = counter;
    }

    //---------------------------------------------------------------------------
    //! @brief  リリースカウンタを取得します
    //! @return リリースカウンタ
    //---------------------------------------------------------------------------
    int GetReleaseCounter() const NN_NOEXCEPT
    {
        return m_ReleaseCounter;
    }

    //---------------------------------------------------------------------------
    //! @brief  リリースカウンタを取得します
    //! @return リリースカウンタ
    //---------------------------------------------------------------------------
    bool IsAlive() const NN_NOEXCEPT
    {
        return m_ReleaseCounter == -1 ? true : false;
    }

    //@}

protected:
    //---------------------------------------------------------------------------
    //! @brief  ヒープへのポインタを取得します
    //! @return ヒープへのポインタ
    //---------------------------------------------------------------------------
    nn::vfx::Heap* GetHeap() const NN_NOEXCEPT
    {
        return m_pHeap;
    }

    //---------------------------------------------------------------------------
    //! @brief          プレビュータイムを設定します
    //! @param[in] time    プレビュータイム
    //---------------------------------------------------------------------------
    void SetTime( float time ) NN_NOEXCEPT
    {
        m_Time = time;
    }

    //---------------------------------------------------------------------------
    //! @brief        破棄コールバックをセットします。
    //! @param[in] callback     TBD
    //---------------------------------------------------------------------------
    void SetDestroyPreviewCallback( DestroyPreviewCallback callback ) NN_NOEXCEPT
    {
        m_DestroyCallback = callback;
    }

public:
    //---------------------------------------------------------------------------
    //! @brief        破棄コールバックを取得します。
    //! @return TBD
    //---------------------------------------------------------------------------
    DestroyPreviewCallback GetDestroyPreviewCallback() const NN_NOEXCEPT
    {
        return m_DestroyCallback;
    }

private:
    //---------------------------------------------------------------------------
    //! @brief        自動移動を計算します。
    //---------------------------------------------------------------------------
    void CalculateAutoMove() NN_NOEXCEPT;

private:
    nn::vfx::Heap*              m_pHeap;             //!< ヒープ
    Guid                        m_Guid;              //!< guid
    VfxPreviewType              m_PreviewType;       //!< プレビュータイプ
    int                         m_ReleaseCounter;    //!< リリースカウンタ
    Preview*                    m_pPrev;             //!< プレビュー:Prev
    Preview*                    m_pNext;             //!< プレビュー:Next
    float                       m_Time;              //!< プレビュータイム
    nn::util::Matrix4x3fType    m_ResMatrix;         //!< リソースマトリクス
    bool                        m_IsSetResPreview;   //!< プレビューリソースがセットされたか？
    ResPreview                  m_ResPreview;        //!< プレビューリソース
    nn::util::Matrix4x3fType    m_AutoMoveMatrix;    //!< 自動移動計算結果
    nn::util::Matrix4x3fType    m_DrawMatrix;        //!< 最終描画マトリクス
    DestroyPreviewCallback      m_DestroyCallback;   //!< TBD
};

} // namespace detail
} // namespace viewer
} // namespace vfx
} // namespace nn
