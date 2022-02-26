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

#include <nn/ui2d/ui2d_Types.h>

namespace nn
{
namespace ui2d
{

struct ResImage;
class TextureInfo;

//---------------------------------------------------------------------------
//! @brief テクスチャマップを制御するクラスです。
//!
//! @details
//! このクラスは通常、Materialクラスによって生成されます。
//! ライブラリユーザ側で生成する必要はありません。
//---------------------------------------------------------------------------
class TexMap
{
public:
    //----------------------------------------
    //! @name コンストラクタ／デストラクタ
    //@{

    //! @brief コンストラクタです。
    //!
    explicit TexMap();

    //! @brief コンストラクタです。
    //!
    //! @details
    //! 指定のテクスチャ情報で初期化します。
    //!
    //! @param[in] pTextureInfo  テクスチャ情報です。
    explicit TexMap(const TextureInfo* pTextureInfo);

    //! @brief デストラクタです。
    //!
    ~TexMap();

    //! @brief リソースを開放します。
    //!
    //! @details
    //! デストラクタの呼び出し以前に呼び出す必要があります。
    //!
    void Finalize();

    //@}

    //----------------------------------------
    //! @name 設定／取得
    //@{

    //! @brief テクスチャの S 方向のラップモードを取得します。
    //!
    //! @return S 方向のラップモードを返します。
    //!
    TexWrap GetWrapModeS() const
    {
        return TexWrap(m_Bits.wrapS);
    }

    //! @brief テクスチャの T 方向のラップモードを取得します。
    //!
    //! @return T 方向のラップモードを返します。
    //!
    TexWrap GetWrapModeT() const
    {
        return TexWrap(m_Bits.wrapT);
    }

    //! @brief テクスチャのラップモードを設定します。
    //!
    //! @param[in] wrapS    テクスチャの S 方向のラップモードです。
    //! @param[in] wrapT    テクスチャの T 方向のラップモードです。
    //!
    void SetWrapMode(TexWrap wrapS, TexWrap wrapT);

    //! @brief テクスチャが縮小されるときに適用されるフィルタモードを取得します。
    //!
    //! @return テクスチャが縮小されるときに適用されるフィルタモードを返します。
    //!
    TexFilter GetMinFilter() const
    {
        return TexFilter(m_Bits.minFilter);
    }

    //! @brief テクスチャが拡大されるときに適用されるフィルタモードを取得します。
    //!
    //! @return テクスチャが拡大されるときに適用されるフィルタモードを返します。
    //!
    TexFilter GetMagFilter() const
    {
        return TexFilter(m_Bits.magFilter);
    }

    //! @brief テクスチャのフィルタモードを設定します。
    //!
    //! @param[in] minFlt   テクスチャが縮小されるときに適用されるフィルタモードです。
    //! @param[in] magFlt   テクスチャが拡大されるときに適用されるフィルタモードです。
    //!
    void SetFilter(TexFilter minFlt, TexFilter magFlt);

    //! @brief テクスチャの情報を設定し、サンプラ設定をリセットします。
    //!
    //! @details
    //! TextureInfo の値をコピーします。
    //!
    //! @param[in] pCopySrc  コピー元です。
    //!
    void Set(const TextureInfo* pCopySrc);

    //! @brief テクスチャ情報を取得します。
    //!
    //! @return テクスチャ情報を返します。
    //!
    const TextureInfo* GetTextureInfo() const { return m_pTextureInfo; }

    //! @brief テクスチャ情報を設定します。
    //!
    //! @param[in] pTextureInfo 設定する TextureInfo のポインタです。
    //!
    void SetTextureInfo(const TextureInfo* pTextureInfo)
    {
        m_pTextureInfo = pTextureInfo;
    }

    // この TexMap が使用している TextureInfo が動的レンダリングテクスチャだった場合、インスタンスの管理責任がこの TexMap にあるかどうか(オーナーかどうか)を設定します。

    //! @brief 内部用機能のため使用禁止です。
    //!
    //! @param[in] flag オーナーかどうかを設定します。
    //!
    void SetDynamicRenderingTextureOwnerFlag(bool flag)
    {
        m_Bits.dynamicTextureOwnerFlag = flag;
    }

    // この TexMap が使用している TextureInfo が動的レンダリングテクスチャだった場合、インスタンスの管理責任がこの TexMap にあるかどうか(オーナーかどうか)を取得します。

    //! @brief 内部用機能のため使用禁止です。
    //!
    //! @return オーナーかどうか。
    //!
    bool GetDynamicRenderingTextureOwnerFlag() const
    {
        return m_Bits.dynamicTextureOwnerFlag != 0;
    }

    // この TexMap がキャプチャテクスチャで上書きされている場合、上書きしているキャプチャテクスチャリソースがレイアウトの階層構造上いくつ上のデータを使用しているかを設定します。

    //! @brief 内部用機能のため使用禁止です。
    //!
    //! @param[in]  offset 上書きされている場合の自分からの相対オフセット。
    //!
    void SetCaptureTextureOverwriteOffset(int offset)
    {
        // 3bit に収まる範囲内で設定可能
        NN_SDK_ASSERT(offset >= 0 && offset < 8);
        m_Bits.captureTextureOverwriteDepthOffset = offset;
    }

    // この TexMap がキャプチャテクスチャで上書きされている場合、上書きしているキャプチャテクスチャリソースがレイアウトの階層構造上いくつ上のデータを使用しているかを取得します。

    //! @brief 内部用機能のため使用禁止です。
    //!
    //! @return 上書きされている場合の自分からの相対オフセット。
    //!
    int GetCaptureTextureOverwriteOffset() const
    {
        return m_Bits.captureTextureOverwriteDepthOffset;
    }

    // この TexMap が使用している TextureInfo がダミーの動的レンダリングテクスチャかどうかを設定します。

    //! @brief 内部用機能のため使用禁止です。
    //!
    //! @param[in] flag ダミーテクスチャかどうかを設定します。
    //!
    void SetDummyDynamicRenderingTextureFlag(bool flag)
    {
        m_Bits.dummyTextureFlag = flag;
    }

    // この TexMap が使用している TextureInfo がダミーの動的レンダリングテクスチャかどうかを取得します。

    //! @brief 内部用機能のため使用禁止です。
    //!
    //! @return ダミーテクスチャかどうか。
    //!
    bool GetDummyDynamicRenderingTextureFlag() const
    {
        return m_Bits.dummyTextureFlag != 0;
    }

    //! @brief サンプラの設定をコピーします。
    //!
    //! @param[in] texMap  サンプラの設定のコピー元です。
    //!
    void CopySamplerSettings(const TexMap& texMap);

    //@}

private:

    // サンプラの設定を初期状態にリセットします。

    //! @brief 内部用機能のため使用禁止です。
    void ResetSamplerSettings();

    // TexMap が参照している TextureInfo に関する設定をリセットします。

    //! @brief 内部用機能のため使用禁止です。
    void ResetTextureInfoState();

    //! @brief 内部用機能のため使用禁止です。
    struct Bits
    {
        uint32_t  wrapS: 2;
        uint32_t  wrapT: 2;
        uint32_t  minFilter: 3;
        uint32_t  magFilter: 1;
        uint32_t  dynamicTextureOwnerFlag: 1;
        uint32_t  captureTextureOverwriteDepthOffset: 3;
        uint32_t  dummyTextureFlag: 1;
    };

    Bits m_Bits;
    const TextureInfo* m_pTextureInfo;

    // コピーを禁止します。
    TexMap( const TexMap& );
    const TexMap& operator=( const TexMap& );

};

} // namespace nn::ui2d
} // namespace nn
