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
#include "gfx_TransientBufferAllocator.h"

namespace nn{ namespace gfx{ namespace util{

    //! @brief TransientBufferAllocator とその初期化に必要なメモリブロックを包含してオブジェクトの構築を行うクラスです。
    //! @tparam THistoryLengthMax 構築する TransientBufferAllocator が記録する履歴の最大数です。
    //! @see TransientBufferAllocator
    template<int THistoryLengthMax>
    class TransientBufferAllocatorHolder
    {
        NN_DISALLOW_COPY(TransientBufferAllocatorHolder);

    public:

        //! @brief 構築する TransientBufferAllocator に設定される履歴の最大数です。
        static const int HistoryLengthMax = THistoryLengthMax;

    public:

        //!@ brief デフォルトコンストラクタです
        TransientBufferAllocatorHolder() NN_NOEXCEPT
        {
        }

        //! @brief このオブジェクトと包含する TransientBufferAllocator を初期化します。
        //! @param[in] pBuffer                 バッファです。
        //! @param[in] baseOffset              管理する領域の先頭のオフセットです。
        //! @param[in] size                    管理する領域のサイズ（バイト）です。
        //! @param[in] allocatableAlignmentMax このアロケータから確保する領域に要求する最大のアライメントです。
        //! @pre このオブジェクトは未初期化でなければなりません。
        //! @pre 各パラメータの事前条件は TransientBufferAllocator::Initialize() と同一です。
        //! @post このオブジェクトは初期化済になります。
        //! @see TransientBufferAllocator::Initialize()
        //! @details
        //! 指定したパラメータを用いてこのオブジェクトが包含する TransientBufferAllocator のインスタンスを初期化します。
        //! 初期化に使用するメモリブロックにはこのオブジェクトが包含しているメモリ領域を使用します。
        //! 履歴の最大数には HistoryLengthMax が指定されます。@n
        //! 同一インスタンスに対するこの関数の呼び出しはスレッドセーフではありません。
        void Initialize(
            nn::gfx::Buffer* pBuffer,
            ptrdiff_t baseOffset,
            size_t size,
            size_t allocatableAlignmentMax
            ) NN_NOEXCEPT
        {
            NN_SDK_REQUIRES(!m_Instance.IsInitialized());
            NN_SDK_ASSERT_EQUAL(sizeof(m_Memory), m_Instance.GetRequiredMemorySize(THistoryLengthMax));
            NN_SDK_ASSERT_GREATER_EQUAL(NN_ALIGNOF(detail::OffsetRange), m_Instance.GetRequiredMemoryAlignment());
            m_Instance.SetMemory(&m_Memory[0], sizeof(m_Memory));
            m_Instance.Initialize(pBuffer, baseOffset, size, allocatableAlignmentMax, THistoryLengthMax);
        }

        //! @brief このオブジェクトと包含する TransientBufferAllocator を破棄します。
        //! @pre このオブジェクトは初期化済でなければなりません。
        //! @post このオブジェクトは未初期化になります。
        //! @details
        //! 同一インスタンスに対するこの関数の呼び出しはスレッドセーフではありません。
        void Finalize() NN_NOEXCEPT
        {
            NN_SDK_REQUIRES(m_Instance.IsInitialized());
            m_Instance.Finalize();
        }

        //! @brief このオブジェクトが包含する TransientBufferAllocator を取得します。
        //! @return のオブジェクトが包含する TransientBufferAllocator のポインタを返します。
        //! @details
        //! 取得される TransientBufferAllocator の初期化および破棄はこのオブジェクトの Initialize(), Finalize() により行います。
        //! 取得される TransientBufferAllocator の生存期間はこのオブジェクトの生存期間と同一です。@n
        //! この関数はスレッドセーフです。
        TransientBufferAllocator* Get() NN_NOEXCEPT
        {
            return &m_Instance;
        }

        //! @brief このオブジェクトが包含する TransientBufferAllocator を取得します。
        //! @return のオブジェクトが包含する TransientBufferAllocator のポインタを返します。
        //! @details
        //! 取得される TransientBufferAllocator の初期化および破棄はこのオブジェクトの Initialize(), Finalize() により行います。
        //! 取得される TransientBufferAllocator の生存期間はこのオブジェクトの生存期間と同一です。@n
        //! この関数はスレッドセーフです。
        const TransientBufferAllocator* Get() const NN_NOEXCEPT
        {
            return &m_Instance;
        }

    private:
        TransientBufferAllocator m_Instance;
        detail::OffsetRange m_Memory[THistoryLengthMax + 1];
    };

}}}
