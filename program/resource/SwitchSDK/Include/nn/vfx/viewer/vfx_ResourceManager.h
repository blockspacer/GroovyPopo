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

#include <nn/vfx/vfx_Resource.h>
#include <nn/vfx/vfx_Heap.h>
#include <nn/vfx/vfx_System.h>
#include <nn/vfx/viewer/vfx_Guid.h>
#include <nn/vfx/viewer/vfx_FileSystem.h>

namespace nn {
namespace vfx {
namespace viewer {
namespace detail {

//---------------------------------------------------------------------------
//! @brief        リソースマネージャクラス
//---------------------------------------------------------------------------
class ResourceManager
{
    NN_DISALLOW_COPY( ResourceManager );
public:

    //----------------------------------------------------------------------
    //! @brief  エフェクトリソース管理構造体。
    //----------------------------------------------------------------------
    class EffectResource
    {
        NN_DISALLOW_COPY( EffectResource );
    public:
        void*                         binary;       //!< TBD
        nn::vfx::viewer::detail::Guid guId;         //!< TBD
        int                           resourceId;   //!< TBD
        int                           releaseCount; //!< TBD
        size_t                        size;         //!< TBD

        //----------------------------------------------------------------------
        //! @brief  TBD
        //----------------------------------------------------------------------
        void Initialize() NN_NOEXCEPT
        {
            binary = NULL;
            guId.Clear();
            releaseCount = -1;
            resourceId = -1;
            size = 0;
        }
    };

    //---------------------------------------------------------------------------
    //! @brief  コンストラクタです。
    //---------------------------------------------------------------------------
    ResourceManager() NN_NOEXCEPT
    {
        m_ResourceCount = 0;
        m_ResourceStartId = 8;
        m_CacheResourceNo = -1;
        m_EffectResourceArray = NULL;
    }

    //---------------------------------------------------------------------------
    //! @brief  リソースマネージャの初期化を行います。
    //! @param[in]      pHeap     パケットデータです.
    //! @param[in]      controlResourceCount     パケットデータです.
    //! @param[in]      userResourceCount     パケットデータです.
    //---------------------------------------------------------------------------
    void Initialize( nn::vfx::Heap* pHeap, int controlResourceCount, int userResourceCount ) NN_NOEXCEPT
    {
        m_pHeap = pHeap;
        m_ResourceCount = controlResourceCount;
        m_ResourceStartId = userResourceCount;

        // 管理用ワークを確保し、初期化
        m_EffectResourceArray = static_cast< EffectResource* >( m_pHeap->Alloc( sizeof( EffectResource ) * m_ResourceCount ) );
        for( int i = 0; i < m_ResourceCount; ++i )
        {
            m_EffectResourceArray[ i ].Initialize();
        }
    }

    //---------------------------------------------------------------------------
    //! @brief  リソースマネージャの終了処理を行います。
    //---------------------------------------------------------------------------
    void Finalize() NN_NOEXCEPT
    {
        for( int i = 0; i < m_ResourceCount; ++i )
        {
            if( m_EffectResourceArray[ i ].binary )
            {
                m_pHeap->Free( m_EffectResourceArray[ i ].binary );
            }
        }

        if ( m_EffectResourceArray != NULL )
        {
            m_pHeap->Free( m_EffectResourceArray );
        }
    }


    //---------------------------------------------------------------------------
    //! @brief  保持するリソースの破棄登録を行います。
    //---------------------------------------------------------------------------
    void EntryResDestroy() NN_NOEXCEPT
    {
        for( int i = 0; i < m_ResourceCount; ++i )
        {
            if( m_EffectResourceArray[ i ].binary )
            {
                Release( m_EffectResourceArray[ i ].guId );
            }
        }
    }

    //---------------------------------------------------------------------------
    //! @brief  リソースを登録します。
    //! @param[in]      effectBinaryPath     パケットデータです.
    //! @param[in]      guid     パケットデータです.
    //! @return TBD
    //---------------------------------------------------------------------------
    bool Entry( const char* effectBinaryPath, nn::vfx::viewer::detail::Guid guid ) NN_NOEXCEPT
    {
        // 登録可能かどうか。
        int slot = GetEmptySlot();
        if( slot == -1 )
        {
            return false;
        }

        EffectResource* res = &m_EffectResourceArray[ slot ];
        res->Initialize();

        // 指定パスのエフェクトバイナリをロードする
        nn::vfx::viewer::detail::FileSystem::Load( &res->binary, &res->size, effectBinaryPath, m_pHeap );
        if ( res->binary == nullptr )
        {
            nn::vfx::detail::OutputWarning( "Viewer BinaryData(%s) Load Failed. \n", effectBinaryPath );
            return false;
        }

        // 実際のリソースIDにオフセット
        res->resourceId = slot + m_ResourceStartId;
        res->guId = guid;

        return true;
    }

    //---------------------------------------------------------------------------
    //! @brief  リリースフラグの立っているリソースを解放する。
    //! @return TBD
    //---------------------------------------------------------------------------
    int Flush( nn::vfx::System* vfxSystem, nn::vfx::Heap* pHeap ) NN_NOEXCEPT
    {
        int resId = -1;

        for( int i = 0; i < m_ResourceCount; ++i )
        {
            if( m_EffectResourceArray[ i ].releaseCount > 0 )
            {
                m_EffectResourceArray[ i ].releaseCount--;

                if( m_EffectResourceArray[ i ].releaseCount == 0 )
                {
                    resId = m_EffectResourceArray[ i ].resourceId;

                    if( m_EffectResourceArray[ i ].binary )
                    {
                        vfxSystem->ClearResource( pHeap, resId );
                        m_pHeap->Free( m_EffectResourceArray[ i ].binary );
                    }
                    else
                    {
                        nn::vfx::detail::OutputError( "[VFXVW] Resource Rleease Error Occured. \n" );
                    }
                    m_EffectResourceArray[ i ].Initialize();

                    return resId;
                }
            }
        }

        return -1;
    }

    //---------------------------------------------------------------------------
    //! @brief  guidを指定してリソースを破棄します。
    //! @param[in]      guid     TBD
    //! @return TBD
    //---------------------------------------------------------------------------
    bool Release( nn::vfx::viewer::detail::Guid guid ) NN_NOEXCEPT
    {
        int slot = SearchResourceSlot( guid );
        if( slot == -1 )
        {
            return false;
        }

        // 遅延解放の為の待機フレームを設定
        m_EffectResourceArray[ slot ].releaseCount = 10;

        return true;
    }

    //---------------------------------------------------------------------------
    //! @brief  guidを指定して、リソースの登録先スロットを検索します。
    //! @param[in]      guid     TBD
    //! @return TBD
    //---------------------------------------------------------------------------
    int SearchResourceSlot( nn::vfx::viewer::detail::Guid guid ) NN_NOEXCEPT
    {
        for( int i = 0; i < m_ResourceCount; ++i )
        {
            if( m_EffectResourceArray[ i ].guId == guid &&
                m_EffectResourceArray[ i ].releaseCount == -1 )
            {
                return i;
            }
        }

        return -1;
    }

    //---------------------------------------------------------------------------
    //! @brief  エミッタセット名を指定して、リソースを取得します。
    //! @param[in] guid     リソースのguid
    //! @return エフェクトリソース
    //---------------------------------------------------------------------------
    EffectResource* GetResource( nn::vfx::viewer::detail::Guid guid ) NN_NOEXCEPT
    {
        int slot = SearchResourceSlot( guid );
        if( slot == -1 )
        {
            return NULL;
        }
        return &m_EffectResourceArray[ slot ];
    }

    //---------------------------------------------------------------------------
    //! @brief  エミッタセット名を指定して、リソースを取得します。
    //! @param[in] index     リソースのインデックス
    //! @return エフェクトリソース
    //---------------------------------------------------------------------------
    EffectResource* GetResource( int index ) NN_NOEXCEPT
    {
        if ( m_ResourceCount <= index )
        {
            return NULL;
        }
        return &m_EffectResourceArray[ index ];
    }

    //---------------------------------------------------------------------------
    //! @brief  管理しているリソース数を取得します。
    //! @return エフェクトリソース数
    //---------------------------------------------------------------------------
    int GetResourceCount() const NN_NOEXCEPT
    {
        return m_ResourceCount;
    }

private:
    //---------------------------------------------------------------------------
    //! @brief   空いているスロットを取得します。
    //! @return TBD
    //---------------------------------------------------------------------------
    int GetEmptySlot() NN_NOEXCEPT
    {
        for( int i = 0; i < m_ResourceCount; ++i )
        {
            if( !m_EffectResourceArray[ i ].binary )
            {
                return i;
            }
        }

        return -1;
    }

    nn::vfx::Heap*      m_pHeap;                    //!< TBD
    int                 m_ResourceCount;            //!< 管理可能な最大リソース数
    int                 m_CacheResourceNo;          //!< 管理可能な最大リソース数
    int                 m_ResourceStartId;          //!< ビューアとして利用可能なリソースID
    EffectResource*     m_EffectResourceArray;      //!< エフェクトリソース配列
};

} // namespace detail
} // namespace viewer
} // namespace vfx
} // namespace nn

