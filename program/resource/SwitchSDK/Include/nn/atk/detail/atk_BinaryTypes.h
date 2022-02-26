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

#include <stdint.h>
#include <nn/nn_Macro.h>
#include <nn/util/util_BytePtr.h>

namespace nn { namespace atk { namespace detail {

    struct BinaryTypes
    {
        static const uint32_t InvalidOffset = 0xffffffff;
        static const uint32_t InvalidSize = 0xffffffff;

        template< typename ItemType, typename CountType = int >
        struct Table
        {
            CountType count;
            ItemType item[ 1 ];
        };

        struct Reference
        {
            uint32_t offset;     // InvalidOffset が入っていたら無効値

            static const uint32_t InvalidOffset = BinaryTypes::InvalidOffset;

            inline bool IsValidOffset() const NN_NOEXCEPT
            {
                if ( offset != InvalidOffset )
                {
                    return true;
                }
                return false;
            }
        };

        struct ReferenceTable : public Table<Reference>
        {
            const void* GetReferedItem( int index ) const NN_NOEXCEPT
            {
                if ( index >= count )
                {
                    return nullptr;
                }

                nn::util::ConstBytePtr bytePtr(this);
                bytePtr += item[ index ].offset;

                return bytePtr.Get();
            }
        };
    };

}}} // namespace nn::atk::detail
