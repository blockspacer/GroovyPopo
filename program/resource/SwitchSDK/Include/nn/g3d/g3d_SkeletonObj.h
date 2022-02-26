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
* @brief スケルトンに関するインスタンスクラスの宣言
*/

#pragma once

#include <nn/gfx.h>
#include <nn/util/util_MemorySplitter.h>
#include <nn/g3d/g3d_Configuration.h>
#include <nn/g3d/g3d_ResSkeleton.h>

namespace nn { namespace g3d {

//! @brief ボーンのローカル行列を表す構造体です。
struct LocalMtxData
{
    Bit32 flag; //!< フラグ。
    nn::util::Vector3fType   scale; //!< スケール値。
    nn::util::Matrix4x3fType mtxRT; //!< 回転、移動行列。
};

//! @brief ボーンのローカル行列を表すクラスです。
//!
//! flag が 0 の場合は scale と mtxRT をそのまま使用して計算されます。
//! さらに次のビットが立っている場合は、ライブラリーがそれぞれのビットに応じた
//! 特殊計算や最適化計算を行うことがあります。
//!
//! ResBone::Flag_SegmentScaleCompensate <br>
//! ResBone::Flag_ScaleUniform           <br>
//! ResBone::Flag_ScaleVolumeOne         <br>
//! ResBone::Flag_RotateZero             <br>
//! ResBone::Flag_TranslateZero          <br>
//!
class LocalMtx : public LocalMtxData
{
public:
    //! @brief コンストラクタです。
    LocalMtx() NN_NOEXCEPT : LocalMtxData()
    {
        flag = 0;
    }

    //! @brief 内容の編集後に呼び出す関数です。
    //!
    //! @details
    //! この関数を呼び出すことで最適化フラグがリセットされます。
    //!
    void EndEdit() NN_NOEXCEPT
    {
        flag &= ~ResBone::Flag_Identity;
    }

    //! @brief LocalMtxData を LocalMtx に変換します。
    //!
    //! @param[in] data LocalMtxData への参照。
    //!
    //! @return LocalMtx への参照を返します。
    //!
    static LocalMtx& DownCast(LocalMtxData& data) NN_NOEXCEPT
    {
        return static_cast<LocalMtx&>(data);
    }

    //! @brief LocalMtxData を LocalMtx に変換します。
    //!
    //! @param[in] data LocalMtxData への参照。
    //!
    //! @return LocalMtx への参照を返します。
    //!
    static const LocalMtx& DownCast(const LocalMtxData& data) NN_NOEXCEPT
    {
        return static_cast<const LocalMtx&>(data);
    }

    //! @brief LocalMtxData を LocalMtx に変換します。
    //!
    //! @param[in] data LocalMtxData へのポインター。
    //!
    //! @return LocalMtxへのポインターを返します。
    //!
    static LocalMtx* DownCast(LocalMtxData* data) NN_NOEXCEPT
    {
        return static_cast<LocalMtx*>(data);
    }

    //! @brief LocalMtxData を LocalMtx に変換します。
    //!
    //! @param[in] data LocalMtxData へのポインター。
    //!
    //! @return LocalMtx へのポインターを返します。
    //!
    static const LocalMtx* DownCast(const LocalMtxData* data) NN_NOEXCEPT
    {
        return static_cast<const LocalMtx*>(data);
    }
};

// ワールドマトリクスを操作するためのクラスです。
class WorldMtxManip
{
public:
    //! @brief コンストラクタです。
    //!
    //! @param[in] pMtx ワールド行列へのポインター。
    //! @param[in] pScale スケールへのポインター。
    //! @param[in] pFlag 処理制御するためのフラグへのポインター。
    //!
    WorldMtxManip(nn::util::Matrix4x3fType* pMtx, nn::util::Vector3fType* pScale, Bit32* pFlag) NN_NOEXCEPT
        : m_pMtx(pMtx), m_pScale(pScale), m_pFlag(pFlag)
    {
        NN_SDK_REQUIRES_NOT_NULL(pMtx);
        NN_SDK_REQUIRES_NOT_NULL(pScale);
        NN_SDK_REQUIRES_NOT_NULL(pFlag);
        // ScaleMode が SCALE_NONE の場合 pScale は NULL
    }

    //! @brief スケールを取得します。
    //!
    //! @return スケールへのポインターを返します。
    //!
    const nn::util::Vector3fType* GetScale() const NN_NOEXCEPT
    {
        return m_pScale;
    }

    //! @brief ワールド行列を取得します。
    //!
    //! @return ワールド行列へのポインターを返します。
    //!
    const nn::util::Matrix4x3fType* GetMtx() const NN_NOEXCEPT
    {
        return m_pMtx;
    }

    //! @brief 編集するためにワールド行列を取得します。
    //!
    //! @return ワールド行列へのポインターを返します。
    //!
    //! @details
    //! 最適化フラグをリセットします。
    //!
    nn::util::Matrix4x3fType* EditMtx() NN_NOEXCEPT
    {
        *m_pFlag &= ~(ResBone::Flag_RotTransZero | ResBone::Flag_HiRotTransZero);
        return m_pMtx;
    }

    //! @brief ResBone::Flag を取得します。
    //!
    //! @return フラグ値を返します。
    //!
    Bit32 GetFlag() const NN_NOEXCEPT
    {
        return *m_pFlag;
    }

private:
    nn::util::Matrix4x3fType* m_pMtx;
    nn::util::Vector3fType*  m_pScale;
    Bit32* m_pFlag;

    NN_DISALLOW_COPY(WorldMtxManip);
};

// ボーンのワールド行列更新後に呼び出されるコールバックの基底クラスです。
class ICalculateWorldCallback
{
public:
    static const uint16_t InvalidBone = 0xFFFF;

    //! @brief デストラクタです。
    virtual ~ICalculateWorldCallback() NN_NOEXCEPT
    {
    }

    //! @brief Exec() 用いる設定です。
    class CallbackArg
    {
    public:
        //! @brief  コンストラクタです。
        //!
        //! @param[in] boneIndex 計算に用いるボーンのインデックスへの参照。
        //! @param[in] callbackBone コールバックを呼び出すボーンのインデックス。
        //!
        CallbackArg(const int& boneIndex, int callbackBone) NN_NOEXCEPT
            : m_BoneIndex(boneIndex), m_CallbackBone(callbackBone)
        {
        }

        //! @brief 計算に用いるボーンのインデックスを取得します。
        //!
        //! @return 計算に用いるボーンのインデックスを返します。
        //!
        int GetBoneIndex() const NN_NOEXCEPT
        {
            return m_BoneIndex;
        }

        //! @brief コールバックを呼び出すボーンのインデックスを取得します。
        //!
        //! @return コールバックを呼び出すボーンのインデックスを返します。
        //!
        int GetCallbackBoneIndex() const NN_NOEXCEPT
        {
            return m_CallbackBone;
        }

        //! @brief コールバックを呼び出すボーンのインデックスを設定します。
        //!
        //! @param[in] boneIndex コールバックを呼び出すボーンのインデックス。
        //!
        //! @details
        //! コールバック計算内部で対象のボーンのインデックスを変更することで
        //! 複数のボーンに対してコールバックを呼び出すことができます。
        //!
        void SetCallbackBoneIndex(int boneIndex) NN_NOEXCEPT
        {
            m_CallbackBone = boneIndex;
        }

    private:
        const int& m_BoneIndex;
        int        m_CallbackBone;

        NN_DISALLOW_COPY(CallbackArg);
    };

    //! @brief コールバックを呼び出します。
    //!
    //! @param[in] arg CallbackArg への参照。
    //! @param[in] manip WorldMtxManip への参照。
    //!
    virtual void Exec(CallbackArg& arg, WorldMtxManip& manip) NN_NOEXCEPT = 0;
};

//--------------------------------------------------------------------------------------------------

//! @brief スケルトンインスタンスです。
class SkeletonObj
{
public:
    class Builder;

    //! @briefprivate シフト値です。
    enum Shift
    {
        Shift_Scale = ResSkeleton::Shift_Scale,
        Shift_Rot   = ResSkeleton::Shift_Rot
    };

    //! @brief スケルトンの状態を表すフラグです。
    enum Flag
    {
        Flag_BlockBufferValid = 0x1 << 0, //!< @briefprivate ユニフォームブロックが構築済みであるかどうかを表すフラグです。
        Flag_InvalidLocalMtx  = 0x1 << 2, //!< @briefprivate 古いローカル行列の一部がキャッシュに乗っているかどうかを表すフラグです。
        Flag_InvalidWorldMtx  = 0x1 << 4, //!< @briefprivate 古いワールド行列の一部がキャッシュに乗っているかどうかを表すフラグです。
    };

    //! @briefprivate マスク値です。
    enum Mask
    {
        Mask_Scale    = ResSkeleton::Mask_Scale,
        Mask_Rot      = ResSkeleton::Mask_Rot,
    };

    //! @brief スケルトンに関するScaleモードです。
    enum ScaleMode
    {
        ScaleMode_None         = ResSkeleton::ScaleMode_None, //!< @brief スケール計算を行わないことを表すフラグです。
        ScaleMode_Std          = ResSkeleton::ScaleMode_Std, //!< @brief 通常の方式でスケール計算を行うことを表すフラグです。
        ScaleMode_Maya         = ResSkeleton::ScaleMode_Maya, //!< @brief Maya 方式でスケール計算を行うことを表すフラグです。
        ScaleMode_Softimage    = ResSkeleton::ScaleMode_Softimage, //!< @brief Softimage 方式でスケール計算を行うことを表すフラグです。
    };

    //! @brief スケルトンに関するRotateモードです。
    enum RotateMode
    {
        RotateMode_EulerXyz       = ResSkeleton::RotateMode_EulerXyz, //!< @brief オイラー角方式で回転計算を行うことを表すフラグです。
        RotateMode_Quat           = ResSkeleton::RotateMode_Quat, //!< @brief クォータニオン方式で回転計算を行うことを表すフラグです。
    };

    //! @brief インスタンスの構築時に渡すバッファーの必要アライメントサイズです。
    enum Alignment
    {
        Alignment_Buffer = MatrixVectorAlignment, //!< @brief Builder に渡すバッファーの必要アライメントサイズです。
    };

    //----------------------------------------
    //! @name 構築/破棄
    //@{

    //! @brief コンストラクタです。
    //!
    //! @details
    //! 実際の構築処理はBuilderで行います。
    //!
    SkeletonObj() NN_NOEXCEPT
        : m_pRes(NULL)
        , m_Flag(0)
        , m_BufferingCount(0)
        , m_pBoneArray(NULL)
        , m_pLocalMtxArray(NULL)
        , m_pWorldMtxArray(NULL)
        , m_pScaleArray(NULL)
        , m_pMemoryPool(NULL)
        , m_MemoryPoolOffset(0)
        , m_pMtxBlockArray(NULL)
        , m_SizePerMtxBlock(0)
        , m_BoneCount(0)
        , m_CallbackBone(0)
        , m_pCallback(NULL)
        , m_pMemLocalMtxArray(NULL)
        , m_pMemWorldMtxArray(NULL)
        , m_pUserPtr(NULL)
        , m_pBufferPtr(NULL)
    {
    }

    //! @brief ユニフォームブロックのサイズを計算します。
    //!
    //! @return 計算したバッファーサイズを返します。
    //!
    size_t CalculateBlockBufferSize(nn::gfx::Device* pDevice) const NN_NOEXCEPT;

    //! @brief メモリプールを渡して、ユニフォームブロックを構築します。
    //!
    //! @param[in] pDevice デバイスへのポインター。
    //! @param[in] pMemoryPool メモリプールへのポインター。
    //! @param[in] offset メモリプールの使用する領域へのオフセット。
    //! @param[in] memoryPoolSize offset以降で使用可能なメモリプールのサイズ。
    //!
    //! @return 構築に成功した場合は true、失敗した場合は false を返します。
    //!
    //! @pre
    //! - pDevice が初期化されている。
    //! - pMemoryPool が初期化されている。
    //! - offset で指されたメモリプールの位置が nn::gfx::Buffer::GetBufferAlignment() のアラインメント上にある。
    //! - memoryPoolSize は CalculateBlockBufferSize() 以上である。
    //! - ユニフォームブロックが未構築である。
    //!
    //! @post
    //! - ユニフォームブロックが構築されている
    //!
    bool SetupBlockBuffer(nn::gfx::Device* pDevice, nn::gfx::MemoryPool* pMemoryPool, ptrdiff_t offset, size_t memoryPoolSize) NN_NOEXCEPT;

    //! @brief ユニフォームブロックを破棄します。
    //!
    //! @param[in] pDevice デバイスへのポインター
    //!
    //! @pre
    //! - pDevice が初期化されている
    //! - ユニフォームブロックが構築済みである。
    //!
    //! @post
    //!- ユニフォームブロックが破棄されている
    //!
    void CleanupBlockBuffer(nn::gfx::Device* pDevice) NN_NOEXCEPT;

    //@}

    //----------------------------------------
    //! @name 更新
    //@{

    //! @brief ローカル行列をリソースに基づいて初期化します。
    //!
    //! @details
    //! 構築時で呼ばれるため、意図的に結果を初期化したい場合以外は呼ぶ必要はありません。
    //!
    void ClearLocalMtx() NN_NOEXCEPT;

    //! @brief ローカル行列からワールド行列を計算します。
    //!
    //! @param[in] baseMtx ベース行列への参照。
    //!
    void CalculateWorldMtx(const nn::util::Matrix4x3fType& baseMtx) NN_NOEXCEPT;

    //! @brief ビルボード行列を計算します。
    //!
    //! @param[out] pMtxArray 計算結果を受け取る行列へのポインター。
    //! @param[in] cameraMtx カメラ行列への参照。
    //! @param[in] boneIndex ボーンのインデックス。
    //!
    void CalculateBillboardMtx(nn::util::Matrix4x3fType* pMtxArray, const nn::util::Matrix4x3fType& cameraMtx, int boneIndex) const NN_NOEXCEPT
    {
        CalculateBillboardMtx(pMtxArray, cameraMtx, boneIndex, false);
    }

    //! @brief ビルボード行列を計算します。
    //!
    //! @param[out] pMtxArray 計算結果を受け取る行列へのポインター。
    //! @param[in] cameraMtx カメラ行列への参照。
    //! @param[in] boneIndex ボーンのインデックス。
    //! @param[in] combineWorld ワールド行列を計算結果に含める場合は ture、含めない場合は false。
    //!
    void CalculateBillboardMtx(nn::util::Matrix4x3fType* pMtxArray, const nn::util::Matrix4x3fType& cameraMtx, int boneIndex,
        bool combineWorld) const NN_NOEXCEPT;

    //! @brief ワールド行列から行列パレットを計算します。
    //!
    //! @param[in] bufferIndex 計算を反映させるユニフォームブロックのインデックス。
    //!
    //! @pre
    //! - インデックスはスケルトンが持つユニフォームブロックのバッファー数の範囲内。
    //!
    //! @details
    //! GPU から参照されるバッファーを書き換えるため、
    //! 前回の描画で GPU が参照し終わった後に呼ぶ必要があります。
    //! 構築時のオプションでダブルバッファー化するか、
    //! フレームバッファーのコピーアウト中などに呼ぶことを想定しています。
    //!
    void CalculateSkeleton(int bufferIndex) NN_NOEXCEPT;
    //@}

    //----------------------------------------
    //! @name 取得/設定
    //@{

    //! @brief ユニフォームブロックが要求するアライメントを取得します。
    //!
    //! @return ユニフォームブロックが要求するアライメントを返します。
    //!
    size_t GetBlockBufferAlignment(nn::gfx::Device* pDevice) const NN_NOEXCEPT;

    //! @brief リソースを取得します。
    //!
    //! @return ResSkeleton へのポインターを返します。
    //!
    const ResSkeleton* GetResource() const NN_NOEXCEPT
    {
        return m_pRes;
    }

    //! @brief Builder で Build() 時に渡されたバッファーを取得します。
    //!
    //! @return Builder で Build() 時に渡されたバッファーへのポインターを返します。
    //!
    void* GetBufferPtr() NN_NOEXCEPT
    {
        return m_pBufferPtr;
    }

    //! @brief SetupBlockBuffer() 時に渡されたメモリプールのポインターを取得します。
    //!
    //! @return メモリプールへのポインターを返します。
    //!
    nn::gfx::MemoryPool* GetMemoryPoolPtr() NN_NOEXCEPT
    {
        return m_pMemoryPool;
    }

    //! @brief SetupBlockBuffer() 時に渡されたメモリプールのオフセットを取得します。
    //!
    //! @return メモリプールのオフセットを返します。
    //!
    ptrdiff_t GetMemoryPoolOffset() NN_NOEXCEPT
    {
        return m_MemoryPoolOffset;
    }

    //! @brief ユニフォームブロックが構築済みであるかどうかを取得します。
    //!
    //! @return 構築済みの場合は true、未構築の場合は false を返します。
    //!
    bool IsBlockBufferValid() const NN_NOEXCEPT
    {
        return (m_Flag & Flag_BlockBufferValid) != 0;
    }

    //! @brief 行列のスケーリング方法を取得します。
    //!
    //! @return 行列のスケーリング方法を返します。
    //!
    int GetScaleMode() const NN_NOEXCEPT
    {
        return m_Flag & Mask_Scale;
    }

    //! @brief 行列の回転方法を取得します。
    //!
    //! @return 行列の回転方法を返します。
    //!
    int GetRotateMode() const NN_NOEXCEPT
    {
        return m_Flag & Mask_Rot;
    }

    //! @brief ローカル行列の配列を取得します。
    //!
    //! @return ローカル行列の配列のポインターを返します。
    //!
    LocalMtx* GetLocalMtxArray() NN_NOEXCEPT
    {
        return LocalMtx::DownCast(m_pLocalMtxArray);
    }

    //! @brief ローカル行列の配列を取得します。
    //!
    //! @return ローカル行列の配列のポインターを返します。
    //!
    const LocalMtx* GetLocalMtxArray() const NN_NOEXCEPT
    {
        return LocalMtx::DownCast(m_pLocalMtxArray);
    }

    //! @brief ワールド行列の配列を取得します。
    //!
    //! @return ワールド行列の配列のポインターを返します。
    //!
    nn::util::Matrix4x3fType* GetWorldMtxArray() NN_NOEXCEPT
    {
        return m_pWorldMtxArray;
    }

    //! @brief ワールド行列の配列を取得します。
    //!
    //! @return ローカル行列の配列のポインターを返します。
    //!
    const nn::util::Matrix4x3fType* GetWorldMtxArray() const NN_NOEXCEPT
    {
        return m_pWorldMtxArray;
    }

    //! @brief スケルトンが保持するユニフォームブロックのバッファーを取得します。
    //!
    //! @param[in] bufferIndex ユニフォームブロックのインデックス。
    //!
    //! @return nn::gfx::Buffer へのポインターを返します。
    //!
    //! @pre
    //! - インデックスはスケルトンが持つバッファー数の範囲内。
    //!
    nn::gfx::Buffer* GetMtxBlock(int bufferIndex) NN_NOEXCEPT
    {
        NN_SDK_REQUIRES_RANGE(bufferIndex, 0, GetBufferingCount());
        return m_pMtxBlockArray ? &m_pMtxBlockArray[bufferIndex] : NULL;
    }

    //! @brief スケルトンが保持するユニフォームブロックのバッファーを取得します。
    //!
    //! @param[in] bufferIndex ユニフォームブロックのインデックス。
    //!
    //! @return nn::gfx::Buffer へのポインターを返します。
    //!
    //! @pre
    //! - インデックスはスケルトンが持つバッファー数の範囲内。
    //!
    const nn::gfx::Buffer* GetMtxBlock(int bufferIndex) const NN_NOEXCEPT
    {
        NN_SDK_REQUIRES_RANGE(bufferIndex, 0, GetBufferingCount());
        return m_pMtxBlockArray ? &m_pMtxBlockArray[bufferIndex] : NULL;
    }

    //! @brief ユニフォームブロックのサイズを取得します。
    //!
    //! @return ユニフォームブロックのサイズを返します。
    //!
    size_t GetMtxBlockSize() const NN_NOEXCEPT
    {
        return m_SizePerMtxBlock;
    }

    //! @brief ユニフォームブロックのバッファーリング数を取得します。
    //!
    //! @return ユニフォームブロックのバッファーリング数を返します。
    //!
    int GetBufferingCount() const NN_NOEXCEPT
    {
        return m_BufferingCount;
    }

    //! @brief ユニフォームブロックのエンディアンスワップを行うかどうかを取得します。
    //!
    //! @return ユニフォームブロックをスワップする場合は true、しない場合は false を返します。
    //!
    bool IsBlockSwapEnabled() const NN_NOEXCEPT
    {
#if defined(NN_BUILD_CONFIG_OS_COS)
        return true;
#else
        return false;
#endif
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

    //----------------------------------------
    //! @name ボーン
    //@{

    //! @brief ボーンの数を取得します。
    //!
    //! @return ボーンの数を返します。
    //!
    int GetBoneCount() const NN_NOEXCEPT
    {
        return m_BoneCount;
    }

    //! @brief インデックス引きでボーンを取得します。
    //!
    //! @param[in] boneIndex ボーンのインデックス。
    //!
    //! @return ResBone へのポインターを返します。
    //!
    //! @pre
    //! インデックスはモデルが持つボーン数の範囲内。
    //!
    const ResBone* GetResBone(int boneIndex) NN_NOEXCEPT
    {
        return &m_pBoneArray[boneIndex];
    }

    //! @brief インデックス引きでボーンを取得します。
    //!
    //! @param[in] boneIndex ボーンのインデックス。
    //!
    //! @return ResBone へのポインターを返します。
    //!
    //! @pre
    //! インデックスはモデルが持つボーン数の範囲内。
    //!
    const ResBone* GetResBone(int boneIndex) const NN_NOEXCEPT
    {
        return &m_pBoneArray[boneIndex];
    }

    //! @brief 名前引きでボーンを取得します。
    //!
    //! @param[in] name ボーン名へのポインター。
    //!
    //! @return ResBone へのポインターを返します。
    //!
    //! @details
    //! 指定した名前のボーンが存在しない場合は NULL を返します。
    //!
    //! 名前引きよりインデックス引きほうが高速に動作します。インデックスを
    //! 取得後、記録しておき、通常はインデックス引きを使用することを推奨します。
    //!
    const ResBone* FindResBone(const char* name) NN_NOEXCEPT
    {
        return m_pRes->FindBone(name);
    }

    //! @brief 名前引きでボーンを取得します。
    //!
    //! @param[in] name ボーン名へのポインター。
    //!
    //! @return ResBone へのポインターを返します。
    //!
    //! @details
    //! 指定した名前のボーンが存在しない場合は NULL を返します。
    //!
    //! 名前引きよりインデックス引きほうが高速に動作します。インデックスを
    //! 取得後、記録しておき、通常はインデックス引きを使用することを推奨します。
    //!
    const ResBone* FindResBone(const char* name) const NN_NOEXCEPT
    {
        return m_pRes->FindBone(name);
    }

    //! @brief インデックスからボーン名を取得します。
    //!
    //! @param[in] boneIndex ボーンのインデックス。
    //!
    //! @return ボーン名へのポインターを返します。
    //!
    //! @pre
    //! インデックスはスケルトンが持つボーン数の範囲内。
    //!
    const char* GetBoneName(int boneIndex) const NN_NOEXCEPT
    {
        return m_pRes->GetBoneName(boneIndex);
    }

    //! @brief ボーンの名前からインデックスを取得します。
    //!
    //! @param[in] name ボーン名へのポインター。
    //!
    //! @return ボーンのインデックスを返します。
    //!
    //! @details
    //! 指定した名前のボーンが存在しない場合は  nn::util::ResDic::Npos を返します。
    //!
    int FindBoneIndex(const char* name) const NN_NOEXCEPT
    {
        return m_pRes->FindBoneIndex(name);
    }

    //@}

    //----------------------------------------
    //! @name コールバック
    //@{

    //! @brief ボーンのワールド行列の計算後に呼び出されるコードバックを設定します。
    //!
    //! @param[in] pCallback コールバックへのポインターを返します。
    //!
    void SetCalculateWorldCallback(ICalculateWorldCallback* pCallback) NN_NOEXCEPT
    {
        m_pCallback = pCallback;
    }

    //! @brief ボーンのワールド行列の計算後に呼び出されるコールバックを取得します。
    //!
    //! @return コールバックへのポインターを返します。
    //!
    ICalculateWorldCallback* GetCalculateWorldCallback() NN_NOEXCEPT
    {
        return m_pCallback;
    }

    //! @brief ボーンのワールド行列の計算後に呼び出されるコールバックを取得します。
    //!
    //! @return pCallback コールバックへのポインターを返します。
    //!
    const ICalculateWorldCallback* GetCalculateWorldCallback() const NN_NOEXCEPT
    {
        return m_pCallback;
    }

    //! @brief コールバック呼び出しを行うボーンのインデックスを設定します。
    //!
    //! @param[in] boneIndex ボーンのインデックス。
    //!
    //! @details
    //! コールバック内で ICalculateWorldCallback::SetCallbackBoneIndex() によって
    //! 対象のボーンインデックスを変更することで、複数のボーンに対してコールバックを
    //! 実行することができます。
    //!
    void SetCallbackBoneIndex(int boneIndex) NN_NOEXCEPT
    {
        m_CallbackBone = (boneIndex >= 0 && boneIndex < m_BoneCount) ?
            static_cast<uint16_t>(boneIndex) : ICalculateWorldCallback::InvalidBone;
    }

    //! @brief コールバック呼び出しを行うボーンのインデックスを取得します。
    //!
    //! @return コールバック呼び出しを行うボーンのインデックスを返します。
    //!
    int GetCallbackBoneIndex() const NN_NOEXCEPT
    {
        return m_CallbackBone >= ICalculateWorldCallback::InvalidBone ? -1 : m_CallbackBone;
    }

    //@}

    //! @briefprivate ユニフォームブロックのバッファー配列を設定します。
    //!
    //! @param[in] pBlockArray バッファー配列へのポインター。
    //!
    void SetMtxBlockArray(nn::gfx::Buffer* pBlockArray) NN_NOEXCEPT
    {
        m_pMtxBlockArray = pBlockArray;
    }

    //! @briefprivate ユニフォームブロックのバッファー配列を取得します。
    //!
    //! @return バッファー配列へのポインターを返します。
    //!
    nn::gfx::Buffer* GetMtxBlockArray() NN_NOEXCEPT
    {
        return m_pMtxBlockArray;
    }

protected:
    class Impl;

    //! @briefprivate ワールド行列計算を行う内部関数です。
    //!
    template<typename CalculateType, bool useCallback>
    void CalculateWorldImpl(const nn::util::Matrix4x3fType& baseMtx);

private:
    class InitializeArgument;
    //! @brief インスタンスの初期化を行います。
    bool Initialize(const InitializeArgument& arg, void* pBuffer, size_t bufferSize) NN_NOEXCEPT;
    void SetupBlockBufferImpl(nn::gfx::Device* pDevice, nn::gfx::MemoryPool* pMemoryPool, ptrdiff_t offset, size_t memoryPoolSize) NN_NOEXCEPT;

    const ResSkeleton*        m_pRes;

    Bit16             m_Flag;
    uint8_t                   m_BufferingCount;
    NN_PADDING1;

    const ResBone*            m_pBoneArray;
    LocalMtxData*             m_pLocalMtxArray;
    nn::util::Matrix4x3fType*     m_pWorldMtxArray;
    nn::util::Vector3fType*       m_pScaleArray; // Softimage の Hierarchical Scaling で使用

    nn::gfx::MemoryPool*      m_pMemoryPool;
    ptrdiff_t                 m_MemoryPoolOffset;
    nn::gfx::Buffer*          m_pMtxBlockArray; // 頂点と法線の行列用ユニフォームバッファー
    size_t                    m_SizePerMtxBlock;

    uint16_t                  m_BoneCount;
    uint16_t                  m_CallbackBone;
    ICalculateWorldCallback*  m_pCallback;

    void* m_pMemLocalMtxArray;
    void* m_pMemWorldMtxArray;
    void* m_pUserPtr;
    void* m_pBufferPtr;

    NN_DISALLOW_COPY(SkeletonObj);
};

//--------------------------------------------------------------------------------------------------

//! @briefprivate SkeletonObj::Initialize() に渡して初期化を行うパラメータです。
class SkeletonObj::InitializeArgument
{
public:
    //! @brief メモリブロックのインデックスを示す列挙子。
    enum MemoryBlockIndex
    {
        MemoryBlockIndex_WorldMtx,
        MemoryBlockIndex_LocalMtx,
        MemoryBlockIndex_Scale,
        MemoryBlockIndex_Buffer,
        MemoryBlockIndex_End
    };

    //! @brief コンストラクタです。
    //!
    //! @param[in] resource ResSkeleton へのポインター。
    //!
    explicit InitializeArgument(const ResSkeleton* resource) NN_NOEXCEPT
        : m_pRes(resource)
        , m_BufferingCount(1)
        , m_MemoryBlock()
    {
        NN_SDK_REQUIRES_NOT_NULL(resource);
        m_WorkMemory.Invalidate();
    }

    //! @brief ユニフォームブロックをバッファーリングする数を設定します。
    //!
    //! @param[in] count ユニフォームブロックをバッファーリングする数。
    //!
    void BufferingCount(int count) NN_NOEXCEPT
    {
        m_BufferingCount = count;
    }

    //! @brief ユニフォームブロックのバッファーリング数を取得します。
    //!
    //! @return ユニフォームブロックのバッファーリング数を返します。
    //!
    int GetBufferingCount() const NN_NOEXCEPT
    {
        return m_BufferingCount;
    }

    //! @brief リソースを取得します。
    //!
    //! @return ResSkeleton へのポインターを返します。
    //!
    const ResSkeleton* GetResource() const NN_NOEXCEPT
    {
        return m_pRes;
    }

    //! @brief SkeletonObj構築に必要なメモリサイズを取得します。
    //!
    //! @return SkeletonObj 構築に必要なメモリサイズを返します。
    //!
    size_t GetWorkMemorySize() const NN_NOEXCEPT
    {
        return m_WorkMemory.GetSize();
    }

    //! @brief SkeletonObj 構築に必要なメモリサイズを計算します。
    //!
    //! @pre
    //! - 設定されたユニフォームブロックのバッファーリング数が１以上。
    //!
    void CalculateMemorySize() NN_NOEXCEPT;

    //! @briefprivate インデックスで指定されたバッファーを取得します。
    void* GetBuffer( void* pHead, int index ) const NN_NOEXCEPT
    {
        NN_SDK_REQUIRES_RANGE(index, 0, MemoryBlockIndex_End);
        return m_MemoryBlock[index].Get(pHead);
    }

    //! @briefprivate インデックスで指定されたバッファーを取得します。
    template< typename T >
    T* GetBuffer( void* pHead, int index ) const NN_NOEXCEPT
    {
        NN_SDK_REQUIRES_RANGE(index, 0, MemoryBlockIndex_End);
        return static_cast<T*>( GetBuffer(pHead, index) );
    }

    //! @brief SkeletonObj構築に必要なメモリサイズが計算済みかを取得します。
    bool IsMemoryCalculated() const NN_NOEXCEPT
    {
        return m_WorkMemory.IsValid();
    }

private:
    const ResSkeleton* m_pRes;
    int m_BufferingCount;
    nn::util::MemorySplitter m_WorkMemory;
    nn::util::MemorySplitter::MemoryBlock m_MemoryBlock[MemoryBlockIndex_End];
};

//! @brief SkeletonObj を構築するクラスです。
class SkeletonObj::Builder : public SkeletonObj::InitializeArgument
{
public:
    //! @brief コンストラクタです。
    //!
    //! @param[in] resource ResSkeleton へのポインター
    //!
    explicit Builder(const ResSkeleton* resource) NN_NOEXCEPT
        : InitializeArgument(resource)
    {
        NN_SDK_ASSERT_NOT_NULL(resource);
    }

    //! @brief SkeletonObj を構築します。
    //!
    //! @param[in] pSkeletonObj SkeletonObj へのポインター。
    //! @param[in] pBuffer バッファーへのポインター。
    //! @param[in] bufferSize バッファーのサイズ。
    //!
    //! @pre
    //! - CalculateMemorySize() を呼び、メモリサイズが計算済みである。
    //! - pBuffer は Alignment_Buffer のアライメント上にある。
    //! - bufferSize >= GetWorkMemorySize() で返すサイズ。
    //!
    bool Build(SkeletonObj* pSkeletonObj, void* pBuffer, size_t bufferSize) NN_NOEXCEPT
    {
        return pSkeletonObj->Initialize(*this, pBuffer, bufferSize);
    }
};

}} // namespace nn::g3d

