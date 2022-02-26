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

#include <type_traits>

#include <nn/nn_Common.h>
#include <nn/nn_Macro.h>
#include <nn/util/util_BitUtil.h>

namespace nn { namespace util {

namespace detail {

template<typename Integer>
void AndEqual(Integer dest[], const Integer source[], int count) NN_NOEXCEPT
{
    for (int i = 0; i < count; i++)
    {
        dest[i] &= source[i];
    }
}

template<typename Integer>
void XorEqual(Integer dest[], const Integer source[], int count) NN_NOEXCEPT
{
    for (int i = 0; i < count; i++)
    {
        dest[i] ^= source[i];
    }
}

template<typename Integer>
void OrEqual(Integer dest[], const Integer source[], int count) NN_NOEXCEPT
{
    for (int i = 0; i < count; i++)
    {
        dest[i] |= source[i];
    }
}

template<typename Integer>
bool Equals(const Integer dest[], const Integer source[], int count) NN_NOEXCEPT
{
    for (int i = 0; i < count; i++)
    {
        if(dest[i] != source[i])
        {
            return false;
        }
    }
    return true;
}

template<typename Integer>
bool IsAnyOn(const Integer _storage[], int count) NN_NOEXCEPT
{
    for (int i = 0; i < count; i++)
    {
        if (_storage[i] != 0)
        {
            return true;
        }
    }
    return false;
}

template<typename Integer>
int CountPopulation(const Integer _storage[], int count) NN_NOEXCEPT
{
    int countOn = 0;
    for (int i = 0; i < count; i++)
    {
        countOn += popcount(_storage[i]);
    }
    return countOn;
}

template<typename Integer>
void Flip(Integer _storage[], int count) NN_NOEXCEPT
{
    for (int i = 0; i < count; i++)
    {
        _storage[i] = ~_storage[i];
    }
}

}

/*!
    @brief      POD のビットフラグ集合を扱う型です。

    @details    POD であることを維持したまま std::bitset() 相当のビットフラグ操作機能を安全に利用することができます。
                template に指定されたビット数 N に応じて BitFlagSet が持つデータメンバが変化します。
                N <= 32 の場合は uint32_t 型の長さ 1 の配列を持ちます。
                それ以外の場合は uint64_t 型の長さ (N + 64 - 1) / 64 の配列を持ちます。
                大きい N を指定した場合は、値渡しのコストやスタックのサイズに注意してください。
                POD として扱えることを目的としているため、デフォルトコンストラクタで初期化されません。
                MakeBitFlagSet を使って初期化された値を取得するか、Reset を呼び出して初期化してください。
                ビットフラグ以外の値を格納する可能性がある型は BitPack を使用してください。

    @tparam     N           BitFlagSet が格納するビット数です。
    @tparam     Tag         異なる型としてインスタンス化するためのタグです。
*/
template< int N, typename Tag = void >
struct BitFlagSet
{
    NN_STATIC_ASSERT(0 < N);
    //!< @cond
    typedef typename std::conditional<N <= 32, uint32_t, uint64_t>::type StorageT;
    //!< @endcond
    static const int StorageBitCount = NN_BITSIZEOF(StorageT);
    static const int StorageCount = (N + StorageBitCount - 1) / StorageBitCount;
    StorageT _storage[StorageCount];

public:

    /*!
        @brief      BitFlagSet の個々のビットフラグにアクセスするための型です。
    */
    class Reference
    {
    friend struct BitFlagSet<N, Tag>;

    public:

        /*!
            @brief      当該ビットフラグに値を設定します。
        */
        Reference& operator=(bool isOn) NN_NOEXCEPT
        {
            m_Set->Set(m_Index, isOn);
            return *this;
        }

        /*!
            @brief      当該ビットフラグに値を設定します。
        */
        Reference& operator=(const Reference& ref) NN_NOEXCEPT
        {
            m_Set->Set(m_Index, ref);
            return *this;
        }

        /*!
            @brief      当該ビットフラグを反転します。
        */
        Reference& Flip() NN_NOEXCEPT
        {
            m_Set->Flip(m_Index);
            return *this;
        }

        /*!
            @brief      当該ビットフラグを反転します。
        */
        bool operator~() const NN_NOEXCEPT
        {
            return !m_Set->Test(m_Index);
        }

        /*!
            @brief      当該ビットフラグを取得します。
        */
        NN_IMPLICIT operator bool() const NN_NOEXCEPT
        {
            return m_Set->Test(m_Index);
        }

    private:
        Reference() NN_NOEXCEPT : m_Set(nullptr){}

        Reference(BitFlagSet<N, Tag>& type, int index) NN_NOEXCEPT : m_Set(&type), m_Index(index){}

        BitFlagSet<N, Tag>* m_Set;
        int                 m_Index;
    };

    /*!
        @brief      全てのビットフラグを反転したコピーを返します。

        @details    関数内で自動変数を確保して、値渡しでコピーを返します。
                    N が大きい場合、スタックのサイズなどに注意してください。

        @return     全てのビットフラグが反転したビットフラグ集合
    */
    BitFlagSet<N, Tag> operator~() const NN_NOEXCEPT
    {
        BitFlagSet<N, Tag> tmp = *this;
        return tmp.Flip();
    }

    /*!
        @brief      2 つのビットフラグ集合間の AND コピーを返します。

        @details    関数内で自動変数を確保して、値渡しでコピーを返します。
                    N が大きい場合、スタックのサイズなどに注意してください。

        @param[in]  other   AND を取るビットフラグ集合
        @return     ビットフラグ集合の AND
    */
    BitFlagSet<N, Tag> operator&(const BitFlagSet<N, Tag>& other) const NN_NOEXCEPT
    {
        BitFlagSet<N, Tag> value = *this;
        value &= other;
        return value;
    }

    /*!
        @brief      2 つのビットフラグ集合間の XOR コピーを返します。

        @details    関数内で自動変数を確保して、値渡しでコピーを返します。
                    N が大きい場合、スタックのサイズなどに注意してください。

        @param[in]  other   XOR を取るビットフラグ集合
        @return     ビットフラグ集合の XOR
    */
    BitFlagSet<N, Tag> operator^(const BitFlagSet<N, Tag>& other) const NN_NOEXCEPT
    {
        BitFlagSet<N, Tag> value = *this;
        value ^= other;
        return value;
    }

    /*!
        @brief      2 つのビットフラグ集合間の OR コピーを返します。

        @details    関数内で自動変数を確保して、値渡しでコピーを返します。
                    N が大きい場合、スタックのサイズなどに注意してください。

        @param[in]  other   OR を取るビットフラグ集合
        @return     ビットフラグ集合の OR
    */
    BitFlagSet<N, Tag> operator|(const BitFlagSet<N, Tag>& other) const NN_NOEXCEPT
    {
        BitFlagSet<N, Tag> value = *this;
        value |= other;
        return value;
    }

    /*!
        @brief      与えられたビットフラグ集合との AND を this に設定します。

        @param[in]  other   AND を取るビットフラグ集合
        @return     *this の参照
    */
    BitFlagSet<N, Tag>& operator&=(const BitFlagSet<N, Tag>& other) NN_NOEXCEPT
    {
        nn::util::detail::AndEqual(_storage, other._storage, StorageCount);
        return *this;
    }

    /*!
        @brief      与えられたビットフラグ集合との XOR を this に設定します。

        @param[in]  other   XOR を取るビットフラグ集合
        @return     *this の参照
    */
    BitFlagSet<N, Tag>& operator^=(const BitFlagSet<N, Tag>& other) NN_NOEXCEPT
    {
        nn::util::detail::XorEqual(_storage, other._storage, StorageCount);
        return *this;
    }

    /*!
        @brief      与えられたビットフラグ集合との OR を this に設定します。

        @param[in]  other   OR を取るビットフラグ集合
        @return     *this の参照
    */
    BitFlagSet<N, Tag>& operator|=(const BitFlagSet<N, Tag>& other) NN_NOEXCEPT
    {
        nn::util::detail::OrEqual(_storage, other._storage, StorageCount);
        return *this;
    }

    /*!
        @brief      ビットフラグ集合同士を比較します。

        @param[in]  other   比較するビットフラグ集合
        @return     全てのビットフラグが等しければ true そうでなければ false
    */
    bool operator==(const BitFlagSet<N, Tag>& other) const NN_NOEXCEPT
    {
        return nn::util::detail::Equals(_storage, other._storage, StorageCount);
    }

    /*!
        @brief      ビットフラグ集合同士を比較します。

        @param[in]  other   比較するビットフラグ集合
        @return     いずれかのビットフラグが等しくなければ true そうでなければ false
    */
    bool operator!=(const BitFlagSet<N, Tag>& other) const NN_NOEXCEPT
    {
        return !operator==(other);
    }

    /*!
        @brief      指定場所のビットフラグ値を返します。

        @param[in]  index     ビットフラグ値を取得する場所

        @pre        index < N
    */
    bool operator[](int index) const NN_NOEXCEPT
    {
        NN_SDK_REQUIRES_RANGE(index, 0, N);

        return Test(index);
    }

    /*!
        @brief      指定場所のビットフラグ値を参照・変更できる BitFlagSet<N, Tag>::Reference 型を返します。

        @param[in]  index     ビットフラグ値を参照・変更する場所

        @pre        index < N
    */
    Reference operator[](int index) NN_NOEXCEPT
    {
        NN_SDK_REQUIRES_RANGE(index, 0, N);

        return Reference(*this, index);
    }

    /*!
        @brief      いずれかのビットフラグが有効であれば true そうでなければ false を返します。
    */
    bool IsAnyOn() const NN_NOEXCEPT
    {
        return nn::util::detail::IsAnyOn(_storage, StorageCount);
    }

    /*!
        @brief      有効なビットフラグの数を返します。
    */
    int CountPopulation() const NN_NOEXCEPT
    {
        return nn::util::detail::CountPopulation(_storage, StorageCount);
    }

    /*!
        @brief      指定フラグのビットフラグを反転します。

        @tparam     FlagT   ビットフラグを反転するフラグ

        @return     *this の参照
    */
    template<typename FlagT>
    BitFlagSet<N, Tag>& Flip() NN_NOEXCEPT
    {
        return Set<FlagT>(!Test<FlagT>());
    }

    /*!
        @brief      指定場所のビットフラグを反転します。

        @param[in]  index     ビットフラグを反転する場所

        @pre        index < N

        @return     *this の参照
    */
    BitFlagSet<N, Tag>& Flip(int index) NN_NOEXCEPT
    {
        NN_SDK_REQUIRES_RANGE(index, 0, N);

        return Set(index, !Test(index));
    }

    /*!
        @brief      全てのビットフラグを反転します。

        @return     *this の参照
    */
    BitFlagSet<N, Tag>& Flip() NN_NOEXCEPT
    {
        nn::util::detail::Flip(_storage, StorageCount);
        Truncate();

        return *this;
    }

    /*!
        @brief      全てのビットフラグが有効であれば true そうでなければ false を返します。
    */
    bool IsAllOn() const NN_NOEXCEPT
    {
        return CountPopulation() == GetCount();
    }

    /*!
        @brief      全てのビットフラグが無効であれば true そうでなければ false を返します。
    */
    bool IsAllOff() const NN_NOEXCEPT
    {
        return !IsAnyOn();
    }

    /*!
        @brief      全てのビットフラグを無効にします。

        @return     *this の参照
    */
    BitFlagSet<N, Tag>& Reset() NN_NOEXCEPT
    {
        std::memset(_storage, 0, sizeof(_storage));

        return *this;
    }

    /*!
        @brief      指定フラグのビットフラグを無効にします。

        @tparam     FlagT   ビットフラグを無効するフラグ

        @return     *this の参照
    */
    template<typename FlagT>
    BitFlagSet<N, Tag>& Reset() NN_NOEXCEPT
    {
        return Set<FlagT>(false);
    }

    /*!
        @brief      指定場所のビットフラグを無効にします。

        @param[in]  index     ビットフラグを無効にする場所

        @pre        index < N

        @return     *this の参照
    */
    BitFlagSet<N, Tag>& Reset(int index) NN_NOEXCEPT
    {
        NN_SDK_REQUIRES_RANGE(index, 0, N);

        return Set(index, false);
    }

    /*!
        @brief      全てのビットフラグを有効にします。

        @return     *this の参照
    */
    BitFlagSet<N, Tag>& Set() NN_NOEXCEPT
    {
        std::memset(_storage, ~0, sizeof(_storage));
        Truncate();

        return *this;
    }

    /*!
        @brief      指定フラグのビットフラグを設定します。

        @param[in]  isOn    設定するビットフラグの値

        @return     *this の参照
    */
    template<typename FlagT>
    BitFlagSet<N, Tag>& Set(bool isOn = true) NN_NOEXCEPT
    {
        return SetImpl(FlagT::StorageIndex, FlagT::StorageMask, isOn);
    }

    /*!
        @brief      指定インデックスのビットフラグを設定します。

        @param[in]  index     設定するビットフラグのインデックス
        @param[in]  isOn    設定するビットフラグの値

        @pre        index < N

        @return     *this の参照
    */
    BitFlagSet<N, Tag>& Set(int index, bool isOn = true) NN_NOEXCEPT
    {
        NN_SDK_REQUIRES_RANGE(index, 0, N);

        return SetImpl(GetStorageIndex(index), MakeStorageMask(index), isOn);
    }

    /*!
        @brief      保持できるビットフラグ数を返します。
    */
    int GetCount() const NN_NOEXCEPT
    {
        return N;
    }

    /*!
        @brief      指定フラグのビットフラグを取得します。

        @return     *this の参照
    */
    template<typename FlagT>
    bool Test() const NN_NOEXCEPT
    {
        return TestImpl(FlagT::StorageIndex, FlagT::StorageMask);
    }

    /*!
        @brief      指定インデックスのビットフラグを取得します。

        @param[in]  index     取得するビットフラグのインデックス

        @pre        index < N

        @return     *this の参照
    */
    bool Test(int index) const NN_NOEXCEPT
    {
        NN_SDK_REQUIRES_RANGE(index, 0, N);

        return TestImpl(GetStorageIndex(index), MakeStorageMask(index));
    }

    /*!
        @brief ビットフラグを定義する型です。

        @tparam     BitIndex ビットフラグのインデックスです
    */
    template< int BitIndex >
    struct Flag
    {
    friend struct BitFlagSet<N, Tag>;

        NN_STATIC_ASSERT(BitIndex < N);

        static const int Index = BitIndex;
        static const BitFlagSet<N, Tag> Mask;

    private:
        static const int StorageIndex = Index / StorageBitCount;
        static const StorageT StorageMask = static_cast<StorageT>(1) << Index % StorageBitCount;

        template< int StorageCount >
        struct StorageTrait
        {
            NN_STATIC_ASSERT(StorageCount == 1);

            typedef StorageT Type;
        };
    };

private:

    BitFlagSet<N, Tag>& SetImpl(int storageIndex, StorageT storageMask, bool isOn) NN_NOEXCEPT
    {
        if (isOn)
        {
            _storage[storageIndex] |= storageMask;
        }
        else
        {
            _storage[storageIndex] &= ~storageMask;
        }
        return *this;
    }

    bool TestImpl(int storageIndex, StorageT storageMask) const NN_NOEXCEPT
    {
        return (_storage[storageIndex] & storageMask) != 0;
    }

    void Truncate() NN_NOEXCEPT
    {
        TruncateIf(std::integral_constant<bool, N % StorageBitCount != 0>());
    }

    void TruncateIf(std::true_type) NN_NOEXCEPT
    {
        _storage[StorageCount - 1] &= MakeStorageMask(N) - 1;
    }

    void TruncateIf(std::false_type){}

    static int GetStorageIndex(int index) NN_NOEXCEPT
    {
        return index / StorageBitCount;
    }

    static StorageT MakeStorageMask(int index) NN_NOEXCEPT
    {
        return static_cast<StorageT>(1) << index % StorageBitCount;
    }
};

template< int N, typename Tag >
template< int BitIndex >
const BitFlagSet<N, Tag> BitFlagSet<N, Tag>::Flag<BitIndex>::Mask = {
    {
        static_cast<
            typename StorageTrait<BitFlagSet<N, Tag>::StorageCount>::Type
            >(1) << BitIndex
    }
};

/*!
    @brief      全てのビットフラグが false に初期化された BitFlagSet を取得します。

    @details    関数内で自動変数を確保して、値渡しでコピーを返します。
                N が大きい場合、スタックのサイズなどに注意してください。

    @return     全てのビットフラグが false のビットフラグ集合
*/
template< int N, typename Tag>
BitFlagSet<N, Tag> MakeBitFlagSet() NN_NOEXCEPT
{
    BitFlagSet<N, Tag> tmp;
    return tmp.Reset();
}

/*!
    @brief      全てのビットフラグが false に初期化された BitFlagSet を取得します。

    @details    関数内で自動変数を確保して、値渡しでコピーを返します。
                N が大きい場合、スタックのサイズなどに注意してください。

    @return     全てのビットフラグが false のビットフラグ集合
*/
template< int N>
BitFlagSet<N, void> MakeBitFlagSet() NN_NOEXCEPT
{
    return MakeBitFlagSet<N, void>();
}

}} // namespace nn::util
