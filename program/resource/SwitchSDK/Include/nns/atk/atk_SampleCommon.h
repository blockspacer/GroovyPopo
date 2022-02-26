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

#include <nn/hid.h>
#include <nn/nn_Log.h>
#include <nn/nn_Abort.h>
#include <nn/hid/hid_Npad.h>

namespace nn { namespace mem {
    class StandardAllocator;
}}

namespace nn { namespace audio {
    struct MemoryPoolType;
    struct AudioRendererConfig;
}}

namespace nns { namespace atk {

    class SamplePad
    {
    public:
        enum Button
        {
            Button_A = 0,
            Button_B,
            Button_X,
            Button_Y,
            Button_L,
            Button_R,
            Button_ZR,
            Button_Left,
            Button_Right,
            Button_Up,
            Button_Down,
            Button_Start,
            Button_Count = Button_Start + 1
        };

        void Initialize();
        void Finalize() NN_NOEXCEPT;
        void Update() NN_NOEXCEPT;

        bool IsHold(Button button) NN_NOEXCEPT;
        bool IsTrigger(Button button) NN_NOEXCEPT;
        template<typename T>
        static Button DebugPadToButton() NN_NOEXCEPT
        {
            for (int i = 0; i < Button_Count; i++)
            {
                if (T::Index == DebugPadMap[i])
                {
                    return static_cast<Button>(i);
                }
            }
            NN_ABORT("Unexpected Button.\n");
        }

    private:
        void ResetCurrentButtons() NN_NOEXCEPT;
        void CopyStateCurrentToPrevious() NN_NOEXCEPT;

        // Button と DebugPad, Button と NPad の対応関係
        static const int DebugPadMap[Button_Count];
        static const int NpadMap[Button_Count];

        nn::hid::DebugPadState m_DebugPadPreviousState;
        nn::hid::DebugPadState m_DebugPadCurrentState;
        nn::hid::NpadFullKeyState m_NpadFullKeyPreviousState;
        nn::hid::NpadFullKeyState m_NpadFullKeyCurrentState;
        nn::hid::NpadHandheldState m_NpadHandheldPreviousState;
        nn::hid::NpadHandheldState m_NpadHandheldCurrentState;
    };


    void* Allocate(std::size_t size) NN_NOEXCEPT;
    void* Allocate(std::size_t size, nn::mem::StandardAllocator& allocator) NN_NOEXCEPT;
    void* Allocate(std::size_t size, int alignment, nn::mem::StandardAllocator& allocator) NN_NOEXCEPT;
    void* Allocate( std::size_t size, int alignment ) NN_NOEXCEPT;

    void Free(void* pMemory) NN_NOEXCEPT;
    void Free(void* pMemory, std::size_t size) NN_NOEXCEPT;
    void Free(void* pMemory, nn::mem::StandardAllocator& allocator) NN_NOEXCEPT;

    void InitializeHeap() NN_NOEXCEPT;
    void FinalizeHeap() NN_NOEXCEPT;

    void InitializeFileSystem() NN_NOEXCEPT;
    void FinalizeFileSystem() NN_NOEXCEPT;
    const char* GetAbsolutePath(const char* relativePath) NN_NOEXCEPT;

    void InitializeHidDevices() NN_NOEXCEPT;
    void FinalizeHidDevices() NN_NOEXCEPT;
    void UpdateHidDevices() NN_NOEXCEPT;

    bool IsHold(SamplePad::Button button) NN_NOEXCEPT;
    template<typename T>
    bool IsHold() NN_NOEXCEPT
    {
        return IsHold(SamplePad::DebugPadToButton<T>());
    }

    bool IsTrigger(SamplePad::Button button) NN_NOEXCEPT;
    template<typename T>
    bool IsTrigger() NN_NOEXCEPT
    {
        return IsTrigger(SamplePad::DebugPadToButton<T>());
    }

    void* GetPoolHeapAddress() NN_NOEXCEPT;
    size_t GetPoolHeapSize() NN_NOEXCEPT;
    void* AllocateForMemoryPool(std::size_t size) NN_NOEXCEPT;
    void* AllocateForMemoryPool(std::size_t size, int alignment) NN_NOEXCEPT;
    void FreeForMemoryPool(void* pMemory) NN_NOEXCEPT;

}}

