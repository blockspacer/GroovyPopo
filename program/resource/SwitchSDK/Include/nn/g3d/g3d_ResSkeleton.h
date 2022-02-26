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
* @brief スケルトンに関するリソースクラスの宣言
*/

#pragma once

#include <nn/util/util_ResDic.h>
#include <nn/g3d/g3d_Configuration.h>
#include <nn/g3d/g3d_ResCommon.h>
#include <nn/gfx/gfx_ResUserData.h>
#include <nn/g3d/detail/g3d_MatrixUtility.h>

NN_PRAGMA_PUSH_WARNINGS
NN_DISABLE_WARNING_SHADOW
#if defined(NN_BUILD_CONFIG_TOOLCHAIN_VC)
NN_ENABLE_WARNING_IMPLICIT_PADDING_WITHIN_CLASS
#endif

namespace nn { namespace g3d {

class ResFile;

//--------------------------------------------------------------------------------------------------

//! @brief ボーンの構造体です。
struct ResBoneData
{
    nn::util::BinPtrToString            pName; //!< 名前。
    nn::util::BinTPtr<nn::gfx::ResUserData> pUserDataArray; //!< ユーザーデータ配列へのポインター。
    nn::util::BinTPtr<nn::util::ResDic> pUserDataDic; //!< ユーザーデータ辞書へのポインター。
    uint8_t reserved[16];

    uint16_t index; //!< ボーン配列中のインデックス。
    uint16_t parentIndex; //!< 親ボーンのインデックス。
    int16_t  smoothMtxIndex; //!< スムーススキニング用の行列パレットにおけるインデックス。
    int16_t  rigidMtxIndex; //!< リジッドスキニング用の行列パレットにおけるインデックス。
    uint16_t billboardIndex; //!< 階層ビルボードの場合に影響を受けるボーンのインデックス。
    uint16_t userDataCount; //!< ユーザーデータ数。
    Bit32 flag; //!< フラグ。
    nn::util::Float3  scale; //!< スケール値。
    union //!< 回転値。
    {
        nn::util::Float4 quat; //!< クォータニオン。
        nn::util::Float3 euler; //!< オイラー角。
    } rotate;
    nn::util::Float3 translate; //!< 移動値。
};



//! @brief ボーンのリソースです。
class ResBone : public nn::util::AccessorBase< ResBoneData > // LOD の情報は含まない
{
    NN_DISALLOW_COPY( ResBone );

public:
    //! @brief 無効なボーンインデックスを示す値です。
    static const int InvalidBoneIndex = 0xFFFF;

    //! @briefprivate
    enum Shift
    {
        Shift_Hierarchy   = 4,
        Shift_Rot         = 12,
        Shift_Billboard   = 16,
        Shift_MirroringState = 20,
        Shift_Transform   = 23
    };

    //! @brief ボーンに関するフラグです。
    enum Flag
    {
        Flag_Visibility                = 0x1 << 0, //!< ビジビリティの初期値です。

        // ビルボード
        Flag_BillboardNone             = 0x0 << Shift_Billboard, //!< ビルボード計算をしません。
        Flag_BillboardChild            = 0x1 << Shift_Billboard, //!< 階層ビルボードの子として計算を行います。
        Flag_BillboardWorldViewVector  = 0x2 << Shift_Billboard, //!< World 方式でビルボード計算を行います。
        Flag_BillboardWorldViewPoint   = 0x3 << Shift_Billboard, //!< WorldViewpoint 方式でビルボード計算を行います。
        Flag_BillboardScreenViewVector = 0x4 << Shift_Billboard, //!< Screen 方式でビルボード計算を行います。
        Flag_BillboardScreenViewPoint  = 0x5 << Shift_Billboard, //!< ScreenViewpoint 方式でビルボード計算を行います。
        Flag_BillboardYaxisViewVector  = 0x6 << Shift_Billboard, //!< YAxis 方式でビルボード計算を行います。
        Flag_BillboardYaxisViewPoint   = 0x7 << Shift_Billboard, //!< YAxisViewpoint 方式でビルボード計算を行います。
        Flag_BillboardMax              = Flag_BillboardYaxisViewPoint, //! @briefprivate

        // ボーンのトランスフォーム
        Flag_SegmentScaleCompensate     = 0x1 << Shift_Transform, //!< MayaSSC で計算します。
        Flag_ScaleUniform               = 0x2 << Shift_Transform, //!< Scale が等方です。
        Flag_ScaleVolumeOne             = 0x4 << Shift_Transform, //!< Scale の体積が１です。
        Flag_RotateZero                 = 0x8 << Shift_Transform, //!< Rotate が 0 です。
        Flag_TranslateZero              = 0x10 << Shift_Transform, //!< Translate が 0 です。
        Flag_ScaleOne                   = Flag_ScaleVolumeOne | Flag_ScaleUniform, //!< Scale が１です。
        Flag_RotTransZero               = Flag_RotateZero | Flag_TranslateZero, //!< Rotate, Translate が 0 です。
        Flag_Identity                   = Flag_ScaleOne | Flag_RotateZero | Flag_TranslateZero, //!< トランスフォームが単位行列です。
        Flag_HiScaleUniform             = Flag_ScaleUniform     << Shift_Hierarchy, //!< 累積 Scale が等方です。
        Flag_HiScaleVolumeOne           = Flag_ScaleVolumeOne   << Shift_Hierarchy, //!< 累積 Scale の体積が１です。
        Flag_HiRotateZero               = Flag_RotateZero       << Shift_Hierarchy, //!< 累積 Rotate が 0 です。
        Flag_HiTranslateZero            = Flag_TranslateZero    << Shift_Hierarchy, //!< 累積 Translate が 0 です。
        Flag_HiScaleOne                 = Flag_ScaleOne         << Shift_Hierarchy, //!< 累積 Scale が１です。
        Flag_HiRotTransZero             = Flag_RotTransZero     << Shift_Hierarchy, //!< 累積 Rotate, Translate が 0 です。
        Flag_HiIdentity                 = Flag_Identity         << Shift_Hierarchy, //!< 累積トランスフォームが単位行列です。
        Flag_BillboardIndexNone         = 0xFFFF //!< 階層ビルボードの影響を受けるボーンがありません。
    };

    //! @brief ボーンに関する Rotate モード
    enum RotateMode
    {
        RotateMode_Quat     = 0x0 << Shift_Rot, //!< Quaternion 方式で回転を計算します。
        RotateMode_EulerXyz = 0x1 << Shift_Rot //!< Euler 角方式で回転を計算します。
    };

    //! @briefprivate ボーンの位置情報を示すフラグです。モーションミラーリングの計算に使用します。
    enum MirroringState
    {
        MirroringState_CenterPreRotate   = 0x0 << Shift_MirroringState, //! @brief 回転前の中央のボーン。
        MirroringState_CenterRotate      = 0x1 << Shift_MirroringState, //! @brief 回転値が入った中央のボーン
        MirroringState_CenterPostRotate  = 0x2 << Shift_MirroringState, //! @brief 回転後にローカル軸の向きが変わった後の中央のボーン。
        MirroringState_SideRoot          = 0x3 << Shift_MirroringState, //! @brief ジョイントのミラーで生成された左右のボーンのルート。
        MirroringState_Side              = 0x4 << Shift_MirroringState  //! @brief ジョイントのミラーで生成された左右のボーン。
    };

    //! @briefprivate
    enum Mask
    {
        Mask_Rot = 0x7 << Shift_Rot,
        Mask_Billboard = 0x7 << Shift_Billboard,
        Mask_MirroringState = 0x7 << Shift_MirroringState,
        Mask_Transform = Flag_SegmentScaleCompensate | Flag_Identity
    };

    //----------------------------------------
    //! @name 取得
    //@{

    //! @brief スケルトン内でのインデックスを取得します。
    //!
    //! @return スケルトン内でのインデックスを返します。
    //!
    int GetIndex() const
    {
        return ToData().index;
    }

    //! @brief スケルトン名を取得します。
    //!
    //! @return スケルトン名へのポインターを返します。
    //!
    const char* GetName() const NN_NOEXCEPT
    {
        return ToData().pName.Get()->GetData();
    }

    //! @brief スムーススキニング用の行列パレットにおけるインデックスを取得します。
    //!
    //! @return スムーススキニング用の行列パレットにおけるインデックスを返します。
    //!
    int GetSmoothMtxIndex() const NN_NOEXCEPT
    {
        return ToData().smoothMtxIndex;
    }

    //! @brief リジッドスキニング用の行列パレットにおけるインデックスを取得します。
    //!
    //! @return リジッドスキニング用の行列パレットにおけるインデックスを返します。
    //!
    int GetRigidMtxIndex() const NN_NOEXCEPT
    {
        return ToData().rigidMtxIndex;
    }

    //! @brief 親ボーンのインデックスを取得します。
    //!
    //! @return 親ボーンのインデックスを返します。
    //!
    int GetParentIndex() const NN_NOEXCEPT
    {
        return ToData().parentIndex;
    }

    //! @brief 回転の計算方式を取得します。
    //!
    //! @return 回転の計算方式を返します。
    //!
    Bit32 GetRotateMode() const NN_NOEXCEPT
    {
        return ToData().flag & Mask_Rot;
    }

    //! @brief ビルボードの計算方式を取得します。
    //!
    //! @return ビルボードの計算方式を返します。
    //!
    Bit32 GetBillboardMode() const NN_NOEXCEPT
    {
        return ToData().flag & Mask_Billboard;
    }

    //! @brief ボーンの配置情報を取得します。
    //!
    //! @return ボーンの配置情報を返します。
    //!
    Bit32 GetMirroringState() const NN_NOEXCEPT
    {
        return ToData().flag & Mask_MirroringState;
    }

    //! @brief ボーンのビジビリティを取得します。
    //!
    //! @return 表示状態の場合 true、非表示状態の場合 false を返します。
    //!
    bool IsVisible() const NN_NOEXCEPT
    {
        return (ToData().flag & Flag_Visibility) != 0;
    }

    //! @brief スケール値を取得します。
    //!
    //! @return スケール値への参照を返します。
    //!
    nn::util::Float3& GetScale() NN_NOEXCEPT
    {
        return ToData().scale;
    }

    //! @brief スケール値を取得します。
    //!
    //! @return スケール値への参照を返します。
    //!
    const nn::util::Float3& GetScale() const NN_NOEXCEPT
    {
        return ToData().scale;
    }

    //! @brief 移動値を取得します。
    //!
    //! @return 移動値への参照を返します。
    //!
    nn::util::Float3& GetTranslate() NN_NOEXCEPT
    {
        return ToData().translate;
    }

    //! @brief 移動値を取得します。
    //!
    //! @return 移動値への参照を返します。
    //!
    const nn::util::Float3& GetTranslate() const NN_NOEXCEPT
    {
        return ToData().translate;
    }

    //! @brief オイラー角で回転を取得します。
    //!
    //! @return 回転値への参照を返します。
    //!
    nn::util::Float3& GetRotateEuler() NN_NOEXCEPT
    {
        NN_SDK_REQUIRES(RotateMode_EulerXyz == GetRotateMode());
        return ToData().rotate.euler;
    }

    //! @brief オイラー角で回転を取得します。
    //!
    //! @return 回転値への参照を返します。
    //!
    const nn::util::Float3& GetRotateEuler() const NN_NOEXCEPT
    {
        NN_SDK_REQUIRES(RotateMode_EulerXyz == GetRotateMode());
        return ToData().rotate.euler;
    }

    //! @brief クォータニオンで回転を取得します。
    //!
    //! @return 回転値への参照を返します。
    //!
    nn::util::Float4& GetRotateQuat() NN_NOEXCEPT
    {
        NN_SDK_REQUIRES(RotateMode_Quat == GetRotateMode());
        return ToData().rotate.quat;
    }

    //! @brief クォータニオンで回転を取得します。
    //!
    //! @return 回転値への参照を返します。
    //!
    const nn::util::Float4& GetRotateQuat() const NN_NOEXCEPT
    {
        NN_SDK_REQUIRES(RotateMode_Quat == GetRotateMode());
        return ToData().rotate.quat;
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

//! @brief スケルトンの構造体です。
struct ResSkeletonData
{
    nn::util::BinBlockSignature         signature; //!< シグネチャー。
    Bit32                               flag; //!< フラグ。
    nn::util::BinTPtr<nn::util::ResDic> pBoneDic; //!< ボーン辞書へのポインター。
    nn::util::BinTPtr<ResBone>          pBoneArray; //!< ボーン配列へのポインター。
    nn::util::BinTPtr<int16_t>          pMtxToBoneTable; //!< スムーススキニングの行列インデックスとボーンのインデックスを対応付けるテーブルへのポインター。
    nn::util::BinTPtr<nn::util::FloatColumnMajor4x3> pInvModelMatrixArray; //!< 頂点からボーンローカル座標系への変換行列配列へのポインター

    nn::util::BinPtr pUserPtr; //!< ユーザーポインター。
    nn::util::BinTPtr<int16_t>          pMirroringBoneTable; //!< ボーンインデックスとミラー先のボーンインデックスを対応付けるテーブルのポインター。モーションミラーリングで使用。

    uint16_t boneCount; //!< ボーン数。
    uint16_t smoothMtxCount; //!< スムーズスキングの行列の数。
    uint16_t rigidMtxCount; //!< リジッドスキニングの行列の数。
    uint8_t  reserved[2];
};

//! @brief スケルトンのリソースです。
class ResSkeleton : public nn::util::AccessorBase< ResSkeletonData > // LOD の情報は含まない
{
    NN_DISALLOW_COPY( ResSkeleton );

public:
    //! @brief ResSkeleton のシグネチャ
    static const uint32_t Signature = NN_UTIL_CREATE_SIGNATURE_4('F', 'S', 'K', 'L');

    //! @briefprivate
    enum Shift
    {
        Shift_MirroringMode = 6,
        Shift_Scale = 8,
        Shift_Rot   = ResBone::Shift_Rot
    };

    //! @briefprivate
    enum Mask
    {
        Mask_MirroringMode   = 0x3 << Shift_MirroringMode,
        Mask_Scale           = 0x3 << Shift_Scale,
        Mask_Rot             = ResBone::Mask_Rot,
        Mask_TransForm       = Mask_Scale | Mask_Rot
    };

    //! @brief スケルトンに関するscaleモードです。
    enum ScaleMode
    {
        // ScaleMode
        ScaleMode_None      = 0x0 << Shift_Scale, //!< スケール計算をしません。
        ScaleMode_Std       = 0x1 << Shift_Scale, //!< 標準的な方法でスケールの計算をします。
        ScaleMode_Maya      = 0x2 << Shift_Scale, //!< Maya 方式でスケールの計算をします。
        ScaleMode_Softimage = 0x3 << Shift_Scale, //!< Softimage 方式でスケールの計算をします。
     };

    //! @brief スケルトンに関するRotateモードです。
    enum RotateMode
    {
        RotateMode_Quat     = ResBone::RotateMode_Quat, //!< クォータニオン方式で回転を計算します。
        RotateMode_EulerXyz = ResBone::RotateMode_EulerXyz, //!< オイラー角方式で回転を計算します。
    };

    //! @brief リセット時にリセットしないものを指定するフラグです。
    enum ResetGuardFlag
    {
        ResetGuardFlag_None       = 0, //!< すべてリセットします。
        ResetGuardFlag_UserPtr    = 0x1 << 0, //!< ユーザーポインターの設定をリセットしません。
    };

    //! @brief モーションミラーリングの計算方式を示すフラグです。
    enum MirroringMode
    {
        MirroringMode_X  = 0x0 << Shift_MirroringMode, //!< ミラー方向が X 軸です。
        MirroringMode_XY = 0x1 << Shift_MirroringMode, //!< 前半のミラー方向が X 軸で、後半は Y 軸です。
        MirroringMode_XZ = 0x2 << Shift_MirroringMode  //!< 前半のミラー方向が X 軸で、後半は Z 軸です。
    };

    //----------------------------------------
    //! @name 構築/破棄
    //@{

    //! @brief スケルトンをリセットします。
    void Reset() NN_NOEXCEPT;

    //! @brief スケルトンをリセットします。
    void Reset(Bit32 resetGuardFlag) NN_NOEXCEPT;

    //@}

    //----------------------------------------
    //! @name 取得/設定
    //@{

    //! @brief スムーススキニングの行列の数を取得します。
    //!
    //! @return スムーススキニングの行列の数を返します。
    //!
    int GetSmoothMtxCount() const NN_NOEXCEPT
    {
        return ToData().smoothMtxCount;
    }

    //! @brief リジッドスキニングの行列の数を取得します。
    //!
    //! @return リジッドスキニングの行列の数を返します。
    //!
    int GetRigidMtxCount() const NN_NOEXCEPT
    {
        return ToData().rigidMtxCount;
    }

    //! @brief マトリクス数を取得します。
    //!
    //! @return マトリクス数を返します。
    //!
    //! @details
    //! GetSmoothMtxCount() と GetRigidMtxCount() を合わせた数になります。
    //!
    int GetMtxCount() const NN_NOEXCEPT
    {
        return GetSmoothMtxCount() + GetRigidMtxCount();
    }

    //! @brief スケールの計算方式を取得します。
    //!
    //! @return スケールの計算方式を返します。
    //!
    Bit32 GetScaleMode() const NN_NOEXCEPT
    {
        return ToData().flag & Mask_Scale;
    }

    //! @brief 回転の計算方式を取得します。
    //!
    //! @return 回転の計算方式を返します。
    //!
    Bit32 GetRotateMode() const NN_NOEXCEPT
    {
        return ToData().flag & Mask_Rot;
    }

    //! @brief モーションミラーリングの計算方式を取得します。
    //!
    //! @return モーションミラーリングの計算方式を返します。
    //!
    Bit32 GetMirroringMode() const NN_NOEXCEPT
    {
        return ToData().flag & Mask_MirroringMode;
    }

    //! @brief モーションミラーリング情報を保持しているかかどうかを取得します。
    //!
    //! @return モーションミラーリング用の情報がバイナリーに含まれている場合 true を返します。
    //!
    bool HasMirroringInfo() const NN_NOEXCEPT
    {
        return ToData().pMirroringBoneTable.Get() ? true : false;
    }

    //! @brief 指定したボーン配下のボーンツリーで最後のボーンの次のインデックスを取得します。
    //!
    //! @param[in] boneIndex ボーンのインデックス。
    //!
    //! @return 指定したボーン配下のボーンツリーで最後のボーンの次のインデックスを返します。
    //!
    //! @details
    //! ボーンは深さ優先順で並んでいるため、EndIndex より小さいインデックスのボーンを辿ることで
    //! 指定したボーン配下のボーンツリーを辿ることができます。
    //!
    int GetBranchEndIndex(int boneIndex) const NN_NOEXCEPT;

    //! @brief ユーザーポインターを設定します。
    //!
    //! @param[in] pUserPtr ポインター。
    //!
    void SetUserPtr(void* pUserPtr) NN_NOEXCEPT
    {
        ToData().pUserPtr.Set(pUserPtr);
    }

    //! @brief ユーザーポインターを取得します。
    //!
    //! @return SetUserPtr() で設定したポインターを返します。
    //!
    void* GetUserPtr() NN_NOEXCEPT
    {
        return ToData().pUserPtr.Get();
    }

    //! @brief ユーザーポインターを取得します。
    //!
    //! @return SetUserPtr() で設定したポインターを返します。
    //!
    const void* GetUserPtr() const NN_NOEXCEPT
    {
        return ToData().pUserPtr.Get();
    }

    //! @brief ユーザーポインターを取得します。
    //!
    //! @tparam T ポインターを受けとる際の要素の型。
    //!
    //! @return SetUserPtr() で設定したポインターを返します。
    //!
    template <typename T>
    T* GetUserPtr() NN_NOEXCEPT
    {
        return static_cast<T*>(GetUserPtr());
    }

    //! @brief ユーザーポインターを取得します。
    //!
    //! @tparam T ポインターを受けとる際の要素の型。
    //!
    //! @return SetUserPtr() で設定したポインターを返します。
    //!
    template <typename T>
    const T* GetUserPtr() const NN_NOEXCEPT
    {
        return static_cast<const T*>(GetUserPtr());
    }

    //! @briefprivate ビルボードの設定を更新します。
    //!
    //! ボーンの階層構造を辿って階層ビルボードの設定を行います。
    //! インスタンスの初期化に影響するため動的に変更はできません。
    //!
    void UpdateBillboardMode() NN_NOEXCEPT;

    //@}

    //----------------------------------------
    //! @name ボーン
    //@{

    //! @brief ボーンの数を取得します。
    //!
    //! @return ボーンの数を返します。
    //!
    int GetBoneCount() const NN_NOEXCEPT
    {
        return ToData().boneCount;
    }

    //! @brief 指定した名前のボーンを取得します。
    //!
    //! @param[in] str ボーン名。
    //!
    //! @return ResBone へのポインターを返します。
    //!
    //! @details
    //! 指定した名前のボーンが無い場合は NULL を返します。
    //!
    //! 名前引きよりインデックス引きほうが高速に動作します。インデックスを
    //! 取得後、記録しておき、通常はインデックス引きを使用することを推奨します。
    //!
    ResBone* FindBone(const char* str) NN_NOEXCEPT
    {
        const nn::util::ResDic* pDic = ToData().pBoneDic.Get();
        if (pDic == NULL)
        {
            return NULL;
        }
        int index = pDic->FindIndex(str);
        if (index == nn::util::ResDic::Npos)
        {
            return NULL;
        }
        ResBone* pArray = ToData().pBoneArray.Get();
        NN_SDK_ASSERT_NOT_NULL(pArray);
        return &pArray[index];
    }
    //! @brief 指定した名前のボーンを取得します。
    //!
    //! @param[in] str ボーン名。
    //!
    //! @return ResBone へのポインターを返します。
    //!
    //! @details
    //! 指定した名前のボーンが無い場合は NULL を返します。
    //!
    //! 名前引きよりインデックス引きほうが高速に動作します。インデックスを
    //! 取得後、記録しておき、通常はインデックス引きを使用することを推奨します。
    //!
    const ResBone* FindBone(const char* str) const NN_NOEXCEPT
    {
        const nn::util::ResDic* pDic = ToData().pBoneDic.Get();
        if (pDic == NULL)
        {
            return NULL;
        }
        int index = pDic->FindIndex(str);
        if (index == nn::util::ResDic::Npos)
        {
            return NULL;
        }
        const ResBone* pArray = ToData().pBoneArray.Get();
        NN_SDK_ASSERT_NOT_NULL(pArray);
        return &pArray[index];
    }

    //! @brief 指定した名前のボーンのインデックスを取得します。
    //!
    //! @param[in] str ボーン名。
    //!
    //! @return ボーンのインデックスを返します。
    //!
    //! @details
    //! 指定した名前のボーンが無い場合は nn::util::ResDic::Npos を返します。
    //!
    int FindBoneIndex(const char* str) const NN_NOEXCEPT
    {
        const nn::util::ResDic* pDic = ToData().pBoneDic.Get();
        if (pDic == NULL)
        {
            return nn::util::ResDic::Npos;
        }
        return pDic->FindIndex(str);
    }
    //! @brief ボーンのインデックスから名前を取得します。
    //!
    //! @param[in] elemIndex ボーンのインデックス。
    //!
    //! @return ボーン名へのポインターを返します。
    //!
    //! @pre
    //! - インデックスはボーンの数の範囲内。
    //!
    //! @details
    //! 指定したインデックスが範囲外の場合は ASSERT します。
    //!
    const char* GetBoneName(int elemIndex) const NN_NOEXCEPT
    {
        NN_SDK_REQUIRES_RANGE(elemIndex, 0, GetBoneCount());
        const nn::util::ResDic* pDic = ToData().pBoneDic.Get();
        if (pDic == NULL)
        {
            return NULL;
        }
        nn::util::string_view pString = pDic->GetKey(elemIndex);
        return pString.data();
    }

    //! @brief インデックス引きでボーンを取得します。
    //!
    //! @param[in] elemIndex ボーンのインデックス。
    //!
    //! @return ResBone へのポインターを返します。
    //!
    //! @pre
    //! - インデックスはボーンの数の範囲内。
    //!
    //! @details
    //! 指定したインデックスが範囲外の場合は ASSERT します。
    //!
    ResBone* GetBone(int elemIndex) NN_NOEXCEPT
    {
        NN_SDK_REQUIRES_RANGE(elemIndex, 0, GetBoneCount());
        ResBone* pArray = ToData().pBoneArray.Get();
        NN_SDK_ASSERT_NOT_NULL(pArray);
        return &pArray[elemIndex];
    }
    //! @brief インデックス引きでボーンを取得します。
    //!
    //! @param[in] elemIndex ボーンのインデックス。
    //!
    //! @return ResBone へのポインターを返します。
    //!
    //! @pre
    //! - インデックスはボーンの数の範囲内。
    //!
    //! @details
    //! 指定したインデックスが範囲外の場合は ASSERT します。
    //!
    const ResBone* GetBone(int elemIndex) const NN_NOEXCEPT
    {
        NN_SDK_REQUIRES_RANGE(elemIndex, 0, GetBoneCount());
        const ResBone* pArray = ToData().pBoneArray.Get();
        NN_SDK_ASSERT_NOT_NULL(pArray);
        return &pArray[elemIndex];
    }

    //! @brief インデックス引きでミラーリング先のボーンインデクスを取得します。
    //!
    //! @param[in] elemIndex ボーンのインデックス。
    //!
    //! @return ミラーリング先のボーンインデクスを返します。
    //!
    //! @pre
    //! - インデックスはボーンの数の範囲内。
    //!
    //! @details
    //! 指定したインデックスが範囲外の場合は ASSERT します。
    //! ミラーリング先が存在しない場合は -1 が返ります。
    //!
    int GetMirroredBoneIndex(int elemIndex) const NN_NOEXCEPT
    {
        NN_SDK_REQUIRES_RANGE(elemIndex, 0, GetBoneCount());
        NN_SDK_ASSERT_NOT_NULL(pMirroringBoneTable.Get());
        return pMirroringBoneTable.Get()[elemIndex];
    }
    //@}
};

}} // namespace nn::g3d

NN_PRAGMA_POP_WARNINGS
