/*--------------------------------------------------------------------------------*
  Copyright (C)Nintendo All rights reserved.

  These coded instructions, statements, and computer programs contain proprietary
  information of Nintendo and/or its licensed developers and are protected by
  national and international copyright laws. They may not be disclosed to third
  parties or copied or duplicated in any form, in whole or in part, without the
  prior written consent of Nintendo.

  The content herein is highly confidential and should be handled accordingly.
 *--------------------------------------------------------------------------------*/

/**
 * @file
 * @brief   ソフトウェアキーボードのインラインモードに関する API の宣言
 */

#pragma once

#include <nn/nn_Result.h>

#include "swkbd_Types.h"
#include "swkbd_InlineTypes.h"


/**
 * @namespace nn::swkbd
 * @brief ソフトウェアキーボードアプレット呼び出しの名前空間です。
 */

namespace nn { namespace swkbd {


    namespace detail {
        class InlineKeyboardImpl;
    }

    /**
     * @brief ソフトウェアキーボードをインラインモードで使用するためのクラスです。
     */
    class InlineKeyboard
    {

        NN_DISALLOW_COPY( InlineKeyboard );

    public:

        //! @name インラインモードの設定
        //! @{

        InlineKeyboard() NN_NOEXCEPT;
        ~InlineKeyboard() NN_NOEXCEPT;


        /**
         * @brief ソフトウェアキーボードをインラインモードで使用するのに必要なバッファサイズを取得します。
         *
         * @return 要求するメモリサイズを返します。
         */
        static size_t GetRequiredWorkBufferSize() NN_NOEXCEPT;

        //------------------------------------------------------------------------
        /**
         * @brief ソフトウェアキーボードをインラインモードで初期化します。
         *
         * @pre
         * - pWorkBuf != nullptr
         *
         * @param[in]  pWorkBuf バッファの先頭ポインタです。@n
         *                      nn::swkbd::InlineKeyboard::GetRequiredWorkBufferSize() で取得できるバイトサイズを指定してください。
         *
         * @return
         *  ソフトウェアキーボード初期化結果を返します。@n
         *  特に問題なければ true を返します。@n
         *  二重に初期化しようとした場合は false を返します。
         *
         * @details エラービューアなど他のアプレットを起動中に本関数を呼び出した場合、起動中のアプレットが終了するまで本関数は内部で待機し、 @n
         *          先に起動中のアプレットが終了すると待機が解除されます。@n
         *          逆に Initialize() 呼び出し後は他のアプレットの起動を伴う関数を呼び出しても
         *          Finalize() が呼び出されるまでアプレットが起動しない点にご注意ください。@n
         *          また、ユーザ操作などの外部要因によって Finalize が呼び出された時と同じ状態に自動的になる場合があります。
         *          この場合 SetFinishedKeyboardCallback() で設定したコールバックが呼び出されるので、
         *          再度 Initialize() しなおすなど適切なハンドリングを行ってください。
         */
        bool Initialize( void* pWorkBuf ) NN_NOEXCEPT;


        /**
         * @brief インラインモードのソフトウェアキーボードを終了します。
         *
         * @pre
         * - ソフトウェアキーボードが初期化済み
         *
         * @details 本関数内で Initialize 時に指定していたバッファを解放します。
         *          ソフトウェアキーボードを使用しなくなる際は
         *          本関数を実行してください。
         */
        void Finalize() NN_NOEXCEPT;

        /// @cond Launch
        /**
         * @brief ソフトウェアキーボードをインラインモードで起動します。
         * @deprecated 本 API は隠蔽予定です。
         *
         * @return
         *  ソフトウェアキーボード起動結果を返します。@n
         *  特に問題なければ true を返します。@n
         *  二重に起動しようとした場合は false を返します。
         *
         * @details 本 API を呼ぶと、ソフトウェアキーボードはインラインモードで表示準備を行います。@n
         *          この際、InitializeArg で設定した値以外は一度リセットされます。@n
         *          SetCustomizedDictionaries 関数や SetUserWordInfo 関数は、ソフトウェアキーボード初期化完了後に呼び出す必要があるため、
         *          FinishedInitializeCallback 関数が呼び出された後に実行してください。
         */
        NN_DEPRECATED bool Launch() NN_NOEXCEPT;
        /// @endcond

        /**
         * @brief ソフトウェアキーボード全体の音量を設定します。@n
         *        インラインモードで初期化したソフトウェアキーボードは
         *        nn::oe::SetExpectedVolumeBalance には従いません。@n
         *        そのため、例えばソフトウェアキーボード表示時にアプリ側の音量が下がるなどの対応は入らないため、
         *        アプリ側の音量はアプリ自身で制御してください。
         *
         * @param[in] volume  音量です。@n
         *                    0.0f ～ 2.0f の範囲で指定してください。@n
         *                    デフォルトは 1.0f です。
         */
        void SetVolume( float volume ) NN_NOEXCEPT;

        /**
         * @brief インラインモードのソフトウェアキーボードを表示します。
         *
         * @param[in] arg ソフトウェアキーボード表示用パラメータです。
         */
        void Appear( const AppearArg& arg ) NN_NOEXCEPT;

        /**
         * @brief インラインモードのソフトウェアキーボードを非表示にします。
         */
        void Disappear() NN_NOEXCEPT;

        /**
         * @brief 入力テキストとして入力されている文字列情報をソフトウェアキーボードに渡します。
         *
         * @param[in] info 入力テキストとして入力されている文字列情報用パラメータです。
         */
        void SetInputText( const InputText& info ) NN_NOEXCEPT;

        /**
         * @brief テキストカーソル位置をソフトウェアキーボードに渡します。
         *
         * @param[in] cursorPos 入力テキスト先頭からの文字数であらわされたテキストカーソルの位置を指定します。@n
         *                      範囲外の位置を指定したら範囲内に補正されます。
         */
        void SetCursorPos( int32_t cursorPos ) NN_NOEXCEPT;

        /**
         * @brief ユーザ辞書をソフトウェアキーボードに渡します。
         *
         * @param[in] info ユーザ辞書用パラメータです。
         *
         * @return 問題なければtrue、設定できなかった時はfalseを返します。
         *
         * @pre
         * - ソフトウェアキーボードが初期化済み
         * - info.pWorkBuf != nullptr
         *
         * @details - FinishedInitializeCallback が呼び出されるまでは、@n
         *            ソフトウェアキーボードは初期化途中のため、表示要求に対応できない可能性があります。@n
         *            FinishedInitializeCallback 関数が呼び出された後に実行してください。@n
         *          - 本関数で渡したユーザ情報はソフトウェアキーボード側のメモリにコピーされます。@n
         *            コピーが完了すると SetReleasedUserWordInfoCallback() にて指定したコールバック関数が呼び出されますので、@n
         *            再度ユーザ辞書を登録する場合は、SetReleasedUserWordInfoCallback() にて指定したコールバック関数が呼び出された後に実行してください。
         *          - キーボード表示中は設定できません。
         */
        bool SetUserWordInfo( const UserWordInfo& info ) NN_NOEXCEPT;

        /**
         * @brief ユーザ辞書の設定を解除します。
         * @return 解除時にtrue、解除できなかった時はfalseを返します。
         * @details キーボード表示中は解除できません。
         */
        bool UnsetUserWordInfo() NN_NOEXCEPT;

        /**
         * @brief ソフトウェアキーボードが返す文字列を UTF-8 にするかどうかを指定します。
         *
         * @param[in] isUseUtf8 ソフトウェアキーボードが返す文字列を UTF-8 にするかどうかのフラグです。@n
         *                      true にすると、ソフトウェアキーボードが返す文字列が UTF-8 になります。@n
         *                      false にすると、ソフトウェアキーボードが返す文字列が UTF-16 になります。@n
         *                      デフォルトは false です。
         *
         * @details 本関数で true を渡すと、文字列が返るコールバック関数は全て UTF-8 版のものが返るようになります。@n
         *          UTF-16 版のコールバック関数には値が返らないため、ご注意ください。
         */
        void SetUtf8Mode( bool isUseUtf8 ) NN_NOEXCEPT;

        /**
        * @brief    カスタマイズ辞書をソフトウェアキーボードに渡します。
        * @param[in] dicSet カスタマイズ辞書セット
        * @return   問題なければ true、設定できなかった場合は false を返します。
        * @pre
        *   - ソフトウェアキーボードが初期化されている（FinishedInitializeCallback が呼び出されている）こと
        *   - dicSet が正しく設定されていること
        *   - すでにカスタマイズ辞書が設定されていないこと
        *   - ソフトウェアキーボードは非表示状態になっていること
        * @details CustomizedDictionarySet::pDictionaries で指定されたメモリは InlineKeyboard の管理化となるため、解放や他の用途に利用しないでください。@n
        *          このメモリが InlineKeyboard の管理化から外れるのは以下のいずれかです。@n
        *          - State_None で UnsetCustomizedDictionaries を呼び出したとき
        *          - State_None 以外で UnsetCustomizedDictionaries を呼び出し、その後 State_None になった時
        */
        bool SetCustomizedDictionaries( const CustomizedDictionarySet& dicSet ) NN_NOEXCEPT;

        /**
        * @brief    カスタマイズ辞書の解除要求を行います。
        * @return   解除要求できた場合は true、できなかった時は false を返します。
        * @pre
        *   - ソフトウェアキーボードは非表示状態になっていること
        * @details  SetCustomizedDictionaries() で指定されたメモリが InlineKeyboard の管理化から外れるのは以下のいずれかです。@n
        *           - State_None で UnsetCustomizedDictionaries を呼び出したとき
        *           - State_None 以外で UnsetCustomizedDictionaries を呼び出し、その後 State_None になった時
        */
        bool UnsetCustomizedDictionaries() NN_NOEXCEPT;

        /// @cond SetUnsetCustomizeDic
        /**
         * @brief カスタマイズ辞書をソフトウェアキーボードに渡します（廃止予定）。
         *
         * @deprecated このメソッドは廃止予定です。SetCustomizedDictionaries() を使用してください。
         *
         * @param[in] pBuffer カスタマイズ辞書データが格納されたバッファです。
         * @param[in] size    カスタマイズ辞書のバッファサイズです。@n
         *                    nn::os::MemoryPageSize でアライメントされたものでなければなりません。
         * @param[in] info    カスタマイズ辞書用パラメータです。
         *
         * @return 問題なければtrue、設定できなかった時はfalseを返します。
         *
         * @pre
         * - ソフトウェアキーボードが初期化済み
         * - pBuffer != nullptr
         *
         * @details 一度カスタマイズ辞書をセットしたら、
         *          カスタマイズ辞書が解放されない限り、単語の追加・削除・カスタマイズ辞書の再設定はできません。@n
         *          カスタマイズ辞書が解放されるのは、以下のタイミングです。@n
         *          - UnsetCustomizeDic() を呼ぶ@n
         *          - Finalize() を呼ぶ@n
         *          - ソフトウェアキーボードが終了する(FinishedKeyboardCallback が呼ばれる)@n
         *          FinishedInitializeCallback が呼び出されるまでは、
         *          ソフトウェアキーボードは初期化途中のため、表示要求に対応できない可能性があります。@n
         *          FinishedInitializeCallback 関数が呼び出された後に実行してください。
         *          - キーボード表示中は設定できません。
         */
        NN_DEPRECATED bool SetCustomizeDic( const void* pBuffer, size_t size, const CustomizeDicInfo& info ) NN_NOEXCEPT;

        /**
         * @brief カスタマイズ辞書の解除要求を行います（廃止予定）。
         * @deprecated このメソッドは廃止予定です。UnsetCustomizedDictionaries() を使用してください。
         * @return 要求を登録できた場合はtrue、できなかった時はfalseを返します。
         * @details キーボード表示中は解除できません。
         *          実際の解除にはこのあとで Calc を呼び出す必要があり、その後、解除処理は遅延実行されます。
         */
        NN_DEPRECATED bool UnsetCustomizeDic() NN_NOEXCEPT;
        /// @endcond

        /**
         * @brief キートップのアルファ値を設定します。
         *
         * @param[in] alpha    キートップのアルファ値です。0～1 の範囲で設定してください。
         */
        void SetKeytopAlpha( float alpha ) NN_NOEXCEPT;

        /**
         * @brief フッターのアルファ値を設定します。
         *
         * @param[in] alpha フッターのアルファ値です。0～1 の範囲で設定してください。
         *
         * @details フローティングモードの場合のみ有効です。@n
         *          フローティングモードでない場合には、SetKeytopAlpha で設定した値がフッターにも適用されます。
         */
        void SetFooterAlpha( float alpha ) NN_NOEXCEPT;

        /**
         * @brief キートップの Scale 値を設定します。
         *
         * @param[in] scale    キートップに掛けるスケール値です。
         *
         * @details 画面の中央を軸にスケールします。
         */
        void SetKeytopScale( float scale ) NN_NOEXCEPT;

        /**
         * @brief キートップの Translate 値を設定します。
         *
         * @param[in] translateX    キートップに掛ける X 方向の Translate 値です。
         * @param[in] translateY    キートップに掛ける Y 方向の Translate 値です。
         *
         * @details X 座標は右方向、Y 座標は上方向を正とする座標系です。@n
         *          縦横方向ともに、1.0 で1画面分移動します。@n
         *          -0.5～0.5の範囲で値を設定してください。
         */
        void SetKeytopTranslate( float translateX, float translateY ) NN_NOEXCEPT;


        /**
         * @brief キートップとフッターを分離するかシングルウィンドウにするかの設定値です。
         */
        enum WindowMode
        {
            WindowMode_Single,   //!< @brief フッターとキートップが一緒にスケール＆移動するシングルウィンドウモードです。デフォルトの設定です。
            WindowMode_Floating, //!< @brief フッターとキートップが分離するフローティングモードです。
            WindowMode_AdjacentFloating //!< @brief フローティングモードでフッターとキートップを隣接させる用のモードです。キートップの下部の枠を表示しません。
        };

        /**
         * @brief キートップとフッター表示を切り離した見た目にするかどうかを設定します。
         *
         * @param[in] mode   キートップとフッターの表示の仕方を決めるウィンドウモードです。
         *
         * @details フッターとキートップを分離して表示するかどうかを設定します。@n
         *          デフォルトの設定は WindowMode_Single です。@n
         *          WindowMode_Floating または WindowMode_AdjacentFloating にすると、フッターは画面下部に固定サイズで表示されます。@n
         *          WindowMode_Single の場合は、フッターもキートップと一体化してスケールします。@n
         *          キートップのみをスケールはするものの、見た目上フッターと隣接させて表示させるので、
         *          キートップ下部の枠が不要であるような場合には、WindowMode_AdjacentFloating に設定することで下部の枠を表示させないことができます。
         */
        void SetWindowMode( WindowMode mode ) NN_NOEXCEPT;

        /**
         * @brief タッチ操作の有効無効を設定します。
         *
         * @param[in] enabled タッチ操作を有効にするかどうかを設定します。
         *
         * @details デフォルトの設定では有効になっています。
         */
        void SetTouchEnabled( bool enabled ) NN_NOEXCEPT;

        /**
         * @brief USB キーボードからの入力の有効無効を設定します。
         *
         * @param[in] enabled USB キーボードからの入力を有効にするかどうかを設定します。
         *
         * @details デフォルトの設定では有効になっています。
         */
        void SetUsbKeyboardEnabled( bool enabled ) NN_NOEXCEPT;

        /**
         * @brief SWKBD がタッチトリガで使用する矩形を取得します。
         *
         * @param[out] primary   1つめの矩形を取得する出力アドレスです。
         * @param[out] secondary 2つめの矩形を取得する出力アドレスです。(フローティングモード時のみ)
         *
         * @return 矩形の数を返します。
         *
         * @details SWKBD が画面を覆う矩形の情報を取得します。@n
         *          タッチパネルのマスクでの使用を想定していますので、座標系は左上原点の (0,0)-(1280,720) のドット単位での座標を返します。@n
         *          secondary は SetWindowMode で WindowMode_Single 以外に設定している場合にのみ値を返します。
         *          SetTouchEnabled の設定に関わらず、矩形を返します。
         */
        int GetTouchRectangles( Rect* primary, Rect* secondary ) const NN_NOEXCEPT;

        /**
         * @brief ソフトウェアキーボードのメイン処理です。ゲームフレーム毎に呼んでください。
         *
         * @return ソフトウェアキーボードの表示状態を返します。@n
         *         SetFinishedInitializeCallback などのコールバックは
         *         本関数を呼ぶことにより実行判定が行われるため、
         *         ソフトウェアキーボードが終了するまでは、本関数をゲームフレーム毎に呼んでください。
         */
        State Calc() NN_NOEXCEPT;

        /**
         * @brief 現時点で設定したパラメータを元に、ソフトウェアキーボードが使用する画面領域の最大の高さ（ピクセル）を返します。
         *
         * @return 画面下部からの、ソフトウェアキーボードが使用する画面領域の最大の高さ（ピクセル）を返します。
         *
         * @details ソフトウェアキーボードが使用する画面領域の最大の高さは、盤面の種類・予測変換機能の使用有無・ユーザ辞書の使用有無・カスタマイズ辞書の使用有無で変わります。@n
         *          上記に関連する設定を変更した際は、適宜本 API を呼んで変更がないか確認してください。
         */
        int GetMaxHeight() NN_NOEXCEPT;

        /**
         * @brief 指定した座標がキーボードに使用されているかどうかを判定します。
         *
         * @param[in] x タッチの x 座標です。
         * @param[in] y タッチの y 座標です。
         *
         * @return 指定した座標がキーボードに使用されているかどうかを返します。
         */
        bool IsUsedTouchPointByKeyboard( int32_t x, int32_t y ) NN_NOEXCEPT;

        //! @}

        //! @name インラインモード コールバック処理
        //! @{


        /**
         * @brief 初期化終了情報取得コールバックを設定します。
         *
         * @param[in] pCallback 初期化終了情報取得コールバック関数へのポインタを指定します。
         *
         * @details 設定したコールバックは Calc() の中で呼び出されます。@n
         *          Initialize() を実行後にコールバックが呼び出されるため、
         *          メソッドを呼び出す前にコールバック関数を設定しておく必要があります。
         */
        void SetFinishedInitializeCallback( FinishedInitializeCallback pCallback ) NN_NOEXCEPT;


        /**
         * @brief ソフトウェアキーボード終了時コールバックを設定します。
         *
         * @param[in] pCallback ソフトウェアキーボード終了時コールバック関数へのポインタを指定します。
         *
         * @details 設定したコールバックは Calc() や GetImage() の中で呼び出されます。@n
         *          ユーザが意図的に終了した時以外にも、アプリが意図せぬタイミングで
         *          ソフトウェアキーボードが終了する場合があります。@n
         *          その際は本関数で指定したコールバック関数が呼ばれます。@n
         *          具体的には HOMEボタン を押し、HOMEメニューから本体設定などを起動したあと
         *          アプリが Calc() や GetImage() を呼び出すとコールバックが発生します。
         */
        void SetFinishedKeyboardCallback( FinishedKeyboardCallback pCallback ) NN_NOEXCEPT;


        /**
         * @brief 入力テキストとして入力されている文字列情報取得コールバックを設定します。UTF-16版。
         *
         * @param[in] pCallback 入力テキストとして入力されている文字列情報取得コールバック関数へのポインタを指定します。
         *
         * @details 設定したコールバックは Calc() の中で呼び出されます。
         */
        void SetChangedStringCallback( ChangedStringCallback pCallback ) NN_NOEXCEPT;


        /**
         * @brief 入力テキストとして入力されている文字列情報取得コールバックを設定します。UTF-8版。
         *
         * @param[in] pCallback 入力テキストとして入力されている文字列情報取得コールバック関数へのポインタを指定します。
         *
         * @details 設定したコールバックは Calc() の中で呼び出されます。
        */
        void SetChangedStringCallbackUtf8( ChangedStringCallbackUtf8 pCallback ) NN_NOEXCEPT;


        /**
         * @brief テキストカーソル位置情報取得コールバックを設定します。UTF-16版。
         *
         * @param[in] pCallback テキストカーソル位置情報取得コールバック関数へのポインタを指定します。
         *
         * @details 設定したコールバックは Calc() の中で呼び出されます。
         */
        void SetMovedCursorCallback( MovedCursorCallback pCallback ) NN_NOEXCEPT;


        /**
        * @brief テキストカーソル位置情報取得コールバックを設定します。UTF-8版。
        *
        * @param[in] pCallback テキストカーソル位置情報取得コールバック関数へのポインタを指定します。
        *
        * @details 設定したコールバックは Calc() の中で呼び出されます。
        */
        void SetMovedCursorCallbackUtf8( MovedCursorCallbackUtf8 pCallback ) NN_NOEXCEPT;


        /**
         * @brief 確定閉じ情報取得コールバックを設定します。UTF-16版。
         *
         * @param[in] pCallback 確定閉じ情報取得コールバック関数へのポインタを指定します。
         *
         * @details 設定したコールバックは Calc() の中で呼び出されます。@n
         *          OK ボタンを押してソフトウェアキーボードが閉じた際に呼び出されます。
         */
        void SetDecidedEnterCallback( DecidedEnterCallback pCallback ) NN_NOEXCEPT;


        /**
        * @brief 確定閉じ情報取得コールバックを設定します。UTF-8版。
        *
        * @param[in] pCallback 確定閉じ情報取得コールバック関数へのポインタを指定します。
        *
        * @details 設定したコールバックは Calc() の中で呼び出されます。@n
        *          OK ボタンを押してソフトウェアキーボードが閉じた際に呼び出されます。
        */
        void SetDecidedEnterCallbackUtf8( DecidedEnterCallbackUtf8 pCallback ) NN_NOEXCEPT;


        /**
         * @brief キャンセル閉じ情報取得コールバックを設定します。
         *
         * @param[in] pCallback キャンセル閉じ情報取得コールバック関数へのポインタを指定します。
         *
         * @details 設定したコールバックは Calc() の中で呼び出されます。@n
         *          キャンセルボタンを押してソフトウェアキーボードが閉じた際に呼び出されます。
         */
        void SetDecidedCancelCallback( DecidedCancelCallback pCallback ) NN_NOEXCEPT;


        /**
         * @brief ユーザ辞書情報の解放情報取得コールバックを設定します。
         *
         * @param[in] pCallback ユーザ辞書情報の解放情報取得コールバック関数へのポインタを指定します。
         *
         * @details 設定したコールバックは Calc() の中で呼び出されます。@n
         *          SetUserWordInfo() で渡したユーザ辞書情報がソフトウェアキーボードにコピーし終えた際に呼び出されます。
         */
        void SetReleasedUserWordInfoCallback( ReleasedUserWordInfoCallback pCallback ) NN_NOEXCEPT;


        //! @}


        //! @name インラインモード 描画結果の取得
        //! @{

        /**
         * @brief ソフトウェアキーボードのレンダリング結果の画像が要求する画面サイズを取得します。
         *
         * @param[out] pWidth  画像が要求する画像の幅（ピクセル）
         * @param[out] pHeight 画像が要求する画像の高さ（ピクセル）
         *
         * @pre
         * - pWidth != nullptr
         * - pHeight != nullptr
         *
         */
        void GetWindowSize( int* pWidth, int* pHeight ) const NN_NOEXCEPT;

        /**
         * @brief ソフトウェアキーボードのレンダリング結果の画像を取得するために必要なバッファの大きさやアライメントの制約を取得します。
         *
         * @param[out] pOutRequiredSize      画像を取得するのに必要なバッファの大きさ（バイト）を受け取る変数のポインタ
         * @param[out] pOutRequiredAlignment 画像を取得するのに必要なバッファのアライメント（バイト）を受け取る変数のポインタ
         *
         * @pre
         * - pOutRequiredSize != nullptr
         * - pOutRequiredAlignment != nullptr
         */
        void GetImageMemoryRequirement( size_t* pOutRequiredSize,
            size_t* pOutRequiredAlignment ) NN_NOEXCEPT;

        /**
         * @brief ソフトウェアキーボードのレンダリング結果の画像を取得します。
         *
         * @param[in] pBuffer    画像を取得するバッファのポインタ
         * @param[in] bufferSize 画像を取得するバッファの大きさ（バイト）
         *
         * @pre
         * - pBuffer != nullptr
         * - pBuffer が GetImageGetImageMemoryRequirement 関数で要求されたバッファの大きさやアライメントの制約に従っている
         * - bufferSize が GetImageMemoryRequirement 関数の pOutRequiredSize と等しい
         *
         * @return ソフトウェアキーボードのレンダリング結果の画像が取得できたかどうかを返します。
         *
         * @details 画像データは以下の状態で取得されます。@n
         *          - 線形タイリングされている@n
         *          - 原点は左上@n
         *          - 各チャンネル 8 ビットの sRGB 色空間の RGBA 画像@n
         */
        bool GetImage( void* pBuffer, size_t bufferSize ) NN_NOEXCEPT;

        //! @}

    protected:
        detail::InlineKeyboardImpl* m_pImpl;
        void* m_pWorkBuf;
    };

}} // namespace nn::swkbd

