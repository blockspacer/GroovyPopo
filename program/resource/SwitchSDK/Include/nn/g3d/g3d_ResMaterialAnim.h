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
* @brief マテリアルアニメーションに関するリソースクラスの宣言
*/

#pragma once

#include <nn/util/util_ResDic.h>
#include <nn/g3d/g3d_Configuration.h>
#include <nn/g3d/g3d_ResCommon.h>
#include <nn/g3d/g3d_ResAnimCurve.h>
#include <nn/g3d/g3d_ResModel.h>
#include <nn/g3d/g3d_Binding.h>

NN_PRAGMA_PUSH_WARNINGS
#if defined(NN_BUILD_CONFIG_TOOLCHAIN_VC)
NN_ENABLE_WARNING_IMPLICIT_PADDING_WITHIN_CLASS
#endif

namespace nn { namespace g3d {

//! @brief シェーダーパラメータアニメーションの要素情報です。
struct ResShaderParamAnimInfo
{
    nn::util::BinPtrToString pName; //!< シェーダーパラメータ名。
    uint16_t beginCurve; //!< ResPerMaterialAnimData 内でのカーブの先頭インデックス。
    uint16_t floatCurveCount; //!< 浮動小数点カーブの数。
    uint16_t intCurveCount; //!< 整数カーブの数。
    uint16_t beginConstant; //!< ResPerMaterialAnimData 内での定数配列の先頭インデックス。
    uint16_t constantCount; //!< 定数アニメーションの数。
    uint16_t subbindIndex; //!< マテリアル内でのシェーダーパラメータのインデックス。
    uint8_t  reserved[4];
};

//! @brief テクスチャーパターンアニメーションの要素情報です。
struct ResTexturePatternAnimInfo
{
    nn::util::BinPtrToString pName; //!< サンプラー名。
    uint16_t curveIndex; //!< ResPerMaterialAnimData 内でのカーブのインデックス。
    uint16_t constantIndex; //!< ResPerMaterialAnimData 内での定数配列のインデックス。
    int8_t   subbindIndex; //!< マテリアル内でのテクスチャーのインデックス
    uint8_t  reserved[3];
};

//! @brief マテリアル毎のアニメーションの構造体です。
struct ResPerMaterialAnimData
{
    nn::util::BinPtrToString pName; //!< マテリアル名。
    nn::util::BinTPtr<ResShaderParamAnimInfo>    pShaderParamAnimInfoArray; //!< シェーダーパラメータアニメーション情報配列へのポインター。
    nn::util::BinTPtr<ResTexturePatternAnimInfo> pTexturePatternAnimInfoArray; //!< テクスチャーパターンアニメーション情報配列へのポインター。
    nn::util::BinTPtr<ResAnimCurve>              pCurveArray; //!< アニメーションカーブ配列へのポインター。
    nn::util::BinTPtr<ResAnimConstant>           pConstantArray; //!< 定数アニメーション配列へのポインター。初期値から変化しないパラメータに使用。
    uint16_t beginShaderParamCurveIndex; //!< ResMaterialAnim 内のカーブの評価結果を格納するバッファーの先頭インデックス。
    uint16_t beginTexturePatternCurveIndex; //!< ResMaterialAnim 内のカーブの評価結果を格納するバッファーの先頭インデックス。
    uint16_t beginVisibilityCurveIndex; //!< ResMaterialAnim 内のカーブの評価結果を格納するバッファーの先頭インデックス。
    uint16_t visibilityCurveIndex; //!< ビジビリティ用カーブのインデックス。
    uint16_t visibilityConstantIndex; //!< ビジビリティ用定数のインデックス。
    uint16_t shaderParamAnimCount; //!< シェーダーパラメータアニメーション数。
    uint16_t texturePatternAnimCount; //!< テクスチャーパターンアニメーション数。
    uint16_t constantCount; //!< 定数アニメーション配列の数。
    uint16_t curveCount;//!< 総アニメーションカーブ数。
    uint8_t  reserved[6];
};

//! @brief マテリアルアニメーションのリソースです。
class ResPerMaterialAnim : public nn::util::AccessorBase< ResPerMaterialAnimData >
{
    NN_DISALLOW_COPY(ResPerMaterialAnim);

public:

    //----------------------------------------
    //! @name 構築/破棄
    //@{

    //! @brief 事前にマテリアルと関連付けを行います。
    //!
    //! @param[in] pTarget ResMaterial へのポインター。
    //!
    //! @return BindResult を返します。
    //!
    //! @details
    //! リソース同士を PreBind() によって事前に関連付けることにより、
    //! MaterialAnimObj::BindFast 時には名前引きを行わない比較的高速な関連付けを行います。
    //!
    BindResult PreBind(const ResMaterial* pTarget) NN_NOEXCEPT;

    //! @brief マテリアルとの関連付けが成功するか、チェックのみを行います。
    //!
    //! @param[in] pTarget ResMaterial へのポインター。
    //!
    //! @return BindResult を返します。
    //!
    //! @details
    //! 対象のマテリアルに対して、アニメーションの関連付けが成功するかをチェックします。
    //! 実際の関連付け処理は行いません。
    //!
    BindResult BindCheck(const ResMaterial* pTarget) const NN_NOEXCEPT;

    //@}

    //----------------------------------------
    //! @name 評価
    //@{

    //! @brief シェーダーパラメータアニメーションを評価します。
    //!
    //! @tparam useFrameCache 計算効率化のためのキャッシュを使用するか、しないか。
    //!
    //! @param[out] pResult アニメーション結果を格納するバッファーへのポインター。
    //! @param[in] frame フレーム位置。
    //! @param[in] pSubBindIndex アニメーションを関連付けたシェーダーパラメータのインデックスを格納した配列へのポインター。
    //! @param[in] pFrameCache 計算の効率化のために使用されるキャッシュへのポインター。
    //!
    template <bool useFrameCache>
    void EvaluateShaderParamAnim(void* pResult, float frame, const uint16_t* pSubBindIndex,
        AnimFrameCache* pFrameCache) const NN_NOEXCEPT;

    //! @brief テクスチャーパターンアニメーションを評価します。
    //!
    //! @tparam useFrameCache 計算効率化のためのキャッシュを使用するか、しないか。
    //!
    //! @param[out] pResult アニメーション結果を格納するバッファーへのポインター。
    //! @param[in] frame フレーム位置。
    //! @param[in] pSubBindIndex アニメーションを関連付けたシェーダーパラメータのインデックスを格納した配列へのポインター。
    //! @param[in] pFrameCache 計算の効率化のために使用されるキャッシュへのポインター。
    //!
    template <bool useFrameCache>
    void EvaluateTexturePatternAnim(int32_t* pResult, float frame, const uint16_t* pSubBindIndex,
        AnimFrameCache* pFrameCache) const NN_NOEXCEPT;

    //! @brief ビジビリティアニメーションを評価します。
    //!
    //! @tparam useFrameCache 計算効率化のためのキャッシュを使用するか、しないか。
    //!
    //! @param[out] pResult アニメーション結果を格納するバッファーへのポインター。
    //! @param[in] frame フレーム位置。
    //! @param[in] pFrameCache 計算の効率化のために使用されるキャッシュへのポインター。
    //!
    template <bool useFrameCache>
    void EvaluateVisibilityAnim(int32_t* pResult, float frame, AnimFrameCache* pFrameCache) const NN_NOEXCEPT;

    //@}

    //----------------------------------------
    //! @name 取得/設定
    //@{

    //! @brief シェーダーパラメータアニメーション数を取得します。
    //!
    //! @return シェーダーパラメータアニメーション数を返します。
    //!
    int GetShaderParamAnimCount() const NN_NOEXCEPT
    {
        return ToData().shaderParamAnimCount;
    }

    //! @brief テクスチャーパターンアニメーション数を取得します。
    //!
    //! @return テクスチャーパターンアニメーション数を返します。
    //!
    int GetTexturePatternAnimCount() const NN_NOEXCEPT
    {
        return ToData().texturePatternAnimCount;
    }

    //! @brief ビジビリティのアニメーションが有効かを返します。
    //!
    //! @return ビジビリティのアニメーションが有効の場合は true、無効の場合は false を返します。
    //!
    bool IsVisibilityAnimEnabled() const NN_NOEXCEPT
    {
        return !(visibilityCurveIndex == AnimFlag_NotBound) ||
            !(visibilityConstantIndex == AnimFlag_NotBound);
    }

    //! @brief ビジビリティのカーブアニメーションが有効かを返します。
    //!
    //! @return ビジビリティのカーブアニメーションが有効の場合は true、無効の場合は false を返します。
    //!
    bool IsVisibilityCurveAnimEnabled() const NN_NOEXCEPT
    {
        return !(visibilityCurveIndex == AnimFlag_NotBound);
    }

    //! @brief マテリアル名を取得します。
    //!
    //! @return マテリアル名へのポインター。
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

    //! @brief 定数アニメーションの数を取得します。
    //!
    //! @return 定数アニメーションの数を返します。
    //!
    int GetConstantCount() const NN_NOEXCEPT
    {
        return ToData().constantCount;
    }
    //! @brief インデックス引きで定数アニメーションを取得します。
    //!
    //! @param[in] elemIndex 定数アニメーションのインデックス。
    //!
    //! @return ResAnimConstant へのポインターを返します。
    //!
    //! @pre
    //! - インデックスは定数アニメーションの数の範囲内。
    //!
    //! @details
    //! 指定したインデックスが範囲外の場合は ASSERT します。
    //!
    ResAnimConstant* GetConstant(int elemIndex) NN_NOEXCEPT
    {
        NN_G3D_REQUIRES_RANGE(elemIndex, 0, GetConstantCount(), GetName());
        ResAnimConstant* pArray = ToData().pConstantArray.Get();
        NN_SDK_ASSERT_NOT_NULL(pArray);
        return &pArray[elemIndex];
    }
    //! @brief インデックス引きで定数アニメーションを取得します。
    //!
    //! @param[in] elemIndex 定数アニメーションのインデックス。
    //!
    //! @return ResAnimConstant へのポインターを返します。
    //!
    //! @pre
    //! - インデックスは定数アニメーションの数の範囲内。
    //!
    //! @details
    //! 指定したインデックスが範囲外の場合は ASSERT します。
    //!
    const ResAnimConstant* GetConstant(int elemIndex) const NN_NOEXCEPT
    {
        NN_G3D_REQUIRES_RANGE(elemIndex, 0, GetConstantCount(), GetName());
        const ResAnimConstant* pArray = ToData().pConstantArray.Get();
        NN_SDK_ASSERT_NOT_NULL(pArray);
        return &pArray[elemIndex];
    }

    //! @brief シェーダーパラメータアニメーションの情報を取得します。
    //!
    //! @param[in] elemIndex シェーダーパラメータアニメーションの情報のインデックス。
    //!
    //! @return ResShaderParamAnimInfo へのポインターを返します。
    //!
    //! @pre
    //! - インデックスはシェーダーパラメータアニメーションの数の範囲内。
    //!
    //! @details
    //! 指定したインデックスが範囲外の場合は ASSERT します。
    //!
    ResShaderParamAnimInfo* GetShaderParamAnimInfo(int elemIndex) NN_NOEXCEPT
    {
        NN_G3D_REQUIRES_RANGE(elemIndex, 0, GetShaderParamAnimCount(), GetName());
        ResShaderParamAnimInfo* pArray = ToData().pShaderParamAnimInfoArray.Get();
        NN_SDK_ASSERT_NOT_NULL(pArray);
        return &pArray[elemIndex];
    }
    //! @brief シェーダーパラメータアニメーションの情報を取得します。
    //!
    //! @param[in] elemIndex シェーダーパラメータアニメーションの情報のインデックス。
    //!
    //! @return ResShaderParamAnimInfo へのポインターを返します。
    //!
    //! @pre
    //! - インデックスはシェーダーパラメータアニメーションの数の範囲内。
    //!
    //! @details
    //! 指定したインデックスが範囲外の場合は ASSERT します。
    //!
    const ResShaderParamAnimInfo* GetShaderParamAnimInfo(int elemIndex) const NN_NOEXCEPT
    {
        NN_G3D_REQUIRES_RANGE(elemIndex, 0, GetShaderParamAnimCount(), GetName());
        const ResShaderParamAnimInfo* pArray = ToData().pShaderParamAnimInfoArray.Get();
        NN_SDK_ASSERT_NOT_NULL(pArray);
        return &pArray[elemIndex];
    }

    //@}
};

//--------------------------------------------------------------------------------------------------

struct ResMaterialAnimData
{
    nn::util::BinBlockSignature                    signature; //!< シグネチャー。
    Bit16                                          flag; //!< フラグ。
    uint8_t                                        reserved0[2];
    nn::util::BinPtrToString                       pName; //!< 名前。
    nn::util::BinPtrToString                       pPath; //!< パス名。
    nn::util::BinTPtr<ResModel>                    pBindModel; //!< 結び付けたモデルへのポインター。
    nn::util::BinTPtr<uint16_t>                    pBindIndexArray; //!< 結び付けたマテリアルのインデックス。
    nn::util::BinTPtr<ResPerMaterialAnim>          pPerMaterialAnimArray; //!< マテリアル毎のアニメーション配列へのポインター。
    nn::util::BinTPtr<const nn::gfx::TextureView*> pTextureArray; //!< テクスチャーへのポインター。
    nn::util::BinTPtr<nn::util::BinPtrToString>    pTextureNameArray; //!< テクスチャー名の配列へのポインター。
    nn::util::BinTPtr<nn::gfx::ResUserData>        pUserDataArray; //!< ユーザーデータ配列へのポインター。
    nn::util::BinTPtr<nn::util::ResDic>            pUserDataDic; //!< ユーザーデータ辞書へのポインター。
    nn::util::BinTPtr<nn::gfx::DescriptorSlot>     pTextureSlotArray; //!< テクスチャーのスロット番号を格納する配列へのポインター。
    int32_t  frameCount; //!< フレーム数。
    uint32_t bakedSize; //!< コマ化に必要なバッファーサイズ。
    uint16_t userDataCount; //!< ユーザーデータ数。
    uint16_t perMaterialAnimCount; //!< マテリアルアニメーション数。
    // Context のサイズに影響
    uint16_t curveCount; //!< カーブ数。
    uint16_t shaderParamAnimCount; //!< 配下のシェーダーパラメータアニメーションの総数。
    uint16_t texturePatternAnimCount; //!< 配下のテクスチャーパターンアニメーションの総数。
    uint16_t visibilityAnimCount; //!< 配下のビジビリティアニメーションの総数。
    uint16_t textureCount; //!< テクスチャー数。
    uint8_t  reserved1[2];
};

//! @brief マテリアルアニメーションのリソースです。
class ResMaterialAnim : public nn::util::AccessorBase< ResMaterialAnimData >
{
    NN_DISALLOW_COPY(ResMaterialAnim);

public:
    //! @brief ResMaterialAnim のシグネチャ
    static const uint32_t Signature = NN_UTIL_CREATE_SIGNATURE_4('F', 'M', 'A', 'A');

    //! @brief シェーダーパラメータアニメーションに関するフラグです。
    enum Flag
    {
        Flag_CurveBaked     = AnimFlag_CurveBaked, //!< @brief カーブがベイク済みです。
        Flag_PlayPolicyLoop = AnimFlag_PlayPolicyLoop //!< @brief ループすることを表します。
    };

    //----------------------------------------
    //! @name 構築/破棄
    //@{

    //! @brief テクスチャーの参照解決を行います。
    //!
    //! @param[in] pCallback テクスチャーの参照解決時に呼び出されるコールバックへのポインター。
    //! @param[in] pUserData コールバック呼び出し時に引数で渡されるユーザーデータへのポインター。
    //!
    //! @return BindResult を返します。
    //!
    //! @details
    //! 引数にテクスチャー名を渡し、pCallback を呼び出します。pCallback は適切な nn::g3d::TextureRef
    //! を返してください。有効でない nn::g3d::TextureRef がコールバックの返り値として返された場合、
    //! そのテクスチャーの参照解決は失敗したと扱われます。
    //! テクスチャーの参照解決が行われないまま描画処理を行うと不適切なアクセスが発生する可能性があります。
    //! 返り値を確認し、テクスチャーの参照解決が成功したことを確認してください。
    //!
    //! 参照が解決されていないテクスチャーに対してのみ、コールバックは呼び出されます。
    //! 参照が解決済みかどうかは nn::g3d::ResMaterialAnim::IsTextureBound() に
    //! よって判定されます。
    //!
    BindResult BindTexture(TextureBindCallback pCallback, void* pUserData) NN_NOEXCEPT;

    //! @brief 指定した名前のテクスチャーの参照を差し替えます。
    //!
    //! @param[in] textureRef 差し替えるテクスチャーのビューおよびディスクリプタスロットを格納した nn::g3d::TextureRef への参照。
    //! @param[in] name 差し替えるテクスチャーの名前。
    //!
    //! @return 差し替えが成功した場合は true、失敗した場合は false を返します。
    //!
    bool ForceBindTexture(const TextureRef& textureRef, const char* name) NN_NOEXCEPT;

    //! @brief テクスチャーの参照設定を解除します。
    void ReleaseTexture() NN_NOEXCEPT;

    //! @brief 事前にモデルと関連付けを行います。
    //!
    //! @param[in] pModel モデルへのポインター。
    //!
    //! @return BindResult を返します。
    //!
    //! @details
    //! リソース同士を PreBind() によって事前に関連付けることにより、
    //! MaterialAnimObj::BindFast 時には名前引きを行わない比較的高速な関連付けを行います。
    //!
    BindResult PreBind(const ResModel* pModel) NN_NOEXCEPT;

    //! @brief モデルとの関連付けが成功するか、チェックのみを行います。
    //!
    //! @param[in] pModel モデルへのポインター。
    //!
    //! @return BindResult を返します。
    //!
    //! @details
    //! 対象のモデルに対して、アニメーションの関連付けが成功するかをチェックします。
    //! 実際の関連付け処理は行いません。
    //!
    BindResult BindCheck(const ResModel* pModel) const NN_NOEXCEPT;

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
    //! @name 取得/設定
    //@{

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

    //! @brief 内包するシェーダーパラメータアニメーションの総数を取得します。
    //!
    //! @return 内包するシェーダーパラメータアニメーションの総数を返します。
    //!
    int GetShaderParamAnimCount() const NN_NOEXCEPT
    {
        return ToData().shaderParamAnimCount;
    }

    //! @brief 内包するテクスチャーパターンアニメーションの総数を取得します。
    //!
    //! @return 内包するテクスチャーパターンアニメーションの総数を返します。
    //!
    int GetTexturePatternAnimCount() const NN_NOEXCEPT
    {
        return ToData().texturePatternAnimCount;
    }

    //! @brief 内包するビジビリティアニメーションの総数を取得します。
    //!
    //! @return 内包するビジビリティアニメーションの総数を返します。
    //!
    int GetVisibilityAnimCount() const NN_NOEXCEPT
    {
        return ToData().visibilityAnimCount;
    }

    //! @brief シェーダーパラメータアニメーションとテクスチャーパターンアニメーションの総和を取得します。
    //!
    //! @return シェーダーパラメータアニメーションとテクスチャーパターンアニメーションの総和を返します。
    //!
    int GetParamAnimCount() const NN_NOEXCEPT
    {
        return GetShaderParamAnimCount() + GetTexturePatternAnimCount();
    }

    //! @brief 内包するアニメーションカーブの総数を取得します。
    //!
    //! @return 内包するアニメーションカーブの総数を返します。
    //!
    int GetCurveCount() const NN_NOEXCEPT
    {
        return ToData().curveCount;
    }

    //! @brief コマ化に必要なバッファーサイズを取得します。
    //!
    //! @return コマ化に必要なバッファーサイズを返します。
    //!
    size_t GetBakedSize() const NN_NOEXCEPT
    {
        return ToData().bakedSize;
    }

    //! @brief アニメーション名を取得します。
    //!
    //! @return アニメーション名へのポインターを返します。
    //!
    const char* GetName() const NN_NOEXCEPT
    {
        return ToData().pName.Get()->GetData();
    }
    //! @brief パス名を取得します。
    //!
    //! @return パス名へのポインターを返します。
    //!
    const char* GetPath() const NN_NOEXCEPT
    {
        return ToData().pPath.Get()->GetData();
    }

    //! @brief 結び付けたモデルを取得します。
    //!
    //! @return モデルへのポインターを返します。
    //!
    ResModel* GetBindModel() NN_NOEXCEPT
    {
        return ToData().pBindModel.Get();
    }
    //! @brief 結び付けたモデルを取得します。
    //!
    //! @return モデルへのポインターを返します。
    //!
    const ResModel* GetBindModel() const NN_NOEXCEPT
    {
        return ToData().pBindModel.Get();
    }

    //! @brief マテリアルアニメーションの数を取得します。
    //!
    //! @return マテリアルアニメーションの数を返します。
    //!
    int GetPerMaterialAnimCount() const NN_NOEXCEPT
    {
        return ToData().perMaterialAnimCount;
    }
    //! @brief インデックス引きでマテリアルアニメーションを取得します。
    //!
    //! @param[in] elemIndex マテリアルアニメーションのインデックス。
    //!
    //! @return ResPerMaterialAnim へのポインターを返します。
    //!
    //! @pre
    //! - インデックスはマテリアルアニメーションの数の範囲内。
    //!
    //! @details
    //! 指定したインデックスが範囲外の場合は ASSERT します。
    //!
    ResPerMaterialAnim* GetPerMaterialAnim(int elemIndex) NN_NOEXCEPT
    {
        NN_G3D_REQUIRES_RANGE(elemIndex, 0, GetPerMaterialAnimCount(), GetName());
        ResPerMaterialAnim* pArray = ToData().pPerMaterialAnimArray.Get();
        NN_SDK_ASSERT_NOT_NULL(pArray);
        return &pArray[elemIndex];
    }
    //! @brief インデックス引きでマテリアルアニメーションを取得します。
    //!
    //! @param[in] elemIndex マテリアルアニメーションのインデックス。
    //!
    //! @return ResPerMaterialAnim へのポインターを返します。
    //!
    //! @pre
    //! - インデックスはマテリアルアニメーションの数の範囲内。
    //!
    //! @details
    //! 指定したインデックスが範囲外の場合は ASSERT します。
    //!
    const ResPerMaterialAnim* GetPerMaterialAnim(int elemIndex) const NN_NOEXCEPT
    {
        NN_G3D_REQUIRES_RANGE(elemIndex, 0, GetPerMaterialAnimCount(), GetName());
        const ResPerMaterialAnim* pArray = ToData().pPerMaterialAnimArray.Get();
        NN_SDK_ASSERT_NOT_NULL(pArray);
        return &pArray[elemIndex];
    }

    //! @brief テクスチャーの数を取得します。
    //!
    //! @return テクスチャーの数を返します。
    //!
    int GetTextureCount() const NN_NOEXCEPT
    {
        return ToData().textureCount;
    }

    //! @brief テクスチャーのインデックスから名前を取得します。
    //!
    //! @param[in] elemIndex テクスチャーのインデックス。
    //!
    //! @return テクスチャー名へのポインターを返します。
    //!
    //! @pre
    //! - インデックスはテクスチャーの数の範囲内。
    //!
    //! @details
    //! 指定したインデックスが範囲外の場合は ASSERT します。
    //!
    const char* GetTextureName(int elemIndex) const NN_NOEXCEPT
    {
        NN_G3D_REQUIRES_RANGE(elemIndex, 0, GetTextureCount(), GetName());
        return (ToData().pTextureNameArray.Get())[elemIndex].Get()->GetData();
    }

    //! @brief インデックス引きでテクスチャーを取得します。
    //!
    //! @param[in] elemIndex テクスチャーのインデックス。
    //!
    //! @return nn::gfx::TextureView へのポインターを返します。
    //!
    //! @pre
    //! - インデックスはテクスチャー参照の数の範囲内。
    //!
    //! @details
    //! 指定したインデックスが範囲外の場合は ASSERT します。
    //!
    const nn::gfx::TextureView* GetTexture(int elemIndex) const NN_NOEXCEPT
    {
        NN_G3D_REQUIRES_RANGE(elemIndex, 0, GetTextureCount(), GetName());
        const nn::gfx::TextureView* const* pArray = ToData().pTextureArray.Get();
        NN_SDK_ASSERT_NOT_NULL(pArray);
        return pArray[elemIndex];
    }

    //! @brief テクスチャーを設定します。
    //!
    //! @param[in] texIndex テクスチャーのインデックス。
    //! @param[in] pTextureView 設定するテクスチャーの nn::gfx::TextureView へのポインター。
    //!
    //! @pre
    //! - インデックスはテクスチャーの数の範囲内。
    //!
    //! @details
    //! 指定したインデックスが範囲外の場合は ASSERT します。
    //!
    void SetTexture(int texIndex, const nn::gfx::TextureView* pTextureView) NN_NOEXCEPT
    {
        NN_G3D_REQUIRES_RANGE(texIndex, 0, GetTextureCount(), GetName());
        const nn::gfx::TextureView** pArray = ToData().pTextureArray.Get();
        NN_SDK_ASSERT_NOT_NULL(pArray);
        pArray[texIndex] = pTextureView;
    }

    //! @brief インデックスで示されるテクスチャーが登録されているディスクリプタスロットを設定します。
    //!
    //! @param[in] texIndex テクスチャーのインデックス。
    //! @param[in] descriptorSlot テクスチャーを登録したディスクリプタスロット。
    //!
    //! @pre
    //! - テクスチャーのインデックスはテクスチャーの数の範囲内。
    //!
    //! @details
    //! 指定したインデックスが範囲外の場合は ASSERT します。
    //!
    void SetTextureDescriptorSlot(int texIndex, const nn::gfx::DescriptorSlot& descriptorSlot) NN_NOEXCEPT
    {
        NN_G3D_REQUIRES_RANGE(texIndex, 0, GetTextureCount(), GetName());
        nn::gfx::DescriptorSlot* pArray = ToData().pTextureSlotArray.Get();
        NN_SDK_ASSERT_NOT_NULL(pArray);
        pArray[texIndex] = descriptorSlot;
    }

    //! @brief インデックスで示されるテクスチャーが登録されているディスクリプタスロットを取得します。
    //!
    //! @param[in] texIndex テクスチャーのインデックス。
    //!
    //! @return ディスクリプタスロットを返します。
    //!
    //! @pre
    //! - テクスチャーのインデックスはテクスチャーの数の範囲内。
    //! - SetTextureDescriptorSlot もしくは BindTexture で適切なディスクリプタスロットを設定している。
    //!
    //! @details
    //! 指定したインデックスが範囲外の場合は ASSERT します。
    //! 正しいディスクリプタスロットを取得するには SetTextureDescriptorSlot もしくは BindTexture で適切なディスクリプタスロットを
    //! 設定している必要があります。
    //!
    nn::gfx::DescriptorSlot GetTextureDescriptorSlot(int texIndex) const NN_NOEXCEPT
    {
        NN_G3D_REQUIRES_RANGE(texIndex, 0, GetTextureCount(), GetName());
        const nn::gfx::DescriptorSlot* pArray = ToData().pTextureSlotArray.Get();
        NN_SDK_ASSERT_NOT_NULL(pArray);
        return pArray[texIndex];
    }

    //! @brief テクスチャーが設定されているかどうかを取得します。
    //!
    //! @param[in] texIndex テクスチャーのインデックス。
    //!
    //! @return テクスチャーが設定されている場合は true、設定されていない場合は false を返します。
    //!
    //! @pre
    //! - インデックスはテクスチャーの数の範囲内。
    //!
    //! @details
    //! 指定したインデックスが範囲外の場合は ASSERT します。
    //! 設定されているテクスチャービューがNULLでなく、ディスクリプタスロットが有効な場合に true になります。
    //!
    bool IsTextureBound(int texIndex) const NN_NOEXCEPT
    {
        NN_G3D_REQUIRES_RANGE(texIndex, 0, GetTextureCount(), GetName());
        return (GetTexture(texIndex) != NULL && GetTextureDescriptorSlot(texIndex).IsValid());
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
    int GetUserDataIndex(const char* str) const NN_NOEXCEPT
    {
        const nn::util::ResDic* pDic = ToData().pUserDataDic.Get();
        if (pDic == NULL)
        {
            return nn::util::ResDic::Npos;
        }
        return pDic->FindIndex(str);
    }
    //! @briefユーザーデータのインデックスから名前を取得します。
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

}} // namespace nn::g3d

NN_PRAGMA_POP_WARNINGS
