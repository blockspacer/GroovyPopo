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
 * :include nn/atk/atk_Sound3DListener.h
 *
 * @file atk_Sound3DListener.h
 */

#pragma once

#include <nn/util/util_MathTypes.h>
#include <nn/util/util_IntrusiveList.h>

namespace nn {
namespace atk {

//---------------------------------------------------------------------------
//! @brief    3D 空間内でのリスニングポイントを表す 3D サウンドリスナークラスです。
//!
//!           リスナーは @ref Sound3DManager クラスに登録して使用します。
//!           リスナーに設定されたリスナー行列の情報を用いて、
//!           サウンドのパラメータを計算して設定します。
//!
//!           この型は @ref nn::util::Vector3fType 型のメンバを持っているため
//!           メモリ上に確保する場合にはアライメントを満たす必要があります。
//!           @ref nn::atk::Sound3DListener::BufferAlignSize バイト境界に配置するようにしてください。
//!
//! @see Sound3DManager クラス
//! @see Sound3DManager::AddListener
//!
//---------------------------------------------------------------------------
class Sound3DListener
{
public:

    //---------------------------------------------------------------------------
    //! @brief  出力先属性をあらわします。
    //!
    //!         今後この列挙体は削除されます。互換性維持のため、残されています。
    //!         以後は、@ref OutputType をご利用ください。
    //!
    //! @see SetListenerOutputType
    //! @see GetListenerOutputType
    //---------------------------------------------------------------------------
    enum ListenerOutputType
    {
        //! TV 出力用の計算をするリスナーであることをあらわします。
        ListenerOutputType_Tv  = (1 << 0),

        //! @internal
        //! DRC 出力用の計算をするリスナーであることをあらわします。
        ListenerOutputType_Drc = (1 << 1),

        //! すべての出力用の計算をするリスナーであることをあらわします。
        ListenerOutputType_Both = (ListenerOutputType_Tv | ListenerOutputType_Drc)
    };

    //---------------------------------------------------------------------------
    //! @brief  出力先属性をあらわします。
    //!
    //! @see SetOutputTypeFlag
    //! @see GetOutputTypeFlag
    //---------------------------------------------------------------------------
    enum OutputType
    {
        //! TV 出力用の計算をするリスナーであることをあらわします。
        OutputType_Tv = (1<<0),

        //! @internal
        //! DRC 0 出力用の計算をするリスナーであることをあらわします。
        OutputType_Drc0 = (1<<1),

        //! @internal
        //! DRC 1 出力用の計算をするリスナーであることをあらわします。
        OutputType_Drc1 = (1<<2),

        //! すべての出力に対して計算をするリスナーであることをあらわします。
        OutputType_All = 0xffffffff
    };

    //---------------------------------------------------------------------------
    //! @brief    インスタンス構築時に必要となるアライメントの大きさです。
    //---------------------------------------------------------------------------
    static const size_t BufferAlignSize = 16;

    //! @name コンストラクタ
    //@{
    //---------------------------------------------------------------------------
    //! @brief    コンストラクタです。
    //!
    //!           インテリアサイズ、最大音量範囲、減衰単位距離はそれぞれ
    //!           1.0 で初期化されます。
    //!
    //---------------------------------------------------------------------------
    Sound3DListener() NN_NOEXCEPT;
    //@}

    //! @name リスナー行列
    //@{
    //---------------------------------------------------------------------------
    //! @brief    リスナー行列を設定します。
    //!
    //!           3D リスナーのリスナー行列を設定します。
    //!           設定された行列は、3D サウンドのパラメータ計算で使用されます。
    //!
    //!           新しくリスナー行列を設定した際、前回設定されたリスナー行列との差分を計算し、
    //!           リスナーの速度が自動的に設定されます。
    //!           リスナーの速度は、ドップラー効果による音程変化に反映されます。
    //!           速度は @ref SetVelocity で明示的に設定することも可能です。
    //!
    //!           座標が別の位置に飛んだ場合、飛んだ距離の差分で速度が計算されてしまうため、
    //!           極めて高速で移動したように処理され、急激な音程変化が発生します。
    //!           このような場合、この関数を呼び出した後で、
    //!           @ref SetVelocity を呼び出し、
    //!           速度を明示的に設定する必要があることに注意してください。
    //!
    //! @param[in] mtx    リスナー行列です。
    //!
    //! @see GetMatrix
    //! @see SetVelocity
    //! @see nn::util::Matrix4x3fType 構造体
    //!
    //---------------------------------------------------------------------------
    void SetMatrix( const nn::util::Matrix4x3fType& mtx ) NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief    現在設定されているリスナー行列を取得します。
    //!
    //! @return   現在設定されているリスナー行列を返します。
    //!
    //! @see SetMatrix
    //! @see nn::util::Matrix4x3fType 構造体
    //!
    //---------------------------------------------------------------------------
    const nn::util::Matrix4x3fType& GetMatrix() const NN_NOEXCEPT { return m_Matrix; }

    //---------------------------------------------------------------------------
    //! @brief    リスナー行列をリセットします。
    //!
    //!           3D サウンドリスナーのリスナー行列と速度をゼロクリアします。
    //!
    //!           この関数でリセットした直後の @ref SetMatrix の呼び出しでは、
    //!           3D サウンドリスナーの速度は設定されません。
    //!
    //! @see SetMatrix
    //! @see SetVelocity
    //!
    //---------------------------------------------------------------------------
    void ResetMatrix() NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief    設定されているリスナーの位置情報を取得します。
    //!
    //!           この情報は、@ref SetMatrix で設定されたリスナー行列を元に計算されます。
    //!
    //! @return   現在設定されているリスナーの位置情報を返します。
    //!
    //! @see SetMatrix
    //! @see nn::util::Vector3fType 構造体
    //!
    //---------------------------------------------------------------------------
    const nn::util::Vector3fType& GetPosition() const NN_NOEXCEPT { return m_Position; }

    //---------------------------------------------------------------------------
    //! @brief    リスナーの速度を設定します。
    //!
    //!           設定された速度は、ドップラー効果による音程変化に反映されます。
    //!
    //!           3D サウンドリスナーの速度は @ref SetMatrix で座標を設定した際、
    //!           前回の登録座標との差分から自動的に設定されますが、
    //!           この関数を使用すると 3D サウンドリスナーの速度を手動で設定することができます。
    //!
    //! @param[in] velocity   リスナーの速度です。
    //!
    //! @see GetVelocity
    //! @see SetMatrix
    //! @see nn::util::Vector3fType 構造体
    //!
    //---------------------------------------------------------------------------
    void SetVelocity( const nn::util::Vector3fType& velocity ) NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief    設定されているリスナーの速度を取得します。
    //!
    //!           通常、@ref SetMatrix で座標を設定した際、
    //!           前回の登録座標との差分から自動的に設定されますが、
    //!           @ref SetVelocity を使用して手動で設定することも可能です。
    //!
    //! @return   現在設定されているリスナーの速度を返します。
    //!
    //! @see SetVelocity
    //! @see SetMatrix
    //! @see nn::util::Vector3fType 構造体
    //!
    //---------------------------------------------------------------------------
    const nn::util::Vector3fType& GetVelocity() const NN_NOEXCEPT { return m_Velocity; }
    //@}

    //! @name パラメータ
    //@{
    //---------------------------------------------------------------------------
    //! @brief    インテリアサイズを設定します。
    //!
    //!           インテリアサイズとは、パンが変化するエリアの大きさで、
    //!           リスナーからの距離（半径）で設定します。
    //!           インテリアサイズを大きくすると、パン変化が緩やかになります。
    //!           逆にインテリアサイズを小さくすると、パン変化が急になります。
    //!
    //!           リスナーからの距離が離れると音量は減衰していきますが、
    //!           減衰量は最大音量範囲の設定 (@ref SetMaxVolumeDistance で設定します)
    //!           に依存します。
    //!
    //! @param[in] interiorSize   インテリアサイズです。
    //!
    //! @see GetInteriorSize
    //! @see SetMaxVolumeDistance
    //!
    //---------------------------------------------------------------------------
    void SetInteriorSize( float interiorSize ) NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief    現在設定されているインテリアサイズを取得します。
    //!
    //! @return   現在設定されているインテリアサイズを返します。
    //!
    //! @see SetInteriorSize
    //!
    //---------------------------------------------------------------------------
    float GetInteriorSize() const NN_NOEXCEPT { return m_InteriorSize; }

    //---------------------------------------------------------------------------
    //! @brief    最大音量範囲を設定します。
    //!
    //!           リスナーとアクターの距離が最大音量範囲よりも小さければ、
    //!           そのアクターが再生するサウンドは音量の減衰無く再生されます。
    //!           最大音量範囲を超えると、その距離に応じて再生するサウンドの音量が減衰されます。
    //!
    //! @param[in] maxVolumeDistance  最大音量範囲です。
    //!
    //! @see GetMaxVolumeDistance
    //!
    //---------------------------------------------------------------------------
    void SetMaxVolumeDistance( float maxVolumeDistance ) NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief    現在設定されている最大音量範囲を取得します。
    //!
    //! @return   現在設定されている最大音量範囲を返します。
    //!
    //! @see SetMaxVolumeDistance
    //!
    //---------------------------------------------------------------------------
    float GetMaxVolumeDistance() const NN_NOEXCEPT { return m_MaxVolumeDistance; }

    //---------------------------------------------------------------------------
    //! @brief    音量減衰の単位距離を設定します。
    //!
    //!           一般的に 3D サウンドアクターと 3D サウンドリスナーの距離が離れると、
    //!           サウンドの音量は減衰していきます。
    //!           減衰の速度は、サウンドごとに設定される減衰係数と、
    //!           この単位距離によって定まります。
    //!
    //!           サウンドが単位距離だけリスナーから離れるたびに、
    //!           サウンドの音量が減衰係数に応じて下がります。
    //!           減衰の仕方はサウンドごとに設定される減衰曲線に従います。
    //!
    //! @param[in] unitDistance   音量減衰の単位距離です。
    //!
    //! @see GetUnitDistance
    //!
    //---------------------------------------------------------------------------
    void SetUnitDistance( float unitDistance ) NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief    現在設定されている音量減衰の単位距離を取得します。
    //!
    //! @return   現在設定されている音量減衰の単位距離を返します。
    //!
    //! @see SetUnitDistance
    //!
    //---------------------------------------------------------------------------
    float GetUnitDistance() const NN_NOEXCEPT { return m_UnitDistance; }

    //---------------------------------------------------------------------------
    //! @brief    リスナーにユーザーパラメータを設定します。
    //!
    //! @param[in] param ユーザーパラメータです。
    //!
    //! @see GetUserParam
    //!
    //---------------------------------------------------------------------------
    void SetUserParam( uint32_t param ) NN_NOEXCEPT { m_UserParam = param; }

    //---------------------------------------------------------------------------
    //! @brief    設定されているユーザーパラメータを取得します。
    //!
    //! @return   現在設定されているリスナーのユーザーパラメータを返します。
    //!
    //! @see SetUserParam
    //!
    //---------------------------------------------------------------------------
    uint32_t GetUserParam() const NN_NOEXCEPT { return m_UserParam; }

    //---------------------------------------------------------------------------
    //! @brief  Biquad フィルタのかかり具合を設定します。
    //!
    //!         本関数で設定された値は、3D サウンド計算のデフォルト実装
    //!         (@ref Sound3DCalculator::CalculateBiquadFilterValue)
    //!         では、下記計算式における「かかり具合」として利用されています。
    //!
    //! @code
    //! Biquad フィルタ値 ＝
    //!     { ( アクターとリスナーとの距離 － 最大音量範囲 ) ÷ 音量減衰単位距離 }
    //!     × かかり具合
    //! @endcode
    //!
    //!         (デフォルト実装 (@ref Sound3DCalculator::CalculateBiquadFilterValue) では、
    //!         上記で求められた Biquad フィルタ値が @ref SetMaxBiquadFilterValue
    //!         で設定した値より大きい場合は、@ref SetMaxBiquadFilterValue
    //!         で設定した値に丸められます。)
    //!
    //!         デフォルトでは 0.5f が設定されています。
    //!
    //! @param[in] value    Biquad フィルタのかかり具合です。
    //!
    //! @see Sound3DCalculator::CalculateBiquadFilterValue
    //! @see GetUnitBiquadFilterValue
    //! @see SetMaxBiquadFilterValue
    //---------------------------------------------------------------------------
    void SetUnitBiquadFilterValue( float value ) NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief  Biquad フィルタのかかり具合を取得します。
    //!
    //!         3D サウンド計算のデフォルト実装 (@ref Sound3DCalculator::CalculateBiquadFilterValue)
    //!         で呼び出されます。
    //!
    //!         また 3D サウンド計算をカスタマイズする際に本関数を呼び出すことで、
    //!         リスナー別の「Biquad フィルタのかかり具合」として利用することができます。
    //!
    //! @return Biquad フィルタのかかり具合を返します。
    //!
    //! @see SetUnitBiquadFilterValue
    //---------------------------------------------------------------------------
    float GetUnitBiquadFilterValue() const NN_NOEXCEPT { return m_UnitBiquadFilterValue; }

    //---------------------------------------------------------------------------
    //! @brief  Biquad フィルタのかかり具合の最大値を設定します。
    //!
    //!         デフォルトでは 1.0f が設定されています。
    //!
    //!         本関数で設定された値が、
    //!         3D サウンド計算のデフォルト実装内でどのようにの利用されているかについては、
    //!         @ref SetUnitBiquadFilterValue 関数のリファレンスを参照しください。
    //!
    //! @param[in] value    Biquad フィルタのかかり具合の最大値です。
    //!
    //! @see SetUnitBiquadFilterValue
    //! @see GetMaxBiquadFilterValue
    //---------------------------------------------------------------------------
    void SetMaxBiquadFilterValue( float value ) NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief  Biquad フィルタのかかり具合の最大値を取得します。
    //!
    //!         3D サウンド計算のデフォルト実装 (@ref Sound3DCalculator::CalculateBiquadFilterValue)
    //!         で呼び出されます。
    //!
    //!         また 3D サウンド計算をカスタマイズする際に本関数を呼び出すことで、
    //!         リスナー別の「Biquad フィルタのかかり具合の最大値」
    //!         として利用することができます。
    //!
    //! @return  Biquad フィルタのかかり具合の最大値を返します。
    //---------------------------------------------------------------------------
    float GetMaxBiquadFilterValue() const NN_NOEXCEPT { return m_MaxBiquadFilterValue; }

    //---------------------------------------------------------------------------
    //! @brief  出力先属性を設定します。
    //!
    //!         @ref Sound3DEngine::UpdateAmbientParam をオーバーライドして
    //!         3D サウンド計算をカスタマイズする際、ここで設定した出力先属性を
    //!         @ref GetOutputTypeFlag で参照することで、
    //!         出力先別に処理を書き分けることができます。
    //!
    //!         type には @ref OutputType の論理和を設定します。
    //!         デフォルトでは OutputType_Tv が設定されています。
    //!
    //! @param[in] outputTypeFlag  設定する出力先属性です。
    //!
    //! @see OutputType
    //! @see GetOutputTypeFlag
    //---------------------------------------------------------------------------
    void SetOutputTypeFlag( uint32_t outputTypeFlag ) NN_NOEXCEPT { m_OutputTypeFlag = outputTypeFlag; }

    //---------------------------------------------------------------------------
    //! @brief  出力先属性を取得します。
    //!
    //!         @ref Sound3DEngine::UpdateAmbientParam をオーバーライドして
    //!         3D サウンド計算をカスタマイズする際、@ref SetOutputTypeFlag
    //!         で設定した出力先属性を本関数経由で参照することで、
    //!         出力先別に処理を書き分けることができます。
    //!
    //!         返り値として @ref OutputType の論理和を取得することができます。
    //!
    //! @return     設定されている出力先属性を返します。
    //!
    //! @see OutputType
    //! @see SetOutputTypeFlag
    //---------------------------------------------------------------------------
    uint32_t GetOutputTypeFlag() const NN_NOEXCEPT { return m_OutputTypeFlag; }
    //@}

private:
    nn::util::Matrix4x3fType m_Matrix;
    nn::util::Vector3fType m_Position;
    nn::util::Vector3fType m_Velocity;
    float m_InteriorSize;
    float m_MaxVolumeDistance;
    float m_UnitDistance;
    uint32_t m_UserParam;
    float m_UnitBiquadFilterValue;
    float m_MaxBiquadFilterValue;
    uint32_t m_OutputTypeFlag;
    bool m_ResetMatrixFlag;

public:
    //! @internal
    util::IntrusiveListNode m_LinkNode;
};

} // namespace nn::atk
} // namespace nn
