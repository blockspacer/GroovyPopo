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

#include <nn/ui2d/viewer/ui2d_Config.h>
#if defined(NN_UI2D_VIEWER_ENABLED)

#include <cstddef>
#include <string>
#include <nn/util/util_IntrusiveList.h>

#include <nn/ui2d/ui2d_ResourceAccessor.h>

namespace nn
{
namespace ui2d
{
namespace viewer
{

class DirResourceAccessor : public nn::ui2d::ResourceAccessor, public IFindableResourceAccessor
{
public:
    //! 実行時型情報です。
    NN_UI2D_RUNTIME_TYPEINFO(nn::ui2d::ResourceAccessor);

    static DirResourceAccessor* Create(const char* pBasePath = "");

    // 指定したパス以下のリソースをアクセサに関連付けます。
    virtual void Attach(const char* resPath) = 0;

    // アクセサに関連づけられたリソースを破棄します。
    virtual void Detach() = 0;

    // リソースの関連づけが行われているか調べます。
    virtual bool IsAttached() const = 0;

    // 関連付けられたリソースのパスを取得します。
    virtual const char* GetResourceBasePath() const = 0;

    // 指定したリソースタイプのファイル名を得る。
    virtual const char* FindFile(
        nn::ui2d::ResType resType,
        const char* pPrevFileName) = 0;

    virtual void* FindData(size_t*  pSize, ResType resType, const char* pName)
    {
        return FindResourceByName(pSize, resType, pName);
    }

    //! @brief フォントを登録します。
    //!
    //! @param[in] name フォント名です。
    //! @param[in] pFont フォントです。
    //!
    virtual void RegisterFont(const char* name, nn::font::Font* pFont) = 0;
};

} // namespace viewer
} // namespace ui2d
} // namespace nn

#endif // NN_UI2D_VIEWER_ENABLED
