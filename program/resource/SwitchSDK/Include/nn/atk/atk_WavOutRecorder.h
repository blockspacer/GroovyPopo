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

#include <nn/atk/atk_Global.h>
#include <nn/atk/atk_DeviceOutRecorder.h>
#include <nn/atk/atk_HardwareManager.h>
#include <nn/atk/atk_Config.h>
#include <nn/atk/atk_Util.h>


namespace nn {
namespace atk {

//---------------------------------------------------------------------------
//! @internal
//! @brief  波形出力を録音するクラスです。
//---------------------------------------------------------------------------
class WavOutRecorder : public DeviceOutRecorder
{
private:
    static const int MaxChannels     = 6;                              //!< 最大チャンネル数
    static const int BytesPerSample  = 2;                              //!< サンプルビット数 : 16bit
    static const int MaxFrameLength = MaxChannels * BytesPerSample;    //!< 最大フレームサイズ

    static const int FlIndex  = 0;
    static const int FrIndex  = 1;
    static const int FcIndex  = 2;
    static const int LfeIndex = 3;
    static const int RlIndex  = 4;
    static const int RrIndex  = 5;

    static const char* DeviceName;

public:
    //---------------------------------------------------------------------------
    //! @brief         コンストラクタです。
    //---------------------------------------------------------------------------
    WavOutRecorder() NN_NOEXCEPT;

    //---------------------------------------------------------------------------
    //! @brief         デストラクタです。
    //---------------------------------------------------------------------------
    virtual ~WavOutRecorder() NN_NOEXCEPT NN_OVERRIDE;

protected:
    virtual uint32_t GetMaxFrameLength() const NN_NOEXCEPT NN_OVERRIDE { return MaxFrameLength; }

    virtual uint32_t GetSamplesPerSec() const NN_NOEXCEPT NN_OVERRIDE
    {
        return detail::driver::HardwareManager::GetInstance().GetRendererSampleRate();
    }

    virtual uint32_t GetValidChannels(uint32_t /* channels */) const NN_NOEXCEPT NN_OVERRIDE;
    virtual void OnStart() NN_NOEXCEPT NN_OVERRIDE;
    virtual void OnStop() NN_NOEXCEPT NN_OVERRIDE;

    virtual uint32_t OnProcessSamples(int16_t* sampleBuffer, uint32_t samples) NN_NOEXCEPT NN_OVERRIDE;

private:

    static WavOutRecorder* g_pThis;      //!< AXコールバックから参照するための this ポインタ
};

} // atk
} // nn
