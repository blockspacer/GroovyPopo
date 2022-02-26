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

#include <cstddef>

#include <nn/util/util_IntrusiveList.h>

#include <nn/ui2d/ui2d_Util.h>
#include <nn/ui2d/ui2d_Common.h>
#include <nn/ui2d/ui2d_Types.h>

namespace nn
{
namespace ui2d
{
namespace detail
{

//---------------------------------------------------------------------------
//! @brief ペインの基底クラスに継承されるクラスです。
//!
//---------------------------------------------------------------------------
class PaneBase
{
    NN_DISALLOW_COPY(PaneBase);

public:
    PaneBase();
    virtual ~PaneBase();

public:

    //! @brief 内部用機能のため使用禁止です。
    nn::util::IntrusiveListNode m_Link;
};

class PaneEffect;

} // namespace nn::ui2d::detail

struct ResPane;
class ResExtUserData;
struct ResExtUserDataList;

class AnimTransform;
class AnimResource;
class Material;
class DrawInfo;
class Pane;
class CaptureTexture;

namespace detail
{
class VectorGraphicsTexture;
}

class StateMachine;

//! @brief ペインの一覧を保持するリストの定義です。
//!
typedef nn::util::IntrusiveList<Pane, nn::util::IntrusiveListMemberNodeTraits<detail::PaneBase, &detail::PaneBase::m_Link, Pane> > PaneList;

//---------------------------------------------------------------------------
//! @brief ペインの基底クラスです。
//!
//---------------------------------------------------------------------------
class Pane : public detail::PaneBase
{
public:
    //! 実行時型情報です。
    NN_UI2D_RUNTIME_TYPEINFO_ROOT();

    //----------------------------------------
    //! @name コンストラクタ／デストラクタ
    //@{

    //! @brief コンストラクタです。
    //!
    //! @details
    //! デフォルト設定の状態で初期化します。
    //!
    Pane();

    //! @brief コンストラクタです。
    //!
    //! @details
    //! データブロックの設定を反映した状態で初期化します。
    //! このコンストラクタではマスク機能のためのリソースを初期化しないため、マスク機能は使用できません。
    //! マスク機能を使用する場合は 4 引数版のコンストラクタを使用してください。
    //!
    //! @param[in] pBlock       ペインデータブロックへのポインタです。
    //! @param[in] buildArgSet  構築時の引数への参照です。
    //!
    Pane(const ResPane* pBlock, const BuildArgSet& buildArgSet);

    //! @brief コンストラクタです。
    //!
    //! @details
    //! データブロックの設定を反映した状態で初期化します。
    //!
    //! @param[out] pOutBuildResultInformation レイアウトデータを初期化した際に収集した統計情報です。
    //! @param[in] pDevice          デバイスです。
    //! @param[in] pBlock       ペインデータブロックへのポインタです。
    //! @param[in] buildArgSet  構築時の引数への参照です。
    //!
    Pane(BuildResultInformation* pOutBuildResultInformation, nn::gfx::Device* pDevice, const ResPane* pBlock, const BuildArgSet& buildArgSet);

    //! @brief コピーコンストラクタです。
    //!
    //! @details
    //! 引数で指定したペインをコピーしたペインを作成します。
    //!
    //! ペインの元々のレイアウトデータの状態ではなく、そのときの状態でコピーします
    //! のでご注意ください。つまり、アニメーションを再生した後はそのままの状態で
    //! コピーします。
    //!
    //! ただし、このメソッドではペインの親子関係はコピーしませんので、ご注意ください。
    //! つまり、ペインは親を持たず、子のリストが空の状態で作られます。
    //! ペインツリーに登録するためには、明示的にペインにAppendChildしてください。
    //!
    //! アニメーションについては、何もバインドされていない状態になります。
    //! SetMtxで設定した行列は設定解除されますので、必要であれば再度設定してください。
    //!
    //! このコンストラクタではキャプチャテクスチャやキャプチャ効果、ベクタグラフィックステクスチャに関する情報はコピーできません。
    //! これらの機能を使用したペインをコピーする場合は Pane::Pane(const Pane& pane, nn::gfx::Device* pDevice, Layout* pLayout) を使用してください。
    //!
    //! @param[in] pane コピー元のペインです。
    //!
    explicit Pane(const Pane& pane)
    {
        CopyImpl(pane, NULL, NULL, NULL);
    }

    //! @brief コピーコンストラクタです。
    //!
    //! @details
    //! 引数で指定したペインをコピーしたペインを作成します。
    //!
    //! ペインの元々のレイアウトデータの状態ではなく、そのときの状態でコピーします
    //! のでご注意ください。つまり、アニメーションを再生した後はそのままの状態で
    //! コピーします。
    //!
    //! ただし、このメソッドではペインの親子関係はコピーしませんので、ご注意ください。
    //! つまり、ペインは親を持たず、子のリストが空の状態で作られます。
    //! ペインツリーに登録するためには、明示的にペインにAppendChildしてください。
    //!
    //! アニメーションについては、何もバインドされていない状態になります。
    //! SetMtxで設定した行列は設定解除されますので、必要であれば再度設定してください。
    //!
    //! このコンストラクタではキャプチャテクスチャやキャプチャ効果、ベクタグラフィックステクスチャに関する情報はコピーできません。
    //! これらの機能を使用したペインをコピーする場合は Pane::Pane(const Pane& pane, nn::gfx::Device* pDevice, Layout* pLayout) を使用してください。
    //!
    //! @param[in] pane コピー元のペインです。
    //! @param[in] pDevice デバイスです。
    //! @param[in] pResAccessor コピー時にキャプチャテクスチャを参照するためのリソースアクセッサです。
    //! @param[in] pNewRootName コピーされたキャプチャテクスチャの新しいルート文字列です。
    //!
    //! @deprecated ui2d ランタイムで動的レンダリングテクスチャリソース管理方法が変更されたため、このコピーコンストラクタは非推奨となりました。
    //!             動的レンダリングテクスチャ関連機能を使用したペインをコピーする場合は Pane::Pane(const Pane& pane, nn::gfx::Device* pDevice, Layout* pLayout) を使用してください。
    //!
    NN_DEPRECATED explicit Pane(const Pane& pane, nn::gfx::Device* pDevice, ResourceAccessor* pResAccessor, const char* pNewRootName)
    {
        NN_UNUSED(pResAccessor);
        NN_UNUSED(pNewRootName);
        CopyImpl(pane, pDevice, NULL, NULL);
    }

    //! @brief コピーコンストラクタです。
    //!
    //! @details
    //! 引数で指定したペインをコピーしたペインを作成します。
    //!
    //! ペインの元々のレイアウトデータの状態ではなく、そのときの状態でコピーします
    //! のでご注意ください。つまり、アニメーションを再生した後はそのままの状態で
    //! コピーします。
    //!
    //! ただし、このメソッドではペインの親子関係はコピーしませんので、ご注意ください。
    //! つまり、ペインは親を持たず、子のリストが空の状態で作られます。
    //! ペインツリーに登録するためには、明示的にペインにAppendChildしてください。
    //!
    //! アニメーションについては、何もバインドされていない状態になります。
    //! SetMtxで設定した行列は設定解除されますので、必要であれば再度設定してください。
    //!
    //! このコンストラクタではキャプチャテクスチャやキャプチャ効果、ベクタグラフィックステクスチャに関する情報はコピーできません。
    //! これらの機能を使用したペインをコピーする場合は Pane::Pane(const Pane& pane, nn::gfx::Device* pDevice, Layout* pLayout) を使用してください。
    //!
    //! @param[in] pane コピー元のペインです。
    //! @param[in] pDevice デバイスです。
    //! @param[in] pResAccessor コピー時にキャプチャテクスチャを参照するためのリソースアクセッサです。
    //! @param[in] pNewRootName コピーされたキャプチャテクスチャの新しいルート文字列です。
    //! @param[in] pLayout このペインを管理しているレイアウトです。
    //!
    //! @deprecated ui2d ランタイムで動的レンダリングテクスチャリソース管理方法が変更されたため、このコピーコンストラクタは非推奨となりました。
    //!             動的レンダリングテクスチャ関連機能を使用したペインをコピーする場合は Pane::Pane(const Pane& pane, nn::gfx::Device* pDevice, Layout* pLayout) を使用してください。
    //!
    NN_DEPRECATED explicit Pane(const Pane& pane, nn::gfx::Device* pDevice, ResourceAccessor* pResAccessor, const char* pNewRootName, const Layout* pLayout)
    {
        NN_UNUSED(pResAccessor);
        NN_UNUSED(pNewRootName);
        CopyImpl(pane, pDevice, pLayout, NULL);
    }

    //! @brief コピーコンストラクタです。
    //!
    //! @details
    //! 引数で指定したペインをコピーしたペインを作成します。
    //!
    //! ペインの元々のレイアウトデータの状態ではなく、そのときの状態でコピーします
    //! のでご注意ください。つまり、アニメーションを再生した後はそのままの状態で
    //! コピーします。
    //!
    //! ただし、このメソッドではペインの親子関係はコピーしませんので、ご注意ください。
    //! つまり、ペインは親を持たず、子のリストが空の状態で作られます。
    //! ペインツリーに登録するためには、明示的にペインにAppendChildしてください。
    //!
    //! アニメーションについては、何もバインドされていない状態になります。
    //! SetMtxで設定した行列は設定解除されますので、必要であれば再度設定してください。
    //!
    //! キャプチャテクスチャやキャプチャ効果、ベクタグラフィックステクスチャなどの動的レンダリングテクスチャを使用している場合は
    //! このコピーコンストラクタを使用してコピーしてください。
    //! pLayout で指定したレイアウトクラスへ動的レンダリングテクスチャのリソースが登録されます。
    //!
    //! @param[in] pane コピー元のペインです。
    //! @param[in] pDevice デバイスです。
    //! @param[in] pLayout 動的レンダリングテクスチャリソースを登録するレイアウトインスタンス。通常、ペインの所属するレイアウトインスタンスを指定します。
    //!
    Pane(const Pane& pane, nn::gfx::Device* pDevice, Layout* pLayout);

    // ClonePaneTree から呼び出される内部用のコピーコンストラクタです。

    //! @brief 内部用機能のため使用禁止です。
    //!
    //! @details
    //! 引数で指定したペインをコピーしたペインを作成します。
    //!
    //! ペインの元々のレイアウトデータの状態ではなく、そのときの状態でコピーします
    //! のでご注意ください。つまり、アニメーションを再生した後はそのままの状態で
    //! コピーします。
    //!
    //! ただし、このメソッドではペインの親子関係はコピーしませんので、ご注意ください。
    //! つまり、ペインは親を持たず、子のリストが空の状態で作られます。
    //! ペインツリーに登録するためには、明示的にペインにAppendChildしてください。
    //!
    //! アニメーションについては、何もバインドされていない状態になります。
    //! SetMtxで設定した行列は設定解除されますので、必要であれば再度設定してください。
    //!
    //! @param[in] pane コピー元のペインです。
    //! @param[in] pDevice デバイスです。
    //! @param[in] pLayout このペインを管理しているレイアウトです。
    //! @param[in] pBuildPaneTreeContext 動的レンダリングテクスチャの共有情報や初期化情報などランタイム内部で使用する情報をペインツリーをまたいで受け渡しするためのデータです。
    //!
    Pane(const Pane& pane, nn::gfx::Device* pDevice, const Layout* pLayout, detail::BuildPaneTreeContext* pBuildPaneTreeContext)
    {
        CopyImpl(pane, pDevice, pLayout, pBuildPaneTreeContext);
    }

    //! @brief デストラクタです。
    //!
    virtual ~Pane();

    //! @brief リソースを開放します。
    //!
    //! @details
    //! デストラクタの呼び出し以前に呼び出す必要があります。
    //!
    //! @param[in] pDevice          デバイスです。
    //!
    virtual void Finalize(nn::gfx::Device* pDevice);

    //@}

    //----------------------------------------
    //! @name 設定／取得
    //@{

    //! @brief ペインの名前を取得します。
    //!
    //! @return ペインの名前を返します。
    //!
    //! @sa SetName
    //!
    const char* GetName() const
    {
        return m_Name;
    }

    //! @brief ペインの名前を設定します。
    //!
    //! @details
    //! セットできるペイン名文字列の最大長は nn::ui2d::ResourceNameStrMax です。
    //!
    //! @param[in] pName 名前です。
    //!
    //! @sa GetName
    //!
    void SetName(const char* pName);

    //! @brief ユーザー情報を取得します。
    //!
    //! @return ユーザー情報を返します。
    //!
    //! @sa SetUserData
    //!
    const char* GetUserData() const
    {
        return m_UserData;
    }

    //! @brief ユーザー情報を設定します。
    //!
    //! @details
    //! セットできるユーザー情報文字列の最大長は nn::ui2d::UserDataStrMax です。
    //!
    //! @param[in] pUserData ユーザー情報です。
    //!
    //! @sa GetUserData
    //!
    void SetUserData(const char* pUserData);

    //! @brief 親のアルファ値の変化が子に影響するかどうかを判定します。
    //!
    //! @return 親のアルファ値の変化が子に影響する場合は true を返します。
    //!
    //! @sa SetInfluencedAlpha
    //!
    bool IsInfluencedAlpha() const
    {
        return detail::TestBit(m_Flag, PaneFlag_InfluencedAlpha);
    }

    //! @brief 親のアルファ値の変化が子に影響するかどうかを設定します。
    //!
    //! @param[in] bAlpha   親のアルファ値の変化が子に影響する場合には true を指定します。
    //!
    //! @sa IsInfluencedAlpha
    //!
    void SetInfluencedAlpha(bool bAlpha)
    {
        detail::SetBit(&m_Flag, PaneFlag_InfluencedAlpha, bAlpha);
    }

    //! @brief 位置調整処理の対象となるかどうかを判定します。
    //!
    //! @return 位置調整処理の対象となる場合は true を返します。
    //!
    //! @sa SetLocationAdjust
    //!
    bool IsLocationAdjust() const
    {
        return detail::TestBit(m_Flag, PaneFlag_LocationAdjust);
    }

    //! @brief 位置調整処理の対象となるかどうかを設定します。
    //!
    //! @param[in] bAdjust  位置調整処理の対象となる場合には true を指定します。
    //!
    //! @sa IsLocationAdjust
    //! @sa DrawInfo::SetLocationAdjust
    //!
    void SetLocationAdjust(bool bAdjust)
    {
        detail::SetBit(&m_Flag, PaneFlag_LocationAdjust, bAdjust);
        this->SetGlbMtxDirty();
    }

    //! @brief ペインの表示位置を取得します。
    //!
    //! @return ペインの表示位置を返します。
    //!
    //! @sa SetTranslate
    //!
    const nn::util::Float3& GetTranslate() const
    {
        return m_Translate;
    }

    //! @brief ペインの表示位置を設定します。
    //!
    //! @param[in] value    ペインの表示位置です。
    //!
    //! @sa GetTranslate
    //!
    void SetTranslate(const nn::util::Float3& value)
    {
        m_Translate = value;
        this->SetGlbMtxDirty();
    }

    //! @brief ペインの表示位置を設定します。
    //!
    //! @details
    //! Z 成分は 0 に設定されます。
    //!
    //! @param[in] value    ペインの表示位置です。
    //!
    //! @sa GetTranslate
    //!
    void SetTranslate(const nn::util::Float2& value)
    {
        const nn::util::Float3 vector = NN_UTIL_FLOAT_3_INITIALIZER(value.v[0], value.v[1], 0.f);
        SetTranslate(vector);
    }

    //! @brief ペインの回転を取得します。
    //!
    //! @return X, Y, Z軸まわりの回転角度(Degree)を格納したベクトルを返します。
    //!
    //! @sa SetRotate
    //!
    const nn::util::Float3& GetRotate() const
    {
        return m_Rotate;
    }

    //! @brief ペインの回転を設定します。
    //!
    //! @param[in] value    X, Y, Z軸まわりの回転角度(Degree)を格納したベクトルです。
    //!
    //! @sa GetRotate
    //!
    void SetRotate(const nn::util::Float3& value)
    {
        m_Rotate = value;
        this->SetGlbMtxDirty();
    }

    //! @brief ペインの拡大率を取得します。
    //!
    //! @return ペインの拡大率を返します。
    //!
    //! @sa SetScale
    //!
    const nn::util::Float2& GetScale() const
    {
        return m_Scale;
    }

    //! @brief ペインの拡大率を設定します。
    //!
    //! @param[in] value    ペインの拡大率です。
    //!
    //! @sa GetScale
    //!
    void SetScale(const nn::util::Float2& value)
    {
        m_Scale = value;
        this->SetGlbMtxDirty();
    }

    //! @brief ペインのサイズを取得します。
    //!
    //! @return ペインのサイズを返します。
    //!
    //! @sa SetSize
    //!
    const Size& GetSize() const
    {
        return m_Size;
    }

    //! @brief ペインのサイズを設定します。
    //!
    //! @param[in] value    ペインのサイズです。
    //!
    //! @sa GetSize
    //!
    void SetSize(const Size& value)
    {
        m_Size = value;
        this->SetGlbMtxDirty();
    }

    //! @brief ペインの左上の座標を取得します。
    //!
    //! @return 座標を返します。
    //!
    const nn::util::Float2 GetVertexPos() const;

    //! @brief ペインの基準位置設定の水平位置指定を取得します。
    //!
    //! @details
    //! 水平位置指定の値については nn::ui2d::HorizontalPosition を参照してください。
    //!
    //! @return 水平位置指定の値を返します。
    //!
    //! @sa SetBasePositionX
    //! @sa GetBasePositionY
    //! @sa nn::ui2d::HorizontalPosition
    //!
    uint8_t  GetBasePositionX() const
    {
        return static_cast<uint8_t >(detail::GetHorizontalPosition(m_BasePosition));
    }

    //! @brief ペインの基準位置設定の水平位置指定を設定します。
    //!
    //! @param[in] val  ペインの基準位置指定です。 nn::ui2d::HorizontalPosition を指定します。
    //!
    //! @sa GetBasePositionX
    //! @sa SetBasePositionY
    //! @sa nn::ui2d::HorizontalPosition
    //!
    void SetBasePositionX(uint8_t  val)
    {
        detail::SetHorizontalPosition(&m_BasePosition, val);
    }

    //! @brief ペインの基準位置設定の垂直位置指定を取得します。
    //!
    //! @details
    //! 垂直位置指定の値については nn::ui2d::VerticalPosition を参照してください。
    //!
    //! @return 垂直位置指定の値を返します。
    //!
    //! @sa SetBasePositionY
    //! @sa GetBasePositionX
    //! @sa nn::ui2d::VerticalPosition
    //!
    uint8_t  GetBasePositionY() const
    {
        return static_cast<uint8_t >(detail::GetVerticalPosition(m_BasePosition));
    }

    //! @brief ペインの基準位置設定の垂直位置指定を設定します。
    //!
    //! @param[in] val  ペインの基準位置指定です。 nn::ui2d::VerticalPosition を指定します。
    //!
    //! @sa GetBasePositionY
    //! @sa SetBasePositionX
    //! @sa nn::ui2d::VerticalPosition
    //!
    void SetBasePositionY(uint8_t  val)
    {
        detail::SetVerticalPosition(&m_BasePosition, val);
    }

    //! @brief 親ペイン相対での原点位置の水平位置指定を取得します。
    //!
    //! @return 水平位置指定の値を返します。
    //!
    //! @sa SetParentRelativePositionX
    //! @sa GetParentRelativePositionY
    //! @sa nn::ui2d::HorizontalPosition
    //!
    uint8_t  GetParentRelativePositionX() const
    {
        return static_cast<uint8_t >((m_BasePosition >> 4) & 0x3);
    }

    //! @brief 親ペイン相対での原点位置の水平位置指定を設定します。
    //!
    //! @param[in] val  ペインの基準位置指定です。 nn::ui2d::HorizontalPosition を指定します。
    //!
    //! @sa GetParentRelativePositionX
    //! @sa SetParentRelativePositionY
    //! @sa nn::ui2d::HorizontalPosition
    //!
    void SetParentRelativePositionX(uint8_t  val)
    {
        NN_SDK_ASSERT(val < HorizontalPosition_MaxHorizontalPosition, "out of bounds: val[%u] < HorizontalPosition_MaxHorizontalPosition[%u] for Pane[%s]", val, HorizontalPosition_MaxHorizontalPosition, GetName());
        m_BasePosition = (m_BasePosition & (~0x30)) | (val << 4);
    }

    //! @brief 親ペイン相対での原点位置の垂直位置指定を取得します。
    //!
    //! @return 垂直位置指定の値を返します。
    //!
    //! @sa SetParentRelativePositionY
    //! @sa GetParentRelativePositionX
    //! @sa nn::ui2d::VerticalPosition
    //!
    uint8_t  GetParentRelativePositionY() const
    {
        return static_cast<uint8_t >((m_BasePosition >> 6) & 0x3);
    }

    //! @brief 親ペイン相対での原点位置の垂直位置指定を設定します。
    //!
    //! @param[in] val  ペインの基準位置指定です。 nn::ui2d::VerticalPosition を指定します。
    //!
    //! @sa GetParentRelativePositionY
    //! @sa SetParentRelativePositionX
    //! @sa nn::ui2d::VerticalPosition
    //!
    void SetParentRelativePositionY(uint8_t  val)
    {
        NN_SDK_ASSERT(val < VerticalPosition_MaxVerticalPosition, "out of bounds: val[%u] < VerticalPosition_MaxVerticalPosition[%u] for Pane[%s]", val, VerticalPosition_MaxVerticalPosition, GetName());
        m_BasePosition = (m_BasePosition & (~0xc0)) | (val << 6);
    }

    //! @brief ペインの Srt 行列を取得します。
    //!
    //! @details
    //! Srt 行列は通常、 Calculate() で計算されます。
    //!
    //! @return Srt行列です。
    //!
    //! @sa SetMtx
    //! @sa Calculate
    //!
    const nn::util::MatrixT4x3fType* GetMtx() const
    {
        return m_pUserMtx;
    }

    //! @brief ペインの Srt 行列を設定します。
    //!
    //! @details
    //! Srt 行列は通常は Calculate() で計算されます。
    //!
    //! この関数で行列を指定すると、設定した行列がそのまま使用されます。
    //!
    //! Calculate() による計算に戻すには ResetMtx() を使用します。
    //!
    //! SetGlobalMtx() を使用した場合には、そちらの設定が優先されます。
    //!
    //! Paneは与えた行列のポインタを保持しますので、ResetMtxで解除するまで
    //! 与えた行列は静的に保持するようにしてください。
    //!
    //! @param[in] pMtx  Srt 行列のポインタです。
    //!
    //! @sa GetMtx
    //! @sa Calculate
    //! @sa ResetMtx
    //! @sa SetGlobalMtx
    //!
    void SetMtx(const nn::util::MatrixT4x3fType* pMtx)
    {
        m_pUserMtx = pMtx;
        m_Flag = detail::SetBit(m_Flag, PaneFlag_UserMatrix, true);
    }

    //! @brief ペインの Srt 行列がユーザによって設定されているか調べます。
    //!
    //! @return
    //! Srt 行列がユーザによって設定されている場合は true を返します。
    //!
    //! @sa SetMtx
    //!
    bool IsUserMtx() const
    {
        return detail::TestBit(m_Flag, PaneFlag_UserMatrix);
    }

    //! @brief Srt 行列の設定を解除します。
    //!
    //! @details
    //! Srt 行列は Calculate() で計算されます。
    //!
    //! @sa SetMtx
    //!
    void ResetMtx()
    {
        m_Flag = detail::SetBit(m_Flag, PaneFlag_UserMatrix, false);
    }

    //! @brief 親子での計算後の Srt 行列を取得します。
    //!
    //! @details
    //! Srt 行列は通常、 Calculate() で計算されます。
    //!
    //! なお、Calculateで計算した場合、この行列はペインのモデル行列にDrawInfoに
    //! SetViewMtxで設定したビュー行列がかかった状態（モデルビュー行列）になります
    //! ので、ご注意ください。
    //!
    //! @return 親子での計算後の Srt 行列を返します。
    //!
    //! @sa SetGlobalMtx
    //! @sa Calculate
    //!
    const nn::util::MatrixT4x3fType& GetGlobalMtx() const
    {
        return m_GlbMtx;
    }

    //! @brief グローバル行列を設定します。
    //!
    //! @details
    //! グローバル行列はペインのローカル座標系からビュー座標系への
    //! 変換行列です。
    //!
    //! グローバル行列は通常は Calculate() で計算されます。
    //!
    //! この関数で行列を指定すると、設定した行列がそのまま使用されます。
    //!
    //! Calculate() による計算に戻すには ResetGlobalMtx() を
    //! 使用します。
    //!
    //! @param[in] mtx  Srt 行列です。
    //!
    //! @sa GetGlobalMtx
    //! @sa ResetGlobalMtx
    //! @sa Calculate
    //!
    //! @date 2010/04/23 設定の状態を記憶するようにしました。
    void SetGlobalMtx(const nn::util::MatrixT4x3fType& mtx)
    {
        m_GlbMtx = mtx;
        m_Flag = detail::SetBit(m_Flag, PaneFlag_UserGlobalMatrix, true);
    }

    //! @brief グローバル行列がユーザによって設定されているか調べます。
    //!
    //! @return
    //! グローバル行列がユーザによって設定されている場合は true を返します。
    //!
    //! @sa SetGlobalMtx
    //!
    bool IsUserGlobalMtx() const
    {
        return detail::TestBit(m_Flag, PaneFlag_UserGlobalMatrix);
    }

    //! @brief グローバル行列の設定を解除します。
    //!
    //! @details
    //! グローバル行列は Calculate() で計算されます。
    //!
    //! @sa SetGlobalMtx
    //!
    void ResetGlobalMtx()
    {
        m_Flag = detail::SetBit(m_Flag, PaneFlag_UserGlobalMatrix, false);
    }

    //! @brief ペインのローカル座標系での矩形を取得します。
    //!
    //! @return 矩形を返します。
    //!
    const nn::font::Rectangle GetPaneRect() const;

    //! @brief 頂点カラーを取得します。
    //!
    //! @details
    //! 派生クラスで実装します。
    //!
    //! このクラスでは idx の指定にかかわらず nn::util::Unorm8x4 {255, 255, 255, 255} (白) を返します。
    //!
    //! @param[in] idx  インデックスです。 nn::ui2d::VertexColor を指定します。
    //!
    //! @return ペインの四隅の頂点カラーを返します。
    //!
    //! @sa SetVertexColor
    //! @sa GetVertexColorElement
    //! @sa nn::ui2d::VertexColor
    //!
    virtual const nn::util::Unorm8x4 GetVertexColor(int  idx) const;

    //! @brief 頂点カラーを設定します。
    //!
    //! @details
    //! idx の指定には nn::ui2d::VertexColor を使用してください。
    //!
    //! 派生クラスでオーバーライドされます。基底クラスではなにもしません。
    //!
    //! @param[in] idx      インデックスです。 nn::ui2d::VertexColor を指定します。
    //! @param[in] value    頂点カラーです。
    //!
    //! @sa GetVertexColor
    //! @sa SetVertexColorElement
    //! @sa nn::ui2d::VertexColor
    //!
    virtual void SetVertexColor(int  idx, const nn::util::Unorm8x4& value);

    //! @brief ペインのアルファ値を取得します。
    //!
    //! @return ペインのアルファ値を返します。
    //!
    //! @sa SetAlpha
    //! @sa GetGlobalAlpha
    //!
    uint8_t  GetAlpha() const
    {
        return m_Alpha;
    }

    //! @brief ペインのアルファ値を設定します。
    //!
    //! @param[in] alpha    アルファ値です。
    //!
    //! @sa GetAlpha
    //! @sa SetGlobalAlpha
    //!
    void SetAlpha(uint8_t  alpha)
    {
        m_Alpha = alpha;
    }

    //! @brief ペインのグローバルアルファ値を取得します。
    //!
    //! @details
    //! グローバルアルファ値は親ペインのアルファ値の影響を受けた
    //! 状態でのペインのアルファ値です。
    //!
    //! @return ペインのグローバルアルファ値を返します。
    //!
    //! @sa SetAlpha
    //! @sa GetAlpha
    //!
    uint8_t  GetGlobalAlpha() const
    {
        return m_GlbAlpha;
    }

    //! @brief ペインのグローバルアルファ値を設定します。
    //!
    //! @details
    //! グローバルアルファ値は親ペインのアルファ値の影響を受けた状態でのペインのアルファ値です。
    //!
    //! グローバルアルファ値は通常、 Calculate() で計算されます。
    //!
    //! @param[in] alpha    グローバルアルファ値です。
    //!
    //! @sa GetGlobalAlpha
    //! @sa SetAlpha
    //! @sa Calculate
    //!
    void SetGlobalAlpha(uint8_t  alpha)
    {
        m_GlbAlpha = alpha;
    }

    //! @brief インデックスを指定してペインの Srt 要素を取得します。
    //!
    //! @details
    //! idx で取得したいペインの表示位置、回転、拡大率、サイズの要素を指定します。
    //!
    //! @param[in] idx  インデックスです。 nn::ui2d::AnimTargetPane を指定します。
    //!
    //! @return 指定された要素の値を返します。
    //!
    //! @sa SetSrtElement
    //! @sa GetTranslate
    //! @sa GetRotate
    //! @sa GetScale
    //! @sa GetSize
    //! @sa nn::ui2d::AnimTargetPane
    //!
    float GetSrtElement(int  idx) const
    {
        NN_SDK_ASSERT(idx < AnimTargetPane_MaxAnimTargetPane, "out of bounds: idx[%u] < AnimTargetPane_MaxAnimTargetPane[%u] for Pane[%s]", idx, AnimTargetPane_MaxAnimTargetPane, GetName());

        const float* pSrts= &m_Translate.v[0];
        return pSrts[idx];
    }

    //! @brief インデックスを指定してペインの Srt 要素を設定します。
    //!
    //! @details
    //! ペインの表示位置、回転、拡大率、サイズの要素を設定します。
    //!
    //! @param[in] idx      インデックスです。 nn::ui2d::AnimTargetPane を指定します。
    //! @param[in] value    要素の値です。
    //!
    //! @sa GetSrtElement
    //! @sa SetTranslate
    //! @sa SetRotate
    //! @sa SetScale
    //! @sa SetSize
    //! @sa nn::ui2d::AnimTargetPane
    //!
    void SetSrtElement(int  idx, float value)
    {
        NN_SDK_ASSERT(idx < AnimTargetPane_MaxAnimTargetPane, "out of bounds: idx[%u] < AnimTargetPane_MaxAnimTargetPane[%u] for Pane[%s]", idx, AnimTargetPane_MaxAnimTargetPane, GetName());

        float* pSrts = &m_Translate.v[0];
        pSrts[idx] = value;
        this->SetGlbMtxDirty();
    }

    //! @brief ペインのアルファ値または頂点カラーの各カラー成分を取得します。
    //!
    //! @details
    //! ペインのアルファ値または四隅の頂点カラーの各成分を返します。
    //!
    //! このクラスでは idx の指定に nn::ui2d::AnimTargetPaneColor_PaneAlpha を指定した場合は
    //! ペインのアルファ値を返しますが、その他の指定では必ず 255 を返します。
    //!
    //! 他のカラー成分を持つ派生クラスでは本関数はオーバーライドされます。
    //!
    //! 一般的な利用用途では、GetVertexColor や GetAlpha の利用をお勧めします。
    //!
    //! @param[in] idx  インデックスです。 nn::ui2d::AnimTargetPaneColor の値を指定します。
    //!
    //! @return ペインのアルファ値または四隅の頂点カラーの各成分を返します。
    //!
    //! @sa SetColorElement
    //! @sa GetVertexColorElement
    //! @sa nn::ui2d::AnimTargetPaneColor
    //!
    virtual uint8_t  GetColorElement(int  idx) const;

    //! @brief ペインのアルファ値または頂点カラーの各カラー成分を設定します。
    //!
    //! @details
    //! 導出クラスでオーバライドされます。
    //!
    //! このクラスではペインのアルファ値のみ設定することができます。
    //!
    //! 一般的な利用用途では、SetVertexColor や SetAlpha の利用をお勧めします。
    //!
    //! @param[in] idx      インデックスです。 nn::ui2d::AnimTargetPaneColor を指定します。
    //! @param[in] value    アルファ値またはカラーの成分値です。
    //!
    //! @sa GetColorElement
    //! @sa SetVertexColorElement
    //! @sa nn::ui2d::AnimTargetPaneColor
    //!
    virtual void SetColorElement(int  idx, uint8_t  value);

    //! @brief 頂点カラーの各カラー成分を取得します。
    //!
    //! @details
    //! 派生クラスでオーバーライドされます。
    //! このクラスでは idx の指定にかかわらず 255 を返します。
    //!
    //! idx の指定には nn::ui2d::AnimTargetPaneColor を使用してください。
    //!
    //! @param[in] idx  インデックスです。 nn::ui2d::AnimTargetPaneColor の値を指定します。
    //!
    //! @return ペイン四隅の頂点カラーの各成分を返します。
    //!
    //! @sa GetColorElement
    //! @sa SetVertexColorElement
    //! @sa nn::ui2d::AnimTargetPaneColor
    //!
    virtual uint8_t  GetVertexColorElement(int  idx) const;

    //! @brief 頂点カラーの各カラー成分を設定します。
    //!
    //! @details
    //! 派生クラスでオーバーライドされます。
    //! このクラスでは何もしません。
    //!
    //! idx の指定には nn::ui2d::AnimTargetPaneColor を使用してください。
    //!
    //! @param[in] idx      インデックスです。 nn::ui2d::AnimTargetPaneColor の値を指定します。
    //! @param[in] value    カラーの成分値です。
    //!
    //! @sa GetVertexColorElement
    //! @sa SetVertexColor
    //! @sa nn::ui2d::AnimTargetPaneColor
    //!
    virtual void SetVertexColorElement(int  idx, uint8_t  value);

    //! @brief 成分を指定してマスクテクスチャ SRT のパラメータを取得します。
    //!
    //! @details
    //! マスクテクスチャSRTの表示位置、回転、拡大率の成分を指定して取得します。
    //!
    //! elementIdx の指定には nn::ui2d::AnimTargetMaskTexMtxSrt の値を使用してください。
    //!
    //! @param[in] elementIdx   成分のインデックスです。
    //!
    //! @return マスクテクスチャSRTの指定された成分を返します。
    //!
    //! @sa SetMaskTexSrtElement
    //! @sa nn::ui2d::AnimTargetMaskTexMtxSrt
    //!
    float GetMaskTexSrtElement(int  elementIdx) const
    {
        NN_SDK_ASSERT(IsMaskEnabled());
        NN_SDK_ASSERT(elementIdx < AnimTargetMaskTexSrt_MaxAnimTargetMask, "out of bounds: idx[%u] < AnimTargetMaskTexSrt_MaxAnimTargetMask[%u] for Pane[%s]", elementIdx, AnimTargetMaskTexSrt_MaxAnimTargetMask, GetName());

        const SystemDataMaskTexture* pMaskData = static_cast<const SystemDataMaskTexture*>(GetSystemExtDataByType(PaneSystemDataType_MaskTexture));

        const float *const srtAry = &(pMaskData->maskTexSrt.translate.x);
        return srtAry[elementIdx];
    }

    //! @brief 成分を指定してマスクテクスチャ SRT のパラメータを設定します。
    //!
    //! @details
    //! マスクテクスチャの表示位置、回転、拡大率の成分を指定して設定します。
    //!
    //! elementIdx の指定には nn::ui2d::AnimTargetMaskTexMtxSrt の値を使用してください。
    //!
    //! @param[in] elementIdx   成分のインデックスです。
    //! @param[in] value    成分の値です。
    //!
    //! @sa GetMaskTexSrtElement
    //! @sa nn::ui2d::AnimTargetMaskTexMtxSrt
    //!
    void SetMaskTexSrtElement(int  elementIdx, float value)
    {
        NN_SDK_ASSERT(elementIdx < AnimTargetMaskTexSrt_MaxAnimTargetMask, "out of bounds: idx[%u] < AnimTargetMaskTexSrt_MaxAnimTargetMask[%u] for Pane[%s]", elementIdx, AnimTargetMaskTexSrt_MaxAnimTargetMask, GetName());

        SystemDataMaskTexture* pMaskData = static_cast<SystemDataMaskTexture*>(GetSystemExtDataForModify(PaneSystemDataType_MaskTexture));

        if (pMaskData != NULL)
        {
            float *const srtAry = &(pMaskData->maskTexSrt.translate.x);

            srtAry[elementIdx] = value;
        }
    }

    //! @brief 成分を指定してドロップシャドウ関連のパラメータを取得します。
    //!
    //! @details
    //! 境界線、光彩、ドロップシャドウの各パラメータの成分を指定して取得します。
    //!
    //! elementIdx の指定には nn::ui2d::AnimTargetDropShadow の値を使用してください。
    //!
    //! @param[in] elementIdx   成分のインデックスです。
    //!
    //! @return ドロップシャドウの指定された成分を返します。
    //!
    //! @sa SetDropShadowElement
    //! @sa nn::ui2d::AnimTargetDropShadow
    //!
    float GetDropShadowElement(int  elementIdx) const
    {
        NN_SDK_ASSERT(IsMaskEnabled());
        NN_SDK_ASSERT(elementIdx < AnimTargetDropShadow_MaxAnimTargetDropShadow, "out of bounds: idx[%u] < AnimTargetDropShadow_MaxAnimTargetDropShadow[%u] for Pane[%s]", elementIdx, AnimTargetDropShadow_MaxAnimTargetDropShadow, GetName());

        const SystemDataDropShadow* pDropShadowData = static_cast<const SystemDataDropShadow*>(GetSystemExtDataByType(PaneSystemDataType_DropShadow));

        if (pDropShadowData != NULL)
        {
            const float *const paramAry = &(pDropShadowData->strokeSize);
            return paramAry[elementIdx];
        }

        return 0.0f;
    }


    //! @brief 成分を指定してドロップシャドウ関連のパラメータを設定します。
    //!
    //! @details
    //! 境界線、光彩、ドロップシャドウの各パラメータの成分を指定して設定します。
    //!
    //! elementIdx の指定には nn::ui2d::AnimTargetDropShadow の値を使用してください。
    //!
    //! @param[in] elementIdx   成分のインデックスです。
    //! @param[in] value    成分の値です。
    //!
    //! @sa GetDropShadowElement
    //! @sa nn::ui2d::AnimTargetDropShadow
    //!
    void SetDropShadowElement(int  elementIdx, float value)
    {
        NN_SDK_ASSERT(elementIdx < AnimTargetDropShadow_MaxAnimTargetDropShadow, "out of bounds: idx[%u] < AnimTargetDropShadow_MaxAnimTargetDropShadow[%u] for Pane[%s]", elementIdx, AnimTargetDropShadow_MaxAnimTargetDropShadow, GetName());

        SystemDataDropShadow* pDropShadowData = static_cast<SystemDataDropShadow*>(GetSystemExtDataForModify(PaneSystemDataType_DropShadow));

        if (pDropShadowData != NULL)
        {
            float *const paramAry = &(pDropShadowData->strokeSize);

            paramAry[elementIdx] = value;
        }
    }

    //! @brief 成分を指定して角丸関連のパラメータを取得します。
    //!
    //! @details
    //! 角丸の各パラメータの成分を指定して取得します。
    //!
    //! elementIdx の指定には nn::ui2d::AnimTargetProceduralShape の値を使用してください。
    //!
    //! @param[in] elementIdx   成分のインデックスです。
    //!
    //! @return 角丸パラメータの指定された成分を返します。
    //!
    //! @sa SetProceduralShapeElement
    //! @sa nn::ui2d::AnimTargetProceduralShape
    //!
    float GetProceduralShapeElement(int  elementIdx) const
    {
        NN_SDK_ASSERT(IsProceduralShapeEnabled());
        NN_SDK_ASSERT(elementIdx < AnimTargetProceduralShape_MaxAnimTargetProceduralShape, "out of bounds: idx[%u] < AnimTargetProceduralShape_MaxAnimTargetProceduralShape[%u] for Pane[%s]", elementIdx, AnimTargetProceduralShape_MaxAnimTargetProceduralShape, GetName());

        const SystemDataProceduralShape* pProceduralShapeData = static_cast<const SystemDataProceduralShape*>(GetSystemExtDataByType(PaneSystemDataType_ProceduralShape));

        if (pProceduralShapeData != NULL)
        {
            const float *const paramAry = &(pProceduralShapeData->exp[0]);
            return paramAry[elementIdx];
        }

        return 0.0f;
    }


    //! @brief 成分を指定して角丸関連のパラメータを設定します。
    //!
    //! @details
    //! 角丸の各パラメータの成分を指定して設定します。
    //!
    //! elementIdx の指定には nn::ui2d::AnimTargetProceduralShape の値を使用してください。
    //!
    //! @param[in] elementIdx   成分のインデックスです。
    //! @param[in] value    成分の値です。
    //!
    //! @sa GetProceduralShapeElement
    //! @sa nn::ui2d::AnimTargetProceduralShape
    //!
    void SetProceduralShapeElement(int  elementIdx, float value)
    {
        NN_SDK_ASSERT(elementIdx < AnimTargetProceduralShape_MaxAnimTargetProceduralShape, "out of bounds: idx[%u] < AnimTargetProceduralShape_MaxAnimTargetProceduralShape[%u] for Pane[%s]", elementIdx, AnimTargetProceduralShape_MaxAnimTargetProceduralShape, GetName());

        SystemDataProceduralShape* pProceduralShapeData = static_cast<SystemDataProceduralShape*>(GetSystemExtDataForModify(PaneSystemDataType_ProceduralShape));

        if (pProceduralShapeData != NULL)
        {
            float *const paramAry = &(pProceduralShapeData->exp[0]);

            paramAry[elementIdx] = value;
        }
    }

    //! @brief  ステートマシンを取得します。（内部用機能のため使用しないでください）
    //! @return ステートマシンのポインタを返します。
    StateMachine* GetStateMachine()
    {
        const SystemDataLayoutStateMachine* pState = static_cast<const SystemDataLayoutStateMachine*>(this->GetSystemExtDataByType(PaneSystemDataType_StateRuntimeInfo));
        return pState != nullptr ? pState->pStateMachine : nullptr;
    }

    //! @brief ペインのマテリアルを取得します。
    //!
    //! @details
    //! 複数のマテリアルを持つペインは最初のマテリアル (インデックスが0) を返します。
    //!
    //! マテリアルを持たない場合はNULLを返します。
    //!
    //! @return マテリアルへのポインタを返します。
    //!
    Material* GetMaterial() const;

    //! @brief ペインが持つマテリアルの数を取得します。
    //!
    //! @return マテリアルの数を返します。
    //!
    //! @sa GetMaterial
    //!
    virtual uint8_t  GetMaterialCount() const;

    //! @brief ペインのマテリアルを取得します。
    //!
    //! @details
    //! インデックスに対応するマテリアルが返されます。
    //!
    //! インデックスの意味は導出されたクラスによって異なります。
    //!
    //! インデックスは GetMaterialCount() が返す値よりも小さくなければなりません。
    //!
    //! マテリアルを持たない場合は NULL を返します。
    //!
    //! @param[in] idx  インデックスです。
    //!
    //! @return マテリアルへのポインタを返します。
    //!
    //! @sa GetMaterialCount
    //!
    virtual Material* GetMaterial(int  idx) const;

    //! @brief 拡張ユーザデータの個数を取得します。
    //!
    //! @return 拡張ユーザデータの個数を返します。
    //!
    //! @sa GetExtUserDataArray
    //!
    uint16_t  GetExtUserDataCount() const;

    //! @brief 拡張ユーザデータ配列を取得します。
    //!
    //! @return 拡張ユーザデータ配列の先頭要素のポインタを返します。
    //!
    //! @sa GetExtUserDataCount
    //!
    const ResExtUserData* GetExtUserDataArray() const;

    //! @brief 拡張ユーザデータを名前をもとに検索します。
    //!
    //! @param[in] pName 検索する拡張ユーザデータ名です。
    //!
    //! @return 見つかった場合は拡張ユーザデータへのポインタ、
    //! 見つからなかった場合は NULL を返します。
    //!
    const ResExtUserData* FindExtUserDataByName(const char* pName) const;

    // 拡張ユーザーデータのアニメーション用の領域を取得する

    //! @brief 内部用機能のため使用禁止です。
    //!
    //! @return 拡張ユーザデータ配列の先頭要素のポインタを返します。
    ResExtUserData* GetExtUserDataArrayForAnimation() const;

    // 拡張ユーザーデータのアニメーション用の領域を名前を元に検索します。

    //! @brief 内部用機能のため使用禁止です。
    //!
    //! @param[in] pName 検索する拡張ユーザデータ名です。
    //!
    //! @return 見つかった場合は拡張ユーザデータへのポインタ、
    //! 見つからなかった場合は NULL を返します。
    ResExtUserData* FindExtUserDataByNameForAnimation(const char* pName) const;

    //@}

    //----------------------------------------
    //! @name 親子関係
    //@{

    //! @brief 親ペインを取得します。
    //!
    //! @return
    //! 親ペインへのポインタを返します。親ペインが存在しない場合は NULL を返します。
    //!
    Pane* GetParent() { return m_pParent; }

    //! @brief 親ペインを取得します。
    //!
    //! @return 親ペインです。
    //!
    const Pane* GetParent() const { return m_pParent; }

    //! @brief 子ペインの一覧を取得します。
    //!
    //! @return 子ペインの一覧を返します。
    //!
    const PaneList& GetChildList() const
    {
        return m_ChildList;
    }

    //! @brief 子ペインの一覧を取得します。
    //!
    //! @return 子ペインの一覧を返します。
    //!
    PaneList& GetChildList()
    {
        return m_ChildList;
    }

    //! @brief ペインを子供ペインリストの末尾に追加します。
    //!
    //! @details
    //! このペインやこのペインを子ペインに持つペインを追加しないように注意してください。
    //!
    //! @param[in] pChild   追加するペインです。
    //!
    //! @sa PrependChild
    //! @sa InsertChild
    //! @sa RemoveChild
    //!
    void AppendChild(Pane* pChild);

    //! @brief ペインを子供ペインリストの先頭に追加します。
    //!
    //! @details
    //! このペインやこのペインを子ペインに持つペインを追加しないように注意してください。
    //!
    //! @param[in] pChild   追加するペインです。
    //!
    //! @sa AppendChild
    //! @sa InsertChild
    //! @sa RemoveChild
    //!
    void PrependChild(Pane* pChild);

    //! @brief 子ペインを指定した位置に挿入します。
    //!
    //! @details
    //! pChild で指定したペインをこのペインの子として追加します。
    //! pNext で指定したペインの前の位置に挿入します。
    //!
    //! このペインやこのペインを子ペインに持つペインを追加しないように注意してください。
    //!
    //! @param[in] pNext    挿入する位置の1つ後となるペインです。
    //! @param[in] pChild   追加するペインです。
    //!
    //! @sa AppendChild
    //! @sa PrependChild
    //! @sa RemoveChild
    //!
    void InsertChild(
        Pane* pNext,
        Pane* pChild);

    //! @brief 子ペインを指定した位置に挿入します。
    //!
    //! @details
    //! pChild で指定したペインをこのペインの子として追加します。
    //! next で指定したイテレータの前の位置に挿入します。
    //!
    //! このペインやこのペインを子ペインに持つペインを追加しないように注意してください。
    //!
    //! @param[in] next     挿入する位置の1つ後となるイテレータです。
    //! @param[in] pChild   追加するペインです。
    //!
    //! @sa AppendChild
    //! @sa PrependChild
    //! @sa RemoveChild
    //!
    void InsertChild(
        PaneList::iterator next,
        Pane* pChild);

    //! @brief ペインを子供ペインリストから削除します。
    //!
    //! @param[in] pChild   削除するペインです。
    //!
    //! @sa AppendChild
    //! @sa PrependChild
    //! @sa InsertChild
    //!
    void RemoveChild(Pane* pChild);

    //@}

    //----------------------------------------
    //! @name 検索
    //@{

    //! @brief 名前でペインを検索します。
    //!
    //! @details
    //! pFindName のペイン名を持つペインを検索します。
    //!
    //! bRecursive に true を渡した場合は子ペインと、
    //! 子ペインが子を持つ場合はさらにその子ペインからも検索します。
    //!
    //! @param[in] pFindName     検索するペイン名です。
    //! @param[in] bRecursive   子ペインも検索する場合は true を指定します。
    //!
    //! @return
    //! pFindName のペイン名を持つペインが見つかった場合はペインへのポインタを、
    //! 見つからなかった場合は NULL を返します。
    //!
    virtual Pane* FindPaneByName(const char* pFindName, bool bRecursive = true);
    //! @brief 名前でペインを検索します。
    //!
    //! @details
    //! pFindName のペイン名を持つペインを検索します。
    //!
    //! bRecursive に true を渡した場合は子ペインと、
    //! 子ペインが子を持つ場合はさらにその子ペインからも検索します。
    //!
    //! @param[in] pFindName     検索するペイン名です。
    //! @param[in] bRecursive   子ペインも検索する場合は true を指定します。
    //!
    //! @return
    //! pFindName のペイン名を持つペインが見つかった場合はペインへのポインタを、
    //! 見つからなかった場合は NULL を返します。
    //!
    virtual const Pane* FindPaneByName(const char* pFindName, bool bRecursive = true) const;

    //! @brief 名前でマテリアルを検索します。
    //!
    //! @details
    //! pFindName のマテリアル名を持つマテリアルを検索します。
    //!
    //! bRecursive に true を渡した場合は子ペインと、
    //! 子ペインが子を持つ場合はさらにその子ペインからも検索します。
    //!
    //! @param[in] pFindName     検索するマテリアル名です。
    //! @param[in] bRecursive   子ペインも検索する場合は true を指定します。
    //!
    //! @return
    //! pFindName のマテリアル名を持つマテリアルが見つかった場合は
    //! マテリアルへのポインタを、見つからなかった場合は NULL を返します。
    //!
    virtual Material* FindMaterialByName(const char* pFindName,bool bRecursive = true);
    //! @brief 名前でマテリアルを検索します。
    //!
    //! @details
    //! pFindName のマテリアル名を持つマテリアルを検索します。
    //!
    //! bRecursive に true を渡した場合は子ペインと、
    //! 子ペインが子を持つ場合はさらにその子ペインからも検索します。
    //!
    //! @param[in] pFindName     検索するマテリアル名です。
    //! @param[in] bRecursive   子ペインも検索する場合は true を指定します。
    //!
    //! @return
    //! pFindName のマテリアル名を持つマテリアルが見つかった場合は
    //! マテリアルへのポインタを、見つからなかった場合は NULL を返します。
    //!
    virtual const Material* FindMaterialByName(const char* pFindName, bool bRecursive = true) const;

    //@}

    //----------------------------------------
    //! @name アニメーション
    //@{

    //! @brief アニメーションを関連付けます。
    //!
    //! @details
    //! bRecursive に true を渡して呼び出した場合は、
    //! 関連付けるペインを子ペインからも検索します。
    //!
    //! bEnabled に false を渡して呼び出した場合は、
    //! アニメーションを無効状態で関連付けます。
    //! 有効にする場合は AnimTransform::SetEnabled() を使用してください。
    //!
    //! @param[in] pAnimTrans       アニメーションへのポインタです。
    //! @param[in] bRecursive       子ペインも検索する場合は true を指定します。
    //! @param[in] bEnabled         アニメーションを無効状態で関連付ける場合は false を指定します。
    //!
    //! @sa UnbindAnimation
    //! @sa UnbindAnimationSelf
    //! @sa AnimTransform::SetEnabled
    //!
    virtual void BindAnimation(
        AnimTransform* pAnimTrans,
        bool bRecursive = true,
        bool bEnabled = true);

    //! @brief アニメーションの関連付けを解除します。
    //!
    //! @param[in] pAnimTrans       アニメーションへのポインタです。
    //! @param[in] bRecursive       子ペインも検索する場合は true を指定します。
    //!
    //! @sa BindAnimation
    //! @sa UnbindAnimationSelf
    //!
    virtual void UnbindAnimation(
        AnimTransform* pAnimTrans,
        bool bRecursive = true);

    //! @brief 自身のアニメーションの関連付けを解除します。
    //!
    //! @param[in] pAnimTrans   アニメーションへのポインタです。
    //!
    //! @sa BindAnimation
    //! @sa UnbindAnimation
    //!
    virtual void UnbindAnimationSelf(AnimTransform* pAnimTrans);

    //@}

    //----------------------------------------
    //! @name 描画
    //@{

    //! @brief ペインが表示されるかどうかを判定します。
    //!
    //! @return ペインが表示される場合は true を、表示されない場合は false を返します。
    //!
    //! @sa SetVisible
    //!
    bool IsVisible() const
    {
        return detail::TestBit(m_Flag, PaneFlag_Visible);
    }

    //! @brief ペインの表示／非表示を設定します。
    //!
    //! @param[in] bVisible ペインを表示させる場合は true を指定します。
    //!
    //! @sa IsVisible
    //!
    void SetVisible(bool bVisible)
    {
        detail::SetBit(&m_Flag, PaneFlag_Visible, bVisible);
    }

#if ! defined(NN_SDK_BUILD_RELEASE)
    //! @brief ペインがビューア上で非表示かどうかを判定します。
    //!
    //! @details
    //! この設定は、Debugビルド及びDevelopmentビルドで有効です。
    //!
    //! @return ペインがビューア上で非表示の場合は true を、表示される場合は false を返します。
    //!
    bool IsViewerInvisible() const
    {
        return detail::TestBit(m_FlagEx, PaneFlagEx_ViewerInvisible);
    }
#endif

    //  コンスタントバッファの更新を行うかどうかを判定します。

    //! @brief 内部用機能のため使用禁止です。
    //!
    //! @return コンスタントバッファの作成と更新を行う場合は true を、しない場合は false を返します。
    //!
    bool IsConstantBufferUpdateNeeded() const;

    //! @brief Calculate を行う際のコンテキストです。
    //!
    struct CalculateContext
    {
        nn::font::RectDrawer* pRectDrawer;   //!< フォント描画クラス
        const nn::util::MatrixT4x3fType* pViewMtx;     //!< ビュー行列
        nn::util::Float2 locationAdjustScale; //!< 位置調整のスケール
        float influenceAlpha;           //!< 子に反映させるアルファ値
        bool isLocationAdjust;          //!< 位置調整を行うか否か
        bool isInvisiblePaneCalculateMtx;   //!< 非表示のペインで Calculate を行うか否か
        bool isAlphaZeroPaneCalculateMtx;   //!< アルファが0でかつ子階層にアルファの影響を与えるペインで Calculate を行うか否か
        bool isInfluenceAlpha;              //!< 子にアルファ値を反映させるか否か
        const Layout* pLayout; //!< レイアウト

        //! @brief デフォルト値に設定します。
        //!
        void SetDefault();

        //! @brief DrawInfoのパラメータを元に初期化します。
        //!
        //! @param[in] drawInfo 描画の情報
        //! @param[in] pLayoutData   レイアウト
        //!
        void Set(const DrawInfo& drawInfo, const Layout* pLayoutData = NULL);
    };

    typedef CalculateContext CalculateMtxContext;

    //! @brief 描画のための行列を計算します。
    //!
    //! @param[in] drawInfo         DrawInfoです。
    //! @param[in] context          描画情報と内部で使用する変数を含むコンテキストです。
    //! @param[in] isDirtyParentMtx 親の行列がDirtyか否かです。
    //!
    virtual void Calculate(DrawInfo& drawInfo, CalculateMtxContext& context, bool isDirtyParentMtx);

    //@}

    //! @brief ユーザーがペインを構築したかどうかを判定します。
    //!
    //! @return ペインをユーザーが構築した場合は true を返します。
    //!
    bool IsUserAllocated() const
    {
        return detail::TestBit(m_Flag, PaneFlag_UserAllocated);
    }

    //! @brief ユーザーが構築したペインであることを宣言します。
    //!
    //! @details
    //! ユーザが構築したと宣言されたペインはライブラリによって解放されません。
    //!
    void SetUserAllocated()
    {
        detail::SetBit(&m_Flag, PaneFlag_UserAllocated, true);
    }

    //! @brief 拡張ユーザデータリストを設定します。
    //!
    //! @param[in] pBlock   拡張ユーザデータリソースへのポインタです。
    //!
    //! @details
    //! 拡張ユーザデータのアニメーションを利用しているペインでは設定はできません。
    //!
    void SetExtUserDataList(const ResExtUserDataList* pBlock);

    // システム用ユーザー拡張データをランタイムで追加します。

    //! @brief 内部用機能のため使用禁止です。
    //!
    //! @param[in] dataType 追加するシステムデータのタイプです。
    //! @param[in] pData    追加するシステムデータのバイナリデータです。
    //! @param[in] dataSize pData のサイズです。
    //!
    void AddDynamicSystemExtUserData(PaneSystemDataType dataType, void* pData, int dataSize);

    //! @brief ペイン自身と子ペインの描画処理を行います。
    //!
    //! @details
    //! Layout および親ペインから呼ばれます。
    //!
    //! DrawSelf() でペイン自身の描画を行い、次に子ペインの Draw() を
    //! 呼び出します。
    //!
    //! @param[in] drawInfo 描画情報です。
    //! @param[in] commandBuffer    コマンドバッファです。
    //!
    virtual void Draw(DrawInfo& drawInfo, nn::gfx::CommandBuffer& commandBuffer);

    //! @brief 子ペインの描画処理の呼び出しのみを行います。
    //!
    //! @details
    //! Layout および Draw() から呼ばれます。
    //!
    //! 子ペインの Draw() を呼び出します。
    //!
    //! @param[in] drawInfo 描画情報です。
    //! @param[in] commandBuffer    コマンドバッファです。
    //!
    void DrawChildren(DrawInfo& drawInfo, nn::gfx::CommandBuffer& commandBuffer);

    //! @brief 自身の描画処理を行います。
    //!
    //! @details
    //! Layout および親ペインから呼ばれます。
    //!
    //! @param[in] drawInfo 描画情報です。
    //! @param[in] commandBuffer    コマンドバッファです。
    //!
    virtual void DrawSelf(DrawInfo& drawInfo, nn::gfx::CommandBuffer& commandBuffer);

    //! @brief 自分自身を含めた配下の描画処理を行う準備ができているかどうか確認します。
    //!
    //! @return 描画準備ができている場合はtrue を返します。
    //!
    NN_FORCEINLINE bool IsDrawTreeReady() const
    {
        // Visible でない場合は描画をしない
        // Visible かつ、 IsConstantBufferReady を満たさない場合は描画をしない
        // PaneFlag_IsConstantBufferReady が false の場合、該当ペイン以下の子階層すべてが、false とみなして描画がスキップされます。
        return detail::TestBit(m_Flag, PaneFlag_Visible) && IsConstantBufferReady();
    }

    //! @brief 自分自身の描画処理を行う準備ができているかどうか確認します。
    //!
    //! @details
    //! このフラグと IsDrawTreeReady() が true の時に自分自身の描画処理が実行されます。
    //!
    //! @return 描画準備ができている場合はtrue を返します。
    //!
    NN_FORCEINLINE bool IsDrawSelfReady() const
    {
        // IsConstantBufferReady の場合は描画する
        return IsConstantBufferReadySelf();
    }

    //! @brief グローバル行列がDirtyか否かを返します。
    //!
    //! @return グローバル行列がDirtyか否か
    //!
    NN_FORCEINLINE bool IsGlbMtxDirty() const
    {
        return detail::TestBit(m_Flag, PaneFlag_IsGlobalMatrixDirty);
    }

    //! @brief 内部フラグのポインタを取得します。
    //!
    //! @details
    //! ペインの表示/非表示等のフラグを保持している内部フラグのポインタを取得します。
    //! このフラグを直接操作するのは非常に危険であるため、通常はSetVisible等のアクセサ
    //! を使用してください。このメソッドを使用するのは、ポインタを直接操作しなければ
    //! いけないようなケースのみにしてください。
    //!
    //! @return 内部フラグのポインタ
    //!
    uint8_t * GetFlagPtr()
    {
        return &m_Flag;
    }

    //! @brief アルファ値のポインタを取得します。
    //!
    //! @details
    //! ペインのアルファ値を保持しているメンバ変数のポインタを取得します。
    //! 通常はSetAlpha等のアクセサを使用してください。このメソッドを使用するのは、
    //! ポインタを直接操作しなければいけないようなケースのみにしてください。
    //!
    //! @return アルファ値のポインタ
    //!
    uint8_t * GetAlphaPtr()
    {
        return &m_Alpha;
    }

    //! @brief グローバルマトリックスを計算します。
    //!
    //! @details
    //! ペインを初期化した直後のグローバルマトリックスは単位行列が設定されていますが、
    //! このメソッドを初期化直後に呼び出すことで 0 フレーム目のグローバルマトリックスの状態にできます。
    //! アニメーションを再生したり SetTranslate 等で SRT を設定していた際には
    //! それらの SRT が反映された状態でグローバルマトリックスが計算されます。
    //!
    //! 本メソッドは、全てのグローバルマトリックスが再計算されます。
    //! 変更があったグローバルマトリックスだけを計算したい場合には同名のオーバーロード関数を使用してください。
    //!
    //! @param[in] context 描画情報と内部で使用する変数を含むコンテキストです。
    //!
    void CalculateGlobalMatrix(Pane::CalculateContext& context)
    {
        CalculateGlobalMatrix(context, true);
    }

    //! @brief グローバルマトリックスを計算します。
    //!
    //! @details
    //! ペインを初期化した直後のグローバルマトリックスは単位行列が設定されていますが、
    //! このメソッドを初期化直後に呼び出すことで 0 フレーム目のグローバルマトリックスの状態にできます。
    //! アニメーションを再生したり SetTranslate 等で SRT を設定していた際には
    //! それらの SRT が反映された状態でグローバルマトリックスが計算されます。
    //!
    //! @param[in] context 描画情報と内部で使用する変数を含むコンテキストです。
    //! @param[in] isDirtyParentMtx 親の行列がDirtyか否かです。
    //!
    void CalculateGlobalMatrix(Pane::CalculateContext& context, bool isDirtyParentMtx);


    //----------------------------------------
    //! @name ペインエフェクト関連機能
    //@{

    //! @brief  ペインエフェクト機能が有効かどうか取得します。
    //!
    //! @return ペインエフェクト機能が有効なら true、無効なら false を返します。
    bool IsPaneEffectEnabled() const
    {
        return detail::TestBit(GetSystemExtDataFlag(), PaneSystemDataType_PaneEffect);
    }

    //  ペインエフェクト処理のためのインスタンスを取得します。

    //! @brief 内部用機能のため使用禁止です。
    //!
    //! @return ペインエフェクト機能が有効ならば内部で使用しているペインエフェクトインスタンスを返します。
    //!
    detail::PaneEffect* GetPaneEffectInstance() const;

    // マスクデータが存在すれば取得します。

    //! @brief 内部用機能のため使用禁止です。
    //!
    //! @return マスクデータが存在すれば SystemDataMaskTexture のポインタを返します。存在しなければ NULL を返します。
    const SystemDataMaskTexture* GetMaskData() const
    {
        return static_cast<const SystemDataMaskTexture*>(GetSystemExtDataByType(PaneSystemDataType_MaskTexture));
    }

    // ドロップシャドウデータが存在すれば取得します。

    //! @brief 内部用機能のため使用禁止です。
    //!
    //! @return ドロップシャドウデータが存在すれば SystemDataDropShadow のポインタを返します。存在しなければ NULL を返します。
    const SystemDataDropShadow* GetDropShadowData() const
    {
        return static_cast<const SystemDataDropShadow*>(GetSystemExtDataByType(PaneSystemDataType_DropShadow));
    }

    //! @brief  マスク機能が有効かどうか取得します。
    //!
    //! @return マスク機能が有効なら true、無効なら false を返します。
    bool IsMaskEnabled() const;

    //! @brief  ドロップシャドウ機能が有効かどうか取得します。
    //!
    //! @return ドロップシャドウ機能が有効なら true、無効なら false を返します。
    bool IsDropShadowEnabled() const;


    //! @brief  ペインエフェクトのキャプチャ結果を最終的に描画する際に描画状態を設定します。
    //!
    //! @details
    //! ペインエフェクトを使用した際は最終的にキャプチャされたペインのテクスチャが画面にレンダリングされます。
    //! このメソッドではその描画の際の描画設定を行います。
    //!
    //! @param[in] commandBuffer    コマンドバッファです。
    //!
    virtual void SetupPaneEffectSourceImageRenderState(nn::gfx::CommandBuffer& commandBuffer) const
    {
        NN_UNUSED(commandBuffer);
    }

    //@}

    //----------------------------------------
    //! @name 角丸関連機能
    //@{

    //! @brief  角丸機能が有効かどうか取得します。
    //!
    //! @return 角丸機能が有効なら true、無効なら false を返します。
    //!
    bool IsProceduralShapeEnabled() const
    {
        return detail::TestBit(GetSystemExtDataFlag(), PaneSystemDataType_ProceduralShapeRuntimeInfo);
    }

    //@}

    //! @brief 内部用機能のため使用禁止です。
    //!
    //! @return 存在すれば SystemDataScreen のポインタを返します。存在しなければ NULL を返します。
    const SystemDataScreen* GetScreenData() const
    {
        return static_cast<const SystemDataScreen*>(GetSystemExtDataByType(PaneSystemDataType_Screen));
    }

    //! @brief コピーコンストラクタでコピーされたインスタンスが正しくコピーされているか確認します。
    //!
    //! @details
    //! このメソッドは ui2d ランタイムのテストのために作成されたメソッドです。
    //! アプリケーションコードでの使用は非推奨となります。
    //!
    //! @param[in]  target  比較対象のペインです。
    //!
    //! @return 同等とみなせる場合は true を返します。
    //!
    bool CompareCopiedInstanceTest(const Pane& target) const;

    //! @brief 内部用機能のため使用禁止です。
    //!
    //! @details
    //! このメソッドは整列ペインの配下にあるペインの場合のみ正しく機能します。
    //!
    //! @return 整列ペイン配下時に整列処理から除外する場合は true を返します。
    //!
    bool IsAlignmentIgnore();

    //! @brief 内部用機能のため使用禁止です。
    //!
    //! @details
    //! このメソッドは整列ペインの配下にあるペインの場合のみ正しく機能します。
    //!
    //! @return 整列ペイン配下時に整列マージンが指定されている場合は true を返します。
    //!
    bool IsAlignmentMarginEnabled();

    //! @brief 内部用機能のため使用禁止です。
    //!
    //! @details
    //! このメソッドは整列ペインの配下にあるペインの場合のみ正しく機能します。
    //!
    //! @return 整列ペイン配下時に NULL ペインの場合は true を返します。
    //!
    bool IsAlignmentNullPane();

    //! @brief 内部用機能のため使用禁止です。
    //!
    //! @details
    //! このメソッドは整列ペインの配下にあるペインの場合のみ正しく機能します。
    //!
    //! @return 整列ペイン配下時に整列マージンが有効な場合に値を得ます。
    //!
    //! @sa IsAlignmentMarginEnabled
    //!
    float GetAlignmentMargin();

    // ペインの行列を計算します。

    //! @brief 内部用機能のため使用禁止です。
    //!
    //! @param[in] context 描画情報と内部で使用する変数を含むコンテキストです。
    //!
    void CalculateGlobalMatrixSelf(CalculateContext& context);
protected:

    //! @brief コピーコンストラクタの実装です。
    //!
    //! @param[in] pane コピー元のペインです。
    //! @param[in] pDevice デバイスです。
    //! @param[in] pResAccessor コピー時にキャプチャテクスチャを参照するためのリソースアクセッサです。
    //! @param[in] pNewRootName コピーされたキャプチャテクスチャの新しいルート文字列です。
    //! @param[in] pLayout このペインを管理しているレイアウトです。
    //!
    //! @deprecated ui2d ランタイムで動的レンダリングテクスチャリソース管理方法が変更されたため、このコピー実装は非推奨となりました。
    //!             動的レンダリングテクスチャ関連機能を使用したペインをコピーする場合は Pane::CopyImpl(const Pane& pane, nn::gfx::Device* pDevice, const Layout* pLayout, detail::BuildPaneTreeContext* pBuildPaneTreeContext) を使用してください。
    //!
    NN_DEPRECATED void CopyImpl(const Pane& pane, nn::gfx::Device* pDevice, ResourceAccessor* pResAccessor, const char* pNewRootName, const Layout* pLayout);

    //! @brief コピーコンストラクタの実装です。
    //!
    //! @param[in] pane コピー元のペインです。
    //! @param[in] pDevice デバイスです。
    //! @param[in] pLayout このペインを管理しているレイアウトです。
    //! @param[in] pBuildPaneTreeContext ペイン間のキャプチャテクスチャの共有など、ペインやパーツ階層をまたいで受け渡す必要のあるデータを保持します。
    //!
    void CopyImpl(const Pane& pane, nn::gfx::Device* pDevice, const Layout* pLayout, detail::BuildPaneTreeContext* pBuildPaneTreeContext);

    //! @brief コンストラクタの処理を共通化したものです。
    //!
    //! @details
    //! データブロックの設定を反映した状態で初期化します。
    //!
    //! @param[out] pOutBuildResultInformation レイアウトデータを初期化した際に収集した統計情報です。
    //! @param[in] pDevice          デバイスです。
    //! @param[in] pBlock       ペインデータブロックへのポインタです。
    //! @param[in] buildArgSet  構築時の引数への参照です。
    //!
    void InitializeByResourceBlock(BuildResultInformation* pOutBuildResultInformation, nn::gfx::Device* pDevice, const ResPane* pBlock, const BuildArgSet& buildArgSet);

    //! @brief モデルビュー行列を計算し DrawInfo に設定します。
    //!
    //! @param[in] drawInfo 描画情報です。
    //!
    virtual void LoadMtx(DrawInfo& drawInfo);

    //! @brief グローバル行列をDirtyにします。
    //!
    NN_FORCEINLINE void SetGlbMtxDirty()
    {
        detail::SetBit(&m_Flag, PaneFlag_IsGlobalMatrixDirty, true);
    }

    //! @brief グローバル行列のDirty状態を解除します。
    //!
    NN_FORCEINLINE void CleanGlbMtx()
    {
        detail::SetBit(&m_Flag, PaneFlag_IsGlobalMatrixDirty, false);
    }

    //! @brief 配下の子供ペインも含め、コンスタントバッファが準備できているかどうかを設定します。
    //!
    //! @param[in] flag 準備できているかどうかのフラグです。
    //!
    NN_FORCEINLINE void SetConstantBufferReady(bool flag)
    {
        detail::SetBit(&m_Flag, PaneFlag_IsConstantBufferReady, flag);
    }

    //! @brief 配下の子供ペインも含め、コンスタントバッファが準備できているかどうかを取得します。
    //!
    //! @return コンスタントバッファが準備できていれば true を返します。
    //!
    NN_FORCEINLINE bool IsConstantBufferReady() const
    {
        return detail::TestBit(m_Flag, PaneFlag_IsConstantBufferReady);
    }

    //! @brief 配下の子供ペインも含まず、自身のみのコンスタントバッファが準備できているかどうかを設定します。
    //!
    //! @param[in] flag 準備できているかどうかのフラグです。
    //!
    NN_FORCEINLINE void SetConstantBufferReadySelf(bool flag)
    {
        detail::SetBit(&m_FlagEx, PaneFlagEx_IsConstantBufferReadySelf, flag);
    }

    //! @brief 配下の子供ペインも含まず、自身のみのコンスタントバッファが準備できているかどうかを取得します。
    //!
    //! @return コンスタントバッファが準備できていれば true を返します。
    //!
    NN_FORCEINLINE bool IsConstantBufferReadySelf() const
    {
        return detail::TestBit(m_FlagEx, PaneFlagEx_IsConstantBufferReadySelf);
    }

#if !defined(NN_SDK_BUILD_RELEASE)
    //! @brief Calculate が完了し、Draw 待ちの状態であるか否かを設定します。
    //!
    //! @param[in] flag 準備できているかどうかのフラグです。
    //!
    NN_FORCEINLINE void SetCalculationFinishedSelf(bool flag)
    {
        detail::SetBit(&m_FlagEx, PaneFlagEx_IsCalculationFinishedSelf, flag);
    }

    //! @brief Calculate が完了し、Draw 待ちの状態であるか否かを取得します。
    //!
    //! @return Calculate が完了し、Draw 待ちの状態であれば true を返します。
    //!
    NN_FORCEINLINE bool IsCalculationFinishedSelf() const
    {
        return detail::TestBit(m_FlagEx, PaneFlagEx_IsCalculationFinishedSelf);
    }
#endif
    //! FindPaneByNameが再帰的に呼ばれる場合に使用します。
    //!
    //! @param[in] pFindName 名前です。
    //!
    //! @return ペインです。
    //!
    virtual Pane* FindPaneByNameRecursive(const char* pFindName);
    //! FindPaneByNameが再帰的に呼ばれる場合に使用します。
    //!
    //! @param[in] pFindName 名前です。
    //!
    //! @return ペインです。
    //!
    virtual const Pane* FindPaneByNameRecursive(const char* pFindName) const;

    //! FindMaterialByNameが再帰的に呼ばれる場合に使用します。
    //!
    //! @param[in] pFindName 名前です。
    //!
    //! @return マテリアルです。
    //!
    virtual Material* FindMaterialByNameRecursive(const char* pFindName);
    //! FindMaterialByNameが再帰的に呼ばれる場合に使用します。
    //!
    //! @param[in] pFindName 名前です。
    //!
    //! @return マテリアルです。
    //!
    virtual const Material* FindMaterialByNameRecursive(const char* pFindName) const;

    // システム用拡張ユーザーデータに関するフラグを取得します。

    //! @brief 内部用機能のため使用禁止です。
    //!
    //! @return フラグです。
    //!
    uint32_t GetSystemExtDataFlag() const
    {
        return m_SystemDataFlags;
    }

    // システム用拡張ユーザーデータの数を取得します。

    //! @brief 内部用機能のため使用禁止です。
    //!
    //! @return システム用拡張ユーザーデータの数。
    //!
    uint32_t GetSystemExtDataCount() const
    {
        if (GetSystemExtDataFlag() != 0)
        {
            const ResExtUserData* pData = nn::util::ConstBytePtr(m_pExtUserDataList, sizeof(*m_pExtUserDataList)).Get<const ResExtUserData>();
            return pData->GetSystemDataCount();
        }

        return 0;
    }

    // システム用拡張ユーザーデータに含まれているバイナリデータをインデックス指定で取得します。

    //! @brief 内部用機能のため使用禁止です。
    //!
    //! @param[in]  index   取得するデータのインデックス。
    //!
    //! @return バイナリデータです。存在しない場合は NULL が返ります。
    //!
    const void* GetSystemExtDataByIndex(int index) const
    {
        if (GetSystemExtDataFlag() != 0)
        {
            const ResExtUserData* pData = nn::util::ConstBytePtr(m_pExtUserDataList, sizeof(*m_pExtUserDataList)).Get<const ResExtUserData>();
            return pData->GetSystemData(index);
        }

        return NULL;
    }

    // システム用拡張ユーザーデータに含まれているバイナリデータをタイプを指定して検索します。

    //! @brief 内部用機能のため使用禁止です。
    //!
    //! @param[in]  type 取得するデータのタイプ。
    //!
    //! @return バイナリデータです。存在しない場合は NULL が返ります。
    //!
    const void* GetSystemExtDataByType(PaneSystemDataType type) const;

    // システム用拡張ユーザーデータに含まれているランタイム用の動的バイナリデータを書き換え用に const を外して取得します。

    //! @brief 内部用機能のため使用禁止です。
    //!
    //! @param[in]  type 取得するデータのタイプ。書き換えに対応していないタイプの場合は内部でアサートします。
    //!
    //! @return バイナリデータです。
    //!
    void* GetSystemExtDataForModify(PaneSystemDataType type);

    // 渡された ResExtUserDataList をもとに m_SystemExtDataFlag を設定します。
    // SetExtUserData などで拡張ユーザーデータが設定されたタイミングでこのメソッドを呼び出して保持情報のフラグを更新します。

    //! @brief 内部用機能のため使用禁止です。
    //!
    //! @param[in]  pExtUserDataList    フラグ情報を作成するユーザーデータリスト。
    //!
    void UpdateSystemExtDataFlag(const ResExtUserDataList* pExtUserDataList);


    // ペインが非表示かどうかを確認し、非表示の場合は ConstantBufferReady を false にします。

    //! @brief 内部用機能のため使用禁止です。
    //!
    //! @return ペインが非表示かどうかです。
    //!
    bool CheckInvisibleAndUpdateConstantBufferReady();

    //  ペインエフェクトの静的キャッシュレンダリングが行われるかチェックします。

    //! @brief 内部用機能のため使用禁止です。
    //!
    //! @return マスクやドロップシャドウの静的キャッシュレンダリングが行われる場合は true を返します。
    //!
    bool IsPaneEffectStaticCacheRenderingNeeded() const;

    //! @brief キャプチャする際のルートグローバル行列を取得します。
    //!
    //! @details
    //! キャプチャペインでキャプチャする際はキャプチャペインのグローバル行列に、ペインエフェクト系でキャプチャする際はペインエフェクトが適用されたペインに設定されます。
    //!
    //! @param[out] mtx 計算結果の行列が書き込まれます。
    //! @param[in]  drawInfo 描画情報です。
    //!
    void CalculateCaptureRootMatrix(nn::util::MatrixT4x3fType& mtx, const DrawInfo& drawInfo) const
    {
        detail::CalculateCaptureRootMatrix(mtx, drawInfo);
    }

    //! @brief マスク機能など、自分自身をキャプチャする際のプロジェクション行列を取得します。
    //!
    //! @param[out] mtx 計算結果の行列が書き込まれます。
    //!
    void CalculateCaptureProjectionMatrix(nn::util::MatrixT4x4fType& mtx) const;

    //! @brief マスク機能など、自分自身をキャプチャする際の描画位置調整のため、マテリアルのコンスタントバッファの行列を上書きします。
    //!
    //! @param[in] drawInfo 描画情報です。
    //!
    void UpdateMaterialConstantBufferForEffectCapture(const DrawInfo& drawInfo);

    //! @brief ペインエフェクトのキャプチャ用の描画設定を上書きします。
    //!
    //! @details
    //! コマンドバッファに直接設定するため、ペインの通常の描画設定を行った後に呼び出されることを想定しています。
    //!
    //! @param[in] commandBuffer    コマンドバッファです。
    //! @param[in] drawInfo 描画情報です。
    //!
    void UpdateRenderStateForPaneEffectCapture(nn::gfx::CommandBuffer& commandBuffer, const DrawInfo& drawInfo);
private:
    Pane* m_pParent;
    PaneList m_ChildList;

    nn::util::Float3 m_Translate;
    nn::util::Float3 m_Rotate;
    nn::util::Float2 m_Scale;
    Size m_Size;

    NN_STATIC_ASSERT(
        (
            3 + // m_Translate
            3 + // m_Rotate
            2 + // m_Scale
            2   // m_Size
        ) == AnimTargetPane_MaxAnimTargetPane
    );

    uint8_t  m_Flag;
    uint8_t  m_Alpha;
    uint8_t  m_GlbAlpha;
    uint8_t  m_BasePosition;
    uint8_t  m_FlagEx;

    // m_GlbMtx が align16 のため 16 バイトにあわせてパディングが入る。
    NN_PADDING3;
    uint32_t m_SystemDataFlags;
    NN_PADDING4;

    nn::util::MatrixT4x3fType m_GlbMtx; //!< ペインのSrt行列 (親子での計算後) です。

    const nn::util::MatrixT4x3fType* m_pUserMtx;
    const ResExtUserDataList* m_pExtUserDataList;

    char m_Name[ResourceNameStrMax + 1];
    char m_UserData[UserDataStrMax + 1];
private:
    void InitializeParams();

    void InitializePaneEffects(BuildResultInformation* pOutBuildResultInformation, nn::gfx::Device* pDevice, const BuildArgSet& buildArgSet);

    // 拡張ユーザーデータがペインクラス内部で動的にメモリを確保されているかどうかの判定
    bool IsExtUserDataMemoryDynamicallyAllocated() const;
    // システム用拡張ユーザーデータの参照テーブルを動的システム用拡張ユーザーデータとして追加します。
    void AddSystemExtUserDataReferenceTable();
    // システム用拡張ユーザーデータ型からシステム用拡張ユーザーデータ参照テーブルのインデックスへ変換します。
    int ConvertSystemExtDataTypeToReferenceTableIndex(PaneSystemDataType type) const;
    void AddDynamicSystemExtUserDataImpl(PaneSystemDataType dataType, void* pData, int dataSize, bool addToSystemDataTop);
    void AddDynamicSystemExtUserDataAllNewImpl(void* pData, int dataSize);
    void AddDynamicSystemExtUserDataNewSystemDataImpl(void* pData, int dataSize);
    void AddDynamicSystemExtUserDataToSystemDataImpl(void* pData, int dataSize, bool addToSystemDataTop);

    // コピー演算子を禁止します。
    const Pane& operator=( const Pane& );
    void CalculateScaleFromPartsRoot(nn::util::Float2* scale, Pane* pane) const;

    void AllocateAndCopyAnimatedExtUserData(const ResExtUserDataList* pExtUserDataList);

    const SystemDataAlignmentExInfo* FindAlignmentExInfo() const;
};


} // namespace nn::ui2d
} // namespace nn
