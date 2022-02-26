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

namespace nn
{
namespace ui2d
{
//----------------------------------------------------------
// 注意：ステートマシン機能はライブラリ内部利用のため、利用しないでください。
//----------------------------------------------------------

//----------------------------------------------------------
// イベント種類
enum StateMachineEventKind
{
    StateMachineEventKind_None,
    StateMachineEventKind_Tick,
    StateMachineEventKind_Hit,
    StateMachineEventKind_NoHit,
    StateMachineEventKind_Decided,
    StateMachineEventKind_StateTransitionCompleted,
    StateMachineEventKind_SubStateTransitionCompleted,
    StateMachineEventKind_VariableChanged,
    StateMachineEventKind_StateChangeRequested,
    StateMachineEventKind_StateVariablesSetRequested,
    StateMachineEventKind_StateVariablesAddRequested,
    StateMachineEventKind_ButtonDecided,
    StateMachineEventKind_Max
};

//----------------------------------------------------------
// イベント
struct StateMachineEvent
{
    //! @brief 内部用機能のため使用禁止です。
    nn::util::IntrusiveListNode m_Link;

    uint32_t kind;
    uint64_t param1;
    uint64_t param2;
    uint64_t param3;
    uint32_t delayFrames;
};

typedef nn::util::IntrusiveList<StateMachineEvent, nn::util::IntrusiveListMemberNodeTraits<StateMachineEvent, &StateMachineEvent::m_Link> > StateMachineEventSet;

//----------------------------------------------------------
// イベントキュー
class StateMachineEventQueue
{
public:

    static const int Max = 20;

    StateMachineEventQueue()
        : m_pStateMachineEventArray(nullptr)
    {
    }

    //----------------------------------------------------------
    void Initialize()
    {
        m_pStateMachineEventArray = nn::ui2d::Layout::NewArray<StateMachineEvent>(Max);
        for (int i = 0; i < Max; i++)
        {
            m_FreeEvents.push_back(m_pStateMachineEventArray[i]);
        }

        Reset();
    }

    //----------------------------------------------------------
    void Finalize()
    {
        nn::ui2d::Layout::DeleteArray(m_pStateMachineEventArray, Max);
        m_pStateMachineEventArray = nullptr;
    }

    //----------------------------------------------------------
    void Reset()
    {
        for (auto iter = m_StateMachineEvents.begin(); iter != m_StateMachineEvents.end(); )
        {
            auto currIter = iter;
            iter++;

            m_FreeEvents.push_back(*currIter);
        }
    }

    //----------------------------------------------------------
    void Push(StateMachineEventKind kind, uint64_t param1, uint64_t param2, uint64_t param3, uint32_t delayFrames)
    {
        if (m_FreeEvents.empty())
        {
            NN_SDK_ASSERT(false);
            return;
        }

        // フリーリストから取って、後ろに追加
        auto iter = m_FreeEvents.begin();
        m_FreeEvents.pop_front();

        iter->kind = kind;
        iter->param1 = param1;
        iter->param2 = param2;
        iter->param3 = param3;

        iter->delayFrames = delayFrames;

        m_StateMachineEvents.push_back(*iter);
    }

    //----------------------------------------------------------
    StateMachineEvent* Peek()
    {
        if (m_StateMachineEvents.empty())
        {
            return nullptr;
        }

        return &m_StateMachineEvents.front();
    }

    //----------------------------------------------------------
    StateMachineEvent* Bottom()
    {
        if (m_StateMachineEvents.empty())
        {
            return nullptr;
        }

        return &m_StateMachineEvents.back();
    }

    //----------------------------------------------------------
    void Pop()
    {
        if (m_StateMachineEvents.empty())
        {
            return;
        }

        auto iter = m_StateMachineEvents.begin();
        m_StateMachineEvents.pop_front();
        m_FreeEvents.push_back(*iter);
    }

    //----------------------------------------------------------
    void MoveTopToTail()
    {
        StateMachineEvent* pHead = Peek();
        if (pHead != nullptr)
        {
            m_StateMachineEvents.pop_front();
            m_StateMachineEvents.push_back(*pHead);
        }
    }

    //----------------------------------------------------------
    const StateMachineEventSet& GetStateMachineEventSet() const
    {
        return m_StateMachineEvents;
    }

private:

    //----------------------------------------------------------
    StateMachineEvent*   m_pStateMachineEventArray;
    StateMachineEventSet m_StateMachineEvents;
    StateMachineEventSet m_FreeEvents;
};

//----------------------------------------------------------
// UI イベント種類
enum StateMachineUiEventKind
{
    StateMachineUiEventKind_None,
    StateMachineUiEventKind_Decide,
    StateMachineUiEventKind_Reset,
    StateMachineUiEventKind_In,
    StateMachineUiEventKind_Out,
    StateMachineUiEventKind_Max
};

//----------------------------------------------------------
struct FeatureParameterType
{
    //----------------------------------------------------------
    FeatureParameterType()
        : m_ResAnimationInfoType(0)
        , m_ParamTypeCount(0)
        , m_pParamTypes(nullptr)
    {
    }

    //----------------------------------------------------------
    uint32_t m_ResAnimationInfoType; //  AnimationTypeMaterialColor など。

    int      m_ParamTypeCount;
    uint8_t* m_pParamTypes; //  nn::ui2d::AnimTargetPaneColor(AnimTargetPaneColor_VertexLtR) など。
};

//----------------------------------------------------------
// 状態が持つ「特徴」パラメーター
class FeatureParameter
{
public:

    //----------------------------------------------------------
    // index で要素の名前情報リソースを取得します。
    static const char* GetParamaterNameByIndex(const ResStateParameterNameOption* pNameOption, int index)
    {
        NN_SDK_ASSERT_MINMAX(index, 0, 3);

        switch (index)
        {
        case 0: return pNameOption->targetNameX;
        case 1: return pNameOption->targetNameY;
        case 2: return pNameOption->targetNameZ;
        case 3: return pNameOption->targetNameW;
        default: return nullptr;
        }
    }

    //----------------------------------------------------------
    FeatureParameter()
        : m_pName(nullptr)
        , m_parameterKind(StateMachineFeatureParameterKind_None)
        , m_index(0)
        , m_AnimContentType(0)
        , m_FeatureParameterTypeCount(0)
        , m_pFeatureParameterTypes(nullptr)
        , m_pResStateParameterNameOption(nullptr)
    {
    }

    //----------------------------------------------------------
    ~FeatureParameter()
    {
    }

    //----------------------------------------------------------
    void Initialzie(const char*pName, StateMachineFeatureParameterKind parameterKind, int index, AnimContentType contentType, uint32_t animationType, int countParamTypes, const uint8_t* pParamTypes)
    {
        const int size = nn::util::Strnlen(pName, CountOfStateMachineName) + 1;
        char* pBuf = static_cast<char*>(nn::ui2d::Layout::AllocateMemory(size));
        nn::util::Strlcpy(pBuf, pName, size);

        m_pName = static_cast<const char*>(pBuf);
        m_parameterKind = parameterKind;
        m_index = static_cast<uint8_t>(index);

        m_AnimContentType = static_cast<uint8_t>(contentType); // 例: nn::ui2d::AnimContentType_Material

        // 暫定で、Count = 1 固定で作っておく。
        m_FeatureParameterTypeCount = 1;
        m_pFeatureParameterTypes = nn::ui2d::Layout::NewArray<FeatureParameterType>(1);

        // 暫定で、Count = 1 固定で作っておく。
        m_pFeatureParameterTypes[0].m_ResAnimationInfoType = animationType; // 例: nn::ui2d::AnimationTypeMaterialColor
        m_pFeatureParameterTypes[0].m_ParamTypeCount = countParamTypes;
        m_pFeatureParameterTypes[0].m_pParamTypes = nn::ui2d::Layout::NewArray<uint8_t>(countParamTypes);
        for (int i = 0; i < countParamTypes; i++)
        {
            m_pFeatureParameterTypes[0].m_pParamTypes[i] = pParamTypes[i]; // 例：AnimTargetMatColor_BufferR
        }
    }

    //----------------------------------------------------------
    void InitialzieForPartsStateLayer(const char*pName, StateMachineFeatureParameterKind parameterKind, int index, AnimContentType contentType, uint32_t animationType, int countParamTypes, const uint8_t* pParamTypes, const ResStateParameterNameOption* pResStateParameterNameOption)
    {
        Initialzie(pName, parameterKind, index, contentType, animationType, countParamTypes, pParamTypes);

        m_pResStateParameterNameOption = pResStateParameterNameOption;
    }

    //----------------------------------------------------------
    void Finalize()
    {
        nn::ui2d::Layout::FreeMemory(const_cast<void*>(static_cast<const void*>(m_pName)));
        m_pName = nullptr;

        // m_pFeatureParameterTypes
        {
            for (int i = 0; i < m_FeatureParameterTypeCount; i++)
            {
                nn::ui2d::Layout::DeleteArray(m_pFeatureParameterTypes[i].m_pParamTypes, 1);
                m_pFeatureParameterTypes[i].m_pParamTypes = nullptr;
            }
            nn::ui2d::Layout::DeleteArray(m_pFeatureParameterTypes, 1);
            m_pFeatureParameterTypes = nullptr;
        }
    }

    //----------------------------------------------------------
    //! @brief 内部用機能のため使用禁止です。
    nn::util::IntrusiveListNode m_Link;

    const char* m_pName;
    StateMachineFeatureParameterKind m_parameterKind; // StateMachineFeatureParameterKind
    uint8_t m_index;

    uint8_t m_AnimContentType; // 例: nn::ui2d::AnimContentType_Material

    int m_FeatureParameterTypeCount;
    FeatureParameterType* m_pFeatureParameterTypes;
    const ResStateParameterNameOption* m_pResStateParameterNameOption;
};

// 状態が持つ「特徴」パラメーター（複数束ねて、特徴となる）
typedef nn::util::IntrusiveList<FeatureParameter, nn::util::IntrusiveListMemberNodeTraits<FeatureParameter, &FeatureParameter::m_Link> > FeatureParameterSet;

//----------------------------------------------------------
// 「特徴」の実際値
struct FeatureParameterLeafStore
{
    //----------------------------------------------------------
    FeatureParameterLeafStore()
        : paramValue(0.0f)
    {
    }

    //----------------------------------------------------------
    float paramValue;
};

//----------------------------------------------------------
// 「特徴」の実際値
struct FeatureParameterLeafStoreSet
{
    //----------------------------------------------------------
    FeatureParameterLeafStoreSet()
        : count(0)
        , pLeafs(nullptr)
    {
    }

    //----------------------------------------------------------
    int count;
    FeatureParameterLeafStore* pLeafs;
};

//----------------------------------------------------------
// 「特徴」の実際値
// FeatureParameterStore は、FeatureParameter と同じ数だけ、同じ順番で格納されていると想定しています。
struct FeatureParameterStore
{
    nn::util::IntrusiveListNode m_Link;

    // 実際の値。
    int count;
    FeatureParameterLeafStoreSet* pLeafSetArray;
};

// 「特徴」の実際値（束ねたもの）
typedef nn::util::IntrusiveList<FeatureParameterStore, nn::util::IntrusiveListMemberNodeTraits<FeatureParameterStore, &FeatureParameterStore::m_Link> > FeatureParameterStoreList;

//----------------------------------------------------------
// 「特徴」の実際値
class FeatureParameterStoreSet : public FeatureParameterStoreList
{
public:
    //----------------------------------------------------------
    const FeatureParameterLeafStore* GetAt(int contentIdx, int infoIdx, int targetIdx) const
    {
        const FeatureParameterStore* pParamStore = GetFeatureParameterStoreAt(contentIdx);
        if (pParamStore == nullptr)
        {
            return nullptr;
        }

        if (infoIdx >= pParamStore->count)
        {
            return nullptr;
        }

        if (targetIdx >= pParamStore->pLeafSetArray[infoIdx].count)
        {
            return nullptr;
        }

        return &pParamStore->pLeafSetArray[infoIdx].pLeafs[targetIdx];
    }

    //----------------------------------------------------------
    const FeatureParameterStore* GetFeatureParameterStoreAt(int contentIdx) const
    {
        int count = 0;
        for (auto iter = this->begin(); iter != this->end(); iter++)
        {
            if (count == contentIdx)
            {
                return &(*iter);
            }
            count++;
        }

        return nullptr;
    }

    void Initialzie(const FeatureParameterSet& featureParameterSet)
    {
        for (auto iter = featureParameterSet.begin(); iter != featureParameterSet.end(); iter++)
        {
            FeatureParameterStore* pStore = nn::ui2d::Layout::AllocateAndConstruct<FeatureParameterStore>();
            pStore->count = iter->m_FeatureParameterTypeCount;
            pStore->pLeafSetArray = nn::ui2d::Layout::NewArray<FeatureParameterLeafStoreSet>(pStore->count);
            for (int i = 0; i < pStore->count; i++)
            {
                const int leafParamCount = iter->m_pFeatureParameterTypes[i].m_ParamTypeCount;
                pStore->pLeafSetArray[i].count = leafParamCount;
                pStore->pLeafSetArray[i].pLeafs = nn::ui2d::Layout::NewArray<FeatureParameterLeafStore>(leafParamCount);
            }

            this->push_back(*pStore);
        }
    }

    void Finalize()
    {
        auto endIter = this->end();
        for (auto iter = this->begin(); iter != endIter; )
        {
            auto currIter = iter;
            iter++;

            this->erase(currIter);
            for (int i = 0; i < currIter->count; i++)
            {
                const int leafParamCount = currIter->pLeafSetArray[i].count;
                nn::ui2d::Layout::DeleteArray(currIter->pLeafSetArray[i].pLeafs, leafParamCount);
            }
            nn::ui2d::Layout::DeleteArray(currIter->pLeafSetArray, currIter->count);

            nn::ui2d::Layout::DeleteObj(&(*currIter));
        }
    }

    void SetAt(int paramIndex, int storeCount, int elementIdx, float value)
    {
        int index = 0;
        for (auto iter = this->begin(); iter != this->end(); iter++)
        {
            if (paramIndex == index)
            {
                NN_SDK_ASSERT(storeCount < iter->count);
                NN_SDK_ASSERT(elementIdx < iter->pLeafSetArray[storeCount].count);

                iter->pLeafSetArray[storeCount].pLeafs[elementIdx].paramValue = value;
            }

            index++;
        }
    }
};

//----------------------------------------------------------
// ステート
class State
{
public:

    //----------------------------------------------------------
    State()
        : m_pName(nullptr)
    {
    }

    //----------------------------------------------------------
    ~State()
    {
    }

    //----------------------------------------------------------
    void Initialzie(const char*pName, const FeatureParameterSet& featureParameterSet)
    {
        const int size = nn::util::Strnlen(pName, CountOfStateMachineName) + 1;
        auto pBuf = nn::ui2d::Layout::AllocateMemory(size);
        nn::util::Strlcpy(static_cast<char*>(pBuf), pName, size);
        m_pName = static_cast<const char*>(pBuf);

        m_FeatureParameterStoreSet.Initialzie(featureParameterSet);
    }

    //----------------------------------------------------------
    void Finalize()
    {
        nn::ui2d::Layout::FreeMemory(const_cast<void*>(static_cast<const void*>(m_pName)));
        m_pName = nullptr;

        m_FeatureParameterStoreSet.Finalize();
    }

    //----------------------------------------------------------
    void SetParameters(int paramIndex, int storeCount, float* pStore)
    {
        // テスト用に適当な値を入れています
        int index = 0;
        for (auto iter = m_FeatureParameterStoreSet.begin(); iter != m_FeatureParameterStoreSet.end(); iter++)
        {
            if (paramIndex == index)
            {
                for (int i = 0; i < iter->count; i++)
                {
                    for (int j = 0; j < iter->pLeafSetArray[i].count; j++)
                    {
                        float val = (storeCount > j) ? pStore[j] : 0.0f;
                        iter->pLeafSetArray[i].pLeafs[j].paramValue = val;
                    }
                }
            }

            index++;
        }
    }

    //----------------------------------------------------------
    const char* GetName() const
    {
        return m_pName;
    }

    //----------------------------------------------------------
    const FeatureParameterStoreSet& GetFeatureParameterStoreSet() const
    {
        return m_FeatureParameterStoreSet;
    }

    //----------------------------------------------------------
    nn::util::IntrusiveListNode m_Link;

private:

    const char* m_pName;

    FeatureParameterStoreSet m_FeatureParameterStoreSet;
};

typedef nn::util::IntrusiveList<State, nn::util::IntrusiveListMemberNodeTraits<State, &State::m_Link> > StateSet;

//----------------------------------------------------------
// ステート変数型
enum StateMachineVariableType
{
    StateMachineVariableType_Bool,
    StateMachineVariableType_Float,
    StateMachineVariableType_Uint32,
    StateMachineVariableType_Uint64,
    StateMachineVariableType_Max,
};

//----------------------------------------------------------
// ステート演算
enum StateMachineOperator
{
    StateMachineOperator_GreaterThan,
    StateMachineOperator_GreaterEqual,
    StateMachineOperator_Equal,
    StateMachineOperator_NotEqual,
    StateMachineOperator_LessEqual,
    StateMachineOperator_LessThan,
    StateMachineOperator_AlwaysPass,
    StateMachineOperator_AlwaysFail,
    StateMachineOperator_Max
};

namespace detail
{
inline ParameterizedAnimType ConvertToParameterizedAnimType(StateMachineEventKind kind)
{
    switch (kind)
    {
    case StateMachineEventKind_StateChangeRequested: return ParameterizedAnimType_EventForStateChange;
    case StateMachineEventKind_StateVariablesSetRequested: return ParameterizedAnimType_EventForVariablesSet;
    case StateMachineEventKind_StateVariablesAddRequested: return ParameterizedAnimType_EventForVariablesAdd;
    default:
        NN_UNEXPECTED_DEFAULT;
    }
}

inline StateMachineEventKind ConvertToStateMachineEventKind(ParameterizedAnimType  kind)
{
    switch (kind)
    {
    case ParameterizedAnimType_EventForStateChange: return nn::ui2d::StateMachineEventKind_StateChangeRequested;
    case ParameterizedAnimType_EventForVariablesSet: return nn::ui2d::StateMachineEventKind_StateVariablesSetRequested;
    case ParameterizedAnimType_EventForVariablesAdd: return nn::ui2d::StateMachineEventKind_StateVariablesAddRequested;
    default:
        NN_UNEXPECTED_DEFAULT;
    }
}

inline bool IsStateVariablesChangeRequestEvent(uint32_t kind)
{
    // StateMachineEventKind
    return
        kind == StateMachineEventKind_StateVariablesSetRequested ||
        kind == StateMachineEventKind_StateVariablesAddRequested;
}
}

//----------------------------------------------------------
// ステート変数
struct StateMachineVariableContent
{
    //----------------------------------------------------------
    union
    {
        bool     boolValue;
        float    floatValue;
        uint32_t uint32Value;
        uint64_t uint64Value;
    } m_Value;

    //----------------------------------------------------------
    StateMachineVariableContent()
        : m_Type(StateMachineVariableType_Bool)
    {
        m_Value.uint64Value = 0;
    }

    //----------------------------------------------------------
    static bool Set(StateMachineVariableContent* pContent, StateMachineVariableType type, void* pValue)
    {
        pContent->m_Type = type;
        switch (pContent->m_Type)
        {
        case StateMachineVariableType_Bool:
            return SetAsBool(pContent, *reinterpret_cast<bool*>(pValue));
        case StateMachineVariableType_Float:
            return SetAsFloat(pContent, *reinterpret_cast<float*>(pValue));
        default:
            return false;
        }
    }

    //----------------------------------------------------------
    static bool SetAsFloat(StateMachineVariableContent* pContent, float newValue)
    {
        NN_SDK_ASSERT(pContent->m_Type == StateMachineVariableType_Float);

        if (newValue != pContent->m_Value.floatValue)
        {
            pContent->m_Value.floatValue = newValue;
            return true;
        }

        return false;
    }

    //----------------------------------------------------------
    static bool SetAsBool(StateMachineVariableContent* pContent, bool newValue)
    {
        NN_SDK_ASSERT(pContent->m_Type == StateMachineVariableType_Bool);

        if (newValue != pContent->m_Value.boolValue)
        {
            pContent->m_Value.boolValue = newValue;
            return true;
        }

        return false;
    }

    //----------------------------------------------------------
    template<class T> static bool Operator_GreaterThan_(T valA, T valB)
    {
        return valA > valB;
    }

    //----------------------------------------------------------
    template<class T> static bool Operator_LessThan_(T valA, T valB)
    {
        return valA < valB;
    }

    //----------------------------------------------------------
    static bool GetOperationResult(StateMachineOperator op, const StateMachineVariableContent& valA, const StateMachineVariableContent& valB)
    {
        // 型は一致している想定
        NN_SDK_ASSERT(valA.m_Type == valB.m_Type);

        switch (op)
        {
        case StateMachineOperator_GreaterThan:
            switch (valA.m_Type)
            {
            case StateMachineVariableType_Bool: return Operator_GreaterThan_(valA.m_Value.boolValue, valB.m_Value.boolValue);
            case StateMachineVariableType_Float: return Operator_GreaterThan_(valA.m_Value.floatValue, valB.m_Value.floatValue);
            default:
                break;
            }
            break;
        case StateMachineOperator_LessThan:
            switch (valA.m_Type)
            {
            case StateMachineVariableType_Bool: return Operator_LessThan_(valA.m_Value.boolValue, valB.m_Value.boolValue);
            case StateMachineVariableType_Float: return Operator_LessThan_(valA.m_Value.floatValue, valB.m_Value.floatValue);
            default:
                break;
            }
            break;
        default:
            break;
        }

        return false;
    }

    //----------------------------------------------------------
    StateMachineVariableType m_Type;
};

//----------------------------------------------------------
// ステート変数
struct StateMachineVariable
{
    nn::util::IntrusiveListNode m_Link;
    char                        m_Name[nn::ui2d::ResourceNameStrMax + 1];
    float                       m_DefaultValue;
    float                       m_MinValue;
    float                       m_MaxValue;
    float                       m_Reserved;
    StateMachineVariableContent m_Content;
};

typedef nn::util::IntrusiveList<StateMachineVariable, nn::util::IntrusiveListMemberNodeTraits<StateMachineVariable, &StateMachineVariable::m_Link> > StateMachineVariableList;

//----------------------------------------------------------
class StateMachineVariableManager
{
public:

    //----------------------------------------------------------
    StateMachineVariableManager()
        : m_pEventQueue(nullptr)
    {
    }

    //----------------------------------------------------------
    void Initialize(StateMachineEventQueue* pEventQueue)
    {
        m_pEventQueue = pEventQueue;
    }

    //----------------------------------------------------------
    void InitialzieVariable_(StateMachineVariable* pVariable, const char* pName, StateMachineVariableType contenType, float defalut, float min, float max)
    {
        nn::util::Strlcpy(pVariable->m_Name, pName, sizeof(pVariable->m_Name));
        pVariable->m_Content.m_Type = contenType;
        pVariable->m_Content.m_Value.uint64Value = 0;

        pVariable->m_DefaultValue = defalut;
        pVariable->m_MinValue = min;
        pVariable->m_MaxValue = max;
    }

    //----------------------------------------------------------
    void Finalize()
    {
        auto endIter = m_StateMachineVariableList.end();
        for (auto iter = m_StateMachineVariableList.begin(); iter != endIter; )
        {
            auto currIter = iter;
            iter++;

            m_StateMachineVariableList.erase(currIter);
            nn::ui2d::Layout::DeleteObj(&(*currIter));
        }
    }

    //----------------------------------------------------------
    bool RegisterNewVariable(const char* pName, StateMachineVariableType contenType, float defalut, float min, float max)
    {
        if (FindRefOnlyByName_(pName) != nullptr)
        {
            return false;
        }

        auto* pNewValuable = nn::ui2d::Layout::AllocateAndConstruct<StateMachineVariable>();
        NN_SDK_ASSERT_NOT_NULL(pNewValuable);

        InitialzieVariable_(pNewValuable, pName, contenType, defalut, min, max);
        this->ResetToDefalut_(pNewValuable);

        m_StateMachineVariableList.push_back(*pNewValuable);

        return true;
    }

    //----------------------------------------------------------
    // 目当ての変数が存在するかを調査することが可能。
    bool Contanis(const char* pName) const
    {
        return FindRefOnlyByName_(pName) != nullptr;
    }

    //----------------------------------------------------------
    bool SetBoolValue(const char* pName, bool newValue)
    {
        auto* pVar = FindByName_(pName);

        // 存在すること、型が一致していること。。。を前提とする。
        NN_SDK_ASSERT_NOT_NULL(pVar);

        if (newValue != pVar->m_Content.m_Value.boolValue)
        {
            pVar->m_Content.m_Value.boolValue = newValue;
            PushModifyEvent_(pVar->m_Name, pVar);

            return true;
        }

        return false;
    }

    //----------------------------------------------------------
    // 使って変更。同時にイベントを飛ばす。
    bool SetFloatValue(const char* pName, float newValue)
    {
        auto* pVar = FindByName_(pName);

        // 存在すること、型が一致していること。。。を前提とする。
        NN_SDK_ASSERT_NOT_NULL(pVar);

        detail::ClampValue(newValue, pVar->m_MinValue, pVar->m_MaxValue);

        if (newValue != pVar->m_Content.m_Value.floatValue)
        {
            pVar->m_Content.m_Value.floatValue = newValue;
            PushModifyEvent_(pVar->m_Name, pVar);
            return true;
        }

        return false;
    }

    //----------------------------------------------------------
    bool GetValueAsBool(const char* pName) const
    {
        auto* pVar = FindRefOnlyByName_(pName);

        // 存在すること、型が一致していること。。。を前提とする。
        NN_SDK_ASSERT_NOT_NULL(pVar);
        NN_SDK_ASSERT(pVar->m_Content.m_Type == StateMachineVariableType_Bool);

        return pVar->m_Content.m_Value.boolValue;
    }

    //----------------------------------------------------------
    float GetValueAsFloat(const char* pName) const
    {
        auto* pVar = FindRefOnlyByName_(pName);

        // 存在すること、型が一致していること。。。を前提とする。
        NN_SDK_ASSERT_NOT_NULL(pVar);
        NN_SDK_ASSERT(pVar->m_Content.m_Type == StateMachineVariableType_Float);

        return pVar->m_Content.m_Value.floatValue;
    }

    //----------------------------------------------------------
    bool ResetToDefalut(const char* pName)
    {
        StateMachineVariable* pVar = FindByName_(pName);
        return ResetToDefalut_(pVar);
    }

private:

    //----------------------------------------------------------
    bool ResetToDefalut_(StateMachineVariable* pVar)
    {
        // 存在すること、型が一致していること。。。を前提とする。
        NN_SDK_ASSERT_NOT_NULL(pVar);
        if (pVar->m_Content.m_Value.floatValue != pVar->m_DefaultValue)
        {
            pVar->m_Content.m_Value.floatValue = pVar->m_DefaultValue;
            return true;
        }

        return false;
    }

    //----------------------------------------------------------
    // StateMachineVariable は外に出さない。すべての編集操作をラップする。
    const StateMachineVariable* FindRefOnlyByName_(const char* pName) const
    {
        for (auto iter = m_StateMachineVariableList.begin(); iter != m_StateMachineVariableList.end(); iter++)
        {
            if (strcmp(iter->m_Name, pName) == 0)
            {
                return &(*iter);
            }
        }

        return nullptr;
    }

    //----------------------------------------------------------
    StateMachineVariable* FindByName_(const char* pName)
    {
        return const_cast<StateMachineVariable*>(FindRefOnlyByName_(pName));
    }

    //----------------------------------------------------------
    void PushModifyEvent_(const char* pName, StateMachineVariable* pVariable)
    {
        NN_SDK_ASSERT_NOT_NULL(m_pEventQueue);

        uint64_t param1 = reinterpret_cast<uint64_t>(pName);
        uint64_t param2 = reinterpret_cast<uint64_t>(pVariable);

        m_pEventQueue->Push(StateMachineEventKind_VariableChanged, param1, param2, 0, 1);
    }

    //----------------------------------------------------------
    StateMachineEventQueue*  m_pEventQueue;
    StateMachineVariableList m_StateMachineVariableList;
};

//----------------------------------------------------------
// 状態遷移条件
struct TransitionConditionBase
{
    //----------------------------------------------------------
    TransitionConditionBase()
        : m_Type(0)
    {
    }

    //----------------------------------------------------------
    virtual ~TransitionConditionBase()
    {
    }

    //----------------------------------------------------------
    // 状態遷移のトリガー条件を満たすか調査します。
    virtual bool IsTriggered(const StateMachineEvent& queue) const = 0;

    //----------------------------------------------------------
    uint8_t  m_Type; // 種類
};

//----------------------------------------------------------
// 状態遷移条件
struct TransitionConditionNone : public TransitionConditionBase
{
    //----------------------------------------------------------
    TransitionConditionNone()
    {
    }

    //----------------------------------------------------------
    virtual ~TransitionConditionNone() NN_OVERRIDE
    {
        TransitionConditionBase::~TransitionConditionBase();
    }

    //----------------------------------------------------------
    virtual bool IsTriggered(const StateMachineEvent& eventObj) const NN_OVERRIDE
    {
        NN_UNUSED(eventObj);
        return true;
    }
};

//----------------------------------------------------------
// 状態遷移条件
struct TransitionConditionStateChangeRequested : public TransitionConditionBase
{
    //----------------------------------------------------------
    explicit TransitionConditionStateChangeRequested(const char* pTargetLayerName, const char* pTargetStateName)
        : m_pTargetLayerName(pTargetLayerName)
        , m_pTargetStateName(pTargetStateName)
    {
    }

    //----------------------------------------------------------
    virtual ~TransitionConditionStateChangeRequested() NN_OVERRIDE
    {
        TransitionConditionBase::~TransitionConditionBase();
    }

    //----------------------------------------------------------
    virtual bool IsTriggered(const StateMachineEvent& eventObj) const NN_OVERRIDE
    {
        if (eventObj.kind == StateMachineEventKind_StateChangeRequested)
        {
            return
                std::strcmp(reinterpret_cast<const char*>(eventObj.param2), m_pTargetLayerName) == 0 &&
                std::strcmp(reinterpret_cast<const char*>(eventObj.param3), m_pTargetStateName) == 0;
        }

        return false;
    }

    const char* m_pTargetLayerName;
    const char* m_pTargetStateName;
};
//----------------------------------------------------------
// 状態遷移条件
struct TransitionConditionIsStateTransitionCompleted : public TransitionConditionBase
{
    //----------------------------------------------------------
    TransitionConditionIsStateTransitionCompleted()
        : m_pLayerName(nullptr)
        , m_pStateName(nullptr)
        , m_pSenderName(nullptr)
    {
    }

    //----------------------------------------------------------
    virtual ~TransitionConditionIsStateTransitionCompleted() NN_OVERRIDE
    {
        TransitionConditionBase::~TransitionConditionBase();
    }

    //----------------------------------------------------------
    virtual bool IsTriggered(const StateMachineEvent& eventObj) const NN_OVERRIDE
    {
        if (eventObj.kind == StateMachineEventKind_StateTransitionCompleted)
        {
            NN_SDK_ASSERT_NOT_NULL(m_pLayerName);
            NN_SDK_ASSERT_NOT_NULL(m_pStateName);

            if (strcmp(m_pLayerName, reinterpret_cast<const char*>(eventObj.param1)) == 0 &&
                strcmp(m_pStateName, reinterpret_cast<const char*>(eventObj.param2)) == 0)
            {
                return true;
            }
        }
        else if (eventObj.kind == StateMachineEventKind_SubStateTransitionCompleted)
        {
            NN_SDK_ASSERT_NOT_NULL(m_pLayerName);
            NN_SDK_ASSERT_NOT_NULL(m_pStateName);
            if (strcmp(m_pLayerName, reinterpret_cast<const char*>(eventObj.param1)) == 0 &&
                strcmp(m_pStateName, reinterpret_cast<const char*>(eventObj.param2)) == 0 &&
                strcmp(m_pSenderName, reinterpret_cast<const char*>(eventObj.param3)) == 0)
            {
                return true;
            }
        }

        return false;
    }

    //----------------------------------------------------------
    const char* m_pLayerName;
    const char* m_pStateName;
    const char* m_pSenderName;
};

//----------------------------------------------------------
// 状態遷移条件
struct TransitionConditionIsHit : public TransitionConditionBase
{
    TransitionConditionIsHit()
    {
    }

    virtual ~TransitionConditionIsHit() NN_OVERRIDE
    {
        TransitionConditionBase::~TransitionConditionBase();
    }

    virtual bool IsTriggered(const StateMachineEvent& eventObj) const NN_OVERRIDE
    {
        if (eventObj.kind == StateMachineEventKind_Hit)
        {
            NN_SDK_ASSERT_NOT_NULL(m_pLayerName);
            if (strcmp(m_pLayerName, reinterpret_cast<const char*>(eventObj.param1)) == 0)
            {
                return true;
            }
        }

        return false;
    }

    const char* m_pLayerName;
};

struct TransitionConditionIsNoHit : public TransitionConditionBase
{
    TransitionConditionIsNoHit()
    {
    }

    virtual ~TransitionConditionIsNoHit() NN_OVERRIDE
    {
        TransitionConditionBase::~TransitionConditionBase();
    }

    virtual bool IsTriggered(const StateMachineEvent& eventObj) const NN_OVERRIDE
    {
        if (eventObj.kind == StateMachineEventKind_NoHit)
        {
            NN_SDK_ASSERT_NOT_NULL(m_pLayerName);
            if (strcmp(m_pLayerName, reinterpret_cast<const char*>(eventObj.param1)) == 0)
            {
                return true;
            }
        }

        return false;
    }

    const char* m_pLayerName;
};

//----------------------------------------------------------
// 状態遷移条件
struct TransitionConditionIsDecided : public TransitionConditionBase
{
    TransitionConditionIsDecided()
    {
    }

    virtual ~TransitionConditionIsDecided() NN_OVERRIDE
    {
        TransitionConditionBase::~TransitionConditionBase();
    }

    virtual bool IsTriggered(const StateMachineEvent& eventObj) const NN_OVERRIDE
    {
        return eventObj.kind == StateMachineEventKind_Decided;
    }
};

//----------------------------------------------------------
// 状態遷移条件：ステートマシン変数の変更
struct TransitionConditionVariableChanged : public TransitionConditionBase
{
    //----------------------------------------------------------
    TransitionConditionVariableChanged()
        : m_StateMachineOperator(StateMachineOperator_GreaterThan)
    {
    }

    //----------------------------------------------------------
    virtual ~TransitionConditionVariableChanged() NN_OVERRIDE
    {
        TransitionConditionBase::~TransitionConditionBase();
    }

    //----------------------------------------------------------
    void Initialzie(const char* pTargetName, StateMachineOperator stateMachineOperator)
    {
        nn::util::Strlcpy(m_Name, pTargetName, sizeof(m_Name));
        m_StateMachineOperator = stateMachineOperator;
    }

    void SetOperatorParameter(StateMachineVariableType type, void* pContent)
    {
        StateMachineVariableContent::Set(&m_Content, type, pContent);
    }

    //----------------------------------------------------------
    virtual bool IsTriggered(const StateMachineEvent& eventObj) const NN_OVERRIDE
    {
        if (eventObj.kind != StateMachineEventKind_VariableChanged)
        {
            return false;
        }

        const char* pName = reinterpret_cast<const char*>(eventObj.param1);
        if (strcmp(pName, m_Name) != 0)
        {
            return false;
        }

        const StateMachineVariable* pVal = reinterpret_cast<const StateMachineVariable*>(eventObj.param2);
        NN_SDK_ASSERT_NOT_NULL(pVal);
        if (!StateMachineVariableContent::GetOperationResult(m_StateMachineOperator, pVal->m_Content, m_Content))
        {
            return false;
        }

        return true;
    }

    //----------------------------------------------------------

    char m_Name[nn::ui2d::ResourceNameStrMax + 1];

    StateMachineOperator                m_StateMachineOperator;
    StateMachineVariableContent         m_Content;
};

//----------------------------------------------------------
// 状態遷移条件：ボタンの決定が行われた
struct TransitionConditionIsButtonDecided : public TransitionConditionBase
{
    TransitionConditionIsButtonDecided()
    {
    }

    virtual ~TransitionConditionIsButtonDecided() NN_OVERRIDE
    {
        TransitionConditionBase::~TransitionConditionBase();
    }

    virtual bool IsTriggered(const StateMachineEvent& eventObj) const NN_OVERRIDE
    {
        return eventObj.kind == StateMachineEventKind_ButtonDecided;
    }
};

//----------------------------------------------------------
struct TransitionTiming
{
    //----------------------------------------------------------
    TransitionTiming()
        : offset(0.0f)
        , duration(0.0f)
        , easingType(0)
        , easingExtraParameter(0.0f)
    {
    }

    //----------------------------------------------------------
    float offset;
    float duration;

    uint8_t easingType;

    float easingExtraParameter;
};

//----------------------------------------------------------
struct TransitionTrackEvent
{
    float                 delay;
    StateMachineEventKind kind;

    const void* pParam1;
    const void* pParam2;
    const void* pParam3;
};

//----------------------------------------------------------
struct TransitionSection
{
    //----------------------------------------------------------
    TransitionSection()
        : isUsedForEvent(false)
    {
    }

    //----------------------------------------------------------
    TransitionTiming            timing;

    union
    {
        // キーフレームとして利用する場合
        FeatureParameterLeafStore   store[4];
        // イベントとして利用する場合
        TransitionTrackEvent        trackEvent;
    };

    bool                        isUsedForEvent;
};

//----------------------------------------------------------
// トラック
class TransitionTimeLineTrack
{
public:
    //----------------------------------------------------------
    TransitionTimeLineTrack()
        : m_TransitionSectionCount(0)
        , m_pTransitionSections(nullptr)
    {
    }

    void Initialize(int kyeCount)
    {
        NN_SDK_ASSERT(m_pTransitionSections == nullptr);
        m_TransitionSectionCount = kyeCount;
        m_pTransitionSections = nn::ui2d::Layout::NewArray<TransitionSection>(kyeCount);
    }

    void Finalize()
    {
        nn::ui2d::Layout::DeleteArray(m_pTransitionSections, m_TransitionSectionCount);
        m_pTransitionSections = nullptr;
        m_TransitionSectionCount = 0;
    }

    int CalculateEventCount() const
    {
        int eventKeyCount = 0;
        for (int sectionIdx = 0; sectionIdx < this->m_TransitionSectionCount; sectionIdx++)
        {
            if (this->m_pTransitionSections[sectionIdx].isUsedForEvent)
            {
                eventKeyCount++;
            }
        }

        return eventKeyCount;
    }

    //----------------------------------------------------------
    int GetSectionIndexFromTime(float time) const
    {
        int idx = -1;
        for (int i = 0; i < this->m_TransitionSectionCount; i++)
        {
            const auto& timing = this->m_pTransitionSections[i].timing;
            if (timing.offset <= time)
            {
                idx = i;
                if (timing.offset + timing.duration > time)
                {
                    break;
                }
            }
        }

        // 終端キーの最後より後ろかどうか
        if (idx != -1 && idx == this->m_TransitionSectionCount - 1)
        {
            if (m_Timing.offset + m_Timing.duration <= time)
            {
                idx++;
            }
        }

        return idx;
    }

    //----------------------------------------------------------
    TransitionSection* GetSectionByIndex(int paramIdx)
    {
        return const_cast<TransitionSection*>(GetSectionByIndex_(paramIdx));
    }

    const TransitionSection* GetSectionByIndex(int paramIdx) const
    {
        return GetSectionByIndex_(paramIdx);
    }

    //----------------------------------------------------------
    void SetupParametrizedAnimationEvent(nn::ui2d::ResParameterizedAnimParameter* pParam, int sectionIdex) const
    {
        if (m_TransitionSectionCount == 0)
        {
            return;
        }

        pParam->parameterizedAnimType = ParameterizedAnimType_EventForStateChange;

        // キーがある場合
        if (sectionIdex < 0)
        {
            // 先頭のキーより前
            pParam->duration = m_pTransitionSections[0].timing.offset - m_Timing.offset;
            pParam->offset = m_Timing.offset;

            pParam->pEvent->pParam1 = nullptr;
            pParam->pEvent->pParam2 = nullptr;
            pParam->pEvent->pParam3 = nullptr;
        }
        else if (sectionIdex >= m_TransitionSectionCount)
        {
            // 最後のキーより後ろ
            pParam->duration = (m_Timing.offset + m_Timing.duration) - m_pTransitionSections[sectionIdex - 1].timing.offset;
            pParam->offset = m_pTransitionSections[sectionIdex - 1].timing.offset;

            pParam->pEvent->pParam1 = nullptr;
            pParam->pEvent->pParam2 = nullptr;
            pParam->pEvent->pParam3 = nullptr;
        }
        else
        {
            // 中間区間
            const TransitionSection* pCurr = &m_pTransitionSections[sectionIdex];
            const TransitionSection* pNext = (sectionIdex + 1 < m_TransitionSectionCount) ? &m_pTransitionSections[sectionIdex + 1] : nullptr;

            const float endFrame = (pNext != nullptr) ? pNext->timing.offset : (m_Timing.offset + m_Timing.duration);

            pParam->duration = endFrame - pCurr->timing.offset;
            pParam->offset = pCurr->timing.offset;

            pParam->pEvent->pParam1 = pCurr->trackEvent.pParam1;
            pParam->pEvent->pParam2 = pCurr->trackEvent.pParam2;
            pParam->pEvent->pParam3 = pCurr->trackEvent.pParam3;

            pParam->parameterizedAnimType = static_cast<uint8_t>(detail::ConvertToParameterizedAnimType(pCurr->trackEvent.kind));
        }
    }

    //----------------------------------------------------------
    void SetupParametrizedAnimation(nn::ui2d::ResParameterizedAnimParameter* pParam, int targetIdx, int sectionIdex) const
    {
        const TransitionTiming*      pTiming = nullptr;
        if (m_TransitionSectionCount == 0)
        {
            // キーが無い場合
            pParam->duration = m_Timing.duration;
            pParam->offset = m_Timing.offset;
            pParam->parameterizedAnimType = m_Timing.easingType;

            pTiming = &m_Timing;
        }
        else
        {
            // キーがある場合
            if (sectionIdex < 0)
            {
                // 先頭のキーより前
                pParam->duration = m_pTransitionSections[0].timing.offset - m_Timing.offset;
                pParam->offset = m_Timing.offset;
                pParam->parameterizedAnimType = m_Timing.easingType;

                // pParam->startValue は、Transition の Start を採用
                pParam->value.targetValue = m_pTransitionSections[0].store[targetIdx].paramValue;

                pTiming = &m_Timing;
            }
            else if (sectionIdex >= m_TransitionSectionCount)
            {
                // 最後のキーより後ろ
                pParam->duration = (m_Timing.offset + m_Timing.duration) - m_pTransitionSections[sectionIdex - 1].timing.offset;
                pParam->offset = m_pTransitionSections[sectionIdex - 1].timing.offset;
                pParam->parameterizedAnimType = m_pTransitionSections[sectionIdex - 1].timing.easingType;

                pParam->value.startValue = m_pTransitionSections[sectionIdex - 1].store[targetIdx].paramValue;
                // pParam->targetValue は、Transition の Traget を採用

                pTiming = &m_pTransitionSections[sectionIdex - 1].timing;
            }
            else
            {
                // 中間区間
                const TransitionSection* pCurr = &m_pTransitionSections[sectionIdex];
                const TransitionSection* pNext = (sectionIdex + 1 < m_TransitionSectionCount) ? &m_pTransitionSections[sectionIdex + 1] : nullptr;

                const float endFrame = (pNext != nullptr) ? pNext->timing.offset : (m_Timing.offset + m_Timing.duration);

                pParam->duration = endFrame - pCurr->timing.offset;
                pParam->offset = pCurr->timing.offset;
                pParam->parameterizedAnimType = pCurr->timing.easingType;

                pParam->value.startValue = pCurr->store[targetIdx].paramValue;
                if (pNext != nullptr)
                {
                    pParam->value.targetValue = pNext->store[targetIdx].paramValue;
                }

                pTiming = &pCurr->timing;
            }
        }

        NN_SDK_ASSERT_NOT_NULL(pTiming);
        if (IsUseExtraParameterParameterizedAnimType(static_cast<ParameterizedAnimType>(pTiming->easingType)))
        {
            pParam->value.targetValue = pTiming->easingExtraParameter;
        }
    }

    //----------------------------------------------------------
    void SetTimeing(float offset, float duration, uint8_t easingType, float easingExtraParameter)
    {
        m_Timing.offset = offset;
        m_Timing.duration = duration;
        m_Timing.easingType = easingType;
        m_Timing.easingExtraParameter = easingExtraParameter;
    }

    //----------------------------------------------------------
    int GetTransitionSectionCount() const
    {
        return m_TransitionSectionCount;
    }

private:

    const TransitionSection* GetSectionByIndex_(int paramIdx) const
    {
        if (paramIdx == -1 || this->m_TransitionSectionCount <= paramIdx)
        {
            return nullptr;
        }

        return &m_pTransitionSections[paramIdx];
    }

    //----------------------------------------------------------
    TransitionTiming      m_Timing;
    int                   m_TransitionSectionCount;
    TransitionSection*    m_pTransitionSections;
};


//----------------------------------------------------------
// パラメーター毎に個別のタイミングで状態遷移する
class TransitionTimeLine
{
public:
    //----------------------------------------------------------
    TransitionTimeLine()
        : m_TotalLength(0.0f)
        , m_TransitionTimeLineTrackCount(0)
        , m_pTransitionTimeLineTracks(nullptr)
    {
    }

    //----------------------------------------------------------
    ~TransitionTimeLine()
    {
    }

    //----------------------------------------------------------
    float GetTotalLength() const
    {
        return m_TotalLength;
    }

    void SetTotalLength(float totalLength)
    {
        m_TotalLength = totalLength;
    }

    void SetupParametrizedAnimationFromTime(nn::ui2d::ResParameterizedAnimParameter* pParam,
        int contentIdx,
        int infoIdx,
        int targetIdx,
        float time,
        const FeatureParameterStoreSet& paramStoreSetStart, const FeatureParameterStoreSet& paramStoreSetTarget) const
    {
        SetupParametrizedAnimation(pParam, contentIdx, infoIdx, targetIdx, GetSectionIndexFromTime(contentIdx, time), paramStoreSetStart, paramStoreSetTarget);
    }

    //----------------------------------------------------------
    void Initialize(int trackCount)
    {
        NN_SDK_ASSERT(m_pTransitionTimeLineTracks == nullptr);

        m_TransitionTimeLineTrackCount = trackCount;
        m_pTransitionTimeLineTracks = nn::ui2d::Layout::NewArray<TransitionTimeLineTrack>(trackCount);
    }

    //----------------------------------------------------------
    void Finalize()
    {
        if (m_pTransitionTimeLineTracks != nullptr)
        {
            for (int i = 0; i < m_TransitionTimeLineTrackCount; i++)
            {
                m_pTransitionTimeLineTracks[i].Finalize();
            }
        }

        nn::ui2d::Layout::DeleteArray(m_pTransitionTimeLineTracks, m_TransitionTimeLineTrackCount);
        m_pTransitionTimeLineTracks = nullptr;
        m_TransitionTimeLineTrackCount = 0;
    }

    //----------------------------------------------------------
    const TransitionTimeLineTrack* GetTrackByFeatureParameterIndex(int targetIdx) const
    {
        return GetTrackByFeatureParameterIndex_(targetIdx);
    }

    TransitionTimeLineTrack* GetTrackByFeatureParameterIndex(int targetIdx)
    {
        return const_cast<TransitionTimeLineTrack*>(GetTrackByFeatureParameterIndex_(targetIdx));
    }

    //----------------------------------------------------------
    int GetSectionIndexFromTime(int contentIdx, float time) const
    {
        const TransitionTimeLineTrack* pTrack = GetTrackByFeatureParameterIndex(contentIdx);
        if (pTrack == nullptr)
        {
            return -1;
        }

        return pTrack->GetSectionIndexFromTime(time);
    }

    //----------------------------------------------------------
    bool SetupParametrizedAnimation(nn::ui2d::ResParameterizedAnimParameter* pParam,
        int contentIdx,
        int infoIdx,
        int targetIdx,
        int paramIdx,
        const FeatureParameterStoreSet& paramStoreSetStart, const FeatureParameterStoreSet& paramStoreSetTarget) const
    {
        const TransitionTimeLineTrack* pTrack = GetTrackByFeatureParameterIndex(contentIdx);
        pParam->value.startValue = paramStoreSetStart.GetAt(contentIdx, infoIdx, targetIdx)->paramValue;
        pParam->value.targetValue = paramStoreSetTarget.GetAt(contentIdx, infoIdx, targetIdx)->paramValue;
        if (pTrack != nullptr)
        {
            pTrack->SetupParametrizedAnimation(pParam, targetIdx, paramIdx);
        }

        return true;
    }

    //----------------------------------------------------------
    void SetupParametrizedAnimationEvent(nn::ui2d::ResParameterizedAnimParameter* pParam, int contentIdx, int paramIdx) const
    {
        const TransitionTimeLineTrack* pTrack = GetTrackByFeatureParameterIndex(contentIdx);
        if (pTrack != nullptr)
        {
            pTrack->SetupParametrizedAnimationEvent(pParam, paramIdx);
        }
    }

private:

    //----------------------------------------------------------
    const TransitionTimeLineTrack* GetTrackByFeatureParameterIndex_(int targetIdx) const
    {
        NN_SDK_ASSERT(targetIdx >= 0);

        if (m_TransitionTimeLineTrackCount <= targetIdx)
        {
            return nullptr;
        }

        return &m_pTransitionTimeLineTracks[targetIdx];
    }

    //----------------------------------------------------------
    float                    m_TotalLength;
    //----------------------------------------------------------
    int                      m_TransitionTimeLineTrackCount;
    TransitionTimeLineTrack* m_pTransitionTimeLineTracks;
};

//----------------------------------------------------------
// 状態遷移
class Transition
{
public:
    //----------------------------------------------------------
    Transition()
        : m_pPrevStateName(nullptr)
        , m_pNextStateName(nullptr)
        , m_IsCancelable(false)
        , m_IsLoop(false)
        , m_pCondition(nullptr)
        , m_pTimeLine(nullptr)
    {
    }

    //----------------------------------------------------------
    ~Transition()
    {
    }

    //----------------------------------------------------------
    void Initialzie(const char* pPrev, const char* pNext, bool isCancelable, bool isLoop)
    {
        {
            const int size = nn::util::Strnlen(pPrev, CountOfStateMachineName) + 1;
            auto pBuf = nn::ui2d::Layout::AllocateMemory(size);
            nn::util::Strlcpy(static_cast<char*>(pBuf), pPrev, size);
            m_pPrevStateName = static_cast<const char*>(pBuf);
        }

        {
            const int size = nn::util::Strnlen(pNext, CountOfStateMachineName) + 1;
            auto pBuf = nn::ui2d::Layout::AllocateMemory(size);
            nn::util::Strlcpy(static_cast<char*>(pBuf), pNext, size);
            m_pNextStateName = static_cast<const char*>(pBuf);
        }
        m_IsCancelable = isCancelable;
        m_IsLoop = isLoop;
    }

    //----------------------------------------------------------
    void Finalize()
    {
        nn::ui2d::Layout::FreeMemory(const_cast<void*>(static_cast<const void*>(m_pPrevStateName)));
        m_pPrevStateName = nullptr;
        nn::ui2d::Layout::FreeMemory(const_cast<void*>(static_cast<const void*>(m_pNextStateName)));
        m_pNextStateName = nullptr;

        nn::ui2d::Layout::DeleteObj(m_pCondition);
        m_pCondition = nullptr;

        m_pTimeLine->Finalize();
        nn::ui2d::Layout::DeleteObj(m_pTimeLine);
        m_pTimeLine = nullptr;
    }

    //----------------------------------------------------------
    // Builder が構築用に使う関数
    void SetCondition(TransitionConditionBase* pCondition)
    {
        NN_SDK_ASSERT_NOT_NULL(pCondition);
        NN_SDK_ASSERT(m_pCondition == nullptr);

        m_pCondition = pCondition;
    }

    const TransitionConditionBase* GetCondition() const
    {
        return m_pCondition;
    }

    //----------------------------------------------------------
    // Builder が構築用に使う関数
    void SetTimeline(TransitionTimeLine* pTimeLine)
    {
        NN_SDK_ASSERT_NOT_NULL(pTimeLine);
        NN_SDK_ASSERT(m_pTimeLine == nullptr);

        m_pTimeLine = pTimeLine;
    }

    const TransitionTimeLine* GetTimeline() const
    {
        return m_pTimeLine;
    }

    //----------------------------------------------------------
    bool IsCancelable() const
    {
        return m_IsCancelable;
    }

    void SetCancelable(bool flag)
    {
        m_IsCancelable = flag;
    }

    //----------------------------------------------------------
    const char* GetPrevStateName() const
    {
        return m_pPrevStateName;
    }

    void SetPrevStateName(const char* pName)
    {
        m_pPrevStateName = pName;
    }

    //----------------------------------------------------------
    const char* GetNextStateName() const
    {
        return m_pNextStateName;
    }

    void SetNextStateName(const char* pName)
    {
        m_pNextStateName = pName;
    }

    //----------------------------------------------------------
    bool IsLoop() const
    {
        return m_IsLoop;
    }

    void SetLoop(bool flag)
    {
        m_IsLoop = flag;
    }
    //----------------------------------------------------------

    nn::util::IntrusiveListNode m_Link;

private:

    // 遷移元
    const char* m_pPrevStateName;
    // 遷移先
    const char* m_pNextStateName;

    // 状態遷移をキャンセル可能かどうか
    bool m_IsCancelable;

    // ループする状態遷移かどうか？ ループ時は、強制的にキャンセル可能になる。
    bool m_IsLoop;

    // 条件
    TransitionConditionBase* m_pCondition;

    // 変化タイミングの情報
    TransitionTimeLine* m_pTimeLine;
};

//----------------------------------------------------------
// アニメーターを管理するもの
class AnimatorSlot
{
public:
    //----------------------------------------------------------
    AnimatorSlot()
        : m_pGroup(nullptr)
        , m_pLayout(nullptr)
        , m_pAnimResFile(nullptr)
        , m_pAnimator(nullptr)
    {
    }

    //----------------------------------------------------------
    void Initialize(nn::ui2d::Layout* pLayout, const FeatureParameterSet& featureParameterSet)
    {
        m_pLayout = pLayout;

        if (featureParameterSet.size() <= 0)
        {
            return;
        }

        // m_Group
        {
            NN_SDK_ASSERT(featureParameterSet.size() > 0);
            m_pGroup = nn::ui2d::Layout::AllocateAndConstruct<Group>();
            for (auto iter = featureParameterSet.begin(); iter != featureParameterSet.end(); iter++)
            {
                auto pPane = pLayout->GetRootPane()->FindPaneByName(iter->m_pName);
                NN_SDK_ASSERT_NOT_NULL(pPane);
                m_pGroup->AppendPane(pPane);
            }
        }
    }

    //----------------------------------------------------------
    void Finalzie()
    {
        // m_Group
        {
            nn::ui2d::Layout::DeleteObj(m_pGroup);
            m_pGroup = nullptr;
        }

        m_pLayout = nullptr;
    }

    //----------------------------------------------------------
    void Bind(nn::gfx::Device* pDevice, void* pAnimResFile)
    {
        NN_SDK_ASSERT(m_pAnimator == nullptr);
        NN_SDK_ASSERT(m_pAnimResFile == nullptr);

        NN_SDK_ASSERT(m_pLayout != nullptr);

        m_pAnimResFile = pAnimResFile;

        if (m_pGroup == nullptr)
        {
            return;
        }

        nn::ui2d::GroupAnimator* pPaneAnimatior = m_pLayout->CreateAnimTransform<nn::ui2d::GroupAnimator>(pDevice, pAnimResFile);
        pPaneAnimatior->Setup(m_pGroup, true);
        pPaneAnimatior->PlayAuto(1.0f);

        m_pAnimator = pPaneAnimatior;
    }

    //----------------------------------------------------------
    void* Unbind()
    {
        if (m_pAnimator != nullptr)
        {
            m_pAnimator->UnbindAll();
            m_pLayout->DeleteAnimTransform(m_pAnimator);

            m_pAnimator = nullptr;
        }

        void* pCurrentRes = m_pAnimResFile;
        m_pAnimResFile = nullptr;

        return pCurrentRes;
    }

    //----------------------------------------------------------
    nn::ui2d::Animator* GetAnimatior()
    {
        return m_pAnimator;
    }

    const nn::ui2d::Animator* GetAnimatior() const
    {
        return m_pAnimator;
    }

    //----------------------------------------------------------
    bool IsStateTransitionCompleted() const
    {
        if (GetAnimatior() == nullptr)
        {
            return true;
        }

        if (!GetAnimatior()->IsEnabled())
        {
            return true;
        }

        if (GetAnimatior()->IsEndFrame())
        {
            return true;
        }

        return false;
    }

    //----------------------------------------------------------
    const void* GetAnimResource() const
    {
        return m_pAnimResFile;
    }

    //----------------------------------------------------------
    const float GetAnimationFrame() const
    {
        if (m_pAnimator == nullptr)
        {
            return 0.0f;
        }

        return m_pAnimator->GetFrame();
    }

    void SetAnimationFrame(float frame)
    {
        if (m_pAnimator != nullptr)
        {
            m_pAnimator->StopAt(frame);
        }
    }

    //----------------------------------------------------------
    const float GetAnimationFrameMax() const
    {
        if (m_pAnimator == nullptr)
        {
            return 0.0f;
        }

        return m_pAnimator->GetFrameMax();
    }

private:

    //----------------------------------------------------------
    nn::ui2d::Group*            m_pGroup;
    nn::ui2d::Layout*           m_pLayout;
    void*                       m_pAnimResFile;
    nn::ui2d::Animator*         m_pAnimator;// アニメーター
};

typedef nn::util::IntrusiveList<Transition, nn::util::IntrusiveListMemberNodeTraits<Transition, &Transition::m_Link> > TransitionSet;

//----------------------------------------------------------
// ステートレイヤー
// 共通の「特徴」を持つステートの集まり
class StateLayer
{
public:
    StateLayer()
        : m_pLayout(nullptr)
        , m_pName(nullptr)
        , m_pCurrentState(nullptr)
        , m_pActiveTransition(nullptr)
        , m_CurrentTempTransitionIndex(0)
        , m_IsCurrentTransitionCancelable(false)
        , m_LastIsStateTransitionCompleted(false)
        , m_IsStateTransitionJustCompleted(false)
        , m_pHitPane(nullptr)
        , m_LayerType(StateMachineLayerType_Normal)
    {
        ResetStateFlags_();

        m_TempTransitionForDirectStateChange[0].SetCancelable(true);
        m_TempTransitionForDirectStateChange[1].SetCancelable(true);
    }

    //----------------------------------------------------------
    bool IsInitialized() const
    {
        if (m_pLayout == nullptr)
        {
            return false;
        }

        if (m_pCurrentState == nullptr)
        {
            return false;
        }

        return true;
    }

    //----------------------------------------------------------
    // 初期化
    void Initialize(nn::gfx::Device* pDevice, nn::ui2d::Layout* pLayout, const char* pName)
    {
        NN_UNUSED(pDevice);

        if (!IsAllTargetFound_(pLayout))
        {
            return;
        }

        m_pLayout = pLayout;

        const int size = nn::util::Strnlen(pName, CountOfStateMachineName) + 1;
        auto pBuf = static_cast<char*>( nn::ui2d::Layout::AllocateMemory(size));
        nn::util::Strlcpy(pBuf, pName, size);
        m_pName = static_cast<const char*>(pBuf);

        m_AnimatorSlot.Initialize(pLayout, m_FeatureParameterSet);
        m_pCurrentState = nullptr;
        ResetStateFlags_();

        m_FeatureParameterStoreSetOfStartValue.Initialzie(m_FeatureParameterSet);
    }

    //----------------------------------------------------------
    // 終了
    void Finalize()
    {
        nn::ui2d::Layout::FreeMemory(const_cast<void*>(static_cast<const void*>(m_pName)));
        m_pName = nullptr;

        m_pCurrentState = nullptr;
        ResetStateFlags_();

        m_FeatureParameterStoreSetOfStartValue.Finalize();

        // FeatureParameterSet
        {
            auto endIter = m_FeatureParameterSet.end();
            for (auto iter = m_FeatureParameterSet.begin(); iter != endIter; )
            {
                auto currIter = iter;
                iter++;

                currIter->Finalize();
                m_FeatureParameterSet.erase(currIter);
                nn::ui2d::Layout::DeleteObj(&(*currIter));
            }
        }

        // m_TransitionSet
        {
            auto endIter = m_TransitionSet.end();
            for (auto iter = m_TransitionSet.begin(); iter != endIter; )
            {
                auto currIter = iter;
                iter++;

                currIter->Finalize();
                m_TransitionSet.erase(currIter);
                nn::ui2d::Layout::DeleteObj(&(*currIter));
            }
        }

        // State
        for (auto iter = m_StateSet.begin(); iter != m_StateSet.end(); )
        {
            auto currIter = iter;
            iter++;

            currIter->Finalize();
            m_StateSet.erase(currIter);
            nn::ui2d::Layout::DeleteObj(&(*currIter));
        }

        UnbindAnimatorSlot_();

        m_AnimatorSlot.Finalzie();
    }

    //----------------------------------------------------------
    State* FindStateByName(const char* pName)
    {
        auto endIter = m_StateSet.end();
        for (auto iter = m_StateSet.begin(); iter != endIter; iter++)
        {
            if (strcmp(iter->GetName(), pName) == 0)
            {
                return &(*iter);
            }
        }

        return nullptr;
    }

    //----------------------------------------------------------
    State* FindStateByIndex(int index)
    {
        auto endIter = m_StateSet.end();
        int stateIndex = 0;
        for (auto iter = m_StateSet.begin(); iter != endIter; iter++)
        {
            if (stateIndex == index)
            {
                return &(*iter);
            }
            stateIndex++;
        }

        return nullptr;
    }

    //----------------------------------------------------------
    const State* FindStateByName(const char* pName) const
    {
        auto endIter = m_StateSet.end();
        for (auto iter = m_StateSet.begin(); iter != endIter; iter++)
        {
            if (strcmp(iter->GetName(), pName) == 0)
            {
                return &(*iter);
            }
        }

        return nullptr;
    }

    //----------------------------------------------------------
    int FindFeatureParameterByName(const char* pName, StateMachineFeatureParameterKind parameterKind) const
    {
        int count = 0;
        auto endIter = m_FeatureParameterSet.end();
        for (auto iter = m_FeatureParameterSet.begin(); iter != endIter; iter++)
        {
            if (strcmp(iter->m_pName, pName) == 0)
            {
                if (iter->m_parameterKind == parameterKind)
                {
                    return count;
                }
            }
            count++;
        }

        return -1;
    }

    //----------------------------------------------------------
    const Transition* FindTransitionByName(const char* pStartStateName, const char* pEndStateName) const
    {
        for (auto iter = this->m_TransitionSet.begin(); iter != this->m_TransitionSet.end(); iter++)
        {
            // 現在のステートを対象としているトリガーかどうか？
            if (strcmp(iter->GetPrevStateName(), pStartStateName) == 0 &&
                strcmp(iter->GetNextStateName(), pEndStateName) == 0)
            {
                return &(*iter);
            }
        }

        return nullptr;
    }

    //----------------------------------------------------------
    const FeatureParameterSet& GetFeatureParameterSet() const
    {
        return m_FeatureParameterSet;
    }

    //----------------------------------------------------------
    const StateSet& GetStateSet() const
    {
        return m_StateSet;
    }

    //----------------------------------------------------------
    const TransitionSet& GetTransitionSet() const
    {
        return m_TransitionSet;
    }

    //----------------------------------------------------------
    State* GetBaseState()
    {
        return &(*m_StateSet.begin());
    }

    const State* GetBaseState() const
    {
        return &(*m_StateSet.begin());
    }

    //----------------------------------------------------------
    State* FindInState()
    {
        return this->FindStateByName("In");
    }

    const State* FindInState() const
    {
        return this->FindStateByName("In");
    }

    //----------------------------------------------------------
    State* FindOutState()
    {
        return this->FindStateByName("Out");
    }

    const State* FindOutState() const
    {
        return this->FindStateByName("Out");
    }

    //----------------------------------------------------------
    State* FindDecideState()
    {
        return this->FindStateByName("Decide");
    }

    const State* FindDecideState() const
    {
        return this->FindStateByName("Decide");
    }

    //----------------------------------------------------------
    void RestoreToInitialState()
    {
        State* pInitialState = this->FindInState();
        if (pInitialState == nullptr)
        {
            pInitialState = this->GetBaseState();
        }
        else {
            pInitialState = this->FindInState();
        }
        NN_SDK_ASSERT_NOT_NULL(pInitialState);
        this->RestoreToState(pInitialState);

        ResetStateFlags_();
        UnbindAnimatorSlot_();
    }

    //----------------------------------------------------------
    void RestoreToState(State* pState)
    {
        m_pCurrentState = &(*pState);
        ApplyFeatureParameterToTargetAll_(pState);
    }

    //----------------------------------------------------------
    void ForceChangeCurrentState(nn::gfx::Device* pDevice, Transition& transition)
    {
        ChangeCurrentState_(pDevice, transition);
    }

    //----------------------------------------------------------
    const State* GetCurrentState() const
    {
        return m_pCurrentState;
    }

    //----------------------------------------------------------
    const char* GetName() const
    {
        return m_pName;
    }

    //----------------------------------------------------------
    float GetCurrentFrame() const
    {
        return m_AnimatorSlot.GetAnimationFrame();
    }

    float GetFrameMax() const
    {
        return m_AnimatorSlot.GetAnimationFrameMax();
    }

    //----------------------------------------------------------
    FeatureParameterStoreSet& GetCurrentFeatureParameterStore()
    {
        return m_FeatureParameterStoreSetOfStartValue;
    }

    const FeatureParameterStoreSet& GetCurrentFeatureParameterStore() const
    {
        return m_FeatureParameterStoreSetOfStartValue;
    }

    //----------------------------------------------------------
    bool HasHit() const
    {
        return m_pHitPane != nullptr;
    }

    bool IsHit(const nn::util::Float2& vec2) const
    {
        if (HasHit()) {
            return (m_HitBoxBottomLeft.v[0] <= vec2.v[0] && vec2.v[0] <= m_HitBoxTopRight.v[0] &&
                m_HitBoxBottomLeft.v[1] <= vec2.v[1] && vec2.v[1] <= m_HitBoxTopRight.v[1]);
        }
        else {
            return false;
        }
    }

    //----------------------------------------------------------

    StateMachineLayerType GetLayerType() const
    {
        return m_LayerType;
    }

    void SetLayerType(StateMachineLayerType type)
    {
        m_LayerType = type;
    }

    //----------------------------------------------------------
    const Pane* GetHitPane() const
    {
        return m_pHitPane;
    }

    //----------------------------------------------------------
    bool IsStateTransitionCompleted() const
    {
        if (m_pActiveTransition == nullptr)
        {
            return true;
        }

        return m_AnimatorSlot.IsStateTransitionCompleted();
    }

    //----------------------------------------------------------
    bool IsStateTransitionJustCompleted() const
    {
        return m_IsStateTransitionJustCompleted;
    }

    //----------------------------------------------------------
    const Transition* GetActiveTransition() const
    {
        if (IsStateTransitionCompleted())
        {
            return nullptr;
        }

        return m_pActiveTransition;
    }

    //----------------------------------------------------------
    void Update()
    {
        // TransitionCompleted 関連
        {
            bool current = IsStateTransitionCompleted();
            m_IsStateTransitionJustCompleted = m_LastIsStateTransitionCompleted != current;
            m_LastIsStateTransitionCompleted = current;
        }
        UpdateHitBox_();
    }

    //----------------------------------------------------------
    void UpdateStateLayerTransitions(nn::gfx::Device* pDevice, const StateMachineEvent& eventMsg);

    //----------------------------------------------------------
    // ビルダーが使う構築用
    void PushBackNewFeatureParameter(FeatureParameter* pNewParam)
    {
        m_FeatureParameterSet.push_back(*pNewParam);
    }

    //----------------------------------------------------------
    // ビルダーが使う構築用
    void PushBackNewState(State* pNewState)
    {
        m_StateSet.push_back(*pNewState);
    }

    //----------------------------------------------------------
    // ビルダーが使う構築用
    void PushBackNewTransition(Transition* pNewTransition)
    {
        m_TransitionSet.push_back(*pNewTransition);
    }

    //----------------------------------------------------------
    // ビルダーが使う構築用
    void SetHitPane(Pane* pHitPane)
    {
        m_pHitPane = pHitPane;
    }

    //----------------------------------------------------------
    // アニメーションリソースの構築処理
    static size_t CalculateAnimationResourceSize(const StateLayer& stateLayer, const Transition& prevTransition);
    static void BuildAnimationResource(void* pOutBuffer, size_t outBufferSize, const StateLayer& stateLayer, const Transition* pPrevTransition, const Transition* pNextTansition, float prevTransitionFrame);

    //----------------------------------------------------------
    nn::util::IntrusiveListNode m_Link;

private:

    //----------------------------------------------------------
    Transition* GetCurrentTransitionForDirectStateChange_()
    {
        return &m_TempTransitionForDirectStateChange[m_CurrentTempTransitionIndex];
    }

    //----------------------------------------------------------
    void ChangeCurrentState_(nn::gfx::Device* pDevice, const Transition& transition)
    {
        // ステート変更を行う
        auto pNextState = FindStateByName(transition.GetNextStateName());
        NN_SDK_ASSERT(pNextState != nullptr);
        NN_SDK_ASSERT_NOT_NULL(transition.GetPrevStateName());
        NN_SDK_ASSERT(FindStateByName(transition.GetPrevStateName()) != nullptr);

        m_pCurrentState = pNextState;

        ResetStateFlags_();
        m_IsCurrentTransitionCancelable = transition.IsCancelable() || transition.IsLoop();

        // 動的に構築する
        {
            const Transition* pPrevTransition = m_pActiveTransition;
            const float prevAnimFrame = m_AnimatorSlot.GetAnimationFrame();

            UnbindAnimatorSlot_();

            BindSlot_(pDevice, &transition, pPrevTransition, prevAnimFrame);
        }
    }

    //----------------------------------------------------------
    void UpdateHitBox_()
    {
        if (m_pHitPane == nullptr) {
            return;
        }

        // グローバル行列からスケールを取得して大きさを計算する。
        nn::util::Vector3f axisX;
        nn::util::Vector3f axisY;
        nn::util::Vector3f axisZ;
        nn::util::Vector3f axisW;
        nn::util::MatrixGetAxes(&axisX, &axisY, &axisZ, &axisW, m_pHitPane->GetGlobalMtx());
        float half_width = std::abs(m_pHitPane->GetSize().width * axisX.GetX()) / 2.f;
        float half_height = std::abs(m_pHitPane->GetSize().height * axisY.GetY()) / 2.f;
        nn::util::Float2 center = NN_UTIL_FLOAT_2_INITIALIZER(axisW.GetX(), axisW.GetY());

        switch (m_pHitPane->GetBasePositionX()) {
        case nn::ui2d::HorizontalPosition_Center:
            break;
        case nn::ui2d::HorizontalPosition_Left:
            center.v[0] += half_width;
            break;
        case nn::ui2d::HorizontalPosition_Right:
            center.v[0] -= half_width;
            break;
        default:
            NN_SDK_ASSERT(false, "Unknown HorizontalPosition type.");
            break;
        }

        switch (m_pHitPane->GetBasePositionY()) {
        case nn::ui2d::VerticalPosition_Center:
            break;
        case nn::ui2d::VerticalPosition_Top:
            center.v[1] -= half_height;
            break;
        case nn::ui2d::VerticalPosition_Bottom:
            center.v[1] += half_height;
            break;
        default:
            NN_SDK_ASSERT(false, "Unknown VerticalPosition type.");
            break;
        }

        m_HitBoxBottomLeft.v[0] = center.v[0] - half_width;
        m_HitBoxBottomLeft.v[1] = center.v[1] - half_height;
        m_HitBoxTopRight.v[0] = center.v[0] + half_width;
        m_HitBoxTopRight.v[1] = center.v[1] + half_height;
    }

    //----------------------------------------------------------
    void ApplyFeatureParameterToPaneSrt_(Pane* pPane, const FeatureParameter& featureParameter, const FeatureParameterStore& featureParameterStore, const int InfoIdx);

    //----------------------------------------------------------
    void ApplyFeatureParameterToPaneVisilility_(Pane* pPane, const FeatureParameter& featureParameter, const FeatureParameterStore& featureParameterStore, const int InfoIdx);

    //----------------------------------------------------------
    void ApplyFeatureParameterToPaneTransparancy_(Pane* pPane, const FeatureParameter& featureParameter, const FeatureParameterStore& featureParameterStore, const int InfoIdx);
    //----------------------------------------------------------
    void ApplyFeatureParameterToPaneRoundRect_(Pane* pPane, const FeatureParameter& featureParameter, const FeatureParameterStore& featureParameterStore, const int InfoIdx);
    //----------------------------------------------------------
    void ApplyFeatureParameterToPanePerCharacterTransform_(Pane* pPane, const FeatureParameter& featureParameter, const FeatureParameterStore& featureParameterStore, const int InfoIdx);

    //----------------------------------------------------------
    void ApplyFeatureParameterToPaneMaskTexSRT_(Pane* pPane, const FeatureParameter& featureParameter, const FeatureParameterStore& featureParameterStore, const int InfoIdx);

    //----------------------------------------------------------
    void ApplyFeatureParameterToMaterialColor_(Material* pMaterial, const FeatureParameter& featureParameter, const FeatureParameterStore& featureParameterStore, const int InfoIdx);

    //----------------------------------------------------------
    void ApplyFeatureParameterToTextureMatrix_(Material* pMaterial, const FeatureParameter& featureParameter, const FeatureParameterStore& featureParameterStore, const int InfoIdx);

    //----------------------------------------------------------
    void ApplyFeatureParameterToTarget_(const FeatureParameter& featureParameter, const FeatureParameterStore& featureParameterStore);

    //----------------------------------------------------------
    void ApplyFeatureParameterToTargetAll_(const State* pState);

    //----------------------------------------------------------
    bool IsAllTargetFound_(const nn::ui2d::Layout* pLayout) const
    {
        for (auto iter = m_FeatureParameterSet.begin(); iter != m_FeatureParameterSet.end(); iter++)
        {
            // 初期化失敗
            if (pLayout->GetRootPane()->FindPaneByName(iter->m_pName) == nullptr)
            {
                return false;
            }
        }

return true;
    }

    //----------------------------------------------------------
    void ResetStateFlags_()
    {
        m_IsStateTransitionJustCompleted = false;
        m_LastIsStateTransitionCompleted = false;
    }

    //----------------------------------------------------------
    void BindSlot_(nn::gfx::Device* pDevice, const Transition* pNextTransition, const Transition* pPrevTransition, float prevAnimFrame)
    {
        const size_t resourceSize = CalculateAnimationResourceSize(*this, *pNextTransition);
        void* pBuff = nn::ui2d::Layout::AllocateMemory(resourceSize);

        BuildAnimationResource(pBuff, resourceSize, *this, pNextTransition, pPrevTransition, prevAnimFrame);

        m_AnimatorSlot.Bind(pDevice, pBuff);

        m_pActiveTransition = pNextTransition;
    }

    //----------------------------------------------------------
    void UnbindAnimatorSlot_()
    {
        auto pCurrentRes = m_AnimatorSlot.Unbind();
        if (pCurrentRes != nullptr)
        {
            nn::ui2d::Layout::FreeMemory(const_cast<void*>(pCurrentRes));
        }

        m_pActiveTransition = nullptr;
    }

    //----------------------------------------------------------
    nn::ui2d::Layout*    m_pLayout;
    const char*          m_pName;
    State*               m_pCurrentState;

    StateSet             m_StateSet;
    FeatureParameterSet  m_FeatureParameterSet;

    AnimatorSlot         m_AnimatorSlot;

    TransitionSet        m_TransitionSet;
    const Transition*    m_pActiveTransition;

    // DirectState指定で利用する Transition
    Transition           m_TempTransitionForDirectStateChange[2];
    int                  m_CurrentTempTransitionIndex;

    bool                 m_IsCurrentTransitionCancelable;
    bool                 m_LastIsStateTransitionCompleted;
    bool                 m_IsStateTransitionJustCompleted;
    nn::ui2d::Pane*      m_pHitPane;
    nn::util::Float2     m_HitBoxBottomLeft;
    nn::util::Float2     m_HitBoxTopRight;

    StateMachineLayerType m_LayerType;

    // カスタムのスタート値（遷移の途中で、遷移した場合に更新される） 現在のTransition に切り替わった時の、値。
    FeatureParameterStoreSet m_FeatureParameterStoreSetOfStartValue;
};

// ステートレイヤー（の束）
typedef nn::util::IntrusiveList<StateLayer, nn::util::IntrusiveListMemberNodeTraits<StateLayer, &StateLayer::m_Link> > StateLayerSet;

class StateMachine;
//----------------------------------------------------------
// ハンドラーベースクラス。
// プログラマーが完全に独自の処理を記述することもできます。
class StateMachineEventHandler
{
    //----------------------------------------------------------
protected:
    StateMachine*   m_pOwnerStateMachine;

public:
    //----------------------------------------------------------

    NN_IMPLICIT StateMachineEventHandler(StateMachine* pOwner)
    {
        m_pOwnerStateMachine = pOwner;
    }

    virtual ~StateMachineEventHandler()
    {
    }

    // 生のイベント完了通知をハンドルするメソッド。
    // 念のため用意しているが、より抽象度の高い、 OnStateChangeCompleted を主に利用する想定です。
    virtual void OnStateChangeCompletedRaw(const char* pStateMachineName, const char* pLayerName, const char* pStateName);

    // イベント完了通知をハンドルするメソッド。
    // 特定の予約名のイベントは、フレームワーク上で特別な意味を持って扱うようにする。
    // プログラマはより抽象度の高い形でUIデータとやり取りを行います。
    virtual void OnStateChangeCompleted(const char* pStateMachineName, StateMachineUiEventKind uiEvent);

    // ステート属性変更をハンドルするメソッドです。
    virtual void OnVariablesChanged(const char* pStateMachineName, const char* pVarName, float oldValue, float newValue);

    static StateMachineUiEventKind GetStateMachineUIEventFromStateName(const char* pStateName)
    {
        if (strcmp(pStateName, "Decide") == 0)
        {
            return StateMachineUiEventKind_Decide;
        }
        else if (strcmp(pStateName, "Cancel") == 0)
        {
            return StateMachineUiEventKind_Decide;
        }
        else if (strcmp(pStateName, "In") == 0)
        {
            return StateMachineUiEventKind_In;
        }
        else if (strcmp(pStateName, "Out") == 0)
        {
            return StateMachineUiEventKind_Out;
        }
        else
        {
            return StateMachineUiEventKind_None;
        }
    }
};

//----------------------------------------------------------
// ステートマシン
class StateMachine
{
public:

    //----------------------------------------------------------
    StateMachine()
        : m_Layout(nullptr)
        , m_pName(nullptr)
        , m_pInOutStateLayer(nullptr)
        , m_hasDecideEvent(false)
        , m_DecidableButtonPaneCount(0)
        , m_pResStateVariableConnectorArray(nullptr)
        , m_pSelfEventHandeler(nullptr)
        , m_pParentEventHandeler(nullptr)
        , m_isDown(false)
    {
    }

    //----------------------------------------------------------
    void Initialize(nn::ui2d::Layout* pLayout, const char* pName)
    {
        m_Layout = pLayout;
        m_pName = pName;
        m_StateMachineEventQueue.Initialize();
        m_StateMachineVariableManager.Initialize(&m_StateMachineEventQueue);

        m_pSelfEventHandeler = Layout::AllocateAndConstruct<StateMachineEventHandler>(this);
    }

    //----------------------------------------------------------
    bool IsInitialized() const
    {
        for (auto iter = m_StateLayerSet.begin(); iter != m_StateLayerSet.end(); iter++)
        {
            if(!iter->IsInitialized())
            {
                return false;
            }
        }

        return true;
    }

    //----------------------------------------------------------
    void Finalize()
    {
        for (auto iter = m_StateLayerSet.begin(); iter != m_StateLayerSet.end(); )
        {
            auto currIter = iter;
            iter++;

            m_StateLayerSet.erase(currIter);

            currIter->Finalize();
            nn::ui2d::Layout::DeleteObj(&(*currIter));
        }

        m_Layout = nullptr;
        m_pName = nullptr;
        m_pResStateVariableConnectorArray = nullptr;

        nn::ui2d::Layout::DeleteObj(m_pSelfEventHandeler);
        m_pSelfEventHandeler = nullptr;
        m_pParentEventHandeler = nullptr;

        m_StateMachineEventQueue.Finalize();
        m_StateMachineVariableManager.Finalize();
    }

    //----------------------------------------------------------
    void RestoreToInitialState()
    {
        for (auto iter = m_StateLayerSet.begin(); iter != m_StateLayerSet.end(); iter++)
        {
            iter->RestoreToInitialState();
        }
    }

    //----------------------------------------------------------
    // 例：ステート変更をリクエストする
    //     kind = StateMachineEventKind_StateChangeRequested
    //     param2 = レイヤー名
    //     param3 = 遷移ステート名
    //         該当トランジションが無い場合は遷移しません。
    //         イベントの引き数仕様は、TransitionConditionBase の派生クラスの実装を参照します。
    void PostEvent(StateMachineEventKind kind, uint64_t param1, uint64_t param2, uint64_t param3, uint32_t delayFrames)
    {
        m_StateMachineEventQueue.Push(kind, param1, param2, param3, delayFrames);
    }

    //----------------------------------------------------------
    // ユーザーの入力を与えて、内部メッセージを m_StateMachineEventQueue に生成します。
    void UpdateUserInput(const nn::util::Float2* pPos, bool isDown, bool isRelease = false)
    {
        NN_UNUSED(pPos);
        NN_UNUSED(isRelease);

        if (isDown && !m_isDown)
        {
            m_StateMachineEventQueue.Push(StateMachineEventKind_Decided, 0, 0, 0, 0);
        }

        m_isDown = isDown;

        // レイヤーのHit更新
        for (auto iter = m_StateLayerSet.begin(); iter != m_StateLayerSet.end(); iter++)
        {
            if (!iter->HasHit())
            {
                continue;
            }

            if (iter->IsHit(*pPos))
            {
                uint64_t param1 = reinterpret_cast<uint64_t>(iter->GetName());
                m_StateMachineEventQueue.Push(StateMachineEventKind_Hit, param1, 0, 0, 0);
            }
            else
            {
                uint64_t param1 = reinterpret_cast<uint64_t>(iter->GetName());
                m_StateMachineEventQueue.Push(StateMachineEventKind_NoHit, param1, 0, 0, 0);
            }
        }

        // 子供のステートマシンも更新します。
        for (auto iter = m_Layout->GetPartsPaneList().begin(); iter != m_Layout->GetPartsPaneList().end(); iter++)
        {
            StateMachine* pStateMachine = iter->GetLayout()->GetStateMachine();
            if (pStateMachine != nullptr)
            {
                pStateMachine->UpdateUserInput(pPos, isDown, isRelease);
            }
        }
    }

    //----------------------------------------------------------
    void Update(nn::gfx::Device* pDevice, float step, StateMachine* pParentState)
    {
        NN_UNUSED(pParentState);

        // 更新
        for (auto iter = m_StateLayerSet.begin(); iter != m_StateLayerSet.end(); iter++)
        {
            UpdateStateLayer_(&(*iter), pParentState);
        }

        // 子供のステートマシンも更新します。
        for (auto iter = m_Layout->GetPartsPaneList().begin(); iter != m_Layout->GetPartsPaneList().end(); iter++)
        {
            StateMachine* pStateMachine = iter->GetLayout()->GetStateMachine();
            if (pStateMachine != nullptr)
            {
                pStateMachine->Update(pDevice, step, this);
            }
        }

        // メッセージキューを舐め、StateLayer にメッセージを振り分ける。。。
        StateMachineEvent* pEventMsg = m_StateMachineEventQueue.Peek();
        StateMachineEvent* pEndEventMsg = m_StateMachineEventQueue.Bottom();

        while (pEventMsg != nullptr)
        {
            // イベントのディレイフレーム対応
            if (pEventMsg->delayFrames > 0)
            {
                // ディレイフレームが完了するまで、処理しません。
                pEventMsg->delayFrames--;
                m_StateMachineEventQueue.MoveTopToTail();
            }
            else
            {
                // ステート変数変更イベントかどうか
                if (detail::IsStateVariablesChangeRequestEvent(pEventMsg->kind))
                {
                    const char* pVarName = reinterpret_cast<const char*>(pEventMsg->param2);
                    float newValue = *reinterpret_cast<const float*>(pEventMsg->param3);

                    if (pEventMsg->kind == StateMachineEventKind_StateVariablesAddRequested)
                    {
                        newValue = m_StateMachineVariableManager.GetValueAsFloat(pVarName) + newValue;
                    }

                    SetFloatValue(pVarName, newValue);
                }
                else
                {
                    for (auto iter = m_StateLayerSet.begin(); iter != m_StateLayerSet.end(); iter++)
                    {
                        iter->UpdateStateLayerTransitions(pDevice, *pEventMsg);
                    }
                }

                m_StateMachineEventQueue.Pop();
            }

            // 終端まで走査したら抜ける
            if (pEventMsg == pEndEventMsg)
            {
                break;
            }

            pEventMsg = m_StateMachineEventQueue.Peek();
        }
    }

    //----------------------------------------------------------
    void SetParentEventHandler(StateMachineEventHandler* pHandler)
    {
        m_pParentEventHandeler = pHandler;
    }

    //----------------------------------------------------------
    void FirstTimeSetup(nn::gfx::Device* pDevice, StateMachine* pParentState)
    {
        NN_UNUSED(pParentState);

        // 親のイベントハンドラーを記録しておきます。
        SetParentEventHandler(pParentState != nullptr ? pParentState->GetStateMachineEventHandler() : nullptr);

        // m_pInOutStateLayer を設定
        m_pInOutStateLayer = FindStateLayerByName("InOut");

        // Decideボタンの情報を収集しておきます。
        m_hasDecideEvent = CheckHasDecideEvent_();

        // 子供のステートマシンも更新します。
        for (auto iter = m_Layout->GetPartsPaneList().begin(); iter != m_Layout->GetPartsPaneList().end(); iter++)
        {
            StateMachine* pSubStateMachine = iter->GetLayout()->GetStateMachine();

            if (pSubStateMachine != nullptr)
            {
                pSubStateMachine->m_pName = iter->GetName();
                pSubStateMachine->FirstTimeSetup(pDevice, this);
            }
        }

        // 注意：
        // 子供のステートマシン の m_hasDecideEvent を利用するので
        // 子供ステートマシンのセットアップが完了してから処理をします。
        SetupDecideButtons_();
    }

    //----------------------------------------------------------
    const StateLayerSet& GetStateLayerSet() const
    {
        return m_StateLayerSet;
    }

    const StateMachineEventQueue& GetStateMachineEventQueue() const
    {
        return m_StateMachineEventQueue;
    }

    //----------------------------------------------------------
    const bool GetHasDecideEvent() const
    {
        return m_hasDecideEvent;
    }

    //----------------------------------------------------------

    const StateMachineVariableManager& GetStateMachineVariableManager() const
    {
        return m_StateMachineVariableManager;
    }

    StateMachineVariableManager& GetStateMachineVariableManager()
    {
        return m_StateMachineVariableManager;
    }

    //----------------------------------------------------------
    void PushBackNewStateLayer(StateLayer* pNewStateLayer)
    {
        m_StateLayerSet.push_back(*pNewStateLayer);
    }

    //----------------------------------------------------------
    void SetVariableConnectors(const ResStateVariableConnector* pStateConnectors, int count)
    {
        m_pResStateVariableConnectorArray = pStateConnectors;
        m_StateVariableConnectorCount = count;
    }

    //----------------------------------------------------------
    void SetFloatValue(const char* pName, float newValue)
    {
        const float oldV = m_StateMachineVariableManager.GetValueAsFloat(pName);
        if (m_StateMachineVariableManager.SetFloatValue(pName, newValue))
        {
            const float newV = m_StateMachineVariableManager.GetValueAsFloat(pName);
            if (m_pParentEventHandeler != nullptr)
            {
                m_pParentEventHandeler->OnVariablesChanged(this->m_pName, pName, oldV, newV);
            }
        }
    }

    void ResetVariableToDefalut(const char* pName)
    {
        const float oldV = m_StateMachineVariableManager.GetValueAsFloat(pName);
        if (m_StateMachineVariableManager.ResetToDefalut(pName))
        {
            const float newV = m_StateMachineVariableManager.GetValueAsFloat(pName);
            if (m_pParentEventHandeler != nullptr)
            {
                m_pParentEventHandeler->OnVariablesChanged(this->m_pName, pName, oldV, newV);
            }
        }
    }

    float GetFloatValue(const char* pName)
    {
        return m_StateMachineVariableManager.GetValueAsFloat(pName);
    }

    void HandleVariablesConnectorsOnVariablesChange(const char* pStateMachineName, const char* pVarName, float newValue)
    {
        NN_UNUSED(pStateMachineName);
        NN_UNUSED(pVarName);
        NN_UNUSED(newValue);

        // コネクターを処理します。
        for (int i = 0; i < m_StateVariableConnectorCount; i++)
        {
            StateMachine* pTargetStateMachine = nullptr;

            const ResStateVariableConnector* pConnector = &m_pResStateVariableConnectorArray[i];
            if (strcmp(pConnector->outStateMachineName, "body") == 0)
            {
                pTargetStateMachine = this;
            }
            else
            {
                nn::ui2d::Parts* pParts = m_Layout->FindPartsPaneByName(pConnector->outStateMachineName);
                if (pParts != nullptr)
                {
                    pTargetStateMachine = pParts->GetLayout()->GetStateMachine();
                }
            }

            if (pTargetStateMachine != nullptr)
            {
                pTargetStateMachine->SetFloatValue(pConnector->outStateVariablesName, newValue);
            }
        }
    }

    //----------------------------------------------------------
    StateMachineEventHandler* GetStateMachineEventHandler()
    {
        return m_pSelfEventHandeler;
    }

    //----------------------------------------------------------
    // ボタンの名前から、ボタン番号を取得します。
    // ボタン番号は、ステートマシンを持つシーン内での番号で
    // 画面遷移情報（次の画面はどの画面に遷移するべきかを取得するための情報）を検索する際のキーとして利用されます。
    int GetScreenChangableButtonIndexByButtonName(const char* pButtonName)
    {
        if (m_pInOutStateLayer == nullptr)
        {
            return -1;
        }

        for (int index = 0; index < m_DecidableButtonPaneCount; index++)
        {
            if (strcmp(m_DecidableButtonPanePtrs[index]->GetName(), pButtonName) == 0)
            {
                return index;
            }
        }

        return -1;
    }

private:

    //----------------------------------------------------------
    bool CheckHasDecideEvent_() const
    {
        for (auto iter = m_StateLayerSet.begin(); iter != m_StateLayerSet.end(); iter++)
        {
            if (iter->FindDecideState() != nullptr)
            {
                return true;
            }
        }

        return false;
    }

    //----------------------------------------------------------
    bool CheckIsSpecifiedButtons_() const
    {
        if (m_pInOutStateLayer == nullptr)
        {
            return false;
        }

        for (auto iter = m_pInOutStateLayer->GetFeatureParameterSet().begin(); iter != m_pInOutStateLayer->GetFeatureParameterSet().end(); iter++)
        {
            if (iter->m_parameterKind == StateMachineFeatureParameterKind_StateMachineEvent)
            {
                return true;
            }
        }

        return false;
    }

    //----------------------------------------------------------
    // DecideButtons の初期化
    void SetupDecideButtons_();

    //----------------------------------------------------------
    // StateLayer の検索
    StateLayer* FindStateLayerByName(const char* pName)
    {
        for (auto iter = m_StateLayerSet.begin(); iter != m_StateLayerSet.end(); iter++)
        {
            if (strcmp(iter->GetName(), pName) == 0)
            {
                return &(*iter);
            }
        }

        return nullptr;
    }

    //----------------------------------------------------------
    // StateLayer の更新
    void UpdateStateLayer_(StateLayer* pStateLayer, StateMachine* pParentState)
    {
        NN_UNUSED(pParentState);

        if (!pStateLayer->IsInitialized())
        {
            return;
        }

        // 更新します
        pStateLayer->Update();

        // ステート完了イベントの送信
        if (pStateLayer->IsStateTransitionJustCompleted())
        {
            uint64_t param1 = reinterpret_cast<uint64_t>(pStateLayer->GetName());
            uint64_t param2 = reinterpret_cast<uint64_t>(pStateLayer->GetCurrentState()->GetName());
            m_StateMachineEventQueue.Push(StateMachineEventKind_StateTransitionCompleted, param1, param2, 0, 0);

            // 親にも通知する。
            if (m_pParentEventHandeler != nullptr)
            {
                const char* pNextStateName = pStateLayer->GetCurrentState()->GetName();
                m_pParentEventHandeler->OnStateChangeCompletedRaw(this->m_pName, pStateLayer->GetName(), pNextStateName);

                // UIとして代表的なメッセージを送る
                m_pParentEventHandeler->OnStateChangeCompleted(this->m_pName, StateMachineEventHandler::GetStateMachineUIEventFromStateName(pNextStateName));
            }
        }
    }

    //----------------------------------------------------------
    // 対象としている、レイアウト
    nn::ui2d::Layout*                m_Layout;
    const char*                      m_pName;

    StateLayerSet                    m_StateLayerSet;
    StateLayer*                      m_pInOutStateLayer;
    bool                             m_hasDecideEvent;

    nn::ui2d::Pane*                  m_DecidableButtonPanePtrs[8];
    int                              m_DecidableButtonPaneCount;

    // イベントキュー
    StateMachineEventQueue           m_StateMachineEventQueue;
    StateMachineVariableManager      m_StateMachineVariableManager;

    const ResStateVariableConnector* m_pResStateVariableConnectorArray;
    int                              m_StateVariableConnectorCount;

    StateMachineEventHandler*        m_pSelfEventHandeler;
    StateMachineEventHandler*        m_pParentEventHandeler;

    bool                             m_isDown;
};

//--------------------------------------------------------------------------------------------------
// 生成クラス（リソースクラスを入力に受け取ってステートマシンを構築します）
class StateMachineFactory
{
public:

    //----------------------------------------------------------
    StateMachineFactory(nn::gfx::Device* pDevice, nn::ui2d::Layout* pLayout)
        : m_pDevice(pDevice)
        , m_pLayout(pLayout)
    {
    }

    //----------------------------------------------------------
    // ステートマシンを構築します。
    void Build(StateMachine* pStateMachine, void* pResStateMachineFile)
    {
        nn::font::detail::BinaryBlockHeader* pBinFile = reinterpret_cast<nn::font::detail::BinaryBlockHeader*>((char*)pResStateMachineFile);

        ResStateMachine* pResStateMachine = (ResStateMachine*)pBinFile;
        DoRelocate_(pResStateMachine, pBinFile);

        pStateMachine->Initialize(m_pLayout, pResStateMachine->name);

        for (int i = 0; i < pResStateMachine->layerCount; i++)
        {
            ResStateLayer* pLayer = pResStateMachine->pLayers.Get() + i;

            auto pStateLayer = BuildStateLayer_(pLayer);
            pStateMachine->PushBackNewStateLayer(pStateLayer);
        }

        // ステートマシン変数を追加
        for (int i = 0; i < pResStateMachine->variableCount; i++)
        {
            const ResStateVariableDescriptions* pResVar = pResStateMachine->pVariables.Get() + i;
            pStateMachine->GetStateMachineVariableManager().RegisterNewVariable(pResVar->name, StateMachineVariableType_Float, pResVar->defaultValue, pResVar->minValue, pResVar->maxValue);
        }

        // ステートマシン変数コネクターを追加
        const ResStateVariableConnector* pResCon = pResStateMachine->pVariablesConnectors.Get();
        pStateMachine->SetVariableConnectors(pResCon, pResStateMachine->variableConnectorCount);
    }

private:

    //----------------------------------------------------------

    void DoRelocate_(ResStateMachine* pResStateMachine, void* pBinFile)
    {
        if (pResStateMachine->isRelocated)
        {
            return;
        }

        pResStateMachine->isRelocated = true;

        pResStateMachine->pLayers.Relocate(pBinFile);
        pResStateMachine->pVariables.Relocate(pBinFile);
        pResStateMachine->pVariablesConnectors.Relocate(pBinFile);

        for (int i = 0; i < pResStateMachine->layerCount; i++)
        {
            ResStateLayer* pLayer = pResStateMachine->pLayers.Get() + i;

            pLayer->pStates.Relocate(pBinFile);
            for (int ii = 0; ii < pLayer->stateCount; ii++)
            {
                ResState* pState = pLayer->pStates.Get() + ii;

                pState->pParameters.Relocate(pBinFile);
                pState->pParameterNameOptions.Relocate(pBinFile);
            }

            pLayer->pTransitions.Relocate(pBinFile);
            for (int ij = 0; ij < pLayer->transitionCount; ij++)
            {
                ResStateTransition* pTransition = pLayer->pTransitions.Get() + ij;

                pTransition->pTracks.Relocate(pBinFile);
                for (int iji = 0; iji < pTransition->trackCount; iji++)
                {
                    ResStateTransitionTrack* pTrack = pTransition->pTracks.Get() + iji;

                    pTrack->pKeys.Relocate(pBinFile);
                }

                pTransition->trigger.pVariableArray.Relocate(pBinFile);
            }
        }
    }

    //----------------------------------------------------------
    // type StateMachineFeatureParameterKind
    int GetTargetTextureIndex_(uint32_t type)
    {
        switch (type)
        {
        case nn::ui2d::StateMachineFeatureParameterKind_Tex0S:
        case nn::ui2d::StateMachineFeatureParameterKind_Tex0R:
        case nn::ui2d::StateMachineFeatureParameterKind_Tex0T:
            return 0;
        case nn::ui2d::StateMachineFeatureParameterKind_Tex1S:
        case nn::ui2d::StateMachineFeatureParameterKind_Tex1R:
        case nn::ui2d::StateMachineFeatureParameterKind_Tex1T:
            return 1;
        case nn::ui2d::StateMachineFeatureParameterKind_Tex2S:
        case nn::ui2d::StateMachineFeatureParameterKind_Tex2R:
        case nn::ui2d::StateMachineFeatureParameterKind_Tex2T:
            return 2;
            break;
        default:
            return -1;
        }
    }

    //----------------------------------------------------------

    void BuildFeatureParameters_(StateLayer* pStateLayer, ResStateLayer* pResStateLayer)
    {
        // 先頭は Base ステート。
        ResState* pResBaseState = pResStateLayer->pStates.Get() + 0;
        for (int paramIdx = 0; paramIdx < pResBaseState->parameterCount; paramIdx++)
        {
            ResStateParameterStore* pResParamStore = pResBaseState->pParameters.Get() + paramIdx;
            FeatureParameter* param1 = nn::ui2d::Layout::AllocateAndConstruct<FeatureParameter>();
            const StateMachineFeatureParameterKind paramKind = static_cast<StateMachineFeatureParameterKind>(pResParamStore->type);
            switch (pResParamStore->type)
            {
            case StateMachineFeatureParameterKind_Position:
            {
                uint8_t param[] = { AnimTargetPane_TranslateX, AnimTargetPane_TranslateY, AnimTargetPane_TranslateZ };
                param1->Initialzie(pResParamStore->targetName, paramKind, 0, nn::ui2d::AnimContentType_Pane, nn::ui2d::AnimationTypePaneSrt, 3, param);
            }
            break;
            case StateMachineFeatureParameterKind_Scale:
            {
                uint8_t param[] = { AnimTargetPane_ScaleX, AnimTargetPane_ScaleY };
                param1->Initialzie(pResParamStore->targetName, paramKind, 0, nn::ui2d::AnimContentType_Pane, nn::ui2d::AnimationTypePaneSrt, 2, param);
            }
            break;
            case StateMachineFeatureParameterKind_Rotate:
            {
                uint8_t param[] = { AnimTargetPane_RotateX, AnimTargetPane_RotateY, AnimTargetPane_RotateZ };
                param1->Initialzie(pResParamStore->targetName, paramKind, 0, nn::ui2d::AnimContentType_Pane, nn::ui2d::AnimationTypePaneSrt, 3, param);
            }
            break;
            case StateMachineFeatureParameterKind_Size:
            {
                uint8_t param[] = { AnimTargetPane_SizeWidth, AnimTargetPane_SizeHeight };
                param1->Initialzie(pResParamStore->targetName, paramKind, 0, nn::ui2d::AnimContentType_Pane, nn::ui2d::AnimationTypePaneSrt, 2, param);
            }
            break;
            case StateMachineFeatureParameterKind_Visibility:
            {
                uint8_t param[] = { AnimTargetVisibility_Visibility };
                param1->Initialzie(pResParamStore->targetName, paramKind, 0, nn::ui2d::AnimContentType_Pane, nn::ui2d::AnimationTypeVisibility, 1, param);
            }
            break;
            case StateMachineFeatureParameterKind_PaneAlpha:
            {
                uint8_t param[] = { AnimTargetPaneColor_PaneAlpha };
                param1->Initialzie(pResParamStore->targetName, paramKind, 0, nn::ui2d::AnimContentType_Pane, nn::ui2d::AnimationTypeVertexColor, 1, param);
            }
            break;
            case StateMachineFeatureParameterKind_MaskTexS:
            {
                uint8_t param[] = { AnimTargetMaskTexSrt_ScaleX, AnimTargetMaskTexSrt_ScaleY };
                param1->Initialzie(pResParamStore->targetName, paramKind, 0, nn::ui2d::AnimContentType_Pane, nn::ui2d::AnimationTypeMaskTexSrt, 2, param);
            }
            break;
            case StateMachineFeatureParameterKind_MaskTexR:
            {
                uint8_t param[] = { AnimTargetMaskTexSrt_Rotate };
                param1->Initialzie(pResParamStore->targetName, paramKind, 0, nn::ui2d::AnimContentType_Pane, nn::ui2d::AnimationTypeMaskTexSrt, 1, param);
            }
            break;
            case StateMachineFeatureParameterKind_MaskTexT:
            {
                uint8_t param[] = { AnimTargetMaskTexSrt_TranslateX, AnimTargetMaskTexSrt_TranslateX };
                param1->Initialzie(pResParamStore->targetName, paramKind, 0, nn::ui2d::AnimContentType_Pane, nn::ui2d::AnimationTypeMaskTexSrt, 2, param);
            }
            break;

            case StateMachineFeatureParameterKind_RoundRect:
            {
                uint8_t param[] = { AnimTargetProceduralShape_ExpLeftTop, AnimTargetProceduralShape_RadiusLeftTop };
                param1->Initialzie(pResParamStore->targetName, paramKind, 0, nn::ui2d::AnimContentType_Pane, nn::ui2d::AnimationTypeProceduralShape, 2, param);
            }
            break;
            case StateMachineFeatureParameterKind_BlackColor:
            case StateMachineFeatureParameterKind_WhiteColor:
            {
                if (pResParamStore->type == StateMachineFeatureParameterKind_BlackColor)
                {
                    uint8_t param[] = { AnimTargetMatColor_BufferR, AnimTargetMatColor_BufferG, AnimTargetMatColor_BufferB, AnimTargetMatColor_BufferA };
                    param1->Initialzie(pResParamStore->targetName, paramKind, 0, nn::ui2d::AnimContentType_Material, nn::ui2d::AnimationTypeMaterialColor, 4, param);
                }
                else
                {
                    uint8_t param[] = { AnimTargetMatColor_Konst0R, AnimTargetMatColor_Konst0G, AnimTargetMatColor_Konst0B, AnimTargetMatColor_Konst0A };
                    param1->Initialzie(pResParamStore->targetName, paramKind, 0, nn::ui2d::AnimContentType_Material, nn::ui2d::AnimationTypeMaterialColor, 4, param);
                }

            }
            break;
            case StateMachineFeatureParameterKind_Tex0S:
            case StateMachineFeatureParameterKind_Tex1S:
            case StateMachineFeatureParameterKind_Tex2S:
            {
                uint8_t param[] = { AnimTargetTexSrt_ScaleS, AnimTargetTexSrt_ScaleT };
                param1->Initialzie(pResParamStore->targetName, paramKind, GetTargetTextureIndex_(pResParamStore->type), nn::ui2d::AnimContentType_Material, nn::ui2d::AnimationTypeTextureSrt, 2, param);
            }
            break;
            case StateMachineFeatureParameterKind_Tex0R:
            case StateMachineFeatureParameterKind_Tex1R:
            case StateMachineFeatureParameterKind_Tex2R:
            {
                uint8_t param[] = { AnimTargetMaskTexSrt_Rotate };
                param1->Initialzie(pResParamStore->targetName, paramKind, GetTargetTextureIndex_(pResParamStore->type), nn::ui2d::AnimContentType_Material, nn::ui2d::AnimationTypeTextureSrt, 1, param);
            }
            break;
            case StateMachineFeatureParameterKind_Tex0T:
            case StateMachineFeatureParameterKind_Tex1T:
            case StateMachineFeatureParameterKind_Tex2T:
            {
                uint8_t param[] = { AnimTargetTexSrt_TranslateS, AnimTargetTexSrt_TranslateT };
                param1->Initialzie(pResParamStore->targetName, paramKind, GetTargetTextureIndex_(pResParamStore->type), nn::ui2d::AnimContentType_Material, nn::ui2d::AnimationTypeTextureSrt, 2, param);
            }
            break;

            case StateMachineFeatureParameterKind_StateMachineEvent:
            {
                uint8_t param[] = { AnimTargetStateMachineEvent_PostToChild };
                param1->Initialzie(pResParamStore->targetName, paramKind, 0, nn::ui2d::AnimContentType_StateMachine, nn::ui2d::AnimationTypeStateMachine, 1, param);
            }
            break;
            case StateMachineFeatureParameterKind_PartsStateMachineLayer:
            {
                // 追加情報を読んで、FeatureParameterに情報を埋める。
                ResStateParameterNameOption* pResParamNameOpt = pResBaseState->pParameterNameOptions.Get() + paramIdx;

                uint8_t param[] = { AnimTargetPane_TranslateX, AnimTargetPane_TranslateX, AnimTargetPane_TranslateX, AnimTargetPane_TranslateX };
                param1->InitialzieForPartsStateLayer(pResParamStore->targetName, paramKind, 0, nn::ui2d::AnimContentType_PartsStateMachine, nn::ui2d::AnimationTypePartsStateMachine, 4, param, pResParamNameOpt);
            }
            break;
            case StateMachineFeatureParameterKind_None:
            {
                // 暫定的にダミーを挿入しておきます。
                uint8_t param[] = { AnimTargetPane_TranslateX };
                param1->Initialzie(pResParamStore->targetName, paramKind, 0, nn::ui2d::AnimContentType_Pane, nn::ui2d::AnimationTypePaneSrt, 1, param);
            }
            break;
            case StateMachineFeatureParameterKind_Max:
            default:
            {
                NN_SDK_ASSERT(false);
                return;
            }
            break;
            }
            pStateLayer->PushBackNewFeatureParameter(param1);
        }
    } //NOLINT(impl/function_size)

    //----------------------------------------------------------

    void BuildStates_(StateLayer* pStateLayer, ResStateLayer* pResStateLayer)
    {
        for (int stateIdx = 0; stateIdx < pResStateLayer->stateCount; stateIdx++)
        {
            ResState* pResState = pResStateLayer->pStates.Get() + stateIdx;

            State* pState1 = nn::ui2d::Layout::AllocateAndConstruct<State>();
            {
                pState1->Initialzie(pResState->name, pStateLayer->GetFeatureParameterSet());

                for (int paramIdx = 0; paramIdx < pResState->parameterCount; paramIdx++)
                {
                    ResStateParameterStore* pStore = pResState->pParameters.Get() + paramIdx;
                    float value[] = { pStore->x, pStore->y, pStore->z, pStore->w };
                    pState1->SetParameters(paramIdx, 4, value);
                }
            }
            pStateLayer->PushBackNewState(pState1);
        }
    }

    //----------------------------------------------------------

    void BuildTransitionTrigger_(Transition* pTransition, ResStateLayer* pResStateLayer, ResStateTransition* pResTransition)
    {
        {
            ResStateTransitionTrigger* pResTrigger = &pResTransition->trigger;
            switch (pResTrigger->triggerKind)
            {
            case StateMachineTriggerKind_IsTick:
            {
                TransitionConditionNone* pCondition = nn::ui2d::Layout::AllocateAndConstruct<TransitionConditionNone>();
                pTransition->SetCondition(pCondition);
            }
            break;
            case StateMachineTriggerKind_IsHit:
            {
                TransitionConditionIsHit* pCondition = nn::ui2d::Layout::AllocateAndConstruct<TransitionConditionIsHit>();
                pCondition->m_pLayerName = pResStateLayer->name;
                pTransition->SetCondition(pCondition);
            }
            break;
            case StateMachineTriggerKind_IsNoHit:
            {
                TransitionConditionIsNoHit* pCondition = nn::ui2d::Layout::AllocateAndConstruct<TransitionConditionIsNoHit>();
                pCondition->m_pLayerName = pResStateLayer->name;
                pTransition->SetCondition(pCondition);
            }
            break;
            case StateMachineTriggerKind_IsDecided:
            {
                TransitionConditionIsDecided* pCondition = nn::ui2d::Layout::AllocateAndConstruct<TransitionConditionIsDecided>();
                pTransition->SetCondition(pCondition);
            }
            break;
            case StateMachineTriggerKind_IsStateTransitionCompleted:
            {
                TransitionConditionIsStateTransitionCompleted* pCondition = nn::ui2d::Layout::AllocateAndConstruct<TransitionConditionIsStateTransitionCompleted>();
                pCondition->m_Type = StateMachineEventKind_StateTransitionCompleted;
                pCondition->m_pLayerName = pResStateLayer->name;
                pCondition->m_pStateName = pResTransition->startName;
                pTransition->SetCondition(pCondition);
            }
            break;
            case StateMachineTriggerKind_IsSubStateTransitionCompleted:
            {
                TransitionConditionIsStateTransitionCompleted* pCondition = nn::ui2d::Layout::AllocateAndConstruct<TransitionConditionIsStateTransitionCompleted>();

                NN_SDK_ASSERT(pResTrigger->variableCount == 3);
                ResStateVariable* pResStateVariable = pResTrigger->pVariableArray.Get();

                pCondition->m_Type = StateMachineTriggerKind_IsSubStateTransitionCompleted;
                pCondition->m_pSenderName = (pResStateVariable + 0)->name;
                pCondition->m_pLayerName = (pResStateVariable + 1)->name;
                pCondition->m_pStateName = (pResStateVariable + 2)->name;

                pTransition->SetCondition(pCondition);
            }
            break;
            case StateMachineTriggerKind_IsVariableChanged:
            {
                TransitionConditionVariableChanged* pCondition = nn::ui2d::Layout::AllocateAndConstruct<TransitionConditionVariableChanged>();

                NN_SDK_ASSERT(pResTrigger->variableCount == 1);

                ResStateVariable* pResStateVariable = pResTrigger->pVariableArray.Get();

                nn::util::Strlcpy(pCondition->m_Name, pResStateVariable->name, sizeof(pCondition->m_Name));
                pCondition->m_StateMachineOperator = static_cast<StateMachineOperator>(pResTrigger->stateMachineOperator);

                StateMachineVariableContent::Set(&pCondition->m_Content, StateMachineVariableType_Float, &pResStateVariable->value0);

                pTransition->SetCondition(pCondition);
            }
            break;
            case StateMachineTriggerKind_IsStateChangeRequested:
            {
                struct TransitionConditionStateChangeRequested* pCondition =
                    nn::ui2d::Layout::AllocateAndConstruct<struct TransitionConditionStateChangeRequested>(pResStateLayer->name, pResTransition->endName);
                pTransition->SetCondition(pCondition);
            }
            break;
            case StateMachineTriggerKind_IsButtonDecided:
            {
                TransitionConditionIsButtonDecided* pCondition = nn::ui2d::Layout::AllocateAndConstruct<TransitionConditionIsButtonDecided>();
                pTransition->SetCondition(pCondition);
            }
            break;
            case StateMachineTriggerKind_Max:
            default:
            {
                NN_SDK_ASSERT(false);
                return;
            }
            break;
            }
        }
    }

    //----------------------------------------------------------

    void BuildTransitions_(StateLayer* pStateLayer, ResStateLayer* pResStateLayer)
    {
        for (int transitionIdx = 0; transitionIdx < pResStateLayer->transitionCount; transitionIdx++)
        {
            ResStateTransition* pResTransition = pResStateLayer->pTransitions.Get() + transitionIdx;

            Transition* pTransition = nn::ui2d::Layout::AllocateAndConstruct<Transition>();
            pTransition->Initialzie(
                pResTransition->startName, pResTransition->endName,
                pResTransition->isCancelable == 1, pResTransition->isLoop == 1);

            TransitionTimeLine* pTimeLine = nn::ui2d::Layout::AllocateAndConstruct<TransitionTimeLine>();
            pTransition->SetTimeline(pTimeLine);

            pTimeLine->SetTotalLength(pResTransition->totalDuration);

            // Track
            pTimeLine->Initialize(pResTransition->trackCount);

            for (int trackIdx = 0; trackIdx < pResTransition->trackCount; trackIdx++)
            {
                TransitionTimeLineTrack* pTrack = pTimeLine->GetTrackByFeatureParameterIndex(trackIdx);

                const ResStateTransitionTrack* pResTrack = pResTransition->pTracks.Get() + trackIdx;

                pTrack->SetTimeing(pResTrack->offset, pResTrack->duration, pResTrack->easingType, pResTrack->easingExtraParameter);

                // Key
                {
                    pTrack->Initialize(pResTrack->keyCount);

                    for (int keyIdx = 0; keyIdx < pResTrack->keyCount; keyIdx++)
                    {
                        TransitionSection* pKie = pTrack->GetSectionByIndex(keyIdx);

                        const ResStateTransitionTrackKey* pResKey = pResTrack->pKeys.Get() + keyIdx;

                        pKie->timing.offset = pResKey->offset;
                        pKie->timing.duration = 1;
                        pKie->timing.easingType = pResKey->easingType;
                        pKie->timing.easingExtraParameter = pResKey->easingExtraParameter;

                        pKie->isUsedForEvent = pResKey->keyEvent.kind != StateMachineEventKind_None;
                        if (pKie->isUsedForEvent)
                        {
                            // イベントの場合
                            pKie->trackEvent.delay = pResKey->keyEvent.delay;
                            pKie->trackEvent.kind = static_cast<StateMachineEventKind>(pResKey->keyEvent.kind);
                            pKie->trackEvent.pParam1 = pResKey->keyEvent.param1;
                            pKie->trackEvent.pParam2 = pResKey->keyEvent.param2;
                            pKie->trackEvent.pParam3 = pResKey->keyEvent.param3;
                        }
                        else
                        {
                            // キーの場合
                            pKie->store[0].paramValue = pResKey->keyParameter.x;
                            pKie->store[1].paramValue = pResKey->keyParameter.y;
                            pKie->store[2].paramValue = pResKey->keyParameter.z;
                            pKie->store[3].paramValue = pResKey->keyParameter.w;
                        }
                    }
                }
            }

            pStateLayer->PushBackNewTransition(pTransition);

            BuildTransitionTrigger_(pTransition, pResStateLayer, pResTransition);
        }
    }

    //----------------------------------------------------------

    StateLayer* BuildStateLayer_(ResStateLayer* pResStateLayer)
    {
        StateLayer* pStateLayer = nn::ui2d::Layout::AllocateAndConstruct<StateLayer>();
        NN_SDK_ASSERT(pResStateLayer->stateCount > 0);

        // FeatureParameterSet の初期化
        {
            BuildFeatureParameters_(pStateLayer, pResStateLayer);
            pStateLayer->Initialize(m_pDevice, m_pLayout, pResStateLayer->name);
        }

        // state の初期化
        BuildStates_(pStateLayer, pResStateLayer);

        // Transition の初期化
        BuildTransitions_(pStateLayer, pResStateLayer);
        pStateLayer->SetLayerType((StateMachineLayerType)pResStateLayer->layerType);

        // コリジョン用の Boundingを設定する
        if (nn::util::Strnlen(pResStateLayer->boundingPaneName, CountOfStateMachineName) > 0)
        {
            pStateLayer->SetHitPane(m_pLayout->GetRootPane()->FindPaneByName(pResStateLayer->boundingPaneName));
            NN_SDK_ASSERT_NOT_NULL(pStateLayer->GetHitPane());
        }

        // 初期ステートを設定します。
        // In があればIn なければ Base。
        pStateLayer->RestoreToInitialState();

        return pStateLayer;
    }

    nn::gfx::Device* m_pDevice;
    nn::ui2d::Layout* m_pLayout;
};


} // namespace ui2d
} // namespace nn

#endif // NN_UI2D_VIEWER_ENABLED
