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

#include <nn/nn_Common.h>
#include <nn/util/util_TypedStorage.h>
#include "gfx_TransientDescriptorAllocator.h"

namespace nn{ namespace gfx{ namespace util{

    //! @brief TransientDescriptorAllocator とその初期化に必要なメモリブロックを包含してオブジェクトの構築を行うクラスです。
    //! @tparam THistoryLengthMax 構築する TransientDescriptorAllocator が記録する履歴の最大数です。
    //! @see TransientDescriptorAllocator
    template<int THistoryLengthMax>
    class TransientDescriptorAllocatorHolder
    {
        NN_DISALLOW_COPY(TransientDescriptorAllocatorHolder);

    public:

        //! @brief 構築する TransientDescriptorAllocator に設定される履歴の最大数です。
        static const int HistoryLengthMax = THistoryLengthMax;

    public:

        //! @brief デフォルトコンストラクタです
        TransientDescriptorAllocatorHolder() NN_NOEXCEPT
        {
        }

        //! @brief このオブジェクトと包含する TransientDescriptorAllocator を初期化します。
        //! @param[in] pDescriptorPool デスクリプタプールです。
        //! @param[in] baseSlotIndex   使用するデスクリプタスロットの先頭のインデックスです。
        //! @param[in] slotCount       使用するデスクリプタスロットの数です。
        //! @pre このオブジェクトは未初期化でなければなりません。
        //! @pre 各パラメータの事前条件は TransientDescriptorAllocator::Initialize() と同一です。
        //! @post このオブジェクトは初期化済になります。
        //! @see TransientDescriptorAllocator::Initialize()
        //! @details
        //! 指定したパラメータを用いてこのオブジェクトが包含する TransientDescriptorAllocator のインスタンスを初期化します。
        //! 初期化に使用するメモリブロックにはこのオブジェクトが包含しているメモリ領域を使用します。
        //! 履歴の最大数には HistoryLengthMax が指定されます。@n
        //! 同一インスタンスに対するこの関数の呼び出しはスレッドセーフではありません。
        void Initialize(
            nn::gfx::DescriptorPool* pDescriptorPool,
            int baseSlotIndex,
            int slotCount
            ) NN_NOEXCEPT
        {
            NN_SDK_REQUIRES(!m_Instance.IsInitialized());
            NN_SDK_ASSERT_EQUAL(sizeof(m_Memory), m_Instance.GetRequiredMemorySize(THistoryLengthMax));
            NN_SDK_ASSERT_GREATER_EQUAL(NN_ALIGNOF(detail::IndexRange), m_Instance.GetRequiredMemoryAlignment());
            m_Instance.SetMemory(&m_Memory[0], sizeof(m_Memory));
            m_Instance.Initialize(pDescriptorPool, baseSlotIndex, slotCount, THistoryLengthMax);
        }

        //! @brief このオブジェクトと包含する TransientDescriptorAllocator を破棄します。
        //! @pre このオブジェクトは初期化済でなければなりません。
        //! @post このオブジェクトは未初期化になります。
        //! @details
        //! 同一インスタンスに対するこの関数の呼び出しはスレッドセーフではありません。
        void Finalize() NN_NOEXCEPT
        {
            NN_SDK_REQUIRES(m_Instance.IsInitialized());
            m_Instance.Finalize();
        }

        //! @brief このオブジェクトが包含する TransientDescriptorAllocator を取得します。
        //! @return のオブジェクトが包含する TransientDescriptorAllocator のポインタを返します。
        //! @details
        //! 取得される TransientDescriptorAllocator の初期化および破棄はこのオブジェクトの Initialize(), Finalize() により行います。
        //! 取得される TransientDescriptorAllocator の生存期間はこのオブジェクトの生存期間と同一です。@n
        //! この関数はスレッドセーフです。
        TransientDescriptorAllocator* Get() NN_NOEXCEPT
        {
            return &m_Instance;
        }

        //! @brief このオブジェクトが包含する TransientDescriptorAllocator を取得します。
        //! @return のオブジェクトが包含する TransientDescriptorAllocator のポインタを返します。
        //! @details
        //! 取得される TransientDescriptorAllocator の初期化および破棄はこのオブジェクトの Initialize(), Finalize() により行います。
        //! 取得される TransientDescriptorAllocator の生存期間はこのオブジェクトの生存期間と同一です。@n
        //! この関数はスレッドセーフです。
        const TransientDescriptorAllocator* Get() const NN_NOEXCEPT
        {
            return &m_Instance;
        }

    private:
        TransientDescriptorAllocator m_Instance;
        detail::IndexRange m_Memory[THistoryLengthMax + 1];
    };

}}}
