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

#include <nn/ui2d.h>
#if defined(NN_BUILD_CONFIG_OS_WIN)
#include <nn/ui2d/viewer/win/ui2d_DirResourceAccessorWin.h>
#else
#include <nn/ui2d/viewer/ui2d_FindableArcResourceAccessor.h>
#endif

namespace nn
{
namespace ui2d
{
namespace viewer
{

class AnimationManager
{
public:
    enum TargetMode
    {
        TargetMode_Pane,
        TargetMode_Group,
    };

    AnimationManager();

    void Setup(
        nn::gfx::Device* pDevice,
        nn::ui2d::Layout* layout,
#if defined(NN_BUILD_CONFIG_OS_WIN)
        viewer::DirResourceAccessor* resourceAccessor
#else
        viewer::FindableArcResourceAccessor* resourceAccessor
#endif
    );

    void Finalize();

    int  GetAnimationCount() const;

    int  GetCurrentAnimationNo() const;

    const char* GetCurrentAnimationName() const
    {
        return m_CurrentAnimationName;
    }

    TargetMode GetTargetMode() const
    {
        return m_TargetMode;
    }

    void SetTargetMode(nn::gfx::Device* pDevice, TargetMode mode);

    void SetCurrentAnimationNo(nn::gfx::Device* pDevice, int  no);

    int  GetCurrentTargetCount();

    int  GetCurrentTargetNo() const
    {
        return m_CurrentTargetNo;
    }

    const char* GetCurrentTargetName() const
    {
        return m_CurrentTargetName;
    }

    void SetCurrentTargetNo(nn::gfx::Device* pDevice, int  no);

    nn::ui2d::Animator* GetAnimator()
    {
        return m_Animator;
    }

    ///--- アニメーションのコントロール
    bool IsCurrentAnimationLoop() const;

    void StartAnimation();

    void StopAnimation();

    void ResetAnimation();

    bool IsAnimationPlaying() const;

    float GetCurrentAnimationFrame() const;

    void SetCurrentAnimationFrame(float frame);

    float GetAnimationFrameMax() const;

    int GetAnimationNoByTagName(const char* name) const;

private:

    void GetAnimTagNameFromAnimFileName(
        char* dst,
        const char* src);


    static const int    AnimFileNameMax = nn::ui2d::LayoutNameStrMax + nn::ui2d::AnimTagNameStrMax + 8;
    static const int    AnimCountMax = 1024;


    nn::ui2d::Layout* m_Layout;
#if defined(NN_BUILD_CONFIG_OS_WIN)
    viewer::DirResourceAccessor* m_ResourceAccessor;
#else
    viewer::FindableArcResourceAccessor* m_ResourceAccessor;
#endif

    struct AnimInfo
    {
        char name[AnimFileNameMax];
        int tagOrder;

        static int CompareAnimationInfo(const void *pLeft, const void *pRight)
        {
            return static_cast<const AnimInfo*>(pLeft)->tagOrder - static_cast<const AnimInfo*>(pRight)->tagOrder;
        }
    };

    int32_t  m_AnimationCount;
    int32_t  m_CurrentAnimationNo;
    char m_CurrentAnimationName[nn::ui2d::AnimTagNameStrMax + 1];
    TargetMode m_TargetMode;
    nn::ui2d::AnimResource m_CurrentAnimResource;
    int32_t  m_CurrentTargetNo;
    const char* m_CurrentTargetName;
    nn::ui2d::Animator* m_Animator;
    int32_t  m_LayoutNameLength;
    AnimInfo mAnimInfoArray[AnimCountMax];
    int32_t  mAnimInfoArrayCount;
};

} // namespace viewer
} // namespace ui2d
} // namespace nn

#endif // NN_UI2D_VIEWER_ENABLED
