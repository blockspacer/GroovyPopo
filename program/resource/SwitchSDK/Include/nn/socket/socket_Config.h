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
 * @brief   socket library configuration
 */

#pragma once

#include <nn/os.h>
#include <nn/socket/socket_Constants.h>

namespace nn     {
namespace socket {

/**
* @brief    These macros are for internal use only and should not be used directly by the application.
*/
#define NN_DETAIL_SOCKET_MAX(a, b) \
    ((a) > (b) ? (a) : (b))
#define NN_DETAIL_SOCKET_BUFFER_ROUNDUP_TO_MSS(baseSize) \
    ((((baseSize) + 1500 - 1) / 1500) * 1500)
#define NN_DETAIL_SOCKET_BUFFER_ROUNDUP_TO_PAGE(size) \
    static_cast<size_t>(((size) + nn::os::MemoryPageSize - 1) & ~(nn::os::MemoryPageSize - 1))

/**
 * @brief Base class for supplying configuration to the socket library.
 *
 * @details
 *     This class or other classes derived from this class are instantiated,
 *     configured and passed to @ref nn::socket::Initialize().
 *     As many of these parameters are low-level in nature, consider using one of the
 *     specialized versions of this class such as @ref ConfigDefaultWithMemory or
 *     @ref ConfigDefaultWithConstrainedMemory.
 *
 */
class Config
{
public:
    /**
     * @brief    Initialize socket library configuration.
     *
     * @param[in] pMemoryPool
     *     Pointer to the start of the memory region being given to socket library.
     *     This parameter must be 4kB aligned.
     * @param[in] memoryPoolSize
     *     Size of the memory region referred to by @a pMemoryPool.
     *     This parameter must be a multiple of 4kB.
     * @param[in] allocatorPoolSize
     *     Amount of memory which is to be reserved for internal use by the socket library allocator.
     *     The following API make use of this memory reservation: @ref nn::socket::GetAddrInfo,
     *     @ref nn::socket::GetHostEntByAddr, @ref nn::socket::GetHostEntByName, @ref nn::socket::GetNameInfo.
     *     This parameter must be a multiple of 4kB.
     * @param[in] tcpInitialSendBufferSize
     *     Base size of socket TCP send buffers.
     * @param[in] tcpInitialReceiveBufferSize
     *     Base size of TCP receive buffers.
     * @param[in] tcpAutoSendBufferSizeMax
     *     This parameter controls the upper bound to which the TCP send buffer would be allowed to grow.
     *     Set to '0' to disable TCP send buffer auto tuning.
     * @param[in] tcpAutoReceiveBufferSizeMax
     *     This parameter controls the upper bound to which the TCP receive buffer would be allowed to grow.
     *     Set to '0' to disable TCP receive buffer auto tuning.
     * @param[in] udpSendBufferSize
     *     Base size of socket UDP send buffers.
     * @param[in] udpReceiveBufferSize
     *     Base size of socket UDP receive buffers.
     * @param[in] socketBufferEfficiency
     *     Buffer multiplying factor by which socket buffer sizes are scaled up.
     *     The valid range for this parameter is from 1 to 8, inclusive.
     *     A value of 1 may sacrifice some aspects of performance for reduced memory use.
     *     A value of 2 will provide a reasonable tradeoff between performance and memory use.
     *     Applications with demanding performance requirements should consider using a value of at least 4.
     *     This parameter is used to provision FreeBSD's internal "sb_efficiency" setting.
     * @param[in] concurrencyCountMax
     *     Application developers should always pass nn::socket::DefaultConcurrencyLimit, unless instructed otherwise.
     *     This parameter controls the maximum number of synchronously called socket operations which will be allowed to operate
     *     concurrently. Additional calls beyond this limit would be serialized, blocked until a prior operation completes.
     * @details
     *     Memory referred to by @a pMemoryPool must remain allocated until after the socket library
     *     is finalized via call to @ref nn::socket::Finalize().
     *     The minimum allowable memory pool size is defined by nn::socket::MinSocketMemoryPoolSize.
     */

    Config(void* pMemoryPool, size_t memoryPoolSize, size_t allocatorPoolSize,
           size_t tcpInitialSendBufferSize, size_t tcpInitialReceiveBufferSize,
           size_t tcpAutoSendBufferSizeMax, size_t tcpAutoReceiveBufferSizeMax,
           size_t udpSendBufferSize,    size_t udpReceiveBufferSize,
           int socketBufferEfficiency,  int concurrencyCountMax) NN_NOEXCEPT
        : m_Version(LibraryVersion)
        , m_IsSystemClient(false)
        , m_pMemoryPool(pMemoryPool)
        , m_MemoryPoolSize(memoryPoolSize)
        , m_AllocatorPoolSize(allocatorPoolSize)
        , m_TcpInitialSendBufferSize(tcpInitialSendBufferSize)
        , m_TcpInitialReceiveBufferSize(tcpInitialReceiveBufferSize)
        , m_TcpAutoSendBufferSizeMax(tcpAutoSendBufferSizeMax)
        , m_TcpAutoReceiveBufferSizeMax(tcpAutoReceiveBufferSizeMax)
        , m_UdpSendBufferSize(udpSendBufferSize)
        , m_UdpReceiveBufferSize(udpReceiveBufferSize)
        , m_SocketBufferEfficiency(socketBufferEfficiency)
        , m_ConcurrencyCountMax(concurrencyCountMax)
    {
    }

    /**
     * @brief        Get socket library version
     * @return       Value providing socket library version information.
     * @details      This method is not applicable for SDK users.
     */
    uint32_t GetVersion() const NN_NOEXCEPT
    {
       return  m_Version;
    }
    /**
     * @brief        Determine if this is system configuration.
     * @return       Value will always be false for SDK users.
     * @details      This method is not applicable for SDK users.
     */
    bool IsSystemClient() const NN_NOEXCEPT
    {
        return m_IsSystemClient;
    }
    /**
     * @brief        Get pointer to base of memory pool.
     * @return       Pointer to memory pool.
     * @details      For details see @a pMemoryPool parameter passed to @ref Config().
     */
    void* GetMemoryPool() const NN_NOEXCEPT
    {
        return m_pMemoryPool;
    }
    /**
     * @brief        Get size of memory pool.
     * @return       Size of memory pool.
     * @details      For details see @a memoryPoolSize parameter passed to @ref Config().
     */
    size_t GetMemoryPoolSize() const NN_NOEXCEPT
    {
        return m_MemoryPoolSize;
    }
    /**
     * @brief        Get size of allocator pool.
     * @return       Size of allocator pool.
     * @details      For details see @a allocatorPoolSize parameter passed to @ref Config().
     */
    size_t GetAllocatorPoolSize() const NN_NOEXCEPT
    {
        return m_AllocatorPoolSize;
    }
    /**
     * @brief        Get base TCP send buffer size.
     * @return       Size of buffer.
     * @details      For details see @a tcpInitialSendBufferSize parameter passed to @ref Config().
     */
    size_t GetTcpInitialSendBufferSize() const NN_NOEXCEPT
    {
        return m_TcpInitialSendBufferSize;
    }
    /**
     * @brief        Set base TCP send buffer size.
     * @param[in]    size New size to be assumed.
     * @return       None.
     * @details      For details see @a tcpInitialSendBufferSize parameter passed to @ref Config().
     */
    void SetTcpInitialSendBufferSize(size_t size) NN_NOEXCEPT
    {
        m_TcpInitialSendBufferSize = size;
    }
    /**
     * @brief        Get base TCP receive buffer size.
     * @return       Size of buffer.
     * @details      For details see @a tcpInitialReceiveBufferSize parameter passed to @ref Config().
     */
    size_t GetTcpInitialReceiveBufferSize() const NN_NOEXCEPT
    {
        return m_TcpInitialReceiveBufferSize;
    }
    /**
     * @brief        Set base TCP receive buffer size.
     * @param[in]    size New size to be assumed.
     * @return       None.
     * @details      For details see @a tcpInitialReceiveBufferSize parameter passed to @ref Config().
     */
    void SetTcpInitialReceiveBufferSize(size_t size) NN_NOEXCEPT
    {
        m_TcpInitialReceiveBufferSize = size;
    }
    /**
     * @brief        Get TCP send buffer upper bound size.
     * @return       Size of buffer.
     * @details      For details see @a tcpAutoSendBufferSizeMax parameter passed to @ref Config().
     */
    size_t GetTcpAutoSendBufferSizeMax() const NN_NOEXCEPT
    {
        return m_TcpAutoSendBufferSizeMax;
    }
    /**
     * @brief        Set TCP send buffer upper bound size.
     * @param[in]    size New size to be assumed.
     * @return       None.
     * @details      For details see @a tcpAutoSendBufferSizeMax parameter passed to @ref Config().
     */
    void SetTcpAutoSendBufferSizeMax(size_t size) NN_NOEXCEPT
    {
        m_TcpAutoSendBufferSizeMax = size;
    }
    /**
     * @brief        Get TCP send buffer upper bound size.
     * @return       Size of buffer.
     * @details      For details see @a tcpAutoReceiveBufferSizeMax parameter passed to @ref Config().
     */
    size_t GetTcpAutoReceiveBufferSizeMax() const NN_NOEXCEPT
    {
        return m_TcpAutoReceiveBufferSizeMax;
    }
    /**
     * @brief        Set TCP send buffer upper bound size.
     * @param[in]    size New size to be assumed.
     * @return       None.
     * @details      For details see @a tcpAutoReceiveBufferSizeMax parameter passed to @ref Config().
     */
    void SetTcpAutoReceiveBufferSizeMax(size_t size) NN_NOEXCEPT
    {
        m_TcpAutoReceiveBufferSizeMax = size;
    }
    /**
     * @brief        Get UDP default send buffer size.
     * @return       Size of buffer.
     * @details      For details see @a udpSendBufferSize parameter passed to @ref Config().
     */
    size_t GetUdpSendBufferSize() const NN_NOEXCEPT
    {
        return m_UdpSendBufferSize;
    }
    /**
     * @brief        Set UDP default send buffer size.
     * @param[in]    size New size to be assumed.
     * @return       None.
     * @details      For details see @a udpSendBufferSize parameter passed to @ref Config().
     */
    void SetUdpSendBufferSize(size_t size) NN_NOEXCEPT
    {
        m_UdpSendBufferSize = size;
    }
   /**
    * @brief        Get UDP default receive buffer size.
    * @return       Size of buffer.
    * @details      For details see @a udpReceiveBufferSize parameter passed to @ref Config().
    */
    size_t GetUdpReceiveBufferSize() const NN_NOEXCEPT
    {
        return m_UdpReceiveBufferSize;
    }
   /**
    * @brief        Set UDP default receive buffer size.
    * @param[in]    size New size to be assumed.
    * @return       None.
    * @details      For details see @a udpReceiveBufferSize parameter passed to @ref Config().
    */
    void SetUdpReceiveBufferSize(size_t size) NN_NOEXCEPT
    {
        m_UdpReceiveBufferSize = size;
    }
   /**
    * @brief        Get socket buffer efficiency multiplier.
    * @return       Value of multiplier.
    * @details      For details see @a socketBufferEfficiency parameter passed to @ref Config().
    */
    int GetSocketBufferEfficiency() const NN_NOEXCEPT
    {
        return m_SocketBufferEfficiency;
    }
   /**
    * @brief        Set socket buffer efficiency multiplier.
    * @param[in]    socketBufferEfficiency New value to be assumed.
    * @return       None.
    * @details      For details see @a socketBufferEfficiency parameter passed to @ref Config().
    */
    void SetSocketBufferEfficiency(int socketBufferEfficiency) NN_NOEXCEPT
    {
        NN_ABORT_UNLESS((socketBufferEfficiency >= 1) && (socketBufferEfficiency <= 8),
                        "socketBufferEfficiency must have value of 1 to 8");
        m_SocketBufferEfficiency = socketBufferEfficiency;
    }
   /**
    * @brief        Get maximum concurrency count.
    * @return       Count value.
    * @details      For details see @a concurrencyCountMax parameter passed to @ref Config().
    */
    int GetConcurrencyCountMax() const NN_NOEXCEPT
    {
        return m_ConcurrencyCountMax;
    }
   /**
    * @brief        Set maximum concurrency count.
    * @param[in]    concurrencyCountMax New value to be assumed.
    * @return       None.
    * @details      For details see @a concurrencyCountMax parameter passed to @ref Config().
    */
    void SetConcurrencyCountMax(int concurrencyCountMax) NN_NOEXCEPT
    {
        m_ConcurrencyCountMax = concurrencyCountMax;
    }

private:
    uint32_t  m_Version;

protected:
    bool      m_IsSystemClient;
    void*     m_pMemoryPool;
    size_t    m_MemoryPoolSize;
    size_t    m_AllocatorPoolSize;
    size_t    m_TcpInitialSendBufferSize;
    size_t    m_TcpInitialReceiveBufferSize;
    size_t    m_TcpAutoSendBufferSizeMax;
    size_t    m_TcpAutoReceiveBufferSizeMax;
    size_t    m_UdpSendBufferSize;
    size_t    m_UdpReceiveBufferSize;
    int       m_SocketBufferEfficiency;
    int       m_ConcurrencyCountMax;
};



/**
 * @brief    Class allowing for full manual configuration of the socket library. Memory pool is included.
 * @param[in] tcpSocketCountMax
 *     Maximum number of TCP sockets for which memory is to be pre-allocated.
 * @param[in] udpSocketCountMax
 *     Maximum number of UDP sockets for which memory is to be pre-allocated.
 * @param[in] tcpInitialSendBufferSize
 *     Base size of socket TCP send buffers.
 * @param[in] tcpInitialReceiveBufferSize
 *     Base size of TCP receive buffers.
 * @param[in] tcpAutoSendBufferSizeMax
 *     This parameter controls the upper bound to which the TCP send buffer would be allowed to grow.
 *     Set to '0' to disable TCP send buffer auto tuning.
 * @param[in] tcpAutoReceiveBufferSizeMax
 *     This parameter controls the upper bound to which the TCP receive buffer would be allowed to grow.
 *     Set to '0' to disable TCP receive buffer auto tuning.
 * @param[in] udpSendBufferSize
 *     Base size of socket UDP send buffers.
 * @param[in] udpReceiveBufferSize
 *     Base size of socket UDP receive buffers.
 * @param[in] socketBufferEfficiency
 *     Buffer multiplying factor by which socket buffer sizes are scaled up.
 *     The valid range for this parameter is from 1 to 8, inclusive.
 *     A value of 1 may sacrifice some aspects of performance for reduced memory use.
 *     A value of 2 will provide a reasonable tradeoff between performance and memory use.
 *     Applications with demanding performance requirements should consider using a value of at least 4.
 *     This parameter is used to provision FreeBSD's internal "sb_efficiency" setting.
 * @param[in] allocatorPoolSize
 *     Amount of memory which is to be reserved for internal use by the socket library allocator.
 *     The following API make use of this memory reservation: @ref nn::socket::GetAddrInfo,
 *     @ref nn::socket::GetHostEntByAddr, @ref nn::socket::GetHostEntByName, @ref nn::socket::GetNameInfo.
 * @details
 *     It must be instantiated with global or static scope. As this class contains a memory pool,
 *     when instantiated the resulting object may consume several megabytes of memory.
 */
template<int    tcpSocketCountMax,        int    udpSocketCountMax,
         size_t tcpInitialSendBufferSize, size_t tcpInitialReceiveBufferSize,
         size_t tcpAutoSendBufferSizeMax, size_t tcpAutoReceiveBufferSizeMax,
         size_t udpSendBufferSize,        size_t udpReceiveBufferSize,
         int    socketBufferEfficiency,   size_t allocatorPoolSize>
class ConfigWithMemory : public Config
{
public:
    static const size_t PerTcpSocketWorstCaseMemoryPoolSize =
        NN_DETAIL_SOCKET_BUFFER_ROUNDUP_TO_PAGE
        (
            NN_DETAIL_SOCKET_BUFFER_ROUNDUP_TO_MSS
            (
                NN_DETAIL_SOCKET_MAX(tcpInitialSendBufferSize, tcpAutoSendBufferSizeMax)
            ) * socketBufferEfficiency
            +
            NN_DETAIL_SOCKET_BUFFER_ROUNDUP_TO_MSS
            (
               NN_DETAIL_SOCKET_MAX(tcpInitialReceiveBufferSize, tcpAutoReceiveBufferSizeMax)
            ) * socketBufferEfficiency
        );
    static const size_t PerUdpSocketWorstCaseMemoryPoolSize =
        NN_DETAIL_SOCKET_BUFFER_ROUNDUP_TO_PAGE
        (
            NN_DETAIL_SOCKET_BUFFER_ROUNDUP_TO_MSS(udpSendBufferSize) * socketBufferEfficiency
            +
            NN_DETAIL_SOCKET_BUFFER_ROUNDUP_TO_MSS(udpReceiveBufferSize) * socketBufferEfficiency
        );
    static const size_t AllocatorPoolSize = NN_DETAIL_SOCKET_BUFFER_ROUNDUP_TO_PAGE(allocatorPoolSize);

    /**
     * @brief     Initialize socket library configuration.
     *
     * @param[in] concurrencyCountMax
     *     See concurrencyCountMax parameter passed to @ref Config().
     * @details
     *     This is the only constructor for this class.
     */
    NN_IMPLICIT ConfigWithMemory(int concurrencyCountMax) NN_NOEXCEPT
        : Config(m_MemoryPool, sizeof(m_MemoryPool), AllocatorPoolSize,
                 tcpInitialSendBufferSize,
                 tcpInitialReceiveBufferSize,
                 tcpAutoSendBufferSizeMax,
                 tcpAutoReceiveBufferSizeMax,
                 udpSendBufferSize,
                 udpReceiveBufferSize,
                 socketBufferEfficiency,
                 concurrencyCountMax)
    {
        NN_STATIC_ASSERT(tcpSocketCountMax >= 0);
        NN_STATIC_ASSERT(udpSocketCountMax >= 0);
        NN_STATIC_ASSERT((udpSocketCountMax != 0) || (tcpSocketCountMax != 0));
        NN_STATIC_ASSERT((socketBufferEfficiency >= 1) && (socketBufferEfficiency <= 8));
    }

private:
#if defined NN_BUILD_CONFIG_OS_WIN32
    uint8_t m_MemoryPool[1];
#else
    uint8_t m_MemoryPool[NN_DETAIL_SOCKET_MAX((PerTcpSocketWorstCaseMemoryPoolSize * tcpSocketCountMax) +
                                              (PerUdpSocketWorstCaseMemoryPoolSize * udpSocketCountMax) +
                                              AllocatorPoolSize,
                                              MinSocketMemoryPoolSize)] NN_ALIGNAS(nn::socket::MemoryPoolAlignment);
#endif
};


/**
 * @brief   Class specifying a default configuration. No memory pool is included.
 * @details
 *           This class should be used directly only if the application has the need
 *           to customize memory parameters. Consider using @ref ConfigDefaultWithMemory or
 *           @ref ConfigDefaultWithConstrainedMemory instead.
 *           The minimum allowable memory pool size is defined by nn::socket::MinSocketMemoryPoolSize.
 */
class ConfigDefault : public Config
{
public:
    static const size_t DefaultTcpInitialSendBufferSize    = 1024 * 32;
    static const size_t DefaultTcpInitialReceiveBufferSize = 1024 * 64;
    static const size_t DefaultTcpAutoSendBufferSizeMax    = DefaultTcpAutoBufferSizeMax;
    static const size_t DefaultTcpAutoReceiveBufferSizeMax = DefaultTcpAutoBufferSizeMax;
    static const size_t DefaultUdpSendBufferSize           = 1024 * 9;
    static const size_t DefaultUdpReceiveBufferSize        = 42240;
    static const int    DefaultSocketBufferEfficiency      = 4;
    static const size_t PerTcpSocketWorstCaseMemoryPoolSize =
        NN_DETAIL_SOCKET_BUFFER_ROUNDUP_TO_PAGE
        (
            NN_DETAIL_SOCKET_BUFFER_ROUNDUP_TO_MSS
            (
                NN_DETAIL_SOCKET_MAX(DefaultTcpInitialSendBufferSize, DefaultTcpAutoSendBufferSizeMax)
            ) * DefaultSocketBufferEfficiency
            +
            NN_DETAIL_SOCKET_BUFFER_ROUNDUP_TO_MSS
            (
               NN_DETAIL_SOCKET_MAX(DefaultTcpInitialReceiveBufferSize, DefaultTcpAutoReceiveBufferSizeMax)
            ) * DefaultSocketBufferEfficiency
        );
    static const size_t PerUdpSocketWorstCaseMemoryPoolSize =
        NN_DETAIL_SOCKET_BUFFER_ROUNDUP_TO_PAGE
        (
            NN_DETAIL_SOCKET_BUFFER_ROUNDUP_TO_MSS(DefaultUdpSendBufferSize) * DefaultSocketBufferEfficiency
            +
            NN_DETAIL_SOCKET_BUFFER_ROUNDUP_TO_MSS(DefaultUdpReceiveBufferSize) * DefaultSocketBufferEfficiency
        );

    /**
     * @brief    Initialize default socket library configuration.
     *
     * @param[in] pMemoryPool
     *     See @a pMemoryPool parameter passed to @ref Config().
     * @param[in] memoryPoolSize
     *     See @a memoryPoolSize parameter passed to @ref Config().
     * @param[in] allocatorPoolSize
     *     See @a allocatorSize parameter passed to @ref Config().
     * @details
     *     Memory referred to by @a pMemoryPool must remain allocated until after the socket library
     *     is finalized via call to @ref nn::socket::Finalize().
     */
    ConfigDefault(void* pMemoryPool, size_t memoryPoolSize, size_t allocatorPoolSize) NN_NOEXCEPT
        : Config(pMemoryPool, memoryPoolSize, allocatorPoolSize,
                 DefaultTcpInitialSendBufferSize,
                 DefaultTcpInitialReceiveBufferSize,
                 DefaultTcpAutoSendBufferSizeMax,
                 DefaultTcpAutoReceiveBufferSizeMax,
                 DefaultUdpSendBufferSize,
                 DefaultUdpReceiveBufferSize,
                 DefaultSocketBufferEfficiency,
                 DefaultConcurrencyLimit)
    {
    }
};

/**
 * @brief Class specifying a default configuration. Custom sized memory pool is included.
 * @param[in] tcpSocketCountMax
 *     Maximum number of TCP sockets for which memory is to be pre-allocated.
 * @param[in] udpSocketCountMax
 *     Maximum number of UDP sockets for which memory is to be pre-allocated.
 * @param[in] socketBufferEfficiency
 *     Buffer multiplying factor by which socket buffer sizes are scaled up.
 *     The valid range for this parameter is from 1 to 8, inclusive.
 *     A value of 1 may sacrifice some aspects of performance for reduced memory use.
 *     A value of 2 will provide a reasonable tradeoff between performance and memory use.
 *     Applications with demanding performance requirements should consider using a value of at least 4.
 *     This parameter is used to provision FreeBSD's internal "sb_efficiency" setting.
 * @details
 *     Default values provided by this class are adequate for most applications.
 *     It must be instantiated with global or static scope. As this class contains a memory pool,
 *     when instantiated the resulting object may consume several megabytes of memory.
 */
template<int tcpSocketCountMax, int udpSocketCountMax,
         int socketBufferEfficiency=ConfigDefault::DefaultSocketBufferEfficiency>
class ConfigDefaultWithConstrainedMemory : public ConfigDefault
{
public:
    static const size_t PerTcpSocketWorstCaseMemoryPoolSize =
        NN_DETAIL_SOCKET_BUFFER_ROUNDUP_TO_PAGE
        (
            NN_DETAIL_SOCKET_BUFFER_ROUNDUP_TO_MSS
            (
                NN_DETAIL_SOCKET_MAX(DefaultTcpInitialSendBufferSize, DefaultTcpAutoSendBufferSizeMax)
            ) * socketBufferEfficiency
            +
            NN_DETAIL_SOCKET_BUFFER_ROUNDUP_TO_MSS
            (
               NN_DETAIL_SOCKET_MAX(DefaultTcpInitialReceiveBufferSize, DefaultTcpAutoReceiveBufferSizeMax)
            ) * socketBufferEfficiency
        );
    static const size_t PerUdpSocketWorstCaseMemoryPoolSize =
        NN_DETAIL_SOCKET_BUFFER_ROUNDUP_TO_PAGE
        (
            NN_DETAIL_SOCKET_BUFFER_ROUNDUP_TO_MSS(DefaultUdpSendBufferSize) * socketBufferEfficiency
            +
            NN_DETAIL_SOCKET_BUFFER_ROUNDUP_TO_MSS(DefaultUdpReceiveBufferSize) * socketBufferEfficiency
        );

    /**
     * @brief    Initialize default socket library configuration.
     *
     * @details
     *     This constructor, which accepts no arguments, will be adequate for most
     *     applications using the socket library.
     */
    NN_IMPLICIT ConfigDefaultWithConstrainedMemory() NN_NOEXCEPT
        : ConfigDefault(m_MemoryPool,
                        sizeof(m_MemoryPool),
                        DefaultSocketAllocatorSize)
    {
        CheckParameters();
        SetSocketBufferEfficiency(socketBufferEfficiency);
    }

    /**
     * @brief    Initialize default socket library configuration, with customized
     *           concurrency.
     *
     * @param[in] concurrencyCountMax
     *     See @a concurrencyCountMax parameter passed to @ref Config().
     * @details
     *     This constructor should be used only if your application has special requirements affecting
     *     socket library concurrency.
     */
    NN_IMPLICIT ConfigDefaultWithConstrainedMemory(int concurrencyCountMax) NN_NOEXCEPT
        : ConfigDefault(m_MemoryPool,
                        sizeof(m_MemoryPool),
                        DefaultSocketAllocatorSize)
    {
        CheckParameters();
        SetConcurrencyCountMax(concurrencyCountMax);
        SetSocketBufferEfficiency(socketBufferEfficiency);
    }

private:
    void CheckParameters()
    {
        NN_STATIC_ASSERT(tcpSocketCountMax >= 0);
        NN_STATIC_ASSERT(udpSocketCountMax >= 0);
        NN_STATIC_ASSERT((udpSocketCountMax != 0) || (tcpSocketCountMax != 0));
        NN_STATIC_ASSERT((socketBufferEfficiency >= 1) && (socketBufferEfficiency <= 8));
    }

    #if defined NN_BUILD_CONFIG_OS_WIN32
    uint8_t m_MemoryPool[1];
    #else
    uint8_t m_MemoryPool
        [
            NN_DETAIL_SOCKET_MAX((PerTcpSocketWorstCaseMemoryPoolSize * tcpSocketCountMax) +
                                 (PerUdpSocketWorstCaseMemoryPoolSize * udpSocketCountMax) +
                                 DefaultSocketAllocatorSize,
                                 MinSocketMemoryPoolSize)
        ] NN_ALIGNAS(nn::socket::MemoryPoolAlignment);
    #endif
};

/**
 * @brief Class specifying a default configuration. Fixed size memory pool is included.
 *
 * @details
 *     Default values provided by this class are adequate for most applications.
 *     It must be instantiated with global or static scope. As this class contains a memory pool,
 *     when instantiated the resulting object may consume several megabytes of memory.
 *
 */
class ConfigDefaultWithMemory : public ConfigDefault
{
public:
    /**
     * @brief    Initialize default socket library configuration.
     *
     * @details
     *     This constructor, which accepts no arguments, will be adequate for most
     *     applications using the socket library.
     */
    NN_IMPLICIT ConfigDefaultWithMemory() NN_NOEXCEPT
        : ConfigDefault(m_MemoryPool,
                        sizeof(m_MemoryPool),
                        DefaultSocketAllocatorSize)
    {
    }

    /**
     * @brief    Initialize default socket library configuration, with customized
     *           concurrency.
     *
     * @param[in] concurrencyCountMax
     *     See @a concurrencyCountMax parameter passed to @ref Config().
     * @details
     *     This constructor should be used only if your application has special requirements affecting
     *     socket library concurrency.
     */
    NN_IMPLICIT ConfigDefaultWithMemory(int concurrencyCountMax) NN_NOEXCEPT
        : ConfigDefault(m_MemoryPool,
                        sizeof(m_MemoryPool),
                        DefaultSocketAllocatorSize)
    {
        SetConcurrencyCountMax(concurrencyCountMax);
    }

private:
    #if defined NN_BUILD_CONFIG_OS_WIN32
    uint8_t m_MemoryPool[1];
    #else
    uint8_t m_MemoryPool[DefaultSocketMemoryPoolSize] NN_ALIGNAS(nn::socket::MemoryPoolAlignment);
    #endif
};

}} /* nn::socket */
