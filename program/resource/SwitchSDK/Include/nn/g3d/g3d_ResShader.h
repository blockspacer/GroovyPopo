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
* @brief シェーダーアーカイブに関するリソースクラスの宣言
*/

#pragma once

#include <nn/os/os_Mutex.h>
#include <nn/os/os_MemoryFence.h>
#include <nn/gfx.h>
#include <nn/util/util_ResDic.h>
#include <nn/g3d/g3d_Configuration.h>
#include <nn/g3d/g3d_ResCommon.h>

NN_PRAGMA_PUSH_WARNINGS
NN_DISABLE_WARNING_SHADOW
#if defined(NN_BUILD_CONFIG_TOOLCHAIN_VC)
NN_ENABLE_WARNING_IMPLICIT_PADDING_WITHIN_CLASS
#endif

namespace nn { namespace g3d {

class ResShadingModel;
class ResShaderArchive;

static const int ShaderLocationNone        = 0xFFFFFFFF;
static const int ShaderLocationOffsetNone  = -1;
static const int ShaderLocationProgramNone = -1;

//! @brief シェーダーステージを表します。
enum Stage
{
    Stage_Vertex,                 //!< 頂点シェーダー
    Stage_Geometry,               //!< ジオメトリシェーダー
    Stage_Pixel,                  //!< ピクセルシェーダー
    Stage_Compute ,               //!< コンピュートシェーダー
    Stage_TessellationControl,    //!< テッセレーション制御シェーダー
    Stage_TessellationEvaluation, //!< テッセレーション評価シェーダー
    Stage_End                     //!< @briefprivate
};

//! @brief シェーダーバリエーション範囲の構造体です。
//!
//! 内部の変数は直接使用しないでください。
//!
struct ShaderRange
{
    //! @briefprivate
    const Bit32* pBegin;
    //! @briefprivate
    const Bit32* pEnd;
};

//--------------------------------------------------------------------------------------------------

//! @brief シェーダープログラムの構造体です。
struct ResShaderProgramData
{
    nn::util::BinTPtr<int32_t> pSamplerTable; //!< サンプラーのロケーションテーブルへのポインター。
    nn::util::BinTPtr<int32_t> pImageTable; //!< イメージのロケーションテーブルへのポインター。
    nn::util::BinTPtr<int32_t> pUniformBlockTable; //!< ユニフォームブロックのロケーションテーブルへのポインター。
    nn::util::BinTPtr<int32_t> pShaderStorageBlockTable; //!< シェーダーストレージブロックのロケーションテーブルへのポインター。
    nn::util::BinTPtr<nn::gfx::ResShaderVariation> pShader; //!< シェーダーへのポインター。
    nn::util::BinTPtr<ResShadingModel> pShadingModel; //!< 親のResShadingModelへのポインター。

    uint32_t attribActiveFlag; //!< 下位ビットから順に 1 属性 1 ビットで有効 / 無効を表すフラグ。
    uint16_t flag; //!< フラグ。
    uint16_t samplerCount; //!< サンプラー数。
    uint16_t imageCount; //!< イメージ数。
    uint16_t uniformBlockCount; //!< ユニフォームブロック数。
    uint16_t shaderStorageBlockCount; //!< シェーダーストレージブロック数。
    uint8_t  reserved2[2];
};

//! @brief シェーダープログラムのリソースです。
class ResShaderProgram : public nn::util::AccessorBase< ResShaderProgramData >
{
    NN_DISALLOW_COPY(ResShaderProgram);

public:
    enum Flag
    {
        Flag_None           = 0,
        Flag_UpdateRequired = 0x1 << 0,
        Flag_Initialized    = 0x1 << 1,
        Flag_Binary         = 0x1 << 2,
        Flag_Source         = 0x1 << 3,
        Flag_Ir             = 0x1 << 4,
    };

    //----------------------------------------
    //! @name 構築/破棄
    //@{

    //! @brief シェーダープログラムをセットアップします。
    //!
    //! @param[in] pDevice デバイスへのポインター。
    //!
    //! @pre
    //! - pDevice が初期化されている。
    //!
    //! @details
    //! この関数は ResShaderArchive::Setup() から間接的によばれます。
    //! シェーダープログラムは内部的に共有されている可能性があるため、直接よばないようにしてください。
    //!
    //! シェーダーの初期化は Update() の呼び出しまで遅延されます。
    //!
    void Setup(nn::gfx::Device* pDevice) NN_NOEXCEPT;

    //! @brief シェーダープログラムをクリーンアップします。
    //!
    //! @param[in] pDevice デバイスへのポインター。
    //!
    //! @pre
    //! - pDevice が初期化されている。
    //!
    //! @details
    //! この関数は ResShaderArchive::Cleanup() から間接的によばれます。
    //! シェーダープログラムは内部的に共有されている可能性があるため、直接よばないようにしてください。
    void Cleanup(nn::gfx::Device* pDevice) NN_NOEXCEPT;

    //! @briefprivate
    void Initialize(nn::gfx::Device* pDevice, bool skipBinaryCheck) NN_NOEXCEPT;

    void UpdateTable() NN_NOEXCEPT;
    //@}

    //----------------------------------------
    //! @name 更新
    //@{

    //! @brief Setup()後、初めての呼び出しで遅延初期化を行います。
    //!
    //! @param[in] pDevice デバイスへのポインター。
    //!
    //! @pre
    //! - pDevice が初期化されている。
    //!
    //! @details
    //! nn::g3d::ResShaderArchive を排他制御有りで初期化した場合、初期化時に排他制御を行います。
    //!
    void Update(nn::gfx::Device* pDevice) NN_NOEXCEPT;

    //@}

    //----------------------------------------
    //! @name 描画
    //@{

    //! @brief シェーダープログラムを設定します。
    //!
    //! @param[in] pCommandBuffer nn::gfx::CommandBuffer へのポインター。
    //!
    //! @pre
    //! - pCommandBuffer が初期化されている。
    //!
    void Load(nn::gfx::CommandBuffer* pCommandBuffer) const NN_NOEXCEPT;

    //@}

    //----------------------------------------
    //! @name 取得
    //@{

    //! @brief インデックス引きでサンプラーのロケーションを取得します。
    //!
    //! @param[in] samplerIndex サンプラーのインデックス。
    //! @param[in] stage ステージ番号。
    //!
    //! @return サンプラーのロケーション番号。
    //!
    //! @pre
    //! - インデックスはサンプラーの数の範囲内。
    //! - ステージ番号はStageの範囲内。
    //!
    //! @details
    //! インデックスは ResShadingModel::GetSamplerIndex() で取得できます。
    //! 初期化時にインデックスを取得しておきキャッシュすることを推奨します。
    //!
    int GetSamplerLocation(int samplerIndex, Stage stage) const NN_NOEXCEPT
    {
        NN_SDK_REQUIRES_RANGE(samplerIndex, 0, samplerCount);
        NN_SDK_REQUIRES_RANGE(stage, Stage_Vertex, Stage_End);
        const int32_t* pSamplerTable = ToData().pSamplerTable.Get();
        return pSamplerTable[samplerIndex * Stage_End + stage];
    }

    //! @brief インデックス引きでイメージのロケーションを取得します。
    //!
    //! @param[in] imageIndex イメージのインデックス。
    //! @param[in] stage ステージ番号。
    //!
    //! @return イメージのロケーション番号。
    //!
    //! @pre
    //! - インデックスはイメージの数の範囲内。
    //! - ステージ番号はStageの範囲内。
    //!
    //! @details
    //! インデックスは ResShadingModel::GetImageIndex() で取得できます。
    //! 初期化時にインデックスを取得しておきキャッシュすることを推奨します。
    //!
    int GetImageLocation(int imageIndex, Stage stage) const NN_NOEXCEPT
    {
        NN_SDK_REQUIRES_RANGE(imageIndex, 0, imageCount);
        NN_SDK_REQUIRES_RANGE(stage, Stage_Vertex, Stage_End);
        const int32_t* pImageTable = ToData().pImageTable.Get();
        return pImageTable[imageIndex * Stage_End + stage];
    }

    //! @brief インデックス引きでユニフォームブロックのロケーションを取得します。
    //!
    //! @param[in] blockIndex ユニフォームブロックのインデックス。
    //! @param[in] stage ステージ番号。
    //!
    //! @return ユニフォームブロックのロケーション番号。
    //!
    //! @pre
    //! - インデックスはユニフォームブロックの数の範囲内。
    //! - ステージ番号はStageの範囲内。
    //!
    //! @details
    //! インデックスは ResShadingModel::GetSystemBlockIndex() か
    //! ResShadingModel::FindUniformBlockIndex() で取得できます。
    //! 初期化時にインデックスを取得しておきキャッシュすることを推奨します。
    //!
    //!
    int GetUniformBlockLocation(int blockIndex, Stage stage) const NN_NOEXCEPT
    {
        NN_SDK_REQUIRES_RANGE(blockIndex, 0, uniformBlockCount);
        NN_SDK_REQUIRES_RANGE(stage, Stage_Vertex, Stage_End);
        const int32_t* pUniformBlockTable = ToData().pUniformBlockTable.Get();
        return pUniformBlockTable[blockIndex * Stage_End + stage];
    }

    //! @brief インデックス引きでシェーダーストレージブロックのロケーションを取得します。
    //!
    //! @param[in] blockIndex シェーダーストレージブロックのインデックス。
    //! @param[in] stage ステージ番号。
    //!
    //! @return シェーダーストレージブロックのロケーション番号。
    //!
    //! @pre
    //! - インデックスはシェーダーストレージブロックの数の範囲内。
    //! - ステージ番号はStageの範囲内。
    //!
    //! @details
    //! インデックスは ResShadingModel::GetSystemShaderStorageBlockIndex() か
    //! ResShadingModel::FindShaderStorageBlockIndex() で取得できます。
    //! 初期化時にインデックスを取得しておきキャッシュすることを推奨します。
    //!
    //!
    int GetShaderStorageBlockLocation(int blockIndex, Stage stage) const NN_NOEXCEPT
    {
        NN_SDK_REQUIRES_RANGE(blockIndex, 0, shaderStorageBlockCount);
        NN_SDK_REQUIRES_RANGE(stage, Stage_Vertex, Stage_End);
        const int32_t* pShaderStorageBlockTable = ToData().pShaderStorageBlockTable.Get();
        return pShaderStorageBlockTable[blockIndex * Stage_End + stage];
    }

    //! @brief インデックス引きで対象の頂点属性がアクティブかどうかを取得します。
    //!
    //! @param[in] attrIndex 頂点属性のインデックス。
    //!
    //! @return 指定した頂点属性がアクティブの場合は true、非アクティブの場合は falseを返します。
    //!
    //! @pre
    //! - 0 <= attribIndex < 32
    //!
    bool IsAttrActive(int attrIndex) const NN_NOEXCEPT
    {
        NN_SDK_REQUIRES_RANGE(attrIndex, 0, static_cast<int>(sizeof(uint32_t) * CHAR_BIT));
        return ( ToData().attribActiveFlag & ( 0x01 << attrIndex ) ) != 0;
    }

    //! @brief シェーダーを取得します。
    //!
    //! @return 取得したシェーダーを返します。
    //!
    const nn::gfx::Shader* GetShader() const NN_NOEXCEPT;

    //! @briefprivate シェーダーバイナリーが使用可能かをチェックします。
    //!
    //! @param[in] pDevice デバイスへのポインター。
    //!
    //! @return 使用可能な場合は true、使用可能でない場合は false を返します。
    //!
    bool IsBinaryAvailable(nn::gfx::Device* pDevice) NN_NOEXCEPT;

    //! @brief シェーダープログラムが初期化されているかどうかを取得します。
    //!
    //! @return 初期化されている場合は true、そうでない場合は false を返します。
    //!
    bool IsInitialized() const NN_NOEXCEPT
    {
        return (ToData().flag & Flag_Initialized) != 0;
    }

    //! @brief シェーディングモデルを取得します。
    //!
    //! @return ResShadingModel のポインターを返します。
    //!
    ResShadingModel* GetShadingModel() NN_NOEXCEPT
    {
        return ToData().pShadingModel.Get();
    }

    //! @brief シェーディングモデルを取得します。
    //!
    //! @return ResShadingModel のポインターを返します。
    //!
    const ResShadingModel* GetShadingModel() const NN_NOEXCEPT
    {
        return ToData().pShadingModel.Get();
    }

    //@}
private:
    bool InitializePerType(nn::gfx::Device* pDevice, nn::gfx::ShaderCodeType type) NN_NOEXCEPT;
    nn::gfx::ShaderStage ConvertToGfxStage(Stage stage) NN_NOEXCEPT
    {
        switch (stage)
        {
            case Stage_Vertex:
                return nn::gfx::ShaderStage_Vertex;
            case Stage_Pixel:
                return nn::gfx::ShaderStage_Pixel;
            case Stage_Geometry:
                return nn::gfx::ShaderStage_Geometry;
            case Stage_Compute:
                return nn::gfx::ShaderStage_Compute;
            case Stage_TessellationControl:
                return nn::gfx::ShaderStage_Hull;
            case Stage_TessellationEvaluation:
                return nn::gfx::ShaderStage_Domain;
            default:
                NN_SDK_ASSERT("unsupported shader stage");
                return nn::gfx::ShaderStage_End;
        }
    }
};

//--------------------------------------------------------------------------------------------------

//! @brief シェーダーオプションの構造体です。
struct ResShaderOptionData
{
    nn::util::BinPtrToString pName; //!< オプション名。
    nn::util::BinTPtr<nn::util::ResDic> pChoiceDic; //!< シェーダーオプションの辞書へのポインター。
    nn::util::BinTPtr<uint32_t> pChoiceValues; //!< choice を int に変換した可変長のバイナリー列へのポインター。

    uint8_t  choiceCount; //!< 選択肢数。
    uint8_t  defaultIndex; //!< デフォルトのインデックス。
    uint16_t branchOffset; //!< ユニフォームブロックにおけるオフセット。
    uint8_t  flag; //!< フラグ。

    uint8_t  keyOffset; //!< static option の場合は 0、dynamic option の場合は staticKeyLength
    uint8_t  bit32Index; //!< 何個目の bit32 か。
    uint8_t  bit32Shift; //!< choiceIndex を何ビットシフトするか。
    Bit32    bit32Mask; //!< 所属する bit32 における該当ビットのマスク。
    uint8_t  reserved[4];
};

//! @brief シェーダーオプションのリソースです。
class ResShaderOption : public nn::util::AccessorBase< ResShaderOptionData >
{
    NN_DISALLOW_COPY(ResShaderOption);

public:
    static const int InvalidOffset = -1;

    enum Flag
    {
        Flag_Branch = 0x1 << 0,
        Flag_Static = 0x1 << 1
    };

    //----------------------------------------
    //! @name 取得
    //@{

    //! @brief デフォルトの Choice のインデックスを取得します。
    //!
    //! @return デフォルトの Choice のインデックスを返します。
    //!
    int GetDefaultIndex() const NN_NOEXCEPT
    {
        return ToData().defaultIndex;
    }

    //! @brief ユニフォームブロックを使用して分岐する際の変数のオフセットを取得します。
    //!
    //! @return ユニフォームブロックを使用して分岐する際の変数のオフセットを返します。
    //!
    ptrdiff_t GetBranchOffset() const NN_NOEXCEPT
    {
        return ToData().branchOffset - 1;
    }

    //! @brief branch かどうかを取得します。
    //!
    //! @return branch の場合は true、それ以外の場合は false を返します。
    //!
    bool IsBranch() const NN_NOEXCEPT
    {
        return !!(flag & Flag_Branch);
    }

    //! @brief static かどうかを取得します。
    //!
    //! @return static の場合は true、それ以外の場合は false を返します。
    //!
    bool IsStatic() const NN_NOEXCEPT
    {
        return !!(flag & Flag_Static);
    }

    //! @brief dynamic かどうかを取得します。
    //!
    //! @return dynamic の場合は true、それ以外の場合は false を返します。
    //!
    bool IsDynamic() const
    {
        return !(flag & Flag_Static);
    }

    //! @brief オプション名を取得します。
    //!
    //! @return オプション名へのポインターを返します。
    //!
    const char* GetName() const NN_NOEXCEPT
    {
        return ToData().pName.Get()->GetData();
    }

    //! @brief Choice の 数を取得します。
    //!
    //! @return Choice の 数を返します。
    //!
    int GetChoiceCount() const NN_NOEXCEPT
    {
        return ToData().choiceCount;
    }

    //! @brief 指定した名前の Choice のインデックスを取得します。
    //!
    //! @param[in] str Choice 名へのポインター。
    //!
    //! @return 指定した名前の Choice のインデックスを返します。
    //!
    //! @details
    //! 指定した名前のオプションが無い場合は nn::util::ResDic::Npos を返します。
    //!
    int FindChoiceIndex(const char* str) const NN_NOEXCEPT
    {
        const nn::util::ResDic* pDic = ToData().pChoiceDic.Get();
        if (pDic == NULL)
        {
            return nn::util::ResDic::Npos;
        }
        return pDic->FindIndex(str);
    }
    //! @brief choice のインデックスから choice の名前を取得します。
    //!
    //! @param[in] elemIndex Choice のインデックス。
    //!
    //! @return Choice の名前を返します。
    //!
    //! @pre
    //! -インデックスは Choice の数の範囲内。
    //!
    //! @details
    //! 指定したインデックスが範囲外の場合は ASSERT します。
    //!
    const char* GetChoiceName(int elemIndex) const NN_NOEXCEPT
    {
        NN_G3D_REQUIRES_RANGE(elemIndex, 0, GetChoiceCount(), GetName());
        const nn::util::ResDic* pDic = ToData().pChoiceDic.Get();
        if (pDic == NULL)
        {
            return NULL;
        }
        nn::util::string_view pString = pDic->GetKey(elemIndex);
        return pString.data();
    }

    //! @brief 指定した Choice でキーを書き換えます。
    //!
    //! @param[out] pStaticKey Static キーへのポインター。
    //! @param[in] choiceIndex 選択した Choice のインデックス
    //!
    void WriteStaticKey(Bit32* pStaticKey, int choiceIndex) const NN_NOEXCEPT;

    //! @brief Static キーから Choice を読み込みます。
    //!
    //! @param[in] pStaticKey キーへのポインター。
    //!
    //! @return Choice のインデックスを返します。
    //!
    int ReadStaticKey(const Bit32* pStaticKey) const NN_NOEXCEPT;

    //! @brief 指定した Choice でキーを書き換えます。
    //!
    //! @param[out] pDynamicKey Dynamic キーへのポインター。
    //! @param[in] choiceIndex 選択した Choice のインデックス
    //!
    void WriteDynamicKey(Bit32* pDynamicKey, int choiceIndex) const NN_NOEXCEPT;

    //! @brief Dynamic キーから Choice を読み込みます。
    //!
    //! @param[in] pDynamicKey キーへのポインター。
    //!
    //! @return Choice のインデックスを返します。
    //!
    int ReadDynamicKey(const Bit32* pDynamicKey) const NN_NOEXCEPT;

    //@}
};

//! @brief 頂点属性変数の構造体です。
struct ResAttrVarData
{
    uint8_t index; //!< シェーディングモデル内でのインデックス。
    int8_t  location; //!< シェーダー内のロケーション。 頂点属性は explicit location 必須。
};

//! @brief 頂点属性変数です。
class ResAttrVar : public nn::util::AccessorBase< ResAttrVarData >
{
    NN_DISALLOW_COPY(ResAttrVar);

public:
    //----------------------------------------
    //! @name 取得
    //@{

    //! @brief シェーディングモデル内でのインデックスを取得します。
    //!
    //! @return シェーディングモデル内でのインデックスを返します。
    //!
    int GetIndex() const NN_NOEXCEPT
    {
        return ToData().index;
    }

    //! @brief シェーダー内のロケーションを取得します。
    //!
    //! @return シェーダー内のロケーションを返します。
    //!
    int GetLocation() const NN_NOEXCEPT
    {
        return ToData().location;
    }

    //@}
};

//--------------------------------------------------------------------------------------------------

//! @brief サンプラー変数の構造体です。
struct ResSamplerVarData
{
    nn::util::BinPtrToString pAlt; //!< アノテーション alt で指定した文字列へのポインター。
    uint8_t index; //!< シェーダー内でのインデックス
    uint8_t reserved[7];
};

//! @brief サンプラー変数です。
class ResSamplerVar : public nn::util::AccessorBase< ResSamplerVarData >
{
    NN_DISALLOW_COPY(ResSamplerVar);

public:
    //----------------------------------------
    //! @name 取得
    //@{

    //! @brief シェーディングモデル内でのインデックスを取得します。
    //!
    //! @return シェーディングモデル内でのインデックスを返します。
    //!
    int GetIndex() const
    {
        return ToData().index;
    }

    //! @brief アノテーション alt で指定した文字列を取得します。
    //!
    //! @return アノテーション alt で指定した文字列を返します。
    //!
    const char* GetAlt() const NN_NOEXCEPT
    {
        return ToData().pAlt.Get()->GetData();
    }

    //@}
};

//--------------------------------------------------------------------------------------------------

//! @brief イメージ変数の構造体です。
struct ResImageVarData
{
    uint8_t index; //!< シェーダー内でのインデックス
};

//! @brief イメージ変数です。
class ResImageVar : public nn::util::AccessorBase< ResImageVarData >
{
    NN_DISALLOW_COPY(ResImageVar);

public:
    //----------------------------------------
    //! @name 取得
    //@{

    //! @brief シェーディングモデル内でのインデックスを取得します。
    //!
    //! @return シェーディングモデル内でのインデックスを返します。
    //!
    int GetIndex() const
    {
        return ToData().index;
    }

    //@}
};

//--------------------------------------------------------------------------------------------------

//! @brief ユニフォーム変数の構造体です。
struct ResUniformVarData
{
    nn::util::BinPtrToString pConverterName; //!< 変換関数名へのポインター。
    int32_t  index; //!< シェーダー内でのインデックス。
    uint16_t offset; //!< 所属するユニフォームブロックの先頭からのバイトオフセット。
    uint8_t  blockIndex; //!< 所属するユニフォームブロックのインデックス。
    uint8_t  reserved[1];
};

//! @brief ユニフォーム変数情報です。
class ResUniformVar : public nn::util::AccessorBase< ResUniformVarData >
{
    NN_DISALLOW_COPY(ResUniformVar);

public:
    //----------------------------------------
    //! @name 取得
    //@{

    //! @brief シェーディングモデル内でのインデックスを取得します。
    //!
    //! @return シェーディングモデル内でのインデックスを返します。
    //!
    int GetIndex() const NN_NOEXCEPT
    {
        return ToData().index;
    }

    //! @brief 所属するユニフォームブロックのインデックスを取得します。
    //!
    //! @return 所属するユニフォームブロックのインデックスを返します。
    //!
    int GetBlockIndex() const NN_NOEXCEPT
    {
        return ToData().blockIndex;
    }

    //! @brief 所属するユニフォームブロックの先頭からのバイトオフセットを取得します。
    //!
    //! @return 所属するユニフォームブロックの先頭からのバイトオフセットを返します。
    //!
    ptrdiff_t GetOffset() const NN_NOEXCEPT
    {
        return ToData().offset - 1;
    }

    //! @brief 変換関数名を取得します。
    //!
    //! @return 変換関数名へのポインターを返します。
    //!
    const char* GetConverter() const NN_NOEXCEPT
    {
        return ToData().pConverterName.Get()->GetData();
    }

    //@}
};

//--------------------------------------------------------------------------------------------------

//! @brief ユニフォームブロック変数情報の構造体です。
struct ResUniformBlockVarData
{
    nn::util::BinTPtr<ResUniformVar>    pUniformArray; //!< ユニフォーム変数情報配列へのポインター。
    nn::util::BinTPtr<nn::util::ResDic> pUniformDic; //!< ユニフォーム変数情報辞書へのポインター。
    nn::util::BinPtr                    pDefault; //!< ユニフォームブロックのデフォルト値。Type_Material の場合のみ値が存在します。
    uint8_t  index; //!< シェーディングモデル内でのインデックス。
    uint8_t  type; //!< ユニフォームブロックのタイプ。
    uint16_t size; //!< ユニフォームブロックのバイト単位のサイズ。
    uint16_t uniformCount; //!< ユニフォーム変数の数。
    uint8_t  reserved[2];
};

//! @brief ユニフォームブロック変数情報です。
class ResUniformBlockVar : public nn::util::AccessorBase< ResUniformBlockVarData >
{
    NN_DISALLOW_COPY(ResUniformBlockVar);

public:
    //! @brief ユニフォームブロックの種類です。
    enum Type
    {
        Type_None, //!< 種類が無指定です。
        Type_Material, //!< マテリアル用のユニフォームブロックです。
        Type_Shape, //!< シェイプ用のユニフォームブロックです。
        Type_Skeleton, //!< スケルトン用のユニフォームブロックです。
        Type_Option, //!< オプション用のユニフォームブロックです。
        Type_Num //!< @briefprivate
    };

    //----------------------------------------
    //! @name 取得
    //@{

    //! @brief シェーディングモデル内でのインデックスを取得します。
    //!
    //! @return シェーディングモデル内でのインデックスを返します。
    //!
    int GetIndex() const NN_NOEXCEPT
    {
        return ToData().index;
    }

    //! @brief ユニフォームブロックのバイト単位のサイズを取得します。
    size_t GetSize() const NN_NOEXCEPT
    {
        return ToData().size;
    }

    //! @brief ユニフォームブロックの種類を取得します。
    Type GetType() const NN_NOEXCEPT
    {
        return static_cast<Type>(ToData().type);
    }

    //! @brief ユニフォームブロックのデフォルト値を取得します。
    const void* GetDefault() const NN_NOEXCEPT
    {
        return ToData().pDefault.Get();
    }

    //! @brief ユニフォーム変数の数を取得します。
    //!
    //! @return ユニフォーム変数の数を返します。
    //!
    int GetUniformCount() const NN_NOEXCEPT
    {
        return ToData().uniformCount;
    }
    //! @brief 指定した名前のユニフォーム変数を取得します。
    //!
    //! @param[in] str ユニフォーム変数名。
    //!
    //! @return ResUniformVar へのポインターを返します。
    //!
    //! @details
    //! 指定した名前のユニフォーム変数が無い場合は NULL を返します。
    //!
    //! 名前引きよりインデックス引きほうが高速に動作します。インデックスを
    //! 取得後、記録しておき、通常はインデックス引きを使用することを推奨します。
    //!
    ResUniformVar* FindUniform(const char* str) NN_NOEXCEPT
    {
        const nn::util::ResDic* pDic = ToData().pUniformDic.Get();
        if (pDic == NULL)
        {
            return NULL;
        }
        int index = pDic->FindIndex(str);
        if (index == nn::util::ResDic::Npos)
        {
            return NULL;
        }
        ResUniformVar* pArray = ToData().pUniformArray.Get();
        NN_SDK_ASSERT_NOT_NULL(pArray);
        return &pArray[index];
    }
    //! @brief 指定した名前のユニフォーム変数を取得します。
    //!
    //! @param[in] str ユニフォーム変数名。
    //!
    //! @return ResUniformVar へのポインターを返します。
    //!
    //! @details
    //! 指定した名前のユニフォーム変数が無い場合は NULL を返します。
    //!
    //! 名前引きよりインデックス引きほうが高速に動作します。インデックスを
    //! 取得後、記録しておき、通常はインデックス引きを使用することを推奨します。
    //!
    const ResUniformVar* FindUniform(const char* str) const NN_NOEXCEPT
    {
        const nn::util::ResDic* pDic = ToData().pUniformDic.Get();
        if (pDic == NULL)
        {
            return NULL;
        }
        int index = pDic->FindIndex(str);
        if (index == nn::util::ResDic::Npos)
        {
            return NULL;
        }
        const ResUniformVar* pArray = ToData().pUniformArray.Get();
        NN_SDK_ASSERT_NOT_NULL(pArray);
        return &pArray[index];
    }
    //! @brief 指定した名前のユニフォーム変数のインデックスを取得します。
    //!
    //! @param[in] str ユニフォーム変数名。
    //!
    //! @return ユニフォーム変数のインデックスを返します。
    //!
    //! @details
    //! 指定した名前のユニフォーム変数が無い場合は nn::util::ResDic::Npos を返します。
    //!
    int FindUniformIndex(const char* str) const NN_NOEXCEPT
    {
        const nn::util::ResDic* pDic = ToData().pUniformDic.Get();
        if (pDic == NULL)
        {
            return NULL;
        }
        return pDic->FindIndex(str);
    }
    //! @brief ユニフォーム変数のインデックスから名前を取得します。
    //!
    //! @param[in] elemIndex ユニフォーム変数のインデックス。
    //!
    //! @return ユニフォーム変数名へのポインターを返します。
    //!
    //! @pre
    //! - インデックスはユニフォーム変数の数の範囲内。
    //!
    //! @details
    //! 指定したインデックスが範囲外の場合は ASSERT します。
    //!
    const char* GetUniformName(int elemIndex) const NN_NOEXCEPT
    {
        NN_SDK_REQUIRES_RANGE(elemIndex, 0, GetUniformCount());
        const nn::util::ResDic* pDic = ToData().pUniformDic.Get();
        if (pDic == NULL)
        {
            return NULL;
        }
        nn::util::string_view pString = pDic->GetKey(elemIndex);
        return pString.data();
    }
    //! @brief インデックス引きでユニフォーム変数を取得します。
    //!
    //! @param[in] elemIndex ユニフォーム変数のインデックス。
    //!
    //! @return ResUniformVarへのポインターを返します。
    //!
    //! @pre
    //! - インデックスはユニフォーム変数の数の範囲内。
    //!
    //! @details
    //! 指定したインデックスが範囲外の場合は ASSERT します。
    //!
    ResUniformVar* GetUniform(int elemIndex) NN_NOEXCEPT
    {
        NN_SDK_REQUIRES_RANGE(elemIndex, 0, GetUniformCount());
        ResUniformVar* pArray = ToData().pUniformArray.Get();
        NN_SDK_ASSERT_NOT_NULL(pArray);
        return &pArray[elemIndex];
    }
    //! @brief インデックス引きでユニフォーム変数を取得します。
    //!
    //! @param[in] elemIndex ユニフォーム変数のインデックス。
    //!
    //! @return ResUniformVarへのポインターを返します。
    //!
    //! @pre
    //! - インデックスはユニフォーム変数の数の範囲内。
    //!
    //! @details
    //! 指定したインデックスが範囲外の場合は ASSERT します。
    //!
    const ResUniformVar* GetUniform(int elemIndex) const NN_NOEXCEPT
    {
        NN_SDK_REQUIRES_RANGE(elemIndex, 0, GetUniformCount());
        const ResUniformVar* pArray = ToData().pUniformArray.Get();
        NN_SDK_ASSERT_NOT_NULL(pArray);
        return &pArray[elemIndex];
    }

    //@}
};

//--------------------------------------------------------------------------------------------------

//! @brief シェーダーストレージ変数の構造体です。
typedef ResUniformVarData ResShaderStorageVarData;

//! @brief シェーダーストレージ変数情報です。
typedef ResUniformVar ResShaderStorageVar;

//--------------------------------------------------------------------------------------------------

//! @brief シェーダーストレージブロック変数情報の構造体です。
struct ResShaderStorageBlockVarData
{
    nn::util::BinTPtr<ResShaderStorageVar>    pShaderStorageArray; //!< シェーダーストレージ変数情報配列へのポインター。
    nn::util::BinTPtr<nn::util::ResDic> pShaderStorageDic; //!< シェーダーストレージ変数情報辞書へのポインター。
    nn::util::BinPtr                    pDefault; //!< シェーダーストレージブロックのデフォルト値。Type_Material の場合のみ値が存在します。
    uint8_t  index; //!< シェーディングモデル内でのインデックス。
    uint8_t  type; //!< シェーダーストレージブロックのタイプ。
    uint16_t size; //!< シェーダーストレージブロックのバイト単位のサイズ。
    uint16_t shaderStorageCount; //!< シェーダーストレージ変数の数。
    uint8_t  reserved[2];
};

//! @brief シェーダーストレージブロック変数情報です。
class ResShaderStorageBlockVar : public nn::util::AccessorBase< ResShaderStorageBlockVarData >
{
    NN_DISALLOW_COPY(ResShaderStorageBlockVar);

public:
    //! @brief シェーダーストレージブロックの種類です。
    enum Type
    {
        Type_None, //!< 種類が無指定です。
        Type_Material, // マテリアル用のシェーダーストレージブロックです。
        Type_Shape, // シェイプ用のシェーダーストレージブロックです。
        Type_Skeleton, //!< スケルトン用のシェーダーストレージブロックです。
        Type_Option, // オプション用のシェーダーストレージブロックです。
        Type_Num //!< @briefprivate
    };

    //----------------------------------------
    //! @name 取得
    //@{

    //! @brief シェーディングモデル内でのインデックスを取得します。
    //!
    //! @return シェーディングモデル内でのインデックスを返します。
    //!
    int GetIndex() const NN_NOEXCEPT
    {
        return ToData().index;
    }

    //! @brief シェーダーストレージブロックのバイト単位のサイズを取得します。
    size_t GetSize() const NN_NOEXCEPT
    {
        return ToData().size;
    }

    //! @brief シェーダーストレージブロックの種類を取得します。
    Type GetType() const NN_NOEXCEPT
    {
        return static_cast<Type>(ToData().type);
    }

    //! @brief シェーダーストレージブロックのデフォルト値を取得します。
    const void* GetDefault() const NN_NOEXCEPT
    {
        return ToData().pDefault.Get();
    }

    //! @brief シェーダーストレージ変数の数を取得します。
    //!
    //! @return シェーダーストレージ変数の数を返します。
    //!
    int GetShaderStorageCount() const NN_NOEXCEPT
    {
        return ToData().shaderStorageCount;
    }
    //! @brief 指定した名前のシェーダーストレージ変数を取得します。
    //!
    //! @param[in] str シェーダーストレージ変数名。
    //!
    //! @return ResShaderStorageVar へのポインターを返します。
    //!
    //! @details
    //! 指定した名前のシェーダーストレージ変数が無い場合は NULL を返します。
    //!
    //! 名前引きよりインデックス引きほうが高速に動作します。インデックスを
    //! 取得後、記録しておき、通常はインデックス引きを使用することを推奨します。
    //!
    ResShaderStorageVar* FindShaderStorage(const char* str) NN_NOEXCEPT
    {
        const nn::util::ResDic* pDic = ToData().pShaderStorageDic.Get();
        if (pDic == NULL)
        {
            return NULL;
        }
        int index = pDic->FindIndex(str);
        if (index == nn::util::ResDic::Npos)
        {
            return NULL;
        }
        ResShaderStorageVar* pArray = ToData().pShaderStorageArray.Get();
        NN_SDK_ASSERT_NOT_NULL(pArray);
        return &pArray[index];
    }
    //! @brief 指定した名前のシェーダーストレージ変数を取得します。
    //!
    //! @param[in] str シェーダーストレージ変数名。
    //!
    //! @return ResShaderStorageVar へのポインターを返します。
    //!
    //! @details
    //! 指定した名前のシェーダーストレージ変数が無い場合は NULL を返します。
    //!
    //! 名前引きよりインデックス引きほうが高速に動作します。インデックスを
    //! 取得後、記録しておき、通常はインデックス引きを使用することを推奨します。
    //!
    const ResShaderStorageVar* FindShaderStorage(const char* str) const NN_NOEXCEPT
    {
        const nn::util::ResDic* pDic = ToData().pShaderStorageDic.Get();
        if (pDic == NULL)
        {
            return NULL;
        }
        int index = pDic->FindIndex(str);
        if (index == nn::util::ResDic::Npos)
        {
            return NULL;
        }
        const ResShaderStorageVar* pArray = ToData().pShaderStorageArray.Get();
        NN_SDK_ASSERT_NOT_NULL(pArray);
        return &pArray[index];
    }
    //! @brief 指定した名前のシェーダーストレージ変数のインデックスを取得します。
    //!
    //! @param[in] str シェーダーストレージ変数名。
    //!
    //! @return シェーダーストレージ変数のインデックスを返します。
    //!
    //! @details
    //! 指定した名前のシェーダーストレージ変数が無い場合は nn::util::ResDic::Npos を返します。
    //!
    int FindShaderStorageIndex(const char* str) const NN_NOEXCEPT
    {
        const nn::util::ResDic* pDic = ToData().pShaderStorageDic.Get();
        if (pDic == NULL)
        {
            return NULL;
        }
        return pDic->FindIndex(str);
    }
    //! @brief シェーダーストレージ変数のインデックスから名前を取得します。
    //!
    //! @param[in] elemIndex シェーダーストレージ変数のインデックス。
    //!
    //! @return シェーダーストレージ変数名へのポインターを返します。
    //!
    //! @pre
    //! - インデックスはシェーダーストレージ変数の数の範囲内。
    //!
    //! @details
    //! 指定したインデックスが範囲外の場合は ASSERT します。
    //!
    const char* GetShaderStorageName(int elemIndex) const NN_NOEXCEPT
    {
        NN_SDK_REQUIRES_RANGE(elemIndex, 0, GetShaderStorageCount());
        const nn::util::ResDic* pDic = ToData().pShaderStorageDic.Get();
        if (pDic == NULL)
        {
            return NULL;
        }
        nn::util::string_view pString = pDic->GetKey(elemIndex);
        return pString.data();
    }
    //! @brief インデックス引きでシェーダーストレージ変数を取得します。
    //!
    //! @param[in] elemIndex シェーダーストレージ変数のインデックス。
    //!
    //! @return ResShaderStorageVarへのポインターを返します。
    //!
    //! @pre
    //! - インデックスはシェーダーストレージ変数の数の範囲内。
    //!
    //! @details
    //! 指定したインデックスが範囲外の場合は ASSERT します。
    //!
    ResShaderStorageVar* GetShaderStorage(int elemIndex) NN_NOEXCEPT
    {
        NN_SDK_REQUIRES_RANGE(elemIndex, 0, GetShaderStorageCount());
        ResShaderStorageVar* pArray = ToData().pShaderStorageArray.Get();
        NN_SDK_ASSERT_NOT_NULL(pArray);
        return &pArray[elemIndex];
    }
    //! @brief インデックス引きでシェーダーストレージ変数を取得します。
    //!
    //! @param[in] elemIndex シェーダーストレージ変数のインデックス。
    //!
    //! @return ResShaderStorageVarへのポインターを返します。
    //!
    //! @pre
    //! - インデックスはシェーダーストレージ変数の数の範囲内。
    //!
    //! @details
    //! 指定したインデックスが範囲外の場合は ASSERT します。
    //!
    const ResShaderStorageVar* GetShaderStorage(int elemIndex) const NN_NOEXCEPT
    {
        NN_SDK_REQUIRES_RANGE(elemIndex, 0, GetShaderStorageCount());
        const ResShaderStorageVar* pArray = ToData().pShaderStorageArray.Get();
        NN_SDK_ASSERT_NOT_NULL(pArray);
        return &pArray[elemIndex];
    }

    //@}
};

//--------------------------------------------------------------------------------------------------

//! @brief シェーダーのシンボル情報です。
struct ResShaderInfo
{
    nn::util::BinTPtr<nn::util::BinPtrToString> pSamplerTable; //!< サンプラーのシンボル名のテーブル。
    nn::util::BinTPtr<nn::util::BinPtrToString> pImageTable; //!< イメージのシンボル名のテーブル。
    nn::util::BinTPtr<nn::util::BinPtrToString> pUniformBlockTable; //!< ユニフォームブロックのシンボル名のテーブル。
    nn::util::BinTPtr<nn::util::BinPtrToString> pShaderStorageBlockTable; //!< シェーダーストレージブロックのシンボル名のテーブル。
};

//! @brief シェーディングモデルの構造体です。
struct ResShadingModelData
{
    nn::util::BinPtrToString pName; //!< シェーディングモデル名。
    nn::util::BinTPtr<ResShaderOption>          pStaticOptionArray; //!< Static オプション配列へのポインター。
    nn::util::BinTPtr<nn::util::ResDic>         pStaticOptionDic; //!< Static オプション辞書へのポインター。
    nn::util::BinTPtr<ResShaderOption>          pDynamicOptionArray; //!< Dynamic オプション配列へのポインター。
    nn::util::BinTPtr<nn::util::ResDic>         pDynamicOptionDic; //!< Dynamic オプション辞書へのポインター。
    nn::util::BinTPtr<ResAttrVar>               pAttrArray; //!< 頂点属性情報配列へのポインター。
    nn::util::BinTPtr<nn::util::ResDic>         pAttrDic; //!< 頂点属性情報辞書へのポインター。
    nn::util::BinTPtr<ResSamplerVar>            pSamplerArray; //!< サンプラー情報配列へのポインター。
    nn::util::BinTPtr<nn::util::ResDic>         pSamplerDic; //!< サンプラー情報辞書へのポインター。
    nn::util::BinTPtr<ResImageVar>              pImageArray; //!< イメージ情報配列へのポインター。
    nn::util::BinTPtr<nn::util::ResDic>         pImageDic; //!< イメージ情報辞書へのポインター。
    nn::util::BinTPtr<ResUniformBlockVar>       pUniformBlockArray; //!< ユニフォームブロック情報配列へのポインター。
    nn::util::BinTPtr<nn::util::ResDic>         pUniformBlockDic; //!< ユニフォームブロック情報辞書へのポインター。
    nn::util::BinTPtr<ResUniformVar>            pUniformArray; //!< ユニフォーム変数情報配列へのポインター。
    nn::util::BinTPtr<ResShaderStorageBlockVar> pShaderStorageBlockArray; //!< シェーダーストレージブロック情報配列へのポインター。
    nn::util::BinTPtr<nn::util::ResDic>         pShaderStorageBlockDic; //!< シェーダーストレージブロック情報辞書へのポインター。
    nn::util::BinTPtr<ResShaderStorageVar>      pShaderStorageArray; //!< シェーダーストレージ変数情報配列へのポインター。
    nn::util::BinTPtr<ResShaderProgram>         pShaderProgramArray; //!< シェーダープログラム配列へのポインター。
    nn::util::BinTPtr<Bit32>                    pKeyTable; //!< キーテーブルへのポインター。
    nn::util::BinTPtr<ResShaderArchive>         pShaderArchive; //!< シェーダーアーカイブへのポインター。親への参照。
    nn::util::BinTPtr<ResShaderInfo>            pShaderInfo; //!< @briefprivate シェーダーのシンボル情報。
    nn::util::BinTPtr<nn::gfx::ResShaderFile>   pShaderFile; //!< シェーダーファイルリソースへのポインター。
    nn::util::BinTPtr<nn::os::MutexType>        pMutexTypePtr; //!< 排他制御処理用のミューテックスへのポインター。
    uint8_t                                     reserved1[16];

    nn::util::BinPtr pUserPtr; //!< ユーザーポインター
    nn::util::BinPtr pCallbackParam; //!< @briefprivate edit ライブラリー用

    int32_t  uniformCount; //!< ユニフォーム変数の数。
    int32_t  shaderStorageCount; //!< シェーダーストレージ変数の数。
    int32_t  defaultProgramIndex; //!< デフォルトのシェーダープログラムのインデックス。
    uint16_t staticOptionCount; //!< Static オプションの数。
    uint16_t dynamicOptionCount; //!< Dynamic オプションの数。
    uint16_t shaderProgramCount; //!< シェーダープログラムの数。
    uint8_t  staticKeyLength; //!< Static キー長。
    uint8_t  dynamicKeyLength; //!< Dynamic キー長。
    uint8_t  attrCount; //!< 頂点属性情報の数。
    uint8_t  samplerCount; //!< サンプラー情報の数。
    uint8_t  imageCount; //!< イメージ情報の数。
    uint8_t  uniformBlockCount; //!< ユニフォームブロック情報の数。
    int8_t   systemBlockIndices[ResUniformBlockVar::Type_Num - 1]; //!< システム既定のユニフォームブロックのインデックス
    uint8_t  shaderStorageBlockCount; //!< シェーダーストレージブロック情報の個数。
    int8_t   systemShaderStorageBlockIndices[ResShaderStorageBlockVar::Type_Num - 1]; //!< システム既定のシェーダーストレージブロックのインデックス
    uint8_t  reserved3[7];
};

//! @brief シェーディングモデルのリソースです。
class ResShadingModel : public nn::util::AccessorBase< ResShadingModelData >
{
    NN_DISALLOW_COPY(ResShadingModel);

public:
    //----------------------------------------
    //! @name 構築/破棄
    //@{

    //! @brief シェーディングモデルをセットアップします。
    //!
    //! @param[in] pDevice デバイスへのポインター。
    //!
    //! @pre
    //! - pDevice が初期化されている。
    //!
    void Setup(nn::gfx::Device* pDevice) NN_NOEXCEPT
    {
        Setup(pDevice, NULL);
    }

    //! @brief シェーディングモデルをセットアップします。
    //!
    //! @param[in] pDevice デバイスへのポインター。
    //! @param[in] pMutexType  排他制御に使用される nn::os::MutexType へのポインター。
    //!
    //! @pre
    //! - pDevice が初期化されている。
    //! - pMutexType が初期化されている。
    //!
    //! @details
    //! この関数を使用して初期化を行った場合、シェーダーの初期化が排他制御されるようになります。
    //!
    void Setup(nn::gfx::Device* pDevice, nn::os::MutexType* pMutexType) NN_NOEXCEPT;

    //! @briefprivate シェーディングモデルをセットアップします。
    //!
    //! @param[in] pDevice デバイスへのポインター。
    //! @param[in] pMemoryPool ResShadingModel が配置されているメモリプールへのポインター。
    //! @param[in] memoryPoolOffset ResShadingModel が配置されているメモリプール上のオフセット。
    //! @param[in] memoryPoolSize ResShadingModel が配置されているメモリプールのサイズ。
    //!
    //! @pre
    //! - pDevice が初期化されている。
    //! - pMemoryPool に ResShadingModel がロードされている。
    //!
    //! @details
    //! ユーザーが自身で用意したメモリプールに ResShadingModel をロードし、そのメモリプールを利用して ResShadingModel をセットアップする場合にこの関数を使用します。
    //!
    void Setup(nn::gfx::Device* pDevice, nn::gfx::MemoryPool* pMemoryPool, ptrdiff_t memoryPoolOffset, size_t memoryPoolSize) NN_NOEXCEPT
    {
        Setup(pDevice, pMemoryPool, memoryPoolOffset, memoryPoolSize, NULL);
    }

    //! @briefprivate シェーディングモデルをセットアップします。
    //!
    //! @param[in] pDevice デバイスへのポインター。
    //! @param[in] pMemoryPool ResShadingModel が配置されているメモリプールへのポインター。
    //! @param[in] memoryPoolOffset ResShadingModel が配置されているメモリプール上のオフセット。
    //! @param[in] memoryPoolSize ResShadingModel が配置されているメモリプールのサイズ。
    //! @param[in] pMutexType  排他制御に使用される nn::os::MutexType へのポインター。
    //!
    //! @pre
    //! - pDevice が初期化されている。
    //! - pMemoryPool に ResShadingModel がロードされている。
    //! - pMutexType が初期化されている。
    //!
    //! @details
    //! ユーザーが自身で用意したメモリプールに ResShadingModel をロードし、そのメモリプールを利用して ResShadingModel をセットアップする場合にこの関数を使用します。
    //!
    //! この関数を使用して初期化を行った場合、シェーダーの初期化が排他制御されるようになります。
    //!
    void Setup(nn::gfx::Device* pDevice, nn::gfx::MemoryPool* pMemoryPool, ptrdiff_t memoryPoolOffset, size_t memoryPoolSize, nn::os::MutexType* pMutexType) NN_NOEXCEPT;

    //! @brief シェーディングモデルをクリーンアップします。
    //!
    //! @param[in] pDevice デバイスへのポインター。
    //!
    //! @pre
    //! - pDevice が初期化されている。
    //!
    void Cleanup(nn::gfx::Device* pDevice) NN_NOEXCEPT;

    //! @briefprivate ResShadingModel を使用可能な状態にします。
    void Relocate() NN_NOEXCEPT;

    //! @briefprivate ResShadingModel を再配置可能な状態にします。
    void Unrelocate() NN_NOEXCEPT;

    //@}

    //----------------------------------------
    //! @name 取得/設定
    //@{

    //! @brief シェーディングモデル名を取得します。
    //!
    //! @return シェーディングモデル名へのポインターを返します。
    //!
    const char* GetName() const NN_NOEXCEPT
    {
        return ToData().pName.Get()->GetData();
    }

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
    //! @tparam T ポインターを受けとる際の型。
    //!
    //! @return SetUserPtr() で設定したポインターを返します。
    //!
    template <typename T>
    T* GetUserPtr() NN_NOEXCEPT
    {
        return static_cast<T*>(ToData().pUserPtr.Get());
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
        return static_cast<const T*>(ToData().pUserPtr.Get());
    }

    //! @briefprivate
    const ResShaderInfo* GetShaderInfo() const NN_NOEXCEPT
    {
        return ToData().pShaderInfo.Get();
    }

    //! @brief このシェーディングモデルのシェーダーバリエーションを持つ nn::gfx::ResShaderFile のポインターを取得します。
    //!
    //! @return nn::gfx::ResShaderFile のポインターを返します。
    //!
    const nn::gfx::ResShaderFile* GetGfxResShaderFile() const NN_NOEXCEPT
    {
        return ToData().pShaderFile.Get();
    }

    //@}

    //----------------------------------------
    //! @name オプション
    //@{

    //! @brief Static オプション情報の数を取得します。
    //!
    //! @return Static オプション情報の数を返します。
    //!
    int GetStaticOptionCount() const NN_NOEXCEPT
    {
        return ToData().staticOptionCount;
    }

    //! @brief 指定した名前の Static オプション情報を取得します。
    //!
    //! @param[in] str Static オプション情報名。
    //!
    //! @return ResShaderOption へのポインターを返します。
    //!
    //! @details
    //! 指定した名前の Static オプション情報が無い場合は NULL を返します。
    //!
    //! 名前引きよりインデックス引きほうが高速に動作します。インデックスを
    //! 取得後、記録しておき、通常はインデックス引きを使用することを推奨します。
    //!
    ResShaderOption* FindStaticOption(const char* str) NN_NOEXCEPT
    {
        const nn::util::ResDic* pDic = ToData().pStaticOptionDic.Get();
        if (pDic == NULL)
        {
            return NULL;
        }
        int index = pDic->FindIndex(str);
        if (index == nn::util::ResDic::Npos)
        {
            return NULL;
        }
        ResShaderOption* pArray = ToData().pStaticOptionArray.Get();
        NN_SDK_ASSERT_NOT_NULL(pArray);
        return &pArray[index];
    }
    //! @brief 指定した名前の Static オプション情報を取得します。
    //!
    //! @param[in] str Static オプション情報名。
    //!
    //! @return ResShaderOption へのポインターを返します。
    //!
    //! @details
    //! 指定した名前の Static オプション情報が無い場合は NULL を返します。
    //!
    //! 名前引きよりインデックス引きほうが高速に動作します。インデックスを
    //! 取得後、記録しておき、通常はインデックス引きを使用することを推奨します。
    //!
    const ResShaderOption* FindStaticOption(const char* str) const NN_NOEXCEPT
    {
        const nn::util::ResDic* pDic = ToData().pStaticOptionDic.Get();
        if (pDic == NULL)
        {
            return NULL;
        }
        int index = pDic->FindIndex(str);
        if (index == nn::util::ResDic::Npos)
        {
            return NULL;
        }
        const ResShaderOption* pArray = ToData().pStaticOptionArray.Get();
        NN_SDK_ASSERT_NOT_NULL(pArray);
        return &pArray[index];
    }

    //! @brief 指定した名前の Static オプション情報のインデックスを取得します。
    //!
    //! @param[in] str Static オプション情報名。
    //!
    //! @return Static オプション情報のインデックスを返します。
    //!
    //! @details
    //! 指定した名前の Static オプション情報が無い場合は nn::util::ResDic::Npos を返します。
    //!
    int FindStaticOptionIndex(const char* str) const NN_NOEXCEPT
    {
        const nn::util::ResDic* pDic = ToData().pStaticOptionDic.Get();
        if (pDic == NULL)
        {
            return nn::util::ResDic::Npos;
        }
        return pDic->FindIndex(str);
    }
    //! @brief Static オプション情報のインデックスから名前を取得します。
    //!
    //! @param[in] elemIndex Static オプション情報のインデックス。
    //!
    //! @return Static オプション情報名へのポインターを返します。
    //!
    //! @pre
    //! - インデックスは Static オプション情報の数の範囲内。
    //!
    //! @details
    //! 指定したインデックスが範囲外の場合は ASSERT します。
    //!
    const char* GetStaticOptionName(int elemIndex) const NN_NOEXCEPT
    {
        NN_G3D_REQUIRES_RANGE(elemIndex, 0, GetStaticOptionCount(), GetName());
        const nn::util::ResDic* pDic = ToData().pStaticOptionDic.Get();
        if (pDic == NULL)
        {
            return NULL;
        }
        nn::util::string_view pString = pDic->GetKey(elemIndex);
        return pString.data();
    }

    //! @brief インデックス引きで Static オプション情報を取得します。
    //!
    //! @param[in] elemIndex Static オプション情報のインデックス。
    //!
    //! @return ResShaderOption へのポインターを返します。
    //!
    //! @pre
    //! - インデックスは Static オプション情報の数の範囲内。
    //!
    //! @details
    //! 指定したインデックスが範囲外の場合は ASSERT します。
    //!
    ResShaderOption* GetStaticOption(int elemIndex) NN_NOEXCEPT
    {
        NN_G3D_REQUIRES_RANGE(elemIndex, 0, GetStaticOptionCount(), GetName());
        ResShaderOption* pArray = ToData().pStaticOptionArray.Get();
        NN_SDK_ASSERT_NOT_NULL(pArray);
        return &pArray[elemIndex];
    }
    //! @brief インデックス引きで Static オプション情報を取得します。
    //!
    //! @param[in] elemIndex Static オプション情報のインデックス。
    //!
    //! @return ResShaderOption へのポインターを返します。
    //!
    //! @pre
    //! - インデックスは Static オプション情報の数の範囲内。
    //!
    //! @details
    //! 指定したインデックスが範囲外の場合は ASSERT します。
    //!
    const ResShaderOption* GetStaticOption(int elemIndex) const NN_NOEXCEPT
    {
        NN_G3D_REQUIRES_RANGE(elemIndex, 0, GetStaticOptionCount(), GetName());
        const ResShaderOption* pArray = ToData().pStaticOptionArray.Get();
        NN_SDK_ASSERT_NOT_NULL(pArray);
        return &pArray[elemIndex];
    }

    //! @brief Dynamic オプション情報の数を取得します。
    //!
    //! @return Dynamic オプション情報の数を返します。
    //!
    //! @details
    //! システムがDynamic オプションを１つ追加するため、ユーザーが定義した数 + 1の値を返します。
    //! システムが追加する Dynamic オプションの名前は system_id です。
    //!
    int GetDynamicOptionCount() const NN_NOEXCEPT
    {
        return ToData().dynamicOptionCount;
    }

    //! @brief 指定した名前の Dynamic オプション情報を取得します。
    //!
    //! @param[in] str Dynamic オプション情報名。
    //!
    //! @return ResShaderOption へのポインターを返します。
    //!
    //! @details
    //! 指定した名前の Dynamic オプション情報が無い場合は NULL を返します。
    //!
    //! 名前引きよりインデックス引きほうが高速に動作します。インデックスを
    //! 取得後、記録しておき、通常はインデックス引きを使用することを推奨します。
    //!
    //! system_id という名前の Dynamic オプションはシステムが追加したオプションです。
    //!
    ResShaderOption* FindDynamicOption(const char* str) NN_NOEXCEPT
    {
        const nn::util::ResDic* pDic = ToData().pDynamicOptionDic.Get();
        if (pDic == NULL)
        {
            return NULL;
        }
        int index = pDic->FindIndex(str);
        if (index == nn::util::ResDic::Npos)
        {
            return NULL;
        }
        ResShaderOption* pArray = ToData().pDynamicOptionArray.Get();
        NN_SDK_ASSERT_NOT_NULL(pArray);
        return &pArray[index];
    }
    //! @brief 指定した名前の Dynamic オプション情報を取得します。
    //!
    //! @param[in] str Dynamic オプション情報名。
    //!
    //! @return ResShaderOption へのポインターを返します。
    //!
    //! @details
    //! 指定した名前の Dynamic オプション情報が無い場合は NULL を返します。
    //!
    //! 名前引きよりインデックス引きほうが高速に動作します。インデックスを
    //! 取得後、記録しておき、通常はインデックス引きを使用することを推奨します。
    //!
    //! system_id という名前の Dynamic オプションはシステムが追加したオプションです。
    //!
    const ResShaderOption* FindDynamicOption(const char* str) const NN_NOEXCEPT
    {
        const nn::util::ResDic* pDic = ToData().pDynamicOptionDic.Get();
        if (pDic == NULL)
        {
            return NULL;
        }
        int index = pDic->FindIndex(str);
        if (index == nn::util::ResDic::Npos)
        {
            return NULL;
        }
        const ResShaderOption* pArray = ToData().pDynamicOptionArray.Get();
        NN_SDK_ASSERT_NOT_NULL(pArray);
        return &pArray[index];
    }

    //! @brief 指定した名前の Dynamic オプション情報のインデックスを取得します。
    //!
    //! @param[in] str Dynamic オプション情報名。
    //!
    //! @return Dynamic オプション情報のインデックスを返します。
    //!
    //! @details
    //! 指定した名前の Dynamic オプション情報が無い場合は nn::util::ResDic::Npos を返します。
    //!
    //! system_id という名前の Dynamic オプションはシステムが追加したオプションです。
    //!
    int FindDynamicOptionIndex(const char* str) const NN_NOEXCEPT
    {
        const nn::util::ResDic* pDic = ToData().pDynamicOptionDic.Get();
        if (pDic == NULL)
        {
            return nn::util::ResDic::Npos;
        }
        return pDic->FindIndex(str);
    }
    //! @brief Dynamic オプション情報のインデックスから名前を取得します。
    //!
    //! @param[in] elemIndex Dynamic オプション情報のインデックス。
    //!
    //! @return Dynamic オプション情報名へのポインターを返します。
    //!
    //! @pre
    //! - インデックスは Dynamic オプション情報の数の範囲内。
    //!
    //! @details
    //! 指定したインデックスが範囲外の場合は ASSERT します。
    //!
    //! system_id という名前の Dynamic オプションはシステムが追加したオプションです。
    //!
    const char* GetDynamicOptionName(int elemIndex) const NN_NOEXCEPT
    {
        NN_G3D_REQUIRES_RANGE(elemIndex, 0, GetDynamicOptionCount(), GetName());
        const nn::util::ResDic* pDic = ToData().pDynamicOptionDic.Get();
        if (pDic == NULL)
        {
            return NULL;
        }
        nn::util::string_view pString = pDic->GetKey(elemIndex);
        return pString.data();
    }

    //! @brief インデックス引きで Dynamic オプション情報を取得します。
    //!
    //! @param[in] elemIndex Dynamic オプション情報のインデックス。
    //!
    //! @return ResShaderOption へのポインターを返します。
    //!
    //! @pre
    //! - インデックスは Dynamic オプション情報の数の範囲内。
    //!
    //! @details
    //! 指定したインデックスが範囲外の場合は ASSERT します。
    //!
    ResShaderOption* GetDynamicOption(int elemIndex) NN_NOEXCEPT
    {
        NN_G3D_REQUIRES_RANGE(elemIndex, 0, GetDynamicOptionCount(), GetName());
        ResShaderOption* pArray = ToData().pDynamicOptionArray.Get();
        NN_SDK_ASSERT_NOT_NULL(pArray);
        return &pArray[elemIndex];
    }
    //! @brief インデックス引きで Dynamic オプション情報を取得します。
    //!
    //! @param[in] elemIndex Dynamic オプション情報のインデックス。
    //!
    //! @return ResShaderOption へのポインターを返します。
    //!
    //! @pre
    //! - インデックスは Dynamic オプション情報の数の範囲内。
    //!
    //! @details
    //! 指定したインデックスが範囲外の場合は ASSERT します。
    //!
    const ResShaderOption* GetDynamicOption(int elemIndex) const NN_NOEXCEPT
    {
        NN_G3D_REQUIRES_RANGE(elemIndex, 0, GetDynamicOptionCount(), GetName());
        const ResShaderOption* pArray = ToData().pDynamicOptionArray.Get();
        NN_SDK_ASSERT_NOT_NULL(pArray);
        return &pArray[elemIndex];
    }

    //@}

    //----------------------------------------
    //! @name 頂点属性
    //@{

    //! @brief 頂点属性情報の数を取得します。
    //!
    //! @return 頂点属性情報の数を返します。
    //!
    int GetAttrCount() const NN_NOEXCEPT
    {
        return ToData().attrCount;
    }

    //! @brief 指定した名前の頂点属性情報を取得します。
    //!
    //! @param[in] str 頂点属性情報名。
    //!
    //! @return ResAttrVar へのポインターを返します。
    //!
    //! @details
    //! 指定した名前の頂点属性情報が無い場合は NULL を返します。
    //!
    //! 名前引きよりインデックス引きほうが高速に動作します。インデックスを
    //! 取得後、記録しておき、通常はインデックス引きを使用することを推奨します。
    //!
    ResAttrVar* FindAttr(const char* str) NN_NOEXCEPT
    {
        const nn::util::ResDic* pDic = ToData().pAttrDic.Get();
        if (pDic == NULL)
        {
            return NULL;
        }
        int index = pDic->FindIndex(str);
        if (index == nn::util::ResDic::Npos)
        {
            return NULL;
        }
        ResAttrVar* pArray = ToData().pAttrArray.Get();
        NN_SDK_ASSERT_NOT_NULL(pArray);
        return &pArray[index];
    }
    //! @brief 指定した名前の頂点属性情報を取得します。
    //!
    //! @param[in] str 頂点属性情報名。
    //!
    //! @return ResAttrVar へのポインターを返します。
    //!
    //! @details
    //! 指定した名前の頂点属性情報が無い場合は NULL を返します。
    //!
    //! 名前引きよりインデックス引きほうが高速に動作します。インデックスを
    //! 取得後、記録しておき、通常はインデックス引きを使用することを推奨します。
    //!
    const ResAttrVar* FindAttr(const char* str) const NN_NOEXCEPT
    {
        const nn::util::ResDic* pDic = ToData().pAttrDic.Get();
        if (pDic == NULL)
        {
            return NULL;
        }
        int index = pDic->FindIndex(str);
        if (index == nn::util::ResDic::Npos)
        {
            return NULL;
        }
        const ResAttrVar* pArray = ToData().pAttrArray.Get();
        NN_SDK_ASSERT_NOT_NULL(pArray);
        return &pArray[index];
    }

    //! @brief 指定した名前の頂点属性情報のインデックスを取得します。
    //!
    //! @param[in] str 頂点属性情報名。
    //!
    //! @return 頂点属性情報のインデックスを返します。
    //!
    //! @details
    //! 指定した名前の頂点属性情報が無い場合は nn::util::ResDic::Npos を返します。
    //!
    int FindAttrIndex(const char* str) const NN_NOEXCEPT
    {
        const nn::util::ResDic* pDic = ToData().pAttrDic.Get();
        if (pDic == NULL)
        {
            return nn::util::ResDic::Npos;
        }
        return pDic->FindIndex(str);
    }
    //! @brief 頂点属性情報のインデックスから名前を取得します。
    //!
    //! @param[in] elemIndex 頂点属性情報のインデックス。
    //!
    //! @return 頂点属性情報名へのポインターを返します。
    //!
    //! @pre
    //! - インデックスは頂点属性情報の数の範囲内。
    //!
    //! @details
    //! 指定したインデックスが範囲外の場合は ASSERT します。
    //!
    const char* GetAttrName(int elemIndex) const NN_NOEXCEPT
    {
        NN_G3D_REQUIRES_RANGE(elemIndex, 0, GetAttrCount(), GetName());
        const nn::util::ResDic* pDic = ToData().pAttrDic.Get();
        if (pDic == NULL)
        {
            return NULL;
        }
        nn::util::string_view pString = pDic->GetKey(elemIndex);
        return pString.data();
    }

    //! @brief インデックス引きで頂点属性情報を取得します。
    //!
    //! @param[in] elemIndex 頂点属性情報のインデックス。
    //!
    //! @return ResAttrVar へのポインターを返します。
    //!
    //! @pre
    //! - インデックスは頂点属性情報の数の範囲内。
    //!
    //! @details
    //! 指定したインデックスが範囲外の場合は ASSERT します。
    //!
    ResAttrVar* GetAttr(int elemIndex) NN_NOEXCEPT
    {
        NN_G3D_REQUIRES_RANGE(elemIndex, 0, GetAttrCount(), GetName());
        ResAttrVar* pArray = ToData().pAttrArray.Get();
        NN_SDK_ASSERT_NOT_NULL(pArray);
        return &pArray[elemIndex];
    }
    //! @brief インデックス引きで頂点属性情報を取得します。
    //!
    //! @param[in] elemIndex 頂点属性情報のインデックス。
    //!
    //! @return ResAttrVar へのポインターを返します。
    //!
    //! @pre
    //! - インデックスは頂点属性情報の数の範囲内。
    //!
    //! @details
    //! 指定したインデックスが範囲外の場合は ASSERT します。
    //!
    const ResAttrVar* GetAttr(int elemIndex) const NN_NOEXCEPT
    {
        NN_G3D_REQUIRES_RANGE(elemIndex, 0, GetAttrCount(), GetName());
        const ResAttrVar* pArray = ToData().pAttrArray.Get();
        NN_SDK_ASSERT_NOT_NULL(pArray);
        return &pArray[elemIndex];
    }

    //@}

    //----------------------------------------
    //! @name サンプラー
    //@{

    //! @brief サンプラー情報の数を取得します。
    //!
    //! @return サンプラー情報の数を返します。
    //!
    int GetSamplerCount() const NN_NOEXCEPT
    {
        return ToData().samplerCount;
    }

    //! @brief 指定した名前のサンプラー情報を取得します。
    //!
    //! @param[in] str サンプラー情報名。
    //!
    //! @return ResSamplerVar へのポインターを返します。
    //!
    //! @details
    //! 指定した名前のサンプラー情報が無い場合は NULL を返します。
    //!
    //! 名前引きよりインデックス引きほうが高速に動作します。インデックスを
    //! 取得後、記録しておき、通常はインデックス引きを使用することを推奨します。
    //!
    ResSamplerVar* FindSampler(const char* str) NN_NOEXCEPT
    {
        const nn::util::ResDic* pDic = ToData().pSamplerDic.Get();
        if (pDic == NULL)
        {
            return NULL;
        }
        int index = pDic->FindIndex(str);
        if (index == nn::util::ResDic::Npos)
        {
            return NULL;
        }
        ResSamplerVar* pArray = ToData().pSamplerArray.Get();
        NN_SDK_ASSERT_NOT_NULL(pArray);
        return &pArray[index];
    }
    //! @brief 指定した名前のサンプラー情報を取得します。
    //!
    //! @param[in] str サンプラー情報名。
    //!
    //! @return ResSamplerVar へのポインターを返します。
    //!
    //! @details
    //! 指定した名前のサンプラー情報が無い場合は NULL を返します。
    //!
    //! 名前引きよりインデックス引きほうが高速に動作します。インデックスを
    //! 取得後、記録しておき、通常はインデックス引きを使用することを推奨します。
    //!
    const ResSamplerVar* FindSampler(const char* str) const NN_NOEXCEPT
    {
        const nn::util::ResDic* pDic = ToData().pSamplerDic.Get();
        if (pDic == NULL)
        {
            return NULL;
        }
        int index = pDic->FindIndex(str);
        if (index == nn::util::ResDic::Npos)
        {
            return NULL;
        }
        const ResSamplerVar* pArray = ToData().pSamplerArray.Get();
        NN_SDK_ASSERT_NOT_NULL(pArray);
        return &pArray[index];
    }

    //! @brief 指定した名前のサンプラー情報のインデックスを取得します。
    //!
    //! @param[in] str サンプラー情報名。
    //!
    //! @return サンプラー情報のインデックスを返します。
    //!
    //! @details
    //! 指定した名前のサンプラー情報が無い場合は nn::util::ResDic::Npos を返します。
    //!
    int FindSamplerIndex(const char* str) const NN_NOEXCEPT
    {
        const nn::util::ResDic* pDic = ToData().pSamplerDic.Get();
        if (pDic == NULL)
        {
            return nn::util::ResDic::Npos;
        }
        return pDic->FindIndex(str);
    }
    //! @brief サンプラー情報のインデックスから名前を取得します。
    //!
    //! @param[in] elemIndex サンプラー情報のインデックス。
    //!
    //! @return サンプラー情報名へのポインターを返します。
    //!
    //! @pre
    //! - インデックスはサンプラー情報の数の範囲内。
    //!
    //! @details
    //! 指定したインデックスが範囲外の場合は ASSERT します。
    //!
    const char* GetSamplerName(int elemIndex) const NN_NOEXCEPT
    {
        NN_G3D_REQUIRES_RANGE(elemIndex, 0, GetSamplerCount(), GetName());
        const nn::util::ResDic* pDic = ToData().pSamplerDic.Get();
        if (pDic == NULL)
        {
            return NULL;
        }
        nn::util::string_view pString = pDic->GetKey(elemIndex);
        return pString.data();
    }

    //! @brief インデックス引きでサンプラー情報を取得します。
    //!
    //! @param[in] elemIndex サンプラー情報のインデックス。
    //!
    //! @return ResSamplerVar へのポインターを返します。
    //!
    //! @pre
    //! - インデックスはサンプラー情報の数の範囲内。
    //!
    //! @details
    //! 指定したインデックスが範囲外の場合は ASSERT します。
    //!
    ResSamplerVar* GetSampler(int elemIndex) NN_NOEXCEPT
    {
        NN_G3D_REQUIRES_RANGE(elemIndex, 0, GetSamplerCount(), GetName());
        ResSamplerVar* pArray = ToData().pSamplerArray.Get();
        NN_SDK_ASSERT_NOT_NULL(pArray);
        return &pArray[elemIndex];
    }
    //! @brief インデックス引きでサンプラー情報を取得します。
    //!
    //! @param[in] elemIndex サンプラー情報のインデックス。
    //!
    //! @return ResSamplerVar へのポインターを返します。
    //!
    //! @pre
    //! - インデックスはサンプラー情報の数の範囲内。
    //!
    //! @details
    //! 指定したインデックスが範囲外の場合は ASSERT します。
    //!
    const ResSamplerVar* GetSampler(int elemIndex) const NN_NOEXCEPT
    {
        NN_G3D_REQUIRES_RANGE(elemIndex, 0, GetSamplerCount(), GetName());
        const ResSamplerVar* pArray = ToData().pSamplerArray.Get();
        NN_SDK_ASSERT_NOT_NULL(pArray);
        return &pArray[elemIndex];
    }

    //@}

    //----------------------------------------
    //! @name イメージ
    //@{

    //! @brief イメージ情報の数を取得します。
    //!
    //! @return イメージ情報の数を返します。
    //!
    int GetImageCount() const NN_NOEXCEPT
    {
        return ToData().imageCount;
    }

    //! @brief 指定した名前のイメージ情報を取得します。
    //!
    //! @param[in] str イメージ情報名。
    //!
    //! @return ResImageVar へのポインターを返します。
    //!
    //! @details
    //! 指定した名前のイメージ情報が無い場合は NULL を返します。
    //!
    //! 名前引きよりインデックス引きほうが高速に動作します。インデックスを
    //! 取得後、記録しておき、通常はインデックス引きを使用することを推奨します。
    //!
    ResImageVar* FindImage(const char* str) NN_NOEXCEPT
    {
        const nn::util::ResDic* pDic = ToData().pImageDic.Get();
        if (pDic == NULL)
        {
            return NULL;
        }
        int index = pDic->FindIndex(str);
        if (index == nn::util::ResDic::Npos)
        {
            return NULL;
        }
        ResImageVar* pArray = ToData().pImageArray.Get();
        NN_SDK_ASSERT_NOT_NULL(pArray);
        return &pArray[index];
    }
    //! @brief 指定した名前のイメージ情報を取得します。
    //!
    //! @param[in] str イメージ情報名。
    //!
    //! @return ResImageVar へのポインターを返します。
    //!
    //! @details
    //! 指定した名前のイメージ情報が無い場合は NULL を返します。
    //!
    //! 名前引きよりインデックス引きほうが高速に動作します。インデックスを
    //! 取得後、記録しておき、通常はインデックス引きを使用することを推奨します。
    //!
    const ResImageVar* FindImage(const char* str) const NN_NOEXCEPT
    {
        const nn::util::ResDic* pDic = ToData().pImageDic.Get();
        if (pDic == NULL)
        {
            return NULL;
        }
        int index = pDic->FindIndex(str);
        if (index == nn::util::ResDic::Npos)
        {
            return NULL;
        }
        const ResImageVar* pArray = ToData().pImageArray.Get();
        NN_SDK_ASSERT_NOT_NULL(pArray);
        return &pArray[index];
    }

    //! @brief 指定した名前のイメージ情報のインデックスを取得します。
    //!
    //! @param[in] str イメージ情報名。
    //!
    //! @return イメージ情報のインデックスを返します。
    //!
    //! @details
    //! 指定した名前のイメージ情報が無い場合は nn::util::ResDic::Npos を返します。
    //!
    int FindImageIndex(const char* str) const NN_NOEXCEPT
    {
        const nn::util::ResDic* pDic = ToData().pImageDic.Get();
        if (pDic == NULL)
        {
            return nn::util::ResDic::Npos;
        }
        return pDic->FindIndex(str);
    }
    //! @brief イメージ情報のインデックスから名前を取得します。
    //!
    //! @param[in] elemIndex イメージ情報のインデックス。
    //!
    //! @return イメージ情報名へのポインターを返します。
    //!
    //! @pre
    //! - インデックスはイメージ情報の数の範囲内。
    //!
    //! @details
    //! 指定したインデックスが範囲外の場合は ASSERT します。
    //!
    const char* GetImageName(int elemIndex) const NN_NOEXCEPT
    {
        NN_G3D_REQUIRES_RANGE(elemIndex, 0, GetImageCount(), GetName());
        const nn::util::ResDic* pDic = ToData().pImageDic.Get();
        if (pDic == NULL)
        {
            return NULL;
        }
        nn::util::string_view pString = pDic->GetKey(elemIndex);
        return pString.data();
    }

    //! @brief インデックス引きでイメージ情報を取得します。
    //!
    //! @param[in] elemIndex イメージ情報のインデックス。
    //!
    //! @return ResImageVar へのポインターを返します。
    //!
    //! @pre
    //! - インデックスはイメージ情報の数の範囲内。
    //!
    //! @details
    //! 指定したインデックスが範囲外の場合は ASSERT します。
    //!
    ResImageVar* GetImage(int elemIndex) NN_NOEXCEPT
    {
        NN_G3D_REQUIRES_RANGE(elemIndex, 0, GetImageCount(), GetName());
        ResImageVar* pArray = ToData().pImageArray.Get();
        NN_SDK_ASSERT_NOT_NULL(pArray);
        return &pArray[elemIndex];
    }
    //! @brief インデックス引きでイメージ情報を取得します。
    //!
    //! @param[in] elemIndex イメージ情報のインデックス。
    //!
    //! @return ResImageVar へのポインターを返します。
    //!
    //! @pre
    //! - インデックスはイメージ情報の数の範囲内。
    //!
    //! @details
    //! 指定したインデックスが範囲外の場合は ASSERT します。
    //!
    const ResImageVar* GetImage(int elemIndex) const NN_NOEXCEPT
    {
        NN_G3D_REQUIRES_RANGE(elemIndex, 0, GetImageCount(), GetName());
        const ResImageVar* pArray = ToData().pImageArray.Get();
        NN_SDK_ASSERT_NOT_NULL(pArray);
        return &pArray[elemIndex];
    }

    //@}

    //----------------------------------------
    //! @name ユニフォーム
    //@{

    //! @brief ユニフォームブロック情報の数を取得します。
    //!
    //! @return ユニフォームブロック情報の数を返します。
    //!
    int GetUniformBlockCount() const NN_NOEXCEPT
    {
        return ToData().uniformBlockCount;
    }

    //! @brief 指定した名前のユニフォームブロック情報を取得します。
    //!
    //! @param[in] str ユニフォームブロック情報名。
    //!
    //! @return ResUniformBlockVar へのポインターを返します。
    //!
    //! @details
    //! 指定した名前のユニフォームブロック情報が無い場合は NULL を返します。
    //!
    //! 名前引きよりインデックス引きほうが高速に動作します。インデックスを
    //! 取得後、記録しておき、通常はインデックス引きを使用することを推奨します。
    //!
    ResUniformBlockVar* FindUniformBlock(const char* str) NN_NOEXCEPT
    {
        const nn::util::ResDic* pDic = ToData().pUniformBlockDic.Get();
        if (pDic == NULL)
        {
            return NULL;
        }
        int index = pDic->FindIndex(str);
        if (index == nn::util::ResDic::Npos)
        {
            return NULL;
        }
        ResUniformBlockVar* pArray = ToData().pUniformBlockArray.Get();
        NN_SDK_ASSERT_NOT_NULL(pArray);
        return &pArray[index];
    }
    //! @brief 指定した名前のユニフォームブロック情報を取得します。
    //!
    //! @param[in] str ユニフォームブロック情報名。
    //!
    //! @return ResUniformBlockVar へのポインターを返します。
    //!
    //! @details
    //! 指定した名前のユニフォームブロック情報が無い場合は NULL を返します。
    //!
    //! 名前引きよりインデックス引きほうが高速に動作します。インデックスを
    //! 取得後、記録しておき、通常はインデックス引きを使用することを推奨します。
    //!
    const ResUniformBlockVar* FindUniformBlock(const char* str) const NN_NOEXCEPT
    {
        const nn::util::ResDic* pDic = ToData().pUniformBlockDic.Get();
        if (pDic == NULL)
        {
            return NULL;
        }
        int index = pDic->FindIndex(str);
        if (index == nn::util::ResDic::Npos)
        {
            return NULL;
        }
        const ResUniformBlockVar* pArray = ToData().pUniformBlockArray.Get();
        NN_SDK_ASSERT_NOT_NULL(pArray);
        return &pArray[index];
    }

    //! @brief 指定した名前のユニフォームブロック情報のインデックスを取得します。
    //!
    //! @param[in] str ユニフォームブロック情報名。
    //!
    //! @return ユニフォームブロック情報のインデックスを返します。
    //!
    //! @details
    //! 指定した名前のユニフォームブロック情報が無い場合は nn::util::ResDic::Npos を返します。
    //!
    int FindUniformBlockIndex(const char* str) const NN_NOEXCEPT
    {
        const nn::util::ResDic* pDic = ToData().pUniformBlockDic.Get();
        if (pDic == NULL)
        {
            return nn::util::ResDic::Npos;
        }
        return pDic->FindIndex(str);
    }
    //! @brief ユニフォームブロック情報のインデックスから名前を取得します。
    //!
    //! @param[in] elemIndex ユニフォームブロック情報のインデックス。
    //!
    //! @return ユニフォームブロック情報名へのポインターを返します。
    //!
    //! @pre
    //! - インデックスはユニフォームブロック情報の数の範囲内。
    //!
    //! @details
    //! 指定したインデックスが範囲外の場合は ASSERT します。
    //!
    const char* GetUniformBlockName(int elemIndex) const NN_NOEXCEPT
    {
        NN_G3D_REQUIRES_RANGE(elemIndex, 0, GetUniformBlockCount(), GetName());
        const nn::util::ResDic* pDic = ToData().pUniformBlockDic.Get();
        if (pDic == NULL)
        {
            return NULL;
        }
        nn::util::string_view pString = pDic->GetKey(elemIndex);
        return pString.data();
    }

    //! @brief インデックス引きでユニフォームブロック情報を取得します。
    //!
    //! @param[in] elemIndex ユニフォームブロック情報のインデックス。
    //!
    //! @return ResUniformBlockVar へのポインターを返します。
    //!
    //! @pre
    //! - インデックスはユニフォームブロック情報の数の範囲内。
    //!
    //! @details
    //! 指定したインデックスが範囲外の場合は ASSERT します。
    //!
    ResUniformBlockVar* GetUniformBlock(int elemIndex) NN_NOEXCEPT
    {
        NN_G3D_REQUIRES_RANGE(elemIndex, 0, GetUniformBlockCount(), GetName());
        ResUniformBlockVar* pArray = ToData().pUniformBlockArray.Get();
        NN_SDK_ASSERT_NOT_NULL(pArray);
        return &pArray[elemIndex];
    }
    //! @brief インデックス引きでユニフォームブロック情報を取得します。
    //!
    //! @param[in] elemIndex ユニフォームブロック情報のインデックス。
    //!
    //! @return ResUniformBlockVar へのポインターを返します。
    //!
    //! @pre
    //! - インデックスはユニフォームブロック情報の数の範囲内。
    //!
    //! @details
    //! 指定したインデックスが範囲外の場合は ASSERT します。
    //!
    const ResUniformBlockVar* GetUniformBlock(int elemIndex) const NN_NOEXCEPT
    {
        NN_G3D_REQUIRES_RANGE(elemIndex, 0, GetUniformBlockCount(), GetName());
        const ResUniformBlockVar* pArray = ToData().pUniformBlockArray.Get();
        NN_SDK_ASSERT_NOT_NULL(pArray);
        return &pArray[elemIndex];
    }

    //! @brief ユニフォーム変数情報の数を取得します。
    //!
    //! @return ユニフォーム変数情報の数を返します。
    //!
    int GetUniformCount() const NN_NOEXCEPT
    {
        return ToData().uniformCount;
    }
    //! @brief インデックス引きでユニフォーム変数情報を取得します。
    //!
    //! @param[in] elemIndex ユニフォーム変数情報のインデックス。
    //!
    //! @return ResUniformVar へのポインターを返します。
    //!
    //! @pre
    //! - インデックスはユニフォーム変数情報の数の範囲内。
    //!
    //! @details
    //! 指定したインデックスが範囲外の場合は ASSERT します。
    //!
    ResUniformVar* GetUniform(int elemIndex) NN_NOEXCEPT
    {
        NN_G3D_REQUIRES_RANGE(elemIndex, 0, GetUniformCount(), GetName());
        ResUniformVar* pArray = ToData().pUniformArray.Get();
        NN_SDK_ASSERT_NOT_NULL(pArray);
        return &pArray[elemIndex];
    }
    //! @brief インデックス引きでユニフォーム変数情報を取得します。
    //!
    //! @param[in] elemIndex ユニフォーム変数情報のインデックス。
    //!
    //! @return ResUniformVar へのポインターを返します。
    //!
    //! @pre
    //! - インデックスはユニフォーム変数情報の数の範囲内。
    //!
    //! @details
    //! 指定したインデックスが範囲外の場合は ASSERT します。
    //!
    const ResUniformVar* GetUniform(int elemIndex) const NN_NOEXCEPT
    {
        NN_G3D_REQUIRES_RANGE(elemIndex, 0, GetUniformCount(), GetName());
        const ResUniformVar* pArray = ToData().pUniformArray.Get();
        NN_SDK_ASSERT_NOT_NULL(pArray);
        return &pArray[elemIndex];
    }

    //! @brief システム既定のユニフォームブロックのインデックスを取得します。
    //!
    //! @return システム既定のユニフォームブロックのインデックスを返します。
    //!
    int GetSystemBlockIndex(ResUniformBlockVar::Type type) const
    {
        NN_G3D_REQUIRES(type != ResUniformBlockVar::Type_None, GetName());
        return ToData().systemBlockIndices[type - 1];
    }

    //@}

    //----------------------------------------
    //! @name シェーダーストレージ
    //@{

    //! @brief シェーダーストレージブロック情報の数を取得します。
    //!
    //! @return シェーダーストレージブロック情報の数を返します。
    //!
    int GetShaderStorageBlockCount() const NN_NOEXCEPT
    {
        return ToData().shaderStorageBlockCount;
    }

    //! @brief 指定した名前のシェーダーストレージブロック情報を取得します。
    //!
    //! @param[in] str シェーダーストレージブロック情報名。
    //!
    //! @return ResUniformBlockVar へのポインターを返します。
    //!
    //! @details
    //! 指定した名前のシェーダーストレージブロック情報が無い場合は NULL を返します。
    //!
    //! 名前引きよりインデックス引きほうが高速に動作します。インデックスを
    //! 取得後、記録しておき、通常はインデックス引きを使用することを推奨します。
    //!
    ResShaderStorageBlockVar* FindShaderStorageBlock(const char* str) NN_NOEXCEPT
    {
        const nn::util::ResDic* pDic = ToData().pShaderStorageBlockDic.Get();
        if (pDic == NULL)
        {
            return NULL;
        }
        int index = pDic->FindIndex(str);
        if (index == nn::util::ResDic::Npos)
        {
            return NULL;
        }
        ResShaderStorageBlockVar* pArray = ToData().pShaderStorageBlockArray.Get();
        NN_SDK_ASSERT_NOT_NULL(pArray);
        return &pArray[index];
    }
    //! @brief 指定した名前のシェーダーストレージブロック情報を取得します。
    //!
    //! @param[in] str シェーダーストレージブロック情報名。
    //!
    //! @return ResUniformBlockVar へのポインターを返します。
    //!
    //! @details
    //! 指定した名前のシェーダーストレージブロック情報が無い場合は NULL を返します。
    //!
    //! 名前引きよりインデックス引きほうが高速に動作します。インデックスを
    //! 取得後、記録しておき、通常はインデックス引きを使用することを推奨します。
    //!
    const ResShaderStorageBlockVar* FindShaderStorageBlock(const char* str) const NN_NOEXCEPT
    {
        const nn::util::ResDic* pDic = ToData().pShaderStorageBlockDic.Get();
        if (pDic == NULL)
        {
            return NULL;
        }
        int index = pDic->FindIndex(str);
        if (index == nn::util::ResDic::Npos)
        {
            return NULL;
        }
        const ResShaderStorageBlockVar* pArray = ToData().pShaderStorageBlockArray.Get();
        NN_SDK_ASSERT_NOT_NULL(pArray);
        return &pArray[index];
    }

    //! @brief 指定した名前のシェーダーストレージブロック情報のインデックスを取得します。
    //!
    //! @param[in] str シェーダーストレージブロック情報名。
    //!
    //! @return シェーダーストレージブロック情報のインデックスを返します。
    //!
    //! @details
    //! 指定した名前のシェーダーストレージブロック情報が無い場合は nn::util::ResDic::Npos を返します。
    //!
    int FindShaderStorageBlockIndex(const char* str) const NN_NOEXCEPT
    {
        const nn::util::ResDic* pDic = ToData().pShaderStorageBlockDic.Get();
        if (pDic == NULL)
        {
            return nn::util::ResDic::Npos;
        }
        return pDic->FindIndex(str);
    }
    //! @brief シェーダーストレージブロック情報のインデックスから名前を取得します。
    //!
    //! @param[in] elemIndex シェーダーストレージブロック情報のインデックス。
    //!
    //! @return シェーダーストレージブロック情報名へのポインターを返します。
    //!
    //! @pre
    //! - インデックスはシェーダーストレージブロック情報の数の範囲内。
    //!
    //! @details
    //! 指定したインデックスが範囲外の場合は ASSERT します。
    //!
    const char* GetShaderStorageBlockName(int elemIndex) const NN_NOEXCEPT
    {
        NN_G3D_REQUIRES_RANGE(elemIndex, 0, GetShaderStorageBlockCount(), GetName());
        const nn::util::ResDic* pDic = ToData().pShaderStorageBlockDic.Get();
        if (pDic == NULL)
        {
            return NULL;
        }
        nn::util::string_view pString = pDic->GetKey(elemIndex);
        return pString.data();
    }

    //! @brief インデックス引きでシェーダーストレージブロック情報を取得します。
    //!
    //! @param[in] elemIndex シェーダーストレージブロック情報のインデックス。
    //!
    //! @return ResUniformBlockVar へのポインターを返します。
    //!
    //! @pre
    //! - インデックスはシェーダーストレージブロック情報の数の範囲内。
    //!
    //! @details
    //! 指定したインデックスが範囲外の場合は ASSERT します。
    //!
    ResShaderStorageBlockVar* GetShaderStorageBlock(int elemIndex) NN_NOEXCEPT
    {
        NN_G3D_REQUIRES_RANGE(elemIndex, 0, GetShaderStorageBlockCount(), GetName());
        ResShaderStorageBlockVar* pArray = ToData().pShaderStorageBlockArray.Get();
        NN_SDK_ASSERT_NOT_NULL(pArray);
        return &pArray[elemIndex];
    }
    //! @brief インデックス引きでシェーダーストレージブロック情報を取得します。
    //!
    //! @param[in] elemIndex シェーダーストレージブロック情報のインデックス。
    //!
    //! @return ResUniformBlockVar へのポインターを返します。
    //!
    //! @pre
    //! - インデックスはシェーダーストレージブロック情報の数の範囲内。
    //!
    //! @details
    //! 指定したインデックスが範囲外の場合は ASSERT します。
    //!
    const ResShaderStorageBlockVar* GetShaderStorageBlock(int elemIndex) const NN_NOEXCEPT
    {
        NN_G3D_REQUIRES_RANGE(elemIndex, 0, GetShaderStorageBlockCount(), GetName());
        const ResShaderStorageBlockVar* pArray = ToData().pShaderStorageBlockArray.Get();
        NN_SDK_ASSERT_NOT_NULL(pArray);
        return &pArray[elemIndex];
    }

    //! @brief シェーダーストレージ変数情報の数を取得します。
    //!
    //! @return シェーダーストレージ変数情報の数を返します。
    //!
    int GetShaderStorageCount() const NN_NOEXCEPT
    {
        return ToData().shaderStorageCount;
    }
    //! @brief インデックス引きでシェーダーストレージ変数情報を取得します。
    //!
    //! @param[in] elemIndex シェーダーストレージ変数情報のインデックス。
    //!
    //! @return ResUniformVar へのポインターを返します。
    //!
    //! @pre
    //! - インデックスはシェーダーストレージ変数情報の数の範囲内。
    //!
    //! @details
    //! 指定したインデックスが範囲外の場合は ASSERT します。
    //!
    ResShaderStorageVar* GetShaderStorage(int elemIndex) NN_NOEXCEPT
    {
        NN_G3D_REQUIRES_RANGE(elemIndex, 0, GetShaderStorageCount(), GetName());
        ResShaderStorageVar* pArray = ToData().pShaderStorageArray.Get();
        NN_SDK_ASSERT_NOT_NULL(pArray);
        return &pArray[elemIndex];
    }
    //! @brief インデックス引きでシェーダーストレージ変数情報を取得します。
    //!
    //! @param[in] elemIndex シェーダーストレージ変数情報のインデックス。
    //!
    //! @return ResUniformVar へのポインターを返します。
    //!
    //! @pre
    //! - インデックスはシェーダーストレージ変数情報の数の範囲内。
    //!
    //! @details
    //! 指定したインデックスが範囲外の場合は ASSERT します。
    //!
    const ResShaderStorageVar* GetShaderStorage(int elemIndex) const NN_NOEXCEPT
    {
        NN_G3D_REQUIRES_RANGE(elemIndex, 0, GetShaderStorageCount(), GetName());
        const ResShaderStorageVar* pArray = ToData().pShaderStorageArray.Get();
        NN_SDK_ASSERT_NOT_NULL(pArray);
        return &pArray[elemIndex];
    }

    //! @brief システム既定のシェーダーストレージブロックのインデックスを取得します。
    //!
    //! @return システム既定のシェーダーストレージブロックのインデックスを返します。
    //!
    int GetSystemShaderStorageBlockIndex(ResShaderStorageBlockVar::Type type) const
    {
        NN_G3D_REQUIRES(type == ResShaderStorageBlockVar::Type_Skeleton, GetName());
        return ToData().systemShaderStorageBlockIndices[type - 1];
    }

    //@}

    //----------------------------------------
    //! @name シェーダープログラム
    //@{

    //! @brief シェーダープログラムの数を取得します。
    //!
    //! @return シェーダープログラムの数を返します。
    //!
    int GetShaderProgramCount() const NN_NOEXCEPT
    {
        return ToData().shaderProgramCount;
    }
    //! @brief インデックス引きでシェーダープログラムを取得します。
    //!
    //! @param[in] elemIndex シェーダープログラムのインデックス。
    //!
    //! @return ResShaderProgram へのポインターを返します。
    //!
    //! @pre
    //! - インデックスはシェーダープログラムの数の範囲内。
    //!
    //! @details
    //! 指定したインデックスが範囲外の場合は ASSERT します。
    //!
    ResShaderProgram* GetShaderProgram(int elemIndex) NN_NOEXCEPT
    {
        NN_G3D_REQUIRES_RANGE(elemIndex, 0, GetShaderProgramCount(), GetName());
        ResShaderProgram* pArray = ToData().pShaderProgramArray.Get();
        NN_SDK_ASSERT_NOT_NULL(pArray);
        return &pArray[elemIndex];
    }
    //! @brief インデックス引きでシェーダープログラムを取得します。
    //!
    //! @param[in] elemIndex シェーダープログラムのインデックス。
    //!
    //! @return ResShaderProgram へのポインターを返します。
    //!
    //! @pre
    //! - インデックスはシェーダープログラムの数の範囲内。
    //!
    //! @details
    //! 指定したインデックスが範囲外の場合は ASSERT します。
    //!
    const ResShaderProgram* GetShaderProgram(int elemIndex) const NN_NOEXCEPT
    {
        NN_G3D_REQUIRES_RANGE(elemIndex, 0, GetShaderProgramCount(), GetName());
        const ResShaderProgram* pArray = ToData().pShaderProgramArray.Get();
        NN_SDK_ASSERT_NOT_NULL(pArray);
        return &pArray[elemIndex];
    }

    //! @brief シェーダープログラムの検索に使用するキーの Bit32 換算の長さを取得します。
    //!
    //! @return シェーダープログラムの検索に使用するキーの Bit32 換算の長さを返します。
    //!
    int GetKeyLength() const NN_NOEXCEPT
    {
        return ToData().staticKeyLength + ToData().dynamicKeyLength;
    }

    //! @brief シェーダープログラムの検索に使用する static オプションのキーの Bit32 換算の長さを取得します。
    //!
    //! @return シェーダープログラムの検索に使用する static オプションのキーの Bit32 換算の長さを返します。
    //!
    int GetStaticKeyLength() const NN_NOEXCEPT
    {
        return ToData().staticKeyLength;
    }

    //! @brief シェーダープログラムの検索に使用する dynamic オプションのキーの Bit32 換算の長さを取得します。
    //!
    //! @return シェーダープログラムの検索に使用する dynamic オプションのキーの Bit32 換算の長さを返します。
    //!
    int GetDynamicKeyLength() const NN_NOEXCEPT
    {
        return ToData().dynamicKeyLength;
    }

    //! @brief デフォルトの Static オプションのシェーダーキーを書き込みます。
    //!
    //! @param[out] pStaticKey シェーダーキーを書き込む先へのポインター。
    //!
    void WriteDefaultStaticKey(Bit32* pStaticKey) const NN_NOEXCEPT;

    //! @brief デフォルトの Dynamic オプションのシェーダーキーを書き込みます。
    //!
    //! @param[out] pDynamicKey シェーダーキーを書き込む先へのポインター。
    //!
    void WriteDefaultDynamicKey(Bit32* pDynamicKey) const NN_NOEXCEPT;

    //! @brief 存在しないシェーダーキーを書き込みます。
    //!
    //! @param[out] pDynamicKey シェーダーキーを書き込む先へのポインター。
    //!
    void WriteInvalidDynamicKey(Bit32* pDynamicKey) const NN_NOEXCEPT;

    //! @brief キーで検索してシェーダープログラムのインデックスを取得します。
    //!
    //! @param[in] pKey シェーダーキーへのポインター。
    //!
    //! @return 検索したシェーダープログラムのインデックスを返します。
    //!
    int FindProgramIndex(const Bit32* pKey) const NN_NOEXCEPT;

    //! @brief キーで検索してシェーダープログラムのインデックスを取得します。
    //!
    //! @param[in] range シェーダーキーの範囲。
    //! @param[in] pDynamicKey Dynamic オプションのキーへのポインター。
    //!
    //! @return 検索したシェーダープログラムのインデックスを返します。
    //!
    int FindProgramIndex(const ShaderRange& range, const Bit32* pDynamicKey) const NN_NOEXCEPT;

    //! @brief static オプションのキーで検索して該当するシェーダープログラムの範囲を取得します。
    //!
    //! @param[out] pRange シェーダーキーの範囲を示す ShaderRange へのポインター。
    //! @param[in] pStaticKey Static オプションのキーへのポインター。
    //!
    //! @return 検索が成功した場合は true、失敗した場合は false を返します。
    //!
    bool FindProgramRange(ShaderRange* pRange, const Bit32* pStaticKey) const NN_NOEXCEPT;

    //! @brief デフォルトプログラムのインデックスを取得します。
    //!
    //! @return デフォルトプログラムのインデックスを返します。
    //!
    int GetDefaultProgramIndex() const NN_NOEXCEPT
    {
        return ToData().defaultProgramIndex;
    }

    //! @brief 指定したシェーダープログラムのインデックスを取得します。
    //!
    //! @return 指定したシェーダープログラムのインデックスを返します。
    //!
    int GetProgramIndex(const ResShaderProgram* pProgram) const NN_NOEXCEPT
    {
        // 配列に並んでいることを利用して計算します。
        int programIndex = static_cast<int>(pProgram - GetShaderProgram(0));
        NN_G3D_ASSERT(0 <= programIndex && programIndex < GetShaderProgramCount(), GetName());
        return programIndex;
    }

    //! @brief 指定したインデックスのシェーダープログラムに対応するキーを取得します。
    //!
    //! @param[in] programIndex シェーダープログラムのインデックス。
    //!
    //! @return 対応するキーを返します。
    //!
    const Bit32* GetKey(int programIndex) const NN_NOEXCEPT;

    //! @brief 指定したインデックスのシェーダープログラムに対応するキーを取得します。
    //!
    //! @param[in] programIndex シェーダープログラムのインデックス。
    //!
    //! @return 対応する Static オプションのキーを返します。
    //!
    const Bit32* GetStaticKey(int programIndex) const NN_NOEXCEPT;

    //! @brief 指定したインデックスのシェーダープログラムに対応するキーを取得します。
    //!
    //! @param[in] programIndex シェーダープログラムのインデックス。
    //!
    //! @return 対応する Dynamic オプションのキーを返します。
    //!
    const Bit32* GetDynamicKey(int programIndex) const NN_NOEXCEPT;

    //! @briefprivate
    void UpdateProgram(nn::gfx::Device* pDevice, int programIndex) NN_NOEXCEPT;

    //@}

    //----------------------------------------
    //! @name デバッグ用
    //@{

    //! @brief キーを文字列に変換します。
    //!
    //! @param[in] pStr 文字列を格納するバッファーへのポインター。
    //! @param[in] strLength 文字列バッファーのサイズ。
    //! @param[in] pKey シェーダーキーへのポインター。
    //! @param[in] keyLength シェーダーキーの長さ。
    //!
    //! @return 書き込んだ文字列数を返します。
    //!
    //! @details
    //! 戻り値は終端文字を含まない文字列長です。pStr が NULL でない場合は必ず終端されます。
    //! strLength が必要なサイズに満たない場合は戻り値は負になります。
    //! pStr を NULL、strLength を 0 にすることで終端文字を含まない文字列長を返します。
    //!
    static int PrintKeyTo(char* pStr, size_t strLength, const Bit32* pKey, int keyLength) NN_NOEXCEPT;

    //! @brief キーの Static オプション表現を文字列に変換します。
    //!
    //! @param[in] pStr 文字列を格納するバッファーへのポインター。
    //! @param[in] strLength 文字列バッファーのサイズ。
    //! @param[in] pKey シェーダーキーへのポインター。
    //!
    //! @return 書き込んだ文字列数を返します。
    //!
    //! @details
    //! 戻り値は終端文字を含まない文字列長です。pStr が NULL でない場合は必ず終端されます。
    //! strLength が必要なサイズに満たない場合は戻り値は負になります。
    //! pStr を NULL、strLength を 0 にすることで終端文字を含まない文字列長を返します。
    //!
    int PrintStaticOptionTo(char* pStr, size_t strLength, const Bit32* pKey) const NN_NOEXCEPT;

    //! @brief キーの Dynamic オプション表現を文字列に変換します。
    //!
    //! @param[in] pStr 文字列を格納するバッファーへのポインター。
    //! @param[in] strLength 文字列バッファーのサイズ。
    //! @param[in] pKey シェーダーキーへのポインター。
    //!
    //! @return 書き込んだ文字列数を返します。
    //!
    //! @details
    //! 戻り値は終端文字を含まない文字列長です。pStr が NULL でない場合は必ず終端されます。
    //! strLength が必要なサイズに満たない場合は戻り値は負になります。
    //! pStr を NULL、strLength を 0 にすることで終端文字を含まない文字列長を返します。
    //!
    int PrintDynamicOptionTo(char* pStr, size_t strLength, const Bit32* pKey) const NN_NOEXCEPT;

    //! @briefprivate シェーダーバイナリーが使用可能かをチェックします。
    //!
    //! @param[in] pDevice デバイスへのポインター。
    //!
    //! @return 使用可能な場合は true、使用可能でない場合は false を返します。
    //!
    bool IsBinaryAvailable(nn::gfx::Device* pDevice) NN_NOEXCEPT;
    //@}
};

//--------------------------------------------------------------------------------------------------

//! @briefprivate シェーダープログラムの更新が必要な場合に呼び出されるコールバックで、内部で使用します。
//!
typedef void (*UpdateProgramCallback)(nn::gfx::Device* pDevice, class ResShadingModel* pShader, int programIndex);

//! @brief シェーダーアーカイブの構造体です。
struct ResShaderArchiveData
{
    nn::util::BinPtrToString            pName; //!< バイナリー変換時に指定されたファイル名（拡張子を除く）。
    nn::util::BinPtrToString            pPath; //!< パス名。
    nn::util::BinTPtr<ResShadingModel>  pShadingModelArray; //!< シェーディングモデル配列へのポインター。
    nn::util::BinTPtr<nn::util::ResDic> pShadingModelDic; //!< シェーディングモデル辞書へのポインター。
    nn::util::BinPtr pUserPtr; //!< ユーザーポインター。
    nn::util::BinPtr pCallback; //!< @briefprivate シェーダープログラムの更新が必要な場合に呼び出されるコールバック。
    nn::util::BinPtr pWorkMemoryPtr; //!< 排他制御処理用に使用されるメモリへのポインター。
    uint8_t reserved1[8];

    uint16_t shadingModelCount; //!< シェーディングモデル数。
    Bit16    flag; //!< フラグ。
    uint8_t  reserved2[4];
};

//! @brief シェーダーアーカイブのリソースです。
class ResShaderArchive : public nn::util::AccessorBase< ResShaderArchiveData >
{
    NN_DISALLOW_COPY( ResShaderArchive );

public:
    //! @briefprivate
    typedef ResShaderArchiveData DataType;

    //! @brief シェーダーアーカイブに関するフラグです。
    enum Flag
    {
        Flag_None = 0,
        Flag_Souce = 0x01 << 0, //!< シェーダーソースをもちます。
        Flag_Binary = 0x01 << 1, //!<  シェーダーバイナリーをもちます。
        Flag_BinaryAvailable = 0x01 << 3, //!< バイナリーが使用可能です。
        Flag_ForceVariation = 0x01 << 4, //!< コンバート時にオプションが強制的にバリエーション化されています。
        Flag_ShaderOfflineDL = 0x01 << 5, //!< @deprecated 廃止予定です。シェーダーがオフライン DL 化されています。
        Flag_Ir = 0x01 << 6, //!< シェーダーが中間言語ソースをもちます。
        Flag_Initialized = 0x01 << 7, //!< シェーダーアーカイブが初期化済みです。
    };

    //! @brief 必要アライメントサイズです。
    enum Alignment
    {
        Alignment_WorkMemory = 8, //!< @brief Setup() に渡すワークメモリの必要アライメントサイズです。
    };

    //----------------------------------------
    //! @name 構築/破棄
    //@{

    //! @brief シェーダーアーカイブをセットアップします。
    //!
    //! @param[in] pDevice デバイスへのポインター。
    //!
    //! @pre
    //! - pDevice が初期化されている。
    //!
    //! @details
    //! シェーダーの初期化は行われません。nn::g3d::ResShaderProgram::Update() の呼び出しまでシェーダーの初期化は遅延されます。
    //! nn::g3d::ResShaderProgram::Update() は nn::g3d::ShaderSelector::UpdateVariation() から呼び出されます。
    //!
    //! 複数スレッドから同一シェーディングモデルを扱わない、もしくは複数スレッドから同一シェーディングモデルを扱うが、ユーザーの手で排他制御する場合に使用してください。
    //!
    void Setup(nn::gfx::Device* pDevice) NN_NOEXCEPT
    {
        Setup(pDevice, NULL, 0);
    }

    //! @brief シェーダーアーカイブをセットアップします。
    //!
    //! @param[in] pDevice デバイスへのポインター。
    //! @param[in] pWorkMemory 排他制御処理用に使用されるメモリへのポインター。
    //! @param[in] workMemorySize 排他制御処理用に使用されるメモリのサイズ。
    //!
    //! @pre
    //! - pDevice が初期化されている。
    //!
    //! @details
    //! この関数を使用して初期化を行った場合、シェーダーの初期化が排他制御されるようになります。複数スレッドから同一シェーディングモデルを扱う場合は、
    //! この関数を使用して初期化してください。
    //!
    //! シェーダーの初期化は行われません。nn::g3d::ResShaderProgram::Update() の呼び出しまでシェーダーの初期化は遅延されます。
    //! nn::g3d::ResShaderProgram::Update() は nn::g3d::ShaderSelector::UpdateVariation() から呼び出されます。
    //!
    void Setup(nn::gfx::Device* pDevice, void* pWorkMemory, size_t workMemorySize) NN_NOEXCEPT;

    //! @brief シェーダーアーカイブをセットアップします。
    //!
    //! @param[in] pDevice デバイスへのポインター。
    //! @param[in] pMemoryPool ResShaderArchive が配置されているメモリプールへのポインター。
    //! @param[in] memoryPoolOffset ResShaderArchive が配置されているメモリプール上のオフセット。
    //! @param[in] memoryPoolSize ResShaderArchive が配置されているメモリプールのサイズ。
    //!
    //! @pre
    //! - pDevice が初期化されている。
    //! - pMemoryPool にリソースがロードされている。
    //!
    //! @details
    //! ユーザーが自身で用意したメモリプールに ResShaderArchive をロードし、そのメモリプールを利用して ResShaderArchive をセットアップする場合にこの関数を使用します。
    //! この関数では内部でメモリプールの作成を行いません。
    //! メモリプールに ResShaderArchive を配置する場合は、シェーダーアーカイブファイルの先頭がファイルのアライメントに合うようにメモリプールに配置してください。
    //! ファイルのアライメントは GetFileHeader() でヘッダーを取得し、nn::util::BinaryFileHeader::GetAlignment() で取得することができます。
    //! ResShaderArchive には CPU アクセスを行うので、メモリプールの特性は CpuCached | GpuCached | ShaderCode が推奨されます。
    //! CpuUncached を設定した場合はパフォーマンスが低下します。CpuInvisible は使用できません。
    //!
    //! シェーダーの初期化は行われません。nn::g3d::ResShaderProgram::Update() の呼び出しまでシェーダーの初期化は遅延されます。
    //! nn::g3d::ResShaderProgram::Update() は nn::g3d::ShaderSelector::UpdateVariation() から呼び出されます。
    //!
    //! 複数スレッドから同一シェーディングモデルを扱わない、もしくは複数スレッドから同一シェーディングモデルを扱うが、ユーザーの手で排他制御する場合に使用してください。
    //!
    void Setup(nn::gfx::Device* pDevice, nn::gfx::MemoryPool* pMemoryPool, ptrdiff_t memoryPoolOffset, size_t memoryPoolSize) NN_NOEXCEPT
    {
        Setup(pDevice, pMemoryPool, memoryPoolOffset, memoryPoolSize, NULL, 0);
    }

    //! @brief シェーダーアーカイブをセットアップします。
    //!
    //! @param[in] pDevice デバイスへのポインター。
    //! @param[in] pMemoryPool ResShaderArchive が配置されているメモリプールへのポインター。
    //! @param[in] memoryPoolOffset ResShaderArchive が配置されているメモリプール上のオフセット。
    //! @param[in] memoryPoolSize ResShaderArchive が配置されているメモリプールのサイズ。
    //! @param[in] pWorkMemory 排他制御処理用に使用されるメモリへのポインター。
    //! @param[in] workMemorySize 排他制御処理用に使用されるメモリのサイズ。
    //!
    //! @pre
    //! - pDevice が初期化されている。
    //! - pMemoryPool にリソースがロードされている。
    //!
    //! @details
    //! ユーザーが自身で用意したメモリプールに ResShaderArchive をロードし、そのメモリプールを利用して ResShaderArchive をセットアップする場合にこの関数を使用します。
    //! この関数では内部でメモリプールの作成を行いません。
    //! メモリプールに ResShaderArchive を配置する場合は、シェーダーアーカイブファイルの先頭がファイルのアライメントに合うようにメモリプールに配置してください。
    //! ファイルのアライメントは GetFileHeader() でヘッダーを取得し、nn::util::BinaryFileHeader::GetAlignment() で取得することができます。
    //! ResShaderArchive には CPU アクセスを行うので、メモリプールの特性は CpuCached | GpuCached | ShaderCode が推奨されます。
    //! CpuUncached を設定した場合はパフォーマンスが低下します。CpuInvisible は使用できません。
    //!
    //! この関数を使用して初期化を行った場合、シェーダーの初期化が排他制御されるようになります。複数スレッドから同一シェーディングモデルを扱う場合は、
    //! この関数を使用して初期化してください。pWorkMemory は Alignment_WorkMemory のアライメントを要求します。必要とするワークメモリのサイズは nn::g3d::ResShaderArchive::GetWorkeMemorySize() で取得可能です。
    //!
    //! シェーダーの初期化は行われません。nn::g3d::ResShaderProgram::Update() の呼び出しまでシェーダーの初期化は遅延されます。
    //! nn::g3d::ResShaderProgram::Update() は nn::g3d::ShaderSelector::UpdateVariation() から呼び出されます。
    //!
    void Setup(nn::gfx::Device* pDevice, nn::gfx::MemoryPool* pMemoryPool, ptrdiff_t memoryPoolOffset, size_t memoryPoolSize, void* pWorkMemory, size_t workMemorySize) NN_NOEXCEPT;

    //! @brief シェーダーアーカイブをクリーンアップします。
    //!
    //! @param[in] pDevice デバイスへのポインター。
    //!
    //! @pre
    //! - pDevice が初期化されている。
    //!
    void Cleanup(nn::gfx::Device* pDevice) NN_NOEXCEPT;

    //@}

    //----------------------------------------
    //! @name 取得/設定
    //@{

    //! @brief シェーダーアーカイブ名を取得します。
    //!
    //! @return シェーダーアーカイブ名へのポインターを返します。
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
    //! @tparam T ポインターを受けとる際の型。
    //!
    //! @return SetUserPtr() で設定したポインターを返します。
    //!
    template <typename T>
    T* GetUserPtr() NN_NOEXCEPT
    {
        return static_cast<T*>(ToData().pUserPtr.Get());
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
        return static_cast<const T*>(ToData().pUserPtr.Get());
    }

    //! @brief ソースを持つかどうかを取得します。
    //!
    //! @return ソースを持つ場合は true、持たない場合は false を返します。
    //!
    bool HasSource() const NN_NOEXCEPT
    {
        return ( ToData().flag & Flag_Souce ) != 0;
    }

    //! @brief バイナリーを持つかどうかを取得します。
    //!
    //! @return バイナリーを持つ場合は true、持たない場合は false を返します。
    //!
    bool HasBinary() const NN_NOEXCEPT
    {
        return ( ToData().flag & Flag_Binary ) != 0;
    }

    //! @brief バイナリーが使用可能かどうかを取得します。
    //!
    //! @return バイナリーが使用可能な場合は true、使用可能でない場合は false を返します。
    //!
    //! @pre
    //! - Setup() が呼び出されている必要があります。
    //!
    bool IsBinaryAvailable() const NN_NOEXCEPT
    {
        return ( ToData().flag & Flag_BinaryAvailable ) != 0;
    }

    //! @brief 中間言語ソースを持つかどうかを取得します。
    //!
    //! @return 中間言語ソースを持つ場合は true、持たない場合は false を返します。
    //!
    bool HasIr() const NN_NOEXCEPT
    {
        return ( ToData().flag & Flag_Ir ) != 0;
    }

    //! @brief Setup() に渡す排他制御用のワークメモリのサイズを取得します。
    //!
    //! @return ワークメモリのサイズを返します。
    //!
    size_t GetWorkMemorySize() NN_NOEXCEPT
    {
        return sizeof(nn::os::MutexType) * GetShadingModelCount();
    }

    //! @brief Setup() に設定した排他制御用のワークメモリへのポインターを取得します。
    //!
    //! @return Setup() に設定した排他制御用のワークメモリへのポインターを返します。
    //!
    //! @details
    //! Setup() 前、Cleanup() 後は NULL を返します。
    //!
    void* GetWorkMemoryPtr() NN_NOEXCEPT
    {
        return ToData().pWorkMemoryPtr.Get();
    }

    //! @briefprivate
    UpdateProgramCallback GetUpdateProgramCallback() const NN_NOEXCEPT
    {
        void* ptr = const_cast<void*>(ToData().pCallback.Get());
        UpdateProgramCallback pCallback = reinterpret_cast<UpdateProgramCallback>(ptr); // 警告対策
        return pCallback;
    }

    //! @briefprivate
    void SetUpdateProgramCallback(UpdateProgramCallback pCallback) NN_NOEXCEPT
    {
        void* ptr = reinterpret_cast<void*>(pCallback); // 警告対策
        ToData().pCallback.Set(ptr);
    }

    //@}

    //----------------------------------------
    //! @name シェーディングモデル
    //@{

    //! @brief シェーディングモデルの数を取得します。
    //!
    //! @return シェーディングモデルの数を返します。
    //!
    int GetShadingModelCount() const NN_NOEXCEPT
    {
        return ToData().shadingModelCount;
    }
    //! @brief 指定した名前のシェーディングモデルを取得します。
    //!
    //! @param[in] str シェーディングモデル名。
    //!
    //! @return ResShadingModel へのポインターを返します。
    //!
    //! @details
    //! 指定した名前のシェーディングモデルが無い場合は NULL を返します。
    //!
    //! 名前引きよりインデックス引きほうが高速に動作します。インデックスを
    //! 取得後、記録しておき、通常はインデックス引きを使用することを推奨します。
    //!
    ResShadingModel* FindShadingModel(const char* str) NN_NOEXCEPT
    {
        const nn::util::ResDic* pDic = ToData().pShadingModelDic.Get();
        if (pDic == NULL)
        {
            return NULL;
        }
        int index = pDic->FindIndex(str);
        if (index == nn::util::ResDic::Npos)
        {
            return NULL;
        }
        return GetShadingModel(index);
    }
    //! @brief 指定した名前のシェーディングモデルを取得します。
    //!
    //! @param[in] str シェーディングモデル名。
    //!
    //! @return ResShadingModel へのポインターを返します。
    //!
    //! @details
    //! 指定した名前のシェーディングモデルが無い場合は NULL を返します。
    //!
    //! 名前引きよりインデックス引きほうが高速に動作します。インデックスを
    //! 取得後、記録しておき、通常はインデックス引きを使用することを推奨します。
    //!
    const ResShadingModel* FindShadingModel(const char* str) const NN_NOEXCEPT
    {
        const nn::util::ResDic* pDic = ToData().pShadingModelDic.Get();
        if (pDic == NULL)
        {
            return NULL;
        }
        int index = pDic->FindIndex(str);
        if (index == nn::util::ResDic::Npos)
        {
            return NULL;
        }
        return GetShadingModel(index);
    }
    //! @brief 指定した名前のシェーディングモデルのインデックスを取得します。
    //!
    //! @param[in] str シェーディングモデル名。
    //!
    //! @return シェーディングモデルのインデックスを返します。
    //!
    //! @details
    //! 指定した名前のシェーディングモデルが無い場合は nn::util::ResDic::Npos を返します。
    //!
    int FindShadingModelIndex(const char* str) const NN_NOEXCEPT
    {
        const nn::util::ResDic* pDic = ToData().pShadingModelDic.Get();
        if (pDic == NULL)
        {
            return NULL;
        }
        return pDic->FindIndex(str);
    }
    //! @brief シェーディングモデルのインデックスから名前を取得します。
    //!
    //! @param[in] elemIndex シェーディングモデルのインデックス。
    //!
    //! @return シェーディングモデル名へのポインターを返します。
    //!
    //! @pre
    //! - インデックスはシェーディングモデルの数の範囲内。
    //!
    //! @details
    //! 指定したインデックスが範囲外の場合は ASSERT します。
    //!
    const char* GetShadingModelName(int elemIndex) const NN_NOEXCEPT
    {
        NN_G3D_REQUIRES_RANGE(elemIndex, 0, GetShadingModelCount(), GetName());
        const nn::util::ResDic* pDic = ToData().pShadingModelDic.Get();
        if (pDic == NULL)
        {
            return NULL;
        }
        nn::util::string_view pString = pDic->GetKey(elemIndex);
        return pString.data();
    }
    //! @brief インデックス引きでシェーディングモデルを取得します。
    //!
    //! @param[in] elemIndex シェーディングモデルのインデックス。
    //!
    //! @return ResShadingModelへのポインターを返します。
    //!
    //! @pre
    //! - インデックスはシェーディングモデルの数の範囲内。
    //!
    //! @details
    //! 指定したインデックスが範囲外の場合は ASSERT します。
    //!
    ResShadingModel* GetShadingModel(int elemIndex) NN_NOEXCEPT
    {
        NN_G3D_REQUIRES_RANGE(elemIndex, 0, GetShadingModelCount(), GetName());
        ResShadingModel* pArray = ToData().pShadingModelArray.Get();
        NN_SDK_ASSERT_NOT_NULL(pArray);
        return &pArray[elemIndex];
    }
    //! @brief インデックス引きでシェーディングモデルを取得します。
    //!
    //! @param[in] elemIndex シェーディングモデルのインデックス。
    //!
    //! @return ResShadingModelへのポインターを返します。
    //!
    //! @pre
    //! - インデックスはシェーディングモデルの数の範囲内。
    //!
    //! @details
    //! 指定したインデックスが範囲外の場合は ASSERT します。
    //!
    const ResShadingModel* GetShadingModel(int elemIndex) const NN_NOEXCEPT
    {
        NN_G3D_REQUIRES_RANGE(elemIndex, 0, GetShadingModelCount(), GetName());
        const ResShadingModel* pArray = ToData().pShadingModelArray.Get();
        NN_SDK_ASSERT_NOT_NULL(pArray);
        return &pArray[elemIndex];
    }

    //@}
};

//--------------------------------------------------------------------------------------------------

//! @brief シェーダーファイルの構造体です。
struct ResShaderFileData
{
    nn::util::BinaryFileHeader          fileHeader; //!< バイナリーファイルヘッダー。
    nn::util::BinTPtr<ResShaderArchive> pShaderArchive; //!< シェーダーアーカイブへのポインター。
    nn::util::BinTPtr<nn::util::BinString> pStringPool; //!< 文字列プールへのポインター。
    uint32_t sizeStringPool; //!< 文字列プールのバイトサイズ。
    uint8_t  reserved[4];
};

//! @brief シェーダーアーカイブのリソースです。
class ResShaderFile : public nn::util::AccessorBase< ResShaderFileData >
{
    NN_DISALLOW_COPY( ResShaderFile );

public:
    //! @brief ResShaderArchive のシグネチャ
    static const int64_t Signature;

    //! @brief 必要アライメントサイズです。
    enum Alignment
    {
        Alignment_WorkMemory = ResShaderArchive::Alignment_WorkMemory //!< @brief Setup() に渡すワークメモリの必要アライメントサイズです。
    };

    //----------------------------------------
    //! @name 構築/破棄
    //@{

    //! @brief ロードしたファイルを ResShaderFile に変換します。
    //!
    //! @param[in] ptr ロードしたファイルへのポインター。
    //!
    //! @return ResShaderFile へのポインターを返します。
    //!
    static ResShaderFile* ResCast(void* ptr) NN_NOEXCEPT;

    //! @brief ResShaderFile を使用可能な状態にします。
    //!
    //! @details
    //! Unrelocate() 後に再度使用可能な状態にするときに使用することを想定しています。
    //! ResCast() の中でも同様の処理が行われています。
    //!
    void Relocate() NN_NOEXCEPT;

    //! @brief ResShaderFile を再配置可能な状態にします。
    //!
    //! @details
    //! Unrelocate() 後はリソースファイルを別のメモリアドレスに移動することが出来ます。
    //! 移動前に ResShaderArchive を Cleanup() しておく必要があります。
    //! 再度使用可能な状態にするためには Relocate() または ResCast() を呼び出す必要があります。
    //!
    void Unrelocate() NN_NOEXCEPT;

    //@}

    //----------------------------------------
    //! @name 構築/破棄
    //@{

    //! @brief シェーダーファイルをセットアップします。
    //!
    //! @param[in] pDevice デバイスへのポインター。
    //!
    //! @pre
    //! - pDevice が初期化されている。
    //!
    //! @details
    //! 配下のシェーダーアーカイブの Setup() を呼び出します。
    //!
    //! シェーダーの初期化は行われません。 nn::g3d::ResShaderProgram::Update() の呼び出しまでシェーダーの初期化は遅延されます。
    //! nn::g3d::ResShaderProgram::Update() は nn::g3d::ShaderSelector::UpdateVariation() から呼び出されます。
    //!
    //! 複数スレッドから同一シェーディングモデルを扱わない、もしくは複数スレッドから同一シェーディングモデルを扱うが、ユーザーの手で排他制御する場合に使用してください。
    //!
    void Setup(nn::gfx::Device* pDevice) NN_NOEXCEPT
    {
        Setup(pDevice, NULL, 0);
    }

    //! @brief シェーダーファイルをセットアップします。
    //!
    //! @param[in] pDevice デバイスへのポインター。
    //! @param[in] pWorkMemory 排他制御処理用に使用されるメモリーへのポインター。
    //! @param[in] workMemorySize 排他制御処理用に使用されるメモリーのサイズ。
    //!
    //! @pre
    //! - pDevice が初期化されている。
    //!
    //! @details
    //! 配下のシェーダーアーカイブの Setup() を呼び出します。
    //!
    //! この関数を使用して初期化を行った場合、シェーダーの初期化が排他制御されるようになります。複数スレッドから同一シェーディングモデルを扱う場合は、
    //! この関数を使用して初期化してください。
    //!
    //! シェーダーの初期化は行われません。 nn::g3d::ResShaderProgram::Update() の呼び出しまでシェーダーの初期化は遅延されます。
    //! nn::g3d::ResShaderProgram::Update() は nn::g3d::ShaderSelector::UpdateVariation() から呼び出されます。
    //!
    void Setup(nn::gfx::Device* pDevice, void* pWorkMemory, size_t workMemorySize) NN_NOEXCEPT
    {
        ToData().pShaderArchive.Get()->Setup(pDevice, pWorkMemory, workMemorySize);
    }

    //! @brief シェーダーファイルをセットアップします。
    //!
    //! @param[in] pDevice デバイスへのポインター。
    //! @param[in] pMemoryPool ResShaderFile が配置されているメモリープールへのポインター。
    //! @param[in] memoryPoolOffset ResShaderFile が配置されているメモリープール上のオフセット。
    //! @param[in] memoryPoolSize ResShaderFile が配置されているメモリープールのサイズ。
    //!
    //! @pre
    //! - pDevice が初期化されている。
    //! - pMemoryPool にリソースがロードされている。
    //!
    //! @details
    //! 配下のシェーダーアーカイブの Setup() を呼び出します。
    //!
    //! ユーザーが自身で用意したメモリープールに ResShaderFile をロードし、そのメモリープールを利用して ResShaderFile をセットアップする場合にこの関数を使用します。
    //! この関数では内部でメモリープールの作成を行いません。
    //! メモリープールに ResShaderFile を配置する場合は、シェーダーファイルの先頭がファイルのアライメントに合うようにメモリープールに配置してください。
    //! ファイルのアライメントは GetFileHeader() でヘッダーを取得し、nn::util::BinaryFileHeader::GetAlignment() で取得することができます。
    //! ResShaderFile には CPU アクセスを行うので、メモリープールの特性は CpuCached | GpuCached | ShaderCode が推奨されます。
    //! CpuUncached を設定した場合はパフォーマンスが低下します。CpuInvisible は使用できません。
    //!
    //! シェーダーの初期化は行われません。nn::g3d::ResShaderProgram::Update() の呼び出しまでシェーダーの初期化は遅延されます。
    //! nn::g3d::ResShaderProgram::Update() は nn::g3d::ShaderSelector::UpdateVariation() から呼び出されます。
    //!
    //! 複数スレッドから同一シェーディングモデルを扱わない、もしくは複数スレッドから同一シェーディングモデルを扱うが、ユーザーの手で排他制御する場合に使用してください。
    //!
    void Setup(nn::gfx::Device* pDevice, nn::gfx::MemoryPool* pMemoryPool, ptrdiff_t memoryPoolOffset, size_t memoryPoolSize) NN_NOEXCEPT
    {
        Setup(pDevice, pMemoryPool, memoryPoolOffset, memoryPoolSize, NULL, 0);
    }

    //! @brief シェーダーファイルをセットアップします。
    //!
    //! @param[in] pDevice デバイスへのポインター。
    //! @param[in] pMemoryPool ResShaderFile が配置されているメモリープールへのポインター。
    //! @param[in] memoryPoolOffset ResShaderFile が配置されているメモリープール上のオフセット。
    //! @param[in] memoryPoolSize ResShaderFile が配置されているメモリープールのサイズ。
    //! @param[in] pWorkMemory 排他制御処理用に使用されるメモリーへのポインター。
    //! @param[in] workMemorySize 排他制御処理用に使用されるメモリーのサイズ。
    //!
    //! @pre
    //! - pDevice が初期化されている。
    //! - pMemoryPool にリソースがロードされている。
    //!
    //! @details
    //! 配下のシェーダーアーカイブの Setup() を呼び出します。
    //!
    //! ユーザーが自身で用意したメモリープールに ResShaderFile をロードし、そのメモリープールを利用して ResShaderFile をセットアップする場合にこの関数を使用します。
    //! この関数では内部でメモリープールの作成を行いません。
    //! メモリープールに ResShaderFile を配置する場合は、シェーダーファイルの先頭がファイルのアライメントに合うようにメモリープールに配置してください。
    //! ファイルのアライメントは GetFileHeader() でヘッダーを取得し、nn::util::BinaryFileHeader::GetAlignment() で取得することができます。
    //! ResShaderFile には CPU アクセスを行うので、メモリープールの特性は CpuCached | GpuCached | ShaderCode が推奨されます。
    //! CpuUncached を設定した場合はパフォーマンスが低下します。CpuInvisible は使用できません。
    //!
    //! この関数を使用して初期化を行った場合、シェーダーの初期化が排他制御されるようになります。複数スレッドから同一シェーディングモデルを扱う場合は、
    //! この関数を使用して初期化してください。pWorkMemory は Alignment_WorkMemory のアライメントを要求します。必要とするワークメモリーのサイズは nn::g3d::ResShaderFile::GetWorkMemorySize() で取得可能です。
    //!
    //! シェーダーの初期化は行われません。nn::g3d::ResShaderProgram::Update() の呼び出しまでシェーダーの初期化は遅延されます。
    //! nn::g3d::ResShaderProgram::Update() は nn::g3d::ShaderSelector::UpdateVariation() から呼び出されます。
    //!
    void Setup(nn::gfx::Device* pDevice, nn::gfx::MemoryPool* pMemoryPool, ptrdiff_t memoryPoolOffset, size_t memoryPoolSize, void* pWorkMemory, size_t workMemorySize) NN_NOEXCEPT;

    //! @brief シェーダーファイルをクリーンアップします。
    //!
    //! @param[in] pDevice デバイスへのポインター。
    //!
    //! @pre
    //! - pDevice が初期化されている。
    //!
    //! @details
    //! 配下のシェーダーアーカイブの Cleanup() を呼び出します。
    //!
    void Cleanup(nn::gfx::Device* pDevice) NN_NOEXCEPT
    {
        ToData().pShaderArchive.Get()->Cleanup(pDevice);
    }

    //@}

    //----------------------------------------
    //! @name 取得/設定
    //@{

    //! @brief バイナリーデータがシェーダーファイルとして有効かどうかを取得します。
    //!
    //! @param[in] ptr バイナリーデータへのポインター。
    //!
    //! @return シェーダーファイルとして有効な場合は true、無効な場合は false を返します。
    //!
    static bool IsValid(const void* ptr) NN_NOEXCEPT;

    //! @brief リソースのヘッダを取得します。
    const nn::util::BinaryFileHeader* GetFileHeader() const NN_NOEXCEPT
    {
        return &ToData().fileHeader;
    }

    //! @brief シェーダーアーカイブを取得します。
    //!
    //! @return ResShaderArchive へのポインターを返します。
    //!
    ResShaderArchive* GetResShaderArchive() NN_NOEXCEPT
    {
        return ToData().pShaderArchive.Get();
    }

    //! @brief Setup() に渡す排他制御用のワークメモリーのサイズを取得します。
    //!
    //! @return ワークメモリーのサイズを返します。
    //!
    size_t GetWorkMemorySize() NN_NOEXCEPT
    {
        return ToData().pShaderArchive.Get()->GetWorkMemorySize();
    }

    //! @brief Setup() に設定した排他制御用のワークメモリへのポインターを取得します。
    //!
    //! @return Setup() に設定した排他制御用のワークメモリへのポインターを返します。
    //!
    //! @details
    //! Setup() 前、Cleanup() 後は NULL を返します。
    //!
    void* GetWorkMemoryPtr() NN_NOEXCEPT
    {
        return ToData().pShaderArchive.Get()->GetWorkMemoryPtr();
    }

    //@}
};

}} // namespace nw::g3d

NN_PRAGMA_POP_WARNINGS
