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
 * @brief Nintendo NX CPU Profiler Functions for recording code blocks
 * @details Nintendo NX CPU Profiler API
 */

#pragma once

#include <nn/nn_Common.h>
#include <nn/nn_Result.h>

#include <nn/profiler/profiler_Api.h>
#include <nn/profiler/profiler_Types.h>
#include <nn/profiler/profiler_Result.h>


namespace nn { namespace profiler {


//! @name Code Blocks API
//! @{


/**
 *  @brief Records a code block entry in the profile data.
 *
 *  @param [in] id An integer to denote this code block.
 *
 *  @retresult
 *      @handleresult{nn::ResultSuccess, The value was recorded into the buffer.}
 *      @handleresult{nn::profiler::ResultSkippedRecord,
 *          The data was not recorded into the buffer.}
 *  @endretresult
 *
 *  @details
 *  This function stores a code block entry marker into the profile data.
 *  The identifier provided is the integer that will appear in the profiler GUI to
 *  denote this specific code block.
 *
 *  An example call to this function is:
 *  @code
 *  nn::profiler::EnterCodeBlock(10);
 *  @endcode
 */
nn::Result EnterCodeBlock(uint64_t id) NN_NOEXCEPT;



/**
 *  @brief Records a code block entry in the profile data.
 *
 *  @param [in] id A string to denote this code block.
 *
 *  @retresult
 *      @handleresult{nn::ResultSuccess, The value was recorded into the buffer.}
 *      @handleresult{nn::profiler::ResultSkippedRecord,
 *          The data was not recorded into the buffer.}
 *      @handleresult{nn::profiler::ResultNullArgument, One of the arguments was NULL.}
 *  @endretresult
 *
 *  @details
 *  This function stores a code block entry marker into the profile data.
 *  The identifier provided is the string that will appear in the profiler GUI to
 *  denote this specific code block.
 *
 *  Strings up to 254 bytes may be used; longer strings will be truncated.
 *  The string used must be null-terminated.
 *  An internal buffer stores the strings provided, so dynamically created strings may be used.
 *  If that internal buffer becomes full, the GUI will use the value UINT64_MAX as the ID.
 *
 *  An example call to this function is:
 *  @code
 *  nn::profiler::EnterCodeBlock("Example Code Block");
 *  @endcode
 */
nn::Result EnterCodeBlock(const char* id) NN_NOEXCEPT;



/**
 *  @brief Records a code block exit in the profile data
 *
 *  @param [in] id An integer to denote this code block.
 *
 *  @retresult
 *      @handleresult{nn::ResultSuccess, The value was recorded into the buffer.}
 *      @handleresult{nn::profiler::ResultSkippedRecord,
 *          The data was not recorded into the buffer.}
 *  @endretresult
 *
 *  @details
 *  This function stores a code block exit marker into the profile data.
 *  The identifier provided is the integer that will appear in the profiler GUI to
 *  denote this specific code block.
 *
 *  An example call to this function is:
 *  @code
 *  nn::profiler::ExitCodeBlock(10);
 *  @endcode
 */
nn::Result ExitCodeBlock(uint64_t id) NN_NOEXCEPT;



/**
 *  @brief Records a code block exit in the profile data
 *
 *  @param [in] id A string to denote this code block.
 *
 *  @retresult
 *      @handleresult{nn::ResultSuccess, The value was recorded into the buffer.}
 *      @handleresult{nn::profiler::ResultSkippedRecord,
 *          The data was not recorded into the buffer.}
 *      @handleresult{nn::profiler::ResultNullArgument, One of the arguments was NULL.}
 *  @endretresult
 *
 *  @details
 *  This function stores a code block exit marker into the profile data.
 *  The identifier provided is the string that will appear in the profiler GUI to
 *  denote this specific code block.
 *
 *  Strings up to 254 bytes may be used; longer strings will be truncated.
 *  The string used must be null-terminated.
 *  An internal buffer stores the strings provided, so dynamically created strings may be used.
 *  If that internal buffer becomes full, the GUI will use the value UINT64_MAX as the ID.
 *
 *  An example call to this function is:
 *  @code
 *  nn::profiler::ExitCodeBlock("Example Code Block");
 *  @endcode
 */
nn::Result ExitCodeBlock(const char* id) NN_NOEXCEPT;


//! @}


/**
 *  @brief A specialized class for code block scope-based Enter/Exit functionality.
 */
class ScopedCodeBlock
{
    NN_DISALLOW_COPY(ScopedCodeBlock);
    NN_DISALLOW_MOVE(ScopedCodeBlock);

public:

    /**
     *  @brief Creates a scoped code block based on an integral ID.
     *
     *  @param [in] id An identifier to denote this code block.
     *
     *  @return The created code block.
     *
     *  @details
     *  On creation, the calls @ref EnterCodeBlock with the id provided.
     *  The identifier provided is the integer that will appear in the profiler GUI to
     *  denote this specific code block.
     *  When the created variable falls out of scope, @ref ExitCodeBlock
     *  is called automatically.
     *
     *  Here is an example on creating a ScopedCodeBlock:
     *  @code
     *  nn::profiler::ScopedCodeBlock block(10);
     *  @endcode
     */
    explicit ScopedCodeBlock(uint64_t id) NN_NOEXCEPT
        : m_Id(id)
    {
        EnterCodeBlock(m_Id);
    }


    /**
     *  @brief Creates a scoped code block based on a string.
     *
     *  @param [in] id An identifier to denote this code block.
     *
     *  @return The created code block.
     *
     *  @details
     *  On creation, the calls @ref EnterCodeBlock with the id provided.
     *  The identifier provided is the string that will appear in the profiler GUI to
     *  denote this specific code block.
     *  When the created variable falls out of scope, @ref ExitCodeBlock
     *  is called automatically.
     *
     *  Here is an example on creating a ScopedCodeBlock:
     *  @code
     *  nn::profiler::ScopedCodeBlock block("Example Code Block");
     *  @endcode
     */
    explicit ScopedCodeBlock(const char* id) NN_NOEXCEPT;


    /**
     *  @brief Exits the scoped code block.
     *
     *  @details
     *  Calls @ref ExitCodeBlock with the identifier specified during construction.
     */
    ~ScopedCodeBlock()
    {
        ExitCodeBlock(m_Id);
    }

private:
    uint64_t m_Id; /*!< Stores the code block id */
};

}} // namespace nn::profiler
