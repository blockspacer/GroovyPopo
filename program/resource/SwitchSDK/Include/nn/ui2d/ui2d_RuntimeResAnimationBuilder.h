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

#include <nn/ui2d/viewer/ui2d_Config.h>
#if defined(NN_UI2D_VIEWER_ENABLED)

#include <nn/util/util_FormatString.h>
#include <nn/util/util_StringUtil.h>

#include <nn/ui2d.h>
#include <nn/font.h>

#include <nn/ui2d/ui2d_StateMachine.h>

namespace nn
{
namespace ui2d
{

//---------------------------------------------------------------------------
//! @brief ステートマシン用の、アニメーションリソースを実行時に作るクラス（ライブラリ内部用のため利用しないでください）
class RuntimeResAnimationBuilder
{
private:

    //----------------------------------------------------------
    // AnimationContent の総数を数えます。
    int CountAnimationContent_(const FeatureParameterSet& featureParameterSet)
    {
        int count = 0;

        auto endIter = featureParameterSet.end();
        for (auto iter = featureParameterSet.begin(); iter != endIter; iter++)
        {
            const bool isPartsStateMachineLayer = iter->m_parameterKind == StateMachineFeatureParameterKind_PartsStateMachineLayer;
            count += isPartsStateMachineLayer ? 4 : 1;
        }

        return count;
    }

    //----------------------------------------------------------
    size_t CalcContentBlockSize_(const FeatureParameter& featureParameter, int featureIndex, const Transition& transition)
    {
        size_t resAnimationContentBlockSize = sizeof(nn::ui2d::ResAnimationContent);

        // ResAnimationContentの直後に、オフセット値と、名前テーブルが埋まっている
        if (featureParameter.m_parameterKind == StateMachineFeatureParameterKind_PartsStateMachineLayer)
        {
            resAnimationContentBlockSize += sizeof(uint32_t) * 2; // AnimInfo へのオフセット + 名前テーブルへのOffset
            resAnimationContentBlockSize += sizeof(uint32_t) * 1 + CountOfStateMachineName * 1;
        }

        const int InfoCount = featureParameter.m_FeatureParameterTypeCount;
        for (int infoIdx = 0; infoIdx < InfoCount; infoIdx++)
        {
            resAnimationContentBlockSize +=
                sizeof(uint32_t) +
                sizeof(nn::ui2d::ResAnimationInfo);

            const int TargetCount = featureParameter.m_pFeatureParameterTypes[infoIdx].m_ParamTypeCount;
            for (int targetIdx = 0; targetIdx < TargetCount; targetIdx++)
            {
                resAnimationContentBlockSize +=
                    sizeof(uint32_t) +
                    sizeof(nn::ui2d::ResAnimationTarget);

                const int KeyCount = 1;
                for (int keyIdx = 0; keyIdx < KeyCount; keyIdx++)
                {
                    resAnimationContentBlockSize += sizeof(nn::ui2d::ResParameterizedAnim);

                    NN_SDK_ASSERT_NOT_NULL(transition.GetTimeline());
                    const TransitionTimeLineTrack* pTrack = transition.GetTimeline()->GetTrackByFeatureParameterIndex(featureIndex);
                    if (pTrack == nullptr)
                    {
                        NN_SDK_ASSERT(false);
                        continue;
                    }
                    // トラックが持つキーの個数 + 1 がアニメーション（区間）の個数。
                    const int parameterizedAnimCount = pTrack->GetTransitionSectionCount() + 1;
                    resAnimationContentBlockSize += sizeof(uint32_t) * parameterizedAnimCount;
                    resAnimationContentBlockSize += sizeof(nn::ui2d::ResParameterizedAnimParameter) + parameterizedAnimCount;

                    // イベントキー用のリソース
                    const int eventKeyCount = pTrack->CalculateEventCount();
                    resAnimationContentBlockSize += sizeof(ResParameterizedAnimParameterEvent) * eventKeyCount;
                }
            }
        }

        return resAnimationContentBlockSize * 2;
    }

    //----------------------------------------------------------
    size_t CalcAnimationBlockSize_(const StateLayer& stateLayer, const Transition& transition)
    {
        size_t totalSize = sizeof(nn::ui2d::ResAnimationContent);

        // fileName
        //totalSize += sizeof(uint32_t) * 1;
        //totalSize += nn::util::align_up(strlen(pDummyFileName), sizeof(uint32_t));

        // offset to contents
        totalSize += sizeof(uint32_t) * stateLayer.GetFeatureParameterSet().size();

        // contents
        int featureIndex = 0;
        auto endIter = stateLayer.GetFeatureParameterSet().end();
        for (auto iter = stateLayer.GetFeatureParameterSet().begin(); iter != endIter; iter++)
        {
            const int animContentPerParamater = iter->m_parameterKind == StateMachineFeatureParameterKind_PartsStateMachineLayer ? 4 : 1;
            const size_t contentSize = CalcContentBlockSize_(*iter, featureIndex, transition) * animContentPerParamater;

            totalSize += contentSize;
            featureIndex++;
        }

        return totalSize;
    }

    //----------------------------------------------------------
    void WriteResParameterizedAnim_(
        const StateLayer& stateLayer,
        const FeatureParameter& featureParameter,
        int parameterizedAnimCount,
        const Transition* pCurrentTransition, float currentTransitionFrame,
        const Transition* pNextTransition,
        const int contentIdx, int infoIdx, int targetIdx,
        nn::util::BytePtr* pBytptr)
    {
        const bool IsEventParameter = featureParameter.m_AnimContentType == AnimContentType_StateMachine;
        auto pParamAnimParam = pBytptr->Get<nn::ui2d::ResParameterizedAnim>();

        pParamAnimParam->parameterizedAnimCount = static_cast<uint16_t>(parameterizedAnimCount);
        pBytptr->Advance(sizeof(nn::ui2d::ResParameterizedAnim));

        uint32_t* pOffsToParams = pBytptr->Get<uint32_t>();
        pBytptr->Advance(sizeof(uint32_t) * pParamAnimParam->parameterizedAnimCount);

        auto pFirstParam = pBytptr->Get<nn::ui2d::ResParameterizedAnimParameter>();
        for (int paramIdx = 0; paramIdx < pParamAnimParam->parameterizedAnimCount; paramIdx++)
        {
            pOffsToParams[paramIdx] = static_cast<uint32_t>(nn::util::BytePtr(pParamAnimParam).Distance(pBytptr->Get()));
            auto pParam = pBytptr->Get<nn::ui2d::ResParameterizedAnimParameter>();
            pBytptr->Advance(sizeof(nn::ui2d::ResParameterizedAnimParameter));

            if (IsEventParameter)
            {
                auto pParamEvent = pBytptr->Get<nn::ui2d::ResParameterizedAnimParameterEvent>();
                pBytptr->Advance(sizeof(nn::ui2d::ResParameterizedAnimParameterEvent));
                pParam->pEvent = pParamEvent;
                pNextTransition->GetTimeline()->SetupParametrizedAnimationEvent(pParam, contentIdx, paramIdx - 1);
            }
            else
            {
                const State* pStatePrev = stateLayer.FindStateByName(pNextTransition->GetPrevStateName());
                const State* pStateNext = stateLayer.FindStateByName(pNextTransition->GetNextStateName());

                if (pStatePrev == nullptr || pStateNext == nullptr)
                {
                    continue;
                }

                pNextTransition->GetTimeline()->SetupParametrizedAnimation(
                    pParam,
                    contentIdx,
                    infoIdx,
                    targetIdx,
                    paramIdx - 1,
                    pStatePrev->GetFeatureParameterStoreSet(),
                    pStateNext->GetFeatureParameterStoreSet());
            }
        }

        // 現在再生中のアニメーションの結果を、スタート値として上書きします。
        if (pCurrentTransition != nullptr && !IsEventParameter)
        {
            const State* pCurrentStateNext = stateLayer.FindStateByName(pCurrentTransition->GetNextStateName());
            if (pCurrentStateNext != nullptr)
            {
                nn::ui2d::ResParameterizedAnimParameter temp = nn::ui2d::ResParameterizedAnimParameter();

                pCurrentTransition->GetTimeline()->SetupParametrizedAnimationFromTime(
                    &temp, contentIdx, infoIdx, targetIdx, currentTransitionFrame,
                    stateLayer.GetCurrentFeatureParameterStore(),
                    pCurrentStateNext->GetFeatureParameterStoreSet());

                // スタート値を記憶しておきます。
                pFirstParam->value.startValue = GetParameterizedAnimValueAtFrameClamped(currentTransitionFrame, &temp);
                // StateLayer のメンバの一部を書き換える必要があるので、const_cast しています。
                const_cast<StateLayer*>(&stateLayer)->GetCurrentFeatureParameterStore().SetAt(contentIdx, infoIdx, targetIdx, pFirstParam->value.startValue);
            }
        }
    }

    //----------------------------------------------------------
    void WriteContentNameTable_(const FeatureParameter& featureParameter, nn::util::BytePtr* pBytptr, int count)
    {
        // オフセット2つの書き込み
        {
            uint32_t* pOffsArray = pBytptr->Get<uint32_t>();

            // AnimInfo へのオフセット
            pOffsArray[0] =
                sizeof(nn::ui2d::ResAnimationContent) +
                sizeof(uint32_t) * 2 +
                sizeof(uint32_t) * 1 +
                CountOfStateMachineName * 1;

            // 名前テーブルへのオフセット
            pOffsArray[1] =
                sizeof(nn::ui2d::ResAnimationContent) +
                sizeof(uint32_t) * 2;

            pBytptr->Advance(sizeof(uint32_t) * 2);
        }

        // 名前テーブルの書き込み(1エントリー固定)
        {
            const size_t HeadOffsetToNameTbl = sizeof(uint32_t) * 1;
            uint32_t* pNameOffsArray = pBytptr->Get<uint32_t>();

            pNameOffsArray[0] = HeadOffsetToNameTbl;

            pBytptr->Advance(HeadOffsetToNameTbl);

            char* pNameTbl = pBytptr->Get<char>();

            const char* pElementName = FeatureParameter::GetParamaterNameByIndex(featureParameter.m_pResStateParameterNameOption, count);
            NN_SDK_ASSERT_NOT_NULL(pElementName);

            memcpy(pNameTbl, pElementName, CountOfStateMachineName);
            pBytptr->Advance(CountOfStateMachineName);
        }
    }

    //----------------------------------------------------------
    // ResAnimationContent を実行時に作るクラス
    void WriteContent_(const StateLayer& stateLayer, const FeatureParameter& featureParameter, const Transition* pNextTransition, const Transition* pCurrentTransition, float currentTransitionFrame, nn::util::BytePtr* pBytptr, const int subElementindex)
    {
        NN_UNUSED(pCurrentTransition);
        NN_UNUSED(currentTransitionFrame);

        // メモリの先頭からデータを書き込んでいく。
        {
            auto pContent = pBytptr->Get<nn::ui2d::ResAnimationContent>();
            pBytptr->Advance(sizeof(nn::ui2d::ResAnimationContent));

            // 特定のアニメーション種類では、拡張ユーザー情報のアニメーションと同じ構造になるように
            // オフセット2つと、名前テーブルを書き込みます。
            if (featureParameter.m_parameterKind == StateMachineFeatureParameterKind_PartsStateMachineLayer)
            {
                WriteContentNameTable_(featureParameter, pBytptr, subElementindex);
            }

            const int contentIdx = stateLayer.FindFeatureParameterByName(featureParameter.m_pName, featureParameter.m_parameterKind);

            // Content 書く
            const int InfoCount = featureParameter.m_FeatureParameterTypeCount;
            {
                pContent->count = static_cast<uint8_t>(InfoCount);
                pContent->type = featureParameter.m_AnimContentType;
                nn::util::Strlcpy(pContent->name, featureParameter.m_pName, sizeof(pContent->name));
            }

            nn::util::BytePtr pBaseForInfo(pContent);
            uint32_t* pOffsToInfo = pBytptr->Get<uint32_t>();
            pBytptr->Advance(sizeof(uint32_t) * InfoCount);

            //----------------------------------------------
            // ResAnimationInfo
            for (int infoIdx = 0; infoIdx < InfoCount; infoIdx++)
            {
                pOffsToInfo[infoIdx] = static_cast<uint32_t>(pBaseForInfo.Distance(pBytptr->Get()));

                auto pInfo = pBytptr->Get<nn::ui2d::ResAnimationInfo>();
                pBytptr->Advance(sizeof(nn::ui2d::ResAnimationInfo));

                // pInfo 書く
                const int TargetCount = featureParameter.m_pFeatureParameterTypes[infoIdx].m_ParamTypeCount;
                {
                    pInfo->count = static_cast<uint8_t>(TargetCount);
                    pInfo->kind = featureParameter.m_pFeatureParameterTypes[infoIdx].m_ResAnimationInfoType;
                }

                nn::util::BytePtr pBaseForTarget(pInfo);
                uint32_t* pOffsToTarget = pBytptr->Get<uint32_t>();
                pBytptr->Advance(sizeof(uint32_t) * TargetCount);

                //----------------------------------------------
                // ResAnimationTarget
                for (int targetIdx = 0; targetIdx < TargetCount; targetIdx++)
                {
                    pOffsToTarget[targetIdx] = static_cast<uint32_t>(pBaseForTarget.Distance(pBytptr->Get()));

                    auto pTarget = pBytptr->Get<nn::ui2d::ResAnimationTarget>();
                    nn::util::BytePtr pBaseForKey(pTarget);
                    pBytptr->Advance(sizeof(nn::ui2d::ResAnimationTarget));

                    // pTarget 書く
                    {
                        pTarget->id = featureParameter.m_index;
                        pTarget->target = featureParameter.m_pFeatureParameterTypes[infoIdx].m_pParamTypes[targetIdx];
                        pTarget->keyCount = 1;
                        pTarget->curveType = nn::ui2d::AnimCurve_ParameterizedAnim;
                        pTarget->keysOffset = static_cast<uint32_t>(pBaseForKey.Distance(pBytptr->Get()));
                    }

                    //----------------------------------------------
                    // ResParameterizedAnim ResParameterizedAnimParameter
                    {
                        const TransitionTimeLineTrack* pTrack = pNextTransition->GetTimeline()->GetTrackByFeatureParameterIndex(contentIdx);
                        if (pTrack == nullptr)
                        {
                            NN_SDK_ASSERT(false);
                            return;
                        }

                        const int animParameterCount = static_cast<uint16_t>(pTrack->GetTransitionSectionCount()) + 1;
                        const int elemIndex = featureParameter.m_parameterKind == StateMachineFeatureParameterKind_PartsStateMachineLayer ? subElementindex : targetIdx;
                        WriteResParameterizedAnim_(stateLayer, featureParameter, animParameterCount, pCurrentTransition, currentTransitionFrame, pNextTransition, contentIdx, infoIdx, elemIndex, pBytptr);
                    }
                }
            }
        }
    }

public:

    //----------------------------------------------------------
    //! @brief Transition に対応するアニメーションリソースのサイズを計算します。（ライブラリ内部用のため利用しないでください）
    //!
    //! @param[in] stateLayer        ステートレイヤーです。
    //! @param[in] nextTansition     次のトランジションです。
    //!
    //! @return アニメーションリソースのサイズを返します。
    //!
    size_t CalculateAnimationResourceSize(const StateLayer& stateLayer, const Transition& nextTansition)
    {
        const size_t resAnimationBlockSize = CalcAnimationBlockSize_(stateLayer, nextTansition);
        const size_t fileSize = resAnimationBlockSize + sizeof(nn::font::detail::BinaryFileHeader);

        return fileSize;
    }

    //----------------------------------------------------------
    //! @brief Transition に対応するアニメーションリソースを生成します。（ライブラリ内部用のため利用しないでください）
    //!
    //! @param[out] pOutBuffer       出力ファイルメモリ領域へのポインターです。
    //! @param[in] outBufferSize     出力ファイルメモリ領域のサイズです。
    //! @param[in] stateLayer        ステートレイヤーです。
    //! @param[in] pNextTansition       次のトランジションです。
    //!
    void Build(void* pOutBuffer, size_t outBufferSize, const StateLayer& stateLayer, const Transition* pNextTansition)
    {
        BuildFromCurrent(pOutBuffer, outBufferSize, stateLayer, pNextTansition, nullptr, 0.0f);
    }

    //----------------------------------------------------------
    //! @brief Transition に対応するアニメーションリソースを生成します。（ライブラリ内部用のため利用しないでください）
    //!
    //! @param[out] pOutBuffer       出力ファイルメモリ領域へのポインターです。
    //! @param[in] outBufferSize     出力ファイルメモリ領域のサイズです。
    //! @param[in] stateLayer        ステートレイヤーです。
    //! @param[in] pNextTansition       次のトランジションです。
    //! @param[in] pCurrentTransition       現在のトランジションです。
    //! @param[in] currentTransitionFrame   現在のトランジションの再生フレームです。
    //!
    void BuildFromCurrent(void* pOutBuffer, size_t outBufferSize, const StateLayer& stateLayer, const Transition* pNextTansition, const Transition* pCurrentTransition, float currentTransitionFrame)
    {
        const FeatureParameterSet& featureParameterSet = stateLayer.GetFeatureParameterSet();

        const size_t resAnimationBlockSize = outBufferSize - sizeof(nn::font::detail::BinaryFileHeader);
        nn::util::BytePtr bytptr(pOutBuffer);

        //----------------------------------------------
        // BinaryFileHeader
        nn::font::detail::BinaryFileHeader* pAnimFileHeader = bytptr.Get<nn::font::detail::BinaryFileHeader>();
        pAnimFileHeader->signature = nn::ui2d::FileSignatureFlan;
        pAnimFileHeader->version = nn::ui2d::BinaryFileFormatVersion;
        pAnimFileHeader->headerSize = sizeof(nn::font::detail::BinaryFileHeader);
        pAnimFileHeader->dataBlocks = 1;
        pAnimFileHeader->byteOrder = nn::font::detail::ByteOrderMark;
        bytptr.Advance(sizeof(nn::font::detail::BinaryFileHeader));

        //----------------------------------------------
        // ResAnimationBlock
        nn::ui2d::ResAnimationBlock* pResAnim = bytptr.Get<nn::ui2d::ResAnimationBlock>();
        pResAnim->animContCount = static_cast<uint16_t>(CountAnimationContent_(featureParameterSet));
        pResAnim->fileCount = 0;
        pResAnim->frameSize = static_cast<uint16_t>(pNextTansition->GetTimeline()->GetTotalLength());
        pResAnim->blockHeader.kind = nn::ui2d::DataBlockKindPaneAnimInfo;
        pResAnim->blockHeader.size = static_cast<uint32_t>(resAnimationBlockSize);
        pResAnim->loop = pNextTansition->IsLoop();

        bytptr.Advance(sizeof(nn::ui2d::ResAnimationBlock));

        nn::util::BytePtr pBaseOfOffset(bytptr.Get<void>());

        // オフセット ⇒ ResAnimationConten
        uint32_t* pOffsOfAnimCont = bytptr.Get<uint32_t>();
        bytptr.Advance(sizeof(uint32_t) * pResAnim->animContCount);

        pResAnim->animContOffsetsOffset = static_cast<uint32_t>(nn::util::BytePtr(pResAnim).Distance(pOffsOfAnimCont));

        //----------------------------------------------
        // nn::ui2d::ResAnimationConten
        int count = 0;
        auto endIter = featureParameterSet.end();
        for (auto iter = featureParameterSet.begin(); iter != endIter; iter++)
        {
            const bool isPartsStateMachineLayer = iter->m_parameterKind == StateMachineFeatureParameterKind_PartsStateMachineLayer;
            const int animContentPerParamater = isPartsStateMachineLayer ? 4 : 1;

            for (int i = 0; i < animContentPerParamater; i++)
            {
                pOffsOfAnimCont[count] = static_cast<uint32_t>(nn::util::BytePtr(pResAnim).Distance(bytptr.Get()));
                WriteContent_(stateLayer, *iter, pNextTansition, pCurrentTransition, currentTransitionFrame, &bytptr, i);
                count++;
            }
        }
    }
};

} // namespace ui2d
} // namespace nn

#endif // NN_UI2D_VIEWER_ENABLED
