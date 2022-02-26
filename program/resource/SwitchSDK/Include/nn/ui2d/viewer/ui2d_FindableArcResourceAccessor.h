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

#include <nn/ui2d/ui2d_ArcResourceAccessor.h>

namespace nn
{
namespace ui2d
{
namespace viewer
{

class FindableArcResourceAccessor : public nn::ui2d::ArcResourceAccessor, public IFindableResourceAccessor
{
public:
    FindableArcResourceAccessor() : nn::ui2d::ArcResourceAccessor() {}

    bool Attach(void* archiveStart, const char* resourceRootDirectory);

    // 指定したリソースタイプのファイル名を得る。
    // DirResourceAccessorのインターフェースと合わせるため、現状クラス内のバッファのアドレスを
    // 返しています。このメソッドから返された領域は、次のFindFileの呼び出し時に破壊されます。
    virtual const char* FindFile(nn::ui2d::ResType resType, const char* pPrevFileName);

    virtual void* FindData(size_t*  pSize, ResType resType, const char* pName)
    {
        return FindResourceByName(pSize, resType, pName);
    }

private:

    static const int    FileNameMax = 128;

    char m_FileNameBuffer[FileNameMax];
    nn::ui2d::ArchiveHandle m_FindArcHandle;

};

} // namespace viewer
} // namespace ui2d
} // namespace nn

#endif // NN_UI2D_VIEWER_ENABLED
