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

#include <nn/font/font_Types.h>
#include <nn/font/font_Font.h>
#include <nn/font/font_GpuBuffer.h>
#include <nn/gfx.h>

namespace nn {
namespace font {

#if defined(_MSC_VER) && _MSC_VER >= 1500
#pragma warning(push)
#pragma warning(disable: 4200) // サイズ0の配列
#endif

class RectDrawer;

namespace detail
{

static const int    UseTextureCountMax = 8; //!< 一回の描画で扱うことの出来るテクスチャの最大数です。
static const int    VertexCountByLetter = 4; //!< 文字単位の頂点数です。

//----------------------------------------
enum BitFlags
{
    BitFlags_ColorBlackWhiteInterpolationEnabled  = (1 << 0),
    BitFlags_BorderEffectEnabled = (1 << 1),
};


//----------------------------------------
//! @brief テクスチャ利用情報構造体
struct TextureUseInfo
{
    const TextureObject* pTexObj;
    uint32_t  useCount;
    uint32_t  bitFlags;
};

//----------------------------------------
//! @brief 頂点構造体です。
struct Vertex
{
    nn::util::Float3   xyz;
    nn::util::Unorm8x4 color;
    nn::util::Float3   uvw; // w には シート番号が入ります。
    void Set(float x, float y, float z, const nn::util::Unorm8x4& c, float u, float v, float w)
    {
        xyz.v[0] = x;
        xyz.v[1] = y;
        xyz.v[2] = z;
        color = c;
        uvw.v[0] = u;
        uvw.v[1] = v;
        uvw.v[2] = w;
    }
};


//-------------------------------------------------------------------------
//! @brief GPU で通常文字描画を行うためのデータセットです。
//-------------------------------------------------------------------------
struct VertexShaderCharAttribute
{
    nn::util::Float4    posAndSize;
    nn::util::Float4    texCoord;
    nn::util::Unorm8x4  color[detail::TextColor_MaxTextColor];
    int32_t             sheetIndex;
    float               italicOffset;
    nn::util::Float4    translate;

    void Set(
        const float x,
        const float y,
        const float width,
        const float height,
        const nn::util::Float4& texCoord_,
        const nn::util::Unorm8x4& upperColor,
        const nn::util::Unorm8x4& lowerColor,
        const int32_t sheetIndex_,
        const float italicOffset_,
        const float rectangleTopXOffset);
};

//-------------------------------------------------------------------------
//! @brief GPU で座標変換付き文字描画を行うためのデータセットです。
//-------------------------------------------------------------------------
struct VertexShaderCharAttributeWithTransform
{
    nn::util::Float4    posAndSize;
    nn::util::Float4    texCoord;
    nn::util::Unorm8x4  color[detail::TextColor_MaxTextColor];
    int32_t             sheetIndex;
    float               italicOffset;
    nn::util::Float4    translate;
    nn::util::Float4    rotateMatrixAndCenterX;
    nn::util::Float4    rotateMatrixAndCenterY;

    void Set(
        const float x,
        const float y,
        const float width,
        const float height,
        const nn::util::Float4& texCoord_,
        const nn::util::Unorm8x4& upperColor,
        const nn::util::Unorm8x4& lowerColor,
        const int32_t sheetIndex_,
        const float italicOffset_,
        const float rectangleTopXOffset,
        const nn::util::Float4& rotateMatrixAndCenterX_,
        const nn::util::Float4& rotateMatrixAndCenterY_,
        const nn::util::Float4& translate_);
};


}   // namespace detail

//---------------------------------------------------------------------------
//! @brief 文字の属性を管理するクラスです。
//---------------------------------------------------------------------------
class CharAttribute
{
public:
    enum Flags
    {
        Flags_BorderEffect = (0x1 << 0)
    };

    nn::util::Float4 pos;
    nn::util::Unorm8x4 color[detail::TextColor_MaxTextColor];
    nn::util::Float4 tex;
private:
    uintptr_t m_pTexObjAndFlags;        //!< 1bit目にflags、それ以外にテクスチャオブジェクトへのポインタ
public:
    int16_t  italicOffset;
    uint8_t  sheetIndex;
    uint8_t  shadowAlpha;

    inline void SetTexObjAndFlags(const TextureObject* pTexObj, uint8_t  flags)
    {
        m_pTexObjAndFlags = reinterpret_cast<uintptr_t>(pTexObj) | static_cast<uintptr_t>(flags);
    }

    inline const TextureObject* GetTexObj() const
    {
        return reinterpret_cast<const TextureObject*>(m_pTexObjAndFlags & static_cast<uintptr_t>(~0x01));
    }

    inline uint8_t  GetFlags() const
    {
        return static_cast<uint8_t >(m_pTexObjAndFlags & static_cast<uintptr_t>(0x01));
    }
};

struct ShadowParameter;
class PerCharacterTransformInfo;

//---------------------------------------------------------------------------
//! @brief フォント影の設定内容です。
//---------------------------------------------------------------------------
struct ShadowParameter
{
    nn::util::Unorm8x4 shadowUpperColor; //!< 影の上端カラー値です。
    nn::util::Unorm8x4 shadowLowerColor; //!< 影の下端カラー値です。
    nn::util::Float2 shadowOffset; //!< 影のオフセット値です。
    nn::util::Float2 shadowScale; //!< 影のスケール値です。
    float shadowItalicOffset; //!< 影のイタリック傾き値です。
};

//---------------------------------------------------------------------------
//! @brief アニメーション情報です。
//---------------------------------------------------------------------------
class PerCharacterTransformInfo
{
public:
    PerCharacterTransformInfo()
    {
        Scale[0] = 1.0f;
        Scale[1] = 1.0f;
        for (int i = 0; i < 3; i++)
        {
            RotationCos[i] = 1.0f;
            RotationSin[i] = 0.0f;
            Translation[i] = 0.0f;
        }
        for (int i = 0; i < 4; i++)
        {
            LT[i] = 255;
            LB[i] = 255;
        }
    }

    float Scale[2]; //!< スケールです。
    float RotationCos[3]; //!< 回転の余弦成分です。
    float RotationSin[3]; //!< 回転の正弦成分です。
    float Translation[3]; //!< 平行移動です。
    uint8_t  LT[4]; //!< 上端カラーです。
    uint8_t  LB[4]; //!< 下端カラーです。
};

//---------------------------------------------------------------------------
//! @brief 定数バッファの生成に使われる追加の描画内容です。
//---------------------------------------------------------------------------
class ConstantBufferAdditionalContent
{
public:
    enum Flags
    {
        Flags_InvisibleBorder = 1 << 0,
        Flags_ShadowEnabled = 1 << 1,
        Flags_PerCharacterTransformEnabled = 1 << 2,
    };

    //! @brief 文字ごとのアニメーションの回転原点を表します。
    enum PerCharacterTransformCenter
    {
        PerCharacterTransformCenter_Center = 0, //!< 文字の中央
        PerCharacterTransformCenter_Bottom = 1, //!< 文字の下端
        PerCharacterTransformCenter_Count
    };

    //! コンストラクタです。
    ConstantBufferAdditionalContent()
    : m_pShadowParam(NULL)
    , m_pPerCharacterTransformInfos(NULL)
    , m_PerCharacterTransformCenter(PerCharacterTransformCenter_Center)
    , m_PerCharacterTransformCenterOffset(0.0f)
    , m_pViewMatrix(NULL)
    , m_pLocalMatrix(NULL)
    , m_InterpolateWhiteAlpha(std::numeric_limits<uint8_t>::max())
    , m_ShadowInterpolateWhiteAlpha(std::numeric_limits<uint8_t>::max())
    , m_Flags(0)
    {
        for (int i = 0; i < 4; i++)
        {
            m_InterpolateBlackColor.v[i] = 0.0f;
            m_InterpolateWhiteColor.v[i] = 1.0f;
            m_ShadowInterpolateBlackColor.v[i] = 0.0f;
            m_ShadowInterpolateWhiteColor.v[i] = 1.0f;
        }
    }

    //! @brief ビュー行列を設定します。
    //!
    //! @param[in] pViewMatrix ビュー行列へのポインタ
    //!
    //! @return オブジェクト自身への参照を返します。
    //!
    ConstantBufferAdditionalContent& SetViewMatrix(const nn::util::MatrixT4x3fType* pViewMatrix)
    {
        m_pViewMatrix = pViewMatrix;
        return *this;
    }

    //! @brief ローカル行列を設定します。
    //!
    //! @param[in] pLocalMatrix ローカル行列へのポインタ
    //!
    //! @return オブジェクト自身への参照を返します。
    //!
    ConstantBufferAdditionalContent& SetLocalMatrix(const nn::util::MatrixT4x3fType* pLocalMatrix)
    {
        m_pLocalMatrix = pLocalMatrix;
        return *this;
    }

    //! @brief 白黒補間の黒カラー値を設定します。
    //!
    //! @details
    //! color.v[3] の値は使われません。常に 0 として扱われます。
    //!
    //! @param[in] color 黒カラー値
    //!
    //! @return オブジェクト自身への参照を返します。
    //!
    ConstantBufferAdditionalContent& SetInterpolateBlack(const nn::util::Unorm8x4& color)
    {
        const float TO_FLOAT = 1.0f / std::numeric_limits<uint8_t>::max();

        m_InterpolateBlackColor.v[0] = static_cast<float>(color.v[0]) * TO_FLOAT;
        m_InterpolateBlackColor.v[1] = static_cast<float>(color.v[1]) * TO_FLOAT;
        m_InterpolateBlackColor.v[2] = static_cast<float>(color.v[2]) * TO_FLOAT;
        // 黒補完のアルファは使用しない。
        m_InterpolateBlackColor.v[3] = 0.0f;

        return *this;
    }

    //! @brief 白黒補間の白カラー値を設定します。
    //!
    //! @param[in] color 白カラー値
    //!
    //! @return オブジェクト自身への参照を返します。
    //!
    ConstantBufferAdditionalContent& SetInterpolateWhite(const nn::util::Unorm8x4& color)
    {
        const float TO_FLOAT = 1.0f / std::numeric_limits<uint8_t>::max();

        m_InterpolateWhiteColor.v[0] = static_cast<float>(color.v[0]) * TO_FLOAT;
        m_InterpolateWhiteColor.v[1] = static_cast<float>(color.v[1]) * TO_FLOAT;
        m_InterpolateWhiteColor.v[2] = static_cast<float>(color.v[2]) * TO_FLOAT;
        m_InterpolateWhiteColor.v[3] = static_cast<float>(color.v[3]) * TO_FLOAT;

        return *this;
    }

    //! @brief 白黒補間のアルファ値を設定します。
    //!
    //! @param[in] alpha アルファ値
    //!
    //! @return オブジェクト自身への参照を返します。
    //!
    ConstantBufferAdditionalContent& SetInterpolateAlpha(uint8_t  alpha)
    {
        m_InterpolateWhiteAlpha = alpha;

        return *this;
    }

    //! @brief 白黒補間の黒カラー値を設定します。
    //!
    //! @details
    //! color.v[3] の値は使われません。常に 0 として扱われます。
    //!
    //! @param[in] color 黒カラー値
    //!
    //! @return オブジェクト自身への参照を返します。
    //!
    ConstantBufferAdditionalContent& SetInterpolateBlack(const nn::util::Float4& color)
    {
        m_InterpolateBlackColor = color;
        // 黒補完のアルファは使用しない。
        m_InterpolateBlackColor.v[3] = 0.0f;

        return *this;
    }

    //! @brief 白黒補間の白カラー値を設定します。
    //!
    //! @param[in] color 白カラー値
    //!
    //! @return オブジェクト自身への参照を返します。
    //!
    ConstantBufferAdditionalContent& SetInterpolateWhite(const nn::util::Float4& color)
    {
        m_InterpolateWhiteColor = color;

        return *this;
    }

    //! @brief 影用の白黒補間の黒カラー値を設定します。
    //!
    //! @details
    //! color.v[3] の値は使われません。常に 0 として扱われます。
    //!
    //! @param[in] color 影用の黒カラー値
    //!
    //! @return オブジェクト自身への参照を返します。
    //!
    ConstantBufferAdditionalContent& SetShadowInterpolateBlack(const nn::util::Unorm8x4& color)
    {
        const float TO_FLOAT = 1.0f / std::numeric_limits<uint8_t>::max();

        m_ShadowInterpolateBlackColor.v[0] = static_cast<float>(color.v[0]) * TO_FLOAT;
        m_ShadowInterpolateBlackColor.v[1] = static_cast<float>(color.v[1]) * TO_FLOAT;
        m_ShadowInterpolateBlackColor.v[2] = static_cast<float>(color.v[2]) * TO_FLOAT;
        // 黒補完のアルファは使用しない。
        m_ShadowInterpolateBlackColor.v[3] = 0;

        return *this;
    }

    //! @brief 影用の白黒補間の白カラー値を設定します。
    //!
    //! @param[in] color 影用の白カラー値
    //!
    //! @return オブジェクト自身への参照を返します。
    //!
    ConstantBufferAdditionalContent& SetShadowInterpolateWhite(const nn::util::Unorm8x4& color)
    {
        const float TO_FLOAT = 1.0f / std::numeric_limits<uint8_t>::max();

        m_ShadowInterpolateWhiteColor.v[0] = static_cast<float>(color.v[0]) * TO_FLOAT;
        m_ShadowInterpolateWhiteColor.v[1] = static_cast<float>(color.v[1]) * TO_FLOAT;
        m_ShadowInterpolateWhiteColor.v[2] = static_cast<float>(color.v[2]) * TO_FLOAT;
        m_ShadowInterpolateWhiteColor.v[3] = static_cast<float>(color.v[3]) * TO_FLOAT;

        return *this;
    }

    //! @brief 影用の白黒補間のアルファ値を設定します。
    //!
    //! @param[in] alpha 影用のアルファ値
    //!
    //! @return オブジェクト自身への参照を返します。
    //!
    ConstantBufferAdditionalContent& SetShadowInterpolateAlpha(uint8_t  alpha)
    {
        m_ShadowInterpolateWhiteAlpha = alpha;

        return *this;
    }

    //! @brief 影用の白黒補間の黒カラー値を設定します。
    //!
    //! @details
    //! color.v[3] の値は使われません。常に 0 として扱われます。
    //!
    //! @param[in] color 影用の黒カラー値
    //!
    //! @return オブジェクト自身への参照を返します。
    //!
    ConstantBufferAdditionalContent& SetShadowInterpolateBlack(const nn::util::Float4& color)
    {
        m_ShadowInterpolateBlackColor = color;
        // 黒補完のアルファは使用しない。
        m_ShadowInterpolateBlackColor.v[3] = 0;

        return *this;
    }

    //! @brief 影用の白黒補間の白カラー値を設定します。
    //!
    //! @param[in] color 影用の白カラー値
    //!
    //! @return オブジェクト自身への参照を返します。
    //!
    ConstantBufferAdditionalContent& SetShadowInterpolateWhite(const nn::util::Float4& color)
    {
        m_ShadowInterpolateWhiteColor = color;

        return *this;
    }

    //! @brief 白黒補間の黒カラー値を取得します。
    //!
    //! @return 白黒補間の黒カラー値です。
    //!
    const nn::util::Unorm8x4 GetInterpolateBlack() const
    {
        nn::util::Unorm8x4 color = { {
            static_cast<uint8_t>(m_InterpolateBlackColor.v[0] * std::numeric_limits<uint8_t>::max()),
            static_cast<uint8_t>(m_InterpolateBlackColor.v[1] * std::numeric_limits<uint8_t>::max()),
            static_cast<uint8_t>(m_InterpolateBlackColor.v[2] * std::numeric_limits<uint8_t>::max()),
            static_cast<uint8_t>(m_InterpolateBlackColor.v[3] * std::numeric_limits<uint8_t>::max())
        } };

        return color;
    }

    //! @brief 白黒補間の白カラー値を取得します。
    //!
    //! @return 白黒補間の白カラー値です。
    //!
    const nn::util::Unorm8x4 GetInterpolateWhite() const
    {
        nn::util::Unorm8x4 color = { {
            static_cast<uint8_t>(m_InterpolateWhiteColor.v[0] * std::numeric_limits<uint8_t>::max()),
            static_cast<uint8_t>(m_InterpolateWhiteColor.v[1] * std::numeric_limits<uint8_t>::max()),
            static_cast<uint8_t>(m_InterpolateWhiteColor.v[2] * std::numeric_limits<uint8_t>::max()),
            static_cast<uint8_t>(m_InterpolateWhiteColor.v[3] * std::numeric_limits<uint8_t>::max())
        } };

        return color;
    }

    //! @brief 白黒補間のアルファ値を取得します。
    //!
    //! @return 白黒補間のアルファ値です。
    //!
    uint8_t GetInterpolateAlpha() const
    {
        return m_InterpolateWhiteAlpha;
    }

    //! @brief 白黒補間の黒カラー値を取得します。
    //!
    //! @return 白黒補間の黒カラー値です。
    //!
    const nn::util::Float4& GetInterpolateBlackFloat() const
    {
        return m_InterpolateBlackColor;
    }

    //! @brief 白黒補間の白カラー値を取得します。
    //!
    //! @return 白黒補間の白カラー値です。
    //!
    const nn::util::Float4& GetInterpolateWhiteFloat() const
    {
        return m_InterpolateWhiteColor;
    }

    //! @brief 影用の白黒補間の黒カラー値を取得します。
    //!
    //! @return 影用の白黒補間の黒カラー値です。
    //!
    const nn::util::Unorm8x4 GetShadowInterpolateBlack() const
    {
        nn::util::Unorm8x4 color = { {
            static_cast<uint8_t>(m_ShadowInterpolateBlackColor.v[0] * std::numeric_limits<uint8_t>::max()),
            static_cast<uint8_t>(m_ShadowInterpolateBlackColor.v[1] * std::numeric_limits<uint8_t>::max()),
            static_cast<uint8_t>(m_ShadowInterpolateBlackColor.v[2] * std::numeric_limits<uint8_t>::max()),
            static_cast<uint8_t>(m_ShadowInterpolateBlackColor.v[3] * std::numeric_limits<uint8_t>::max())
        } };

        return color;
    }

    //! @brief 影用の白黒補間の白カラー値を取得します。
    //!
    //! @return 影用の白黒補間の白カラー値です。
    //!
    const nn::util::Unorm8x4 GetShadowInterpolateWhite() const
    {
        nn::util::Unorm8x4 color = { {
            static_cast<uint8_t>(m_ShadowInterpolateWhiteColor.v[0] * std::numeric_limits<uint8_t>::max()),
            static_cast<uint8_t>(m_ShadowInterpolateWhiteColor.v[1] * std::numeric_limits<uint8_t>::max()),
            static_cast<uint8_t>(m_ShadowInterpolateWhiteColor.v[2] * std::numeric_limits<uint8_t>::max()),
            static_cast<uint8_t>(m_ShadowInterpolateWhiteColor.v[3] * std::numeric_limits<uint8_t>::max())
        } };

        return color;
    }

    //! @brief 影用の白黒補間のアルファ値を取得します。
    //!
    //! @return 影用の白黒補間のアルファ値です。
    //!
    uint8_t GetShadowInterpolateAlpha() const
    {
        return m_ShadowInterpolateWhiteAlpha;
    }

    //! @brief 影用の白黒補間の黒カラー値(float)を取得します。
    //!
    //! @return 影用の白黒補間の黒カラー値です。
    //!
    const nn::util::Float4& GetShadowInterpolateBlackFloat() const
    {
        return m_ShadowInterpolateBlackColor;
    }

    //! @brief 影用の白黒補間の白カラー値(float)を取得します。
    //!
    //! @return 影用の白黒補間の白カラー値です。
    //!
    const nn::util::Float4& GetShadowInterpolateWhiteFloat() const
    {
        return m_ShadowInterpolateWhiteColor;
    }

    //! @brief フチを描画するかを指定します。
    //!
    //! @param[in] visible フチを描画しない場合は false を指定
    //!
    //! @return オブジェクト自身への参照を返します。
    //!
    ConstantBufferAdditionalContent& SetBorderVisibility(bool visible)
    {
        if (visible)
        {
            m_Flags &= ~Flags_InvisibleBorder;
        }
        else
        {
            m_Flags |= Flags_InvisibleBorder;
        }

        return *this;
    }

    //! @brief 影のパラメータを設定します。
    //!
    //! @details
    //! 影を描画したいときに指定します。
    //!
    //! @param[in] pShadowParam 影のパラメータ
    //!
    //! @return オブジェクト自身への参照を返します。
    //!
    ConstantBufferAdditionalContent& SetShadowParam(const ShadowParameter* pShadowParam)
    {
        m_pShadowParam = pShadowParam;
        return *this;
    }

    //! @brief 文字ごとのアニメーション情報を設定します。
    //!
    //! @details
    //! 文字ごとのアニメーションをするときに指定します。
    //!
    //! @param[in] pPerCharacterTransformInfos 文字ごとのアニメーション情報
    //!
    //! @return オブジェクト自身への参照を返します。
    //!
    ConstantBufferAdditionalContent& SetPerCharacterTransformInfos(const PerCharacterTransformInfo* pPerCharacterTransformInfos)
    {
        m_pPerCharacterTransformInfos = pPerCharacterTransformInfos;
        return *this;
    }

    //! @brief 文字ごとのアニメーションの回転原点を設定します。
    //!
    //! @details
    //! 文字ごとのアニメーションをするときに指定します。
    //!
    //! @param[in] perCharacterTransformCenter 文字ごとのアニメーションの回転原点
    //!
    //! @return オブジェクト自身への参照を返します。
    //!
    ConstantBufferAdditionalContent& SetPerCharacterTransformCenter(PerCharacterTransformCenter perCharacterTransformCenter)
    {
        m_PerCharacterTransformCenter = perCharacterTransformCenter;
        return *this;
    }

    //! @brief 文字ごとのアニメーションの回転原点のオフセットを設定します。
    //!
    //! @details
    //! 文字ごとのアニメーションをするときに指定します。
    //!
    //! @param[in] perCharacterTransformCenterOffset 文字ごとのアニメーションの回転原点のオフセット
    //!
    //! @return オブジェクト自身への参照を返します。
    //!
    ConstantBufferAdditionalContent& SetPerCharacterTransformCenterOffset(float perCharacterTransformCenterOffset)
    {
        m_PerCharacterTransformCenterOffset = perCharacterTransformCenterOffset;
        return *this;
    }
private:
    const ShadowParameter* m_pShadowParam;
    const PerCharacterTransformInfo* m_pPerCharacterTransformInfos;
    PerCharacterTransformCenter m_PerCharacterTransformCenter;
    float m_PerCharacterTransformCenterOffset;

    const nn::util::MatrixT4x3fType* m_pViewMatrix; //!< ビューマトリックスです。
    const nn::util::MatrixT4x3fType* m_pLocalMatrix; //!< ローカルマトリクスです。
    nn::util::Float4    m_InterpolateBlackColor; //!< 黒補間のカラー値です。
    nn::util::Float4    m_InterpolateWhiteColor; //!< 白補間のカラー値です。
    uint8_t             m_InterpolateWhiteAlpha; //!< 白補間のアルファ値です。
    nn::util::Float4    m_ShadowInterpolateBlackColor; //!< 影用の黒補間のカラー値です。
    nn::util::Float4    m_ShadowInterpolateWhiteColor; //!< 影用の白補間のカラー値です。
    uint8_t             m_ShadowInterpolateWhiteAlpha; //!< 影用の白補間のアルファ値です。
    uint32_t m_Flags;

    friend class DispStringBuffer;
};

//---------------------------------------------------------------------------
//! @brief 文字列表示用バッファを管理するクラスです。
//---------------------------------------------------------------------------
class DispStringBuffer
{
private:
    struct ShaderParam
    {
        float m_Mtx[4][4];
        float m_InterpolateWidth[4];
        float m_InterpolateOffset[4];
        float m_ShadowInterpolateWidth[4];
        float m_ShadowInterpolateOffset[4];
    };

public:
    //! @brief Initialize() の引数オブジェクトです。
    class InitializeArg
    {
    public:
        //! @brief コンストラクタです。
        InitializeArg()
            : m_pDrawBuffer(NULL)
            , m_pConstantBuffer(NULL)
            , m_CharCountMax(0)
            , m_ShadowEnabled(false)
            , m_DoubleDrawnBorder(false)
            , m_PerCharacterTransformEnabled(false)
            , m_PerCharacterTransformAutoShadowAlpha(false)
        {}

        //! @brief 格納できる最大文字数を指定します。
        //!
        //! @param[in] value 最大文字数
        //!
        //! @return オブジェクト自身への参照を返します。
        //!
        InitializeArg& SetCharCountMax(int value)
        {
            m_CharCountMax = value;
            return *this;
        }

        //! @brief 影の描画を行うかを指定します。
        //!
        //! @param[in] value 影の描画を行う場合は true を指定
        //!
        //! @return オブジェクト自身への参照を返します。
        //!
        InitializeArg& SetShadowEnabled(bool value)
        {
            m_ShadowEnabled = value;
            return *this;
        }

        //! @brief 文字単位の変換機能を使用するかどうか指定します。
        //!
        //! @details
        //! 文字単位の変換機能を使用する場合、使用するコンスタントバッファのサイズが増加します。
        //!
        //! @param[in] value 文字単位の座標変換機能を使用する場合は true を指定します。
        //!
        //! @return オブジェクト自身への参照を返します。
        //!
        InitializeArg& SetPerCharacterTransformEnabled(bool value)
        {
            m_PerCharacterTransformEnabled = value;
            return *this;
        }

        //! @brief 文字毎の変換の影のアルファを文字のアルファに自動追従させるかどうか指定します。
        //!
        //! @param[in] value 影のアルファを文字のアルファに自動追従させる場合は true を指定します。
        //!
        //! @return オブジェクト自身への参照を返します。
        //!
        InitializeArg& SetPerCharacterTransformAutoShadowAlpha(bool value)
        {
            m_PerCharacterTransformAutoShadowAlpha = value;
            return *this;
        }

        //! @brief フチの２回描画を行うかを指定します。
        //!
        //! @param[in] value フチの２回描画を行う場合は true を指定
        //!
        //! @return オブジェクト自身への参照を返します。
        //!
        InitializeArg& SetDoubleDrawnBorder(bool value)
        {
            m_DoubleDrawnBorder = value;
            return *this;
        }

        //! @brief CPUが使用するメモリを指定します。
        //!
        //! @details
        //! 必要なメモリ量は DispStringBuffer::GetRequiredDrawBufferSize() で取得します。
        //!
        //! 必要なメモリ量が 0 のときはメモリを指定しないでください。
        //!
        //! @param[in] value CPU が使用するメモリ
        //!
        //! @return オブジェクト自身への参照を返します。
        //!
        InitializeArg& SetDrawBuffer(void* value)
        {
            m_pDrawBuffer = value;
            return *this;
        }

        //! @brief 文字列を描画するために使用する際にコンスタントバッファとして使用する GpuBuffer を指定します。
        //!
        //! @details
        //! 必要なメモリ量は DispStringBuffer::GetRequiredConstantBufferSize() で取得します。
        //!
        //! @param[in] pGpuBuffer コンスタントバッファとして使用する GpuBuffer です。
        //!
        //! @return オブジェクト自身への参照を返します。
        //!
        InitializeArg& SetConstantBuffer(GpuBuffer* pGpuBuffer)
        {
            m_pConstantBuffer = pGpuBuffer;
            return *this;
        }

    private:
        // アサーションを分かり易くするために一部のメンバーへのアクセサを定義します。
        int GetCharCountMax() const { return m_CharCountMax; }
        void* GetDrawBuffer() const { return m_pDrawBuffer; }

        void*       m_pDrawBuffer;
        GpuBuffer*  m_pConstantBuffer;
        int         m_CharCountMax;
        bool        m_ShadowEnabled;
        bool        m_DoubleDrawnBorder;
        bool        m_PerCharacterTransformEnabled;
        bool        m_PerCharacterTransformAutoShadowAlpha;

        friend class DispStringBuffer;
    };

    //! @brief コンストラクタです。
    DispStringBuffer();

    //! @brief デストラクタです。
    ~DispStringBuffer();

    //! @brief CPUが使用するメモリの必要量を取得します。
    //!
    //! @details
    //! この関数によって取得したサイズのメモリを確保し、InitializeArg::SetDrawBuffer() で指定します。
    //!
    //! @param[in] arg 初期化パラメータ
    //!
    //! @return メモリ量（バイト数）を返します。
    //!
    static size_t GetRequiredDrawBufferSize(const InitializeArg& arg);

    //! @brief コンスタントバッファで使用するメモリの必要量を取得します。
    //!
    //! @param[in] pDevice デバイス
    //! @param[in] arg 初期化パラメータ
    //!
    //! @return メモリ量（バイト数）を返します。
    //!
    static size_t GetRequiredConstantBufferSize(nn::gfx::Device* pDevice, const InitializeArg& arg);

    //! @brief 初期化を行います。
    //!
    //! @param[in] pDevice デバイス
    //! @param[in] arg 初期化パラメータ
    //!
    //! @return
    //! 初期化に成功したときは true を返します。
    //!
    bool Initialize(nn::gfx::Device* pDevice, const InitializeArg& arg);

    //! @brief 初期化が済んでいるか調べます。
    //!
    //! @return
    //! 初期化済みのときは true を返します。
    //!
    bool IsInitialized() const { return m_CharCountMax > 0; }

    //! @brief 終了処理を行います。
    //!
    //! @details
    //! Initialize() で渡したメモリの解放は行われません。ユーザ自身での解放が必要です。
    //!
    //! @param[in] pDevice デバイス
    //!
    void Finalize(nn::gfx::Device* pDevice);

    //! @brief 格納可能な文字数を取得します。
    int  GetCharCountMax() const
    {
        return m_CharCountMax;
    }

    //! @brief 格納した文字数を取得します。
    int  GetCharCount() const
    {
        return m_CharCount;
    }

    //! @brief 格納した文字数を設定します。
    void SetCharCount(int  count)
    {
        m_CharCount = count;
    }

    //! @brief 文字属性配列を取得します。
    CharAttribute* GetCharAttributes() const
    {
        return m_pCharAttrs;
    }

    //! @brief 文字属性配列を設定します。
    void SetCharAttributes(CharAttribute* pAttrs)
    {
        m_pCharAttrs = pAttrs;
    }

    //! @brief 初期化時に指定したdrawBufferを取得します。
    //!
    //! @return アドレスを返します。
    //!
    void* GetDrawBuffer() const
    {
        return m_pCharAttrs;
    }

    //! @brief コンスタントバッファとして使用する GpuBuffer を設定します。
    //!
    void SetConstantBuffer(GpuBuffer* pGpuBuffer);

    //! @brief コンスタントバッファから GpuAddress を取得します。
    //!
    void GetConstantBufferGpuAddress(nn::gfx::GpuAddress* pGpuAddr) const;

    //! @brief 定数バッファの内容を作成します。
    //!
    //! @param[in] projectionMtx プロジェクション行列
    //! @param[in] pAdditionalContent 追加の描画内容
    //! @param[in] isDrawFromRightToLeftEnabled 右側の文字から描画するかどうか
    //! @param[in] isOriginToCenterEnabled 各文字の原点をテキストボックスの中心にするかどうか
    //!
    void BuildConstantBuffer(
        const nn::util::MatrixT4x4fType& projectionMtx,
        const ConstantBufferAdditionalContent* pAdditionalContent,
        bool isDrawFromRightToLeftEnabled,
        bool isOriginToCenterEnabled);

    //! @brief 定数バッファの内容を作成します。
    //!
    //! @param[in] projectionMtx プロジェクション行列
    //! @param[in] pAdditionalContent 追加の描画内容
    //! @param[in] isDrawFromRightToLeftEnabled 右側の文字から描画するかどうか
    //!
    void BuildConstantBuffer(
        const nn::util::MatrixT4x4fType& projectionMtx,
        const ConstantBufferAdditionalContent* pAdditionalContent,
        bool isDrawFromRightToLeftEnabled)
    {
        BuildConstantBuffer(projectionMtx, pAdditionalContent, isDrawFromRightToLeftEnabled, false);
    }

    //! @brief 定数バッファの内容を作成します。
    //!
    //! @param[in] projectionMtx プロジェクション行列
    //! @param[in] pAdditionalContent 追加の描画内容
    //!
    void BuildConstantBuffer(
        const nn::util::MatrixT4x4fType& projectionMtx,
        const ConstantBufferAdditionalContent* pAdditionalContent)
    {
        BuildConstantBuffer(projectionMtx, pAdditionalContent, false);
    }

    //! @brief 定数バッファの内容を作成します。
    //!
    //! @param[in] projectionMtx プロジェクション行列
    //!
    void BuildConstantBuffer(
        const nn::util::MatrixT4x4fType& projectionMtx)
    {
        BuildConstantBuffer(projectionMtx, NULL);
    }

    // フォントの高さを設定します。

    //! @brief 内部用機能のため使用禁止です。
    //!
    //! @param[in] fontHeight フォントの高さです。
    //!
    void SetFontHeight(float fontHeight);

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
    bool CompareCopiedInstanceTest(const DispStringBuffer& target) const;

    //! @brief 特殊フチ形式のフチを 2 回に分けて描くかどうかを設定します。
    //!
    //! @details
    //! nn::font::DispStringBuffer::InitializeArg::SetDoubleDrawnBorder(true) に設定していなかった
    //! DispStringBUffer に対して SetDoubleDrawnBorderEnabled を true にした場合、
    //! コンスタントバッファの消費サイズが増えるため、
    //! あらかじめ nn::font::DispStringBuffer::GetRequiredConstantBufferSize で取得した領域を
    //! オーバーすることがあります。この問題を防ぐために、SetDoubleDrawnBorderEnabled を
    //! 切り替える DispStringBuffer にはあらかじめ
    //! nn::font::DispStringBuffer::InitializeArg::SetDoubleDrawnBorder(true) に設定して初期化してください。
    //!
    //! @param[in] isDoubleDrawnBorderEnabled 特殊フチ形式のフチを 2 回に分けて描くかどうかです。
    //!
    void SetDoubleDrawnBorderEnabled(bool isDoubleDrawnBorderEnabled)
    {
        m_DoubleDrawnBorder = isDoubleDrawnBorderEnabled;
    }

    //! @brief 特殊フチ形式のフチを 2 回に分けて描くかどうかを取得します。
    //!
    //! @return 特殊フチ形式のフチを 2 回に分けて描くかどうかです。
    //!
    bool IsDoubleDrawnBorderEnabled() const
    {
        return m_DoubleDrawnBorder;
    }

protected:
    //! @brief        テクスチャ使用情報を構築します。
    //!
    //! @param[in]    shadowEnabled 影が有効な場合に true とします。
    //!
    void                BuildTextureUseInfos(bool shadowEnabled);

private:

    void BuildCommonConstantBufferData(
        DispStringBuffer::ShaderParam&  shaderParam,
        const nn::util::MatrixT4x4fType& projectionMtx,
        const ConstantBufferAdditionalContent& content) const;

    void BuildPerCharacterAttributeConstantBuffer(
        const ConstantBufferAdditionalContent& content,
        bool isDrawFromRightToLeftEnabled,
        bool isOriginToCenterEnabled);

    // コンスタントバッファの書き込む関数
    void BuildPerCharacterParams(
        ptrdiff_t   bufferOffset,
        const PerCharacterTransformInfo* pPerCharacterTransformInfos,
        const ConstantBufferAdditionalContent::PerCharacterTransformCenter perCharacterTransformCenter,
        const float perCharacterTransformCenterOffset,
        const ShadowParameter*  pShadowParam,
        bool isDrawFromRightToLeftEnabled,
        bool isOriginToCenterEnabled);

    // 文字ごとに影をコンスタントバッファに書き込む関数
    void BuildShadowBufferPerCharacterParams(
        ptrdiff_t   bufferOffset,
        const PerCharacterTransformInfo* pPerCharacterTransformInfos,
        const ConstantBufferAdditionalContent::PerCharacterTransformCenter perCharacterTransformCenter,
        const float perCharacterTransformCenterOffset,
        const ShadowParameter*  pShadowParam,
        const uint32_t* pConstantBufferOffsetCharCount,
        nn::util::BytePtr constantBuffer,
        uint32_t offsetScale,
        uint32_t* pTexCharCount,
        int charIndex,
        bool isOriginToCenterEnabled);

    // 文字ごとに文字をコンスタントバッファに書き込む関数
    void BuildCharacterBufferPerCharacterParams(
        ptrdiff_t   bufferOffset,
        const PerCharacterTransformInfo* pPerCharacterTransformInfos,
        const ConstantBufferAdditionalContent::PerCharacterTransformCenter perCharacterTransformCenter,
        const float perCharacterTransformCenterOffset,
        const uint32_t* pConstantBufferOffsetCharCount,
        nn::util::BytePtr constantBuffer,
        uint32_t offsetScale,
        uint32_t* pTexCharCount,
        int charIndex,
        bool isOriginToCenterEnabled);

    void CalculatePerCharacterTransform(
        nn::util::Float4& outRotateMatrixAndCenterX,
        nn::util::Float4& outRotateMatrixAndCenterY,
        nn::util::Float4& outTranslate,
        const float x,
        const float y,
        const float width,
        const float height,
        const float italicOffset,
        const float rectangleTopXOffset,
        const PerCharacterTransformInfo& perCharacterTransformInfo,
        const ConstantBufferAdditionalContent::PerCharacterTransformCenter perCharacterTransformCenter,
        const float perCharacterTransformCenterOffset,
        bool isOriginToCenterEnabled,
        float originToCenterShadowOffset) const;

    class VertexBufferData
    {
    public:// TODO:アクセサ
        detail::TextureUseInfo textureUseInfos[detail::UseTextureCountMax];
        int textureUseInfoPos; //!< 使用数
    };

    uint32_t  m_ConstantBufferOffset;     //! コンスタントバッファの GpuBuffer 上での先頭ポインタからのオフセット
    uint32_t  m_PerCharacterParamOffset;

    uint32_t            m_DrawContentFlags; //! DrawContent で渡されたフラグの一時保管領域
    VertexBufferData    m_VertexBufferData;

    int32_t             m_CharCountMax;         //!< 格納可能な文字数
    int32_t             m_CharCount;            //!< 格納した文字数
    CharAttribute*      m_pCharAttrs;       //!< 頂点属性配列
    uint8_t*            m_pCharUseTextureIndices;  //!< 文字ごとに使用するテクスチャのインデックス(VertexBufferData::TextureUseInfo)
    GpuBuffer*          m_pConstantBuffer;
    bool                m_ShadowEnabled;
    bool                m_DoubleDrawnBorder;    //!< フチの２回描画を行うかどうか
    bool                m_PerCharacterTransformEnabled;    //!< 文字単位の座標変換機能を使用するかどうか
    bool                m_PerCharacterTransformAutoShadowAlpha; //!< 文字毎の変換の影のアルファを文字のアルファに自動追従させるかどうか
    float               m_FontHeight; //!< フォントの高さ

    friend class RectDrawer;
};

#if defined(_MSC_VER) && _MSC_VER >= 1500
#pragma warning(pop)
#endif

}   // namespace font
}   // namespace nn
