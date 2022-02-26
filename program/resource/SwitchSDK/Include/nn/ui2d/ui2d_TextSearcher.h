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

#include <nn/font/font_Types.h>
#include <nn/ui2d/ui2d_Types.h>

namespace nn
{
namespace ui2d
{

class Layout;
class TextBox;

//---------------------------------------------------------------------------
//! @brief テキストIDからテキストの検索を行う基底クラスです。
//!
//! @details
//! 純粋仮想クラスとなっています。
//!
//---------------------------------------------------------------------------
class TextSearcher
{
public:
    /// テキストの情報を保持する構造体です。UTF-16版。
    struct TextInfo {
        const uint16_t* pText;     ///< テキストです。NULLだった場合は、レイアウトエディタで指定された文字列をセットします。
                                ///< テキストボックスを空にする場合は、空文字を指定してください。
        int32_t  textLength;         ///< テキストの長さです。バイト数ではなく文字数で指定してください。0のときはtextがnull終端される箇所までの長さを設定します。
        int32_t  bufferLength;       ///< テキストを保持するために確保するバッファの長さです。バイト数ではなく文字数で指定してください。
                                ///< 0のときは、textLengthで指定された長さを設定します。レイアウトエディタの「文字列の長さを強制的に指定する」機能で指定
                                ///< された長さを使いたい場合、forceAssignedTextLengthをコピーしてください。
        int32_t forceAssignedTextLength;    ///< レイアウトエディタで「文字列の長さを強制的に指定する」のチェックが入っている場合、このメンバに指定された
                                        ///< 文字数が入った状態でSearchTextに渡されます。チェックが入っていない場合は-1が設定されます。
                                        ///< 出力としては使われません。

        //! @brief デフォルト値に設定します。
        //!
        void SetDefault()
        {
            pText = NULL;
            textLength = 0;
            bufferLength = 0;
            forceAssignedTextLength = -1;
        }
    };

    /// テキストの情報を保持する構造体です。UTF-8版。
    struct TextInfoUtf8 {
        const char* pText;     ///< テキストです。NULLだった場合は、レイアウトエディタで指定された文字列をセットします。
                                ///< テキストボックスを空にする場合は、空文字を指定してください。
        int32_t  textLength;         ///< テキストの長さです。バイト数ではなく文字数で指定してください。0のときはtextがnull終端される箇所までの長さを設定します。
        int32_t  bufferLength;       ///< テキストを保持するために確保するバッファの長さです。バイト数ではなく文字数で指定してください。
                                ///< 0のときは、textLengthで指定された長さを設定します。レイアウトエディタの「文字列の長さを強制的に指定する」機能で指定
                                ///< された長さを使いたい場合、forceAssignedTextLengthをコピーしてください。
        int32_t forceAssignedTextLength;    ///< レイアウトエディタで「文字列の長さを強制的に指定する」のチェックが入っている場合、このメンバに指定された
                                        ///< 文字数が入った状態でSearchTextに渡されます。チェックが入っていない場合は-1が設定されます。
                                        ///< 出力としては使われません。

        //! @brief デフォルト値に設定します。
        //!
        void SetDefault()
        {
            pText = NULL;
            textLength = 0;
            bufferLength = 0;
            forceAssignedTextLength = -1;
        }
    };

    //! @brief デストラクタです。
    //!
    virtual ~TextSearcher() {}

    //! @brief テキストIDからテキストの検索を行います。UTF-16版。
    //!
    //! @details
    //! LayoutクラスのBuildメソッド内で、引数にTextSearcherを設定したときに、TextBoxそれぞれについて呼び出されます。
    //!
    //! TextBoxにテキストIDが設定していない場合も呼び出されますのでご注意ください。その場合、引数messageIdはNULLと
    //! なります。
    //!
    //! 指定されたテキストIDに対応するテキストを探し、引数pTextInfoに設定してください。TextInfoの引数textを設定せずに
    //! returnした場合は、レイアウトエディタで指定された文字列がテキストボックスに入ります。
    //!
    //! @param[in] pTextInfo         テキストの設定情報。メンバforceAssignedTextLengthは入力で、それ以外は設定の出力となります。
    //!                             設定方法については @ref TextInfo をご参照ください。
    //! @param[in] messageId        テキストボックスに設定されたテキストID。テキストIDが設定されていない場合はNULLとなります。
    //! @param[in] pLayout           テキストボックスを保持するレイアウト。ただし構築中であることにご注意ください。GetNameは使用できます。
    //! @param[in] pTextBox          テキストを設定するテキストボックス
    //! @param[in] pResourceLayout   テキストボックスを構築するときに使用しているリソースを保持しているレイアウト。
    //!                             部品のプロパティを使用して上書きしたときに、その上書きを行ったレイアウトのポインタが渡されます。
    //!                             それ以外の場合は、引数pLayoutと同じポインタが渡されます。
    //!
    virtual void SearchText(TextInfo* pTextInfo, const char* messageId, Layout* pLayout, TextBox* pTextBox, Layout* pResourceLayout)
    {
        NN_SDK_ASSERT(false, "A function which was not overridden was called.");

        NN_UNUSED(pTextInfo);
        NN_UNUSED(messageId);
        NN_UNUSED(pLayout);
        NN_UNUSED(pTextBox);
        NN_UNUSED(pResourceLayout);
    }

    //! @brief テキストIDからテキストの検索を行います。UTF-8版。
    //!
    //! @details
    //! LayoutクラスのBuildメソッド内で、引数にTextSearcherを設定したときに、TextBoxそれぞれについて呼び出されます。
    //!
    //! TextBoxにテキストIDが設定していない場合も呼び出されますのでご注意ください。その場合、引数messageIdはNULLと
    //! なります。
    //!
    //! 指定されたテキストIDに対応するテキストを探し、引数pTextInfoに設定してください。TextInfoの引数textを設定せずに
    //! returnした場合は、レイアウトエディタで指定された文字列がテキストボックスに入ります。
    //!
    //! @param[in] pTextInfo         テキストの設定情報。メンバforceAssignedTextLengthは入力で、それ以外は設定の出力となります。
    //!                             設定方法については @ref TextInfo をご参照ください。
    //! @param[in] messageId        テキストボックスに設定されたテキストID。テキストIDが設定されていない場合はNULLとなります。
    //! @param[in] pLayout           テキストボックスを保持するレイアウト。ただし構築中であることにご注意ください。GetNameは使用できます。
    //! @param[in] pTextBox          テキストを設定するテキストボックス
    //! @param[in] pResourceLayout   テキストボックスを構築するときに使用しているリソースを保持しているレイアウト。
    //!                             部品のプロパティを使用して上書きしたときに、その上書きを行ったレイアウトのポインタが渡されます。
    //!                             それ以外の場合は、引数pLayoutと同じポインタが渡されます。
    //!
    virtual void SearchTextUtf8(TextInfoUtf8* pTextInfo, const char* messageId, Layout* pLayout, TextBox* pTextBox, Layout* pResourceLayout)
    {
        NN_SDK_ASSERT(false, "A function which was not overridden was called.");

        NN_UNUSED(pTextInfo);
        NN_UNUSED(messageId);
        NN_UNUSED(pLayout);
        NN_UNUSED(pTextBox);
        NN_UNUSED(pResourceLayout);
    }
};

} // namespace nn::ui2d
} // namespace nn
