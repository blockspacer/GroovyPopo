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
* @brief アニメーションオブジェクトに関するクラスの宣言
*/

#pragma once

#include <nn/g3d/g3d_Configuration.h>
#include <nn/g3d/g3d_ResAnimCurve.h>
#include <nn/g3d/g3d_Binding.h>
#include <nn/g3d/detail/g3d_Flag.h>

#include <limits>

namespace nn { namespace g3d {

class ResModel;
class ModelObj;

//--------------------------------------------------------------------------------------------------

//! @brief アニメーションのフレームを制御するクラスです。
class AnimFrameCtrl
{
public:
    //! @brief 入力フレームを再生ポリシーに応じて加工する関数の型です。
    typedef float (*PlayPolicy)(
        float inputFrame, float startFrame, float endFrame, void* pUserPtr);

    //----------------------------------------
    //! @name 構築/破棄
    //@{

    //! @brief コンストラクタです。
    AnimFrameCtrl() NN_NOEXCEPT
        : m_Frame(0.0f)
        , m_StartFrame(0.0f)
        , m_EndFrame(0.0f)
        , m_Step(1.0f)
        , m_pPlayPolicy(PlayOneTime)
        , m_pUserPtr(NULL)
    {
    }

    //! @brief 初期化を行います。
    //!
    //! @param[in] startFrame 開始フレーム。
    //! @param[in] endFrame 終了フレーム。
    //! @param[in] pPlayPolicy 再生ポリシー。
    //!
    void Initialize(float startFrame, float endFrame, PlayPolicy pPlayPolicy) NN_NOEXCEPT;

    //@}

    //----------------------------------------
    //! @name 更新
    //@{

    //! @brief フレーム更新幅に応じて現在のフレーム位置を更新します。
    void UpdateFrame() NN_NOEXCEPT
    {
        m_Frame = m_pPlayPolicy(m_Frame + m_Step, m_StartFrame, m_EndFrame, m_pUserPtr);
    }

    //@}

    //----------------------------------------
    //! @name 設定/取得
    //@{

    //! @brief 現在のフレーム位置を設定します。
    //!
    //! @param[in] frame フレーム位置。
    //!
    void SetFrame(float frame) NN_NOEXCEPT
    {
        m_Frame = m_pPlayPolicy(frame, m_StartFrame, m_EndFrame, m_pUserPtr);
    }

    //! @brief 現在のフレーム位置を取得します。
    //!
    //! @return 現在のフレーム位置を返します。
    //!
    float GetFrame() const NN_NOEXCEPT
    {
        return m_Frame;
    }

    //! @brief 開始フレームと終了フレームを設定します。
    //!
    //! @param[in] start 開始位置。
    //! @param[in] end 終了位置。
    //!
    void SetFrameRange(float start, float end) NN_NOEXCEPT
    {
        NN_SDK_REQUIRES(end - start >= 0.0f);
        m_StartFrame = start;
        m_EndFrame = end;
    }

    //! @brief 開始フレームを取得します。
    //!
    //! @return 開始フレーム位置を返します。
    //!
    float GetStartFrame() const NN_NOEXCEPT
    {
        return m_StartFrame;
    }

    //! @brief 終了フレームを取得します。
    //!
    //! @return 終了フレーム位置を返します。
    //!
    float GetEndFrame() const NN_NOEXCEPT
    {
        return m_EndFrame;
    }

    //! @brief フレーム更新幅を設定します。
    //!
    //! @param[in] step フレーム更新幅。
    //!
    void SetStep(float step) NN_NOEXCEPT
    {
        m_Step = step;
    }

    //! @brief フレーム更新幅を取得します。
    //!
    //! @return フレーム更新幅を返します。
    //!
    float GetStep() const NN_NOEXCEPT
    {
        return m_Step;
    }

    //! @brief アニメーション再生ポリシーを設定します。
    //!
    //! @param[in] policy アニメーション再生ポリシー。
    //!
    void SetPlayPolicy(PlayPolicy policy) NN_NOEXCEPT
    {
        m_pPlayPolicy = policy;
    }

    //! @brief アニメーション再生ポリシーを取得します。
    //!
    //! @return アニメーション再生ポリシーを返します。
    //!
    PlayPolicy GetPlayPolicy() const NN_NOEXCEPT
    {
        return m_pPlayPolicy;
    }

    //! @brief ユーザーポインターを設定します。
    //!
    //! @param[in] pUserPtr ポインター。
    //!
    void SetUserPtr(void* pUserPtr) NN_NOEXCEPT
    {
        m_pUserPtr = pUserPtr;
    }

    //! @brief ユーザーポインターを取得します。
    //!
    //! @return SetUserPtr() で設定したポインターを返します。
    //!
    void* GetUserPtr() NN_NOEXCEPT
    {
        return m_pUserPtr;
    }

    //! @brief ユーザーポインターを取得します。
    //!
    //! @return SetUserPtr() で設定したポインターを返します。
    //!
    const void* GetUserPtr() const NN_NOEXCEPT
    {
        return m_pUserPtr;
    }

    //! @brief ユーザーポインターを取得します。
    //!
    //! @tparam T ポインターを受けとる際の型。
    //!
    //! @return SetUserPtr() で設定したポインターを返します。
    //!
    template <typename T>
    T* GetUserPtr() NN_NOEXCEPT
    {
        return static_cast<T*>(m_pUserPtr);
    }

    //! @brief ユーザーポインターを取得します。
    //!
    //! @tparam T ポインターを受けとる際の型。
    //!
    //! @return SetUserPtr() で設定したポインターを返します。
    //!
    template <typename T>
    const T* GetUserPtr() const NN_NOEXCEPT
    {
        return static_cast<const T*>(m_pUserPtr);
    }

    //@}

    //! @brief ワンタイム再生用の標準アニメーションポリシー関数です。
    //!
    //! @param[in] inputFrame フレーム位置。
    //! @param[in] startFrame 開始フレーム。
    //! @param[in] endFrame 終了フレーム。
    //! @param[in] pUserData ユーザーデータ。
    //!
    static float PlayOneTime(float inputFrame, float startFrame, float endFrame, void* pUserData) NN_NOEXCEPT;

    //! @brief ループ再生用の標準アニメーションポリシー関数です。
    //!
    //! @param[in] inputFrame フレーム位置。
    //! @param[in] startFrame 開始フレーム。
    //! @param[in] endFrame 終了フレーム。
    //! @param[in] pUserData ユーザーデータ。
    //!
    static float PlayLoop(float inputFrame, float startFrame, float endFrame, void* pUserData) NN_NOEXCEPT;

    static const float InvalidFrame;

private:
    float m_Frame;
    float m_StartFrame;
    float m_EndFrame;
    float m_Step;
    PlayPolicy m_pPlayPolicy;
    void* m_pUserPtr;

    NN_DISALLOW_COPY(AnimFrameCtrl);
};

//--------------------------------------------------------------------------------------------------

//! @brief アニメーションの関連付け状態を管理するクラスです。
class AnimBindTable
{
public:
    //! @briefprivate 関連付けに関するフラグです。
    //!
    enum BindFlag
    {
        //! @brief 有効です。
        BindFlag_Enabled             = 0,

        //! @brief 更新処理を無視するが、最後の結果が Apply されます。
        BindFlag_SkipCalculate       = 0x1 << 0,

        //! @brief Apply を無視するので、単独では無駄な更新が行われます。
        BindFlag_SkipApply           = 0x1 << 1,

        //! @brief Calculate も Apply もスキップします。
        BindFlag_Disabled            = BindFlag_SkipCalculate | BindFlag_SkipApply
    };

    //! @briefprivate
    enum Shift
    {
        Shift_Reverse = 15,
        Shift_Flag    = Shift_Reverse * 2
    };

    //! @briefprivate
    enum Mask
    {
        Mask_Index = 0x7FFF,
        Mask_Flag  = 0x3 << Shift_Flag
    };

    //! @briefprivate バインド状態の管理に関するフラグです。
    //!
    enum Flag
    {
        Flag_NotBound           = 0x7FFF,   // [0 .. 0x7FFE] が有効なインデックス
        Flag_ReverseNotBound    = Flag_NotBound     << Shift_Reverse,
        Flag_ReverseIndexMask   = Mask_Index        << Shift_Reverse, // Target から Anim へのバインド
        Flag_Enabled            = BindFlag_Enabled       << Shift_Flag,
        Flag_SkipCalculate      = BindFlag_SkipCalculate << Shift_Flag,
        Flag_SkipApply          = BindFlag_SkipApply     << Shift_Flag,
        Flag_Disabled           = BindFlag_Disabled      << Shift_Flag,
        Flag_TargetBound        = 0x1   // BindTable がバインド済み
    };

    //----------------------------------------
    //! @name 構築
    //@{

    //! @brief コンストラクタです。
    AnimBindTable() NN_NOEXCEPT
        : m_pBindArray(NULL)
        , m_Flag(0)
        , m_Size(0)
        , m_AnimCount(0)
        , m_TargetCount(0)
    {
    }

    //! @brief 初期化を行います。
    //!
    //! @param[in] pBindArray 関連付け状態を記録するテーブルへのポインター。
    //! @param[in] tableSize テーブルサイズ。
    //!
    void Initialize(Bit32* pBindArray, int tableSize) NN_NOEXCEPT;

    //@}

    //----------------------------------------
    //! @name 設定/取得
    //@{

    //! @brief アニメーションの総数を設定します。
    //!
    //! @param[in] animCount アニメーションの総数。
    //!
    void SetAnimCount(int animCount) NN_NOEXCEPT
    {
        NN_SDK_REQUIRES(0 <= animCount && animCount <= m_Size);
        m_AnimCount = static_cast<uint16_t>(animCount);
    }

    //! @brief 関連付け状態を記録するテーブルのサイズを取得します。
    //!
    //! @return 関連付け状態を記録するテーブルのサイズを返します。
    //!
    int GetSize() const NN_NOEXCEPT
    {
        return m_Size;
    }

    //! @brief 記録されているアニメーションの総数を取得します。
    //!
    //! @return 記録されているアニメーションの総数を返します。
    //!
    int GetAnimCount() const NN_NOEXCEPT
    {
        return m_AnimCount;
    }

    //! @brief 記録されているアニメーション対象の総数を取得します。
    //!
    //! @return 記録されているアニメーション対象の総数を返します。
    //!
    int GetTargetCount() const NN_NOEXCEPT
    {
        return m_TargetCount;
    }

    //@}

    //----------------------------------------
    //! @name 関連付け
    //@{

    //! @brief 関連付け状態をクリアします。
    //!
    //! @param[in] targetCount 関連付け状態をクリアする数。
    //!
    void ClearAll(int targetCount) NN_NOEXCEPT;

    //! @brief アニメーションからアニメーション対象への片方向の関連付けを元に、
    //! 両方向の関連付けを復元して記録します。
    //!
    //! @param[in] pBindIndexArray 関連付けた対象のインデックスを記録した配列へのポインター。
    //!
    void BindAll(const uint16_t* pBindIndexArray) NN_NOEXCEPT;

    //! @brief アニメーションとアニメーション対象の関連付けを行います。
    //!
    //! @param[in] animIndex アニメーションのインデックス。
    //! @param[in] targetIndex アニメーション対象のインデックス。
    //!
    void Bind(int animIndex, int targetIndex) NN_NOEXCEPT
    {
        NN_SDK_REQUIRES_RANGE(animIndex, 0, m_AnimCount + 1);
        NN_SDK_REQUIRES_RANGE(targetIndex, 0, m_TargetCount + 1);
        m_pBindArray[animIndex] &= ~(Mask_Index | Mask_Flag);
        m_pBindArray[animIndex] |= (targetIndex & Mask_Index);
        m_pBindArray[targetIndex] &= ~Flag_ReverseIndexMask;
        m_pBindArray[targetIndex] |= (animIndex & Mask_Index) << Shift_Reverse;
    }

    //! @brief アニメーションとアニメーション対象の関連付けを解除します。
    //!
    //! @param[in] animIndex アニメーションのインデックス。
    //! @param[in] targetIndex アニメーション対象のインデックス。
    //!
    void Unbind(int animIndex, int targetIndex) NN_NOEXCEPT
    {
        NN_SDK_REQUIRES_RANGE(animIndex, 0, m_AnimCount + 1);
        NN_SDK_REQUIRES_RANGE(targetIndex, 0, m_TargetCount + 1);
        NN_SDK_REQUIRES(targetIndex == GetTargetIndex(animIndex));
        m_pBindArray[animIndex] |= (Flag_NotBound | Flag_Disabled);
        m_pBindArray[targetIndex] |= Flag_ReverseNotBound;
    }

    //! @brief アニメーションに関連付けられているアニメーション対象のインデックスを取得します。
    //!
    //! @param[in] animIndex アニメーションのインデックス。
    //!
    //! @return アニメーション対象のインデックスを返します。
    //!
    int GetTargetIndex(int animIndex) const NN_NOEXCEPT
    {
        NN_SDK_REQUIRES_RANGE(animIndex, 0, m_AnimCount + 1);
        return  m_pBindArray[animIndex] & Mask_Index;
    }

    //! @brief アニメーション対象に関連付けられているアニメーションのインデックスを取得します。
    //!
    //! @param[in] targetIndex アニメーション対象のインデックス。
    //!
    //! @return アニメーションのインデックスを返します。
    //!
    int GetAnimIndex(int targetIndex) const NN_NOEXCEPT
    {
        NN_SDK_REQUIRES_RANGE(targetIndex, 0, m_TargetCount + 1);
        return ( m_pBindArray[targetIndex] >> Shift_Reverse) & Mask_Index;
    }

    //! @brief バインドテーブルを関連付け済み状態にします。
    void SetTargetBound() NN_NOEXCEPT
    {
        m_Flag |= Flag_TargetBound;
    }

    //! @brief バインドテーブルを関連付けされていない状態にします。
    void SetTargetUnbound() NN_NOEXCEPT
    {
        m_Flag &= ~Flag_TargetBound;
    }

    //! @brief バインドテーブルが関連付け済みかどうかを取得します。
    //!
    //! @return 関連付け済みの場合は true、関連付けがされていない場合は false を返します。
    //!
    bool IsTargetBound() const NN_NOEXCEPT
    {
        return 0 != (m_Flag & Flag_TargetBound);
    }

    //@}

    //----------------------------------------
    //! @name 有効/無効
    //@{

    //! @brief アニメーションの計算が有効かどうかを取得します。
    //!
    //! @param[in] animIndex アニメーションのインデックス。
    //!
    //! @return 有効な場合は true、無効な場合は false を返します。
    //!
    bool IsCalculateEnabled(int animIndex) const NN_NOEXCEPT
    {
        NN_SDK_REQUIRES_RANGE(animIndex, 0, m_AnimCount + 1);
        return CheckFlag(m_pBindArray[animIndex], Flag_SkipCalculate, 0);
    }

    //! @brief アニメーションの適用が有効かどうかを取得します。
    //!
    //! @param[in] animIndex アニメーションのインデックス。
    //!
    //! @return 有効な場合は true、無効な場合は false。
    //!
    bool IsApplyEnabled(int animIndex) const NN_NOEXCEPT
    {
        NN_SDK_REQUIRES_RANGE(animIndex, 0, m_AnimCount + 1);
        return CheckFlag(m_pBindArray[animIndex], Flag_SkipApply, 0);
    }

    //! @brief アニメーションの計算と適用が共に有効かどうかを取得します。
    //!
    //! @param[in] animIndex アニメーションのインデックス。
    //!
    //! @return アニメーションの計算と適用が共に有効な場合は true、それ以外の場合は false。
    //!
    bool IsEnabled(int animIndex) const NN_NOEXCEPT
    {
        NN_SDK_REQUIRES_RANGE(animIndex, 0, m_AnimCount + 1);
        return CheckFlag(m_pBindArray[animIndex], Mask_Flag, Flag_Enabled);
    }

    //! @brief アニメーションの計算と適用が共に無効かどうかを取得します。
    //!
    //! @param[in] animIndex アニメーションのインデックス。
    //!
    //! @return アニメーションの計算と適用が共に無効な場合は true、それ以外の場合は false を返します。
    //!
    bool IsDisabled(int animIndex) const NN_NOEXCEPT
    {
        NN_SDK_REQUIRES_RANGE(animIndex, 0, m_AnimCount + 1);
        return CheckFlag(m_pBindArray[animIndex], Mask_Flag, Flag_Disabled);
    }

    //! @brief アニメーションの有効/無効に関するフラグを取得します。
    //!
    //! @param[in] animIndex アニメーションのインデックス。
    //!
    //! @return アニメーションの有効/無効に関するフラグを返します。
    //!
    BindFlag GetBindFlag(int animIndex) const NN_NOEXCEPT
    {
        NN_SDK_REQUIRES_RANGE(animIndex, 0, m_AnimCount + 1);
        return BindFlag(m_pBindArray[animIndex] >> Shift_Flag);
    }

    //! @brief アニメーションの有効/無効に関するフラグを設定します。
    //!
    //! @param[in] animIndex アニメーションのインデックス。
    //! @param[in] flag アニメーションの有効/無効に関するフラグ。
    //!
    void SetBindFlag(int animIndex, BindFlag flag) NN_NOEXCEPT
    {
        SetBindFlagRaw(animIndex, CreateFlagValue<Bit32>(Bit32(flag), Shift_Flag, Mask_Flag));
    }

    //! @brief アニメーションの有効/無効に関するフラグを設定します。
    //!
    //! @param[in] animIndex アニメーションのインデックス。
    //! @param[in] flag アニメーションの有効/無効に関するフラグ。
    //!
    void SetBindFlagRaw(int animIndex, Bit32 flag) NN_NOEXCEPT
    {
        NN_SDK_REQUIRES_RANGE(animIndex, 0, m_AnimCount + 1);
        // flag のマスクもシフトも行わない
        m_pBindArray[animIndex] &= ~Mask_Flag;
        m_pBindArray[animIndex] |= flag;
    }

    //@}

private:
    Bit32* m_pBindArray;
    Bit16 m_Flag;
    uint16_t m_Size;
    uint16_t m_AnimCount;
    uint16_t m_TargetCount;

    NN_DISALLOW_COPY(AnimBindTable);
};

//--------------------------------------------------------------------------------------------------

//! @brief アニメーション高速化のためのコンテクストクラスです。
//!
//! 前回評価時のフレームの前後のキーが指すフレーム数を記録し、次回評価時にはキーの探索が必要かを判定し、
//! 必要なときのキーの探索を行うことで、高速化を行います。
//! 逆再生時やカーブがベイクされている場合は使用されません。
//!
class AnimContext
{
public:
    //----------------------------------------
    //! @name 構築
    //@{

    //! @brief コンストラクタです。
    AnimContext() NN_NOEXCEPT
        : m_pFrameCacheArray(NULL)
        , m_Size(0)
        , m_CurveCount(0)
        , m_LastFrame(0.0f)
    {
    }

    //! @brief 初期化を行います。
    //!
    //! @param[in] pFrameCacheArray キーインデックスの配列へのポインター。
    //! @param[in] arraySize 配列のサイズ。
    //!
    void Initialize(AnimFrameCache* pFrameCacheArray, int arraySize) NN_NOEXCEPT;

    //@}

    //----------------------------------------
    //! @name 取得/設定
    //@{

    //! @brief アニメーションカーブの数を設定します。
    //!
    //! @param[in] curveCount アニメーションカーブの数。
    //!
    void SetCurveCount(int curveCount) NN_NOEXCEPT
    {
        NN_G3D_WARNING(m_pFrameCacheArray == NULL || curveCount <= m_Size, "Too many curves to store context.");
        m_CurveCount = curveCount;
        Reset();
    }

    //! @brief コンテクストをリセットします。
    void Reset() NN_NOEXCEPT
    {
        m_LastFrame = AnimFrameCtrl::InvalidFrame;
        if (IsFrameCacheValid())
        {
            for (int idxCurve = 0; idxCurve < m_CurveCount; ++idxCurve)
            {
                m_pFrameCacheArray[idxCurve].start = std::numeric_limits<float>::infinity();
            }
        }
    }

    //! @brief アニメーションカーブの数を取得します。
    //!
    //! @return アニメーションカーブの数を返します。
    //!
    int GetCurveCount() const NN_NOEXCEPT
    {
        return m_CurveCount;
    }

    //! @brief キーインデックスの記録が有効かどうかを取得します。
    //!
    //! @return キーインデックスの記録が有効な場合は true、無効な場合は false を返します。
    //!
    bool IsFrameCacheValid() const NN_NOEXCEPT
    {
        return 0 < m_CurveCount && m_CurveCount <= m_Size;
    }

    //! @brief キーインデックスの配列を取得します。
    //!
    //! @return キーインデックスの配列へのポインターを返します。
    //!
    AnimFrameCache* GetFrameCacheArray(int startIndex) NN_NOEXCEPT
    {
        return &m_pFrameCacheArray[startIndex];
    }

    //! @brief 直近のフレームを設定します。
    //!
    //! @param[in] frame 直近のフレーム位置。
    //!
    void SetLastFrame(float frame) NN_NOEXCEPT
    {
        m_LastFrame = frame;
    }

    //! @brief 直近のフレームを取得します。
    //!
    //! @return 直近のフレーム位置を返します。
    //!
    float GetLastFrame() const NN_NOEXCEPT
    {
        return m_LastFrame;
    }

    //@}

private:
    AnimFrameCache* m_pFrameCacheArray;
    int m_Size;
    int m_CurveCount;
    float m_LastFrame;

    NN_DISALLOW_COPY(AnimContext);
};

//--------------------------------------------------------------------------------------------------

//! @brief アニメーションインスタンスの基底クラスです。
class AnimObj
{
public:
    //! @brief 関連付けに関するフラグです。
    enum BindFlag
    {
        BindFlag_Enabled       = AnimBindTable::BindFlag_Enabled, //!< @brief 有効です。
        BindFlag_SkipCalculate = AnimBindTable::BindFlag_SkipCalculate, //!< @brief 更新処理を無視するが、最後の結果が Apply されます。
        BindFlag_SkipApply     = AnimBindTable::BindFlag_SkipApply, //!< @brief Apply を無視するので、単独では無駄な更新が行われます。
        BindFlag_Disabled      = AnimBindTable::BindFlag_Disabled //!< @brief Calculate も Apply もスキップします。
    };

    //----------------------------------------
    //! @name 破棄
    //@{

    //! @brief デストラクタです。
    virtual ~AnimObj() NN_NOEXCEPT
    {
    }

    //@}

    //----------------------------------------
    //! @name フレーム制御
    //@{

    //! @brief フレームコントローラを取得します。
    //!
    //! @return フレームコントローラを返します。
    //!
    AnimFrameCtrl& GetFrameCtrl() NN_NOEXCEPT
    {
        return *m_pFrameCtrl;
    }

    //! @brief フレームコントローラを取得します。
    //!
    //! @return フレームコントローラを返します。
    //!
    const AnimFrameCtrl& GetFrameCtrl() const NN_NOEXCEPT
    {
        return *m_pFrameCtrl;
    }

    //! @brief フレームコントローラを設定します。
    //!
    //! @param[in] pFrameCtrl フレームコントローラ。
    //!
    //! @details
    //! デフォルトのフレームコントローラはリソースの変更時に自動的に初期化されますが、
    //! ここで設定したフレームコントローラは初期化されません。
    //!
    void SetFrameCtrl(AnimFrameCtrl* pFrameCtrl) NN_NOEXCEPT
    {
        m_pFrameCtrl = pFrameCtrl != NULL ? pFrameCtrl : &m_DefaultFrameCtrl;
    }

    //! @brief このオブジェクトが所有権を持つデフォルトのフレームコントローラを取得します。
    //!
    //! @return デフォルトのフレームコントローラを返します。
    //!
    AnimFrameCtrl& GetDefaultFrameCtrl() NN_NOEXCEPT
    {
        return m_DefaultFrameCtrl;
    }

    //! @brief このオブジェクトが所有権を持つデフォルトのフレームコントローラを取得します。
    //!
    //! @return デフォルトのフレームコントローラを返します。
    //!
    const AnimFrameCtrl& GetDefaultFrameCtrl() const NN_NOEXCEPT
    {
        return m_DefaultFrameCtrl;
    }

    //@}

    //----------------------------------------
    //! @name コンテクスト
    //@{

    //! @brief アニメーション高速化のためのコンテクストを取得します。
    //!
    //! @return アニメーション高速化のためのコンテクストを返します。
    //!
    AnimContext& GetContext() NN_NOEXCEPT
    {
        return m_Context;
    }

    //! @brief アニメーション高速化のためのコンテクストを取得します。
    //!
    //! @return アニメーション高速化のためのコンテクストを返します。
    //!
    const AnimContext& GetContext() const NN_NOEXCEPT
    {
        return m_Context;
    }

    //! @brief アニメーション高速化のためのコンテクストを破棄します。
    void InvalidateContext() NN_NOEXCEPT
    {
        m_Context.Reset();
    }

    //@}

    //----------------------------------------
    //! @name 更新
    //@{

    //! @brief アニメーション結果を初期化します。
    //!
    //! @details
    //! バインド時に呼ばれるため、意図的に結果を初期化したい場合以外は呼ぶ必要はありません。
    //!
    virtual void ClearResult() NN_NOEXCEPT = 0;

    //! @brief アニメーションカーブを評価します。
    //!
    //! @details
    //! 前回の計算時からフレームが変わっていない場合は計算を行いません。
    //!
    virtual void Calculate() NN_NOEXCEPT = 0;

    //@}

    //----------------------------------------
    //! @name 取得/設定
    //@{

    //! @brief アニメーションカーブの総数を取得します。
    //!
    //! @return アニメーションカーブの総数を返します。
    //!
    int GetCurveCount() const NN_NOEXCEPT
    {
        return m_Context.GetCurveCount();
    }

    //! @brief Build 時に渡されたバッファーを取得します。
    //!
    //! @return Build 時に渡されたバッファーへのポインターを返します。
    //!
    void* GetBufferPtr() NN_NOEXCEPT
    {
        return m_pBufferPtr;
    }

    //@}

protected:

    //! @briefprivate
    AnimObj() NN_NOEXCEPT
        : m_pFrameCtrl(NULL)
        , m_DefaultFrameCtrl()
        , m_Context()
        , m_pResultBuffer(NULL)
        , m_pBufferPtr(NULL)
    {
        SetFrameCtrl(NULL);
    }

    void SetBufferPtr(void* pBuffer) NN_NOEXCEPT
    {
        m_pBufferPtr = pBuffer;
    }

    //! @briefprivate
    void SetResultBuffer(void* pBuffer) NN_NOEXCEPT
    {
        m_pResultBuffer = pBuffer;
    }

    //! @briefprivate
    void* GetResultBuffer() NN_NOEXCEPT
    {
        return m_pResultBuffer;
    }

    //! @briefprivate
    const void* GetResultBuffer() const NN_NOEXCEPT
    {
        return m_pResultBuffer;
    }

    //! @briefprivate
    void ResetFrameCtrl(int frameCount, bool loop) NN_NOEXCEPT;

    //! @briefprivate
    bool IsFrameChanged() const NN_NOEXCEPT
    {
        return m_Context.GetLastFrame() != m_pFrameCtrl->GetFrame();
    }

    //! @briefprivate
    void UpdateLastFrame() NN_NOEXCEPT
    {
        m_Context.SetLastFrame(m_pFrameCtrl->GetFrame());
    }

    //@}

private:
    AnimFrameCtrl* m_pFrameCtrl;
    AnimFrameCtrl m_DefaultFrameCtrl;
    AnimContext m_Context;
    void* m_pResultBuffer;
    void* m_pBufferPtr; // m_pResultBuffer は NULL になる場合があるので、別途覚えておく。

    NN_DISALLOW_COPY(AnimObj);
};

//--------------------------------------------------------------------------------------------------

//! @brief モデルアニメーションインスタンスの基底クラスです。
class ModelAnimObj : public AnimObj
{
public:

    //----------------------------------------
    //! @name 関連付け
    //@{

    // アニメーション種類ごとの関数
    // 派生クラスから呼び出す場合は派生先の関数を明示して呼び出す方が効率がよい。

    //! @brief アニメーション対象に関連付けます。
    //!
    //! @param[in] pModel アニメーション対象のモデルリソースへのポインター。
    //!
    //! @return BindResult を返します。
    //!
    virtual BindResult Bind(const ResModel* pModel) NN_NOEXCEPT = 0;

    //! @brief アニメーション対象に関連付けます。
    //!
    //! @param[in] pModelObj アニメーション対象のモデルへのポインター。
    //!
    //! @return BindResult を返します。
    //!
    virtual BindResult Bind(const ModelObj* pModelObj) NN_NOEXCEPT = 0;

    //! @brief アニメーション対象に関連付けます。
    //!
    //! @param[in] pModel アニメーション対象のモデルリソースへのポインター。
    //!
    //! @return 関連付けが成功の場合は true、失敗の場合は false を返します。
    //!
    //! @details
    //! リソース同士を PreBind によって事前に関連付けることにより、
    //! BindFast 時には名前引きを行わない比較的高速な関連付けを行います。
    //!
    virtual void BindFast(const ResModel* pModel) NN_NOEXCEPT = 0;

    //! @brief アニメーション対象への関連付け状態を表すオブジェクトを取得します。
    //!
    //! @return アニメーション対象への関連付け状態を表すオブジェクトを返します。
    //!
    AnimBindTable& GetBindTable() NN_NOEXCEPT
    {
        return m_BindTable;
    }

    //! @brief アニメーション対象への関連付け状態を表すオブジェクトを取得します。
    //!
    //! @return アニメーション対象への関連付け状態を表すオブジェクトを返します。
    //!
    const AnimBindTable& GetBindTable() const NN_NOEXCEPT
    {
        return m_BindTable;
    }

    //! @brief アニメーション対象に関連付けられているかどうかを取得します。
    //!
    //! @return アニメーション対象に関連付けられている場合は true、関連付けられていない場合は false を返します。
    //!
    bool IsTargetBound() const NN_NOEXCEPT
    {
        return GetBindTable().IsTargetBound();
    }

    //@}

    //----------------------------------------
    //! @name 更新
    //@{

    //! @brief アニメーション結果を対象に適用します。
    //!
    //! @param[in] pModelObj モデルへのポインター。
    //!
    virtual void ApplyTo(ModelObj* pModelObj) const NN_NOEXCEPT = 0;

    //@}

    //----------------------------------------
    //! @name 取得/設定
    //@{

    //! @brief 処理するアニメーションの個数を取得します。
    //!
    //! @return 処理するアニメーションの個数を返します。
    //!
    //! @details
    //! ボーンやマテリアルに対応するアニメーションの個数です。
    //!
    int GetAnimCount() const NN_NOEXCEPT
    {
        return m_BindTable.GetAnimCount();
    }

    //! @brief ターゲットの総数を取得します。
    //!
    //! @return ターゲットの総数を返します。
    //!
    //! @details
    //! アニメーション対象となりうるボーンやマテリアルの最大数です。
    //! この数を超えてアニメーションをバインドすることはできません。
    //!
    int GetTargetCount() const NN_NOEXCEPT
    {
        return m_BindTable.GetTargetCount();
    }

    //@}

protected:
    //! @briefprivate
    ModelAnimObj() NN_NOEXCEPT
        : AnimObj()
        , m_BindTable()
    {
    }

    //! @briefprivate
    void SetTargetBound() NN_NOEXCEPT
    {
        GetBindTable().SetTargetBound();
        InvalidateContext(); // バインドし直した場合、前回評価のコンテクストは無効。
    }

    //! @briefprivate
    void SetTargetUnbound() NN_NOEXCEPT
    {
        GetBindTable().SetTargetUnbound();
    }

    //! @briefprivate
    void SetBindFlagImpl(int targetIndex, BindFlag flag) NN_NOEXCEPT;

    //! @briefprivate
    BindFlag GetBindFlagImpl(int targetIndex) const NN_NOEXCEPT;

private:
    NN_DISALLOW_COPY(ModelAnimObj) NN_NOEXCEPT;

    AnimBindTable m_BindTable;
};

}} // namespace nn::g3d

