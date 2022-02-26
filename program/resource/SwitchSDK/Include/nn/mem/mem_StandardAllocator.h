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
 * @brief   StandardAllocator に関する公開ヘッダファイル
 * @details StandardAllocator クラスの定義
 */

#pragma once

#include <nn/nn_Common.h>
#include <nn/os.h>
#include <nn/mem/detail/mem_InternalClass.h>

namespace nn { namespace mem {

//--------------------------------------------------------------------------
/**
 * @brief  メモリ管理のためのクラスです
 *
 * @details
 *  nn::mem::StandardAllocator はメモリの確保・解放などを行うためのクラスです。
 */
class StandardAllocator
{
    NN_DISALLOW_COPY( StandardAllocator );
    NN_DISALLOW_MOVE( StandardAllocator );

public:
    /**
     *  @brief 確保されたメモリ領域を巡るときに呼び出されるコールバック関数の型です。
     *
     *  @param[in]  addr    確保されているメモリの先頭アドレス
     *  @param[in]  size    確保されているメモリのサイズ
     *  @param[in]  userPtr ユーザから渡されるポインタ
     *
     *  @retval 0   WalkAllocatedBlocks() の実行を終了します。
     *  @retval 1   WalkAllocatedBlocks() の実行を継続します。
     *
     *  @details
     *   WalkAllocatedBlocks() で利用されるコールバック関数の型です。 @n
     *   コールバック内でメモリの確保や解放を行う場合は、0 を返すようにしてください。
     */
    typedef int (*WalkCallback) ( void *addr, size_t size, void *userPtr );

    /**
     *  @brief アロケータの内部状態を取得するための型です。
     */
    struct AllocatorHash
    {
        size_t allocCount;  //!< 確保されたメモリ領域の数です。
        size_t allocSize;   //!< 確保されたメモリ領域の合計サイズです。
        size_t hash;        //!< アロケータの状態をハッシュ化した値です。
    };

    //! @name コンストラクタ・デストラクタ
    //! @{

    /**
     * @brief  コンストラクタです。
     *
     * @details
     *  このコンストラクタではメモリ領域の初期化を行いません。
     *  後ほど Initialize() を呼んで StandardAllocator で管理するメモリ領域を初期化してください。@n
     *  @n
     *  本 API はスレッドセーフではありません。
     *
     */
    StandardAllocator() NN_NOEXCEPT;

    /**
     * @brief  コンストラクタです。
     *
     * @param[in]  addr      アロケータが使用する領域の先頭アドレス
     * @param[in]  size      アロケータが使用する領域のサイズ
     *
     * @pre  メモリ領域がアロケータの管理領域よりも大きい必要があります。
     * @post アロケータは初期化済みとなります。
     *
     * @details
     *  指定されたメモリ領域を StandardAllocator で管理するために初期化します。@n
     *  @n
     *  本アロケータでは、渡されたメモリ領域を 4KB にアラインした上で使用します。@n
     *  そのため、addr, size は 4KB 単位である方が効率的です。@n
     *  また、アロケータの管理領域に最低 16KB 必要となりますので、 4KB アラインを取った上で 16KB 以上の領域を指定する必要があります。@n
     *  @n
     *  addr に nullptr を渡した場合、size で指定したサイズの仮想アドレス空間を予約し、
     *  予約された仮想アドレス空間内で適宜必要な物理メモリを割り当てて動作します。
     *  物理メモリが必要になった時点で割り当てを行い、不要になった物理メモリは解放します。
     *  この機能は nn::os::IsVirtualAddressMemoryEnabled() が true を返す場合にのみ使用可能です。
     *  詳細は nn::os::IsVirtualAddressMemoryEnabled() を参照してください。@n
     *  @n
     *  本 API はスレッドセーフではありません。@n
     *  本 API の動作中にアロケータに対する操作は行わないでください。
     *
     */
    StandardAllocator(void* addr, size_t size) NN_NOEXCEPT;

    /**
     * @brief  コンストラクタです。スレッド毎のキャッシュ機能の有効・無効が選択できます。
     *
     * @param[in]  addr             アロケータが使用する領域の先頭アドレス
     * @param[in]  size             アロケータが使用する領域のサイズ
     * @param[in]  isCacheEnable    スレッド毎のメモリキャッシュ機能を有効にするか
     *
     * @pre  メモリ領域がアロケータの管理領域よりも大きい必要があります。
     * @post アロケータは初期化済みとなります。
     *
     * @details
     *  指定されたメモリ領域を StandardAllocator で管理するために初期化します。@n
     *  @n
     *  本アロケータでは、渡されたメモリ領域を 4KB にアラインした上で使用します。@n
     *  そのため、addr, size は 4KB 単位である方が効率的です。@n
     *  また、アロケータの管理領域に最低 16KB 必要となりますので、 4KB アラインを取った上で 16KB 以上の領域を指定する必要があります。@n
     *  @n
     *  isCacheEnable に true を渡すことでマルチスレッド時の確保・解放速度を向上させる機能を有効にします。@n
     *  この機能では TLS スロットを 1 つ消費します。@n
     *  isCacheEnable を true にした場合の確保・解放のより詳細な仕様については NintendoSDK ドキュメントの @confluencelink{89102552, StandardAllocator} の説明を参照してください。@n
     *  @n
     *  addr に nullptr を渡した場合、size で指定したサイズの仮想アドレス空間を予約し、
     *  予約された仮想アドレス空間内で適宜必要な物理メモリを割り当てて動作します。
     *  物理メモリが必要になった時点で割り当てを行い、不要になった物理メモリは解放します。
     *  この機能は nn::os::IsVirtualAddressMemoryEnabled() が true を返す場合にのみ使用可能です。
     *  詳細は nn::os::IsVirtualAddressMemoryEnabled() を参照してください。@n
     *  @n
     *  本 API はスレッドセーフではありません。@n
     *  本 API の動作中にアロケータに対する操作は行わないでください。
     *
     */
    StandardAllocator(void* addr, size_t size, bool isCacheEnable) NN_NOEXCEPT;

    /**
     * @brief  デストラクタです。
     *
     * @details
     *  アロケータが初期化済みであれば Finalize() を呼び、アロケータを破棄します。@n
     *  @n
     *  本 API はスレッドセーフではありません。
     *
     */
    ~StandardAllocator() NN_NOEXCEPT
    {
        if (m_Initialized)
        {
            Finalize();
        }
    }

    //! @}

    //! @name 初期化・終了処理
    //! @{

    /**
     * @brief  指定されたメモリ領域を StandardAllocator で管理するために初期化します。
     *
     * @param[in]  addr      アロケータが使用する領域の先頭アドレス
     * @param[in]  size      アロケータが使用する領域のサイズ
     *
     * @pre  アロケータが未初期化である必要があります。
     * @pre  メモリ領域がアロケータの管理領域よりも大きい必要があります。
     * @post アロケータは初期化済みとなります。
     *
     * @details
     *  指定されたメモリ領域を StandardAllocator で管理するために初期化します。@n
     *  @n
     *  本アロケータでは、渡されたメモリ領域を 4KB にアラインした上で使用します。@n
     *  そのため、addr, size は 4KB 単位である方が効率的です。@n
     *  また、アロケータの管理領域に最低 16KB 必要となりますので、 4KB アラインを取った上で 16KB 以上の領域を指定する必要があります。@n
     *  @n
     *  addr に nullptr を渡した場合、size で指定したサイズの仮想アドレス空間を予約し、
     *  予約された仮想アドレス空間内で適宜必要な物理メモリを割り当てて動作します。
     *  物理メモリが必要になった時点で割り当てを行い、不要になった物理メモリは解放します。
     *  この機能は nn::os::IsVirtualAddressMemoryEnabled() が true を返す場合にのみ使用可能です。
     *  詳細は nn::os::IsVirtualAddressMemoryEnabled() を参照してください。@n
     *  @n
     *  本 API はスレッドセーフではありません。@n
     *  本 API の動作中にアロケータに対する操作は行わないでください。
     *
     */
    void Initialize( void* addr, size_t size ) NN_NOEXCEPT;

    /**
     * @brief  指定されたメモリ領域を StandardAllocator で管理するために初期化します。スレッド毎のキャッシュ機能の有効・無効が選択できます。
     *
     * @param[in]  addr             アロケータが使用する領域の先頭アドレス
     * @param[in]  size             アロケータが使用する領域のサイズ
     * @param[in]  isCacheEnable    スレッド毎のメモリキャッシュ機能を有効にするか
     *
     * @pre  アロケータが未初期化である必要があります。
     * @pre  メモリ領域がアロケータの管理領域よりも大きい必要があります。
     * @post アロケータは初期化済みとなります。
     *
     * @details
     *  指定されたメモリ領域を StandardAllocator で管理するために初期化します。@n
     *  @n
     *  本アロケータでは、渡されたメモリ領域を 4KB にアラインした上で使用します。@n
     *  そのため、addr, size は 4KB 単位である方が効率的です。@n
     *  また、アロケータの管理領域に最低 16KB 必要となりますので、 4KB アラインを取った上で 16KB 以上の領域を指定する必要があります。@n
     *  @n
     *  isCacheEnable に true を渡すことでマルチスレッド時の確保・解放速度を向上させる機能を有効にします。@n
     *  この機能では TLS スロットを 1 つ消費します。@n
     *  isCacheEnable を true にした場合の確保・解放のより詳細な仕様については NintendoSDK ドキュメントの @confluencelink{89102552, StandardAllocator} の説明を参照してください。@n
     *  @n
     *  addr に nullptr を渡した場合、size で指定したサイズの仮想アドレス空間を予約し、
     *  予約された仮想アドレス空間内で適宜必要な物理メモリを割り当てて動作します。
     *  物理メモリが必要になった時点で割り当てを行い、不要になった物理メモリは解放します。
     *  この機能は nn::os::IsVirtualAddressMemoryEnabled() が true を返す場合にのみ使用可能です。
     *  詳細は nn::os::IsVirtualAddressMemoryEnabled() を参照してください。@n
     *  @n
     *  本 API はスレッドセーフではありません。@n
     *  本 API の動作中にアロケータに対する操作は行わないでください。
     *
     */
    void Initialize(void* addr, size_t size, bool isCacheEnable) NN_NOEXCEPT;

    /**
     * @brief  アロケータを破棄します。
     *
     * @pre  アロケータが初期化されている必要があります。
     * @post アロケータは未初期化となります。
     *
     * @details
     *  アロケータを破棄します。@n
     *  本 API はスレッドセーフではありません。@n
     *  本 API の動作中にアロケータに対する操作は行わないでください。
     *
     */
    void Finalize() NN_NOEXCEPT;

    //! @}

    //! @name メモリ領域操作 API
    //! @{

    /**
     * @brief  メモリ領域を確保します。
     *
     * @param[in]  size               確保するメモリのサイズ（バイト）
     *
     * @return 確保したメモリの先頭へのポインタが返ります。失敗すると NULL が返ります。
     *
     * @pre アロケータが初期化されている必要があります。
     * @post アロケータから確保されたメモリ領域の先頭アドレスが返ります。
     *
     * @details
     *  アロケータから、指定されたサイズのメモリを確保します。@n
     *  確保されたメモリ領域の初期値は不定です。@n
     *  確保されるメモリ領域は、先頭アドレスが NN_ALIGNOF(std::max_align_t) バイトアライメントされます。@n
     *  サイズが 0 の場合、メモリは確保されず、NULL が返ります。@n
     *  割り当てられたメモリブロックのサイズは要求サイズより大きい場合があります。sizeof(std::max_align_t)の倍数にパディングされます。@n
     *  @n
     *  本 API はスレッドセーフです。
     *
     */
    void* Allocate( size_t size ) NN_NOEXCEPT;

    /**
     * @brief  アライメントを指定してメモリ領域を確保します。
     *
     * @param[in]  size               確保するメモリのサイズ（バイト）
     * @param[in]  alignment          確保するメモリのアライメント
     *
     * @return 確保したメモリの先頭へのポインタが返ります。失敗すると NULL が返ります。
     *
     * @pre  アロケータが初期化されている必要があります。
     * @pre  size > 0 である必要があります。
     * @pre  アライメントには正の 2 のべき乗を指定する必要があります。
     * @post アロケータから確保されたメモリ領域の先頭アドレスが返ります。
     *
     * @details
     *  アロケータから、指定されたサイズ、及びアライメントでメモリを確保します。@n
     *  確保されたメモリ領域の初期値は不定です。@n
     *  アライメントには正の 2 のべき乗を指定してください。@n
     *  サイズが 0 の場合、メモリは確保されず、NULL が返ります。@n
     *  割り当てられたメモリブロックのサイズは要求サイズより大きい場合があります。sizeof(std::max_align_t)の倍数にパディングされます。@n
     *  @n
     *  本 API はスレッドセーフです。
     *
     */
    void* Allocate( size_t size, size_t alignment ) NN_NOEXCEPT;

    /**
     * @brief  メモリ領域を解放します。
     *
     * @param[in]  addr               開放するメモリ領域の先頭アドレス
     *
     * @pre アロケータが初期化されている必要があります。
     * @pre addr はアロケータから確保されたメモリ領域の先頭アドレスである必要があります。
     *
     * @details
     *  メモリ領域を解放します。@n
     *  @n
     *  本 API はスレッドセーフです。
     *
     */
    void Free( void* addr ) NN_NOEXCEPT;

    /**
     * @brief  メモリ領域を指定したサイズで再確保します。
     *
     * @param[in]  addr     確保済みメモリ領域の先頭アドレス@n NULL を指定した場合 Allocate() と同様の動作になります。
     * @param[in]  newSize  新しく割り当てるサイズ@n 0 を指定した場合 Free() と同様の動作になります。
     *
     * @return 再確保したメモリ領域の先頭アドレスが返ります。失敗した場合 NULL が返ります。
     *
     * @pre アロケータが初期化されている必要があります。
     * @pre addr は StandardAllocator から Allocate() によって確保されたアドレスでなくてはなりません。
     *
     * @post アロケータから再確保されたメモリ領域の先頭アドレスが返ります。
     *
     * @details
     *  確保済みのメモリ領域を、指定したサイズに変更して割り当てなおします。@n
     *  新しいサイズが前よりも大きい場合、古いメモリ領域のデータは全てコピーされます。@n
     *  増えた分のメモリ領域の初期値は不定です。@n
     *  新しいサイズが前よりも小さい場合、古いメモリ領域のデータを newSize 分だけコピーします。@n
     *  サイズが大きくなる場合でも小さくなる場合でも、メモリ領域のアドレスは変更される可能性があります。@n
     *  新たに割り当てられたメモリ領域は、 addr のアライメントを保持しません。先頭アドレスが NN_ALIGNOF(std::max_align_t) バイトアライメントされます。@n
     *  @n
     *  本 API はスレッドセーフです。
     *
     */
    void* Reallocate( void* addr, size_t newSize ) NN_NOEXCEPT;

    /**
     * @brief   特定スレッド用にキャッシュしている空き領域を全てのスレッドが利用可能になるよう解放します。
     *
     * @pre アロケータが初期化されている必要があります。
     *
     * @details
     *  スレッド毎のメモリキャッシュ機能でキャッシュしている空き領域を解放し、全てのスレッドから確保可能な状態にします。@n
     *  本 API はコンストラクタ呼び出し時、もしくは Initialize() 時に isCacheEnable に true を与えた場合にのみ有効です。@n
     *  それ以外の場合は何も処理せず return します。@n
     *  解放されるキャッシュは本 API を呼び出したスレッドのキャッシュのみで、他のスレッドがキャッシュを持っている場合、そちらは解放されません。@n
     *  全てのスレッドが持つキャッシュを解放したい場合、全てのスレッドで本 API を呼び出す必要があります。@n
     *  @n
     *  本 API はスレッドセーフです。
     */
    void ClearThreadCache() const NN_NOEXCEPT;

    /**
     * @brief   アロケータの管理領域を整理します。
     *
     * @pre アロケータが初期化されている必要があります。
     *
     * @details
     *  アロケータの管理領域のサイズは動的に変化します。@n
     *  管理領域は確保時に必要に応じて自動で整理されますが、本 API を呼び出すとそれを明示的に実行でき、メモリの断片化を抑制できる可能性があります。@n
     *  また本 API を呼び出すことで、その後 GetAllocatableSize() と GetTotalFreeSize() で取得できる値が増加することがあります。@n
     *  @n
     *  本 API はスレッドセーフです。
     */
    void CleanUpManagementArea() const NN_NOEXCEPT;

    //! @}

    //! @name デバッグ用 API
    //! @{

    /**
     * @brief  指定した確保済みメモリ領域のサイズを取得します。
     *
     * @param[in]  addr  サイズを取得するメモリ領域の先頭アドレス
     *
     * @pre アロケータが初期化されている必要があります。
     * @pre addr は StandardAllocator から Allocate() によって確保されたアドレスでなくてはなりません。
     *
     * @details
     *  指定した確保済みメモリ領域のサイズを取得します。@n
     *  本 API で取得できるサイズは実際に確保されたメモリ領域のサイズで、 Allocate() 時に指定したサイズではありません。@n
     *  @n
     *  本 API はスレッドセーフです。
     *
     */
    size_t GetSizeOf( const void* addr ) const NN_NOEXCEPT;

    /**
     * @brief  アロケータに存在する空き領域の合計を取得します。
     *
     * @return 空き領域の合計サイズが返ります。
     *
     * @pre アロケータが初期化されている必要があります。
     *
     * @details
     *  アロケータで管理している空き領域の合計を取得します。@n
     *  空き領域が複数存在する場合、実際に確保可能なサイズは合計サイズよりも小さくなります。@n
     *  実際に確保可能なサイズを取得するには GetAllocatableSize() を使用してください。@n
     *  @n
     *  本 API はアロケータ内の確保・解放状況により時間がかかる場合があります。@n
     *  @n
     *  仮想アドレスメモリ管理機能が有効で StandardAllocator で指定した仮想アドレス空間が物理アドレス空間よりも大きい場合、物理メモリが割り当て可能な空き領域のサイズを返します。@n
     *  @n
     *  本 API は StandardAllocator の仮想アドレスメモリ管理機能が無効である場合はスレッドセーフで、有効である場合はスレッドセーフではありません。
     *
     */
    size_t GetTotalFreeSize() const NN_NOEXCEPT;

    /**
     * @brief  アロケータから確保可能な最大サイズを取得します。
     *
     * @return 確保可能なサイズが返ります。
     *
     * @pre アロケータが初期化されている必要があります。
     *
     * @details
     *  アロケータから確保可能な最大サイズを取得します。@n
     *  空き領域が複数存在する場合、その中で最大の領域のサイズを取得します。@n
     *  @n
     *  本 API で返却されたサイズは必ず確保可能であることを保証しますが、アルゴリズム上、本 API で返却されたサイズ以上の領域が確保可能な場合があります。@n
     *  @n
     *  本 API はアロケータ内の確保・解放状況により時間がかかる場合があります。@n
     *  @n
     *  仮想アドレスメモリ管理機能が有効で StandardAllocator で指定した仮想アドレス空間が物理アドレス空間よりも大きい場合、物理メモリが割り当て可能な空き領域のサイズを返します。@n
     *  @n
     *  本 API は StandardAllocator の仮想アドレスメモリ管理機能が無効である場合はスレッドセーフで、有効である場合はスレッドセーフではありません。
     *
     */
    size_t GetAllocatableSize() const NN_NOEXCEPT;

    /**
     * @brief  確保されたメモリ領域に対して順にコールバック関数を呼び出します。
     *
     * @param[in]  callback  メモリ領域ごとに呼び出されるコールバック関数
     * @param[in]  userPtr   コールバック関数に渡されるポインタ
     *
     * @pre アロケータが初期化されている必要があります。
     * @pre Initialize() 時に isCacheEnable に true を指定しており、かつコールバック関数内で確保されたメモリ領域に書き込みを行う場合、全てのスレッドで ClearThreadCache() を呼び出した直後に本 API が呼ばれる必要があります。
     *
     * @details
     *  確保済みのメモリ領域全てに対し、順にコールバック関数を呼び出します。@n
     *  @n
     *  Initialize() 時に isCacheEnable に true を指定し、スレッド毎のメモリキャッシュ機能を有効にしている場合、本 API ではそのキャッシュ領域に対してもコールバック関数を呼び出します。@n
     *  そのため、本 API を用いて確保されたメモリ領域に書き込みを行う場合は必ず全てのスレッドで ClearThreadCache() を呼び出した直後に本 API を呼び出してください。@n
     *  @n
     *  本 API はスレッドセーフです。
     *
     */
    void WalkAllocatedBlocks( WalkCallback callback, void* userPtr ) const NN_NOEXCEPT;

    /**
     * @brief  アロケータ内部の情報を表示します。
     *
     * @details
     *  アロケータの内部状態を出力します。@n
     *  @n
     *  出力されるフォーマットの詳細については NintendoSDK ドキュメントの @confluencelink{89102552, StandardAllocator} の説明を参照してください。
     */
    void Dump() const NN_NOEXCEPT;

    /**
     * @brief  アロケータの内部の情報を基にハッシュを生成します。
     *
     * @return アロケータの内部状態が返ります。
     *
     * @details
     *  アロケータの内部状態を取得します。
     */
    AllocatorHash Hash() const NN_NOEXCEPT;

    //! @}

private:
    bool                                m_Initialized;          //!< 初期化済みフラグ
    bool                                m_EnableThreadCache;    //!< TLS を利用したスレッド毎の確保・解放高速化フラグ
    uintptr_t                           m_CentralAllocatorAddr; //!< アロケータとして使用するメモリ領域の先頭アドレス
    nn::os::TlsSlot                     m_TlsSlot;              //!< TLS スロット
    detail::InternalCentralHeapStorage  m_CentralHeap;          //!< 内部実装クラス
};

}}  // namespace nn::mem
