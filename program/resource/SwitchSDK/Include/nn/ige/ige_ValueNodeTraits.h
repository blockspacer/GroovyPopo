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

#include <nn/util/util_StringView.h>
#
namespace nn { namespace ige {

class BufferView;
class BufferWriter;
template <typename T, typename TValueNodeTraits> class ValueNode;
template <typename T, typename TValueArrayNodeTraits> class ValueArrayNode;

template<typename T>
class ValueNodeTraits
{
public:
    typedef ValueNode<T, ValueNodeTraits<T>> TNode;

    static util::string_view GetTypeName() NN_NOEXCEPT;
    static void Serialize(BufferWriter* pWriter, T* pValue, const TNode& node) NN_NOEXCEPT;
    static void Deserialize(const BufferView& buffer, T* pValue, const TNode& node) NN_NOEXCEPT;
};

template<typename T>
class ValueArrayNodeTraits
{
public:
    typedef ValueArrayNode<T, ValueArrayNodeTraits<T>> TNode;

    static util::string_view GetTypeName() NN_NOEXCEPT;
    static void Serialize(BufferWriter* pWriter, T* pValue, int count, const TNode& node) NN_NOEXCEPT;
    static void Deserialize(const BufferView& buffer, T* pValue, int count, const TNode& node) NN_NOEXCEPT;
};

class StringNodeTraits
{
public:
    typedef ValueArrayNode<char, StringNodeTraits> TNode;

    static util::string_view GetTypeName() NN_NOEXCEPT;
    static void Serialize(BufferWriter* pWriter, char* pValue, int count, const TNode& node) NN_NOEXCEPT;
    static void Deserialize(const BufferView& buffer, char* pValue, int count, const TNode& node) NN_NOEXCEPT;
};

}} // namespace nn::ige
