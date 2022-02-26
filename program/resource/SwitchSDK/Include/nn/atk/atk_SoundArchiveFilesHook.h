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

#include <cstddef>
#include <cstdint>
#include <nn/nn_Macro.h>

namespace nn {
namespace atk {
namespace detail {

namespace fnd {
    class FileStream;
}

//! @brief  サウンドアーカイブに関連するファイルアクセスをフックします。
class SoundArchiveFilesHook
{
public:
    static const char ItemTypeStreamSound[];
    static const char ItemTypeWaveSound[];
    static const char ItemTypeSequenceSound[];
    static const char FileTypeStreamBinary[];
    static const char FileTypeWaveSoundBinary[];
    static const char FileTypeSequenceBinary[];
    static const char FileTypeBankBinary[];
    static const char FileTypeWaveArchiveBinary[];
    static const char FileTypeStreamPrefetchBinary[];

protected:
    SoundArchiveFilesHook() NN_NOEXCEPT : m_IsEnabled(true) { }
    virtual ~SoundArchiveFilesHook() NN_NOEXCEPT { }

public:
    //---------------------------------------------------------------------------
    //! @brief         フックが有効かどうかを調べます。
    //!
    //! @return        有効な場合は true、無効な場合は false を返します。
    //---------------------------------------------------------------------------
    bool GetIsEnable() const NN_NOEXCEPT { return m_IsEnabled; }

    //---------------------------------------------------------------------------
    //! @brief         フックの有効/無効を設定します。
    //!
    //! @param[in]     value     有効性を指定します。
    //---------------------------------------------------------------------------
    void SetIsEnable(bool value) NN_NOEXCEPT { m_IsEnabled = value; }

    //---------------------------------------------------------------------------
    //! @brief         指定アイテムラベルがフック対象かどうかを調べます。
    //!
    //! @param[in]     itemLabel     アイテムラベルを指定します。
    //!
    //! @return        対象の場合は true、対象でない場合は false を返します。
    //---------------------------------------------------------------------------
    bool IsTargetItem(const char* itemLabel) NN_NOEXCEPT
    {
        return m_IsEnabled ? IsTargetItemImpl(itemLabel) : false;
    }

    //---------------------------------------------------------------------------
    //! @brief         フックの有無が変化しないようにロックします。
    //---------------------------------------------------------------------------
    void Lock() NN_NOEXCEPT { LockImpl(); }

    //---------------------------------------------------------------------------
    //! @brief         フックのロックを解除します。
    //---------------------------------------------------------------------------
    void Unlock() NN_NOEXCEPT { UnlockImpl(); }

    //---------------------------------------------------------------------------
    //! @brief         指定バッファを使って指定アイテムラベルに関連するファイルを開きます。
    //!
    //! @param[in]     buffer            ファイルを開くのに利用するバッファを指定します。
    //! @param[in]     bufferLength      buffer の長さを指定します。
    //! @param[in]     cacheBuffer       ファイルロード用のキャッシュバッファを指定します。
    //! @param[in]     cacheBufferLength cacheBuffer の長さを指定します。
    //! @param[in]     itemLabel         アイテムラベルを指定します。
    //! @param[in]     fileType          取得するファイルタイプを指定します。
    //!
    //! @return        nn::atk::detail::fnd::FileStream を返します。
    //---------------------------------------------------------------------------
    fnd::FileStream* OpenFile(void* buffer, size_t bufferLength, void* cacheBuffer, size_t cacheBufferLength, const char* itemLabel, const char* fileType) NN_NOEXCEPT
    {
        return m_IsEnabled ? OpenFileImpl(buffer, bufferLength, cacheBuffer, cacheBufferLength, itemLabel, fileType) : NULL;
    }

    //---------------------------------------------------------------------------
    //! @brief         指定アイテムラベルに関連するファイルデータへのアドレスを取得します。
    //!
    //! @param[in]     itemLabel    アイテムラベルを指定します。
    //! @param[in]     itemType     アイテム種別を指定します。
    //! @param[in]     fileType     取得するファイルタイプを指定します。
    //! @param[in]     fileIndex    取得するファイルのインデックスがあれば指定します。
    //!
    //! @return        ファイルデータへのアドレスを返します。
    //---------------------------------------------------------------------------
    const void* GetFileAddress(const char* itemLabel, const char* itemType, const char* fileType, uint32_t fileIndex = 0) NN_NOEXCEPT
    {
        return m_IsEnabled ? GetFileAddressImpl(itemLabel, itemType, fileType, fileIndex) : NULL;
    }

protected:
    //---------------------------------------------------------------------------
    //! @brief         指定アイテムラベルがフック対象かどうかを調べます。
    //!
    //! @param[in]     itemLabel     アイテムラベルを指定します。
    //!
    //! @return        対象の場合は true、対象でない場合は false を返します。
    //---------------------------------------------------------------------------
    virtual bool IsTargetItemImpl(const char* itemLabel) NN_NOEXCEPT
    {
        (void)itemLabel;
        return false;
    }

    //---------------------------------------------------------------------------
    //! @brief         フックの有無が変化しないようにロックします。
    //---------------------------------------------------------------------------
    virtual void LockImpl() NN_NOEXCEPT { }

    //---------------------------------------------------------------------------
    //! @brief         フックのロックを解除します。
    //---------------------------------------------------------------------------
    virtual void UnlockImpl() NN_NOEXCEPT { }

    //---------------------------------------------------------------------------
    //! @brief         指定バッファを使って指定アイテムラベルに関連するファイルを開きます。
    //!
    //! @param[in]     buffer            ファイルを開くのに利用するバッファを指定します。
    //! @param[in]     bufferLength      buffer の長さを指定します。
    //! @param[in]     cacheBuffer       ファイルロード用のキャッシュバッファを指定します。
    //! @param[in]     cacheBufferLength cacheBuffer の長さを指定します。
    //! @param[in]     itemLabel         アイテムラベルを指定します。
    //! @param[in]     fileType          取得するファイルタイプを指定します。
    //!
    //! @return        nn::atk::detail::fnd::FileStream を返します。
    //---------------------------------------------------------------------------
    virtual fnd::FileStream* OpenFileImpl(void* buffer, size_t bufferLength, void* cacheBuffer, size_t cacheBufferLength, const char* itemLabel, const char* fileType) NN_NOEXCEPT
    {
        (void)buffer;
        (void)bufferLength;
        (void)cacheBuffer;
        (void)cacheBufferLength;
        (void)itemLabel;
        (void)fileType;
        return NULL;
    }

    //---------------------------------------------------------------------------
    //! @brief         指定アイテムラベルに関連するファイルデータへのアドレスを取得します。
    //!
    //! @param[in]     itemLabel    アイテムラベルを指定します。
    //! @param[in]     itemType     アイテム種別を指定します。
    //! @param[in]     fileType     取得するファイルタイプを指定します。
    //! @param[in]     fileIndex    取得するファイルのインデックスがあれば指定します。
    //!
    //! @return        ファイルデータへのアドレスを返します。
    //---------------------------------------------------------------------------
    virtual const void* GetFileAddressImpl(const char* itemLabel, const char* itemType, const char* fileType, uint32_t fileIndex = 0) NN_NOEXCEPT
    {
        (void)itemLabel;
        (void)itemType;
        (void)fileType;
        (void)fileIndex;
        return NULL;
    }

private:
    bool    m_IsEnabled;
#if defined(NN_BUILD_CONFIG_TOOLCHAIN_CLANG)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunused-private-field"
#endif
    uint8_t m_Padding[3];
#if defined(NN_BUILD_CONFIG_TOOLCHAIN_CLANG)
#pragma clang diagnostic pop
#endif
};

} // namespace nn::atk::detail
} // namespace nn::atk
} // namespace nn
