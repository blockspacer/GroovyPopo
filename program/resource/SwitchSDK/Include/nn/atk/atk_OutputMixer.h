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

#include <nn/atk/atk_OutputReceiver.h>
#include <nn/atk/atk_EffectAux.h>
#include <nn/atk/atk_EffectBase.h>
#include <nn/atk/fnd/os/atkfnd_CriticalSection.h>

namespace nn {
namespace atk {

//! @internal
class OutputMixer : public OutputReceiver
{
private:
    typedef util::IntrusiveList< EffectBase, util::IntrusiveListMemberNodeTraits<EffectBase, &EffectBase::m_Link> > EffectList;
    typedef util::IntrusiveList< EffectAux, util::IntrusiveListMemberNodeTraits<EffectAux, &EffectAux::m_AuxLinkNode> > EffectAuxList;

public:
    //! @internal
    static const int RequiredAlignment = NN_ALIGNOF(EffectList);

    //! @internal
    void UpdateEffectAux() NN_NOEXCEPT;
    //! @internal
    void OnChangeOutputMode() NN_NOEXCEPT;
    //! @internal
    bool HasEffect(int bus) const NN_NOEXCEPT;

    //! @internal
    virtual void AppendEffectImpl(EffectBase* pEffect, int bus, void* buffer, size_t bufferSize) NN_NOEXCEPT;
    //! @internal
    virtual void AppendEffectImpl(EffectAux* pEffect, int bus, void* buffer, size_t bufferSize) NN_NOEXCEPT;
    //! @internal
    void RemoveEffectImpl(EffectBase* pEffect, int bus) NN_NOEXCEPT;
    //! @internal
    void RemoveEffectImpl(EffectAux* pEffect, int bus) NN_NOEXCEPT;
    //! @internal
    void ClearEffectImpl(int bus) NN_NOEXCEPT;

protected:
    //! @internal
    OutputMixer() NN_NOEXCEPT;
    //! @internal
    static size_t GetRequiredMemorySize(int bus, bool isEffectEnabled) NN_NOEXCEPT;
    //! @internal
    void Initialize(int bus, bool isEffectEnabled, void* buffer, size_t bufferSize) NN_NOEXCEPT;
    //! @internal
    void Finalize() NN_NOEXCEPT;
    //! @internal
    bool AppendEffect(EffectBase* pEffect, int bus, void* buffer, size_t bufferSize) NN_NOEXCEPT;
    //! @internal
    bool AppendEffect(EffectAux* pEffect, int bus, void* buffer, size_t bufferSize) NN_NOEXCEPT;
    //! @internal
    bool RemoveEffect(EffectBase* pEffect, int bus) NN_NOEXCEPT;
    //! @internal
    bool RemoveEffect(EffectAux* pEffect, int bus) NN_NOEXCEPT;
    //! @internal
    bool ClearEffect(int bus) NN_NOEXCEPT;
    //! @internal
    inline bool IsEffectEnabled() const NN_NOEXCEPT
    {
        return m_IsEffectEnabled;
    }

private:
    mutable detail::fnd::CriticalSection m_EffectListLock;
    EffectList* m_pEffectList;
    EffectAuxList* m_pEffectAuxList;
    bool m_IsEffectEnabled;
};

} // namesapace nn::atk
} // namesapace nn
