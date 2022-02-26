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
#include <nn/ui2d/ui2d_Common.h>
#include <nn/ui2d/ui2d_DrawInfo.h>

#include <nn/util/util_MathTypes.h>

namespace nn
{
namespace ui2d
{

//----------------------------------------

//! @brief 内部用機能のため使用禁止です。
enum WindowFrameMode
{
    WindowFrameMode_Around,
    WindowFrameMode_Horizontal,
    WindowFrameMode_HorizontalNoContent,
    WindowFrameMode_MaxWindowFrameMode
};
NN_STATIC_ASSERT(WindowFrameMode_MaxWindowFrameMode <= 256);

//----------------------------------------

//! @brief 内部用機能のため使用禁止です。
struct WindowContent
{
    nn::util::Unorm8x4 vtxColors[VertexColor_MaxVertexColor];
    detail::TexCoordArray texCoordArray;
};

//----------------------------------------
// ウィンドウのフレームサイズをウィンドウクラス内で保持する構造体です。
struct WindowFrameSizeInternal
{
    uint16_t           left; //!< 左辺の枠の幅です。
    uint16_t           right; //!< 右辺の枠の幅です。
    uint16_t           top; //!< 上辺の枠の高さです。
    uint16_t           bottom; //!< 下辺の枠の高さです。
};

//----------------------------------------

// ウィンドウのコンテントの拡大量とフレームのサイズを持つ構造体です。

//! @brief 内部用機能のため使用禁止です。
struct WindowSize
{
    int16_t  left; // 左辺の拡大量です。固定少数(WindowInflationFixedPoint)です。
    int16_t  right; // 右辺の拡大量です。固定少数(WindowInflationFixedPoint)です。
    int16_t  top; // 上辺の拡大量です。固定少数(WindowInflationFixedPoint)です。
    int16_t  bottom; // 下辺の拡大量です。固定少数(WindowInflationFixedPoint)です。
    WindowFrameSizeInternal frameSize; // 枠のサイズです。
};

namespace detail
{

//----------------------------------------
struct Rect
{
    float x, y, w, h;

    const nn::util::Float2 Position() const
    {
        const nn::util::Float2 result = NN_UTIL_FLOAT_2_INITIALIZER(x, y);
        return result;
    }

    const nn::ui2d::Size Size() const
    {
        const nn::ui2d::Size result = {w, h};
        return result;
    }
};

//----------------------------------------
inline void
GetLTFrameSize(
    nn::util::Float2*             pPt,
    Size*                   pSize,
    const nn::util::Float2&       basePt,
    const Size&             winSize,
    const WindowFrameSize&  frameSize
)
{
    *pPt = basePt;

    pSize->width = winSize.width - frameSize.right;
    pSize->height = frameSize.top;
}

//----------------------------------------
inline void
GetRTFrameSize(
    nn::util::Float2*             pPt,
    Size*                   pSize,
    const nn::util::Float2&       basePt,
    const Size&             winSize,
    const WindowFrameSize&  frameSize
)
{
    pPt->v[0] = basePt.v[0] + winSize.width - frameSize.right;
    pPt->v[1] = basePt.v[1];

    pSize->width  = frameSize.right;
    pSize->height = winSize.height - frameSize.bottom;
}

//----------------------------------------
inline void
GetLBFrameSize(
    nn::util::Float2*             pPt,
    Size*                   pSize,
    const nn::util::Float2&       basePt,
    const Size&             winSize,
    const WindowFrameSize&  frameSize
)
{
    pPt->v[0] = basePt.v[0];
    pPt->v[1] = basePt.v[1] - frameSize.top;

    pSize->width  = frameSize.left;
    pSize->height = winSize.height - frameSize.top;
}

//----------------------------------------
inline void
GetRBFrameSize(
    nn::util::Float2*             pPt,
    Size*                   pSize,
    const nn::util::Float2&       basePt,
    const Size&             winSize,
    const WindowFrameSize&  frameSize
)
{
    pPt->v[0] = basePt.v[0] + frameSize.left;
    pPt->v[1] = basePt.v[1] - winSize.height + frameSize.bottom;

    pSize->width  = winSize.width - frameSize.left;
    pSize->height = frameSize.bottom;
}

//----------------------------------------
// フレームのテクスチャ座標変換を設定します。
inline int DecideFrameTransform(
    WindowFrame frame,
    TextureFlip flip,
    bool vertexColorEnabled)
{
    static const uint32_t  frameFlag[WindowFrame_MaxWindowFrame] =
    {
        detail::FrameSpecFlag_FrameLt, // WINDOWFRAME_LT
        detail::FrameSpecFlag_FrameRt, // WINDOWFRAME_RT
        detail::FrameSpecFlag_FrameLb, // WINDOWFRAME_LG
        detail::FrameSpecFlag_FrameRb, // WINDOWFRAME_RB
        detail::FrameSpecFlag_FrameL, // WINDOWFRAME_L
        detail::FrameSpecFlag_FrameR, // WINDOWFRAME_R
        detail::FrameSpecFlag_FrameT, // WINDOWFRAME_T
        detail::FrameSpecFlag_FrameB // WINDOWFRAME_B
    };

    static const uint32_t  flipFlag[TextureFlip_MaxTextureFlip] =
    {
        detail::FrameSpecFlag_Normal,       // TEXTUREFLIP_NONE
        detail::FrameSpecFlag_FlipHFlip,   // TEXTUREFLIP_FLIPH
        detail::FrameSpecFlag_FlipVFlip,   // TEXTUREFLIP_FLIPV
        detail::FrameSpecFlag_FlipR90,     // TEXTUREFLIP_ROTATE90
        detail::FrameSpecFlag_FlipR180,    // TEXTUREFLIP_ROTATE180
        detail::FrameSpecFlag_FlipR270     // TEXTUREFLIP_ROTATE270
    };

    uint32_t  flag = frameFlag[frame] | flipFlag[flip];

    if (!vertexColorEnabled) { flag |= detail::FrameSpecFlag_VertexEffectConstColor; }

    return static_cast<int>(flag);
}

//----------------------------------------
// フレームのテクスチャ座標変換を設定します。
inline int DecideFrameTransform(
    WindowFrame frame,
    TextureFlip flip)
{
    return DecideFrameTransform(frame, flip, false);
}

//----------------------------------------
// フレームのテクスチャ座標変換を設定します。
inline void SetupFrameTransform(
    const DrawInfo& drawInfo,
    Material& material,
    WindowFrame frame,
    TextureFlip flip,
    bool vertexColorEnabled)
{
    material.GetConstantBufferForVertexShader(drawInfo)->frameSpec = DecideFrameTransform(frame, flip, vertexColorEnabled);
}

//----------------------------------------
// フレームのテクスチャ座標変換を設定します。
inline void SetupFrameTransform(
    const DrawInfo& drawInfo,
    Material& material,
    WindowFrame frame,
    TextureFlip flip)
{
    SetupFrameTransform(drawInfo, material, frame, flip, false);
}


//----------------------------------------
// フレームのテクスチャ座標変換を取得します。
inline int DecideFrame4Transform(
    WindowFrame frame,
    TextureFlip flip,
    bool vertexColorEnabled)
{
    static const uint32_t  frameFlag[4] =
    {
        detail::FrameSpecFlag_Frame4Lt, // WINDOWFRAME_LT
        detail::FrameSpecFlag_Frame4Rt, // WINDOWFRAME_RT
        detail::FrameSpecFlag_Frame4Lb, // WINDOWFRAME_LG
        detail::FrameSpecFlag_Frame4Rb, // WINDOWFRAME_RB
    };

    static const uint32_t  flipFlag[TextureFlip_MaxTextureFlip] =
    {
        detail::FrameSpecFlag_Normal,       // TEXTUREFLIP_NONE
        detail::FrameSpecFlag_FlipHFlip,   // TEXTUREFLIP_FLIPH
        detail::FrameSpecFlag_FlipVFlip,   // TEXTUREFLIP_FLIPV
        detail::FrameSpecFlag_FlipR90,     // TEXTUREFLIP_ROTATE90
        detail::FrameSpecFlag_FlipR180,    // TEXTUREFLIP_ROTATE180
        detail::FrameSpecFlag_FlipR270     // TEXTUREFLIP_ROTATE270
    };

    uint32_t  flag = frameFlag[frame] | flipFlag[flip];

    if (!vertexColorEnabled) { flag |= detail::FrameSpecFlag_VertexEffectConstColor; }

    return static_cast<int>(flag);
}

//----------------------------------------
// フレームのテクスチャ座標変換を取得します。
inline int DecideFrame4Transform(
    WindowFrame frame,
    TextureFlip flip)
{
    return DecideFrame4Transform(frame, flip, false);
}



//----------------------------------------
// フレームのテクスチャ座標変換を設定します。
inline void SetupFrame4Transform(
    const DrawInfo& drawInfo,
    Material& material,
    WindowFrame frame,
    TextureFlip flip,
    bool vertexColorEnabled)
{
    material.GetConstantBufferForVertexShader(drawInfo)->frameSpec = DecideFrame4Transform(frame, flip, vertexColorEnabled);
}

//----------------------------------------
// フレームのテクスチャ座標変換を設定します。
inline void SetupFrame4Transform(
    const DrawInfo& drawInfo,
    Material& material,
    WindowFrame frame,
    TextureFlip flip)
{
    material.GetConstantBufferForVertexShader(drawInfo)->frameSpec = DecideFrame4Transform(frame, flip, false);
}



//----------------------------------------
// 水平ウィンドウのフレームのテクスチャ座標変換を取得します。
inline int DecideHorizontalFrameTransform(
    WindowFrame frame,
    TextureFlip flip,
    bool vertexColorEnabled)
{
    NN_SDK_ASSERT(frame < WindowFrame_LeftBottom, "out of bounds: frame[%d] < WindowFrame_LeftBottom", frame);

    static const uint32_t  frameFlag[2] =
    {
        detail::FrameSpecFlag_FrameHorizontalL,
        detail::FrameSpecFlag_FrameHorizontalR,
    };

    static const uint32_t  flipFlag[TextureFlip_MaxTextureFlip] =
    {
        detail::FrameSpecFlag_Normal,       // TEXTUREFLIP_NONE
        detail::FrameSpecFlag_FlipHFlip,   // TEXTUREFLIP_FLIPH
        detail::FrameSpecFlag_FlipVFlip,   // TEXTUREFLIP_FLIPV
        detail::FrameSpecFlag_FlipR90,     // TEXTUREFLIP_ROTATE90
        detail::FrameSpecFlag_FlipR180,    // TEXTUREFLIP_ROTATE180
        detail::FrameSpecFlag_FlipR270     // TEXTUREFLIP_ROTATE270
    };

    uint32_t  flag = frameFlag[frame] | flipFlag[flip];

    if (!vertexColorEnabled) { flag |= detail::FrameSpecFlag_VertexEffectConstColor; }

    return static_cast<int>(flag);
}

//----------------------------------------
// 水平ウィンドウのフレームのテクスチャ座標変換を取得します。
inline int DecideHorizontalFrameTransform(
    WindowFrame frame,
    TextureFlip flip)
{
    return DecideHorizontalFrameTransform(frame, flip, false);
}

//----------------------------------------
// 水平ウィンドウのフレームのテクスチャ座標変換を設定します。
inline void SetupHorizontalFrameTransform(
    const DrawInfo& drawInfo,
    Material& material,
    WindowFrame frame,
    TextureFlip flip,
    bool vertexColorEnabled)
{
    material.GetConstantBufferForVertexShader(drawInfo)->frameSpec = DecideHorizontalFrameTransform(frame, flip, vertexColorEnabled);
}

//----------------------------------------
// 水平ウィンドウのフレームのテクスチャ座標変換を設定します。
inline void SetupHorizontalFrameTransform(
    const DrawInfo& drawInfo,
    Material& material,
    WindowFrame frame,
    TextureFlip flip)
{
    SetupHorizontalFrameTransform(drawInfo, material, frame, flip, false);
}

//----------------------------------------
// @brief フレームのテクスチャ座標変換を取得します。
inline int DecideHorizontalNocontextFrameTransform(
    WindowFrame frame,
    TextureFlip flip,
    bool vertexColorEnabled)
{
    NN_SDK_ASSERT(frame < WindowFrame_LeftBottom, "out of bounds: frame[%d] < WindowFrame_LeftBottom", frame);

    static const uint32_t  frameFlag[WindowFrame_MaxWindowFrame] =
    {
        detail::FrameSpecFlag_FrameHorizontalNoContentL,
        detail::FrameSpecFlag_FrameHorizontalNoContentR,
    };

    static const uint32_t  flipFlag[TextureFlip_MaxTextureFlip] =
    {
        detail::FrameSpecFlag_Normal,       // TEXTUREFLIP_NONE
        detail::FrameSpecFlag_FlipHFlip,   // TEXTUREFLIP_FLIPH
        detail::FrameSpecFlag_FlipVFlip,   // TEXTUREFLIP_FLIPV
        detail::FrameSpecFlag_FlipR90,     // TEXTUREFLIP_ROTATE90
        detail::FrameSpecFlag_FlipR180,    // TEXTUREFLIP_ROTATE180
        detail::FrameSpecFlag_FlipR270     // TEXTUREFLIP_ROTATE270
    };

    uint32_t  flag = frameFlag[frame] | flipFlag[flip];

    if (!vertexColorEnabled) { flag |= detail::FrameSpecFlag_VertexEffectConstColor; }

    return static_cast<int>(flag);
}

//----------------------------------------
// @brief フレームのテクスチャ座標変換を取得します。
inline int DecideHorizontalNocontextFrameTransform(
    WindowFrame frame,
    TextureFlip flip)
{
    return DecideHorizontalNocontextFrameTransform(frame, flip, false);
}

//----------------------------------------
// @brief フレームのテクスチャ座標変換を設定します。
inline void SetupHorizontalNocontextFrameTransform(
    const DrawInfo& drawInfo,
    Material& material,
    WindowFrame frame,
    TextureFlip flip,
    bool vertexColorEnabled)
{
    material.GetConstantBufferForVertexShader(drawInfo)->frameSpec = DecideHorizontalNocontextFrameTransform(frame, flip, vertexColorEnabled);
}

//----------------------------------------
// @brief フレームのテクスチャ座標変換を設定します。
inline void SetupHorizontalNocontextFrameTransform(
    const DrawInfo& drawInfo,
    Material& material,
    WindowFrame frame,
    TextureFlip flip)
{
    SetupHorizontalNocontextFrameTransform(drawInfo, material, frame, flip, false);
}

//----------------------------------------
inline void
GetHorizontalLeftFrameSize(
    nn::util::Float2* pPt,
    Size* pSize,
    const nn::util::Float2& basePt,
    const Size& winSize,
    const WindowFrameSize& frameSize
)
{
    *pPt = basePt;

    pSize->width = frameSize.left;
    pSize->height = winSize.height;
}

//----------------------------------------
inline void
GetHorizontalRightFrameSize(
    nn::util::Float2* pPt,
    Size* pSize,
    const nn::util::Float2& basePt,
    const Size& winSize,
    const WindowFrameSize& frameSize
)
{
    pPt->v[0] = basePt.v[0] + winSize.width - frameSize.right;
    pPt->v[1] = basePt.v[1];

    pSize->width  = frameSize.right;
    pSize->height = winSize.height;
}

} // detail

} // namespace nn::ui2d
} // namespace nn
