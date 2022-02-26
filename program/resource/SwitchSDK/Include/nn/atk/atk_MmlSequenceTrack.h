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

#include <nn/atk/atk_SequenceTrack.h>

namespace nn {
namespace atk {
namespace detail {
namespace driver {

class MmlParser;

/* ========================================================================
        class definition
   ======================================================================== */

class MmlSequenceTrack : public SequenceTrack
{
public:
    MmlSequenceTrack() NN_NOEXCEPT;

    void SetMmlParser( const MmlParser* parser ) NN_NOEXCEPT { m_pParser = parser; }
    const MmlParser* GetMmlParser() const NN_NOEXCEPT { return m_pParser; }

protected:
    virtual SequenceTrack::ParseResult Parse( bool doNoteOn ) NN_NOEXCEPT NN_OVERRIDE;

private:
    const MmlParser* m_pParser;
};

} // namespace nn::atk::detail::driver
} // namespace nn::atk::detail
} // namespace nn::atk
} // namespace nn

