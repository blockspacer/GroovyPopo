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
* @brief シーンアニメーションに関するリソースクラスの宣言
*/

#pragma once

#include <nn/util/util_ResDic.h>
#include <nn/g3d/g3d_Configuration.h>
#include <nn/g3d/g3d_ResCommon.h>
#include <nn/g3d/g3d_ResAnimCurve.h>
#include <nn/gfx/gfx_ResUserData.h>
#include <nn/g3d/g3d_Binding.h>

NN_PRAGMA_PUSH_WARNINGS
#if defined(NN_BUILD_CONFIG_TOOLCHAIN_VC)
NN_ENABLE_WARNING_IMPLICIT_PADDING_WITHIN_CLASS
#endif

namespace nn { namespace g3d {

//! @brief カメラアニメーションの結果を格納する構造体です。
struct CameraAnimResult
{
    struct Projection
    {
        float nearZ; //!< ニアZ。
        float farZ; //!< ファーZ。
        float aspect; //!< アスペクト比。
        union
        {
            float height; //!< 高さ。
            float fovy; //!< 画角。
        };
    } proj;
    struct View
    {
        float pos[3]; //!< 位置。
        union
        {
            float aim[3]; //!< 目標点。
            float rotate[3]; //!< 回転。
        };
        float twist; //!< 捻じれ。
    } view;
};

//! @brief ライトアニメーションの結果を格納する構造体です。
struct LightAnimResult
{
    int32_t enable;
    float pos[3]; //!< 位置。
    union
    {
        float aim[3]; //!< 目標点。
        float dir[3]; //!< 方向。
    };
    float distAttn[2]; //!< 距離減衰情報。
    float angleAttn[2]; //!< 角度減衰情報。
    float color[2][3]; //!< カラー。
};

//! @brief フォグアニメーションの結果を格納する構造体です。
struct FogAnimResult
{
    float distAttn[2]; //!< 距離減衰。
    float color[3]; //!< カラー。
};

class BindFuncTable;

//--------------------------------------------------------------------------------------------------

//! @brief カメラアニメーションの構造体です。
struct ResCameraAnimData
{
    nn::util::BinBlockSignature         signature; //!< シグネチャー。
    Bit16                               flag; //!< フラグ。
    uint8_t                             reserved0[2];
    nn::util::BinPtrToString            pName; //!< 名前。
    nn::util::BinTPtr<ResAnimCurve>     pCurveArray; //!< アニメーションカーブ配列へのポインター。
    nn::util::BinTPtr<CameraAnimResult> pBaseValueArray; //!< 初期値配列へのポインター。
    nn::util::BinTPtr<nn::gfx::ResUserData> pUserDataArray; //!< ユーザーデータ配列へのポインター。
    nn::util::BinTPtr<nn::util::ResDic> pUserDataDic; //!< ユーザーデータ辞書へのポインター。
    int32_t  frameCount; //!< フレーム数。
    uint32_t bakedSize; //!< コマ化に必要なバッファーサイズ。
    uint16_t userDataCount; //!< ユーザーデータ数。
    uint8_t  curveCount; //!< アニメーションカーブ数。
    uint8_t  reserved1[5];
};

//! @brief カメラアニメーションのリソースです。
class ResCameraAnim : public nn::util::AccessorBase< ResCameraAnimData >
{
    NN_DISALLOW_COPY(ResCameraAnim);

public:
    //! @brief ResCameraAnim のシグネチャ
    static const uint32_t Signature = NN_UTIL_CREATE_SIGNATURE_4('F', 'C', 'A', 'M');

    //! @briefprivate
    enum Shift
    {
        Shift_Rot  = 8,
        Shift_Proj = 10
    };

    //! @briefprivate
    enum Mask
    {
        Mask_Rot  = 0x1 << Shift_Rot,
        Mask_Proj = 0x1 << Shift_Proj
    };

    //! @brief カメラアニメーションに関するフラグです。
    enum Flag
    {
        Flag_CurveBaked         = AnimFlag_CurveBaked, //!< カーブがベイク済みです。
        Flag_PlayPolicyLoop     = AnimFlag_PlayPolicyLoop, //!< ループすることを表します。
        // RotateMode
        Flag_RotAim             = 0x0 << Shift_Rot, //!< カメラの視線を注視点から計算します。
        Flag_RotEulerZxy        = 0x1 << Shift_Rot, //!< カメラの視線を回転角から計算します。
        // ProjectionMode
        Flag_ProjOrtho          = 0x0 << Shift_Proj, //!< 平行投影で計算します。
        Flag_ProjPersp          = 0x1 << Shift_Proj, //!< 透視投影で計算します。
    };

    //----------------------------------------
    //! @name 構築/破棄
    //@{

    //! @brief アニメーションの初期化を行います。
    //!
    //! @param[out] pResult アニメーション結果を格納するバッファーへのポインター。
    //!
    void Initialize(CameraAnimResult* pResult) const NN_NOEXCEPT;

    //! @brief アニメーションをコマ化します。
    //!
    //! @param[out] pBuffer コマ化した結果を格納するバッファーへのポインター。
    //! @param[in] bufferSize コマ化した結果を格納するバッファーへのポインター。
    //!
    //! @return コマ化が成功した場合は true、失敗した場合は false を返します。
    //!
    //! @pre
    //! - bufferSize >= GetBakedSize()
    //!
    bool BakeCurve(void* pBuffer, size_t bufferSize) NN_NOEXCEPT;

    //! @brief アニメーションをカーブに戻します。
    //!
    //! @return BakeCurve() 時に渡したバッファーへのポインターを返します。
    //!
    void* ResetCurve() NN_NOEXCEPT;

    //! @brief アニメーションをリセットします。
    void Reset() NN_NOEXCEPT;

    //@}

    //----------------------------------------
    //! @name 評価
    //@{

    //! @brief アニメーションを評価します。
    //!
    //! @param[out] pResult アニメーション結果を格納するバッファーへのポインター。
    //! @param[in] frame フレーム位置。
    //!
    void Evaluate(CameraAnimResult* pResult, float frame) const NN_NOEXCEPT;

    //! @brief アニメーションを評価します。
    //!
    //! @param[out] pResult アニメーション結果を格納するバッファーへのポインター。
    //! @param[in] frame フレーム位置。
    //! @param[in] pFrameCache 計算の効率化のために使用されるキャッシュへのポインター。
    //!
    void Evaluate(CameraAnimResult* pResult, float frame, AnimFrameCache* pFrameCache) const NN_NOEXCEPT;

    //@}

    //----------------------------------------
    //! @name 取得
    //@{

    //! @brief アニメーション名を取得します
    //!
    //! @return アニメーション名へのポインター。
    //!
    const char* GetName() const NN_NOEXCEPT
    {
        return ToData().pName.Get()->GetData();
    }

    //! @brief アニメーションカーブの数を取得します。
    //!
    //! @return アニメーションカーブの数を返します。
    //!
    int GetCurveCount() const NN_NOEXCEPT
    {
        return ToData().curveCount;
    }
    //! @brief インデックス引きでアニメーションカーブを取得します。
    //!
    //! @param[in] elemIndex アニメーションカーブのインデックス。
    //!
    //! @return ResAnimCurve へのポインターを返します。
    //!
    //! @pre
    //! - インデックスはアニメーションカーブの数の範囲内。
    //!
    //! @details
    //! 指定したインデックスが範囲外の場合は ASSERT します。
    //!
    ResAnimCurve* GetCurve(int elemIndex) NN_NOEXCEPT
    {
        NN_G3D_REQUIRES_RANGE(elemIndex, 0, GetCurveCount(), GetName());
        ResAnimCurve* pArray = ToData().pCurveArray.Get();
        NN_SDK_ASSERT_NOT_NULL(pArray);
        return &pArray[elemIndex];
    }
    //! @brief インデックス引きでアニメーションカーブを取得します。
    //!
    //! @param[in] elemIndex アニメーションカーブのインデックス。
    //!
    //! @return ResAnimCurve へのポインターを返します。
    //!
    //! @pre
    //! - インデックスはアニメーションカーブの数の範囲内。
    //!
    //! @details
    //! 指定したインデックスが範囲外の場合は ASSERT します。
    //!
    const ResAnimCurve* GetCurve(int elemIndex) const NN_NOEXCEPT
    {
        NN_G3D_REQUIRES_RANGE(elemIndex, 0, GetCurveCount(), GetName());
        const ResAnimCurve* pArray = ToData().pCurveArray.Get();
        NN_SDK_ASSERT_NOT_NULL(pArray);
        return &pArray[elemIndex];
    }

    //! @brief ループするかどうかを取得します。
    //!
    //! @return ループする場合は true、しない場合は false を返します。
    //!
    bool IsLooped() const NN_NOEXCEPT
    {
        return 0 != (ToData().flag & Flag_PlayPolicyLoop);
    }

    //! @brief カーブがベイクされているかどうかを取得します。
    //!
    //! @return ベイクされている場合は true、されていない場合は false を返します。
    //!
    bool IsCurveBaked() const NN_NOEXCEPT
    {
        return 0 != (ToData().flag & Flag_CurveBaked);
    }

    //! @brief フレーム数を取得します。
    //!
    //! @return フレーム数を返します。
    //!
    int GetFrameCount() const NN_NOEXCEPT
    {
        return ToData().frameCount;
    }

    //! @brief 回転の計算方式を取得します。
    //!
    //! @return 回転の計算方式を返します。
    //!
    Bit32 GetRotateMode() const NN_NOEXCEPT
    {
        return ToData().flag & Mask_Rot;
    }

    //! @brief プロジェクションの計算方式を取得します。
    //!
    //! @return プロジェクションの計算方式を返します。
    //!
    Bit32 GetProjectionMode() const NN_NOEXCEPT
    {
        return ToData().flag & Mask_Proj;
    }

    //! @brief コマ化に必要なバッファーサイズを取得します。
    //!
    //! @return コマ化に必要なバッファーサイズを返します。
    //!
    size_t GetBakedSize() const NN_NOEXCEPT
    {
        return ToData().bakedSize;
    }

    //@}

    //----------------------------------------
    //! @name ユーザーデータ
    //@{

    //! @brief ユーザーデータの数を取得します。
    //!
    //! @return ユーザーデータの数を返します。
    //!
    int GetUserDataCount() const NN_NOEXCEPT
    {
        return ToData().userDataCount;
    }
    //! @brief 指定した名前のユーザーデータを取得します。
    //!
    //! @param[in] str ユーザーデータ名。
    //!
    //! @return nn::gfx::ResUserData へのポインターを返します。
    //!
    //! @details
    //! 指定した名前のユーザーデータが無い場合は NULL を返します。
    //!
    //! 名前引きよりインデックス引きほうが高速に動作します。インデックスを
    //! 取得後、記録しておき、通常はインデックス引きを使用することを推奨します。
    //!
    nn::gfx::ResUserData* FindUserData(const char* str) NN_NOEXCEPT
    {
        const nn::util::ResDic* pDic = ToData().pUserDataDic.Get();
        if (pDic == NULL)
        {
            return NULL;
        }
        int index = pDic->FindIndex(str);
        if (index == nn::util::ResDic::Npos)
        {
            return NULL;
        }
        nn::gfx::ResUserData* pArray = ToData().pUserDataArray.Get();
        NN_SDK_ASSERT_NOT_NULL(pArray);
        return &pArray[index];
    }
    //! @brief 指定した名前のユーザーデータを取得します。
    //!
    //! @param[in] str ユーザーデータ名。
    //!
    //! @return nn::gfx::ResUserData へのポインターを返します。
    //!
    //! @details
    //! 指定した名前のユーザーデータが無い場合は NULL を返します。
    //!
    //! 名前引きよりインデックス引きほうが高速に動作します。インデックスを
    //! 取得後、記録しておき、通常はインデックス引きを使用することを推奨します。
    //!
    const nn::gfx::ResUserData* FindUserData(const char* str) const NN_NOEXCEPT
    {
        const nn::util::ResDic* pDic = ToData().pUserDataDic.Get();
        if (pDic == NULL)
        {
            return NULL;
        }
        int index = pDic->FindIndex(str);
        if (index == nn::util::ResDic::Npos)
        {
            return NULL;
        }
        const nn::gfx::ResUserData* pArray = ToData().pUserDataArray.Get();
        NN_SDK_ASSERT_NOT_NULL(pArray);
        return &pArray[index];
    }
    //! @brief 指定した名前のユーザーデータのインデックスを取得します。
    //!
    //! @param[in] str ユーザーデータ名。
    //!
    //! @return ユーザーデータのインデックスを返します。
    //!
    //! @details
    //! 指定した名前のユーザーデータが無い場合は nn::util::ResDic::Npos を返します。
    //!
    int FindUserDataIndex(const char* str) const NN_NOEXCEPT
    {
        const nn::util::ResDic* pDic = ToData().pUserDataDic.Get();
        if (pDic == NULL)
        {
            return nn::util::ResDic::Npos;
        }
        return pDic->FindIndex(str);
    }
    //! @brief ユーザーデータのインデックスから名前を取得します。
    //!
    //! @param[in] elemIndex ユーザーデータのインデックス。
    //!
    //! @return ユーザーデータ名へのポインターを返します。
    //!
    //! @pre
    //! - インデックスはユーザーデータの数の範囲内。
    //!
    //! @details
    //! 指定したインデックスが範囲外の場合は ASSERT します。
    //!
    const char* GetUserDataName(int elemIndex) const NN_NOEXCEPT
    {
        NN_G3D_REQUIRES_RANGE(elemIndex, 0, GetUserDataCount(), GetName());
        const nn::util::ResDic* pDic = ToData().pUserDataDic.Get();
        if (pDic == NULL)
        {
            return NULL;
        }
        nn::util::string_view pString = pDic->GetKey(elemIndex);
        return pString.data();
    }
    //! @brief インデックス引きでユーザーデータを取得します。
    //!
    //! @param[in] elemIndex ユーザーデータのインデックス。
    //!
    //! @return nn::gfx::ResUserData へのポインターを返します。
    //!
    //! @pre
    //! - インデックスはユーザーデータの数の範囲内。
    //!
    //! @details
    //! 指定したインデックスが範囲外の場合は ASSERT します。
    //!
    nn::gfx::ResUserData* GetUserData(int elemIndex) NN_NOEXCEPT
    {
        NN_G3D_REQUIRES_RANGE(elemIndex, 0, GetUserDataCount(), GetName());
        nn::gfx::ResUserData* pArray = ToData().pUserDataArray.Get();
        NN_SDK_ASSERT_NOT_NULL(pArray);
        return &pArray[elemIndex];
    }
    //! @brief インデックス引きでユーザーデータを取得します。
    //!
    //! @param[in] elemIndex ユーザーデータのインデックス。
    //!
    //! @return nn::gfx::ResUserData へのポインターを返します。
    //!
    //! @pre
    //! - インデックスはユーザーデータの数の範囲内。
    //!
    //! @details
    //! 指定したインデックスが範囲外の場合は ASSERT します。
    //!
    const nn::gfx::ResUserData* GetUserData(int elemIndex) const NN_NOEXCEPT
    {
        NN_G3D_REQUIRES_RANGE(elemIndex, 0, GetUserDataCount(), GetName());
        const nn::gfx::ResUserData* pArray = ToData().pUserDataArray.Get();
        NN_SDK_ASSERT_NOT_NULL(pArray);
        return &pArray[elemIndex];
    }

    //@}
};

//--------------------------------------------------------------------------------------------------

//! @brief ライトアニメーションの構造体です。
struct ResLightAnimData
{
    nn::util::BinBlockSignature         signature; //!< シグネチャー。
    Bit16                               flag; //!< フラグ。
    uint8_t                             reserved0[2];
    nn::util::BinPtrToString            pName; //!< 名前。
    nn::util::BinTPtr<ResAnimCurve>     pCurveArray; //!< アニメーションカーブ配列へのポインター。
    nn::util::BinTPtr<Bit32>            pBaseValueArray; //!< 初期値配列へのポインター。
    nn::util::BinTPtr<nn::gfx::ResUserData> pUserDataArray; //!< ユーザーデータ配列へのポインター。
    nn::util::BinTPtr<nn::util::ResDic> pUserDataDic; //!< ユーザーデータ辞書へのポインター。
    nn::util::BinPtrToString            pLightType; //!< ライトタイプ名。
    nn::util::BinPtrToString            pDistAttnFunc; //!< 距離減衰関数名。
    nn::util::BinPtrToString            pAngleAttnFunc; //!< 角度減衰関数名。
    int32_t  frameCount; //!< フレーム数。
    uint32_t bakedSize; //!< コマ化に必要なバッファーサイズ。
    uint16_t userDataCount; //!< ユーザーデータ数。
    uint8_t  curveCount; //!< アニメーションカーブ数。
    int8_t   lightTypeIndex; //!< ライトタイプインデックス。
    int8_t   distAttnFuncIndex; //!< 距離減衰関数インデックス。
    int8_t   angleAttnFuncIndex; //!< 角度減衰関数インデックス。
    uint8_t  reserved1[2];
};

//! @brief ライトアニメーションのリソースです。
class ResLightAnim : public nn::util::AccessorBase< ResLightAnimData >
{
    NN_DISALLOW_COPY(ResLightAnim);

public:
    //! @brief ResLightAnim のシグネチャ
    static const uint32_t Signature =  NN_UTIL_CREATE_SIGNATURE_4('F', 'L', 'I', 'T');

    //! @brief ライトアニメーションに関するフラグです。
    enum Flag
    {
        Flag_CurveBaked     = AnimFlag_CurveBaked, //!< @brief カーブがベイク済みです。
        Flag_PlayPolicyLoop = AnimFlag_PlayPolicyLoop, //!< @brief ループすることを表します。
        Flag_CurveEnable    = 0x1 << 8, //!< @brief ライトの有効無効のアニメーションが存在します。
        Flag_BaseEnable     = 0x1 << 9, //!< @brief ライトの有効無効の初期値が存在します。
        Flag_BasePos        = 0x1 << 10, //!< @brief 位置の初期値が存在します。
        Flag_BaseDir        = 0x1 << 11, //!< @brief 角度の初期値が存在します。
        Flag_BaseDistAttn   = 0x1 << 12, //!< @brief 距離減衰の初期値が存在します。
        Flag_BaseAngleAttn  = 0x1 << 13, //!< @brief 角度減衰の初期値が存在します。
        Flag_BaseColor0     = 0x1 << 14, //!< @brief カラー0の初期値が存在します。
        Flag_BaseColor1      = 0x1 << 15 //!< @brief カラー1の初期値が存在します。
    };

    //----------------------------------------
    //! @name 構築/破棄
    //@{

    //! @brief アニメーションの初期化を行います。
    //!
    //! @param[out] pResult アニメーション結果を格納するバッファーへのポインター。
    //!
    void Initialize(LightAnimResult* pResult) const NN_NOEXCEPT;

    //! @brief 名前が一致する関数と参照解決を行います。
    //!
    //! @param[in] funcTable 関数名とインデックスの変換テーブルのポインター。
    //!
    //! @return BindResult を返します。
    //!
    BindResult Bind(const BindFuncTable& funcTable) NN_NOEXCEPT;

    //! @brief 参照設定を解除します。
    void Release() NN_NOEXCEPT;

    //! @brief アニメーションをコマ化します。
    //!
    //! @param[out] pBuffer コマ化した結果を格納するバッファーへのポインター。
    //! @param[in] bufferSize コマ化した結果を格納するバッファーへのポインター。
    //!
    //! @return コマ化が成功した場合は true、失敗した場合は false を返します。
    //!
    //! @pre
    //! - bufferSize >= GetBakedSize()
    //!
    bool BakeCurve(void* pBuffer, size_t bufferSize) NN_NOEXCEPT;

    //! @brief アニメーションをカーブに戻します。
    //!
    //! @return BakeCurve() 時に渡したバッファーへのポインターを返します。
    //!
    void* ResetCurve() NN_NOEXCEPT;

    //! @brief アニメーションをリセットします。
    void Reset() NN_NOEXCEPT;

    //@}

    //----------------------------------------
    //! @name 評価
    //@{

    //! @brief アニメーションを評価します。
    //!
    //! @param[out] pResult アニメーション結果を格納するバッファーへのポインター。
    //! @param[in] frame フレーム位置。
    //!
    void Evaluate(LightAnimResult* pResult, float frame) const NN_NOEXCEPT;

    //! @brief アニメーションを評価します。
    //!
    //! @param[out] pResult アニメーション結果を格納するバッファーへのポインター。
    //! @param[in] frame フレーム位置。
    //! @param[in] pFrameCache 計算の効率化のために使用されるキャッシュへのポインター。
    //!
    void Evaluate(LightAnimResult* pResult, float frame, AnimFrameCache* pFrameCache) const NN_NOEXCEPT;

    //@}

    //----------------------------------------
    //! @name 取得
    //@{

    //! @brief アニメーション名を取得します
    //!
    //! @return アニメーション名へのポインター。
    //!
    const char* GetName() const NN_NOEXCEPT
    {
        return ToData().pName.Get()->GetData();
    }
    //! @brief ライトタイプ名を取得します。
    //!
    //! @return ライトタイプ名へのポインター。
    //!
    const char* GetLightType() const NN_NOEXCEPT
    {
        return ToData().pLightType.Get()->GetData();
    }
    //! @brief 距離減衰関数名を取得します。
    //!
    //! @return 距離減衰関数名へのポインター。
    //!
    const char* GetDistAttnFunc() const NN_NOEXCEPT
    {
        return ToData().pDistAttnFunc.Get()->GetData();
    }
    //! @brief 角度減衰関数名を取得します。
    //!
    //! @return 角度減衰関数名へのポインター。
    //!
    const char* GetAngleAttnFunc() const NN_NOEXCEPT
    {
        return ToData().pAngleAttnFunc.Get()->GetData();
    }

    //! @brief アニメーションカーブの数を取得します。
    //!
    //! @return アニメーションカーブの数を返します。
    //!
    int GetCurveCount() const NN_NOEXCEPT
    {
        return ToData().curveCount;
    }
    //! @brief インデックス引きでアニメーションカーブを取得します。
    //!
    //! @param[in] elemIndex アニメーションカーブのインデックス。
    //!
    //! @return ResAnimCurve へのポインターを返します。
    //!
    //! @pre
    //! - インデックスはアニメーションカーブの数の範囲内。
    //!
    //! @details
    //! 指定したインデックスが範囲外の場合は ASSERT します。
    //!
    ResAnimCurve* GetCurve(int elemIndex) NN_NOEXCEPT
    {
        NN_G3D_REQUIRES_RANGE(elemIndex, 0, GetCurveCount(), GetName());
        ResAnimCurve* pArray = ToData().pCurveArray.Get();
        NN_SDK_ASSERT_NOT_NULL(pArray);
        return &pArray[elemIndex];
    }
    //! @brief インデックス引きでアニメーションカーブを取得します。
    //!
    //! @param[in] elemIndex アニメーションカーブのインデックス。
    //!
    //! @return ResAnimCurve へのポインターを返します。
    //!
    //! @pre
    //! - インデックスはアニメーションカーブの数の範囲内。
    //!
    //! @details
    //! 指定したインデックスが範囲外の場合は ASSERT します。
    //!
    const ResAnimCurve* GetCurve(int elemIndex) const NN_NOEXCEPT
    {
        NN_G3D_REQUIRES_RANGE(elemIndex, 0, GetCurveCount(), GetName());
        const ResAnimCurve* pArray = ToData().pCurveArray.Get();
        NN_SDK_ASSERT_NOT_NULL(pArray);
        return &pArray[elemIndex];
    }

    //! @brief 事前に関連付けたライトタイプのインデックスを取得します。
    //!
    //! @return 事前に関連付けたライトタイプのインデックスを返します
    //!
    int GetLightTypeIndex() const NN_NOEXCEPT
    {
        return ToData().lightTypeIndex;
    }

    //! @brief 事前に関連付けた距離減衰関数のインデックスを取得します。
    //!
    //! @return 事前に関連付けた距離減衰関数のインデックスを返します
    //!
    int GetDistAttnFuncIndex() const NN_NOEXCEPT
    {
        return ToData().distAttnFuncIndex;
    }

    //! @brief 事前に関連付けた角度減衰関数のインデックスを取得します。
    //!
    //! @return 事前に関連付けた角度減衰関数のインデックスを返します
    //!
    int GetAngleAttnFuncIndex() const NN_NOEXCEPT
    {
        return ToData().angleAttnFuncIndex;
    }

    //! @brief ループするかどうかを取得します。
    //!
    //! @return ループする場合は true、しない場合は false を返します。
    //!
    bool IsLooped() const NN_NOEXCEPT
    {
        return 0 != (ToData().flag & Flag_PlayPolicyLoop);
    }

    //! @brief カーブがベイクされているかどうかを取得します。
    //!
    //! @return ベイクされている場合は true、されていない場合は false を返します。
    //!
    bool IsCurveBaked() const NN_NOEXCEPT
    {
        return 0 != (ToData().flag & Flag_CurveBaked);
    }

    //! @brief フレーム数を取得します。
    //!
    //! @return フレーム数を返します。
    //!
    int GetFrameCount() const NN_NOEXCEPT
    {
        return ToData().frameCount;
    }

    //! @brief コマ化に必要なバッファーサイズを取得します。
    //!
    //! @return コマ化に必要なバッファーサイズを返します。
    //!
    size_t GetBakedSize() const NN_NOEXCEPT
    {
        return ToData().bakedSize;
    }

    //@}

    //----------------------------------------
    //! @name ユーザーデータ
    //@{

    //! @brief ユーザーデータの数を取得します。
    //!
    //! @return ユーザーデータの数を返します。
    //!
    int GetUserDataCount() const NN_NOEXCEPT
    {
        return ToData().userDataCount;
    }
    //! @brief 指定した名前のユーザーデータを取得します。
    //!
    //! @param[in] str ユーザーデータ名。
    //!
    //! @return nn::gfx::ResUserData へのポインターを返します。
    //!
    //! @details
    //! 指定した名前のユーザーデータが無い場合は NULL を返します。
    //!
    //! 名前引きよりインデックス引きほうが高速に動作します。インデックスを
    //! 取得後、記録しておき、通常はインデックス引きを使用することを推奨します。
    //!
    nn::gfx::ResUserData* FindUserData(const char* str) NN_NOEXCEPT
    {
        const nn::util::ResDic* pDic = ToData().pUserDataDic.Get();
        if (pDic == NULL)
        {
            return NULL;
        }
        int index = pDic->FindIndex(str);
        if (index == nn::util::ResDic::Npos)
        {
            return NULL;
        }
        nn::gfx::ResUserData* pArray = ToData().pUserDataArray.Get();
        NN_SDK_ASSERT_NOT_NULL(pArray);
        return &pArray[index];
    }
    //! @brief 指定した名前のユーザーデータを取得します。
    //!
    //! @param[in] str ユーザーデータ名。
    //!
    //! @return nn::gfx::ResUserData へのポインターを返します。
    //!
    //! @details
    //! 指定した名前のユーザーデータが無い場合は NULL を返します。
    //!
    //! 名前引きよりインデックス引きほうが高速に動作します。インデックスを
    //! 取得後、記録しておき、通常はインデックス引きを使用することを推奨します。
    //!
    const nn::gfx::ResUserData* FindUserData(const char* str) const NN_NOEXCEPT
    {
        const nn::util::ResDic* pDic = ToData().pUserDataDic.Get();
        if (pDic == NULL)
        {
            return NULL;
        }
        int index = pDic->FindIndex(str);
        if (index == nn::util::ResDic::Npos)
        {
            return NULL;
        }
        const nn::gfx::ResUserData* pArray = ToData().pUserDataArray.Get();
        NN_SDK_ASSERT_NOT_NULL(pArray);
        return &pArray[index];
    }
    //! @brief 指定した名前のユーザーデータのインデックスを取得します。
    //!
    //! @param[in] str ユーザーデータ名。
    //!
    //! @return ユーザーデータのインデックスを返します。
    //!
    //! @details
    //! 指定した名前のユーザーデータが無い場合は nn::util::ResDic::Npos を返します。
    //!
    int FindUserDataIndex(const char* str) const NN_NOEXCEPT
    {
        const nn::util::ResDic* pDic = ToData().pUserDataDic.Get();
        if (pDic == NULL)
        {
            return nn::util::ResDic::Npos;
        }
        return pDic->FindIndex(str);
    }
    //! @brief ユーザーデータのインデックスから名前を取得します。
    //!
    //! @param[in] elemIndex ユーザーデータのインデックス。
    //!
    //! @return ユーザーデータ名へのポインターを返します。
    //!
    //! @pre
    //! - インデックスはユーザーデータの数の範囲内。
    //!
    //! @details
    //! 指定したインデックスが範囲外の場合は ASSERT します。
    //!
    const char* GetUserDataName(int elemIndex) const NN_NOEXCEPT
    {
        NN_G3D_REQUIRES_RANGE(elemIndex, 0, GetUserDataCount(), GetName());
        const nn::util::ResDic* pDic = ToData().pUserDataDic.Get();
        if (pDic == NULL)
        {
            return NULL;
        }
        nn::util::string_view pString = pDic->GetKey(elemIndex);
        return pString.data();
    }
    //! @brief インデックス引きでユーザーデータを取得します。
    //!
    //! @param[in] elemIndex ユーザーデータのインデックス。
    //!
    //! @return nn::gfx::ResUserData へのポインターを返します。
    //!
    //! @pre
    //! - インデックスはユーザーデータの数の範囲内。
    //!
    //! @details
    //! 指定したインデックスが範囲外の場合は ASSERT します。
    //!
    nn::gfx::ResUserData* GetUserData(int elemIndex) NN_NOEXCEPT
    {
        NN_G3D_REQUIRES_RANGE(elemIndex, 0, GetUserDataCount(), GetName());
        nn::gfx::ResUserData* pArray = ToData().pUserDataArray.Get();
        NN_SDK_ASSERT_NOT_NULL(pArray);
        return &pArray[elemIndex];
    }
    //! @brief インデックス引きでユーザーデータを取得します。
    //!
    //! @param[in] elemIndex ユーザーデータのインデックス。
    //!
    //! @return nn::gfx::ResUserData へのポインターを返します。
    //!
    //! @pre
    //! - インデックスはユーザーデータの数の範囲内。
    //!
    //! @details
    //! 指定したインデックスが範囲外の場合は ASSERT します。
    //!
    const nn::gfx::ResUserData* GetUserData(int elemIndex) const NN_NOEXCEPT
    {
        NN_G3D_REQUIRES_RANGE(elemIndex, 0, GetUserDataCount(), GetName());
        const nn::gfx::ResUserData* pArray = ToData().pUserDataArray.Get();
        NN_SDK_ASSERT_NOT_NULL(pArray);
        return &pArray[elemIndex];
    }

    //@}
};

//--------------------------------------------------------------------------------------------------

//! @brief フォグアニメーションの構造体です。
struct ResFogAnimData
{
    nn::util::BinBlockSignature         signature; //!< シグネチャー。
    Bit16                               flag; //!< フラグ。
    uint8_t                             reserved0[2];
    nn::util::BinPtrToString            pName; //!< 名前。
    nn::util::BinTPtr<ResAnimCurve>     pCurveArray; //!< アニメーションカーブ配列へのポインター。
    nn::util::BinTPtr<FogAnimResult>    pBaseValueArray; //!< 初期値配列へのポインター。
    nn::util::BinTPtr<nn::gfx::ResUserData> pUserDataArray; //!< ユーザーデータ配列へのポインター。
    nn::util::BinTPtr<nn::util::ResDic> pUserDataDic; //!< ユーザーデータ辞書へのポインター。
    nn::util::BinPtrToString            pDistAttnFunc; //!< 距離減衰関数名。
    int32_t  frameCount; //!< フレーム数。
    uint32_t bakedSize; //!< コマ化に必要なバッファーサイズ。
    uint16_t userDataCount; //!< ユーザーデータ数。
    uint8_t  curveCount; //!< アニメーションカーブ数。
    int8_t   distAttnFuncIndex; //!< 距離減衰関数インデックス。
    uint8_t  reserved1[4];
};

//! @brief フォグアニメーションのリソースです。
class ResFogAnim : public nn::util::AccessorBase< ResFogAnimData >
{
    NN_DISALLOW_COPY(ResFogAnim);

public:
    //! @brief ResFogAnim のシグネチャ
    static const uint32_t Signature = NN_UTIL_CREATE_SIGNATURE_4('F', 'F', 'O', 'G');

    //! @brief フォグアニメーションに関するフラグです。
    enum Flag
    {
        Flag_CurveBaked     = AnimFlag_CurveBaked, //!< @brief カーブがベイク済みです。
        Flag_PlayPolicyLoop = AnimFlag_PlayPolicyLoop //!< @brief ループすることを表します。
    };

    //----------------------------------------
    //! @name 構築/破棄
    //@{

    //! @brief アニメーションの初期化を行います。
    //!
    //! @param[out] pResult アニメーション結果を格納するバッファーへのポインター。
    //!
    void Initialize(FogAnimResult* pResult) const NN_NOEXCEPT;

    //! @brief 名前が一致する関数と参照解決を行います。
    //!
    //! @param[in] funcTable 関数名とインデックスの変換テーブルのポインター。
    //!
    //! @return BindResult を返します。
    //!
    BindResult Bind(const BindFuncTable& funcTable) NN_NOEXCEPT;

    //! @brief 参照設定を解除します。
    void Release() NN_NOEXCEPT;

    //! @brief アニメーションをコマ化します。
    //!
    //! @param[out] pBuffer コマ化した結果を格納するバッファーへのポインター。
    //! @param[in] bufferSize コマ化した結果を格納するバッファーへのポインター。
    //!
    //! @return コマ化が成功した場合は true、失敗した場合は false を返します。
    //!
    //! @pre
    //! - bufferSize >= GetBakedSize()
    //!
    bool BakeCurve(void* pBuffer, size_t bufferSize) NN_NOEXCEPT;

    //! @brief アニメーションをカーブに戻します。
    //!
    //! @return BakeCurve() 時に渡したバッファーへのポインターを返します。
    //!
    void* ResetCurve() NN_NOEXCEPT;

    //! @brief アニメーションをリセットします。
    void Reset() NN_NOEXCEPT;

    //@}

    //----------------------------------------
    //! @name 評価
    //@{

    //! @brief アニメーションを評価します。
    //!
    //! @param[out] pResult アニメーション結果を格納するバッファーへのポインター。
    //! @param[in] frame フレーム位置。
    //!
    void Evaluate(FogAnimResult* pResult, float frame) const NN_NOEXCEPT;

    //! @brief アニメーションを評価します。
    //!
    //! @param[out] pResult アニメーション結果を格納するバッファーへのポインター。
    //! @param[in] frame フレーム位置。
    //! @param[in] pFrameCache 計算の効率化のために使用されるキャッシュへのポインター。
    //!
    void Evaluate(FogAnimResult* pResult, float frame, AnimFrameCache* pFrameCache) const NN_NOEXCEPT;

    //@}

    //----------------------------------------
    //! @name 取得
    //@{

    //! @brief アニメーション名を取得します
    //!
    //! @return アニメーション名へのポインター。
    //!
    const char* GetName() const NN_NOEXCEPT
    {
        return ToData().pName.Get()->GetData();
    }
    //! @brief 距離減衰関数名を取得します。
    //!
    //! @return 距離減衰関数名へのポインター。
    //!
    const char* GetDistAttnFunc() const NN_NOEXCEPT
    {
        return ToData().pDistAttnFunc.Get()->GetData();
    }

    //! @brief アニメーションカーブの数を取得します。
    //!
    //! @return アニメーションカーブの数を返します。
    //!
    int GetCurveCount() const NN_NOEXCEPT
    {
        return ToData().curveCount;
    }
    //! @brief インデックス引きでアニメーションカーブを取得します。
    //!
    //! @param[in] elemIndex アニメーションカーブのインデックス。
    //!
    //! @return ResAnimCurve へのポインターを返します。
    //!
    //! @pre
    //! - インデックスはアニメーションカーブの数の範囲内。
    //!
    //! @details
    //! 指定したインデックスが範囲外の場合は ASSERT します。
    //!
    ResAnimCurve* GetCurve(int elemIndex) NN_NOEXCEPT
    {
        NN_G3D_REQUIRES_RANGE(elemIndex, 0, GetCurveCount(), GetName());
        ResAnimCurve* pArray = ToData().pCurveArray.Get();
        NN_SDK_ASSERT_NOT_NULL(pArray);
        return &pArray[elemIndex];
    }
    //! @brief インデックス引きでアニメーションカーブを取得します。
    //!
    //! @param[in] elemIndex アニメーションカーブのインデックス。
    //!
    //! @return ResAnimCurve へのポインターを返します。
    //!
    //! @pre
    //! - インデックスはアニメーションカーブの数の範囲内。
    //!
    //! @details
    //! 指定したインデックスが範囲外の場合は ASSERT します。
    //!
    const ResAnimCurve* GetCurve(int elemIndex) const NN_NOEXCEPT
    {
        NN_G3D_REQUIRES_RANGE(elemIndex, 0, GetCurveCount(), GetName());
        const ResAnimCurve* pArray = ToData().pCurveArray.Get();
        NN_SDK_ASSERT_NOT_NULL(pArray);
        return &pArray[elemIndex];
    }

    //! @brief 事前に関連付けた距離減衰関数のインデックスを取得します。
    //!
    //! @return 事前に関連付けた距離減衰関数のインデックスを返します
    //!
    int GetDistAttnFuncIndex() const NN_NOEXCEPT
    {
        return ToData().distAttnFuncIndex;
    }

    //! @brief ループするかどうかを取得します。
    //!
    //! @return ループする場合は true、しない場合は false を返します。
    //!
    bool IsLooped() const NN_NOEXCEPT
    {
        return 0 != (ToData().flag & Flag_PlayPolicyLoop);
    }

    //! @brief カーブがベイクされているかどうかを取得します。
    //!
    //! @return ベイクされている場合は true、されていない場合は false を返します。
    //!
    bool IsCurveBaked() const NN_NOEXCEPT
    {
        return 0 != (ToData().flag & Flag_CurveBaked);
    }

    //! @brief フレーム数を取得します。
    //!
    //! @return フレーム数を返します。
    //!
    int GetFrameCount() const NN_NOEXCEPT
    {
        return ToData().frameCount;
    }

    //! @brief コマ化に必要なバッファーサイズを取得します。
    //!
    //! @return コマ化に必要なバッファーサイズを返します。
    //!
    size_t GetBakedSize() const NN_NOEXCEPT
    {
        return ToData().bakedSize;
    }

    //@}

    //----------------------------------------
    //! @name ユーザーデータ
    //@{

    //! @brief ユーザーデータの数を取得します。
    //!
    //! @return ユーザーデータの数を返します。
    //!
    int GetUserDataCount() const NN_NOEXCEPT
    {
        return ToData().userDataCount;
    }
    //! @brief 指定した名前のユーザーデータを取得します。
    //!
    //! @param[in] str ユーザーデータ名。
    //!
    //! @return nn::gfx::ResUserData へのポインターを返します。
    //!
    //! @details
    //! 指定した名前のユーザーデータが無い場合は NULL を返します。
    //!
    //! 名前引きよりインデックス引きほうが高速に動作します。インデックスを
    //! 取得後、記録しておき、通常はインデックス引きを使用することを推奨します。
    //!
    nn::gfx::ResUserData* FindUserData(const char* str) NN_NOEXCEPT
    {
        const nn::util::ResDic* pDic = ToData().pUserDataDic.Get();
        if (pDic == NULL)
        {
            return NULL;
        }
        int index = pDic->FindIndex(str);
        if (index == nn::util::ResDic::Npos)
        {
            return NULL;
        }
        nn::gfx::ResUserData* pArray = ToData().pUserDataArray.Get();
        NN_SDK_ASSERT_NOT_NULL(pArray);
        return &pArray[index];
    }
    //! @brief 指定した名前のユーザーデータを取得します。
    //!
    //! @param[in] str ユーザーデータ名。
    //!
    //! @return nn::gfx::ResUserData へのポインターを返します。
    //!
    //! @details
    //! 指定した名前のユーザーデータが無い場合は NULL を返します。
    //!
    //! 名前引きよりインデックス引きほうが高速に動作します。インデックスを
    //! 取得後、記録しておき、通常はインデックス引きを使用することを推奨します。
    //!
    const nn::gfx::ResUserData* FindUserData(const char* str) const NN_NOEXCEPT
    {
        const nn::util::ResDic* pDic = ToData().pUserDataDic.Get();
        if (pDic == NULL)
        {
            return NULL;
        }
        int index = pDic->FindIndex(str);
        if (index == nn::util::ResDic::Npos)
        {
            return NULL;
        }
        const nn::gfx::ResUserData* pArray = ToData().pUserDataArray.Get();
        NN_SDK_ASSERT_NOT_NULL(pArray);
        return &pArray[index];
    }
    //! @brief 指定した名前のユーザーデータのインデックスを取得します。
    //!
    //! @param[in] str ユーザーデータ名。
    //!
    //! @return ユーザーデータのインデックスを返します。
    //!
    //! @details
    //! 指定した名前のユーザーデータが無い場合は nn::util::ResDic::Npos を返します。
    //!
    int FindUserDataIndex(const char* str) const NN_NOEXCEPT
    {
        const nn::util::ResDic* pDic = ToData().pUserDataDic.Get();
        if (pDic == NULL)
        {
            return nn::util::ResDic::Npos;
        }
        return pDic->FindIndex(str);
    }
    //! @brief ユーザーデータのインデックスから名前を取得します。
    //!
    //! @param[in] elemIndex ユーザーデータのインデックス。
    //!
    //! @return ユーザーデータ名へのポインターを返します。
    //!
    //! @pre
    //! - インデックスはユーザーデータの数の範囲内。
    //!
    //! @details
    //! 指定したインデックスが範囲外の場合は ASSERT します。
    //!
    const char* GetUserDataName(int elemIndex) const NN_NOEXCEPT
    {
        NN_G3D_REQUIRES_RANGE(elemIndex, 0, GetUserDataCount(), GetName());
        const nn::util::ResDic* pDic = ToData().pUserDataDic.Get();
        if (pDic == NULL)
        {
            return NULL;
        }
        nn::util::string_view pString = pDic->GetKey(elemIndex);
        return pString.data();
    }
    //! @brief インデックス引きでユーザーデータを取得します。
    //!
    //! @param[in] elemIndex ユーザーデータのインデックス。
    //!
    //! @return nn::gfx::ResUserData へのポインターを返します。
    //!
    //! @pre
    //! - インデックスはユーザーデータの数の範囲内。
    //!
    //! @details
    //! 指定したインデックスが範囲外の場合は ASSERT します。
    //!
    nn::gfx::ResUserData* GetUserData(int elemIndex) NN_NOEXCEPT
    {
        NN_G3D_REQUIRES_RANGE(elemIndex, 0, GetUserDataCount(), GetName());
        nn::gfx::ResUserData* pArray = ToData().pUserDataArray.Get();
        NN_SDK_ASSERT_NOT_NULL(pArray);
        return &pArray[elemIndex];
    }
    //! @brief インデックス引きでユーザーデータを取得します。
    //!
    //! @param[in] elemIndex ユーザーデータのインデックス。
    //!
    //! @return nn::gfx::ResUserData へのポインターを返します。
    //!
    //! @pre
    //! - インデックスはユーザーデータの数の範囲内。
    //!
    //! @details
    //! 指定したインデックスが範囲外の場合は ASSERT します。
    //!
    const nn::gfx::ResUserData* GetUserData(int elemIndex) const NN_NOEXCEPT
    {
        NN_G3D_REQUIRES_RANGE(elemIndex, 0, GetUserDataCount(), GetName());
        const nn::gfx::ResUserData* pArray = ToData().pUserDataArray.Get();
        NN_SDK_ASSERT_NOT_NULL(pArray);
        return &pArray[elemIndex];
    }

    //@}
};

//--------------------------------------------------------------------------------------------------

//! @brief シーンアニメーションの構造体です。
struct ResSceneAnimData
{
    nn::util::BinBlockSignature         signature; //!< シグネチャー。
    uint8_t                             reserved[4];
    nn::util::BinPtrToString            pName; //!< 名前。
    nn::util::BinPtrToString            pPath; //!< パス名。
    nn::util::BinTPtr<ResCameraAnim>    pCameraAnimArray; //!< カメラニメーション配列へのポインター。
    nn::util::BinTPtr<nn::util::ResDic> pCameraAnimDic; //!< カメラニメーション辞書へのポインター。
    nn::util::BinTPtr<ResLightAnim>     pLightAnimArray; //!< ライトニメーション配列へのポインター。
    nn::util::BinTPtr<nn::util::ResDic> pLightAnimDic; //!< ライトニメーション辞書へのポインター。
    nn::util::BinTPtr<ResFogAnim>       pFogAnimArray; //!< フォグニメーション配列へのポインター。
    nn::util::BinTPtr<nn::util::ResDic> pFogAnimDic; //!< フォグニメーション辞書へのポインター。
    nn::util::BinTPtr<nn::gfx::ResUserData> pUserDataArray; //!< ユーザーデータ配列へのポインター。
    nn::util::BinTPtr<nn::util::ResDic> pUserDataDic; //!< ユーザーデータ辞書へのポインター。

    uint16_t userDataCount; //!< ユーザーデータ数。
    uint16_t cameraAnimCount; //!< カメラニメーション数。
    uint16_t lightAnimCount; //!< ライトニメーション数。
    uint16_t fogAnimCount; //!< フォグニメーション数。
};

//! @brief シーンアニメーションのリソースです。
class ResSceneAnim : public nn::util::AccessorBase< ResSceneAnimData >
{
    NN_DISALLOW_COPY(ResSceneAnim);

public:
    //! @brief ResSceneAnim のシグネチャ
    static const uint32_t Signature = NN_UTIL_CREATE_SIGNATURE_4('F', 'S', 'C', 'N');

    //----------------------------------------
    //! @name 構築/破棄
    //@{

    //! @brief 名前が一致する関数と参照解決を行います。
    //!
    //! @param[in] funcTable 関数名とインデックスの変換テーブルのポインター。
    //!
    //! @return BindResult を返します。
    //!
    BindResult Bind(const BindFuncTable& funcTable) NN_NOEXCEPT;

    //! @brief 参照設定を解除します。
    void Release() NN_NOEXCEPT;

    //! @brief アニメーションをリセットします。
    void Reset() NN_NOEXCEPT;

    //@}

    //----------------------------------------
    //! @name 取得
    //@{

    //! @brief アニメーション名を取得します
    //!
    //! @return アニメーション名へのポインター。
    //!
    const char* GetName() const NN_NOEXCEPT
    {
        return ToData().pName.Get()->GetData();
    }
    //! @brief パス名を取得します
    //!
    //! @return パス名へのポインター。
    //!
    const char* GetPath() const NN_NOEXCEPT
    {
        return ToData().pPath.Get()->GetData();
    }

    //@}

    //----------------------------------------
    //! @name カメラアニメーション
    //@{

    //! @brief カメラアニメーションの数を取得します。
    //!
    //! @return カメラアニメーションの数を返します。
    //!
    int GetCameraAnimCount() const NN_NOEXCEPT
    {
        return ToData().cameraAnimCount;
    }
    //! @brief 指定した名前のカメラアニメーションを取得します。
    //!
    //! @param[in] str カメラアニメーション名。
    //!
    //! @return ResCameraAnim へのポインターを返します。
    //!
    //! @details
    //! 指定した名前のカメラアニメーションが無い場合は NULL を返します。
    //!
    //! 名前引きよりインデックス引きほうが高速に動作します。インデックスを
    //! 取得後、記録しておき、通常はインデックス引きを使用することを推奨します。
    //!
    ResCameraAnim* FindCameraAnim(const char* str) NN_NOEXCEPT
    {
        const nn::util::ResDic* pDic = ToData().pCameraAnimDic.Get();
        if (pDic == NULL)
        {
            return NULL;
        }
        int index = pDic->FindIndex(str);
        if (index == nn::util::ResDic::Npos)
        {
            return NULL;
        }
        ResCameraAnim* pArray = ToData().pCameraAnimArray.Get();
        NN_SDK_ASSERT_NOT_NULL(pArray);
        return &pArray[index];
    }
    //! @brief 指定した名前のカメラアニメーションを取得します。
    //!
    //! @param[in] str カメラアニメーション名。
    //!
    //! @return ResCameraAnim へのポインターを返します。
    //!
    //! @details
    //! 指定した名前のカメラアニメーションが無い場合は NULL を返します。
    //!
    //! 名前引きよりインデックス引きほうが高速に動作します。インデックスを
    //! 取得後、記録しておき、通常はインデックス引きを使用することを推奨します。
    //!
    const ResCameraAnim* FindCameraAnim(const char* str) const NN_NOEXCEPT
    {
        const nn::util::ResDic* pDic = ToData().pCameraAnimDic.Get();
        if (pDic == NULL)
        {
            return NULL;
        }
        int index = pDic->FindIndex(str);
        if (index == nn::util::ResDic::Npos)
        {
            return NULL;
        }
        const ResCameraAnim* pArray = ToData().pCameraAnimArray.Get();
        NN_SDK_ASSERT_NOT_NULL(pArray);
        return &pArray[index];
    }
    //! @brief 指定した名前のカメラアニメーションのインデックスを取得します。
    //!
    //! @param[in] str カメラアニメーション名。
    //!
    //! @return カメラアニメーションのインデックスを返します。
    //!
    //! @details
    //! 指定した名前のカメラアニメーションが無い場合は nn::util::ResDic::Npos を返します。
    //!
    int FindCameraAnimIndex(const char* str) const NN_NOEXCEPT
    {
        const nn::util::ResDic* pDic = ToData().pCameraAnimDic.Get();
        if (pDic == NULL)
        {
            return nn::util::ResDic::Npos;
        }
        return pDic->FindIndex(str);
    }
    //! @brief カメラアニメーションのインデックスから名前を取得します。
    //!
    //! @param[in] elemIndex カメラアニメーションのインデックス。
    //!
    //! @return カメラアニメーション名へのポインターを返します。
    //!
    //! @pre
    //! - インデックスはカメラアニメーションの数の範囲内。
    //!
    //! @details
    //! 指定したインデックスが範囲外の場合は ASSERT します。
    //!
    const char* GetCameraAnimName(int elemIndex) const NN_NOEXCEPT
    {
        NN_G3D_REQUIRES_RANGE(elemIndex, 0, GetCameraAnimCount(), GetName());
        const nn::util::ResDic* pDic = ToData().pCameraAnimDic.Get();
        if (pDic == NULL)
        {
            return NULL;
        }
        nn::util::string_view pString = pDic->GetKey(elemIndex);
        return pString.data();
    }
    //! @brief インデックス引きでカメラアニメーションを取得します。
    //!
    //! @param[in] elemIndex カメラアニメーションのインデックス。
    //!
    //! @return ResCameraAnim へのポインターを返します。
    //!
    //! @pre
    //! - インデックスはカメラアニメーションの数の範囲内。
    //!
    //! @details
    //! 指定したインデックスが範囲外の場合は ASSERT します。
    //!
    ResCameraAnim* GetCameraAnim(int elemIndex) NN_NOEXCEPT
    {
        NN_G3D_REQUIRES_RANGE(elemIndex, 0, GetCameraAnimCount(), GetName());
        ResCameraAnim* pArray = ToData().pCameraAnimArray.Get();
        NN_SDK_ASSERT_NOT_NULL(pArray);
        return &pArray[elemIndex];
    }
    //! @brief インデックス引きでカメラアニメーションを取得します。
    //!
    //! @param[in] elemIndex カメラアニメーションのインデックス。
    //!
    //! @return ResCameraAnim へのポインターを返します。
    //!
    //! @pre
    //! - インデックスはカメラアニメーションの数の範囲内。
    //!
    //! @details
    //! 指定したインデックスが範囲外の場合は ASSERT します。
    //!
    const ResCameraAnim* GetCameraAnim(int elemIndex) const NN_NOEXCEPT
    {
        NN_G3D_REQUIRES_RANGE(elemIndex, 0, GetCameraAnimCount(), GetName());
        const ResCameraAnim* pArray = ToData().pCameraAnimArray.Get();
        NN_SDK_ASSERT_NOT_NULL(pArray);
        return &pArray[elemIndex];
    }

    //@}

    //----------------------------------------
    //! @name ライトアニメーション
    //@{

    //! @brief ライトアニメーションの数を取得します。
    //!
    //! @return ライトアニメーションの数を返します。
    //!
    int GetLightAnimCount() const NN_NOEXCEPT
    {
        return ToData().lightAnimCount;
    }
    //! @brief 指定した名前のライトアニメーションを取得します。
    //!
    //! @param[in] str ライトアニメーション名。
    //!
    //! @return ResLightAnim へのポインターを返します。
    //!
    //! @details
    //! 指定した名前のライトアニメーションが無い場合は NULL を返します。
    //!
    //! 名前引きよりインデックス引きほうが高速に動作します。インデックスを
    //! 取得後、記録しておき、通常はインデックス引きを使用することを推奨します。
    //!
    ResLightAnim* FindLightAnim(const char* str) NN_NOEXCEPT
    {
        const nn::util::ResDic* pDic = ToData().pLightAnimDic.Get();
        if (pDic == NULL)
        {
            return NULL;
        }
        int index = pDic->FindIndex(str);
        if (index == nn::util::ResDic::Npos)
        {
            return NULL;
        }
        ResLightAnim* pArray = ToData().pLightAnimArray.Get();
        NN_SDK_ASSERT_NOT_NULL(pArray);
        return &pArray[index];
    }
    //! @brief 指定した名前のライトアニメーションを取得します。
    //!
    //! @param[in] str ライトアニメーション名。
    //!
    //! @return ResLightAnim へのポインターを返します。
    //!
    //! @details
    //! 指定した名前のライトアニメーションが無い場合は NULL を返します。
    //!
    //! 名前引きよりインデックス引きほうが高速に動作します。インデックスを
    //! 取得後、記録しておき、通常はインデックス引きを使用することを推奨します。
    //!
    const ResLightAnim* FindLightAnim(const char* str) const NN_NOEXCEPT
    {
        const nn::util::ResDic* pDic = ToData().pLightAnimDic.Get();
        if (pDic == NULL)
        {
            return NULL;
        }
        int index = pDic->FindIndex(str);
        if (index == nn::util::ResDic::Npos)
        {
            return NULL;
        }
        const ResLightAnim* pArray = ToData().pLightAnimArray.Get();
        NN_SDK_ASSERT_NOT_NULL(pArray);
        return &pArray[index];
    }
    //! @brief 指定した名前のライトアニメーションのインデックスを取得します。
    //!
    //! @param[in] str ライトアニメーション名。
    //!
    //! @return ライトアニメーションのインデックスを返します。
    //!
    //! @details
    //! 指定した名前のライトアニメーションが無い場合は nn::util::ResDic::Npos を返します。
    //!
    int FindLightAnimIndex(const char* str) const NN_NOEXCEPT
    {
        const nn::util::ResDic* pDic = ToData().pLightAnimDic.Get();
        if (pDic == NULL)
        {
            return nn::util::ResDic::Npos;
        }
        return pDic->FindIndex(str);
    }
    //! @brief ライトアニメーションのインデックスから名前を取得します。
    //!
    //! @param[in] elemIndex ライトアニメーションのインデックス。
    //!
    //! @return ライトアニメーション名へのポインターを返します。
    //!
    //! @pre
    //! - インデックスはライトアニメーションの数の範囲内。
    //!
    //! @details
    //! 指定したインデックスが範囲外の場合は ASSERT します。
    //!
    const char* GetLightAnimName(int elemIndex) const NN_NOEXCEPT
    {
        NN_G3D_REQUIRES_RANGE(elemIndex, 0, GetLightAnimCount(), GetName());
        const nn::util::ResDic* pDic = ToData().pLightAnimDic.Get();
        if (pDic == NULL)
        {
            return NULL;
        }
        nn::util::string_view pString = pDic->GetKey(elemIndex);
        return pString.data();
    }
    //! @brief インデックス引きでライトアニメーションを取得します。
    //!
    //! @param[in] elemIndex ライトアニメーションのインデックス。
    //!
    //! @return ResLightAnim へのポインターを返します。
    //!
    //! @pre
    //! - インデックスはライトアニメーションの数の範囲内。
    //!
    //! @details
    //! 指定したインデックスが範囲外の場合は ASSERT します。
    //!
    ResLightAnim* GetLightAnim(int elemIndex) NN_NOEXCEPT
    {
        NN_G3D_REQUIRES_RANGE(elemIndex, 0, GetLightAnimCount(), GetName());
        ResLightAnim* pArray = ToData().pLightAnimArray.Get();
        NN_SDK_ASSERT_NOT_NULL(pArray);
        return &pArray[elemIndex];
    }
    //! @brief インデックス引きでライトアニメーションを取得します。
    //!
    //! @param[in] elemIndex ライトアニメーションのインデックス。
    //!
    //! @return ResLightAnim へのポインターを返します。
    //!
    //! @pre
    //! - インデックスはライトアニメーションの数の範囲内。
    //!
    //! @details
    //! 指定したインデックスが範囲外の場合は ASSERT します。
    //!
    const ResLightAnim* GetLightAnim(int elemIndex) const NN_NOEXCEPT
    {
        NN_G3D_REQUIRES_RANGE(elemIndex, 0, GetLightAnimCount(), GetName());
        const ResLightAnim* pArray = ToData().pLightAnimArray.Get();
        NN_SDK_ASSERT_NOT_NULL(pArray);
        return &pArray[elemIndex];
    }

    //@}

    //----------------------------------------
    //! @name フォグアニメーション
    //@{

    //! @brief フォグアニメーションの数を取得します。
    //!
    //! @return フォグアニメーションの数を返します。
    //!
    int GetFogAnimCount() const NN_NOEXCEPT
    {
        return ToData().fogAnimCount;
    }
    //! @brief 指定した名前のフォグアニメーションを取得します。
    //!
    //! @param[in] str フォグアニメーション名。
    //!
    //! @return ResFogAnim へのポインターを返します。
    //!
    //! @details
    //! 指定した名前のフォグアニメーションが無い場合は NULL を返します。
    //!
    //! 名前引きよりインデックス引きほうが高速に動作します。インデックスを
    //! 取得後、記録しておき、通常はインデックス引きを使用することを推奨します。
    //!
    ResFogAnim* FindFogAnim(const char* str) NN_NOEXCEPT
    {
        const nn::util::ResDic* pDic = ToData().pFogAnimDic.Get();
        if (pDic == NULL)
        {
            return NULL;
        }
        int index = pDic->FindIndex(str);
        if (index == nn::util::ResDic::Npos)
        {
            return NULL;
        }
        ResFogAnim* pArray = ToData().pFogAnimArray.Get();
        NN_SDK_ASSERT_NOT_NULL(pArray);
        return &pArray[index];
    }
    //! @brief 指定した名前のフォグアニメーションを取得します。
    //!
    //! @param[in] str フォグアニメーション名。
    //!
    //! @return ResFogAnim へのポインターを返します。
    //!
    //! @details
    //! 指定した名前のフォグアニメーションが無い場合は NULL を返します。
    //!
    //! 名前引きよりインデックス引きほうが高速に動作します。インデックスを
    //! 取得後、記録しておき、通常はインデックス引きを使用することを推奨します。
    //!
    const ResFogAnim* FindFogAnim(const char* str) const NN_NOEXCEPT
    {
        const nn::util::ResDic* pDic = ToData().pFogAnimDic.Get();
        if (pDic == NULL)
        {
            return NULL;
        }
        int index = pDic->FindIndex(str);
        if (index == nn::util::ResDic::Npos)
        {
            return NULL;
        }
        const ResFogAnim* pArray = ToData().pFogAnimArray.Get();
        NN_SDK_ASSERT_NOT_NULL(pArray);
        return &pArray[index];
    }
    //! @brief 指定した名前のフォグアニメーションのインデックスを取得します。
    //!
    //! @param[in] str フォグアニメーション名。
    //!
    //! @return フォグアニメーションのインデックスを返します。
    //!
    //! @details
    //! 指定した名前のフォグアニメーションが無い場合は nn::util::ResDic::Npos を返します。
    //!
    int FindFogAnimIndex(const char* str) const NN_NOEXCEPT
    {
        const nn::util::ResDic* pDic = ToData().pFogAnimDic.Get();
        if (pDic == NULL)
        {
            return nn::util::ResDic::Npos;
        }
        return pDic->FindIndex(str);
    }
    //! @brief フォグアニメーションのインデックスから名前を取得します。
    //!
    //! @param[in] elemIndex フォグアニメーションのインデックス。
    //!
    //! @return フォグアニメーション名へのポインターを返します。
    //!
    //! @pre
    //! - インデックスはフォグアニメーションの数の範囲内。
    //!
    //! @details
    //! 指定したインデックスが範囲外の場合は ASSERT します。
    //!
    const char* GetFogAnimName(int elemIndex) const NN_NOEXCEPT
    {
        NN_G3D_REQUIRES_RANGE(elemIndex, 0, GetFogAnimCount(), GetName());
        const nn::util::ResDic* pDic = ToData().pFogAnimDic.Get();
        if (pDic == NULL)
        {
            return NULL;
        }
        nn::util::string_view pString = pDic->GetKey(elemIndex);
        return pString.data();
    }
    //! @brief インデックス引きでフォグアニメーションを取得します。
    //!
    //! @param[in] elemIndex フォグアニメーションのインデックス。
    //!
    //! @return ResFogAnim へのポインターを返します。
    //!
    //! @pre
    //! - インデックスはフォグアニメーションの数の範囲内。
    //!
    //! @details
    //! 指定したインデックスが範囲外の場合は ASSERT します。
    //!
    ResFogAnim* GetFogAnim(int elemIndex) NN_NOEXCEPT
    {
        NN_G3D_REQUIRES_RANGE(elemIndex, 0, GetFogAnimCount(), GetName());
        ResFogAnim* pArray = ToData().pFogAnimArray.Get();
        NN_SDK_ASSERT_NOT_NULL(pArray);
        return &pArray[elemIndex];
    }
    //! @brief インデックス引きでフォグアニメーションを取得します。
    //!
    //! @param[in] elemIndex フォグアニメーションのインデックス。
    //!
    //! @return ResFogAnim へのポインターを返します。
    //!
    //! @pre
    //! - インデックスはフォグアニメーションの数の範囲内。
    //!
    //! @details
    //! 指定したインデックスが範囲外の場合は ASSERT します。
    //!
    const ResFogAnim* GetFogAnim(int elemIndex) const NN_NOEXCEPT
    {
        NN_G3D_REQUIRES_RANGE(elemIndex, 0, GetFogAnimCount(), GetName());
        const ResFogAnim* pArray = ToData().pFogAnimArray.Get();
        NN_SDK_ASSERT_NOT_NULL(pArray);
        return &pArray[elemIndex];
    }

    //@}

    //----------------------------------------
    //! @name ユーザーデータ
    //@{

    //! @brief ユーザーデータの数を取得します。
    //!
    //! @return ユーザーデータの数を返します。
    //!
    int GetUserDataCount() const NN_NOEXCEPT
    {
        return ToData().userDataCount;
    }
    //! @brief 指定した名前のユーザーデータを取得します。
    //!
    //! @param[in] str ユーザーデータ名。
    //!
    //! @return nn::gfx::ResUserData へのポインターを返します。
    //!
    //! @details
    //! 指定した名前のユーザーデータが無い場合は NULL を返します。
    //!
    //! 名前引きよりインデックス引きほうが高速に動作します。インデックスを
    //! 取得後、記録しておき、通常はインデックス引きを使用することを推奨します。
    //!
    nn::gfx::ResUserData* FindUserData(const char* str) NN_NOEXCEPT
    {
        const nn::util::ResDic* pDic = ToData().pUserDataDic.Get();
        if (pDic == NULL)
        {
            return NULL;
        }
        int index = pDic->FindIndex(str);
        if (index == nn::util::ResDic::Npos)
        {
            return NULL;
        }
        nn::gfx::ResUserData* pArray = ToData().pUserDataArray.Get();
        NN_SDK_ASSERT_NOT_NULL(pArray);
        return &pArray[index];
    }
    //! @brief 指定した名前のユーザーデータを取得します。
    //!
    //! @param[in] str ユーザーデータ名。
    //!
    //! @return nn::gfx::ResUserData へのポインターを返します。
    //!
    //! @details
    //! 指定した名前のユーザーデータが無い場合は NULL を返します。
    //!
    //! 名前引きよりインデックス引きほうが高速に動作します。インデックスを
    //! 取得後、記録しておき、通常はインデックス引きを使用することを推奨します。
    //!
    const nn::gfx::ResUserData* FindUserData(const char* str) const NN_NOEXCEPT
    {
        const nn::util::ResDic* pDic = ToData().pUserDataDic.Get();
        if (pDic == NULL)
        {
            return NULL;
        }
        int index = pDic->FindIndex(str);
        if (index == nn::util::ResDic::Npos)
        {
            return NULL;
        }
        const nn::gfx::ResUserData* pArray = ToData().pUserDataArray.Get();
        NN_SDK_ASSERT_NOT_NULL(pArray);
        return &pArray[index];
    }
    //! @brief 指定した名前のユーザーデータのインデックスを取得します。
    //!
    //! @param[in] str ユーザーデータ名。
    //!
    //! @return ユーザーデータのインデックスを返します。
    //!
    //! @details
    //! 指定した名前のユーザーデータが無い場合は nn::util::ResDic::Npos を返します。
    //!
    int FindUserDataIndex(const char* str) const NN_NOEXCEPT
    {
        const nn::util::ResDic* pDic = ToData().pUserDataDic.Get();
        if (pDic == NULL)
        {
            return nn::util::ResDic::Npos;
        }
        return pDic->FindIndex(str);
    }
    //! @brief ユーザーデータのインデックスから名前を取得します。
    //!
    //! @param[in] elemIndex ユーザーデータのインデックス。
    //!
    //! @return ユーザーデータ名へのポインターを返します。
    //!
    //! @pre
    //! - インデックスはユーザーデータの数の範囲内。
    //!
    //! @details
    //! 指定したインデックスが範囲外の場合は ASSERT します。
    //!
    const char* GetUserDataName(int elemIndex) const NN_NOEXCEPT
    {
        NN_G3D_REQUIRES_RANGE(elemIndex, 0, GetUserDataCount(), GetName());
        const nn::util::ResDic* pDic = ToData().pUserDataDic.Get();
        if (pDic == NULL)
        {
            return NULL;
        }
        nn::util::string_view pString = pDic->GetKey(elemIndex);
        return pString.data();
    }
    //! @brief インデックス引きでユーザーデータを取得します。
    //!
    //! @param[in] elemIndex ユーザーデータのインデックス。
    //!
    //! @return nn::gfx::ResUserData へのポインターを返します。
    //!
    //! @pre
    //! - インデックスはユーザーデータの数の範囲内。
    //!
    //! @details
    //! 指定したインデックスが範囲外の場合は ASSERT します。
    //!
    nn::gfx::ResUserData* GetUserData(int elemIndex) NN_NOEXCEPT
    {
        NN_G3D_REQUIRES_RANGE(elemIndex, 0, GetUserDataCount(), GetName());
        nn::gfx::ResUserData* pArray = ToData().pUserDataArray.Get();
        NN_SDK_ASSERT_NOT_NULL(pArray);
        return &pArray[elemIndex];
    }
    //! @brief インデックス引きでユーザーデータを取得します。
    //!
    //! @param[in] elemIndex ユーザーデータのインデックス。
    //!
    //! @return nn::gfx::ResUserData へのポインターを返します。
    //!
    //! @pre
    //! - インデックスはユーザーデータの数の範囲内。
    //!
    //! @details
    //! 指定したインデックスが範囲外の場合は ASSERT します。
    //!
    const nn::gfx::ResUserData* GetUserData(int elemIndex) const NN_NOEXCEPT
    {
        NN_G3D_REQUIRES_RANGE(elemIndex, 0, GetUserDataCount(), GetName());
        const nn::gfx::ResUserData* pArray = ToData().pUserDataArray.Get();
        NN_SDK_ASSERT_NOT_NULL(pArray);
        return &pArray[elemIndex];
    }

    //@}
};

//--------------------------------------------------------------------------------------------------

//! @brief リソースに関連付ける関数名のテーブルです。
class BindFuncTable
{
public:

    //! @brief 関数名を格納する構造体です。
    struct Name
    {
        size_t len; //!< 長さ。
        const char* str; //!< 文字列へのポインター。
    };

    //----------------------------------------
    //! @name 構築/破棄
    //@{

    BindFuncTable()
        : m_LightTypeCount(0)
        , m_LightDistAttnCount(0)
        , m_LightAngleAttnCount(0)
        , m_FogDistAttnCount(0)
        , m_LightTypeArray(NULL)
        , m_LightDistAttnArray(NULL)
        , m_LightAngleAttnArray(NULL)
        , m_FogDistAttnArray(NULL)
    {
    }

    //@}

    //----------------------------------------
    //! @name 取得/設定
    //@{

    //! @brief ライトタイプの関数名一覧を設定します。
    //!
    //! @param[in] pLightTyepArray ライトタイプ関数名の配列へのポインター。
    //! @param[in] count 配列の要素数。
    //!
    void SetLightTypeArray(const Name* pLightTyepArray, int count) NN_NOEXCEPT
    {
        NN_SDK_REQUIRES(pLightTyepArray != NULL || count == 0);
        m_LightTypeCount = count;
        m_LightTypeArray = pLightTyepArray;
    }

    //! @brief ライトタイプの関数名一覧を取得します。
    //!
    //! @return ライトタイプの関数名の配列へのポインター。
    //!
    const Name* GetLightTypeArray() const NN_NOEXCEPT
    {
        return m_LightTypeArray;
    }

    //! @brief ライトタイプの関数名の数を取得します。
    //!
    //! @return ライトタイプの関数名の数。
    //!
    int GetLightTypeCount() const NN_NOEXCEPT
    {
        return m_LightTypeCount;
    }

    //! @brief ライトの距離減衰の関数名一覧を設定します。
    //!
    //! @param[in] pLightDistAttnArray ライトの距離減衰の関数名の配列へのポインター。
    //! @param[in] count 配列の要素数。
    //!
    void SetLightDistAttnArray(const Name* pLightDistAttnArray, int count) NN_NOEXCEPT
    {
        NN_SDK_REQUIRES(pLightDistAttnArray != NULL || count == 0);
        m_LightDistAttnCount = count;
        m_LightDistAttnArray = pLightDistAttnArray;
    }

    //! @brief ライトの距離減衰の関数名一覧を取得します。
    //!
    //! @return ライトの距離減衰の関数名の配列へのポインター。
    //!
    const Name* GetLightDistAttnArray() const NN_NOEXCEPT
    {
        return m_LightDistAttnArray;
    }

    //! @brief ライトの距離減衰の関数名の数を取得します。
    //!
    //! @return ライトの距離減衰の関数名の数。
    //!
    int GetLightDistAttnCount() const
    {
        return m_LightDistAttnCount;
    }

    //! @brief ライトの角度減衰の関数名一覧を設定します。
    //!
    //! @param[in] pLightAngleAttnArray ライトの角度減衰の関数名の配列へのポインター。
    //! @param[in] count 配列の要素数。
    //!
    void SetLightAngleAttnArray(const Name* pLightAngleAttnArray, int count) NN_NOEXCEPT
    {
        NN_SDK_REQUIRES(pLightAngleAttnArray != NULL || count == 0);
        m_LightAngleAttnCount = count;
        m_LightAngleAttnArray = pLightAngleAttnArray;
    }

    //! @brief ライトの角度減衰の関数名一覧を取得します。
    //!
    //! @return ライトの角度減衰の関数名の配列へのポインター。
    //!
    const Name* GetLightAngleAttnArray() const NN_NOEXCEPT
    {
        return m_LightAngleAttnArray;
    }

    //! @brief ライトの角度減衰の関数名の数を取得します。
    //!
    //! @return ライトの角度減衰の関数名の数。
    //!
    int GetLightAngleAttnCount() const NN_NOEXCEPT
    {
        return m_LightAngleAttnCount;
    }

    //! @brief フォグの距離減衰の関数名一覧を設定します。
    //!
    //! @param[in] pFogDistAttnArray ォグの距離減衰の関数名の配列へのポインター。
    //! @param[in] count 配列の要素数。
    //!
    void SetFogDistAttnArray(const Name* pFogDistAttnArray, int count) NN_NOEXCEPT
    {
        NN_SDK_REQUIRES(pFogDistAttnArray != NULL || count == 0);
        m_FogDistAttnCount = count;
        m_FogDistAttnArray = pFogDistAttnArray;
    }

    //! @brief フォグの距離減衰の関数名一覧を取得します。
    //!
    //! @return フォグの距離減衰の関数名の配列へのポインター。
    //!
    const Name* GetFogDistAttnArray() const NN_NOEXCEPT
    {
        return m_FogDistAttnArray;
    }

    //! @brief フォグの距離減衰の関数名の数を取得します。
    //!
    //! @return フォグの距離減衰の関数名の数。
    //!
    int GetFogDistAttnCount() const NN_NOEXCEPT
    {
        return m_FogDistAttnCount;
    }

    //@}

private:
    int m_LightTypeCount;
    int m_LightDistAttnCount;
    int m_LightAngleAttnCount;
    int m_FogDistAttnCount;
    const Name* m_LightTypeArray;
    const Name* m_LightDistAttnArray;
    const Name* m_LightAngleAttnArray;
    const Name* m_FogDistAttnArray;
};

}} // namespace nn::g3d

NN_PRAGMA_POP_WARNINGS
