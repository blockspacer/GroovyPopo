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
* @brief CPU および GPU の負荷計測に関する API の宣言
*/

#pragma once

#include <nn/nn_Macro.h>
#include <nn/nn_TimeSpan.h>
#include <nn/util/util_Color.h>
#include <nn/util/util_StringUtil.h>
#include <nn/util/util_IntrusiveList.h>
#include <nn/util/util_PlacementArray.h>
#include <nn/gfx.h>
#include <nn/os.h>



namespace nn
{
namespace perf
{

namespace detail
{
class ReferenceTimestamp;
}


//! @brief  現在のスレッドが動作するコア ID を返す関数のインタフェースです。例外を発生させてはいけません。
//! @return スレッドが操作するコア番号を返します。
typedef int (*GetCoreNumberFunction)();

/**
* @brief LoadMeterBase クラスの初期化に必要な情報を表すクラスです。
*        CpuMeter と GpuMeter クラスの初期化時に引数で渡します。
*/
class LoadMeterInfo
{
public:
    /**
    * @brief コンストラクタです。
    * @details 内部で以下の関数を呼びます。
    *          - SeSectionCountMax(0)
    *          - SetBufferNum(2) //ダブルバッファ
    *          - SetCoreCount(0)
    */
    LoadMeterInfo() NN_NOEXCEPT
    {
        this->SetSectionCountMax(0);
        this->SetBufferCount(2);
        this->SetCoreCount(0);
    }

    /**
    * @brief 各パラメータを既定値に設定するためのヘルパー関数です。
    * @details 内部で以下の関数を呼びます。
    *          - SeSectionCountMax(0)
    *          - SetBufferCount(2) //ダブルバッファ
    *          - SetCoreCount(0)
    */
    void SetDefault() NN_NOEXCEPT
    {
        this->SetSectionCountMax(0);
        this->SetBufferCount(2);
        this->SetCoreCount(0);
    };

    //! @name 設定
    //! @{

    /**
    * @brief  LoadMeter の計測区間の上限数を設定します。
    * @param[in] sectionCountMax 計測区間の上限数です。
    */
    void SetSectionCountMax( int sectionCountMax ) NN_NOEXCEPT
    {
        this->m_SectionCountMax = sectionCountMax;
    }

    /**
    * @brief LoadMeter が持つ計測結果保存用のバッファの数を指定します。何フレーム前までの計測結果を保持するかを決定します。
    * @param[in] bufferCount 計測結果を保存する配列バッファの数です。ダブルバッファの 2以上を指定する必要があります。
    * @details フレーム終了時に Next() で集計する結果は (bufferCount - 2) フレーム前の計測結果です。
    */
    void SetBufferCount( int bufferCount) NN_NOEXCEPT
    {
        NN_SDK_ASSERT(bufferCount > 1, "bufferCount must be more than 2");
        this->m_BufferCount = bufferCount;
    }

    /**
    * @brief アプリケーションが動作する CPU コア数を設定します。
    * @param[in] coreCount CPU コア数を指定します。
    * @details GPU コマンドを並列で発行する場合に、正常に GPU の負荷計測を行うための設定です。
    * @n       CpuMeter の初期化ではこの値は無視されます。
    */
    void SetCoreCount( int coreCount) NN_NOEXCEPT
    {
        this->m_CoreCount = coreCount;
    }

    //! @}
    //! @name 取得
    //! @{

    /**
    * @brief LoadMeter の計測区間の上限数を取得します。
    * @return 計測区間の上限数を返します。
    */
    int GetSectionCountMax() NN_NOEXCEPT
    {
        return this->m_SectionCountMax;
    }

    /**
    * @brief LoadMeter が持つ計測結果保存用のバッファの数を取得します。
    * @return 計測結果保存用のバッファの数を返します。
    */
    int GetBufferCount() NN_NOEXCEPT
    {
        return this->m_BufferCount;
    }

    /**
    * @brief CPU コア数を取得します。
    * @return CPU コア数を返します。
    */
    int GetCoreCount() NN_NOEXCEPT
    {
        return this->m_CoreCount;
    }

    //! @}

private:
    int   m_SectionCountMax;    //!< LoadMeterBase が計測可能な区間の上限数です。
    int   m_BufferCount;        //!< 計測結果を保存する配列バッファの数です。何フレーム前までの計測結果を保持するかを決定します。
    int   m_CoreCount;          //!< アプリケーションが動作する CPU コア数です。
};

//! @brief        負荷メーターの基底クラスです。
class LoadMeterBase : public nn::util::IntrusiveListBaseNode<LoadMeterBase>
{
private:
    friend class MeterDrawer;
    friend class LoadMeterCenter;

public:
    //! @brief １つの計測区間(セクション)の計測結果を保持します。
    struct Section
    {
        Section()
            :begin( 0 ),
            end(0),
            color(nn::util::Color4u8::Green()),
            name(NULL),
            tag(0),
            depth(0)
        {}

        nn::os::Tick       begin;           //!< セクションの開始時刻です。
        nn::os::Tick       end;             //!< セクションの終了時刻です。
        nn::util::Color4u8 color;           //!< セクションの色です。
        const char*        name;            //!< セクションの名前です。
        uint32_t           tag;             //!< セクションのタグです。
        int                parent;          //!< 入れ子の親区間（-1ならルート）を示します。
        int                depth;           //!< 入れ子の深さを示します。0 の場合は入れ子ではありません。
    };

    //! @name 計測結果取得
    //! @{

    //! @brief        フレームの最初に計測した開始時刻を取得します。
    //! @return       フレームの最初に計測した開始時刻を返します。
    //! @details      最新の Next() で集計されたフレームの結果が返ります。
    const nn::os::Tick& GetLastTotalBegin() const NN_NOEXCEPT
    {
        return m_SectionArray[ this->m_ReferenceBufferIndex ][ 0 ].begin;
    }

    //! @brief        フレームの最後に計測した終了時刻を取得します。
    //! @return       最後に計測した終了時刻を返します。
    //! @details      最新の Next() で集計されたフレームの結果が返ります。
    const nn::os::Tick& GetLastTotalEnd() const NN_NOEXCEPT
    {
        return m_FinalEnd[ this->m_ReferenceBufferIndex ];
    }

    //! @brief        フレームのトータルの消費時間を取得します。
    //! @return       トータルの消費時間を返します。
    //! @details      入れ子で計測を行った場合、子の時間は加算されません。
    //! @n            最新の Next() で集計されたフレームの結果が返ります。
    nn::TimeSpan GetLastTotalSpan() const NN_NOEXCEPT;

    //! @brief        フレームの計測結果の個数を取得します。
    //! @return       計測結果の個数を返します。
    //! @details      最新の Next() で集計されたフレームの結果が返ります。
    int GetLastSectionCount() const NN_NOEXCEPT
    {
        return m_SectionCount[ this->m_ReferenceBufferIndex ];
    }

    //! @brief            インデクスで指定した計測結果を取得します。
    //! @param[in] index  計測区間を示すインデックスです。
    //! @return           計測結果を返します。
    //! @details          最新の Next() で集計されたフレームの結果が返ります。
    const Section& GetLastResult( int index ) const NN_NOEXCEPT
    {
        return m_SectionArray[ this->m_ReferenceBufferIndex ][ index ];
    }

    //! @}
    //! @name 取得
    //! @{

    //! @brief        表示名を取得します。
    //! @return       表示名です。
    const char* GetName() const NN_NOEXCEPT
    {
        return this->m_Name;
    }

    //! @brief        表示色を取得します。
    //! @return       表示色です。
    const nn::util::Color4u8& GetColor() const NN_NOEXCEPT
    {
        return this->m_Color;
    }

    //! @brief     ログ表示のために用いるフレームレートを取得します。
    //! @return    フレームレートです。
    const float GetFrameRate() const NN_NOEXCEPT
    {
        return this->m_FrameRate;
    }

    //! @}
    //! @name 設定
    //! @{

    //! @brief        負荷メーターの表示名を設定します。
    //! @param[in]    name      表示名です。
    //! @details      空文字を渡した場合には 0 がセットされます。
    void SetName( const char* name ) NN_NOEXCEPT
    {
        if( name )
        {
            nn::util::Strlcpy(this->m_Name, name, 128);
        }
        else
        {
            memset(this->m_Name, 0, 128);
        }
    }

    //! @brief        表示色を設定します。
    //! @param[in]    color     表示色です。
    virtual void SetColor( const nn::util::Color4u8Type& color ) NN_NOEXCEPT
    {
        this->m_Color = color;
    }

    //! @brief        ログ表示のために用いるフレームレートを設定します。
    //! @param[in]    frameRate 設定するフレームレートです。
    void SetFrameRate( float frameRate ) NN_NOEXCEPT
    {
        this->m_FrameRate = frameRate;
    }

    //! @}

    //! @brief        LoadMeterBase を自身にアタッチします。
    //! @return       meter     アタッチする LoadMeterBase です。
    //! @details      アタッチすると、自身の Next() や Dump() を呼んだ時に、アタッチしたオブジェクトの Next() や Dump() も同時に呼ばれるようになります。
    void AttachLoadMeter( LoadMeterBase* meter ) NN_NOEXCEPT;

    //! @brief        アタッチされている LoadMeterBase を解除します。
    //! @return       meter     アタッチを解除する LoadMeterBase です。
    void DetachLoadMeter( LoadMeterBase* meter ) NN_NOEXCEPT;

    //! @brief    自身とアタッチされている LoadMeterBase の計測結果の保存先バッファを切り替えます。毎フレームの終了時に呼び出してください。
    //! @details  アタッチされている LoadMeterBase の Next() も内部で再帰的に呼ばれるため、親負荷メーターの Next() だけを呼んでください。
    //! @n        この関数が呼ばれない場合、正常な測定結果を得ることができません。
    virtual void Next() NN_NOEXCEPT;

    //! @brief    自身とアタッチされている LoadMeterBase の計測結果をログに出力します。
    //! @details  リリースビルド時もログが出力されます。
    void Dump() NN_NOEXCEPT;

protected:

    //! @brief        コンストラクタです。
    LoadMeterBase() NN_NOEXCEPT;

    //! @brief        デストラクタです。
    virtual ~LoadMeterBase() NN_NOEXCEPT
    {}

    //! @brief        計測結果を格納するメモリを初期化します。
    //! @param[in]    info 初期化用の情報です。
    //! @param[in]    pMemory 初期用のバッファへのポインタ
    //! @param[in]    memorySize バイトでのバッファサイズ
    void Initialize( const LoadMeterInfo& info, void* pMemory, size_t memorySize ) NN_NOEXCEPT;

    //! @brief        計測結果の格納メモリの終了処理を行います。
    void Finalize() NN_NOEXCEPT;

    //! @brief        初期化に必要なバッファのアラインメントを返します。
    //! @return       バッファのアラインメントを返します。
    static size_t GetBufferAlignment() NN_NOEXCEPT;

    //! @brief        初期化に必要なバッファのサイズを返します。
    //! @param[in]    info 初期化用の情報です。
    //! @return       バッファのサイズを返します。
    static size_t CalculateBufferSize(LoadMeterInfo info) NN_NOEXCEPT;

    //! @brief セクションを開始します。
    //! @param[in] name   セクション分類用の名前です。
    //! @param[in] tag    セクション分類用のタグです。
    //! @param[in] color  セクションの色です。
    //! @param[in] parent セクションが入れ子になっているかどうか。
    //! @param[in] depth  セクションの入れ子の深さです。
    void BeginSection( const char* name, uint32_t tag, const nn::util::Color4u8Type& color, int parent, int depth) NN_NOEXCEPT;

    //! @brief 現在のセクションを終了します。
    //! @param[in] sectionIndex 現在計測中のセクションを指定します。
    void EndSection( int sectionIndex ) NN_NOEXCEPT;

    LoadMeterInfo                          m_Info;                 //!< LoadMeterBase の初期化情報を保持します。
    char                                   m_Name[128];            //!< LoadMeterBase の名前です。
    float                                  m_FrameRate;            //!< 目標フレームレートです。
    nn::util::Color4u8                     m_Color;                //!< デフォルトの色です。

    nn::util::PlacementArray<nn::util::PlacementArray<Section>> m_SectionArray;//!< フレーム毎のセクションの計測結果を保持する配列です (m_SectionArray[m_Info.bufferCount][m_Info.sectionCountMax])。
    nn::util::PlacementArray<int>          m_SectionCount;         //!< フレーム毎の計測済みセクション数を保持する配列です (m_SectionCount[m_Info.bufferCount])。
    nn::util::PlacementArray<nn::os::Tick> m_FinalEnd;             //!< フレーム毎の最後の終了時刻を保持する配列です (m_FinalEnd[m_Info.bufferCount])。
    int                                    m_CurrentBufferIndex;   //!< 現在のフレームの計測結果を保存するバッファの指定です。
    int                                    m_ReferenceBufferIndex; //!< 結果を参照するバッファの指定です。(LoadMeterInfo.bufferCount - 1) フレーム前のバッファを指します。
    int                                    m_OverCount;            //!< 保持できるセクション数をオーバーしている個数です。

public:
    typedef nn::util::IntrusiveList<LoadMeterBase, nn::util::IntrusiveListBaseNodeTraits<LoadMeterBase>> LoadMeterList;
    LoadMeterList m_LoadMeterList; //!< LoadMeterBase クラスをノードとするリストです。
};

//! @brief        複数区間計測可能な CPU 負荷メータークラスです。
class CpuMeter : public LoadMeterBase
{
public:
    //! @name コンストラクタ/デストラクタ
    //! @{

    //! @brief        コンストラクタです。
    CpuMeter() NN_NOEXCEPT
      : LoadMeterBase(),
        m_CurrentSectionIndex( -1 ),
        m_Depth( 0 )
    {}

    //! @brief        デストラクタです。
    virtual ~CpuMeter() NN_NOEXCEPT NN_OVERRIDE
    {}

    //! @}
    //! @name 初期化/終了
    //! @{

    //! @brief        計測結果を格納するメモリを初期化します。
    //! @param[in]    info 初期化用の情報です。
    //! @param[in]    pMemory 初期用のバッファへのポインタ
    //! @param[in]    memorySize バイトでのバッファサイズ
    void Initialize( const LoadMeterInfo& info, void* pMemory, size_t memorySize) NN_NOEXCEPT
    {
        nn::util::BytePtr memory( pMemory );
        m_Info = info;
        m_Info.SetCoreCount(1);
        memory.AlignUp(LoadMeterBase::GetBufferAlignment());
        LoadMeterBase::Initialize( m_Info, pMemory, memorySize );
        memory.Advance(LoadMeterBase::CalculateBufferSize(m_Info));
    }

    //! @brief        計測結果の格納メモリの終了処理を行います。
    void Finalize() NN_NOEXCEPT
    {
        LoadMeterBase::Finalize();
    }

    //! @brief        初期化に必要なバッファのアラインメントを返します。
    //! @return       バッファのアラインメントを返します。
    static size_t GetBufferAlignment() NN_NOEXCEPT
    {
        return LoadMeterBase::GetBufferAlignment();
    }

    //! @brief        初期化に必要なバッファのサイズを返します。
    //! @param[in]    info 初期化用の情報です。
    //! @return       バッファのサイズを返します。
    static size_t CalculateBufferSize(LoadMeterInfo info) NN_NOEXCEPT
    {
        return LoadMeterBase::CalculateBufferSize(info);
    }

    //! @}
    //! @name 計測
    //! @{

    //! @brief        負荷計測を開始します。
    void BeginMeasure() NN_NOEXCEPT
    {
        BeginMeasure( NULL, 0, m_Color );
    }

    //! @brief        負荷計測を開始します。
    //! @param[in]    name    セクション分類用の名前です。
    void BeginMeasure( const char* name ) NN_NOEXCEPT
    {
        BeginMeasure( name, 0, m_Color );
    }

    //! @brief        負荷計測を開始します。
    //! @param[in]    tag    セクション分類用のタグです。
    void BeginMeasure( uint32_t tag ) NN_NOEXCEPT
    {
        BeginMeasure( NULL, tag, m_Color );
    }

    //! @brief        負荷計測を開始します。
    //! @param[in]    color     メーターの色です。
    void BeginMeasure( const nn::util::Color4u8Type& color ) NN_NOEXCEPT
    {
        BeginMeasure( NULL, 0, color );
    }

    //! @brief        負荷計測を開始します。
    //! @param[in]    name    セクション分類用の名前です。
    //! @param[in]    tag     セクション分類用のタグです。
    void BeginMeasure( const char* name, uint32_t tag ) NN_NOEXCEPT
    {
        BeginMeasure( name, tag, m_Color );
    }

    //! @brief        負荷計測を開始します。
    //! @param[in]    name    セクション分類用の名前です。
    //! @param[in]    color   メーターの色です。
    void BeginMeasure( const char* name, const nn::util::Color4u8Type& color ) NN_NOEXCEPT
    {
        BeginMeasure( name, 0, color );
    }

    //! @brief        負荷計測を開始します。
    //! @param[in]    tag    セクション分類用のタグです。
    //! @param[in]    color  メーターの色です。
    void BeginMeasure( uint32_t tag, const nn::util::Color4u8Type& color ) NN_NOEXCEPT
    {
        BeginMeasure( NULL, tag, color );
    }

    //! @brief        負荷計測を開始します。
    //! @param[in]    name    セクション分類用の名前です。
    //! @param[in]    tag     セクション分類用のタグです。
    //! @param[in]    color   メーターの色です。
    void BeginMeasure( const char* name, uint32_t tag, const nn::util::Color4u8Type& color ) NN_NOEXCEPT
    {
        LoadMeterBase::BeginSection( name, tag, color, m_CurrentSectionIndex, m_Depth );
        m_CurrentSectionIndex = m_SectionCount[ m_CurrentBufferIndex ];
        ++m_SectionCount[ m_CurrentBufferIndex ];
        m_Depth++;
    }

    //! @brief        負荷計測を終了します。
    void EndMeasure() NN_NOEXCEPT
    {
        LoadMeterBase::EndSection( m_CurrentSectionIndex );
        m_CurrentSectionIndex = m_SectionArray[m_CurrentBufferIndex][ m_CurrentSectionIndex ].parent;
        --m_Depth;
    }

    //! @}
    virtual void Next() NN_NOEXCEPT NN_OVERRIDE
    {
        m_CurrentSectionIndex = -1;
        m_Depth = 0;
        LoadMeterBase::Next();
    }

private:
    int    m_CurrentSectionIndex;  //!< 現在計測中のセクションの指定をします。
    int    m_Depth;                //!< 現在計測中のセクションの入れ子の深さを示します。
};

//! @brief       複数区間計測可能な GPU 負荷メータークラスです。
//! @details     GPU タイムスタンプを取得して計測を行いますが、最終的には CPU 時間に変換された Tick 値が Section 構造体に格納されます。
class GpuMeter : public LoadMeterBase
{
public:

    //! @name コンストラクタ/デストラクタ
    //! @{

    //! @brief        コンストラクタです。
    GpuMeter() NN_NOEXCEPT
    : LoadMeterBase()
    , m_Device( NULL )
    , m_pReferenceTimestamp(nullptr)
    {}

    //! @brief        デストラクタです。
    virtual ~GpuMeter() NN_NOEXCEPT NN_OVERRIDE
    {}

    //! @}
    //! @name 初期化/終了
    //! @{

    //! @brief        計測結果の格納メモリとタイムスタンプが書き込まれる GPU バッファを初期化します。
    //! @param[in]    pDevice デバイスへのポインタです。
    //! @param[in]    info 初期化用の情報です。
    //! @param[in]    pMemory 初期用のバッファへのポインタ
    //! @param[in]    memorySize バイトでのバッファサイズ
    //! @param[in]    pMemoryPool Gpuタイムスタンプが書き込まれるメモリプールへのポインタ
    //! @param[in]    memoryPoolOffset Gpuタイムスタンプが書き込まれるメモリプールへのオフセット
    //! @param[in]    memoryPoolSize Gpuタイムスタンプが書き込まれるメモリプールのサイズ
    //! @details      pDevice はクラス内部で記憶され Next() 内部のタイムスタンプの変換に使用されます。
    void Initialize( nn::gfx::Device* pDevice, const LoadMeterInfo& info, void* pMemory, size_t memorySize, nn::gfx::MemoryPool* pMemoryPool, ptrdiff_t memoryPoolOffset, size_t memoryPoolSize ) NN_NOEXCEPT;

    //! @brief        計測結果の格納メモリと GPU バッファの終了処理を行います。
    //! @param[in]    pDevice デバイスへのポインタです。
    void Finalize( nn::gfx::Device* pDevice ) NN_NOEXCEPT;

    //! @brief        初期化に必要なバッファのアラインメントを返します。
    //! @return       バッファのアラインメントを返します。
    static size_t GetBufferAlignment() NN_NOEXCEPT;

    //! @brief        初期化に必要なバッファのサイズを返します。
    //! @param[in]    info 初期化用の情報です。
    //! @return       バッファのサイズを返します。
    static size_t CalculateBufferSize( LoadMeterInfo info ) NN_NOEXCEPT;

    //! @brief        初期化でメモリプール上に確保するクエリバッファのアラインメントを返します。
    //! @param[in]    pDevice デバイスへのポインタです。
    //! @param[in]    info 初期化用の情報です。
    //! @return       メモリプールのアラインメントを返します。
    static size_t GetQueryBufferAlignment( nn::gfx::Device* pDevice, LoadMeterInfo info ) NN_NOEXCEPT;

    //! @brief        初期化でメモリプール上に確保するクエリバッファのサイズを返します。
    //! @param[in]    pDevice デバイスへのポインタです。
    //! @param[in]    info 初期化用の情報です。
    //! @return       メモリプールのサイズを返します。
    static size_t CalculateQueryBufferSize( nn::gfx::Device* pDevice, LoadMeterInfo info ) NN_NOEXCEPT;

    //! @}
    //! @name 計測
    //! @{

    //! @brief        負荷計測を開始します。
    //! @param[in]    pCommandBuffer タイムスタンプの要求コマンドを追加するコマンドバッファです。
    //! @details      計測終了の EndMeasure() が呼ばれるまではコマンドを発行する CPU コアを移動させないでください。
    void BeginMeasure( nn::gfx::CommandBuffer* pCommandBuffer ) NN_NOEXCEPT
    {
        BeginMeasure( pCommandBuffer, NULL, 0 );
    }

    //! @brief        負荷計測を開始します。
    //! @param[in]    pCommandBuffer タイムスタンプの要求コマンドを追加するコマンドバッファです。
    //! @param[in]    name    セクション分類用の名前です。
    //! @details      計測終了の EndMeasure() が呼ばれるまではコマンドを発行する CPU コアを移動させないでください。
    void BeginMeasure( nn::gfx::CommandBuffer* pCommandBuffer, const char* name ) NN_NOEXCEPT
    {
        BeginMeasure( pCommandBuffer, name, 0 );
    }

    //! @brief        負荷計測を開始します。
    //! @param[in]    pCommandBuffer タイムスタンプの要求コマンドを追加するコマンドバッファです。
    //! @param[in]    tag    セクション分類用のタグです。
    //! @details      計測終了の EndMeasure() が呼ばれるまではコマンドを発行する CPU コアを移動させないでください。
    void BeginMeasure( nn::gfx::CommandBuffer* pCommandBuffer, uint32_t tag ) NN_NOEXCEPT
    {
        BeginMeasure( pCommandBuffer, NULL, tag );
    }

    //! @brief        負荷計測を開始します。
    //! @param[in]    pCommandBuffer タイムスタンプの要求コマンドを追加するコマンドバッファです
    //! @param[in]    name    セクション分類用の名前です。
    //! @param[in]    tag    セクション分類用のタグです。
    //! @details      計測終了の EndMeasure() が呼ばれるまではコマンドを発行する CPU コアを移動させないでください。
    void BeginMeasure( nn::gfx::CommandBuffer* pCommandBuffer, const char* name, uint32_t tag ) NN_NOEXCEPT;

    //! @brief        負荷計測を開始します。
    //! @param[in]    pCommandBuffer タイムスタンプの要求コマンドを追加するコマンドバッファです。
    //! @param[in]    color     メーターの色を変更します。
    //! @details      計測終了の EndMeasure() が呼ばれるまではコマンドを発行する CPU コアを移動させないでください。
    void BeginMeasure( nn::gfx::CommandBuffer* pCommandBuffer, const nn::util::Color4u8Type& color ) NN_NOEXCEPT
    {
        BeginMeasure( pCommandBuffer, NULL, 0, color );
    }

    //! @brief        負荷計測を開始します。
    //! @param[in]    pCommandBuffer タイムスタンプの要求コマンドを追加するコマンドバッファです。
    //! @param[in]    name    セクション分類用の名前です。
    //! @param[in]    color     メーターの色を変更します。
    //! @details      計測終了の EndMeasure() が呼ばれるまではコマンドを発行する CPU コアを移動させないでください。
    void BeginMeasure( nn::gfx::CommandBuffer* pCommandBuffer, const char* name, const nn::util::Color4u8Type& color ) NN_NOEXCEPT
    {
        BeginMeasure( pCommandBuffer, name, 0, color );
    }

    //! @brief        負荷計測を開始します。
    //! @param[in]    pCommandBuffer タイムスタンプの要求コマンドを追加するコマンドバッファです。
    //! @param[in]    tag    セクション分類用のタグです。
    //! @param[in]    color     メーターの色を変更します。
    //! @details      計測終了の EndMeasure() が呼ばれるまではコマンドを発行する CPU コアを移動させないでください。
    void BeginMeasure( nn::gfx::CommandBuffer* pCommandBuffer, uint32_t tag, const nn::util::Color4u8Type& color ) NN_NOEXCEPT
    {
        BeginMeasure( pCommandBuffer, NULL, tag, color );
    }

    //! @brief        負荷計測を開始します。
    //! @param[in]    pCommandBuffer タイムスタンプの要求コマンドを追加するコマンドバッファです。
    //! @param[in]    name    セクション分類用の名前です。
    //! @param[in]    tag    セクション分類用のタグです。
    //! @param[in]    color     メーターの色を変更します。
    //! @details      計測終了の EndMeasure() が呼ばれるまではコマンドを発行する CPU コアを移動させないでください。
    void BeginMeasure( nn::gfx::CommandBuffer* pCommandBuffer, const char* name, uint32_t tag, const nn::util::Color4u8Type& color ) NN_NOEXCEPT;

    //! @brief        負荷計測を終了します。
    //! @param[in]    pCommandBuffer タイムスタンプの要求コマンドを追加するコマンドバッファです。
    void EndMeasure( nn::gfx::CommandBuffer* pCommandBuffer ) NN_NOEXCEPT;

    //! @}
    //! @brief    自身とアタッチされている負荷メーターで GPU に記録されたタイムスタンプの値を読み出し、計測結果の格納先バッファを切り替えます。
    //! @details  毎フレームの終了時に呼び出してください。この関数が呼ばれない場合、正常な測結果を得ることができません。
    //! @n        「バッファ数 - 2」フレーム前にコマンドに積んだ計測結果を集計します。
    //! @n        アタッチされている子負荷メータの Next() も内部で再帰的に呼ばれるため、親負荷メータの Next() だけを呼んでください。
    virtual void Next() NN_NOEXCEPT NN_OVERRIDE;

    //! @brief        コアに対応した表示色を取得します。
    //! @return       表示色です。
    const nn::util::Color4u8& GetColor() NN_NOEXCEPT
    {
        int coreNumber = m_GetCoreNumberFunction();
        NN_SDK_ASSERT( coreNumber < m_Info.GetCoreCount() );
        return this->m_CoreColor[coreNumber];
    }

    //! @brief        コアに対応した表示色を設定します。
    //! @param[in]    color     表示色です。
    virtual void SetColor( const nn::util::Color4u8Type& color ) NN_NOEXCEPT NN_OVERRIDE
    {
        int coreNumber = m_GetCoreNumberFunction();
        NN_SDK_ASSERT( coreNumber < m_Info.GetCoreCount() );
        this->m_CoreColor[coreNumber] = color;
    }

    //! @brief  現在のスレッドが動作するコア ID を返す関数を登録します。
    //! @param[in]  getCoreNumberFunction コア番号を返す関数です。
    //! @details    設定しない場合は nn::os::GetCurrentCoreNumber() が設定されます。
    void SetGetCoreNumberFunction( nn::perf::GetCoreNumberFunction getCoreNumberFunction ) NN_NOEXCEPT
    {
        m_GetCoreNumberFunction = getCoreNumberFunction;
    }

private:
    //! @brief        ReferenceTimestamp には初期化に必要なバッファのアラインメントを返します。
    //! @return       バッファのアラインメントを返します。
    static size_t GetReferenceTimestampBufferAlignment() NN_NOEXCEPT;

    //! @brief        ReferenceTimestamp には初期化に必要なバッファのサイズを返します。
    //! @return       バッファのサイズを返します。
    static size_t CalculateReferenceTimestampBufferSize() NN_NOEXCEPT;

    //! @brief        GPU の計測区間を処理開始時間順に並び替えるための比較関数です。
    //! @param[in]    lhs      比較する計測区間
    //! @param[in]    rhs      比較する計測区間
    //! @return       lhs の計測開始時刻が rhs よりも早いなら true を返します。
    static bool CompareSectionBeginTick( const Section& lhs, const Section& rhs ) NN_NOEXCEPT
    {
        return (lhs.begin.GetInt64Value() < rhs.begin.GetInt64Value());
    }

    //! @brief        セクションのデーターの書き込み
    //! @details      GPU からもらったタイムスタンプを nn::os::Tick へ変換を行います。
    //! @param[in]    pSectionArray                 出力配列
    //! @param[in]    sectionArrayCount             出力配列の項目数
    //! @param[in]    pTimestampBufferArray         GPU からもらったタイムスタンプの配列
    //! @param[in]    timestampBufferArrayCount     GPU タイムスタンプの配列の項目数
    //! @return       最後のタイムスタンプ
     nn::os::Tick FillSectionTimestamp(
        LoadMeterBase::Section* pSectionArray, int sectionArrayCount,
        const nn::gfx::TimestampBuffer* pTimestampBufferArray, int timestampBufferArrayCount);


     //! @brief        ReferenceTimestamp の初期化
     //! @param[in]    pDevice デバイスへのポインタです。
     //! @param[in]    pMemory 初期化のためのメモリポインター
    void InitializeReferenceTimestamp(nn::gfx::Device* pDevice, nn::util::BytePtr* pMemory);

    //! @brief        ReferenceTimestamp の完了処理
    //! @param[in]    pDevice デバイスへのポインタです。
    void FinalizeReferenceTimestamp(nn::gfx::Device* pDevice);


    nn::util::PlacementArray<nn::gfx::Buffer>               m_QueryBuffer;              //!< 計測結果が書き込まれる Gpu バッファ配列へのポインタです (*m_GueryBuffer[m_Info.bufferCount]) 。
    nn::util::PlacementArray<nn::gfx::TimestampBuffer>      m_TimestampBuffer;          //!< GPU バッファからタイムスタンプの値を受け取るバッファの配列です。
    nn::os::MutexType                                       m_Mutex;                    //!< マルチスレッドで同時にアクセスされた場合のミューテックスです。
    nn::gfx::Device*                                        m_Device;                   //!< デバイスへのポインタです。タイムスタンプの変換に使用します。

    nn::util::PlacementArray<nn::util::PlacementArray<int>> m_SectionCountCore;         //!< コア別にフレーム毎の計測済みセクション数を保持する配列です
    nn::util::PlacementArray<int>                           m_CurrentSectionIndexCore;  //!< コア別に現在計測中のセクションの指定をします。
    nn::util::PlacementArray<nn::util::Color4u8>            m_CoreColor;                //!< コア毎のデフォルトの色です。
    nn::util::PlacementArray<int>                           m_CoreDepth;                //!< コア別の現在計測中のセクションの入れ子の深さを示します。
    nn::perf::GetCoreNumberFunction                         m_GetCoreNumberFunction;    //!< 動作中のスレッドのコア番号を取得する関数です
    detail:: ReferenceTimestamp*                            m_pReferenceTimestamp;      //!< 基準のタイムスタンプの取得
};

} // namespace perf
} // namespace nns
