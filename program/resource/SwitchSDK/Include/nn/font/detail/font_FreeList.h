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

namespace nn
{
namespace font
{
namespace detail
{

/**
 *  固定サイズのメモリエリアを管理する仕組み（フリーリスト）です。
 */
class FreeList
{
public:
    FreeList() : m_pFree(NULL), m_pWork(NULL) {}

    /**
     *  フリーリストを初期化します。
     *
     *  @param[in] work         ワーク領域。elementSize*countの大きさとしてください。sizeof(void*) のアライメントである必要があります。
     *  @param[in] elementSize    一つの要素のサイズ。sizeof(void*) の倍数としてください。
     *  @param[in] count          要素の最大数。
     */
    void Initialize(void* work, int elementSize, int count)
    {
        NN_SDK_ASSERT(work);
        NN_SDK_ASSERT(elementSize > 0 && elementSize % sizeof(void*) == 0);
        NN_SDK_ASSERT(count > 0);

        void** pWorkAsPtr = static_cast<void**>(work);
        int sizeDividePtrSize = elementSize / sizeof(void*);

        m_pFree = work;
        /*
         *  フリーリストに繋がれている間は、領域をリンクリストとして使う。
         */
        for (int i = 0; i < count - 1; i++) {
            pWorkAsPtr[sizeDividePtrSize * i] = &pWorkAsPtr[sizeDividePtrSize * (i + 1)];
        }
        pWorkAsPtr[sizeDividePtrSize * (count - 1)] = NULL;

        m_pWork = work;
    }

    /**
     * フリーリストから要素を得ます。
     *
     * @return  新しい要素へのポインタ。領域のデータは不定です。
     *          フリーリストが空のときにはNULLが返ります。
     */
    void* Get()
    {
        void* pFree = m_pFree;

        if (pFree == NULL) {
            return NULL;
        } else {
            m_pFree = *static_cast<void**>(pFree);  // 次の要素のポインタを辿ってm_pFreeに入れる
            return pFree;
        }
    }

    /**
     *  不要な要素をフリーリストへ戻します。
     *
     *  @param[in] elementPtr     要素へのポインタ。
     */
    void Put(void* pElement)
    {
        // リストに戻されたのだから、領域内にポインタを書き込んでもOK
        //
        // 戻されたものの先頭に、今のm_pFreeのアドレスを記憶しておく
        // そして、m_pFreeを、今返された pElement にする。
        *static_cast<void**>(pElement) = m_pFree;
        m_pFree = pElement;
    }

    /**
     *  フリーリストのワークエリアへのポインタを得ます。
     *
     *  @return ワークエリアへのポインタ
     */
    void* GetWork() const
    {
        return m_pWork;
    }

    /**
     *  内部状態を生成時の状態に戻します。
     *
     *  initで与えたバッファを解放したときなど、内部に保持しているポインタが
     *  使用できなくなったとき等にご使用ください。
     *
     *  このメソッドを呼んだ後再び使うには、initを呼び出す必要があります。
     */
    void Cleanup()
    {
        m_pFree = NULL;
        m_pWork = NULL;
    }

    /**
     *  フリーリストが空になっているか否かを返します。
     *
     *  @return 空になっていて要素を確保できないなら true、まだ要素があるなら false を返します。
     */
    bool IsEmpty() const
    {
        return (m_pFree == NULL);
    }

private:
    void* m_pFree;        // 未使用領域のリンクリストの先頭
    void* m_pWork;        // ワークエリアへのポインタ
};

} // namespace nn::font::detail
} // namespace nn::font
} // namespace nn
