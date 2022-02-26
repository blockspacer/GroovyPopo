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

namespace nn {
namespace atk {
namespace detail {
namespace driver {

class MmlCommand
{
public:

    enum Mml
    {
        // 可変長パラメータコマンド
        Mml_Wait                     = 0x80,
        Mml_Prg                      = 0x81,

        // 制御コマンド
        Mml_OpenTrack                = 0x88,
        Mml_Jump                     = 0x89,
        Mml_Call                     = 0x8a,

        // 接頭コマンド
        Mml_Random                   = 0xa0,
        Mml_Variable                 = 0xa1,
        Mml_If                       = 0xa2,
        Mml_Time                     = 0xa3,
        Mml_TimeRandom               = 0xa4,
        Mml_TimeVariable             = 0xa5,

        // uint8_tパラメータコマンド
        Mml_Timebase                 = 0xb0,
        Mml_EnvHold                  = 0xb1,
        Mml_Monophonic               = 0xb2,
        Mml_VelocityRange            = 0xb3,
        Mml_BiquadType               = 0xb4,
        Mml_BiquadValue              = 0xb5,
        Mml_BankSelect               = 0xb6,
        // b7-bc の 6 つがあいている
        Mml_ModPhase                 = 0xbd,
        Mml_ModCurve                 = 0xbe,
        Mml_FrontBypass              = 0xbf,
        Mml_Pan                      = 0xc0,
        Mml_Volume                   = 0xc1,
        Mml_MainVolume               = 0xc2,
        Mml_Transpose                = 0xc3,
        Mml_PitchBend                = 0xc4,
        Mml_BendRange                = 0xc5,
        Mml_Prio                     = 0xc6,
        Mml_NoteWait                 = 0xc7,
        Mml_Tie                      = 0xc8,
        Mml_Porta                    = 0xc9,
        Mml_ModDepth                 = 0xca,
        Mml_ModSpeed                 = 0xcb,
        Mml_ModType                  = 0xcc,
        Mml_ModRange                 = 0xcd,
        Mml_PortaSw                  = 0xce,
        Mml_PortaTime                = 0xcf,
        Mml_Attack                   = 0xd0,
        Mml_Decay                    = 0xd1,
        Mml_Sustain                  = 0xd2,
        Mml_Release                  = 0xd3,
        Mml_LoopStart                = 0xd4,
        Mml_Volume2                  = 0xd5,
        Mml_Printvar                 = 0xd6,
        Mml_SurroundPan              = 0xd7,
        Mml_LpfCutoff                = 0xd8,
        Mml_FxsendA                  = 0xd9,
        Mml_FxsendB                  = 0xda,
        Mml_Mainsend                 = 0xdb,
        Mml_InitPan                  = 0xdc,
        Mml_Mute                     = 0xdd,
        Mml_FxsendC                  = 0xde,
        Mml_Damper                   = 0xdf,

        // int16_tパラメータコマンド
        Mml_ModDelay                 = 0xe0,
        Mml_Tempo                    = 0xe1,
        Mml_SweepPitch               = 0xe3,
        Mml_ModPeriod                = 0xe4,

        // 拡張コマンド
        Mml_ExCommand                = 0xf0,

        // その他
        Mml_EnvReset                 = 0xfb,
        Mml_LoopEnd                  = 0xfc,
        Mml_Ret                      = 0xfd,
        Mml_AllocTrack               = 0xfe,
        Mml_Fin                      = 0xff
    };

    enum MmlEx
    {
        MmlEx_Setvar     = 0x80,
        MmlEx_Addvar     = 0x81,
        MmlEx_Subvar     = 0x82,
        MmlEx_Mulvar     = 0x83,
        MmlEx_Divvar     = 0x84,
        MmlEx_Shiftvar   = 0x85,
        MmlEx_Randvar    = 0x86,
        MmlEx_Andvar     = 0x87,
        MmlEx_Orvar      = 0x88,
        MmlEx_Xorvar     = 0x89,
        MmlEx_Notvar     = 0x8a,
        MmlEx_Modvar     = 0x8b,

        MmlEx_CmpEq      = 0x90,
        MmlEx_CmpGe      = 0x91,
        MmlEx_CmpGt      = 0x92,
        MmlEx_CmpLe      = 0x93,
        MmlEx_CmpLt      = 0x94,
        MmlEx_CmpNe      = 0x95,

        MmlEx_Mod2Curve  = 0xa0,
        MmlEx_Mod2Phase  = 0xa1,
        MmlEx_Mod2Depth  = 0xa2,
        MmlEx_Mod2Speed  = 0xa3,
        MmlEx_Mod2Type   = 0xa4,
        MmlEx_Mod2Range  = 0xa5,

        MmlEx_Mod3Curve  = 0xa6,
        MmlEx_Mod3Phase  = 0xa7,
        MmlEx_Mod3Depth  = 0xa8,
        MmlEx_Mod3Speed  = 0xa9,
        MmlEx_Mod3Type   = 0xaa,
        MmlEx_Mod3Range  = 0xab,

        MmlEx_Mod4Curve  = 0xac,
        MmlEx_Mod4Phase  = 0xad,
        MmlEx_Mod4Depth  = 0xae,
        MmlEx_Mod4Speed  = 0xaf,
        MmlEx_Mod4Type   = 0xb0,
        MmlEx_Mod4Range  = 0xb1,

        MmlEx_Userproc   = 0xe0,
        MmlEx_Mod2Delay  = 0xe1,
        MmlEx_Mod2Period = 0xe2,
        MmlEx_Mod3Delay  = 0xe3,
        MmlEx_Mod3Period = 0xe4,
        MmlEx_Mod4Delay  = 0xe5,
        MmlEx_Mod4Period = 0xe6
    };
};


} // namespace nn::atk::detail::driver
} // namespace nn::atk::detail
} // namespace nn::atk
} // namespace nn

