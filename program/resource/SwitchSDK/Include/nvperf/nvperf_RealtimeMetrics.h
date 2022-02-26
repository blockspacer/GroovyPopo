#pragma once

/*
 * Copyright 2014-2018  NVIDIA Corporation.  All rights reserved.
 *
 * NOTICE TO USER:
 *
 * This source code is subject to NVIDIA ownership rights under U.S. and
 * international Copyright laws.
 *
 * This software and the information contained herein is PROPRIETARY and
 * CONFIDENTIAL to NVIDIA and is being provided under the terms and conditions
 * of a form of NVIDIA software license agreement.
 *
 * NVIDIA MAKES NO REPRESENTATION ABOUT THE SUITABILITY OF THIS SOURCE
 * CODE FOR ANY PURPOSE.  IT IS PROVIDED "AS IS" WITHOUT EXPRESS OR
 * IMPLIED WARRANTY OF ANY KIND.  NVIDIA DISCLAIMS ALL WARRANTIES WITH
 * REGARD TO THIS SOURCE CODE, INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY, NONINFRINGEMENT, AND FITNESS FOR A PARTICULAR PURPOSE.
 * IN NO EVENT SHALL NVIDIA BE LIABLE FOR ANY SPECIAL, INDIRECT, INCIDENTAL,
 * OR CONSEQUENTIAL DAMAGES, OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS
 * OF USE, DATA OR PROFITS,  WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE
 * OR OTHER TORTIOUS ACTION,  ARISING OUT OF OR IN CONNECTION WITH THE USE
 * OR PERFORMANCE OF THIS SOURCE CODE.
 *
 * U.S. Government End Users.   This source code is a "commercial item" as
 * that term is defined at  48 C.F.R. 2.101 (OCT 1995), consisting  of
 * "commercial computer  software"  and "commercial computer software
 * documentation" as such terms are  used in 48 C.F.R. 12.212 (SEPT 1995)
 * and is provided to the U.S. Government only as a commercial end item.
 * Consistent with 48 C.F.R.12.212 and 48 C.F.R. 227.7202-1 through
 * 227.7202-4 (JUNE 1995), all U.S. Government End Users acquire the
 * source code with only those rights set forth herein.
 *
 * Any use of this source code in individual and commercial software must
 * include, in the user documentation and internal comments to the code,
 * the above Disclaimer and U.S. Government End Users Notice.
 */

#include <math.h>
#include <stdint.h>
#include <string.h>
#include <new>
#include <algorithm>

namespace nv { namespace metrics {

    inline double CounterDiv(double lhs, double rhs)
    {
        if (!lhs && !rhs)
        {
            return 0.0;
        }
        return lhs / rhs;
    }

    struct MetricValue
    {
        double sum;
        double avg;
        double peak_sustained;  // this is avg peak_sustained
        double cycles_elapsed;  // this is avg cycles_elapsed

        double sum_per_cycle_elapsed() const
        {
            return CounterDiv(sum, cycles_elapsed);
        }
        double avg_per_cycle_elapsed() const
        {
            return CounterDiv(avg, cycles_elapsed);
        }
        double pct_of_peak_sustained_elapsed() const
        {
            const double result = 100.0 * CounterDiv(avg, cycles_elapsed * peak_sustained);
            return result;
        }

        MetricValue& operator+=(const MetricValue& rhs)
        {
            sum += rhs.sum;
            avg += rhs.avg;
            peak_sustained += rhs.peak_sustained;
            cycles_elapsed = (std::max)(cycles_elapsed, rhs.cycles_elapsed);
            return *this;
        }
        // scalar addition not supported
        MetricValue& operator-=(const MetricValue& rhs)
        {
            sum -= rhs.sum;
            avg -= rhs.avg;
            // peak_sustained remains unmodified
            cycles_elapsed = (std::max)(cycles_elapsed, rhs.cycles_elapsed);
            return *this;
        }
        // scalar subtraction not supported
        MetricValue& operator*=(const MetricValue& rhs)
        {
            sum *= rhs.sum;
            avg *= rhs.avg;
            peak_sustained *= rhs.peak_sustained;
            cycles_elapsed = (std::max)(cycles_elapsed, rhs.cycles_elapsed);
            return *this;
        }
        MetricValue& operator*=(const double rhs)
        {
            sum *= rhs;
            avg *= rhs;
            peak_sustained *= rhs;
            // cycles_elapsed remains unmodified
            return *this;
        }
        MetricValue& operator/=(const MetricValue& rhs)
        {
            sum = CounterDiv(sum, rhs.avg);
            avg = CounterDiv(avg, rhs.avg);
            peak_sustained = CounterDiv(peak_sustained, rhs.peak_sustained);
            cycles_elapsed = (std::max)(cycles_elapsed, rhs.cycles_elapsed);
            return *this;
        }
        MetricValue& operator/=(const double rhs)
        {
            sum = CounterDiv(sum, rhs);
            avg = CounterDiv(avg, rhs);
            peak_sustained = CounterDiv(peak_sustained, rhs);
            // cycles_elapsed remains unmodified
            return *this;
        }
    };

    inline MetricValue operator+(const MetricValue& lhs, const MetricValue& rhs)
    {
        MetricValue result = lhs;
        result += rhs;
        return result;
    }
    // scalar addition not supported
    inline MetricValue operator-(const MetricValue& lhs, const MetricValue& rhs)
    {
        MetricValue result = lhs;
        result -= rhs;
        return result;
    }
    // scalar subtraction not supported
    inline MetricValue operator*(const MetricValue& lhs, const MetricValue& rhs)
    {
        MetricValue result = lhs;
        result *= rhs;
        return result;
    }
    inline MetricValue operator*(const MetricValue& lhs, double rhs)
    {
        MetricValue result = lhs;
        result *= rhs;
        return result;
    }
    inline MetricValue operator*(double lhs, const MetricValue& rhs)
    {
        MetricValue result = rhs;
        result *= lhs;
        return result;
    }
    inline MetricValue operator/(const MetricValue& lhs, const MetricValue& rhs)
    {
        MetricValue result = lhs;
        result /= rhs;
        return result;
    }
    inline MetricValue operator/(const MetricValue& lhs, double rhs)
    {
        MetricValue result = lhs;
        result /= rhs;
        return result;
    }
    inline MetricValue operator/(double lhs, const MetricValue& rhs)
    {
        MetricValue result;
        result.sum = CounterDiv(lhs, rhs.sum);
        result.avg = CounterDiv(lhs, rhs.avg);
        result.peak_sustained = CounterDiv(lhs, rhs.peak_sustained);
        result.cycles_elapsed = rhs.cycles_elapsed;
        return result;
    }

    inline MetricValue fmin(const MetricValue& lhs, const MetricValue& rhs)
    {
        MetricValue result = (rhs.avg < lhs.avg) ? rhs : lhs; // returns lhs in case of NaN argument
        return result;
    }
    template <class... TArgs>
    MetricValue fmin(const MetricValue& lhs, const MetricValue& rhs, TArgs... args)
    {
        MetricValue lhs2 = fmin(lhs, rhs);
        MetricValue result = fmin(lhs2, args...);
        return result;
    }
    inline MetricValue fmin(const MetricValue& lhs, double rhs)
    {
        MetricValue result;
        result.sum = ::fmin(lhs.sum, rhs);
        result.avg = ::fmin(lhs.avg, rhs);
        result.peak_sustained = ::fmin(lhs.peak_sustained, rhs);
        result.cycles_elapsed = lhs.cycles_elapsed;
        return result;
    }
    inline MetricValue fmin(double lhs, const MetricValue& rhs)
    {
        MetricValue result;
        result.sum = ::fmin(lhs, rhs.sum);
        result.avg = ::fmin(lhs, rhs.avg);
        result.peak_sustained = ::fmin(lhs, rhs.peak_sustained);
        result.cycles_elapsed = rhs.cycles_elapsed;
        return result;
    }
    inline MetricValue fmax(const MetricValue& lhs, const MetricValue& rhs)
    {
        MetricValue result = (rhs.avg > lhs.avg) ? rhs : lhs; // returns lhs in case of NaN argument
        return result;
    }
    template <class... TArgs>
    MetricValue fmax(const MetricValue& lhs, const MetricValue& rhs, TArgs... args)
    {
        MetricValue lhs2 = fmax(lhs, rhs);
        MetricValue result = fmax(lhs2, args...);
        return result;
    }
    inline MetricValue fmax(const MetricValue& lhs, double rhs)
    {
        MetricValue result;
        result.sum = ::fmax(lhs.sum, rhs);
        result.avg = ::fmax(lhs.avg, rhs);
        result.peak_sustained = ::fmax(lhs.peak_sustained, rhs);
        result.cycles_elapsed = lhs.cycles_elapsed;
        return result;
    }
    inline MetricValue fmax(double lhs, const MetricValue& rhs)
    {
        MetricValue result;
        result.sum = ::fmax(lhs, rhs.sum);
        result.avg = ::fmax(lhs, rhs.avg);
        result.peak_sustained = ::fmax(lhs, rhs.peak_sustained);
        result.cycles_elapsed = rhs.cycles_elapsed;
        return result;
    }
    inline MetricValue MaxPercent(const MetricValue& lhs, const MetricValue& rhs)
    {
        MetricValue result = (rhs.pct_of_peak_sustained_elapsed() > lhs.pct_of_peak_sustained_elapsed()) ? rhs : lhs; // returns lhs in case of NaN
        return result;
    }
    template <class... TArgs>
    MetricValue MaxPercent(const MetricValue& lhs, const MetricValue& rhs, TArgs... args)
    {
        MetricValue lhs2 = MaxPercent(lhs, rhs);
        MetricValue result = MaxPercent(lhs2, args...);
        return result;
    }


    struct RawMetricsContext
    {
        bool configuring;
        double* pValues;    // points at GPU-specific values[] array, used by Unpack logic
        uint16_t* pCounts;  // points at GPU-specific count[] array, used by Configuration and Unpack logic
    };

    typedef MetricValue(*MetricEvalFn)(RawMetricsContext* pMetricsClass);

    struct MetricDesc
    {
        const char* pName;
        const char* pDesc;
        MetricEvalFn metricEvalFn;
    };

    struct MetricDescComparator
    {
        bool operator()(const MetricDesc& lhs, const MetricDesc& rhs) const
        {
            return strcmp(lhs.pName, rhs.pName) < 0;
        }
        bool operator()(const MetricDesc& lhs, const char* pName) const
        {
            return strcmp(lhs.pName, pName) < 0;
        }
        bool operator()(const char* pName, const MetricDesc& rhs) const
        {
            return strcmp(pName, rhs.pName) < 0;
        }
    };


    template <class T, class... TArgs>
    struct MaxSizeof;

    template <class T>
    struct MaxSizeof<T>
    {
        static const size_t size = sizeof(T);
    };
    
    template <class T, class... TArgs>
    struct MaxSizeof
    {
        static const size_t size = sizeof(T) > MaxSizeof<TArgs...>::size ? sizeof(T) : MaxSizeof<TArgs...>::size;
    };

    template <class... TChipMetrics>
    struct MetricsStorage
    {
        union {
            char buffer[MaxSizeof<TChipMetrics...>::size];  // buffer first so that it takes priority in union initialization
            RawMetricsContext rawMetricsContext;
            double forceAlignDouble;
            uint64_t forceAlignUint64_t;
        };
    };

    template <class TMetrics>
    RawMetricsContext* NewRawMetricsContext(char* pStorageBuffer)
    {
        // placement new into storage
        TMetrics* pContext = new (pStorageBuffer) TMetrics{};
        pContext->pCounts = pContext->counts;
        pContext->pValues = pContext->values;
        pContext->configuring = false;
        return pContext;
    }

    struct ChipDesc
    {
        RawMetricsContext* pRawMetricsContexts[2];
        const uint64_t* pRawMetricIds;
        const char* const* ppRawMetricNames;
        size_t numRawMetricIds;
        nv::metrics::MetricDesc* pMetricDescs;
        size_t numMetricDescs;
        const char* pChipName;
    };

    template <class ChipMetrics>
    void ChipDescInitialize(nv::metrics::ChipDesc& chipDesc, const char* pChipName, char* pStorageBuffer[2], nv::metrics::MetricDesc* pMetricDescs, size_t numMetricDescs)
    {
        for (uint32_t isolated = 0; isolated < 2; ++isolated)
        {
            chipDesc.pRawMetricsContexts[isolated] = NewRawMetricsContext<ChipMetrics>(pStorageBuffer[isolated]);
        }

        chipDesc.pRawMetricIds = ChipMetrics::GetRawMetricIds();
        chipDesc.ppRawMetricNames = ChipMetrics::GetRawMetricNames();
        chipDesc.numRawMetricIds = ChipMetrics::NumRawMetricIds;
        chipDesc.pMetricDescs = pMetricDescs;
        chipDesc.numMetricDescs = numMetricDescs;
        chipDesc.pChipName = pChipName;

        std::sort(pMetricDescs, pMetricDescs + numMetricDescs, nv::metrics::MetricDescComparator());
    }

    inline void ChipDescResetContexts(const ChipDesc& chipDesc, double defaultVal)
    {
        for (uint32_t isolated = 0; isolated < 2; ++isolated)
        {
            RawMetricsContext* pRawMetricsContext = chipDesc.pRawMetricsContexts[isolated];
            pRawMetricsContext->configuring = false;
            for (uint32_t rawMetricIndex = 0; rawMetricIndex < chipDesc.numRawMetricIds; ++rawMetricIndex)
            {
                pRawMetricsContext->pCounts[rawMetricIndex] = 0;
                pRawMetricsContext->pValues[rawMetricIndex] = defaultVal;
            }
        }
    }

    inline const MetricDesc* FindMetricDesc(const ChipDesc& chipDesc, const char* pMetricName)
    {
        const MetricDesc* const pFirst = chipDesc.pMetricDescs;
        const MetricDesc* const pLast = pFirst + chipDesc.numMetricDescs;
        const MetricDesc* pMetricDesc = std::lower_bound(pFirst, pLast, pMetricName, nv::metrics::MetricDescComparator());
        if (pMetricDesc == pLast || strcmp(pMetricDesc->pName, pMetricName))
        {
            return nullptr;
        }
        return pMetricDesc;
    }

}} // namespace nv::metrics

#define NVPA_METRIC_STATIC_INVOKE(name_, desc_) \
    static nv::metrics::MetricValue name_(nv::metrics::RawMetricsContext* pMetricsClass) { return ((NV_METRICS_CLASS*)pMetricsClass)->name_(); }

#define NV_METRICS_INVOKER_NAME2(line_) NvMetrics##line_
#define NV_METRICS_INVOKER_NAME(line_) NV_METRICS_INVOKER_NAME2(line_)

#define NVPA_METRIC_DESC_BRACE_INIT(name_, desc_) \
    { #name_, desc_, & NV_METRICS_INVOKER_NAME(__LINE__) ::name_ },

#define NVPA_DEFINE_METRIC_DESCS(MetricDefinitions_, varname_) \
    struct NV_METRICS_INVOKER_NAME(__LINE__) {                 \
        MetricDefinitions_(NVPA_METRIC_STATIC_INVOKE)          \
    };                                                         \
    static nv::metrics::MetricDesc varname_[] = {              \
        MetricDefinitions_(NVPA_METRIC_DESC_BRACE_INIT)        \
        {nullptr, nullptr, nullptr}                            \
    };                                                         \
    static const size_t varname_##_count = sizeof(varname_) / sizeof(varname_[0]) - 1
