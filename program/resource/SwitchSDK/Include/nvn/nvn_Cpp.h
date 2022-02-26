/*--------------------------------------------------------------------------------*
  Copyright (C)Nintendo All rights reserved.

  These coded instructions, statements, and computer programs contain proprietary
  information of Nintendo and/or its licensed developers and are protected by
  national and international copyright laws. They may not be disclosed to third
  parties or copied or duplicated in any form, in whole or in part, without the
  prior written consent of Nintendo.

  The content herein is highly confidential and should be handled accordingly.
 *--------------------------------------------------------------------------------*/


#ifndef __nvn_cpp_h_
#define __nvn_cpp_h_

/// \file nvn_Cpp.h
///     C++ headers for the NVN 3D API.
/// \defgroup nvn_cpp_interface NVN C++ API Bindings
///     C++ Interface for NVN API version 53.311
/// \{

/// \defgroup nvn_cpp_apiclasses API Classes
///     C++ Interface class types.
/// \defgroup nvn_cpp_api_typedefs API Basic Types
///     C++ Interface basic data types.
/// \defgroup nvn_cpp_enum API Enum Types
///     C++ Interface enumerated types.
/// \defgroup nvn_cpp_handle API Handle Types
///     C++ Interface handles for GPU resources.
/// \defgroup nvn_cpp_struct API Structure Types
///     C++ Interface structure data types.
/// \defgroup nvn_cpp_funcptrs API Function Pointer Types
///     C++ Interface function pointer data types.
/// \defgroup nvn_cpp_defines API #defines
///     C++ Interface miscellaneous \#defines.
/// \defgroup nvn_cpp_entryfuncptrs API Entry Point Function Pointers
///     C++ Interface function pointer types for API entry points.

#include <stddef.h>

// Clients using older MSVC versions without support for stdint.h need
// to supply their own versions.
#if (defined(_MSC_VER) && (_MSC_VER >= 1600)) || defined(__GNUC__)
#include <stdint.h>
#endif

#if (defined(_MSC_VER) || defined(__clang__) || \
     (defined(__GNUC__) && !defined(__sun__) && !defined(__FreeBSD__)))
#pragma pack(push, 8)
#endif

/// \addtogroup nvn_cpp_defines
/// \{

/// \brief Major version of the NVN API.
#define NVN_API_MAJOR_VERSION                                       53

/// \brief Minor version of the NVN API.
#define NVN_API_MINOR_VERSION                                       311

/// \}

#ifndef NVNAPIENTRY
# ifdef _WIN32
#  if (_MSC_VER >= 800) || defined(_STDCALL_SUPPORTED)  /* Mimic <windef.h> */
#   define NVNAPIENTRY __stdcall
#  else
#   define NVNAPIENTRY
#  endif
# else
#  define NVNAPIENTRY
# endif
#endif

#ifndef NVNAPI
# if defined(__GNUC__)
#  define NVNAPI extern __attribute__ ((visibility ("default")))
# else
#  define NVNAPI extern
# endif
#endif

#ifndef NVNAPIENTRYP
# define NVNAPIENTRYP NVNAPIENTRY *
#endif

#ifndef NVN_ENUM_32BIT
#define NVN_ENUM_32BIT(ENUMTYPE)    NVN_##ENUMTYPE##_LARGE = 0x7FFFFFFF
#endif

/*************************************************************/

/// \addtogroup nvn_cpp_api_typedefs
/// \{

typedef uint8_t NVNboolean;

/// \}

#ifndef NVN_PRE_ALIGN
  #if defined(_MSC_VER)
    #define NVN_PRE_ALIGN(ALIGN)    __declspec(align(ALIGN))
  #elif defined(__GNUC__) || defined(__clang__)
    #define NVN_PRE_ALIGN(ALIGN)
  #else
    #error "Update NVN alignment macros for this compiler."
  #endif
#endif
#ifndef NVN_POST_ALIGN
  #if defined(_MSC_VER)
    #define NVN_POST_ALIGN(ALIGN)
  #elif defined(__GNUC__) || defined(__clang__)
    #define NVN_POST_ALIGN(ALIGN)   __attribute__((aligned(ALIGN)))
  #else
    #error "Update NVN alignment macros for this compiler."
  #endif
#endif

#ifndef NVN_PRE_DEPRECATED
  #if defined(_MSC_VER)
    #define NVN_PRE_DEPRECATED    __declspec(deprecated)
  #elif defined(__GNUC__) || defined(__clang__)
    #define NVN_PRE_DEPRECATED
  #else
    #error "Update NVN deprecated macros for this compiler."
  #endif
#endif
#ifndef NVN_POST_DEPRECATED
  #if defined(_MSC_VER)
    #define NVN_POST_DEPRECATED
  #elif defined(__GNUC__) || defined(__clang__)
    #define NVN_POST_DEPRECATED   __attribute__((deprecated))
  #else
    #error "Update NVN deprecated macros for this compiler."
  #endif
#endif

/// \addtogroup nvn_cpp_api_typedefs
/// \{

/// \brief Platform native window type.
typedef void * NVNnativeWindow;

/// \brief A pointer to a subroutine linkage map from the offline complier.
typedef void * NVNsubroutineLinkageMapPtr;

/// \brief Defines a storage class used for memory mappings in GPU virtual
/// memory pools.
///
/// When memory from a physical memory pool is mapped into a virtual memory
/// pool, the mapping must be established with a storage class appropriate
/// for the texture or buffer resources using the virtual memory.  For
/// textures, the required storage class can be determined by calling
/// nvn::TextureBuilder::GetStorageClass or nvn::Texture::GetStorageClass.
/// For buffers, NVN_STORAGE_CLASS_BUFFER should be used.
typedef int NVNstorageClass;

/// \brief Helps keep track of domains in debug groups.
typedef int NVNdebugDomainId;


/// \}



/// Main namespace for the NVN 3D API.
namespace nvn {

// Macro defining common methods for NVN C++ enumerant classes.
// "Class" is the C++ class, "Enum" is the nested enum definition,
// and "CEnum" is the equivalent C enum.
#define NVN_ENUM_CLASS(Class, CEnum)                         \
private:                                                     \
    Enum m_value;                                            \
public:                                                      \
    Class() {}                                               \
    Class(const Class &other) : m_value(other.m_value) {}    \
    Class(Enum other)         : m_value(other) {}            \
    operator Enum() const                                    \
        { return (Enum) m_value; }                           \
    inline bool operator == (const Class &other) const       \
        { return m_value == other.m_value; }                 \
    inline bool operator != (const Class &other) const       \
        { return m_value != other.m_value; }                 \
    inline bool operator == (const Enum &other) const        \
        { return m_value == other; }                         \
    inline bool operator != (const Enum &other) const        \
        { return m_value != other; }

// Macro defining common methods for NVN C++ bitfield classes.
// "Class" is the C++ class, "Enum" is the nested enum definition,
// and "CEnum" is the equivalent C enum.
#define NVN_BITFIELD_CLASS(Class, CEnum)                     \
private:                                                     \
    int m_value;                                             \
public:                                                      \
    Class() {}                                               \
    Class(const Class &other)  : m_value(other.m_value) {}   \
    Class(int other)           : m_value(Enum(other)) {}     \
    Class(Enum other)          : m_value(other) {}           \
    operator int() const                                     \
        { return m_value; }                                  \
    inline bool operator == (const Class &other) const       \
        { return m_value == other.m_value; }                 \
    inline bool operator != (const Class &other) const       \
        { return m_value != other.m_value; }                 \
    inline bool operator == (const Enum &other) const        \
        { return m_value == int(other); }                    \
    inline bool operator != (const Enum &other) const        \
        { return m_value != int(other); }                    \
    inline Class operator | (const Class &other) const       \
        { return Class(m_value | other.m_value); }           \
    inline Class operator & (const Class &other) const       \
        { return Class(m_value & other.m_value); }           \
    inline Class operator ^ (const Class &other) const       \
        { return Class(m_value ^ other.m_value); }           \
    inline Class & operator |= (const Class &other)          \
        { m_value |= other.m_value; return *this; }          \
    inline Class & operator &= (const Class &other)          \
        { m_value &= other.m_value; return *this; }          \
    inline Class & operator ^= (const Class &other)          \
        { m_value ^= other.m_value; return *this; }          \
    inline Class operator | (const Enum &other) const        \
        { return Class(m_value | int(other)); }              \
    inline Class operator & (const Enum &other) const        \
        { return Class(m_value & int(other)); }              \
    inline Class operator ^ (const Enum &other) const        \
        { return Class(m_value ^ int(other)); }              \
    inline Class & operator |= (const Enum &other)           \
        { m_value |= other; return *this; }                  \
    inline Class & operator &= (const Enum &other)           \
        { m_value &= other; return *this; }                  \
    inline Class & operator ^= (const Enum &other)           \
        { m_value ^= other; return *this; }                  \

// Forward declaration of API objects in the "nvn" namespace.
/// \addtogroup nvn_cpp_apiclasses
/// \{

class DeviceBuilder;
class Device;
class QueueBuilder;
class Queue;
class CommandBuffer;
class BlendState;
class ChannelMaskState;
class ColorState;
class DepthStencilState;
class MultisampleState;
class PolygonState;
class VertexAttribState;
class VertexStreamState;
class Program;
class MemoryPoolBuilder;
class MemoryPool;
class TexturePool;
class SamplerPool;
class BufferBuilder;
class Buffer;
class Texture;
class TextureBuilder;
class TextureView;
class SamplerBuilder;
class Sampler;
class Sync;
class Window;
class WindowBuilder;
class EventBuilder;
class Event;

/// \}

#ifdef NVN_OVERLOAD_CPP_OBJECTS

// If NVN_OVERLOAD_CPP_OBJECTS is defined prior to including this
// header, the classes defined below will be embedded in the "nvn::objects"
// namespace, allowing applications to define their own classes in "nvn".
// If not defined, the classes will be embedded in "nvn".

// Forward declaration of API objects in the "nvn::objects" namespace.
namespace objects {
    class DeviceBuilder;
    class Device;
    class QueueBuilder;
    class Queue;
    class CommandBuffer;
    class BlendState;
    class ChannelMaskState;
    class ColorState;
    class DepthStencilState;
    class MultisampleState;
    class PolygonState;
    class VertexAttribState;
    class VertexStreamState;
    class Program;
    class MemoryPoolBuilder;
    class MemoryPool;
    class TexturePool;
    class SamplerPool;
    class BufferBuilder;
    class Buffer;
    class Texture;
    class TextureBuilder;
    class TextureView;
    class SamplerBuilder;
    class Sampler;
    class Sync;
    class Window;
    class WindowBuilder;
    class EventBuilder;
    class Event;
} // namespace nvn::objects

#endif

// NVN_OBJECT:  Macro used to nest NVN API objects into structures in
// the "nvn" namespace.  The API objects used should be in the "nvn"
// or "nvn::objects" namespace, depending on NVN_OVERLOAD_CPP_OBJECTS.
#ifdef NVN_OVERLOAD_CPP_OBJECTS
#define NVN_OBJECT(_type)   objects::_type
#else
#define NVN_OBJECT(_type)   _type
#endif

/// \addtogroup nvn_cpp_handle
/// \{

/// \brief GPU address of memory inside a buffer object.
typedef uint64_t BufferAddress;

/// \brief GPU handle used to refer to a command buffer object.
typedef uint64_t CommandHandle;

/// \brief GPU handle used to refer to a texture object for texture mapping
/// operations.
typedef uint64_t TextureHandle;

/// \brief GPU handle used to refer to a texture object for image load and
/// store operations.
typedef uint64_t ImageHandle;

/// \brief GPU address of memory inside a buffer object.
///
/// \warning Except for buffer and linear textures, a TextureAddress
/// returned by Texture::GetTextureAddress corresponds to a special memory
/// mapping that can not be used to view or manipulate memory in commands
/// accepting a buffer address.
typedef uint64_t TextureAddress;

/// \}
/// \addtogroup nvn_cpp_struct
/// \{

/// \brief Structure containing a GPU handle used to refer to a separate
/// sampler object for texture mapping operations.
///
/// To use a separate sampler handle to perform a texture lookup in a
/// shader, it must be combined with a separate texture handle in shader
/// code.
struct SeparateSamplerHandle {

    /// \brief GPU handle used to refer to a separate sampler object for
    /// texture mapping operations.
    uint64_t    value;

};

/// \brief Structure containing a GPU handle used to refer to a separate
/// texture object for texture mapping operations.
///
/// To use a separate texture handle to perform a texture lookup in a
/// shader, it must be combined with a separate sampler handle in shader
/// code.
struct SeparateTextureHandle {

    /// \brief GPU handle used to refer to a separate texture object for
    /// texture mapping operations.
    uint64_t    value;

};

/// \}
/// \addtogroup nvn_cpp_enum
/// \{

/// \brief Identifies a property of the device to query.
struct DeviceInfo {

    /// \brief Enumerated values for the type DeviceInfo.
    enum Enum {

        /// \brief Identifies the major version of the API.
        ///
        /// Major API version can be queried before creating an NVN device
        /// by passing a NULL device pointer to nvn::Device::GetInteger.
        API_MAJOR_VERSION = 0,

        /// \brief Identifies the minor version of the API.
        ///
        /// Minor API version can be queried before creating an NVN device
        /// by passing a NULL device pointer to nvn::Device::GetInteger.
        API_MINOR_VERSION = 1,

        /// \brief Number of uniform buffer bindings supported for each
        /// shader stage.
        UNIFORM_BUFFER_BINDINGS_PER_STAGE = 2,

        /// \brief Maximum size (in bytes) of a bound uniform buffer.
        MAX_UNIFORM_BUFFER_SIZE = 3,

        /// \brief Minimum alignment of uniform buffer bindings.
        UNIFORM_BUFFER_ALIGNMENT = 4,

        /// \brief Number of color buffer binding points.
        COLOR_BUFFER_BINDINGS = 5,

        /// \brief Number of vertex buffer binding points.
        VERTEX_BUFFER_BINDINGS = 6,

        /// \brief Number of transform feedback buffer binding points.
        TRANSFORM_FEEDBACK_BUFFER_BINDINGS = 7,

        /// \brief Number of shader storage buffer bindings supported for
        /// each shader stage.
        SHADER_STORAGE_BUFFER_BINDINGS_PER_STAGE = 8,

        /// \brief Number of texture and sampler bindings supported for
        /// each shader stage.
        TEXTURE_BINDINGS_PER_STAGE = 9,

        /// \brief Minimum alignment for counter reports.
        COUNTER_ALIGNMENT = 10,

        /// \brief Minimum alignment of transform feedback buffer bindings.
        TRANSFORM_FEEDBACK_BUFFER_ALIGNMENT = 11,

        /// \brief Minimum alignment of transform feedback control data.
        TRANSFORM_FEEDBACK_CONTROL_ALIGNMENT = 12,

        /// \brief Minimum alignment of indirect draw data.
        INDIRECT_DRAW_ALIGNMENT = 13,

        /// \brief Number of vertex attributes.
        VERTEX_ATTRIBUTES = 14,

        /// \brief Size (in bytes) of a single texture descriptor stored in
        /// the pool.
        TEXTURE_DESCRIPTOR_SIZE = 15,

        /// \brief Size in bytes of a single sampler descriptor.
        SAMPLER_DESCRIPTOR_SIZE = 16,

        /// \brief Number of entries reserved at the beginning of the
        /// texture pool for internal driver usage.
        RESERVED_TEXTURE_DESCRIPTORS = 17,

        /// \brief Number of samplers that must be reserved for use by the
        /// driver.
        RESERVED_SAMPLER_DESCRIPTORS = 18,

        /// \brief Minimum alignment for command data in a command builder.
        COMMAND_BUFFER_COMMAND_ALIGNMENT = 19,

        /// \brief Minimum alignment for control data in a command builder.
        COMMAND_BUFFER_CONTROL_ALIGNMENT = 20,

        /// \brief Minimum size (in bytes) recommended for command data in
        /// a command builder.
        COMMAND_BUFFER_MIN_COMMAND_SIZE = 21,

        /// \brief Minimum size (in bytes) recommended for control data in
        /// a command builder.
        COMMAND_BUFFER_MIN_CONTROL_SIZE = 22,

        /// \brief Minimum scale factor for providing scratch memory for
        /// non-compute shaders.
        ///
        /// The minimum total amount of memory required to successfully run
        /// a non-compute shader can be computed by multiplying the amount
        /// of memory required for each shader 'warp' (collection of 32
        /// threads) by this factor, and padding to a multiple of the value
        /// of the property
        /// nvn::DeviceInfo::SHADER_SCRATCH_MEMORY_GRANULARITY.  The
        /// per-warp scratch memory usage for a shader is provided in the
        /// shader compiler output.
        ///
        /// \note The minimum scaling factor may be different for compute
        /// and non-compute shaders.
        SHADER_SCRATCH_MEMORY_SCALE_FACTOR_MINIMUM = 23,

        /// \brief Scale factor for the recommended amount of memory
        ///
        /// The recommended total amount of memory required to successfully
        /// run a non-compute shader can be computed by multiplying the
        /// amount of memory required for each shader 'warp' (collection of
        /// 32 threads) by this factor, and then padding to a multiple of
        /// the value of the property
        /// nvn::DeviceInfo::SHADER_SCRATCH_MEMORY_GRANULARITY.  The
        /// per-warp scratch memory usage for a shader, as well as the
        /// recommended total memory size for NX, are provided in the
        /// shader compiler output.
        SHADER_SCRATCH_MEMORY_SCALE_FACTOR_RECOMMENDED = 24,

        /// \brief Alignment required (in bytes) for the shader scratch
        /// memory.
        SHADER_SCRATCH_MEMORY_ALIGNMENT = 25,

        /// \brief Required granularity for shader scratch memory provided
        /// to NVN.
        SHADER_SCRATCH_MEMORY_GRANULARITY = 26,

        /// \brief Maximum sampler anisotropy.
        MAX_TEXTURE_ANISOTROPY = 27,

        /// \brief Maximum number of threads in the X dimension supported
        /// in a compute work group.
        MAX_COMPUTE_WORK_GROUP_SIZE_X = 28,

        /// \brief Maximum number of threads in the Y dimension supported
        /// in a compute work group.
        MAX_COMPUTE_WORK_GROUP_SIZE_Y = 29,

        /// \brief Maximum number of threads in the Z dimension supported
        /// in a compute work group.
        MAX_COMPUTE_WORK_GROUP_SIZE_Z = 30,

        /// \brief Maximum total number of threads supported in a compute
        /// work group.
        MAX_COMPUTE_WORK_GROUP_SIZE_THREADS = 31,

        /// \brief Maximum number of work groups in the X dimension
        /// supported in a compute dispatch.
        MAX_COMPUTE_DISPATCH_WORK_GROUPS_X = 32,

        /// \brief Maximum number of work groups in the Y dimension
        /// supported in a compute dispatch.
        MAX_COMPUTE_DISPATCH_WORK_GROUPS_Y = 33,

        /// \brief Maximum number of work groups in the Z dimension
        /// supported in a compute dispatch.
        MAX_COMPUTE_DISPATCH_WORK_GROUPS_Z = 34,

        /// \brief Number of image bindings supported for each shader
        /// stage.
        IMAGE_BINDINGS_PER_STAGE = 35,

        /// \brief Maximum number of entries (including reserved ones)
        /// supported in a texture descriptor pool.
        MAX_TEXTURE_POOL_SIZE = 36,

        /// \brief Maximum number of entries (including reserved ones)
        /// supported in a sampler descriptor pool.
        MAX_SAMPLER_POOL_SIZE = 37,

        /// \brief Maximum number of viewports.
        MAX_VIEWPORTS = 38,

        /// \brief Granularity of memory pool regions (pages or sub-page
        /// units), where each region may not contain both texture and
        /// buffer resources.
        ///
        /// For textures (other than those with a target of
        /// TEXTURE_TARGET::TARGET_BUFFER), the GPU memory subsystem
        /// arranges the texture contents in its GPU memory pages for
        /// optimal performance.  This arrangment can cause memory
        /// addresses used by a texture and a buffer in the same page to
        /// collide, even if their nominal ranges within the pool don't
        /// overlap.  Applications must not place texture and buffer
        /// resources within any 4KB-aligned region of the memory pool.  On
        /// some GPUs supported by the Windows reference implementation,
        /// these aligned regions are 64KB in size.
        MEMPOOL_TEXTURE_OBJECT_PAGE_ALIGNMENT = 39,

        /// \brief 1 if SamplerReduction::MIN and SamplerReduction::MAX are
        /// supported by the NVN implementation; 0 otherwise.
        ///
        /// These modes are not supported on the Windows reference
        /// implementation for GPUs older than second-generation Maxwell
        /// GPUs.
        SUPPORTS_MIN_MAX_FILTERING = 40,

        /// \brief 1 if Format::STENCIL8 is supported by the NVN
        /// implementation; 0 otherwise.
        ///
        /// This format is not supported on the Windows reference
        /// implementation for GPUs older than second-generation Maxwell
        /// GPUs.
        SUPPORTS_STENCIL8_FORMAT = 41,

        /// \brief 1 if ASTC texture formats are supported by the NVN
        /// implementation; 0 otherwise.
        ///
        /// Some NVN implementations do not natively support ASTC texture
        /// compression. For the NVN Windows reference implementation,
        /// limited ASTC support is emulated in the driver. For details,
        /// please refer to the
        /// \ref nvn_guide_formats_restrictions_on_Windows
        /// "Formats Restrictions on the NVN Windows Reference
        /// Implementation"
        /// section of the Programming Guide.
        SUPPORTS_ASTC_FORMATS = 42,

        /// \brief Total size of L2 cache in the GPU core.
        L2_SIZE = 43,

        /// \brief Maximum number of levels supported for any texture
        /// target.
        MAX_TEXTURE_LEVELS = 44,

        /// \brief Maximum number of layers supported for any array texture
        /// target.
        ///
        /// For cube map arrays, this value should be divided by 6 to
        /// obtain the number of cube map array layers supported; i.e., the
        /// limit applies to the total number of cubemap faces in the
        /// texture.
        MAX_TEXTURE_LAYERS = 45,

        /// \brief Maximum supported GLSLC GPU code section binary major
        /// version.
        ///
        /// For precompiled shaders produced with the offline shader
        /// compiler, GLSLC, this represents the maximum major version
        /// number of the GPU code section binary that NVN can support.  It
        /// is an error to try to import a binary whose major/minor
        /// versions are greater than the maximum supported version.
        GLSLC_MAX_SUPPORTED_GPU_CODE_MAJOR_VERSION = 46,

        /// \brief Minimum supported GLSLC binary version.
        ///
        /// For precompiled shaders produced with the offline shader
        /// compiler, GLSLC, this represents the minimum major version
        /// number of the GPU code section binary that NVN can support.  It
        /// is an error to try to import a binary whose major/minor
        /// versions are less than the minimum supported version.
        GLSLC_MIN_SUPPORTED_GPU_CODE_MAJOR_VERSION = 47,

        /// \brief Maximum supported GLSLC binary version.
        ///
        /// For precompiled shaders produced with the offline shader
        /// compiler, GLSLC, this represents the maximum minor version
        /// number of the GPU code section binary that NVN can support.  It
        /// is an error to try to import a binary whose major/minor
        /// versions are greater than the maximum supported version.
        GLSLC_MAX_SUPPORTED_GPU_CODE_MINOR_VERSION = 48,

        /// \brief Minimum supported GLSLC binary version.
        ///
        /// For precompiled shaders produced with the offline shader
        /// compiler, GLSLC, this represents the minimum minor version
        /// number of the GPU code section binary that NVN can support.  It
        /// is an error to try to import a binary whose major/minor
        /// versions are less than the minimum supported version.
        GLSLC_MIN_SUPPORTED_GPU_CODE_MINOR_VERSION = 49,

        /// \brief 1 if conservative rasterization is supported supported
        /// by the NVN implementation; 0 otherwise.
        ///
        /// Conservative rasterization requires a second-generation or
        /// newer Maxwell GPU.
        SUPPORTS_CONSERVATIVE_RASTER = 50,

        /// \brief The number of bits of subpixel precision used to
        /// represent vertex window coordinates.
        SUBPIXEL_BITS = 51,

        /// \brief The maximum number of additional bits of subpixel
        /// precision that can be used to represent vertex window
        /// coordinates during conservative rasterization.
        MAX_SUBPIXEL_BIAS_BITS = 52,

        /// \brief Minimum alignment of indirect dispatch data.
        INDIRECT_DISPATCH_ALIGNMENT = 53,

        /// \brief Minimum alignment for ZCull save/restore buffers.
        ZCULL_SAVE_RESTORE_ALIGNMENT = 54,

        /// \brief Minimum scale factor for providing scratch memory for
        /// compute shaders.
        ///
        /// The minimum total amount of memory required to successfully run
        /// a compute shader can be computed by multiplying the amount of
        /// memory required for each shader 'warp' (collection of 32
        /// threads) by this factor, and then padding to a multiple of the
        /// value of the property
        /// nvn::DeviceInfo::SHADER_SCRATCH_MEMORY_GRANULARITY.  The
        /// per-warp scratch memory usage for a shader is provided in the
        /// shader compiler output.
        ///
        /// \note The minimum scaling factor may be different for compute
        /// and non-compute shaders.
        SHADER_SCRATCH_MEMORY_COMPUTE_SCALE_FACTOR_MINIMUM = 55,

        /// \brief Minimum alignment required for stride in linear
        /// textures, in bytes.
        ///
        /// Stride (distance in bytes between consecutive rows) in linear
        /// textures must be a multiple of this value.
        LINEAR_TEXTURE_STRIDE_ALIGNMENT = 56,

        /// \brief Minimum alignment required for stride in linear render
        /// targets, in bytes.
        ///
        /// Stride (distance in bytes between consecutive rows) in linear
        /// render targets must be rounded up to a multiple of this value.
        LINEAR_RENDER_TARGET_STRIDE_ALIGNMENT = 57,

        /// \brief Size of memory pages used in physical and virtual memory
        /// pools.
        ///
        /// Physical and virtual memory pools are made up of a collection
        /// of fixed-size memory pages.  The size of all physical and
        /// virtual memory pools must be a multiple of the page size.
        /// Virtual memory mappings must also be aligned on page size
        /// boundaries.
        MEMORY_POOL_PAGE_SIZE = 58,

        /// \brief 1 if the implementation always returns zero values when
        /// reading from unpopulated portions of virtual memory pools; 0
        /// otherwise.
        ///
        /// On NX and second-generation Maxwell GPUs, virtual memory pools
        /// include hardware support that returns zero when accessing
        /// unpopulated portions of virtual memory pools.  On older GPUs,
        /// values returned from such accesses are undefined.
        SUPPORTS_ZERO_FROM_UNMAPPED_VIRTUAL_POOL_PAGES = 59,

        /// \brief Alignment required (in bytes) for the offset and size of
        /// uniform buffer updates via CommandBuffer::UpdateUniformBuffer.
        UNIFORM_BUFFER_UPDATE_ALIGNMENT = 60,

        /// \brief Maximum texture size (in pixels) supported for 1D, 2D
        /// and 1D array and 2D array targets.
        MAX_TEXTURE_SIZE = 61,

        /// \brief Maximum texture size (in pixels) supported for buffer
        /// textures.
        MAX_BUFFER_TEXTURE_SIZE = 62,

        /// \brief Maximum texture size (in pixels) supported by 3D texture
        /// target.
        MAX_3D_TEXTURE_SIZE = 63,

        /// \brief Maximum texture size (in pixels) supported by cubemap
        /// texture target.
        MAX_CUBE_MAP_TEXTURE_SIZE = 64,

        /// \brief Maximum texture size (in pixels) supported by rectangle
        /// texture target.
        MAX_RECTANGLE_TEXTURE_SIZE = 65,

        /// \brief 1 if GLSL shaders using NV_geometry_shader_passthrough
        /// are supported by the NVN implementation; 0 otherwise.
        ///
        /// This feature is not supported on the Windows reference
        /// implementation for GPUs older than second-generation Maxwell
        /// GPUs.
        SUPPORTS_PASSTHROUGH_GEOMETRY_SHADERS = 66,

        /// \brief 1 if the viewport swizzle operation is supported by the
        /// NVN implementation; 0 otherwise.
        ///
        /// This feature is not supported on the Windows reference
        /// implementation for GPUs older than second-generation Maxwell
        /// GPUs.
        SUPPORTS_VIEWPORT_SWIZZLE = 67,

        /// \brief 1 if the NVN implementation supports packaged texture
        /// data in sparse tiled form; 0 otherwise.
        ///
        /// This feature is not supported on the Windows reference
        /// implementation for GPUs older than second-generation Maxwell
        /// GPUs.  The memory layout used for NX packaged textures using a
        /// sparse tiled form is not compatible with the layout used on
        /// older GPUs.
        SUPPORTS_SPARSE_TILED_PACKAGED_TEXTURES = 68,

        /// \brief 1 if advanced blending modes (other than
        /// BlendAdvancedMode::NONE) are supported by the NVN
        /// implementation; 0 otherwise.
        ///
        /// Advanced blending modes are not supported on the Windows
        /// reference implementation for GPUs older than first-generation
        /// Maxwell GPUs.
        SUPPORTS_ADVANCED_BLEND_MODES = 69,

        /// \brief Maximum value supported by
        /// nvn::WindowBuilder::SetPresentInterval.
        MAX_PRESENT_INTERVAL = 70,

        /// \brief 1 if DrawTexture is supported by the NVN implementation;
        /// 0 otherwise.
        ///
        /// This feature is not supported on the Windows reference
        /// implementation for GPUs older than first-generation Maxwell
        /// GPUs.
        SUPPORTS_DRAW_TEXTURE = 71,

        /// \brief 1 if the NVN implementation supports target-independent
        /// rasterization, 0 otherwise.
        ///
        /// This feature is not supported on the Windows reference
        /// implementation for GPUs older than second-generation Maxwell
        /// GPUs.
        SUPPORTS_TARGET_INDEPENDENT_RASTERIZATION = 72,

        /// \brief 1 if the NVN implementation supports fragment coverage
        /// to color, 0 otherwise.
        ///
        /// This feature is not supported on the Windows reference
        /// implementation for GPUs older than second-generation Maxwell
        /// GPUs.
        SUPPORTS_FRAGMENT_COVERAGE_TO_COLOR = 73,

        /// \brief 1 if the NVN implementation supports post-depth
        /// coverage, 0 otherwise.
        ///
        /// This feature is not supported on the Windows reference
        /// implementation for GPUs older than second-generation Maxwell
        /// GPUs.
        SUPPORTS_POST_DEPTH_COVERAGE = 74,

        /// \brief 1 if the NVN implementation supports using texture
        /// handles for image accesses, 0 otherwise.
        ///
        /// On NX, texture and image descriptors written into the texture
        /// pool use the same format, and texture handle values can be used
        /// in CommandBuffer::BindImage or for bindless image access.  On
        /// the Windows reference implementation, this is also true except
        /// when running on Kepler-family GPUs.  On those GPUs, texture and
        /// image descriptors do not have the same format, and their
        /// handles can not be used interchangeably.
        ///
        /// \warning When using cubemap and cubemap array textures as
        /// images, separate image handles must be used even on platforms
        /// where this property is set to 1.
        SUPPORTS_IMAGES_USING_TEXTURE_HANDLES = 75,

        /// \brief 1 if the NVN implementation supports programmable sample
        /// locations, 0 otherwise.
        ///
        /// This feature is not supported on the Windows reference
        /// implementation for GPUs older than second-generation Maxwell
        /// GPUs.
        SUPPORTS_SAMPLE_LOCATIONS = 76,

        /// \brief Total number of programmable sample locations in a
        /// MultisampleState object.
        MAX_SAMPLE_LOCATION_TABLE_ENTRIES = 77,

        /// \brief Amount of padding required at the end of a memory pool
        /// holding shader code.
        ///
        /// Shader code may be stored in memory pools with the
        /// MemoryPoolFlags::SHADER_CODE flag set. However, applications
        /// must avoid storing shader code near the end of the memory pool.
        /// GPU shader cores may pre-fetch beyond the last byte of actual
        /// shader code and could fault if unpopulated virtual memory
        /// beyond the end of the pool is accessed. On NX, the last 1KB of
        /// the memory pool should not be used for shader code.
        SHADER_CODE_MEMORY_POOL_PADDING_SIZE = 78,

        /// \brief Maximum number of vertices in each patch primitive.
        MAX_PATCH_SIZE = 79,

        /// \brief Required granularity for per-queue command memory.
        QUEUE_COMMAND_MEMORY_GRANULARITY = 80,

        /// \brief Minimum allowed size for per-queue command memory.
        QUEUE_COMMAND_MEMORY_MIN_SIZE = 81,

        /// \brief Default size for per-queue command memory.
        QUEUE_COMMAND_MEMORY_DEFAULT_SIZE = 82,

        /// \brief Required granularity for per-queue compute memory.
        QUEUE_COMPUTE_MEMORY_GRANULARITY = 83,

        /// \brief Minimum allowed size for per-queue compute memory, if
        /// non-zero.
        QUEUE_COMPUTE_MEMORY_MIN_SIZE = 84,

        /// \brief Default size for per-queue compute memory.
        QUEUE_COMPUTE_MEMORY_DEFAULT_SIZE = 85,

        /// \brief Minimum flush threshold size for per-queue command
        /// memory provided to NVN.
        QUEUE_COMMAND_MEMORY_MIN_FLUSH_THRESHOLD = 86,

        /// \brief 1 if GLSL fragment shaders with interlocks
        /// (NV_fragment_shader_interlock) are supported by the NVN
        /// implementation; 0 otherwise.
        ///
        /// These modes are not supported on the Windows reference
        /// implementation for GPUs older than second-generation Maxwell
        /// GPUs.
        SUPPORTS_FRAGMENT_SHADER_INTERLOCK = 87,

        /// \brief Maximum value supported by
        /// nvn::WindowBuilder::SetTextures.
        MAX_TEXTURES_PER_WINDOW = 88,

        /// \brief Minimum value supported by
        /// nvn::WindowBuilder::SetTextures.
        MIN_TEXTURES_PER_WINDOW = 89,

        /// \brief 1 if the NVN debug layer is supported by this version of
        /// the driver, 0 otherwise.
        ///
        /// Support for NVN debug layer can be queried before creating an
        /// NVN device by passing a NULL device pointer to
        /// nvn::Device::GetInteger.
        SUPPORTS_DEBUG_LAYER = 90,

        /// \brief Minimum allowed size for queue control memory.
        QUEUE_CONTROL_MEMORY_MIN_SIZE = 91,

        /// \brief Default size for queue control memory.
        QUEUE_CONTROL_MEMORY_DEFAULT_SIZE = 92,

        /// \brief Required granularity for queue control memory.
        QUEUE_CONTROL_MEMORY_GRANULARITY = 93,

        /// \brief Number of separate texture bindings supported for each
        /// shader stage.
        ///
        /// This value identifies the number of separate texture bindings
        /// available for devices initialized with
        /// nvn::DeviceFlagBits::ENABLE_SEPARATE_SAMPLER_TEXTURE_SUPPORT
        /// set. For devices initialized without the bit set, separate
        /// texture bindings may not be used.
        SEPARATE_TEXTURE_BINDINGS_PER_STAGE = 94,

        /// \brief Number of separate sampler bindings supported for each
        /// shader stage.
        ///
        /// This value identifies the number of separate sampler bindings
        /// available for devices initialized with
        /// nvn::DeviceFlagBits::ENABLE_SEPARATE_SAMPLER_TEXTURE_SUPPORT
        /// set. For devices initialized without the bit set, separate
        /// sampler bindings may not be used.
        SEPARATE_SAMPLER_BINDINGS_PER_STAGE = 95,

        /// \brief Value of the last valid debug groups domain ID.
        DEBUG_GROUPS_MAX_DOMAIN_ID = 96,

        /// \brief Does the device support reduction operations for events.
        ///
        /// This value indicates whether a device supports extended
        /// reduction operations when an event is signaled. A value of 0
        /// indicates that only the EventSignalMode::WRITE mode is
        /// supported. A value of 1 indicates that all EventSignalMode
        /// values are supported.
        EVENTS_SUPPORT_REDUCTION_OPERATIONS = 97,

        NVN_ENUM_32BIT(DEVICE_INFO),
    };
    NVN_ENUM_CLASS(DeviceInfo, NVNdeviceInfo);
};

/// \brief Identifies the type of a GPU error.
struct DeviceError {

    /// \brief Enumerated values for the type DeviceError.
    enum Enum {

        /// \brief The GPU attempted to access memory outside of a memory
        /// pool.
        ACCESS_VIOLATION = 0x00000001,

        NVN_ENUM_32BIT(DEVICE_ERROR),
    };
    NVN_ENUM_CLASS(DeviceError, NVNdeviceError);
};

/// \brief Identifies the type of a texture object.
struct TextureTarget {

    /// \brief Enumerated values for the type TextureTarget.
    enum Enum {

        /// \brief One-dimensional texture.
        TARGET_1D = 0x00000000,

        /// \brief Two-dimensional texture.
        TARGET_2D = 0x00000001,

        /// \brief Three-dimensional texture.
        TARGET_3D = 0x00000002,

        /// \brief Array of one-dimensional textures.
        TARGET_1D_ARRAY = 0x00000003,

        /// \brief Array of two-dimensional textures.
        TARGET_2D_ARRAY = 0x00000004,

        /// \brief Two-dimensional texture, with multiple samples per
        /// texel.
        TARGET_2D_MULTISAMPLE = 0x00000005,

        /// \brief Array of two-dimensional textures, with multiple samples
        /// per texel.
        TARGET_2D_MULTISAMPLE_ARRAY = 0x00000006,

        /// \brief Two-dimensional texture, accessed with non-normalized
        /// coordinates.
        TARGET_RECTANGLE = 0x00000007,

        /// \brief Cube map texture, with six two-dimensional cube faces.
        TARGET_CUBEMAP = 0x00000008,

        /// \brief Array of cube map textures, each with six
        /// two-dimensional cube faces.
        TARGET_CUBEMAP_ARRAY = 0x00000009,

        /// \brief Buffer texture
        TARGET_BUFFER = 0x0000000A,

        NVN_ENUM_32BIT(TEXTURE_TARGET),
    };
    NVN_ENUM_CLASS(TextureTarget, NVNtextureTarget);
};

/// \brief Identifies a source value for one of the four output components
/// produced by the texture swizzle operation.
struct TextureSwizzle {

    /// \brief Enumerated values for the type TextureSwizzle.
    enum Enum {

        /// \brief Output component returned as 0 or 0.0, depending on the
        /// texture format.
        ZERO = 0x00000000,

        /// \brief Output component returned as 1 or 1.0, depending on the
        /// texture format.
        ONE = 0x00000001,

        /// \brief Output component filled with the first (R) component of
        /// the source image.
        R = 0x00000002,

        /// \brief Output component filled with the second (G) component of
        /// the source image.
        G = 0x00000003,

        /// \brief Output component filled with the third (B) component of
        /// the source image.
        B = 0x00000004,

        /// \brief Output component filled with the fourth (A) component of
        /// the source image.
        A = 0x00000005,

        NVN_ENUM_32BIT(TEXTURE_SWIZZLE),
    };
    NVN_ENUM_CLASS(TextureSwizzle, NVNtextureSwizzle);
};

/// \brief For textures containing depth and stencil data, identifies
/// whether texture lookups will return depth or stencil values.
struct TextureDepthStencilMode {

    /// \brief Enumerated values for the type TextureDepthStencilMode.
    enum Enum {

        /// \brief Return depth data as floating-point values.
        DEPTH = 0x00000000,

        /// \brief Return stencil data as unsigned integer values.
        STENCIL = 0x00000001,

        NVN_ENUM_32BIT(TEXTURE_DEPTH_STENCIL_MODE),
    };
    NVN_ENUM_CLASS(TextureDepthStencilMode, NVNtextureDepthStencilMode);
};

/// \brief Identifies how data elements stored in texture or buffer objects
/// are encoded.
///
/// Specifies how data elements stored in texture or buffer objects are
/// encoded and decoded, described in detail in the \ref nvn_guide_formats
/// "programming guide".  Not all formats are supported for all uses
/// (texture, vertex pulling); see the \ref nvn_guide_formats_table
/// "formats table" to determine what formats may be used in what
/// situations.
struct Format {

    /// \brief Enumerated values for the type Format.
    enum Enum {

        /// \brief Placeholder enum indicating no defined format; never
        /// valid.
        NONE = 0x00000000,

        /// \brief One 8-bit unsigned normalized component.
        R8 = 0x00000001,

        /// \brief One 8-bit signed normalized component.
        R8SN = 0x00000002,

        /// \brief One 8-bit unsigned integer component.
        R8UI = 0x00000003,

        /// \brief One 8-bit signed integer component.
        R8I = 0x00000004,

        /// \brief One 16-bit floating-point component.
        R16F = 0x00000005,

        /// \brief One 16-bit unsigned normalized component.
        R16 = 0x00000006,

        /// \brief One 16-bit signed normalized component.
        R16SN = 0x00000007,

        /// \brief One 16-bit unsigned integer component.
        R16UI = 0x00000008,

        /// \brief One 16-bit signed integer component.
        R16I = 0x00000009,

        /// \brief One 32-bit floating-point component.
        R32F = 0x0000000A,

        /// \brief One 32-bit unsigned integer component.
        R32UI = 0x0000000B,

        /// \brief One 32-bit signed integer component.
        R32I = 0x0000000C,

        /// \brief Two 8-bit unsigned normalized components.
        RG8 = 0x0000000D,

        /// \brief Two 8-bit signed normalized components.
        RG8SN = 0x0000000E,

        /// \brief Two 8-bit unsigned integer components.
        RG8UI = 0x0000000F,

        /// \brief Two 8-bit signed integer components.
        RG8I = 0x00000010,

        /// \brief Two 16-bit floating-point components.
        RG16F = 0x00000011,

        /// \brief Two 16-bit unsigned normalized components.
        RG16 = 0x00000012,

        /// \brief Two 16-bit signed normalized components.
        RG16SN = 0x00000013,

        /// \brief Two 16-bit unsigned integer components.
        RG16UI = 0x00000014,

        /// \brief Two 16-bit signed integer components.
        RG16I = 0x00000015,

        /// \brief Two 32-bit floating-point components.
        RG32F = 0x00000016,

        /// \brief Two 32-bit unsigned integer components.
        RG32UI = 0x00000017,

        /// \brief Two 32-bit signed integer components.
        RG32I = 0x00000018,

        /// \brief Three 8-bit unsigned normalized components.
        RGB8 = 0x00000019,

        /// \brief Three 8-bit signed normalized components.
        RGB8SN = 0x0000001A,

        /// \brief Three 8-bit unsigned integer components.
        RGB8UI = 0x0000001B,

        /// \brief Three 8-bit signed integer components.
        RGB8I = 0x0000001C,

        /// \brief Three 16-bit floating-point components.
        RGB16F = 0x0000001D,

        /// \brief Three 16-bit unsigned normalized components.
        RGB16 = 0x0000001E,

        /// \brief Three 16-bit signed normalized components.
        RGB16SN = 0x0000001F,

        /// \brief Three 16-bit unsigned integer components.
        RGB16UI = 0x00000020,

        /// \brief Three 16-bit signed integer components.
        RGB16I = 0x00000021,

        /// \brief Three 32-bit floating-point components.
        RGB32F = 0x00000022,

        /// \brief Three 32-bit unsigned integer components.
        RGB32UI = 0x00000023,

        /// \brief Three 32-bit signed integer components.
        RGB32I = 0x00000024,

        /// \brief Four 8-bit unsigned normalized components.
        RGBA8 = 0x00000025,

        /// \brief Four 8-bit signed normalized components.
        RGBA8SN = 0x00000026,

        /// \brief Four 8-bit unsigned integer components.
        RGBA8UI = 0x00000027,

        /// \brief Four 8-bit signed integer components.
        RGBA8I = 0x00000028,

        /// \brief Four 16-bit floating-point components.
        RGBA16F = 0x00000029,

        /// \brief Four 16-bit unsigned normalized components.
        RGBA16 = 0x0000002A,

        /// \brief Four 16-bit signed normalized components.
        RGBA16SN = 0x0000002B,

        /// \brief Four 16-bit unsigned integer components.
        RGBA16UI = 0x0000002C,

        /// \brief Four 16-bit signed integer components.
        RGBA16I = 0x0000002D,

        /// \brief Four 32-bit floating-point components.
        RGBA32F = 0x0000002E,

        /// \brief Four 32-bit unsigned integer components.
        RGBA32UI = 0x0000002F,

        /// \brief Four 32-bit signed integer components.
        RGBA32I = 0x00000030,

        /// \brief An 8-bit unsigned integer stencil component.
        STENCIL8 = 0x00000031,

        /// \brief A 16-bit unsigned normalized depth component.
        DEPTH16 = 0x00000032,

        /// \brief A 24-bit unsigned normalized depth component in the LSBs
        /// of a 32-bit word.
        DEPTH24 = 0x00000033,

        /// \brief A 32-bit floating-point depth component.
        DEPTH32F = 0x00000034,

        /// \brief A 24-bit unsigned normalized depth component packed with
        /// an 8-bit unsigned integer stencil component.
        DEPTH24_STENCIL8 = 0x00000035,

        /// \brief A 32-bit floating-point depth component packed with an
        /// 8-bit unsigned integer stencil component.
        DEPTH32F_STENCIL8 = 0x00000036,

        /// \brief Three 8-bit unsigned normalized components, padded to 32
        /// bits per pixel and encoded in the sRGB color space.
        RGBX8_SRGB = 0x00000037,

        /// \brief Four 8-bit unsigned normalized components, encoded in
        /// the sRGB color space.
        RGBA8_SRGB = 0x00000038,

        /// \brief Four 4-bit unsigned normalized components.
        RGBA4 = 0x00000039,

        /// \brief Three 5-bit unsigned normalized components.
        RGB5 = 0x0000003A,

        /// \brief Three 5-bit unsigned normalized RGB components with a
        /// 1-bit A component.
        RGB5A1 = 0x0000003B,

        /// \brief Three 5-bit (R,B) and 6-bit (G) unsigned normalized
        /// components.
        RGB565 = 0x0000003C,

        /// \brief Three 10-bit (R,G,B) and 2-bit (A) unsigned normalized
        /// components.
        RGB10A2 = 0x0000003D,

        /// \brief Three 10-bit (R,G,B) and 2-bit (A) unsigned integer
        /// components.
        RGB10A2UI = 0x0000003E,

        /// \brief Three 10-bit (B) and 11-bit (R,G) floating-point
        /// components.
        R11G11B10F = 0x0000003F,

        /// \brief Three floating-point components with separate 9-bit
        /// mantissas and a shared 5-bit exponent.
        RGB9E5F = 0x00000040,

        /// \brief DXT1 compressed texture blocks with an RGB format.
        RGB_DXT1 = 0x00000041,

        /// \brief DXT1 compressed texture blocks with an RGBA format.
        RGBA_DXT1 = 0x00000042,

        /// \brief DXT3 compressed texture blocks.
        RGBA_DXT3 = 0x00000043,

        /// \brief DXT5 compressed texture blocks
        RGBA_DXT5 = 0x00000044,

        /// \brief DXT1 compressed texture blocks with an sRGB-encoded RGB
        /// format.
        RGB_DXT1_SRGB = 0x00000045,

        /// \brief DXT1 compressed texture blocks with an sRGB-encoded RGBA
        /// format.
        RGBA_DXT1_SRGB = 0x00000046,

        /// \brief DXT3 compressed texture blocks with an sRGB color
        /// encoding.
        RGBA_DXT3_SRGB = 0x00000047,

        /// \brief DXT5 compressed texture blocks with an sRGB color
        /// encoding.
        RGBA_DXT5_SRGB = 0x00000048,

        /// \brief RGTC compressed 1-component unsigned normalized blocks.
        RGTC1_UNORM = 0x00000049,

        /// \brief RGTC compressed 1-component signed normalized blocks.
        RGTC1_SNORM = 0x0000004A,

        /// \brief RGTC compressed 2-component unsigned normalized blocks.
        RGTC2_UNORM = 0x0000004B,

        /// \brief RGTC compressed 2-component signed normalized blocks.
        RGTC2_SNORM = 0x0000004C,

        /// \brief BPTC compressed 4-component unsigned normalized blocks.
        BPTC_UNORM = 0x0000004D,

        /// \brief BPTC compressed 4-component unsigned normalized blocks
        /// with an sRGB color encoding.
        BPTC_UNORM_SRGB = 0x0000004E,

        /// \brief BPTC compressed 3-component signed floating-point
        /// blocks.
        BPTC_SFLOAT = 0x0000004F,

        /// \brief BPTC compressed 3-component unsigned floating-point
        /// blocks.
        BPTC_UFLOAT = 0x00000050,

        /// \brief One 8-bit unsigned integer component, cast to float.
        R8_UI2F = 0x00000051,

        /// \brief One 8-bit signed integer component, cast to float.
        R8_I2F = 0x00000052,

        /// \brief One 16-bit unsigned integer component, cast to float.
        R16_UI2F = 0x00000053,

        /// \brief One 16-bit signed integer component, cast to float.
        R16_I2F = 0x00000054,

        /// \brief One 32-bit unsigned integer component, cast to float.
        R32_UI2F = 0x00000055,

        /// \brief One 32-bit signed integer component, cast to float.
        R32_I2F = 0x00000056,

        /// \brief Two 8-bit unsigned integer components, cast to float.
        RG8_UI2F = 0x00000057,

        /// \brief Two 8-bit signed integer components, cast to float.
        RG8_I2F = 0x00000058,

        /// \brief Two 16-bit unsigned integer components, cast to float.
        RG16_UI2F = 0x00000059,

        /// \brief Two 16-bit signed integer components, cast to float.
        RG16_I2F = 0x0000005A,

        /// \brief Two 32-bit unsigned integer components, cast to float.
        RG32_UI2F = 0x0000005B,

        /// \brief Two 32-bit signed integer components, cast to float.
        RG32_I2F = 0x0000005C,

        /// \brief Three 8-bit unsigned integer components, cast to float.
        RGB8_UI2F = 0x0000005D,

        /// \brief Three 8-bit signed integer components, cast to float.
        RGB8_I2F = 0x0000005E,

        /// \brief Three 16-bit unsigned integer components, cast to float.
        RGB16_UI2F = 0x0000005F,

        /// \brief Three 16-bit signed integer components, cast to float.
        RGB16_I2F = 0x00000060,

        /// \brief Three 32-bit unsigned integer components, cast to float.
        RGB32_UI2F = 0x00000061,

        /// \brief Three 32-bit signed integer components, cast to float.
        RGB32_I2F = 0x00000062,

        /// \brief Four 8-bit unsigned integer components, cast to float.
        RGBA8_UI2F = 0x00000063,

        /// \brief Four 8-bit signed integer components, cast to float.
        RGBA8_I2F = 0x00000064,

        /// \brief Four 16-bit unsigned integer components, cast to float.
        RGBA16_UI2F = 0x00000065,

        /// \brief Four 16-bit signed integer components, cast to float.
        RGBA16_I2F = 0x00000066,

        /// \brief Four 32-bit unsigned integer components, cast to float.
        RGBA32_UI2F = 0x00000067,

        /// \brief Four 32-bit signed integer components, cast to float.
        RGBA32_I2F = 0x00000068,

        /// \brief Three 10-bit (R,G,B) and 2-bit (A) signed normalized
        /// components.
        RGB10A2SN = 0x00000069,

        /// \brief Three 10-bit (R,G,B) and 2-bit (A) signed integer
        /// components.
        RGB10A2I = 0x0000006A,

        /// \brief Three 10-bit (R,G,B) and 2-bit (A) unsigned integer
        /// components, cast to floating-point.
        RGB10A2_UI2F = 0x0000006B,

        /// \brief Three 10-bit (R,G,B) and 2-bit (A) signed integer
        /// components, cast to floating-point.
        RGB10A2_I2F = 0x0000006C,

        /// \brief Three 8-bit unsigned normalized components, padded to 32
        /// bits per pixel.
        RGBX8 = 0x0000006D,

        /// \brief Three 8-bit signed normalized components, padded to 32
        /// bits per pixel.
        RGBX8SN = 0x0000006E,

        /// \brief Three 8-bit unsigned integer components, padded to 32
        /// bits per pixel.
        RGBX8UI = 0x0000006F,

        /// \brief Three 8-bit signed integer components, padded to 32 bits
        /// per pixel.
        RGBX8I = 0x00000070,

        /// \brief Three 16-bit floating-point components, padded to 64
        /// bits per pixel.
        RGBX16F = 0x00000071,

        /// \brief Three 16-bit unsigned normalized components, padded to
        /// 64 bits per pixel.
        RGBX16 = 0x00000072,

        /// \brief Three 16-bit signed normalized components, padded to 64
        /// bits per pixel.
        RGBX16SN = 0x00000073,

        /// \brief Three 16-bit unsigned integer components, padded to 64
        /// bits per pixel.
        RGBX16UI = 0x00000074,

        /// \brief Three 16-bit signed integer components, padded to 64
        /// bits per pixel.
        RGBX16I = 0x00000075,

        /// \brief Three 32-bit floating-point components, padded to 128
        /// bits per pixel.
        RGBX32F = 0x00000076,

        /// \brief Three 32-bit unsigned integer components, padded to 128
        /// bits per pixel.
        RGBX32UI = 0x00000077,

        /// \brief Three 32-bit signed integer components, padded to 128
        /// bits per pixel.
        RGBX32I = 0x00000078,

        /// \brief ASTC compressed texture blocks with an RGBA format and
        /// 4x4 texel block footprint.
        RGBA_ASTC_4x4 = 0x00000079,

        /// \brief ASTC compressed texture blocks with an RGBA format and
        /// 5x4 texel block footprint.
        RGBA_ASTC_5x4 = 0x0000007A,

        /// \brief ASTC compressed texture blocks with an RGBA format and
        /// 5x5 texel block footprint.
        RGBA_ASTC_5x5 = 0x0000007B,

        /// \brief ASTC compressed texture blocks with an RGBA format and
        /// 6x5 texel block footprint.
        RGBA_ASTC_6x5 = 0x0000007C,

        /// \brief ASTC compressed texture blocks with an RGBA format and
        /// 6x6 texel block footprint.
        RGBA_ASTC_6x6 = 0x0000007D,

        /// \brief ASTC compressed texture blocks with an RGBA format and
        /// 8x5 texel block footprint.
        RGBA_ASTC_8x5 = 0x0000007E,

        /// \brief ASTC compressed texture blocks with an RGBA format and
        /// 8x6 texel block footprint.
        RGBA_ASTC_8x6 = 0x0000007F,

        /// \brief ASTC compressed texture blocks with an RGBA format and
        /// 8x8 texel block footprint.
        RGBA_ASTC_8x8 = 0x00000080,

        /// \brief ASTC compressed texture blocks with an RGBA format and
        /// 10x5 texel block footprint.
        RGBA_ASTC_10x5 = 0x00000081,

        /// \brief ASTC compressed texture blocks with an RGBA format and
        /// 10x6 texel block footprint.
        RGBA_ASTC_10x6 = 0x00000082,

        /// \brief ASTC compressed texture blocks with an RGBA format and
        /// 10x8 texel block footprint.
        RGBA_ASTC_10x8 = 0x00000083,

        /// \brief ASTC compressed texture blocks with an RGBA format and
        /// 10x10 texel block footprint.
        RGBA_ASTC_10x10 = 0x00000084,

        /// \brief ASTC compressed texture blocks with an RGBA format and
        /// 12x10 texel block footprint.
        RGBA_ASTC_12x10 = 0x00000085,

        /// \brief ASTC compressed texture blocks with an RGBA format and
        /// 12x12 texel block footprint.
        RGBA_ASTC_12x12 = 0x00000086,

        /// \brief ASTC compressed texture blocks with an sRGB plus Alpha
        /// format and 4x4 texel block footprint.
        RGBA_ASTC_4x4_SRGB = 0x00000087,

        /// \brief ASTC compressed texture blocks with an sRGB plus Alpha
        /// format and 5x4 texel block footprint.
        RGBA_ASTC_5x4_SRGB = 0x00000088,

        /// \brief ASTC compressed texture blocks with an sRGB plus Alpha
        /// format and 5x5 texel block footprint.
        RGBA_ASTC_5x5_SRGB = 0x00000089,

        /// \brief ASTC compressed texture blocks with an sRGB plus Alpha
        /// format and 6x5 texel block footprint.
        RGBA_ASTC_6x5_SRGB = 0x0000008A,

        /// \brief ASTC compressed texture blocks with an sRGB plus Alpha
        /// format and 6x6 texel block footprint.
        RGBA_ASTC_6x6_SRGB = 0x0000008B,

        /// \brief ASTC compressed texture blocks with an sRGB plus Alpha
        /// format and 8x5 texel block footprint.
        RGBA_ASTC_8x5_SRGB = 0x0000008C,

        /// \brief ASTC compressed texture blocks with an sRGB plus Alpha
        /// format and 8x6 texel block footprint.
        RGBA_ASTC_8x6_SRGB = 0x0000008D,

        /// \brief ASTC compressed texture blocks with an sRGB plus Alpha
        /// format and 8x8 texel block footprint.
        RGBA_ASTC_8x8_SRGB = 0x0000008E,

        /// \brief ASTC compressed texture blocks with an sRGB plus Alpha
        /// format and 10x5 texel block footprint.
        RGBA_ASTC_10x5_SRGB = 0x0000008F,

        /// \brief ASTC compressed texture blocks with an sRGB plus Alpha
        /// format and 10x6 texel block footprint.
        RGBA_ASTC_10x6_SRGB = 0x00000090,

        /// \brief ASTC compressed texture blocks with an sRGB plus Alpha
        /// format and 10x8 texel block footprint.
        RGBA_ASTC_10x8_SRGB = 0x00000091,

        /// \brief ASTC compressed texture blocks with an sRGB plus Alpha
        /// format and 10x10 texel block footprint.
        RGBA_ASTC_10x10_SRGB = 0x00000092,

        /// \brief ASTC compressed texture blocks with an sRGB plus Alpha
        /// format and 12x10 texel block footprint.
        RGBA_ASTC_12x10_SRGB = 0x00000093,

        /// \brief ASTC compressed texture blocks with an sRGB plus Alpha
        /// format and 12x12 texel block footprint.
        RGBA_ASTC_12x12_SRGB = 0x00000094,

        /// \brief Three 5-bit (R,B) and 6-bit (G) unsigned normalized
        /// components.
        BGR565 = 0x00000095,

        /// \brief Three 5-bit unsigned normalized components.
        BGR5 = 0x00000096,

        /// \brief Three 5-bit unsigned normalized RGB components with a
        /// 1-bit A component.
        BGR5A1 = 0x00000097,

        /// \brief One 1-bit A component with three 5-bit unsigned
        /// normalized RGB components.
        A1BGR5 = 0x00000098,

        /// \brief Three 8-bit unsigned normalized components, padded to 32
        /// bits per pixel.
        BGRX8 = 0x00000099,

        /// \brief Four 8-bit unsigned normalized components.
        BGRA8 = 0x0000009A,

        /// \brief Three 8-bit unsigned normalized components, padded to 32
        /// bits per pixel and encoded in the sRGB color space.
        BGRX8_SRGB = 0x0000009B,

        /// \brief Four 8-bit unsigned normalized components, encoded in
        /// the sRGB color space.
        BGRA8_SRGB = 0x0000009C,

        NVN_ENUM_32BIT(FORMAT),
    };
    NVN_ENUM_CLASS(Format, NVNformat);
};

/// \brief Identifies a coefficient to be multiplied with each component of
/// the source or destination color when blending.
///
/// For the default BlendEquation::ADD mode, blending is performed by
/// evaluating 'C = Fs * Cs + Fd * Cd' for each color component.  In these
/// equations, Cs represents the fragment shader output (source) color and
/// Cd represents the color value stored in the framebuffer. The BlendFunc
/// enum specifies values used for the blend factor terms Fs and Fd.  There
/// are separate blend functions for RGB and alpha color components.
struct BlendFunc {

    /// \brief Enumerated values for the type BlendFunc.
    enum Enum {

        /// \brief Use the constant 0.0.
        ZERO = 0x00000001,

        /// \brief Use the constant 1.0.
        ONE = 0x00000002,

        /// \brief Use by the corresponding component of the source color.
        SRC_COLOR = 0x00000003,

        /// \brief Use 1.0 minus the corresponding component of the source
        /// color.
        ONE_MINUS_SRC_COLOR = 0x00000004,

        /// \brief Use the alpha component of the source color.
        SRC_ALPHA = 0x00000005,

        /// \brief Use 1.0 minus the alpha component of the source color.
        ONE_MINUS_SRC_ALPHA = 0x00000006,

        /// \brief Use the alpha component of the destination color.
        DST_ALPHA = 0x00000007,

        /// \brief Use 1.0 minus the alpha component of the destination
        /// color.
        ONE_MINUS_DST_ALPHA = 0x00000008,

        /// \brief Use the corresponding component of the destination
        /// color.
        DST_COLOR = 0x00000009,

        /// \brief Use 1.0 minus the corresponding component of the
        /// destination color.
        ONE_MINUS_DST_COLOR = 0x0000000A,

        /// \brief Use min(As,1-Ad), where As and Ad are the alpha
        /// components of the source and destination color.
        SRC_ALPHA_SATURATE = 0x0000000B,

        /// \brief Use the corresponding component of the second source
        /// color.
        SRC1_COLOR = 0x00000010,

        /// \brief Use 1.0 minus the corresponding component of the second
        /// source color.
        ONE_MINUS_SRC1_COLOR = 0x00000011,

        /// \brief Use the alpha component of the second source color.
        SRC1_ALPHA = 0x00000012,

        /// \brief Use 1.0 minus the alpha component of the second source
        /// color.
        ONE_MINUS_SRC1_ALPHA = 0x00000013,

        /// \brief Use the corresponding component of the constant blend
        /// color.
        CONSTANT_COLOR = 0x00000061,

        /// \brief Use 1.0 minus the corresponding component of the
        /// constant blend color.
        ONE_MINUS_CONSTANT_COLOR = 0x00000062,

        /// \brief Use the alpha component of the constant blend color.
        CONSTANT_ALPHA = 0x00000063,

        /// \brief Use 1.0 minus the alpha component of the constant blend
        /// color.
        ONE_MINUS_CONSTANT_ALPHA = 0x00000064,

        NVN_ENUM_32BIT(BLEND_FUNC),
    };
    NVN_ENUM_CLASS(BlendFunc, NVNblendFunc);
};

/// \brief Specifies an advanced blending mode to combine source and
/// destination colors.
///
/// Advanced blending in NVN provides capabilities similar to those
/// provided by the NV_blend_equation_advanced OpenGL extension.  These
/// blend modes specify a technique for combining source and destination
/// color components, and use the source and destination alpha components
/// to evaluate how colors are combined with overlapping color components.
/// The advanced blending modes provided here are similar to those
/// supported in various standards, including several OpenGL and OpenGL ES
/// extensions, OpenVG, the SVG compositing standard, the PDF document
/// format, and various other APIs.  For more information on the operation
/// of these blend modes, please consult the NV_blend_equation_advanced
/// specification.
struct BlendAdvancedMode {

    /// \brief Enumerated values for the type BlendAdvancedMode.
    enum Enum {

        /// \brief Use normal blending equations specified with
        /// BlendState::SetBlendEquation and BlendState::SetBlendFunc.
        BLEND_NONE = 0x00000000,

        /// \brief Use the ZERO blending mode.
        BLEND_ZERO = 0x00000005,

        /// \brief Use the SRC blending mode.
        BLEND_SRC = 0x00000006,

        /// \brief Use the DST blending mode.
        BLEND_DST = 0x00000007,

        /// \brief Use the SRC_OVER blending mode.
        BLEND_SRC_OVER = 0x00000008,

        /// \brief Use the DST_OVER blending mode.
        BLEND_DST_OVER = 0x00000009,

        /// \brief Use the SRC_IN blending mode.
        BLEND_SRC_IN = 0x0000000A,

        /// \brief Use the DST_IN blending mode.
        BLEND_DST_IN = 0x0000000B,

        /// \brief Use the SRC_OUT blending mode.
        BLEND_SRC_OUT = 0x0000000C,

        /// \brief Use the DST_OUT blending mode.
        BLEND_DST_OUT = 0x0000000D,

        /// \brief Use the SRC_ATOP blending mode.
        BLEND_SRC_ATOP = 0x0000000E,

        /// \brief Use the DST_ATOP blending mode.
        BLEND_DST_ATOP = 0x0000000F,

        /// \brief Use the XOR blending mode.
        BLEND_XOR = 0x00000010,

        /// \brief Use the PLUS blending mode.
        BLEND_PLUS = 0x00000011,

        /// \brief Use the PLUS_CLAMPED blending mode.
        BLEND_PLUS_CLAMPED = 0x00000012,

        /// \brief Use the PLUS_CLAMPED_ALPHA blending mode.
        BLEND_PLUS_CLAMPED_ALPHA = 0x00000013,

        /// \brief Use the PLUS_DARKER blending mode.
        BLEND_PLUS_DARKER = 0x00000014,

        /// \brief Use the MULTIPLY blending mode.
        BLEND_MULTIPLY = 0x00000015,

        /// \brief Use the SCREEN blending mode.
        BLEND_SCREEN = 0x00000016,

        /// \brief Use the OVERLAY blending mode.
        BLEND_OVERLAY = 0x00000017,

        /// \brief Use the DARKEN blending mode.
        BLEND_DARKEN = 0x00000018,

        /// \brief Use the LIGHTEN blending mode.
        BLEND_LIGHTEN = 0x00000019,

        /// \brief Use the COLORDODGE blending mode.
        BLEND_COLORDODGE = 0x0000001A,

        /// \brief Use the COLORBURN blending mode.
        BLEND_COLORBURN = 0x0000001B,

        /// \brief Use the HARDLIGHT blending mode.
        BLEND_HARDLIGHT = 0x0000001C,

        /// \brief Use the SOFTLIGHT blending mode.
        BLEND_SOFTLIGHT = 0x0000001D,

        /// \brief Use the DIFFERENCE blending mode.
        BLEND_DIFFERENCE = 0x0000001E,

        /// \brief Use the MINUS blending mode.
        BLEND_MINUS = 0x00000001F,

        /// \brief Use the MINUS_CLAMPED blending mode.
        BLEND_MINUS_CLAMPED = 0x00000020,

        /// \brief Use the EXCLUSION blending mode.
        BLEND_EXCLUSION = 0x00000021,

        /// \brief Use the CONTRAST blending mode.
        BLEND_CONTRAST = 0x00000022,

        /// \brief Use the INVERT blending mode.
        BLEND_INVERT = 0x00000023,

        /// \brief Use the INVERT_RGB blending mode.
        BLEND_INVERT_RGB = 0x00000024,

        /// \brief Use the INVERT_OVG blending mode.
        BLEND_INVERT_OVG = 0x00000025,

        /// \brief Use the LINEARDODGE blending mode.
        BLEND_LINEARDODGE = 0x00000026,

        /// \brief Use the LINEARBURN blending mode.
        BLEND_LINEARBURN = 0x00000027,

        /// \brief Use the VIVIDLIGHT blending mode.
        BLEND_VIVIDLIGHT = 0x00000028,

        /// \brief Use the LINEARLIGHT blending mode.
        BLEND_LINEARLIGHT = 0x00000029,

        /// \brief Use the PINLIGHT blending mode.
        BLEND_PINLIGHT = 0x0000002A,

        /// \brief Use the HARDMIX blending mode.
        BLEND_HARDMIX = 0x0000002B,

        /// \brief Use the RED blending mode.
        BLEND_RED = 0x0000002C,

        /// \brief Use the GREEN blending mode.
        BLEND_GREEN = 0x0000002D,

        /// \brief Use the BLUE blending mode.
        BLEND_BLUE = 0x0000002E,

        /// \brief Use the HSL_HUE blending mode.
        BLEND_HSL_HUE = 0x0000002F,

        /// \brief Use the HSL_SATURATION blending mode.
        BLEND_HSL_SATURATION = 0x00000030,

        /// \brief Use the HSL_COLOR blending mode.
        BLEND_HSL_COLOR = 0x00000031,

        /// \brief Use the HSL_LUMINOSITY blending mode.
        BLEND_HSL_LUMINOSITY = 0x00000032,

        NVN_ENUM_32BIT(BLEND_ADVANCED_MODE),
    };
    NVN_ENUM_CLASS(BlendAdvancedMode, NVNblendAdvancedMode);
};

/// \brief Specifies an advanced mode for advanced blending modes.
///
/// Advanced blending in NVN provides capabilities similar to those
/// provided by the NV_blend_equation_advanced OpenGL extension.  The
/// overlap modes specify how coverage for partially covered sources and/or
/// destinations is treated in the blending equations.
struct BlendAdvancedOverlap {

    /// \brief Enumerated values for the type BlendAdvancedOverlap.
    enum Enum {

        /// \brief Treat the coverage of source and destination coverage as
        /// uncorrelated.
        ///
        /// For uncorrelated coverage, the portions of the pixel covered by
        /// the source and destination are considered to be distributed
        /// independently.  The area of overlap is As * Ad, where As and Ad
        /// are the source and destination alpha.
        UNCORRELATED = 0x00000000,

        /// \brief Treat the coverage of source and destination coverage as
        /// disjoint.
        ///
        /// For disjoint coverage, the blending equations are configured to
        /// minimize the amount of overlap between the source and
        /// destination.  The area of overlap is max(0,As+Ad-1), where As
        /// and Ad are the source and destination alpha values.
        DISJOINT = 0x00000001,

        /// \brief Treat the coverage of source and destination coverage as
        /// conjoint.
        ///
        /// For conjoint coverage, the blending equations are configured to
        /// maximize the amount of overlap between the source and
        /// destination.  The area of overlap is min(As,Ad), where As and
        /// Ad are the source and destination alpha values.
        CONJOINT = 0x00000002,

        NVN_ENUM_32BIT(BLEND_ADVANCED_OVERLAP),
    };
    NVN_ENUM_CLASS(BlendAdvancedOverlap, NVNblendAdvancedOverlap);
};

/// \brief Specifies the equation used for blending source and destination
/// colors.
///
/// Blending is performed by evaluating a function for each component of
/// the source and destination colors (Cs and Cd), called the blend
/// equation.  Several blend equations also involve source and destination
/// blend factors (Fs and Fd), called the blend function.
struct BlendEquation {

    /// \brief Enumerated values for the type BlendEquation.
    enum Enum {

        /// \brief Compute C = Fs * Cs + Fd * Cd.
        ADD = 0x00000001,

        /// \brief Compute C = Fs * Cs - Fd * Cd.
        SUB = 0x00000002,

        /// \brief Compute C = Fd * Cd - Fs * Cs.
        REVERSE_SUB = 0x00000003,

        /// \brief Compute min(Cs, Cd).
        MIN = 0x00000004,

        /// \brief Compute max(Cs, Cd).
        MAX = 0x00000005,

        NVN_ENUM_32BIT(BLEND_EQUATION),
    };
    NVN_ENUM_CLASS(BlendEquation, NVNblendEquation);
};

/// \brief Specifies a bit-wise operation performed on source and
/// destination colors.
///
/// Logical operations are performed by evaluating a function for each
/// component of the source and destination colors (Cs and Cd).  Before
/// performing logical operations, the source color is converted to the
/// format used to store destination values.  If enabled, the same
/// operation is performed on all render targets.  There is no support for
/// separate logical operations on color and alpha components.
struct LogicOp {

    /// \brief Enumerated values for the type LogicOp.
    enum Enum {

        /// \brief Compute C = 0.
        CLEAR = 0x00000000,

        /// \brief Compute C = Cs & Cd.
        AND = 0x00000001,

        /// \brief Compute C = Cs & (~Cd).
        AND_REVERSE = 0x00000002,

        /// \brief Compute C = Cs.
        COPY = 0x00000003,

        /// \brief Compute C = (~Cs) & Cd.
        AND_INVERTED = 0x00000004,

        /// \brief Compute C = Cd.
        NOOP = 0x00000005,

        /// \brief Compute C = Cs ^ Cd.
        XOR = 0x00000006,

        /// \brief Compute C = Cs | Cd.
        OR = 0x00000007,

        /// \brief Compute C = ~(Cs | Cd).
        NOR = 0x00000008,

        /// \brief Compute C = ~(Cs ^ Cd).
        EQUIV = 0x00000009,

        /// \brief Compute C = ~Cd.
        INVERT = 0x0000000A,

        /// \brief Compute C = Cs | (~Cd).
        OR_REVERSE = 0x0000000B,

        /// \brief Compute C = ~Cs.
        COPY_INVERTED = 0x0000000C,

        /// \brief Compute C = (~Cs) | Cd.
        OR_INVERTED = 0x0000000D,

        /// \brief Compute C = ~(Cs & Cd).
        NAND = 0x0000000E,

        /// \brief Compute C = ~0 (set all bits to one).
        SET = 0x0000000F,

        NVN_ENUM_32BIT(LOGIC_OP),
    };
    NVN_ENUM_CLASS(LogicOp, NVNlogicOp);
};

/// \brief Specifies a function used to compare 'test' and 'reference'
/// values for alpha tests, where the 'test' value is the alpha value from
/// the fragment.
struct AlphaFunc {

    /// \brief Enumerated values for the type AlphaFunc.
    enum Enum {

        /// \brief Comparison always fails.
        NEVER = 0x00000001,

        /// \brief Comparison passes if the test value is less than the
        /// reference value.
        LESS = 0x00000002,

        /// \brief Comparison passes if the test value is equal to the
        /// reference value.
        EQUAL = 0x00000003,

        /// \brief Comparison passes if the test value is less than or
        /// equal to the reference value.
        LEQUAL = 0x00000004,

        /// \brief Comparison passes if the test value is greater than the
        /// reference value.
        GREATER = 0x00000005,

        /// \brief Comparison passes if the test value is not equal to
        /// reference value.
        NOTEQUAL = 0x00000006,

        /// \brief Comparison passes if the test value is greater than or
        /// equal to the reference value.
        GEQUAL = 0x00000007,

        /// \brief Comparison always passes.
        ALWAYS = 0x00000008,

        NVN_ENUM_32BIT(ALPHA_FUNC),
    };
    NVN_ENUM_CLASS(AlphaFunc, NVNalphaFunc);
};

/// \brief Specifies the type of primitive to render in draw calls.
struct DrawPrimitive {

    /// \brief Enumerated values for the type DrawPrimitive.
    enum Enum {

        /// \brief Renders a collection of points.
        POINTS = 0x00000000,

        /// \brief Renders a collection of independent lines specified by
        /// pairs of vertices.
        LINES = 0x00000001,

        /// \brief Renders a line loop by connecting each new vertex to the
        /// previous one, and closing the loop by connecting the last
        /// vertex to the first.
        LINE_LOOP = 0x00000002,

        /// \brief Renders a line strip by connecting each new vertex to
        /// the previous one.
        LINE_STRIP = 0x00000003,

        /// \brief Renders a collection of independent triangles specified
        /// by groups of three vertices.
        TRIANGLES = 0x00000004,

        /// \brief Renders a triangle strip, where each new vertex forms a
        /// triangle with the two previous vertices.
        TRIANGLE_STRIP = 0x00000005,

        /// \brief Renders a triangle fan, where each new vertex forms a
        /// triangle with the previous vertex and the first vertex in the
        /// primitive.
        TRIANGLE_FAN = 0x00000006,

        /// \brief Renders a collection of independent quadrilaterals
        /// specified by groups of four vertices.
        QUADS = 0x00000007,

        /// \brief Renders a quadrilateral strip, where each pair of new
        /// vertices forms a quadrilateral with the previous pair of
        /// vertices.
        QUAD_STRIP = 0x00000008,

        /// \brief Renders a single polygon by connecting each vertex to
        /// the next, closing the loop by connecting the last vertex to the
        /// first.
        POLYGON = 0x00000009,

        /// \brief Renders a collection of independent line segments with
        /// adjacency information.
        ///
        /// Each group of four vertices specifies a line segment connecting
        /// the second and third vertices.  The segment is treated as
        /// though it were connected to segments connecting the first and
        /// second vertices and the third and fourth vertices.  If a
        /// geometry shader is present, it can process the line with the
        /// connecting information.  If no geometry shader is present, the
        /// primitive is rendered as independent lines, where the first and
        /// fourth vertex in each group is ignored.
        LINES_ADJACENCY = 0x0000000A,

        /// \brief Renders a collection of connected line segments with
        /// adjacency information.
        ///
        /// Each new vertex forms a group of four vertices with the three
        /// previous vertices, and the group is treated a forming line
        /// segment connecting the second and third vertices of the group.
        /// The segment is treated as though it were connected to segments
        /// connecting the first and second vertices and the third and
        /// fourth vertices.  If a geometry shader is present, it can
        /// process the line with the connecting information.  If no
        /// geometry shader is present, the primitive is rendered as
        /// independent lines, where the first and fourth vertex in each
        /// group are ignored.
        LINE_STRIP_ADJACENCY = 0x0000000B,

        /// \brief Renders a collection of independent triangles with
        /// adjacency information.
        ///
        /// Each group of six vertices specifies a triangle connecting the
        /// first, third, and fifth vertices.  The triangles is treated as
        /// though it were connected to triangles connecting the first,
        /// second, and third vertices, the third, fourth, and fifth
        /// vertices, and the fifth, sixth, and first vertices of the
        /// group.  If a geometry shader is present, it can process the
        /// triangle with the connecting information.  If no geometry
        /// shader is present, the primitive is rendered as independent
        /// triangles, where the second, fourth, and sixth vertices in each
        /// group are ignored.
        TRIANGLES_ADJACENCY = 0x0000000C,

        /// \brief Renders a strip of connected triangles with adjacency
        /// information.
        ///
        /// Each group of six vertices specifies a triangle connecting the
        /// first, third, and fifth vertices.  The triangles is treated as
        /// though it were connected to triangles connecting the first,
        /// second, and third vertices, the third, fourth, and fifth
        /// vertices, and the fifth, sixth, and first vertices of the
        /// group.  If a geometry shader is present, it can process the
        /// triangle with the connecting information.  If no geometry
        /// shader is present, the primitive is rendered as independent
        /// triangles, where the second, fourth, and sixth vertices in each
        /// group are ignored.
        TRIANGLE_STRIP_ADJACENCY = 0x0000000D,

        /// \brief Renders a collection of independent patches.
        ///
        /// Each group of N vertices is processed as a separate patch by
        /// tessellation control and evaluation shaders.  The value N is
        /// specified by CommandBuffer::SetPatchSize.  If no tessellation
        /// evaluation or geometry shader is present, the patch will be
        /// rendered as independent points.
        PATCHES = 0x0000000E,

        NVN_ENUM_32BIT(DRAW_PRIMITIVE),
    };
    NVN_ENUM_CLASS(DrawPrimitive, NVNdrawPrimitive);
};

/// \brief Specifies a data type used for index values in DrawElements
/// commands.
struct IndexType {

    /// \brief Enumerated values for the type IndexType.
    enum Enum {

        /// \brief Index values are specified as (8-bit) unsigned bytes.
        UNSIGNED_BYTE = 0x00000000,

        /// \brief Index values are specified as 16-bit unsigned integers.
        UNSIGNED_SHORT = 0x00000001,

        /// \brief Index values are specified as 32-bit unsigned integers.
        UNSIGNED_INT = 0x00000002,

        NVN_ENUM_32BIT(INDEX_TYPE),
    };
    NVN_ENUM_CLASS(IndexType, NVNindexType);
};

/// \brief Specifies a function used to compare 'test' and 'render target'
/// values for depth tests, where the 'test' value is the depth value for
/// the fragment.
struct DepthFunc {

    /// \brief Enumerated values for the type DepthFunc.
    enum Enum {

        /// \brief Comparison always fails.
        NEVER = 0x00000001,

        /// \brief Comparison passes if the test value is less than the
        /// render target value.
        LESS = 0x00000002,

        /// \brief Comparison passes if the test value is equal to the
        /// render target value.
        EQUAL = 0x00000003,

        /// \brief Comparison passes if the test value is less than or
        /// equal to the render target value.
        LEQUAL = 0x00000004,

        /// \brief Comparison passes if the test value is greater than the
        /// render target value.
        GREATER = 0x00000005,

        /// \brief Comparison passes if the test value is not equal to
        /// render target value.
        NOTEQUAL = 0x00000006,

        /// \brief Comparison passes if the test value is greater than or
        /// equal to the render target value.
        GEQUAL = 0x00000007,

        /// \brief Comparison always passes.
        ALWAYS = 0x00000008,

        NVN_ENUM_32BIT(DEPTH_FUNC),
    };
    NVN_ENUM_CLASS(DepthFunc, NVNdepthFunc);
};

/// \brief Specifies a magnification filter used for texture sampling.
///
/// When sampling from a texture, the magnification filter is used if the
/// GPU determines that the texture is being magnified (where each texel is
/// one pixel or larger in screen space).
struct MagFilter {

    /// \brief Enumerated values for the type MagFilter.
    enum Enum {

        /// \brief Returns the value of the nearest texel to the sample
        /// location.
        NEAREST = 0x00000000,

        /// \brief Returns the weighted average of the nearest texels to
        /// the sample location.
        LINEAR = 0x00000001,

        NVN_ENUM_32BIT(MAG_FILTER),
    };
    NVN_ENUM_CLASS(MagFilter, NVNmagFilter);
};

/// \brief Specifies a minification filter used for texture sampling.
///
/// When sampling from a texture, the minification filter is used if the
/// GPU determines that the texture is being minified (where each texel is
/// smaller than one pixel in screen space).  If the texture being sampled
/// includes mipmaps (lower-resolution versions of the full-resolution
/// 'base' image), the minification filter may read values from the mipmap
/// images.
struct MinFilter {

    /// \brief Enumerated values for the type MinFilter.
    enum Enum {

        /// \brief Returns the value of the nearest texel to the sample
        /// location in the base texture image.
        NEAREST = 0x00000000,

        /// \brief Returns the weighted average of the nearest texels to
        /// the sample location in the base texture image.
        LINEAR = 0x00000001,

        /// \brief Returns the value of the nearest texel to the sample
        /// location in a single mipmap image.
        NEAREST_MIPMAP_NEAREST = 0x00000002,

        /// \brief Returns the weighted average of the nearest texels to
        /// the sample location in a single mipmap image.
        LINEAR_MIPMAP_NEAREST = 0x00000003,

        /// \brief Determines the value of the nearest texel to the sample
        /// location in each of two mipmap images and returns a weighted
        /// average of those values.
        NEAREST_MIPMAP_LINEAR = 0x00000004,

        /// \brief Computes a weighted average of the nearest texels to the
        /// sample location in each of two mipmap images and returns a
        /// weighted average of those computed values.
        LINEAR_MIPMAP_LINEAR = 0x00000005,

        NVN_ENUM_32BIT(MIN_FILTER),
    };
    NVN_ENUM_CLASS(MinFilter, NVNminFilter);
};

/// \brief Specifies a wrap mode when mapping a normalized texture
/// coordinate to texels in a texture map.
///
/// When sampling from a texture, shaders typically provide normalized
/// coordinate values in the range [0,1] that are mapped to texels in a
/// texture map.  Values outside the range [0,1] are legal, and are mapped
/// to texels using the wrap mode.
struct WrapMode {

    /// \brief Enumerated values for the type WrapMode.
    enum Enum {

        /// \brief Texture coordinates are clamped to the range [0,1]
        /// before filtering.
        CLAMP = 0x00000000,

        /// \brief When filtering, texels outside the [0,1] range are
        /// mapped back to values in [0,1] by using only the fractional
        /// portion of the coordinate.
        REPEAT = 0x00000001,

        /// \brief Texture coordinates are mirrored around zero and then
        /// treated like CLAMP.
        MIRROR_CLAMP = 0x00000002,

        /// \brief Texture coordinates are mirrored around zero and then
        /// treated like CLAMP_TO_EDGE.
        MIRROR_CLAMP_TO_EDGE = 0x00000003,

        /// \brief Texture coordinates are mirrored around zero and then
        /// treated like CLAMP_TO_BORDER.
        MIRROR_CLAMP_TO_BORDER = 0x00000004,

        /// \brief When filtering, texels outside the [0,1] range are
        /// treated as sampling the border color.
        CLAMP_TO_BORDER = 0x00000005,

        /// \brief Texture coordinates are mirrored around zero and then
        /// treated like REPEAT.
        MIRRORED_REPEAT = 0x00000006,

        /// \brief When filtering, texels outside the [0,1] range are
        /// treated as sampling the texel on the nearest edge or corner of
        /// the texture.
        CLAMP_TO_EDGE = 0x00000007,

        NVN_ENUM_32BIT(WRAP_MODE),
    };
    NVN_ENUM_CLASS(WrapMode, NVNwrapMode);
};

/// \brief Specifies whether a texture is sampled.
///
/// For textures containing depth values, a texture can be sampled by
/// returning filtered depth values directly.  Such textures can also be
/// treated as a 'shadow map' by comparing each depth value with a
/// reference 'R' coordinate to produce a result of 0.0 or 1.0.  When depth
/// comparisons are enabled, a final texture value is produced by filtering
/// the 0.0/1.0 comparison results.  When comparisons are enabled, one of
/// the comparison functions in CompareFunc are used for the comparison.
///
/// \note When comparisons are enabled, the reference 'R' coordinate is
/// converted to the format of the texture prior to the comparison.
/// \warning If comparisons are enabled for a texture with no depth
/// component, the results of texture filtering are undefined.
struct CompareMode {

    /// \brief Enumerated values for the type CompareMode.
    enum Enum {

        /// \brief Returns filtered depth values without performing any
        /// comparison.
        NONE = 0x00000000,

        /// \brief Compares each depth value sampled with a reference value
        /// and returns filtered results.
        COMPARE_R_TO_TEXTURE = 0x00000001,

        NVN_ENUM_32BIT(COMPARE_MODE),
    };
    NVN_ENUM_CLASS(CompareMode, NVNcompareMode);
};

/// \brief Specifies a function used to compare 'texture' and 'reference'
/// values.
struct CompareFunc {

    /// \brief Enumerated values for the type CompareFunc.
    enum Enum {

        /// \brief Comparison always fails.
        NEVER = 0x00000001,

        /// \brief Comparison passes if the reference value is less than
        /// the texture value.
        LESS = 0x00000002,

        /// \brief Comparison passes if the reference value is equal to the
        /// texture value.
        EQUAL = 0x00000003,

        /// \brief Comparison passes if the reference value is less than or
        /// equal to the texture value.
        LEQUAL = 0x00000004,

        /// \brief Comparison passes if the reference value is greater than
        /// the texture value.
        GREATER = 0x00000005,

        /// \brief Comparison passes if the reference value is not equal to
        /// texture value.
        NOTEQUAL = 0x00000006,

        /// \brief Comparison passes if the reference value is greater than
        /// or equal to the texture value.
        GEQUAL = 0x00000007,

        /// \brief Comparison always passes.
        ALWAYS = 0x00000008,

        NVN_ENUM_32BIT(COMPARE_FUNC),
    };
    NVN_ENUM_CLASS(CompareFunc, NVNcompareFunc);
};

/// \brief Specifies a reduction filter used for texture sampling.
struct SamplerReduction {

    /// \brief Enumerated values for the type SamplerReduction.
    enum Enum {

        /// \brief Computes a weighted average of samples accessed for
        /// texture sampling.
        AVERAGE = 0x00000000,

        /// \brief Returns the per-component minimum value across samples
        /// accessed for texture sampling.
        MIN = 0x00000001,

        /// \brief Returns the per-component maximum value across samples
        /// accessed for texture sampling.
        MAX = 0x00000002,

        NVN_ENUM_32BIT(SAMPLER_REDUCTION),
    };
    NVN_ENUM_CLASS(SamplerReduction, NVNsamplerReduction);
};

/// \brief Specifies the set of primitives (front- or back-facing) that a
/// command affects.
///
/// Polygon primitives are considered front- or back-facing based on their
/// orientation in screen space.  Certain operations (culling, stencil
/// test) can treat front- and back-facing primitives differently.  This
/// enumerant is used to indicate which type(s) of primitives an operation
/// affects.
struct Face {

    /// \brief Enumerated values for the type Face.
    enum Enum {

        /// \brief Specifies that an operation applies to neither front-
        /// nor back-facing primitives.
        NONE = 0x00000000,

        /// \brief Specifies that an operation applies to front-facing
        /// primitives only.
        FRONT = 0x00000001,

        /// \brief Specifies that an operation applies to back-facing
        /// primitives only.
        BACK = 0x00000002,

        /// \brief Specifies that an operation applies to both front- and
        /// back-facing primitives.
        FRONT_AND_BACK = 0x00000003,

        NVN_ENUM_32BIT(FACE),
    };
    NVN_ENUM_CLASS(Face, NVNface);
};

/// \brief Specifies a function used to compare 'test' and 'render target'
/// values for stencil tests, where the 'test' value is the fragment
/// stencil value generated by the given facing and stencil reference and
/// mask values.
struct StencilFunc {

    /// \brief Enumerated values for the type StencilFunc.
    enum Enum {

        /// \brief Comparison always fails.
        NEVER = 0x00000001,

        /// \brief Comparison passes if the test value is less than the
        /// render target value.
        LESS = 0x00000002,

        /// \brief Comparison passes if the test value is equal to the
        /// render target value.
        EQUAL = 0x00000003,

        /// \brief Comparison passes if the test value is less than or
        /// equal to the render target value.
        LEQUAL = 0x00000004,

        /// \brief Comparison passes if the test value is greater than the
        /// render target value.
        GREATER = 0x00000005,

        /// \brief Comparison passes if the test value is not equal to
        /// render target value.
        NOTEQUAL = 0x00000006,

        /// \brief Comparison passes if the test value is greater than or
        /// equal to the render target value.
        GEQUAL = 0x00000007,

        /// \brief Comparison always passes.
        ALWAYS = 0x00000008,

        NVN_ENUM_32BIT(STENCIL_FUNC),
    };
    NVN_ENUM_CLASS(StencilFunc, NVNstencilFunc);
};

/// \brief Specifies an operation performed on stencil values in the
/// stencil test.
///
/// When a stencil test is performed, the value stored in the stencil
/// buffer can be updated based on the results of the depth and stencil
/// tests.
struct StencilOp {

    /// \brief Enumerated values for the type StencilOp.
    enum Enum {

        /// \brief Do not modify the stencil value.
        KEEP = 0x00000001,

        /// \brief Set the stencil value to zero.
        ZERO = 0x00000002,

        /// \brief Replace the stencil value with the reference value.
        REPLACE = 0x00000003,

        /// \brief Increment the stencil value, clamping the result to the
        /// maximum value.
        INCR = 0x00000004,

        /// \brief Decrement the stencil value, clamping the result to
        /// zero.
        DECR = 0x00000005,

        /// \brief Invert all bits in the stencil value.
        INVERT = 0x00000006,

        /// \brief Increment the stencil value, where values larger than
        /// the maximum value wrap to zero.
        INCR_WRAP = 0x00000007,

        /// \brief Decrement the stencil value, where values less than zero
        /// wrap to the maximum value.
        DECR_WRAP = 0x00000008,

        NVN_ENUM_32BIT(STENCIL_OP),
    };
    NVN_ENUM_CLASS(StencilOp, NVNstencilOp);
};

/// \brief Specifies the orientation of front-facing polygon primitives.
///
/// When polygon primitives are rasterized, the GPU computes a signed area,
/// determining if the vertices are in clockwise or counter-clockwise
/// order.  This enumerant identifies which orientation is treated as
/// front-facing (pointed toward the viewer).
struct FrontFace {

    /// \brief Enumerated values for the type FrontFace.
    enum Enum {

        /// \brief Clockwise primitives are considered front-facing.
        CW = 0x00000000,

        /// \brief Counter-clockwise primitives are considered
        /// front-facing.
        CCW = 0x00000001,

        NVN_ENUM_32BIT(FRONT_FACE),
    };
    NVN_ENUM_CLASS(FrontFace, NVNfrontFace);
};

/// \brief Specifies how polygon primitives should be rasterized.
struct PolygonMode {

    /// \brief Enumerated values for the type PolygonMode.
    enum Enum {

        /// \brief Polygon primitives should be rasterized as points.
        POINT = 0x00000000,

        /// \brief Polygon primitives should be rasterized as outlines.
        LINE = 0x00000001,

        /// \brief Polygon primitives should be rasterized as filled
        /// polygons.
        FILL = 0x00000002,

        NVN_ENUM_32BIT(POLYGON_MODE),
    };
    NVN_ENUM_CLASS(PolygonMode, NVNpolygonMode);
};

/// \brief Specifies whether the depth values of rasterized polygon
/// primitives should be offset.
///
/// When rasterizing polygon primitives, the GPU can offset depth values
/// for the primitive to primitives with similar depth or to ensure that a
/// wireframe outline of a filled primitive is always on top of the filled
/// primitive.  Offset can be enabled separately for polygons drawn in
/// point, line, or filled modes.
struct PolygonOffsetEnable {

    /// \brief Enumerated values for the type PolygonOffsetEnable.
    enum Enum {

        /// \brief Polygon primitives rendered in any mode should not be
        /// offset.
        NONE = 0x00000000,

        /// \brief Polygon primitives rendered in POINT mode should be
        /// offset.
        POINT = 0x00000001,

        /// \brief Polygon primitives rendered in LINE mode should be
        /// offset.
        LINE = 0x00000002,

        /// \brief Polygon primitives rendered in FILL mode should be
        /// offset.
        FILL = 0x00000004,

        NVN_ENUM_32BIT(POLYGON_OFFSET_ENABLE),
    };
    NVN_BITFIELD_CLASS(PolygonOffsetEnable, NVNpolygonOffsetEnable);
};

/// \brief Specifies the condition required for a fence to signal its sync
/// object.
struct SyncCondition {

    /// \brief Enumerated values for the type SyncCondition.
    enum Enum {

        /// \brief The sync object should be signaled when all previous
        /// commands have fully completed on the GPU.
        ALL_GPU_COMMANDS_COMPLETE = 0x00000000,

        /// \brief The sync object should be signaled when the vertices of
        /// previous commands have been fully processed by the GPU.
        GRAPHICS_WORLD_SPACE_COMPLETE = 0x00000001,

        NVN_ENUM_32BIT(SYNC_CONDITION),
    };
    NVN_ENUM_CLASS(SyncCondition, NVNsyncCondition);
};

/// \brief Specifies the result of waiting on a sync object.
struct SyncWaitResult {

    /// \brief Enumerated values for the type SyncWaitResult.
    enum Enum {

        /// \brief The sync object was already signaled when the wait was
        /// requested.
        ALREADY_SIGNALED = 0x00000000,

        /// \brief The sync object not signaled when the wait was
        /// requested, but was signaled prior to the end of the wait
        /// period.
        CONDITION_SATISFIED = 0x00000001,

        /// \brief The condition was not satisfied before the wait period
        /// expired.
        TIMEOUT_EXPIRED = 0x00000002,

        /// \brief An error occured while waiting on the sync object.
        ///
        /// Waiting on a sync object that has not been signaled with a
        /// fence command is an error and will produce a result of FAILED.
        FAILED = 0x00000003,

        NVN_ENUM_32BIT(SYNC_WAIT_RESULT),
    };
    NVN_ENUM_CLASS(SyncWaitResult, NVNsyncWaitResult);
};

/// \brief Specifies the result of acquiring a texture from a window.
struct QueueAcquireTextureResult {

    /// \brief Enumerated values for the type QueueAcquireTextureResult.
    enum Enum {

        /// \brief The texture was acquired successfully.
        SUCCESS = 0x00000000,

        /// \brief Native window system error. This error is not
        /// recoverable.
        NATIVE_ERROR = 0x00000001,

        NVN_ENUM_32BIT(QUEUE_ACQUIRE_TEXTURE_RESULT),
    };
    NVN_ENUM_CLASS(QueueAcquireTextureResult, NVNqueueAcquireTextureResult);
};

/// \brief Specifies the result of acquiring a texture from a window.
struct WindowAcquireTextureResult {

    /// \brief Enumerated values for the type WindowAcquireTextureResult.
    enum Enum {

        /// \brief The texture was acquired successfully.
        SUCCESS = 0x00000000,

        /// \brief Native window system error. This error is not
        /// recoverable.
        NATIVE_ERROR = 0x00000001,

        NVN_ENUM_32BIT(WINDOW_ACQUIRE_TEXTURE_RESULT),
    };
    NVN_ENUM_CLASS(WindowAcquireTextureResult, NVNwindowAcquireTextureResult);
};

/// \brief Specifies a programmable shader stage.
struct ShaderStage {

    /// \brief Enumerated values for the type ShaderStage.
    enum Enum {

        /// \brief Specifies the vertex shader stage.
        VERTEX = 0x00000000,

        /// \brief Specifies the fragment shader stage.
        FRAGMENT = 0x00000001,

        /// \brief Specifies the geometry shader stage.
        GEOMETRY = 0x00000002,

        /// \brief Specifies the tessellation control shader stage.
        TESS_CONTROL = 0x00000003,

        /// \brief Specifies the tessellation evaluation shader stage.
        TESS_EVALUATION = 0x00000004,

        /// \brief Specifies the compute shader stage.
        COMPUTE = 0x00000005,

        NVN_ENUM_32BIT(SHADER_STAGE),
    };
    NVN_ENUM_CLASS(ShaderStage, NVNshaderStage);
};

/// \brief Used in TiledCacheDecideAction to suggest whether the tiled
/// cache should be flushed or enabled/disabled.
struct TiledCacheAction {

    /// \brief Enumerated values for the type TiledCacheAction.
    enum Enum {

        /// \brief Tiled caching should be enabled so subsequent work can
        /// be binned.
        ENABLE = 1,

        /// \brief Tiled caching should be disabled so subsequent work is
        /// not binned.
        ///
        /// If tiled caching is enabled, this action will first flush the
        /// binner.
        DISABLE = 2,

        /// \brief Tiled cache binner is flushed with tiling and subsequent
        /// work is binned in a new render pass.
        FLUSH = 3,

        /// \brief Tiled cache binner is flushed as untiled and subsequent
        /// work is binned in a new render pass.
        FLUSH_NO_TILING = 4,

        /// \brief Enables subsequent binning of render target state
        /// changes.
        ///
        /// If tiled caching is enabled, this action will first flush the
        /// binner.
        ENABLE_RENDER_TARGET_BINNING = 5,

        /// \brief Disables subsequent binning of render target state
        /// changes.
        ///
        /// If tiled caching is enabled, this action will first flush the
        /// binner.
        DISABLE_RENDER_TARGET_BINNING = 6,

        NVN_ENUM_32BIT(TILED_CACHE_ACTION),
    };
    NVN_ENUM_CLASS(TiledCacheAction, NVNtiledCacheAction);
};

/// \brief Specifies a set of programmable shader stages.
struct ShaderStageBits {

    /// \brief Enumerated values for the type ShaderStageBits.
    enum Enum {

        /// \brief Specifies the vertex shader stage.
        VERTEX = 0x00000001,

        /// \brief Specifies the fragment shader stage.
        FRAGMENT = 0x00000002,

        /// \brief Specifies the geometry shader stage.
        GEOMETRY = 0x00000004,

        /// \brief Specifies the tessellation control shader stage.
        TESS_CONTROL = 0x00000008,

        /// \brief Specifies the tessellation evaluation shader stage.
        TESS_EVALUATION = 0x00000010,

        /// \brief Specifies the compute shader stage.
        COMPUTE = 0x00000020,

        /// \brief Specifies all shader stages used to process graphics
        /// primitives.
        ///
        /// Includes all stages except for the compute shader stage, which
        /// is used to process compute shader dispatches.
        ALL_GRAPHICS_BITS = 0x0000001F,

        NVN_ENUM_32BIT(SHADER_STAGE_BITS),
    };
    NVN_BITFIELD_CLASS(ShaderStageBits, NVNshaderStageBits);
};

/// \brief Specifies additional operations performed when a FenceSync
/// command is processed.
struct SyncFlagBits {

    /// \brief Enumerated values for the type SyncFlagBits.
    enum Enum {

        /// \brief Ensures that all GPU memory transactions performed for
        /// commands prior to the fence will be visible to the CPU when the
        /// sync object is signaled.
        FLUSH_FOR_CPU = 0x00000001,

        NVN_ENUM_32BIT(SYNC_FLAG_BITS),
    };
    NVN_BITFIELD_CLASS(SyncFlagBits, NVNsyncFlagBits);
};

/// \brief Specifies additional flags when creating a device.
struct DeviceFlagBits {

    /// \brief Enumerated values for the type DeviceFlagBits.
    enum Enum {

        /// \brief Enable (TRUE) or disable (FALSE) the debug layer for the
        /// device.
        ///
        /// If the debug layer is enabled on NX, the application must call
        /// nv::SetGraphicsDevtoolsAllocator prior to nvnDeviceInitialize
        /// to provide an allocator that can be used by the debug layer.
        DEBUG_ENABLE = 0x00000001,

        /// \brief If non-zero, the debug layer will skip API calls where
        /// errors are detected.
        ///
        /// By default, the debug layer will continue execution after
        /// detecting and reporting errors and should behave the same as
        /// when the debug layer is disabled.  When this flag is set, the
        /// debug layer will return control to the caller when errors are
        /// detected without executing the command.  This feature is
        /// present for testing purposes and is not expected to be used by
        /// regular applications.
        DEBUG_SKIP_CALLS_ON_ERROR = 0x00000002,

        /// \brief If non-zero, the debug layer will perform draw-time
        /// validation checks.
        ///
        /// Enable extra draw-time validation from the debug layer. The
        /// debug layer must be enabled for this flag to take effect.
        /// Draw-time validation will catch common mistakes that would
        /// otherwise lead to hardware errors or hangs, but the additional
        /// state tracking required may further reduce performance.  When
        /// issuing draw commands, the debug layer will verify that
        /// previous commands have put the GPU in a valid state for
        /// drawing.
        DEBUG_DRAW_VALIDATION = 0x00000004,

        /// \brief Enable deferred finalization of textures, buffers and
        /// sampler objects.
        ///
        /// If non-zero, finalization of textures, buffers and sampler
        /// objects will be not be done immediately, but rather deferred to
        /// a later call to Device::ApplyDeferredFinalizes.
        DEFERRED_FINALIZE = 0x00000008,

        /// \brief If non-zero, the debug layer will perform full draw-time
        /// validation checks.
        ///
        /// Enable comprehensive draw-time validation from the debug layer.
        /// The both debug layer and draw-time validation must be enabled
        /// for this flag to take effect.  This will enable the most checks
        /// in debug layer, but also has the most impact on performance.
        DEBUG_DRAW_VALIDATION_HEAVY = 0x00000010,

        /// \brief Enable (TRUE) or disable (FALSE) level 0 debug layer for
        /// the device.
        ///
        /// Enables minimal debug layer features. Object pointer validation
        /// is not performed.  Only performs simple parameter checks, and
        /// invalid API usage, cases where the driver would return FALSE.
        DEBUG_ENABLE_LEVEL_0 = 0x00000020,

        /// \brief Enable (TRUE) or disable (FALSE) level 1 debug layer for
        /// the device.
        ///
        /// Enables object validation and better handwritten checks, on top
        /// of debug layer level 0.
        DEBUG_ENABLE_LEVEL_1 = 0x00000040,

        /// \brief Enable (TRUE) or disable (FALSE) level 2 debug layer for
        /// the device.
        ///
        /// Enables memory pool object tracking on top of previous debug
        /// layer levels.  This allows for the debug layer to notify when
        /// textures, buffers and samplers are not finalized when the
        /// underlying memory pool is finalized.  This flag is synonymous
        /// with DEBUG_ENABLE for backwards compatibility.
        DEBUG_ENABLE_LEVEL_2 = 0x00000001,

        /// \brief Enable (TRUE) or disable (FALSE) level 3 debug layer for
        /// the device.
        ///
        /// Enables draw time validation on top of previous debug layer
        /// levels.  This allows for the debug layer to perform additional
        /// checks for invalid queue states on every draw call or compute
        /// dispatch, at queue submit time.  This flag is synonymous with
        /// DEBUG_DRAW_VALIDATION for backwards compatibility.
        DEBUG_ENABLE_LEVEL_3 = 0x00000004,

        /// \brief Enable (TRUE) or disable (FALSE) level 4 debug layer for
        /// the device.
        ///
        /// Enables full draw time validation on top of previous debug
        /// layer levels.  This allows for the debug layer to perform heavy
        /// draw-time validation checks such as  reporting inflight command
        /// buffer command memory being reused.  This flag is synonymous
        /// with DEBUG_DRAW_VALIDATION_HEAVY for backwards compatibility.
        DEBUG_ENABLE_LEVEL_4 = 0x00000010,

        /// \brief Enable (TRUE) or disable (FALSE) deferred deletion of
        /// firmware memory.
        ///
        /// By default, NX firmware memory used to store GPU page table
        /// entries will be freed when memory pools are finalized. If this
        /// bit is set, the firmware memory will be retained for future
        /// memory pool mappings and will not be freed until the device is
        /// finalized. This setting has no effect on the Windows reference
        /// implementation.
        ///
        /// \note This flag will be deprecated in the future.
        DEFERRED_FIRMWARE_MEMORY_RECLAIM = 0x00000080,

        /// \brief Enable (TRUE) or disable (FALSE) separate
        /// sampler/texture support.
        ///
        /// Separate sampler and texture support allows applications to
        /// execute shaders using separate sets of binding points for
        /// samplers (with no texture information) and textures (with no
        /// sampler information).  When using separate texture and sampler
        /// bindings, independent texture-only and sampler-only handles are
        /// combined in shader code when performing a texture lookup.
        ///
        /// Supporting separate texture and sampler bindings requires
        /// additional per-queue memory even if the underlying shaders do
        /// not use separate samplers and textures.  It is recommended to
        /// only enable this flag if you plan on using programs compiled
        /// from shaders containing separate sampler and textures.
        ///
        /// If this flag is not set, applications may not use separate
        /// texture and sampler bindings
        /// (CommandBuffer::BindSeparateTexture,
        /// CommandBuffer::BindSeparateSampler).
        ENABLE_SEPARATE_SAMPLER_TEXTURE_SUPPORT = 0x00000100,

        NVN_ENUM_32BIT(DEVICE_FLAG_BITS),
    };
    NVN_BITFIELD_CLASS(DeviceFlagBits, NVNdeviceFlagBits);
};

/// \brief Specifies the set of color buffer channels enabled for writing.
struct ClearColorMask {

    /// \brief Enumerated values for the type ClearColorMask.
    enum Enum {

        /// \brief Enables writes to the first (R) component of the color
        /// buffer.
        R = 0x00000001,

        /// \brief Enables writes to the second (G) component of the color
        /// buffer.
        G = 0x00000002,

        /// \brief Enables writes to the third (B) component of the color
        /// buffer.
        B = 0x00000004,

        /// \brief Enables writes to the fourth (A) component of the color
        /// buffer.
        A = 0x00000008,

        /// \brief Enables writes to all components of the color buffer.
        RGBA = 0x0000000F,

        NVN_ENUM_32BIT(CLEAR_COLOR_MASK),
    };
    NVN_BITFIELD_CLASS(ClearColorMask, NVNclearColorMask);
};

/// \brief Specifies the type of operation being performed when a debug
/// callback is made.
struct DebugCallbackSource {

    /// \brief Enumerated values for the type DebugCallbackSource.
    enum Enum {

        /// \brief A debug callback is being issued while processing an API
        /// command.
        API = 0x00000000,

        /// \brief A debug callback is being issued while processing tools
        /// command.
        TOOLS = 0x00000001,

        NVN_ENUM_32BIT(DEBUG_CALLBACK_SOURCE),
    };
    NVN_ENUM_CLASS(DebugCallbackSource, NVNdebugCallbackSource);
};

/// \brief Specifies the type of event triggering a debug callback.
struct DebugCallbackType {

    /// \brief Enumerated values for the type DebugCallbackType.
    enum Enum {

        /// \brief A debug callback is being issued because of an error in
        /// API usage.
        API_ERROR = 0x00000000,

        /// \brief A debug callback is being issued because of any warning
        /// in API usage.
        API_WARNING = 0x00000001,

        /// \brief A debug callback is being issued because of an error in
        /// tools usage.
        TOOLS_ERROR = 0x00000002,

        /// \brief A debug callback is being issued because of any warning
        /// in tools usage.
        TOOLS_WARNING = 0x00000003,

        NVN_ENUM_32BIT(DEBUG_CALLBACK_TYPE),
    };
    NVN_ENUM_CLASS(DebugCallbackType, NVNdebugCallbackType);
};

/// \brief Specifies the severity of the event triggering a debug callback.
struct DebugCallbackSeverity {

    /// \brief Enumerated values for the type DebugCallbackSeverity.
    enum Enum {

        /// \brief A debug callback is issued for errors in API usage or
        /// conditions that may lead to GPU exceptions.
        HIGH = 0x00000000,

        /// \brief A debug callback is issued to notify about potentially
        /// significant performance or undefined behavior problems.
        MEDIUM = 0x00000001,

        /// \brief A debug callback is issued to notify about minor
        /// performance or undefined behavior problems.
        LOW = 0x00000002,

        /// \brief A debug callback is issued to report a condition that is
        /// neither illegal.
        NOTIFICATION = 0x00000003,

        NVN_ENUM_32BIT(DEBUG_CALLBACK_SEVERITY),
    };
    NVN_ENUM_CLASS(DebugCallbackSeverity, NVNdebugCallbackSeverity);
};

/// \brief Specifies properties of memory pools created from a memory pool
/// builder.
struct MemoryPoolFlags {

    /// \brief Enumerated values for the type MemoryPoolFlags.
    enum Enum {

        /// \brief Indicates that the memory pool will not be accessed by
        /// the CPU.
        ///
        /// When CPU_NO_ACCESS is set, the pool memory may not be accessed
        /// by the CPU.
        ///
        /// Applications must set exactly one of CPU_NO_ACCESS,
        /// CPU_UNCACHED, and CPU_CACHED when initializing a memory pool.
        ///
        /// \note On NX, initializing a pool with CPU_NO_ACCESS does not
        /// change existing CPU memory mappings and thus does not prevent
        /// applications from accessing the pool memory using the pointer
        /// provided to nvn::MemoryPoolBuilder::SetStorage.
        CPU_NO_ACCESS = 0x00000001,

        /// \brief Indicates that the memory pool will be accessed by the
        /// CPU without caching.
        ///
        /// When CPU_UNCACHED is set, writes to pool memory with the CPU
        /// will be visible to subsequent commands submitted to the GPU.
        /// CPU reads from pool memory will reflect the results of all GPU
        /// commands that have completed execution at the time of the read.
        ///  CPU_UNCACHED memory will not be cached in the CPU's internal
        /// caches, which will result in significantly reduced performance
        /// for reads and random-access writes.  Sequential writes to
        /// CPU_UNCACHED memory will have good performance.
        ///
        /// Applications must set exactly one of CPU_NO_ACCESS,
        /// CPU_UNCACHED, and CPU_CACHED when initializing a memory pool.
        CPU_UNCACHED = 0x00000002,

        /// \brief Indicates that the memory pool will be accessed by the
        /// CPU with caching.
        ///
        /// When CPU_CACHED is set, writes to pool memory with the NX CPU
        /// will be stored in a write-back CPU cache and may not be visible
        /// to subsequent commands submitted to the GPU.
        /// MemoryPool::FlushMappedRange or Buffer::FlushMappedRange must
        /// be called to ensure such CPU writes are visible to the GPU.
        /// CPU reads from pool memory may return values from the CPU cache
        /// that do not reflect writes to main memory performed by
        /// previously completed GPU commands.
        /// MemoryPool::InvalidateMappedRange or
        /// Buffer::InvalidateMappedRange must be called to ensure such GPU
        /// writes are visible to the CPU.  On the Windows reference
        /// implementation, CPU_CACHED memory pools use separate copies of
        /// pool memory for CPU and GPU accesses.  FlushMappedRange and
        /// InvalidateMappedRange copy memory between the two copies.
        ///
        /// Applications must set exactly one of CPU_NO_ACCESS,
        /// CPU_UNCACHED, and CPU_CACHED when initializing a memory pool.
        CPU_CACHED = 0x00000004,

        /// \brief Indicates that the memory pool will not be accessed by
        /// the GPU.
        ///
        /// When GPU_NO_ACCESS is set, pool memory will not be accessed by
        /// the GPU.
        ///
        /// Applications must set exactly one of GPU_NO_ACCESS,
        /// GPU_UNCACHED, and GPU_CACHED when initializing a memory pool.
        GPU_NO_ACCESS = 0x00000008,

        /// \brief Indicates that the memory pool will be accessed by the
        /// GPU without caching.
        ///
        /// When GPU_UNCACHED is set, pool memory will not be cached in the
        /// GPU's internal caches, which will result in significantly
        /// reduced performance for GPU operations that repeatedly access
        /// the pool memory.  However, if pool memory is not fetched more
        /// than once by the GPU, GPU_UNCACHED may improve performance by
        /// not consuming GPU cache lines for resources in the pool.
        /// Command memory used by CommandBuffer objects may benefit from
        /// using GPU_UNCACHED.
        ///
        /// Applications must set exactly one of GPU_NO_ACCESS,
        /// GPU_UNCACHED, and GPU_CACHED when initializing a memory pool.
        GPU_UNCACHED = 0x00000010,

        /// \brief Indicates that the memory pool will be accessed by the
        /// GPU with caching.
        ///
        /// When GPU_CACHED is set, pool memory will be cached in the GPU's
        /// internal caches, which is the preferred behavior for most use
        /// cases.  CPU writes to GPU_CACHED memory pools will be visible
        /// to any commands submitted to the GPU after the write.  GPU
        /// writes to GPU_CACHED memory pools are guaranteed to be visible
        /// to the CPU only after waiting on a Sync object signalled by
        /// calling Queue::FenceSync with SyncFlagBits::FLUSH_FOR_CPU set.
        ///
        /// Applications must set exactly one of GPU_NO_ACCESS,
        /// GPU_UNCACHED, and GPU_CACHED when initializing a memory pool.
        GPU_CACHED = 0x00000020,

        /// \brief Indicates that the memory pool may be used to store
        /// shader code for execution.
        ///
        /// When SHADER_CODE is set, pool memory may be used to store
        /// executable shader code passed to Program::SetShaders using GPU
        /// addresses in ShaderData::data.  When this flag is set, the
        /// memory pool is mapped so that its contents can be used for
        /// shader execution.  If the flag is not set, GPU addresses in the
        /// pool may not be used to point at shaders.
        ///
        /// \note In the current NVN implementation, shaders may not be run
        /// from physical or virtual memory pools.  Memory pools with the
        /// SHADER_CODE flag set may not set either the PHYSICAL or VIRTUAL
        /// flag.
        ///
        /// \note Applications must avoid loading shaders into the last 1KB
        /// of a pool with SHADER_CODE set.  GPU shader cores may pre-fetch
        /// from instruction memory beyond the last byte of actual shader
        /// code, which could fault if the GPU virtual address space beyond
        /// the end of the pool is unpopulated.
        SHADER_CODE = 0x00000040,

        /// \brief Indicates that the memory pool supports compressible
        /// textures.
        ///
        /// When COMPRESSIBLE is set, this pool can be used to initialize
        /// compressible textures.  For compressible textures, the NVN
        /// driver will set up special hardware resources and memory
        /// mappings allowing the contents of the texture to be
        /// transparently compressed by the GPU to save memory bandwidth.
        /// This form of compression can significantly increase performance
        /// when using textures as render targets.  The COMPRESSIBLE bit is
        /// ignored for virtual memory pools, but should be set on physical
        /// memory pools providing storage for compressible textures.
        ///
        /// \warning It is an error to initialize compressible textures
        /// using non-virtual memory pools without the COMPRESSIBLE bit
        /// set.
        ///
        /// \warning When reusing memory from a compressible pool for
        /// compressible textures, applications must clear compressible
        /// textures before reading or writing with the GPU.  Failing to do
        /// so could result in undefined behavior including GPU errors.
        COMPRESSIBLE = 0x00000080,

        /// \brief Indicates that the memory pool may only be used as
        /// physical storage for memory mappings into virtual memory pools.
        ///
        /// When PHYSICAL is set, the memory pool is used only to provide
        /// storage that can be mapped into virtual memory pools.  Physical
        /// pools can not be used to create Texture or Buffer resources.
        /// Physical pools are not directly accessible by the GPU and must
        /// be initialized with GPU_NO_ACCESS set. When mapping memory into
        /// virtual pools, the pools providing memory must have been
        /// initalized with PHYSICAL set.
        ///
        /// \note The size of all physical memory pools must be a multiple
        /// of the GPU page size (DeviceInfo::MEMORY_POOL_PAGE_SIZE).
        ///
        /// \note On the Windows reference implementation, physical pools
        /// may not be initialized with the CPU_UNCACHED flag set.
        ///
        /// \note When a compressible texture is created in a virtual pool,
        /// the physical memory pool(s) providing storage for the texture
        /// must be initialized with the COMPRESSIBLE flag set.
        PHYSICAL = 0x00000100,

        /// \brief Indicates that the memory pool may be used for sparse
        /// operations.
        ///
        /// When VIRTUAL is set, the memory pool is used only to provide
        /// GPU-accessible virtual memory that can be used to create buffer
        /// and texture resources.  Virtual memory pools have no physical
        /// memory storage, and all pages of the virtual memory pool are
        /// initially unpopulated.  When reading unpopulated memory in the
        /// GPU, accesses will return zero (NX and second-generation
        /// Maxwell GPUs) or undefined values.   After a virtual memory
        /// pool is initialized, it can be populated with pages from
        /// physical pools using MemoryPool::MapVirtual.  Virtual pools are
        /// not directly accessible by the CPU and must be initialized with
        /// CPU_NO_ACCESS set.
        ///
        /// \note The size of all virtual memory pools must be a multiple
        /// of the GPU page size (DeviceInfo::MEMORY_POOL_PAGE_SIZE).
        VIRTUAL = 0x000000200,

        NVN_ENUM_32BIT(MEMORY_POOL_FLAGS),
    };
    NVN_BITFIELD_CLASS(MemoryPoolFlags, NVNmemoryPoolFlags);
};

/// \brief Specifies the type of counter value reported in a counter query.
struct CounterType {

    /// \brief Enumerated values for the type CounterType.
    enum Enum {

        /// \brief Doesn't report a counter; zero is written in the counter
        /// portion of the report structure.
        ///
        /// The timestamp for this report will be taken at the bottom of
        /// the GPU pipeline and will not be produced until previous
        /// primitives have been processed by the GPU.  Note that this
        /// timestamp does not wait on the completion of all memory stores
        /// resulting from the processing of previous primitives.
        TIMESTAMP = 0x00000000,

        /// \brief Reports the number of samples passing the depth and
        /// stencil test in the framebuffer.
        SAMPLES_PASSED = 0x00000001,

        /// \brief Reports the number of vertices processed in input
        /// primitives specified by Draw commands.
        INPUT_VERTICES = 0x00000002,

        /// \brief Reports the number of individual primitives processed in
        /// input primitives specified by Draw commands.
        ///
        /// For independent point, line, and triangle primitives, each
        /// point, line, or triangle is counted separately.  For strip,
        /// loop, and fan primitives, each individual line segment or
        /// triangle is counted separately.  Quadrilateral and polygon
        /// primitives will be decomposed into triangles, and each triangle
        /// will be counted separately.  For adjacency primitives, each
        /// 'main' line segment or triangle will be counted, but
        /// 'adjacency' line segments or triangles will not be counted.
        INPUT_PRIMITIVES = 0x00000003,

        /// \brief Reports the number of vertex shader invocations executed
        /// by the GPU.
        ///
        /// Even though each input vertex is logically processed by the
        /// vertex shader, this count may not match INPUT_VERTICES.  For
        /// example, if indices are duplicated, the underlying vertex might
        /// be processed only once. Additionally, long strip primitives may
        /// be split into batches, where shared vertices are processed
        /// multiple times.
        VERTEX_SHADER_INVOCATIONS = 0x00000004,

        /// \brief Reports the number of tessellation control shader
        /// invocations executed by the GPU.
        TESS_CONTROL_SHADER_INVOCATIONS = 0x00000005,

        /// \brief Reports the number of tessellation evaluation shader
        /// invocations executed by the GPU.
        ///
        /// When tessellating a patch primitive, the fixed-function
        /// tessellator may split a patch into multiple sub-patches, where
        /// vertices on the edges of sub-patches may be processed (and
        /// counted) multiple times.  Additionally, vertices in tessellated
        /// patches may be processed only once by the tessellation
        /// evaluation shader but used in multiple output primitives.
        TESS_EVALUATION_SHADER_INVOCATIONS = 0x00000006,

        /// \brief Reports the number of geometry shader invocations
        /// executed by the GPU.
        GEOMETRY_SHADER_INVOCATIONS = 0x00000007,

        /// \brief Reports the number of fragment shader invocations
        /// executed by the GPU.
        ///
        /// If no fragment shader is bound, no fragment shader invocations
        /// will be counted. If per-fragment tests such as the depth or
        /// stencil test are enabled, fragments may sometimes be discarded
        /// before invoking (and counting) the fragment shader.  If the
        /// framebuffer has multiple samples, the fragment shader may be
        /// executed multiple times per sample. If the GPU can determine
        /// that the results of the fragment shader will have no effect or
        /// if it can determine the results without executing the fragment
        /// shader, it may skip fragment shader invocations without
        /// counting them. For example, if the fragment shader produces a
        /// single output color, but the framebuffer has no color buffer
        /// bound or has color writes disabled, any color produced by the
        /// shader would be discarded.
        FRAGMENT_SHADER_INVOCATIONS = 0x00000008,

        /// \brief Reports the number of primitives generated by the
        /// fixed-function tessellator.
        TESS_EVALUATION_SHADER_PRIMITIVES = 0x00000009,

        /// \brief Reports the number of primitives produced by geometry
        /// shader invocations.
        GEOMETRY_SHADER_PRIMITIVES = 0x0000000A,

        /// \brief Reports the number of primitives processed by the
        /// primitive pipeline and sent to the clipper.
        CLIPPER_INPUT_PRIMITIVES = 0x0000000B,

        /// \brief Reports the number of primitives surviving the primitive
        /// clipping stage.
        CLIPPER_OUTPUT_PRIMITIVES = 0x0000000C,

        /// \brief Reports the number of primitives produced for clipping
        /// and rasterization.
        ///
        /// Captures the number of point, line, or triangle primitives
        /// produced for the clipping and rasterization stages to process.
        /// If rasterizer discard is enabled, primitives will be counted
        /// prior to being discarded.
        PRIMITIVES_GENERATED = 0x0000000D,

        /// \brief Reports the number of primitives captured during
        /// transform feedback.
        ///
        /// Captures the number of point, line, or triangle primitives that
        /// have been captured in transform feedback.  If the buffers
        /// provided for transform feedback are too small to capture all
        /// primitives, this counter stops updating once overflow has
        /// occurred.
        TRANSFORM_FEEDBACK_PRIMITIVES_WRITTEN = 0x0000000E,

        /// \brief Report zcull counters.
        ///
        /// Reports four 32-bit words with ZCull counters with no GPU
        /// timestamp:
        ///
        /// \li Word 0: the number of 8x8 tiles processed by ZCull (whether
        /// culled or not)
        /// \li Word 1: the number of 4x2 pixel blocks culled due to
        /// failing the depth test by being behind previous primitives.
        /// \li Word 2: the number of 8x8 pixel blocks culled because they
        /// were in front of previous primitives.
        /// \li Word 3: the number of 4x4 pixel blocks that were culled due
        /// to failing the stencil test.
        ZCULL_STATS = 0x0000000F,

        /// \brief Doesn't report a counter; zero is written in the counter
        /// portion of the report structure.
        ///
        /// The timestamp for this report will be taken at the top of the
        /// GPU pipeline and does not wait for the completion of previous
        /// primitives.
        TIMESTAMP_TOP = 0x00000010,

        NVN_ENUM_32BIT(COUNTER_TYPE),
    };
    NVN_ENUM_CLASS(CounterType, NVNcounterType);
};

/// \brief Specifies the event type in a command buffer memory callback.
struct CommandBufferMemoryEvent {

    /// \brief Enumerated values for the type CommandBufferMemoryEvent.
    enum Enum {

        /// \brief The command buffer has run out of command memory.
        ///
        /// Applications should respond to this event by providing new
        /// memory by calling CommandBuffer::AddCommandMemory.  Failing to
        /// do so will result in program termination.
        OUT_OF_COMMAND_MEMORY = 0x00000000,

        /// \brief The command buffer has run out of control memory.
        ///
        /// Applications should respond to this event by providing new
        /// memory by calling CommandBuffer::AddControlMemory.  Failing to
        /// do so will result in program termination.
        OUT_OF_CONTROL_MEMORY = 0x00000001,

        NVN_ENUM_32BIT(COMMAND_BUFFER_MEMORY_EVENT),
    };
    NVN_ENUM_CLASS(CommandBufferMemoryEvent, NVNcommandBufferMemoryEvent);
};

/// \brief Specifies how query results will affect conditional rendering.
struct ConditionalRenderMode {

    /// \brief Enumerated values for the type ConditionalRenderMode.
    enum Enum {

        /// \brief Rendering will be enabled if and only if the compared
        /// values are equal.
        RENDER_IF_EQUAL = 0,

        /// \brief Rendering will be enabled if and only if the compared
        /// values are not equal.
        RENDER_IF_NOT_EQUAL = 1,

        NVN_ENUM_32BIT(CONDITIONAL_RENDER_MODE),
    };
    NVN_ENUM_CLASS(ConditionalRenderMode, NVNconditionalRenderMode);
};

/// \brief Specifies the location of the pixel with window coordinates (0,
/// 0).
struct WindowOriginMode {

    /// \brief Enumerated values for the type WindowOriginMode.
    enum Enum {

        /// \brief The origin is located at the lower-left corner of the
        /// window, following default OpenGL conventions.
        LOWER_LEFT = 0,

        /// \brief The origin is located at the upper-left corner of the
        /// window, following Direct3D conventions.
        UPPER_LEFT = 1,

        NVN_ENUM_32BIT(WINDOW_ORIGIN_MODE),
    };
    NVN_ENUM_CLASS(WindowOriginMode, NVNwindowOriginMode);
};

/// \brief Specifies the near plane of the clip volume.
struct DepthMode {

    /// \brief Enumerated values for the type DepthMode.
    enum Enum {

        /// \brief The near clip plane is located at -W (i.e., -1 when
        /// depth is normalized as z/w), following default OpenGL
        /// conventions.
        NEAR_IS_MINUS_W = 0,

        /// \brief The near clip plane is located at zero, following
        /// Direct3D conventions.
        NEAR_IS_ZERO = 1,

        NVN_ENUM_32BIT(DEPTH_MODE),
    };
    NVN_ENUM_CLASS(DepthMode, NVNdepthMode);
};

/// \brief Identifies special properties of texture allocations.
///
/// Texture flags are set in TextureBuilder objects and are used when
/// setting up new Texture objects using Texture::Initialize.
struct TextureFlags {

    /// \brief Enumerated values for the type TextureFlags.
    enum Enum {

        /// \brief This texture can be displayed on-screen.
        ///
        /// \note Linear textures cannot be displayed. Setting the DISPLAY
        /// flag together with LINEAR or LINEAR_RENDER_TARGET is not
        /// supported.
        DISPLAY = 0x00000001,

        /// \brief This texture can be used to hold images produced by the
        /// GPU video decode engine.  Textures with the VIDEO_DECODE bit
        /// set may have different memory storage or alignment requirements
        /// than textures without the bit set.
        VIDEO_DECODE = 0x00000002,

        /// \brief This texture can be used for image loads and stores.
        ///
        /// On most devices, this flag has no effect.  However, on the
        /// Windows reference implementation, it affects the reported
        /// alignment requirements of buffer textures on GPUs older than
        /// second-generation Maxwell GPUs.
        IMAGE = 0x00000004,

        /// \brief The contents of this texture can be compressed by the
        /// GPU.
        ///
        /// For textures with this bit set, the NVN driver will set up
        /// special hardware resources and memory mappings allowing the
        /// contents of the texture to be automatically compressed by the
        /// GPU to save memory bandwidth.  This form of compression can
        /// significantly increase performance when using textures as
        /// render targets.
        ///
        /// \note Setting this bit may increase the required alignment and
        /// total memory size of the texture, due to special hardware
        /// memory mappings.
        ///
        /// \note Textures with depth/stencil formats must set the
        /// compressible bit.
        ///
        /// \note This form of texture compression has nothing to do with
        /// textures using compressed block formats, such as
        /// Format::RGB_DXT1.  Applications should not set this bit for
        /// textures with compressed block formats.
        ///
        /// \warning When reusing memory from a compressible pool for
        /// compressible textures, applications must clear compressible
        /// textures before reading or writing with the GPU.  Failing to do
        /// so could result in undefined behavior, including GPU errors or
        /// hangs.
        COMPRESSIBLE = 0x00000008,

        /// \brief This texture is stored in linear format.
        ///
        /// For textures with this bit or
        /// TextureFlags::LINEAR_RENDER_TARGET set, images are stored as a
        /// two-dimensional array of texels.
        /// Each row of a linear texture is stored as an array of texels,
        /// and the difference in bytes between the addresses of adjacent
        /// row is called the _stride_. The stride is programmed by the
        /// application, and is typically computed by multiplying the width
        /// of the texture and the number of bytes for each texel, and then
        /// padding out to match hardware requirements.
        ///
        /// For linear textures, the offset in a memory pool and stride
        /// must both be multiples of
        /// DeviceInfo::LINEAR_TEXTURE_STRIDE_ALIGNMENT.  If the texture is
        /// also used as a render target, additional requirements apply and
        /// the flag DeviceInfo::LINEAR_RENDER_TARGET should also be set.
        ///
        /// Linear textures only support two-dimensional images, with
        /// targets of TextureTarget::TARGET_2D or
        /// TextureTarget::TARGET_RECTANGLE.  Linear textures also do not
        /// support multiple mipmap levels.
        LINEAR = 0x00000010,

        /// \brief This texture is arranged in memory to store as much of
        /// the texture as possible as collections of fixed-size texture
        /// tiles aligned on page boundaries.
        ///
        /// When using textures stored in virtual memory pools,
        /// applications can populate only some pages of the memory pool
        /// and leave the remainder unpopulated.  Textures that are
        /// partially populated in this way are referred to as _sparse_.
        /// Some mipmap levels of the texture may be decomposed into a
        /// collection of fixed-size tiles, where each tile is aligned to a
        /// single page in the virtual memory pool.  The remaining set of
        /// mipmap levels that can not be decomposed into tiles is referred
        /// to as the mipmap _tail_.
        ///
        /// When set, this bit controls the memory layout of the texture to
        /// maximize the number of mipmap levels that can be arranged into
        /// page-aligned tiles.  On NX and second-generation Maxwell GPUs,
        /// the tail will begin at the first mipmap level smaller than the
        /// fixed tile size.  On older GPUs, the tail begins at the first
        /// mipmap level that is not a multiple of the fixed tile size.
        SPARSE = 0x00000020,

        /// \brief This texture is stored in linear format and will be used
        /// as a render target.
        ///
        /// For textures with this bit or TextureFlags::LINEAR set, images
        /// are stored as a two-dimensional array of texels.
        /// Each row of a linear texture is stored as an array of texels,
        /// and the difference in bytes between the addresses of adjacent
        /// row is called the _stride_. The stride is programmed by the
        /// application, and is typically computed by multiplying the width
        /// of the texture and the number of bytes for each texel, and then
        /// padding out to match hardware requirements.
        ///
        /// For linear texture render targets, the offset in a memory pool
        /// and stride must both be multiples of
        /// DeviceInfo::LINEAR_RENDER_TARGET_STRIDE_ALIGNMENT.
        ///
        /// Linear textures only support two-dimensional images, with
        /// targets of TextureTarget::TARGET_2D or
        /// TextureTarget::TARGET_RECTANGLE.  Linear textures also do not
        /// support multiple mipmap levels.  Linear textures may not be
        /// used as depth/stencil render targets.  They also may not be
        /// bound as a render target at the same time as any non-linear
        /// color or depth/stencil texture.
        LINEAR_RENDER_TARGET = 0x00000040,

        /// \brief Enable adaptive ZCull for a given depth texture target.
        ///
        /// When adaptive ZCull is enabled, hardware automatically tunes
        /// the assignment of on-chip ZCull resources to portions of the
        /// depth texture based on rendering results from previous frames.
        /// Adaptive ZCull can potentially improve ZCull precision and
        /// improve cull rate.
        ///
        /// When applications switch between multiple depth textures with
        /// any using adaptive ZCull, CommandBuffer::SaveZCullData and
        /// CommandBuffer::RestoreZCullData should be used to preserve
        /// information on previous rendering operations for tuning ZCull
        /// resource assignment.
        ///
        /// Use of adaptive ZCull is described in more detail in the \ref
        /// nvn_guide_adaptive_zcull "programming guide".
        ADAPTIVE_ZCULL = 0x00000080,

        /// \brief Favor fast clear over depth compression for DEPTH16
        /// depth textures.
        ///
        /// By default, framebuffer compression for DEPTH16 formats is
        /// optimized to minimize bandwidth usage when rendering geometric
        /// primitives.  If this flag is set, an alternate compression
        /// algorithm is used that accelerates depth buffer clears, but may
        /// reduce performance of geometric rendering relative to the
        /// default algorithm.
        ///
        /// \note This flag has no effect on the Windows reference
        /// implementation.
        DEPTH16_PREFER_FAST_CLEAR = 0x00000100,

        /// \brief Use minimal storage layout for texture.
        ///
        /// Prefer smallest tile size for storage layout. Choosing a
        /// smaller tile size reduces cache friendliness but makes texture
        /// storage size smaller due to less padding used.  This flag may
        /// not be used with TextureFlags::LINEAR, TextureFlags::SPARSE,
        /// TextureFlags::VIDEO_DECODE or buffer textures.
        MINIMAL_LAYOUT = 0x00000200,

        /// \brief Enables support for stencil culling.
        ///
        /// By default stencil culling is disabled and all ZCull memory is
        /// used to store depth information. When enabling stencil culling
        /// one bit per pixel/sample is used to store stencil information
        /// in the ZCull memory. Note that enabling stencil culling results
        /// in a reduced precision of the depth culling.
        /// This flag is only supported for depth formats that have stencil
        /// bits.
        ZCULL_SUPPORT_STENCIL = 0x00000400,

        /// \brief Disable Both compression for 4xAA color textures that
        /// have a 32 or 64 color bit depth.
        ///
        /// Color textures that use 4xAA multi-sampling format and that
        /// have a color depth of 32 or 64 bit can be compressed by either
        /// using arithmetic compression, by using a color reduction or by
        /// using a combination of both. By default NVN will use the
        /// combination of both, which is called Both compression. If this
        /// flag is set only the color reduction or the arithmetic
        /// compression can be used to compress the color data.
        ///
        /// \note This flag only has an effect on 4xAA color texture that
        /// have a color depth of 32 or 64 bit.
        /// \note This flag has no effect on the Windows reference
        /// implementation.
        DISABLE_BOTH_COMPRESSION = 0x00001000,

        NVN_ENUM_32BIT(TEXTURE_FLAGS),
    };
    NVN_BITFIELD_CLASS(TextureFlags, NVNtextureFlags);
};

/// \brief Controls the ordering of commands and invalidation of caches for
/// CommandBuffer::Barrier.
///
/// Rendering and compute dispatch commands sent to NVN queues are executed
/// on the GPU using a hardware pipeline, with no guarantee that work for
/// one draw call or dispatch completes before work on the next draw call
/// or dispatch starts.  Additionally, some units of the GPU maintain
/// caches that are not automatically invalidated when cached data are
/// overwritten by the CPU or other units of the GPU.  This bitfield type
/// is used by CommandBuffer::Barrier to order the processing of commands
/// sent before and after the barrier and to invalidate GPU caches before
/// processing commands sent after the barrier.
struct BarrierBits {

    /// \brief Enumerated values for the type BarrierBits.
    enum Enum {

        /// \brief Ensure that primitives and compute dispatches from after
        /// the barrier don't start until all such work from before the
        /// barrier has completed.
        ///
        /// \warning ORDER_PRIMITIVES does not ensure that all work before
        /// the barrier completes before fetching per-draw data for
        /// indirect draws and compute dispatches after the barrier.  If
        /// per-draw data needed by indirect draws or dispatches are
        /// produced by commands executed by the GPU, a barrier with the
        /// ORDER_INDIRECT_DATA bit set is required.
        ORDER_PRIMITIVES = 0x00000001,

        /// \brief Ensure that fragments from after the barrier don't start
        /// until all fragments from before the barrier have finished.
        ///
        /// ORDER_FRAGMENTS ensures that all fragments spawned by commands
        /// sent before the barrier complete execution on the GPU before
        /// processing any fragments spawned by commands sent after the
        /// barrier.  This is useful for cases like render-to-texture,
        /// where one rendering pass writes to a texture as a render
        /// target, and the next pass reads the values written in a
        /// fragment shader.
        ///
        /// \note ORDER_FRAGMENTS _only_ orders fragment shader execution
        /// and subsequent per-fragment operations (e.g., depth and stencil
        /// tests, blending, and stores to the framebuffer).  Vertex
        /// processing for draw calls sent after the barrier may begin
        /// before fragments from before the barrier complete; to ensure
        /// proper order for dependent vertex processing work,
        /// ORDER_PRIMITIVES should be used.
        ///
        /// \warning There are a few exceptions to the ordering guarantee
        /// for ORDER_FRAGMENTS. The GPU may begin processing depth/stencil
        /// clears or depth/stencil testing (including buffer updates) for
        /// fragments after the barrier before completing all processing of
        /// fragments before the barrier. Additionally, on the Windows
        /// reference implementation for Pascal and newer GPUs, the GPU may
        /// begin processing color clears for fragments after the barrier
        /// before completing all processing of fragments before the
        /// barrier. When fragments from after the barrier perform such
        /// updates to a texture read by fragments from before the barrier,
        /// a heavier barrier like ORDER_PRIMITIVES is necessary to ensure
        /// the "new" fragments don't overwrite values read by "old" ones.
        ORDER_FRAGMENTS = 0x00000002,

        /// \brief Ensure that fragments from after the barrier don't start
        /// until all fragments from before the barrier in the same
        /// screen-space tile have finished.
        ///
        /// ORDER_FRAGMENTS_TILED behaves like ORDER_FRAGMENTS, except that
        /// it only guarantees the ordering of fragments within
        /// screen-space tiles.  If two fragments belong to different
        /// screen-space tiles, work for one fragment from after the
        /// barrier may begin before work for the other fragment from
        /// before the barrier completes.  This mode is useful for
        /// render-to-texture operations where the second texturing pass
        /// accesses texels written during the first pass with the same
        /// (x,y) coordinates.
        ORDER_FRAGMENTS_TILED = 0x00000004,

        /// \brief Ensure that all work before the barrier completes before
        /// fetching per-draw data for indirect draws and compute
        /// dispatches after the barrier.
        ///
        /// When executing indirect draws or compute dispatches, the GPU
        /// might fetch per-draw data before previous GPU work has
        /// completed, even in cases where the application waits on a sync
        /// object using Queue::WaitSync.  A barrier with this bit set is
        /// required to ensure that the GPU doesn't prematurely fetch the
        /// indirect data.  Note that this bit is not required when the
        /// indirect data are written by the CPU.
        ///
        /// \warning ORDER_INDIRECT_DATA requires more expensive
        /// synchronization than ORDER_PRIMITIVES and should be used only
        /// when required.
        ORDER_INDIRECT_DATA = 0x00000008,

        /// \brief Invalidate caches used to hold texture data fetched via
        /// sampler or image variables.
        ///
        /// When texture memory is written to by either the CPU or by GPU
        /// commands other than copy commands, applications must invalidate
        /// the texture cache.  If the texture memory is written by
        /// non-copy GPU commands, applications must also ensure that the
        /// GPU commands producing the new data have completed, either by
        /// setting an ORDER bit in the barrier or by waiting on a Sync
        /// object.
        INVALIDATE_TEXTURE = 0x00000010,

        /// \brief Invalidate caches used to hold shader code and data from
        /// uniform or shader storage blocks.
        ///
        /// When shader code, uniform blocks, or shader storage blocks are
        /// written to by either the CPU or the GPU commands other than
        /// copy commands, applications must invalidate the shader caches.
        /// If the memory is written by non-copy GPU commands, applications
        /// must also ensure that the GPU commands producing the new data
        /// have completed, either by setting an ORDER bit in the barrier
        /// or by waiting on a Sync object.
        ///
        /// In particular, before executing new shader code used by
        /// Program::SetShaders, the shader caches must be invalidated to
        /// ensure it they are not holding old instructions using the same
        /// memory as the new shader code.
        INVALIDATE_SHADER = 0x00000020,

        /// \brief Invalidate caches used to hold texture and sampler
        /// descriptors.
        ///
        /// When new texture, image, or sampler descriptors are registered
        /// in the texture or sampler pools using
        /// TexturePool::RegisterTexture, TexturePool::RegisterImage,
        /// SamplerPool::RegisterSampler, or
        /// SamplerPool::RegisterSamplerBuilder, the texture descriptor
        /// cache must be invalidated to avoid the possibility of fetching
        /// stale texture or sampler pool entries.
        ///
        /// note The texture and sampler descriptor caches are
        /// automatically invalidated when command buffers are flushed, as
        /// well as when switching to a new texture or sampler pool.
        INVALIDATE_TEXTURE_DESCRIPTOR = 0x00000040,

        /// \brief Invalidate GPU ZCull storage after direct depth texture
        /// updates.
        ///
        /// For depth textures, the GPU maintains internal _ZCull_ storage
        /// that is used to accelerate depth and stencil testing.  The
        /// ZCull hardware tracks updates to the depth buffer when
        /// processing clears and depth buffer updates from the depth test,
        /// and maintains a low-resolution representation of the depth
        /// buffer.  ZCull data can often be used to quickly determine if
        /// fragments will pass or fail the depth test.  When a depth
        /// texture is updated via copy commands such as
        /// CommandBuffer::CopyTextureToTexture, the ZCull storage will be
        /// out of date and require invalidation.  When a depth buffer is
        /// updated only by clears or normal rendering, barriers with
        /// INVALIDATE_ZCULL will unnecessarily reduce performance.
        ///
        /// \note The ZCull storage is automatically invalidated when
        /// switching to a new depth render target using
        /// CommandBuffer::SetRenderTargets.
        INVALIDATE_ZCULL = 0x00000080,

        /// \brief Invalidates the L2 cache.
        ///
        /// Invalidating the L2 cache may be necessary when writing
        /// different RAW formats to the same physical memory location.For
        /// more details, see the Memory Pool Aliasing section of the NVN
        /// Programming Guide.
        ///
        /// \warning in addition to invalidating the cache, this flag
        /// performs a wait-for-idle. This may negatively impact
        /// performance.
        INVALIDATE_L2_CACHE = 0x00000100,

        NVN_ENUM_32BIT(BARRIER_BITS),
    };
    NVN_BITFIELD_CLASS(BarrierBits, NVNbarrierBits);
};

/// \brief Flags controlling the operation of copy commands.
struct CopyFlags {

    /// \brief Enumerated values for the type CopyFlags.
    enum Enum {

        /// \brief No special operations should be performed on the copy.
        NONE = 0x00000000,

        /// \brief Enable linear filtering for texture-to-texture copies.
        ///
        /// Linear filtering is only supported for
        /// CommandBuffer::CopyTextureToTexture.  This flag should not be
        /// set for any other copy command.
        LINEAR_FILTER = 0x00000001,

        /// \brief Enable use of the 2D engine copy path.
        ///
        /// ENGINE_2D is only supported for
        /// CommandBuffer::CopyBufferToTexture. This flag should not be set
        /// for any other copy command.
        ///
        /// This flag directs CommandBuffer::CopyBufferToTexture to use the
        /// same hardware engine that CommandBuffer::CopyTextureToTexture
        /// uses, which is capable of transparently compressing
        /// compressible textures. See TextureFlags::COMPRESSIBLE. This
        /// flag can be used with all of the color formats that
        /// CommandBuffer::CopyTextureToTexture supports, except for
        /// block-based compressed texture formats. Only compressible
        /// formats will have their data compressed.
        ENGINE_2D = 0x00000002,

        /// \brief Mirrors data in the X direction while copying.
        ///
        /// MIRROR_X is only supported by
        /// CommandBuffer::CopyTextureToTexture, or by
        /// CommandBuffer::CopyBufferToTexture when using ENGINE_2D. This
        /// flag should not be set for any other copy command.
        MIRROR_X = 0x00000004,

        /// \brief Mirrors data in the Y direction while copying.
        MIRROR_Y = 0x00000008,

        /// \brief Mirrors data in the Z direction while copying.
        MIRROR_Z = 0x00000010,

        NVN_ENUM_32BIT(COPY_FLAGS),
    };
    NVN_BITFIELD_CLASS(CopyFlags, NVNcopyFlags);
};

/// \brief Specifies an input value to use for one component of the
/// position vector produced by the viewport swizzle operation.
///
/// As part of the viewport transformation, each vertex is transformed by
/// the viewport swizzle operation before the vertex is transformed to
/// screen space.  The viewport swizzle produces a four-component swizzled
/// position, where each component is produced by selecting and optionally
/// negating one of the four compenents of the original vertex position.
/// This type specifies the value selected for one of the four output
/// components.
///
/// \note There is a separate set of four viewport swizzle values for each
/// viewport.  A primitive broadcast to multiple viewports will have a
/// separate swizzle applied for each viewport.
struct ViewportSwizzle {

    /// \brief Enumerated values for the type ViewportSwizzle.
    enum Enum {

        /// \brief Select the X component of the input position vector.
        POSITIVE_X = 0,

        /// \brief Select and negate the X component of the input position
        /// vector.
        NEGATIVE_X = 1,

        /// \brief Select the Y component of the input position vector.
        POSITIVE_Y = 2,

        /// \brief Select and negate the Y component of the input position
        /// vector.
        NEGATIVE_Y = 3,

        /// \brief Select the Z component of the input position vector.
        POSITIVE_Z = 4,

        /// \brief Select and negate the Z component of the input position
        /// vector.
        NEGATIVE_Z = 5,

        /// \brief Select the W component of the input position vector.
        POSITIVE_W = 6,

        /// \brief Select and negate the W component of the input position
        /// vector.
        NEGATIVE_W = 7,

        NVN_ENUM_32BIT(VIEWPORT_SWIZZLE),
    };
    NVN_ENUM_CLASS(ViewportSwizzle, NVNviewportSwizzle);
};

/// \brief Specifies how coverage affects final color values when
/// target-independent rasterization is enabled.
///
/// When target-independent rasterization is enabled, pixel or sample color
/// values in the framebuffer are updated whenever any of the associated
/// raster samples are covered by a primitive.  Applications can enable
/// coverage modulation to account for partial coverage, where color (RGB)
/// or alpha components are multiplied by a factor that reflects the
/// fraction of raster samples that are covered.  This modulation mode
/// allows applications to specify that some, all, or none of the color
/// components are multiplied by this factor.
struct CoverageModulationMode {

    /// \brief Enumerated values for the type CoverageModulationMode.
    enum Enum {

        /// \brief Do not modify color (RGB) or alpha components (default).
        NONE = 0x00000000,

        /// \brief Multiply color (RGB) components by the modulation
        /// factor, but do not modify alpha.
        RGB = 0x00000001,

        /// \brief Multiply alpha components by the modulation factor, but
        /// do not modify color (RGB).
        ALPHA = 0x00000002,

        /// \brief Multiply color (RGB) and alpha components by the
        /// modulation factor.
        RGBA = 0x00000003,

        NVN_ENUM_32BIT(COVERAGE_MODULATION_MODE),
    };
    NVN_ENUM_CLASS(CoverageModulationMode, NVNcoverageModulationMode);
};

/// \brief Flags controlling the behavior of Queue objects.
struct QueueFlags {

    /// \brief Enumerated values for the type QueueFlags.
    enum Enum {

        /// \brief Disable support for fragment shader interlocks.
        ///
        /// If set, this flag indicates that the application will not be
        /// using fragment shader interlocks (e.g., the
        /// NV_fragment_shader_interlock GLSL extension) and that no
        /// per-queue memory needs to be allocated to support the feature.
        ///
        /// \note If this bit is set, it is illegal to bind a Program
        /// object using the fragment shader interlock feature.
        NO_FRAGMENT_INTERLOCK = 0x00000001,

        /// \brief Disable ZCull.
        ///
        /// Setting this flag reduces the overhead of switching between NVN
        /// queues, but significantly reduces the performance of rendering
        /// with depth testing enabled. Due to the rendering performance
        /// cost, we recommend only setting this flag for queues that
        /// perform relatively little rendering work or do not require
        /// depth testing.
        ///
        /// \note If this bit is set, ZCull related functionality like
        /// nvn::CommandBuffer::SaveZCullData() and
        /// nvn::CommandBuffer::LoadZCullData() will be treated as no-ops
        /// by the hardware.
        NO_ZCULL = 0x00000002,

        /// \brief No special operation behavior is required by the queue.
        NONE = 0x00000000,

        NVN_ENUM_32BIT(QUEUE_FLAGS),
    };
    NVN_BITFIELD_CLASS(QueueFlags, NVNqueueFlags);
};

/// \brief Debug layer object type.
struct DebugObjectType {

    /// \brief Enumerated values for the type DebugObjectType.
    enum Enum {

        /// \brief Window object type.
        WINDOW = 0,

        /// \brief Queue object type.
        QUEUE = 1,

        /// \brief CommandBuffer object type.
        COMMAND_BUFFER = 2,

        /// \brief MemoryPool object type.
        MEMORY_POOL = 3,

        /// \brief Sync object type.
        SYNC = 7,

        /// \brief Program object type.
        PROGRAM = 9,

        /// \brief TexturePool object type.
        TEXTURE_POOL = 10,

        /// \brief SamplerPool object type.
        SAMPLER_POOL = 11,

        NVN_ENUM_32BIT(DEBUG_OBJECT_TYPE),
    };
    NVN_ENUM_CLASS(DebugObjectType, NVNdebugObjectType);
};

/// \brief Method used by CommandBuffer::SignalEvent to update event
/// memory. When CommandBuffer::SignalEvent is called, this mode specifies
/// how memory associated with the Event object is updated.
struct EventSignalMode {

    /// \brief Enumerated values for the type EventSignalMode.
    enum Enum {

        /// \brief Store the value provided by the SignalEvent command to
        /// event memory.
        ///
        /// This mode is supported on all platforms.
        WRITE = 0,

        /// \brief Atomically add the value provided by the SignalEvent
        /// command to event memory.
        ///
        /// This mode is not supported on all platforms. Support is
        /// determined by querying the EVENTS_SUPPORT_REDUCTION_OPERATIONS
        /// device property.
        ADD = 1,

        NVN_ENUM_32BIT(EVENT_SIGNAL_MODE),
    };
    NVN_ENUM_CLASS(EventSignalMode, NVNeventSignalMode);
};

/// \brief Method used by CommandBuffer::WaitEvent to wait for the
/// completion of a previous signal. When CommandBuffer::WaitEvent is
/// executed by the GPU, the value in the memory associated with the Event
/// object is compared to a provided value. This mode specifies whether
/// this comparison is considered to pass or fail.
struct EventWaitMode {

    /// \brief Enumerated values for the type EventWaitMode.
    enum Enum {

        /// \brief The test passes if the value in event memory is exactly
        /// equal to the value provided to WaitEvent.
        EQUAL = 0,

        /// \brief The test passes if the value in event memory is greater
        /// than or equal to the value provided to WaitEvent, where the
        /// comparison uses a wrapped comparison:
        ///
        ///     uint32_t(*memory_value - wait_value) < 0x80000000U;
        ///
        /// This comparison allows for an incrementing event value that
        /// requires no special-case wrapping logic.  When a counter wraps,
        /// a wrapped value like 0x00000001 will be considered greater than
        /// a pre-wrapped value like 0xFFFFFFFE.
        GEQUAL_WRAP = 1,

        NVN_ENUM_32BIT(EVENT_WAIT_MODE),
    };
    NVN_ENUM_CLASS(EventWaitMode, NVNeventWaitMode);
};

/// \brief Mode determining when CommandBuffer::SignalEvent signals the
/// event.
struct EventSignalLocation {

    /// \brief Enumerated values for the type EventSignalLocation.
    enum Enum {

        /// \brief Signal the event when the command is processed by the
        /// top of the GPU pipeline.
        TOP = 0,

        /// \brief Signal the event when vertex processing pipeline has
        /// completed work on all previously submitted draw calls. Does not
        /// wait for rasterization and fragment processing.
        VERTEX_PIPE = 1,

        /// \brief Signal the event when work for all previously submitted
        /// GPU commands has completed.
        BOTTOM = 2,

        NVN_ENUM_32BIT(EVENT_SIGNAL_LOCATION),
    };
    NVN_ENUM_CLASS(EventSignalLocation, NVNeventSignalLocation);
};

/// \brief Return values for GL/NVN sync conversion interop functions.
struct SyncInteropStatus {

    /// \brief Enumerated values for the type SyncInteropStatus.
    enum Enum {

        /// \brief The call was sucessful.
        OK = 0,

        /// \brief The sync was not ready to be converted yet. Please make
        /// sure it's flushed and try again.
        AGAIN = 1,

        /// \brief The sync was already signaled.
        ALREADY = 2,

        /// \brief The GLsync was not found. Was it flushed?
        NOT_FOUND = 3,

        /// \brief On the Windows reference platform, OpenGL cannot use
        /// sync interop with both OpenCL and NVN within the same
        /// application.
        INVALID = 4,

        NVN_ENUM_32BIT(SYNC_INTEROP_STATUS),
    };
    NVN_ENUM_CLASS(SyncInteropStatus, NVNsyncInteropStatus);
};

/// \brief Flags controlling the behavior of CommandBuffer::SignalEvent.
struct EventSignalFlags {

    /// \brief Enumerated values for the type EventSignalFlags.
    enum Enum {

        /// \brief Do not wait for completion of memory transactions from
        /// commands prior to the SignalEvent command.
        ///
        /// By default, an event is not signaled until all GPU memory
        /// transactions from prior commands have completed. When this bit
        /// is set, the event may be signaled without waiting for all
        /// memory transactions.
        NO_WAIT_MEMORY = 1,

        NVN_ENUM_32BIT(EVENT_SIGNAL_FLAGS),
    };
    NVN_BITFIELD_CLASS(EventSignalFlags, NVNeventSignalFlags);
};

/// \brief Class of a color format.
///
/// This enum is used only for color reduction to describe a set of formats
/// that are supported for color reduction. The programming guide has a
/// table of formats that enumerates the mapping from color format to
/// format class.
struct FormatClass {

    /// \brief Enumerated values for the type FormatClass.
    enum Enum {

        /// \brief UNORM8 class, for instance R8, RGBA8.
        UNORM8 = 0,

        /// \brief UNORM10 class, for instance R10B10G10A2.
        UNORM10 = 1,

        /// \brief UNORM16 class, for instance RGB16.
        UNORM16 = 2,

        /// \brief FP11 class, for instance R11G11B10F.
        FP11 = 3,

        /// \brief FP16 class, for instance RGBA16F.
        FP16 = 4,

        /// \brief SRGB8 class, for instance BGRA8_SRGB.
        SRGB8 = 5,

        NVN_ENUM_32BIT(FORMAT_CLASS),
    };
    NVN_ENUM_CLASS(FormatClass, NVNformatClass);
};

/// \brief Priority of a Queue object.
///
/// Priority govern the order in which Queues are run on the GPU. See
/// nvn::QueueBuilder::SetQueuePriority for more information.
struct QueuePriority {

    /// \brief Enumerated values for the type QueuePriority.
    enum Enum {

        /// \brief This gives a Queue the lowest Priority possible.
        LOW = 10,

        /// \brief The default setting.
        NORMAL = 100,

        /// \brief This gives a Queue the highest Priority possible.
        HIGH = 1000,

        NVN_ENUM_32BIT(QUEUE_PRIORITY),
    };
    NVN_ENUM_CLASS(QueuePriority, NVNqueuePriority);
};

/// \brief Raw storage class used when accessing buffers and textures in
/// main memory on NX.
///
/// On the NX device, each storage class (StorageClass) also has an
/// associated raw storage class used to perform format-specific
/// optimizations when the GPU's L2 cache reads data from or writes data to
/// main memory.  When a physical memory page in a MemoryPool object is
/// first used with one raw storage class and then used with a different
/// raw storage class, applications must ensure that the GPU's L2 cache is
/// invalidated before it executes commands using the new raw storage
/// class.  This is done automatically if the Queue used to submit commands
/// is flushed between commands using the new and old raw storage classes.
/// The L2 cache can be invalidated manually using CommandBuffer::Barrier
/// with the Barrier::INVALIDATE_L2_CACHE bit set.
///
/// \note The raw storage class has no effect on the Windows reference
/// implementation.
struct RawStorageClass {

    /// \brief Enumerated values for the type RawStorageClass.
    enum Enum {

        /// \brief No raw storage class.
        NONE = 0,

        /// \brief Raw storage class used for linear and buffer textures
        /// and Buffer objects.
        LINEAR = 1,

        /// \brief Raw storage class used for textures (other than linear
        /// and buffer textures) using any color texture format or
        /// Format::DEPTH32F.
        COLOR_AND_ZF32 = 2,

        /// \brief Raw storage class used for textures (other than linear
        /// and buffer textures) using Format::STENCIL8 or Format::DEPTH16.
        S8_AND_Z16 = 3,

        /// \brief Raw storage class used for textures (other than linear
        /// and buffer textures) using Format::DEPTH24 or
        /// Format::DEPTH24_STENCIL8.
        Z24_AND_Z24S8 = 4,

        /// \brief Raw storage class used for textures (other than linear
        /// and buffer textures) using Format::DEPTH32F_STENCIL8.
        ZF32S8 = 5,

        NVN_ENUM_32BIT(RAW_STORAGE_CLASS),
    };
    NVN_ENUM_CLASS(RawStorageClass, NVNrawStorageClass);
};

/// \brief Sample shading mode used when executing a fragment shader.
struct SampleShadingMode {

    /// \brief Enumerated values for the type SampleShadingMode.
    enum Enum {

        /// \brief Default mode. The fragment shader is evaluated either by
        /// fragment or by sample, depending on whether a keyword such as
        /// gl_SamplePosition is specified within the fragment shader.
        DEFAULT_FRAGMENT = 0,

        /// \brief Force the fragment shader to be evaluated per sample.
        FORCE_SAMPLE_SHADING_ON = 1,

        /// \brief Force the fragment shader to be evaluated per fragment,
        /// overriding any behavior from any keywords in the fragment
        /// shader.
        FORCE_SAMPLE_SHADING_OFF = 2,

        NVN_ENUM_32BIT(SAMPLE_SHADING_MODE),
    };
    NVN_ENUM_CLASS(SampleShadingMode, NVNsampleShadingMode);
};

/// \}
/// \addtogroup nvn_cpp_struct
/// \{

/// \brief Data structure representing the shader data.
///
/// The data structure is for one shader stage.  The contents of this
/// struct contain everything NVN needs to set up the program for use with
/// Program::SetShaders
struct ShaderData {

    /// \brief Address within a memory pool where the program resides.
    BufferAddress   data;

    /// \brief Control section from the offline compiler. This address must
    /// be aligned to 8 bytes.
    const void*     control;

};

/// \brief Data structure containing hashes to replace the hashes built
/// into the shader control sections.
///
/// The data structure is for one shader stage and is meant to be used with
/// NVNshaderDataExt. The values in this structure will replace the values
/// built into the stage's control section. This data structure is binary
/// identical to the GLSLC data structure GLSLCdebugDataHash and can be
/// used in place of references to that structure.
struct DebugDataHash {

    /// \brief The build ID from the GLSLCdebugDataHash section for this
    /// shader stage.
    uint32_t    buildId[4];

    /// \brief The low 32 bits of the hash value from the
    /// GLSLCdebugDataHash section for this shader stage.
    uint32_t    debugHashLo32;

    /// \brief The high 32 bits of the hash value from the
    /// GLSLCdebugDataHash section for this shader stage
    uint32_t    debugHashHi32;

};

/// \brief Data structure representing the extended shader data with
/// optional replacement debug hashes.
///
/// The data structure is for one shader stage.  The contents of this
/// struct contain everything NVN needs to set up the program for use with
/// Program::SetShaders. This is an extended version of NVNshaderData which
/// includes optional debug hashes that the driver uses to replace the
/// debug hashes in the control binary.
struct ShaderDataExt {

    /// \brief Address within a memory pool where the program resides.
    BufferAddress           data;

    /// \brief Control section from the offline compiler. This address must
    /// be aligned to 8 bytes.
    const void*             control;

    /// \brief Debug data hash override for the debug data hash embedded in
    /// the control section. The definition of this data comes from
    /// NvnGlslc headers. If NULL, Program::SetShadersExt will behave the
    /// same as Program::SetShaders.
    const DebugDataHash*    debugDataHash;

};

/// \}
/// \addtogroup nvn_cpp_enum
/// \{

/// \brief Error status returned by _GetError_
struct QueueGetErrorResult {

    /// \brief Enumerated values for the type QueueGetErrorResult.
    enum Enum {

        /// \brief No GPU error was detected for queue.
        GPU_NO_ERROR = 0x00000000,

        /// \brief Unrecognized GPU error.
        GPU_ERROR_UNKNOWN = 0x00000001,

        /// \brief GPU encountered MMU fault.
        GPU_ERROR_MMU_FAULT = 0x00000002,

        /// \brief GPU encountered pushbuffer DMA error.
        GPU_ERROR_PBDMA_EXCEPTION = 0x00000003,

        /// \brief GPU encountered graphics engine exception.
        GPU_ERROR_ENGINE_EXCEPTION = 0x00000004,

        /// \brief GPU encountered timeout.
        GPU_ERROR_TIMEOUT = 0x00000005,

        NVN_ENUM_32BIT(QUEUE_GET_ERROR_RESULT),
    };
    NVN_ENUM_CLASS(QueueGetErrorResult, NVNqueueGetErrorResult);
};

/// \brief Read or write access.
struct MemoryAccess {

    /// \brief Enumerated values for the type MemoryAccess.
    enum Enum {

        /// \brief Read access.
        MEMORY_ACCESS_READ = 0x00000000,

        /// \brief Write access.
        MEMORY_ACCESS_WRITE = 0x00000001,

        NVN_ENUM_32BIT(MEMORY_ACCESS),
    };
    NVN_ENUM_CLASS(MemoryAccess, NVNmemoryAccess);
};

/// \}
/// \addtogroup nvn_cpp_struct
/// \{

/// \brief Data structure representing GPU error info.
union QueueErrorInfo {

    struct {
        /// \brief Fault address.
        uint64_t        faultAddress;

        /// \brief Specifies if fault was caused by read or write memory
        /// access.
        MemoryAccess::Enum accessType;

    } mmuFault;

    /// \brief Reserved for future use.
    int     reserved[16];

};

/// \brief Data structure representing values used by indirect DrawArrays
/// commands.
///
/// This data structure is not used by any API command, but instead
/// represents the layout of GPU-accessible memory pointed to by
/// CommandBuffer::DrawArraysIndirect.
struct DrawArraysIndirectData {

    /// \brief Number of vertices in each instance (copy) of the primitives
    /// to be drawn.
    int     count;

    /// \brief Number of instances (copies) of the primitives to be drawn.
    int     instanceCount;

    /// \brief Index number of the first vertex in each drawn instance.
    int     first;

    /// \brief Instance number of the first instance drawn.
    int     baseInstance;

};

/// \brief Data structure representing values used by indirect DrawElements
/// commands.
///
/// This data structure is not used by any API command, but instead
/// represents the layout of GPU-accessible memory pointed to by
/// CommandBuffer::DrawElementsIndirect.
struct DrawElementsIndirectData {

    /// \brief Number of vertices in each instance (copy) of the primitives
    /// to be drawn.
    int     count;

    /// \brief Number of instances (copies) of the primitives to be drawn.
    int     instanceCount;

    /// \brief Offset (in units of indices) in the index buffer of the
    /// first vertex in each drawn instance.
    int     firstIndex;

    /// \brief Base vertex number to be added to each vertex index fetched
    /// from the index buffer.
    int     baseVertex;

    /// \brief Instance number of the first instance drawn.
    int     baseInstance;

};

/// \brief Data structure representing values used by indirect
/// DispatchCompute commands.
///
/// This data structure is not used by any API command, but instead
/// represents the layout of GPU-accessible memory pointed to by
/// CommandBuffer::DispatchComputeIndirect.
struct DispatchComputeIndirectData {

    /// \brief The number of compute work groups in the X dimension.
    int     groupsX;

    /// \brief The number of compute work groups in the Y dimension.
    int     groupsY;

    /// \brief The number of compute work groups in the Z dimension.
    int     groupsZ;

};

/// \brief Data structure describing how counter values are written to
/// buffer memory.
///
/// This structure can be used to interpret values written in reports from
/// CommandBuffer::ReportCounter for all counters types except for
/// CounterType::ZCULL_STATS.
struct CounterData {

    /// \brief Value of requested counter.
    uint64_t    counter;

    /// \brief GPU timestamp indicating when the counter was written.
    ///
    /// Please refer to the \ref nvn_guide_counter_resolution_of_timestamp
    /// "NVN Programming Guide" for details.
    uint64_t    timestamp;

};

/// \brief Data structure describing device error condition.
struct DeviceErrorData {

    /// \brief Type of the error.
    DeviceError::Enum   error;

    /// \brief Queue object that generated the error.
    NVN_OBJECT(Queue)*  queue;

    /// \brief Reserved for future use.
    int                 reserved[16];

};

/// \brief Data structure describing a rectangle in integer coordinates.
struct Rectangle {

    /// \brief X coordinate.
    int     x;

    /// \brief Y coordinate.
    int     y;

    /// \brief Width of rectangle.
    int     width;

    /// \brief Height of rectangle.
    int     height;

};

/// \brief Data structure describing a virtual memory pool mapping request.
///
/// MemoryPool::MapVirtual can be used to map memory from a physical memory
/// pool (one using MemoryPoolFlags::PHYSICAL) into a virtual memory pool
/// (one using MemoryPoolFlags::VIRTUAL).  Each individual mapping request
/// will map a contiguous set of pages in the physical pool into a
/// contiguous range of the virtual pool, using a specified storage class
/// for the virtual memory mapping.
struct MappingRequest {

    /// \brief Memory pool providing the physical memory for the virtual
    /// memory mapping.
    ///
    /// This parameter must be NULL or a pointer to a physical memory pool
    /// (initialized with MemoryPoolFlags::PHYSICAL).  If NULL, the range
    /// of the virtual memory pool will be unmapped, where GPU accesses
    /// will return zero values (NX and second-generation Maxwell GPUs) or
    /// undefined values.
    NVN_OBJECT(MemoryPool)*     physicalPool;

    /// \brief Offset (in bytes) of the first mapped page from the physical
    /// pool.
    ///
    /// Must be zero if _physicalPool_ is NULL.
    /// Must be a multiple of the value of
    /// nvn::DeviceInfo::MEMORY_POOL_PAGE_SIZE.
    int64_t                     physicalOffset;

    /// \brief Offset (in bytes) of the first mapped page in the virtual
    /// pool.
    /// Must be a multiple of the value of
    /// nvn::DeviceInfo::MEMORY_POOL_PAGE_SIZE.
    int64_t                     virtualOffset;

    /// \brief Size (in bytes) of the requested virtual memory mapping.
    /// Must be a multiple of the value of
    /// nvn::DeviceInfo::MEMORY_POOL_PAGE_SIZE.
    uint64_t                    size;

    /// \brief Storage class used for the virtual memory mapping.
    ///
    /// When memory from a physical memory pool is mapped into a virtual
    /// memory pool, the mapping must be established with a storage class
    /// appropriate for the texture or buffer resources using the virtual
    /// memory.  For textures, the required storage class can be determined
    /// by calling nvn::TextureBuilder::GetStorageClass or
    /// nvn::Texture::GetStorageClass.  For buffers,
    /// NVN_STORAGE_CLASS_BUFFER should be used.
    NVNstorageClass             storageClass;

};

/// \brief Data structure describing a region of a texture used in copy
/// commands.
///
/// \note Copy commands accessing a texture accept one or more pointers to
/// copy region structures.  NULL pointers are not supported.
struct CopyRegion {

    /// \brief X offset (in texels) of first texel copied.
    int     xoffset;

    /// \brief Y offset (in texels) of first texel copied.
    int     yoffset;

    /// \brief Z offset (in texels) of first texel copied.
    int     zoffset;

    /// \brief Width (in texels) of the region being copied.
    int     width;

    /// \brief Height (in texels) of the region being copied; should be one
    /// for one-dimensional images.
    int     height;

    /// \brief Depth (in texels) of the region being copied; should be one
    /// for one- and two-dimensional images.
    int     depth;

};

/// \brief Data structure used by queries of the memory layout used by
/// sparse textures.
///
/// This structure is used to return properties of the layout of a sparse
/// texture in memory.  In a sparse texture, some mipmap levels can be
/// decomposed into a collection of fixed-size tiles, where each tile is
/// aligned on a virtual memory pool page boundary.  The set of smaller
/// mipmap levels that are not split into tiles is referred to as the
/// mipmap _tail_.
struct TextureSparseTileLayout {

    /// \brief The number of mipmap levels that can be split into
    /// fixed-size tiles aligned on memory page boundaries.
    ///
    /// This value will always be less than or equal to the number of
    /// mipmap levels in the texture.  Zero is returned if the texture does
    /// not have TextureFlags::SPARSE or if none of the mipmap levels
    /// support fully aligned tiles.
    int     numTiledLevels;

    /// \brief Width (in pixels) of the fixed-size tile used for sparse
    /// storage of a texture.
    ///
    /// If no portion of the texture can be split into tiles
    /// (_numTiledLevels_ is zero), zero will be returned.
    int     tileWidth;

    /// \brief Height (in pixels) of the fixed-size tile used for sparse
    /// storage of a texture.
    ///
    /// If no portion of the texture can be split into tiles
    /// (_numTiledLevels_ is zero), zero will be returned.
    int     tileHeight;

    /// \brief Depth (in pixels) of the fixed-size tile used for sparse
    /// storage of a texture.
    ///
    /// If no portion of the texture can be split into tiles
    /// (_numTiledLevels_ is zero), zero will be returned.  If a texture
    /// that can be split into tiles is not three-dimensional, the tile
    /// depth will always be one.
    int     tileDepth;

    /// \brief Reserved for future use.
    int     reserved[4];

};

/// \brief Data structure describing the organization of packaged texture
/// data in memory.
///
/// This structure specifies the memory layout of packaged texture data
/// produced by the NVN texture packager.  By default, the driver and
/// texture packager select an optimal layout for a texture based on its
/// size and format.  This structure allows applications to provide the
/// driver with a layout descriptor produced by the NVN texture packager so
/// that old packaged texture assets can continue to be used even if the
/// default layout changes in a new driver.
struct PackagedTextureLayout {

    /// \brief Data produced by the NVN texture packager indicating the
    /// layout of the packaged texture data in memory.
    char    layout[8];

};

/// \brief Data structure specifying a range of buffer memory.
///
/// This structure specifies a range of buffer memory. It is used in the
/// API calls that bind multiple buffer ranges in a single call.
struct BufferRange {

    /// \brief Memory address of the beginning of the buffer range.
    BufferAddress   address;

    /// \brief Size of the buffer range in bytes.
    uint64_t        size;

};

/// \brief Data structure describing a region used by the DrawTexture
/// command.
struct DrawTextureRegion {

    /// \brief X coordinate of the first corner of an axis aligned
    /// rectangle in a window or texture.
    float   x0;

    /// \brief Y coordinate of the first corner of an axis aligned
    /// rectangle in a window or texture.
    float   y0;

    /// \brief X coordinate of the second corner of an axis aligned
    /// rectangle in a window or texture.
    float   x1;

    /// \brief Y coordinate of the second corner of an axis aligned
    /// rectangle in a window or texture.
    float   y1;

};

/// \}
/// \addtogroup nvn_cpp_defines
/// \{

/// \brief Boolean condition with a value of false.
#define NVN_FALSE                                                   0x0000

/// \brief Boolean condition with a value of true.
#define NVN_TRUE                                                    0x0001

/// \brief When waiting on Sync objects, do not wait if the sync object has
/// not yet been signaled.
#define NVN_WAIT_TIMEOUT_NONE                                       0x0ULL

/// \brief When waiting on Sync objects, wait indefinitely until the sync
/// object is signaled.
#define NVN_WAIT_TIMEOUT_MAXIMUM                                    0xFFFFFFFFFFFFFFFFULL

/// \brief Required base address alignment for app-specified memory used to
/// back memory pools.
#define NVN_MEMORY_POOL_STORAGE_ALIGNMENT                           0x1000

/// \brief Required size alignment for app-specified memory used to back
/// memory pools.
#define NVN_MEMORY_POOL_STORAGE_GRANULARITY                         0x1000

/// \brief Fixed storage class used for memory mappings of buffers, buffer
/// textures, and linear textures.
#define NVN_STORAGE_CLASS_BUFFER                                    0x100

/// \}

#ifdef NVN_OVERLOAD_CPP_OBJECTS
namespace objects { // nvn::objects
#endif

/// \addtogroup nvn_cpp_funcptrs
/// \{

/// \brief Generic function pointer type returned when querying API entry
/// points.
typedef void (NVNAPIENTRYP GenericFuncPtrFunc)();

/// \brief Function pointer type used to retreive main API entry points
/// such as nvnDeviceInitialize and nvnDeviceGetProcAddress.
///
/// \param name Name of the C entry point to query.
typedef GenericFuncPtrFunc (NVNAPIENTRYP BootstrapLoaderFunc)(const char * name);

/// \brief Function pointer type used to specify a function to call for
/// debug callbacks.
///
/// \param source Value passed to the callback to indicate the source of
/// the callback.
///
/// \param type Value passed to the callback to indicate the reason for the
/// callback.
///
/// \param id Value passed to the callback providing an 'error code' for
/// the callback.
/// Specifies an implementation-specific identifier number providing more
/// info on the callback.  For errors, this is an error code.
///
/// \param severity Value passed to the callback to indicate the severity
/// of the callback.
///
/// \param message Text string passed to the callback with more information
/// on the callback.
///
/// \param userParam Value passed to the callback holding a 'user pointer'
/// specified when the callback was registered.
/// Can be used by application code to specify a pointer to a data
/// structure that should be used by the callback function.  For example,
/// if the callback logs messages to a file, userParam could be a file
/// descriptor.
typedef void (NVNAPIENTRYP DebugCallbackFunc)(DebugCallbackSource::Enum source,
                                              DebugCallbackType::Enum type,
                                              int id,
                                              DebugCallbackSeverity::Enum severity,
                                              const char * message,
                                              void* userParam);

/// \brief Function pointer called when we have run out of memory in a
/// command buffer.
///
/// \param cmdBuf Command buffer that has run out of memory.
///
/// \param event Type of out-of-memory event triggering the callback.
///
/// \param minSize Minimum size of required memory in bytes.
/// When running out of memory during regular command set construction it
/// is recommended for applications to provide at least the minimum amount
/// of memory, given by the device properties
/// DeviceInfo::COMMAND_BUFFER_MIN_COMMAND_SIZE and
/// DeviceInfo::COMMAND_BUFFER_MIN_CONTROL_SIZE. When building command sets
/// from other command sets using CommandBuffer::CopyCommands, this
/// parameter will specify the minimum amount of size required to copy all
/// or portions of the command set being copied.  Applications can reduce
/// the frequency of out-of-memory callbacks by providing memory blocks
/// larger than the minimum required size.
///
/// \param[in,out] callbackData Generic pointer registered as the callback
/// data for the command buffer.
typedef void (NVNAPIENTRYP CommandBufferMemoryCallbackFunc)(CommandBuffer *cmdBuf,
                                                            CommandBufferMemoryEvent::Enum event,
                                                            size_t minSize,
                                                            void *callbackData);

/// \brief Function pointer to be triggered for every object in the debug
/// layer object database during a walk.
///
/// \param object Pointer to the API object. Type of the object is given as
/// parameter into Device::WalkDebugDatabasePointer
///
/// \param[in,out] userParam Generic user pointer passed into
/// Device::WalkDebugDatabasePointer.
typedef void (NVNAPIENTRYP WalkDebugDatabaseCallbackFunc)(void* object,
                                                          void *userParam);

/// \}
/// \addtogroup nvn_cpp_apiclasses
/// \{

/// \brief Object specifying state used to construct new device objects.
NVN_PRE_ALIGN(8)
class DeviceBuilder {
protected:
    char reserved[64];
public:

    /// \brief Set default state for the DeviceBuilder object.
    inline void SetDefaults();

    /// \brief Set flags used for during device initialzation.
    ///
    /// \param flags Flags to use during device initialization. See
    /// nvn::DeviceFlagBits.
    inline void SetFlags(DeviceFlagBits flags);

    /// \brief Get flags associated with this DeviceBuilder object.
    inline DeviceFlagBits GetFlags() const;

} NVN_POST_ALIGN(8);

/// \brief API class used to represent a specific GPU/device.
NVN_PRE_ALIGN(8)
class Device {
protected:
    char reserved[12288];
    Device(const Device &other) /* disabled */;
    const Device & operator =(const Device &other) /* disabled */;
public:
    Device() {}

    /// \brief Initialize a Device object from a device builder.
    ///
    /// \param builder DeviceBuilder object holding properties of the new
    /// Device object.
    inline NVNboolean Initialize(const DeviceBuilder *builder);

    /// \brief Finalize a Device object.
    inline void Finalize();

    /// \brief Sets the debug label string for a Device object.
    ///
    /// Annotates a Device object with a descriptive text label. This label
    /// may then be used by the debug layer, an external debugger, or a
    /// profiler tool.
    ///
    /// \param label Text string used to annotate the Device object.
    inline void SetDebugLabel(const char * label);

    /// \brief Query a function pointer for a specified C interface entry
    /// point for a device.
    ///
    /// \param name Name of the C entry point to query.
    inline GenericFuncPtrFunc GetProcAddress(const char * name) const;

    /// \brief Query properties of a device.
    ///
    /// \note _device_ may be NULL when querying supported API or GPU
    /// shader code versions prior to creating a device object.  For all
    /// other queries, _device_ must point at a valid NVN device object.
    ///
    /// \param pname Property to query.
    ///
    /// \param[out] v Value associated with the property, returned to the
    /// application.
    inline void GetInteger(DeviceInfo pname,
                           int *v) const;

    /// \brief Get timestamp from the device, without queuing like the
    /// ReportCounter functions. See the \ref nvn_guide_counters "Counters"
    /// section of the NVN Programming Guide for more detail.
    ///
    /// \note _device_ must point at a valid NVN device object.
    inline uint64_t GetCurrentTimestampInNanoseconds() const;

    /// \brief Configure the shader cache for the Windows reference
    /// platform to store fat binary intermediate results. If the NVN
    /// platform is NX, this function does nothing.
    ///
    /// On the Windows reference implemenetation, Program::SetShaders will
    /// compile machine code that can run on the attached GPU, using
    /// intermediate shader code and specialization information stored in
    /// the provided control data. If the same shader is compiled with
    /// different specialization information (via glslcCompileSpecialized)
    /// using this cache can avoid re-processing the same shader code on
    /// each call to Program::SetShaders.
    ///
    /// \param numMaxEntries Maximum number of entries that the
    /// intermediate shader cache can hold. If this is 0, the cache will be
    /// disabled and any previous cache resources will be cleaned up.  If a
    /// non-zero value is used, the cache will hold intermediate code for
    /// up to _numMaxEntries_ unique shaders.  Programs containing multiple
    /// shader stages will consume multiple entries in this cache.
    inline void SetIntermediateShaderCache(int numMaxEntries);

    /// \brief Get a handle for a combined pair of Texture and Sampler
    /// objects.
    ///
    /// \param textureID Registered ID of the Texture object.
    /// Must be greater than or equal to 256 (the value of
    /// nvn::DeviceInfo::RESERVED_TEXTURE_DESCRIPTORS).
    ///
    /// \param samplerID Registered ID of the Sampler object.
    /// Must be greater than or equal to 256 (the value of
    /// nvn::DeviceInfo::RESERVED_SAMPLER_DESCRIPTORS).
    inline TextureHandle GetTextureHandle(int textureID,
                                          int samplerID) const;

    /// \brief Get a handle for a Texture object for use in texel fetches.
    ///
    /// A handle produced by this entry point can be used for texel fetches
    /// via the texelFetch() GLSL built-in function.  Using such handles in
    /// texture lookups will use undefined sampler state.
    ///
    /// \param textureID Registered ID of the Texture object.
    /// Must be greater than or equal to 256 (the value of
    /// nvn::DeviceInfo::RESERVED_TEXTURE_DESCRIPTORS).
    inline TextureHandle GetTexelFetchHandle(int textureID) const;

    /// \brief Get a handle to use for image loads and stores to a Texture
    /// object.
    ///
    /// \param imageID Registered image ID of the Texture object.
    /// Must be greater than or equal to 256 (the value of
    /// nvn::DeviceInfo::RESERVED_TEXTURE_DESCRIPTORS).
    inline ImageHandle GetImageHandle(int imageID) const;

    /// \brief Install or remove a callback function to be called whenever
    /// the debug layer generates a message.
    ///
    /// Multiple callback functions (each with separate callback data) are
    /// supported.
    ///
    /// \param callback Pointer to a callback function to call.
    ///
    /// \param[in,out] callbackData Pointer to arbitrary data to pass in
    /// each call to the callback function.
    ///
    /// \param enable Enable (TRUE) or disable (FALSE) the specified
    /// callback function.
    inline void InstallDebugCallback(const DebugCallbackFunc callback,
                                     void *callbackData,
                                     NVNboolean enable);

    /// \brief This returns a unique ID by atomically incrementing a global
    /// counter on each request.
    ///
    /// \param name Name to associate to new domain id.
    inline NVNdebugDomainId GenerateDebugDomainId(const char * name) const;

    /// \brief Set the global location for window coordinates (0,0).
    ///
    /// Applications should set the window origin mode in the device before
    /// creating any queues or command buffers, and should use that mode
    /// for the lifetime of the application. Rendering results are
    /// undefined if there are any inconsistencies between the window
    /// origin mode (a) currently set for the device, (b) set when creating
    /// the command buffer used to create rendering commands, or (c) set
    /// when creating the queue used to submit rendering commands.
    ///
    /// \param windowOriginMode New location for the window origin.
    inline void SetWindowOriginMode(WindowOriginMode windowOriginMode);

    /// \brief Set the global clipping range for normalized depth in clip
    /// space.
    ///
    /// Applications should set the depth mode in the device before
    /// creating any queues or command buffers, and should use that mode
    /// for the lifetime of the application. Rendering results are
    /// undefined if there are any inconsistencies between the depth mode
    /// (a) currently set for the device, (b) set when creating the command
    /// buffer used to create rendering commands, or (c) set when creating
    /// the queue used to submit rendering commands.
    ///
    /// \param depthMode New policy for depth clipping.
    inline void SetDepthMode(DepthMode depthMode);

    /// \brief Register a fast clear color value.
    ///
    /// Applications can register specific combinations of clear values and
    /// formats that it expects to use for future framebuffer clears.
    /// Clearing the framebuffer using one of these registered clear colors
    /// will result in reduced memory bandwidth usage and increased
    /// performance when used with compressible textures.
    ///
    /// The number of clear values that can be registered is limited.
    /// Additionally, this command is supported only for formats with more
    /// than 16 bits per pixel and floating-point or normalized components.
    ///  If a clear value or format can not be registered, NVN_FALSE is
    /// returned.  If registration is successful, NVN_TRUE is returned.
    ///
    /// \param color Array of four floating-point (RGBA) clear color
    /// values.
    ///
    /// \param format Texture format to encode the value into.
    inline NVNboolean RegisterFastClearColor(const float *color,
                                             Format format);

    /// \brief Register a signed integer fast clear color value.
    ///
    /// Applications can register specific combinations of clear values and
    /// formats that it expects to use for future framebuffer clears.
    /// Clearing the framebuffer using one of these registered clear colors
    /// will result in reduced memory bandwidth usage and increased
    /// performance when used with compressible textures.
    ///
    /// The number of clear values that can be registered is limited.
    /// Additionally, this command is supported only for formats with more
    /// than 16 bits per pixel and signed integer components.  If a clear
    /// value or format can not be registered, NVN_FALSE is returned.  If
    /// registration is successful, NVN_TRUE is returned.
    ///
    /// \param color Array of four signed integer (RGBA) clear color
    /// values.
    ///
    /// \param format Texture format to encode the value into.
    inline NVNboolean RegisterFastClearColori(const int *color,
                                              Format format);

    /// \brief Register an unsigned integer fast clear color value.
    ///
    /// Applications can register specific combinations of clear values and
    /// formats that it expects to use for future framebuffer clears.
    /// Clearing the framebuffer using one of these registered clear colors
    /// will result in reduced memory bandwidth usage and increased
    /// performance when used with compressible textures.
    ///
    /// The number of clear values that can be registered is limited.
    /// Additionally, this command is supported only for formats with more
    /// than 16 bits per pixel and unsigned integer components.  If a clear
    /// value or format can not be registered, NVN_FALSE is returned.  If
    /// registration is successful, NVN_TRUE is returned.
    ///
    /// \param color Array of four unsigned integer (RGBA) clear color
    /// values.
    ///
    /// \param format Texture format to encode the value into.
    inline NVNboolean RegisterFastClearColorui(const uint32_t *color,
                                               Format format);

    /// \brief Register a fast clear depth value.
    ///
    /// Applications can register specific clear values that it expects to
    /// use for future depth buffer clears. Clearing the depth buffer using
    /// one of these registered clear depth values will result in reduced
    /// memory bandwidth usage and increased performance when used with
    /// compressible textures.
    ///
    /// The number of clear values that can be registered is finite. If a
    /// clear value can not be registered, NVN_FALSE is returned. If
    /// registration is successful, NVN_TRUE is returned.
    /// \note 16-bit depth textures (Format::DEPTH16) have limited support
    /// for fast clears.  See documentation on
    /// TextureFlags::DEPTH16_PREFER_FAST_CLEAR for more information.
    ///
    /// \param depth Depth value.
    inline NVNboolean RegisterFastClearDepth(float depth);

    /// \brief Query the window origin mode for a device.
    inline WindowOriginMode GetWindowOriginMode() const;

    /// \brief Query the depth mode for a device.
    inline DepthMode GetDepthMode() const;

    /// \brief Returns the timestamp contained in a counter report, with
    /// time values converted from the platform-specific timestamp
    /// resolution to nanoseconds.
    ///
    /// \param counterData Pointer to a full CounterData structure
    /// containing the timestamp value to convert.
    inline uint64_t GetTimestampInNanoseconds(const CounterData *counterData) const;

    /// \brief Finalize buffers, textures, and samplers previously
    /// scheduled for finalization.
    ///
    /// When a device is initialized with DeviceFlagBits::DEFERRED_FINALIZE
    /// set, calls to Buffer::Finalize, Texture::Finalize, and
    /// Sampler::Finalize don't immediately finalize buffer, texture, and
    /// sampler objects, but instead schedule them for future finalization.
    ///  This command finalizes such objects, where the set of objects
    /// finalized by this command depends on the _age_ parameter.  This
    /// command has no effect if deferred finalization is not enabled in
    /// _device_.
    ///
    /// \param age Minimum age for deferred finalizations.
    /// Objects scheduled for finalization have an 'age' that is initially
    /// set to zero and incremented each time
    /// Device::ApplyDeferredFinalizes is called.  Each object scheduled
    /// for finalization will be finalized by this call if and only if its
    /// age (before incrementing) is greater than or equal to _age_.
    inline void ApplyDeferredFinalizes(int age);

    /// \brief Finalize a command handle.
    ///
    /// This command can be used to free any memory associated with a
    /// command handle that may have been allocated by the debug layer or
    /// other developer tools that track the contents of a command set.  If
    /// command handles are not explicitly finalized, associated memory
    /// will be freed only when the command or control memory associated
    /// with the command set is freed or reused.
    ///
    /// This command has no effect if the debug layer and other developer
    /// tools are disabled.
    ///
    /// \param handles Command handle to finalize.
    inline void FinalizeCommandHandle(CommandHandle handles);

    /// \brief Walk through the debug layer object database. Debug layer
    /// must be enabled.
    ///
    /// Reports the debug layer's list of objects of the given type through
    /// the callback function.  At least Level 1 debug layer must be
    /// enabled, as object tracking is not done at debug layer level 0.
    /// See \ref nvn_guide_api_object_tracking "API Object Tracking" for
    /// detail.
    ///
    /// \param type Type of objects to walk through.
    ///
    /// \param callback Callback function to be called for every object
    /// matching the given device and object type.
    ///
    /// \param[in,out] userParam Value to be passed to the callback holding
    /// a 'user pointer'
    inline void WalkDebugDatabase(DebugObjectType type,
                                  WalkDebugDatabaseCallbackFunc callback,
                                  void *userParam) const;

    /// \brief Get a handle for a separate Texture object.
    ///
    /// \param textureID Registered ID of the Texture object.
    /// Must be greater than or equal to 256 (the value of
    /// nvn::DeviceInfo::RESERVED_TEXTURE_DESCRIPTORS).
    inline SeparateTextureHandle GetSeparateTextureHandle(int textureID) const;

    /// \brief Get a handle for a separate Texture object. Deprecated in
    /// favor of Device::GetSeparateTextureHandle.
    ///
    /// \param textureID Registered ID of the Texture object.
    /// Must be greater than or equal to 256 (the value of
    /// nvn::DeviceInfo::RESERVED_TEXTURE_DESCRIPTORS).
    NVN_PRE_DEPRECATED
    inline SeparateTextureHandle getSeparateTextureHandle(int textureID) const
    NVN_POST_DEPRECATED;

    /// \brief Get a handle for a separate Sampler object.
    ///
    /// \param samplerID Registered ID of the Sampler object.
    /// Must be greater than or equal to 256 (the value of
    /// nvn::DeviceInfo::RESERVED_SAMPLER_DESCRIPTORS).
    inline SeparateSamplerHandle GetSeparateSamplerHandle(int samplerID) const;

    /// \brief Get a handle for a separate Sampler object. Deprecated in
    /// favor of Device::GetSeparateSamplerHandle.
    ///
    /// \param samplerID Registered ID of the Sampler object.
    /// Must be greater than or equal to 256 (the value of
    /// nvn::DeviceInfo::RESERVED_SAMPLER_DESCRIPTORS).
    NVN_PRE_DEPRECATED
    inline SeparateSamplerHandle getSeparateSamplerHandle(int samplerID) const
    NVN_POST_DEPRECATED;

    /// \brief Determines if an external debugger is attached when running
    /// an NVN application.
    inline NVNboolean IsExternalDebuggerAttached() const;

} NVN_POST_ALIGN(8);

/// \brief Object specifying state used to construct new queue objects.
NVN_PRE_ALIGN(8)
class QueueBuilder {
protected:
    char reserved[64];
public:

    /// \brief Set the Device associated with a QueueBuilder object.
    ///
    /// \param device Device owning Queue objects created by the
    /// QueueBuilder.
    inline QueueBuilder & SetDevice(Device *device);

    /// \brief Set default state for the QueueBuilder object.
    ///
    /// This method does not modify the device associated with the builder,
    /// which is programmed by SetDevice.
    inline QueueBuilder & SetDefaults();

    /// \brief Set properties of Queue objects created from the builder.
    ///
    /// The default value for queue flags is QueueFlags::NONE.
    ///
    /// \param flags Flags controlling the behavior of queues initialized
    /// using the builder. See nvn::QueueFlags.
    inline QueueBuilder & SetFlags(QueueFlags flags);

    /// \brief Specify the size of the internal command buffer used by a
    /// Queue object.
    ///
    /// Each NVN queue has an internal command buffer used to send commands
    /// to the GPU.  This command buffer is used by some Queue class
    /// methods and is also used for any required submit-time processing of
    /// command sets sent by Queue::SubmitCommands.  Most queue commands do
    /// not require a large amount of command memory.  However, compute
    /// dispatches and shader subroutine uniform updates can require a
    /// substantial amount of per-queue command memory.  The internal queue
    /// command buffer memory is managed automatically by the NVN driver,
    /// and internal queue flushes may be required if a significant amount
    /// of command memory is used since the last explicit queue flush.  For
    /// workloads requiring a significant amount of command memory,
    /// applications can specify a command memory size larger than the 64KB
    /// default size.
    ///
    /// The default command memory size can be queried using the property
    /// DeviceInfo::QUEUE_COMMAND_MEMORY_DEFAULT_SIZE.
    ///
    /// \param size Size (in bytes) of the queue's internal command memory.
    /// Must be greater than or equal to 65536 (the value of
    /// nvn::DeviceInfo::QUEUE_COMMAND_MEMORY_MIN_SIZE).
    /// Must be a multiple of 4096 (the value of
    /// nvn::DeviceInfo::QUEUE_COMMAND_MEMORY_GRANULARITY).
    inline QueueBuilder & SetCommandMemorySize(size_t size);

    /// \brief Specify the size of the internal buffers used for compute
    /// dispatches in a Queue object.
    ///
    /// Each NVN queue may have an internal ring buffer used to hold
    /// information on compute dispatches that have been submitted to the
    /// queue but have not completed execution.  The queue compute memory
    /// is managed automatically by the NVN driver, but the total size of
    /// this ring buffer can limit the number of compute dispatches in
    /// flight.  Applications using a large number of compute dispatches
    /// per frame may benefit from increasing the size of this buffer.
    /// Applications not using compute shaders at all may set the size to
    /// zero to save memory.
    ///
    /// The default compute memory size can be queried using the property
    /// DeviceInfo::QUEUE_COMPUTE_MEMORY_DEFAULT_SIZE.
    ///
    /// \note If the queue compute memory size is zero, it is illegal to
    /// submit command sets containing compute work, such as
    /// CommandBuffer::DispatchCompute.
    ///
    /// \param size Size (in bytes) of the embedded queue command buffer.
    /// Must be a multiple of 256 (the value of
    /// nvn::DeviceInfo::QUEUE_COMPUTE_MEMORY_GRANULARITY).
    inline QueueBuilder & SetComputeMemorySize(size_t size);

    /// \brief Specify the size of the control memory buffer used by a
    /// Queue object.
    ///
    /// Each NVN queue has an internal control memory buffer used for
    /// gathering command buffer references consumed by the GPU. For
    /// example Queue::SubmitCommands and CommandBuffer::CallCommands will
    /// add these references. The control buffer needs to be large enough
    /// so that NVN doesn't need to stall due to waiting for the buffer to
    /// drain. On the other hand, unnecessarily large buffer wastes memory.
    /// The default size of 16KB should be enough for most applications,
    /// but an application may want to tune this value for its needs. NVN
    /// will issue a debug layer performance warning in case the buffer is
    /// too small.
    ///
    /// The default queue control memory size can be queried using the
    /// property DeviceInfo::QUEUE_CONTROL_MEMORY_DEFAULT_SIZE.
    ///
    /// \param size Size (in bytes) of the queue control memory.
    /// Must be greater than or equal to 4096 (the value of
    /// nvn::DeviceInfo::QUEUE_CONTROL_MEMORY_MIN_SIZE).
    /// Must be a multiple of 4096 (the value of
    /// nvn::DeviceInfo::QUEUE_CONTROL_MEMORY_GRANULARITY).
    inline QueueBuilder & SetControlMemorySize(size_t size);

    /// \brief Query the amount of extra memory needed for a queue
    /// initialized using a QueueBuilder.
    ///
    /// Each NVN queue requires a separate block of per-queue memory for
    /// the GPU to use, which can be provided by
    /// QueueBuilder::SetQueueMemory.  This command allows applications to
    /// determine the size of the provided memory, given the current
    /// QueueBuilder state. For correct operation, the QueueBuilder must
    /// have a device set via QueueBuilder::SetDevice.
    inline size_t GetQueueMemorySize() const;

    /// \brief Provides memory to be used by queues pools initialized using
    /// a QueueBuilder.
    ///
    /// Each NVN queue created from a queue builder requires per-queue
    /// memory for several uses:
    ///
    /// \li Embedded command buffer memory, with a size provided by
    /// QueueBuilder::SetCommandMemorySize.
    /// \li Embedded control memory, with a size provided by
    /// QueueBuilder::SetControlMemorySize.
    /// \li Compute shader ring buffers, with a size provided by
    /// QueueBuilder::SetComputeMemorySize.
    /// \li Scratch memory for fragment shader interlocks, which can be
    /// disabled by setting QueueFlags::NO_FRAGMENT_INTERLOCK.
    /// \li Scratch memory for other internal GPU operations.
    ///
    /// When a queue is initialized using a queue builder on NX, _size_
    /// bytes of memory pointed to by _memory_ will be owned by the queue
    /// until the queue is finalized.  Applications must not access
    /// _memory_ directly after calling Queue::Initialize.  _size_ must be
    /// sufficiently large to satisfy the requirements described above; the
    /// minimum size can be queried using QueueBuilder::GetQueueMemorySize.
    ///  On the Windows reference implementation, per-queue memory is
    /// allocated by the driver, and the memory provided by _memory_ (if
    /// any) is ignored.
    ///
    /// The default queue memory size is zero, indicating that no memory is
    /// provided by the application.  In that case, the NVN driver will
    /// allocate a buffer of sufficient size when the queue is initialized.
    ///
    /// \param[in,out] memory Storage used for the queue memory.
    /// Must be aligned to a multiple of 4KB
    /// (NVN_MEMORY_POOL_STORAGE_ALIGNMENT).
    ///
    /// \param size Size (in bytes) of the queue memory.
    /// Must be aligned to a multiple of 4KB
    /// (NVN_MEMORY_POOL_STORAGE_GRANULARITY).
    inline QueueBuilder & SetQueueMemory(void *memory,
                                         size_t size);

    /// \brief Specify a flush threshold for a new Queue object.
    ///
    /// Queue objects include an embedded command buffer that is
    /// automatically managed by the NVN driver.  This command buffer is
    /// used to send commands to the GPU for various Queue methods or for
    /// command sets that require additional processing when
    /// Queue::SubmitCommands is called.  Most commands require very little
    /// command memory, but certain commands like compute dispatches or
    /// shader subroutine uniform updates may require significant amounts
    /// of command memory.  The NVN driver will automatically flush the
    /// queue when the amount of command memory used since the last queue
    /// flush approaches a specified flush threshold.
    ///
    /// By default, the flush threshold is 1/8 of the total available
    /// command memory.  Setting a higher threshold may reduce the number
    /// of automatic flushes performed, but less frequent flushes may cause
    /// the driver to have to wait for the completion of previous flushes
    /// before reusing command memory.
    ///
    /// \param threshold Threshold (in bytes) of queue command memory usage
    /// triggering automatic flushes.
    /// Must be greater than or equal to 4096 (the value of
    /// nvn::DeviceInfo::QUEUE_COMMAND_MEMORY_MIN_FLUSH_THRESHOLD).
    inline QueueBuilder & SetCommandFlushThreshold(size_t threshold);

    /// \brief Set a priority on a QueueBuilder object.
    ///
    /// Setting a priority on a QueueBuilder object allows for additional
    /// control of the relative execution timing between Queues. Queue
    /// Priority affects both the execution order of the Queues in their
    /// timeslice as well as the (best-effort, round-robin) timesharing of
    /// the GPU resources. GPU-side scheduling of Queues is a function of
    /// method pipelining and shader run-time, and behavior is difficult to
    /// describe in a correct and precise fashion. This function has no
    /// effect on the Windows reference implementation. A few rules help
    /// guide the use of Priorities: \li Mixing Normal and Low should be
    /// avoided, as this increases the number of context switches on the
    /// GPU: prefer High and Low.
    /// \li Content that should render in the frame should be submitted on
    /// High priority Queues, while Low priority is well suited for Compute
    /// content that can run for multiple frames.
    /// \li Queues that have equal priorities will almost surely run in
    /// order of creation.
    ///
    /// \param priority The priority to set on the queue.
    inline QueueBuilder & SetQueuePriority(const QueuePriority priority);

    /// \brief Get the priority associated with this QueueBuilder object.
    inline QueuePriority GetQueuePriority() const;

    /// \brief Get the Device associated with this QueueBuilder object.
    inline const Device * GetDevice() const;

    /// \brief Get queue flags associated with this QueueBuilder object.
    inline int GetFlags() const;

    /// \brief Get the command memory size associated with this
    /// QueueBuilder object.
    ///
    /// The function will return true if the command memory size was
    /// explicitly set.
    ///
    /// \param[out] size Command memory size.
    inline NVNboolean GetCommandMemorySize(int *size) const;

    /// \brief Get the compute memory size associated with this
    /// QueueBuilder object.
    ///
    /// The function will return true if the compute memory size was
    /// explicitly set.
    ///
    /// \param[out] size Compute memory size.
    inline NVNboolean GetComputeMemorySize(int *size) const;

    /// \brief Get the control memory size associated with this
    /// QueueBuilder object.
    ///
    /// The function will return true if the control memory size was
    /// explicitly set.
    ///
    /// \param[out] size Control memory size.
    inline NVNboolean GetControlMemorySize(int *size) const;

    /// \brief Get the command flush threshold associated with this
    /// QueueBuilder object.
    ///
    /// The function will return true if the flush threshold was explicitly
    /// set.
    ///
    /// \param[out] flushThreshold Flush threshold.
    inline NVNboolean GetCommandFlushThreshold(int *flushThreshold) const;

    /// \brief Get the size of the memory associated with this QueueBuilder
    /// object.
    inline int GetMemorySize() const;

    /// \brief Get the memory associated with this QueueBuilder object.
    inline void* GetMemory() const;

} NVN_POST_ALIGN(8);

/// \brief API class used to send commands to the GPU.
NVN_PRE_ALIGN(8)
class Queue {
protected:
    char reserved[8192];
    Queue(const Queue &other) /* disabled */;
    const Queue & operator =(const Queue &other) /* disabled */;
public:
    Queue() {}

    /// \brief Get GPU error status of the queue.
    ///
    /// Applications can use this function to check if queue encountered
    /// unrecoverable GPU error such as invalid GPU memory accesses,
    /// invalid command, timeout or other errors. After GPU error queue is
    /// in faulted state and following behavior is expected:
    /// 1. nvnQueueFinish() returns immediately.
    /// 2. Sync objects placed into faulted queue signal completion.
    /// 3. Faulted queue can be finalized to free resources.
    /// 4. Remaining nvnQueue API calls for faulted queue are ignored or
    /// return error.
    /// 5. Other device queues are not affected or blocked by faulted
    /// queue.
    ///
    /// \param[out] errorInfo Pointer to structure to save information
    /// about error. If set to NULL, information will not be retrieved.
    inline QueueGetErrorResult GetError(QueueErrorInfo *errorInfo);

    /// \brief Query total queue command memory usage.
    ///
    /// This command returns the total amount of queue command memory (in
    /// bytes) used by queue commands since the queue was initialized or
    /// since usage counts were last reset by
    /// Queue::ResetMemoryUsageCounts.  The total memory usage count can be
    /// used by application developers to determine the amount of memory to
    /// provide when initializing a queue. Note that memory usage counts
    /// may exceed the total amount of memory provided to the queue since
    /// the NVN driver will automatically reuse queue memory when required.
    inline size_t GetTotalCommandMemoryUsed();

    /// \brief Query total queue control memory usage.
    ///
    /// This command returns the total amount of queue control memory (in
    /// bytes) used by queue commands since the queue was initialized or
    /// since usage counts were last reset by
    /// Queue::ResetMemoryUsageCounts.  The total memory usage count can be
    /// used by application developers to determine the amount of memory to
    /// provide when initializing a queue. Note that memory usage counts
    /// may exceed the total amount of memory provided to the queue since
    /// the NVN driver will automatically reuse queue memory when required.
    inline size_t GetTotalControlMemoryUsed();

    /// \brief Query total queue compute memory usage.
    ///
    /// This command returns the total amount of queue compute memory (in
    /// bytes) used by queue commands since the queue was initialized or
    /// since usage counts were last reset by
    /// Queue::ResetMemoryUsageCounts.  The total memory usage count can be
    /// used by application developers to determine the amount of memory to
    /// provide when initializing a queue. Note that memory usage counts
    /// may exceed the total amount of memory provided to the queue since
    /// the NVN driver will automatically reuse queue memory when required.
    inline size_t GetTotalComputeMemoryUsed();

    /// \brief Reset usage counters for command, control and compute memory
    ///
    /// This command resets the accumulated usage counters for queue
    /// command, control, and compute memory.
    ///
    /// \note Queue memory consumed by GPU commands may not be added to the
    /// usage counters reset by this command until the queue is next
    /// flushed.  When total queue memory usage is queried, any memory used
    /// by unflushed commands will be added to the total usage counters.
    /// As a result, querying memory usage immediately after resetting
    /// counters may return non-zero values if the queue has unflushed
    /// commands.
    inline void ResetMemoryUsageCounts();

    /// \brief Initialize a Queue object from a queue builder.
    ///
    /// \param builder QueueBuilder object holding properties of the new
    /// Queue object.
    inline NVNboolean Initialize(const QueueBuilder *builder);

    /// \brief Finalize a Queue object.
    ///
    /// \note Finalizing a queue will wait for the completion of all
    /// commands previously submitted to the queue, including those sent by
    /// the driver during queue initialization.
    inline void Finalize();

    /// \brief Sets the debug label string for a Queue object.
    ///
    /// Annotates a Queue object with a descriptive text label. This label
    /// may then be used by the debug layer, an external debugger, or a
    /// profiler tool.
    ///
    /// \param label Text string used to annotate the Queue object.
    inline void SetDebugLabel(const char * label);

    /// \brief Submit commands encapsulated in command buffers to a queue
    /// for processing.
    ///
    /// \param numCommands Number of command buffers to send.
    ///
    /// \param handles Array of numCommands handles for the command buffers
    /// to send.
    inline void SubmitCommands(int numCommands,
                               const CommandHandle *handles);

    /// \brief Flush commands previously submitted to a queue for
    /// processing.
    ///
    /// Commands submitted to Queue objects may be batched up by the
    /// implementation to avoid the overhead that would be involved in
    /// flushing each command immediately after it is submitted.  Flush can
    /// be used to manually flush commands that were previously batched up.
    inline void Flush();

    /// \brief Wait until all commands previously submitted to a queue have
    /// completed execution.
    ///
    /// This command will not return to the caller until the previously
    /// submitted commands have completed.  To determine if submitted
    /// commands have completed without blocking, applications can use Sync
    /// objects and Sync::WaitSync().  Finish will automatically flush the
    /// previously submitted commands as part of its operation.
    inline void Finish();

    /// \brief Display the contents of a texture into the window.
    ///
    /// This command presents the texture specified by _textureIndex_ in
    /// the native window associated with _window_. It also triggers an
    /// implicit call to Queue::Flush so that the texture is guaranteed to
    /// be shown on the screen in finite time.
    ///
    /// \param window The window where the texture should be presented.
    ///
    /// \param textureIndex The index of the texture to be displayed
    /// on-screen.
    /// The texture must be previously acquired from _window_ using
    /// Window::AcquireTexture.  Queue::PresentTexture transfers the
    /// ownership of the texture back to the platform windowing system, and
    /// the application must re-acquire it with Window::AcquireTexture
    /// before using it again.
    inline void PresentTexture(Window *window,
                               int textureIndex);

    /// \brief Acquire a texture from the window.  Deprecated in favor of
    /// Window::AcquireTexture.
    ///
    /// This is semantically equivalent to calling
    ///
    /// \code{.c}
    ///   Sync displayReleaseSync;
    ///   displayReleaseSync.Initialize(...);
    ///   window.AcquireTexture(&displayReleaseSync, textureIndex);
    ///   queue.WaitSync(&displayReleaseSync);
    ///   displayReleaseSync.Finalize();
    /// \endcode
    ///
    /// \param window Window object to acquire a texture from.
    ///
    /// \param[out] textureIndex An index into the texture array that was
    /// passed in when _window_ was initialized, which identifies the
    /// texture that is now released from the platform windowing system to
    /// the application.
    NVN_PRE_DEPRECATED
    inline QueueAcquireTextureResult AcquireTexture(Window *window,
                                                    int *textureIndex)
    NVN_POST_DEPRECATED;

    /// \brief Mark a Sync object as signaled when all previous commands
    /// have completed in the Queue object.
    ///
    /// \warning FenceSync commands are not automatically flushed for
    /// processing by the GPU.  Before waiting on a FenceSync call with the
    /// CPU using Sync::Wait or from another Queue using Queue::WaitSync,
    /// applications must ensure that the previous FenceSync call was
    /// flushed using Queue::Flush.  Waiting on a FenceSync call using
    /// Queue::WaitSync in the same queue requires no manual flush.
    ///
    /// \note If tiled caching is enabled, this command will trigger an
    /// implicit tiled cache flush.
    ///
    /// \param sync Sync object to mark as signaled when commands complete.
    ///
    /// \param condition Type of command completion to wait for.
    ///
    /// \param flags Bitfield of additional operations to perform before
    /// signaling. See nvn::SyncFlagBits.
    inline void FenceSync(Sync *sync,
                          SyncCondition condition,
                          SyncFlagBits flags);

    /// \brief Wait for a Sync object to be signaled on the GPU before
    /// processing any further commands in the specified Queue object.
    ///
    /// \param sync Sync object to wait on.
    inline NVNboolean WaitSync(const Sync *sync);

} NVN_POST_ALIGN(8);

/// \brief Collection of commands to send to the GPU via queues.
NVN_PRE_ALIGN(8)
class CommandBuffer {
protected:
    char reserved[160];
    CommandBuffer(const CommandBuffer &other) /* disabled */;
    const CommandBuffer & operator =(const CommandBuffer &other) /* disabled */;
public:
    CommandBuffer() {}

    /// \brief Initializes a CommandBuffer object for the specified device.
    ///
    /// \param device Device owning the CommandBuffer object.
    inline NVNboolean Initialize(Device *device);

    /// \brief Finalize a CommandBuffer object.
    inline void Finalize();

    /// \brief Sets the debug label string for a CommandBuffer object.
    ///
    /// Annotates a CommandBuffer object with a descriptive text label.
    /// This label may then be used by the debug layer, an external
    /// debugger, or a profiler tool.
    ///
    /// \param label Text string used to annotate the CommandBuffer object.
    inline void SetDebugLabel(const char * label);

    /// \brief Set a callback function for a CommandBuffer object that will
    /// be called if it runs out of memory.
    ///
    /// \param callback Callback function to use on out-of-memory events.
    inline void SetMemoryCallback(CommandBufferMemoryCallbackFunc callback);

    /// \brief Specify a generic pointer that will be passed to a callback
    /// function if the command buffer runs out of memory.
    ///
    /// \param[in,out] callbackData Callback data to be passed in an
    /// out-of-memory event callback.
    inline void SetMemoryCallbackData(void *callbackData);

    /// \brief Enable or disable the memory callback when out of command
    /// memory.
    ///
    /// If a sequence of commands is known to fit within allocated command
    /// memory, applications can prevent the driver from conservatively
    /// invoking the out-of-memory callback when previously allocated
    /// command memory is nearly exhausted.  This function does not affect
    /// behavior related to control memory.
    ///
    /// \param enabled True (default) if the out-of-memory callback should
    /// be enabled; false otherwise.
    inline void SetCommandMemoryCallbackEnabled(NVNboolean enabled);

    /// \brief Add command data memory for a CommandBuffer object. This can
    /// be called at any time, even outside an out-of-memory callback or
    /// while recording or not recording. Any leftover space in the
    /// previous command memory will remain unused. Only the last provided
    /// block is consumed.
    ///
    /// \param pool Memory pool used to store commands.
    ///
    /// \param offset Offset (in bytes) relative to pool of the memory used
    /// to store commands.
    /// Must be a multiple of 4 (the value of
    /// nvn::DeviceInfo::COMMAND_BUFFER_COMMAND_ALIGNMENT).
    ///
    /// \param size Size (in bytes) of memory used to store commands.
    /// Must be a multiple of 4 (the value of
    /// nvn::DeviceInfo::COMMAND_BUFFER_COMMAND_ALIGNMENT).
    inline void AddCommandMemory(const MemoryPool *pool,
                                 ptrdiff_t offset,
                                 size_t size);

    /// \brief Add control memory for a CommandBuffer object. This can be
    /// called at any time, even outside an out-of-memory callback or while
    /// recording or not recording. Any leftover space in the previous
    /// control memory will remain unused. Only the last provided block is
    /// consumed.
    ///
    /// \param[in,out] memory Storage used for the command buffer control
    /// information.
    /// Must be a multiple of 8 (the value of
    /// nvn::DeviceInfo::COMMAND_BUFFER_CONTROL_ALIGNMENT).
    ///
    /// \param size Size (in bytes) of memory used to store control
    /// information.
    /// Must be a multiple of 8 (the value of
    /// nvn::DeviceInfo::COMMAND_BUFFER_CONTROL_ALIGNMENT).
    inline void AddControlMemory(void *memory,
                                 size_t size);

    /// \brief Get the total size in bytes of the command memory block that
    /// was lastly added to the CommandBuffer. Please see the
    /// \ref nvn_guide_command_buffer_memory_management
    /// "Command Buffer Memory Management"
    /// section of the NVN Programming Guide for further detail.
    inline size_t GetCommandMemorySize() const;

    /// \brief Get the used size in bytes of the command memory block that
    /// was lastly added to the CommandBuffer. Please see the
    /// \ref nvn_guide_command_buffer_memory_management
    /// "Command Buffer Memory Management"
    /// section of the NVN Programming Guide for further detail.
    inline size_t GetCommandMemoryUsed() const;

    /// \brief Get the unused size in bytes of the command memory block
    /// that was lastly added to the CommandBuffer. Please see the
    /// \ref nvn_guide_command_buffer_memory_management
    /// "Command Buffer Memory Management"
    /// section of the NVN Programming Guide for further detail.
    inline size_t GetCommandMemoryFree() const;

    /// \brief Get the total size in bytes of the control memory block that
    /// was lastly added to the CommandBuffer. Please see the
    /// \ref nvn_guide_command_buffer_memory_management
    /// "Command Buffer Memory Management"
    /// section of the NVN Programming Guide for further detail.
    inline size_t GetControlMemorySize() const;

    /// \brief Get the used size in bytes of the control memory block that
    /// was lastly added to the CommandBuffer. Please see the
    /// \ref nvn_guide_command_buffer_memory_management
    /// "Command Buffer Memory Management"
    /// section of the NVN Programming Guide for further detail.
    inline size_t GetControlMemoryUsed() const;

    /// \brief Get the unused size in bytes of the control memory block
    /// that was lastly added to the CommandBuffer. Please see the
    /// \ref nvn_guide_command_buffer_memory_management
    /// "Command Buffer Memory Management"
    /// section of the NVN Programming Guide for further detail.
    inline size_t GetControlMemoryFree() const;

    /// \brief Begin recording a new set of commands in this CommandBuffer
    /// object.
    inline void BeginRecording();

    /// \brief Complete recording a new set of commands in this
    /// CommandBuffer object.
    inline CommandHandle EndRecording();

    /// \brief Insert calls to previously recorded command sets into the
    /// command set under construction.
    ///
    /// CallCommands will copy control data from the previously recorded
    /// command sets into the new command set.  Command data from these
    /// command sets are called directly.  The command buffer under
    /// construction must hold sufficient memory to hold the control data.
    /// The command data from called command sets must not be freed until
    /// all command sets referencing the data are no longer in use.  The
    /// command data in the called command sets should be recorded in a
    /// memory pool using MemoryPoolFlags::CPU_UNCACHED.
    ///
    /// \param numCommands Number of command sets to call.
    ///
    /// \param handles Array of numCommands command set handles to call.
    inline void CallCommands(int numCommands,
                             const CommandHandle *handles);

    /// \brief Insert copies of previously recorded command sets into the
    /// command set under construction.
    ///
    /// CopyCommands will copy command and control data from the previously
    /// recorded command sets into the new command set.  The command buffer
    /// must hold sufficient memory to hold the copied data.  The command
    /// data in the called command sets should be recorded in a memory pool
    /// using MemoryPoolFlags::CPU_CACHED.
    ///
    /// \param numCommands Number of command sets to copy.
    ///
    /// \param handles Array of numCommands command set handles to copy.
    inline void CopyCommands(int numCommands,
                             const CommandHandle *handles);

    /// \brief Specify a new BlendState object to bind for subsequent
    /// commands.
    ///
    /// \param blend BlendState object to be bound.
    inline void BindBlendState(const BlendState *blend);

    /// \brief Specify a new ChannelMaskState object to bind for subsequent
    /// commands.
    ///
    /// \param channelMask ChannelMaskState object to be bound.
    inline void BindChannelMaskState(const ChannelMaskState *channelMask);

    /// \brief Specify a new ColorState object to bind for subsequent
    /// commands.
    ///
    /// \param color ColorState object to be bound.
    inline void BindColorState(const ColorState *color);

    /// \brief Specify a new MultisampleState object to bind for subsequent
    /// commands.
    ///
    /// \param multisample MultisampleState object to be bound.
    inline void BindMultisampleState(const MultisampleState *multisample);

    /// \brief Specify a new PolygonState object to bind for subsequent
    /// commands.
    ///
    /// \param polygon PolygonState object to be bound.
    inline void BindPolygonState(const PolygonState *polygon);

    /// \brief Specify a new DepthStencilState object to bind for
    /// subsequent commands.
    ///
    /// \param depthStencil DepthStencilState object to be bound.
    inline void BindDepthStencilState(const DepthStencilState *depthStencil);

    /// \brief Specify a vertex attribute layout for subsequent commands.
    ///
    /// All attributes beyond _numAttribs_ are disabled, and fetching from
    /// them in the shader will return zero.
    ///
    /// \param numAttribs Number of vertex attributes to bind.
    /// Must be less than or equal to 16 (the value of
    /// nvn::DeviceInfo::VERTEX_ATTRIBUTES).
    ///
    /// \param attribs VertexAttribState objects to be bound.
    inline void BindVertexAttribState(int numAttribs,
                                      const VertexAttribState *attribs);

    /// \brief Specify a vertex stream layout for subsequent commands.
    ///
    /// \param numStreams Number of vertex streams to bind.
    /// Must be less than or equal to 16 (the value of
    /// nvn::DeviceInfo::VERTEX_BUFFER_BINDINGS).
    ///
    /// \param streams VertexStreamState objects to be bound.
    inline void BindVertexStreamState(int numStreams,
                                      const VertexStreamState *streams);

    /// \brief Bind or unbind shaders in a Program object for subsequent
    /// rendering commands.
    ///
    /// This command binds all shaders present in _program_, and optionally
    /// disables additional shader stages based on the bitfield _stages_.
    /// If _program_ is NULL, all shader stages specified in the bitfield
    /// _stages_ are disabled.  If _program_ is non-NULL, all shaders
    /// present in _program_ will be bound and any stages in _stages_ that
    /// don't have shaders in _program_ are disabled.  For example, using
    /// ShaderStageBits::ALL_GRAPHICS_BITS with a program with only a
    /// vertex and fragment shader will bind the vertex and fragment
    /// shaders in the program and disable the optional tessellation
    /// control, tessellation evaluation, and geometry shader stages.
    ///
    /// \warning When _program_ is non-NULL, all stages present in
    /// _program_ must be enabled in the bitfield _stages_.
    ///
    /// \warning In the C interface, it is possible to pass enums of the
    /// type NVNshaderStage (NVN_VERTEX_SHADER) in _stages_.  This is not
    /// legal; _stages_ requires a bitfield built from values of type
    /// NVNshaderStageBits (NVN_VERTEX_SHADER_BIT).
    ///
    /// \param program Program object to be bound.
    ///
    /// \param stages Bitfield identifying the set of shader stages to be
    /// updated. See nvn::ShaderStageBits.
    inline void BindProgram(const Program *program,
                            ShaderStageBits stages);

    /// \brief Bind a range of a Buffer object to use for fetching vertex
    /// attributes.
    ///
    /// \param index Vertex buffer binding point number to update.
    /// Must be less than 16 (the value of
    /// nvn::DeviceInfo::VERTEX_BUFFER_BINDINGS).
    ///
    /// \param buffer GPU address of vertex buffer memory to bind.
    ///
    /// \param size Size (in bytes) of the range of the Buffer object to
    /// bind.
    inline void BindVertexBuffer(int index,
                                 BufferAddress buffer,
                                 size_t size);

    /// \brief Bind one or more Buffer object ranges to use for fetching
    /// vertex attributes.
    ///
    /// \param first First vertex buffer binding point number to update.
    /// Must be less than 16 (the value of
    /// nvn::DeviceInfo::VERTEX_BUFFER_BINDINGS).
    ///
    /// \param count Number of contiguous vertex buffer binding points to
    /// update.
    /// Must be greater than or equal to zero.
    /// Must be less than or equal to 16 (the value of
    /// nvn::DeviceInfo::VERTEX_BUFFER_BINDINGS).
    ///
    /// \param buffers Array of vertex buffer ranges to bind.
    inline void BindVertexBuffers(int first,
                                  int count,
                                  const BufferRange *buffers);

    /// \brief Bind a range of a Buffer object to use for fetching shader
    /// uniform values.
    ///
    /// \param stage Shader stage owning the binding point to update.
    ///
    /// \param index Uniform buffer binding point number to update.
    /// Must be greater than or equal to zero.
    /// Must be less than 14 (the value of
    /// nvn::DeviceInfo::UNIFORM_BUFFER_BINDINGS_PER_STAGE).
    ///
    /// \param buffer GPU address of buffer object memory to bind.
    /// Must be a multiple of 256 (the value of
    /// nvn::DeviceInfo::UNIFORM_BUFFER_ALIGNMENT).
    ///
    /// \param size Size (in bytes) of the range of the Buffer object to
    /// bind.
    /// Must be less than or equal to 65536 (the value of
    /// nvn::DeviceInfo::MAX_UNIFORM_BUFFER_SIZE).
    inline void BindUniformBuffer(ShaderStage stage,
                                  int index,
                                  BufferAddress buffer,
                                  size_t size);

    /// \brief Bind one or more Buffer object ranges to use for fetching
    /// shader uniform values.
    ///
    /// \param stage Shader stage owning the binding point to update.
    ///
    /// \param first First uniform buffer binding point number to update.
    /// Must be less than 14 (the value of
    /// nvn::DeviceInfo::UNIFORM_BUFFER_BINDINGS_PER_STAGE).
    ///
    /// \param count Number of contiguous uniform buffer binding points to
    /// update.
    /// Must be greater than or equal to zero.
    /// Must be less than or equal to 14 (the value of
    /// nvn::DeviceInfo::UNIFORM_BUFFER_BINDINGS_PER_STAGE).
    ///
    /// \param buffers Array of buffer object ranges to bind.
    inline void BindUniformBuffers(ShaderStage stage,
                                   int first,
                                   int count,
                                   const BufferRange *buffers);

    /// \brief Bind a range of a Buffer object to use for capturing
    /// transformed vertex data.
    ///
    /// \param index Transform feedback buffer binding point number to
    /// update.
    /// Must be less than 4 (the value of
    /// nvn::DeviceInfo::TRANSFORM_FEEDBACK_BUFFER_BINDINGS).
    ///
    /// \param buffer GPU address of transform feedback buffer memory to
    /// bind.
    /// If specified as zero, no data should be captured for the updated
    /// binding point.
    /// Must be a multiple of 4 (the value of
    /// nvn::DeviceInfo::TRANSFORM_FEEDBACK_BUFFER_ALIGNMENT).
    ///
    /// \param size Size (in bytes) of the range of the Buffer object to
    /// bind.
    /// Must be a multiple of 4 (the value of
    /// nvn::DeviceInfo::TRANSFORM_FEEDBACK_BUFFER_ALIGNMENT).
    inline void BindTransformFeedbackBuffer(int index,
                                            BufferAddress buffer,
                                            size_t size);

    /// \brief Bind one or more Buffer object ranges to use for capturing
    /// transformed vertex data.
    ///
    /// \param first First transform feedback buffer binding point number
    /// to update.
    /// Must be less than 4 (the value of
    /// nvn::DeviceInfo::TRANSFORM_FEEDBACK_BUFFER_BINDINGS).
    ///
    /// \param count Number of contiguous transform feedback buffer binding
    /// points to update.
    /// Must be greater than or equal to zero.
    /// Must be less than or equal to 4 (the value of
    /// nvn::DeviceInfo::TRANSFORM_FEEDBACK_BUFFER_BINDINGS).
    ///
    /// \param buffers Array of buffer object ranges to bind.
    /// If specified as zero, no data should be captured for the updated
    /// binding point.
    inline void BindTransformFeedbackBuffers(int first,
                                             int count,
                                             const BufferRange *buffers);

    /// \brief Bind a range of a Buffer object to use for random-access
    /// reads and writes in shaders.
    ///
    /// \param stage Shader stage owning the binding point to update.
    ///
    /// \param index Shader storage buffer binding point number to update.
    /// Must be less than 16 (the value of
    /// nvn::DeviceInfo::SHADER_STORAGE_BUFFER_BINDINGS_PER_STAGE).
    ///
    /// \param buffer GPU address of buffer object memory to bind.
    ///
    /// \param size Size (in bytes) of the range of the Buffer object to
    /// bind.
    inline void BindStorageBuffer(ShaderStage stage,
                                  int index,
                                  BufferAddress buffer,
                                  size_t size);

    /// \brief Bind one or more Buffer object ranges to use for
    /// random-access reads and writes in shaders.
    ///
    /// \param stage Shader stage owning the binding point to update.
    ///
    /// \param first First shader storage buffer binding point number to
    /// update.
    /// Must be less than 16 (the value of
    /// nvn::DeviceInfo::SHADER_STORAGE_BUFFER_BINDINGS_PER_STAGE).
    ///
    /// \param count Number of contiguous shader storage buffer binding
    /// points to update.
    /// Must be greater than or equal to zero.
    /// Must be less than or equal to 16 (the value of
    /// nvn::DeviceInfo::SHADER_STORAGE_BUFFER_BINDINGS_PER_STAGE).
    ///
    /// \param buffers Array of buffer object ranges to bind.
    inline void BindStorageBuffers(ShaderStage stage,
                                   int first,
                                   int count,
                                   const BufferRange *buffers);

    /// \brief Bind a Texture/Sampler pair to use for texture mapping in
    /// shaders.
    ///
    /// \param stage Shader stage owning the binding point to update.
    ///
    /// \param index Texture/sampler binding point number to update.
    /// Must be less than 32 (the value of
    /// nvn::DeviceInfo::TEXTURE_BINDINGS_PER_STAGE).
    ///
    /// \param texture Handle for the pair of Texture and Sampler objects
    /// to bind.
    inline void BindTexture(ShaderStage stage,
                            int index,
                            TextureHandle texture);

    /// \brief Bind one or more Texture/Sampler pairs to use for texture
    /// mapping in shaders.
    ///
    /// \param stage Shader stage owning the binding points to update.
    ///
    /// \param first First texture/sampler binding point number to update.
    /// Must be less than 32 (the value of
    /// nvn::DeviceInfo::TEXTURE_BINDINGS_PER_STAGE).
    ///
    /// \param count Number of contiguous texture/sampler binding points to
    /// update.
    /// Must be less than or equal to 32 (the value of
    /// nvn::DeviceInfo::TEXTURE_BINDINGS_PER_STAGE).
    ///
    /// \param textures Array of handles for pairs of Texture and Sampler
    /// objects to bind.
    inline void BindTextures(ShaderStage stage,
                             int first,
                             int count,
                             const TextureHandle *textures);

    /// \brief Bind a Texture to use for image loads and stores in shaders.
    ///
    /// \param stage Shader stage owning the binding point to update.
    ///
    /// \param index Image binding point number to update.
    /// Must be less than 8 (the value of
    /// nvn::DeviceInfo::IMAGE_BINDINGS_PER_STAGE).
    ///
    /// \param image Image handle for the Texture object to bind.
    inline void BindImage(ShaderStage stage,
                          int index,
                          ImageHandle image);

    /// \brief Bind one or more Textures to use for image loads and stores
    /// in shaders.
    ///
    /// \param stage Shader stage owning the binding points to update.
    ///
    /// \param first First image binding point number to update.
    /// Must be less than 8 (the value of
    /// nvn::DeviceInfo::IMAGE_BINDINGS_PER_STAGE).
    ///
    /// \param count Number of contiguous image binding points to update.
    /// Must be less than or equal to 8 (the value of
    /// nvn::DeviceInfo::IMAGE_BINDINGS_PER_STAGE).
    ///
    /// \param images Array of handles for Texture objects to bind.
    inline void BindImages(ShaderStage stage,
                           int first,
                           int count,
                           const ImageHandle *images);

    /// \brief Specify the number of vertices in each patch when rendering
    /// patch primitives.
    ///
    /// \param size Number of vertices present in each patch.
    /// Must be greater than zero.
    /// Must be less than or equal to 32 (the value of
    /// nvn::DeviceInfo::MAX_PATCH_SIZE).
    inline void SetPatchSize(int size);

    /// \brief Specify default inner tessellation levels when rendering
    /// patch primitives.
    ///
    /// The default inner and outer tessellation levels are used when a
    /// tessellation evaluation shader is bound without a tessellation
    /// control shader.  When a tessellation control shader is bound,
    /// default levels are ignored.
    ///
    /// \note Tessellation levels are clamped to the range [1.0, 64.0], and
    /// may be further clamped according to the tessellation spacing.
    ///
    /// \param levels Two floating-point inner tessellation levels.
    inline void SetInnerTessellationLevels(const float *levels);

    /// \brief Specify default outer tessellation levels when rendering
    /// patch primitives.
    ///
    /// The default inner and outer tessellation levels are used when a
    /// tessellation evaluation shader is bound without a tessellation
    /// control shader.  When a tessellation control shader is bound,
    /// default levels are ignored.
    ///
    /// \note If any outer tessellation level is less than or equal to
    /// zero, a patch will be discarded, except for the fourth level when
    /// processing triangular patches.
    ///
    /// \note Tessellation levels greater than zero are clamped to the
    /// range [1.0, 64.0] and may be further clamped according to the
    /// tessellation spacing.
    ///
    /// \param levels Four floating-point outer tessellation levels.
    inline void SetOuterTessellationLevels(const float *levels);

    /// \brief Enable and disable primitive restart and specify a primitive
    /// restart index.
    ///
    /// When enabled, a primitive is restarted (with the same primitive
    /// type) whenever an index value matching the restart index value is
    /// found.
    ///
    /// \note The vertex index value must match the restart index exactly.
    /// For example, primitives rendered using the index type
    /// IndexType::UNSIGNED_BYTE wil not be restarted if the restart index
    /// is outside the range [0,255].
    ///
    /// \param enable Enable (TRUE) or disable (FALSE) primitive restart
    /// operations.
    ///
    /// \param index Index value used to indicate that a primitive should
    /// be restarted.
    inline void SetPrimitiveRestart(NVNboolean enable,
                                    int index);

    /// \brief Start a transform feedback (stream output) operation.
    ///
    /// \param buffer GPU address of buffer memory holding 32 bytes of
    /// control information used by the transform feedback operation.
    /// \warning Must point to valid buffer storage.
    /// Must be a multiple of 4 (the value of
    /// nvn::DeviceInfo::TRANSFORM_FEEDBACK_CONTROL_ALIGNMENT).
    inline void BeginTransformFeedback(BufferAddress buffer);

    /// \brief Complete a transform feedback (stream output) operation.
    ///
    /// \param buffer GPU address of buffer memory holding 32 bytes of
    /// control information used by the transform feedback operation.
    /// \warning Must point to valid buffer storage.
    /// Must be a multiple of 4 (the value of
    /// nvn::DeviceInfo::TRANSFORM_FEEDBACK_CONTROL_ALIGNMENT).
    inline void EndTransformFeedback(BufferAddress buffer);

    /// \brief Pause a transform feedback (stream output) operation.
    ///
    /// \param buffer GPU address of buffer memory holding 32 bytes of
    /// control information used by the transform feedback operation.
    /// The provided address may be zero, in which case, no control
    /// information will be written to memory.
    ///
    /// \note If an address of zero is used, the current state of the
    /// transform feedback operation is not saved to memory.  If any other
    /// transform feedback operation is performed before resuming, or if
    /// transform feedback is resumed using a non-zero address, this state
    /// is lost.
    /// Must be a multiple of 4 (the value of
    /// nvn::DeviceInfo::TRANSFORM_FEEDBACK_CONTROL_ALIGNMENT).
    inline void PauseTransformFeedback(BufferAddress buffer);

    /// \brief Resume a transform feedback (stream output) operation.
    ///
    /// \param buffer GPU address of buffer memory holding 32 bytes of
    /// control information used by the transform feedback operation.
    /// The provided address may be zero, in which case, no control
    /// information will be read from memory.
    ///
    /// \note If an address of zero is used, the state of the previously
    /// paused transform feedback operation is not restored from memory.
    /// If any other transform feedback operation is performed before
    /// resuming with a zero address, this state is lost.
    /// Must be a multiple of 4 (the value of
    /// nvn::DeviceInfo::TRANSFORM_FEEDBACK_CONTROL_ALIGNMENT).
    inline void ResumeTransformFeedback(BufferAddress buffer);

    /// \brief Draw primitives captured in a previous transform feedback
    /// (stream output) operation.
    ///
    /// \warning Applications must call CommandBuffer::Barrier with the
    /// BarrierBits::ORDER_INDIRECT_DATA bit set to ensure that the GPU
    /// doesn't fetch the indirect data before previous commands are
    /// finished writing the data.  Waiting via Queue::Finish,
    /// Queue::WaitSync, or CommandBuffer::WaitSync is not sufficient to
    /// ensure that GPU-produced indirect data are not fetched prematurely.
    ///
    /// \param mode Type of primitive to render.
    /// When rendering primitives captured by transform feedback, this
    /// parameter should typically be POINTS, LINES, or TRIANGLES, since
    /// transform feedback captures only independent primitives (even if
    /// the original geometry were provided as strips).
    ///
    /// \param buffer GPU address of buffer memory holding 32 bytes of
    /// control information used by the transform feedback operation.
    /// \warning Must point to valid buffer storage.
    /// Must be a multiple of 4 (the value of
    /// nvn::DeviceInfo::TRANSFORM_FEEDBACK_CONTROL_ALIGNMENT).
    inline void DrawTransformFeedback(DrawPrimitive mode,
                                      BufferAddress buffer);

    /// \brief Render a primitive with consecutive vertex indices.
    ///
    /// \param mode Type of the primitive to render.
    ///
    /// \param first Index of the first vertex in the primitive.
    ///
    /// \param count Number of vertices in the primitive.
    inline void DrawArrays(DrawPrimitive mode,
                           int first,
                           int count);

    /// \brief Render a primitive with vertex indices fetched from an index
    /// buffer.
    ///
    /// \param mode Type of the primitive to render.
    ///
    /// \param type Type of index values stored in the index buffer.
    ///
    /// \param count Number of vertices in the primitive.
    ///
    /// \param indexBuffer GPU address of buffer object memory used as the
    /// index buffer.
    inline void DrawElements(DrawPrimitive mode,
                             IndexType type,
                             int count,
                             BufferAddress indexBuffer);

    /// \brief Render a primitive with vertex indices fetched from an index
    /// buffer, with a base vertex value added to each fetched index.
    ///
    /// \param mode Type of the primitive to render.
    ///
    /// \param type Type of index values stored in the index buffer.
    ///
    /// \param count Number of vertices in the primitive.
    ///
    /// \param indexBuffer GPU address of buffer object memory used as the
    /// index buffer.
    ///
    /// \param baseVertex Base vertex value, added to each index fetched
    /// from the index buffer.
    inline void DrawElementsBaseVertex(DrawPrimitive mode,
                                       IndexType type,
                                       int count,
                                       BufferAddress indexBuffer,
                                       int baseVertex);

    /// \brief Render multiple copies (instances) of a primitive with
    /// consecutive vertex indices.
    ///
    /// \param mode Type of the primitive to render.
    ///
    /// \param first Index of the first vertex in the primitive.
    ///
    /// \param count Number of vertices in the primitive.
    ///
    /// \param baseInstance Instance number of the first primitive
    /// instance.
    ///
    /// \param instanceCount Number of primitive instances to render.
    inline void DrawArraysInstanced(DrawPrimitive mode,
                                    int first,
                                    int count,
                                    int baseInstance,
                                    int instanceCount);

    /// \brief Render multiple copies (instances) of a primitive with
    /// vertex indices fetched from an index buffer.
    ///
    /// \param mode Type of the primitive to render.
    ///
    /// \param type Type of index values stored in the index buffer.
    ///
    /// \param count Number of vertices in the primitive.
    ///
    /// \param indexBuffer GPU address of buffer object memory used as the
    /// index buffer.
    ///
    /// \param baseVertex Base vertex value, added to each index fetched
    /// from the index buffer.
    ///
    /// \param baseInstance Instance number of the first primitive
    /// instance.
    ///
    /// \param instanceCount Number of primitive instances to render.
    inline void DrawElementsInstanced(DrawPrimitive mode,
                                      IndexType type,
                                      int count,
                                      BufferAddress indexBuffer,
                                      int baseVertex,
                                      int baseInstance,
                                      int instanceCount);

    /// \brief Render primitives with consecutive vertex indices, using
    /// draw parameters fetched from an indirect data buffer.
    ///
    /// The parameters of the draw are stored in Buffer object memory using
    /// the DrawArraysIndirectData structure.
    ///
    /// \warning If the indirect data are produced by previous commands
    /// executed on the GPU, applications must call CommandBuffer::Barrier
    /// with the BarrierBits::ORDER_INDIRECT_DATA bit set to ensure that
    /// the GPU doesn't fetch the indirect data before previous commands
    /// are finished writing the data.  Waiting via Queue::Finish,
    /// Queue::WaitSync, or CommandBuffer::WaitSync is not sufficient to
    /// ensure that GPU-produced indirect data are not fetched prematurely.
    ///
    /// \param mode Type of the primitive to render.
    ///
    /// \param indirectBuffer GPU address of buffer object memory holding
    /// the indirect draw data.
    /// Must be a multiple of 4 (the value of
    /// nvn::DeviceInfo::INDIRECT_DRAW_ALIGNMENT).
    inline void DrawArraysIndirect(DrawPrimitive mode,
                                   BufferAddress indirectBuffer);

    /// \brief Render primitives with vertex indices fetched from an index
    /// buffer, using draw parameters fetched from an indirect data buffer.
    ///
    /// The parameters of the draw are stored in Buffer object memory using
    /// the DrawElementsIndirectData structure.
    ///
    /// \warning If the indirect data are produced by previous commands
    /// executed on the GPU, applications must call CommandBuffer::Barrier
    /// with the BarrierBits::ORDER_INDIRECT_DATA bit set to ensure that
    /// the GPU doesn't fetch the indirect data before previous commands
    /// are finished writing the data.  Waiting via Queue::Finish,
    /// Queue::WaitSync, or CommandBuffer::WaitSync is not sufficient to
    /// ensure that GPU-produced indirect data are not fetched prematurely.
    ///
    /// \param mode Type of the primitive to render.
    ///
    /// \param type Type of index values stored in the index buffer.
    ///
    /// \param indexBuffer GPU address of buffer object memory used as the
    /// index buffer.
    ///
    /// \param indirectBuffer GPU address of buffer object memory holding
    /// the indirect draw data.
    /// Must be a multiple of 4 (the value of
    /// nvn::DeviceInfo::INDIRECT_DRAW_ALIGNMENT).
    inline void DrawElementsIndirect(DrawPrimitive mode,
                                     IndexType type,
                                     BufferAddress indexBuffer,
                                     BufferAddress indirectBuffer);

    /// \brief Render multiple primitives with vertices fetched from a
    /// vertex buffer, using draw parameters fetched from an indirect data
    /// buffer and the number of primitives fetched from a parameter
    /// buffer.
    ///
    /// The parameters of the draw are stored in Buffer object memory using
    /// the DrawArraysIndirectData structure.
    ///
    /// \warning If the indirect data or the draw count data are produced
    /// by previous commands executed on the GPU, applications must call
    /// CommandBuffer::Barrier with the BarrierBits::ORDER_INDIRECT_DATA
    /// bit set to ensure that the GPU doesn't fetch the indirect data
    /// before previous commands are finished writing the data.  Waiting
    /// via Queue::Finish, Queue::WaitSync, or CommandBuffer::WaitSync is
    /// not sufficient to ensure that GPU-produced indirect data are not
    /// fetched prematurely.
    ///
    /// \param mode Type of the primitive to render.
    ///
    /// \param indirectBuffer GPU address of buffer object memory holding
    /// the indirect draw data.
    /// Must be a multiple of 4 (the value of
    /// nvn::DeviceInfo::INDIRECT_DRAW_ALIGNMENT).
    ///
    /// \param parameterBuffer GPU address of buffer object memory holding
    /// the draw count.
    /// Must be a multiple of 4 (the value of
    /// nvn::DeviceInfo::INDIRECT_DRAW_ALIGNMENT).
    ///
    /// \param maxDrawCount Specifies the maximum number of draws that are
    /// allowed.
    /// If the draw count in GPU memory pointed to by _parameterBuffer_ is
    /// greater than _maxDrawCount_, only the first _maxDrawCount_
    /// primitives will be drawn.
    ///
    /// \param stride Stride (in bytes) between elements in
    /// _indirectBuffer_.
    inline void MultiDrawArraysIndirectCount(DrawPrimitive mode,
                                             BufferAddress indirectBuffer,
                                             BufferAddress parameterBuffer,
                                             int maxDrawCount,
                                             ptrdiff_t stride);

    /// \brief Render multiple primitives with vertex indices fetched from
    /// an index buffer, using draw parameters fetched from an indirect
    /// data buffer and the number of primitives fetched from a parameter
    /// buffer.
    ///
    /// The parameters of the draw are stored in buffer object memory using
    /// the DrawElementsIndirectData structure.
    ///
    /// \warning If the indirect data or the draw count data are produced
    /// by previous commands executed on the GPU, applications must call
    /// CommandBuffer::Barrier with the BarrierBits::ORDER_INDIRECT_DATA
    /// bit set to ensure that the GPU doesn't fetch the indirect data
    /// before previous commands are finished writing the data.  Waiting
    /// via Queue::Finish, Queue::WaitSync, or CommandBuffer::WaitSync is
    /// not sufficient to ensure that GPU-produced indirect data are not
    /// fetched prematurely.
    ///
    /// \param mode Type of the primitive to render.
    ///
    /// \param type Type of index values stored in the index buffer.
    ///
    /// \param indexBuffer GPU address of buffer object memory used as the
    /// index buffer.
    ///
    /// \param indirectBuffer GPU address of buffer object memory holding
    /// the indirect draw data.
    /// Must be a multiple of 4 (the value of
    /// nvn::DeviceInfo::INDIRECT_DRAW_ALIGNMENT).
    ///
    /// \param parameterBuffer GPU address of buffer object memory holding
    /// the draw count.
    /// Must be a multiple of 4 (the value of
    /// nvn::DeviceInfo::INDIRECT_DRAW_ALIGNMENT).
    ///
    /// \param maxDrawCount Specifies the maximum number of draws that are
    /// allowed.
    /// If the draw count in GPU memory pointed to by _parameterBuffer_ is
    /// greater than _maxDrawCount_, only the first _maxDrawCount_
    /// primitives will be drawn.
    ///
    /// \param stride Stride (in bytes) between elements in
    /// _indirectBuffer_.
    inline void MultiDrawElementsIndirectCount(DrawPrimitive mode,
                                               IndexType type,
                                               BufferAddress indexBuffer,
                                               BufferAddress indirectBuffer,
                                               BufferAddress parameterBuffer,
                                               int maxDrawCount,
                                               ptrdiff_t stride);

    /// \brief Clear a color buffer using floating-point clear color
    /// values.
    ///
    /// This command should be used only to clear color buffers with
    /// floating-point or _normalized_ signed or unsigned integer
    /// components.  When clearing color buffers with unnormalized integer
    /// components (with formats ending in "I" or "UI"), the values in
    /// `color` will be re-interpreted as integers.  For example, clearing
    /// an integer color buffer to the value 1.0 will instead clear to
    /// 0x3F800000, the IEEE-754 encoding of 1.0.
    ///
    /// \warning When using the Windows reference implementation on Pascal
    /// and newer GPUs, calling CommandBuffer::Barrier with the barrier
    /// BarrierBits::ORDER_FRAGMENTS is not sufficient to ensure that
    /// previous rendering using the color buffer as a texture completes
    /// before the color buffer clear starts. In such cases, a heavier
    /// barrier like BarrierBits::ORDER_PRIMITIVES is necessary to ensure
    /// that the clear doesn't corrupt rendering commands submitted prior
    /// to the barrier.
    ///
    /// \param index Index of the color target to clear.
    /// Must be less than 8 (the value of
    /// nvn::DeviceInfo::COLOR_BUFFER_BINDINGS).
    ///
    /// \param color Array of four floating-point (RGBA) clear color
    /// values.
    ///
    /// \param mask Bitfield indicating which color channels should be
    /// cleared. See nvn::ClearColorMask.
    inline void ClearColor(int index,
                           const float *color,
                           ClearColorMask mask);

    /// \brief Clear a color buffer using signed integer clear color
    /// values.
    ///
    /// This command should be used only to clear color buffers with
    /// _unnormalized_ signed integer components (with formats ending in
    /// "I").  When clearing color buffers with floating-point or
    /// normalized integer components, the values in `color` will be
    /// re-interpreted as floating-point values.  For example, clearing a
    /// floating-point color buffer to the value 0x3F800000 will instead
    /// clear to 1.0 (the IEEE-754 floating-point equivalent). When
    /// clearing color buffers with unsigned integer components, the signed
    /// clear color will be re-interpreted as unsigned.
    ///
    /// \warning When using the Windows reference implementation on Pascal
    /// and newer GPUs, calling CommandBuffer::Barrier with the barrier
    /// BarrierBits::ORDER_FRAGMENTS is not sufficient to ensure that
    /// previous rendering using the color buffer as a texture completes
    /// before the color buffer clear starts. In such cases, a heavier
    /// barrier like BarrierBits::ORDER_PRIMITIVES is necessary to ensure
    /// that the clear doesn't corrupt rendering commands submitted prior
    /// to the barrier.
    ///
    /// \param index Index of the color target to clear.
    /// Must be less than 8 (the value of
    /// nvn::DeviceInfo::COLOR_BUFFER_BINDINGS).
    ///
    /// \param color Array of four signed integer (RGBA) clear color
    /// values.
    ///
    /// \param mask Bitfield indicating which color channels should be
    /// cleared. See nvn::ClearColorMask.
    inline void ClearColori(int index,
                            const int *color,
                            ClearColorMask mask);

    /// \brief Clear a color buffer using unsigned integer clear color
    /// values.
    ///
    /// This command should be used only to clear color buffers with
    /// _unnormalized_ unsigned integer components (with formats ending in
    /// "UI").  When clearing color buffers with floating-point or
    /// normalized integer components, the values in `color` will be
    /// re-interpreted as floating-point values.  For example, clearing a
    /// floating-point color buffer to the value 0x3F800000 will instead
    /// clear to 1.0 (the IEEE-754 floating-point equivalent). When
    /// clearing color buffers with signed integer components, the unsigned
    /// clear color will be re-interpreted as signed.
    ///
    /// \warning When using the Windows reference implementation on Pascal
    /// and newer GPUs, calling CommandBuffer::Barrier with the barrier
    /// BarrierBits::ORDER_FRAGMENTS is not sufficient to ensure that
    /// previous rendering using the color buffer as a texture completes
    /// before the color buffer clear starts. In such cases, a heavier
    /// barrier like BarrierBits::ORDER_PRIMITIVES is necessary to ensure
    /// that the clear doesn't corrupt rendering commands submitted prior
    /// to the barrier.
    ///
    /// \param index Index of the color target to clear.
    /// Must be less than 8 (the value of
    /// nvn::DeviceInfo::COLOR_BUFFER_BINDINGS).
    ///
    /// \param color Array of four unsigned integer (RGBA) clear color
    /// values.
    ///
    /// \param mask Bitfield indicating which color channels should be
    /// cleared. See nvn::ClearColorMask.
    inline void ClearColorui(int index,
                             const uint32_t *color,
                             ClearColorMask mask);

    /// \brief Clear a depth/stencil buffer.
    ///
    /// This command clears depth and stencil buffers of all formats.
    ///
    /// \warning On NX and the Windows reference implementation, calling
    /// CommandBuffer::Barrier with the barrier
    /// BarrierBits::ORDER_FRAGMENTS is not sufficient to ensure that
    /// previous rendering using the depth/stencil buffer as a texture
    /// completes before the depth/stencil buffer clear starts. In such
    /// cases, a heavier barrier like BarrierBits::ORDER_PRIMITIVES is
    /// necessary to ensure that the clear does not corrupt rendering
    /// commands submitted prior to the barrier.
    ///
    /// \param depthValue Floating-point depth clear value (in the range
    /// [0.0, 1.0]).
    ///
    /// \param depthMask Enable (TRUE) or disable (FALSE) clearing of the
    /// depth channel.
    ///
    /// \param stencilValue Stencil clear value.
    ///
    /// \param stencilMask Mask indicating the bits of the stencil buffer
    /// to clear.
    inline void ClearDepthStencil(float depthValue,
                                  NVNboolean depthMask,
                                  int stencilValue,
                                  int stencilMask);

    /// \brief Dispatch a new grid of compute shader work groups for
    /// processing.
    ///
    /// Each compute shader dispatch spawns a three-dimensional array of
    /// work groups for processing.  Each work group is processed
    /// independently, with a three-dimensional array of threads whose size
    /// is specified in the compute shader.
    ///
    /// \param groupsX The number of compute work groups in the X
    /// dimension.
    /// Must be less than or equal to 65535 (the value of
    /// nvn::DeviceInfo::MAX_COMPUTE_DISPATCH_WORK_GROUPS_X).
    ///
    /// \param groupsY The number of compute work groups in the Y
    /// dimension.
    /// Must be less than or equal to 65535 (the value of
    /// nvn::DeviceInfo::MAX_COMPUTE_DISPATCH_WORK_GROUPS_Y).
    ///
    /// \param groupsZ The number of compute work groups in the Z
    /// dimension.
    /// Must be less than or equal to 65535 (the value of
    /// nvn::DeviceInfo::MAX_COMPUTE_DISPATCH_WORK_GROUPS_Z).
    inline void DispatchCompute(int groupsX,
                                int groupsY,
                                int groupsZ);

    /// \brief Dispatch a new grid of compute shader work groups for
    /// processing.
    ///
    /// Each compute shader dispatch spawns a three-dimensional array of
    /// work groups for processing.  Each work group is processed
    /// independently, with a three-dimensional array of threads whose size
    /// is specified in the compute shader.  The number of compute
    /// workgroups in each dimension is read from buffer memory.
    ///
    /// \warning If the indirect data are produced by previous commands
    /// executed on the GPU, applications must call CommandBuffer::Barrier
    /// with the BarrierBits::ORDER_INDIRECT_DATA bit set to ensure that
    /// the GPU doesn't fetch the indirect data before previous commands
    /// are finished writing the data.  Waiting via Queue::Finish,
    /// Queue::WaitSync, or CommandBuffer::WaitSync is not sufficient to
    /// ensure that GPU-produced indirect data are not fetched prematurely.
    ///
    /// \param indirectBuffer GPU address of buffer object memory holding
    /// the indirect dispatch data.
    /// Must be a multiple of 4 (the value of
    /// nvn::DeviceInfo::INDIRECT_DISPATCH_ALIGNMENT).
    inline void DispatchComputeIndirect(BufferAddress indirectBuffer);

    /// \brief Specify the rectangle for viewport 0.
    ///
    /// \param x Smallest X coordinate of the viewport rectangle.
    ///
    /// \param y Smallest Y coordinate of the viewport rectangle.
    ///
    /// \param w Width of the viewport rectangle.
    ///
    /// \param h Height of the viewport rectangle.
    inline void SetViewport(int x,
                            int y,
                            int w,
                            int h);

    /// \brief Specify one or more viewport rectangles.
    ///
    /// \param first Index of the first viewport to modify.
    ///
    /// \param count Number of contiguous viewports to set.
    ///
    /// \param ranges An array of values to use for the viewports.
    /// The array must contain 4 * _count_ values corresponding to the
    /// range of viewports specified by _first_ and _count_. Each quartet
    /// of values corresponds to, in order: the smallest X coordinate of
    /// the viewport, the smallest Y coordinate of the viewport, the width
    /// of the viewport, and the height of the viewport.
    inline void SetViewports(int first,
                             int count,
                             const float *ranges);

    /// \brief Specify a set of viewport swizzle selectors for one or more
    /// viewport rectangles.
    ///
    /// As part of the viewport transformation, each vertex is transformed
    /// by the viewport swizzle operation before the vertex is transformed
    /// to screen space.  The viewport swizzle produces a four-component
    /// swizzled position, where each component is produced by selecting
    /// and optionally negating one of the four compenents of the original
    /// vertex position.  This command specifies sets of four swizzle
    /// selectors used to produce the transformed position.  The default
    /// swizzles for each viewport are POSITIVE_X, POSITIVE_Y, POSITIVE_Z,
    /// and POSITIVE_W, which specify no change to the incoming position.
    ///
    /// \note There is a separate set of four viewport swizzle values for
    /// each viewport.  A primitive broadcast to multiple viewports will
    /// have a separate swizzle applied for each viewport.
    ///
    /// \note Viewport swizzles rasterization is only supported on NX and
    /// second-generation Maxwell and later GPUs. Query the
    /// SUPPORTS_VIEWPORT_SWIZZLE device property to determine if this
    /// command is supported.
    ///
    /// \param first Index of the first viewport to modify.
    ///
    /// \param count Number of consecutive viewports to update.
    ///
    /// \param swizzles An array of swizzle values to use for the
    /// viewports.
    /// This array must contain 4 * _count_ values corresponding to the
    /// range of viewports specified by _first_ and _count_.  Each set of
    /// four values identifies the input position component used for the X,
    /// Y, Z, and W components, respectively, of the position vector
    /// produced by the viewport swizzle operation.
    inline void SetViewportSwizzles(int first,
                                    int count,
                                    const ViewportSwizzle *swizzles);

    /// \brief Specify the scissor rectangle used for clipping primitives
    /// and clears.
    ///
    /// \param x Smallest X coordinate of the scissor rectangle.
    ///
    /// \param y Smallest Y coordinate of the scissor rectangle.
    ///
    /// \param w Width of the scissor rectangle.
    ///
    /// \param h Height of the scissor rectangle.
    inline void SetScissor(int x,
                           int y,
                           int w,
                           int h);

    /// \brief Specify one or more scissor rectangles.
    ///
    /// \param first Index of the first scissor rectangle to modify.
    ///
    /// \param count Number of contiguous scissor rectangles to set.
    ///
    /// \param rects An array of values to use for the scissor rectangles.
    /// The array must contain 4 * _count_ values corresponding to the
    /// range of scissor rectangles specified by _first_ and _count_. Each
    /// quartet of values corresponds to, in order: the smallest X
    /// coordinate of the rectangle, the smallest Y coordinate of the
    /// rectangle, the width of the rectangle, and the height of the
    /// rectangle.
    inline void SetScissors(int first,
                            int count,
                            const int *rects);

    /// \brief Set the range of depth values for rendered primitives.
    ///
    /// \param n Depth value corresponding to the near clip plane.
    ///
    /// \param f Depth value corresponding to the far clip plane.
    inline void SetDepthRange(float n,
                              float f);

    /// \brief Control the operation of the depth bounds test.
    ///
    /// This command lets applications enable or disable the depth bounds
    /// test.  When enabled, the depth bounds test will discard any
    /// fragments where the stored Z value in the depth buffer is outside
    /// the range [_n_, _f_].  By default, the depth bounds test is
    /// disabled.
    ///
    /// \warning When the depth bounds test is enabled, _n_ must be less
    /// than or equal to _f_.
    ///
    /// \param enable Enable or disable the depth bounds test.
    ///
    /// \param n Minimum value of the depth bounds test range.
    ///
    /// \param f Maximum value of the depth bounds test range.
    inline void SetDepthBounds(NVNboolean enable,
                               float n,
                               float f);

    /// \brief Sets multiple ranges of depth values for rendered
    /// primitives.
    ///
    /// \param first Index of the first depth range to modify.
    ///
    /// \param count Number of contiguous depth ranges to set.
    ///
    /// \param ranges An array of values to use for the depth ranges. It
    /// must contain _count_ pairs of values corresponding to the depth
    /// ranges specified by _first_ and _count_.
    inline void SetDepthRanges(int first,
                               int count,
                               const float *ranges);

    /// \brief Perform a specified operation related to tiled caching.
    ///
    /// \param action Tiled caching operation to perform.
    /// Actions include enabling, disabling, or flushing the tiled cache
    /// binner.
    inline void SetTiledCacheAction(TiledCacheAction action);

    /// \brief Sets the tile size used for tiled caching.
    ///
    /// If the tile size has changed, the hardware will automatically flush
    /// the tiled cache binner.  The tile size is clamped to [16, 16384] in
    /// either dimension.
    ///
    /// \param tileWidth Sets the tile width, in pixels.
    ///
    /// \param tileHeight Sets the tile height, in pixels.
    inline void SetTiledCacheTileSize(int tileWidth,
                                      int tileHeight);

    /// \brief Bind a separate Texture to use for texture mapping in
    /// shaders.
    ///
    /// This command binds a separate texture handle (without sampler
    /// information) for use with shaders using separate texture types.
    /// These bindings are not used for variables with combined sampler
    /// types (such as "sampler2D" in GLSL shaders).
    ///
    /// \param stage Shader stage owning the separate Texture binding point
    /// to update.
    ///
    /// \param index Separate Texture binding point number to update.
    /// Must be less than 128 (the value of
    /// nvn::DeviceInfo::SEPARATE_TEXTURE_BINDINGS_PER_STAGE).
    ///
    /// \param textureHandle Separate Texture handle for the separate
    /// Texture object to bind.
    inline void BindSeparateTexture(ShaderStage stage,
                                    int index,
                                    SeparateTextureHandle textureHandle);

    /// \brief Bind a separate Texture to use for texture mapping in
    /// shaders. Deprecated in favor of CommandBuffer::BindSeparateTexture.
    ///
    /// This command binds a separate texture handle (without sampler
    /// information) for use with shaders using separate texture types.
    /// These bindings are not used for variables with combined sampler
    /// types (such as "sampler2D" in GLSL shaders).
    ///
    /// \param stage Shader stage owning the separate Texture binding point
    /// to update.
    ///
    /// \param index Separate Texture binding point number to update.
    /// Must be less than 128 (the value of
    /// nvn::DeviceInfo::SEPARATE_TEXTURE_BINDINGS_PER_STAGE).
    ///
    /// \param textureHandle Separate Texture handle for the separate
    /// Texture object to bind.
    NVN_PRE_DEPRECATED
    inline void bindSeparateTexture(ShaderStage stage,
                                    int index,
                                    SeparateTextureHandle textureHandle)
    NVN_POST_DEPRECATED;

    /// \brief Bind a separate Sampler to use for texture mapping in
    /// shaders.
    ///
    /// This command binds a separate sampler handle (without texture
    /// information) for use with shaders using separate sampler types.
    /// These bindings are not used for variables with combined sampler
    /// types (such as "sampler2D" in GLSL shaders).
    ///
    /// \param stage Shader stage owning the separate Sampler binding point
    /// to update.
    ///
    /// \param index Separate Sampler binding point number to update.
    /// Must be less than 32 (the value of
    /// nvn::DeviceInfo::SEPARATE_SAMPLER_BINDINGS_PER_STAGE).
    ///
    /// \param samplerHandle Separate Sampler handle for the separate
    /// Sampler object to bind.
    inline void BindSeparateSampler(ShaderStage stage,
                                    int index,
                                    SeparateSamplerHandle samplerHandle);

    /// \brief Bind a separate Sampler to use for texture mapping in
    /// shaders. Deprecated in favor of CommandBuffer::BindSeparateSampler.
    ///
    /// This command binds a separate sampler handle (without texture
    /// information) for use with shaders using separate sampler types.
    /// These bindings are not used for variables with combined sampler
    /// types (such as "sampler2D" in GLSL shaders).
    ///
    /// \param stage Shader stage owning the separate Sampler binding point
    /// to update.
    ///
    /// \param index Separate Sampler binding point number to update.
    /// Must be less than 32 (the value of
    /// nvn::DeviceInfo::SEPARATE_SAMPLER_BINDINGS_PER_STAGE).
    ///
    /// \param samplerHandle Separate Sampler handle for the separate
    /// Sampler object to bind.
    NVN_PRE_DEPRECATED
    inline void bindSeparateSampler(ShaderStage stage,
                                    int index,
                                    SeparateSamplerHandle samplerHandle)
    NVN_POST_DEPRECATED;

    /// \brief Bind one or more separate textures to use for texture
    /// mapping in shaders.
    ///
    /// This command binds one or more separate texture handles (without
    /// sampler information) for use with shaders using separate texture
    /// types.  These bindings are not used for variables with combined
    /// sampler types (such as "sampler2D" in GLSL shaders).  This command
    /// is equivalent to making multiple calls to BindSeparateTexture.
    ///
    /// \param stage Shader stage owning the binding points to update.
    ///
    /// \param first First separate texture binding point number to update.
    /// Must be less than 128 (the value of
    /// nvn::DeviceInfo::SEPARATE_TEXTURE_BINDINGS_PER_STAGE).
    ///
    /// \param count Number of contiguous separate texture binding points
    /// to update.
    /// Must be less than or equal to 128 (the value of
    /// nvn::DeviceInfo::SEPARATE_TEXTURE_BINDINGS_PER_STAGE).
    ///
    /// \param textureHandles Array of handles of separate Texture objects
    /// to bind.
    inline void BindSeparateTextures(ShaderStage stage,
                                     int first,
                                     int count,
                                     const SeparateTextureHandle *textureHandles);

    /// \brief Bind one or more Samplers to use for texture mapping in
    /// shaders.
    ///
    /// This command binds one or more separate sampler handles (without
    /// texture information) for use with shaders using separate sampler
    /// types.  These bindings are not used for variables with combined
    /// sampler types (such as "sampler2D" in GLSL shaders).  This command
    /// is equivalent to making multiple calls to BindSeparateSampler.
    ///
    /// \param stage Shader stage owning the binding points to update.
    ///
    /// \param first First separate sampler binding point number to update.
    /// Must be less than 32 (the value of
    /// nvn::DeviceInfo::SEPARATE_SAMPLER_BINDINGS_PER_STAGE).
    ///
    /// \param count Number of contiguous separate sampler binding points
    /// to update.
    /// Must be less than or equal to 32 (the value of
    /// nvn::DeviceInfo::SEPARATE_SAMPLER_BINDINGS_PER_STAGE).
    ///
    /// \param samplerHandles Array of handles of separate Sampler objects
    /// to bind.
    inline void BindSeparateSamplers(ShaderStage stage,
                                     int first,
                                     int count,
                                     const SeparateSamplerHandle *samplerHandles);

    /// \brief Set a mask that will be applied to the stencil index and
    /// reference values prior to performing the stencil test.
    ///
    /// There are separate stencil value masks for front- and back-facing
    /// primitives. This function can update the value of either or both
    /// masks.  If stencil testing is disabled, the stencil value mask has
    /// no effect.  The default value for both front and back masks is 0xFF
    /// (all bits set).
    ///
    /// \note In API versions prior to 53.8, the default mask was
    /// unintentionally set to zero.
    ///
    /// \param faces Specifies the face(s) (front and/or back) using the
    /// new stencil value mask.
    ///
    /// \param mask Mask to be applied to stencil index and reference
    /// values prior to the stencil test
    inline void SetStencilValueMask(Face faces,
                                    int mask);

    /// \brief Specify a mask to apply to stencil values written by the
    /// stencil test.
    ///
    /// Bits in the existing stencil index corresponding to zero bits in
    /// the stencil mask will not be updated by the stencil test. There are
    /// separate stencil masks for front- and back-facing primitives. This
    /// function can update the value of either or both masks.  If stencil
    /// testing is disabled, the stencil mask has no effect.  The default
    /// value for both front and back masks is 0xFF (all bits set).
    ///
    /// \note In API versions prior to 53.8, the default mask was
    /// unintentionally set to zero.
    ///
    /// \param faces Specifies the face(s) (front and/or back) using the
    /// new stencil mask.
    ///
    /// \param mask Mask to apply to stencil values written by the stencil
    /// test.
    inline void SetStencilMask(Face faces,
                               int mask);

    /// \brief Specify a stencil reference value used by the stencil test.
    ///
    /// There are separate reference values for front- and back-facing
    /// primitives. This function can update the value of either or both
    /// values.  If stencil testing is disabled, the stencil reference
    /// value has no effect.  The default reference value for both front
    /// and back is zero.
    ///
    /// \param faces Specifies the face(s) (front and/or back) using the
    /// new stencil reference value.
    ///
    /// \param ref Stencil reference value used by the stencil test.
    inline void SetStencilRef(Face faces,
                              int ref);

    /// \brief Specifies a constant color that may be used by blending
    /// functions.
    ///
    /// \param blendColor Array of four floating-point (RGBA) blend color
    /// values.
    inline void SetBlendColor(const float *blendColor);

    /// \brief Specifies the size (in pixels) of point primitives to
    /// render.
    ///
    /// If the last enabled vertex, tessellation control, tessellation
    /// evaluation, or geometry shader writes to gl_PointSize, this value
    /// is ignored and point size written in that shader will be used.
    ///
    /// \param pointSize Size (in pixels) of point primitives to render.
    inline void SetPointSize(float pointSize);

    /// \brief Specifies the width (in pixels) of line primitives to
    /// render.
    ///
    /// \param lineWidth Width (in pixels) of line primitives to render.
    inline void SetLineWidth(float lineWidth);

    /// \brief Specifies values used to compute a depth offset value when
    /// polygon offset is enabled.
    ///
    /// The offset is computed by multiplying the maximum depth slope of
    /// the primitive [sqrt((dz/dx)^2 + (dz/dy)^2)] by _factor_, adding in
    /// the minimum resolvable difference in depth values multiplied by
    /// _units_, and clamping the result as described in the documentation
    /// for _clamp_.
    ///
    /// \param factor Value multiplied by the maximum depth slope when
    /// computing offset.
    ///
    /// \param units Value multiplied by the minimum resolvable depth
    /// difference when computing offset.
    ///
    /// \param clamp Value used to clamp the computed offset.
    /// If `clamp` is positive, the computed depth offset will be clamped
    /// to a value less than or equal to `clamp`. If `clamp` is negative,
    /// the computed depth offset will be clamped to a value greater than
    /// or equal to `clamp`. If `clamp` is zero, the computed depth offset
    /// will not be clamped.
    inline void SetPolygonOffsetClamp(float factor,
                                      float units,
                                      float clamp);

    /// \brief Specifies a reference value to use for alpha testing.
    ///
    /// \param ref Reference value for the alpha test.
    inline void SetAlphaRef(float ref);

    /// \brief Specifies a mask of samples that may be covered by rendered
    /// primitives.
    ///
    /// If bit N is zero in the provided mask, sample N will never be
    /// considered to be covered when rasterizing primitives.
    ///
    /// The default sample mask is ~0 (all bits enabled).
    ///
    /// \param mask Mask of samples that may be covered by rendered
    /// primitives.
    inline void SetSampleMask(int mask);

    /// \brief Enable or disable rasterizer discard.
    ///
    /// \param discard True if primitives should be discarded; false
    /// (default) otherwise.
    inline void SetRasterizerDiscard(NVNboolean discard);

    /// \brief Enable/disable depth clamping of rendered primitives.
    ///
    /// When enabled, primitives are not clipped to the near and far clip
    /// planes.  Instead, primitives are rasterized without clipping in Z,
    /// and interpolated Z values are clamped to the extents of the depth
    /// range.
    ///
    /// \param clamp TRUE if depth clamping should be enabled; FALSE
    /// otherwise.
    inline void SetDepthClamp(NVNboolean clamp);

    /// \brief Enable/disable conservative rasterization of rendered
    /// primitives.
    ///
    /// Conservative rasterization is only supported on second-generation
    /// Maxwell and later GPUs. Query the SUPPORTS_CONSERVATIVE_RASTER
    /// device property to determine support.
    ///
    /// By default, conservative rasterization is disabled.
    ///
    /// \param enable TRUE if conservative rasterization should be enabled;
    /// FALSE, otherwise.
    inline void SetConservativeRasterEnable(NVNboolean enable);

    /// \brief Specifies the degree of primitive dilation when conservative
    /// rasterization is enabled.
    ///
    /// This provides a mechanism for "over-conservative" rasterization by
    /// dilating (expanding) primitives prior to rasterization. *dilate* is
    /// automatically clamped to the range [0.0, 0.75] and rounded up to a
    /// multiple of 0.25. When *dilate* is 0.0, ordinary conservative
    /// rasterization is performed. When dilate is greater than 0.0,
    /// rasterized primitives are dilated by *dilate* pixels. If
    /// conservative rasterization is disabled, dilation has no effect on
    /// rasterization. This function is only supported on second-generation
    /// Maxwell and later GPUs. Query the SUPPORTS_CONSERVATIVE_RASTER
    /// device property to determine support.
    ///
    /// By default, conservative rasterization is disabled.
    ///
    /// \warning Changing the dilation setting requires idling the GPU and
    /// results in reduced performance. Applications should avoid changing
    /// the dilation factor frequently.
    ///
    /// \param dilate The fraction of a pixel by which to dilate rasterized
    /// primitives.
    inline void SetConservativeRasterDilate(float dilate);

    /// \brief Specifies the number of additional bits of subpixel
    /// precision to be used for primitive rasterization when conservative
    /// rasterization is enabled.
    ///
    /// When vertices are transformed to window coordinates, they are
    /// snapped to fixed-point positions. By default, the number of bits of
    /// fractional precision within a pixel is given by the device property
    /// SUBPIXEL_BITS. This function enables applications to increase the
    /// subpixel precision for snapped vertex positions. If conservative
    /// rasterization is disabled, additional bias bits have no effect on
    /// rasterization.
    ///
    /// Additional subpixel precision allows for consistent conservative
    /// rasterization when rendering the same geometry at different
    /// resolutions. Rasterizing a scene at 64x64 with 3 extra subpixel
    /// bits in X and Y will have the same snapping behavior as rasterizing
    /// the same scene at 512x512 with no extra subpixel bits.
    ///
    /// By default, the subpixel precision bias is 0.
    ///
    /// \param xBits The number of additional bits of subpixel precision in
    /// the horizontal direction.
    /// Must be less than or equal to 8 (the value of
    /// nvn::DeviceInfo::MAX_SUBPIXEL_BIAS_BITS).
    ///
    /// \param yBits The number of additional bits of subpixel precision in
    /// the vertical direction.
    /// Must be less than or equal to 8 (the value of
    /// nvn::DeviceInfo::MAX_SUBPIXEL_BIAS_BITS).
    inline void SetSubpixelPrecisionBias(int xBits,
                                         int yBits);

    /// \brief Copy texture data from buffer memory into a region of a
    /// Texture object.
    ///
    /// \param src Address of the buffer holding the source data for the
    /// copy.
    ///
    /// \param dstTexture Texture object used as the destination for the
    /// copy.
    ///
    /// \param dstView View of the texture object used as the destination
    /// for the copy.
    /// A texture view, if specified, can be used to override properties of
    /// the corresponding texture _dstTexture_. This parameter identifies a
    /// specific mipmap level, set of layers or alternate format in the
    /// specified texture. Other fields, such as swizzle, are ignored. If
    /// NULL is specified, the base level of the texture _dstTexture_ will
    /// be used as-is.
    ///
    /// \param dstRegion Region of the destination texture to update for
    /// the copy.
    ///
    /// \param flags Specifies options used when performing the copy. See
    /// nvn::CopyFlags.
    inline void CopyBufferToTexture(BufferAddress src,
                                    const Texture *dstTexture,
                                    const TextureView *dstView,
                                    const CopyRegion *dstRegion,
                                    CopyFlags flags);

    /// \brief Copy texture data from a region of a Texture object into
    /// buffer memory.
    ///
    /// \param srcTexture Texture object used as the source for the copy.
    ///
    /// \param srcView View of the texture object used as the source for
    /// the copy.
    /// A texture view, if specified, can be used to override properties of
    /// the corresponding texture _srcTexture_. This parameter identifies a
    /// specific mipmap level, set of layers or alternate format in the
    /// specified texture. Other fields, such as swizzle, are ignored. If
    /// NULL is specified, the base level of the texture _srcTexture_ will
    /// be used as-is.
    ///
    /// \param srcRegion Region of the source texture to copy from.
    ///
    /// \param dst Address of the buffer holding the destination data for
    /// the copy.
    ///
    /// \param flags Specifies options used when performing the copy. See
    /// nvn::CopyFlags.
    inline void CopyTextureToBuffer(const Texture *srcTexture,
                                    const TextureView *srcView,
                                    const CopyRegion *srcRegion,
                                    BufferAddress dst,
                                    CopyFlags flags);

    /// \brief Copy texture data from a region of one Texture object into a
    /// region of another Texture object.
    ///
    /// Copies texture data from the region _srcRegion_ of a view of the
    /// source texture to the region _dstRegion_ of a view of the
    /// destination texture.  If _srcRegion_ and _dstRegion_ are of
    /// different sizes, the source region will be scaled when copying.  If
    /// CopyFlags::LINEAR_FILTER is set in _flags_ when scaling, the copy
    /// will average texels from the source.  Otherwise, the copy will
    /// select the nearest source texel for each destination texel.
    ///
    /// \note If the source and destination regions overlap in memory, the
    /// results of the copy will be undefined.
    ///
    /// \note Filtering across layers of a source texture is not supported;
    /// the number of layers in _srcRegion_ and _dstRegion_ must match.
    ///
    /// \note Linear filtering is supported for a limited subset of source
    /// and destination formats, as documented in the Programming Guide.
    ///
    /// \note Copies involving multisample textures are not supported.
    ///
    /// \note Copies of compressed formats must be aligned on compression
    /// block boundaries and do not support scaling.
    ///
    /// \param srcTexture Texture object holding the source data for the
    /// copy.
    ///
    /// \param srcView View of the texture object used as the source for
    /// the copy.
    /// A texture view, if specified, can be used to override properties of
    /// the corresponding texture _srcTexture_. This parameter identifies a
    /// specific mipmap level, set of layers or alternate format in the
    /// specified texture. Other fields, such as swizzle, are ignored. If
    /// NULL is specified, the base level of the texture _srcTexture_ will
    /// be used as-is.
    ///
    /// \param srcRegion Region of the source texture to read from.
    ///
    /// \param dstTexture Texture object used as the destination for the
    /// copy.
    ///
    /// \param dstView View of the texture object used as the destination
    /// for the copy.
    /// A texture view, if specified, can be used to override properties of
    /// the corresponding texture _dstTexture_. This parameter identifies a
    /// specific mipmap level, set of layers or alternate format in the
    /// specified texture. Other fields, such as swizzle, are ignored. If
    /// NULL is specified, the base level of the texture _dstTexture_ will
    /// be used as-is.
    ///
    /// \param dstRegion Region of the destination texture to write to.
    ///
    /// \param flags Specifies options used when performing the copy. See
    /// nvn::CopyFlags.
    inline void CopyTextureToTexture(const Texture *srcTexture,
                                     const TextureView *srcView,
                                     const CopyRegion *srcRegion,
                                     const Texture *dstTexture,
                                     const TextureView *dstView,
                                     const CopyRegion *dstRegion,
                                     CopyFlags flags);

    /// \brief Schedule a copy of data from one Buffer object into another
    /// Buffer object.
    ///
    /// \param src Address of the buffer holding the source data for the
    /// copy.
    ///
    /// \param dst Address of the buffer holding the destination data for
    /// the copy.
    ///
    /// \param size Number of bytes to copy between source and destination
    /// buffers.
    ///
    /// \param flags Specifies options used when performing the copy. See
    /// of nvn::CopyFlags.
    inline void CopyBufferToBuffer(BufferAddress src,
                                   BufferAddress dst,
                                   size_t size,
                                   CopyFlags flags);

    /// \brief Clear a buffer to a 32-bit integer value.
    ///
    /// \param dst Address of the buffer holding the destination data for
    /// the clear.  This address should be aligned to 4 bytes.
    ///
    /// \param size Number of bytes to clear.
    /// Size must be a multiple of four.  Clears with zero size are treated
    /// as no-ops.
    ///
    /// \param value 32-bit integer value to clear with.
    inline void ClearBuffer(BufferAddress dst,
                            size_t size,
                            uint32_t value);

    /// \brief Clear a color buffer using floating point clear color
    /// values.
    ///
    /// Clears texture data from the region _region_ of a view of the
    /// destination texture.
    ///
    /// This command supports only texture formats that can be bound as a
    /// render target.  Please refer to the 'Render' column in the
    /// programming guide's \ref nvn_guide_formats_table "formats table"
    /// for supported formats.
    ///
    /// \warning Unlike Copy commands, ClearTexure renders to the texture
    /// using the 3D Engine. Before accessing the cleared texture,
    /// applications must ensure that the GPU commands producing the new
    /// data have completed with BarrierBits of ORDER_FRAGMENTS and
    /// INVALIDATE_TEXTURE or by waiting on a Sync object.
    ///
    /// \param dstTexture Texture object used as the destination for the
    /// clear.
    ///
    /// \param dstView View of the texture object used as the destination
    /// for the clear.
    /// A texture view, if specified, can be used to override properties of
    /// the corresponding texture _dstTexture_.  If NULL is specified, the
    /// base level of the texture _dstTexture_ will be used as-is.
    ///
    /// \param dstRegion Region of the destination texture to write to.
    ///
    /// \param color Array of four floating-point (RGBA) clear color
    /// values.
    ///
    /// \param mask Bitfield indicating which color channels should be
    /// cleared. See of nvn::ClearColorMask.
    inline void ClearTexture(const Texture *dstTexture,
                             const TextureView *dstView,
                             const CopyRegion *dstRegion,
                             const float *color,
                             ClearColorMask mask);

    /// \brief Clear a color buffer using signed integer clear color
    /// values.
    ///
    /// Clears texture data from the region _region_ of a view of the
    /// destination texture.
    ///
    /// This command supports only texture formats that can be bound as a
    /// render target.  Please refer to the 'Render' column in the
    /// programming guide's \ref nvn_guide_formats_table "formats table"
    /// for supported formats.
    ///
    /// \warning Unlike Copy commands, ClearTexure renders to the texture
    /// using the 3D Engine. Before accessing the cleared texture,
    /// applications must ensure that the GPU commands producing the new
    /// data have completed with BarrierBits of ORDER_FRAGMENTS and
    /// INVALIDATE_TEXTURE or by waiting on a Sync object.
    ///
    /// \param dstTexture Texture object used as the destination for the
    /// clear.
    ///
    /// \param dstView View of the texture object used as the destination
    /// for the clear.
    /// A texture view, if specified, can be used to override properties of
    /// the corresponding texture _dstTexture_.  If NULL is specified, the
    /// base level of the texture _dstTexture_ will be used as-is.
    ///
    /// \param dstRegion Region of the destination texture to write to.
    ///
    /// \param color Array of four signed integer (RGBA) clear color
    /// values.
    ///
    /// \param mask Bitfield indicating which color channels should be
    /// cleared. See nvn::ClearColorMask.
    inline void ClearTexturei(const Texture *dstTexture,
                              const TextureView *dstView,
                              const CopyRegion *dstRegion,
                              const int *color,
                              ClearColorMask mask);

    /// \brief Clear a color buffer using unsigned integer clear color
    /// values.
    ///
    /// Clears texture data from the region _region_ of a view of the
    /// destination texture.
    ///
    /// This command supports only texture formats that can be bound as a
    /// render target.  Please refer to the 'Render' column in the
    /// programming guide's \ref nvn_guide_formats_table "formats table"
    /// for supported formats.
    ///
    /// \warning Unlike Copy commands, ClearTexure renders to the texture
    /// using the 3D Engine. Before accessing the cleared texture,
    /// applications must ensure that the GPU commands producing the new
    /// data have completed with BarrierBits of ORDER_FRAGMENTS and
    /// INVALIDATE_TEXTURE or by waiting on a Sync object.
    ///
    /// \param dstTexture Texture object used as the destination for the
    /// clear.
    ///
    /// \param dstView View of the texture object used as the destination
    /// for the clear.
    /// A texture view, if specified, can be used to override properties of
    /// the corresponding texture _dstTexture_.  If NULL is specified, the
    /// base level of the texture _dstTexture_ will be used as-is.
    ///
    /// \param dstRegion Region of the destination texture to write to.
    ///
    /// \param color Array of four unsigned integer (RGBA) clear color
    /// values.
    ///
    /// \param mask Bitfield indicating which color channels should be
    /// cleared. See of nvn::ClearColorMask.
    inline void ClearTextureui(const Texture *dstTexture,
                               const TextureView *dstView,
                               const CopyRegion *dstRegion,
                               const uint32_t *color,
                               ClearColorMask mask);

    /// \brief Update the contents of a uniform buffer.
    ///
    /// This command schedules an update of the contents of a uniform
    /// buffer, which will execute without waiting for previous draw calls
    /// prior to the update.  The update will wait on the completion of
    /// previous compute dispatches.  Draw calls and compute dispatches
    /// issued after the update will use the new uniform buffer values.
    /// Calls before the update will use the old values.
    ///
    /// To get correct ordering of updates, the range of buffer memory
    /// identified by _buffer_ and _bufferSize_ must exactly match the
    /// memory range bound via CommandBuffer::BindUniformBuffer.  If any
    /// other uniform buffer binding (using a different base address or
    /// size) overlaps the range of memory updated by this command, shader
    /// accesses using that binding (from commands before or after the
    /// update) will return undefined values.
    ///
    /// \param buffer GPU address of the full uniform buffer to update.
    /// Must be a multiple of 256 (the value of
    /// nvn::DeviceInfo::UNIFORM_BUFFER_ALIGNMENT).
    ///
    /// \param bufferSize Size (in bytes) of the full uniform buffer to
    /// update.
    /// Must be less than or equal to 65536 (the value of
    /// nvn::DeviceInfo::MAX_UNIFORM_BUFFER_SIZE).
    ///
    /// \param updateOffset Offset (in bytes) of the first byte to update
    /// inside the uniform buffer.
    /// Must be a multiple of 4 (the value of
    /// nvn::DeviceInfo::UNIFORM_BUFFER_UPDATE_ALIGNMENT).
    ///
    /// \param updateSize Number of bytes to update inside the uniform
    /// buffer.
    /// Must be less than or equal to 65536 (the value of
    /// nvn::DeviceInfo::MAX_UNIFORM_BUFFER_SIZE).
    /// Must be a multiple of 4 (the value of
    /// nvn::DeviceInfo::UNIFORM_BUFFER_UPDATE_ALIGNMENT).
    ///
    /// \param data New data to write into the uniform buffer.  The data is
    /// consumed before the call returns.
    inline void UpdateUniformBuffer(BufferAddress buffer,
                                    size_t bufferSize,
                                    ptrdiff_t updateOffset,
                                    size_t updateSize,
                                    const void *data);

    /// \brief Write a GPU counter and timestamp into buffer object memory.
    ///
    /// The GPU maintains a set of counters for each queue that will be
    /// incremented on certain events.  This function requests that the
    /// current value of the counter be written to a specified offset in
    /// the specified buffer object.  16 bytes will be written to the
    /// buffer object.
    ///
    /// Except for the CounterType::ZCULL_STATS counter, the first 8 bytes
    /// hold the 64-bit counter value and the second 8 bytes hold a
    /// timestamp when the counter value was reported.
    /// CounterType::ZCULL_STATS reports four 32-bit counter values.
    ///
    /// \note If tiled caching is enabled, this command will trigger an
    /// implicit tiled cache flush.
    ///
    /// \note For timestamp, please refer to the \ref
    /// nvn_guide_counter_resolution_of_timestamp "NVN Programming Guide"
    /// for details.
    ///
    /// \param counter Type of counter that will be written.
    ///
    /// \param buffer GPU address of buffer memory where the counter will
    /// be written.
    /// Must be a multiple of 16 (the value of
    /// nvn::DeviceInfo::COUNTER_ALIGNMENT).
    inline void ReportCounter(CounterType counter,
                              BufferAddress buffer);

    /// \brief Reset a GPU counter to zero.
    ///
    /// The GPU maintains a set of counters for each queue that will be
    /// incremented on certain events.  This function resets the current
    /// value of the counter to zero.
    ///
    /// \note If tiled caching is enabled, this command will trigger an
    /// implicit tiled cache flush.
    ///
    /// \param counter Type of counter that will be written.
    inline void ResetCounter(CounterType counter);

    /// \brief Write a GPU counter report and timestamp into buffer object
    /// memory using an application-provided value.
    ///
    /// This command will cause the GPU to write a counter report (of type
    /// CounterData) to buffer object memory, with the counter value
    /// replaced by an unsigned 32-bit value provided in the command.  In
    /// the report, the first 8 bytes hold a 64-bit integer set to the
    /// provided report value and the second 8 bytes hold a timestamp when
    /// the report was performed.
    ///
    /// \note If tiled caching is enabled, this command will trigger an
    /// implicit tiled cache flush.
    ///
    /// \note For timestamp, please refer to the \ref
    /// nvn_guide_counter_resolution_of_timestamp "NVN Programming Guide"
    /// for the details.
    ///
    /// \param value Value that be written in the counter report.
    ///
    /// \param buffer GPU address of buffer memory where the counter will
    /// be written.
    /// Must be a multiple of 16 (the value of
    /// nvn::DeviceInfo::COUNTER_ALIGNMENT).
    inline void ReportValue(uint32_t value,
                            BufferAddress buffer);

    /// \brief Sets the render enable for a queue.
    ///
    /// The render enable allows applications to disable all rendering to a
    /// queue unconditionally.  If rendering is disabled for a queue, all
    /// rendering commands (framebuffer clears, Draw commands) will be
    /// discarded without any processing.  Commands changing queue state
    /// will be executed even if rendering is disabled and will affect
    /// subsequent rendering commands if/when rendering is re-enabled.
    /// SetRenderEnable and SetRenderEnableConditional affect the same
    /// state in the queue.
    ///
    /// \param enable NVN_TRUE if rendering should be enabled; NVN_FALSE if
    /// rendering should be disabled.
    inline void SetRenderEnable(NVNboolean enable);

    /// \brief Enables rendering for a queue, depending on the equality of
    /// counters.
    ///
    /// Enable or disable rendering based on whether two 64-bit values in
    /// buffer memory are equal. If the values are written using the
    /// SAMPLES_PASSED counter, this function can be used in an
    /// implementation of occlusion culling. SetRenderEnable and
    /// SetRenderEnableConditional affect the same state in the queue.
    ///
    /// \param mode NVN_CONDITIONAL_RENDER_MODE_RENDER_IF_EQUAL if
    /// rendering should be enabled if the buffer values are equal,
    /// NVN_CONDITIONAL_RENDER_MODE_RENDER_IF_NOT_EQUAL otherwise.
    ///
    /// \param counters Buffer address of the two values to be compared.
    /// There must be at least 32 bytes of buffer memory starting at this
    /// address. The first value is contained in the first eight bytes
    /// starting at this address, and the second value is contained in the
    /// eight bytes starting at _bufferAddress_ + 16. Note that this layout
    /// is consistent with two counter query results stored consecutively
    /// in memory.
    /// Must be a multiple of 16 (the value of
    /// nvn::DeviceInfo::COUNTER_ALIGNMENT).
    inline void SetRenderEnableConditional(ConditionalRenderMode mode,
                                           BufferAddress counters);

    /// \brief Sets color and depth/stencil textures in the framebuffer.
    ///
    /// \param numColors Number of color textures in the framebuffer.
    /// If zero is specified, the framebuffer will be programmed with no
    /// color textures.  Any color binding point numbered _numColors_ or
    /// higher will be programmed with no color texture.
    /// Must be less than or equal to 8 (the value of
    /// nvn::DeviceInfo::COLOR_BUFFER_BINDINGS).
    ///
    /// \param colors Array of _numColors_ color textures to program in the
    /// framebuffer.
    /// If any element in the array is set to NULL, the corresponding color
    /// target in the framebuffer will be programmed with no texture.
    ///
    /// \param colorViews Array of _numColors_ color texture views to
    /// program in the framebuffer.
    /// The texture views, if specified, can be used to override properties
    /// of the corresponding texture in the array _colors_.  If NULL is
    /// specified, all textures in _colors_ will be used as-is.  If a
    /// non-NULL array of views is specified but one or more entries in
    /// that array is NULL, the corresponding textures in _colors_ will be
    /// used as-is.
    ///
    /// \param depthStencil Depth/stencil texture to program in the
    /// framebuffer.
    /// If NULL is specified, the framebuffer will be programmed with no
    /// depth/stencil buffer.
    ///
    /// \param depthStencilView Depth/stencil texture view to program in
    /// the framebuffer.
    /// A texture view, if specified, can be used to override properties of
    /// the corresponding texture _depthStencil_.  If NULL is specified,
    /// the texture _depthStencil_ will be used as-is.
    inline void SetRenderTargets(int numColors,
                                 const Texture * const *colors,
                                 const TextureView * const *colorViews,
                                 const Texture *depthStencil,
                                 const TextureView *depthStencilView);

    /// \brief Discard the contents of the specified bound color render
    /// target.
    ///
    /// After discarding the contents of a color render target, its values
    /// will become undefined. This command can be useful for saving memory
    /// bandwidth for 'transient' rendering operations such as rendering to
    /// a multisample texture.  After rendering to a multisample texture,
    /// the contents are often filtered to produce single-sample texels
    /// using CommandBuffer::Downsample.  Discarding the contents of a
    /// multisample texture after the downsample allows the GPU to throw
    /// away its contents before writing values to memory.
    ///
    /// Only the portion of the render target contained inside the current
    /// scissor rectangle will be discarded.
    ///
    /// \param index Number of the color target whose contents should be
    /// discarded.
    /// Must be less than 8 (the value of
    /// nvn::DeviceInfo::COLOR_BUFFER_BINDINGS).
    inline void DiscardColor(int index);

    /// \brief Discard the contents of the currently bound depth/stencil
    /// render target.
    ///
    /// After discarding the contents of a depth/stencil render target, its
    /// values will become undefined. This command can be useful for saving
    /// memory bandwidth for 'transient' rendering operations such as
    /// rendering to a depth/stencil buffer that is only used to generate a
    /// color buffer image in the same frame.  Discarding the contents of a
    /// texture allows the GPU to throw away its contents before writing
    /// values to main memory.
    ///
    /// Only the portion of the render target contained inside the current
    /// scissor rectangle will be discarded.
    inline void DiscardDepthStencil();

    /// \brief Computes a filtered version of a multisample source texture
    /// to produce a single-sample destination texture.
    ///
    /// This command completes the execution of all previous rendering
    /// commands before performing the downsample operation.  In
    /// particular, it will flush all primitives accumulated by tiled
    /// caching logic.  When using tiled caching,
    /// CommandBuffer::TiledDownsample will perform better.  This command
    /// is performed by the RSTR2D unit.
    ///
    /// \param src Source texture for the downsample operation.
    ///
    /// \param dst Destination texture for the downsample operation.
    inline void Downsample(const Texture *src,
                           const Texture *dst);

    /// \brief Computes a filtered version of a multisample source texture
    /// to produce a single-sample destination texture.
    ///
    /// When tiled caching is enabled, the downsample operation performed
    /// by this command will be accumulated along with other rendered
    /// primitives.  The actual downsample will be performed on a
    /// tile-by-tile basis, and may complete on one tile before regular
    /// rendering for another tile begins.  When tiled caching is disabled,
    /// this command will behave similarly to CommandBuffer::Downsample,
    /// which fully flushes previous work before starting the downsample
    /// operation.  This command is performed by the 3D pipeline.
    ///
    /// \param src Source texture for the downsample operation.
    ///
    /// \param dst Destination texture for the downsample operation.
    inline void TiledDownsample(const Texture *src,
                                const Texture *dst);

    /// \brief Computes a filtered version of a multisample source texture
    /// and texture view to produce a single-sample destination texture
    /// through a texture view.
    ///
    /// This command completes the execution of all previous rendering
    /// commands before performing the downsample operation.  In
    /// particular, it will flush all primitives accumulated by tiled
    /// caching logic.  When using tiled caching,
    /// CommandBuffer::TiledDownsample will perform better.  This command
    /// is performed by the RSTR2D unit. The texture views may only
    /// describe one single level/layer of a texture.
    ///
    /// \param src Source texture for the downsample operation.
    ///
    /// \param srcView Source texture view for the downsample operation.
    /// Selects one level/layer.
    ///
    /// \param dst Destination texture for the downsample operation.
    ///
    /// \param dstView Destination texture view for the downsample
    /// operation. Selects one level/layer
    inline void DownsampleTextureView(const Texture *src,
                                      const TextureView *srcView,
                                      const Texture *dst,
                                      const TextureView *dstView);

    /// \brief Computes a filtered version of a multisample source texture
    /// texture view to produce a single-sample destination texture view.
    ///
    /// When tiled caching is enabled, the downsample operation performed
    /// by this command will be accumulated along with other rendered
    /// primitives.  The actual downsample will be performed on a
    /// tile-by-tile basis, and may complete on one tile before regular
    /// rendering for another tile begins.  When tiled caching is disabled,
    /// this command will behave similarly to CommandBuffer::Downsample,
    /// which fully flushes previous work before starting the downsample
    /// operation.  This command is performed by the 3D pipeline. The
    /// texture views must each describe only a single level/layer of a
    /// texture.
    ///
    /// \param src Source texture for the downsample operation.
    ///
    /// \param srcView Source texture view for the downsample operation.
    /// Selects one level/layer.
    ///
    /// \param dst Destination texture for the downsample operation.
    ///
    /// \param dstView Destination texture view for the downsample
    /// operation. Selects one level/layer.
    inline void TiledDownsampleTextureView(const Texture *src,
                                           const TextureView *srcView,
                                           const Texture *dst,
                                           const TextureView *dstView);

    /// \brief Specify a barrier ordering execution of GPU commands and
    /// invalidating internal GPU caches.
    ///
    /// Rendering and compute dispatch commands sent to NVN queues are
    /// executed on the GPU using a hardware pipeline, with no guarantee
    /// that work for one draw call or dispatch completes before work on
    /// the next draw call or dispatch starts.  Additionally, some units of
    /// the GPU maintain caches that are not automatically invalidated when
    /// cached data are overwritten by the CPU or other units of the GPU.
    /// This command allows applications to order the processing of
    /// commands sent before and after the barrier and to invalidate GPU
    /// caches before processing commands sent after the barrier.
    ///
    /// \param barrier Bitfield controlling the order of commands before
    /// and after the barrier and the invalidation of GPU caches for
    /// commands after the barrier. See nvn::BarrierBits.
    inline void Barrier(BarrierBits barrier);

    /// \brief Wait for a Sync object to be signaled on the GPU before
    /// processing any further commands in the specified CommandBuffer
    /// object.
    ///
    /// \param sync Sync object to wait on.
    inline void WaitSync(const Sync *sync);

    /// \brief Mark a Sync object as signaled when all previous commands
    /// have completed in the CommandBuffer object.
    ///
    /// \warning FenceSync commands are not automatically flushed for
    /// processing by the GPU.  Before waiting on a FenceSync call with the
    /// CPU using Sync::Wait or from another Queue using Queue::WaitSync,
    /// applications must ensure that the previous FenceSync call was
    /// flushed using Queue::Flush.  Waiting on a FenceSync call using
    /// Queue::WaitSync in the same queue requires no manual flush.
    ///
    /// \note If tiled caching is enabled, this command will trigger an
    /// implicit tiled cache flush.
    ///
    /// \param sync Sync object to mark as signaled when commands complete.
    ///
    /// \param condition Type of command completion to wait for.
    ///
    /// \param flags Bitfield of additional operations to perform before
    /// signaling. See nvn::SyncFlagBits.
    inline void FenceSync(Sync *sync,
                          SyncCondition condition,
                          SyncFlagBits flags);

    /// \brief Specify a new texture pool used for subsequent rendering
    /// operations.
    ///
    /// \param texturePool TexturePool object to be programmed.
    inline void SetTexturePool(const TexturePool *texturePool);

    /// \brief Specify a new sampler pool used for subsequent rendering
    /// operations.
    ///
    /// \param samplerPool SamplerPool object to be programmed.
    inline void SetSamplerPool(const SamplerPool *samplerPool);

    /// \brief Specify scratch memory required for shader execution.
    ///
    /// The scratch memory provided by this function allows shaders to
    /// require temporary (scratch) memory during execution to save the
    /// results of intermediate computations or flow control state.  Each
    /// queue has its own block of scratch memory, programmed via command
    /// buffers.  Scratch memory will be shared by all active shaders
    /// running on the queue.  Multiple queues can share the same block of
    /// scratch memory.
    ///
    /// Each compiled shader will report a minimum and recommended amount
    /// of scratch memory required.  We recommend that applications provide
    /// the largest recommended amount of scratch memory for optimal
    /// performance.  Providing less memory may resolve in lower shader
    /// performance, and providing less than the minimum amount of memory
    /// will result in crashes.  For more information, consult the
    /// programming guide.
    ///
    /// \param pool Memory pool providing storage for the shader scratch
    /// memory.
    ///
    /// \param offset Offset (in bytes) of the first byte of shader scratch
    /// memory.
    /// Must be a multiple of 4096 (the value of
    /// nvn::DeviceInfo::SHADER_SCRATCH_MEMORY_ALIGNMENT).
    ///
    /// \param size Size (in bytes) of the scratch memory.
    /// Must be a multiple of 131072 (the value of
    /// nvn::DeviceInfo::SHADER_SCRATCH_MEMORY_GRANULARITY).
    inline void SetShaderScratchMemory(const MemoryPool *pool,
                                       ptrdiff_t offset,
                                       size_t size);

    /// \brief Save the current ZCull state for the current depth render
    /// target.
    ///
    /// The ZCull unit of the GPU tracks the contents of the currently
    /// bound depth render target and uses this information to perform
    /// high-speed depth and stencil testing.  When applications switch
    /// between depth render targets, ZCull discards tracking information
    /// for the old render target. This command allows applications to
    /// instead save the ZCull state to buffer memory so that it can be
    /// later restored when switching back to the old target.  When saving
    /// information for a depth render target, SaveZCullData should be
    /// called immediately before switching render targets.
    ///
    /// \param dst GPU address of buffer object memory where the ZCull
    /// contents will be stored.
    /// Must be a multiple of 32 (the value of
    /// nvn::DeviceInfo::ZCULL_SAVE_RESTORE_ALIGNMENT).
    ///
    /// \param size Size (in bytes) of the destination buffer.
    /// To determine the amount of memory required to save and restore
    /// ZCull data, call Texture::GetZCullStorageSize() or
    /// TextureBuilder::GetZCullStorageSize().
    inline void SaveZCullData(BufferAddress dst,
                              size_t size);

    /// \brief Update the current ZCull state to use saved state from a
    /// previous render target.
    ///
    /// The ZCull unit of the GPU tracks the contents of the currently
    /// bound depth render target and uses this information to perform
    /// high-speed depth and stencil testing.  When applications switch
    /// between depth render targets, ZCull discards tracking information
    /// for the old render target.  This command allows applications
    /// restore previously saved ZCull state from buffer memory so that it
    /// can be used when continuing to render to the new target.  When
    /// restoring information for a depth render target, RestoreZCullData
    /// should be called immediately after switching render targets.
    ///
    /// \warning The data restored by this command must have been
    /// previously saved by CommandBuffer::SaveZCullData.  Additionally,
    /// the same depth/stencil texture and texture view must be bound as a
    /// render target at the time of the restore and the previous save.  In
    /// case of a mismatch, the results of the restore are undefined and
    /// may result in GPU exceptions.
    ///
    /// \warning If the current contents of the depth render target are
    /// inconsistent with the contents of the render target when
    /// SaveZCullData was called, the state written by RestoreZCullData is
    /// invalid and the results of depth and stencil testing are undefined.
    ///
    /// \param src GPU address of buffer object memory where the ZCull
    /// contents will be loaded from.
    /// Must be a multiple of 32 (the value of
    /// nvn::DeviceInfo::ZCULL_SAVE_RESTORE_ALIGNMENT).
    ///
    /// \param size Size (in bytes) of the source buffer.
    /// To determine the amount of memory required to save and restore
    /// ZCull data, call Texture::GetZCullStorageSize() or
    /// TextureBuilder::GetZCullStorageSize().
    inline void RestoreZCullData(BufferAddress src,
                                 size_t size);

    /// \brief Set the row stride used for copies between buffer and
    /// texture memory.
    ///
    /// This command sets the stride (in bytes) between rows of texels in
    /// buffer memory when copying texels between buffer and texture
    /// memory.  With the default stride of zero (default), texel data in
    /// buffer memory will be treated as densely packed, where each new row
    /// in memory starts immediately after the end of the previous row.
    ///
    /// \note Setting a stride affects any subsequent calls to
    /// CommandBuffer::CopyBufferToTexture and
    /// CommandBuffer::CopyTextureToBuffer using this command buffer.
    /// Unlike most CommandBuffer calls, this command does not store the
    /// stride in command or control memory and does not affect future
    /// submissions of previously recorded copy commands or copy commands
    /// recorded using other command buffers.
    ///
    /// \param stride Stride (in bytes) between rows of texels in buffer
    /// memory.
    inline void SetCopyRowStride(ptrdiff_t stride);

    /// \brief Set the image stride used for copies between buffer and
    /// texture memory.
    ///
    /// This command sets the stride (in bytes) between layers of texels in
    /// buffer memory when copying texels between buffer and texture
    /// memory.  With the default stride of zero (default), texel data in
    /// buffer memory will be treated as densely packed, where each new
    /// layer in memory starts immediately after the end of the previous
    /// layer.
    ///
    /// \note Setting a stride affects any subsequent calls to
    /// CommandBuffer::CopyBufferToTexture and
    /// CommandBuffer::CopyTextureToBuffer using this command buffer.
    /// Unlike most CommandBuffer calls, this command does not store the
    /// stride in command or control memory and does not affect future
    /// submissions of previously recorded copy commands or copy commands
    /// recorded using other command buffers.
    ///
    /// \param stride Stride (in bytes) between 2D arrays of texels in
    /// buffer memory.
    inline void SetCopyImageStride(ptrdiff_t stride);

    /// \brief Get the current copy row stride stored in a CommandBuffer
    /// object.
    inline ptrdiff_t GetCopyRowStride() const;

    /// \brief Get the current copy image stride stored in a CommandBuffer
    /// object.
    inline ptrdiff_t GetCopyImageStride() const;

    /// \brief Draws an on-screen rectangle displaying the contents of a
    /// provided texture.
    ///
    /// This command renders an axis-aligned rectangle into color target
    /// zero of the framebuffer, displaying some or all of the contents of
    /// a two-dimensional texture or rectangle texture.  The caller
    /// specifies a texture handle identifying the source texture, a
    /// destination region in window coordinates, and a source region in
    /// non-normalized texture coordinates.  For each fragment produced by
    /// the rectangle, DrawTexture computes coordinates in the source
    /// region, performs a texture lookup, and uses the texture result as
    /// the fragment color.  Most per-fragment operations (including
    /// blending) are performed, and the results are written into color
    /// target zero.
    ///
    /// DrawTexture has a number of limitations.  Using illegal state may
    /// result in undefined behavior or GPU errors:
    /// - The source texture must be two-dimensional (or rectangle).
    /// - Level of detail selection is not supported; level zero of the
    /// original texture is used.
    /// - Calling DrawTexture with multiple color targets is illegal.
    /// - Calling DrawTexture with blend modes selecting a second source
    /// color is illegal.
    /// - Conservative rasterization is treated as disabled.
    /// - Depth, depth bounds, and stencil tests are treated as disabled.
    /// - Anisotropic filtering and wrap modes that mirror or repeat are
    /// unsupported.
    ///
    /// \param texture Handle for the pair of Texture and Sampler objects
    /// to use.
    ///
    /// \param dstRegion Region identifying the portion of the color target
    /// to update.
    /// Coordinates (x0,y0) and (x1,y1) identify opposite corners of the
    /// region, in screen coordinates.  All coordinates must be in the
    /// range [-65536, +65535], and x1-x0 and y1-y0 must be in the range
    /// [0, 65535].
    ///
    /// \param srcRegion Region identifying the portion of _texture_ to
    /// access.
    /// Coordinates (x0,y0) and (x1,y1) identify opposite corners of the
    /// region, in non-normalized texel coordinates.  All coordinates must
    /// be in the range [-65536, +65535].  The texture region will be
    /// mirrored horizontally and/or vertically if x0 > x1 or y0 > y1,
    /// respectively.
    inline void DrawTexture(TextureHandle texture,
                            const DrawTextureRegion *dstRegion,
                            const DrawTextureRegion *srcRegion);

    /// \brief Sets the subroutine uniform parameters
    ///
    /// Sets subroutine uniform parameters for program subroutine, similar
    /// to the ARB_shader_subroutine functionality in OpenGL.  Subroutines
    /// allow a application to dynamically switch between using different
    /// sets of functions instead without having to recompile the program,
    /// similar to setting function pointers in C-based languages.  In
    /// order to use this feature, some linkage information from the
    /// offline compiler needs to be set in the program first via a call to
    /// SetSubroutineLinkage.  This function allows a program to set a
    /// range of uniform locations to point to the input subroutines
    /// denoted by the index values in the array _values_.  See the offline
    /// compiler documentation and the NVN documentation for more
    /// information on how to use this feature.
    ///
    /// \param program Program object to perform the operation on.
    ///
    /// \param stage Shader stage to update.
    ///
    /// \param first First subroutine uniform location in the program to
    /// update, and corresponds to the first array entry in the _values_
    /// input.
    ///
    /// \param count Number of subroutine uniform locations to update,
    /// starting with uniform with location _first_, and corresponds to the
    /// number of entries in the input array _values_.
    ///
    /// \param values An array of _count_ subroutine index values to be
    /// assigned to successive subroutine uniform locations, where the
    /// first entry in the array corresponds to subroutine uniform location
    /// _first_.
    inline void SetProgramSubroutines(Program *program,
                                      ShaderStage stage,
                                      const int first,
                                      const int count,
                                      const int *values);

    /// \brief Binds or unbinds a coverage modulation table used for
    /// target-independent rasterization.
    ///
    /// By default, the color modulation feature of target-independent
    /// rasterization scales color using a factor based on the fraction of
    /// covered samples.  For example, if three out of four samples for a
    /// fragment are covered, the default modulation factor will be 0.75.
    /// This function allows applications to override this behavior by
    /// specifying a 16-element table of modulation factors.  If the
    /// modulation table is used with a render target with N raster samples
    /// per color sample and M samples are covered in a fragment, the
    /// modulation factor will be _entries_[(M-1) * 16 / N].  If no
    /// modulation table is bound (i.e., _entries_ is NULL), the modulation
    /// factor will be M / N.
    ///
    /// By default, no modulation table is bound.
    ///
    /// \param entries Table of color modulation factors.
    /// If NULL, color modulation will use the fraction of covered raster
    /// samples as the color modulation factor.
    inline void BindCoverageModulationTable(const float *entries);

    /// \brief Resolve depth values in a compressible depth render target.
    ///
    /// Compressible depth render targets may store depth values in a
    /// compressed form, where individual values are reconstructed using
    /// compressed depth data and the current set of sample locations.
    /// This command decompresses the depth buffer, resolving individual
    /// depth values based on the current sample locations.  This command
    /// should be used in the unlikely event that an application needs to
    /// change sample locations for a render target but use depth values
    /// that reflect the old locations.  It is not necessary to decompress
    /// depth render targets for any other reason, including to use them
    /// for texture mapping.  If there is no current depth render target,
    /// this command will have no effect.
    inline void ResolveDepthBuffer();

    /// \brief Enable or disable color reduction.
    ///
    /// \param enable If NVN_TRUE, enable color reduction. Otherwise
    /// disable.
    inline void SetColorReductionEnable(NVNboolean enable);

    /// \brief Specify new color reduction thresholds for a format class.
    ///
    /// \param formatClass Format class for which to set thresholds
    ///
    /// \param thresholdConservative Color reduction threshold for possible
    /// primitive edges.
    /// This threshold is used if a tile is used for more than one
    /// rasterization operation before flushing to L2 cache.
    ///
    /// \param thresholdAggressive Color reduction threshold for interior
    /// pixels.
    /// This threshold is used if a tile is used for exactly once in a
    /// rasterization operation before flushing to L2 cache.
    inline void SetColorReductionThresholds(FormatClass formatClass,
                                            int thresholdConservative,
                                            int thresholdAggressive);

    /// \brief Pushes a debug group into the command stream.
    ///
    /// Debug groups provide a method for annotating a command stream with
    /// discrete groups of commands using a descriptive name. New debug
    /// groups are pushed to the top of the debug group stack. Debug groups
    /// are strictly hierarchical and their sequences may be nested within
    /// other debug groups but can not overlap. These groups may then be
    /// used by the debug layer, an external debugger, or a profiler tool.
    ///
    /// This entry point is for use with debug groups whose description is
    /// comprised of a static string.
    ///
    /// \param domainId Debug domain ID.
    ///
    /// \param description Text string used to annotate the debug group.
    inline void PushDebugGroupStatic(uint32_t domainId,
                                     const char * description);

    /// \brief Pushes a debug group into the command stream.
    ///
    /// Debug groups provide a method for annotating a command stream with
    /// discrete groups of commands using a descriptive name. New debug
    /// groups are pushed to the top of the debug group stack. Debug groups
    /// are strictly hierarchical and their sequences may be nested within
    /// other debug groups but can not overlap. These groups may then be
    /// used by the debug layer, an external debugger, or a profiler tool.
    ///
    /// This entry point is for use with debug groups whose description is
    /// comprised of a dynamic string.
    ///
    /// \param domainId Debug domain ID.
    ///
    /// \param description Text string used to annotate the debug group.
    inline void PushDebugGroupDynamic(uint32_t domainId,
                                      const char * description);

    /// \brief Pushes a debug group into the command stream.
    ///
    /// Debug groups provide a method for annotating a command stream with
    /// discrete groups of commands using a descriptive name. New debug
    /// groups are pushed to the top of the debug group stack. Debug groups
    /// are strictly hierarchical and their sequences may be nested within
    /// other debug groups but can not overlap. These groups may then be
    /// used by the debug layer, an external debugger, or a profiler tool.
    ///
    /// This entry point is equivalent to calling
    /// nvn::CommandBuffer::PushDebugGroupStatic with a debug domain ID of
    /// 0.
    ///
    /// \param description Text string used to annotate the debug group.
    inline void PushDebugGroup(const char * description);

    /// \brief Pops the active debug group off the command stream.
    ///
    /// This entry point is equivalent to calling
    /// nvn::CommandBuffer::PopDebugGroupId with a debug domain ID of 0.
    inline void PopDebugGroup();

    /// \brief Pops the active debug group off the command stream.
    ///
    /// \param domainId Debug domain ID.
    inline void PopDebugGroupId(uint32_t domainId);

    /// \brief Inserts a debug event marker to the command stream.
    ///
    /// Inserts a debug marker to annotate a command stream with a
    /// descriptive text marker. These event markers may then be used by
    /// the debug layer, an external debugger, or a profiler tool.
    ///
    /// This entry point is for use with debug markers whose description is
    /// comprised of a static string.
    ///
    /// \param domainId Debug domain ID.
    ///
    /// \param description Text string used to annotate the event marker.
    inline void InsertDebugMarkerStatic(uint32_t domainId,
                                        const char * description);

    /// \brief Inserts a debug event marker to the command stream.
    ///
    /// Inserts a debug marker to annotate a command stream with a
    /// descriptive text marker. These event markers may then be used by
    /// the debug layer, an external debugger, or a profiler tool.
    ///
    /// This entry point can be used with dynamically allocated strings, as
    /// it copies the data onto the command buffer itself.
    ///
    /// \param domainId Debug domain ID.
    ///
    /// \param description Text string used to annotate the event marker.
    inline void InsertDebugMarkerDynamic(uint32_t domainId,
                                         const char * description);

    /// \brief Inserts a debug event marker to the command stream.
    ///
    /// Inserts a debug marker to annotate a command stream with a
    /// descriptive text marker. These event markers may then be used by
    /// the debug layer, an external debugger, or a profiler tool.
    ///
    /// This entry point is identical to
    /// nvn::CommandBuffer::InsertDebugMarkerStatic with a domainId of 0.
    ///
    /// \param description Text string used to annotate the event marker.
    inline void InsertDebugMarker(const char * description);

    /// \brief Query a callback function for a CommandBuffer object that
    /// will be called if it runs out of memory.
    inline CommandBufferMemoryCallbackFunc GetMemoryCallback() const;

    /// \brief Query a generic pointer that will be passed to a callback
    /// function if the command buffer runs out of memory.
    inline void* GetMemoryCallbackData() const;

    /// \brief Query whether the command buffer is currently recording.
    inline NVNboolean IsRecording() const;

    /// \brief Pause work on the GPU until an event is signaled.
    ///
    /// When applications use this command, they are required to ensure
    /// that the memory associated with the event will eventually contain a
    /// value that passes the test specified by _waitMode_. If not, a GPU
    /// timeout error will occur.
    ///
    /// \warning On the Windows reference implementation, Event objects do
    /// not support synchronization between queues.  Submitting a command
    /// set containing a WaitEvent command on one queue to wait on an event
    /// signaled by a CommandBuffer::SignalEvent command submitted to
    /// another queue may result in GPU timeout errors.  Applications can
    /// use Sync objects to reliably synchronize between queues on all
    /// platforms.
    ///
    /// \param event Event object to wait on.
    ///
    /// \param waitMode Event wait mode.
    /// The wait mode specifies a test use to compare the value in memory
    /// associated with the Event object and the _value_ provided in this
    /// command.  The event is considered to be signaled when this test
    /// passes.
    ///
    /// \param value Value to compare to the value in the Event object's
    /// memory.
    inline void WaitEvent(const Event *event,
                          EventWaitMode waitMode,
                          uint32_t value);

    /// \brief Use the GPU to signal an Event object.
    ///
    /// \param event Event object to signal.
    ///
    /// \param mode Method used to update the memory associated with the
    /// Event object.
    ///
    /// \param location Location in the GPU pipeline that the command must
    /// reach before the signal operation is performed.
    ///
    /// \param flags Flags used to control the signal operation. See
    /// nvn::EventSignalFlags.
    ///
    /// \param value Value used to update the memory associated with the
    /// Event object.
    inline void SignalEvent(const Event *event,
                            EventSignalMode mode,
                            EventSignalLocation location,
                            EventSignalFlags flags,
                            uint32_t value);

    /// \brief Sets the criteria that is used by the ZCull HW for stencil
    /// culling.
    ///
    /// This function sets the criteria that is used by the ZCull HW for
    /// stencil culling. ZCull will only try to discard pixels/fragments if
    /// the cull criteria matches the current stencil state and if the
    /// depth texture of the current render target was created using the
    /// ZCULL_SUPPORT_STENCIL flag.
    /// Note that calling this function will make the current ZCull region
    /// invalid until the next clear. This might have a negative impact on
    /// the depth culling performance.
    ///
    /// \param func Stencil function to be used for stencil culling.
    ///
    /// \param ref Specify a stencil reference value used for stencil
    /// culling.
    ///
    /// \param mask Mask used for stencil culling.
    inline void SetStencilCullCriteria(StencilFunc func,
                                       int ref,
                                       int mask);

} NVN_POST_ALIGN(8);

/// \brief API state object controlling blending for one color target.
NVN_PRE_ALIGN(8)
class BlendState {
protected:
    char reserved[8];
public:

    /// \brief Set default state for the BlendState object.
    ///
    /// \note This is semantically equivalent to calling
    ///
    /// \code{.c}
    ///   BlendState::SetBlendTarget(0);
    ///   BlendState::SetBlendFunc(NVN_BLEND_FUNC_ONE, NVN_BLEND_FUNC_ZERO,
    ///                            NVN_BLEND_FUNC_ONE, NVN_BLEND_FUNC_ZERO);
    ///   BlendState::SetBlendEquation(NVN_BLEND_EQUATION_ADD,
    ///                                NVN_BLEND_EQUATION_ADD);
    ///   BlendState::SetAdvancedMode(NVN_BLEND_ADVANCED_MODE_NONE);
    ///   BlendState::SetAdvancedOverlap(NVN_BLEND_ADVANCED_OVERLAP_UNCORRELATED);
    ///   BlendState::SetAdvancedPremultipliedSrc(NVN_TRUE);
    ///   BlendState::SetAdvancedNormalizedDst(NVN_TRUE);
    /// \endcode
    inline BlendState & SetDefaults();

    /// \brief Specify the number of the color target to update via the
    /// BlendState object.
    ///
    /// \param target Color target number to update when binding the blend
    /// state object.
    inline BlendState & SetBlendTarget(int target);

    /// \brief Specify blend functions for source and destination color and
    /// alpha channels.
    ///
    /// \param srcFunc Blend factor used for source RGB color components.
    ///
    /// \param dstFunc Blend factor used for destination RGB color
    /// components.
    ///
    /// \param srcFuncAlpha Blend factor used for source alpha color
    /// components.
    ///
    /// \param dstFuncAlpha Blend factor used for destination alpha color
    /// components.
    inline BlendState & SetBlendFunc(BlendFunc srcFunc,
                                     BlendFunc dstFunc,
                                     BlendFunc srcFuncAlpha,
                                     BlendFunc dstFuncAlpha);

    /// \brief Specify blend equations for color and alpha channels.
    ///
    /// \param modeRGB Blend equation used for RGB color components.
    ///
    /// \param modeAlpha Blend equation used for alpha color components.
    inline BlendState & SetBlendEquation(BlendEquation modeRGB,
                                         BlendEquation modeAlpha);

    /// \brief Specify a mode for advanced blending equations.
    ///
    /// Advanced blending in NVN provides capabilities similar to those
    /// provided by the NV_blend_equation_advanced OpenGL extension.  These
    /// blend modes specify a technique for combining source and
    /// destination color components, and use the source and destination
    /// alpha components to evaluate how colors are combined with
    /// overlapping color components.  The advanced blending modes provided
    /// here are similar to those supported in various standards, including
    /// several OpenGL and OpenGL ES extensions, OpenVG, the SVG
    /// compositing standard, the PDF document format, and various other
    /// APIs.  For more information on the operation of these blend modes,
    /// please consult the NV_blend_equation_advanced specification.
    ///
    /// When advanced blending is enabled (not set to
    /// BlendAdvancedMode::NONE), only a single render target is supported.
    ///  Advanced blending wtih multiple color targets has undefined
    /// behavior and may result in to GPU errors.  Advanced modes other
    /// than NONE are supported only for color target zero.
    ///
    /// Advanced blending modes are supported on NX but are not supported
    /// on the Windows reference implementation for GPUs older than
    /// first-generation Maxwell GPUs.
    ///
    /// \param overlap Mode to use for advanced blending equations.
    inline BlendState & SetAdvancedMode(BlendAdvancedMode overlap);

    /// \brief Specify blend overlap mode for advanced blending equations.
    ///
    /// Advanced blending in NVN provides capabilities similar to those
    /// provided by the NV_blend_equation_advanced OpenGL extension.  The
    /// overlap modes specify how coverage for partially covered sources
    /// and/or destinations is treated in the blending equations.  This
    /// state has no effect if the advanced blending mode is
    /// BlendAdvancedMode::NONE.
    ///
    /// \param overlap Overlap mode used for advanced blending equations.
    inline BlendState & SetAdvancedOverlap(BlendAdvancedOverlap overlap);

    /// \brief Specify whether the source color is considered premultiplied
    /// for advanced blending equations.
    ///
    /// Advanced blending in NVN provides capabilities similar to those
    /// provided by the NV_blend_equation_advanced OpenGL extension.  By
    /// default, blending is performed with pre-multiplied colors, where a
    /// white pixel with 50% coverage has an (R,G,B,A) value of (0.5, 0.5,
    /// 0.5, 0.5).  If NVN_FALSE is specified, the source color is assumed
    /// not to be premultiplied and the same white pixel would be passed as
    /// (1.0, 1.0, 1.0, 0.5).  Destination colors in advanced blending are
    /// always considered pre-multiplied.  This state has no effect if the
    /// advanced blending mode is BlendAdvancedMode::NONE.
    ///
    /// \param premultiplied TRUE if source color components are considered
    /// to be pre-multiplied by the source alpha.
    inline BlendState & SetAdvancedPremultipliedSrc(NVNboolean premultiplied);

    /// \brief Specify whether the destination color target has unsigned
    /// normalized components (values in [0,1]).
    ///
    /// Advanced blending in NVN provides capabilities similar to those
    /// provided by the NV_blend_equation_advanced OpenGL extension.  By
    /// default, blending assumes that the destination values are stored
    /// with unsigned normalized components (e.g., using formats like
    /// Format::RGBA8).  If this value is set to NVN_FALSE, advanced
    /// blending will not assume that the destination input and the output
    /// will be clamped to [0,1] and will clamp values manually when
    /// required.  This state has no effect if the advanced blending mode
    /// is BlendAdvancedMode::NONE.
    ///
    /// \param normalized TRUE if destination color components are unsigned
    /// normalized ([0,1]) values.
    inline BlendState & SetAdvancedNormalizedDst(NVNboolean normalized);

    /// \brief Query the number of the color target to update via the
    /// BlendState object.
    inline int GetBlendTarget() const;

    /// \brief Query blend functions for source and destination color and
    /// alpha channels.
    ///
    /// \param[out] srcFunc Blend factor used for source RGB color
    /// components.
    ///
    /// \param[out] dstFunc Blend factor used for destination RGB color
    /// components.
    ///
    /// \param[out] srcFuncAlpha Blend factor used for source alpha color
    /// components.
    ///
    /// \param[out] dstFuncAlpha Blend factor used for destination alpha
    /// color components.
    inline void GetBlendFunc(BlendFunc *srcFunc,
                             BlendFunc *dstFunc,
                             BlendFunc *srcFuncAlpha,
                             BlendFunc *dstFuncAlpha) const;

    /// \brief Query blend equations for color and alpha channels.
    ///
    /// \param[out] modeRGB Blend equation used for RGB color components.
    ///
    /// \param[out] modeAlpha Blend equation used for alpha color
    /// components.
    inline void GetBlendEquation(BlendEquation *modeRGB,
                                 BlendEquation *modeAlpha) const;

    /// \brief Query the mode for advanced blending equations.
    inline BlendAdvancedMode GetAdvancedMode() const;

    /// \brief Query the blend overlap mode for advanced blending
    /// equations.
    inline BlendAdvancedOverlap GetAdvancedOverlap() const;

    /// \brief Query whether the source color is considered premultiplied
    /// for advanced blending equations.
    inline NVNboolean GetAdvancedPremultipliedSrc() const;

    /// \brief Query whether the destination color target has unsigned
    /// normalized components (values in [0,1]).
    inline NVNboolean GetAdvancedNormalizedDst() const;

} NVN_POST_ALIGN(8);

/// \brief API state object holding per-channel write masks for all color
/// buffers.
NVN_PRE_ALIGN(4)
class ChannelMaskState {
protected:
    char reserved[4];
public:

    /// \brief Set default state for the ChannelMaskState object.
    ///
    /// \note This is semantically equivalent to calling
    ///
    /// \code{.c}
    ///   for (NVNuint i = 0; i < DeviceInfo::COLOR_BUFFER_BINDINGS; ++i) {
    ///     ChannelMaskState::SetChannelMask(i, NVN_TRUE, NVN_TRUE,
    ///                                         NVN_TRUE, NVN_TRUE);
    ///   };
    /// \endcode
    inline ChannelMaskState & SetDefaults();

    /// \brief Program RGBA channel masks for a single color target.
    ///
    /// \param index Number of the color target to update.
    /// Must be less than 8 (the value of
    /// nvn::DeviceInfo::COLOR_BUFFER_BINDINGS).
    ///
    /// \param r Enable (TRUE) or disable (FALSE) writes to the first (R)
    /// component.
    ///
    /// \param g Enable (TRUE) or disable (FALSE) writes to the second (G)
    /// component.
    ///
    /// \param b Enable (TRUE) or disable (FALSE) writes to the third (B)
    /// component.
    ///
    /// \param a Enable (TRUE) or disable (FALSE) writes to the fourth (A)
    /// component.
    inline ChannelMaskState & SetChannelMask(int index,
                                             NVNboolean r,
                                             NVNboolean g,
                                             NVNboolean b,
                                             NVNboolean a);

    /// \brief Query RGBA channel masks for a single color target.
    ///
    /// \param index Number of the color target to query.
    /// Must be less than 8 (the value of
    /// nvn::DeviceInfo::COLOR_BUFFER_BINDINGS).
    ///
    /// \param[out] r Enable (TRUE) or disable (FALSE) writes to the first
    /// (R) component.
    ///
    /// \param[out] g Enable (TRUE) or disable (FALSE) writes to the second
    /// (G) component.
    ///
    /// \param[out] b Enable (TRUE) or disable (FALSE) writes to the third
    /// (B) component.
    ///
    /// \param[out] a Enable (TRUE) or disable (FALSE) writes to the fourth
    /// (A) component.
    inline void GetChannelMask(int index,
                               NVNboolean *r,
                               NVNboolean *g,
                               NVNboolean *b,
                               NVNboolean *a) const;

} NVN_POST_ALIGN(4);

/// \brief API state object controlling processing of color values.
NVN_PRE_ALIGN(4)
class ColorState {
protected:
    char reserved[4];
public:

    /// \brief Set default state for the ColorState object.
    ///
    /// \note This is semantically equivalent to calling
    ///
    /// \code{.c}
    ///   for (NVNuint i = 0; i < DeviceInfo::COLOR_BUFFER_BINDINGS; ++i) {
    ///     ColorState::SetBlendEnable(i, NVN_FALSE);
    ///   };
    ///   ColorState::SetLogicOp(NVN_LOGIC_OP_COPY);
    ///   ColorState::SetAlphaTest(NVN_ALPHA_FUNC_ALWAYS);
    /// \endcode
    inline ColorState & SetDefaults();

    /// \brief Specifies the blending enable for a single color target.
    ///
    /// \param index Number of the color target to update.
    /// Must be less than 8 (the value of
    /// nvn::DeviceInfo::COLOR_BUFFER_BINDINGS).
    ///
    /// \param enable TRUE if blending should be enabled for the specified
    /// target; FALSE, otherwise.
    inline ColorState & SetBlendEnable(int index,
                                       NVNboolean enable);

    /// \brief Specifies the logical operation to perform for all color
    /// targets.
    ///
    /// If programmed to anything but COPY, arithmetic blending has no
    /// effect. Unlike blending, there is no support for separate logical
    /// operations for each color target.
    ///
    /// \param logicOp Logical operation to perform on all color targets.
    inline ColorState & SetLogicOp(LogicOp logicOp);

    /// \brief Specifies a comparison function to use for alpha testing.
    ///
    /// Using AlphaFunc::ALWAYS disables the alpha test entirely.
    ///
    /// \note Applications may get better performance from performing an
    /// alpha test in the fragment shader and discarding fragments that
    /// fail.  Doing so allows the fragment shader to skip other color
    /// calculations for failing fragments.
    ///
    /// \param alphaTest Comparison function used by the alpha test.
    inline ColorState & SetAlphaTest(AlphaFunc alphaTest);

    /// \brief Query the blending enable for a single color target.
    ///
    /// \param index Number of the color target to query.
    /// Must be less than 8 (the value of
    /// nvn::DeviceInfo::COLOR_BUFFER_BINDINGS).
    inline NVNboolean GetBlendEnable(int index) const;

    /// \brief Query the logical operation to perform for all color
    /// targets.
    inline LogicOp GetLogicOp() const;

    /// \brief Query the comparison function to use for alpha testing.
    inline AlphaFunc GetAlphaTest() const;

} NVN_POST_ALIGN(4);

/// \brief API state object controlling depth and stencil testing
/// operations.
NVN_PRE_ALIGN(8)
class DepthStencilState {
protected:
    char reserved[8];
public:

    /// \brief Set default state for the DepthStencilState object.
    ///
    /// \note This is semantically equivalent to calling
    ///
    /// \code{.c}
    ///   DepthStencilState::SetDepthFunc(NVN_DEPTH_FUNC_LESS);
    ///   DepthStencilState::SetDepthTestEnable(NVN_FALSE);
    ///   DepthStencilState::SetDepthWriteEnable(NVN_TRUE);
    ///   DepthStencilState::SetStencilFunc(NVN_FACE_FRONT_AND_BACK,
    ///                                     NVN_STENCIL_FUNC_ALWAYS);
    ///   DepthStencilState::SetStencilOp(NVN_FACE_FRONT_AND_BACK,
    ///     NVN_STENCIL_OP_KEEP, NVN_STENCIL_OP_KEEP, NVN_STENCIL_OP_KEEP);
    ///   DepthStencilState::SetStencilTestEnable(NVN_FALSE);
    /// \endcode
    inline DepthStencilState & SetDefaults();

    /// \brief Specify whether depth testing is enabled or disabled.
    ///
    /// \param enable Enable (TRUE) or disable (FALSE) depth testing.
    inline DepthStencilState & SetDepthTestEnable(NVNboolean enable);

    /// \brief Specify whether the depth buffer should be updated by the
    /// depth test.
    ///
    /// If depth testing is disabled, the depth buffer is not updated,
    /// regardless of the depth write enable.
    ///
    /// \param enable Enable (TRUE) or disable (FALSE) writes to the depth
    /// buffer.
    inline DepthStencilState & SetDepthWriteEnable(NVNboolean enable);

    /// \brief Specifies the comparison function used when depth testing is
    /// enabled.
    ///
    /// \param func Comparison function used when depth testing is enabled.
    /// If depth testing is disabled, the depth function has no effect.
    inline DepthStencilState & SetDepthFunc(DepthFunc func);

    /// \brief Specify whether the stencil testing is enabled or disabled.
    ///
    /// \param enable Enable (TRUE) or disable (FALSE) stencil testing.
    inline DepthStencilState & SetStencilTestEnable(NVNboolean enable);

    /// \brief Specifies the comparison function used when stencil testing
    /// is enabled.
    ///
    /// There are separate stencil functions for front- and back-facing
    /// primitives. This function can update the value of either or both
    /// functions in the DepthStencilState object.  If stencil testing is
    /// disabled, the stencil function has no effect.
    ///
    /// \param faces Specifies the face(s) (front and/or back) using the
    /// new stencil function.
    ///
    /// \param func Comparison function used when stencil testing is
    /// enabled.
    inline DepthStencilState & SetStencilFunc(Face faces,
                                              StencilFunc func);

    /// \brief Specifies operations performed on stored stencil values by
    /// the stencil test.
    ///
    /// There are separate sets of stencil operations for front- and
    /// back-facing primitives. This function can update the value of
    /// either or both sets of operations in the DepthStencilState object.
    /// If stencil testing is disabled, the stencil operations have no
    /// effect.
    ///
    /// \param faces Specifies the face(s) (front and/or back) using the
    /// new stencil operations.
    ///
    /// \param fail Operation performed on the stencil value when the
    /// stencil test fails.
    ///
    /// \param depthFail Operation performed on the stencil value when the
    /// stencil test passes and the depth test fails.
    ///
    /// \param depthPass Operation performed on the stencil value when the
    /// stencil and depth tests pass.
    inline DepthStencilState & SetStencilOp(Face faces,
                                            StencilOp fail,
                                            StencilOp depthFail,
                                            StencilOp depthPass);

    /// \brief Query whether depth testing is enabled or disabled.
    inline NVNboolean GetDepthTestEnable() const;

    /// \brief Query whether the depth buffer should be updated by the
    /// depth test.
    inline NVNboolean GetDepthWriteEnable() const;

    /// \brief Query the comparison function used when depth testing is
    /// enabled.
    inline DepthFunc GetDepthFunc() const;

    /// \brief Query whether the stencil testing is enabled or disabled.
    inline NVNboolean GetStencilTestEnable() const;

    /// \brief Query the comparison function used when stencil testing is
    /// enabled.
    ///
    /// \param face Specifies the face (front or back) to query.
    inline StencilFunc GetStencilFunc(Face face) const;

    /// \brief Query operations performed on stored stencil values by the
    /// stencil test.
    ///
    /// \param face Specifies the face (front or back) to query the stencil
    /// operations.
    ///
    /// \param[out] fail Operation performed on the stencil value when the
    /// stencil test fails.
    ///
    /// \param[out] depthFail Operation performed on the stencil value when
    /// the stencil test passes and the depth test fails.
    ///
    /// \param[out] depthPass Operation performed on the stencil value when
    /// the stencil and depth tests pass.
    inline void GetStencilOp(Face face,
                             StencilOp *fail,
                             StencilOp *depthFail,
                             StencilOp *depthPass) const;

} NVN_POST_ALIGN(8);

/// \brief API state object controlling multisample rasterization and
/// sample processing.
NVN_PRE_ALIGN(8)
class MultisampleState {
protected:
    char reserved[24];
public:

    /// \brief Set default state for the MultisampleState object.
    ///
    /// \note This is semantically equivalent to calling
    ///
    /// \code{.c}
    ///   MultisampleState::SetMultisampleEnable(NVN_TRUE);
    ///   MultisampleState::SetAlphaToCoverageEnable(NVN_FALSE);
    ///   MultisampleState::SetAlphaToCoverageDither(NVN_TRUE);
    ///   MultisampleState::SetSamples(0);
    ///   MultisampleState::SetRasterSamples(0);
    ///   MultisampleState::SetCoverageModulationMode(CoverageModulationMode::NONE);
    ///   MultisampleState::SetCoverageToColorEnable(NVN_FALSE);
    ///   MultisampleState::SetCoverageToColorOutput(0);
    ///   MultisampleState::SetSampleLocationsEnable(NVN_FALSE);
    ///   MultisampleState::SetSampleLocationsGridEnable(NVN_FALSE);
    /// \endcode All sample locations in the MultisampleState object are
    /// initialized to pixel centers (0.5).
    inline MultisampleState & SetDefaults();

    /// \brief Enable or disable multisample rasterization.
    ///
    /// \param enable Enable (TRUE) or disable (FALSE) multisample
    /// rasterization.
    /// When multisample rasterization is enabled, separate coverage
    /// information is computed for every sample in a pixel.  When
    /// multisample rasterization is disabled, a single coverage value is
    /// used for all samples in a pixel.
    inline MultisampleState & SetMultisampleEnable(NVNboolean enable);

    /// \brief Specify the number of samples in the framebuffer.
    ///
    /// \warning If the sample count in the MultisampleState object and the
    /// textures in the framebuffer don't match, the results of rendering
    /// are undefined.
    ///
    /// \param samples Specifies the number of samples in the framebuffer.
    /// If non-multisample framebuffers are used, the value should be
    /// specified as 0.
    inline MultisampleState & SetSamples(int samples);

    /// \brief Enable or disable alpha-to-coverage operation.
    ///
    /// When alpha-to-coverage operations are enabled, the fourth (A)
    /// component of fragment color zero is used to produce a sample mask
    /// that is combined with raster coverage to produce an updated sample
    /// mask.  An alpha value of 0.0 disables all samples; an alpha value
    /// of 1.0 enables all samples.
    ///
    /// \param enable Enable (TRUE) or disable (FALSE) the
    /// alpha-to-coverage operation.
    inline MultisampleState & SetAlphaToCoverageEnable(NVNboolean enable);

    /// \brief Enable or disable dithering in the alpha-to-coverage
    /// operation.
    ///
    /// When alpha-to-coverage operations are enabled via
    /// MultisampleState::SetAlphaToCoverage, the fourth (A) component of
    /// fragment color zero is used to produce a sample mask that is
    /// combined with raster coverage to produce an updated sample mask.
    /// An alpha value of 0.0 disables all samples; an alpha value of 1.0
    /// enables all samples.  If alpha-to-coverage dithering is enabled by
    /// this command, the same alpha value will produce different sample
    /// masks for different pixels.  If dithering is disabled, the same
    /// alpha value will produce the same sample mask in all pixels.
    /// Dithering will allow for more effective levels of transparency, but
    /// may result in more visible noise.  Dithering has no effect if
    /// alpha-to-coverage is disabled.
    ///
    /// \param dither Enable (TRUE) or disable (FALSE) the
    /// alpha-to-coverage dithering operation.
    inline MultisampleState & SetAlphaToCoverageDither(NVNboolean dither);

    /// \brief Query the multisample rasterization enable.
    inline NVNboolean GetMultisampleEnable() const;

    /// \brief Query the number of samples in a MultisampleState object.
    inline int GetSamples() const;

    /// \brief Query the alpha-to-coverage enable.
    inline NVNboolean GetAlphaToCoverageEnable() const;

    /// \brief Query the alpha-to-coverage dithering enable.
    inline NVNboolean GetAlphaToCoverageDither() const;

    /// \brief Specify the raster sample count for target-independent
    /// rasterization.
    ///
    /// When using a non-zero value (supported:  2, 4, 8, and 16), this
    /// command enables target-independent rasterization, where the
    /// rasterizer evaluates sample coverage using the specified number of
    /// sample locations.  This raster sample count may differ from the
    /// number of stored samples in the framebuffer.  When using a zero
    /// value (default), the command disables target-independent
    /// rasterization and has the rasterizer evaluate sample coverage based
    /// on the stored samples in the framebuffer.
    ///
    /// \param rasterSamples Raster sample count.
    /// If depth or stencil tests are performed with target-independent
    /// rasterization, the raster sample count must be equal to the number
    /// of samples in the depth/stencil render target.  If non-zero, the
    /// raster sample count must be greater than or equal to the color
    /// sample count set by MultisampleState::SetSamples.
    inline MultisampleState & SetRasterSamples(int rasterSamples);

    /// \brief Query the raster sample count for target-independent
    /// rasterization.
    inline int GetRasterSamples();

    /// \brief Set the coverage modulation mode for target-independent
    /// rasterization.
    ///
    /// When target-independent rasterization is enabled, pixel or sample
    /// color values in the framebuffer are updated whenever any of the
    /// associated raster samples are covered by a primitive.  Applications
    /// can enable coverage modulation to account for partial coverage,
    /// where color (RGB) or alpha components are multiplied by a factor
    /// that reflects the fraction of raster samples that are covered.
    /// This modulation mode allows applications to specify that some, all,
    /// or none of the color components are multiplied by this factor.
    ///
    /// \param mode Specifies the set of color channels to multiply by the
    /// coverage modulation factor.
    inline MultisampleState & SetCoverageModulationMode(CoverageModulationMode mode);

    /// \brief Query the coverage modulation mode for target-independent
    /// rasterization.
    inline CoverageModulationMode GetCoverageModulationMode() const;

    /// \brief Enable or disable the fragment coverage-to-color feature.
    ///
    /// When enabled using MultisampleState::SetCoverageToColorEnable, the
    /// fragment coverage-to-color feature writes a coverage mask for each
    /// fragment into the red component of the fragment color for one color
    /// output.  The color output number recieving the coverage mask is
    /// specified by MultisampleState::SetCoverageToColorOutput.
    ///
    /// The coverage mask used for coverage-to-color reflects not only
    /// raster sample coverage, but also the results of per-fragment
    /// operations including other sample masks, alpha to coverage, alpha
    /// test, and the depth bounds, depth, and stencil tests.
    ///
    /// \warning When coverage-to-color is enabled, the color target
    /// receiving the coverage mask must have a one-component format with
    /// signed or unsigned integer components (e.g., R8I, R32UI).
    /// Coverage-to-color has no effect if other color target formats are
    /// used.
    ///
    /// \param enable Fragment coverage-to-color enable.
    inline MultisampleState & SetCoverageToColorEnable(NVNboolean enable);

    /// \brief Query the enable for the fragment coverage-to-color feature.
    inline NVNboolean GetCoverageToColorEnable() const;

    /// \brief Specify the color output updated by the fragment
    /// coverage-to-color feature.
    ///
    /// When enabled using MultisampleState::SetCoverageToColorEnable, the
    /// fragment coverage-to-color feature writes a coverage mask for each
    /// fragment into the red component of the fragment color for one color
    /// output.  The color output number recieving the coverage mask is
    /// specified by MultisampleState::SetCoverageToColorOutput.
    ///
    /// The coverage mask used for coverage-to-color reflects not only
    /// raster sample coverage, but also the results of per-fragment
    /// operations including other sample masks, alpha to coverage, alpha
    /// test, and the depth bounds, depth, and stencil tests.
    ///
    /// \warning When coverage-to-color is enabled, the color target
    /// receiving the coverage mask must have a one-component format with
    /// signed or unsigned integer components (e.g., R8I, R32UI).
    /// Coverage-to-color has no effect if other color target formats are
    /// used.
    ///
    /// \param color Color output number updated by the fragment
    /// coverage-to-color feature.
    inline MultisampleState & SetCoverageToColorOutput(int color);

    /// \brief Query the color output updated by the fragment
    /// coverage-to-color feature.
    inline int GetCoverageToColorOutput() const;

    /// \brief Enable or disable programmable sample locations.
    ///
    /// By default, rasterization in NVN uses a set of fixed sample
    /// locations based on the total number of raster samples.  When
    /// programmable sample locations are enabled, rasterization instead
    /// uses a set of locations from the MultisampleState object programmed
    /// using MultisampleState::SetSampleLocations.
    ///
    /// \param enable Enable/disable programmble sample locations.
    inline MultisampleState & SetSampleLocationsEnable(NVNboolean enable);

    /// \brief Query the programmable sample locations enable.
    inline NVNboolean GetSampleLocationsEnable() const;

    /// \brief Query the size of a pixel grid used for programmable sample
    /// locations.
    ///
    /// When programmable sample locations are enabled, the rasterizer can
    /// use the same set of locations for each pixel, or use a different
    /// set of locations for each pixel in a small fixed-size grid, based
    /// on the enable set by
    /// MultisampleState::SetSampleLocationsGridEnable.  This function
    /// returns the width and height of the fixed-size grid used when
    /// enabled.  The grid will have a total of 16/N pixels, where N is the
    /// total number of raster samples in the multisample state object.
    /// Please refer to the documentation of
    /// MultisampleState::SetSampleLocations for more details.
    ///
    /// \param[out] width Returns the width (in pixels) of the sample
    /// location pixel grid.
    ///
    /// \param[out] height Returns the height (in pixels) of the sample
    /// location pixel grid.
    inline void GetSampleLocationsGrid(int *width,
                                       int *height);

    /// \brief Set the programmable sample locations grid enable.
    ///
    /// When programmable sample locations are enabled, the rasterizer can
    /// use the same set of locations for each pixel (disabled), or use a
    /// different set of locations for each pixel in a small fixed-size
    /// grid.
    ///
    /// \param enable Enable or disable the programmble sample locations
    /// grid.
    /// If set to NVN_TRUE, different sample locations are used for each
    /// pixel in a fixed-size grid.  If set to NVN_FALSE, the same
    /// locations are used for all pixels.
    inline MultisampleState & SetSampleLocationsGridEnable(NVNboolean enable);

    /// \brief Query the programmable sample locations grid enable.
    inline NVNboolean GetSampleLocationsGridEnable() const;

    /// \brief Specifies programmable sample locations for rasterization.
    ///
    /// When programmable sample locations are enabled using
    /// MultisampleState::SetSampleLocationsEnable, rasterization uses a
    /// set of sample locations programmed in the multisample state object
    /// using this command.  Each sample location is a pair of (x,y) values
    /// identifying the location of a sample within its pixel.
    ///
    /// The rasterizer may use the same sample locations for each pixel or
    /// varying sample locations within a repeating pixel grid, depending
    /// on state set using MultisampleState::SetSampleLocationsGridEnable.
    /// If the pixel grid is disabled, a sample with index _s_ in the pixel
    /// with coordinates (x,y) always uses entry _s_ in the array of sample
    /// locations.  If the pixel grid is enabled and the grid has a width
    /// and height of _gw_ and _gh_, respectively, sample _s_ in pixel
    /// (x,y) uses entry _s_ + (_y_ % _gh_) * _gw_ + (_x_ % _gw_).
    ///
    /// \param start Index of the first sample location to update.
    ///
    /// \param count Number of sample locations to update.
    ///
    /// \param values Array of sample location value pairs.
    /// _values_ is an array of 2 * _count_ floating-point values, where
    /// each sample (x,y) location is specified using two values in the
    /// range [0.0, 1.0).  The pair (0.5, 0.5) identifies the center of a
    /// pixel.  Sample locations are converted to fixed-point values when
    /// the multisample state object is updated.
    inline MultisampleState & SetSampleLocations(int start,
                                                 int count,
                                                 const float *values);

} NVN_POST_ALIGN(8);

/// \brief API state object controlling rasterization of polygons.
NVN_PRE_ALIGN(4)
class PolygonState {
protected:
    char reserved[4];
public:

    /// \brief Set default state for the PolygonState object.
    ///
    /// \note This is semantically equivalent to calling
    ///
    /// \code{.c}
    ///   PolygonState::SetCullFace(NVN_FACE_NONE);
    ///   PolygonState::SetFrontFace(NVN_FRONT_FACE_CCW);
    ///   PolygonState::SetPolygonMode(NVN_POLYGON_MODE_FILL);
    ///   PolygonState::SetPolygonOffsetEnables(0);
    /// \endcode
    inline PolygonState & SetDefaults();

    /// \brief Specify controls for culling of front- and back-facing
    /// polygons.
    ///
    /// \param face Specifies the set of faces (if any) that should be
    /// cullled.
    inline PolygonState & SetCullFace(Face face);

    /// \brief Specify whether clockwise or counter-clockwise polygons are
    /// considered front-facing.
    ///
    /// \param face Treat clockwise (CW) or counter-clockwise (CCW)
    /// polygons as front-facing.
    inline PolygonState & SetFrontFace(FrontFace face);

    /// \brief Specifies whether polygons are rendered as points, lines, or
    /// triangles.
    ///
    /// \param polygonMode Render polygons as points (POINT), lines (LINE),
    /// or triangles (FILL).
    inline PolygonState & SetPolygonMode(PolygonMode polygonMode);

    /// \brief Specify whether polygon offset is applied for point-mode,
    /// line-mode, and filled polygons.
    ///
    /// \param enables Bitfield indicating whether polygon offset is
    /// applied for point-mode, line-mode, and filled polygons. See
    /// nvn::PolygonOffsetEnable.
    inline PolygonState & SetPolygonOffsetEnables(PolygonOffsetEnable enables);

    /// \brief Query the control for culling of front- and back-facing
    /// polygons.
    inline Face GetCullFace() const;

    /// \brief Query whether clockwise or counter-clockwise polygons are
    /// considered front-facing.
    inline FrontFace GetFrontFace() const;

    /// \brief Query whether polygons are rendered as points, lines, or
    /// triangles.
    inline PolygonMode GetPolygonMode() const;

    /// \brief Query whether polygon offset is applied for point-mode,
    /// line-mode, and filled polygons.
    inline PolygonOffsetEnable GetPolygonOffsetEnables() const;

} NVN_POST_ALIGN(4);

/// \brief API state object controlling vertex attrib state.
NVN_PRE_ALIGN(4)
class VertexAttribState {
protected:
    char reserved[4];
public:

    /// \brief Set default state for the VertexAttribState object.
    ///
    /// \note This is semantically equivalent to calling
    ///
    /// \code{.c}
    ///   VertexAttribState::SetFormat(NVN_FORMAT_NONE, 0);
    ///   VertexAttribState::SetStreamIndex(0);
    /// \endcode
    ///
    /// except that the object will be disabled until the first valid call
    /// to VertexAttribState::SetFormat().
    inline VertexAttribState & SetDefaults();

    /// \brief Specifies the format (in memory) for values for a single
    /// vertex attribute. Specifying format is mandatory, and doing so also
    /// enables the vertex attribute for fetching.
    ///
    /// \param format Format of the specified vertex attribute.
    ///
    /// \param relativeOffset Offset (in bytes) of the values for the
    /// vertex attribute relative to the base address from the vertex
    /// buffer binding.
    inline VertexAttribState & SetFormat(Format format,
                                         ptrdiff_t relativeOffset);

    /// \brief Specifies the vertex attribute stream binding index.
    ///
    /// \param streamIndex Index of the stream to use for the attribute.
    /// Must be less than 16 (the value of
    /// nvn::DeviceInfo::VERTEX_BUFFER_BINDINGS).
    inline VertexAttribState & SetStreamIndex(int streamIndex);

    /// \brief Query the format (in memory) for values for a single vertex
    /// attribute.
    ///
    /// \param[out] format Format of the specified vertex attribute.
    ///
    /// \param[out] relativeOffset Offset (in bytes) of the values for the
    /// vertex attribute relative to the base address from the vertex
    /// buffer binding.
    inline void GetFormat(Format *format,
                          ptrdiff_t *relativeOffset) const;

    /// \brief Query the vertex attribute stream binding index.
    inline int GetStreamIndex() const;

} NVN_POST_ALIGN(4);

/// \brief API state object controlling vertex stream state.
NVN_PRE_ALIGN(8)
class VertexStreamState {
protected:
    char reserved[8];
public:

    /// \brief Set default state for the VertexStreamState object.
    ///
    /// \note This is semantically equivalent to calling
    ///
    /// \code{.c}
    ///   VertexStreamState::SetDivisor(0);
    ///   VertexStreamState::SetStride(0);
    /// \endcode
    inline VertexStreamState & SetDefaults();

    /// \brief Specifies the stride (in bytes) for vertex attribute records
    /// fetched from a single vertex binding point.
    ///
    /// \param stride Stride (in bytes) between vertex attribute records
    /// for the specified binding point.
    inline VertexStreamState & SetStride(ptrdiff_t stride);

    /// \brief Specifies the frequency divisor for vertices in vertex
    /// binding N.
    ///
    /// If zero, specifies that the vertex binding holds a separate set of
    /// values for each vertex in a rendered primitive.  If set to a
    /// non-zero value X, a separate set of values will be used for each
    /// group of X instances, with the same set of values used for each
    /// vertex in a single instance.
    ///
    /// \param divisor Frequency divisor for vertex attributes using the
    /// specified binding point.
    inline VertexStreamState & SetDivisor(int divisor);

    /// \brief Query the stride (in bytes) for a VertexStreamState object.
    inline ptrdiff_t GetStride() const;

    /// \brief Query the frequency divisor in a VertexStreamState object.
    inline int GetDivisor() const;

} NVN_POST_ALIGN(8);

/// \brief Collection of programmable shaders used to process primitives.
NVN_PRE_ALIGN(8)
class Program {
protected:
    char reserved[192];
    Program(const Program &other) /* disabled */;
    const Program & operator =(const Program &other) /* disabled */;
public:
    Program() {}

    /// \brief Initialize a Program object for the specified device.
    ///
    /// \param device Device owning the program.
    inline NVNboolean Initialize(Device *device);

    /// \brief Finalize a Program object.
    inline void Finalize();

    /// \brief Sets the debug label string for a Program object.
    ///
    /// Annotates a Program object with a descriptive text label. This
    /// label may then be used by the debug layer, an external debugger, or
    /// a profiler tool.
    ///
    /// \param label Text string used to annotate the Program object.
    inline void SetDebugLabel(const char * label);

    /// \brief Initialize a program object to use pre-compiled GPU code
    /// from one or multiple shaders
    ///
    /// \param count Number of GPU programs to load in.
    ///
    /// \param stageData GPU control information to help set up
    inline NVNboolean SetShaders(int count,
                                 const ShaderData *stageData);

    /// \brief Initialize a program object to use pre-compiled GPU code
    /// from one or multiple shaders and optionally replaces the debug data
    /// hashes
    ///
    /// \param count Number of GPU programs to load in.
    ///
    /// \param stageDataExt GPU control information to help set up the
    /// program as well as replacement debug information
    inline NVNboolean SetShadersExt(int count,
                                    const ShaderDataExt *stageDataExt);

    /// \brief Overrides how the program's fragment shader is evaluated.
    ///
    /// By default a fragment shader is evaluated per sample only if a
    /// keyword such as gl_SamplePosition is used. This function can be
    /// used to force a specific sample shading mode.
    ///
    /// \note: This function must be called before calling SetShaders
    /// otherwise it will have no effect.
    ///
    /// \param shadingMode Sample shading mode to override.
    inline void SetSampleShading(SampleShadingMode shadingMode);

    /// \brief Sets the program's subroutine linkage maps for future use
    /// with nvnCommandBufferSetProgramSubroutines.
    ///
    /// When using shader subroutines, applications update the values of
    /// subroutine uniforms using nvnCommandBufferSetProgramSubroutines.
    /// Such updates require the driver to adjust function pointers and
    /// related data to reflect the new uniform values. This linkage
    /// mapping must be set before submitting any commands from a
    /// nvnCommandBufferSetProgramSubroutines call. For each shader stage
    /// with subroutines, the compiler provides a _linkage map_ that
    /// applications must provide to the driver to support subroutine
    /// updates.  The linkage maps for a program must be set before binding
    /// the program or attempting to update subroutine uniforms. The
    /// linkage maps must be exactly what the compiler outputs for the
    /// input program in order for correctness. If the driver detects any
    /// inconsistencies between the input linkage maps and what the program
    /// requires, such as not providing linkage maps for all the stages of
    /// the program that use subroutines, NVN_FALSE is returned.
    /// Additionally, the data may be modified internally by the driver, so
    /// the data backed by this pointer must remain live for the lifetime
    /// of the program after calling SetSubroutineLinkage, and must not be
    /// modified by the application. The driver might also write to the
    /// contents of the linkage map during run-time.
    ///
    /// \param count Number of linkage maps to use, one for each shader
    /// stage that uses subroutines.
    ///
    /// \param linkageMapPtrs An array of subroutine linkage map pointers
    /// where each subroutine linkage map was produced by the offline
    /// compiler.
    inline NVNboolean SetSubroutineLinkage(int count,
                                           const NVNsubroutineLinkageMapPtr *linkageMapPtrs);

} NVN_POST_ALIGN(8);

/// \brief Object specifying state used to construct new memory pool
/// objects.
NVN_PRE_ALIGN(8)
class MemoryPoolBuilder {
protected:
    char reserved[64];
public:

    /// \brief Set the Device associated with a MemoryPoolBuilder object.
    ///
    /// \param device Device owning MemoryPool objects created by the
    /// MemoryPoolBuilder.
    inline MemoryPoolBuilder & SetDevice(Device *device);

    /// \brief Set default state for the MemoryPoolBuilder object.
    ///
    /// This method does not modify the device associated with the builder,
    /// which is programmed by SetDevice.
    ///
    /// \note This is semantically equivalent to calling
    ///
    /// \code{.c}
    ///   MemoryPoolBuilder::SetStorage(NULL, 0);
    ///   MemoryPoolBuilder::SetFlags(NVN_MEMORY_POOL_FLAGS_CPU_NO_ACCESS_BIT |
    ///                               NVN_MEMORY_POOL_FLAGS_GPU_CACHED_BIT);
    /// \endcode
    inline MemoryPoolBuilder & SetDefaults();

    /// \brief Provides memory to be used by memory pools initialized using
    /// a MemoryPoolBuilder.
    ///
    /// When a memory pool is initialized using a memory pool builder,
    /// _size_ bytes of memory pointed to by _memory_ will be owned by the
    /// memory pool until the memory pool is finalized.  Applications must
    /// not access _memory_ directly after calling MemoryPool::Initialize.
    /// For memory pools created with MemoryPoolFlags::CPU_CACHED or
    /// MemoryPoolFlags::CPU_UNCACHED, applications can retrieve a CPU
    /// pointer to the pool using MemoryPool::Map that can be used until
    /// the pool is finalized.  The memory provided by _memory_ can be
    /// re-used or freed by the application after the pool is finalized
    /// using MemoryPool::Finalize.
    ///
    /// Virtual memory pools (MemoryPoolFlags::VIRTUAL) do not own any
    /// storage and must be initialized using a NULL _memory_ parameter.
    ///
    /// On NX, non-virtual memory pools must be initialized with
    /// application-provided memory.  Initialization for non-virtual memory
    /// pools will fail if a builder with a NULL _memory_ parameter is
    /// provided.
    ///
    /// On Windows, NVN allocates separate memory for non-virtual memory
    /// pools and does not take ownership of the application-provided
    /// memory.  If _memory_ is NULL, the contents of the allocated pool
    /// memory will be uninitialized.  If _memory_ is non-NULL, the
    /// allocated pool memory will be initialized by copying the _size_
    /// bytes pointed to by _memory_.
    ///
    /// \param[in,out] memory Storage used for the memory pool.
    /// Must be aligned to a multiple of 4KB
    /// (NVN_MEMORY_POOL_STORAGE_ALIGNMENT).
    ///
    /// \param size Size (in bytes) of the memory pool.
    /// Must be aligned to a multiple of 4KB
    /// (NVN_MEMORY_POOL_STORAGE_GRANULARITY).
    /// Must be greater than zero.
    inline MemoryPoolBuilder & SetStorage(void *memory,
                                          size_t size);

    /// \brief Set properties of MemoryPool objects created from the
    /// builder.
    ///
    /// \param flags Bitfield specifying properties of the memory pool to
    /// initialize. See nvn::MemoryPoolFlags
    inline MemoryPoolBuilder & SetFlags(MemoryPoolFlags flags);

    /// \brief Query the device associated with a MemoryPoolBuilder.
    inline const Device * GetDevice() const;

    /// \brief Query the memory allocation associated with a
    /// MemoryPoolBuilder.
    inline void* GetMemory() const;

    /// \brief Query the size associated with a MemoryPoolBuilder.
    inline size_t GetSize() const;

    /// \brief Query the flags associated with a MemoryPoolBuilder.
    inline MemoryPoolFlags GetFlags() const;

} NVN_POST_ALIGN(8);

/// \brief Block of GPU-accessible memory that can be used for storage of
/// buffer and texture objects.
NVN_PRE_ALIGN(8)
class MemoryPool {
protected:
    char reserved[256];
    MemoryPool(const MemoryPool &other) /* disabled */;
    const MemoryPool & operator =(const MemoryPool &other) /* disabled */;
public:
    MemoryPool() {}

    /// \brief Initialize a MemoryPool object using a MemoryPoolBuilder
    /// object.
    ///
    /// \note On Windows, MemoryPool::Initialize may return NVN_FALSE if
    /// video memory on the discrete GPU is exhausted. Reducing the size of
    /// the memory pool and/or shutting down other applications that
    /// consume video memory may resolve this error.
    ///
    /// \param builder MemoryPoolBuilder object holding properties of the
    /// new MemoryPool object.
    inline NVNboolean Initialize(const MemoryPoolBuilder *builder);

    /// \brief Sets the debug label string for a MemoryPool object.
    ///
    /// Annotates a MemoryPool object with a descriptive text label. This
    /// label may then be used by the debug layer, an external debugger, or
    /// a profiler tool.
    ///
    /// \param label Text string used to annotate the MemoryPool object.
    inline void SetDebugLabel(const char * label);

    /// \brief Finalize a MemoryPool object.
    inline void Finalize();

    /// \brief Returns a CPU pointer to the memory owned by the MemoryPool
    /// object.
    ///
    /// If the memory pool does not support CPU access
    /// (MemoryPoolFlags::CPU_NO_ACCESS), NULL is returned.
    inline void* Map() const;

    /// \brief Flush previous CPU writes to a range of the memory pool to
    /// be visible to the GPU.
    ///
    /// \note On NX, calling nn::os::FlushDataCache on an equivalent memory
    /// range will have the same effect as calling this function.
    ///
    /// \param offset Offset (in bytes) of the first byte of the memory
    /// pool range to flush.
    ///
    /// \param size Size (in bytes) of the memory pool range to flush.
    inline void FlushMappedRange(ptrdiff_t offset,
                                 size_t size) const;

    /// \brief Invalidate CPU caches for a range of the memory pool that
    /// may have been written by the GPU.
    ///
    /// \note On NX, calling nn::os::FlushDataCache on an equivalent memory
    /// range will have the same effect as calling this function.
    ///
    /// \param offset Offset (in bytes) of the first byte of the memory
    /// pool range to invalidate.
    ///
    /// \param size Size (in bytes) of the memory pool range to invalidate.
    inline void InvalidateMappedRange(ptrdiff_t offset,
                                      size_t size) const;

    /// \brief Query the GPU address of the data store of a memory pool
    /// object.
    ///
    /// All buffer resources stored in a memory pool are accessed using a
    /// single GPU address mapping created for the entire pool.  This
    /// function returns the GPU address of the first byte of this mapping.
    ///
    /// \note The address for a buffer object can be obtained by adding the
    /// buffer address of the memory pool owning the buffer object and the
    /// offset of the buffer object within its pool.
    inline BufferAddress GetBufferAddress() const;

    /// \brief Map memory pages from physical memory pools into a virtual
    /// memory pool.
    ///
    /// This command can be used to map memory from a physical memory pool
    /// (one using MemoryPoolFlags::PHYSICAL) into a virtual memory pool
    /// (one using MemoryPoolFlags::VIRTUAL).  Each individual mapping
    /// request will map a contiguous set of pages in the physical pool
    /// into a contiguous range of the virtual pool, using a specified
    /// storage class for the virtual memory mapping.  Applications can
    /// specify one or multiple mapping requests in a single call;
    /// performing multiple mappings in a single call can reduce CPU
    /// overhead required to perform the mappings.
    ///
    /// Returns NVN_TRUE if all virtual mapping requests were successfully
    /// processed.  If any mapping request fails, NVN_FALSE will be
    /// returned.  If NVN_FALSE is returned, some of the virtual mapping
    /// requests may have been fully or partially satisfied prior to the
    /// failure.  There is no guarantee that the mappings in _virtualPool_
    /// are unchanged if this command fails.  Virtual mappings may fail if
    /// the graphics firmware has insufficient memory to set up the new
    /// mapping.
    ///
    /// \param numRequests Number of mapping requests to perform.
    ///
    /// \param requests Array of mapping requests to perform.
    inline NVNboolean MapVirtual(int numRequests,
                                 const MappingRequest *requests);

    /// \brief Query the size associated with a MemoryPool object.
    inline size_t GetSize() const;

    /// \brief Query the flags associated with a MemoryPool object.
    inline MemoryPoolFlags GetFlags() const;

} NVN_POST_ALIGN(8);

/// \brief Block of GPU-accessible memory that can be used to store
/// descriptors for texture objects.
NVN_PRE_ALIGN(8)
class TexturePool {
protected:
    char reserved[32];
    TexturePool(const TexturePool &other) /* disabled */;
    const TexturePool & operator =(const TexturePool &other) /* disabled */;
public:
    TexturePool() {}

    /// \brief Initialize a TexturePool object.
    ///
    /// The texture pool is a contiguous block of memory where texture
    /// descriptors are written. Texture descriptors hold information about
    /// textures and are used by the GPU when using textures in rendering
    /// operations. Texture descriptors are written into the pool using
    /// RegisterTexture. The pool itself must be set to the queue before
    /// rendering with any individual textures. A number of entries at the
    /// beginning of the pool are reserved for use by the driver. The
    /// number of reserved entries can be found by querying the device
    /// property RESERVED_TEXTURE_DESCRIPTORS. The size of each entry can
    /// be found by querying TEXTURE_DESCRIPTOR_SIZE.
    ///
    /// \param memoryPool MemoryPool object providing storage for the
    /// texture pool. The memory pool must be CPU-accessible (except on the
    /// Windows reference implementation).
    ///
    /// \param offset Offset (in bytes) of the first byte of memory pool
    /// storage used by the texture pool.
    /// Must be a multiple of 32 (the value of
    /// nvn::DeviceInfo::TEXTURE_DESCRIPTOR_SIZE).
    ///
    /// \param numDescriptors The number of texture descriptors that can be
    /// stored within the texture pool, including texture descriptors
    /// reserved to the driver.
    /// Must be greater than or equal to 256 (the value of
    /// nvn::DeviceInfo::RESERVED_TEXTURE_DESCRIPTORS).
    /// Must be less than or equal to 1048576 (the value of
    /// nvn::DeviceInfo::MAX_TEXTURE_POOL_SIZE).
    inline NVNboolean Initialize(const MemoryPool *memoryPool,
                                 ptrdiff_t offset,
                                 int numDescriptors);

    /// \brief Sets the debug label string for a TexturePool object.
    ///
    /// Annotates a TexturePool object with a descriptive text label. This
    /// label may then be used by the debug layer, an external debugger, or
    /// a profiler tool.
    ///
    /// \param label Text string used to annotate the TexturePool object.
    inline void SetDebugLabel(const char * label);

    /// \brief Finalize a TexturePool object.
    inline void Finalize();

    /// \brief Register a texture descriptor in a texture pool for use via
    /// texture handles.
    ///
    /// The identifier *id* specifies an index into *texturePool* where the
    /// descriptor is written.  The texture pool index is used together
    /// with a sampler pool index to generate a texture handle used for
    /// subsequent texture binding operations or bindless texture accesses.
    /// \note If a texture needs to be used for both texture and image
    /// accesses, it must be registered separately for both use cases
    /// unless the device property
    /// DeviceInfo::SUPPORTS_IMAGES_USING_TEXTURE_HANDLES is true.
    /// \note In case of the NVN Windows reference implementation, if the
    /// texture is initialized with an ASTC format, any format specified in
    /// the _view_ parameter will be ignored. For the detail, please refer
    /// \ref nvn_guide_formats_restrictions_on_Windows
    /// "Formats Restrictions on the NVN Windows Reference Implementation"
    /// section of the Programming Guide.
    ///
    /// \param id Offset (in descriptors) in the texture pool used to store
    /// the texture descriptor.
    /// Must be greater than or equal to 256 (the value of
    /// nvn::DeviceInfo::RESERVED_TEXTURE_DESCRIPTORS).
    /// Must be less than or equal to 1048576 (the value of
    /// nvn::DeviceInfo::MAX_TEXTURE_POOL_SIZE).
    ///
    /// \param texture Texture object to register.
    ///
    /// \param view View of the texture object to register.
    inline void RegisterTexture(int id,
                                const Texture *texture,
                                const TextureView *view) const;

    /// \brief Register an image descriptor in a texture pool for use via
    /// image loads and stores.
    ///
    /// The identifier *id* specifies an index into *texturePool* where the
    /// descriptor is written.  The texture pool index is used to generate
    /// an image handle used for subsequent image binding operations or
    /// bindless image accesses.
    ///
    /// \note If a texture needs to be used for both texture and image
    /// accesses, it must be registered separately for both use cases
    /// unless the device property
    /// DeviceInfo::SUPPORTS_IMAGES_USING_TEXTURE_HANDLES is true.
    ///
    /// \param id Offset (in descriptors) in the texture pool used to store
    /// the image descriptor.
    /// Must be greater than or equal to 256 (the value of
    /// nvn::DeviceInfo::RESERVED_TEXTURE_DESCRIPTORS).
    /// Must be less than or equal to 1048576 (the value of
    /// nvn::DeviceInfo::MAX_TEXTURE_POOL_SIZE).
    ///
    /// \param texture Texture object to register.
    ///
    /// \param view View of the texture object to register.
    inline void RegisterImage(int id,
                              const Texture *texture,
                              const TextureView *view) const;

    /// \brief Query the memory pool used by a TexturePool object.
    inline const MemoryPool * GetMemoryPool() const;

    /// \brief Query the memory pool offset used by a TexturePool object.
    inline ptrdiff_t GetMemoryOffset() const;

    /// \brief Query the number of descriptors in the memory of a
    /// TexturePool object.
    inline int GetSize() const;

} NVN_POST_ALIGN(8);

/// \brief Block of GPU-accessible memory that can be used to store
/// descriptors for sampler objects.
NVN_PRE_ALIGN(8)
class SamplerPool {
protected:
    char reserved[32];
    SamplerPool(const SamplerPool &other) /* disabled */;
    const SamplerPool & operator =(const SamplerPool &other) /* disabled */;
public:
    SamplerPool() {}

    /// \brief Initialize a SamplerPool object.
    ///
    /// The sampler pool is a contiguous block of memory where sampler
    /// descriptors are written. Sampler descriptors hold information about
    /// textures and are used by the GPU when using samplers in rendering
    /// operations. Sampler descriptors are written into the pool using the
    /// RegisterSampler or RegisterSamplerBuilder methods. The pool itself
    /// must be set to the queue before rendering with any individual
    /// textures. A number of entries at the beginning of the pool are
    /// reserved for use by the driver. The number of reserved entries can
    /// be found by querying the device property
    /// RESERVED_SAMPLER_DESCRIPTORS. The size of each entry can be found
    /// by querying SAMPLER_DESCRIPTOR_SIZE.
    ///
    /// \param memoryPool MemoryPool object providing storage for the
    /// sampler pool. The memory pool must be CPU-accessible (except on the
    /// Windows reference implementation).
    ///
    /// \param offset Offset (in bytes) of the first byte of memory pool
    /// storage used by the sampler pool.
    /// Must be a multiple of 32 (the value of
    /// nvn::DeviceInfo::SAMPLER_DESCRIPTOR_SIZE).
    ///
    /// \param numDescriptors The number of sampler descriptors that can be
    /// stored within the sampler pool, including samplers reserved to the
    /// driver.
    /// Must be greater than or equal to 256 (the value of
    /// nvn::DeviceInfo::RESERVED_SAMPLER_DESCRIPTORS).
    /// Must be less than or equal to 4096 (the value of
    /// nvn::DeviceInfo::MAX_SAMPLER_POOL_SIZE).
    inline NVNboolean Initialize(const MemoryPool *memoryPool,
                                 ptrdiff_t offset,
                                 int numDescriptors);

    /// \brief Sets the debug label string for a SamplerPool object.
    ///
    /// Annotates a SamplerPool object with a descriptive text label. This
    /// label may then be used by the debug layer, an external debugger, or
    /// a profiler tool.
    ///
    /// \param label Text string used to annotate the SamplerPool object.
    inline void SetDebugLabel(const char * label);

    /// \brief Finalize a SamplerPool object.
    inline void Finalize();

    /// \brief Register a sampler descriptor in a sampler pool for use via
    /// texture handles, using a Sampler object.
    ///
    /// The identifier *id* specifies an index into *samplerPool* where the
    /// descriptor is written.  The sampler pool index is used together
    /// with a texture pool index to generate a texture handle used for
    /// subsequent texture binding operations or bindless texture accesses.
    ///
    /// \note The SamplerPool::RegisterSamplerBuilder method performs
    /// identical sampler descriptor registration directly from sampler
    /// builder state, without a Sampler object.
    ///
    /// \param id Offset (in descriptors) in the sampler pool used to store
    /// the sampler descriptor.
    /// Must be greater than or equal to 256 (the value of
    /// nvn::DeviceInfo::RESERVED_SAMPLER_DESCRIPTORS).
    /// Must be less than or equal to 4096 (the value of
    /// nvn::DeviceInfo::MAX_SAMPLER_POOL_SIZE).
    ///
    /// \param sampler Sampler object to register.
    inline void RegisterSampler(int id,
                                const Sampler *sampler) const;

    /// \brief Register a sampler descriptor in a sampler pool for use via
    /// texture handles, using SamplerBuilder state.
    ///
    /// The identifier *id* specifies an index into *samplerPool* where the
    /// descriptor is written.  The sampler pool index is used together
    /// with a texture pool index to generate a texture handle used for
    /// subsequent texture binding operations or bindless texture accesses.
    ///
    /// \note The SamplerPool::RegisterSampler method performs identical
    /// sampler descriptor registration using a Sampler object.
    ///
    /// \param id Offset (in descriptors) in the sampler pool used to store
    /// the sampler descriptor.
    /// Must be greater than or equal to 256 (the value of
    /// nvn::DeviceInfo::RESERVED_SAMPLER_DESCRIPTORS).
    /// Must be less than or equal to 4096 (the value of
    /// nvn::DeviceInfo::MAX_SAMPLER_POOL_SIZE).
    ///
    /// \param builder Sampler builder state to register.
    inline void RegisterSamplerBuilder(int id,
                                       const SamplerBuilder *builder) const;

    /// \brief Query the memory pool used by a SamplerPool object.
    inline const MemoryPool * GetMemoryPool() const;

    /// \brief Query the memory pool offset used by a SamplerPool object.
    inline ptrdiff_t GetMemoryOffset() const;

    /// \brief Query the number of descriptors in the memory of a
    /// SamplerPool object.
    inline int GetSize() const;

} NVN_POST_ALIGN(8);

/// \brief Object specifying state used to construct new buffer objects.
NVN_PRE_ALIGN(8)
class BufferBuilder {
protected:
    char reserved[64];
public:

    /// \brief Set the Device associated with a BufferBuilder object.
    ///
    /// \param device Device owning Buffer objects created by the
    /// BufferBuilder.
    inline BufferBuilder & SetDevice(Device *device);

    /// \brief Set default state for the BufferBuilder object.
    ///
    /// This method does not modify the device associated with the builder,
    /// which is programmed by SetDevice.
    ///
    /// \note This is semantically equivalent to calling
    ///
    /// \code{.c}
    ///   BufferBuilder::SetStorage(NULL, 0, 0);
    /// \endcode
    inline BufferBuilder & SetDefaults();

    /// \brief Set the range of a MemoryPool used for a Buffer object
    /// created from the BufferBuilder.
    ///
    /// \param pool MemoryPool object providing storage for the new Buffer
    /// object.
    ///
    /// \param offset Offset (in bytes) of the first byte of pool storage
    /// used by the new Buffer object.
    ///
    /// \param size Size (in bytes) of the new buffer object.
    inline BufferBuilder & SetStorage(MemoryPool *pool,
                                      ptrdiff_t offset,
                                      size_t size);

    /// \brief Query the device associated with this BufferBuilder object.
    inline const Device * GetDevice() const;

    /// \brief Query the memory pool used by a BufferBuilder object.
    inline MemoryPool * GetMemoryPool() const;

    /// \brief Query the memory pool offset used by a BufferBuilder object.
    inline ptrdiff_t GetMemoryOffset() const;

    /// \brief Query the size (in bytes) of the storage in a BufferBuilder
    /// object.
    inline size_t GetSize() const;

} NVN_POST_ALIGN(8);

/// \brief Block of GPU-accessible memory used to hold data used for
/// various purposes.
NVN_PRE_ALIGN(8)
class Buffer {
protected:
    char reserved[48];
    Buffer(const Buffer &other) /* disabled */;
    const Buffer & operator =(const Buffer &other) /* disabled */;
public:
    Buffer() {}

    /// \brief Initialize a new buffer object from a buffer builder.
    ///
    /// Buffer objects are created using the BufferBuilder object, with
    /// storage provided by a memory pool or a previously existing Buffer
    /// object.
    ///
    /// \param builder BufferBuilder object holding properties of the new
    /// Buffer object.
    inline NVNboolean Initialize(const BufferBuilder *builder);

    /// \brief Sets the debug label string for a Buffer object.
    ///
    /// Annotates a Buffer object with a descriptive text label. This label
    /// may then be used by the debug layer, an external debugger, or a
    /// profiler tool.
    ///
    /// \param label Text string used to annotate the Buffer object.
    inline void SetDebugLabel(const char * label);

    /// \brief Finalize a Buffer object.
    inline void Finalize();

    /// \brief Returns a CPU pointer to the memory of a previously created
    /// Buffer object.
    ///
    /// If the memory pool used by the buffer object does not support CPU
    /// access (MemoryPoolFlags::CPU_NO_ACCESS), NULL is returned.
    inline void* Map() const;

    /// \brief Query the GPU address of the data store of a buffer object.
    ///
    /// \note The address for a buffer object can also be obtained by
    /// adding the buffer address of the memory pool owning the buffer
    /// object and the offset of the buffer object within its pool.
    inline BufferAddress GetAddress() const;

    /// \brief Flush previous CPU writes to a range of a buffer to be
    /// visible to the GPU.
    ///
    /// \note On NX, calling nn::os::FlushDataCache on an equivalent memory
    /// range will have the same effect as calling this function.
    ///
    /// \param offset Offset (in bytes) of the first byte of the buffer
    /// range to flush.
    ///
    /// \param size Size (in bytes) of the buffer range to flush.
    inline void FlushMappedRange(ptrdiff_t offset,
                                 size_t size) const;

    /// \brief Invalidate CPU caches for a range of a buffer that may have
    /// been written by the GPU.
    ///
    /// \note On NX, calling nn::os::FlushDataCache on an equivalent memory
    /// range will have the same effect as calling this function.
    ///
    /// \param offset Offset (in bytes) of the first byte of the buffer to
    /// invalidate.
    ///
    /// \param size Size (in bytes) of the buffer to invalidate.
    inline void InvalidateMappedRange(ptrdiff_t offset,
                                      size_t size) const;

    /// \brief Query the memory pool used by a Buffer object.
    inline MemoryPool * GetMemoryPool() const;

    /// \brief Query the memory pool offset used by a Buffer object.
    inline ptrdiff_t GetMemoryOffset() const;

    /// \brief Query the size (in bytes) of the storage of a Buffer object.
    inline size_t GetSize() const;

    /// \brief Returns the debug layer unique ID of this buffer. Returns 0
    /// if debug layer disabled.
    inline uint64_t GetDebugID() const;

} NVN_POST_ALIGN(8);

/// \brief Block of GPU-accessible memory and related state used to hold
/// image data.
NVN_PRE_ALIGN(8)
class Texture {
protected:
    char reserved[192];
    Texture(const Texture &other) /* disabled */;
    const Texture & operator =(const Texture &other) /* disabled */;
public:
    Texture() {}

    /// \brief Initialize a texture object from a texture builder using
    /// storage provided by a memory pool.
    ///
    /// \param builder TextureBuilder object holding properties of the new
    /// Texture object.
    inline NVNboolean Initialize(const TextureBuilder *builder);

    /// \brief Returns the amount of memory (in bytes) required to hold a
    /// ZCull save/restore buffer for a depth texture. The buffer should be
    /// aligned to DeviceInfo::ZCULL_SAVE_RESTORE_ALIGNMENT bytes.
    ///
    /// If the texture has a format with no depth or stencil components,
    /// this function returns zero.
    inline size_t GetZCullStorageSize() const;

    /// \brief Finalize a Texture object.
    inline void Finalize();

    /// \brief Sets the debug label string for a Texture object.
    ///
    /// Annotates a Texture object with a descriptive text label. This
    /// label may then be used by the debug layer, an external debugger, or
    /// a profiler tool.
    ///
    /// \param label Text string used to annotate the Texture object.
    inline void SetDebugLabel(const char * label);

    /// \brief Returns the storage class needed for virtual memory mappings
    /// for a Texture object.
    ///
    /// When memory from a physical memory pool is mapped into a virtual
    /// memory pool, the mapping must be established with a storage class
    /// appropriate for texture or buffer resources using the virtual
    /// memory.  The value returned by this command should be used for
    /// memory mappings used by this texture.
    inline NVNstorageClass GetStorageClass() const;

    /// \brief Returns the offset (in bytes) of the first byte of storage
    /// used by a view of a Texture object.
    ///
    /// The offset returned is a value relative to the offset of the first
    /// byte of storage used by the full texture.  Textures are stored in
    /// (virtually) contiguous memory.  For array and cube map textures,
    /// each layer or face is stored in memory as a unit with offsets
    /// increasing by layer number.  For mipmapped textures, each mipmap
    /// level in the texture (or in each layer for array and cube map
    /// textures) is stored as a unit with offsets increasing by mipmap
    /// level number.
    ///
    /// \param textureView View of the texture used to determine an offset.
    inline ptrdiff_t GetViewOffset(const TextureView *textureView) const;

    /// \brief Query the flags for a Texture object.
    inline TextureFlags GetFlags() const;

    /// \brief Query the target for a Texture object.
    inline TextureTarget GetTarget() const;

    /// \brief Query the width for a Texture object.
    inline int GetWidth() const;

    /// \brief Query the height for a Texture object.
    inline int GetHeight() const;

    /// \brief Query the depth for a Texture object.
    inline int GetDepth() const;

    /// \brief Query the level count for a Texture object.
    inline int GetLevels() const;

    /// \brief Query the format for a Texture object.
    inline Format GetFormat() const;

    /// \brief Query the sample count for a Texture object.
    inline int GetSamples() const;

    /// \brief Query the swizzles for a Texture object.
    ///
    /// \param[out] r Input component value used for the first (R) output
    /// component.
    ///
    /// \param[out] g Input component value used for the second (G) output
    /// component.
    ///
    /// \param[out] b Input component value used for the third (B) output
    /// component.
    ///
    /// \param[out] a Input component value used for the fourth (A) output
    /// component.
    inline void GetSwizzle(TextureSwizzle *r,
                           TextureSwizzle *g,
                           TextureSwizzle *b,
                           TextureSwizzle *a) const;

    /// \brief Query the depth/stencil mode for a Texture object.
    inline TextureDepthStencilMode GetDepthStencilMode() const;

    /// \brief Query the stride for a Texture object.
    inline ptrdiff_t GetStride() const;

    /// \brief Returns the GPU address of the first byte of memory used by
    /// a texture.
    ///
    /// \warning Except for buffer and linear textures, a TextureAddress
    /// returned by Texture::GetTextureAddress corresponds to a special
    /// memory mapping that can not be used to view or manipulate memory in
    /// commands accepting a buffer address.
    inline TextureAddress GetTextureAddress() const;

    /// \brief Returns properties of the sparse memory layout for a
    /// texture.
    ///
    /// \param[out] layout Pointer to a TextureSparseTileLayout structure
    /// to receive layout information.
    inline void GetSparseTileLayout(TextureSparseTileLayout *layout) const;

    /// \brief Copy texture data from memory into a region of a Texture
    /// object.
    ///
    /// Copies texture data from the memory pointed to by _data_ to the
    /// specified region of a texture, using the CPU.  This command behaves
    /// like CommandBuffer::CopyBufferToTexture, except that the copy is
    /// performed immediately and does not involve the GPU on NX. On the
    /// Windows reference implementation, this copy will use the GPU to
    /// update GPU-attached video memory.  If the texture is stored in a
    /// memory pool created with MemoryPoolFlags::CPU_CACHED, the caller
    /// must use Texture::FlushTexels to flush relevant portions of the CPU
    /// cache prior to using the updated texture in the GPU.
    ///
    /// \warning Because the CPU does not have access to GPU compression
    /// information needed to properly interpret GPU-compressed textures,
    /// it is an error to use this command to operate on a texture with the
    /// TextureFlags::COMPRESSIBLE flag set.
    ///
    /// \param textureView View of the texture object used as the
    /// destination for the copy.
    /// A texture view, if specified, can be used to override properties of
    /// the corresponding texture _texture_. This parameter identifies a
    /// specific mipmap level, set of layers or alternate format in the
    /// specified texture. Other fields, such as swizzle, are ignored. If
    /// NULL is specified, the base level of the texture _texture_ will be
    /// used as-is.
    ///
    /// \param region Region of the destination texture to update.
    ///
    /// \param data Pointer to source texel data in system memory.
    inline void WriteTexels(const TextureView *textureView,
                            const CopyRegion *region,
                            const void *data) const;

    /// \brief Copy texture data from memory into a region of a Texture
    /// object with user-specified stride.
    ///
    /// Copies texture data from the memory pointed to by _data_ to the
    /// specified region of a texture, using the CPU.  This command behaves
    /// like the combination of CommandBuffer::SetCopyRowStride,
    /// CommandBuffer::SetCopyImageStride, and
    /// CommandBuffer::CopyBufferToTexture, except that no stride state is
    /// retained, and the copy is performed immediately and does not
    /// involve the GPU on NX. Note this means that _rowStride_ must be at
    /// least the number of bytes in a row of the image, and _imageStride_
    /// must be at least the number of bytes in one full image. On the
    /// Windows reference implementation, this copy will use the GPU to
    /// update GPU-attached video memory. If the texture is stored in a
    /// memory pool created with MemoryPoolFlags::CPU_CACHED, the caller
    /// must use Texture::FlushTexels to flush relevant portions of the CPU
    /// cache prior to using the updated texture in the GPU.
    ///
    /// \warning Because the CPU does not have access to GPU compression
    /// information needed to properly interpret GPU-compressed textures,
    /// it is an error to use this command to operate on a texture with the
    /// TextureFlags::COMPRESSIBLE flag set.
    ///
    /// \param textureView View of the texture object used as the
    /// destination for the copy.
    /// A texture view, if specified, can be used to override properties of
    /// the corresponding texture _texture_. This parameter identifies a
    /// specific mipmap level, set of layers or alternate format in the
    /// specified texture. Other fields, such as swizzle, are ignored. If
    /// NULL is specified, the base level of the texture _texture_ will be
    /// used as-is.
    ///
    /// \param region Region of the destination texture to update.
    ///
    /// \param data Pointer to source texel data in system memory.
    ///
    /// \param rowStride Stride (in bytes) between rows of texels in
    /// _data_.
    /// If specified as 0, rows of texels in _data_ will be treated as
    /// densely packed, where each new row in memory starts immediately
    /// after the end of the previous row. For compressed texture formats,
    /// _rowStride_ indicates the stride between rows of blocks.
    ///
    /// \param imageStride Stride (in bytes) between 2D arrays of texels in
    /// _data_.
    /// If specified as 0, 2D arrays of texels (corresponding to 2d texture
    /// layers, cube faces, or slices of a 3D texture) will be treated as
    /// densely packed, where each 2D array in memory starts immediately
    /// after the end of the previous 2D array. If _imageStride_ is 0 and
    /// _rowStride_ is non-zero, each 2D array is treated as an array of
    /// rows, each consuming _rowStride_ bytes.
    inline void WriteTexelsStrided(const TextureView *textureView,
                                   const CopyRegion *region,
                                   const void *data,
                                   ptrdiff_t rowStride,
                                   ptrdiff_t imageStride) const;

    /// \brief Read texture data from a region of a Texture object into
    /// memory.
    ///
    /// Copies texture data from the specified region of a texture to the
    /// memory pointed to by _data_, using the CPU.  This command behaves
    /// like CommandBuffer::CopyTextureToBuffer, except that the copy is
    /// performed immediately and does not involve the GPU on NX. On the
    /// Windows reference implementation, this copy will use the GPU to
    /// read GPU-attached video memory.  If the texture is stored in a
    /// memory pool created with MemoryPoolFlags::CPU_CACHED and has been
    /// updated using the GPU, the caller must use
    /// Texture::InvalidateTexels to invalidate relevant portions of the
    /// CPU cache prior to using this command to read from the texture.
    ///
    /// \warning Because the CPU does not have access to GPU compression
    /// information needed to properly interpret GPU-compressed textures,
    /// it is an error to use this command to operate on a texture with the
    /// TextureFlags::COMPRESSIBLE flag set.
    ///
    /// \param textureView View of the texture object used as the source
    /// for the copy.
    /// A texture view, if specified, can be used to override properties of
    /// the corresponding texture _texture_. This parameter identifies a
    /// specific mipmap level, set of layers or alternate format in the
    /// specified texture. Other fields, such as swizzle, are ignored. If
    /// NULL is specified, the base level of the texture _texture_ will be
    /// used as-is.
    ///
    /// \param region Region of the source texture to read.
    ///
    /// \param[in,out] data Pointer to destination texel data in system
    /// memory.
    inline void ReadTexels(const TextureView *textureView,
                           const CopyRegion *region,
                           void *data) const;

    /// \brief Read texture data from a region of a Texture object into
    /// memory.
    ///
    /// Copies texture data from the specified region of a texture to the
    /// memory pointed to by _data_, using the CPU.  This command behaves
    /// like the combination of CommandBuffer::SetCopyRowStride,
    /// CommandBuffer::SetCopyImageStride, and
    /// CommandBuffer::CopyTextureToBuffer, except that no stride state is
    /// retained, and the copy is performed immediately and does not
    /// involve the GPU on NX. Note this means that _rowStride_ must be at
    /// least the number of bytes in a row of the image, and _imageStride_
    /// must be at least the number of bytes in one full image. On the
    /// Windows reference implementation, this copy will use the GPU to
    /// read GPU-attached video memory.  If the texture is stored in a
    /// memory pool created with MemoryPoolFlags::CPU_CACHED and has been
    /// updated using the GPU, the caller must use
    /// Texture::InvalidateTexels to invalidate relevant portions of the
    /// CPU cache prior to using this command to read from the texture.
    ///
    /// \warning Because the CPU does not have access to GPU compression
    /// information needed to properly interpret GPU-compressed textures,
    /// it is an error to use this command to operate on a texture with the
    /// TextureFlags::COMPRESSIBLE flag set.
    ///
    /// \param textureView View of the texture object used as the source
    /// for the copy.
    /// A texture view, if specified, can be used to override properties of
    /// the corresponding texture _texture_. This parameter identifies a
    /// specific mipmap level, set of layers or alternate format in the
    /// specified texture. Other fields, such as swizzle, are ignored. If
    /// NULL is specified, the base level of the texture _texture_ will be
    /// used as-is.
    ///
    /// \param region Region of the source texture to read.
    ///
    /// \param[in,out] data Pointer to destination texel data in system
    /// memory.
    ///
    /// \param rowStride Stride (in bytes) between rows of texels in
    /// _data_.
    /// If specified as 0, rows of texels in _data_ will be treated as
    /// densely packed, where each new row in memory starts immediately
    /// after the end of the previous row. For compressed texture formats,
    /// _rowStride_ indicates the stride between rows of blocks.
    ///
    /// \param imageStride Stride (in bytes) between 2D arrays of texels in
    /// _data_.
    /// If specified as 0, 2D arrays of texels (corresponding to 2d texture
    /// layers, cube faces, or slices of a 3D texture) will be treated as
    /// densely packed, where each 2D array in memory starts immediately
    /// after the end of the previous 2D array. If _imageStride_ is 0 and
    /// _rowStride_ is non-zero, each 2D array is treated as an array of
    /// rows, each consuming _rowStride_ bytes.
    inline void ReadTexelsStrided(const TextureView *textureView,
                                  const CopyRegion *region,
                                  void *data,
                                  ptrdiff_t rowStride,
                                  ptrdiff_t imageStride) const;

    /// \brief Flush CPU cache lines holding portions of a texture.
    ///
    /// When a texture object is stored in a memory pool created using
    /// MemoryPoolFlags::CPU_CACHED, texel updates using
    /// Texture::WriteTexels will use the CPU cache and may not be
    /// immediately visible to the GPU.  Before using the contents of such
    /// textures on the GPU, this command should be used to flush relevant
    /// portions of the CPU cache to ensure that updated texels are written
    /// to main memory and will be visible to the GPU.  This operation is
    /// not needed for textures stored in memory pools created using
    /// MemoryPoolFlags::CPU_UNCACHED.
    ///
    /// \param textureView View of the texture object used for the flush
    /// operation.
    /// A texture view, if specified, can be used to specify the layer set
    /// of the corresponding texture _texture_.  If NULL is specified, the
    /// base level of the texture _texture_ will be used as-is.
    ///
    /// \param region Region of the texture used by the flush operation.
    inline void FlushTexels(const TextureView *textureView,
                            const CopyRegion *region) const;

    /// \brief Invalidate CPU cache lines holding portions of a texture.
    ///
    /// When a texture object is stored in a memory pool created using
    /// MemoryPoolFlags::CPU_CACHED, texels accessed by the CPU may be
    /// stored in CPU caches.  If texel values are updated using the GPU,
    /// subsequent accesses using Texture::ReadTexels may read old texel
    /// values if any of the old data remains in the CPU cache.  Before
    /// reading GPU-modified textures using Texture::ReadTexels, this
    /// command should be used to invalidate relevant portions of the CPU
    /// cache to ensure that stale texel values do not remain in the CPU
    /// caches.  This operation is not needed for textures stored in memory
    /// pools created using MemoryPoolFlags::CPU_UNCACHED.
    ///
    /// \param textureView View of the texture object used for the
    /// invalidate operation.
    /// A texture view, if specified, can be used to specify the layer set
    /// of the corresponding texture _texture_.  If NULL is specified, the
    /// base level of the texture _texture_ will be used as-is.
    ///
    /// \param region Region of the texture used for the invalidate
    /// operation.
    inline void InvalidateTexels(const TextureView *textureView,
                                 const CopyRegion *region) const;

    /// \brief Query the memory pool used by a Texture object.
    inline MemoryPool * GetMemoryPool() const;

    /// \brief Query the memory pool offset used by a Texture object.
    inline ptrdiff_t GetMemoryOffset() const;

    /// \brief Query the storage size for a Texture object.
    inline int GetStorageSize() const;

    /// \brief Checks if texture objects are equivalent.
    ///
    /// \param otherTexture Texture object to compare against.
    inline NVNboolean Compare(const Texture *otherTexture) const;

    /// \brief Returns the debug layer unique ID of this texture. Returns 0
    /// if debug layer disabled.
    inline uint64_t GetDebugID() const;

    /// \brief Query the raw storage class associated with this Texture
    /// object.
    inline RawStorageClass GetRawStorageClass() const;

} NVN_POST_ALIGN(8);

/// \brief Object specifying state used to construct new texture objects.
NVN_PRE_ALIGN(8)
class TextureBuilder {
protected:
    char reserved[128];
public:

    /// \brief Set the Device associated with a TextureBuilder object.
    ///
    /// \param device Device owning Texture objects created by the
    /// TextureBuilder.
    inline TextureBuilder & SetDevice(Device *device);

    /// \brief Set default state for the TextureBuilder object.
    ///
    /// This method does not modify the device associated with the builder,
    /// which is programmed by SetDevice.
    ///
    /// \note This is semantically equivalent to calling
    ///
    /// \code{.c}
    ///   TextureBuilder::SetFlags(0);
    ///   TextureBuilder::SetTarget(TextureTarget::TARGET_2D);
    ///   TextureBuilder::SetWidth(0);
    ///   TextureBuilder::SetHeight(0);
    ///   TextureBuilder::SetDepth(0);
    ///   TextureBuilder::SetLevels(1);
    ///   TextureBuilder::SetFormat(Format::NONE);
    ///   TextureBuilder::SetSamples(0);
    ///   TextureBuilder::SetSwizzle(TextureSwizzle::R, TextureSwizzle::G,
    ///                              TextureSwizzle::B, TextureSwizzle::A);
    ///   TextureBuilder::SetDepthStencilMode(TextureDepthStencilMode::DEPTH);
    ///   TextureBuilder::SetStorage(NULL, 0, 0);
    ///   TextureBuilder::SetPackagedTextureData(NULL);
    ///   TextureBuilder::SetPackagedTextureLayout(NULL);
    ///   TextureBuilder::SetStride(0);
    ///   TextureBuilder::SetGLTextureName(0);
    /// \endcode
    inline TextureBuilder & SetDefaults();

    /// \brief Set flags indentifying special properties of Texture objects
    /// created from the TextureBuilder.
    ///
    /// \param flags TextureFlags value specifying special properties for
    /// created Texture objects.
    inline TextureBuilder & SetFlags(TextureFlags flags);

    /// \brief Set the target (texture type) for Texture objects created
    /// from the TextureBuilder.
    ///
    /// \param target Texture target to use.
    inline TextureBuilder & SetTarget(TextureTarget target);

    /// \brief Set the width (in texels) for Texture objects created from
    /// the TextureBuilder.
    ///
    /// \param width Width (in texels) used for new textures.
    inline TextureBuilder & SetWidth(int width);

    /// \brief Set the height (in texels) for Texture objects created from
    /// the TextureBuilder.
    ///
    /// \param height Height (in texels) used for new textures.
    inline TextureBuilder & SetHeight(int height);

    /// \brief Set the depth (in texels) for Texture objects created from
    /// the TextureBuilder.
    ///
    /// \param depth Depth (in texels) used for new textures.
    inline TextureBuilder & SetDepth(int depth);

    /// \brief Set the width (in texels) for Texture objects created from
    /// the TextureBuilder.
    ///
    /// This command also sets the height and depth (in texels) to one.
    ///
    /// \param width Width (in texels) used for new textures.
    /// Must be greater than zero.
    inline TextureBuilder & SetSize1D(int width);

    /// \brief Set the width and height (in texels) for Texture objects
    /// created from the TextureBuilder.
    ///
    /// This command also sets the depth (in texels) to one.
    ///
    /// \param width Width (in texels) used for new textures.
    /// Must be greater than zero.
    ///
    /// \param height Height (in texels) used for new textures.
    /// Must be greater than zero.
    inline TextureBuilder & SetSize2D(int width,
                                      int height);

    /// \brief Set the width, height, and depth (in texels) for Texture
    /// objects created from the TextureBuilder.
    ///
    /// \param width Width (in texels) used for new textures.
    /// Must be greater than zero.
    ///
    /// \param height Height (in texels) used for new textures.
    /// Must be greater than zero.
    ///
    /// \param depth Depth (in texels) used for new textures.
    /// Must be greater than zero.
    inline TextureBuilder & SetSize3D(int width,
                                      int height,
                                      int depth);

    /// \brief Set the number of mipmap levels for Texture objects created
    /// from the TextureBuilder.
    ///
    /// \param numLevels Number of mipmap levels to create for the texture.
    /// \li Must be greater than zero.
    /// \li Must be one for targets TextureTarget::TARGET_RECTANGLE and
    /// TextureTarget::TARGET_BUFFER.
    /// Must be greater than zero.
    /// Must be less than or equal to 15 (the value of
    /// nvn::DeviceInfo::MAX_TEXTURE_LEVELS).
    inline TextureBuilder & SetLevels(int numLevels);

    /// \brief Set the Format for Texture objects created from the
    /// TextureBuilder.
    ///
    /// Note that compressed formats are not supported for 1D or 1D array
    /// textures.
    ///
    /// \param format Format to use for new textures.
    inline TextureBuilder & SetFormat(Format format);

    /// \brief Set the sample count for Texture objects created from the
    /// TextureBuilder.
    ///
    /// \param samples Sample count to use for new textures.
    /// \li Must be zero for non-multisample textures (those with targets
    /// other than TextureTarget::TEXTURE_MULTISAMPLE and
    /// TextureTarget::TARGET_MULTISAMPLE_ARRAY).
    /// \li Must be 2, 4, 8 or 16 for multisample textures.
    /// \li May be 16 only for textures with a format of Format::STENCIL8.
    inline TextureBuilder & SetSamples(int samples);

    /// \brief Set the texture swizzle for Texture objects created from the
    /// TextureBuilder.
    ///
    /// Lookups from Texture objects will return a four-component vector
    /// with (R,G,B,A) components.  The texture Format specifies the
    /// (R,G,B,A) components used for storing texel data.  The texture
    /// swizzle allows applications to produce a new (R,G,B,A) vector by
    /// specifying a source component value (R, G, B, A, or the constants 0
    /// or 1) to use for each component of the new output vector. For
    /// example, an application can use the swizzle to remap a texture with
    /// a format of Format::R8 into a 'LUMINANCE8' texture by using a
    /// swizzle pattern of (R,R,R,1).
    ///
    /// \param r Input component value used for the first (R) output
    /// component.
    ///
    /// \param g Input component value used for the second (G) output
    /// component.
    ///
    /// \param b Input component value used for the third (B) output
    /// component.
    ///
    /// \param a Input component value used for the fourth (A) output
    /// component.
    inline TextureBuilder & SetSwizzle(TextureSwizzle r,
                                       TextureSwizzle g,
                                       TextureSwizzle b,
                                       TextureSwizzle a);

    /// \brief Set the values returned for depth/stencil Texture objects
    /// created from the TextureBuilder.
    ///
    /// If the texture has a format with no depth or stencil components,
    /// the depth/stencil mode is ignored.  If depth/stencil mode selects
    /// the stencil component in a texture with only a depth component or
    /// vice versa, the results of texture lookups are undefined.
    ///
    /// \param mode Specifies whether to return depth or stencil values
    /// from a texture with depth and/or stencil components.
    inline TextureBuilder & SetDepthStencilMode(TextureDepthStencilMode mode);

    /// \brief Returns the amount of memory (in bytes) required to hold a
    /// texture to be created using the provided TextureBuilder object. For
    /// correct operation, the TextureBuilder must have a device set via
    /// TextureBuilder::SetDevice.
    inline size_t GetStorageSize() const;

    /// \brief Returns the minimum alignment in bytes (in bytes) required
    /// to hold a texture to be created using the provided TextureBuilder
    /// object.
    ///
    /// For correct operation, the TextureBuilder must have a device set
    /// via TextureBuilder::SetDevice.
    inline size_t GetStorageAlignment() const;

    /// \brief Set the location in a MemoryPool used for a Texture object
    /// created from the TextureBuilder.
    ///
    /// \param pool MemoryPool object providing storage for the new Texture
    /// object.
    ///
    /// \param offset Offset (in bytes) of the first byte of pool storage
    /// used by the new Texture object.
    /// \li The provided offset must be a multiple of the required
    /// alignment for the provided TextureBuilder object.
    inline TextureBuilder & SetStorage(MemoryPool *pool,
                                       ptrdiff_t offset);

    /// \brief Specify optional packaged texture data used to initialize
    /// texture storage.
    ///
    /// On the Windows reference implementation, texture data produced by
    /// the texture packager is not in a form that can be consumed directly
    /// by the GPU. When creating a texture with a non-NULL packaged
    /// texture data pointer, the Windows reference implementation will
    /// convert the texture data from the packaged form to the native
    /// format of the GPU, storing the results in the pool memory specified
    /// in the TextureBuilder used to create the texture. If the packaged
    /// texture data pointer is NULL, no conversion or copy will be
    /// performed.
    /// On the target device, the packaged texture data pointer is not
    /// used; packaged texture data may be loaded directly in the
    /// underlying pool storage used by the texture.
    /// For more information please refer to the
    /// \ref nvn_guide_formats_restrictions_on_Windows
    /// "Formats Restrictions on the NVN Windows Reference Implementation"
    ///  section of the Programming Guide.
    ///
    /// \param data CPU pointer to NX-optimized texture data built by the
    /// texture packager; ignored on NX.
    inline TextureBuilder & SetPackagedTextureData(const void *data);

    /// \brief Specify the layout of packaged texture data used for texture
    /// storage.
    ///
    /// This command specifies the memory layout of packaged texture data
    /// produced by the NVN texture packager.  By default, the driver and
    /// texture packager select an optimal layout for a texture based on
    /// its size and format.  This command allows applications to provide
    /// the driver with a layout descriptor produced by the NVN texture
    /// packager so that old packaged texture assets can continue to be
    /// used even if the default layout changes in a new driver.
    ///
    /// \param layout Description of the texture layout produced by the NVN
    /// texture packager.
    inline TextureBuilder & SetPackagedTextureLayout(const PackagedTextureLayout *layout);

    /// \brief Specifies the stride for linear textures. Only used when the
    /// TextureFlags::LINEAR_BIT flag is set. The stride is defined as the
    /// number of bytes between consecutive rows in the linear image. If
    /// the texture is to be used as a render target, then this value must
    /// be aligned to at least
    /// deviceInfo::LINEAR_RENDER_TARGET_STRIDE_ALIGNMENT. If it is only
    /// used as a texture, then it needs to be aligned to
    /// deviceInfo::LINEAR_TEXTURE_STRIDE_ALIGNMENT Should be at least
    /// (width * bpp).
    ///
    /// Specify stride for linear textures.
    ///
    /// \param stride Stride for this linear texture.
    /// \li Must be greater than zero for linear textures.
    /// \li Must be aligned to either
    /// deviceInfo::LINEAR_RENDER_TARGET_STRIDE_ALIGNMENT or
    /// deviceInfo::LINEAR_TEXTURE_STRIDE_ALIGNMENT.
    /// Must be greater than or equal to zero.
    inline TextureBuilder & SetStride(ptrdiff_t stride);

    /// \brief Set the name of the OpenGL texture to use as an external
    /// texture source.
    ///
    /// If a texture builder with a non-zero texture name is used to
    /// initialize a new NVN texture, the texture will be initialized to
    /// use all the properties, including texel storage, of the OpenGL or
    /// OpenGL ES texture with the provided name.  The texture name will be
    /// used to look up OpenGL or OpenGL ES texture object using the
    /// OpenGL/OpenGL ES context current on the thread initializing the NVN
    /// texture.  If no OpenGL/OpenGL ES context is current, or if the name
    /// does not identify a valid texture object, the results of NVN
    /// texture initialization are undefined.  There are a number of
    /// restrictions on the GL texture object:
    /// \li Texture storage must be immutable.
    /// \li The format and target of the texture must have NVN equivalents.
    /// \li Sparse textures are unsupported.
    /// \li Buffer textures are unsupported.
    /// Synchronization and lifetime management must be handled by the
    /// application. In particular, the application must ensure that all
    /// NVN commands accessing the texture have completed (e.g., by calling
    /// Queue::Finish()) before calling either glDeleteTextures() or
    /// Texture::Finalize().
    /// By default, the texture name associated with a TextureBuilder is
    /// zero, which indicates that no GL texture should be used for NVN
    /// texture initialization.
    ///
    /// \param name The name of the OpenGL/OpenGL ES texture to use as a
    /// source.
    /// \li Must be zero, or the name of a valid OpenGL or OpenGL ES
    /// texture.
    /// \li If non-zero, the texture properties must be compatible with
    /// NVN.
    inline TextureBuilder & SetGLTextureName(uint32_t name);

    /// \brief Returns the storage class needed for virtual memory mappings
    /// for a texture to be created using the provided TextureBuilder
    /// object.
    ///
    /// When memory from a physical memory pool is mapped into a virtual
    /// memory pool, the mapping must be established with a storage class
    /// appropriate for texture or buffer resources using the virtual
    /// memory.  The value returned by this command should be used for
    /// memory mappings of textures created with this TextureBuilder.
    inline NVNstorageClass GetStorageClass() const;

    /// \brief Query the device associated with this TextureBuilder object.
    inline const Device * GetDevice() const;

    /// \brief Query the flags for a TextureBuilder object.
    inline TextureFlags GetFlags() const;

    /// \brief Query the target for a TextureBuilder object.
    inline TextureTarget GetTarget() const;

    /// \brief Query the width for a TextureBuilder object.
    inline int GetWidth() const;

    /// \brief Query the height for a TextureBuilder object.
    inline int GetHeight() const;

    /// \brief Query the depth for a TextureBuilder object.
    inline int GetDepth() const;

    /// \brief Query the level count for a TextureBuilder object.
    inline int GetLevels() const;

    /// \brief Query the format for a TextureBuilder object.
    inline Format GetFormat() const;

    /// \brief Query the sample count for a TextureBuilder object.
    inline int GetSamples() const;

    /// \brief Query the swizzles for a TextureBuilder object.
    ///
    /// \param[out] r Input component value used for the first (R) output
    /// component.
    ///
    /// \param[out] g Input component value used for the second (G) output
    /// component.
    ///
    /// \param[out] b Input component value used for the third (B) output
    /// component.
    ///
    /// \param[out] a Input component value used for the fourth (A) output
    /// component.
    inline void GetSwizzle(TextureSwizzle *r,
                           TextureSwizzle *g,
                           TextureSwizzle *b,
                           TextureSwizzle *a) const;

    /// \brief Query the depth/stencil mode for a TextureBuilder object.
    inline TextureDepthStencilMode GetDepthStencilMode() const;

    /// \brief Query the packaged texture data pointer for a TextureBuilder
    /// object.
    inline const void * GetPackagedTextureData() const;

    /// \brief Query the packaged texture layout associated with this
    /// TextureBuilder object.
    ///
    /// Returns true if a layout of packaged texture data is associated
    /// with this TextureBuilder. In this case the layout is returned in
    /// data.
    ///
    /// \param[out] data Packaged texture layout associated with this
    /// TextureBuilder object.
    inline NVNboolean GetPackagedTextureLayout(PackagedTextureLayout *data) const;

    /// \brief Query the stride for a TextureBuilder object.
    inline ptrdiff_t GetStride() const;

    /// \brief Returns properties of the sparse memory layout for a texture
    /// to be created using the provided TextureBuilder object.
    ///
    /// For correct operation, the TextureBuilder must have a device set
    /// via TextureBuilder::SetDevice.
    ///
    /// \param[out] layout Pointer to a TextureSparseTileLayout structure
    /// to receive layout information.
    inline void GetSparseTileLayout(TextureSparseTileLayout *layout) const;

    /// \brief Returns the name of the OpenGL or OpenGL ES texture to use
    /// as an external texture source.
    ///
    /// If the returned name is zero, no source texture will be used. See
    /// SetGLTextureName for details.
    inline uint32_t GetGLTextureName() const;

    /// \brief Returns the amount of memory (in bytes) required to hold a
    /// ZCull save/restore buffer for a depth texture. The buffer should be
    /// aligned to DeviceInfo::ZCULL_SAVE_RESTORE_ALIGNMENT bytes.
    ///
    /// If the texture has a format with no depth or stencil components,
    /// this function returns zero.
    inline size_t GetZCullStorageSize() const;

    /// \brief Query the memory pool used by a TextureBuilder object.
    inline MemoryPool * GetMemoryPool() const;

    /// \brief Query the memory pool offset used by a TextureBuilder
    /// object.
    inline ptrdiff_t GetMemoryOffset() const;

    /// \brief Query the raw storage class associated with this
    /// TextureBuilder object.
    inline RawStorageClass GetRawStorageClass() const;

} NVN_POST_ALIGN(8);

/// \brief Object used to specify an alternate 'view' of a full texture
/// object for various rendering operations.
///
/// The TextureView object can be used to specify an alternate 'view' of
/// the storage of a previously created texture object.  Views allow
/// applications to re-interpret texture storage using a different texel
/// format, component swizzle pattern, depth/stencil mode, or target, and
/// can be used to select a subset of the full set of texture mipmap levels
/// or array texture layers.  When a texture object is used with a non-NULL
/// view, any properties not set explicitly by texture view class methods
/// will be taken from the original texture object.  In APIs accepting
/// texture views, a NULL pointer can be used to indicate that all
/// properties should be taken from the original texture object.
NVN_PRE_ALIGN(8)
class TextureView {
protected:
    char reserved[40];
public:

    /// \brief Set default state for the TextureView object.
    ///
    /// A TextureView object programmed with the default state will use all
    /// of the properties of the original texture using the texture view
    /// object.  Only those properties explicitly programmed in the
    /// TextureView object will affect the view of the original texture.
    inline TextureView & SetDefaults();

    /// \brief Update a texture view to select a subset of the mipmap
    /// levels in a Texture object.
    ///
    /// By default, texture objects registered in the texture pool will use
    /// all mipmap levels in the original texture.  Selecting levels in the
    /// texture view when registering a texture will limit the registered
    /// texture to a subset of the levels of the original texture object.
    /// Other rendering operations using a texture will default to using
    /// the base level (numbered zero) of the original texture. Selecting
    /// levels in the texture view in these cases will instead use the
    /// baseLevel parameter of the texture view.
    ///
    /// \param baseLevel Specifies the minimum (base) level in a view of a
    /// Texture object.
    /// A base level of zero selects the highest-resolution mipmap level of
    /// the texture; larger values select lower-resolution mipmap levels.
    /// Must be less than the number of layers in the original texture
    /// object.
    /// Must be less than 15 (the value of
    /// nvn::DeviceInfo::MAX_TEXTURE_LEVELS).
    ///
    /// \param numLevels Specifies the number of mipmap levels in a view of
    /// a Texture object.
    /// If non-zero, selects the specified number of mipmap levels.  If
    /// zero, selects all mipmap levels below baseLevel.  The sum of
    /// _baseLevel_ and _numLevels_ in the view must be less than or equal
    /// to the number of levels in the original texture object.
    inline TextureView & SetLevels(int baseLevel,
                                   int numLevels);

    /// \brief Update a texture view to select a subset of the layers in a
    /// Texture object.
    ///
    /// By default, texture objects registered in the texture pool or used
    /// for rendering will use all layers in the original texture.
    /// Selecting layers in the texture view will limit the view to a
    /// subset of the layers of the original texture object.
    ///
    /// \param minLayer Specifies the minimum layer number in a view of a
    /// Texture object.
    /// Must be less than the total number of layers in the texture.
    /// Must be less than 2048 (the value of
    /// nvn::DeviceInfo::MAX_TEXTURE_LAYERS).
    ///
    /// \param numLayers Specifies the number of layers to use when
    /// accessing this texture.
    /// If non-zero, selects the specified number of layers.  If zero,
    /// selects all layers greater than or equal to minLayer.  The sum of
    /// _minLayer_ and *numLayers* in the view must be less than or equal
    /// to the number of layers in the original texture object.
    inline TextureView & SetLayers(int minLayer,
                                   int numLayers);

    /// \brief Update a texture view to select an alternate texture format.
    ///
    /// Using a format in a texture view allows applications to reinterpret
    /// the storage belonging to a texture object with an alternate format.
    ///  For example, a texture with a format of Format::RGBA8 can be
    /// reinterpreted with a format of Format::RGBA8UI to obtain the
    /// components of the texture as raw integer values instead of as
    /// normalized values in [0,1].  The format selected for the texture
    /// view must be compatible with the format of the original texture
    /// object.  The format specified in a texture view overrides the
    /// format specified by TextureBuilder::SetFormat.
    ///
    /// \param format Format to use in the texture view.
    inline TextureView & SetFormat(Format format);

    /// \brief Update a texture view to select an alternate component
    /// swizzle.
    ///
    /// The swizzles specified in a texture view override those specified
    /// by TextureBuilder::SetSwizzle.
    ///
    /// \param r Input component value used for the first (R) output
    /// component.
    ///
    /// \param g Input component value used for the second (G) output
    /// component.
    ///
    /// \param b Input component value used for the third (B) output
    /// component.
    ///
    /// \param a Input component value used for the fourth (A) output
    /// component.
    inline TextureView & SetSwizzle(TextureSwizzle r,
                                    TextureSwizzle g,
                                    TextureSwizzle b,
                                    TextureSwizzle a);

    /// \brief Update a texture view to select an alternate depth/stencil
    /// mode.
    ///
    /// The swizzles specified in a texture view override those specified
    /// by TextureBuilder::SetDepthStencilMode.
    ///
    /// \param mode Specifies whether to return depth or stencil values
    /// from a texture with depth and/or stencil components.
    inline TextureView & SetDepthStencilMode(TextureDepthStencilMode mode);

    /// \brief Update a texture view to select an alternate texture target.
    ///
    /// The swizzles specified in a texture view override those specified
    /// by TextureBuilder::SetTarget.
    ///
    /// \param target Texture target to use.
    inline TextureView & SetTarget(TextureTarget target);

    /// \brief Query the set of levels associated with a TextureView
    /// object.
    ///
    /// Returns NVN_TRUE if a set of levels has been specified in the view.
    /// Returns NVN_FALSE if no set of levels has been specified.
    ///
    /// \param[out] baseLevel Minimum (base) level in a view of a Texture
    /// object.  Zero is returned if no set of levels has been specified.
    ///
    /// \param[out] numLevels Number of mipmap levels in a view of a
    /// Texture object.  Zero is returned if no set of levels has been
    /// specified.
    inline NVNboolean GetLevels(int *baseLevel,
                                int *numLevels) const;

    /// \brief Query the set of layers associated with a TextureView
    /// object.
    ///
    /// Returns NVN_TRUE if a set of layers has been specified in the view.
    /// Returns NVN_FALSE if no set of layers has been specified.
    ///
    /// \param[out] minLayer The minimum layer number in a view of a
    /// Texture object.  Zero is returned if no set of layers has been
    /// specified.
    ///
    /// \param[out] numLayers The number of layers in a view of a Texture
    /// object.  Zero is returned if no set of layers has been specified.
    inline NVNboolean GetLayers(int *minLayer,
                                int *numLayers) const;

    /// \brief Query the format associated with a TextureView object.
    ///
    /// Returns NVN_TRUE if a format has been specified in the view.
    /// Returns NVN_FALSE if no format has been specified.
    ///
    /// \param[out] format Format in the texture view.  Format::NONE is
    /// returned if no format has been specified.
    inline NVNboolean GetFormat(Format *format) const;

    /// \brief Query the swizzles associated with a TextureView object.
    ///
    /// Returns NVN_TRUE if swizzles have been specified in the view.
    /// Returns NVN_FALSE if no swizzles have been specified.
    ///
    /// \param[out] r Input component value used for the first (R) output
    /// component.  TextureSwizzle::ZERO is returned if no swizzles have
    /// been specified.
    ///
    /// \param[out] g Input component value used for the second (G) output
    /// component.  TextureSwizzle::ZERO is returned if no swizzles have
    /// been specified.
    ///
    /// \param[out] b Input component value used for the third (B) output
    /// component.  TextureSwizzle::ZERO is returned if no swizzles have
    /// been specified.
    ///
    /// \param[out] a Input component value used for the fourth (A) output
    /// component.  TextureSwizzle::ZERO is returned if no swizzles have
    /// been specified.
    inline NVNboolean GetSwizzle(TextureSwizzle *r,
                                 TextureSwizzle *g,
                                 TextureSwizzle *b,
                                 TextureSwizzle *a) const;

    /// \brief Query the depth/stencil mode associated with a TextureView
    /// object.
    ///
    /// Returns NVN_TRUE if a depth/stencil mode has been specified in the
    /// view. Returns NVN_FALSE if no depth/stencil mode has been
    /// specified.
    ///
    /// \param[out] mode Selector to return depth or stencil values from a
    /// texture with depth and/or stencil components.
    /// TextureDepthStencilMode::DEPTH is returned if no depth/stencil mode
    /// has been specified.
    inline NVNboolean GetDepthStencilMode(TextureDepthStencilMode *mode) const;

    /// \brief Query the target associated with a TextureView object.
    ///
    /// Returns NVN_TRUE if a target has been specified in the view.
    /// Returns NVN_FALSE if no target has been specified.
    ///
    /// \param[out] target Target associated with the texture view.
    /// TextureTarget::TARGET_1D is returned if no target has been
    /// specified.
    inline NVNboolean GetTarget(TextureTarget *target) const;

    /// \brief Checks if TextureView objects are equivalent.
    ///
    /// \param otherView TextureView object to compare against.
    inline NVNboolean Compare(const TextureView *otherView) const;

} NVN_POST_ALIGN(8);

/// \brief Object specifying state used to construct new sampler objects.
NVN_PRE_ALIGN(8)
class SamplerBuilder {
protected:
    char reserved[96];
public:

    /// \brief Set the Device associated with a SamplerBuilder object.
    ///
    /// \param device Device owning Sampler objects created by the
    /// SamplerBuilder.
    inline SamplerBuilder & SetDevice(Device *device);

    /// \brief Set default state for the SamplerBuilder object.
    ///
    /// This method does not modify the device associated with the builder,
    /// which is programmed by SetDevice.
    ///
    /// \note This is semantically equivalent to calling
    ///
    /// \code{.c}
    ///   SamplerBuilder::SetMinMagFilter(NVN_MIN_FILTER_LINEAR, NVN_MAG_FILTER_LINEAR);
    ///   SamplerBuilder::SetWrapMode(NVN_WRAP_MODE_REPEAT, NVN_WRAP_MODE_REPEAT,
    ///                               NVN_WRAP_MODE_REPEAT);
    ///   SamplerBuilder::SetLodClamp(0.0, 1000.0);
    ///   SamplerBuilder::SetLodBias(0.0);
    ///   SamplerBuilder::SetCompare(NVN_COMPARE_MODE_NONE, NVN_COMPARE_FUNC_LESS);
    ///   SamplerBuilder::SetBorderColor({ 0.0, 0.0, 0.0, 0.0 });
    ///   SamplerBuilder::SetMaxAnisotropy(1.0);
    ///   SamplerBuilder::SetReductionFilter(NVN_SAMPLER_REDUCTION_AVERAGE);
    ///   SamplerBuilder::SetLodSnap(0.0);
    /// \endcode
    inline SamplerBuilder & SetDefaults();

    /// \brief Set the minification and magnification filters for Sampler
    /// objects created from the SamplerBuilder.
    ///
    /// \param min Specifies the minification filter.
    ///
    /// \param mag Specifies the magnification filter.
    inline SamplerBuilder & SetMinMagFilter(MinFilter min,
                                            MagFilter mag);

    /// \brief Set the coordinate wrap modes for Sampler objects created
    /// from the SamplerBuilder.
    ///
    /// \param s Specifies the wrap mode for the first (S) texture
    /// coordinate.
    ///
    /// \param t Specifies the wrap mode for the second (T) texture
    /// coordinate.
    ///
    /// \param r Specifies the wrap mode for the third (R) texture
    /// coordinate.
    inline SamplerBuilder & SetWrapMode(WrapMode s,
                                        WrapMode t,
                                        WrapMode r);

    /// \brief Set the minimum and maximum level of detail used for Sampler
    /// objects created from the SamplerBuilder.
    ///
    /// When a sampler builder is used to initialize a sampler object,
    /// _min_ will be clamped to [0.0, 15.0], and _max_ will be clamped to
    /// [max(0.0, _min_), 15.0].
    ///
    /// \param min Specifies the minimum level of detail used with the
    /// sampler.
    /// \li Must be less than or equal to max.
    ///
    /// \param max Specifies the maximum level of detail used with the
    /// sampler.
    /// \li Must be greater than or equal to min.
    inline SamplerBuilder & SetLodClamp(float min,
                                        float max);

    /// \brief Set the level-of-detail bias used for Sampler objects
    /// created from the SamplerBuilder.
    ///
    /// When a sampler builder is used to initialize a sampler object,
    /// _bias_ will be clamped to [-16.0, 15.996].
    ///
    /// \param bias Specifies the bias to add to the computed level of
    /// detail when the sampler is used.
    inline SamplerBuilder & SetLodBias(float bias);

    /// \brief Set the depth comparison mode used for Sampler objects
    /// created from the SamplerBuilder.
    ///
    /// When used with textures without depth components, the depth
    /// comparison mode is ignored.
    ///
    /// \param mode Specifies whether depth comparisons are performed when
    /// using the sampler.
    ///
    /// \param func If depth comparisons are enabled, specifies the
    /// comparison function used with the sampler.
    inline SamplerBuilder & SetCompare(CompareMode mode,
                                       CompareFunc func);

    /// \brief Set floating-point border color values used for Sampler
    /// objects created from the SamplerBuilder.
    ///
    /// \param borderColor Specifies four floating-point components
    /// (R,G,B,A) for the border color.
    inline SamplerBuilder & SetBorderColor(const float *borderColor);

    /// \brief Set signed integer border color values used for Sampler
    /// objects created from the SamplerBuilder.
    ///
    /// \param borderColor Specifies four signed integer components
    /// (R,G,B,A) for the border color.
    inline SamplerBuilder & SetBorderColori(const int *borderColor);

    /// \brief Set unsigned integer border color values used for Sampler
    /// objects created from the SamplerBuilder.
    ///
    /// \param borderColor Specifies four unsigned integer components
    /// (R,G,B,A) for the border color.
    inline SamplerBuilder & SetBorderColorui(const uint32_t *borderColor);

    /// \brief Set the maximum level of anisotropy used for Sampler objects
    /// created from the SamplerBuilder.
    ///
    /// \param maxAniso Specifies the maximum level of anisotropy used with
    /// the sampler.
    /// Must be greater than zero.
    /// Must be less than or equal to 16 (the value of
    /// nvn::DeviceInfo::MAX_TEXTURE_ANISOTROPY).
    inline SamplerBuilder & SetMaxAnisotropy(float maxAniso);

    /// \brief Set the reduction filter used for Sampler objects created
    /// from the SamplerBuilder.
    ///
    /// \param filter Specifies the reduction filter used for the sampler.
    inline SamplerBuilder & SetReductionFilter(SamplerReduction filter);

    /// \brief Set LOD snap used for Sampler objects created from the
    /// SamplerBuilder.
    ///
    /// The texture LOD snap feature is an optimization that reduces
    /// texture bandwidth consumption with minification filters that
    /// average samples in multiple texture LODs. When performing texture
    /// lookups, hardware will round (snap) the computed texture LOD to the
    /// nearest integer if the difference between the computed LOD and the
    /// integer value is less than the snap value in the sampler. LOD
    /// values that are not snapped are adjusted so that the function
    /// adjusting the LOD value is continuous. When snapping occurs,
    /// hardware will save bandwidth by sampling from only one mipmap level
    /// instead of two.
    ///
    /// \param lodSnap Specifies the LOD snap used for the sampler.
    /// Specifies the LOD snap used for the sampler. Valid values are in
    /// the range [0.0, 0.5], where 0.0 will result in no snapping and 0.5
    /// would round most computed LODs to integer values. Not all values
    /// are supported by the hardware; the driver will select a supported
    /// snap value close to the value provided in the sampler.
    inline SamplerBuilder & SetLodSnap(float lodSnap);

    /// \brief Query the device associated with this SamplerBuilder object.
    inline const Device * GetDevice() const;

    /// \brief Query the minification and magnification filters for a
    /// SamplerBuilder object.
    ///
    /// \param[out] min Minification filter for the SamplerBuilder.
    ///
    /// \param[out] mag Magnification filter for the SamplerBuilder.
    inline void GetMinMagFilter(MinFilter *min,
                                MagFilter *mag) const;

    /// \brief Query the coordinate wrap modes for a SamplerBuilder object.
    ///
    /// \param[out] s Wrap mode for the first (S) texture coordinate.
    ///
    /// \param[out] t Wrap mode for the second (T) texture coordinate.
    ///
    /// \param[out] r Wrap mode for the third (R) texture coordinate.
    inline void GetWrapMode(WrapMode *s,
                            WrapMode *t,
                            WrapMode *r) const;

    /// \brief Query the minimum and maximum level of detail in a
    /// SamplerBuilder object.
    ///
    /// \param[out] min Minimum level of detail used with the sampler.
    ///
    /// \param[out] max Maximum level of detail used with the sampler.
    inline void GetLodClamp(float *min,
                            float *max) const;

    /// \brief Query the level-of-detail bias for a SamplerBuilder object.
    inline float GetLodBias() const;

    /// \brief Query the depth comparison modes for a SamplerBuilder
    /// object.
    ///
    /// \param[out] mode Enable for depth comparisons in the
    /// SamplerBuilder.
    ///
    /// \param[out] func Comparison function in the SamplerBuilder.
    inline void GetCompare(CompareMode *mode,
                           CompareFunc *func) const;

    /// \brief Query floating-point border color values for a
    /// SamplerBuilder object.
    ///
    /// \param[out] borderColor Four floating-point components (R,G,B,A)
    /// for the border color.
    inline void GetBorderColor(float *borderColor) const;

    /// \brief Query signed integer border color values for a
    /// SamplerBuilder object.
    ///
    /// \param[out] borderColor Four signed integer components (R,G,B,A)
    /// for the border color.
    inline void GetBorderColori(int *borderColor) const;

    /// \brief Query unsigned integer border color values for a
    /// SamplerBuilder object.
    ///
    /// \param[out] borderColor Four unsigned integer components (R,G,B,A)
    /// for the border color.
    inline void GetBorderColorui(uint32_t *borderColor) const;

    /// \brief Query the maximum level of anisotropy for a SamplerBuilder
    /// object.
    inline float GetMaxAnisotropy() const;

    /// \brief Query the reduction filter for a SamplerBuilder object.
    inline SamplerReduction GetReductionFilter() const;

    /// \brief Query the LOD snap for a SamplerBuilder object.
    inline float GetLodSnap() const;

} NVN_POST_ALIGN(8);

/// \brief GPU-accessible object controlling how texture data are fetched
/// and filtered.
NVN_PRE_ALIGN(8)
class Sampler {
protected:
    char reserved[96];
    Sampler(const Sampler &other) /* disabled */;
    const Sampler & operator =(const Sampler &other) /* disabled */;
public:
    Sampler() {}

    /// \brief Initialize a Sampler object using the specified
    /// SamplerBuilder object.
    ///
    /// \param builder SamplerBuilder object specifying the state of the
    /// new sampler object.
    inline NVNboolean Initialize(const SamplerBuilder *builder);

    /// \brief Finalize a Sampler object.
    inline void Finalize();

    /// \brief Sets the debug label string for a Sampler object.
    ///
    /// Annotates a Sampler object with a descriptive text label. This
    /// label may then be used by the debug layer, an external debugger, or
    /// a profiler tool.
    ///
    /// \param label Text string used to annotate the Sampler object.
    inline void SetDebugLabel(const char * label);

    /// \brief Query the minification and magnification filters for a
    /// Sampler object.
    ///
    /// \param[out] min Minification filter for the Sampler.
    ///
    /// \param[out] mag Magnification filter for the Sampler.
    inline void GetMinMagFilter(MinFilter *min,
                                MagFilter *mag) const;

    /// \brief Query the coordinate wrap modes for a Sampler object.
    ///
    /// \param[out] s Wrap mode for the first (S) texture coordinate.
    ///
    /// \param[out] t Wrap mode for the second (T) texture coordinate.
    ///
    /// \param[out] r Wrap mode for the third (R) texture coordinate.
    inline void GetWrapMode(WrapMode *s,
                            WrapMode *t,
                            WrapMode *r) const;

    /// \brief Query the minimum and maximum level of detail in a Sampler
    /// object.
    ///
    /// \param[out] min Minimum level of detail used with the sampler.
    ///
    /// \param[out] max Maximum level of detail used with the sampler.
    inline void GetLodClamp(float *min,
                            float *max) const;

    /// \brief Query the level-of-detail bias for a Sampler object.
    inline float GetLodBias() const;

    /// \brief Query the depth comparison modes for a Sampler object.
    ///
    /// \param[out] mode Enable for depth comparisons in the Sampler.
    ///
    /// \param[out] func Comparison function in the Sampler.
    inline void GetCompare(CompareMode *mode,
                           CompareFunc *func) const;

    /// \brief Query floating-point border color values for a Sampler
    /// object.
    ///
    /// \param[out] borderColor Four floating-point components (R,G,B,A)
    /// for the border color.
    inline void GetBorderColor(float *borderColor) const;

    /// \brief Query signed integer border color values for a Sampler
    /// object.
    ///
    /// \param[out] borderColor Four signed integer components (R,G,B,A)
    /// for the border color.
    inline void GetBorderColori(int *borderColor) const;

    /// \brief Query unsigned integer border color values for a Sampler
    /// object.
    ///
    /// \param[out] borderColor Four unsigned integer components (R,G,B,A)
    /// for the border color.
    inline void GetBorderColorui(uint32_t *borderColor) const;

    /// \brief Query the maximum level of anisotropy for a Sampler object.
    inline float GetMaxAnisotropy() const;

    /// \brief Query the reduction filter for a Sampler object.
    inline SamplerReduction GetReductionFilter() const;

    /// \brief Checks if sampler objects are equivalent.
    ///
    /// \param otherSampler Sampler object to compare against.
    inline NVNboolean Compare(const Sampler *otherSampler) const;

    /// \brief Returns the debug layer unique ID of this sampler. Returns 0
    /// if debug layer disabled.
    inline uint64_t GetDebugID() const;

} NVN_POST_ALIGN(8);

/// \brief Synchronization primitive object used to order execution of
/// commands and track progress.
NVN_PRE_ALIGN(8)
class Sync {
protected:
    char reserved[64];
    Sync(const Sync &other) /* disabled */;
    const Sync & operator =(const Sync &other) /* disabled */;
public:
    Sync() {}

    /// \brief Initialize a Sync object for the specified device.
    ///
    /// \param device Device owning the Sync object.
    inline NVNboolean Initialize(Device *device);

    /// \brief Finalize a Sync object.
    inline void Finalize();

    /// \brief Sets the debug label string for a Sync object.
    ///
    /// Annotates a Sync object with a descriptive text label. This label
    /// may then be used by the debug layer, an external debugger, or a
    /// profiler tool.
    ///
    /// \param label Text string used to annotate the Sync object.
    inline void SetDebugLabel(const char * label);

    /// \brief Wait for a Sync object to be signaled on the CPU before
    /// returning.
    ///
    /// The value returned indicates the status of the Sync object when the
    /// command returns.
    ///
    /// \param timeoutNs Minimum time (in nanoseconds) to wait for the Sync
    /// object to be signaled.
    inline SyncWaitResult Wait(uint64_t timeoutNs) const;

    /// \brief Use a fenced and flushed GLsync object to initialize a NVN
    /// Sync object.
    ///
    /// Use this function instead of Initialize for NVN Sync objects
    /// created from GLsync objects. This function initializes a Sync
    /// object that signals after a corresponding GLsync object. This
    /// enables OpenGL to NVN synchronization (fencing is done on the
    /// OpenGL object and waiting on its NVN counterpart). The value
    /// returned indicates the status of the Sync object. See
    /// nvn::Sync::CreateGLSync.
    ///
    /// \param device Device owning the Sync object.
    ///
    /// \param glSync The name of the OpenGL or OpenGL ES sync to use as a
    /// source.
    /// \li The GLsync object must be a valid OpenGL or OpenGL ES GLsync
    /// object.
    /// \li The GLsync object must have already been flushed in the GL
    /// command stream.
    /// \li It needs to be valid for the lifetime of the Sync object.
    ///
    /// \param[out] status Detailed error status.
    inline NVNboolean InitializeFromFencedGLSync(Device *device,
                                                 const void *glSync,
                                                 SyncInteropStatus *status);

    /// \brief Use a fenced and flushed NVN Sync object to create a GLsync.
    ///
    /// This function initializes a GLsync object that signals after a
    /// corresponding NVN sync object. This enables NVN to OpenGL
    /// synchronization (fencing is done on the NVN Sync object and waiting
    /// on its GLsync counterpart).
    /// The value returned indicates the status of the Sync object. See
    /// nvn::Sync::InitializeFromFencedGLSync.
    ///
    /// \param[out] glSync The GLsync to initialize, cast to uint64_t*.
    inline SyncInteropStatus CreateGLSync(uint64_t *glSync) const;

} NVN_POST_ALIGN(8);

/// \brief Object that manages texture presentation to the platform
/// windowing system.
NVN_PRE_ALIGN(8)
class Window {
protected:
    char reserved[384];
    Window(const Window &other) /* disabled */;
    const Window & operator =(const Window &other) /* disabled */;
public:
    Window() {}

    /// \brief Initialize a new window object from a window builder.
    ///
    /// \param builder WindowBuilder object holding properties of the new
    /// Window object.
    inline NVNboolean Initialize(const WindowBuilder *builder);

    /// \brief Finalize a Window object.
    inline void Finalize();

    /// \brief Sets the debug label string for a Window object.
    ///
    /// Annotates a Window object with a descriptive text label. This label
    /// may then be used by the debug layer, an external debugger, or a
    /// profiler tool.
    ///
    /// \param label Text string used to annotate the Window object.
    inline void SetDebugLabel(const char * label);

    /// \brief Acquire a texture from the window.
    ///
    /// This function will block until the GPU has completed rendering the
    /// previous frame to the acquired texture, but may return before the
    /// acquired texture is ready for rendering the next frame.  The sync
    /// object _textureAvailableSync_ can be used to determine when the
    /// acquired texture can be rendered to.
    ///
    /// The caller is responsible for ensuring that it doesn't render to
    /// the acquired texture prematurely.  Passing _textureAvailableSync_
    /// to Queue::WaitSync ensures that any subsequent commands sent to a
    /// queue are not processed by the GPU until the acquired texture is
    /// ready for use.
    ///
    /// Applications should also use _textureAvailableSync_ to match their
    /// rendering rate to the presentation rate.  This can be done by
    /// calling nvn::Sync::Wait on _textureAvailableSync_ before calling
    /// nvn::Queue::PresentTexture.
    ///
    /// \note Waiting for _textureAvailableSync_ does not provide any
    /// guarantees regarding GPU work completion.  For example, when double
    /// buffering between textures A and B, GPU may still be rendering
    /// texture B when A has become available for rendering (i.e.
    /// _textureAvailableSync_ associated with acquiring texture A has been
    /// signaled).
    ///
    /// \param textureAvailableSync Sync object that will be signaled when
    /// the texture is available for rendering a new frame.
    /// If the texture has not been presented before with
    /// Queue::PresentTexture, _textureAvailableSync_ will be signaled
    /// immediately.  If Window::AcquireTexture returns an error,
    /// _textureAvailableSync_ will not be modified.
    ///
    /// \param[out] textureIndex An index into the texture array that was
    /// passed in when _window_ was initialized, which identifies the
    /// texture that is now released from the platform windowing system to
    /// the application.
    inline WindowAcquireTextureResult AcquireTexture(Sync *textureAvailableSync,
                                                     int *textureIndex);

    /// \brief Query the native window for a Window object.
    inline NVNnativeWindow GetNativeWindow() const;

    /// \brief Query the present interval for a Window object.
    inline int GetPresentInterval() const;

    /// \brief Sets the present interval for a Window object.
    ///
    /// Sets the present interval for the window object. The present
    /// interval specifies the minimum number of display refresh between
    /// frames when presenting to a Window object.  Legal values are in the
    /// range [0,MAX_PRESENT_INTERVAL]:
    ///
    /// * 0:  Update the display without waiting for the next refresh.
    /// This will display frames as fast as possible but will likely result
    /// in visual tearing.
    /// * 1:  Update the display on the next refresh.  This limits the
    /// frame rate to the display's refresh rate.
    /// * _n_ (> 1):  Update the display on the next refresh, but no sooner
    /// than _n_ refreshes since the previous update. This limits the frame
    /// rate to 1 / _n_ of the display's refresh rate.
    ///
    /// \note The production environment will not support a present
    /// interval of 0. In that environment, setting a present interval of 0
    /// will behave as though 1 were specified.
    ///
    /// \note On the Windows reference implementation, calling
    /// SetPresentInterval with an interval different than 1 is only
    /// supported if the application provides a native window to the
    /// WindowBuilder.
    ///
    /// \note On NX, the display refresh rate will always be 60Hz. On the
    /// Windows reference implementation, the refresh rate depend on the
    /// capabilities of the connected monitor and might be in some cases
    /// different than 60Hz.
    ///
    /// \param presentInterval Present interval to use.
    /// Must be greater than or equal to zero.
    /// Must be less than or equal to 5 (the value of
    /// nvn::DeviceInfo::MAX_PRESENT_INTERVAL).
    inline void SetPresentInterval(int presentInterval);

    /// \brief Defines the rectangle inside the window texture that gets
    /// displayed.
    ///
    /// This method sets the rectangle inside a texture that gets
    /// displayed. If the rectangle size does not match the window size,
    /// the texture rectangle gets scaled to fit the window size. Passing a
    /// width or height of zero will reset the crop rectangle to match the
    /// dimension of the window texture.
    ///
    /// \param x Smallest X coordinate of the display rectangle in window
    /// coordinates.
    ///
    /// \param y Smallest Y coordinate of the display rectangle in window
    /// coordinates.
    ///
    /// \param w Width of the display rectangle.
    ///
    /// \param h Height of the display rectangle.
    inline void SetCrop(int x,
                        int y,
                        int w,
                        int h);

    /// \brief Returns the dimensions of the crop rectangle that defines
    /// the region inside the window texture which gets displayed.
    ///
    /// \param[out] crop The crop rectangle that is used for presenting.
    inline void GetCrop(Rectangle *crop) const;

    /// \brief Set the number of textures for presenting to this window.
    ///
    /// SetNumActiveTextures takes affect after numTextures - 1 frames in
    /// latest case.
    /// \warning This function must not be called while the app is holding
    /// any texture which is before presented to the window after acquiring
    /// it.
    ///
    /// \param numActiveTextures Number of color textures to use. Must be
    /// less than or equal to the number of textures set by
    /// nvn::WindowBuilder::SetTextures.
    /// Must be greater than or equal to 2 (the value of
    /// nvn::DeviceInfo::MIN_TEXTURES_PER_WINDOW).
    /// Must be less than or equal to 4 (the value of
    /// nvn::DeviceInfo::MAX_TEXTURES_PER_WINDOW).
    inline void SetNumActiveTextures(int numActiveTextures);

    /// \brief Query the number of active textures for presenting to this
    /// window.
    ///
    /// This value reflects the value set by
    /// nvn::Window::SetNumActiveTextures immediately after calling the
    /// function but the actual change of the frame buffer behavior may
    /// take more time up to numTextures - 1 frames.
    inline int GetNumActiveTextures() const;

    /// \brief Query the maximum available number of textures for
    /// presenting to this window.
    ///
    /// This value is the same number of preallocated textures by
    /// nvn::WindowBuilder::SetTextures
    inline int GetNumTextures() const;

} NVN_POST_ALIGN(8);

/// \brief Object specifying state used to construct new window objects.
NVN_PRE_ALIGN(8)
class WindowBuilder {
protected:
    char reserved[64];
public:

    /// \brief Set the Device associated with a WindowBuilder object.
    ///
    /// \param device Device owning Window objects created by the
    /// WindowBuilder.
    inline WindowBuilder & SetDevice(Device *device);

    /// \brief Set default state for the WindowBuilder object.
    ///
    /// This method does not modify the device associated with the builder,
    /// which is programmed by SetDevice.
    ///
    /// \note This is semantically equivalent to calling
    ///
    /// \code{.c}
    ///   WindowBuilder::SetNativeWindow(NULL);
    ///   WindowBuilder::SetTextures(0, NULL);
    ///   WindowBuilder::SetPresentInterval(1);
    /// \endcode
    inline WindowBuilder & SetDefaults();

    /// \brief Set the native window where the textures of this window will
    /// be presented to.
    ///
    /// On NX, the native window should be of type
    /// nn::vi::NativeWindowHandle.
    ///
    /// On Windows, the native window (if provided) should be of type HWND
    /// and applications must have set a pixel format using the Windows
    /// `SetPixelFormat` API. The pixel format must be an RGBA format that
    /// supports NVIDIA hardware acceleration and has the
    /// `PFD_DRAW_TO_WINDOW`, `PFD_SUPPORT_OPENGL`, and `PFD_DOUBLEBUFFER`
    /// properties set. The window class that was registered using the
    /// Windows `RegisterClass` function, needs to have the CS_OWNDC flag
    /// set. If no native window is provided on Windows, applications must
    /// bind a "donor" OpenGL context and drawable using `wglMakeCurrent`
    /// before calling nvn::Queue::PresentTexture.
    ///
    /// \param nativeWindow NativeWindow where the textures of this window
    /// will be presented to.
    inline WindowBuilder & SetNativeWindow(NVNnativeWindow nativeWindow);

    /// \brief Sets the textures used for presenting to this window.
    ///
    /// The textures must specify TextureFlags::DISPLAY, and must not be
    /// finalized before the nvn::Window is finalized.
    ///
    /// For additional restrictions such as a list of supported formats,
    /// refer to the NVN programming guide.
    ///
    /// \param numTextures Number of color textures to use.
    /// Must be greater than or equal to 2 (the value of
    /// nvn::DeviceInfo::MIN_TEXTURES_PER_WINDOW).
    /// Must be less than or equal to 4 (the value of
    /// nvn::DeviceInfo::MAX_TEXTURES_PER_WINDOW).
    ///
    /// \param textures Array of numTextures pointers for color textures.
    /// The pointer must remain valid until the window is initialized with
    /// Window::Initialize.
    inline WindowBuilder & SetTextures(int numTextures,
                                       Texture * const *textures);

    /// \brief Sets the present interval used when presenting to this
    /// window.
    ///
    /// Sets the present interval for the window object. The present
    /// interval specifies the minimum number of display refresh between
    /// frames when presenting to a Window object.  Legal values are in the
    /// range [0,MAX_PRESENT_INTERVAL]:
    ///
    /// * 0:  Update the display without waiting for the next refresh.
    /// This will display frames as fast as possible but will likely result
    /// in visual tearing.
    /// * 1:  Update the display on the next refresh.  This limits the
    /// frame rate to the display's refresh rate.
    /// * _n_ (> 1):  Update the display on the next refresh, but no sooner
    /// than _n_ refreshes since the previous update. This limits the frame
    /// rate to 1 / _n_ of the display's refresh rate.
    ///
    /// \note The production environment will not support a present
    /// interval of 0. In that environment, setting a present interval of 0
    /// will behave as though 1 were specified.
    ///
    /// \note On the Windows reference implementation, calling
    /// SetPresentInterval with an interval different than 1 is only
    /// supported if the application provides a native window to the
    /// WindowBuilder.
    ///
    /// \note On NX, the display refresh rate will always be 60Hz. On the
    /// Windows reference implementation, the refresh rate depend on the
    /// capabilities of the connected monitor and might be in some cases
    /// different than 60Hz.
    ///
    /// \param presentInterval Present interval to use.
    /// Must be greater than or equal to zero.
    /// Must be less than or equal to 5 (the value of
    /// nvn::DeviceInfo::MAX_PRESENT_INTERVAL).
    inline WindowBuilder & SetPresentInterval(int presentInterval);

    /// \brief Set the number of textures for presenting to this window.
    ///
    /// \param numActiveTextures Number of color textures to use. Must be
    /// less than or equal the number of textures set by
    /// nvn::WindowBuilder::SetTextures.
    /// Must be greater than or equal to 2 (the value of
    /// nvn::DeviceInfo::MIN_TEXTURES_PER_WINDOW).
    /// Must be less than or equal to 4 (the value of
    /// nvn::DeviceInfo::MAX_TEXTURES_PER_WINDOW).
    inline WindowBuilder & SetNumActiveTextures(int numActiveTextures);

    /// \brief Query the device associated with this WindowBuilder object.
    inline const Device * GetDevice() const;

    /// \brief Query the number of textures associated with this
    /// WindowBuilder object.
    inline int GetNumTextures() const;

    /// \brief Returns the texture with index textureId associated with
    /// this WindowBuilder object.
    ///
    /// The texture array stored in the WindowBuilder object is supposed to
    /// remain valid until the window is initialized with
    /// Window::Initialize. Therefore GetTexture should be called before
    /// the window is initialized to make sure it returns a valid pointer.
    ///
    /// \param textureId Index of the texture to be returned.
    inline const Texture * GetTexture(int textureId) const;

    /// \brief Query the native window for a WindowBuilder object.
    inline NVNnativeWindow GetNativeWindow() const;

    /// \brief Query the present interval for a WindowBuilder object.
    inline int GetPresentInterval() const;

    /// \brief Query the number of active textures for presenting to this
    /// window
    inline int GetNumActiveTextures() const;

} NVN_POST_ALIGN(8);

/// \brief Object specifying state used to construct new event objects.
NVN_PRE_ALIGN(8)
class EventBuilder {
protected:
    char reserved[32];
public:

    /// \brief Set default state for the EventBuilder object.
    inline EventBuilder & SetDefaults();

    /// \brief Set the location in a MemoryPool used to hold the state of
    /// an Event created from the EventBuilder.
    ///
    /// Each Event object created by an event builder has a 32-bit unsigned
    /// integer value stored in GPU-accessible memory in a memory pool.
    /// API commands are provided to read and write to this value and to
    /// wait for it to reach a specified value.  When an Event object is
    /// initialized, its initial value comes from the current contents of
    /// the associated storage.
    ///
    /// \param pool Memory pool used to hold the state of the event.
    /// If the event is signaled or queried by the CPU, a CPU_UNCACHED
    /// memory pool is recommended.  If a CPU_CACHED memory pool is used,
    /// the event object's memory must be flushed from the CPU data cache
    /// using MemoryPool::FlushMappedRange after calling Event::Signal and
    /// must be invalidated from the CPU data cache using
    /// MemoryPool::InvalidateMappedRange before calling Event::GetValue.
    /// Memory pools created with the VIRTUAL, PHYSICAL, and GPU_NO_ACCESS
    /// flags are not supported. Memory pools created with GPU_CACHED are
    /// only supported if the pool also uses CPU_NO_ACCESS. Memory pools
    /// created with CPU_CACHED are not supported on the Windows reference
    /// implementation.
    ///
    /// \param offset offset in _pool_ for event write back location.
    /// _offset_ must be aligned to the size of an event which is a 32-bit
    /// unsigned integer.
    inline EventBuilder & SetStorage(const MemoryPool *pool,
                                     int64_t offset);

    /// \brief Get the memory pool and the offset associated with this
    /// EventBuilder object.
    ///
    /// \param[out] offset Offset into the memory pool.
    inline const MemoryPool * GetStorage(int64_t *offset) const;

} NVN_POST_ALIGN(8);

/// \brief Event object.
NVN_PRE_ALIGN(8)
class Event {
protected:
    char reserved[64];
    Event(const Event &other) /* disabled */;
    const Event & operator =(const Event &other) /* disabled */;
public:
    Event() {}

    /// \brief Initialize an Event object.
    ///
    /// \param builder EventBuilder object to use.
    inline NVNboolean Initialize(const EventBuilder *builder);

    /// \brief Finalize a Event object.
    inline void Finalize();

    /// \brief Get the value associated with an Event object.
    inline uint32_t GetValue() const;

    /// \brief Use the CPU to signal an NVNevent object.
    ///
    /// \param mode Event signal mode.
    ///
    /// \param value Signal value.
    inline void Signal(EventSignalMode mode,
                       uint32_t value);

} NVN_POST_ALIGN(8);

/// \}
/// \addtogroup nvn_cpp_entryfuncptrs
/// \{

typedef void (NVNAPIENTRY *DeviceBuilderSetDefaultsFunc) (DeviceBuilder *builder);
typedef void (NVNAPIENTRY *DeviceBuilderSetFlagsFunc) (DeviceBuilder *builder, int flags);
typedef int (NVNAPIENTRY *DeviceBuilderGetFlagsFunc) (const DeviceBuilder *builder);
typedef NVNboolean (NVNAPIENTRY *DeviceInitializeFunc) (Device *device, const DeviceBuilder *builder);
typedef void (NVNAPIENTRY *DeviceFinalizeFunc) (Device *device);
typedef void (NVNAPIENTRY *DeviceSetDebugLabelFunc) (Device *device, const char * label);
typedef GenericFuncPtrFunc (NVNAPIENTRY *DeviceGetProcAddressFunc) (const Device *device, const char * name);
typedef void (NVNAPIENTRY *DeviceGetIntegerFunc) (const Device *device, DeviceInfo::Enum pname, int *v);
typedef uint64_t (NVNAPIENTRY *DeviceGetCurrentTimestampInNanosecondsFunc) (const Device *device);
typedef void (NVNAPIENTRY *DeviceSetIntermediateShaderCacheFunc) (Device *device, int numMaxEntries);
typedef TextureHandle (NVNAPIENTRY *DeviceGetTextureHandleFunc) (const Device *device, int textureID, int samplerID);
typedef TextureHandle (NVNAPIENTRY *DeviceGetTexelFetchHandleFunc) (const Device *device, int textureID);
typedef ImageHandle (NVNAPIENTRY *DeviceGetImageHandleFunc) (const Device *device, int imageID);
typedef void (NVNAPIENTRY *DeviceInstallDebugCallbackFunc) (Device *device, const DebugCallbackFunc callback, void *callbackData, NVNboolean enable);
typedef NVNdebugDomainId (NVNAPIENTRY *DeviceGenerateDebugDomainIdFunc) (const Device *device, const char * name);
typedef void (NVNAPIENTRY *DeviceSetWindowOriginModeFunc) (Device *device, WindowOriginMode::Enum windowOriginMode);
typedef void (NVNAPIENTRY *DeviceSetDepthModeFunc) (Device *device, DepthMode::Enum depthMode);
typedef NVNboolean (NVNAPIENTRY *DeviceRegisterFastClearColorFunc) (Device *device, const float *color, Format::Enum format);
typedef NVNboolean (NVNAPIENTRY *DeviceRegisterFastClearColoriFunc) (Device *device, const int *color, Format::Enum format);
typedef NVNboolean (NVNAPIENTRY *DeviceRegisterFastClearColoruiFunc) (Device *device, const uint32_t *color, Format::Enum format);
typedef NVNboolean (NVNAPIENTRY *DeviceRegisterFastClearDepthFunc) (Device *device, float depth);
typedef WindowOriginMode::Enum (NVNAPIENTRY *DeviceGetWindowOriginModeFunc) (const Device *device);
typedef DepthMode::Enum (NVNAPIENTRY *DeviceGetDepthModeFunc) (const Device *device);
typedef uint64_t (NVNAPIENTRY *DeviceGetTimestampInNanosecondsFunc) (const Device *device, const CounterData *counterData);
typedef void (NVNAPIENTRY *DeviceApplyDeferredFinalizesFunc) (Device *device, int age);
typedef void (NVNAPIENTRY *DeviceFinalizeCommandHandleFunc) (Device *device, CommandHandle handles);
typedef void (NVNAPIENTRY *DeviceWalkDebugDatabaseFunc) (const Device *device, DebugObjectType::Enum type, WalkDebugDatabaseCallbackFunc callback, void *userParam);
typedef SeparateTextureHandle (NVNAPIENTRY *DeviceGetSeparateTextureHandleFunc) (const Device *device, int textureID);
typedef SeparateSamplerHandle (NVNAPIENTRY *DeviceGetSeparateSamplerHandleFunc) (const Device *device, int samplerID);
typedef NVNboolean (NVNAPIENTRY *DeviceIsExternalDebuggerAttachedFunc) (const Device *device);
typedef QueueGetErrorResult::Enum (NVNAPIENTRY *QueueGetErrorFunc) (Queue *queue, QueueErrorInfo *errorInfo);
typedef size_t (NVNAPIENTRY *QueueGetTotalCommandMemoryUsedFunc) (Queue *queue);
typedef size_t (NVNAPIENTRY *QueueGetTotalControlMemoryUsedFunc) (Queue *queue);
typedef size_t (NVNAPIENTRY *QueueGetTotalComputeMemoryUsedFunc) (Queue *queue);
typedef void (NVNAPIENTRY *QueueResetMemoryUsageCountsFunc) (Queue *queue);
typedef void (NVNAPIENTRY *QueueBuilderSetDeviceFunc) (QueueBuilder *builder, Device *device);
typedef void (NVNAPIENTRY *QueueBuilderSetDefaultsFunc) (QueueBuilder *builder);
typedef void (NVNAPIENTRY *QueueBuilderSetFlagsFunc) (QueueBuilder *builder, int flags);
typedef void (NVNAPIENTRY *QueueBuilderSetCommandMemorySizeFunc) (QueueBuilder *builder, size_t size);
typedef void (NVNAPIENTRY *QueueBuilderSetComputeMemorySizeFunc) (QueueBuilder *builder, size_t size);
typedef void (NVNAPIENTRY *QueueBuilderSetControlMemorySizeFunc) (QueueBuilder *builder, size_t size);
typedef size_t (NVNAPIENTRY *QueueBuilderGetQueueMemorySizeFunc) (const QueueBuilder *builder);
typedef void (NVNAPIENTRY *QueueBuilderSetQueueMemoryFunc) (QueueBuilder *builder, void *memory, size_t size);
typedef void (NVNAPIENTRY *QueueBuilderSetCommandFlushThresholdFunc) (QueueBuilder *builder, size_t threshold);
typedef void (NVNAPIENTRY *QueueBuilderSetQueuePriorityFunc) (QueueBuilder *builder, const QueuePriority::Enum priority);
typedef QueuePriority::Enum (NVNAPIENTRY *QueueBuilderGetQueuePriorityFunc) (const QueueBuilder *builder);
typedef const Device * (NVNAPIENTRY *QueueBuilderGetDeviceFunc) (const QueueBuilder *builder);
typedef int (NVNAPIENTRY *QueueBuilderGetFlagsFunc) (const QueueBuilder *builder);
typedef NVNboolean (NVNAPIENTRY *QueueBuilderGetCommandMemorySizeFunc) (const QueueBuilder *builder, int *size);
typedef NVNboolean (NVNAPIENTRY *QueueBuilderGetComputeMemorySizeFunc) (const QueueBuilder *builder, int *size);
typedef NVNboolean (NVNAPIENTRY *QueueBuilderGetControlMemorySizeFunc) (const QueueBuilder *builder, int *size);
typedef NVNboolean (NVNAPIENTRY *QueueBuilderGetCommandFlushThresholdFunc) (const QueueBuilder *builder, int *flushThreshold);
typedef int (NVNAPIENTRY *QueueBuilderGetMemorySizeFunc) (const QueueBuilder *builder);
typedef void* (NVNAPIENTRY *QueueBuilderGetMemoryFunc) (const QueueBuilder *builder);
typedef NVNboolean (NVNAPIENTRY *QueueInitializeFunc) (Queue *queue, const QueueBuilder *builder);
typedef void (NVNAPIENTRY *QueueFinalizeFunc) (Queue *queue);
typedef void (NVNAPIENTRY *QueueSetDebugLabelFunc) (Queue *queue, const char * label);
typedef void (NVNAPIENTRY *QueueSubmitCommandsFunc) (Queue *queue, int numCommands, const CommandHandle *handles);
typedef void (NVNAPIENTRY *QueueFlushFunc) (Queue *queue);
typedef void (NVNAPIENTRY *QueueFinishFunc) (Queue *queue);
typedef void (NVNAPIENTRY *QueuePresentTextureFunc) (Queue *queue, Window *window, int textureIndex);
typedef QueueAcquireTextureResult::Enum (NVNAPIENTRY *QueueAcquireTextureFunc) (Queue *queue, Window *window, int *textureIndex);
typedef void (NVNAPIENTRY *WindowBuilderSetDeviceFunc) (WindowBuilder *builder, Device *device);
typedef void (NVNAPIENTRY *WindowBuilderSetDefaultsFunc) (WindowBuilder *builder);
typedef void (NVNAPIENTRY *WindowBuilderSetNativeWindowFunc) (WindowBuilder *builder, NVNnativeWindow nativeWindow);
typedef void (NVNAPIENTRY *WindowBuilderSetTexturesFunc) (WindowBuilder *builder, int numTextures, Texture * const *textures);
typedef void (NVNAPIENTRY *WindowBuilderSetPresentIntervalFunc) (WindowBuilder *builder, int presentInterval);
typedef void (NVNAPIENTRY *WindowBuilderSetNumActiveTexturesFunc) (WindowBuilder *builder, int numActiveTextures);
typedef const Device * (NVNAPIENTRY *WindowBuilderGetDeviceFunc) (const WindowBuilder *builder);
typedef int (NVNAPIENTRY *WindowBuilderGetNumTexturesFunc) (const WindowBuilder *builder);
typedef const Texture * (NVNAPIENTRY *WindowBuilderGetTextureFunc) (const WindowBuilder *builder, int textureId);
typedef NVNnativeWindow (NVNAPIENTRY *WindowBuilderGetNativeWindowFunc) (const WindowBuilder *builder);
typedef int (NVNAPIENTRY *WindowBuilderGetPresentIntervalFunc) (const WindowBuilder *builder);
typedef int (NVNAPIENTRY *WindowBuilderGetNumActiveTexturesFunc) (const WindowBuilder *builder);
typedef NVNboolean (NVNAPIENTRY *WindowInitializeFunc) (Window *window, const WindowBuilder *builder);
typedef void (NVNAPIENTRY *WindowFinalizeFunc) (Window *window);
typedef void (NVNAPIENTRY *WindowSetDebugLabelFunc) (Window *window, const char * label);
typedef WindowAcquireTextureResult::Enum (NVNAPIENTRY *WindowAcquireTextureFunc) (Window *window, Sync *textureAvailableSync, int *textureIndex);
typedef NVNnativeWindow (NVNAPIENTRY *WindowGetNativeWindowFunc) (const Window *window);
typedef int (NVNAPIENTRY *WindowGetPresentIntervalFunc) (const Window *window);
typedef void (NVNAPIENTRY *WindowSetPresentIntervalFunc) (Window *window, int presentInterval);
typedef void (NVNAPIENTRY *WindowSetCropFunc) (Window *window, int x, int y, int w, int h);
typedef void (NVNAPIENTRY *WindowGetCropFunc) (const Window *window, Rectangle *crop);
typedef void (NVNAPIENTRY *WindowSetNumActiveTexturesFunc) (Window *window, int numActiveTextures);
typedef int (NVNAPIENTRY *WindowGetNumActiveTexturesFunc) (const Window *window);
typedef int (NVNAPIENTRY *WindowGetNumTexturesFunc) (const Window *window);
typedef NVNboolean (NVNAPIENTRY *ProgramInitializeFunc) (Program *program, Device *device);
typedef void (NVNAPIENTRY *ProgramFinalizeFunc) (Program *program);
typedef void (NVNAPIENTRY *ProgramSetDebugLabelFunc) (Program *program, const char * label);
typedef NVNboolean (NVNAPIENTRY *ProgramSetShadersFunc) (Program *program, int count, const ShaderData *stageData);
typedef NVNboolean (NVNAPIENTRY *ProgramSetShadersExtFunc) (Program *program, int count, const ShaderDataExt *stageDataExt);
typedef void (NVNAPIENTRY *ProgramSetSampleShadingFunc) (Program *program, SampleShadingMode::Enum shadingMode);
typedef void (NVNAPIENTRY *MemoryPoolBuilderSetDeviceFunc) (MemoryPoolBuilder *builder, Device *device);
typedef void (NVNAPIENTRY *MemoryPoolBuilderSetDefaultsFunc) (MemoryPoolBuilder *builder);
typedef void (NVNAPIENTRY *MemoryPoolBuilderSetStorageFunc) (MemoryPoolBuilder *builder, void *memory, size_t size);
typedef void (NVNAPIENTRY *MemoryPoolBuilderSetFlagsFunc) (MemoryPoolBuilder *builder, int flags);
typedef const Device * (NVNAPIENTRY *MemoryPoolBuilderGetDeviceFunc) (const MemoryPoolBuilder *builder);
typedef void* (NVNAPIENTRY *MemoryPoolBuilderGetMemoryFunc) (const MemoryPoolBuilder *builder);
typedef size_t (NVNAPIENTRY *MemoryPoolBuilderGetSizeFunc) (const MemoryPoolBuilder *builder);
typedef int (NVNAPIENTRY *MemoryPoolBuilderGetFlagsFunc) (const MemoryPoolBuilder *builder);
typedef NVNboolean (NVNAPIENTRY *MemoryPoolInitializeFunc) (MemoryPool *pool, const MemoryPoolBuilder *builder);
typedef void (NVNAPIENTRY *MemoryPoolSetDebugLabelFunc) (MemoryPool *pool, const char * label);
typedef void (NVNAPIENTRY *MemoryPoolFinalizeFunc) (MemoryPool *pool);
typedef void* (NVNAPIENTRY *MemoryPoolMapFunc) (const MemoryPool *pool);
typedef void (NVNAPIENTRY *MemoryPoolFlushMappedRangeFunc) (const MemoryPool *pool, ptrdiff_t offset, size_t size);
typedef void (NVNAPIENTRY *MemoryPoolInvalidateMappedRangeFunc) (const MemoryPool *pool, ptrdiff_t offset, size_t size);
typedef BufferAddress (NVNAPIENTRY *MemoryPoolGetBufferAddressFunc) (const MemoryPool *pool);
typedef NVNboolean (NVNAPIENTRY *MemoryPoolMapVirtualFunc) (MemoryPool *virtualPool, int numRequests, const MappingRequest *requests);
typedef size_t (NVNAPIENTRY *MemoryPoolGetSizeFunc) (const MemoryPool *pool);
typedef int (NVNAPIENTRY *MemoryPoolGetFlagsFunc) (const MemoryPool *pool);
typedef NVNboolean (NVNAPIENTRY *TexturePoolInitializeFunc) (TexturePool *texturePool, const MemoryPool *memoryPool, ptrdiff_t offset, int numDescriptors);
typedef void (NVNAPIENTRY *TexturePoolSetDebugLabelFunc) (TexturePool *pool, const char * label);
typedef void (NVNAPIENTRY *TexturePoolFinalizeFunc) (TexturePool *pool);
typedef void (NVNAPIENTRY *TexturePoolRegisterTextureFunc) (const TexturePool *texturePool, int id, const Texture *texture, const TextureView *view);
typedef void (NVNAPIENTRY *TexturePoolRegisterImageFunc) (const TexturePool *texturePool, int id, const Texture *texture, const TextureView *view);
typedef const MemoryPool * (NVNAPIENTRY *TexturePoolGetMemoryPoolFunc) (const TexturePool *pool);
typedef ptrdiff_t (NVNAPIENTRY *TexturePoolGetMemoryOffsetFunc) (const TexturePool *pool);
typedef int (NVNAPIENTRY *TexturePoolGetSizeFunc) (const TexturePool *pool);
typedef NVNboolean (NVNAPIENTRY *SamplerPoolInitializeFunc) (SamplerPool *samplerPool, const MemoryPool *memoryPool, ptrdiff_t offset, int numDescriptors);
typedef void (NVNAPIENTRY *SamplerPoolSetDebugLabelFunc) (SamplerPool *pool, const char * label);
typedef void (NVNAPIENTRY *SamplerPoolFinalizeFunc) (SamplerPool *pool);
typedef void (NVNAPIENTRY *SamplerPoolRegisterSamplerFunc) (const SamplerPool *samplerPool, int id, const Sampler *sampler);
typedef void (NVNAPIENTRY *SamplerPoolRegisterSamplerBuilderFunc) (const SamplerPool *samplerPool, int id, const SamplerBuilder *builder);
typedef const MemoryPool * (NVNAPIENTRY *SamplerPoolGetMemoryPoolFunc) (const SamplerPool *pool);
typedef ptrdiff_t (NVNAPIENTRY *SamplerPoolGetMemoryOffsetFunc) (const SamplerPool *pool);
typedef int (NVNAPIENTRY *SamplerPoolGetSizeFunc) (const SamplerPool *pool);
typedef void (NVNAPIENTRY *BufferBuilderSetDeviceFunc) (BufferBuilder *builder, Device *device);
typedef void (NVNAPIENTRY *BufferBuilderSetDefaultsFunc) (BufferBuilder *builder);
typedef void (NVNAPIENTRY *BufferBuilderSetStorageFunc) (BufferBuilder *builder, MemoryPool *pool, ptrdiff_t offset, size_t size);
typedef const Device * (NVNAPIENTRY *BufferBuilderGetDeviceFunc) (const BufferBuilder *builder);
typedef MemoryPool * (NVNAPIENTRY *BufferBuilderGetMemoryPoolFunc) (const BufferBuilder *builder);
typedef ptrdiff_t (NVNAPIENTRY *BufferBuilderGetMemoryOffsetFunc) (const BufferBuilder *builder);
typedef size_t (NVNAPIENTRY *BufferBuilderGetSizeFunc) (const BufferBuilder *builder);
typedef NVNboolean (NVNAPIENTRY *BufferInitializeFunc) (Buffer *buffer, const BufferBuilder *builder);
typedef void (NVNAPIENTRY *BufferSetDebugLabelFunc) (Buffer *buffer, const char * label);
typedef void (NVNAPIENTRY *BufferFinalizeFunc) (Buffer *buffer);
typedef void* (NVNAPIENTRY *BufferMapFunc) (const Buffer *buffer);
typedef BufferAddress (NVNAPIENTRY *BufferGetAddressFunc) (const Buffer *buffer);
typedef void (NVNAPIENTRY *BufferFlushMappedRangeFunc) (const Buffer *buffer, ptrdiff_t offset, size_t size);
typedef void (NVNAPIENTRY *BufferInvalidateMappedRangeFunc) (const Buffer *buffer, ptrdiff_t offset, size_t size);
typedef MemoryPool * (NVNAPIENTRY *BufferGetMemoryPoolFunc) (const Buffer *buffer);
typedef ptrdiff_t (NVNAPIENTRY *BufferGetMemoryOffsetFunc) (const Buffer *buffer);
typedef size_t (NVNAPIENTRY *BufferGetSizeFunc) (const Buffer *buffer);
typedef uint64_t (NVNAPIENTRY *BufferGetDebugIDFunc) (const Buffer *buffer);
typedef void (NVNAPIENTRY *TextureBuilderSetDeviceFunc) (TextureBuilder *builder, Device *device);
typedef void (NVNAPIENTRY *TextureBuilderSetDefaultsFunc) (TextureBuilder *builder);
typedef void (NVNAPIENTRY *TextureBuilderSetFlagsFunc) (TextureBuilder *builder, int flags);
typedef void (NVNAPIENTRY *TextureBuilderSetTargetFunc) (TextureBuilder *builder, TextureTarget::Enum target);
typedef void (NVNAPIENTRY *TextureBuilderSetWidthFunc) (TextureBuilder *builder, int width);
typedef void (NVNAPIENTRY *TextureBuilderSetHeightFunc) (TextureBuilder *builder, int height);
typedef void (NVNAPIENTRY *TextureBuilderSetDepthFunc) (TextureBuilder *builder, int depth);
typedef void (NVNAPIENTRY *TextureBuilderSetSize1DFunc) (TextureBuilder *builder, int width);
typedef void (NVNAPIENTRY *TextureBuilderSetSize2DFunc) (TextureBuilder *builder, int width, int height);
typedef void (NVNAPIENTRY *TextureBuilderSetSize3DFunc) (TextureBuilder *builder, int width, int height, int depth);
typedef void (NVNAPIENTRY *TextureBuilderSetLevelsFunc) (TextureBuilder *builder, int numLevels);
typedef void (NVNAPIENTRY *TextureBuilderSetFormatFunc) (TextureBuilder *builder, Format::Enum format);
typedef void (NVNAPIENTRY *TextureBuilderSetSamplesFunc) (TextureBuilder *builder, int samples);
typedef void (NVNAPIENTRY *TextureBuilderSetSwizzleFunc) (TextureBuilder *builder, TextureSwizzle::Enum r, TextureSwizzle::Enum g, TextureSwizzle::Enum b, TextureSwizzle::Enum a);
typedef void (NVNAPIENTRY *TextureBuilderSetDepthStencilModeFunc) (TextureBuilder *builder, TextureDepthStencilMode::Enum mode);
typedef size_t (NVNAPIENTRY *TextureBuilderGetStorageSizeFunc) (const TextureBuilder *builder);
typedef size_t (NVNAPIENTRY *TextureBuilderGetStorageAlignmentFunc) (const TextureBuilder *builder);
typedef void (NVNAPIENTRY *TextureBuilderSetStorageFunc) (TextureBuilder *builder, MemoryPool *pool, ptrdiff_t offset);
typedef void (NVNAPIENTRY *TextureBuilderSetPackagedTextureDataFunc) (TextureBuilder *builder, const void *data);
typedef void (NVNAPIENTRY *TextureBuilderSetPackagedTextureLayoutFunc) (TextureBuilder *builder, const PackagedTextureLayout *layout);
typedef void (NVNAPIENTRY *TextureBuilderSetStrideFunc) (TextureBuilder *builder, ptrdiff_t stride);
typedef void (NVNAPIENTRY *TextureBuilderSetGLTextureNameFunc) (TextureBuilder *builder, uint32_t name);
typedef NVNstorageClass (NVNAPIENTRY *TextureBuilderGetStorageClassFunc) (const TextureBuilder *builder);
typedef const Device * (NVNAPIENTRY *TextureBuilderGetDeviceFunc) (const TextureBuilder *builder);
typedef int (NVNAPIENTRY *TextureBuilderGetFlagsFunc) (const TextureBuilder *builder);
typedef TextureTarget::Enum (NVNAPIENTRY *TextureBuilderGetTargetFunc) (const TextureBuilder *builder);
typedef int (NVNAPIENTRY *TextureBuilderGetWidthFunc) (const TextureBuilder *builder);
typedef int (NVNAPIENTRY *TextureBuilderGetHeightFunc) (const TextureBuilder *builder);
typedef int (NVNAPIENTRY *TextureBuilderGetDepthFunc) (const TextureBuilder *builder);
typedef int (NVNAPIENTRY *TextureBuilderGetLevelsFunc) (const TextureBuilder *builder);
typedef Format::Enum (NVNAPIENTRY *TextureBuilderGetFormatFunc) (const TextureBuilder *builder);
typedef int (NVNAPIENTRY *TextureBuilderGetSamplesFunc) (const TextureBuilder *builder);
typedef void (NVNAPIENTRY *TextureBuilderGetSwizzleFunc) (const TextureBuilder *builder, TextureSwizzle *r, TextureSwizzle *g, TextureSwizzle *b, TextureSwizzle *a);
typedef TextureDepthStencilMode::Enum (NVNAPIENTRY *TextureBuilderGetDepthStencilModeFunc) (const TextureBuilder *builder);
typedef const void * (NVNAPIENTRY *TextureBuilderGetPackagedTextureDataFunc) (const TextureBuilder *builder);
typedef NVNboolean (NVNAPIENTRY *TextureBuilderGetPackagedTextureLayoutFunc) (const TextureBuilder *builder, PackagedTextureLayout *data);
typedef ptrdiff_t (NVNAPIENTRY *TextureBuilderGetStrideFunc) (const TextureBuilder *builder);
typedef void (NVNAPIENTRY *TextureBuilderGetSparseTileLayoutFunc) (const TextureBuilder *builder, TextureSparseTileLayout *layout);
typedef uint32_t (NVNAPIENTRY *TextureBuilderGetGLTextureNameFunc) (const TextureBuilder *builder);
typedef size_t (NVNAPIENTRY *TextureBuilderGetZCullStorageSizeFunc) (const TextureBuilder *builder);
typedef MemoryPool * (NVNAPIENTRY *TextureBuilderGetMemoryPoolFunc) (const TextureBuilder *builder);
typedef ptrdiff_t (NVNAPIENTRY *TextureBuilderGetMemoryOffsetFunc) (const TextureBuilder *builder);
typedef RawStorageClass::Enum (NVNAPIENTRY *TextureBuilderGetRawStorageClassFunc) (const TextureBuilder *builder);
typedef void (NVNAPIENTRY *TextureViewSetDefaultsFunc) (TextureView *view);
typedef void (NVNAPIENTRY *TextureViewSetLevelsFunc) (TextureView *view, int baseLevel, int numLevels);
typedef void (NVNAPIENTRY *TextureViewSetLayersFunc) (TextureView *view, int minLayer, int numLayers);
typedef void (NVNAPIENTRY *TextureViewSetFormatFunc) (TextureView *view, Format::Enum format);
typedef void (NVNAPIENTRY *TextureViewSetSwizzleFunc) (TextureView *view, TextureSwizzle::Enum r, TextureSwizzle::Enum g, TextureSwizzle::Enum b, TextureSwizzle::Enum a);
typedef void (NVNAPIENTRY *TextureViewSetDepthStencilModeFunc) (TextureView *view, TextureDepthStencilMode::Enum mode);
typedef void (NVNAPIENTRY *TextureViewSetTargetFunc) (TextureView *view, TextureTarget::Enum target);
typedef NVNboolean (NVNAPIENTRY *TextureViewGetLevelsFunc) (const TextureView *view, int *baseLevel, int *numLevels);
typedef NVNboolean (NVNAPIENTRY *TextureViewGetLayersFunc) (const TextureView *view, int *minLayer, int *numLayers);
typedef NVNboolean (NVNAPIENTRY *TextureViewGetFormatFunc) (const TextureView *view, Format *format);
typedef NVNboolean (NVNAPIENTRY *TextureViewGetSwizzleFunc) (const TextureView *view, TextureSwizzle *r, TextureSwizzle *g, TextureSwizzle *b, TextureSwizzle *a);
typedef NVNboolean (NVNAPIENTRY *TextureViewGetDepthStencilModeFunc) (const TextureView *view, TextureDepthStencilMode *mode);
typedef NVNboolean (NVNAPIENTRY *TextureViewGetTargetFunc) (const TextureView *view, TextureTarget *target);
typedef NVNboolean (NVNAPIENTRY *TextureViewCompareFunc) (const TextureView *view, const TextureView *otherView);
typedef NVNboolean (NVNAPIENTRY *TextureInitializeFunc) (Texture *texture, const TextureBuilder *builder);
typedef size_t (NVNAPIENTRY *TextureGetZCullStorageSizeFunc) (const Texture *texture);
typedef void (NVNAPIENTRY *TextureFinalizeFunc) (Texture *texture);
typedef void (NVNAPIENTRY *TextureSetDebugLabelFunc) (Texture *texture, const char * label);
typedef NVNstorageClass (NVNAPIENTRY *TextureGetStorageClassFunc) (const Texture *texture);
typedef ptrdiff_t (NVNAPIENTRY *TextureGetViewOffsetFunc) (const Texture *texture, const TextureView *textureView);
typedef int (NVNAPIENTRY *TextureGetFlagsFunc) (const Texture *texture);
typedef TextureTarget::Enum (NVNAPIENTRY *TextureGetTargetFunc) (const Texture *texture);
typedef int (NVNAPIENTRY *TextureGetWidthFunc) (const Texture *texture);
typedef int (NVNAPIENTRY *TextureGetHeightFunc) (const Texture *texture);
typedef int (NVNAPIENTRY *TextureGetDepthFunc) (const Texture *texture);
typedef int (NVNAPIENTRY *TextureGetLevelsFunc) (const Texture *texture);
typedef Format::Enum (NVNAPIENTRY *TextureGetFormatFunc) (const Texture *texture);
typedef int (NVNAPIENTRY *TextureGetSamplesFunc) (const Texture *texture);
typedef void (NVNAPIENTRY *TextureGetSwizzleFunc) (const Texture *texture, TextureSwizzle *r, TextureSwizzle *g, TextureSwizzle *b, TextureSwizzle *a);
typedef TextureDepthStencilMode::Enum (NVNAPIENTRY *TextureGetDepthStencilModeFunc) (const Texture *texture);
typedef ptrdiff_t (NVNAPIENTRY *TextureGetStrideFunc) (const Texture *texture);
typedef TextureAddress (NVNAPIENTRY *TextureGetTextureAddressFunc) (const Texture *texture);
typedef void (NVNAPIENTRY *TextureGetSparseTileLayoutFunc) (const Texture *texture, TextureSparseTileLayout *layout);
typedef void (NVNAPIENTRY *TextureWriteTexelsFunc) (const Texture *texture, const TextureView *textureView, const CopyRegion *region, const void *data);
typedef void (NVNAPIENTRY *TextureWriteTexelsStridedFunc) (const Texture *texture, const TextureView *textureView, const CopyRegion *region, const void *data, ptrdiff_t rowStride, ptrdiff_t imageStride);
typedef void (NVNAPIENTRY *TextureReadTexelsFunc) (const Texture *texture, const TextureView *textureView, const CopyRegion *region, void *data);
typedef void (NVNAPIENTRY *TextureReadTexelsStridedFunc) (const Texture *texture, const TextureView *textureView, const CopyRegion *region, void *data, ptrdiff_t rowStride, ptrdiff_t imageStride);
typedef void (NVNAPIENTRY *TextureFlushTexelsFunc) (const Texture *texture, const TextureView *textureView, const CopyRegion *region);
typedef void (NVNAPIENTRY *TextureInvalidateTexelsFunc) (const Texture *texture, const TextureView *textureView, const CopyRegion *region);
typedef MemoryPool * (NVNAPIENTRY *TextureGetMemoryPoolFunc) (const Texture *texture);
typedef ptrdiff_t (NVNAPIENTRY *TextureGetMemoryOffsetFunc) (const Texture *texture);
typedef int (NVNAPIENTRY *TextureGetStorageSizeFunc) (const Texture *texture);
typedef NVNboolean (NVNAPIENTRY *TextureCompareFunc) (const Texture *texture, const Texture *otherTexture);
typedef uint64_t (NVNAPIENTRY *TextureGetDebugIDFunc) (const Texture *texture);
typedef RawStorageClass::Enum (NVNAPIENTRY *TextureGetRawStorageClassFunc) (const Texture *texture);
typedef void (NVNAPIENTRY *SamplerBuilderSetDeviceFunc) (SamplerBuilder *builder, Device *device);
typedef void (NVNAPIENTRY *SamplerBuilderSetDefaultsFunc) (SamplerBuilder *builder);
typedef void (NVNAPIENTRY *SamplerBuilderSetMinMagFilterFunc) (SamplerBuilder *builder, MinFilter::Enum min, MagFilter::Enum mag);
typedef void (NVNAPIENTRY *SamplerBuilderSetWrapModeFunc) (SamplerBuilder *builder, WrapMode::Enum s, WrapMode::Enum t, WrapMode::Enum r);
typedef void (NVNAPIENTRY *SamplerBuilderSetLodClampFunc) (SamplerBuilder *builder, float min, float max);
typedef void (NVNAPIENTRY *SamplerBuilderSetLodBiasFunc) (SamplerBuilder *builder, float bias);
typedef void (NVNAPIENTRY *SamplerBuilderSetCompareFunc) (SamplerBuilder *builder, CompareMode::Enum mode, CompareFunc::Enum func);
typedef void (NVNAPIENTRY *SamplerBuilderSetBorderColorFunc) (SamplerBuilder *builder, const float *borderColor);
typedef void (NVNAPIENTRY *SamplerBuilderSetBorderColoriFunc) (SamplerBuilder *builder, const int *borderColor);
typedef void (NVNAPIENTRY *SamplerBuilderSetBorderColoruiFunc) (SamplerBuilder *builder, const uint32_t *borderColor);
typedef void (NVNAPIENTRY *SamplerBuilderSetMaxAnisotropyFunc) (SamplerBuilder *builder, float maxAniso);
typedef void (NVNAPIENTRY *SamplerBuilderSetReductionFilterFunc) (SamplerBuilder *builder, SamplerReduction::Enum filter);
typedef void (NVNAPIENTRY *SamplerBuilderSetLodSnapFunc) (SamplerBuilder *builder, float lodSnap);
typedef const Device * (NVNAPIENTRY *SamplerBuilderGetDeviceFunc) (const SamplerBuilder *builder);
typedef void (NVNAPIENTRY *SamplerBuilderGetMinMagFilterFunc) (const SamplerBuilder *builder, MinFilter *min, MagFilter *mag);
typedef void (NVNAPIENTRY *SamplerBuilderGetWrapModeFunc) (const SamplerBuilder *builder, WrapMode *s, WrapMode *t, WrapMode *r);
typedef void (NVNAPIENTRY *SamplerBuilderGetLodClampFunc) (const SamplerBuilder *builder, float *min, float *max);
typedef float (NVNAPIENTRY *SamplerBuilderGetLodBiasFunc) (const SamplerBuilder *builder);
typedef void (NVNAPIENTRY *SamplerBuilderGetCompareFunc) (const SamplerBuilder *builder, CompareMode *mode, CompareFunc *func);
typedef void (NVNAPIENTRY *SamplerBuilderGetBorderColorFunc) (const SamplerBuilder *builder, float *borderColor);
typedef void (NVNAPIENTRY *SamplerBuilderGetBorderColoriFunc) (const SamplerBuilder *builder, int *borderColor);
typedef void (NVNAPIENTRY *SamplerBuilderGetBorderColoruiFunc) (const SamplerBuilder *builder, uint32_t *borderColor);
typedef float (NVNAPIENTRY *SamplerBuilderGetMaxAnisotropyFunc) (const SamplerBuilder *builder);
typedef SamplerReduction::Enum (NVNAPIENTRY *SamplerBuilderGetReductionFilterFunc) (const SamplerBuilder *builder);
typedef float (NVNAPIENTRY *SamplerBuilderGetLodSnapFunc) (const SamplerBuilder *builder);
typedef NVNboolean (NVNAPIENTRY *SamplerInitializeFunc) (Sampler *sampler, const SamplerBuilder *builder);
typedef void (NVNAPIENTRY *SamplerFinalizeFunc) (Sampler *sampler);
typedef void (NVNAPIENTRY *SamplerSetDebugLabelFunc) (Sampler *sampler, const char * label);
typedef void (NVNAPIENTRY *SamplerGetMinMagFilterFunc) (const Sampler *sampler, MinFilter *min, MagFilter *mag);
typedef void (NVNAPIENTRY *SamplerGetWrapModeFunc) (const Sampler *sampler, WrapMode *s, WrapMode *t, WrapMode *r);
typedef void (NVNAPIENTRY *SamplerGetLodClampFunc) (const Sampler *sampler, float *min, float *max);
typedef float (NVNAPIENTRY *SamplerGetLodBiasFunc) (const Sampler *sampler);
typedef void (NVNAPIENTRY *SamplerGetCompareFunc) (const Sampler *sampler, CompareMode *mode, CompareFunc *func);
typedef void (NVNAPIENTRY *SamplerGetBorderColorFunc) (const Sampler *sampler, float *borderColor);
typedef void (NVNAPIENTRY *SamplerGetBorderColoriFunc) (const Sampler *sampler, int *borderColor);
typedef void (NVNAPIENTRY *SamplerGetBorderColoruiFunc) (const Sampler *sampler, uint32_t *borderColor);
typedef float (NVNAPIENTRY *SamplerGetMaxAnisotropyFunc) (const Sampler *sampler);
typedef SamplerReduction::Enum (NVNAPIENTRY *SamplerGetReductionFilterFunc) (const Sampler *sampler);
typedef NVNboolean (NVNAPIENTRY *SamplerCompareFunc) (const Sampler *sampler, const Sampler *otherSampler);
typedef uint64_t (NVNAPIENTRY *SamplerGetDebugIDFunc) (const Sampler *sampler);
typedef void (NVNAPIENTRY *BlendStateSetDefaultsFunc) (BlendState *blend);
typedef void (NVNAPIENTRY *BlendStateSetBlendTargetFunc) (BlendState *blend, int target);
typedef void (NVNAPIENTRY *BlendStateSetBlendFuncFunc) (BlendState *blend, BlendFunc::Enum srcFunc, BlendFunc::Enum dstFunc, BlendFunc::Enum srcFuncAlpha, BlendFunc::Enum dstFuncAlpha);
typedef void (NVNAPIENTRY *BlendStateSetBlendEquationFunc) (BlendState *blend, BlendEquation::Enum modeRGB, BlendEquation::Enum modeAlpha);
typedef void (NVNAPIENTRY *BlendStateSetAdvancedModeFunc) (BlendState *blend, BlendAdvancedMode::Enum overlap);
typedef void (NVNAPIENTRY *BlendStateSetAdvancedOverlapFunc) (BlendState *blend, BlendAdvancedOverlap::Enum overlap);
typedef void (NVNAPIENTRY *BlendStateSetAdvancedPremultipliedSrcFunc) (BlendState *blend, NVNboolean premultiplied);
typedef void (NVNAPIENTRY *BlendStateSetAdvancedNormalizedDstFunc) (BlendState *blend, NVNboolean normalized);
typedef int (NVNAPIENTRY *BlendStateGetBlendTargetFunc) (const BlendState *blend);
typedef void (NVNAPIENTRY *BlendStateGetBlendFuncFunc) (const BlendState *blend, BlendFunc *srcFunc, BlendFunc *dstFunc, BlendFunc *srcFuncAlpha, BlendFunc *dstFuncAlpha);
typedef void (NVNAPIENTRY *BlendStateGetBlendEquationFunc) (const BlendState *blend, BlendEquation *modeRGB, BlendEquation *modeAlpha);
typedef BlendAdvancedMode::Enum (NVNAPIENTRY *BlendStateGetAdvancedModeFunc) (const BlendState *blend);
typedef BlendAdvancedOverlap::Enum (NVNAPIENTRY *BlendStateGetAdvancedOverlapFunc) (const BlendState *blend);
typedef NVNboolean (NVNAPIENTRY *BlendStateGetAdvancedPremultipliedSrcFunc) (const BlendState *blend);
typedef NVNboolean (NVNAPIENTRY *BlendStateGetAdvancedNormalizedDstFunc) (const BlendState *blend);
typedef void (NVNAPIENTRY *ColorStateSetDefaultsFunc) (ColorState *color);
typedef void (NVNAPIENTRY *ColorStateSetBlendEnableFunc) (ColorState *color, int index, NVNboolean enable);
typedef void (NVNAPIENTRY *ColorStateSetLogicOpFunc) (ColorState *color, LogicOp::Enum logicOp);
typedef void (NVNAPIENTRY *ColorStateSetAlphaTestFunc) (ColorState *color, AlphaFunc::Enum alphaTest);
typedef NVNboolean (NVNAPIENTRY *ColorStateGetBlendEnableFunc) (const ColorState *color, int index);
typedef LogicOp::Enum (NVNAPIENTRY *ColorStateGetLogicOpFunc) (const ColorState *color);
typedef AlphaFunc::Enum (NVNAPIENTRY *ColorStateGetAlphaTestFunc) (const ColorState *color);
typedef void (NVNAPIENTRY *ChannelMaskStateSetDefaultsFunc) (ChannelMaskState *channelMask);
typedef void (NVNAPIENTRY *ChannelMaskStateSetChannelMaskFunc) (ChannelMaskState *channelMask, int index, NVNboolean r, NVNboolean g, NVNboolean b, NVNboolean a);
typedef void (NVNAPIENTRY *ChannelMaskStateGetChannelMaskFunc) (const ChannelMaskState *channelMask, int index, NVNboolean *r, NVNboolean *g, NVNboolean *b, NVNboolean *a);
typedef void (NVNAPIENTRY *MultisampleStateSetDefaultsFunc) (MultisampleState *multisample);
typedef void (NVNAPIENTRY *MultisampleStateSetMultisampleEnableFunc) (MultisampleState *multisample, NVNboolean enable);
typedef void (NVNAPIENTRY *MultisampleStateSetSamplesFunc) (MultisampleState *multisample, int samples);
typedef void (NVNAPIENTRY *MultisampleStateSetAlphaToCoverageEnableFunc) (MultisampleState *multisample, NVNboolean enable);
typedef void (NVNAPIENTRY *MultisampleStateSetAlphaToCoverageDitherFunc) (MultisampleState *multisample, NVNboolean dither);
typedef NVNboolean (NVNAPIENTRY *MultisampleStateGetMultisampleEnableFunc) (const MultisampleState *multisample);
typedef int (NVNAPIENTRY *MultisampleStateGetSamplesFunc) (const MultisampleState *multisample);
typedef NVNboolean (NVNAPIENTRY *MultisampleStateGetAlphaToCoverageEnableFunc) (const MultisampleState *multisample);
typedef NVNboolean (NVNAPIENTRY *MultisampleStateGetAlphaToCoverageDitherFunc) (const MultisampleState *multisample);
typedef void (NVNAPIENTRY *MultisampleStateSetRasterSamplesFunc) (MultisampleState *multisample, int rasterSamples);
typedef int (NVNAPIENTRY *MultisampleStateGetRasterSamplesFunc) (MultisampleState *multisample);
typedef void (NVNAPIENTRY *MultisampleStateSetCoverageModulationModeFunc) (MultisampleState *multisample, CoverageModulationMode::Enum mode);
typedef CoverageModulationMode::Enum (NVNAPIENTRY *MultisampleStateGetCoverageModulationModeFunc) (const MultisampleState *multisample);
typedef void (NVNAPIENTRY *MultisampleStateSetCoverageToColorEnableFunc) (MultisampleState *multisample, NVNboolean enable);
typedef NVNboolean (NVNAPIENTRY *MultisampleStateGetCoverageToColorEnableFunc) (const MultisampleState *multisample);
typedef void (NVNAPIENTRY *MultisampleStateSetCoverageToColorOutputFunc) (MultisampleState *multisample, int color);
typedef int (NVNAPIENTRY *MultisampleStateGetCoverageToColorOutputFunc) (const MultisampleState *multisample);
typedef void (NVNAPIENTRY *MultisampleStateSetSampleLocationsEnableFunc) (MultisampleState *multisample, NVNboolean enable);
typedef NVNboolean (NVNAPIENTRY *MultisampleStateGetSampleLocationsEnableFunc) (const MultisampleState *multisample);
typedef void (NVNAPIENTRY *MultisampleStateGetSampleLocationsGridFunc) (MultisampleState *multisample, int *width, int *height);
typedef void (NVNAPIENTRY *MultisampleStateSetSampleLocationsGridEnableFunc) (MultisampleState *multisample, NVNboolean enable);
typedef NVNboolean (NVNAPIENTRY *MultisampleStateGetSampleLocationsGridEnableFunc) (const MultisampleState *multisample);
typedef void (NVNAPIENTRY *MultisampleStateSetSampleLocationsFunc) (MultisampleState *multisample, int start, int count, const float *values);
typedef void (NVNAPIENTRY *PolygonStateSetDefaultsFunc) (PolygonState *polygon);
typedef void (NVNAPIENTRY *PolygonStateSetCullFaceFunc) (PolygonState *polygon, Face::Enum face);
typedef void (NVNAPIENTRY *PolygonStateSetFrontFaceFunc) (PolygonState *polygon, FrontFace::Enum face);
typedef void (NVNAPIENTRY *PolygonStateSetPolygonModeFunc) (PolygonState *polygon, PolygonMode::Enum polygonMode);
typedef void (NVNAPIENTRY *PolygonStateSetPolygonOffsetEnablesFunc) (PolygonState *polygon, int enables);
typedef Face::Enum (NVNAPIENTRY *PolygonStateGetCullFaceFunc) (const PolygonState *polygon);
typedef FrontFace::Enum (NVNAPIENTRY *PolygonStateGetFrontFaceFunc) (const PolygonState *polygon);
typedef PolygonMode::Enum (NVNAPIENTRY *PolygonStateGetPolygonModeFunc) (const PolygonState *polygon);
typedef int (NVNAPIENTRY *PolygonStateGetPolygonOffsetEnablesFunc) (const PolygonState *polygon);
typedef void (NVNAPIENTRY *DepthStencilStateSetDefaultsFunc) (DepthStencilState *depthStencil);
typedef void (NVNAPIENTRY *DepthStencilStateSetDepthTestEnableFunc) (DepthStencilState *depthStencil, NVNboolean enable);
typedef void (NVNAPIENTRY *DepthStencilStateSetDepthWriteEnableFunc) (DepthStencilState *depthStencil, NVNboolean enable);
typedef void (NVNAPIENTRY *DepthStencilStateSetDepthFuncFunc) (DepthStencilState *depthStencil, DepthFunc::Enum func);
typedef void (NVNAPIENTRY *DepthStencilStateSetStencilTestEnableFunc) (DepthStencilState *depthStencil, NVNboolean enable);
typedef void (NVNAPIENTRY *DepthStencilStateSetStencilFuncFunc) (DepthStencilState *depthStencil, Face::Enum faces, StencilFunc::Enum func);
typedef void (NVNAPIENTRY *DepthStencilStateSetStencilOpFunc) (DepthStencilState *depthStencil, Face::Enum faces, StencilOp::Enum fail, StencilOp::Enum depthFail, StencilOp::Enum depthPass);
typedef NVNboolean (NVNAPIENTRY *DepthStencilStateGetDepthTestEnableFunc) (const DepthStencilState *depthStencil);
typedef NVNboolean (NVNAPIENTRY *DepthStencilStateGetDepthWriteEnableFunc) (const DepthStencilState *depthStencil);
typedef DepthFunc::Enum (NVNAPIENTRY *DepthStencilStateGetDepthFuncFunc) (const DepthStencilState *depthStencil);
typedef NVNboolean (NVNAPIENTRY *DepthStencilStateGetStencilTestEnableFunc) (const DepthStencilState *depthStencil);
typedef StencilFunc::Enum (NVNAPIENTRY *DepthStencilStateGetStencilFuncFunc) (const DepthStencilState *depthStencil, Face::Enum face);
typedef void (NVNAPIENTRY *DepthStencilStateGetStencilOpFunc) (const DepthStencilState *depthStencil, Face::Enum face, StencilOp *fail, StencilOp *depthFail, StencilOp *depthPass);
typedef void (NVNAPIENTRY *VertexAttribStateSetDefaultsFunc) (VertexAttribState *attrib);
typedef void (NVNAPIENTRY *VertexAttribStateSetFormatFunc) (VertexAttribState *attrib, Format::Enum format, ptrdiff_t relativeOffset);
typedef void (NVNAPIENTRY *VertexAttribStateSetStreamIndexFunc) (VertexAttribState *attrib, int streamIndex);
typedef void (NVNAPIENTRY *VertexAttribStateGetFormatFunc) (const VertexAttribState *attrib, Format *format, ptrdiff_t *relativeOffset);
typedef int (NVNAPIENTRY *VertexAttribStateGetStreamIndexFunc) (const VertexAttribState *attrib);
typedef void (NVNAPIENTRY *VertexStreamStateSetDefaultsFunc) (VertexStreamState *stream);
typedef void (NVNAPIENTRY *VertexStreamStateSetStrideFunc) (VertexStreamState *stream, ptrdiff_t stride);
typedef void (NVNAPIENTRY *VertexStreamStateSetDivisorFunc) (VertexStreamState *stream, int divisor);
typedef ptrdiff_t (NVNAPIENTRY *VertexStreamStateGetStrideFunc) (const VertexStreamState *stream);
typedef int (NVNAPIENTRY *VertexStreamStateGetDivisorFunc) (const VertexStreamState *stream);
typedef NVNboolean (NVNAPIENTRY *CommandBufferInitializeFunc) (CommandBuffer *cmdBuf, Device *device);
typedef void (NVNAPIENTRY *CommandBufferFinalizeFunc) (CommandBuffer *cmdBuf);
typedef void (NVNAPIENTRY *CommandBufferSetDebugLabelFunc) (CommandBuffer *cmdBuf, const char * label);
typedef void (NVNAPIENTRY *CommandBufferSetMemoryCallbackFunc) (CommandBuffer *cmdBuf, CommandBufferMemoryCallbackFunc callback);
typedef void (NVNAPIENTRY *CommandBufferSetMemoryCallbackDataFunc) (CommandBuffer *cmdBuf, void *callbackData);
typedef void (NVNAPIENTRY *CommandBufferSetCommandMemoryCallbackEnabledFunc) (CommandBuffer *cmdBuf, NVNboolean enabled);
typedef void (NVNAPIENTRY *CommandBufferAddCommandMemoryFunc) (CommandBuffer *cmdBuf, const MemoryPool *pool, ptrdiff_t offset, size_t size);
typedef void (NVNAPIENTRY *CommandBufferAddControlMemoryFunc) (CommandBuffer *cmdBuf, void *memory, size_t size);
typedef size_t (NVNAPIENTRY *CommandBufferGetCommandMemorySizeFunc) (const CommandBuffer *cmdBuf);
typedef size_t (NVNAPIENTRY *CommandBufferGetCommandMemoryUsedFunc) (const CommandBuffer *cmdBuf);
typedef size_t (NVNAPIENTRY *CommandBufferGetCommandMemoryFreeFunc) (const CommandBuffer *cmdBuf);
typedef size_t (NVNAPIENTRY *CommandBufferGetControlMemorySizeFunc) (const CommandBuffer *cmdBuf);
typedef size_t (NVNAPIENTRY *CommandBufferGetControlMemoryUsedFunc) (const CommandBuffer *cmdBuf);
typedef size_t (NVNAPIENTRY *CommandBufferGetControlMemoryFreeFunc) (const CommandBuffer *cmdBuf);
typedef void (NVNAPIENTRY *CommandBufferBeginRecordingFunc) (CommandBuffer *cmdBuf);
typedef CommandHandle (NVNAPIENTRY *CommandBufferEndRecordingFunc) (CommandBuffer *cmdBuf);
typedef void (NVNAPIENTRY *CommandBufferCallCommandsFunc) (CommandBuffer *cmdBuf, int numCommands, const CommandHandle *handles);
typedef void (NVNAPIENTRY *CommandBufferCopyCommandsFunc) (CommandBuffer *cmdBuf, int numCommands, const CommandHandle *handles);
typedef void (NVNAPIENTRY *CommandBufferBindBlendStateFunc) (CommandBuffer *cmdBuf, const BlendState *blend);
typedef void (NVNAPIENTRY *CommandBufferBindChannelMaskStateFunc) (CommandBuffer *cmdBuf, const ChannelMaskState *channelMask);
typedef void (NVNAPIENTRY *CommandBufferBindColorStateFunc) (CommandBuffer *cmdBuf, const ColorState *color);
typedef void (NVNAPIENTRY *CommandBufferBindMultisampleStateFunc) (CommandBuffer *cmdBuf, const MultisampleState *multisample);
typedef void (NVNAPIENTRY *CommandBufferBindPolygonStateFunc) (CommandBuffer *cmdBuf, const PolygonState *polygon);
typedef void (NVNAPIENTRY *CommandBufferBindDepthStencilStateFunc) (CommandBuffer *cmdBuf, const DepthStencilState *depthStencil);
typedef void (NVNAPIENTRY *CommandBufferBindVertexAttribStateFunc) (CommandBuffer *cmdBuf, int numAttribs, const VertexAttribState *attribs);
typedef void (NVNAPIENTRY *CommandBufferBindVertexStreamStateFunc) (CommandBuffer *cmdBuf, int numStreams, const VertexStreamState *streams);
typedef void (NVNAPIENTRY *CommandBufferBindProgramFunc) (CommandBuffer *cmdBuf, const Program *program, int stages);
typedef void (NVNAPIENTRY *CommandBufferBindVertexBufferFunc) (CommandBuffer *cmdBuf, int index, BufferAddress buffer, size_t size);
typedef void (NVNAPIENTRY *CommandBufferBindVertexBuffersFunc) (CommandBuffer *cmdBuf, int first, int count, const BufferRange *buffers);
typedef void (NVNAPIENTRY *CommandBufferBindUniformBufferFunc) (CommandBuffer *cmdBuf, ShaderStage::Enum stage, int index, BufferAddress buffer, size_t size);
typedef void (NVNAPIENTRY *CommandBufferBindUniformBuffersFunc) (CommandBuffer *cmdBuf, ShaderStage::Enum stage, int first, int count, const BufferRange *buffers);
typedef void (NVNAPIENTRY *CommandBufferBindTransformFeedbackBufferFunc) (CommandBuffer *cmdBuf, int index, BufferAddress buffer, size_t size);
typedef void (NVNAPIENTRY *CommandBufferBindTransformFeedbackBuffersFunc) (CommandBuffer *cmdBuf, int first, int count, const BufferRange *buffers);
typedef void (NVNAPIENTRY *CommandBufferBindStorageBufferFunc) (CommandBuffer *cmdBuf, ShaderStage::Enum stage, int index, BufferAddress buffer, size_t size);
typedef void (NVNAPIENTRY *CommandBufferBindStorageBuffersFunc) (CommandBuffer *cmdBuf, ShaderStage::Enum stage, int first, int count, const BufferRange *buffers);
typedef void (NVNAPIENTRY *CommandBufferBindTextureFunc) (CommandBuffer *cmdBuf, ShaderStage::Enum stage, int index, TextureHandle texture);
typedef void (NVNAPIENTRY *CommandBufferBindTexturesFunc) (CommandBuffer *cmdBuf, ShaderStage::Enum stage, int first, int count, const TextureHandle *textures);
typedef void (NVNAPIENTRY *CommandBufferBindImageFunc) (CommandBuffer *cmdBuf, ShaderStage::Enum stage, int index, ImageHandle image);
typedef void (NVNAPIENTRY *CommandBufferBindImagesFunc) (CommandBuffer *cmdBuf, ShaderStage::Enum stage, int first, int count, const ImageHandle *images);
typedef void (NVNAPIENTRY *CommandBufferSetPatchSizeFunc) (CommandBuffer *cmdBuf, int size);
typedef void (NVNAPIENTRY *CommandBufferSetInnerTessellationLevelsFunc) (CommandBuffer *cmdBuf, const float *levels);
typedef void (NVNAPIENTRY *CommandBufferSetOuterTessellationLevelsFunc) (CommandBuffer *cmdBuf, const float *levels);
typedef void (NVNAPIENTRY *CommandBufferSetPrimitiveRestartFunc) (CommandBuffer *cmdBuf, NVNboolean enable, int index);
typedef void (NVNAPIENTRY *CommandBufferBeginTransformFeedbackFunc) (CommandBuffer *cmdBuf, BufferAddress buffer);
typedef void (NVNAPIENTRY *CommandBufferEndTransformFeedbackFunc) (CommandBuffer *cmdBuf, BufferAddress buffer);
typedef void (NVNAPIENTRY *CommandBufferPauseTransformFeedbackFunc) (CommandBuffer *cmdBuf, BufferAddress buffer);
typedef void (NVNAPIENTRY *CommandBufferResumeTransformFeedbackFunc) (CommandBuffer *cmdBuf, BufferAddress buffer);
typedef void (NVNAPIENTRY *CommandBufferDrawTransformFeedbackFunc) (CommandBuffer *cmdBuf, DrawPrimitive::Enum mode, BufferAddress buffer);
typedef void (NVNAPIENTRY *CommandBufferDrawArraysFunc) (CommandBuffer *cmdBuf, DrawPrimitive::Enum mode, int first, int count);
typedef void (NVNAPIENTRY *CommandBufferDrawElementsFunc) (CommandBuffer *cmdBuf, DrawPrimitive::Enum mode, IndexType::Enum type, int count, BufferAddress indexBuffer);
typedef void (NVNAPIENTRY *CommandBufferDrawElementsBaseVertexFunc) (CommandBuffer *cmdBuf, DrawPrimitive::Enum mode, IndexType::Enum type, int count, BufferAddress indexBuffer, int baseVertex);
typedef void (NVNAPIENTRY *CommandBufferDrawArraysInstancedFunc) (CommandBuffer *cmdBuf, DrawPrimitive::Enum mode, int first, int count, int baseInstance, int instanceCount);
typedef void (NVNAPIENTRY *CommandBufferDrawElementsInstancedFunc) (CommandBuffer *cmdBuf, DrawPrimitive::Enum mode, IndexType::Enum type, int count, BufferAddress indexBuffer, int baseVertex, int baseInstance, int instanceCount);
typedef void (NVNAPIENTRY *CommandBufferDrawArraysIndirectFunc) (CommandBuffer *cmdBuf, DrawPrimitive::Enum mode, BufferAddress indirectBuffer);
typedef void (NVNAPIENTRY *CommandBufferDrawElementsIndirectFunc) (CommandBuffer *cmdBuf, DrawPrimitive::Enum mode, IndexType::Enum type, BufferAddress indexBuffer, BufferAddress indirectBuffer);
typedef void (NVNAPIENTRY *CommandBufferMultiDrawArraysIndirectCountFunc) (CommandBuffer *cmdBuf, DrawPrimitive::Enum mode, BufferAddress indirectBuffer, BufferAddress parameterBuffer, int maxDrawCount, ptrdiff_t stride);
typedef void (NVNAPIENTRY *CommandBufferMultiDrawElementsIndirectCountFunc) (CommandBuffer *cmdBuf, DrawPrimitive::Enum mode, IndexType::Enum type, BufferAddress indexBuffer, BufferAddress indirectBuffer, BufferAddress parameterBuffer, int maxDrawCount, ptrdiff_t stride);
typedef void (NVNAPIENTRY *CommandBufferClearColorFunc) (CommandBuffer *cmdBuf, int index, const float *color, int mask);
typedef void (NVNAPIENTRY *CommandBufferClearColoriFunc) (CommandBuffer *cmdBuf, int index, const int *color, int mask);
typedef void (NVNAPIENTRY *CommandBufferClearColoruiFunc) (CommandBuffer *cmdBuf, int index, const uint32_t *color, int mask);
typedef void (NVNAPIENTRY *CommandBufferClearDepthStencilFunc) (CommandBuffer *cmdBuf, float depthValue, NVNboolean depthMask, int stencilValue, int stencilMask);
typedef void (NVNAPIENTRY *CommandBufferDispatchComputeFunc) (CommandBuffer *cmdBuf, int groupsX, int groupsY, int groupsZ);
typedef void (NVNAPIENTRY *CommandBufferDispatchComputeIndirectFunc) (CommandBuffer *cmdBuf, BufferAddress indirectBuffer);
typedef void (NVNAPIENTRY *CommandBufferSetViewportFunc) (CommandBuffer *cmdBuf, int x, int y, int w, int h);
typedef void (NVNAPIENTRY *CommandBufferSetViewportsFunc) (CommandBuffer *cmdBuf, int first, int count, const float *ranges);
typedef void (NVNAPIENTRY *CommandBufferSetViewportSwizzlesFunc) (CommandBuffer *cmdBuf, int first, int count, const ViewportSwizzle *swizzles);
typedef void (NVNAPIENTRY *CommandBufferSetScissorFunc) (CommandBuffer *cmdBuf, int x, int y, int w, int h);
typedef void (NVNAPIENTRY *CommandBufferSetScissorsFunc) (CommandBuffer *cmdBuf, int first, int count, const int *rects);
typedef void (NVNAPIENTRY *CommandBufferSetDepthRangeFunc) (CommandBuffer *cmdBuf, float n, float f);
typedef void (NVNAPIENTRY *CommandBufferSetDepthBoundsFunc) (CommandBuffer *cmdBuf, NVNboolean enable, float n, float f);
typedef void (NVNAPIENTRY *CommandBufferSetDepthRangesFunc) (CommandBuffer *cmdBuf, int first, int count, const float *ranges);
typedef void (NVNAPIENTRY *CommandBufferSetTiledCacheActionFunc) (CommandBuffer *cmdBuf, TiledCacheAction::Enum action);
typedef void (NVNAPIENTRY *CommandBufferSetTiledCacheTileSizeFunc) (CommandBuffer *cmdBuf, int tileWidth, int tileHeight);
typedef void (NVNAPIENTRY *CommandBufferBindSeparateTextureFunc) (CommandBuffer *cmdBuf, ShaderStage::Enum stage, int index, SeparateTextureHandle textureHandle);
typedef void (NVNAPIENTRY *CommandBufferBindSeparateSamplerFunc) (CommandBuffer *cmdBuf, ShaderStage::Enum stage, int index, SeparateSamplerHandle samplerHandle);
typedef void (NVNAPIENTRY *CommandBufferBindSeparateTexturesFunc) (CommandBuffer *cmdBuf, ShaderStage::Enum stage, int first, int count, const SeparateTextureHandle *textureHandles);
typedef void (NVNAPIENTRY *CommandBufferBindSeparateSamplersFunc) (CommandBuffer *cmdBuf, ShaderStage::Enum stage, int first, int count, const SeparateSamplerHandle *samplerHandles);
typedef void (NVNAPIENTRY *CommandBufferSetStencilValueMaskFunc) (CommandBuffer *cmdBuf, Face::Enum faces, int mask);
typedef void (NVNAPIENTRY *CommandBufferSetStencilMaskFunc) (CommandBuffer *cmdBuf, Face::Enum faces, int mask);
typedef void (NVNAPIENTRY *CommandBufferSetStencilRefFunc) (CommandBuffer *cmdBuf, Face::Enum faces, int ref);
typedef void (NVNAPIENTRY *CommandBufferSetBlendColorFunc) (CommandBuffer *cmdBuf, const float *blendColor);
typedef void (NVNAPIENTRY *CommandBufferSetPointSizeFunc) (CommandBuffer *cmdBuf, float pointSize);
typedef void (NVNAPIENTRY *CommandBufferSetLineWidthFunc) (CommandBuffer *cmdBuf, float lineWidth);
typedef void (NVNAPIENTRY *CommandBufferSetPolygonOffsetClampFunc) (CommandBuffer *cmdBuf, float factor, float units, float clamp);
typedef void (NVNAPIENTRY *CommandBufferSetAlphaRefFunc) (CommandBuffer *cmdBuf, float ref);
typedef void (NVNAPIENTRY *CommandBufferSetSampleMaskFunc) (CommandBuffer *cmdBuf, int mask);
typedef void (NVNAPIENTRY *CommandBufferSetRasterizerDiscardFunc) (CommandBuffer *cmdBuf, NVNboolean discard);
typedef void (NVNAPIENTRY *CommandBufferSetDepthClampFunc) (CommandBuffer *cmdBuf, NVNboolean clamp);
typedef void (NVNAPIENTRY *CommandBufferSetConservativeRasterEnableFunc) (CommandBuffer *cmdBuf, NVNboolean enable);
typedef void (NVNAPIENTRY *CommandBufferSetConservativeRasterDilateFunc) (CommandBuffer *cmdBuf, float dilate);
typedef void (NVNAPIENTRY *CommandBufferSetSubpixelPrecisionBiasFunc) (CommandBuffer *cmdBuf, int xBits, int yBits);
typedef void (NVNAPIENTRY *CommandBufferCopyBufferToTextureFunc) (CommandBuffer *cmdBuf, BufferAddress src, const Texture *dstTexture, const TextureView *dstView, const CopyRegion *dstRegion, int flags);
typedef void (NVNAPIENTRY *CommandBufferCopyTextureToBufferFunc) (CommandBuffer *cmdBuf, const Texture *srcTexture, const TextureView *srcView, const CopyRegion *srcRegion, BufferAddress dst, int flags);
typedef void (NVNAPIENTRY *CommandBufferCopyTextureToTextureFunc) (CommandBuffer *cmdBuf, const Texture *srcTexture, const TextureView *srcView, const CopyRegion *srcRegion, const Texture *dstTexture, const TextureView *dstView, const CopyRegion *dstRegion, int flags);
typedef void (NVNAPIENTRY *CommandBufferCopyBufferToBufferFunc) (CommandBuffer *cmdBuf, BufferAddress src, BufferAddress dst, size_t size, int flags);
typedef void (NVNAPIENTRY *CommandBufferClearBufferFunc) (CommandBuffer *cmdBuf, BufferAddress dst, size_t size, uint32_t value);
typedef void (NVNAPIENTRY *CommandBufferClearTextureFunc) (CommandBuffer *cmdBuf, const Texture *dstTexture, const TextureView *dstView, const CopyRegion *dstRegion, const float *color, int mask);
typedef void (NVNAPIENTRY *CommandBufferClearTextureiFunc) (CommandBuffer *cmdBuf, const Texture *dstTexture, const TextureView *dstView, const CopyRegion *dstRegion, const int *color, int mask);
typedef void (NVNAPIENTRY *CommandBufferClearTextureuiFunc) (CommandBuffer *cmdBuf, const Texture *dstTexture, const TextureView *dstView, const CopyRegion *dstRegion, const uint32_t *color, int mask);
typedef void (NVNAPIENTRY *CommandBufferUpdateUniformBufferFunc) (CommandBuffer *cmdBuf, BufferAddress buffer, size_t bufferSize, ptrdiff_t updateOffset, size_t updateSize, const void *data);
typedef void (NVNAPIENTRY *CommandBufferReportCounterFunc) (CommandBuffer *cmdBuf, CounterType::Enum counter, BufferAddress buffer);
typedef void (NVNAPIENTRY *CommandBufferResetCounterFunc) (CommandBuffer *cmdBuf, CounterType::Enum counter);
typedef void (NVNAPIENTRY *CommandBufferReportValueFunc) (CommandBuffer *cmdBuf, uint32_t value, BufferAddress buffer);
typedef void (NVNAPIENTRY *CommandBufferSetRenderEnableFunc) (CommandBuffer *cmdBuf, NVNboolean enable);
typedef void (NVNAPIENTRY *CommandBufferSetRenderEnableConditionalFunc) (CommandBuffer *cmdBuf, ConditionalRenderMode::Enum mode, BufferAddress counters);
typedef void (NVNAPIENTRY *CommandBufferSetRenderTargetsFunc) (CommandBuffer *cmdBuf, int numColors, const Texture * const *colors, const TextureView * const *colorViews, const Texture *depthStencil, const TextureView *depthStencilView);
typedef void (NVNAPIENTRY *CommandBufferDiscardColorFunc) (CommandBuffer *cmdBuf, int index);
typedef void (NVNAPIENTRY *CommandBufferDiscardDepthStencilFunc) (CommandBuffer *cmdBuf);
typedef void (NVNAPIENTRY *CommandBufferDownsampleFunc) (CommandBuffer *cmdBuf, const Texture *src, const Texture *dst);
typedef void (NVNAPIENTRY *CommandBufferTiledDownsampleFunc) (CommandBuffer *cmdBuf, const Texture *src, const Texture *dst);
typedef void (NVNAPIENTRY *CommandBufferDownsampleTextureViewFunc) (CommandBuffer *cmdBuf, const Texture *src, const TextureView *srcView, const Texture *dst, const TextureView *dstView);
typedef void (NVNAPIENTRY *CommandBufferTiledDownsampleTextureViewFunc) (CommandBuffer *cmdBuf, const Texture *src, const TextureView *srcView, const Texture *dst, const TextureView *dstView);
typedef void (NVNAPIENTRY *CommandBufferBarrierFunc) (CommandBuffer *cmdBuf, int barrier);
typedef void (NVNAPIENTRY *CommandBufferWaitSyncFunc) (CommandBuffer *cmdBuf, const Sync *sync);
typedef void (NVNAPIENTRY *CommandBufferFenceSyncFunc) (CommandBuffer *cmdBuf, Sync *sync, SyncCondition::Enum condition, int flags);
typedef void (NVNAPIENTRY *CommandBufferSetTexturePoolFunc) (CommandBuffer *cmdBuf, const TexturePool *texturePool);
typedef void (NVNAPIENTRY *CommandBufferSetSamplerPoolFunc) (CommandBuffer *cmdBuf, const SamplerPool *samplerPool);
typedef void (NVNAPIENTRY *CommandBufferSetShaderScratchMemoryFunc) (CommandBuffer *cmdBuf, const MemoryPool *pool, ptrdiff_t offset, size_t size);
typedef void (NVNAPIENTRY *CommandBufferSaveZCullDataFunc) (CommandBuffer *cmdBuf, BufferAddress dst, size_t size);
typedef void (NVNAPIENTRY *CommandBufferRestoreZCullDataFunc) (CommandBuffer *cmdBuf, BufferAddress src, size_t size);
typedef void (NVNAPIENTRY *CommandBufferSetCopyRowStrideFunc) (CommandBuffer *cmdBuf, ptrdiff_t stride);
typedef void (NVNAPIENTRY *CommandBufferSetCopyImageStrideFunc) (CommandBuffer *cmdBuf, ptrdiff_t stride);
typedef ptrdiff_t (NVNAPIENTRY *CommandBufferGetCopyRowStrideFunc) (const CommandBuffer *cmdBuf);
typedef ptrdiff_t (NVNAPIENTRY *CommandBufferGetCopyImageStrideFunc) (const CommandBuffer *cmdBuf);
typedef void (NVNAPIENTRY *CommandBufferDrawTextureFunc) (CommandBuffer *cmdBuf, TextureHandle texture, const DrawTextureRegion *dstRegion, const DrawTextureRegion *srcRegion);
typedef NVNboolean (NVNAPIENTRY *ProgramSetSubroutineLinkageFunc) (Program *program, int count, const NVNsubroutineLinkageMapPtr *linkageMapPtrs);
typedef void (NVNAPIENTRY *CommandBufferSetProgramSubroutinesFunc) (CommandBuffer *cmdBuf, Program *program, ShaderStage::Enum stage, const int first, const int count, const int *values);
typedef void (NVNAPIENTRY *CommandBufferBindCoverageModulationTableFunc) (CommandBuffer *cmdBuf, const float *entries);
typedef void (NVNAPIENTRY *CommandBufferResolveDepthBufferFunc) (CommandBuffer *cmdBuf);
typedef void (NVNAPIENTRY *CommandBufferSetColorReductionEnableFunc) (CommandBuffer *cmdBuf, NVNboolean enable);
typedef void (NVNAPIENTRY *CommandBufferSetColorReductionThresholdsFunc) (CommandBuffer *cmdBuf, FormatClass::Enum formatClass, int thresholdConservative, int thresholdAggressive);
typedef void (NVNAPIENTRY *CommandBufferPushDebugGroupStaticFunc) (CommandBuffer *cmdBuf, uint32_t domainId, const char * description);
typedef void (NVNAPIENTRY *CommandBufferPushDebugGroupDynamicFunc) (CommandBuffer *cmdBuf, uint32_t domainId, const char * description);
typedef void (NVNAPIENTRY *CommandBufferPushDebugGroupFunc) (CommandBuffer *cmdBuf, const char * description);
typedef void (NVNAPIENTRY *CommandBufferPopDebugGroupFunc) (CommandBuffer *cmdBuf);
typedef void (NVNAPIENTRY *CommandBufferPopDebugGroupIdFunc) (CommandBuffer *cmdBuf, uint32_t domainId);
typedef void (NVNAPIENTRY *CommandBufferInsertDebugMarkerStaticFunc) (CommandBuffer *cmdBuf, uint32_t domainId, const char * description);
typedef void (NVNAPIENTRY *CommandBufferInsertDebugMarkerDynamicFunc) (CommandBuffer *cmdBuf, uint32_t domainId, const char * description);
typedef void (NVNAPIENTRY *CommandBufferInsertDebugMarkerFunc) (CommandBuffer *cmdBuf, const char * description);
typedef CommandBufferMemoryCallbackFunc (NVNAPIENTRY *CommandBufferGetMemoryCallbackFunc) (const CommandBuffer *cmdBuf);
typedef void* (NVNAPIENTRY *CommandBufferGetMemoryCallbackDataFunc) (const CommandBuffer *cmdBuf);
typedef NVNboolean (NVNAPIENTRY *CommandBufferIsRecordingFunc) (const CommandBuffer *cmdBuf);
typedef NVNboolean (NVNAPIENTRY *SyncInitializeFunc) (Sync *sync, Device *device);
typedef void (NVNAPIENTRY *SyncFinalizeFunc) (Sync *sync);
typedef void (NVNAPIENTRY *SyncSetDebugLabelFunc) (Sync *sync, const char * label);
typedef void (NVNAPIENTRY *QueueFenceSyncFunc) (Queue *queue, Sync *sync, SyncCondition::Enum condition, int flags);
typedef SyncWaitResult::Enum (NVNAPIENTRY *SyncWaitFunc) (const Sync *sync, uint64_t timeoutNs);
typedef NVNboolean (NVNAPIENTRY *QueueWaitSyncFunc) (Queue *queue, const Sync *sync);
typedef NVNboolean (NVNAPIENTRY *SyncInitializeFromFencedGLSyncFunc) (Sync *sync, Device *device, const void *glSync, SyncInteropStatus *status);
typedef SyncInteropStatus::Enum (NVNAPIENTRY *SyncCreateGLSyncFunc) (const Sync *sync, uint64_t *glSync);
typedef void (NVNAPIENTRY *EventBuilderSetDefaultsFunc) (EventBuilder *builder);
typedef void (NVNAPIENTRY *EventBuilderSetStorageFunc) (EventBuilder *builder, const MemoryPool *pool, int64_t offset);
typedef const MemoryPool * (NVNAPIENTRY *EventBuilderGetStorageFunc) (const EventBuilder *builder, int64_t *offset);
typedef NVNboolean (NVNAPIENTRY *EventInitializeFunc) (Event *event, const EventBuilder *builder);
typedef void (NVNAPIENTRY *EventFinalizeFunc) (Event *event);
typedef uint32_t (NVNAPIENTRY *EventGetValueFunc) (const Event *event);
typedef void (NVNAPIENTRY *EventSignalFunc) (Event *event, EventSignalMode::Enum mode, uint32_t value);
typedef void (NVNAPIENTRY *CommandBufferWaitEventFunc) (CommandBuffer *cmdBuf, const Event *event, EventWaitMode::Enum waitMode, uint32_t value);
typedef void (NVNAPIENTRY *CommandBufferSignalEventFunc) (CommandBuffer *cmdBuf, const Event *event, EventSignalMode::Enum mode, EventSignalLocation::Enum location, int flags, uint32_t value);
typedef void (NVNAPIENTRY *CommandBufferSetStencilCullCriteriaFunc) (CommandBuffer *cmdBuf, StencilFunc::Enum func, int ref, int mask);

/// \}

#ifdef NVN_OVERLOAD_CPP_OBJECTS
} // namespace nvn::objects
#endif

} // namespace nvn

/// \}

#if (defined(_MSC_VER) || defined(__clang__) || \
     (defined(__GNUC__) && !defined(__sun__) && !defined(__FreeBSD__)))
#pragma pack(pop)
#endif

#endif /* __nvn_cpp_h_ */
