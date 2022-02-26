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
#include <nn/TargetConfigs/build_Base.h>

#if !defined( NN_BUILD_CONFIG_OS_COS )
#include <nn/os/os_Mutex.h>
#endif
#include <nn/vfx/vfx_Heap.h>
#include <nn/vfx/vfx_Enum.h>

namespace nn {
namespace vfx {
namespace detail {

//---------------------------------------------------------------------------
//! @briefprivate   静的ヒープからメモリを設定します。
//! @param[in]      pHeap ヒープへのポインタ
//---------------------------------------------------------------------------
void SetStaticHeap( Heap* pHeap ) NN_NOEXCEPT;

//---------------------------------------------------------------------------
//! @briefprivate   静的ヒープを取得します。
//! @return         静的ヒープへのポインタ
//---------------------------------------------------------------------------
Heap* GetStaticHeap() NN_NOEXCEPT;

//---------------------------------------------------------------------------
//! @briefprivate       静的ヒープからメモリを確保します。
//! @param[in] size     確保するメモリサイズ
//! @param[in] alignment    アライメントサイズ
//! @return             メモリの先頭アドレス
//---------------------------------------------------------------------------
void* AllocFromStaticHeap( size_t size, size_t alignment = 128 ) NN_NOEXCEPT;

//---------------------------------------------------------------------------
//! @briefprivate   静的ヒープからメモリを解放します。
//! @param[in] ptr  メモリの先頭アドレス
//---------------------------------------------------------------------------
void FreeFromStaticHeap( void* ptr ) NN_NOEXCEPT;

//---------------------------------------------------------------------------
//! @brief  静的ヒープから確保されたメモリサイズを取得します。
//! @return メモリサイズ
//---------------------------------------------------------------------------
size_t GetAllocatedSizeFromStaticHeap() NN_NOEXCEPT;

//---------------------------------------------------------------------------
//! @brief      （※旧 API ）静的ヒープから確保されたメモリサイズを取得します。
//! @deprecated 旧型式の API 名です。nn::vfx::detail::GetAllocatedSizeFromStaticHeap() に乗り換えてください。
//! @return     静的ヒープから確保されたメモリサイズ
//! @see        nn::vfx::Config::GetAllocatedSizeFromStaticHeap()
//---------------------------------------------------------------------------
NN_DEPRECATED inline size_t GetAllocedSizeFromStaticHeap() NN_NOEXCEPT
{
    return GetAllocatedSizeFromStaticHeap();
}

//---------------------------------------------------------------------------
//! @briefprivate           動的ヒープからメモリを設定します。
//! @param[in] pHeap  ヒープへのポインタ
//---------------------------------------------------------------------------
void SetDynamicHeap( Heap* pHeap ) NN_NOEXCEPT;

//---------------------------------------------------------------------------
//! @briefprivate           動的ヒープ本体のアドレスを取得します。
//! @return ヒープへのポインタ
//---------------------------------------------------------------------------
Heap* GetDynamicHeap() NN_NOEXCEPT;

//---------------------------------------------------------------------------
//! @briefprivate       動的ヒープからメモリを確保します。
//! @param[in] size             確保するメモリサイズ
//! @param[in] alignmentAddr    確保するメモリ先頭アドレスのアライメント
//! @param[in] alignmentSize    確保するメモリサイズのアライメント
//! @return             メモリの先頭アドレス
//---------------------------------------------------------------------------
void* AllocFromDynamicHeap( size_t size, size_t alignmentAddr = 256, size_t alignmentSize = 256 ) NN_NOEXCEPT;

//---------------------------------------------------------------------------
//! @briefprivate           動的ヒープからメモリを解放します。
//! @param[in] ptr          メモリの先頭アドレス
//! @param[in] immediate    即時解放フラグ
//---------------------------------------------------------------------------
void FreeFromDynamicHeap( void* ptr, bool immediate = true ) NN_NOEXCEPT;

//---------------------------------------------------------------------------
//! @briefprivate           遅延解放用のリストを初期化します。
//!
//!                         動的ヒープから破棄されるメモリは遅延解放されます。
//! @param[in] freeListCount  遅延リストの数
//---------------------------------------------------------------------------
void InitializeDelayFreeList( int freeListCount ) NN_NOEXCEPT;

//---------------------------------------------------------------------------
//! @briefprivate 遅延解放用のリストを破棄します。
//!
//!               動的ヒープから破棄されるメモリは遅延解放されます。
//---------------------------------------------------------------------------
void FinalizeDelayFreeList() NN_NOEXCEPT;

//---------------------------------------------------------------------------
//! @briefprivate 遅延解放用のリスト内アドレスを解放します。
//---------------------------------------------------------------------------
void FlushDelayFreeList() NN_NOEXCEPT;

//---------------------------------------------------------------------------
//! @briefprivate   遅延解放の為のフリーリストへアドレスを追加する
//! @param[in] ptr  遅延解放させるメモリのアドレス
//---------------------------------------------------------------------------
void AddFreeListForDynamicHeap( void* ptr ) NN_NOEXCEPT;


//---------------------------------------------------------------------------
//! @brief          ランタイムログ出力を抑制行います。
//! @param[in] flag 制御フラグ
//---------------------------------------------------------------------------
void SetSuppressOutputLog( bool flag ) NN_NOEXCEPT;

//---------------------------------------------------------------------------
//! @brief              ランタイムログ出力を行います。
//! @param[in] format   フォーマット文字列
//---------------------------------------------------------------------------
void OutputLog( const char* format, ... ) NN_NOEXCEPT;

//---------------------------------------------------------------------------
//! @brief              ランタイム警告出力を行います。
//! @param[in] format   フォーマット文字列
//---------------------------------------------------------------------------
void OutputWarning( const char* format, ... ) NN_NOEXCEPT;

//---------------------------------------------------
//! @brief                  ランタイム警告出力を行います。
//! @param[in] context      データ参照元へのポインタ
//! @param[in] warningId    警告ID
//---------------------------------------------------
void Warning( void* context, RuntimeWarningId warningId ) NN_NOEXCEPT;

//---------------------------------------------------------------------------
//! @brief  ランタイム警告のビットフィールドを取得します。
//! @return ランタイム警告のビットフィールド
//---------------------------------------------------------------------------
uint32_t GetWarningBitFlag() NN_NOEXCEPT;

//---------------------------------------------------------------------------
//! @brief              ランタイムエラー出力を行います。
//! @param[in] format   フォーマット文字列
//---------------------------------------------------------------------------
void OutputError( const char* format, ... ) NN_NOEXCEPT;

//---------------------------------------------------
//! @brief              ログ・警告・エラー出力時コールバックの定義です。
//! @param[in] format   フォーマット文字列
//! @param[in] args    省略可能な引数
//---------------------------------------------------
typedef void( *OutputMessageCallback )( const char* format, va_list args );

//---------------------------------------------------
//! @brief  ランタイムログ出力時コールバックを設定します。
//! @param[in] callback コールバック
//---------------------------------------------------
void SetOutputLogCallBack( OutputMessageCallback callback ) NN_NOEXCEPT;

//---------------------------------------------------
//! @brief  ランタイム警告出力時コールバックを設定します。
//! @param[in] callback コールバック
//---------------------------------------------------
void SetOutputWarningCallBack( OutputMessageCallback callback ) NN_NOEXCEPT;

//---------------------------------------------------
//! @brief  ランタイムエラー出力時コールバックを設定します。
//! @param[in] callback コールバック
//---------------------------------------------------
void SetOutputErrorCallBack( OutputMessageCallback callback ) NN_NOEXCEPT;

//---------------------------------------------------------------------------
//! @brief  ランタイム警告のビットフィールドを取得します。
//! @return ランタイム警告のビットフィールド
//---------------------------------------------------------------------------
uint32_t GetWarningBitFlag() NN_NOEXCEPT;

//---------------------------------------------------------------------------
//! @brief  文字列出力を行います。
//! @param[in] format   フォーマット文字列
//! @param[in] args    省略可能な引数
//---------------------------------------------------------------------------
void VPrintf( const char *format, va_list args ) NN_NOEXCEPT;

//------------------------------------------------------------------------------
//! @briefprivate   fract() GLSL 4.00準拠
//! @param[in]      x   float 値
//! @return         x の小数部
//------------------------------------------------------------------------------
inline float Fract( float x ) NN_NOEXCEPT
{
    return ( x - ::std::floorf( x ) );
}

//------------------------------------------------------------------------------
//! @briefprivate   step() GLSL 4.00準拠
//! @param[in]      edge    閾値
//! @param[in]      x       入力 float 値
//! @return         x の小数部
//------------------------------------------------------------------------------
inline float Step( float edge, float x ) NN_NOEXCEPT
{
    return ( x < edge ) ? 0.0f : 1.0f;
}

//---------------------------------------------------------------------------
//! @briefprivate   nn::util::Float3 <- nn::util::Float2 の簡略化。
//!                 要素 w は無視する。
//! @param[out]     pOutValue   出力用 nn::util::Float3
//! @param[in]      source      入力用 nn::util::Float2
//---------------------------------------------------------------------------
inline void Float2Copy( nn::util::Float3* pOutValue, const nn::util::Float2& source ) NN_NOEXCEPT
{
    pOutValue->x = source.x;
    pOutValue->y = source.y;
}

//---------------------------------------------------------------------------
//! @briefprivate   nn::util::Float2 <- nn::util::Float3 の簡略化。
//!                 要素 w は無視する。
//! @param[out]     pOutValue   出力用 nn::util::Float2
//! @param[in]      source      入力用 nn::util::Float3
//---------------------------------------------------------------------------
inline void Float2Copy( nn::util::Float2* pOutValue, const nn::util::Float3& source ) NN_NOEXCEPT
{
    pOutValue->x = source.x;
    pOutValue->y = source.y;
}

//---------------------------------------------------------------------------
//! @briefprivate   nn::util::Float2 <- nn::util::Float4 の簡略化。
//!                 要素 w は無視する。
//! @param[out]     pOutValue   出力用 nn::util::Float2
//! @param[in]      source      入力用 nn::util::Float4
//---------------------------------------------------------------------------
inline void Float2Copy( nn::util::Float2* pOutValue, const nn::util::Float4& source ) NN_NOEXCEPT
{
    pOutValue->x = source.x;
    pOutValue->y = source.y;
}

//---------------------------------------------------------------------------
//! @briefprivate   nn::util::Float3 <- nn::util::Float4 の簡略化。
//!                 要素 w は無視する。
//! @param[out]     pOutValue   出力用 nn::util::Float3
//! @param[in]      source      入力用 nn::util::Float4
//---------------------------------------------------------------------------
inline void Float3Copy( nn::util::Float3* pOutValue, const nn::util::Float4& source ) NN_NOEXCEPT
{
    pOutValue->x = source.x;
    pOutValue->y = source.y;
    pOutValue->z = source.z;
}

//---------------------------------------------------------------------------
//! @briefprivate   nn::util::Float4 <- nn::util::Float3 の簡略化。
//!                 要素 w にはアクセスしません。
//! @param[out]     pOutValue   出力用 nn::util::Float4
//! @param[in]      source      入力用 nn::util::Float3
//---------------------------------------------------------------------------
inline void Float3Copy( nn::util::Float4* pOutValue, const nn::util::Float3& source ) NN_NOEXCEPT
{
    pOutValue->x = source.x;
    pOutValue->y = source.y;
    pOutValue->z = source.z;
}

//---------------------------------------------------------------------------
//! @briefprivate   nn::util::Float4 <- nn::util::Float4 （ただしxyz要素のみコピー）の簡略化。
//!                 要素 w にはアクセスしません。
//! @param[out]     pOutValue   出力用 nn::util::Float4
//! @param[in]      source      入力用 nn::util::Float4
//---------------------------------------------------------------------------
inline void Float3Copy( nn::util::Float4* pOutValue, const nn::util::Float4& source ) NN_NOEXCEPT
{
    pOutValue->x = source.x;
    pOutValue->y = source.y;
    pOutValue->z = source.z;
}

//---------------------------------------------------------------------------
//! @briefprivate   nn::util::Vector3fType <- nn::util::Vector3fType
//! @param[out]     pOutValue   出力用 nn::util::Vector3fType
//! @param[in]      source      入力用 nn::util::Vector3fType
//---------------------------------------------------------------------------
inline void Vector3fCopy( nn::util::Vector3fType* pOutValue, const nn::util::Vector3fType& source ) NN_NOEXCEPT
{
    *pOutValue = source;
}

//---------------------------------------------------------------------------
//! @briefprivate   nn::util::Vector4fType <- nn::util::Vector3fType （ただしxyz要素のみコピー）の簡略化。
//!                 要素 w にはアクセスしません。
//! @param[out]     pOutValue   出力用 nn::util::Vector4fType
//! @param[in]      source      入力用 nn::util::Vector3fType
//---------------------------------------------------------------------------
inline void Vector3fCopy( nn::util::Vector4fType* pOutValue, const nn::util::Vector3fType& source ) NN_NOEXCEPT
{
    nn::util::VectorSetX( pOutValue, nn::util::VectorGetX( source ) );
    nn::util::VectorSetY( pOutValue, nn::util::VectorGetY( source ) );
    nn::util::VectorSetZ( pOutValue, nn::util::VectorGetZ( source ) );
}

//---------------------------------------------------------------------------
//! @briefprivate   nn::util::Vector3fType <- nn::util::Vector4fType （ただしxyz要素のみコピー）の簡略化。
//!                 要素 w にはアクセスしません。
//! @param[out]     pOutValue   出力用 nn::util::Vector3fType
//! @param[in]      source      入力用 nn::util::Vector4fType
//---------------------------------------------------------------------------
inline void Vector3fCopy( nn::util::Vector3fType* pOutValue, const nn::util::Vector4fType& source ) NN_NOEXCEPT
{
    nn::util::VectorSetX( pOutValue, nn::util::VectorGetX( source ) );
    nn::util::VectorSetY( pOutValue, nn::util::VectorGetY( source ) );
    nn::util::VectorSetZ( pOutValue, nn::util::VectorGetZ( source ) );
}

//---------------------------------------------------------------------------
//! @briefprivate   nn::util::Vector4fType <- nn::util::Vector4fType （ただしxyz要素のみコピー）の簡略化。
//!                 要素 w にはアクセスしません。
//! @param[out]     pOutValue   出力用 nn::util::Vector4fType
//! @param[in]      source      入力用 nn::util::Vector4fType
//---------------------------------------------------------------------------
inline void Vector3fCopy( nn::util::Vector4fType* pOutValue, const nn::util::Vector4fType& source ) NN_NOEXCEPT
{
    nn::util::VectorSetX( pOutValue, nn::util::VectorGetX( source ) );
    nn::util::VectorSetY( pOutValue, nn::util::VectorGetY( source ) );
    nn::util::VectorSetZ( pOutValue, nn::util::VectorGetZ( source ) );
}

//---------------------------------------------------------------------------
//! @briefprivate   SRT 行列を生成する処理の簡略化。
//! @param[out]     pOutMatrix  出力用 nn::util::Matrix4x3fType
//! @param[in]      scale       スケール
//! @param[in]      rotateXyz   回転（XYZ軸）
//! @param[in]      translate   平行移動
//---------------------------------------------------------------------------
inline void MatrixCreateSrtXyz( nn::util::Matrix4x3fType* pOutMatrix, const nn::util::Vector3fType& scale, const nn::util::Vector3fType& rotateXyz, const nn::util::Vector3fType& translate ) NN_NOEXCEPT
{
    // SRT 行列を生成（回転はXYZ）
    nn::util::MatrixSetScaleRotateXyz( pOutMatrix, scale, rotateXyz );
    nn::util::MatrixSetAxisW( pOutMatrix, translate );
}

//---------------------------------------------------------------------------
//! @briefprivate   nn::util::Float4x4 を単位行列で初期化する簡略化。
//! @param[out]     pOutMatrix  出力用 nn::util::Float4x4
//---------------------------------------------------------------------------
inline void MatrixIdentity( nn::util::Float4x4* pOutMatrix ) NN_NOEXCEPT
{
    pOutMatrix->m[ 0 ][ 0 ] = 1.f;
    pOutMatrix->m[ 0 ][ 1 ] = 0.f;
    pOutMatrix->m[ 0 ][ 2 ] = 0.f;
    pOutMatrix->m[ 0 ][ 3 ] = 0.f;

    pOutMatrix->m[ 1 ][ 0 ] = 0.f;
    pOutMatrix->m[ 1 ][ 1 ] = 1.f;
    pOutMatrix->m[ 1 ][ 2 ] = 0.f;
    pOutMatrix->m[ 1 ][ 3 ] = 0.f;

    pOutMatrix->m[ 2 ][ 0 ] = 0.f;
    pOutMatrix->m[ 2 ][ 1 ] = 0.f;
    pOutMatrix->m[ 2 ][ 2 ] = 1.f;
    pOutMatrix->m[ 2 ][ 3 ] = 0.f;

    pOutMatrix->m[ 3 ][ 0 ] = 0.f;
    pOutMatrix->m[ 3 ][ 1 ] = 0.f;
    pOutMatrix->m[ 3 ][ 2 ] = 0.f;
    pOutMatrix->m[ 3 ][ 3 ] = 1.f;
}

//---------------------------------------------------------------------------
//! @briefprivate   Matrix4x4fType <- Matrix4x3fType の簡略化。
//! @param[out]     pOutMatrix  出力用 nn::util::Matrix4x4fType
//! @param[in]      source      入力となる nn::util::Matrix4x3fType
//---------------------------------------------------------------------------
inline void Matrix4x3fTo4x4f( nn::util::Matrix4x4fType* pOutMatrix, const nn::util::Matrix4x3fType& source ) NN_NOEXCEPT
{
    nn::util::Vector3fType axisX, axisY, axisZ, axisW;
    nn::util::MatrixGetAxisX( &axisX, source );
    nn::util::MatrixGetAxisY( &axisY, source );
    nn::util::MatrixGetAxisZ( &axisZ, source );
    nn::util::MatrixGetAxisW( &axisW, source );

    nn::util::MatrixSet( pOutMatrix,
        nn::util::VectorGetX( axisX ), nn::util::VectorGetY( axisX ), nn::util::VectorGetZ( axisX ), 0,
        nn::util::VectorGetX( axisY ), nn::util::VectorGetY( axisY ), nn::util::VectorGetZ( axisY ), 0,
        nn::util::VectorGetX( axisZ ), nn::util::VectorGetY( axisZ ), nn::util::VectorGetZ( axisZ ), 0,
        nn::util::VectorGetX( axisW ), nn::util::VectorGetY( axisW ), nn::util::VectorGetZ( axisW ), 1 );
}

//---------------------------------------------------------------------------
//! @briefprivate   Float4x4 <- Matrix4x3fType の簡略化。
//! @param[out]     pOutValue   出力用 nn::util::Float4x4
//! @param[in]      source      入力となる nn::util::Matrix4x3fType
//---------------------------------------------------------------------------
inline void MatrixStore( nn::util::Float4x4* pOutValue, const nn::util::Matrix4x3fType& source ) NN_NOEXCEPT
{
    nn::util::Vector3fType axisX, axisY, axisZ, axisW;
    nn::util::MatrixGetAxisX( &axisX, source );
    nn::util::MatrixGetAxisY( &axisY, source );
    nn::util::MatrixGetAxisZ( &axisZ, source );
    nn::util::MatrixGetAxisW( &axisW, source );

    pOutValue->m[ 0 ][ 0 ] = nn::util::VectorGetX( axisX );
    pOutValue->m[ 0 ][ 1 ] = nn::util::VectorGetY( axisX );
    pOutValue->m[ 0 ][ 2 ] = nn::util::VectorGetZ( axisX );
    pOutValue->m[ 0 ][ 3 ] = 0;

    pOutValue->m[ 1 ][ 0 ] = nn::util::VectorGetX( axisY );
    pOutValue->m[ 1 ][ 1 ] = nn::util::VectorGetY( axisY );
    pOutValue->m[ 1 ][ 2 ] = nn::util::VectorGetZ( axisY );
    pOutValue->m[ 1 ][ 3 ] = 0;

    pOutValue->m[ 2 ][ 0 ] = nn::util::VectorGetX( axisZ );
    pOutValue->m[ 2 ][ 1 ] = nn::util::VectorGetY( axisZ );
    pOutValue->m[ 2 ][ 2 ] = nn::util::VectorGetZ( axisZ );
    pOutValue->m[ 2 ][ 3 ] = 0;

    pOutValue->m[ 3 ][ 0 ] = nn::util::VectorGetX( axisW );
    pOutValue->m[ 3 ][ 1 ] = nn::util::VectorGetY( axisW );
    pOutValue->m[ 3 ][ 2 ] = nn::util::VectorGetZ( axisW );
    pOutValue->m[ 3 ][ 3 ] = 1;
}

//---------------------------------------------------------------------------
//! @briefprivate   パーティクルソート用の構造体
//---------------------------------------------------------------------------
struct SortData
{
    float        param; //!< 比較値
    int          index; //!< 自身のインデックス
};

//---------------------------------------------------------------------------
//! @briefprivate   昇順に並べる場合の比較関数（インデックス順維持）
//---------------------------------------------------------------------------
template < class DataType >
class SortCompareGreaterIndexStable
{
public:
    //---------------------------------------------------------------------------
    //! @briefprivate       比較関数。std::sort の仕様に準ずる。
    //! @param[in]      lhs 左辺値
    //! @param[in]      rhs 右辺値
    //! @return             比較結果。std::sort の仕様に準ずる。
    //---------------------------------------------------------------------------
    bool operator()( const DataType& lhs, const DataType& rhs ) const NN_NOEXCEPT
    {
        if( lhs.param > rhs.param )
        {
            return false;
        }
        else if( lhs.param < rhs.param )
        {
            return true;
        }
        else
        {
            // valueが等しい場合はindex順（配列に積まれた順）に従う（※これは昇順／降順に依らない）
            return ( lhs.index < rhs.index );
        }
    }
};

//---------------------------------------------------------------------------
//! @briefprivate   降順に並べる場合の比較関数（インデックス順維持）
//---------------------------------------------------------------------------
template < class DataType >
class SortCompareLessIndexStable
{
public:
    //---------------------------------------------------------------------------
    //! @briefprivate       比較関数。std::sort の仕様に準ずる。
    //! @param[in]      lhs 左辺値
    //! @param[in]      rhs 右辺値
    //! @return             比較結果。std::sort の仕様に準ずる。
    //---------------------------------------------------------------------------
    bool operator()( const DataType& lhs, const DataType& rhs ) const NN_NOEXCEPT
    {
        if( lhs.param < rhs.param )
        {
            return false;
        }
        else if( lhs.param > rhs.param )
        {
            return true;
        }
        else
        {
            // valueが等しい場合はindex順（配列に積まれた順）に従う（※これは昇順／降順に依らない）
            return ( lhs.index < rhs.index );
        }
    }
};

//---------------------------------------------------------------------------
//! @briefprivate   EmitterSet ソート用の比較関数（uintで比較）
//---------------------------------------------------------------------------
template < class DataType >
class SortCompareLessUInt
{
public:
    //---------------------------------------------------------------------------
    //! @briefprivate       比較関数。std::sort の仕様に準ずる。
    //! @param[in]      lhs 左辺値
    //! @param[in]      rhs 右辺値
    //! @return             比較結果。std::sort の仕様に準ずる。
    //---------------------------------------------------------------------------
    bool operator() ( const DataType& lhs, const DataType& rhs ) const NN_NOEXCEPT
    {
        return ( lhs.param > rhs.param );
    }
};

//---------------------------------------------------------------------------
//! @briefprivate   Zソートのための比較関数
//---------------------------------------------------------------------------
template < class DataType >
class SortCompareViewZ
{
public:
    //---------------------------------------------------------------------------
    //! @briefprivate       比較関数。std::sort の仕様に準ずる。
    //! @param[in]      lhs 左辺値
    //! @param[in]      rhs 右辺値
    //! @return             比較結果。std::sort の仕様に準ずる。
    //---------------------------------------------------------------------------
    bool operator() ( const DataType& lhs, const DataType& rhs ) const NN_NOEXCEPT
    {
        if( ( lhs.param < 0.0f ) && ( rhs.param < 0.0f ) )
        {
            return ( lhs.param < rhs.param );
        }
        else
        {
            return ( lhs.param > rhs.param );
        }
    }
};

//---------------------------------------------------------------------------
//! @briefprivate   逆Zソートのための比較関数
//---------------------------------------------------------------------------
template < class DataType >
class SortCompareViewInvZ
{
public:
    //---------------------------------------------------------------------------
    //! @briefprivate       比較関数。std::sort の仕様に準ずる。
    //! @param[in]      lhs 左辺値
    //! @param[in]      rhs 右辺値
    //! @return             比較結果。std::sort の仕様に準ずる。
    //---------------------------------------------------------------------------
    bool operator() (const DataType& lhs, const DataType& rhs) const NN_NOEXCEPT
    {
        if ((lhs.param < 0.0f) && (rhs.param < 0.0f))
        {
            return (lhs.param > rhs.param);
        }
        else
        {
            return (lhs.param < rhs.param);
        }
    }
};



//---------------------------------------------------------------------------
//! @briefprivate  Mutex
//---------------------------------------------------------------------------
class Mutex
{
    NN_DISALLOW_COPY( Mutex );
public:
    Mutex() NN_NOEXCEPT
    {
#if !defined( NN_BUILD_CONFIG_OS_COS )
        nn::os::InitializeMutex( &m_Mutex, false, 0 );
#endif
    }

    ~Mutex() NN_NOEXCEPT
    {
#if !defined( NN_BUILD_CONFIG_OS_COS )
        nn::os::FinalizeMutex( &m_Mutex );
#endif
    }

    void Lock() NN_NOEXCEPT
    {
#if !defined( NN_BUILD_CONFIG_OS_COS )
        nn::os::LockMutex( &m_Mutex );
#endif
    }

    void Unlock() NN_NOEXCEPT
    {
#if !defined( NN_BUILD_CONFIG_OS_COS )
        nn::os::UnlockMutex( &m_Mutex );
#endif
    }

private:
#if !defined( NN_BUILD_CONFIG_OS_COS )
    nn::os::MutexType       m_Mutex;
#endif
};

//------------------------------------------------------------------------------
//! @brief                  エルミート補間
//! @param[out] pOutPos     補完された位置
//! @param[in]  p1          始点
//! @param[in]  v1          始点の方向
//! @param[in]  p2          終点
//! @param[in]  v2          終点の方向
//! @param[in]  relativePos 相対位置
//------------------------------------------------------------------------------
void HermiteInterpolationOnCubic(
    nn::util::Vector3fType*         pOutPos,
    const nn::util::Vector3fType&   p1,
    const nn::util::Vector3fType&   v1,
    const nn::util::Vector3fType&   p2,
    const nn::util::Vector3fType&   v2,
    float                           relativePos ) NN_NOEXCEPT;

//------------------------------------------------------------------------------
//! @brief      入力されたベクトルを任意の軸周りに回転させます。単位は Radian。
//! @param[out] pOutVec         変換後の方向
//! @param[in]  inVec           入力となる方向
//! @param[in]  axis            回転軸
//! @param[in]  rotationRadian  回転量（Radian）
//------------------------------------------------------------------------------
void VectorRotateArbitraryAxis(
    nn::util::Vector3fType*         pOutVec,
    const nn::util::Vector3fType&   inVec,
    const nn::util::Vector3fType&   axis,
    float                           rotationRadian ) NN_NOEXCEPT;

} // namespace detail
} // namespace vfx
} // namespace nn
