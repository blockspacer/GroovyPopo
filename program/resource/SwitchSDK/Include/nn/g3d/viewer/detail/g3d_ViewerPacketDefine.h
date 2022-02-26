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

#include <nn/g3d/viewer/g3d_ViewerDefine.h>



#include <nn/gfx/gfx_Enum.h>
#include <nn/util/util_StringView.h>
#include <nn/util/util_BinTypes.h>
#include <nn/nn_Common.h>

//--------------------------------------------------------------------------------------------------
// バージョン
enum {
    NN_G3D_VERSION_EDITMAJOR  = NN_G3D_VIEWER_VERSION_MAJOR
};
enum {
    NN_G3D_VERSION_EDITMINOR  = NN_G3D_VIEWER_VERSION_MINOR,
};
enum {
    NN_G3D_VERSION_EDITMICRO  = NN_G3D_VIEWER_VERSION_MICRO,
};
enum {
    NN_G3D_VERSION_EDITBUGFIX = 0,
};

#define NN_G3D_MAKE_U8X4_AS_U32(a,b,c,d)    \
    static_cast<uint32_t>(                       \
          (static_cast<uint8_t>(a) << 0)        \
        | (static_cast<uint8_t>(b) << 8)        \
        | (static_cast<uint8_t>(c) << 16)        \
        | (static_cast<uint8_t>(d) << 24)        \
        )

#define NN_G3D_EDIT_MAGIC               \
    NN_G3D_MAKE_U8X4_AS_U32('N','W','3','E')

#define NN_G3D_EDIT_VERSION             \
    NN_G3D_MAKE_U8X4_AS_U32(            \
    NN_G3D_VERSION_EDITMAJOR,           \
    NN_G3D_VERSION_EDITMINOR,           \
    NN_G3D_VERSION_EDITMICRO,           \
    NN_G3D_VERSION_EDITBUGFIX           \
    )

// クライアントから送られたパケットに後方互換があるか
#define NN_G3D_VIEWER_PACKET_IS_BACKWARD_COMPTIBLE(major, minor, micro) ((major == NN_G3D_VERSION_EDITMAJOR) && (minor <= NN_G3D_VERSION_EDITMINOR))

// サーバーから送られたパケットに後方互換があるか
#define NN_G3D_VIEWER_SERVER_PACKET_IS_BACKWARD_COMPTIBLE(major, minor, micro) ((major == NN_G3D_VERSION_EDITMAJOR) && (minor >= NN_G3D_VERSION_EDITMINOR))

namespace nn {
namespace g3d {
namespace viewer {
namespace detail {

#if defined( _MSC_VER )
#pragma warning(push)
#pragma warning(disable:4201)
#endif


struct SetupRenderInfoData
{
    uint32_t type;
    nn::util::BinString ofsName;
    nn::util::BinPtr ofsChoice;
    nn::util::BinPtr ofsDefault;
};

struct SetupRenderInfoChoiceData
{
    uint32_t numChoice;
    union
    {
        int32_t iValue[1];
        float fValue[1];
        nn::util::BinString ofsString[1];
    };
};

struct SetupRenderInfoDefaultData
{
    uint32_t numDefault;
    union
    {
        int32_t iValue[1];
        float fValue[1];
        nn::util::BinString ofsString[1];
    };
};

} // namespace detail

namespace detail {

// 実機側のチャンネル名です。
static const char* EDIT_CHANNEL_NAME = "NWG3D_EDIT";

// ネットワーク生存確認用のチャンネル名です。
static const char* EDIT_PING_CHANNEL_NAME = "NWG3D_EDIT_PING";

// PC版で使用するポート番号です。
static const uint16_t EDIT_DEFAULT_PORT_NUMBER = 61440;
static const uint16_t EDIT_PING_PORT_NUMBER = EDIT_DEFAULT_PORT_NUMBER + 1;

static const size_t NN_G3D_EDIT_FILENAME_MAX = 256;

static const size_t NN_G3D_EDIT_ALIGNMENT = 32;

struct PacketHeader
{
    uint32_t magic;      // 送信クライアントを識別するためのマジックコード

    union
    {
        uint8_t version[4];// パケットバージョン
        uint32_t verWord;
    };

    int32_t dataSize;   // データサイズ
    uint32_t command;    // コマンド
};

struct Packet
{
    PacketHeader    header;
    int8_t          data[4];// データ（可変）
};

//--------------------------------------------------------------------------------------------------
// Ping

enum PingState
{
    PingState_Normal = 0,
    PingState_Keeping = 1 << 0,
    PingState_Lock = 1 << 1,
    PingState_Unlock = 1 << 2,
    PingState_Closed = 1 << 3,
};

//! @brief ライブラリビルド時のプラットフォームのアドレスのビット数です。
enum PlatformAddressType
{
    PlatformAddressType_Address32 = 0x0,
    PlatformAddressType_Address64 = 0x1,
};

enum PingFlagMask
{
    PingFlagMask_None = 0x0,
    PingFlagMask_PlatformAddressType = 0x3, //!< 今のところ 32/64 だけだが念のため 2 ビット用意
};

NN_FORCEINLINE PlatformAddressType GetPlatformAddressType(uint32_t pingFlag) NN_NOEXCEPT
{
    return static_cast<PlatformAddressType>(pingFlag & PingFlagMask_PlatformAddressType);
}

struct PingBlock
{
    uint32_t uniqueID;
    uint32_t codePage;
    uint32_t state;
    uint32_t flag;
};

struct PingPacket
{
    PacketHeader    header;
    PingBlock       block;
};

//--------------------------------------------------------------------------------------------------
// アタッチ
enum AttachKind
{
    ATTACH_MODEL = 0x00,
    DETACH_MODEL = 0x01,
    ATTACH_SHADER_ARCHIVE = 0x02,
    DETACH_SHADER_ARCHIVE = 0x03,
    ATTACH_KIND_ERROR = 0xFF
};

enum AttachFlag
{
    ATTACH_SHADER_ARCHIVE_IS_BINARY = 0x01
};

struct AttachBlock
{
    uint32_t attachedKey;
    uint16_t attachKind;
    uint16_t flag;
    int8_t fileName[NN_G3D_EDIT_FILENAME_MAX];
    int8_t attachFileName[NN_G3D_EDIT_FILENAME_MAX];
};

struct AttachPacket
{
    PacketHeader    header;
    AttachBlock     block;
};

struct FileLoadedBlock
{
    uint32_t fileDataKind;
    uint32_t toolKey;
    uint32_t resFileKey;
    uint32_t modelObjKey;
    uint32_t resModelKey;
    uint32_t padding[3];
};

struct FileLoadedPacket
{
    PacketHeader    header;
    FileLoadedBlock block;
};

struct MaterialShaderInfoData
{
    int32_t shaderArchiveIndex;
    int8_t isOptimized;
    int8_t isOptimizationSkipped;
};

struct MaterialShaderInfoArrayData
{
    uint32_t numMaterial;
    MaterialShaderInfoData arrayData[1];// データ（可変）
};

struct FileInfoData
{
    struct OffsetFileInfo
    {
        uint32_t ofsFileName;
        uint32_t fileSize;
        uint16_t fileDataKind;
        uint16_t padding;
        uint32_t fileAlignment;
    };
    OffsetFileInfo fileInfo[1];// numFile分 nn::util::BinPtr が並びます
};

struct MultiFileDataBlock
{
    uint16_t numFile;
    uint16_t loopCount;
    uint32_t padding[2];
    nn::util::BinPtr ofsFileInfo;
};

struct ModelOptimizedShaderBlock
{
    uint32_t modelObjKey;
    nn::util::BinPtr ofsMaterialShaderIndices;
    MultiFileDataBlock multiFile;
};

struct ModelOptimizedShaderPacket
{
    PacketHeader header;
    ModelOptimizedShaderBlock block;
};

//--------------------------------------------------------------------------------------------------
// ファイルデータ

enum FileDataKind
{
    FILEDATA_MODEL =                0x00,
    FILEDATA_TEXTURE =              0x01,

    FILEDATA_SHADER_PARAM_ANIM =    0x02,
    FILEDATA_MAT_COLOR_ANIM =       0x03,
    FILEDATA_TEXTURE_SRT_ANIM =     0x04,
    FILEDATA_TEXTURE_PATTERN_ANIM = 0x05,
    FILEDATA_SKELETAL_ANIM =        0x06,
    FILEDATA_BONE_VISIBILITY_ANIM = 0x07,
    FILEDATA_MAT_VISIBILITY_ANIM =  0x08,
    FILEDATA_SHAPE_ANIM =           0x09,
    FILEDATA_SCENE_ANIM =           0x0A,
    FILEDATA_MATERIAL_ANIM =        0x0B,

    FILEDATA_SHADER_DEFINE =        0x11,
    FILEDATA_SHADER_ARCHIVE =       0x12,
    FILEDATA_SHADER_PROGRAM =       0x13,

    // ファイルデータではないが、メモリープールを持つため、遅延破棄の既存実装の都合上追加
    // TODO: 本来は別途 enum を定義して使う
    FILEDATA_EDIT_MODEL_OBJ =       0x14,

    FILEDATA_KIND_MAX,
    FILEDATA_KIND_ERROR =           0xFF
};

struct FileDataBlock
{
    uint32_t             key;
    uint32_t             resFileKey;
    uint32_t             kind;
    uint32_t             fileSize;
    uint32_t             pathSize;
    int32_t             shadingModelIndex;
    int32_t             shaderProgramIndex;
    uint32_t             fileAlignment;
    int8_t              fileName[NN_G3D_EDIT_FILENAME_MAX];
    int8_t              retargetingHostModelName[NN_G3D_EDIT_FILENAME_MAX];
};

struct FileDataPacket
{
    PacketHeader    header;
    FileDataBlock   block;
};

//--------------------------------------------------------------------------------------------------
// エディットデータ

enum EditValueKind
{
    BOOL_VALUE = 0x00,
    U32_VALUE,
    S32_VALUE,
    F32_VALUE,
    VECTOR4_VALUE,
    SAMPLER_VALUE,
    SAMPLER_TEXTURE_REF,
    SHADER_ARCHIVE_REF,
    TRANSFORM_VALUE,
    EDIT_VALUE_KIND_MAX
};

enum EditPlayPolicyKind
{
    EDIT_PLAY_POLICY_AUTO = 0x00,
    EDIT_PLAY_POLICY_ONCE,
    EDIT_PLAY_POLICY_LOOP
};

struct EditValueInfoBlock
{
    uint32_t key;
    uint16_t valueKind;
    uint16_t editTargetKind;
    uint32_t length;
    uint32_t indexCount;
};

struct EditValuePacket
{
    PacketHeader        header;
    EditValueInfoBlock  block;
};

struct EditSimpleValue
{
    union
    {
        union
        {
            uint32_t                       flag;
            uint32_t                         uValue;
            struct
            {
                uint16_t                     uHigh;
                uint16_t                     uLow;
            };
        };

        int32_t                             sValue;
        float                             fValue;
        bool                            bValue;
    };
};

struct EditValueBlock
{
    EditSimpleValue     value;
    int32_t                 index[1];// データ（可変）
};

struct EditVector3Value
{
    union
    {
        float fValue[3];
        int32_t sValue[3];
        uint32_t uValue[3];
    };
};

struct EditVector4Value
{
    union
    {
        float     fValue[4];
        int32_t     sValue[4];
        uint32_t     uValue[4];
    };
};

struct EditTransformValue
{
    int index;
    EditVector4Value rotate;
    EditVector3Value scale;
    EditVector3Value translate;
};

struct EditVector4ValueBlock
{
    EditVector4Value    value;
    int32_t                 index[1];// データ（可変）
};

struct EditTransformValueBlock
{
    EditTransformValue value[1]; // データ（可変）
};

struct EditMatrix44Value
{
    enum { DIM_MAJOR = 4, DIM_MINOR = 4, DIM = DIM_MAJOR * DIM_MINOR };

    union
    {
        struct
        {
            float m00, m01, m02, m03;
            float m10, m11, m12, m13;
            float m20, m21, m22, m23;
            float m30, m31, m32, m33;
        };
        float a[DIM];
        float m[DIM_MAJOR][DIM_MINOR];
    };
};

struct EditSamplerValue
{
    int32_t samplerIndex;

    union
    {
        nn::gfx::TextureAddressMode addressMode;
        nn::gfx::FilterMode fiterMode;
        nn::gfx::TextureBorderColorType textureBorderColorType;
        int maxAnisotoropy;
        float                 lod;
        float                 bias;
        uint32_t                 uValue;
        float                 fValue;
        int32_t                 sValue;
    };
};

struct EditSamplerValueBlock
{
    EditSamplerValue    value;
    int32_t                 index[1];// データ（可変）
};

struct EditSamplerTextureRef
{
    int32_t samplerIndex;
    int8_t  textureName[NN_G3D_EDIT_FILENAME_MAX];
};

struct EditShaderArchiveRef
{
    int8_t  shaderArchiveName[NN_G3D_EDIT_FILENAME_MAX];
    int8_t  shadingModelName[NN_G3D_EDIT_FILENAME_MAX];
};

struct EditShaderParamVectorValue
{
    EditVector4Value    value;
    int8_t                  paramName[NN_G3D_EDIT_FILENAME_MAX];
};

struct EditShaderParamVectorValueBlock
{
    EditShaderParamVectorValue  value;
    int32_t                         index[1];// データ（可変）
};

struct EditShaderParamMatrixValue
{
    EditMatrix44Value   value;
    int8_t                  paramName[NN_G3D_EDIT_FILENAME_MAX];
};

struct EditShaderParamMatrixValueBlock
{
    EditShaderParamMatrixValue  value;
    int32_t                         index[1];// データ（可変）
};

//--------------------------------------------------------------------------------------------------
// シェーディングモデル変更通知用データ

struct EditShadingModelValueBlock
{
    int32_t                 index[1];// データ（可変）
};

//--------------------------------------------------------------------------------------------------
// アニメーションエディットデータ

struct BindAnimInfo
{
    uint32_t modelKey;
    uint32_t attachModelFlag;
    uint32_t animationKeySize;
};

struct BindAnimInfoBlock
{
    BindAnimInfo info;
    uint32_t animationKeys[1];
};

struct BindAnimPacket
{
    PacketHeader        header;
    BindAnimInfoBlock   block;
};

struct AnimCurveInfo
{
    uint32_t animationKey;
    uint16_t animationKind;
    uint16_t padding;
};

struct AnimCurveValue
{
    union
    {
        int32_t index;
        int32_t materialIndex;
        int32_t vertexShapeIndex;
        int32_t cameraIndex;
        int32_t lightIndex;
        int32_t fogIndex;
        int32_t boneAnimIndex;
    };

    int32_t curveIndex;
    uint32_t curveSize;
};

struct AnimCurveBlock
{
    AnimCurveInfo               info;
    AnimCurveValue              value;
    uint32_t                         curveData[1];// ResAnimCurve のデータが入ります。
};

struct AnimCurvePacket
{
    PacketHeader    header;
    AnimCurveBlock  block;
};

struct AnimEditInfoBlock
{
    uint32_t animationKey;
    union
    {
        float fValue;
        uint32_t iValue;
    };
    uint32_t modelKey;
    uint32_t padding[1];
};

struct AnimEditInfoPacket
{
    PacketHeader        header;
    AnimEditInfoBlock   block;
};

//--------------------------------------------------------------------------------------------------
// アニメーションフレームコントロールデータ

struct FrameCtrlBlock
{
    float frame;
    float frameStep;
    union
    {
        uint32_t playPolicy;
        uint32_t modelKey;
    };
    float frameCount;
    float startFrame;
};

struct FrameCtrlPacket
{
    PacketHeader        header;
    FrameCtrlBlock      block;
};

//--------------------------------------------------------------------------------------------------
// 描画情報受信用データ

struct RenderInfoRecvBlock
{
    uint32_t modelKey;
    uint16_t numRenderInfo;
    uint16_t numMaterialIndex;

    nn::util::BinPtr ofsRenderInfoArray;
    nn::util::BinPtr ofsMaterialIndexArray;
};

struct RenderInfoRecvPacket
{
    PacketHeader        header;
    RenderInfoRecvBlock block;
};

//--------------------------------------------------------------------------------------------------
// 描画情報送信用データ

struct RenderInfoSendInfo
{
    uint32_t modelKey;
    int32_t materialIndex;
    uint32_t labelInfoNum;
    uint32_t padding;
};

struct RenderInfoLabelInfo
{
    uint32_t labelOffset;
    uint32_t renderInfoType;
    uint32_t valueNum;
    uint32_t valueOffset;

    uint32_t padding[2];

    union
    {
        // renderInfoType が STRING 型の時に使用します。
        struct
        {
            uint32_t itemNum;
            uint32_t itemOffset;
        };
        // renderInfoType が INT 型の時に使用します。
        struct
        {
            int32_t iMinValue;
            int32_t iMaxValue;
        };
        // renderInfoType が FLOAT 型の時に使用します。
        struct
        {
            float fMinValue;
            float fMaxValue;
        };
    };
};

struct RenderInfoChoiceInfo
{
    uint32_t choiceOffset;
    uint32_t aliasOffset;
    uint32_t aliasSize;
    uint32_t padding;
};

struct RenderInfoSendPacket
{
    PacketHeader        header;
    RenderInfoSendInfo  info;
};

//--------------------------------------------------------------------------------------------------
// 描画情報選択編集用データ
struct RenderInfoEditInfo
{
    uint32_t type;

    uint32_t modelKey;
    int32_t materialIndex;

    union
    {
        int32_t slotIndex;
        int32_t arraySize;
    };

    union
    {
        int32_t itemIndex;
        int32_t iValue;
        float fValue;
    };
    uint32_t labelOffset;
    uint32_t padding[2];
};

struct RenderInfoEditPacket
{
    PacketHeader        header;
    RenderInfoEditInfo  info;
};

//--------------------------------------------------------------------------------------------------
// 描画情報辞書更新用データ

struct RenderInfoUpdateBlock
{
    uint32_t modelKey;
    int32_t materialIndex;
    uint32_t renderInfoDicDataSize;
    uint32_t renderInfoArrayDataSize;
    uint32_t renderInfoData[4];// ResDic, ResRenderInfo 配列の実データが入ります。
};

struct RenderInfoUpdatePacket
{
    PacketHeader            header;
    RenderInfoUpdateBlock   block;
};

//--------------------------------------------------------------------------------------------------
// ボーンバインド編集用データ

struct BondBindEditBlock
{
    uint32_t parentModelKey;
    uint32_t parentBoneIndex;
    uint32_t childModelKey;
    uint16_t isBind;
    uint16_t padding;
};

struct BondBindEditPacket
{
    PacketHeader        header;
    BondBindEditBlock   block;
};

//--------------------------------------------------------------------------------------------------
// 配置情報受信用データ

struct ModelLayoutRecvBlock
{
    uint32_t modelKey;
    uint32_t padding;
};

struct ModelLayoutRecvPacket
{
    PacketHeader            header;
    ModelLayoutRecvBlock    block;
};

//--------------------------------------------------------------------------------------------------
// 配置情報編集用データ

struct ModelLayoutEditBlock
{
    uint32_t isBind;
    uint32_t modelKey;
    uint32_t padding;

    union
    {
        struct
        {
            float x, y, z;
        };
        float a[3];
    } scale;

    union
    {
        struct
        {
            float x, y, z;
        };
        float a[3];
    } rotate;

    union
    {
        struct
        {
            float x, y, z;
        };
        float a[3];
    } translate;
};

struct ModelLayoutEditPacket
{
    PacketHeader            header;
    ModelLayoutEditBlock    block;
};

//--------------------------------------------------------------------------------------------------
// シェイプ Lod level 受信用データ

struct ShapeLodLevelEditBlock
{
    uint32_t modelKey;
    int32_t lodLevel;
};

struct ShapeLodLevelEditPacket
{
    PacketHeader            header;
    ShapeLodLevelEditBlock  block;
};

//--------------------------------------------------------------------------------------------------
// 変更されたシェーダプログラム要求用データ

struct ShaderProgramSendInfo
{
    uint32_t shaderArchiveKey;
    int32_t shadingModelIndex;
    int32_t shaderProgramIndex;
    uint32_t optionInfoNum;
};

struct ShaderProgramOptionInfo
{
    uint32_t optionOffset;
    uint32_t choiceOffset;
};

struct ShaderProgramSendPacket
{
    PacketHeader            header;
    ShaderProgramSendInfo   info;
};

//--------------------------------------------------------------------------------------------------
struct TextureBindingBlock
{
    uint32_t bindTargetKey;
    int32_t textureKeyArrayCount;
    uint32_t textureKeyArrayData[1]; // バインド、アンバインドするテクスチャキーの配列が入ります
};

struct TextureBindingPacket
{
    PacketHeader        header;
    TextureBindingBlock block;
};

//--------------------------------------------------------------------------------------------------
// 3DEditorへ通知する選択要求用データ

struct PickupSendInfo
{
    int32_t materialPickupNum;
    uint32_t padding[3];
};

struct MaterialPickupInfo
{
    uint32_t modelObjKey;
    int32_t materialIndex;
};

struct PickupSendPacket
{
    PacketHeader            header;
    PickupSendInfo          info;
};

//--------------------------------------------------------------------------------------------------
// ランタイムのエラーコードです。
enum RuntimeErrorCode
{
    RUNTIME_ERROR_CODE_NO_ERROR = 0,
    RUNTIME_ERROR_CODE_INVALID_MATERIAL_COUNT = 1,
    RUNTIME_ERROR_CODE_ATTACH_CANCELED = 2,
    RUNTIME_ERROR_CODE_LOAD_FILE_FAILED = 3,
    RUNTIME_ERROR_CODE_INSUFFICIENT_MEMORY = 4,
    RUNTIME_ERROR_CODE_BIND_ANIM_FAILED = 5,
    RUNTIME_ERROR_CODE_RETARGET_HOST_MODEL_NOT_FOUND = 6,
    RUNTIME_ERROR_CODE_DUPLICATE_MODEL_OBJ_KEY = 7,
    RUNTIME_ERROR_CODE_INVALID_SHADER_DETECTED = 8,
    RUNTIME_ERROR_CODE_INVALID_MODEL_ATTACHED = 9,
    RUNTIME_ERROR_CODE_TARGET_MODEL_NOT_FOUND = 10,
    RUNTIME_ERROR_CODE_OPEN_FILE_FAILED = 11,
    RUNTIME_ERROR_CODE_INVALID_SHAPE_COUNT = 12,
    RUNTIME_ERROR_CODE_INVALID_MESH_COUNT = 13,
    RUNTIME_ERROR_CODE_INVALID_SUBMESH_COUNT = 14,
    RUNTIME_ERROR_CODE_UNKNOWN_ERROR = -1,
};

//--------------------------------------------------------------------------------------------------
// ランタイムから 3DEditor へのメッセージ通知用データ

enum MessageType
{
    MessageType_UserInfo = 0, //!< ユーザーの情報としてメッセージを表示します。
    MessageType_UserWarning = 1, //!< ユーザーの警告としてメッセージを表示します。
    MessageType_UserError = 2, //!< ユーザーのエラーとしてメッセージを表示します。
    MessageType_RuntimeInfo = 3, //!< ランタイムライブラリの情報としてメッセージを通知します。
    MessageType_RuntimeWarning = 4, //!< ランタイムライブラリの警告としてメッセージを通知します。
    MessageType_RuntimeError = 5, //!< ランタイムライブラリのエラーとしてメッセージを通知します。
};

struct MessageNotificationInfo
{
    int32_t messageType; // ViewerServer::SendUserMessageArg::MessageType の値を利用
    int32_t messageDestination; // ViewerServer::SendUserMessageArg::MessageDestination の値を利用
    int32_t messageDataSize;
    int32_t messageCodePage; // 指定しない場合は -1
    int8_t  message[1]; // 可変長
    int8_t  padding[3];
};

struct MessageNotificationPacket
{
    PacketHeader    header;
    MessageNotificationInfo info;
};

//--------------------------------------------------------------------------------------------------
// ユーザスクリプト用データ

// 今のところモデル以外に適用する公開インタフェースは存在しないが、
// 将来モデル以外にスクリプトを適用したい要望はありそうなので、パケットは汎用的に作っておく

//! @brief スクリプト適用対象の種類です。
enum UserScriptTargetKind
{
    USER_SCRIPT_TARGET_KIND_MODEL,                    //!< モデルです。
    USER_SCRIPT_TARGET_KIND_TEXTURE,                  //!< テクスチャです。
    USER_SCRIPT_TARGET_KIND_SKELTAL_ANIM,             //!< スケルタルアニメーションです。
    USER_SCRIPT_TARGET_KIND_SHADER_PARAM_ANIM,        //!< シェーダパラメータアニメーションです。
    USER_SCRIPT_TARGET_KIND_TEXTURE_PATTERN_ANIM,     //!< テクスチャパターンアニメーションです。
    USER_SCRIPT_TARGET_KIND_BONE_VISIBILITY_ANIM,     //!< ボーンビジビリティアニメーションです。
    USER_SCRIPT_TARGET_KIND_MATERIAL_VISIBILITY_ANIM, //!< マテリアルビジビリティアニメーションです。
    USER_SCRIPT_TARGET_KIND_SHAPE_ANIM,               //!< シェイプアニメーションです。
    USER_SCRIPT_TARGET_KIND_SCENE_ANIM,               //!< シーンアニメーションです。
    USER_SCRIPT_TARGET_KIND_SHADER,                   //!< シェーダです。
};

struct UserScriptBlock
{
    uint32_t targetModelObjKey;

    int32_t scriptTextLength;
    int32_t targetNameLength;
    int32_t selectedMaterialCount;
    int32_t selectedShapeCount;
    int32_t selectedBoneCount;

    int32_t targetKind; //!< UserScriptTargetKind

    // dynamicData は下記のようなバイト列になる
    // selectedBoneIndices[selectedBoneCount * sizeof(int32_t)],
    // selectedShapeIndices[selectedShapeCount * sizeof(int32_t)],
    // selectedMaterialIndices[selectedMaterialCount * sizeof(int32_t)]
    // scriptText[scriptTextLength + 1],
    // targetName[targetNameLength + 1],
    int8_t  dynamicData[1];
    int8_t  padding[3];
};

struct UserScriptPacket
{
    PacketHeader    header;
    UserScriptBlock block;
};

//--------------------------------------------------------------------------------------------------
// 引数用構造体

struct EditMaterialArg
{
    uint32_t     key;
    uint16_t     valueKind;
    uint16_t     editTargetKind;
    void*   value;
    uint32_t     indexCount;
    int32_t*    index;
};

struct BindAnimArg
{
    uint32_t     modelKey;
    uint32_t*    animationKeys;
    uint32_t     animationKeySize;
};

struct EditAnimCurveArg
{
    uint32_t     animationKey;
    uint16_t     animationKind;
    void*   value;
    void*   curveData;
    uint32_t     curveDataSize;
};

struct EditBoneArg
{
    uint32_t     modelKey;
    uint16_t     valueKind;
    uint16_t     editTargetKind;
    void*   value;
    uint32_t     indexCount;
    int32_t*    index;
};

//--------------------------------------------------------------------------------------------------
// コマンド定義

enum CommandCategory
{
    FILEDATA_CATEGORY =             0x00,
    SYSTEM_CATEGORY =               0x01,
    MATERIAL_CATEGORY =             0x02,
    ANIMATION_CATEGORY =            0x03,
    EDIT_CATEGORY =                 0x04,
    SHADER_CATEGORY =               0x05,
    BONE_CATEGORY =                 0x06,
    MODEL_CATEGORY =                0x07,
    MODEL_ANIMATION_CATEGORY =      0x08,
    SCENE_ANIMATION_CATEGORY =      0x09,
    PICK_CATEGORY =                 0x0A,
    OTHER_CATEGORY =                0x0B,
    TEXTURE_CATEGORY =              0x0C,
};

enum CommandCategoryFlag
{
    FILEDATA_CATEGORY_FLAG =            FILEDATA_CATEGORY << 8,
    SYSTEM_CATEGORY_FLAG =              SYSTEM_CATEGORY << 8,
    MATERIAL_CATEGORY_FLAG =            MATERIAL_CATEGORY << 8,
    ANIMATION_CATEGORY_FLAG =           ANIMATION_CATEGORY << 8,
    EDIT_CATEGORY_FLAG =                EDIT_CATEGORY << 8,
    SHADER_CATEGORY_FLAG =              SHADER_CATEGORY << 8,
    BONE_CATEGORY_FLAG =                BONE_CATEGORY << 8,
    MODEL_CATEGORY_FLAG =               MODEL_CATEGORY << 8,
    MODEL_ANIMATION_CATEGORY_FLAG =     MODEL_ANIMATION_CATEGORY << 8,
    SCENE_ANIMATION_CATEGORY_FLAG =     SCENE_ANIMATION_CATEGORY << 8,
    PICK_CATEGORY_FLAG =                PICK_CATEGORY << 8,
    OTHER_CATEGORY_FLAG =               OTHER_CATEGORY << 8,
    TEXTURE_CATEGORY_FLAG =             TEXTURE_CATEGORY << 8,
};

enum CommandFlag
{
    FILEDATA_LOAD_FILE_COMMAND_FLAG =           0x01 | FILEDATA_CATEGORY_FLAG,
    FILEDATA_UNLOAD_FILE_COMMAND_FLAG =         0x02 | FILEDATA_CATEGORY_FLAG,
    FILEDATA_RELOAD_FILE_COMMAND_FLAG =         0x03 | FILEDATA_CATEGORY_FLAG,
    FILEDATA_UNLOAD_ALL_COMMAND_FLAG =          0x04 | FILEDATA_CATEGORY_FLAG,

    SYSTEM_PING_RECV_COMMAND_FLAG =             0x01 | SYSTEM_CATEGORY_FLAG,
    SYSTEM_PING_SEND_COMMAND_FLAG =             0x02 | SYSTEM_CATEGORY_FLAG,
    SYSTEM_BEGIN_FREEZE_COMMAND_FLAG =          0x03 | SYSTEM_CATEGORY_FLAG,
    SYSTEM_END_FREEZE_COMMAND_FLAG =            0x04 | SYSTEM_CATEGORY_FLAG,
    SYSTEM_PACKET_VERSION_ERROR_COMMAND_FLAG =  0x05 | SYSTEM_CATEGORY_FLAG,
    SYSTEM_CODEPAGE_COMMAND_FLAG =              0x06 | SYSTEM_CATEGORY_FLAG,
    SYSTEM_BEGIN_FREEZE_NO_SYNC_COMMAND_FLAG =  0x07 | SYSTEM_CATEGORY_FLAG,
    SYSTEM_RUNTIME_LOG_COMMAND_FLAG =           0x08 | SYSTEM_CATEGORY_FLAG,

    SYSTEM_PLAY_FRAME_CTRL_COMMAND_FLAG =       0x10 | SYSTEM_CATEGORY_FLAG,
    SYSTEM_STOP_FRAME_CTRL_COMMAND_FLAG =       0x11 | SYSTEM_CATEGORY_FLAG,
    SYSTEM_SEND_FRAME_COMMAND_FLAG =            0x12 | SYSTEM_CATEGORY_FLAG,
    SYSTEM_SEND_FRAME_STEP_COMMAND_FLAG =       0x13 | SYSTEM_CATEGORY_FLAG,
    SYSTEM_SEND_MODEL_NEXT_ANIM_COMMAND_FLAG =  0x14 | SYSTEM_CATEGORY_FLAG,
    SYSTEM_SEND_MODEL_PREV_ANIM_COMMAND_FLAG =  0x15 | SYSTEM_CATEGORY_FLAG,

    SYSTEM_RUNTIME_STATE_NORMAL_COMMAND_FLAG =  0x20 | SYSTEM_CATEGORY_FLAG,

    SYSTEM_RUNTIME_ERROR_COMMAND_FLAG =         0x30 | SYSTEM_CATEGORY_FLAG,

    EDIT_MATERIAL_COMMAND_FLAG =                0x00 | MATERIAL_CATEGORY_FLAG,

    EDIT_SEND_ATTACH_COMMAND_FLAG =             0x00 | EDIT_CATEGORY_FLAG,
    EDIT_RECV_ATTACH_COMMAND_FLAG =             0x01 | EDIT_CATEGORY_FLAG,
    EDIT_SEND_DETACH_COMMAND_FLAG =             0x02 | EDIT_CATEGORY_FLAG,
    EDIT_FILE_LOADED_COMMAND_FLAG =             0x03 | EDIT_CATEGORY_FLAG,
    EDIT_FILE_RELOADED_COMMAND_FLAG =           0x04 | EDIT_CATEGORY_FLAG,
    EDIT_SEND_RENDER_INFO_COMMAND_FLAG =        0x10 | EDIT_CATEGORY_FLAG,
    EDIT_RECV_RENDER_INFO_COMMAND_FLAG =        0x11 | EDIT_CATEGORY_FLAG,
    EDIT_SELECT_EDIT_RENDER_INFO_COMMAND_FLAG = 0x12 | EDIT_CATEGORY_FLAG,
    EDIT_RENDER_INFO_ARRAY_SIZE_COMMAND_FLAG =  0x13 | EDIT_CATEGORY_FLAG,
    EDIT_UPDATE_RENDER_INFO_COMMAND_FLAG =      0x14 | EDIT_CATEGORY_FLAG,
    EDIT_SEND_MODIFIED_SHADER_COMMAND_FLAG =    0x15 | EDIT_CATEGORY_FLAG,
    EDIT_RECV_MODIFIED_SHADER_COMMAND_FLAG =    0x16 | EDIT_CATEGORY_FLAG,
    EDIT_LOAD_SHADER_ARCHIVE_COMMAND_FLAG =     0x17 | EDIT_CATEGORY_FLAG,
    EDIT_RESET_SHADER_ARCHIVE_COMMAND_FLAG =    0x18 | EDIT_CATEGORY_FLAG,

    ANIMATION_PLAY_FRAME_CTRL_COMMAND_FLAG =    0x00 | ANIMATION_CATEGORY_FLAG,
    ANIMATION_STOP_FRAME_CTRL_COMMAND_FLAG =    0x01 | ANIMATION_CATEGORY_FLAG,
    ANIMATION_PLAY_POLICY_COMMAND_FLAG =        0x02 | ANIMATION_CATEGORY_FLAG,
    ANIMATION_FRAME_STEP_COMMAND_FLAG =         0x03 | ANIMATION_CATEGORY_FLAG,
    ANIMATION_FRAME_COUNT_COMMAND_FLAG =        0x04 | ANIMATION_CATEGORY_FLAG,
    ANIMATION_START_FRAME_COMMAND_FLAG =        0x05 | ANIMATION_CATEGORY_FLAG,

    EDIT_SHADER_COMMAND_FLAG =                  0x00 | SHADER_CATEGORY_FLAG,

    EDIT_BONE_COMMAND_FLAG =                    0x00 | BONE_CATEGORY_FLAG,

    EDIT_MODEL_BONE_BIND_COMMAND_FLAG =         0x00 | MODEL_CATEGORY_FLAG,
    EDIT_MODEL_LAYOUT_COMMAND_FLAG =            0x01 | MODEL_CATEGORY_FLAG,
    EDIT_RECV_MODEL_LAYOUT_COMMAND_FLAG =       0x02 | MODEL_CATEGORY_FLAG,
    EDIT_SEND_MODEL_LAYOUT_COMMAND_FLAG =       0x03 | MODEL_CATEGORY_FLAG,
    EDIT_SET_SHAPE_LOD_LEVEL_COMMAND_FLAG =     0x04 | MODEL_CATEGORY_FLAG,
    EDIT_RESET_SHAPE_LOD_LEVEL_COMMAND_FLAG =   0x05 | MODEL_CATEGORY_FLAG,

    MODEL_ANIMATION_BIND_COMMAND_FLAG =         0x00 | MODEL_ANIMATION_CATEGORY_FLAG,
    MODEL_ANIMATION_UNBIND_COMMAND_FLAG =       0x01 | MODEL_ANIMATION_CATEGORY_FLAG,
    MODEL_ANIMATION_EDIT_CURVE_COMMAND_FLAG =   0x02 | MODEL_ANIMATION_CATEGORY_FLAG,
    MODEL_ANIMATION_PLAY_COMMAND_FLAG =         0x03 | MODEL_ANIMATION_CATEGORY_FLAG,
    MODEL_ANIMATION_STOP_COMMAND_FLAG =         0x04 | MODEL_ANIMATION_CATEGORY_FLAG,
    MODEL_ANIMATION_EDIT_RETARGET_HOST_MODEL_COMMAND_FLAG = 0x05 | MODEL_ANIMATION_CATEGORY_FLAG,
    MODEL_ANIMATION_EDIT_MIRRORING_ENABLED_COMMAND_FLAG =  0x06  | MODEL_ANIMATION_CATEGORY_FLAG,

    SCENE_ANIMATION_BIND_COMMAND_FLAG =         0x00 | SCENE_ANIMATION_CATEGORY_FLAG,
    SCENE_ANIMATION_UNBIND_COMMAND_FLAG =       0x01 | SCENE_ANIMATION_CATEGORY_FLAG,
    SCENE_ANIMATION_EDIT_CURVE_COMMAND_FLAG =   0x02 | SCENE_ANIMATION_CATEGORY_FLAG,

    PICK_TOOL_MATERIAL_COMMAND_FLAG =           0x00 | PICK_CATEGORY_FLAG,
    PICK_RUNTIME_MODEL_COMMAND_FLAG =           0x01 | PICK_CATEGORY_FLAG,
    PICK_RUNTIME_MATERIAL_COMMAND_FLAG =        0x02 | PICK_CATEGORY_FLAG,
    PICK_RUNTIME_BONE_COMMAND_FLAG =            0x03 | PICK_CATEGORY_FLAG,
    PICK_RUNTIME_SHAPE_COMMAND_FLAG =           0x04 | PICK_CATEGORY_FLAG,

    OTHER_EXECUTE_USER_SCRIPT_FLAG =            0x00 | OTHER_CATEGORY_FLAG,
    OTHER_USER_MESSAGE_COMMAND_FLAG =           0x01 | OTHER_CATEGORY_FLAG,

    TEXTURE_BIND_COMMAND_FLAG =                 0x00 | TEXTURE_CATEGORY_FLAG,
};

static const uint32_t COMMAND_CATEGORY_FLAG_MASK = 0xFF00;

enum EditTargetKind
{
    EDIT_TARGET_MATERIAL_VISIBILITY =                       0x00 | MATERIAL_CATEGORY_FLAG,
    EDIT_TARGET_MATERIAL_DISPLAYFACE =                      0x01 | MATERIAL_CATEGORY_FLAG,

    EDIT_TARGET_MATERIAL_RENDER_STATE_MODE =                0x10 | MATERIAL_CATEGORY_FLAG,
    EDIT_TARGET_MATERIAL_RENDER_STATE_BLEND_MODE =          0x11 | MATERIAL_CATEGORY_FLAG,

    EDIT_TARGET_MATERIAL_DEPTHTEST_ENABLE =                 0x12 | MATERIAL_CATEGORY_FLAG,
    EDIT_TARGET_MATERIAL_DEPTHTEST_WRITE_ENABLE =           0x13 | MATERIAL_CATEGORY_FLAG,
    EDIT_TARGET_MATERIAL_DEPTHTEST_FUNC =                   0x14 | MATERIAL_CATEGORY_FLAG,

    EDIT_TARGET_MATERIAL_ALPHATEST_ENABLE =                 0x15 | MATERIAL_CATEGORY_FLAG,
    EDIT_TARGET_MATERIAL_ALPHATEST_FUNC =                   0x16 | MATERIAL_CATEGORY_FLAG,
    EDIT_TARGET_MATERIAL_ALPHATEST_VALUE =                  0x17 | MATERIAL_CATEGORY_FLAG,

    EDIT_TARGET_MATERIAL_COLOR_COMBINE =                    0x20 | MATERIAL_CATEGORY_FLAG,
    EDIT_TARGET_MATERIAL_ALPHA_COMBINE =                    0x21 | MATERIAL_CATEGORY_FLAG,
    EDIT_TARGET_MATERIAL_COLOR_SRC_BLEND =                  0x22 | MATERIAL_CATEGORY_FLAG,
    EDIT_TARGET_MATERIAL_COLOR_DST_BLEND =                  0x23 | MATERIAL_CATEGORY_FLAG,
    EDIT_TARGET_MATERIAL_ALPHA_SRC_BLEND =                  0x24 | MATERIAL_CATEGORY_FLAG,
    EDIT_TARGET_MATERIAL_ALPHA_DST_BLEND =                  0x25 | MATERIAL_CATEGORY_FLAG,

    EDIT_TARGET_MATERIAL_CONSTANT_COLOR =                   0x26 | MATERIAL_CATEGORY_FLAG,
    EDIT_TARGET_MATERIAL_LOGIC_OP =                         0x27 | MATERIAL_CATEGORY_FLAG,

    EDIT_TARGET_MATERIAL_SAMPLER_NAME =                     0x40 | MATERIAL_CATEGORY_FLAG,
    EDIT_TARGET_MATERIAL_SAMPLER_TEXTURE_REF =              0x41 | MATERIAL_CATEGORY_FLAG,
    EDIT_TARGET_MATERIAL_SAMPLER_WRAP_U =                   0x42 | MATERIAL_CATEGORY_FLAG,
    EDIT_TARGET_MATERIAL_SAMPLER_WRAP_V =                   0x43 | MATERIAL_CATEGORY_FLAG,
    EDIT_TARGET_MATERIAL_SAMPLER_WRAP_W =                   0x44 | MATERIAL_CATEGORY_FLAG,
    EDIT_TARGET_MATERIAL_SAMPLER_MAG_FILTER =               0x45 | MATERIAL_CATEGORY_FLAG,
    EDIT_TARGET_MATERIAL_SAMPLER_MIN_FILTER =               0x46 | MATERIAL_CATEGORY_FLAG,
    EDIT_TARGET_MATERIAL_SAMPLER_MIP_FILTER =               0x47 | MATERIAL_CATEGORY_FLAG,
    EDIT_TARGET_MATERIAL_SAMPLER_MAX_ANISO  =               0x48 | MATERIAL_CATEGORY_FLAG,
    EDIT_TARGET_MATERIAL_SAMPLER_MIN_LOD  =                 0x49 | MATERIAL_CATEGORY_FLAG,
    EDIT_TARGET_MATERIAL_SAMPLER_MAX_LOD  =                 0x4A | MATERIAL_CATEGORY_FLAG,
    EDIT_TARGET_MATERIAL_SAMPLER_LOD_BIAS =                 0x4B | MATERIAL_CATEGORY_FLAG,

    EDIT_TARGET_MATERIAL_SHADER_PARAM_BOOL =                0x60 | MATERIAL_CATEGORY_FLAG,
    EDIT_TARGET_MATERIAL_SHADER_PARAM_BOOL2 =               0x61 | MATERIAL_CATEGORY_FLAG,
    EDIT_TARGET_MATERIAL_SHADER_PARAM_BOOL3 =               0x62 | MATERIAL_CATEGORY_FLAG,
    EDIT_TARGET_MATERIAL_SHADER_PARAM_BOOL4 =               0x63 | MATERIAL_CATEGORY_FLAG,

    EDIT_TARGET_MATERIAL_SHADER_PARAM_INT =                 0x64 | MATERIAL_CATEGORY_FLAG,
    EDIT_TARGET_MATERIAL_SHADER_PARAM_INT2 =                0x65 | MATERIAL_CATEGORY_FLAG,
    EDIT_TARGET_MATERIAL_SHADER_PARAM_INT3 =                0x66 | MATERIAL_CATEGORY_FLAG,
    EDIT_TARGET_MATERIAL_SHADER_PARAM_INT4 =                0x67 | MATERIAL_CATEGORY_FLAG,

    EDIT_TARGET_MATERIAL_SHADER_PARAM_UINT =                0x68 | MATERIAL_CATEGORY_FLAG,
    EDIT_TARGET_MATERIAL_SHADER_PARAM_UINT2 =               0x69 | MATERIAL_CATEGORY_FLAG,
    EDIT_TARGET_MATERIAL_SHADER_PARAM_UINT3 =               0x6A | MATERIAL_CATEGORY_FLAG,
    EDIT_TARGET_MATERIAL_SHADER_PARAM_UINT4 =               0x6B | MATERIAL_CATEGORY_FLAG,

    EDIT_TARGET_MATERIAL_SHADER_PARAM_FLOAT =               0x6C | MATERIAL_CATEGORY_FLAG,
    EDIT_TARGET_MATERIAL_SHADER_PARAM_FLOAT2 =              0x6D | MATERIAL_CATEGORY_FLAG,
    EDIT_TARGET_MATERIAL_SHADER_PARAM_FLOAT3 =              0x6E | MATERIAL_CATEGORY_FLAG,
    EDIT_TARGET_MATERIAL_SHADER_PARAM_FLOAT4 =              0x6F | MATERIAL_CATEGORY_FLAG,

    EDIT_TARGET_MATERIAL_SHADER_PARAM_FLOAT2x2 =            0x70 | MATERIAL_CATEGORY_FLAG,
    EDIT_TARGET_MATERIAL_SHADER_PARAM_FLOAT2x3 =            0x71 | MATERIAL_CATEGORY_FLAG,
    EDIT_TARGET_MATERIAL_SHADER_PARAM_FLOAT2x4 =            0x72 | MATERIAL_CATEGORY_FLAG,

    EDIT_TARGET_MATERIAL_SHADER_PARAM_FLOAT3x2 =            0x73 | MATERIAL_CATEGORY_FLAG,
    EDIT_TARGET_MATERIAL_SHADER_PARAM_FLOAT3x3 =            0x74 | MATERIAL_CATEGORY_FLAG,
    EDIT_TARGET_MATERIAL_SHADER_PARAM_FLOAT3x4 =            0x75 | MATERIAL_CATEGORY_FLAG,

    EDIT_TARGET_MATERIAL_SHADER_PARAM_FLOAT4x2 =            0x76 | MATERIAL_CATEGORY_FLAG,
    EDIT_TARGET_MATERIAL_SHADER_PARAM_FLOAT4x3 =            0x77 | MATERIAL_CATEGORY_FLAG,
    EDIT_TARGET_MATERIAL_SHADER_PARAM_FLOAT4x4 =            0x78 | MATERIAL_CATEGORY_FLAG,

    EDIT_TARGET_MATERIAL_SHADER_PARAM_SRT3D =               0x79 | MATERIAL_CATEGORY_FLAG,
    EDIT_TARGET_MATERIAL_SHADER_PARAM_SRT2D =               0x7A | MATERIAL_CATEGORY_FLAG,

    EDIT_TARGET_MATERIAL_SHADER_PARAM_TEXSRT_MAYA =         0x7B | MATERIAL_CATEGORY_FLAG,
    EDIT_TARGET_MATERIAL_SHADER_PARAM_TEXSRT_3DSMAX =       0x7C | MATERIAL_CATEGORY_FLAG,
    EDIT_TARGET_MATERIAL_SHADER_PARAM_TEXSRT_SOFTIMAGE =    0x7D | MATERIAL_CATEGORY_FLAG,

    EDIT_TARGET_MODEL_ANIMATION_SHADER_PARAM_CURVE =        0x00 | MODEL_ANIMATION_CATEGORY_FLAG,
    EDIT_TARGET_MODEL_ANIMATION_TEX_PATTERN_CURVE =         0x01 | MODEL_ANIMATION_CATEGORY_FLAG,
    EDIT_TARGET_MODEL_ANIMATION_BONE_VISIBILITY_CURVE =     0x02 | MODEL_ANIMATION_CATEGORY_FLAG,
    EDIT_TARGET_MODEL_ANIMATION_MAT_VISIBILITY_CURVE =      0x03 | MODEL_ANIMATION_CATEGORY_FLAG,
    EDIT_TARGET_MODEL_ANIMATION_SHAPE_CURVE =               0x04 | MODEL_ANIMATION_CATEGORY_FLAG,
    EDIT_TARGET_MODEL_ANIMATION_MATERIAL_CURVE =            0x05 | MODEL_ANIMATION_CATEGORY_FLAG,
    EDIT_TARGET_MODEL_ANIMATION_BONE_CURVE =                0x06 | MODEL_ANIMATION_CATEGORY_FLAG,

    EDIT_TARGET_UPDATE_SHADING_MODEL =                      0x00 | SHADER_CATEGORY_FLAG,

    EDIT_TARGET_BONE_VISIBILITY =                           0x00 | BONE_CATEGORY_FLAG,
    EDIT_TARGET_BONE_BILLBOARD =                            0x01 | BONE_CATEGORY_FLAG,
    EDIT_TARGET_BONE_TRANSFORM =                            0x02 | BONE_CATEGORY_FLAG,
    EDIT_TARGET_CAMERA_TRANSFORM =                          0x03 | BONE_CATEGORY_FLAG, // 適切なカテゴリがないのでボーンカテゴリにしておく

    EDIT_TARGET_SCENE_ANIMATION_CAMERA_CURVE =              0x00 | SCENE_ANIMATION_CATEGORY_FLAG,
    EDIT_TARGET_SCENE_ANIMATION_LIGHT_CURVE =               0x01 | SCENE_ANIMATION_CATEGORY_FLAG,
    EDIT_TARGET_SCENE_ANIMATION_FOG_CURVE =                 0x02 | SCENE_ANIMATION_CATEGORY_FLAG,

    EDIT_TARGET_KIND_MAX = 0xFFFF
};

#if defined( _MSC_VER )
#pragma warning(pop)
#endif

} // namespace detail

}}} // namespace nn::g3d::viewer


