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

#include <nn/g3d/viewer/g3d_ViewerDefine.h>

#include "g3d_ViewerCallback.h"

#include <nn/nn_Common.h>
#include <nn/nn_Allocator.h>
#include <nn/g3d/g3d_ResCommon.h>
#include <nn/util/util_Vector.h>
#include <nn/util/util_VectorApi.h>

// HTC が無効の場合は実装を空にします
#if !NN_DETAIL_G3D_VIEWER_CONFIG_IS_VIEWER_EMPTY
#define NN_DETAIL_G3D_VIEWER_CALL_PROXY(func) func
#define NN_DETAIL_G3D_VIEWER_CALL_PROXY_RESULT(func) func
#define NN_DETAIL_G3D_VIEWER_CALL_PROXY_BOOL(func) func
#define NN_DETAIL_G3D_VIEWER_CALL_PROXY_FLOAT(func) func
#define NN_DETAIL_G3D_VIEWER_CALL_PROXY_INT(func) func
#define NN_DETAIL_G3D_VIEWER_CALL_PROXY_POINTER(func) func
#else
#define NN_DETAIL_G3D_VIEWER_CALL_PROXY(func) static_cast<void>(0)
#define NN_DETAIL_G3D_VIEWER_CALL_PROXY_RESULT(func) ViewerResult_ViewerDisabled
#define NN_DETAIL_G3D_VIEWER_CALL_PROXY_BOOL(func) false
#define NN_DETAIL_G3D_VIEWER_CALL_PROXY_FLOAT(func) 0.0f
#define NN_DETAIL_G3D_VIEWER_CALL_PROXY_INT(func) 0
#define NN_DETAIL_G3D_VIEWER_CALL_PROXY_POINTER(func) nullptr
#endif

namespace nn { namespace g3d {
    class ModelObj;
    class ResShaderArchive;
}}

namespace nn { namespace g3d { namespace viewer {
    class ViewerCallbacks;

    //! @brief 3DEditor と通信を行うサーバークラスです。
    class ViewerServer
    {
        NN_DISALLOW_COPY(ViewerServer);
    public:
        //! @brief サーバー初期化時に渡す引数です。
        class InitializeArg
        {
            friend class ViewerServer;
        public:
            //! @brief コンストラクタです。
            //!
            //! @param[in] pDevice デバイスへのポインタです。
            //! @param[in] allocateMemoryFunction サーバーがメモリを確保する際にコールするメモリ確保関数です。
            //! @param[in] freeMemoryFunction サーバーがメモリを解放する際にコールするメモリ解放関数です。
            //!
            //! @pre
            //! - pDevice != nullptr
            //! - pDevice が初期化されている。
            //! - allocateMemoryFunction != nullptr
            //! - freeMemoryFunction != nullptr
            //!
            NN_DETAIL_G3D_VIEWER_API
                InitializeArg(
                nn::gfx::Device* pDevice,
                const nn::AlignedAllocateFunctionWithUserData allocateMemoryFunction,
                const nn::FreeFunctionWithUserData freeMemoryFunction) NN_NOEXCEPT
                : m_pDevice(pDevice)
                , m_AllocateFunction(allocateMemoryFunction)
                , m_pAllocateFunctionUserData(nullptr)
                , m_FreeFunction(freeMemoryFunction)
                , m_pFreeUserData(nullptr)
                , m_CallbackFunction(nullptr)
                , m_pCallbackFunctionUserData(nullptr)
                , m_TextureBindCallback(ViewerDefaultTextureBindCallback)
                , m_pTextureBindCallbackUserData(nullptr)
                , m_CodePage(0)
            {
                NN_SDK_REQUIRES_NOT_NULL(pDevice);
                NN_SDK_REQUIRES(pDevice->ToData()->state == pDevice->ToData()->State_Initialized);
                NN_SDK_REQUIRES_NOT_NULL(allocateMemoryFunction);
                NN_SDK_REQUIRES_NOT_NULL(freeMemoryFunction);
            }

            //! @brief コンストラクタです。
            //!
            //! @param[in] pDevice デバイスへのポインタです。
            //! @param[in] allocateMemoryFunction サーバーがメモリを確保する際にコールするメモリ確保関数です。
            //! @param[in] freeMemoryFunction サーバーがメモリを解放する際にコールするメモリ解放関数です。
            //! @param[in] pMemoryFunctionUserData メモリ確保、解放関数に渡される任意のデータです。
            //!
            //! @pre
            //! - pDevice != nullptr
            //! - pDevice が初期化されている。
            //! - allocateMemoryFunction != nullptr
            //! - freeMemoryFunction != nullptr
            //!
            NN_DETAIL_G3D_VIEWER_API
                InitializeArg(
                nn::gfx::Device* pDevice,
                const nn::AlignedAllocateFunctionWithUserData allocateMemoryFunction,
                const nn::FreeFunctionWithUserData freeMemoryFunction,
                void* pMemoryFunctionUserData) NN_NOEXCEPT
                : m_pDevice(pDevice)
                , m_AllocateFunction(allocateMemoryFunction)
                , m_pAllocateFunctionUserData(pMemoryFunctionUserData)
                , m_FreeFunction(freeMemoryFunction)
                , m_pFreeUserData(pMemoryFunctionUserData)
                , m_CallbackFunction(nullptr)
                , m_pCallbackFunctionUserData(nullptr)
                , m_TextureBindCallback(ViewerDefaultTextureBindCallback)
                , m_pTextureBindCallbackUserData(nullptr)
                , m_CodePage(0)
            {
                NN_SDK_REQUIRES_NOT_NULL(pDevice);
                NN_SDK_REQUIRES(pDevice->ToData()->state == pDevice->ToData()->State_Initialized);
                NN_SDK_REQUIRES_NOT_NULL(allocateMemoryFunction);
                NN_SDK_REQUIRES_NOT_NULL(freeMemoryFunction);
            }

            //! @brief サーバーのコールバック関数を設定します。
            //!
            //! @param[in] callback 設定するコールバック関数です。
            //! @param[in] pUserData コールバック関数がコールされるときに渡される任意のデータへのポインタです。
            //!
            //! @details
            //! コールバッククラスと同時に設定することはできません。
            //!
            NN_DETAIL_G3D_VIEWER_API
                void SetCallback(ViewerCallback callback, void* pUserData) NN_NOEXCEPT
            {
                m_CallbackFunction = callback;
                m_pCallbackFunctionUserData = pUserData;
            }

            //! @brief サーバーのコールバッククラスを設定します。
            //!
            //! @param[in] pCallbacks 設定するコールバッククラスです。
            //!
            //! @details
            //! コールバック関数と同時に設定することはできません。
            //!
            NN_DETAIL_G3D_VIEWER_API
                void SetCallback(ViewerCallbacks* pCallbacks) NN_NOEXCEPT
            {
                if (pCallbacks == nullptr)
                {
                    m_CallbackFunction = nullptr;
                    m_pCallbackFunctionUserData = nullptr;
                    return;
                }

                m_CallbackFunction = CallViewerCallbacks;
                m_pCallbackFunctionUserData = pCallbacks;
            }

            //! @brief テクスチャバインド時のコールバック関数を設定します。
            //!
            //! @param[in] callback 設定するコールバック関数です。
            //! @param[in] pUserData コールバック関数がコールされるときに渡される任意のデータへのポインタです。
            //!
            //! @details
            //! 設定しない場合、ライブラリが用意したコールバック関数が使用されます。
            //!
            NN_DETAIL_G3D_VIEWER_API
                void SetTextureBindCallback(ViewerTextureBindCallback callback, void* pUserData) NN_NOEXCEPT
            {
                if (callback == nullptr)
                {
                    m_TextureBindCallback = ViewerDefaultTextureBindCallback;
                    m_pTextureBindCallbackUserData = pUserData;
                    return;
                }

                m_TextureBindCallback = callback;
                m_pTextureBindCallbackUserData =pUserData;
            }

            //! @brief サーバーが取り扱う文字列のコードページを設定します。
            //!
            //! @param[in] codePage 設定するコードページです。
            //!
            NN_DETAIL_G3D_VIEWER_API
                void SetCodePage(int codePage) NN_NOEXCEPT
            {
                m_CodePage = codePage;
            }

        private:

            nn::gfx::Device* m_pDevice;

            const nn::AlignedAllocateFunctionWithUserData m_AllocateFunction;
            void* m_pAllocateFunctionUserData;

            const nn::FreeFunctionWithUserData m_FreeFunction;
            void* m_pFreeUserData;

            ViewerCallback m_CallbackFunction;
            void* m_pCallbackFunctionUserData;

            ViewerTextureBindCallback m_TextureBindCallback;
            void* m_pTextureBindCallbackUserData;

            int m_CodePage;
        };


        //! @brief nn::g3d::viewer::ViewerServer::QueueLayoutModelCommand 実行時に渡す引数です。
        class LayoutModelArg
        {
            friend class ViewerServer;
        public:
            //! @brief コンストラクタです。
            //! @param[in] pModelObj 配置情報を設定するモデルです。
            //! @pre
            //! - pModelObj != nullptr
            explicit LayoutModelArg(const nn::g3d::ModelObj* pModelObj) NN_NOEXCEPT
                : m_pModelObj(pModelObj)
                , m_Scale(nn::util::Vector3f(1.0f, 1.0f, 1.0f))
                , m_Rotate(nn::util::Vector3f(0.0f, 0.0f, 0.0f))
                , m_Translate(nn::util::Vector3f(0.0f, 0.0f, 0.0f))
            {
                NN_SDK_REQUIRES_NOT_NULL(pModelObj);
            }

            //! @brief コンストラクタです。
            //! @param[in] pModelObj 配置情報を設定するモデルです。
            //! @param[in] scale 設定する配置情報のスケール値です。
            //! @param[in] rotate 設定する配置情報の回転値です。
            //! @param[in] translate 設定する配置情報の移動値です。
            //! @pre
            //! - pModelObj != nullptr
            LayoutModelArg(
                const nn::g3d::ModelObj* pModelObj,
                const nn::util::Vector3fType& scale,
                const nn::util::Vector3fType& rotate,
                const nn::util::Vector3fType& translate) NN_NOEXCEPT
                : m_pModelObj(pModelObj)
                , m_Scale(scale)
                , m_Rotate(rotate)
                , m_Translate(translate)
            {
                NN_SDK_REQUIRES_NOT_NULL(pModelObj);
            }

            //! @brief モデル配置情報のスケール値を設定します。
            //! @param[in] scale 設定するスケール値です。
            void SetScale(const nn::util::Vector3fType& scale) NN_NOEXCEPT
            {
                m_Scale = scale;
            }

            //! @brief モデル配置情報のスケール値を設定します。
            //! @param[in] sx 設定する X 軸の移動値です。
            //! @param[in] sy 設定する Y 軸の移動値です。
            //! @param[in] sz 設定する Z 軸の移動値です。
            void SetScale(float sx, float sy, float sz) NN_NOEXCEPT
            {
                nn::util::VectorSet(&m_Scale, sx, sy, sz);
            }

            //! @brief モデル配置情報の回転値を設定します。
            //! @param[in] rotate 設定する回転値です。
            void SetRotate(const nn::util::Vector3fType& rotate) NN_NOEXCEPT
            {
                m_Rotate = rotate;
            }

            //! @brief モデル配置情報の回転値を設定します。
            //! @param[in] rx 設定する X 軸の移動値です。
            //! @param[in] ry 設定する Y 軸の移動値です。
            //! @param[in] rz 設定する Z 軸の移動値です。

            void SetRotate(float rx, float ry, float rz) NN_NOEXCEPT
            {
                nn::util::VectorSet(&m_Rotate, rx, ry, rz);
            }

            //! @brief モデル配置情報の移動値を設定します。
            //! @param[in] translate 設定する移動値です。
            void SetTranslate(const nn::util::Vector3fType& translate) NN_NOEXCEPT
            {
                m_Translate = translate;
            }

            //! @brief モデル配置情報の移動値を設定します。
            //! @param[in] tx 設定する X 軸の移動値です。
            //! @param[in] ty 設定する Y 軸の移動値です。
            //! @param[in] tz 設定する Z 軸の移動値です。
            void SetTranslate(float tx, float ty, float tz) NN_NOEXCEPT
            {
                nn::util::VectorSet(&m_Translate, tx, ty, tz);
            }

        private:
            const ModelObj* m_pModelObj;   //!< モデル配置対象の ModelObj です。
            nn::util::Vector3fType m_Scale; //!< モデル配置対象に設定する Scale です。
            nn::util::Vector3fType m_Rotate; //!< モデル配置対象に設定する Rotate です。
            nn::util::Vector3fType m_Translate; //!< モデル配置対象に設定する Translate です。
        };

        //! @brief nn::g3d::viewer::ViewerServer::QueueSendUserMessageCommand 実行時に渡す引数です。
        class SendUserMessageArg
        {
            friend class ViewerServer;
        public:
            //! @brief 表示するメッセージの種類です。
            enum MessageType
            {
                MessageType_Info = 0, //!< 情報としてメッセージを表示します。
                MessageType_Warning = 1, //!< 警告としてメッセージを表示します。
                MessageType_Error = 2, //!< エラーとしてメッセージを表示します。
            };

            //! @brief メッセージの出力先です。
            enum MessageDestination
            {
                MessageDestination_Dialog = 0, //!< ダイアログボックスでメッセージを表示します。
                MessageDestination_Log = 1, //!< ログバーにメッセージを表示します。
            };

        public:
            //! @brief コンストラクタです。
            //! @param[in] message 表示する文字列を指定します。
            //! @pre
            //! - message.length() > 0
            explicit SendUserMessageArg(const nn::util::string_view& message) NN_NOEXCEPT
                : m_MessageType(MessageType_Info)
                , m_MessageDestination(MessageDestination_Dialog)
                , m_Message(message)
            {
                NN_SDK_REQUIRES_GREATER(message.length(), 0u);
            }

            //! @brief コンストラクタです。
            //! @param[in] message 表示する文字列を指定します。
            //! @param[in] messageType 表示するメッセージの種類を指定します。
            //! @param[in] messageDestination メッセージの出力先を指定します。
            //! @pre
            //! - message.length() > 0
            SendUserMessageArg(
                const nn::util::string_view& message,
                MessageType messageType,
                MessageDestination messageDestination) NN_NOEXCEPT
                : m_MessageType(messageType)
                , m_MessageDestination(messageDestination)
                , m_Message(message)
            {
                NN_SDK_REQUIRES_GREATER(message.length(), 0u);
            }

            //! @brief 表示するメッセージの種類を設定します。
            //! @param[in] messageType 表示するメッセージの種類を指定します。
            void SetMessageType(MessageType messageType) NN_NOEXCEPT
            {
                m_MessageType = messageType;
            }

            //! @brief メッセージの出力先を設定します。
            //! @param[in] messageDestination メッセージの出力先を指定します。
            void SetMessageDestination(MessageDestination messageDestination) NN_NOEXCEPT
            {
                m_MessageDestination = messageDestination;
            }

        private:
            MessageType m_MessageType;
            MessageDestination m_MessageDestination;
            nn::util::string_view m_Message;
        };

    public:
        //! @brief サーバーを初期化します。
        //!
        //! @param[in] arg 初期化引数です。
        //!
        //! @pre
        //! - nn::htcs が初期化されている。
        //! - nn::fs のアロケータが設定されている。
        //!
        //! @return 処理結果を返します。
        //! @retval ViewerResult_Success                   - 正常に処理が終了しました。
        //! @retval ViewerResult_ViewerDisabled            - nn::g3d::viewer が無効化されています。
        //!
        NN_DETAIL_G3D_VIEWER_API
            static ViewerResult Initialize(const InitializeArg& arg) NN_NOEXCEPT
        {
            NN_UNUSED(arg);
            return NN_DETAIL_G3D_VIEWER_CALL_PROXY_RESULT(ViewerServerProxy::Initialize(arg));
        }

        //! @brief サーバーを終了します。
        NN_DETAIL_G3D_VIEWER_API
            static void Finalize() NN_NOEXCEPT
        {
            NN_DETAIL_G3D_VIEWER_CALL_PROXY_RESULT(ViewerServerProxy::Finalize());
        }

        //! @brief サーバーのインスタンスを取得します。
        //!
        //! @return サーバーのインスタンスを返します。
        //!
        NN_DETAIL_G3D_VIEWER_API
            static ViewerServer& GetInstance() NN_NOEXCEPT
        {
#if !NN_DETAIL_G3D_VIEWER_CONFIG_IS_VIEWER_EMPTY
            return NN_DETAIL_G3D_VIEWER_CALL_PROXY_RESULT(ViewerServerProxy::GetInstance());
#else
            static ViewerServer s_Instance;
            return s_Instance;
#endif
        }

        //! @brief サーバーが初期化済みであるかを判定します。
        //!
        //! @return 初期化済みの場合は true、そうでなければ false を返します。
        //!
        NN_DETAIL_G3D_VIEWER_API
            static bool IsInitialized() NN_NOEXCEPT
        {
            return NN_DETAIL_G3D_VIEWER_CALL_PROXY_BOOL(ViewerServerProxy::IsInitialized());
        }

        //! @brief サーバーが 3DEditor と接続済みか判定します。
        //!
        //! @return 接続されている場合は true、そうでなければ false を返します。
        //!
        NN_DETAIL_G3D_VIEWER_API
            bool IsConnected() const NN_NOEXCEPT
        {
            return NN_DETAIL_G3D_VIEWER_CALL_PROXY_BOOL(m_Proxy.IsConnected());
        }

        //! @brief サーバーが 3DEditor と通信可能であるかを判定します。
        //!
        //! @return 通信可能であれば true、そうでなければ false を返します。
        //!
        NN_DETAIL_G3D_VIEWER_API
            bool IsOpened() const NN_NOEXCEPT
        {
            return NN_DETAIL_G3D_VIEWER_CALL_PROXY_BOOL(m_Proxy.IsOpened());
        }

        //! @brief サーバーと 3DEditor を通信可能な状態にすることを要求します。
        //!
        //! @return 処理結果を返します。
        //! @retval ViewerResult_Success              - 正常に処理が終了しました。
        //! @retval ViewerResult_ViewerDisabled       - nn::g3d::viewer が無効化されています。
        //! @retval ViewerResult_ServerNotInitialized - サーバーが未初期化です。
        //! @retval ViewerResult_ServerClosing        - サーバーがクローズ中です。
        //! @retval ViewerResult_UnknownError         - 不明なエラーです。
        //!
        //! @details
        //! サーバーを通信可能な状態にするためには、この関数をコールした後に、
        //! nn::g3d::viewer::ViewerServer::Poll() を呼ぶ必要があります。
        //! 通信可能かどうかは nn::g3d::viewer::ViewerServer::IsOpened() で判定することができます。
        //!
        NN_DETAIL_G3D_VIEWER_API
            ViewerResult Open() NN_NOEXCEPT
        {
            return NN_DETAIL_G3D_VIEWER_CALL_PROXY_RESULT(m_Proxy.Open());
        }

        //! @brief  サーバーと 3DEditor の通信を切断することを要求します。
        //!
        //! @details
        //! サーバーを切断状態にするためには、この関数をコールした後に、
        //! nn::g3d::viewer::ViewerServer::Poll() を呼ぶ必要があります。
        //! 切断されているかどうかは nn::g3d::viewer::ViewerServer::IsOpened() で判定することができます。
        //!
        NN_DETAIL_G3D_VIEWER_API
            void Close() NN_NOEXCEPT
        {
            NN_DETAIL_G3D_VIEWER_CALL_PROXY_RESULT(m_Proxy.Close());
        }

        //! @brief 接続状況、パケットの送受信を監視します。
        NN_DETAIL_G3D_VIEWER_API
            void Poll() NN_NOEXCEPT
        {
            NN_DETAIL_G3D_VIEWER_CALL_PROXY_RESULT(m_Proxy.Poll());
        }

        //! @brief 3DEditor から受信したコマンド、もしくはユーザーからエンキューされたコマンドを実行します。
        NN_DETAIL_G3D_VIEWER_API
            void ExecuteCommands() NN_NOEXCEPT
        {
            NN_DETAIL_G3D_VIEWER_CALL_PROXY_RESULT(m_Proxy.ExecuteCommands());
        }

        //! @brief 指定したモデルが編集対象になっている場合に、バインドされているスケルタルアニメーションを計算します。
        //!
        //! @param[in] pModelObj スケルタルアニメーションを計算するモデルを指定します。
        //!
        NN_DETAIL_G3D_VIEWER_API
            void CalculateSkeletalAnimations(const nn::g3d::ModelObj* pModelObj) NN_NOEXCEPT
        {
            NN_UNUSED(pModelObj);
            NN_DETAIL_G3D_VIEWER_CALL_PROXY_RESULT(m_Proxy.CalculateSkeletalAnimations(pModelObj));
        }

        //! @brief 編集対象になっているアニメーションを計算します。
        NN_DETAIL_G3D_VIEWER_API
            void CalculateAnimations() NN_NOEXCEPT
        {
            NN_DETAIL_G3D_VIEWER_CALL_PROXY_RESULT(m_Proxy.CalculateAnimations());
        }

        //! @brief 対象モデルがアタッチ中であるかどうかを判定します。
        //!
        //! @param[in] pModelObj アタッチ中かどうかを判定する nn::g3d::ModelObj を渡します。
        //!
        //! @return アタッチ中である場合は true、そうでなければ false を返します。
        //!
        NN_DETAIL_G3D_VIEWER_API
            bool IsModelAttaching(const nn::g3d::ModelObj* pModelObj) const NN_NOEXCEPT
        {
            NN_UNUSED(pModelObj);
            return NN_DETAIL_G3D_VIEWER_CALL_PROXY_BOOL(m_Proxy.IsModelAttaching(pModelObj));
        }

        //! @brief 対象モデルがアタッチ完了しているかどうかを判定します。
        //!
        //! @param[in] pModelObj アタッチ済みかどうかを判定する nn::g3d::ModelObj を渡します。
        //!
        //! @return アタッチされている場合は true、そうでなければ false を返します。
        NN_DETAIL_G3D_VIEWER_API
            bool IsModelAttached(const nn::g3d::ModelObj* pModelObj) const NN_NOEXCEPT
        {
            NN_UNUSED(pModelObj);
            return NN_DETAIL_G3D_VIEWER_CALL_PROXY_BOOL(m_Proxy.IsModelAttached(pModelObj));
        }

        //! @brief シェーダアーカイブがアタッチ中かどうか判定します。
        //!
        //! @param[in] pShaderArchive アタッチ中かどうかを判定する nn::g3d::ResShaderArchive を渡します。
        //!
        //! @return アタッチ中である場合は true、そうでなければ false を返します。
        //!
        NN_DETAIL_G3D_VIEWER_API
            bool IsShaderArchiveAttaching(const nn::g3d::ResShaderArchive* pShaderArchive) const NN_NOEXCEPT
        {
            NN_UNUSED(pShaderArchive);
            return NN_DETAIL_G3D_VIEWER_CALL_PROXY_BOOL(m_Proxy.IsShaderArchiveAttaching(pShaderArchive));
        }

        //! @brief シェーダアーカイブがアタッチ完了しているかどうか判定します。
        //!
        //! @param[in] pShaderArchive アタッチ済みかどうかを判定する nn::g3d::ResShaderArchive を渡します。
        //!
        //! @return アタッチされている場合は true、そうでなければ false を返します。
        //!
        NN_DETAIL_G3D_VIEWER_API
            bool IsShaderArchiveAttached(const nn::g3d::ResShaderArchive* pShaderArchive) const NN_NOEXCEPT
        {
            NN_UNUSED(pShaderArchive);
            return NN_DETAIL_G3D_VIEWER_CALL_PROXY_BOOL(m_Proxy.IsShaderArchiveAttached(pShaderArchive));
        }

        //! @brief 編集対象とするモデルをアタッチするコマンドをエンキューします。
        //!
        //! @param[in] pAttachModelObj アタッチするモデルを渡します。
        //! @param[in] attachFileName アタッチするモデル中間ファイル名を渡します。3DEditor でファイルを開く際のヒントとして使われます。
        //!
        //! @pre
        //! - pAttachModelObj != nullptr
        //!
        //! @return 処理結果を返します。
        //! @retval ViewerResult_Success              - 正常にエンキューされました。
        //! @retval ViewerResult_ServerNotInitialized - サーバーが未初期化です。
        //! @retval ViewerResult_NotConnected         - サーバーが 3DEditor に接続されていません。
        //! @retval ViewerResult_AlreadyAttached      - 対象モデルは既にアタッチされています。
        //! @retval ViewerResult_CommandQueueFull     - キューが一杯です。
        //! @retval ViewerResult_ViewerDisabled       - nn::g3d::viewer が無効化されています。
        //!
        //! @details
        //! ExecuteCommands を実行することでキューのコマンドは実行されます。
        //!
        NN_DETAIL_G3D_VIEWER_API
            ViewerResult QueueAttachModelCommand(nn::g3d::ModelObj* pAttachModelObj, const char* attachFileName) NN_NOEXCEPT
        {
            NN_UNUSED(pAttachModelObj);
            NN_UNUSED(attachFileName);
            return NN_DETAIL_G3D_VIEWER_CALL_PROXY_RESULT(m_Proxy.QueueAttachModelCommand(pAttachModelObj, attachFileName));
        }

        //! @brief 編集対象とするモデルをアタッチするコマンドをキューに積みます。
        //!
        //! @param[in] pAttachModelObj アタッチするモデルを渡します。
        //!
        //! @pre
        //! - pAttachModelObj != nullptr
        //!
        //! @return 処理結果を返します。
        //! @retval ViewerResult_Success              - 正常にエンキューされました。
        //! @retval ViewerResult_ServerNotInitialized - サーバーが未初期化です。
        //! @retval ViewerResult_NotConnected         - サーバーが 3DEditor に接続されていません。
        //! @retval ViewerResult_AlreadyAttached      - 対象モデルは既にアタッチされています。
        //! @retval ViewerResult_CommandQueueFull     - キューが一杯です。
        //! @retval ViewerResult_ViewerDisabled       - nn::g3d::viewer が無効化されています。
        //!
        //! @details
        //! ExecuteCommands を実行することでキューのコマンドは実行されます。
        //!
        NN_DETAIL_G3D_VIEWER_API
            ViewerResult QueueAttachModelCommand(nn::g3d::ModelObj* pAttachModelObj) NN_NOEXCEPT
        {
            return QueueAttachModelCommand(pAttachModelObj, nullptr);
        }

        //! @brief アタッチされたモデルをデタッチするコマンドをエンキューします。
        //!
        //! @param[in] pDetachModelObj デタッチするモデルを渡します。
        //!
        //! @pre
        //! - pDetachModelObj != nullptr
        //!
        //! @return 処理結果を返します。
        //! @retval ViewerResult_Success              - 正常にエンキューされました。
        //! @retval ViewerResult_ServerNotInitialized - サーバーが未初期化です。
        //! @retval ViewerResult_NotAttached          - 対象モデルがアタッチされていません。
        //! @retval ViewerResult_CommandQueueFull     - キューが一杯です。
        //! @retval ViewerResult_ViewerDisabled       - nn::g3d::viewer が無効化されています。
        //!
        //! @details
        //! ExecuteCommands を実行することでキューのコマンドは実行されます。
        //!
        NN_DETAIL_G3D_VIEWER_API
            ViewerResult QueueDetachModelCommand(const nn::g3d::ModelObj* pDetachModelObj) NN_NOEXCEPT
        {
            NN_UNUSED(pDetachModelObj);
            return NN_DETAIL_G3D_VIEWER_CALL_PROXY_RESULT(m_Proxy.QueueDetachModelCommand(pDetachModelObj));
        }

        //! @brief 編集対象とするシェーダアーカイブをアタッチするコマンドをエンキューします。
        //!
        //! @param[in] pAttachResShaderArchive アタッチするシェーダアーカイブを渡します。
        //! @param[in] attachFileName アタッチするシェーダ定義中間ファイル名を渡します。3DEditor でファイルを開く際のヒントとして使われます。
        //!
        //! @pre
        //! - pAttachResShaderArchive != nullptr
        //!
        //! @return 処理結果を返します。
        //! @retval ViewerResult_Success              - 正常にエンキューされました。
        //! @retval ViewerResult_ServerNotInitialized - サーバーが未初期化です。
        //! @retval ViewerResult_NotConnected         - サーバーが 3DEditor に接続されていません。
        //! @retval ViewerResult_AlreadyAttached      - 対象モデルは既にアタッチされています。
        //! @retval ViewerResult_CommandQueueFull     - キューが一杯です。
        //! @retval ViewerResult_ViewerDisabled       - nn::g3d::viewer が無効化されています。
        //!
        //! @details
        //! ExecuteCommands を実行することでキューのコマンドは実行されます。
        //!
        NN_DETAIL_G3D_VIEWER_API
            ViewerResult QueueAttachShaderArchiveCommand(nn::g3d::ResShaderArchive* pAttachResShaderArchive, const char* attachFileName) NN_NOEXCEPT
        {
            NN_UNUSED(pAttachResShaderArchive);
            NN_UNUSED(attachFileName);
            return NN_DETAIL_G3D_VIEWER_CALL_PROXY_RESULT(m_Proxy.QueueAttachShaderArchiveCommand(pAttachResShaderArchive, attachFileName));
        }

        //! @brief 編集対象とするシェーダアーカイブをアタッチするコマンドをエンキューします。
        //!
        //! @param[in] pAttachResShaderArchive アタッチするシェーダアーカイブを渡します。
        //!
        //! @pre
        //! - pAttachResShaderArchive != nullptr
        //!
        //! @return 処理結果を返します。
        //! @retval ViewerResult_Success              - 正常にエンキューされました。
        //! @retval ViewerResult_ServerNotInitialized - サーバーが未初期化です。
        //! @retval ViewerResult_NotConnected         - サーバーが 3DEditor に接続されていません。
        //! @retval ViewerResult_AlreadyAttached      - 対象モデルは既にアタッチされています。
        //! @retval ViewerResult_InvalidArgument      - 引数が不正です。
        //! @retval ViewerResult_CommandQueueFull     - キューが一杯です。
        //! @retval ViewerResult_ViewerDisabled       - nn::g3d::viewer が無効化されています。
        //!
        //! @details
        //! ExecuteCommands を実行することでキューのコマンドは実行されます。
        //! ForceVariation フラグが立っているシェーダアーカイブをアタッチすることはできません。
        //!
        NN_DETAIL_G3D_VIEWER_API
            ViewerResult QueueAttachShaderArchiveCommand(nn::g3d::ResShaderArchive* pAttachResShaderArchive) NN_NOEXCEPT
        {
            return QueueAttachShaderArchiveCommand(pAttachResShaderArchive, nullptr);
        }

        //! @brief アタッチしたシェーダアーカイブをデタッチするコマンドをエンキューします。
        //!
        //! @param[in] pDetachResShaderArchive アタッチするシェーダアーカイブを渡します。
        //!
        //! @pre
        //! - pAttachResShaderArchive != nullptr
        //!
        //! @return 処理結果を返します。
        //! @retval ViewerResult_Success              - 正常にエンキューされました。
        //! @retval ViewerResult_ServerNotInitialized - サーバーが未初期化です。
        //! @retval ViewerResult_NotAttached          - 対象モデルがアタッチされていません。
        //! @retval ViewerResult_CommandQueueFull     - キューが一杯です。
        //! @retval ViewerResult_ViewerDisabled       - nn::g3d::viewer が無効化されています。
        //!
        //! @details
        //! ExecuteCommands を実行することでキューのコマンドは実行されます。
        //!
        NN_DETAIL_G3D_VIEWER_API
            ViewerResult QueueDetachShaderArchiveCommand(const nn::g3d::ResShaderArchive* pDetachResShaderArchive) NN_NOEXCEPT
        {
            NN_UNUSED(pDetachResShaderArchive);
            return NN_DETAIL_G3D_VIEWER_CALL_PROXY_RESULT(m_Proxy.QueueDetachShaderArchiveCommand(pDetachResShaderArchive));
        }

        //! @brief 3DEditor に文字列型の描画情報の選択肢を追加するコマンドをエンキューします。
        //!
        //! @pre
        //! - labelName != nullptr
        //! - itemName != nullptr
        //!
        //! @param[in] labelName 描画情報のラベル名です。
        //! @param[in] itemName 描画情報のアイテム名です。(文字列型での設定値になります。）
        //! @param[in] aliasItemName 描画情報のアイテム名のエイリアス名です。(文字列型での設定値になります。未指定の場合は nullptr です。）
        //!
        //! @return 処理結果を返します。
        //! @retval ViewerResult_Success                - 正常に処理が終了しました。
        //! @retval ViewerResult_ServerNotInitialized   - サーバーが未初期化です。
        //! @retval ViewerResult_MemoryAllocationFailed - メモリ確保に失敗しました。
        //! @retval ViewerResult_ViewerDisabled         - nn::g3d::viewer が無効化されています。
        //!
        //! @details
        //! ExecuteCommands を実行することでキューのコマンドは実行されます。
        //!
        NN_DETAIL_G3D_VIEWER_API
            ViewerResult QueueAddRenderInfoStringChoiceCommand(const char* labelName, const char* itemName, const char* aliasItemName = nullptr) NN_NOEXCEPT
        {
            NN_UNUSED(labelName);
            NN_UNUSED(itemName);
            NN_UNUSED(aliasItemName);
            return NN_DETAIL_G3D_VIEWER_CALL_PROXY_RESULT(m_Proxy.QueueAddRenderInfoStringChoiceCommand(labelName, itemName, aliasItemName));
        }

        //! @brief 3DEditor に送信するint型の描画情報の値の範囲を登録するコマンドをエンキューします。
        //!
        //! @pre
        //! - labelName != nullptr
        //! - minValue <= maxValue
        //!
        //! @param[in] labelName 描画情報のラベル名です。
        //! @param[in] minValue 描画情報の最小値です。
        //! @param[in] maxValue 描画情報の最大値です。
        //!
        //! @return 処理結果を返します。
        //! @retval ViewerResult_Success                - 正常に処理が終了しました。
        //! @retval ViewerResult_ServerNotInitialized   - サーバーが未初期化です。
        //! @retval ViewerResult_MemoryAllocationFailed - メモリ確保に失敗しました。
        //! @retval ViewerResult_ViewerDisabled         - nn::g3d::viewer が無効化されています。
        //!
        //! @details
        //! ExecuteCommands を実行することでキューのコマンドは実行されます。
        //!
        NN_DETAIL_G3D_VIEWER_API
            ViewerResult QueueSetRenderInfoIntRangeCommand(const char* labelName, int minValue, int maxValue) NN_NOEXCEPT
        {
            NN_UNUSED(labelName);
            NN_UNUSED(minValue);
            NN_UNUSED(maxValue);
            return NN_DETAIL_G3D_VIEWER_CALL_PROXY_RESULT(m_Proxy.QueueSetRenderInfoIntRangeCommand(labelName, minValue, maxValue));
        }

        //! @brief 3DEditor に送信するfloat型の描画情報の値の範囲を設定するコマンドをエンキューします。
        //!
        //! @pre
        //! - labelName != nullptr
        //! - minValue <= maxValue
        //!
        //! @param[in] labelName 描画情報のラベル名です。
        //! @param[in] minValue 描画情報の最小値です。
        //! @param[in] maxValue 描画情報の最大値です。
        //!
        //! @return 処理結果を返します。
        //! @retval ViewerResult_Success                - 正常に処理が終了しました。
        //! @retval ViewerResult_ServerNotInitialized   - サーバーが未初期化です。
        //! @retval ViewerResult_MemoryAllocationFailed - メモリ確保に失敗しました。
        //! @retval ViewerResult_ViewerDisabled         - nn::g3d::viewer が無効化されています。
        //!
        //! @details
        //! ExecuteCommands を実行することでキューのコマンドは実行されます。
        //!
        NN_DETAIL_G3D_VIEWER_API
            ViewerResult QueueSetRenderInfoFloatRangeCommand(const char* labelName, float minValue, float maxValue) NN_NOEXCEPT
        {
            NN_UNUSED(labelName);
            NN_UNUSED(minValue);
            NN_UNUSED(maxValue);
            return NN_DETAIL_G3D_VIEWER_CALL_PROXY_RESULT(m_Proxy.QueueSetRenderInfoFloatRangeCommand(labelName, minValue, maxValue));
        }

        //! @brief 3DEditor に送信する文字列型の描画情報のデフォルト値を設定するコマンドをエンキューします。
        //!
        //! @pre
        //! - labelName != nullptr
        //! - value != nullptr
        //!
        //! @param[in] labelName 描画情報のラベル名です。
        //! @param[in] value 描画情報のデフォルト値です。
        //!
        //! @return 処理結果を返します。
        //! @retval ViewerResult_Success                - 正常に処理が終了しました。
        //! @retval ViewerResult_ServerNotInitialized   - サーバーが未初期化です。
        //! @retval ViewerResult_MemoryAllocationFailed - メモリ確保に失敗しました。
        //! @retval ViewerResult_ViewerDisabled         - nn::g3d::viewer が無効化されています。
        //!
        //! @details
        //! ExecuteCommands を実行することでキューのコマンドは実行されます。
        //! 同ラベルの複数描画情報に対してデフォルト値を設定する場合は、この関数を複数回実行します。
        //! 関数をコールした順番にデフォルト値が設定されます。
        //!
        NN_DETAIL_G3D_VIEWER_API
            ViewerResult QueuePushRenderInfoStringDefaultCommand(const char* labelName, const char* value) NN_NOEXCEPT
        {
            NN_UNUSED(labelName);
            NN_UNUSED(value);
            return NN_DETAIL_G3D_VIEWER_CALL_PROXY_RESULT(m_Proxy.QueuePushRenderInfoStringDefaultCommand(labelName, value));
        }

        //! @brief 3DEditor に送信するint型の描画情報のデフォルト値を登録するコマンドをエンキューします。
        //!
        //! @pre
        //! - labelName != nullptr
        //!
        //! @param[in] labelName 描画情報のラベル名です。
        //! @param[in] value 描画情報のデフォルト値です。
        //!
        //! @return 処理結果を返します。
        //! @retval ViewerResult_Success                - 正常に処理が終了しました。
        //! @retval ViewerResult_ServerNotInitialized   - サーバーが未初期化です。
        //! @retval ViewerResult_MemoryAllocationFailed - メモリ確保に失敗しました。
        //! @retval ViewerResult_ViewerDisabled         - nn::g3d::viewer が無効化されています。
        //!
        //! @details
        //! ExecuteCommands を実行することでキューのコマンドは実行されます。
        //! 同ラベルの複数描画情報に対してデフォルト値を設定する場合は、この関数を複数回実行します。
        //! 関数をコールした順番にデフォルト値が設定されます。
        //!
        NN_DETAIL_G3D_VIEWER_API
            ViewerResult QueuePushRenderInfoIntDefaultCommand(const char* labelName, int value) NN_NOEXCEPT
        {
            NN_UNUSED(labelName);
            NN_UNUSED(value);
            return NN_DETAIL_G3D_VIEWER_CALL_PROXY_RESULT(m_Proxy.QueuePushRenderInfoIntDefaultCommand(labelName, value));
        }

        //! @brief 3DEditor に送信するfloat型の描画情報のデフォルト値を登録するコマンドをエンキューします。
        //!
        //! @pre
        //! - labelName != nullptr
        //!
        //! @param[in] labelName 描画情報のラベル名です。
        //! @param[in] value 描画情報のデフォルト値です。
        //!
        //! @return 処理結果を返します。
        //! @retval ViewerResult_Success                - 正常に処理が終了しました。
        //! @retval ViewerResult_ServerNotInitialized   - サーバーが未初期化です。
        //! @retval ViewerResult_MemoryAllocationFailed - メモリ確保に失敗しました。
        //! @retval ViewerResult_ViewerDisabled         - nn::g3d::viewer が無効化されています。
        //!
        //! @details
        //! ExecuteCommands を実行することでキューのコマンドは実行されます。
        //! 同ラベルの複数描画情報に対してデフォルト値を設定する場合は、この関数を複数回実行します。
        //! 関数をコールした順番にデフォルト値が設定されます。
        //!
        NN_DETAIL_G3D_VIEWER_API
            ViewerResult QueuePushRenderInfoFloatDefaultCommand(const char* labelName, float value) NN_NOEXCEPT
        {
            NN_UNUSED(labelName);
            NN_UNUSED(value);
            return NN_DETAIL_G3D_VIEWER_CALL_PROXY_RESULT(m_Proxy.QueuePushRenderInfoFloatDefaultCommand(labelName, value));
        }

        //! @brief 3DEditor 上のマテリアルを選択状態にするコマンドをエンキューします。
        //!
        //! @pre
        //! - pModelObj != nullptr
        //!
        //! @param[in] pModelObj 選択対象のモデルです。
        //! @param[in] materialIndex 選択対象のマテリアルインデックスです。
        //!
        //! @return 処理結果を返します。
        //! @retval ViewerResult_Success                - 正常に処理が終了しました。
        //! @retval ViewerResult_ServerNotInitialized   - サーバーが未初期化です。
        //! @retval ViewerResult_MemoryAllocationFailed - メモリ確保に失敗しました。
        //! @retval ViewerResult_ViewerDisabled         - nn::g3d::viewer が無効化されています。
        //!
        //! @details
        //! ExecuteCommands を実行することでキューのコマンドは実行されます。
        //!
        NN_DETAIL_G3D_VIEWER_API
            ViewerResult QueueSelectMaterialCommand(const nn::g3d::ModelObj* pModelObj, int materialIndex) NN_NOEXCEPT
        {
            NN_UNUSED(pModelObj);
            NN_UNUSED(materialIndex);
            return NN_DETAIL_G3D_VIEWER_CALL_PROXY_RESULT(m_Proxy.QueueSelectMaterialCommand(pModelObj, materialIndex));
        }

        //! @brief 3DEditor 上のマテリアルを選択解除するコマンドをエンキューします。
        //!
        //! @return 処理結果を返します。
        //! @retval ViewerResult_Success                - 正常に処理が終了しました。
        //! @retval ViewerResult_ServerNotInitialized   - サーバーが未初期化です。
        //! @retval ViewerResult_MemoryAllocationFailed - メモリ確保に失敗しました。
        //! @retval ViewerResult_ViewerDisabled         - nn::g3d::viewer が無効化されています。
        //!
        //! @details
        //! ExecuteCommands を実行することでキューのコマンドは実行されます。
        //!
        NN_DETAIL_G3D_VIEWER_API
            ViewerResult QueueClearMaterialSelectionCommand() NN_NOEXCEPT
        {
            return NN_DETAIL_G3D_VIEWER_CALL_PROXY_RESULT(m_Proxy.QueueClearMaterialSelectionCommand());
        }

        //! @brief 3DEditor のモデル配置情報を編集するコマンドをエンキューします。
        //!
        //! @pre
        //! - arg.pModelObj != nullptr
        //!
        //! @param[in] arg 引数です。
        //!
        //! @return 処理結果を返します。
        //! @retval ViewerResult_Success                - 正常に処理が終了しました。
        //! @retval ViewerResult_ServerNotInitialized   - サーバーが未初期化です。
        //! @retval ViewerResult_MemoryAllocationFailed - メモリ確保に失敗しました。
        //! @retval ViewerResult_ViewerDisabled         - nn::g3d::viewer が無効化されています。
        //!
        //! @details
        //! ExecuteCommands を実行することでキューのコマンドは実行されます。
        //!
        NN_DETAIL_G3D_VIEWER_API
            ViewerResult QueueLayoutModelCommand(const LayoutModelArg& arg) NN_NOEXCEPT
        {
            NN_UNUSED(arg);
            return NN_DETAIL_G3D_VIEWER_CALL_PROXY_RESULT(m_Proxy.QueueLayoutModelCommand(arg));
        }

        //! @brief 3DEditor に任意のメッセージを表示させるコマンドをエンキューします。
        //!
        //! @param[in] arg 引数です。
        //!
        //! @return 処理結果を返します。
        //! @retval ViewerResult_Success                - 正常に処理が終了しました。
        //! @retval ViewerResult_ServerNotInitialized   - サーバーが未初期化です。
        //! @retval ViewerResult_MemoryAllocationFailed - メモリ確保に失敗しました。
        //! @retval ViewerResult_ViewerDisabled         - nn::g3d::viewer が無効化されています。
        //!
        //! @details
        //! ExecuteCommands を実行することでキューのコマンドは実行されます。
        //!
        NN_DETAIL_G3D_VIEWER_API
            ViewerResult QueueSendUserMessageCommand(const SendUserMessageArg& arg) NN_NOEXCEPT
        {
            NN_UNUSED(arg);
            return NN_DETAIL_G3D_VIEWER_CALL_PROXY_RESULT(m_Proxy.QueueSendUserMessageCommand(arg));
        }

        //! @briefprivate nn::g3d::viewer::ViewerServer がアニメーション再生中であるかを取得します。
        //! @return 再生中であれば true、そうでなければ false を返します。
        //! @details外部から設定を書き換えない限り、3DEditor のアニメーションコントロールと同じ状態になります。
        NN_DETAIL_G3D_VIEWER_API
            bool IsAnimPlaying() const NN_NOEXCEPT
        {
            return NN_DETAIL_G3D_VIEWER_CALL_PROXY_BOOL(m_Proxy.IsAnimPlaying());
        }

        //! @briefprivate nn::g3d::viewer::ViewerServer 内のアニメーション再生フラグを設定します。
        //! @detailsこの関数はライブラリ内部の状態を書き換えるだけで、3DEditor のアニメーションコントロールの状態は同期されません。
        NN_DETAIL_G3D_VIEWER_API
            void SetAnimPlaying(bool isPlaying) NN_NOEXCEPT
        {
            NN_UNUSED(isPlaying);
            NN_DETAIL_G3D_VIEWER_CALL_PROXY_BOOL(m_Proxy.SetAnimPlaying(isPlaying));
        }

        //! @briefprivate nn::g3d::viewer::ViewerServer のアニメーション再生ポリシーがループ再生か判定します。
        //! @return ループ再生中であれば true、そうでなければ false を返します。
        //! @details外部から設定を書き換えない限り、3DEditor のアニメーションコントロールと同じ状態になります。
        NN_DETAIL_G3D_VIEWER_API
            bool IsAnimPlayPolicyLoop() const NN_NOEXCEPT
        {
            return NN_DETAIL_G3D_VIEWER_CALL_PROXY_BOOL(m_Proxy.IsAnimPlayPolicyLoop());
        }

        //! @briefprivate nn::g3d::viewer::ViewerServer の現在のフレームを取得します。
        //! @return 現在のフレームを返します。
        //! @details外部から設定を書き換えない限り、3DEditor のアニメーションコントロールと同じ状態になります。
        NN_DETAIL_G3D_VIEWER_API
            float GetCurrentFrame() const NN_NOEXCEPT
        {
            return NN_DETAIL_G3D_VIEWER_CALL_PROXY_FLOAT(m_Proxy.GetCurrentFrame());
        }

        //! @briefprivate 現在のフレームを設定します。
        //! @detailsこの関数はライブラリ内部の状態を書き換えるだけで、3DEditor のアニメーションコントロールの状態は同期されません。
        NN_DETAIL_G3D_VIEWER_API
            void SetCurrentFrame(float frame) NN_NOEXCEPT
        {
            NN_UNUSED(frame);
            NN_DETAIL_G3D_VIEWER_CALL_PROXY(m_Proxy.SetCurrentFrame(frame));
        }

        //! @briefprivate nn::g3d::viewer::ViewerServer のフレームステップを取得します。
        //! @return フレームステップを返します。
        //! @details外部から設定を書き換えない限り、3DEditor のアニメーションコントロールと同じ状態になります。
        NN_DETAIL_G3D_VIEWER_API
            float GetFrameStep() const NN_NOEXCEPT
        {
            return NN_DETAIL_G3D_VIEWER_CALL_PROXY_FLOAT(m_Proxy.GetFrameStep());
        }

        //! @briefprivate nn::g3d::viewer::ViewerServer のフレームステップを設定します。
        //! @detailsこの関数はライブラリ内部の状態を書き換えるだけで、3DEditor のアニメーションコントロールの状態は同期されません。
        NN_DETAIL_G3D_VIEWER_API
            void SetFrameStep(float frameStep) NN_NOEXCEPT
        {
            NN_UNUSED(frameStep);
            NN_DETAIL_G3D_VIEWER_CALL_PROXY(m_Proxy.SetFrameStep(frameStep));
        }

        //! @briefprivate nn::g3d::viewer::ViewerServer のアニメーションコントロール上のフレーム数を取得します。
        //! @return 現在のフレーム数を返します。
        //! @details外部から設定を書き換えない限り、3DEditor のアニメーションコントロールと同じ状態になります。
        NN_DETAIL_G3D_VIEWER_API
            float GetFrameCount() const NN_NOEXCEPT
        {
            return NN_DETAIL_G3D_VIEWER_CALL_PROXY_FLOAT(m_Proxy.GetFrameCount());
        }

        //! @briefprivate フレームステップに乗算するプレビュー調整用の割合を取得します。
        //! @return プレビュー調整用の割合を返します。
        NN_DETAIL_G3D_VIEWER_API
            float GetPreviewFrameStepRate() const NN_NOEXCEPT
        {
            return NN_DETAIL_G3D_VIEWER_CALL_PROXY_FLOAT(m_Proxy.GetPreviewFrameStepRate());
        }

        //! @briefprivate フレームステップに乗算するプレビュー調整用の割合を設定します。
        NN_DETAIL_G3D_VIEWER_API
            void SetPreviewFrameStepRate(float stepRate) NN_NOEXCEPT
        {
            NN_UNUSED(stepRate);
            NN_DETAIL_G3D_VIEWER_CALL_PROXY(m_Proxy.SetPreviewFrameStepRate(stepRate));
        }

    private:
        // TODO: 以下は現在利用されているかどうかが不明な非公開関数。必要であれば公開する。
        //       特に API の見直しを行っていないので、公開する場合はインタフェースを見直す。

        //! @briefprivate アニメーション数を取得します。
        NN_DETAIL_G3D_VIEWER_API
            int GetModelAnimCount() const NN_NOEXCEPT
        {
            return NN_DETAIL_G3D_VIEWER_CALL_PROXY_INT(m_Proxy.GetModelAnimCount());
        }

        //! @briefprivate 指定モデルにバインドされているアニメーション数を取得します。
        NN_DETAIL_G3D_VIEWER_API
            int GetModelAnimBoundCount(const nn::g3d::ModelObj* pModelObj) const NN_NOEXCEPT
        {
            NN_UNUSED(pModelObj);
            return NN_DETAIL_G3D_VIEWER_CALL_PROXY_INT(m_Proxy.GetModelAnimBoundCount(pModelObj));
        }

        //! @briefprivate 指定モデルにバインドされているアニメーションが登録されているインデックスを取得します。
        //!               GetModelAnimBoundCount で総数を取得して、boundAnimIndex には、総数 - 1 までの値を渡します。
        //!               関数実行時の返り値が 0 以上であれば、GetModelAnimFrameCount、GetModelAnimName にこの値を引数に渡すことで、
        //!               指定モデルのバインドされている各アニメーションの情報を取得する事ができます。
        NN_DETAIL_G3D_VIEWER_API
            int GetActiveModelAnimIndex(const nn::g3d::ModelObj* pModelObj, int boundAnimIndex) const NN_NOEXCEPT
        {
            NN_UNUSED(pModelObj);
            NN_UNUSED(boundAnimIndex);
            return NN_DETAIL_G3D_VIEWER_CALL_PROXY_INT(m_Proxy.GetActiveModelAnimIndex(pModelObj, boundAnimIndex));
        }

        //! @briefprivate アニメーションのフレーム数を取得します。
        NN_DETAIL_G3D_VIEWER_API
            float GetModelAnimFrameCount(int modelAnimIndex) const NN_NOEXCEPT
        {
            NN_UNUSED(modelAnimIndex);
            return NN_DETAIL_G3D_VIEWER_CALL_PROXY_FLOAT(m_Proxy.GetModelAnimFrameCount(modelAnimIndex));
        }

        //! @briefprivate アニメーションの名前を取得します。
        NN_DETAIL_G3D_VIEWER_API
            const char* GetModelAnimName(int modelAnimIndex) const NN_NOEXCEPT
        {
            NN_UNUSED(modelAnimIndex);
            return NN_DETAIL_G3D_VIEWER_CALL_PROXY_POINTER(m_Proxy.GetModelAnimName(modelAnimIndex));
        }

        //! @briefprivate アニメーションの種類を取得します。
        NN_DETAIL_G3D_VIEWER_API
            detail::ViewerAnimKind GetModelAnimKind(int modelAnimIndex) const NN_NOEXCEPT
        {
            NN_UNUSED(modelAnimIndex);
#if !NN_DETAIL_G3D_VIEWER_CONFIG_IS_VIEWER_EMPTY
            return m_Proxy.GetModelAnimKind(modelAnimIndex);
#else
            return static_cast<detail::ViewerAnimKind>(-1);
#endif
        }

        //! @briefprivate 指定モデルの次のアニメーションへ設定を移動します。
        NN_DETAIL_G3D_VIEWER_API
            void MoveNextModelAnim(const nn::g3d::ModelObj* pModelObj) NN_NOEXCEPT
        {
            NN_UNUSED(pModelObj);
            NN_DETAIL_G3D_VIEWER_CALL_PROXY(m_Proxy.MoveNextModelAnim(pModelObj));
        }

        //! @briefprivate 指定モデルの前のアニメーションへ設定を移動します。
        NN_DETAIL_G3D_VIEWER_API
            void MovePrevModelAnim(const nn::g3d::ModelObj* pModelObj) NN_NOEXCEPT
        {
            NN_UNUSED(pModelObj);
            NN_DETAIL_G3D_VIEWER_CALL_PROXY(m_Proxy.MovePrevModelAnim(pModelObj));
        }

    private:
        NN_DETAIL_G3D_VIEWER_API
            ViewerServer() NN_NOEXCEPT
        {
        }

        static void CallViewerCallbacks(void* pOutArg, const void* pInArg, CallbackType type, void* pUserData) NN_NOEXCEPT
        {
            NN_UNUSED(pOutArg);
            NN_UNUSED(pInArg);
            NN_UNUSED(type);
            NN_UNUSED(pUserData);
            NN_DETAIL_G3D_VIEWER_CALL_PROXY_RESULT(ViewerServerProxy::CallViewerCallbacks(pOutArg, pInArg, type, pUserData));
        }

        static nn::g3d::TextureRef ViewerDefaultTextureBindCallback(
            const char* name, const nn::gfx::ResTextureFile* pResTextureFile, void* pUserData) NN_NOEXCEPT
        {
            NN_UNUSED(name);
            NN_UNUSED(pResTextureFile);
            NN_UNUSED(pUserData);
#if !NN_DETAIL_G3D_VIEWER_CONFIG_IS_VIEWER_EMPTY
            return ViewerServerProxy::ViewerDefaultTextureBindCallback(name, pResTextureFile, pUserData);
#else
            return nn::g3d::TextureRef();
#endif
        }

        class ViewerServerProxy
        {
        public:
            static ViewerResult Initialize(const InitializeArg& arg) NN_NOEXCEPT;
            static void Finalize() NN_NOEXCEPT;
            static ViewerServer& GetInstance() NN_NOEXCEPT;
            static bool IsInitialized() NN_NOEXCEPT;
            bool IsConnected() const NN_NOEXCEPT;
            ViewerResult Open() NN_NOEXCEPT;
            void Close() NN_NOEXCEPT;
            void Poll() NN_NOEXCEPT;
            void ExecuteCommands() NN_NOEXCEPT;
            void CalculateSkeletalAnimations(const nn::g3d::ModelObj* pModelObj) NN_NOEXCEPT;
            void CalculateAnimations() NN_NOEXCEPT;
            bool IsModelAttaching(const nn::g3d::ModelObj* modelObj) const NN_NOEXCEPT;
            bool IsModelAttached(const nn::g3d::ModelObj* modelObj) const NN_NOEXCEPT;
            bool IsShaderArchiveAttaching(const nn::g3d::ResShaderArchive* pShaderArchive) const NN_NOEXCEPT;
            bool IsShaderArchiveAttached(const nn::g3d::ResShaderArchive* pShaderArchive) const NN_NOEXCEPT;
            ViewerResult QueueAttachModelCommand(nn::g3d::ModelObj* pAttachModelObj, const char* attachFileName) NN_NOEXCEPT;
            ViewerResult QueueDetachModelCommand(const nn::g3d::ModelObj* pDetachModelObj) NN_NOEXCEPT;
            ViewerResult QueueAttachShaderArchiveCommand(nn::g3d::ResShaderArchive* pAttachResShaderArchive, const char* attachFileName) NN_NOEXCEPT;
            ViewerResult QueueDetachShaderArchiveCommand(const nn::g3d::ResShaderArchive* pDetachResShaderArchive) NN_NOEXCEPT;
            ViewerResult QueueAddRenderInfoStringChoiceCommand(const char* labelName, const char* itemName, const char* aliasItemName = nullptr) NN_NOEXCEPT;
            ViewerResult QueueSetRenderInfoIntRangeCommand(const char* labelName, int minValue, int maxValue) NN_NOEXCEPT;
            ViewerResult QueueSetRenderInfoFloatRangeCommand(const char* labelName, float minValue, float maxValue) NN_NOEXCEPT;
            ViewerResult QueuePushRenderInfoStringDefaultCommand(const char* labelName, const char* value) NN_NOEXCEPT;
            ViewerResult QueuePushRenderInfoIntDefaultCommand(const char* labelName, int value) NN_NOEXCEPT;
            ViewerResult QueuePushRenderInfoFloatDefaultCommand(const char* labelName, float value) NN_NOEXCEPT;
            ViewerResult QueueSelectMaterialCommand(const nn::g3d::ModelObj* pModelObj, int materialIndex) NN_NOEXCEPT;
            ViewerResult QueueClearMaterialSelectionCommand() NN_NOEXCEPT;
            ViewerResult QueueLayoutModelCommand(const LayoutModelArg& arg) NN_NOEXCEPT;
            ViewerResult QueueSendUserMessageCommand(const SendUserMessageArg& arg) NN_NOEXCEPT;

            bool IsAnimPlayPolicyLoop() const NN_NOEXCEPT;
            void SetAnimPlaying(bool isPlaying) NN_NOEXCEPT;
            bool IsAnimPlaying() const NN_NOEXCEPT;
            float GetCurrentFrame() const NN_NOEXCEPT;
            void SetCurrentFrame(float value) NN_NOEXCEPT;
            float GetFrameStep() const NN_NOEXCEPT;
            void SetFrameStep(float value) NN_NOEXCEPT;
            void SetPreviewFrameStepRate(float value) NN_NOEXCEPT;
            float GetPreviewFrameStepRate() const NN_NOEXCEPT;
            float GetFrameCount() const NN_NOEXCEPT;

            int GetModelAnimCount() const NN_NOEXCEPT;
            int GetModelAnimBoundCount(const nn::g3d::ModelObj* modelObj) const NN_NOEXCEPT;
            int GetActiveModelAnimIndex(const nn::g3d::ModelObj* modelObj, int boundAnimIndex) const NN_NOEXCEPT;
            float GetModelAnimFrameCount(int modelAnimIndex) const NN_NOEXCEPT;
            const char* GetModelAnimName(int modelAnimindex) const NN_NOEXCEPT;
            detail::ViewerAnimKind GetModelAnimKind(int modelAnimIndex) const NN_NOEXCEPT;
            void MoveNextModelAnim(const nn::g3d::ModelObj* modelObj) NN_NOEXCEPT;
            void MovePrevModelAnim(const nn::g3d::ModelObj* modelObj) NN_NOEXCEPT;

            static void CallViewerCallbacks(void* pOutArg, const void* pInArg, CallbackType type, void* pUserData) NN_NOEXCEPT;
            static nn::g3d::TextureRef ViewerDefaultTextureBindCallback(
                const char* name, const nn::gfx::ResTextureFile* pResTextureFile, void* pUserData) NN_NOEXCEPT;

            bool IsOpened() const NN_NOEXCEPT;

        private:
            class Impl;
            Impl* m_pImpl;
        };

        ViewerServerProxy m_Proxy;
    };
}}} // namespace nn::g3d::viewer
