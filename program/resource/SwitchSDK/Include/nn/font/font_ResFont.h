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

#include <nn/font/font_ResFontBase.h>

namespace nn {
namespace font {

//---------------------------------------------------------------------------
//! @brief        フォントリソースに対する操作を実装するクラスです。
//---------------------------------------------------------------------------
class ResFont : public ResFontBase
{
public:
    //! 実行時型情報です。
    NN_FONT_RUNTIME_TYPEINFO(ResFontBase);

    /* ------------------------------------------------------------------------
            関数
       ------------------------------------------------------------------------ */

    //! @name コンストラクタ / デストラクタ
    //@{

    //! コンストラクタです。
                            ResFont();

    //! デストラクタです。
    //!
    //! @details
    //! フォントリソースが関連づけられている場合は、デストラクタが呼び出される前に Finalize() または RemoveResource() を実行する必要があります。
    //!
    virtual                 ~ResFont();

    //! @brief 終了処理を行います。
    //!
    //! @details
    //! フォントリソースが関連づけられている場合に RemoveResource() を呼び出します。
    //! 関連づけられているフォントリソースを取得する必要があるときは、代わりに RemoveResource() を使用してください。
    //!
    //! @param[in] pDevice デバイス
    //!
    virtual void Finalize(nn::gfx::Device* pDevice) NN_OVERRIDE;

    //@}

    //! @name リソースの関連付け / 解除
    //@{

    //! @brief          リソースを初期状態に戻します。
    //!
    //! ResFont::SetResource() によって書き換わったリソースのバイナリを
    //! 初期状態に戻します。
    //!
    //! リソースをメモリ上で移動する場合などは、事前にこの処理を行う必要があります。
    //!
    //! @param[in, out] pResource  初期状態に戻すリソースです。
    //!
    static void RevertResource(void* pResource);

    //! @brief          フォントリソースを関連付けてフォントを使用可能にします。
    //!
    //! @details
    //! フォントリソースは最初に ResFont に関連付けられたときに
    //! 内容が書きかえられ、メモリ上で移動できなくなります。
    //!
    //! 使用しなくなったフォントリソースをもとに戻すには
    //! RevertResource() を使用します。
    //!
    //! @param[in] pDevice デバイス
    //! @param[in] pBfntData   ResFont に関連付ける bfnt へのポインタ。nn::font::ResourceAlignment byte アライメントされている必要があります。
    //!
    //! @return         リソースの関連付けが成功したなら true を、
    //!                 そうでなければ false を返します。@n
    //!                 リソース bfnt が不正であると失敗します。
    //!
    bool                    SetResource(nn::gfx::Device* pDevice, void* pBfntData)
    {
        return SetResource(pDevice, pBfntData, NULL, 0, 0);
    }

    //! @brief          フォントリソースを関連付けてフォントを使用可能にします。
    //!
    //! @details
    //! フォントリソースは最初に ResFont に関連付けられたときに
    //! 内容が書きかえられ、メモリ上で移動できなくなります。
    //!
    //! 使用しなくなったフォントリソースをもとに戻すには
    //! RevertResource() を使用します。
    //!
    //! @param[in] pDevice デバイス
    //! @param[in]  pBfntData   ResFont に関連付ける bfnt へのポインタ。nn::font::ResourceAlignment byte アライメントされている必要があります。
    //! @param[in]  pMemoryPool メモリプールです。バイナリをメモリプール内に配置した場合にそのメモリプールを指定します。未配置の場合は nullptr を指定してください。
    //! @param[in]  memoryPoolOffset 配置先メモリプールの先頭からリソース配置位置へのオフセットです。
    //! @param[in]  memoryPoolSize 配置先メモリプールのサイズです。
    //!
    //! @return         リソースの関連付けが成功したなら true を、
    //!                 そうでなければ false を返します。@n
    //!                 リソース bfnt が不正であると失敗します。
    //!
    bool                    SetResource(nn::gfx::Device* pDevice, void* pBfntData, nn::gfx::MemoryPool* pMemoryPool, ptrdiff_t memoryPoolOffset, size_t memoryPoolSize);

    //! @brief      フォントリソースの割り当てを解除します。
    //!
    //! @details
    //! この関数を呼び出しても SetResource() で書き換えられた
    //! フォントリソースは元には戻りません。
    //!
    //! 使用しなくなったフォントリソースをもとに戻すには
    //! RevertResource() を使用します。
    //!
    //! @param[in] pDevice デバイス
    //!
    //! @return     リソースが関連付けされている場合は、関連付けされているリソースへの
    //!             ポインタを返します。
    //!             関連付けされていない場合は NULL を返します。
    //!
    void*                   RemoveResource(nn::gfx::Device* pDevice);

    using ResFontBase::IsManaging;

    //@}

    //! @name リソースリロケーション関連
    //@{

    //! @brief フォントリソースをリロケートします。
    //!
    //! @details
    //! フォントリソースでは ResFontBase::GenTextureNames 内でリロケートされるため
    //! ユーザーがこのメソッドを明示的に呼び出す必要はありません。
    //! @see    nn::util::RelocationTable::Relocate()
    //!
    //! @param[in]  pBfntData オフセットを解決するフォントリソースへのポインタ。
    //!
    static void Relocate(void* pBfntData)
    {
        NN_UNUSED(pBfntData);
    }

    //! @brief アンリロケートします。
    //!
    //! @details
    //! RemoveResource を呼び出した後にこのメソッドを呼び出すことで
    //! フォントリソースのメモリを移動可能な状態にすることができます。
    //! @see    nn::util::RelocationTable::Unrelocate()
    //!
    //! @param[in]  pBfntData 再利用したいフォントリソースへのポインタ。
    //!
    static void Unrelocate(void* pBfntData);

    //@}



private:
    /* ------------------------------------------------------------------------
            関数
       ------------------------------------------------------------------------ */

    //! @brief      メモリ上にロードされたbffntファイルを再構築して
    //!             使用できるようにします。
    //!
    //! @param[in,out]  pFileHeader  メモリ上にロードしたbffntファイルの
    //!                             ファイルヘッダへのポインタ
    //!
    //! @return     再構築に成功した場合はFINFブロック本体へのポインタを返します。
    //!             再構築に失敗した場合はNULLを返します。
    //!
    static FontInformation* Rebuild(nn::font::detail::BinaryFileHeader* pFileHeader);
};

}   // namespace font
}   // namespace nn
