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
#include <nn/nn_SdkAssert.h>
#include <cstddef>

namespace nn { namespace atk { namespace detail { namespace fnd {
    //! @internal
    //! @brief ワークバッファが使用するメモリ要求量を計算するためのクラスです
    //! @details
    //!        BufferSizeCalculator::Add() は WorkBufferAllocator::Allocate() と対応関係があり、
    //!        Add() の順序通りに Allocate() を行えば、WorkBufferAllocator が最悪ケースで確保するメモリ使用量と一致します。
    //!        初回の Add で設定したアライメントは、 GetBufferSize() で取得するメモリ要求量には含まれないため、
    //!        呼び出し側でワークバッファのアライメントを要求するか、アライメント調整用のメモリを余分に確保するなどの処理を加えてください。
    class WorkBufferSizeCalculator
    {
    public:
        //! @internal
        //! @brief        コンストラクタです
        WorkBufferSizeCalculator() NN_NOEXCEPT : m_Size( 0 ), m_Alignment( 0 ), m_Offset( 0 ) {}

        //! @internal
        //! @brief        要求メモリ量を加算します。アライメントのことは考慮されません。
        //! @param[in]    size 加算するメモリ量です
        void Add(size_t size) NN_NOEXCEPT;

        //! @internal
        //! @brief        要求メモリ量を加算します。アライメントのことも考慮されたうえで、メモリ要求量の加算を行います。
        //! @param[in]    size      加算する要求メモリ量です
        //! @param[in]    alignment 加算する要求メモリにおいて、必要とされるアライメントです
        //! @pre
        //!  - alignment は 2 のべき乗である
        void Add(size_t size, size_t alignment) NN_NOEXCEPT;

        //! @internal
        //! @brief        指定サイズのメモリ要求量を個数分加算します。アライメントのことも考慮されたうえで、メモリ要求量の加算が行われます。
        //! @param[in]    objectSize オブジェクト 1 個あたりが要求するメモリ量です
        //! @param[in]    alignment  オブジェクトが要求するアライメントです
        //! @param[in]    count      オブジェクトの個数です
        //! @pre
        //!  - alignment は 2 のべき乗である
        //!  - count > 0
        void Add(size_t objectSize, size_t alignment, int count) NN_NOEXCEPT;
        //! @inernal
        //! @brief        クラスインスタンス生成のためのメモリ要求量を加算します。アライメントのことも考慮されたうえで、メモリ要求量の加算が行われます。
        //! @tparam       T メモリ要求を行うクラスです
        template<class T>
        void Add() NN_NOEXCEPT
        {
            AddImpl(sizeof(T), NN_ALIGNOF(T));
        }
        //! @inernal
        //! @brief        クラスインスタンス生成のためのメモリ要求量を複数個分加算します。アライメントのことも考慮されたうえで、メモリ要求量の加算が行われます。
        //! @tparam       T メモリ要求を行うクラスです
        //! @param[in]    count オブジェクトの個数です
        //! @pre
        //!  - count > 0
        template<class T>
        void Add(int count) NN_NOEXCEPT
        {
            NN_SDK_ASSERT_GREATER(count, 0);
            for(int i = 0; i < count; ++i)
            {
                AddImpl(sizeof(T), NN_ALIGNOF(T));
            }
        }
        //! @inernal
        //! @brief        メモリ要求量のアラインアップを行います。
        //! @param[in]    alignment アラインアップするアライメントです
        //! @pre
        //!  - alignment は 2 のべき乗である
        void AlignUp(size_t alignment) NN_NOEXCEPT;
        //! @internal
        //! @brief        メモリ要求量を取得します。 Add 関数や AlignUp 関数で加算したメモリ要求量の合計が取得できます。
        //! @return       メモリ要求量です
        size_t GetSize() const NN_NOEXCEPT;

    private:
        void AddImpl(size_t size, size_t alignment) NN_NOEXCEPT;

    private:
        size_t m_Size;
        size_t m_Alignment;
        size_t m_Offset;
    };
}}}}
