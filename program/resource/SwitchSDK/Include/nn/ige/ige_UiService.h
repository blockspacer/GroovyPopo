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
#include <nn/ige/ige_EditService.h>
#include <nn/ige/ige_ControlFactory.h>

#include <nn/ige/ige_Button.h>
#include <nn/ige/ige_Slider.h>
#include <nn/ige/ige_CheckBox.h>
#include <nn/ige/ige_GroupBox.h>
#include <nn/ige/ige_TextBox.h>
#include <nn/ige/ige_Label.h>
#include <nn/ige/ige_Panel.h>

namespace nn { namespace ige {
namespace detail { class IgeAllocator; }

//! @briefprivate   クライアントと接続し、クライアントに値の編集が可能な UI を提供するサービスです。
//!                 クライアント側では Nintendo.InGameEditing.Ui.dll 内の UiService クラスを使用することを前提とします。
//! @remarks        キーを含む全ての文字列は UTF-8 として扱われます。
//!                 サービスまたは子要素の構築時に与えられた文字列やバッファの領域は、
//!                 特に記述がない限り、サービスまたは子要素の破棄時まで保持する必要があります。
class UiService : public ControlFactory
{
    friend class ControlFactory;
    NN_DISALLOW_COPY(UiService);

public:
    //! @briefprivate  初期化引数です。
    class InitializeArg
    {
    public:
        InitializeArg() NN_NOEXCEPT
            : portName()
            , allocateFunc(NULL)
            , freeFunc(NULL)
            , pAllocateUserData(NULL)
            , pFreeUserData(NULL)
        {
        }

    public:
        util::string_view portName;                         //!< 通信に使用するポート名を指定します。31文字以下である必要があります。
        AlignedAllocateFunctionWithUserData allocateFunc;   //!< アロケーション関数を指定します。
        FreeFunctionWithUserData freeFunc;                  //!< フリー関数を指定します。
        void* pAllocateUserData;                            //!< アロケーション関数に渡される任意のデータを指定します。
        void* pFreeUserData;                                //!< フリー関数に渡される任意のデータを指定します。
    };

    //! @briefprivate   コンストラクタです。
    UiService() NN_NOEXCEPT;

    //! @briefprivate   デストラクタです。
    virtual ~UiService()  NN_NOEXCEPT NN_OVERRIDE;

    //! @briefprivate   サービスの初期化を行います。
    //! @param[in]  arg 初期化引数です。
    //! @pre            arg.portName     != ""
    //! @pre            arg.allocateFunc != NULL
    //! @pre            arg.freeFunc     != NULL
    //! @details        初期化前に構築したノードがある場合、すべて破棄されます。
    void Initialize(InitializeArg arg) NN_NOEXCEPT;

    //! @briefprivate   サービスが初期化済みであるかどうかを取得します。
    bool IsInitialized() const NN_NOEXCEPT;

    //! @briefprivate   サービスの終了処理を行います。
    //! @details        構築済みの全てのノードが破棄されます。
    void Finalize() NN_NOEXCEPT;

    //! @briefprivate   通信チャンネルを開き、編集サービスを開始します。
    //! @return     サービスが開始できたかどうかを返します。
    //!             false を返した場合、ポート名の重複などによりサービスを開始できなかった可能性があります。
    //! @pre        IsInitialized() == true
    //! @pre        nn::htcs::IsInitialized() == true
    bool Start() NN_NOEXCEPT;

    //! @briefprivate   サービスを開始している状態かどうかを取得します。
    //! @return         サービスを開始している状態かどうかを取得します。
    bool IsStarted() const NN_NOEXCEPT;

    //! @briefprivate   通信チャンネルを閉じ、サービスを停止します。
    //! @details        構築済みのノードは破棄されません。
    void Stop() NN_NOEXCEPT;

    //! @briefprivate   サービスに対し、クライアントが接続済みの状態であるかどうかを取得します。
    //! @return         サービスに対し、クライアントが接続済みの状態であるかどうかを取得します。
    bool IsConnected() const NN_NOEXCEPT;

    //! @briefprivate   初期化時に指定されたポート名を取得します。
    //! @return         初期化時に指定されたポート名を取得します。
    const util::string_view& GetPortName() const NN_NOEXCEPT;

    //! @briefprivate   通信チャンネルの定期処理を行います。
    void Poll() NN_NOEXCEPT;

    //! @briefprivate   受信データの反映など、各ノードの状態を更新します。
    void Update() NN_NOEXCEPT;

    //! @briefprivate       初期化済みのノードを子要素として登録します。
    //! @param[in]  pNode   登録するノードです。
    //! @pre                pNode != NULL
    //! @pre                IsRegisterd() == true
    //! @pre                pNode.IsRegisterd() == false
    void Register(Node* pNode) NN_NOEXCEPT;

    //! @briefprivate       子ノードの登録を解除します。
    //! @param[in]  pNode   登録を解除する子ノードです。
    //! @pre                pNode != NULL
    //! @pre                IsRegisterd() == true
    void Unregister(Node* pNode) NN_NOEXCEPT;

private:
    void FinalizeUiService() NN_NOEXCEPT;
private:
    EditService m_EditService;
};

}} // namespace nn::ige
