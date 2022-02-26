/*--------------------------------------------------------------------------------*
  Copyright (C)Nintendo All rights reserved.

  These coded instructions, statements, and computer programs contain proprietary
  information of Nintendo and/or its licensed developers and are protected by
  national and international copyright laws. They may not be disclosed to third
  parties or copied or duplicated in any form, in whole or in part, without the
  prior written consent of Nintendo.

  The content herein is highly confidential and should be handled accordingly.
 *--------------------------------------------------------------------------------*/


#ifndef __nvn_h_
#define __nvn_h_

/// \file nvn.h
///     C headers for the NVN 3D API.
/// \defgroup nvn_c_interface NVN C API Bindings
///     C Interface for NVN API version 53.311
/// \{

/// \defgroup nvn_c_functions API Functions
///     C Interface API entry points.
/// \defgroup nvn_c_objects API Objects
///     C Interface API object types.
/// \defgroup nvn_c_api_typedefs API Basic Types
///     C Interface basic data types.
/// \defgroup nvn_c_enum API Enum Types
///     C Interface enumerated types.
/// \defgroup nvn_c_handle API Handle Types
///     C Interface handles for GPU resources.
/// \defgroup nvn_c_struct API Structure Types
///     C Interface structure data types.
/// \defgroup nvn_c_funcptrs API Function Pointer Types
///     C Interface function pointer data types.
/// \defgroup nvn_c_defines API #defines
///     C Interface miscellaneous \#defines.
/// \defgroup nvn_c_entryfuncptrs API Entry Point Function Pointers
///     C Interface function pointer types for API entry points.

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

/// \addtogroup nvn_c_defines
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

/// \addtogroup nvn_c_api_typedefs
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

/// \addtogroup nvn_c_api_typedefs
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
/// nvnTextureBuilderGetStorageClass or nvnTextureGetStorageClass.  For
/// buffers, NVN_STORAGE_CLASS_BUFFER should be used.
typedef int NVNstorageClass;

/// \brief Helps keep track of domains in debug groups.
typedef int NVNdebugDomainId;


/// \}
/// \addtogroup nvn_c_objects
/// \{

typedef struct NVNdeviceBuilder NVNdeviceBuilder;
typedef struct NVNdevice NVNdevice;
typedef struct NVNqueueBuilder NVNqueueBuilder;
typedef struct NVNqueue NVNqueue;
typedef struct NVNcommandBuffer NVNcommandBuffer;
typedef struct NVNblendState NVNblendState;
typedef struct NVNchannelMaskState NVNchannelMaskState;
typedef struct NVNcolorState NVNcolorState;
typedef struct NVNdepthStencilState NVNdepthStencilState;
typedef struct NVNmultisampleState NVNmultisampleState;
typedef struct NVNpolygonState NVNpolygonState;
typedef struct NVNvertexAttribState NVNvertexAttribState;
typedef struct NVNvertexStreamState NVNvertexStreamState;
typedef struct NVNprogram NVNprogram;
typedef struct NVNmemoryPoolBuilder NVNmemoryPoolBuilder;
typedef struct NVNmemoryPool NVNmemoryPool;
typedef struct NVNtexturePool NVNtexturePool;
typedef struct NVNsamplerPool NVNsamplerPool;
typedef struct NVNbufferBuilder NVNbufferBuilder;
typedef struct NVNbuffer NVNbuffer;
typedef struct NVNtexture NVNtexture;
typedef struct NVNtextureBuilder NVNtextureBuilder;
typedef struct NVNtextureView NVNtextureView;
typedef struct NVNsamplerBuilder NVNsamplerBuilder;
typedef struct NVNsampler NVNsampler;
typedef struct NVNsync NVNsync;
typedef struct NVNwindow NVNwindow;
typedef struct NVNwindowBuilder NVNwindowBuilder;
typedef struct NVNeventBuilder NVNeventBuilder;
typedef struct NVNevent NVNevent;

#ifndef NVN_OBJECTS_DEFINED
/// \brief Object specifying state used to construct new device objects.
NVN_PRE_ALIGN(8) struct NVNdeviceBuilder {
    char reserved[64];
} NVN_POST_ALIGN(8);

/// \brief API class used to represent a specific GPU/device.
NVN_PRE_ALIGN(8) struct NVNdevice {
    char reserved[12288];
} NVN_POST_ALIGN(8);

/// \brief Object specifying state used to construct new queue objects.
NVN_PRE_ALIGN(8) struct NVNqueueBuilder {
    char reserved[64];
} NVN_POST_ALIGN(8);

/// \brief API class used to send commands to the GPU.
NVN_PRE_ALIGN(8) struct NVNqueue {
    char reserved[8192];
} NVN_POST_ALIGN(8);

/// \brief Collection of commands to send to the GPU via queues.
NVN_PRE_ALIGN(8) struct NVNcommandBuffer {
    char reserved[160];
} NVN_POST_ALIGN(8);

/// \brief API state object controlling blending for one color target.
NVN_PRE_ALIGN(8) struct NVNblendState {
    char reserved[8];
} NVN_POST_ALIGN(8);

/// \brief API state object holding per-channel write masks for all color
/// buffers.
NVN_PRE_ALIGN(4) struct NVNchannelMaskState {
    char reserved[4];
} NVN_POST_ALIGN(4);

/// \brief API state object controlling processing of color values.
NVN_PRE_ALIGN(4) struct NVNcolorState {
    char reserved[4];
} NVN_POST_ALIGN(4);

/// \brief API state object controlling depth and stencil testing
/// operations.
NVN_PRE_ALIGN(8) struct NVNdepthStencilState {
    char reserved[8];
} NVN_POST_ALIGN(8);

/// \brief API state object controlling multisample rasterization and
/// sample processing.
NVN_PRE_ALIGN(8) struct NVNmultisampleState {
    char reserved[24];
} NVN_POST_ALIGN(8);

/// \brief API state object controlling rasterization of polygons.
NVN_PRE_ALIGN(4) struct NVNpolygonState {
    char reserved[4];
} NVN_POST_ALIGN(4);

/// \brief API state object controlling vertex attrib state.
NVN_PRE_ALIGN(4) struct NVNvertexAttribState {
    char reserved[4];
} NVN_POST_ALIGN(4);

/// \brief API state object controlling vertex stream state.
NVN_PRE_ALIGN(8) struct NVNvertexStreamState {
    char reserved[8];
} NVN_POST_ALIGN(8);

/// \brief Collection of programmable shaders used to process primitives.
NVN_PRE_ALIGN(8) struct NVNprogram {
    char reserved[192];
} NVN_POST_ALIGN(8);

/// \brief Object specifying state used to construct new memory pool
/// objects.
NVN_PRE_ALIGN(8) struct NVNmemoryPoolBuilder {
    char reserved[64];
} NVN_POST_ALIGN(8);

/// \brief Block of GPU-accessible memory that can be used for storage of
/// buffer and texture objects.
NVN_PRE_ALIGN(8) struct NVNmemoryPool {
    char reserved[256];
} NVN_POST_ALIGN(8);

/// \brief Block of GPU-accessible memory that can be used to store
/// descriptors for texture objects.
NVN_PRE_ALIGN(8) struct NVNtexturePool {
    char reserved[32];
} NVN_POST_ALIGN(8);

/// \brief Block of GPU-accessible memory that can be used to store
/// descriptors for sampler objects.
NVN_PRE_ALIGN(8) struct NVNsamplerPool {
    char reserved[32];
} NVN_POST_ALIGN(8);

/// \brief Object specifying state used to construct new buffer objects.
NVN_PRE_ALIGN(8) struct NVNbufferBuilder {
    char reserved[64];
} NVN_POST_ALIGN(8);

/// \brief Block of GPU-accessible memory used to hold data used for
/// various purposes.
NVN_PRE_ALIGN(8) struct NVNbuffer {
    char reserved[48];
} NVN_POST_ALIGN(8);

/// \brief Block of GPU-accessible memory and related state used to hold
/// image data.
NVN_PRE_ALIGN(8) struct NVNtexture {
    char reserved[192];
} NVN_POST_ALIGN(8);

/// \brief Object specifying state used to construct new texture objects.
NVN_PRE_ALIGN(8) struct NVNtextureBuilder {
    char reserved[128];
} NVN_POST_ALIGN(8);

/// \brief Object used to specify an alternate 'view' of a full texture
/// object for various rendering operations.
///
/// The NVNtextureView object can be used to specify an alternate 'view' of
/// the storage of a previously created texture object.  Views allow
/// applications to re-interpret texture storage using a different texel
/// format, component swizzle pattern, depth/stencil mode, or target, and
/// can be used to select a subset of the full set of texture mipmap levels
/// or array texture layers.  When a texture object is used with a non-NULL
/// view, any properties not set explicitly by texture view class methods
/// will be taken from the original texture object.  In APIs accepting
/// texture views, a NULL pointer can be used to indicate that all
/// properties should be taken from the original texture object.
NVN_PRE_ALIGN(8) struct NVNtextureView {
    char reserved[40];
} NVN_POST_ALIGN(8);

/// \brief Object specifying state used to construct new sampler objects.
NVN_PRE_ALIGN(8) struct NVNsamplerBuilder {
    char reserved[96];
} NVN_POST_ALIGN(8);

/// \brief GPU-accessible object controlling how texture data are fetched
/// and filtered.
NVN_PRE_ALIGN(8) struct NVNsampler {
    char reserved[96];
} NVN_POST_ALIGN(8);

/// \brief Synchronization primitive object used to order execution of
/// commands and track progress.
NVN_PRE_ALIGN(8) struct NVNsync {
    char reserved[64];
} NVN_POST_ALIGN(8);

/// \brief Object that manages texture presentation to the platform
/// windowing system.
NVN_PRE_ALIGN(8) struct NVNwindow {
    char reserved[384];
} NVN_POST_ALIGN(8);

/// \brief Object specifying state used to construct new window objects.
NVN_PRE_ALIGN(8) struct NVNwindowBuilder {
    char reserved[64];
} NVN_POST_ALIGN(8);

/// \brief Object specifying state used to construct new event objects.
NVN_PRE_ALIGN(8) struct NVNeventBuilder {
    char reserved[32];
} NVN_POST_ALIGN(8);

/// \brief NVNevent object.
NVN_PRE_ALIGN(8) struct NVNevent {
    char reserved[64];
} NVN_POST_ALIGN(8);

#define NVN_OBJECTS_DEFINED
#endif // #ifndef NVN_OBJECTS_DEFINED

/// \}
/// \addtogroup nvn_c_handle
/// \{

/// \brief GPU address of memory inside a buffer object.
typedef uint64_t NVNbufferAddress;

/// \brief GPU handle used to refer to a command buffer object.
typedef uint64_t NVNcommandHandle;

/// \brief GPU handle used to refer to a texture object for texture mapping
/// operations.
typedef uint64_t NVNtextureHandle;

/// \brief GPU handle used to refer to a texture object for image load and
/// store operations.
typedef uint64_t NVNimageHandle;

/// \brief GPU address of memory inside a buffer object.
///
/// \warning Except for buffer and linear textures, a NVNtextureAddress
/// returned by nvnTextureGetTextureAddress corresponds to a special memory
/// mapping that can not be used to view or manipulate memory in commands
/// accepting a buffer address.
typedef uint64_t NVNtextureAddress;

/// \}
/// \addtogroup nvn_c_struct
/// \{

/// \brief Structure containing a GPU handle used to refer to a separate
/// sampler object for texture mapping operations.
///
/// To use a separate sampler handle to perform a texture lookup in a
/// shader, it must be combined with a separate texture handle in shader
/// code.
typedef struct NVNseparateSamplerHandle {

    /// \brief GPU handle used to refer to a separate sampler object for
    /// texture mapping operations.
    uint64_t    value;

} NVNseparateSamplerHandle;

/// \brief Structure containing a GPU handle used to refer to a separate
/// texture object for texture mapping operations.
///
/// To use a separate texture handle to perform a texture lookup in a
/// shader, it must be combined with a separate sampler handle in shader
/// code.
typedef struct NVNseparateTextureHandle {

    /// \brief GPU handle used to refer to a separate texture object for
    /// texture mapping operations.
    uint64_t    value;

} NVNseparateTextureHandle;

/// \}
/// \addtogroup nvn_c_enum
/// \{

/// \brief Identifies a property of the device to query.
typedef enum NVNdeviceInfo {

    /// \brief Identifies the major version of the API.
    ///
    /// Major API version can be queried before creating an NVN device by
    /// passing a NULL device pointer to nvnDeviceGetInteger.
    NVN_DEVICE_INFO_API_MAJOR_VERSION = 0,

    /// \brief Identifies the minor version of the API.
    ///
    /// Minor API version can be queried before creating an NVN device by
    /// passing a NULL device pointer to nvnDeviceGetInteger.
    NVN_DEVICE_INFO_API_MINOR_VERSION = 1,

    /// \brief Number of uniform buffer bindings supported for each shader
    /// stage.
    NVN_DEVICE_INFO_UNIFORM_BUFFER_BINDINGS_PER_STAGE = 2,

    /// \brief Maximum size (in bytes) of a bound uniform buffer.
    NVN_DEVICE_INFO_MAX_UNIFORM_BUFFER_SIZE = 3,

    /// \brief Minimum alignment of uniform buffer bindings.
    NVN_DEVICE_INFO_UNIFORM_BUFFER_ALIGNMENT = 4,

    /// \brief Number of color buffer binding points.
    NVN_DEVICE_INFO_COLOR_BUFFER_BINDINGS = 5,

    /// \brief Number of vertex buffer binding points.
    NVN_DEVICE_INFO_VERTEX_BUFFER_BINDINGS = 6,

    /// \brief Number of transform feedback buffer binding points.
    NVN_DEVICE_INFO_TRANSFORM_FEEDBACK_BUFFER_BINDINGS = 7,

    /// \brief Number of shader storage buffer bindings supported for each
    /// shader stage.
    NVN_DEVICE_INFO_SHADER_STORAGE_BUFFER_BINDINGS_PER_STAGE = 8,

    /// \brief Number of texture and sampler bindings supported for each
    /// shader stage.
    NVN_DEVICE_INFO_TEXTURE_BINDINGS_PER_STAGE = 9,

    /// \brief Minimum alignment for counter reports.
    NVN_DEVICE_INFO_COUNTER_ALIGNMENT = 10,

    /// \brief Minimum alignment of transform feedback buffer bindings.
    NVN_DEVICE_INFO_TRANSFORM_FEEDBACK_BUFFER_ALIGNMENT = 11,

    /// \brief Minimum alignment of transform feedback control data.
    NVN_DEVICE_INFO_TRANSFORM_FEEDBACK_CONTROL_ALIGNMENT = 12,

    /// \brief Minimum alignment of indirect draw data.
    NVN_DEVICE_INFO_INDIRECT_DRAW_ALIGNMENT = 13,

    /// \brief Number of vertex attributes.
    NVN_DEVICE_INFO_VERTEX_ATTRIBUTES = 14,

    /// \brief Size (in bytes) of a single texture descriptor stored in the
    /// pool.
    NVN_DEVICE_INFO_TEXTURE_DESCRIPTOR_SIZE = 15,

    /// \brief Size in bytes of a single sampler descriptor.
    NVN_DEVICE_INFO_SAMPLER_DESCRIPTOR_SIZE = 16,

    /// \brief Number of entries reserved at the beginning of the texture
    /// pool for internal driver usage.
    NVN_DEVICE_INFO_RESERVED_TEXTURE_DESCRIPTORS = 17,

    /// \brief Number of samplers that must be reserved for use by the
    /// driver.
    NVN_DEVICE_INFO_RESERVED_SAMPLER_DESCRIPTORS = 18,

    /// \brief Minimum alignment for command data in a command builder.
    NVN_DEVICE_INFO_COMMAND_BUFFER_COMMAND_ALIGNMENT = 19,

    /// \brief Minimum alignment for control data in a command builder.
    NVN_DEVICE_INFO_COMMAND_BUFFER_CONTROL_ALIGNMENT = 20,

    /// \brief Minimum size (in bytes) recommended for command data in a
    /// command builder.
    NVN_DEVICE_INFO_COMMAND_BUFFER_MIN_COMMAND_SIZE = 21,

    /// \brief Minimum size (in bytes) recommended for control data in a
    /// command builder.
    NVN_DEVICE_INFO_COMMAND_BUFFER_MIN_CONTROL_SIZE = 22,

    /// \brief Minimum scale factor for providing scratch memory for
    /// non-compute shaders.
    ///
    /// The minimum total amount of memory required to successfully run a
    /// non-compute shader can be computed by multiplying the amount of
    /// memory required for each shader 'warp' (collection of 32 threads)
    /// by this factor, and padding to a multiple of the value of the
    /// property #NVN_DEVICE_INFO_SHADER_SCRATCH_MEMORY_GRANULARITY.  The
    /// per-warp scratch memory usage for a shader is provided in the
    /// shader compiler output.
    ///
    /// \note The minimum scaling factor may be different for compute and
    /// non-compute shaders.
    NVN_DEVICE_INFO_SHADER_SCRATCH_MEMORY_SCALE_FACTOR_MINIMUM = 23,

    /// \brief Scale factor for the recommended amount of memory
    ///
    /// The recommended total amount of memory required to successfully run
    /// a non-compute shader can be computed by multiplying the amount of
    /// memory required for each shader 'warp' (collection of 32 threads)
    /// by this factor, and then padding to a multiple of the value of the
    /// property #NVN_DEVICE_INFO_SHADER_SCRATCH_MEMORY_GRANULARITY.  The
    /// per-warp scratch memory usage for a shader, as well as the
    /// recommended total memory size for NX, are provided in the shader
    /// compiler output.
    NVN_DEVICE_INFO_SHADER_SCRATCH_MEMORY_SCALE_FACTOR_RECOMMENDED = 24,

    /// \brief Alignment required (in bytes) for the shader scratch memory.
    NVN_DEVICE_INFO_SHADER_SCRATCH_MEMORY_ALIGNMENT = 25,

    /// \brief Required granularity for shader scratch memory provided to
    /// NVN.
    NVN_DEVICE_INFO_SHADER_SCRATCH_MEMORY_GRANULARITY = 26,

    /// \brief Maximum sampler anisotropy.
    NVN_DEVICE_INFO_MAX_TEXTURE_ANISOTROPY = 27,

    /// \brief Maximum number of threads in the X dimension supported in a
    /// compute work group.
    NVN_DEVICE_INFO_MAX_COMPUTE_WORK_GROUP_SIZE_X = 28,

    /// \brief Maximum number of threads in the Y dimension supported in a
    /// compute work group.
    NVN_DEVICE_INFO_MAX_COMPUTE_WORK_GROUP_SIZE_Y = 29,

    /// \brief Maximum number of threads in the Z dimension supported in a
    /// compute work group.
    NVN_DEVICE_INFO_MAX_COMPUTE_WORK_GROUP_SIZE_Z = 30,

    /// \brief Maximum total number of threads supported in a compute work
    /// group.
    NVN_DEVICE_INFO_MAX_COMPUTE_WORK_GROUP_SIZE_THREADS = 31,

    /// \brief Maximum number of work groups in the X dimension supported
    /// in a compute dispatch.
    NVN_DEVICE_INFO_MAX_COMPUTE_DISPATCH_WORK_GROUPS_X = 32,

    /// \brief Maximum number of work groups in the Y dimension supported
    /// in a compute dispatch.
    NVN_DEVICE_INFO_MAX_COMPUTE_DISPATCH_WORK_GROUPS_Y = 33,

    /// \brief Maximum number of work groups in the Z dimension supported
    /// in a compute dispatch.
    NVN_DEVICE_INFO_MAX_COMPUTE_DISPATCH_WORK_GROUPS_Z = 34,

    /// \brief Number of image bindings supported for each shader stage.
    NVN_DEVICE_INFO_IMAGE_BINDINGS_PER_STAGE = 35,

    /// \brief Maximum number of entries (including reserved ones)
    /// supported in a texture descriptor pool.
    NVN_DEVICE_INFO_MAX_TEXTURE_POOL_SIZE = 36,

    /// \brief Maximum number of entries (including reserved ones)
    /// supported in a sampler descriptor pool.
    NVN_DEVICE_INFO_MAX_SAMPLER_POOL_SIZE = 37,

    /// \brief Maximum number of viewports.
    NVN_DEVICE_INFO_MAX_VIEWPORTS = 38,

    /// \brief Granularity of memory pool regions (pages or sub-page
    /// units), where each region may not contain both texture and buffer
    /// resources.
    ///
    /// For textures (other than those with a target of
    /// TEXTURE_TARGET::TARGET_BUFFER), the GPU memory subsystem arranges
    /// the texture contents in its GPU memory pages for optimal
    /// performance.  This arrangment can cause memory addresses used by a
    /// texture and a buffer in the same page to collide, even if their
    /// nominal ranges within the pool don't overlap.  Applications must
    /// not place texture and buffer resources within any 4KB-aligned
    /// region of the memory pool.  On some GPUs supported by the Windows
    /// reference implementation, these aligned regions are 64KB in size.
    NVN_DEVICE_INFO_MEMPOOL_TEXTURE_OBJECT_PAGE_ALIGNMENT = 39,

    /// \brief 1 if #NVN_SAMPLER_REDUCTION_MIN and
    /// #NVN_SAMPLER_REDUCTION_MAX are supported by the NVN implementation;
    /// 0 otherwise.
    ///
    /// These modes are not supported on the Windows reference
    /// implementation for GPUs older than second-generation Maxwell GPUs.
    NVN_DEVICE_INFO_SUPPORTS_MIN_MAX_FILTERING = 40,

    /// \brief 1 if #NVN_FORMAT_STENCIL8 is supported by the NVN
    /// implementation; 0 otherwise.
    ///
    /// This format is not supported on the Windows reference
    /// implementation for GPUs older than second-generation Maxwell GPUs.
    NVN_DEVICE_INFO_SUPPORTS_STENCIL8_FORMAT = 41,

    /// \brief 1 if ASTC texture formats are supported by the NVN
    /// implementation; 0 otherwise.
    ///
    /// Some NVN implementations do not natively support ASTC texture
    /// compression. For the NVN Windows reference implementation, limited
    /// ASTC support is emulated in the driver. For details, please refer
    /// to the
    /// \ref nvn_guide_formats_restrictions_on_Windows
    /// "Formats Restrictions on the NVN Windows Reference Implementation"
    /// section of the Programming Guide.
    NVN_DEVICE_INFO_SUPPORTS_ASTC_FORMATS = 42,

    /// \brief Total size of L2 cache in the GPU core.
    NVN_DEVICE_INFO_L2_SIZE = 43,

    /// \brief Maximum number of levels supported for any texture target.
    NVN_DEVICE_INFO_MAX_TEXTURE_LEVELS = 44,

    /// \brief Maximum number of layers supported for any array texture
    /// target.
    ///
    /// For cube map arrays, this value should be divided by 6 to obtain
    /// the number of cube map array layers supported; i.e., the limit
    /// applies to the total number of cubemap faces in the texture.
    NVN_DEVICE_INFO_MAX_TEXTURE_LAYERS = 45,

    /// \brief Maximum supported GLSLC GPU code section binary major
    /// version.
    ///
    /// For precompiled shaders produced with the offline shader compiler,
    /// GLSLC, this represents the maximum major version number of the GPU
    /// code section binary that NVN can support.  It is an error to try to
    /// import a binary whose major/minor versions are greater than the
    /// maximum supported version.
    NVN_DEVICE_INFO_GLSLC_MAX_SUPPORTED_GPU_CODE_MAJOR_VERSION = 46,

    /// \brief Minimum supported GLSLC binary version.
    ///
    /// For precompiled shaders produced with the offline shader compiler,
    /// GLSLC, this represents the minimum major version number of the GPU
    /// code section binary that NVN can support.  It is an error to try to
    /// import a binary whose major/minor versions are less than the
    /// minimum supported version.
    NVN_DEVICE_INFO_GLSLC_MIN_SUPPORTED_GPU_CODE_MAJOR_VERSION = 47,

    /// \brief Maximum supported GLSLC binary version.
    ///
    /// For precompiled shaders produced with the offline shader compiler,
    /// GLSLC, this represents the maximum minor version number of the GPU
    /// code section binary that NVN can support.  It is an error to try to
    /// import a binary whose major/minor versions are greater than the
    /// maximum supported version.
    NVN_DEVICE_INFO_GLSLC_MAX_SUPPORTED_GPU_CODE_MINOR_VERSION = 48,

    /// \brief Minimum supported GLSLC binary version.
    ///
    /// For precompiled shaders produced with the offline shader compiler,
    /// GLSLC, this represents the minimum minor version number of the GPU
    /// code section binary that NVN can support.  It is an error to try to
    /// import a binary whose major/minor versions are less than the
    /// minimum supported version.
    NVN_DEVICE_INFO_GLSLC_MIN_SUPPORTED_GPU_CODE_MINOR_VERSION = 49,

    /// \brief 1 if conservative rasterization is supported supported by
    /// the NVN implementation; 0 otherwise.
    ///
    /// Conservative rasterization requires a second-generation or newer
    /// Maxwell GPU.
    NVN_DEVICE_INFO_SUPPORTS_CONSERVATIVE_RASTER = 50,

    /// \brief The number of bits of subpixel precision used to represent
    /// vertex window coordinates.
    NVN_DEVICE_INFO_SUBPIXEL_BITS = 51,

    /// \brief The maximum number of additional bits of subpixel precision
    /// that can be used to represent vertex window coordinates during
    /// conservative rasterization.
    NVN_DEVICE_INFO_MAX_SUBPIXEL_BIAS_BITS = 52,

    /// \brief Minimum alignment of indirect dispatch data.
    NVN_DEVICE_INFO_INDIRECT_DISPATCH_ALIGNMENT = 53,

    /// \brief Minimum alignment for ZCull save/restore buffers.
    NVN_DEVICE_INFO_ZCULL_SAVE_RESTORE_ALIGNMENT = 54,

    /// \brief Minimum scale factor for providing scratch memory for
    /// compute shaders.
    ///
    /// The minimum total amount of memory required to successfully run a
    /// compute shader can be computed by multiplying the amount of memory
    /// required for each shader 'warp' (collection of 32 threads) by this
    /// factor, and then padding to a multiple of the value of the property
    /// #NVN_DEVICE_INFO_SHADER_SCRATCH_MEMORY_GRANULARITY.  The per-warp
    /// scratch memory usage for a shader is provided in the shader
    /// compiler output.
    ///
    /// \note The minimum scaling factor may be different for compute and
    /// non-compute shaders.
    NVN_DEVICE_INFO_SHADER_SCRATCH_MEMORY_COMPUTE_SCALE_FACTOR_MINIMUM = 55,

    /// \brief Minimum alignment required for stride in linear textures, in
    /// bytes.
    ///
    /// Stride (distance in bytes between consecutive rows) in linear
    /// textures must be a multiple of this value.
    NVN_DEVICE_INFO_LINEAR_TEXTURE_STRIDE_ALIGNMENT = 56,

    /// \brief Minimum alignment required for stride in linear render
    /// targets, in bytes.
    ///
    /// Stride (distance in bytes between consecutive rows) in linear
    /// render targets must be rounded up to a multiple of this value.
    NVN_DEVICE_INFO_LINEAR_RENDER_TARGET_STRIDE_ALIGNMENT = 57,

    /// \brief Size of memory pages used in physical and virtual memory
    /// pools.
    ///
    /// Physical and virtual memory pools are made up of a collection of
    /// fixed-size memory pages.  The size of all physical and virtual
    /// memory pools must be a multiple of the page size.   Virtual memory
    /// mappings must also be aligned on page size boundaries.
    NVN_DEVICE_INFO_MEMORY_POOL_PAGE_SIZE = 58,

    /// \brief 1 if the implementation always returns zero values when
    /// reading from unpopulated portions of virtual memory pools; 0
    /// otherwise.
    ///
    /// On NX and second-generation Maxwell GPUs, virtual memory pools
    /// include hardware support that returns zero when accessing
    /// unpopulated portions of virtual memory pools.  On older GPUs,
    /// values returned from such accesses are undefined.
    NVN_DEVICE_INFO_SUPPORTS_ZERO_FROM_UNMAPPED_VIRTUAL_POOL_PAGES = 59,

    /// \brief Alignment required (in bytes) for the offset and size of
    /// uniform buffer updates via nvnCommandBufferUpdateUniformBuffer.
    NVN_DEVICE_INFO_UNIFORM_BUFFER_UPDATE_ALIGNMENT = 60,

    /// \brief Maximum texture size (in pixels) supported for 1D, 2D and 1D
    /// array and 2D array targets.
    NVN_DEVICE_INFO_MAX_TEXTURE_SIZE = 61,

    /// \brief Maximum texture size (in pixels) supported for buffer
    /// textures.
    NVN_DEVICE_INFO_MAX_BUFFER_TEXTURE_SIZE = 62,

    /// \brief Maximum texture size (in pixels) supported by 3D texture
    /// target.
    NVN_DEVICE_INFO_MAX_3D_TEXTURE_SIZE = 63,

    /// \brief Maximum texture size (in pixels) supported by cubemap
    /// texture target.
    NVN_DEVICE_INFO_MAX_CUBE_MAP_TEXTURE_SIZE = 64,

    /// \brief Maximum texture size (in pixels) supported by rectangle
    /// texture target.
    NVN_DEVICE_INFO_MAX_RECTANGLE_TEXTURE_SIZE = 65,

    /// \brief 1 if GLSL shaders using NV_geometry_shader_passthrough are
    /// supported by the NVN implementation; 0 otherwise.
    ///
    /// This feature is not supported on the Windows reference
    /// implementation for GPUs older than second-generation Maxwell GPUs.
    NVN_DEVICE_INFO_SUPPORTS_PASSTHROUGH_GEOMETRY_SHADERS = 66,

    /// \brief 1 if the viewport swizzle operation is supported by the NVN
    /// implementation; 0 otherwise.
    ///
    /// This feature is not supported on the Windows reference
    /// implementation for GPUs older than second-generation Maxwell GPUs.
    NVN_DEVICE_INFO_SUPPORTS_VIEWPORT_SWIZZLE = 67,

    /// \brief 1 if the NVN implementation supports packaged texture data
    /// in sparse tiled form; 0 otherwise.
    ///
    /// This feature is not supported on the Windows reference
    /// implementation for GPUs older than second-generation Maxwell GPUs.
    /// The memory layout used for NX packaged textures using a sparse
    /// tiled form is not compatible with the layout used on older GPUs.
    NVN_DEVICE_INFO_SUPPORTS_SPARSE_TILED_PACKAGED_TEXTURES = 68,

    /// \brief 1 if advanced blending modes (other than
    /// #NVN_BLEND_ADVANCED_MODE_NONE) are supported by the NVN
    /// implementation; 0 otherwise.
    ///
    /// Advanced blending modes are not supported on the Windows reference
    /// implementation for GPUs older than first-generation Maxwell GPUs.
    NVN_DEVICE_INFO_SUPPORTS_ADVANCED_BLEND_MODES = 69,

    /// \brief Maximum value supported by
    /// nvnWindowBuilderSetPresentInterval.
    NVN_DEVICE_INFO_MAX_PRESENT_INTERVAL = 70,

    /// \brief 1 if DrawTexture is supported by the NVN implementation; 0
    /// otherwise.
    ///
    /// This feature is not supported on the Windows reference
    /// implementation for GPUs older than first-generation Maxwell GPUs.
    NVN_DEVICE_INFO_SUPPORTS_DRAW_TEXTURE = 71,

    /// \brief 1 if the NVN implementation supports target-independent
    /// rasterization, 0 otherwise.
    ///
    /// This feature is not supported on the Windows reference
    /// implementation for GPUs older than second-generation Maxwell GPUs.
    NVN_DEVICE_INFO_SUPPORTS_TARGET_INDEPENDENT_RASTERIZATION = 72,

    /// \brief 1 if the NVN implementation supports fragment coverage to
    /// color, 0 otherwise.
    ///
    /// This feature is not supported on the Windows reference
    /// implementation for GPUs older than second-generation Maxwell GPUs.
    NVN_DEVICE_INFO_SUPPORTS_FRAGMENT_COVERAGE_TO_COLOR = 73,

    /// \brief 1 if the NVN implementation supports post-depth coverage, 0
    /// otherwise.
    ///
    /// This feature is not supported on the Windows reference
    /// implementation for GPUs older than second-generation Maxwell GPUs.
    NVN_DEVICE_INFO_SUPPORTS_POST_DEPTH_COVERAGE = 74,

    /// \brief 1 if the NVN implementation supports using texture handles
    /// for image accesses, 0 otherwise.
    ///
    /// On NX, texture and image descriptors written into the texture pool
    /// use the same format, and texture handle values can be used in
    /// nvnCommandBufferBindImage or for bindless image access.  On the
    /// Windows reference implementation, this is also true except when
    /// running on Kepler-family GPUs.  On those GPUs, texture and image
    /// descriptors do not have the same format, and their handles can not
    /// be used interchangeably.
    ///
    /// \warning When using cubemap and cubemap array textures as images,
    /// separate image handles must be used even on platforms where this
    /// property is set to 1.
    NVN_DEVICE_INFO_SUPPORTS_IMAGES_USING_TEXTURE_HANDLES = 75,

    /// \brief 1 if the NVN implementation supports programmable sample
    /// locations, 0 otherwise.
    ///
    /// This feature is not supported on the Windows reference
    /// implementation for GPUs older than second-generation Maxwell GPUs.
    NVN_DEVICE_INFO_SUPPORTS_SAMPLE_LOCATIONS = 76,

    /// \brief Total number of programmable sample locations in a
    /// NVNmultisampleState object.
    NVN_DEVICE_INFO_MAX_SAMPLE_LOCATION_TABLE_ENTRIES = 77,

    /// \brief Amount of padding required at the end of a memory pool
    /// holding shader code.
    ///
    /// Shader code may be stored in memory pools with the
    /// #NVN_MEMORY_POOL_FLAGS_SHADER_CODE_BIT flag set. However,
    /// applications must avoid storing shader code near the end of the
    /// memory pool. GPU shader cores may pre-fetch beyond the last byte of
    /// actual shader code and could fault if unpopulated virtual memory
    /// beyond the end of the pool is accessed. On NX, the last 1KB of the
    /// memory pool should not be used for shader code.
    NVN_DEVICE_INFO_SHADER_CODE_MEMORY_POOL_PADDING_SIZE = 78,

    /// \brief Maximum number of vertices in each patch primitive.
    NVN_DEVICE_INFO_MAX_PATCH_SIZE = 79,

    /// \brief Required granularity for per-queue command memory.
    NVN_DEVICE_INFO_QUEUE_COMMAND_MEMORY_GRANULARITY = 80,

    /// \brief Minimum allowed size for per-queue command memory.
    NVN_DEVICE_INFO_QUEUE_COMMAND_MEMORY_MIN_SIZE = 81,

    /// \brief Default size for per-queue command memory.
    NVN_DEVICE_INFO_QUEUE_COMMAND_MEMORY_DEFAULT_SIZE = 82,

    /// \brief Required granularity for per-queue compute memory.
    NVN_DEVICE_INFO_QUEUE_COMPUTE_MEMORY_GRANULARITY = 83,

    /// \brief Minimum allowed size for per-queue compute memory, if
    /// non-zero.
    NVN_DEVICE_INFO_QUEUE_COMPUTE_MEMORY_MIN_SIZE = 84,

    /// \brief Default size for per-queue compute memory.
    NVN_DEVICE_INFO_QUEUE_COMPUTE_MEMORY_DEFAULT_SIZE = 85,

    /// \brief Minimum flush threshold size for per-queue command memory
    /// provided to NVN.
    NVN_DEVICE_INFO_QUEUE_COMMAND_MEMORY_MIN_FLUSH_THRESHOLD = 86,

    /// \brief 1 if GLSL fragment shaders with interlocks
    /// (NV_fragment_shader_interlock) are supported by the NVN
    /// implementation; 0 otherwise.
    ///
    /// These modes are not supported on the Windows reference
    /// implementation for GPUs older than second-generation Maxwell GPUs.
    NVN_DEVICE_INFO_SUPPORTS_FRAGMENT_SHADER_INTERLOCK = 87,

    /// \brief Maximum value supported by nvnWindowBuilderSetTextures.
    NVN_DEVICE_INFO_MAX_TEXTURES_PER_WINDOW = 88,

    /// \brief Minimum value supported by nvnWindowBuilderSetTextures.
    NVN_DEVICE_INFO_MIN_TEXTURES_PER_WINDOW = 89,

    /// \brief 1 if the NVN debug layer is supported by this version of the
    /// driver, 0 otherwise.
    ///
    /// Support for NVN debug layer can be queried before creating an NVN
    /// device by passing a NULL device pointer to nvnDeviceGetInteger.
    NVN_DEVICE_INFO_SUPPORTS_DEBUG_LAYER = 90,

    /// \brief Minimum allowed size for queue control memory.
    NVN_DEVICE_INFO_QUEUE_CONTROL_MEMORY_MIN_SIZE = 91,

    /// \brief Default size for queue control memory.
    NVN_DEVICE_INFO_QUEUE_CONTROL_MEMORY_DEFAULT_SIZE = 92,

    /// \brief Required granularity for queue control memory.
    NVN_DEVICE_INFO_QUEUE_CONTROL_MEMORY_GRANULARITY = 93,

    /// \brief Number of separate texture bindings supported for each
    /// shader stage.
    ///
    /// This value identifies the number of separate texture bindings
    /// available for devices initialized with
    /// #NVN_DEVICE_FLAG_ENABLE_SEPARATE_SAMPLER_TEXTURE_SUPPORT_BIT set.
    /// For devices initialized without the bit set, separate texture
    /// bindings may not be used.
    NVN_DEVICE_INFO_SEPARATE_TEXTURE_BINDINGS_PER_STAGE = 94,

    /// \brief Number of separate sampler bindings supported for each
    /// shader stage.
    ///
    /// This value identifies the number of separate sampler bindings
    /// available for devices initialized with
    /// #NVN_DEVICE_FLAG_ENABLE_SEPARATE_SAMPLER_TEXTURE_SUPPORT_BIT set.
    /// For devices initialized without the bit set, separate sampler
    /// bindings may not be used.
    NVN_DEVICE_INFO_SEPARATE_SAMPLER_BINDINGS_PER_STAGE = 95,

    /// \brief Value of the last valid debug groups domain ID.
    NVN_DEVICE_INFO_DEBUG_GROUPS_MAX_DOMAIN_ID = 96,

    /// \brief Does the device support reduction operations for events.
    ///
    /// This value indicates whether a device supports extended reduction
    /// operations when an event is signaled. A value of 0 indicates that
    /// only the #NVN_EVENT_SIGNAL_MODE_WRITE mode is supported. A value of
    /// 1 indicates that all #NVNeventSignalMode values are supported.
    NVN_DEVICE_INFO_EVENTS_SUPPORT_REDUCTION_OPERATIONS = 97,

    NVN_ENUM_32BIT(DEVICE_INFO),
} NVNdeviceInfo;

/// \brief Identifies the type of a GPU error.
typedef enum NVNdeviceError {

    /// \brief The GPU attempted to access memory outside of a memory pool.
    NVN_DEVICE_ERROR_ACCESS_VIOLATION = 0x00000001,

    NVN_ENUM_32BIT(DEVICE_ERROR),
} NVNdeviceError;

/// \brief Identifies the type of a texture object.
typedef enum NVNtextureTarget {

    /// \brief One-dimensional texture.
    NVN_TEXTURE_TARGET_1D = 0x00000000,

    /// \brief Two-dimensional texture.
    NVN_TEXTURE_TARGET_2D = 0x00000001,

    /// \brief Three-dimensional texture.
    NVN_TEXTURE_TARGET_3D = 0x00000002,

    /// \brief Array of one-dimensional textures.
    NVN_TEXTURE_TARGET_1D_ARRAY = 0x00000003,

    /// \brief Array of two-dimensional textures.
    NVN_TEXTURE_TARGET_2D_ARRAY = 0x00000004,

    /// \brief Two-dimensional texture, with multiple samples per texel.
    NVN_TEXTURE_TARGET_2D_MULTISAMPLE = 0x00000005,

    /// \brief Array of two-dimensional textures, with multiple samples per
    /// texel.
    NVN_TEXTURE_TARGET_2D_MULTISAMPLE_ARRAY = 0x00000006,

    /// \brief Two-dimensional texture, accessed with non-normalized
    /// coordinates.
    NVN_TEXTURE_TARGET_RECTANGLE = 0x00000007,

    /// \brief Cube map texture, with six two-dimensional cube faces.
    NVN_TEXTURE_TARGET_CUBEMAP = 0x00000008,

    /// \brief Array of cube map textures, each with six two-dimensional
    /// cube faces.
    NVN_TEXTURE_TARGET_CUBEMAP_ARRAY = 0x00000009,

    /// \brief NVNbuffer texture
    NVN_TEXTURE_TARGET_BUFFER = 0x0000000A,

    NVN_ENUM_32BIT(TEXTURE_TARGET),
} NVNtextureTarget;

/// \brief Identifies a source value for one of the four output components
/// produced by the texture swizzle operation.
typedef enum NVNtextureSwizzle {

    /// \brief Output component returned as 0 or 0.0, depending on the
    /// texture format.
    NVN_TEXTURE_SWIZZLE_ZERO = 0x00000000,

    /// \brief Output component returned as 1 or 1.0, depending on the
    /// texture format.
    NVN_TEXTURE_SWIZZLE_ONE = 0x00000001,

    /// \brief Output component filled with the first (R) component of the
    /// source image.
    NVN_TEXTURE_SWIZZLE_R = 0x00000002,

    /// \brief Output component filled with the second (G) component of the
    /// source image.
    NVN_TEXTURE_SWIZZLE_G = 0x00000003,

    /// \brief Output component filled with the third (B) component of the
    /// source image.
    NVN_TEXTURE_SWIZZLE_B = 0x00000004,

    /// \brief Output component filled with the fourth (A) component of the
    /// source image.
    NVN_TEXTURE_SWIZZLE_A = 0x00000005,

    NVN_ENUM_32BIT(TEXTURE_SWIZZLE),
} NVNtextureSwizzle;

/// \brief For textures containing depth and stencil data, identifies
/// whether texture lookups will return depth or stencil values.
typedef enum NVNtextureDepthStencilMode {

    /// \brief Return depth data as floating-point values.
    NVN_TEXTURE_DEPTH_STENCIL_MODE_DEPTH = 0x00000000,

    /// \brief Return stencil data as unsigned integer values.
    NVN_TEXTURE_DEPTH_STENCIL_MODE_STENCIL = 0x00000001,

    NVN_ENUM_32BIT(TEXTURE_DEPTH_STENCIL_MODE),
} NVNtextureDepthStencilMode;

/// \brief Identifies how data elements stored in texture or buffer objects
/// are encoded.
///
/// Specifies how data elements stored in texture or buffer objects are
/// encoded and decoded, described in detail in the \ref nvn_guide_formats
/// "programming guide".  Not all formats are supported for all uses
/// (texture, vertex pulling); see the \ref nvn_guide_formats_table
/// "formats table" to determine what formats may be used in what
/// situations.
typedef enum NVNformat {

    /// \brief Placeholder enum indicating no defined format; never valid.
    NVN_FORMAT_NONE = 0x00000000,

    /// \brief One 8-bit unsigned normalized component.
    NVN_FORMAT_R8 = 0x00000001,

    /// \brief One 8-bit signed normalized component.
    NVN_FORMAT_R8SN = 0x00000002,

    /// \brief One 8-bit unsigned integer component.
    NVN_FORMAT_R8UI = 0x00000003,

    /// \brief One 8-bit signed integer component.
    NVN_FORMAT_R8I = 0x00000004,

    /// \brief One 16-bit floating-point component.
    NVN_FORMAT_R16F = 0x00000005,

    /// \brief One 16-bit unsigned normalized component.
    NVN_FORMAT_R16 = 0x00000006,

    /// \brief One 16-bit signed normalized component.
    NVN_FORMAT_R16SN = 0x00000007,

    /// \brief One 16-bit unsigned integer component.
    NVN_FORMAT_R16UI = 0x00000008,

    /// \brief One 16-bit signed integer component.
    NVN_FORMAT_R16I = 0x00000009,

    /// \brief One 32-bit floating-point component.
    NVN_FORMAT_R32F = 0x0000000A,

    /// \brief One 32-bit unsigned integer component.
    NVN_FORMAT_R32UI = 0x0000000B,

    /// \brief One 32-bit signed integer component.
    NVN_FORMAT_R32I = 0x0000000C,

    /// \brief Two 8-bit unsigned normalized components.
    NVN_FORMAT_RG8 = 0x0000000D,

    /// \brief Two 8-bit signed normalized components.
    NVN_FORMAT_RG8SN = 0x0000000E,

    /// \brief Two 8-bit unsigned integer components.
    NVN_FORMAT_RG8UI = 0x0000000F,

    /// \brief Two 8-bit signed integer components.
    NVN_FORMAT_RG8I = 0x00000010,

    /// \brief Two 16-bit floating-point components.
    NVN_FORMAT_RG16F = 0x00000011,

    /// \brief Two 16-bit unsigned normalized components.
    NVN_FORMAT_RG16 = 0x00000012,

    /// \brief Two 16-bit signed normalized components.
    NVN_FORMAT_RG16SN = 0x00000013,

    /// \brief Two 16-bit unsigned integer components.
    NVN_FORMAT_RG16UI = 0x00000014,

    /// \brief Two 16-bit signed integer components.
    NVN_FORMAT_RG16I = 0x00000015,

    /// \brief Two 32-bit floating-point components.
    NVN_FORMAT_RG32F = 0x00000016,

    /// \brief Two 32-bit unsigned integer components.
    NVN_FORMAT_RG32UI = 0x00000017,

    /// \brief Two 32-bit signed integer components.
    NVN_FORMAT_RG32I = 0x00000018,

    /// \brief Three 8-bit unsigned normalized components.
    NVN_FORMAT_RGB8 = 0x00000019,

    /// \brief Three 8-bit signed normalized components.
    NVN_FORMAT_RGB8SN = 0x0000001A,

    /// \brief Three 8-bit unsigned integer components.
    NVN_FORMAT_RGB8UI = 0x0000001B,

    /// \brief Three 8-bit signed integer components.
    NVN_FORMAT_RGB8I = 0x0000001C,

    /// \brief Three 16-bit floating-point components.
    NVN_FORMAT_RGB16F = 0x0000001D,

    /// \brief Three 16-bit unsigned normalized components.
    NVN_FORMAT_RGB16 = 0x0000001E,

    /// \brief Three 16-bit signed normalized components.
    NVN_FORMAT_RGB16SN = 0x0000001F,

    /// \brief Three 16-bit unsigned integer components.
    NVN_FORMAT_RGB16UI = 0x00000020,

    /// \brief Three 16-bit signed integer components.
    NVN_FORMAT_RGB16I = 0x00000021,

    /// \brief Three 32-bit floating-point components.
    NVN_FORMAT_RGB32F = 0x00000022,

    /// \brief Three 32-bit unsigned integer components.
    NVN_FORMAT_RGB32UI = 0x00000023,

    /// \brief Three 32-bit signed integer components.
    NVN_FORMAT_RGB32I = 0x00000024,

    /// \brief Four 8-bit unsigned normalized components.
    NVN_FORMAT_RGBA8 = 0x00000025,

    /// \brief Four 8-bit signed normalized components.
    NVN_FORMAT_RGBA8SN = 0x00000026,

    /// \brief Four 8-bit unsigned integer components.
    NVN_FORMAT_RGBA8UI = 0x00000027,

    /// \brief Four 8-bit signed integer components.
    NVN_FORMAT_RGBA8I = 0x00000028,

    /// \brief Four 16-bit floating-point components.
    NVN_FORMAT_RGBA16F = 0x00000029,

    /// \brief Four 16-bit unsigned normalized components.
    NVN_FORMAT_RGBA16 = 0x0000002A,

    /// \brief Four 16-bit signed normalized components.
    NVN_FORMAT_RGBA16SN = 0x0000002B,

    /// \brief Four 16-bit unsigned integer components.
    NVN_FORMAT_RGBA16UI = 0x0000002C,

    /// \brief Four 16-bit signed integer components.
    NVN_FORMAT_RGBA16I = 0x0000002D,

    /// \brief Four 32-bit floating-point components.
    NVN_FORMAT_RGBA32F = 0x0000002E,

    /// \brief Four 32-bit unsigned integer components.
    NVN_FORMAT_RGBA32UI = 0x0000002F,

    /// \brief Four 32-bit signed integer components.
    NVN_FORMAT_RGBA32I = 0x00000030,

    /// \brief An 8-bit unsigned integer stencil component.
    NVN_FORMAT_STENCIL8 = 0x00000031,

    /// \brief A 16-bit unsigned normalized depth component.
    NVN_FORMAT_DEPTH16 = 0x00000032,

    /// \brief A 24-bit unsigned normalized depth component in the LSBs of
    /// a 32-bit word.
    NVN_FORMAT_DEPTH24 = 0x00000033,

    /// \brief A 32-bit floating-point depth component.
    NVN_FORMAT_DEPTH32F = 0x00000034,

    /// \brief A 24-bit unsigned normalized depth component packed with an
    /// 8-bit unsigned integer stencil component.
    NVN_FORMAT_DEPTH24_STENCIL8 = 0x00000035,

    /// \brief A 32-bit floating-point depth component packed with an 8-bit
    /// unsigned integer stencil component.
    NVN_FORMAT_DEPTH32F_STENCIL8 = 0x00000036,

    /// \brief Three 8-bit unsigned normalized components, padded to 32
    /// bits per pixel and encoded in the sRGB color space.
    NVN_FORMAT_RGBX8_SRGB = 0x00000037,

    /// \brief Four 8-bit unsigned normalized components, encoded in the
    /// sRGB color space.
    NVN_FORMAT_RGBA8_SRGB = 0x00000038,

    /// \brief Four 4-bit unsigned normalized components.
    NVN_FORMAT_RGBA4 = 0x00000039,

    /// \brief Three 5-bit unsigned normalized components.
    NVN_FORMAT_RGB5 = 0x0000003A,

    /// \brief Three 5-bit unsigned normalized RGB components with a 1-bit
    /// A component.
    NVN_FORMAT_RGB5A1 = 0x0000003B,

    /// \brief Three 5-bit (R,B) and 6-bit (G) unsigned normalized
    /// components.
    NVN_FORMAT_RGB565 = 0x0000003C,

    /// \brief Three 10-bit (R,G,B) and 2-bit (A) unsigned normalized
    /// components.
    NVN_FORMAT_RGB10A2 = 0x0000003D,

    /// \brief Three 10-bit (R,G,B) and 2-bit (A) unsigned integer
    /// components.
    NVN_FORMAT_RGB10A2UI = 0x0000003E,

    /// \brief Three 10-bit (B) and 11-bit (R,G) floating-point components.
    NVN_FORMAT_R11G11B10F = 0x0000003F,

    /// \brief Three floating-point components with separate 9-bit
    /// mantissas and a shared 5-bit exponent.
    NVN_FORMAT_RGB9E5F = 0x00000040,

    /// \brief DXT1 compressed texture blocks with an RGB format.
    NVN_FORMAT_RGB_DXT1 = 0x00000041,

    /// \brief DXT1 compressed texture blocks with an RGBA format.
    NVN_FORMAT_RGBA_DXT1 = 0x00000042,

    /// \brief DXT3 compressed texture blocks.
    NVN_FORMAT_RGBA_DXT3 = 0x00000043,

    /// \brief DXT5 compressed texture blocks
    NVN_FORMAT_RGBA_DXT5 = 0x00000044,

    /// \brief DXT1 compressed texture blocks with an sRGB-encoded RGB
    /// format.
    NVN_FORMAT_RGB_DXT1_SRGB = 0x00000045,

    /// \brief DXT1 compressed texture blocks with an sRGB-encoded RGBA
    /// format.
    NVN_FORMAT_RGBA_DXT1_SRGB = 0x00000046,

    /// \brief DXT3 compressed texture blocks with an sRGB color encoding.
    NVN_FORMAT_RGBA_DXT3_SRGB = 0x00000047,

    /// \brief DXT5 compressed texture blocks with an sRGB color encoding.
    NVN_FORMAT_RGBA_DXT5_SRGB = 0x00000048,

    /// \brief RGTC compressed 1-component unsigned normalized blocks.
    NVN_FORMAT_RGTC1_UNORM = 0x00000049,

    /// \brief RGTC compressed 1-component signed normalized blocks.
    NVN_FORMAT_RGTC1_SNORM = 0x0000004A,

    /// \brief RGTC compressed 2-component unsigned normalized blocks.
    NVN_FORMAT_RGTC2_UNORM = 0x0000004B,

    /// \brief RGTC compressed 2-component signed normalized blocks.
    NVN_FORMAT_RGTC2_SNORM = 0x0000004C,

    /// \brief BPTC compressed 4-component unsigned normalized blocks.
    NVN_FORMAT_BPTC_UNORM = 0x0000004D,

    /// \brief BPTC compressed 4-component unsigned normalized blocks with
    /// an sRGB color encoding.
    NVN_FORMAT_BPTC_UNORM_SRGB = 0x0000004E,

    /// \brief BPTC compressed 3-component signed floating-point blocks.
    NVN_FORMAT_BPTC_SFLOAT = 0x0000004F,

    /// \brief BPTC compressed 3-component unsigned floating-point blocks.
    NVN_FORMAT_BPTC_UFLOAT = 0x00000050,

    /// \brief One 8-bit unsigned integer component, cast to float.
    NVN_FORMAT_R8_UI2F = 0x00000051,

    /// \brief One 8-bit signed integer component, cast to float.
    NVN_FORMAT_R8_I2F = 0x00000052,

    /// \brief One 16-bit unsigned integer component, cast to float.
    NVN_FORMAT_R16_UI2F = 0x00000053,

    /// \brief One 16-bit signed integer component, cast to float.
    NVN_FORMAT_R16_I2F = 0x00000054,

    /// \brief One 32-bit unsigned integer component, cast to float.
    NVN_FORMAT_R32_UI2F = 0x00000055,

    /// \brief One 32-bit signed integer component, cast to float.
    NVN_FORMAT_R32_I2F = 0x00000056,

    /// \brief Two 8-bit unsigned integer components, cast to float.
    NVN_FORMAT_RG8_UI2F = 0x00000057,

    /// \brief Two 8-bit signed integer components, cast to float.
    NVN_FORMAT_RG8_I2F = 0x00000058,

    /// \brief Two 16-bit unsigned integer components, cast to float.
    NVN_FORMAT_RG16_UI2F = 0x00000059,

    /// \brief Two 16-bit signed integer components, cast to float.
    NVN_FORMAT_RG16_I2F = 0x0000005A,

    /// \brief Two 32-bit unsigned integer components, cast to float.
    NVN_FORMAT_RG32_UI2F = 0x0000005B,

    /// \brief Two 32-bit signed integer components, cast to float.
    NVN_FORMAT_RG32_I2F = 0x0000005C,

    /// \brief Three 8-bit unsigned integer components, cast to float.
    NVN_FORMAT_RGB8_UI2F = 0x0000005D,

    /// \brief Three 8-bit signed integer components, cast to float.
    NVN_FORMAT_RGB8_I2F = 0x0000005E,

    /// \brief Three 16-bit unsigned integer components, cast to float.
    NVN_FORMAT_RGB16_UI2F = 0x0000005F,

    /// \brief Three 16-bit signed integer components, cast to float.
    NVN_FORMAT_RGB16_I2F = 0x00000060,

    /// \brief Three 32-bit unsigned integer components, cast to float.
    NVN_FORMAT_RGB32_UI2F = 0x00000061,

    /// \brief Three 32-bit signed integer components, cast to float.
    NVN_FORMAT_RGB32_I2F = 0x00000062,

    /// \brief Four 8-bit unsigned integer components, cast to float.
    NVN_FORMAT_RGBA8_UI2F = 0x00000063,

    /// \brief Four 8-bit signed integer components, cast to float.
    NVN_FORMAT_RGBA8_I2F = 0x00000064,

    /// \brief Four 16-bit unsigned integer components, cast to float.
    NVN_FORMAT_RGBA16_UI2F = 0x00000065,

    /// \brief Four 16-bit signed integer components, cast to float.
    NVN_FORMAT_RGBA16_I2F = 0x00000066,

    /// \brief Four 32-bit unsigned integer components, cast to float.
    NVN_FORMAT_RGBA32_UI2F = 0x00000067,

    /// \brief Four 32-bit signed integer components, cast to float.
    NVN_FORMAT_RGBA32_I2F = 0x00000068,

    /// \brief Three 10-bit (R,G,B) and 2-bit (A) signed normalized
    /// components.
    NVN_FORMAT_RGB10A2SN = 0x00000069,

    /// \brief Three 10-bit (R,G,B) and 2-bit (A) signed integer
    /// components.
    NVN_FORMAT_RGB10A2I = 0x0000006A,

    /// \brief Three 10-bit (R,G,B) and 2-bit (A) unsigned integer
    /// components, cast to floating-point.
    NVN_FORMAT_RGB10A2_UI2F = 0x0000006B,

    /// \brief Three 10-bit (R,G,B) and 2-bit (A) signed integer
    /// components, cast to floating-point.
    NVN_FORMAT_RGB10A2_I2F = 0x0000006C,

    /// \brief Three 8-bit unsigned normalized components, padded to 32
    /// bits per pixel.
    NVN_FORMAT_RGBX8 = 0x0000006D,

    /// \brief Three 8-bit signed normalized components, padded to 32 bits
    /// per pixel.
    NVN_FORMAT_RGBX8SN = 0x0000006E,

    /// \brief Three 8-bit unsigned integer components, padded to 32 bits
    /// per pixel.
    NVN_FORMAT_RGBX8UI = 0x0000006F,

    /// \brief Three 8-bit signed integer components, padded to 32 bits per
    /// pixel.
    NVN_FORMAT_RGBX8I = 0x00000070,

    /// \brief Three 16-bit floating-point components, padded to 64 bits
    /// per pixel.
    NVN_FORMAT_RGBX16F = 0x00000071,

    /// \brief Three 16-bit unsigned normalized components, padded to 64
    /// bits per pixel.
    NVN_FORMAT_RGBX16 = 0x00000072,

    /// \brief Three 16-bit signed normalized components, padded to 64 bits
    /// per pixel.
    NVN_FORMAT_RGBX16SN = 0x00000073,

    /// \brief Three 16-bit unsigned integer components, padded to 64 bits
    /// per pixel.
    NVN_FORMAT_RGBX16UI = 0x00000074,

    /// \brief Three 16-bit signed integer components, padded to 64 bits
    /// per pixel.
    NVN_FORMAT_RGBX16I = 0x00000075,

    /// \brief Three 32-bit floating-point components, padded to 128 bits
    /// per pixel.
    NVN_FORMAT_RGBX32F = 0x00000076,

    /// \brief Three 32-bit unsigned integer components, padded to 128 bits
    /// per pixel.
    NVN_FORMAT_RGBX32UI = 0x00000077,

    /// \brief Three 32-bit signed integer components, padded to 128 bits
    /// per pixel.
    NVN_FORMAT_RGBX32I = 0x00000078,

    /// \brief ASTC compressed texture blocks with an RGBA format and 4x4
    /// texel block footprint.
    NVN_FORMAT_RGBA_ASTC_4x4 = 0x00000079,

    /// \brief ASTC compressed texture blocks with an RGBA format and 5x4
    /// texel block footprint.
    NVN_FORMAT_RGBA_ASTC_5x4 = 0x0000007A,

    /// \brief ASTC compressed texture blocks with an RGBA format and 5x5
    /// texel block footprint.
    NVN_FORMAT_RGBA_ASTC_5x5 = 0x0000007B,

    /// \brief ASTC compressed texture blocks with an RGBA format and 6x5
    /// texel block footprint.
    NVN_FORMAT_RGBA_ASTC_6x5 = 0x0000007C,

    /// \brief ASTC compressed texture blocks with an RGBA format and 6x6
    /// texel block footprint.
    NVN_FORMAT_RGBA_ASTC_6x6 = 0x0000007D,

    /// \brief ASTC compressed texture blocks with an RGBA format and 8x5
    /// texel block footprint.
    NVN_FORMAT_RGBA_ASTC_8x5 = 0x0000007E,

    /// \brief ASTC compressed texture blocks with an RGBA format and 8x6
    /// texel block footprint.
    NVN_FORMAT_RGBA_ASTC_8x6 = 0x0000007F,

    /// \brief ASTC compressed texture blocks with an RGBA format and 8x8
    /// texel block footprint.
    NVN_FORMAT_RGBA_ASTC_8x8 = 0x00000080,

    /// \brief ASTC compressed texture blocks with an RGBA format and 10x5
    /// texel block footprint.
    NVN_FORMAT_RGBA_ASTC_10x5 = 0x00000081,

    /// \brief ASTC compressed texture blocks with an RGBA format and 10x6
    /// texel block footprint.
    NVN_FORMAT_RGBA_ASTC_10x6 = 0x00000082,

    /// \brief ASTC compressed texture blocks with an RGBA format and 10x8
    /// texel block footprint.
    NVN_FORMAT_RGBA_ASTC_10x8 = 0x00000083,

    /// \brief ASTC compressed texture blocks with an RGBA format and 10x10
    /// texel block footprint.
    NVN_FORMAT_RGBA_ASTC_10x10 = 0x00000084,

    /// \brief ASTC compressed texture blocks with an RGBA format and 12x10
    /// texel block footprint.
    NVN_FORMAT_RGBA_ASTC_12x10 = 0x00000085,

    /// \brief ASTC compressed texture blocks with an RGBA format and 12x12
    /// texel block footprint.
    NVN_FORMAT_RGBA_ASTC_12x12 = 0x00000086,

    /// \brief ASTC compressed texture blocks with an sRGB plus Alpha
    /// format and 4x4 texel block footprint.
    NVN_FORMAT_RGBA_ASTC_4x4_SRGB = 0x00000087,

    /// \brief ASTC compressed texture blocks with an sRGB plus Alpha
    /// format and 5x4 texel block footprint.
    NVN_FORMAT_RGBA_ASTC_5x4_SRGB = 0x00000088,

    /// \brief ASTC compressed texture blocks with an sRGB plus Alpha
    /// format and 5x5 texel block footprint.
    NVN_FORMAT_RGBA_ASTC_5x5_SRGB = 0x00000089,

    /// \brief ASTC compressed texture blocks with an sRGB plus Alpha
    /// format and 6x5 texel block footprint.
    NVN_FORMAT_RGBA_ASTC_6x5_SRGB = 0x0000008A,

    /// \brief ASTC compressed texture blocks with an sRGB plus Alpha
    /// format and 6x6 texel block footprint.
    NVN_FORMAT_RGBA_ASTC_6x6_SRGB = 0x0000008B,

    /// \brief ASTC compressed texture blocks with an sRGB plus Alpha
    /// format and 8x5 texel block footprint.
    NVN_FORMAT_RGBA_ASTC_8x5_SRGB = 0x0000008C,

    /// \brief ASTC compressed texture blocks with an sRGB plus Alpha
    /// format and 8x6 texel block footprint.
    NVN_FORMAT_RGBA_ASTC_8x6_SRGB = 0x0000008D,

    /// \brief ASTC compressed texture blocks with an sRGB plus Alpha
    /// format and 8x8 texel block footprint.
    NVN_FORMAT_RGBA_ASTC_8x8_SRGB = 0x0000008E,

    /// \brief ASTC compressed texture blocks with an sRGB plus Alpha
    /// format and 10x5 texel block footprint.
    NVN_FORMAT_RGBA_ASTC_10x5_SRGB = 0x0000008F,

    /// \brief ASTC compressed texture blocks with an sRGB plus Alpha
    /// format and 10x6 texel block footprint.
    NVN_FORMAT_RGBA_ASTC_10x6_SRGB = 0x00000090,

    /// \brief ASTC compressed texture blocks with an sRGB plus Alpha
    /// format and 10x8 texel block footprint.
    NVN_FORMAT_RGBA_ASTC_10x8_SRGB = 0x00000091,

    /// \brief ASTC compressed texture blocks with an sRGB plus Alpha
    /// format and 10x10 texel block footprint.
    NVN_FORMAT_RGBA_ASTC_10x10_SRGB = 0x00000092,

    /// \brief ASTC compressed texture blocks with an sRGB plus Alpha
    /// format and 12x10 texel block footprint.
    NVN_FORMAT_RGBA_ASTC_12x10_SRGB = 0x00000093,

    /// \brief ASTC compressed texture blocks with an sRGB plus Alpha
    /// format and 12x12 texel block footprint.
    NVN_FORMAT_RGBA_ASTC_12x12_SRGB = 0x00000094,

    /// \brief Three 5-bit (R,B) and 6-bit (G) unsigned normalized
    /// components.
    NVN_FORMAT_BGR565 = 0x00000095,

    /// \brief Three 5-bit unsigned normalized components.
    NVN_FORMAT_BGR5 = 0x00000096,

    /// \brief Three 5-bit unsigned normalized RGB components with a 1-bit
    /// A component.
    NVN_FORMAT_BGR5A1 = 0x00000097,

    /// \brief One 1-bit A component with three 5-bit unsigned normalized
    /// RGB components.
    NVN_FORMAT_A1BGR5 = 0x00000098,

    /// \brief Three 8-bit unsigned normalized components, padded to 32
    /// bits per pixel.
    NVN_FORMAT_BGRX8 = 0x00000099,

    /// \brief Four 8-bit unsigned normalized components.
    NVN_FORMAT_BGRA8 = 0x0000009A,

    /// \brief Three 8-bit unsigned normalized components, padded to 32
    /// bits per pixel and encoded in the sRGB color space.
    NVN_FORMAT_BGRX8_SRGB = 0x0000009B,

    /// \brief Four 8-bit unsigned normalized components, encoded in the
    /// sRGB color space.
    NVN_FORMAT_BGRA8_SRGB = 0x0000009C,

    NVN_ENUM_32BIT(FORMAT),
} NVNformat;

/// \brief Identifies a coefficient to be multiplied with each component of
/// the source or destination color when blending.
///
/// For the default #NVN_BLEND_EQUATION_ADD mode, blending is performed by
/// evaluating 'C = Fs * Cs + Fd * Cd' for each color component.  In these
/// equations, Cs represents the fragment shader output (source) color and
/// Cd represents the color value stored in the framebuffer. The
/// #NVNblendFunc enum specifies values used for the blend factor terms Fs
/// and Fd.  There are separate blend functions for RGB and alpha color
/// components.
typedef enum NVNblendFunc {

    /// \brief Use the constant 0.0.
    NVN_BLEND_FUNC_ZERO = 0x00000001,

    /// \brief Use the constant 1.0.
    NVN_BLEND_FUNC_ONE = 0x00000002,

    /// \brief Use by the corresponding component of the source color.
    NVN_BLEND_FUNC_SRC_COLOR = 0x00000003,

    /// \brief Use 1.0 minus the corresponding component of the source
    /// color.
    NVN_BLEND_FUNC_ONE_MINUS_SRC_COLOR = 0x00000004,

    /// \brief Use the alpha component of the source color.
    NVN_BLEND_FUNC_SRC_ALPHA = 0x00000005,

    /// \brief Use 1.0 minus the alpha component of the source color.
    NVN_BLEND_FUNC_ONE_MINUS_SRC_ALPHA = 0x00000006,

    /// \brief Use the alpha component of the destination color.
    NVN_BLEND_FUNC_DST_ALPHA = 0x00000007,

    /// \brief Use 1.0 minus the alpha component of the destination color.
    NVN_BLEND_FUNC_ONE_MINUS_DST_ALPHA = 0x00000008,

    /// \brief Use the corresponding component of the destination color.
    NVN_BLEND_FUNC_DST_COLOR = 0x00000009,

    /// \brief Use 1.0 minus the corresponding component of the destination
    /// color.
    NVN_BLEND_FUNC_ONE_MINUS_DST_COLOR = 0x0000000A,

    /// \brief Use min(As,1-Ad), where As and Ad are the alpha components
    /// of the source and destination color.
    NVN_BLEND_FUNC_SRC_ALPHA_SATURATE = 0x0000000B,

    /// \brief Use the corresponding component of the second source color.
    NVN_BLEND_FUNC_SRC1_COLOR = 0x00000010,

    /// \brief Use 1.0 minus the corresponding component of the second
    /// source color.
    NVN_BLEND_FUNC_ONE_MINUS_SRC1_COLOR = 0x00000011,

    /// \brief Use the alpha component of the second source color.
    NVN_BLEND_FUNC_SRC1_ALPHA = 0x00000012,

    /// \brief Use 1.0 minus the alpha component of the second source
    /// color.
    NVN_BLEND_FUNC_ONE_MINUS_SRC1_ALPHA = 0x00000013,

    /// \brief Use the corresponding component of the constant blend color.
    NVN_BLEND_FUNC_CONSTANT_COLOR = 0x00000061,

    /// \brief Use 1.0 minus the corresponding component of the constant
    /// blend color.
    NVN_BLEND_FUNC_ONE_MINUS_CONSTANT_COLOR = 0x00000062,

    /// \brief Use the alpha component of the constant blend color.
    NVN_BLEND_FUNC_CONSTANT_ALPHA = 0x00000063,

    /// \brief Use 1.0 minus the alpha component of the constant blend
    /// color.
    NVN_BLEND_FUNC_ONE_MINUS_CONSTANT_ALPHA = 0x00000064,

    NVN_ENUM_32BIT(BLEND_FUNC),
} NVNblendFunc;

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
typedef enum NVNblendAdvancedMode {

    /// \brief Use normal blending equations specified with
    /// nvnBlendStateSetBlendEquation and nvnBlendStateSetBlendFunc.
    NVN_BLEND_ADVANCED_MODE_NONE = 0x00000000,

    /// \brief Use the ZERO blending mode.
    NVN_BLEND_ADVANCED_MODE_ZERO = 0x00000005,

    /// \brief Use the SRC blending mode.
    NVN_BLEND_ADVANCED_MODE_SRC = 0x00000006,

    /// \brief Use the DST blending mode.
    NVN_BLEND_ADVANCED_MODE_DST = 0x00000007,

    /// \brief Use the SRC_OVER blending mode.
    NVN_BLEND_ADVANCED_MODE_SRC_OVER = 0x00000008,

    /// \brief Use the DST_OVER blending mode.
    NVN_BLEND_ADVANCED_MODE_DST_OVER = 0x00000009,

    /// \brief Use the SRC_IN blending mode.
    NVN_BLEND_ADVANCED_MODE_SRC_IN = 0x0000000A,

    /// \brief Use the DST_IN blending mode.
    NVN_BLEND_ADVANCED_MODE_DST_IN = 0x0000000B,

    /// \brief Use the SRC_OUT blending mode.
    NVN_BLEND_ADVANCED_MODE_SRC_OUT = 0x0000000C,

    /// \brief Use the DST_OUT blending mode.
    NVN_BLEND_ADVANCED_MODE_DST_OUT = 0x0000000D,

    /// \brief Use the SRC_ATOP blending mode.
    NVN_BLEND_ADVANCED_MODE_SRC_ATOP = 0x0000000E,

    /// \brief Use the DST_ATOP blending mode.
    NVN_BLEND_ADVANCED_MODE_DST_ATOP = 0x0000000F,

    /// \brief Use the XOR blending mode.
    NVN_BLEND_ADVANCED_MODE_XOR = 0x00000010,

    /// \brief Use the PLUS blending mode.
    NVN_BLEND_ADVANCED_MODE_PLUS = 0x00000011,

    /// \brief Use the PLUS_CLAMPED blending mode.
    NVN_BLEND_ADVANCED_MODE_PLUS_CLAMPED = 0x00000012,

    /// \brief Use the PLUS_CLAMPED_ALPHA blending mode.
    NVN_BLEND_ADVANCED_MODE_PLUS_CLAMPED_ALPHA = 0x00000013,

    /// \brief Use the PLUS_DARKER blending mode.
    NVN_BLEND_ADVANCED_MODE_PLUS_DARKER = 0x00000014,

    /// \brief Use the MULTIPLY blending mode.
    NVN_BLEND_ADVANCED_MODE_MULTIPLY = 0x00000015,

    /// \brief Use the SCREEN blending mode.
    NVN_BLEND_ADVANCED_MODE_SCREEN = 0x00000016,

    /// \brief Use the OVERLAY blending mode.
    NVN_BLEND_ADVANCED_MODE_OVERLAY = 0x00000017,

    /// \brief Use the DARKEN blending mode.
    NVN_BLEND_ADVANCED_MODE_DARKEN = 0x00000018,

    /// \brief Use the LIGHTEN blending mode.
    NVN_BLEND_ADVANCED_MODE_LIGHTEN = 0x00000019,

    /// \brief Use the COLORDODGE blending mode.
    NVN_BLEND_ADVANCED_MODE_COLORDODGE = 0x0000001A,

    /// \brief Use the COLORBURN blending mode.
    NVN_BLEND_ADVANCED_MODE_COLORBURN = 0x0000001B,

    /// \brief Use the HARDLIGHT blending mode.
    NVN_BLEND_ADVANCED_MODE_HARDLIGHT = 0x0000001C,

    /// \brief Use the SOFTLIGHT blending mode.
    NVN_BLEND_ADVANCED_MODE_SOFTLIGHT = 0x0000001D,

    /// \brief Use the DIFFERENCE blending mode.
    NVN_BLEND_ADVANCED_MODE_DIFFERENCE = 0x0000001E,

    /// \brief Use the MINUS blending mode.
    NVN_BLEND_ADVANCED_MODE_MINUS = 0x00000001F,

    /// \brief Use the MINUS_CLAMPED blending mode.
    NVN_BLEND_ADVANCED_MODE_MINUS_CLAMPED = 0x00000020,

    /// \brief Use the EXCLUSION blending mode.
    NVN_BLEND_ADVANCED_MODE_EXCLUSION = 0x00000021,

    /// \brief Use the CONTRAST blending mode.
    NVN_BLEND_ADVANCED_MODE_CONTRAST = 0x00000022,

    /// \brief Use the INVERT blending mode.
    NVN_BLEND_ADVANCED_MODE_INVERT = 0x00000023,

    /// \brief Use the INVERT_RGB blending mode.
    NVN_BLEND_ADVANCED_MODE_INVERT_RGB = 0x00000024,

    /// \brief Use the INVERT_OVG blending mode.
    NVN_BLEND_ADVANCED_MODE_INVERT_OVG = 0x00000025,

    /// \brief Use the LINEARDODGE blending mode.
    NVN_BLEND_ADVANCED_MODE_LINEARDODGE = 0x00000026,

    /// \brief Use the LINEARBURN blending mode.
    NVN_BLEND_ADVANCED_MODE_LINEARBURN = 0x00000027,

    /// \brief Use the VIVIDLIGHT blending mode.
    NVN_BLEND_ADVANCED_MODE_VIVIDLIGHT = 0x00000028,

    /// \brief Use the LINEARLIGHT blending mode.
    NVN_BLEND_ADVANCED_MODE_LINEARLIGHT = 0x00000029,

    /// \brief Use the PINLIGHT blending mode.
    NVN_BLEND_ADVANCED_MODE_PINLIGHT = 0x0000002A,

    /// \brief Use the HARDMIX blending mode.
    NVN_BLEND_ADVANCED_MODE_HARDMIX = 0x0000002B,

    /// \brief Use the RED blending mode.
    NVN_BLEND_ADVANCED_MODE_RED = 0x0000002C,

    /// \brief Use the GREEN blending mode.
    NVN_BLEND_ADVANCED_MODE_GREEN = 0x0000002D,

    /// \brief Use the BLUE blending mode.
    NVN_BLEND_ADVANCED_MODE_BLUE = 0x0000002E,

    /// \brief Use the HSL_HUE blending mode.
    NVN_BLEND_ADVANCED_MODE_HSL_HUE = 0x0000002F,

    /// \brief Use the HSL_SATURATION blending mode.
    NVN_BLEND_ADVANCED_MODE_HSL_SATURATION = 0x00000030,

    /// \brief Use the HSL_COLOR blending mode.
    NVN_BLEND_ADVANCED_MODE_HSL_COLOR = 0x00000031,

    /// \brief Use the HSL_LUMINOSITY blending mode.
    NVN_BLEND_ADVANCED_MODE_HSL_LUMINOSITY = 0x00000032,

    NVN_ENUM_32BIT(BLEND_ADVANCED_MODE),
} NVNblendAdvancedMode;

/// \brief Specifies an advanced mode for advanced blending modes.
///
/// Advanced blending in NVN provides capabilities similar to those
/// provided by the NV_blend_equation_advanced OpenGL extension.  The
/// overlap modes specify how coverage for partially covered sources and/or
/// destinations is treated in the blending equations.
typedef enum NVNblendAdvancedOverlap {

    /// \brief Treat the coverage of source and destination coverage as
    /// uncorrelated.
    ///
    /// For uncorrelated coverage, the portions of the pixel covered by the
    /// source and destination are considered to be distributed
    /// independently.  The area of overlap is As * Ad, where As and Ad are
    /// the source and destination alpha.
    NVN_BLEND_ADVANCED_OVERLAP_UNCORRELATED = 0x00000000,

    /// \brief Treat the coverage of source and destination coverage as
    /// disjoint.
    ///
    /// For disjoint coverage, the blending equations are configured to
    /// minimize the amount of overlap between the source and destination.
    /// The area of overlap is max(0,As+Ad-1), where As and Ad are the
    /// source and destination alpha values.
    NVN_BLEND_ADVANCED_OVERLAP_DISJOINT = 0x00000001,

    /// \brief Treat the coverage of source and destination coverage as
    /// conjoint.
    ///
    /// For conjoint coverage, the blending equations are configured to
    /// maximize the amount of overlap between the source and destination.
    /// The area of overlap is min(As,Ad), where As and Ad are the source
    /// and destination alpha values.
    NVN_BLEND_ADVANCED_OVERLAP_CONJOINT = 0x00000002,

    NVN_ENUM_32BIT(BLEND_ADVANCED_OVERLAP),
} NVNblendAdvancedOverlap;

/// \brief Specifies the equation used for blending source and destination
/// colors.
///
/// Blending is performed by evaluating a function for each component of
/// the source and destination colors (Cs and Cd), called the blend
/// equation.  Several blend equations also involve source and destination
/// blend factors (Fs and Fd), called the blend function.
typedef enum NVNblendEquation {

    /// \brief Compute C = Fs * Cs + Fd * Cd.
    NVN_BLEND_EQUATION_ADD = 0x00000001,

    /// \brief Compute C = Fs * Cs - Fd * Cd.
    NVN_BLEND_EQUATION_SUB = 0x00000002,

    /// \brief Compute C = Fd * Cd - Fs * Cs.
    NVN_BLEND_EQUATION_REVERSE_SUB = 0x00000003,

    /// \brief Compute min(Cs, Cd).
    NVN_BLEND_EQUATION_MIN = 0x00000004,

    /// \brief Compute max(Cs, Cd).
    NVN_BLEND_EQUATION_MAX = 0x00000005,

    NVN_ENUM_32BIT(BLEND_EQUATION),
} NVNblendEquation;

/// \brief Specifies a bit-wise operation performed on source and
/// destination colors.
///
/// Logical operations are performed by evaluating a function for each
/// component of the source and destination colors (Cs and Cd).  Before
/// performing logical operations, the source color is converted to the
/// format used to store destination values.  If enabled, the same
/// operation is performed on all render targets.  There is no support for
/// separate logical operations on color and alpha components.
typedef enum NVNlogicOp {

    /// \brief Compute C = 0.
    NVN_LOGIC_OP_CLEAR = 0x00000000,

    /// \brief Compute C = Cs & Cd.
    NVN_LOGIC_OP_AND = 0x00000001,

    /// \brief Compute C = Cs & (~Cd).
    NVN_LOGIC_OP_AND_REVERSE = 0x00000002,

    /// \brief Compute C = Cs.
    NVN_LOGIC_OP_COPY = 0x00000003,

    /// \brief Compute C = (~Cs) & Cd.
    NVN_LOGIC_OP_AND_INVERTED = 0x00000004,

    /// \brief Compute C = Cd.
    NVN_LOGIC_OP_NOOP = 0x00000005,

    /// \brief Compute C = Cs ^ Cd.
    NVN_LOGIC_OP_XOR = 0x00000006,

    /// \brief Compute C = Cs | Cd.
    NVN_LOGIC_OP_OR = 0x00000007,

    /// \brief Compute C = ~(Cs | Cd).
    NVN_LOGIC_OP_NOR = 0x00000008,

    /// \brief Compute C = ~(Cs ^ Cd).
    NVN_LOGIC_OP_EQUIV = 0x00000009,

    /// \brief Compute C = ~Cd.
    NVN_LOGIC_OP_INVERT = 0x0000000A,

    /// \brief Compute C = Cs | (~Cd).
    NVN_LOGIC_OP_OR_REVERSE = 0x0000000B,

    /// \brief Compute C = ~Cs.
    NVN_LOGIC_OP_COPY_INVERTED = 0x0000000C,

    /// \brief Compute C = (~Cs) | Cd.
    NVN_LOGIC_OP_OR_INVERTED = 0x0000000D,

    /// \brief Compute C = ~(Cs & Cd).
    NVN_LOGIC_OP_NAND = 0x0000000E,

    /// \brief Compute C = ~0 (set all bits to one).
    NVN_LOGIC_OP_SET = 0x0000000F,

    NVN_ENUM_32BIT(LOGIC_OP),
} NVNlogicOp;

/// \brief Specifies a function used to compare 'test' and 'reference'
/// values for alpha tests, where the 'test' value is the alpha value from
/// the fragment.
typedef enum NVNalphaFunc {

    /// \brief Comparison always fails.
    NVN_ALPHA_FUNC_NEVER = 0x00000001,

    /// \brief Comparison passes if the test value is less than the
    /// reference value.
    NVN_ALPHA_FUNC_LESS = 0x00000002,

    /// \brief Comparison passes if the test value is equal to the
    /// reference value.
    NVN_ALPHA_FUNC_EQUAL = 0x00000003,

    /// \brief Comparison passes if the test value is less than or equal to
    /// the reference value.
    NVN_ALPHA_FUNC_LEQUAL = 0x00000004,

    /// \brief Comparison passes if the test value is greater than the
    /// reference value.
    NVN_ALPHA_FUNC_GREATER = 0x00000005,

    /// \brief Comparison passes if the test value is not equal to
    /// reference value.
    NVN_ALPHA_FUNC_NOTEQUAL = 0x00000006,

    /// \brief Comparison passes if the test value is greater than or equal
    /// to the reference value.
    NVN_ALPHA_FUNC_GEQUAL = 0x00000007,

    /// \brief Comparison always passes.
    NVN_ALPHA_FUNC_ALWAYS = 0x00000008,

    NVN_ENUM_32BIT(ALPHA_FUNC),
} NVNalphaFunc;

/// \brief Specifies the type of primitive to render in draw calls.
typedef enum NVNdrawPrimitive {

    /// \brief Renders a collection of points.
    NVN_DRAW_PRIMITIVE_POINTS = 0x00000000,

    /// \brief Renders a collection of independent lines specified by pairs
    /// of vertices.
    NVN_DRAW_PRIMITIVE_LINES = 0x00000001,

    /// \brief Renders a line loop by connecting each new vertex to the
    /// previous one, and closing the loop by connecting the last vertex to
    /// the first.
    NVN_DRAW_PRIMITIVE_LINE_LOOP = 0x00000002,

    /// \brief Renders a line strip by connecting each new vertex to the
    /// previous one.
    NVN_DRAW_PRIMITIVE_LINE_STRIP = 0x00000003,

    /// \brief Renders a collection of independent triangles specified by
    /// groups of three vertices.
    NVN_DRAW_PRIMITIVE_TRIANGLES = 0x00000004,

    /// \brief Renders a triangle strip, where each new vertex forms a
    /// triangle with the two previous vertices.
    NVN_DRAW_PRIMITIVE_TRIANGLE_STRIP = 0x00000005,

    /// \brief Renders a triangle fan, where each new vertex forms a
    /// triangle with the previous vertex and the first vertex in the
    /// primitive.
    NVN_DRAW_PRIMITIVE_TRIANGLE_FAN = 0x00000006,

    /// \brief Renders a collection of independent quadrilaterals specified
    /// by groups of four vertices.
    NVN_DRAW_PRIMITIVE_QUADS = 0x00000007,

    /// \brief Renders a quadrilateral strip, where each pair of new
    /// vertices forms a quadrilateral with the previous pair of vertices.
    NVN_DRAW_PRIMITIVE_QUAD_STRIP = 0x00000008,

    /// \brief Renders a single polygon by connecting each vertex to the
    /// next, closing the loop by connecting the last vertex to the first.
    NVN_DRAW_PRIMITIVE_POLYGON = 0x00000009,

    /// \brief Renders a collection of independent line segments with
    /// adjacency information.
    ///
    /// Each group of four vertices specifies a line segment connecting the
    /// second and third vertices.  The segment is treated as though it
    /// were connected to segments connecting the first and second vertices
    /// and the third and fourth vertices.  If a geometry shader is
    /// present, it can process the line with the connecting information.
    /// If no geometry shader is present, the primitive is rendered as
    /// independent lines, where the first and fourth vertex in each group
    /// is ignored.
    NVN_DRAW_PRIMITIVE_LINES_ADJACENCY = 0x0000000A,

    /// \brief Renders a collection of connected line segments with
    /// adjacency information.
    ///
    /// Each new vertex forms a group of four vertices with the three
    /// previous vertices, and the group is treated a forming line segment
    /// connecting the second and third vertices of the group.  The segment
    /// is treated as though it were connected to segments connecting the
    /// first and second vertices and the third and fourth vertices.  If a
    /// geometry shader is present, it can process the line with the
    /// connecting information.  If no geometry shader is present, the
    /// primitive is rendered as independent lines, where the first and
    /// fourth vertex in each group are ignored.
    NVN_DRAW_PRIMITIVE_LINE_STRIP_ADJACENCY = 0x0000000B,

    /// \brief Renders a collection of independent triangles with adjacency
    /// information.
    ///
    /// Each group of six vertices specifies a triangle connecting the
    /// first, third, and fifth vertices.  The triangles is treated as
    /// though it were connected to triangles connecting the first, second,
    /// and third vertices, the third, fourth, and fifth vertices, and the
    /// fifth, sixth, and first vertices of the group.  If a geometry
    /// shader is present, it can process the triangle with the connecting
    /// information.  If no geometry shader is present, the primitive is
    /// rendered as independent triangles, where the second, fourth, and
    /// sixth vertices in each group are ignored.
    NVN_DRAW_PRIMITIVE_TRIANGLES_ADJACENCY = 0x0000000C,

    /// \brief Renders a strip of connected triangles with adjacency
    /// information.
    ///
    /// Each group of six vertices specifies a triangle connecting the
    /// first, third, and fifth vertices.  The triangles is treated as
    /// though it were connected to triangles connecting the first, second,
    /// and third vertices, the third, fourth, and fifth vertices, and the
    /// fifth, sixth, and first vertices of the group.  If a geometry
    /// shader is present, it can process the triangle with the connecting
    /// information.  If no geometry shader is present, the primitive is
    /// rendered as independent triangles, where the second, fourth, and
    /// sixth vertices in each group are ignored.
    NVN_DRAW_PRIMITIVE_TRIANGLE_STRIP_ADJACENCY = 0x0000000D,

    /// \brief Renders a collection of independent patches.
    ///
    /// Each group of N vertices is processed as a separate patch by
    /// tessellation control and evaluation shaders.  The value N is
    /// specified by nvnCommandBufferSetPatchSize.  If no tessellation
    /// evaluation or geometry shader is present, the patch will be
    /// rendered as independent points.
    NVN_DRAW_PRIMITIVE_PATCHES = 0x0000000E,

    NVN_ENUM_32BIT(DRAW_PRIMITIVE),
} NVNdrawPrimitive;

/// \brief Specifies a data type used for index values in DrawElements
/// commands.
typedef enum NVNindexType {

    /// \brief Index values are specified as (8-bit) unsigned bytes.
    NVN_INDEX_TYPE_UNSIGNED_BYTE = 0x00000000,

    /// \brief Index values are specified as 16-bit unsigned integers.
    NVN_INDEX_TYPE_UNSIGNED_SHORT = 0x00000001,

    /// \brief Index values are specified as 32-bit unsigned integers.
    NVN_INDEX_TYPE_UNSIGNED_INT = 0x00000002,

    NVN_ENUM_32BIT(INDEX_TYPE),
} NVNindexType;

/// \brief Specifies a function used to compare 'test' and 'render target'
/// values for depth tests, where the 'test' value is the depth value for
/// the fragment.
typedef enum NVNdepthFunc {

    /// \brief Comparison always fails.
    NVN_DEPTH_FUNC_NEVER = 0x00000001,

    /// \brief Comparison passes if the test value is less than the render
    /// target value.
    NVN_DEPTH_FUNC_LESS = 0x00000002,

    /// \brief Comparison passes if the test value is equal to the render
    /// target value.
    NVN_DEPTH_FUNC_EQUAL = 0x00000003,

    /// \brief Comparison passes if the test value is less than or equal to
    /// the render target value.
    NVN_DEPTH_FUNC_LEQUAL = 0x00000004,

    /// \brief Comparison passes if the test value is greater than the
    /// render target value.
    NVN_DEPTH_FUNC_GREATER = 0x00000005,

    /// \brief Comparison passes if the test value is not equal to render
    /// target value.
    NVN_DEPTH_FUNC_NOTEQUAL = 0x00000006,

    /// \brief Comparison passes if the test value is greater than or equal
    /// to the render target value.
    NVN_DEPTH_FUNC_GEQUAL = 0x00000007,

    /// \brief Comparison always passes.
    NVN_DEPTH_FUNC_ALWAYS = 0x00000008,

    NVN_ENUM_32BIT(DEPTH_FUNC),
} NVNdepthFunc;

/// \brief Specifies a magnification filter used for texture sampling.
///
/// When sampling from a texture, the magnification filter is used if the
/// GPU determines that the texture is being magnified (where each texel is
/// one pixel or larger in screen space).
typedef enum NVNmagFilter {

    /// \brief Returns the value of the nearest texel to the sample
    /// location.
    NVN_MAG_FILTER_NEAREST = 0x00000000,

    /// \brief Returns the weighted average of the nearest texels to the
    /// sample location.
    NVN_MAG_FILTER_LINEAR = 0x00000001,

    NVN_ENUM_32BIT(MAG_FILTER),
} NVNmagFilter;

/// \brief Specifies a minification filter used for texture sampling.
///
/// When sampling from a texture, the minification filter is used if the
/// GPU determines that the texture is being minified (where each texel is
/// smaller than one pixel in screen space).  If the texture being sampled
/// includes mipmaps (lower-resolution versions of the full-resolution
/// 'base' image), the minification filter may read values from the mipmap
/// images.
typedef enum NVNminFilter {

    /// \brief Returns the value of the nearest texel to the sample
    /// location in the base texture image.
    NVN_MIN_FILTER_NEAREST = 0x00000000,

    /// \brief Returns the weighted average of the nearest texels to the
    /// sample location in the base texture image.
    NVN_MIN_FILTER_LINEAR = 0x00000001,

    /// \brief Returns the value of the nearest texel to the sample
    /// location in a single mipmap image.
    NVN_MIN_FILTER_NEAREST_MIPMAP_NEAREST = 0x00000002,

    /// \brief Returns the weighted average of the nearest texels to the
    /// sample location in a single mipmap image.
    NVN_MIN_FILTER_LINEAR_MIPMAP_NEAREST = 0x00000003,

    /// \brief Determines the value of the nearest texel to the sample
    /// location in each of two mipmap images and returns a weighted
    /// average of those values.
    NVN_MIN_FILTER_NEAREST_MIPMAP_LINEAR = 0x00000004,

    /// \brief Computes a weighted average of the nearest texels to the
    /// sample location in each of two mipmap images and returns a weighted
    /// average of those computed values.
    NVN_MIN_FILTER_LINEAR_MIPMAP_LINEAR = 0x00000005,

    NVN_ENUM_32BIT(MIN_FILTER),
} NVNminFilter;

/// \brief Specifies a wrap mode when mapping a normalized texture
/// coordinate to texels in a texture map.
///
/// When sampling from a texture, shaders typically provide normalized
/// coordinate values in the range [0,1] that are mapped to texels in a
/// texture map.  Values outside the range [0,1] are legal, and are mapped
/// to texels using the wrap mode.
typedef enum NVNwrapMode {

    /// \brief NVNtexture coordinates are clamped to the range [0,1] before
    /// filtering.
    NVN_WRAP_MODE_CLAMP = 0x00000000,

    /// \brief When filtering, texels outside the [0,1] range are mapped
    /// back to values in [0,1] by using only the fractional portion of the
    /// coordinate.
    NVN_WRAP_MODE_REPEAT = 0x00000001,

    /// \brief NVNtexture coordinates are mirrored around zero and then
    /// treated like CLAMP.
    NVN_WRAP_MODE_MIRROR_CLAMP = 0x00000002,

    /// \brief NVNtexture coordinates are mirrored around zero and then
    /// treated like CLAMP_TO_EDGE.
    NVN_WRAP_MODE_MIRROR_CLAMP_TO_EDGE = 0x00000003,

    /// \brief NVNtexture coordinates are mirrored around zero and then
    /// treated like CLAMP_TO_BORDER.
    NVN_WRAP_MODE_MIRROR_CLAMP_TO_BORDER = 0x00000004,

    /// \brief When filtering, texels outside the [0,1] range are treated
    /// as sampling the border color.
    NVN_WRAP_MODE_CLAMP_TO_BORDER = 0x00000005,

    /// \brief NVNtexture coordinates are mirrored around zero and then
    /// treated like REPEAT.
    NVN_WRAP_MODE_MIRRORED_REPEAT = 0x00000006,

    /// \brief When filtering, texels outside the [0,1] range are treated
    /// as sampling the texel on the nearest edge or corner of the texture.
    NVN_WRAP_MODE_CLAMP_TO_EDGE = 0x00000007,

    NVN_ENUM_32BIT(WRAP_MODE),
} NVNwrapMode;

/// \brief Specifies whether a texture is sampled.
///
/// For textures containing depth values, a texture can be sampled by
/// returning filtered depth values directly.  Such textures can also be
/// treated as a 'shadow map' by comparing each depth value with a
/// reference 'R' coordinate to produce a result of 0.0 or 1.0.  When depth
/// comparisons are enabled, a final texture value is produced by filtering
/// the 0.0/1.0 comparison results.  When comparisons are enabled, one of
/// the comparison functions in #NVNcompareFunc are used for the
/// comparison.
///
/// \note When comparisons are enabled, the reference 'R' coordinate is
/// converted to the format of the texture prior to the comparison.
/// \warning If comparisons are enabled for a texture with no depth
/// component, the results of texture filtering are undefined.
typedef enum NVNcompareMode {

    /// \brief Returns filtered depth values without performing any
    /// comparison.
    NVN_COMPARE_MODE_NONE = 0x00000000,

    /// \brief Compares each depth value sampled with a reference value and
    /// returns filtered results.
    NVN_COMPARE_MODE_COMPARE_R_TO_TEXTURE = 0x00000001,

    NVN_ENUM_32BIT(COMPARE_MODE),
} NVNcompareMode;

/// \brief Specifies a function used to compare 'texture' and 'reference'
/// values.
typedef enum NVNcompareFunc {

    /// \brief Comparison always fails.
    NVN_COMPARE_FUNC_NEVER = 0x00000001,

    /// \brief Comparison passes if the reference value is less than the
    /// texture value.
    NVN_COMPARE_FUNC_LESS = 0x00000002,

    /// \brief Comparison passes if the reference value is equal to the
    /// texture value.
    NVN_COMPARE_FUNC_EQUAL = 0x00000003,

    /// \brief Comparison passes if the reference value is less than or
    /// equal to the texture value.
    NVN_COMPARE_FUNC_LEQUAL = 0x00000004,

    /// \brief Comparison passes if the reference value is greater than the
    /// texture value.
    NVN_COMPARE_FUNC_GREATER = 0x00000005,

    /// \brief Comparison passes if the reference value is not equal to
    /// texture value.
    NVN_COMPARE_FUNC_NOTEQUAL = 0x00000006,

    /// \brief Comparison passes if the reference value is greater than or
    /// equal to the texture value.
    NVN_COMPARE_FUNC_GEQUAL = 0x00000007,

    /// \brief Comparison always passes.
    NVN_COMPARE_FUNC_ALWAYS = 0x00000008,

    NVN_ENUM_32BIT(COMPARE_FUNC),
} NVNcompareFunc;

/// \brief Specifies a reduction filter used for texture sampling.
typedef enum NVNsamplerReduction {

    /// \brief Computes a weighted average of samples accessed for texture
    /// sampling.
    NVN_SAMPLER_REDUCTION_AVERAGE = 0x00000000,

    /// \brief Returns the per-component minimum value across samples
    /// accessed for texture sampling.
    NVN_SAMPLER_REDUCTION_MIN = 0x00000001,

    /// \brief Returns the per-component maximum value across samples
    /// accessed for texture sampling.
    NVN_SAMPLER_REDUCTION_MAX = 0x00000002,

    NVN_ENUM_32BIT(SAMPLER_REDUCTION),
} NVNsamplerReduction;

/// \brief Specifies the set of primitives (front- or back-facing) that a
/// command affects.
///
/// Polygon primitives are considered front- or back-facing based on their
/// orientation in screen space.  Certain operations (culling, stencil
/// test) can treat front- and back-facing primitives differently.  This
/// enumerant is used to indicate which type(s) of primitives an operation
/// affects.
typedef enum NVNface {

    /// \brief Specifies that an operation applies to neither front- nor
    /// back-facing primitives.
    NVN_FACE_NONE = 0x00000000,

    /// \brief Specifies that an operation applies to front-facing
    /// primitives only.
    NVN_FACE_FRONT = 0x00000001,

    /// \brief Specifies that an operation applies to back-facing
    /// primitives only.
    NVN_FACE_BACK = 0x00000002,

    /// \brief Specifies that an operation applies to both front- and
    /// back-facing primitives.
    NVN_FACE_FRONT_AND_BACK = 0x00000003,

    NVN_ENUM_32BIT(FACE),
} NVNface;

/// \brief Specifies a function used to compare 'test' and 'render target'
/// values for stencil tests, where the 'test' value is the fragment
/// stencil value generated by the given facing and stencil reference and
/// mask values.
typedef enum NVNstencilFunc {

    /// \brief Comparison always fails.
    NVN_STENCIL_FUNC_NEVER = 0x00000001,

    /// \brief Comparison passes if the test value is less than the render
    /// target value.
    NVN_STENCIL_FUNC_LESS = 0x00000002,

    /// \brief Comparison passes if the test value is equal to the render
    /// target value.
    NVN_STENCIL_FUNC_EQUAL = 0x00000003,

    /// \brief Comparison passes if the test value is less than or equal to
    /// the render target value.
    NVN_STENCIL_FUNC_LEQUAL = 0x00000004,

    /// \brief Comparison passes if the test value is greater than the
    /// render target value.
    NVN_STENCIL_FUNC_GREATER = 0x00000005,

    /// \brief Comparison passes if the test value is not equal to render
    /// target value.
    NVN_STENCIL_FUNC_NOTEQUAL = 0x00000006,

    /// \brief Comparison passes if the test value is greater than or equal
    /// to the render target value.
    NVN_STENCIL_FUNC_GEQUAL = 0x00000007,

    /// \brief Comparison always passes.
    NVN_STENCIL_FUNC_ALWAYS = 0x00000008,

    NVN_ENUM_32BIT(STENCIL_FUNC),
} NVNstencilFunc;

/// \brief Specifies an operation performed on stencil values in the
/// stencil test.
///
/// When a stencil test is performed, the value stored in the stencil
/// buffer can be updated based on the results of the depth and stencil
/// tests.
typedef enum NVNstencilOp {

    /// \brief Do not modify the stencil value.
    NVN_STENCIL_OP_KEEP = 0x00000001,

    /// \brief Set the stencil value to zero.
    NVN_STENCIL_OP_ZERO = 0x00000002,

    /// \brief Replace the stencil value with the reference value.
    NVN_STENCIL_OP_REPLACE = 0x00000003,

    /// \brief Increment the stencil value, clamping the result to the
    /// maximum value.
    NVN_STENCIL_OP_INCR = 0x00000004,

    /// \brief Decrement the stencil value, clamping the result to zero.
    NVN_STENCIL_OP_DECR = 0x00000005,

    /// \brief Invert all bits in the stencil value.
    NVN_STENCIL_OP_INVERT = 0x00000006,

    /// \brief Increment the stencil value, where values larger than the
    /// maximum value wrap to zero.
    NVN_STENCIL_OP_INCR_WRAP = 0x00000007,

    /// \brief Decrement the stencil value, where values less than zero
    /// wrap to the maximum value.
    NVN_STENCIL_OP_DECR_WRAP = 0x00000008,

    NVN_ENUM_32BIT(STENCIL_OP),
} NVNstencilOp;

/// \brief Specifies the orientation of front-facing polygon primitives.
///
/// When polygon primitives are rasterized, the GPU computes a signed area,
/// determining if the vertices are in clockwise or counter-clockwise
/// order.  This enumerant identifies which orientation is treated as
/// front-facing (pointed toward the viewer).
typedef enum NVNfrontFace {

    /// \brief Clockwise primitives are considered front-facing.
    NVN_FRONT_FACE_CW = 0x00000000,

    /// \brief Counter-clockwise primitives are considered front-facing.
    NVN_FRONT_FACE_CCW = 0x00000001,

    NVN_ENUM_32BIT(FRONT_FACE),
} NVNfrontFace;

/// \brief Specifies how polygon primitives should be rasterized.
typedef enum NVNpolygonMode {

    /// \brief Polygon primitives should be rasterized as points.
    NVN_POLYGON_MODE_POINT = 0x00000000,

    /// \brief Polygon primitives should be rasterized as outlines.
    NVN_POLYGON_MODE_LINE = 0x00000001,

    /// \brief Polygon primitives should be rasterized as filled polygons.
    NVN_POLYGON_MODE_FILL = 0x00000002,

    NVN_ENUM_32BIT(POLYGON_MODE),
} NVNpolygonMode;

/// \brief Specifies whether the depth values of rasterized polygon
/// primitives should be offset.
///
/// When rasterizing polygon primitives, the GPU can offset depth values
/// for the primitive to primitives with similar depth or to ensure that a
/// wireframe outline of a filled primitive is always on top of the filled
/// primitive.  Offset can be enabled separately for polygons drawn in
/// point, line, or filled modes.
typedef enum NVNpolygonOffsetEnable {

    /// \brief Polygon primitives rendered in any mode should not be
    /// offset.
    NVN_POLYGON_OFFSET_ENABLE_NONE = 0x00000000,

    /// \brief Polygon primitives rendered in POINT mode should be offset.
    NVN_POLYGON_OFFSET_ENABLE_POINT_BIT = 0x00000001,

    /// \brief Polygon primitives rendered in LINE mode should be offset.
    NVN_POLYGON_OFFSET_ENABLE_LINE_BIT = 0x00000002,

    /// \brief Polygon primitives rendered in FILL mode should be offset.
    NVN_POLYGON_OFFSET_ENABLE_FILL_BIT = 0x00000004,

    NVN_ENUM_32BIT(POLYGON_OFFSET_ENABLE),
} NVNpolygonOffsetEnable;

/// \brief Specifies the condition required for a fence to signal its sync
/// object.
typedef enum NVNsyncCondition {

    /// \brief The sync object should be signaled when all previous
    /// commands have fully completed on the GPU.
    NVN_SYNC_CONDITION_ALL_GPU_COMMANDS_COMPLETE = 0x00000000,

    /// \brief The sync object should be signaled when the vertices of
    /// previous commands have been fully processed by the GPU.
    NVN_SYNC_CONDITION_GRAPHICS_WORLD_SPACE_COMPLETE = 0x00000001,

    NVN_ENUM_32BIT(SYNC_CONDITION),
} NVNsyncCondition;

/// \brief Specifies the result of waiting on a sync object.
typedef enum NVNsyncWaitResult {

    /// \brief The sync object was already signaled when the wait was
    /// requested.
    NVN_SYNC_WAIT_RESULT_ALREADY_SIGNALED = 0x00000000,

    /// \brief The sync object not signaled when the wait was requested,
    /// but was signaled prior to the end of the wait period.
    NVN_SYNC_WAIT_RESULT_CONDITION_SATISFIED = 0x00000001,

    /// \brief The condition was not satisfied before the wait period
    /// expired.
    NVN_SYNC_WAIT_RESULT_TIMEOUT_EXPIRED = 0x00000002,

    /// \brief An error occured while waiting on the sync object.
    ///
    /// Waiting on a sync object that has not been signaled with a fence
    /// command is an error and will produce a result of FAILED.
    NVN_SYNC_WAIT_RESULT_FAILED = 0x00000003,

    NVN_ENUM_32BIT(SYNC_WAIT_RESULT),
} NVNsyncWaitResult;

/// \brief Specifies the result of acquiring a texture from a window.
typedef enum NVNqueueAcquireTextureResult {

    /// \brief The texture was acquired successfully.
    NVN_QUEUE_ACQUIRE_TEXTURE_RESULT_SUCCESS = 0x00000000,

    /// \brief Native window system error. This error is not recoverable.
    NVN_QUEUE_ACQUIRE_TEXTURE_RESULT_NATIVE_ERROR = 0x00000001,

    NVN_ENUM_32BIT(QUEUE_ACQUIRE_TEXTURE_RESULT),
} NVNqueueAcquireTextureResult;

/// \brief Specifies the result of acquiring a texture from a window.
typedef enum NVNwindowAcquireTextureResult {

    /// \brief The texture was acquired successfully.
    NVN_WINDOW_ACQUIRE_TEXTURE_RESULT_SUCCESS = 0x00000000,

    /// \brief Native window system error. This error is not recoverable.
    NVN_WINDOW_ACQUIRE_TEXTURE_RESULT_NATIVE_ERROR = 0x00000001,

    NVN_ENUM_32BIT(WINDOW_ACQUIRE_TEXTURE_RESULT),
} NVNwindowAcquireTextureResult;

/// \brief Specifies a programmable shader stage.
typedef enum NVNshaderStage {

    /// \brief Specifies the vertex shader stage.
    NVN_SHADER_STAGE_VERTEX = 0x00000000,

    /// \brief Specifies the fragment shader stage.
    NVN_SHADER_STAGE_FRAGMENT = 0x00000001,

    /// \brief Specifies the geometry shader stage.
    NVN_SHADER_STAGE_GEOMETRY = 0x00000002,

    /// \brief Specifies the tessellation control shader stage.
    NVN_SHADER_STAGE_TESS_CONTROL = 0x00000003,

    /// \brief Specifies the tessellation evaluation shader stage.
    NVN_SHADER_STAGE_TESS_EVALUATION = 0x00000004,

    /// \brief Specifies the compute shader stage.
    NVN_SHADER_STAGE_COMPUTE = 0x00000005,

    NVN_ENUM_32BIT(SHADER_STAGE),
} NVNshaderStage;

/// \brief Used in TiledCacheDecideAction to suggest whether the tiled
/// cache should be flushed or enabled/disabled.
typedef enum NVNtiledCacheAction {

    /// \brief Tiled caching should be enabled so subsequent work can be
    /// binned.
    NVN_TILED_CACHE_ACTION_ENABLE = 1,

    /// \brief Tiled caching should be disabled so subsequent work is not
    /// binned.
    ///
    /// If tiled caching is enabled, this action will first flush the
    /// binner.
    NVN_TILED_CACHE_ACTION_DISABLE = 2,

    /// \brief Tiled cache binner is flushed with tiling and subsequent
    /// work is binned in a new render pass.
    NVN_TILED_CACHE_ACTION_FLUSH = 3,

    /// \brief Tiled cache binner is flushed as untiled and subsequent work
    /// is binned in a new render pass.
    NVN_TILED_CACHE_ACTION_FLUSH_NO_TILING = 4,

    /// \brief Enables subsequent binning of render target state changes.
    ///
    /// If tiled caching is enabled, this action will first flush the
    /// binner.
    NVN_TILED_CACHE_ACTION_ENABLE_RENDER_TARGET_BINNING = 5,

    /// \brief Disables subsequent binning of render target state changes.
    ///
    /// If tiled caching is enabled, this action will first flush the
    /// binner.
    NVN_TILED_CACHE_ACTION_DISABLE_RENDER_TARGET_BINNING = 6,

    NVN_ENUM_32BIT(TILED_CACHE_ACTION),
} NVNtiledCacheAction;

/// \brief Specifies a set of programmable shader stages.
typedef enum NVNshaderStageBits {

    /// \brief Specifies the vertex shader stage.
    NVN_SHADER_STAGE_VERTEX_BIT = 0x00000001,

    /// \brief Specifies the fragment shader stage.
    NVN_SHADER_STAGE_FRAGMENT_BIT = 0x00000002,

    /// \brief Specifies the geometry shader stage.
    NVN_SHADER_STAGE_GEOMETRY_BIT = 0x00000004,

    /// \brief Specifies the tessellation control shader stage.
    NVN_SHADER_STAGE_TESS_CONTROL_BIT = 0x00000008,

    /// \brief Specifies the tessellation evaluation shader stage.
    NVN_SHADER_STAGE_TESS_EVALUATION_BIT = 0x00000010,

    /// \brief Specifies the compute shader stage.
    NVN_SHADER_STAGE_COMPUTE_BIT = 0x00000020,

    /// \brief Specifies all shader stages used to process graphics
    /// primitives.
    ///
    /// Includes all stages except for the compute shader stage, which is
    /// used to process compute shader dispatches.
    NVN_SHADER_STAGE_ALL_GRAPHICS_BITS = 0x0000001F,

    NVN_ENUM_32BIT(SHADER_STAGE_BITS),
} NVNshaderStageBits;

/// \brief Specifies additional operations performed when a FenceSync
/// command is processed.
typedef enum NVNsyncFlagBits {

    /// \brief Ensures that all GPU memory transactions performed for
    /// commands prior to the fence will be visible to the CPU when the
    /// sync object is signaled.
    NVN_SYNC_FLAG_FLUSH_FOR_CPU_BIT = 0x00000001,

    NVN_ENUM_32BIT(SYNC_FLAG_BITS),
} NVNsyncFlagBits;

/// \brief Specifies additional flags when creating a device.
typedef enum NVNdeviceFlagBits {

    /// \brief Enable (TRUE) or disable (FALSE) the debug layer for the
    /// device.
    ///
    /// If the debug layer is enabled on NX, the application must call
    /// nv::SetGraphicsDevtoolsAllocator prior to nvnDeviceInitialize to
    /// provide an allocator that can be used by the debug layer.
    NVN_DEVICE_FLAG_DEBUG_ENABLE_BIT = 0x00000001,

    /// \brief If non-zero, the debug layer will skip API calls where
    /// errors are detected.
    ///
    /// By default, the debug layer will continue execution after detecting
    /// and reporting errors and should behave the same as when the debug
    /// layer is disabled.  When this flag is set, the debug layer will
    /// return control to the caller when errors are detected without
    /// executing the command.  This feature is present for testing
    /// purposes and is not expected to be used by regular applications.
    NVN_DEVICE_FLAG_DEBUG_SKIP_CALLS_ON_ERROR_BIT = 0x00000002,

    /// \brief If non-zero, the debug layer will perform draw-time
    /// validation checks.
    ///
    /// Enable extra draw-time validation from the debug layer. The debug
    /// layer must be enabled for this flag to take effect.  Draw-time
    /// validation will catch common mistakes that would otherwise lead to
    /// hardware errors or hangs, but the additional state tracking
    /// required may further reduce performance.  When issuing draw
    /// commands, the debug layer will verify that previous commands have
    /// put the GPU in a valid state for drawing.
    NVN_DEVICE_FLAG_DEBUG_DRAW_VALIDATION_BIT = 0x00000004,

    /// \brief Enable deferred finalization of textures, buffers and
    /// sampler objects.
    ///
    /// If non-zero, finalization of textures, buffers and sampler objects
    /// will be not be done immediately, but rather deferred to a later
    /// call to nvnDeviceApplyDeferredFinalizes.
    NVN_DEVICE_FLAG_DEFERRED_FINALIZE_BIT = 0x00000008,

    /// \brief If non-zero, the debug layer will perform full draw-time
    /// validation checks.
    ///
    /// Enable comprehensive draw-time validation from the debug layer. The
    /// both debug layer and draw-time validation must be enabled for this
    /// flag to take effect.  This will enable the most checks in debug
    /// layer, but also has the most impact on performance.
    NVN_DEVICE_FLAG_DEBUG_DRAW_VALIDATION_HEAVY_BIT = 0x00000010,

    /// \brief Enable (TRUE) or disable (FALSE) level 0 debug layer for the
    /// device.
    ///
    /// Enables minimal debug layer features. Object pointer validation is
    /// not performed.  Only performs simple parameter checks, and invalid
    /// API usage, cases where the driver would return FALSE.
    NVN_DEVICE_FLAG_DEBUG_ENABLE_LEVEL_0_BIT = 0x00000020,

    /// \brief Enable (TRUE) or disable (FALSE) level 1 debug layer for the
    /// device.
    ///
    /// Enables object validation and better handwritten checks, on top of
    /// debug layer level 0.
    NVN_DEVICE_FLAG_DEBUG_ENABLE_LEVEL_1_BIT = 0x00000040,

    /// \brief Enable (TRUE) or disable (FALSE) level 2 debug layer for the
    /// device.
    ///
    /// Enables memory pool object tracking on top of previous debug layer
    /// levels.  This allows for the debug layer to notify when textures,
    /// buffers and samplers are not finalized when the underlying memory
    /// pool is finalized.  This flag is synonymous with DEBUG_ENABLE for
    /// backwards compatibility.
    NVN_DEVICE_FLAG_DEBUG_ENABLE_LEVEL_2_BIT = 0x00000001,

    /// \brief Enable (TRUE) or disable (FALSE) level 3 debug layer for the
    /// device.
    ///
    /// Enables draw time validation on top of previous debug layer levels.
    ///  This allows for the debug layer to perform additional checks for
    /// invalid queue states on every draw call or compute dispatch, at
    /// queue submit time.  This flag is synonymous with
    /// DEBUG_DRAW_VALIDATION for backwards compatibility.
    NVN_DEVICE_FLAG_DEBUG_ENABLE_LEVEL_3_BIT = 0x00000004,

    /// \brief Enable (TRUE) or disable (FALSE) level 4 debug layer for the
    /// device.
    ///
    /// Enables full draw time validation on top of previous debug layer
    /// levels.  This allows for the debug layer to perform heavy draw-time
    /// validation checks such as  reporting inflight command buffer
    /// command memory being reused.  This flag is synonymous with
    /// DEBUG_DRAW_VALIDATION_HEAVY for backwards compatibility.
    NVN_DEVICE_FLAG_DEBUG_ENABLE_LEVEL_4_BIT = 0x00000010,

    /// \brief Enable (TRUE) or disable (FALSE) deferred deletion of
    /// firmware memory.
    ///
    /// By default, NX firmware memory used to store GPU page table entries
    /// will be freed when memory pools are finalized. If this bit is set,
    /// the firmware memory will be retained for future memory pool
    /// mappings and will not be freed until the device is finalized. This
    /// setting has no effect on the Windows reference implementation.
    ///
    /// \note This flag will be deprecated in the future.
    NVN_DEVICE_FLAG_DEFERRED_FIRMWARE_MEMORY_RECLAIM_BIT = 0x00000080,

    /// \brief Enable (TRUE) or disable (FALSE) separate sampler/texture
    /// support.
    ///
    /// Separate sampler and texture support allows applications to execute
    /// shaders using separate sets of binding points for samplers (with no
    /// texture information) and textures (with no sampler information).
    /// When using separate texture and sampler bindings, independent
    /// texture-only and sampler-only handles are combined in shader code
    /// when performing a texture lookup.
    ///
    /// Supporting separate texture and sampler bindings requires
    /// additional per-queue memory even if the underlying shaders do not
    /// use separate samplers and textures.  It is recommended to only
    /// enable this flag if you plan on using programs compiled from
    /// shaders containing separate sampler and textures.
    ///
    /// If this flag is not set, applications may not use separate texture
    /// and sampler bindings (nvnCommandBufferBindSeparateTexture,
    /// nvnCommandBufferBindSeparateSampler).
    NVN_DEVICE_FLAG_ENABLE_SEPARATE_SAMPLER_TEXTURE_SUPPORT_BIT = 0x00000100,

    NVN_ENUM_32BIT(DEVICE_FLAG_BITS),
} NVNdeviceFlagBits;

/// \brief Specifies the set of color buffer channels enabled for writing.
typedef enum NVNclearColorMask {

    /// \brief Enables writes to the first (R) component of the color
    /// buffer.
    NVN_CLEAR_COLOR_MASK_R_BIT = 0x00000001,

    /// \brief Enables writes to the second (G) component of the color
    /// buffer.
    NVN_CLEAR_COLOR_MASK_G_BIT = 0x00000002,

    /// \brief Enables writes to the third (B) component of the color
    /// buffer.
    NVN_CLEAR_COLOR_MASK_B_BIT = 0x00000004,

    /// \brief Enables writes to the fourth (A) component of the color
    /// buffer.
    NVN_CLEAR_COLOR_MASK_A_BIT = 0x00000008,

    /// \brief Enables writes to all components of the color buffer.
    NVN_CLEAR_COLOR_MASK_RGBA = 0x0000000F,

    NVN_ENUM_32BIT(CLEAR_COLOR_MASK),
} NVNclearColorMask;

/// \brief Specifies the type of operation being performed when a debug
/// callback is made.
typedef enum NVNdebugCallbackSource {

    /// \brief A debug callback is being issued while processing an API
    /// command.
    NVN_DEBUG_CALLBACK_SOURCE_API = 0x00000000,

    /// \brief A debug callback is being issued while processing tools
    /// command.
    NVN_DEBUG_CALLBACK_SOURCE_TOOLS = 0x00000001,

    NVN_ENUM_32BIT(DEBUG_CALLBACK_SOURCE),
} NVNdebugCallbackSource;

/// \brief Specifies the type of event triggering a debug callback.
typedef enum NVNdebugCallbackType {

    /// \brief A debug callback is being issued because of an error in API
    /// usage.
    NVN_DEBUG_CALLBACK_TYPE_API_ERROR = 0x00000000,

    /// \brief A debug callback is being issued because of any warning in
    /// API usage.
    NVN_DEBUG_CALLBACK_TYPE_API_WARNING = 0x00000001,

    /// \brief A debug callback is being issued because of an error in
    /// tools usage.
    NVN_DEBUG_CALLBACK_TYPE_TOOLS_ERROR = 0x00000002,

    /// \brief A debug callback is being issued because of any warning in
    /// tools usage.
    NVN_DEBUG_CALLBACK_TYPE_TOOLS_WARNING = 0x00000003,

    NVN_ENUM_32BIT(DEBUG_CALLBACK_TYPE),
} NVNdebugCallbackType;

/// \brief Specifies the severity of the event triggering a debug callback.
typedef enum NVNdebugCallbackSeverity {

    /// \brief A debug callback is issued for errors in API usage or
    /// conditions that may lead to GPU exceptions.
    NVN_DEBUG_CALLBACK_SEVERITY_HIGH = 0x00000000,

    /// \brief A debug callback is issued to notify about potentially
    /// significant performance or undefined behavior problems.
    NVN_DEBUG_CALLBACK_SEVERITY_MEDIUM = 0x00000001,

    /// \brief A debug callback is issued to notify about minor performance
    /// or undefined behavior problems.
    NVN_DEBUG_CALLBACK_SEVERITY_LOW = 0x00000002,

    /// \brief A debug callback is issued to report a condition that is
    /// neither illegal.
    NVN_DEBUG_CALLBACK_SEVERITY_NOTIFICATION = 0x00000003,

    NVN_ENUM_32BIT(DEBUG_CALLBACK_SEVERITY),
} NVNdebugCallbackSeverity;

/// \brief Specifies properties of memory pools created from a memory pool
/// builder.
typedef enum NVNmemoryPoolFlags {

    /// \brief Indicates that the memory pool will not be accessed by the
    /// CPU.
    ///
    /// When CPU_NO_ACCESS is set, the pool memory may not be accessed by
    /// the CPU.
    ///
    /// Applications must set exactly one of CPU_NO_ACCESS, CPU_UNCACHED,
    /// and CPU_CACHED when initializing a memory pool.
    ///
    /// \note On NX, initializing a pool with CPU_NO_ACCESS does not change
    /// existing CPU memory mappings and thus does not prevent applications
    /// from accessing the pool memory using the pointer provided to
    /// nvnMemoryPoolBuilderSetStorage.
    NVN_MEMORY_POOL_FLAGS_CPU_NO_ACCESS_BIT = 0x00000001,

    /// \brief Indicates that the memory pool will be accessed by the CPU
    /// without caching.
    ///
    /// When CPU_UNCACHED is set, writes to pool memory with the CPU will
    /// be visible to subsequent commands submitted to the GPU.  CPU reads
    /// from pool memory will reflect the results of all GPU commands that
    /// have completed execution at the time of the read.  CPU_UNCACHED
    /// memory will not be cached in the CPU's internal caches, which will
    /// result in significantly reduced performance for reads and
    /// random-access writes.  Sequential writes to CPU_UNCACHED memory
    /// will have good performance.
    ///
    /// Applications must set exactly one of CPU_NO_ACCESS, CPU_UNCACHED,
    /// and CPU_CACHED when initializing a memory pool.
    NVN_MEMORY_POOL_FLAGS_CPU_UNCACHED_BIT = 0x00000002,

    /// \brief Indicates that the memory pool will be accessed by the CPU
    /// with caching.
    ///
    /// When CPU_CACHED is set, writes to pool memory with the NX CPU will
    /// be stored in a write-back CPU cache and may not be visible to
    /// subsequent commands submitted to the GPU.
    /// nvnMemoryPoolFlushMappedRange or nvnBufferFlushMappedRange must be
    /// called to ensure such CPU writes are visible to the GPU.  CPU reads
    /// from pool memory may return values from the CPU cache that do not
    /// reflect writes to main memory performed by previously completed GPU
    /// commands.  nvnMemoryPoolInvalidateMappedRange or
    /// nvnBufferInvalidateMappedRange must be called to ensure such GPU
    /// writes are visible to the CPU.  On the Windows reference
    /// implementation, CPU_CACHED memory pools use separate copies of pool
    /// memory for CPU and GPU accesses.  FlushMappedRange and
    /// InvalidateMappedRange copy memory between the two copies.
    ///
    /// Applications must set exactly one of CPU_NO_ACCESS, CPU_UNCACHED,
    /// and CPU_CACHED when initializing a memory pool.
    NVN_MEMORY_POOL_FLAGS_CPU_CACHED_BIT = 0x00000004,

    /// \brief Indicates that the memory pool will not be accessed by the
    /// GPU.
    ///
    /// When GPU_NO_ACCESS is set, pool memory will not be accessed by the
    /// GPU.
    ///
    /// Applications must set exactly one of GPU_NO_ACCESS, GPU_UNCACHED,
    /// and GPU_CACHED when initializing a memory pool.
    NVN_MEMORY_POOL_FLAGS_GPU_NO_ACCESS_BIT = 0x00000008,

    /// \brief Indicates that the memory pool will be accessed by the GPU
    /// without caching.
    ///
    /// When GPU_UNCACHED is set, pool memory will not be cached in the
    /// GPU's internal caches, which will result in significantly reduced
    /// performance for GPU operations that repeatedly access the pool
    /// memory.  However, if pool memory is not fetched more than once by
    /// the GPU, GPU_UNCACHED may improve performance by not consuming GPU
    /// cache lines for resources in the pool.  Command memory used by
    /// NVNcommandBuffer objects may benefit from using GPU_UNCACHED.
    ///
    /// Applications must set exactly one of GPU_NO_ACCESS, GPU_UNCACHED,
    /// and GPU_CACHED when initializing a memory pool.
    NVN_MEMORY_POOL_FLAGS_GPU_UNCACHED_BIT = 0x00000010,

    /// \brief Indicates that the memory pool will be accessed by the GPU
    /// with caching.
    ///
    /// When GPU_CACHED is set, pool memory will be cached in the GPU's
    /// internal caches, which is the preferred behavior for most use
    /// cases.  CPU writes to GPU_CACHED memory pools will be visible to
    /// any commands submitted to the GPU after the write.  GPU writes to
    /// GPU_CACHED memory pools are guaranteed to be visible to the CPU
    /// only after waiting on a NVNsync object signalled by calling
    /// nvnQueueFenceSync with #NVN_SYNC_FLAG_FLUSH_FOR_CPU_BIT set.
    ///
    /// Applications must set exactly one of GPU_NO_ACCESS, GPU_UNCACHED,
    /// and GPU_CACHED when initializing a memory pool.
    NVN_MEMORY_POOL_FLAGS_GPU_CACHED_BIT = 0x00000020,

    /// \brief Indicates that the memory pool may be used to store shader
    /// code for execution.
    ///
    /// When SHADER_CODE is set, pool memory may be used to store
    /// executable shader code passed to nvnProgramSetShaders using GPU
    /// addresses in ShaderData::data.  When this flag is set, the memory
    /// pool is mapped so that its contents can be used for shader
    /// execution.  If the flag is not set, GPU addresses in the pool may
    /// not be used to point at shaders.
    ///
    /// \note In the current NVN implementation, shaders may not be run
    /// from physical or virtual memory pools.  Memory pools with the
    /// SHADER_CODE flag set may not set either the PHYSICAL or VIRTUAL
    /// flag.
    ///
    /// \note Applications must avoid loading shaders into the last 1KB of
    /// a pool with SHADER_CODE set.  GPU shader cores may pre-fetch from
    /// instruction memory beyond the last byte of actual shader code,
    /// which could fault if the GPU virtual address space beyond the end
    /// of the pool is unpopulated.
    NVN_MEMORY_POOL_FLAGS_SHADER_CODE_BIT = 0x00000040,

    /// \brief Indicates that the memory pool supports compressible
    /// textures.
    ///
    /// When COMPRESSIBLE is set, this pool can be used to initialize
    /// compressible textures.  For compressible textures, the NVN driver
    /// will set up special hardware resources and memory mappings allowing
    /// the contents of the texture to be transparently compressed by the
    /// GPU to save memory bandwidth.  This form of compression can
    /// significantly increase performance when using textures as render
    /// targets.  The COMPRESSIBLE bit is ignored for virtual memory pools,
    /// but should be set on physical memory pools providing storage for
    /// compressible textures.
    ///
    /// \warning It is an error to initialize compressible textures using
    /// non-virtual memory pools without the COMPRESSIBLE bit set.
    ///
    /// \warning When reusing memory from a compressible pool for
    /// compressible textures, applications must clear compressible
    /// textures before reading or writing with the GPU.  Failing to do so
    /// could result in undefined behavior including GPU errors.
    NVN_MEMORY_POOL_FLAGS_COMPRESSIBLE_BIT = 0x00000080,

    /// \brief Indicates that the memory pool may only be used as physical
    /// storage for memory mappings into virtual memory pools.
    ///
    /// When PHYSICAL is set, the memory pool is used only to provide
    /// storage that can be mapped into virtual memory pools.  Physical
    /// pools can not be used to create NVNtexture or NVNbuffer resources.
    /// Physical pools are not directly accessible by the GPU and must be
    /// initialized with GPU_NO_ACCESS set. When mapping memory into
    /// virtual pools, the pools providing memory must have been initalized
    /// with PHYSICAL set.
    ///
    /// \note The size of all physical memory pools must be a multiple of
    /// the GPU page size (#NVN_DEVICE_INFO_MEMORY_POOL_PAGE_SIZE).
    ///
    /// \note On the Windows reference implementation, physical pools may
    /// not be initialized with the CPU_UNCACHED flag set.
    ///
    /// \note When a compressible texture is created in a virtual pool, the
    /// physical memory pool(s) providing storage for the texture must be
    /// initialized with the COMPRESSIBLE flag set.
    NVN_MEMORY_POOL_FLAGS_PHYSICAL_BIT = 0x00000100,

    /// \brief Indicates that the memory pool may be used for sparse
    /// operations.
    ///
    /// When VIRTUAL is set, the memory pool is used only to provide
    /// GPU-accessible virtual memory that can be used to create buffer and
    /// texture resources.  Virtual memory pools have no physical memory
    /// storage, and all pages of the virtual memory pool are initially
    /// unpopulated.  When reading unpopulated memory in the GPU, accesses
    /// will return zero (NX and second-generation Maxwell GPUs) or
    /// undefined values.   After a virtual memory pool is initialized, it
    /// can be populated with pages from physical pools using
    /// nvnMemoryPoolMapVirtual.  Virtual pools are not directly accessible
    /// by the CPU and must be initialized with CPU_NO_ACCESS set.
    ///
    /// \note The size of all virtual memory pools must be a multiple of
    /// the GPU page size (#NVN_DEVICE_INFO_MEMORY_POOL_PAGE_SIZE).
    NVN_MEMORY_POOL_FLAGS_VIRTUAL_BIT = 0x000000200,

    NVN_ENUM_32BIT(MEMORY_POOL_FLAGS),
} NVNmemoryPoolFlags;

/// \brief Specifies the type of counter value reported in a counter query.
typedef enum NVNcounterType {

    /// \brief Doesn't report a counter; zero is written in the counter
    /// portion of the report structure.
    ///
    /// The timestamp for this report will be taken at the bottom of the
    /// GPU pipeline and will not be produced until previous primitives
    /// have been processed by the GPU.  Note that this timestamp does not
    /// wait on the completion of all memory stores resulting from the
    /// processing of previous primitives.
    NVN_COUNTER_TYPE_TIMESTAMP = 0x00000000,

    /// \brief Reports the number of samples passing the depth and stencil
    /// test in the framebuffer.
    NVN_COUNTER_TYPE_SAMPLES_PASSED = 0x00000001,

    /// \brief Reports the number of vertices processed in input primitives
    /// specified by Draw commands.
    NVN_COUNTER_TYPE_INPUT_VERTICES = 0x00000002,

    /// \brief Reports the number of individual primitives processed in
    /// input primitives specified by Draw commands.
    ///
    /// For independent point, line, and triangle primitives, each point,
    /// line, or triangle is counted separately.  For strip, loop, and fan
    /// primitives, each individual line segment or triangle is counted
    /// separately.  Quadrilateral and polygon primitives will be
    /// decomposed into triangles, and each triangle will be counted
    /// separately.  For adjacency primitives, each 'main' line segment or
    /// triangle will be counted, but 'adjacency' line segments or
    /// triangles will not be counted.
    NVN_COUNTER_TYPE_INPUT_PRIMITIVES = 0x00000003,

    /// \brief Reports the number of vertex shader invocations executed by
    /// the GPU.
    ///
    /// Even though each input vertex is logically processed by the vertex
    /// shader, this count may not match INPUT_VERTICES.  For example, if
    /// indices are duplicated, the underlying vertex might be processed
    /// only once. Additionally, long strip primitives may be split into
    /// batches, where shared vertices are processed multiple times.
    NVN_COUNTER_TYPE_VERTEX_SHADER_INVOCATIONS = 0x00000004,

    /// \brief Reports the number of tessellation control shader
    /// invocations executed by the GPU.
    NVN_COUNTER_TYPE_TESS_CONTROL_SHADER_INVOCATIONS = 0x00000005,

    /// \brief Reports the number of tessellation evaluation shader
    /// invocations executed by the GPU.
    ///
    /// When tessellating a patch primitive, the fixed-function tessellator
    /// may split a patch into multiple sub-patches, where vertices on the
    /// edges of sub-patches may be processed (and counted) multiple times.
    ///  Additionally, vertices in tessellated patches may be processed
    /// only once by the tessellation evaluation shader but used in
    /// multiple output primitives.
    NVN_COUNTER_TYPE_TESS_EVALUATION_SHADER_INVOCATIONS = 0x00000006,

    /// \brief Reports the number of geometry shader invocations executed
    /// by the GPU.
    NVN_COUNTER_TYPE_GEOMETRY_SHADER_INVOCATIONS = 0x00000007,

    /// \brief Reports the number of fragment shader invocations executed
    /// by the GPU.
    ///
    /// If no fragment shader is bound, no fragment shader invocations will
    /// be counted. If per-fragment tests such as the depth or stencil test
    /// are enabled, fragments may sometimes be discarded before invoking
    /// (and counting) the fragment shader.  If the framebuffer has
    /// multiple samples, the fragment shader may be executed multiple
    /// times per sample. If the GPU can determine that the results of the
    /// fragment shader will have no effect or if it can determine the
    /// results without executing the fragment shader, it may skip fragment
    /// shader invocations without counting them. For example, if the
    /// fragment shader produces a single output color, but the framebuffer
    /// has no color buffer bound or has color writes disabled, any color
    /// produced by the shader would be discarded.
    NVN_COUNTER_TYPE_FRAGMENT_SHADER_INVOCATIONS = 0x00000008,

    /// \brief Reports the number of primitives generated by the
    /// fixed-function tessellator.
    NVN_COUNTER_TYPE_TESS_EVALUATION_SHADER_PRIMITIVES = 0x00000009,

    /// \brief Reports the number of primitives produced by geometry shader
    /// invocations.
    NVN_COUNTER_TYPE_GEOMETRY_SHADER_PRIMITIVES = 0x0000000A,

    /// \brief Reports the number of primitives processed by the primitive
    /// pipeline and sent to the clipper.
    NVN_COUNTER_TYPE_CLIPPER_INPUT_PRIMITIVES = 0x0000000B,

    /// \brief Reports the number of primitives surviving the primitive
    /// clipping stage.
    NVN_COUNTER_TYPE_CLIPPER_OUTPUT_PRIMITIVES = 0x0000000C,

    /// \brief Reports the number of primitives produced for clipping and
    /// rasterization.
    ///
    /// Captures the number of point, line, or triangle primitives produced
    /// for the clipping and rasterization stages to process.  If
    /// rasterizer discard is enabled, primitives will be counted prior to
    /// being discarded.
    NVN_COUNTER_TYPE_PRIMITIVES_GENERATED = 0x0000000D,

    /// \brief Reports the number of primitives captured during transform
    /// feedback.
    ///
    /// Captures the number of point, line, or triangle primitives that
    /// have been captured in transform feedback.  If the buffers provided
    /// for transform feedback are too small to capture all primitives,
    /// this counter stops updating once overflow has occurred.
    NVN_COUNTER_TYPE_TRANSFORM_FEEDBACK_PRIMITIVES_WRITTEN = 0x0000000E,

    /// \brief Report zcull counters.
    ///
    /// Reports four 32-bit words with ZCull counters with no GPU
    /// timestamp:
    ///
    /// \li Word 0: the number of 8x8 tiles processed by ZCull (whether
    /// culled or not)
    /// \li Word 1: the number of 4x2 pixel blocks culled due to failing
    /// the depth test by being behind previous primitives.
    /// \li Word 2: the number of 8x8 pixel blocks culled because they were
    /// in front of previous primitives.
    /// \li Word 3: the number of 4x4 pixel blocks that were culled due to
    /// failing the stencil test.
    NVN_COUNTER_TYPE_ZCULL_STATS = 0x0000000F,

    /// \brief Doesn't report a counter; zero is written in the counter
    /// portion of the report structure.
    ///
    /// The timestamp for this report will be taken at the top of the GPU
    /// pipeline and does not wait for the completion of previous
    /// primitives.
    NVN_COUNTER_TYPE_TIMESTAMP_TOP = 0x00000010,

    NVN_ENUM_32BIT(COUNTER_TYPE),
} NVNcounterType;

/// \brief Specifies the event type in a command buffer memory callback.
typedef enum NVNcommandBufferMemoryEvent {

    /// \brief The command buffer has run out of command memory.
    ///
    /// Applications should respond to this event by providing new memory
    /// by calling nvnCommandBufferAddCommandMemory.  Failing to do so will
    /// result in program termination.
    NVN_COMMAND_BUFFER_MEMORY_EVENT_OUT_OF_COMMAND_MEMORY = 0x00000000,

    /// \brief The command buffer has run out of control memory.
    ///
    /// Applications should respond to this event by providing new memory
    /// by calling nvnCommandBufferAddControlMemory.  Failing to do so will
    /// result in program termination.
    NVN_COMMAND_BUFFER_MEMORY_EVENT_OUT_OF_CONTROL_MEMORY = 0x00000001,

    NVN_ENUM_32BIT(COMMAND_BUFFER_MEMORY_EVENT),
} NVNcommandBufferMemoryEvent;

/// \brief Specifies how query results will affect conditional rendering.
typedef enum NVNconditionalRenderMode {

    /// \brief Rendering will be enabled if and only if the compared values
    /// are equal.
    NVN_CONDITIONAL_RENDER_MODE_RENDER_IF_EQUAL = 0,

    /// \brief Rendering will be enabled if and only if the compared values
    /// are not equal.
    NVN_CONDITIONAL_RENDER_MODE_RENDER_IF_NOT_EQUAL = 1,

    NVN_ENUM_32BIT(CONDITIONAL_RENDER_MODE),
} NVNconditionalRenderMode;

/// \brief Specifies the location of the pixel with window coordinates (0,
/// 0).
typedef enum NVNwindowOriginMode {

    /// \brief The origin is located at the lower-left corner of the
    /// window, following default OpenGL conventions.
    NVN_WINDOW_ORIGIN_MODE_LOWER_LEFT = 0,

    /// \brief The origin is located at the upper-left corner of the
    /// window, following Direct3D conventions.
    NVN_WINDOW_ORIGIN_MODE_UPPER_LEFT = 1,

    NVN_ENUM_32BIT(WINDOW_ORIGIN_MODE),
} NVNwindowOriginMode;

/// \brief Specifies the near plane of the clip volume.
typedef enum NVNdepthMode {

    /// \brief The near clip plane is located at -W (i.e., -1 when depth is
    /// normalized as z/w), following default OpenGL conventions.
    NVN_DEPTH_MODE_NEAR_IS_MINUS_W = 0,

    /// \brief The near clip plane is located at zero, following Direct3D
    /// conventions.
    NVN_DEPTH_MODE_NEAR_IS_ZERO = 1,

    NVN_ENUM_32BIT(DEPTH_MODE),
} NVNdepthMode;

/// \brief Identifies special properties of texture allocations.
///
/// NVNtexture flags are set in NVNtextureBuilder objects and are used when
/// setting up new NVNtexture objects using nvnTextureInitialize.
typedef enum NVNtextureFlags {

    /// \brief This texture can be displayed on-screen.
    ///
    /// \note Linear textures cannot be displayed. Setting the DISPLAY flag
    /// together with LINEAR or LINEAR_RENDER_TARGET is not supported.
    NVN_TEXTURE_FLAGS_DISPLAY_BIT = 0x00000001,

    /// \brief This texture can be used to hold images produced by the GPU
    /// video decode engine.  Textures with the VIDEO_DECODE bit set may
    /// have different memory storage or alignment requirements than
    /// textures without the bit set.
    NVN_TEXTURE_FLAGS_VIDEO_DECODE_BIT = 0x00000002,

    /// \brief This texture can be used for image loads and stores.
    ///
    /// On most devices, this flag has no effect.  However, on the Windows
    /// reference implementation, it affects the reported alignment
    /// requirements of buffer textures on GPUs older than
    /// second-generation Maxwell GPUs.
    NVN_TEXTURE_FLAGS_IMAGE_BIT = 0x00000004,

    /// \brief The contents of this texture can be compressed by the GPU.
    ///
    /// For textures with this bit set, the NVN driver will set up special
    /// hardware resources and memory mappings allowing the contents of the
    /// texture to be automatically compressed by the GPU to save memory
    /// bandwidth.  This form of compression can significantly increase
    /// performance when using textures as render targets.
    ///
    /// \note Setting this bit may increase the required alignment and
    /// total memory size of the texture, due to special hardware memory
    /// mappings.
    ///
    /// \note Textures with depth/stencil formats must set the compressible
    /// bit.
    ///
    /// \note This form of texture compression has nothing to do with
    /// textures using compressed block formats, such as
    /// #NVN_FORMAT_RGB_DXT1.  Applications should not set this bit for
    /// textures with compressed block formats.
    ///
    /// \warning When reusing memory from a compressible pool for
    /// compressible textures, applications must clear compressible
    /// textures before reading or writing with the GPU.  Failing to do so
    /// could result in undefined behavior, including GPU errors or hangs.
    NVN_TEXTURE_FLAGS_COMPRESSIBLE_BIT = 0x00000008,

    /// \brief This texture is stored in linear format.
    ///
    /// For textures with this bit or
    /// #NVN_TEXTURE_FLAGS_LINEAR_RENDER_TARGET_BIT set, images are stored
    /// as a two-dimensional array of texels.
    /// Each row of a linear texture is stored as an array of texels, and
    /// the difference in bytes between the addresses of adjacent row is
    /// called the _stride_. The stride is programmed by the application,
    /// and is typically computed by multiplying the width of the texture
    /// and the number of bytes for each texel, and then padding out to
    /// match hardware requirements.
    ///
    /// For linear textures, the offset in a memory pool and stride must
    /// both be multiples of
    /// #NVN_DEVICE_INFO_LINEAR_TEXTURE_STRIDE_ALIGNMENT.  If the texture
    /// is also used as a render target, additional requirements apply and
    /// the flag DeviceInfo::LINEAR_RENDER_TARGET should also be set.
    ///
    /// Linear textures only support two-dimensional images, with targets
    /// of #NVN_TEXTURE_TARGET_2D or #NVN_TEXTURE_TARGET_RECTANGLE.  Linear
    /// textures also do not support multiple mipmap levels.
    NVN_TEXTURE_FLAGS_LINEAR_BIT = 0x00000010,

    /// \brief This texture is arranged in memory to store as much of the
    /// texture as possible as collections of fixed-size texture tiles
    /// aligned on page boundaries.
    ///
    /// When using textures stored in virtual memory pools, applications
    /// can populate only some pages of the memory pool and leave the
    /// remainder unpopulated.  Textures that are partially populated in
    /// this way are referred to as _sparse_.  Some mipmap levels of the
    /// texture may be decomposed into a collection of fixed-size tiles,
    /// where each tile is aligned to a single page in the virtual memory
    /// pool.  The remaining set of mipmap levels that can not be
    /// decomposed into tiles is referred to as the mipmap _tail_.
    ///
    /// When set, this bit controls the memory layout of the texture to
    /// maximize the number of mipmap levels that can be arranged into
    /// page-aligned tiles.  On NX and second-generation Maxwell GPUs, the
    /// tail will begin at the first mipmap level smaller than the fixed
    /// tile size.  On older GPUs, the tail begins at the first mipmap
    /// level that is not a multiple of the fixed tile size.
    NVN_TEXTURE_FLAGS_SPARSE_BIT = 0x00000020,

    /// \brief This texture is stored in linear format and will be used as
    /// a render target.
    ///
    /// For textures with this bit or #NVN_TEXTURE_FLAGS_LINEAR_BIT set,
    /// images are stored as a two-dimensional array of texels.
    /// Each row of a linear texture is stored as an array of texels, and
    /// the difference in bytes between the addresses of adjacent row is
    /// called the _stride_. The stride is programmed by the application,
    /// and is typically computed by multiplying the width of the texture
    /// and the number of bytes for each texel, and then padding out to
    /// match hardware requirements.
    ///
    /// For linear texture render targets, the offset in a memory pool and
    /// stride must both be multiples of
    /// #NVN_DEVICE_INFO_LINEAR_RENDER_TARGET_STRIDE_ALIGNMENT.
    ///
    /// Linear textures only support two-dimensional images, with targets
    /// of #NVN_TEXTURE_TARGET_2D or #NVN_TEXTURE_TARGET_RECTANGLE.  Linear
    /// textures also do not support multiple mipmap levels.  Linear
    /// textures may not be used as depth/stencil render targets.  They
    /// also may not be bound as a render target at the same time as any
    /// non-linear color or depth/stencil texture.
    NVN_TEXTURE_FLAGS_LINEAR_RENDER_TARGET_BIT = 0x00000040,

    /// \brief Enable adaptive ZCull for a given depth texture target.
    ///
    /// When adaptive ZCull is enabled, hardware automatically tunes the
    /// assignment of on-chip ZCull resources to portions of the depth
    /// texture based on rendering results from previous frames. Adaptive
    /// ZCull can potentially improve ZCull precision and improve cull
    /// rate.
    ///
    /// When applications switch between multiple depth textures with any
    /// using adaptive ZCull, nvnCommandBufferSaveZCullData and
    /// nvnCommandBufferRestoreZCullData should be used to preserve
    /// information on previous rendering operations for tuning ZCull
    /// resource assignment.
    ///
    /// Use of adaptive ZCull is described in more detail in the \ref
    /// nvn_guide_adaptive_zcull "programming guide".
    NVN_TEXTURE_FLAGS_ADAPTIVE_ZCULL_BIT = 0x00000080,

    /// \brief Favor fast clear over depth compression for DEPTH16 depth
    /// textures.
    ///
    /// By default, framebuffer compression for DEPTH16 formats is
    /// optimized to minimize bandwidth usage when rendering geometric
    /// primitives.  If this flag is set, an alternate compression
    /// algorithm is used that accelerates depth buffer clears, but may
    /// reduce performance of geometric rendering relative to the default
    /// algorithm.
    ///
    /// \note This flag has no effect on the Windows reference
    /// implementation.
    NVN_TEXTURE_FLAGS_DEPTH16_PREFER_FAST_CLEAR_BIT = 0x00000100,

    /// \brief Use minimal storage layout for texture.
    ///
    /// Prefer smallest tile size for storage layout. Choosing a smaller
    /// tile size reduces cache friendliness but makes texture storage size
    /// smaller due to less padding used.  This flag may not be used with
    /// #NVN_TEXTURE_FLAGS_LINEAR_BIT, #NVN_TEXTURE_FLAGS_SPARSE_BIT,
    /// #NVN_TEXTURE_FLAGS_VIDEO_DECODE_BIT or buffer textures.
    NVN_TEXTURE_FLAGS_MINIMAL_LAYOUT_BIT = 0x00000200,

    /// \brief Enables support for stencil culling.
    ///
    /// By default stencil culling is disabled and all ZCull memory is used
    /// to store depth information. When enabling stencil culling one bit
    /// per pixel/sample is used to store stencil information in the ZCull
    /// memory. Note that enabling stencil culling results in a reduced
    /// precision of the depth culling.
    /// This flag is only supported for depth formats that have stencil
    /// bits.
    NVN_TEXTURE_FLAGS_ZCULL_SUPPORT_STENCIL_BIT = 0x00000400,

    /// \brief Disable Both compression for 4xAA color textures that have a
    /// 32 or 64 color bit depth.
    ///
    /// Color textures that use 4xAA multi-sampling format and that have a
    /// color depth of 32 or 64 bit can be compressed by either using
    /// arithmetic compression, by using a color reduction or by using a
    /// combination of both. By default NVN will use the combination of
    /// both, which is called Both compression. If this flag is set only
    /// the color reduction or the arithmetic compression can be used to
    /// compress the color data.
    ///
    /// \note This flag only has an effect on 4xAA color texture that have
    /// a color depth of 32 or 64 bit.
    /// \note This flag has no effect on the Windows reference
    /// implementation.
    NVN_TEXTURE_FLAGS_DISABLE_BOTH_COMPRESSION_BIT = 0x00001000,

    NVN_ENUM_32BIT(TEXTURE_FLAGS),
} NVNtextureFlags;

/// \brief Controls the ordering of commands and invalidation of caches for
/// nvnCommandBufferBarrier.
///
/// Rendering and compute dispatch commands sent to NVN queues are executed
/// on the GPU using a hardware pipeline, with no guarantee that work for
/// one draw call or dispatch completes before work on the next draw call
/// or dispatch starts.  Additionally, some units of the GPU maintain
/// caches that are not automatically invalidated when cached data are
/// overwritten by the CPU or other units of the GPU.  This bitfield type
/// is used by nvnCommandBufferBarrier to order the processing of commands
/// sent before and after the barrier and to invalidate GPU caches before
/// processing commands sent after the barrier.
typedef enum NVNbarrierBits {

    /// \brief Ensure that primitives and compute dispatches from after the
    /// barrier don't start until all such work from before the barrier has
    /// completed.
    ///
    /// \warning ORDER_PRIMITIVES does not ensure that all work before the
    /// barrier completes before fetching per-draw data for indirect draws
    /// and compute dispatches after the barrier.  If per-draw data needed
    /// by indirect draws or dispatches are produced by commands executed
    /// by the GPU, a barrier with the ORDER_INDIRECT_DATA bit set is
    /// required.
    NVN_BARRIER_ORDER_PRIMITIVES_BIT = 0x00000001,

    /// \brief Ensure that fragments from after the barrier don't start
    /// until all fragments from before the barrier have finished.
    ///
    /// ORDER_FRAGMENTS ensures that all fragments spawned by commands sent
    /// before the barrier complete execution on the GPU before processing
    /// any fragments spawned by commands sent after the barrier.  This is
    /// useful for cases like render-to-texture, where one rendering pass
    /// writes to a texture as a render target, and the next pass reads the
    /// values written in a fragment shader.
    ///
    /// \note ORDER_FRAGMENTS _only_ orders fragment shader execution and
    /// subsequent per-fragment operations (e.g., depth and stencil tests,
    /// blending, and stores to the framebuffer).  Vertex processing for
    /// draw calls sent after the barrier may begin before fragments from
    /// before the barrier complete; to ensure proper order for dependent
    /// vertex processing work, ORDER_PRIMITIVES should be used.
    ///
    /// \warning There are a few exceptions to the ordering guarantee for
    /// ORDER_FRAGMENTS. The GPU may begin processing depth/stencil clears
    /// or depth/stencil testing (including buffer updates) for fragments
    /// after the barrier before completing all processing of fragments
    /// before the barrier. Additionally, on the Windows reference
    /// implementation for Pascal and newer GPUs, the GPU may begin
    /// processing color clears for fragments after the barrier before
    /// completing all processing of fragments before the barrier. When
    /// fragments from after the barrier perform such updates to a texture
    /// read by fragments from before the barrier, a heavier barrier like
    /// ORDER_PRIMITIVES is necessary to ensure the "new" fragments don't
    /// overwrite values read by "old" ones.
    NVN_BARRIER_ORDER_FRAGMENTS_BIT = 0x00000002,

    /// \brief Ensure that fragments from after the barrier don't start
    /// until all fragments from before the barrier in the same
    /// screen-space tile have finished.
    ///
    /// ORDER_FRAGMENTS_TILED behaves like ORDER_FRAGMENTS, except that it
    /// only guarantees the ordering of fragments within screen-space
    /// tiles.  If two fragments belong to different screen-space tiles,
    /// work for one fragment from after the barrier may begin before work
    /// for the other fragment from before the barrier completes.  This
    /// mode is useful for render-to-texture operations where the second
    /// texturing pass accesses texels written during the first pass with
    /// the same (x,y) coordinates.
    NVN_BARRIER_ORDER_FRAGMENTS_TILED_BIT = 0x00000004,

    /// \brief Ensure that all work before the barrier completes before
    /// fetching per-draw data for indirect draws and compute dispatches
    /// after the barrier.
    ///
    /// When executing indirect draws or compute dispatches, the GPU might
    /// fetch per-draw data before previous GPU work has completed, even in
    /// cases where the application waits on a sync object using
    /// nvnQueueWaitSync.  A barrier with this bit set is required to
    /// ensure that the GPU doesn't prematurely fetch the indirect data.
    /// Note that this bit is not required when the indirect data are
    /// written by the CPU.
    ///
    /// \warning ORDER_INDIRECT_DATA requires more expensive
    /// synchronization than ORDER_PRIMITIVES and should be used only when
    /// required.
    NVN_BARRIER_ORDER_INDIRECT_DATA_BIT = 0x00000008,

    /// \brief Invalidate caches used to hold texture data fetched via
    /// sampler or image variables.
    ///
    /// When texture memory is written to by either the CPU or by GPU
    /// commands other than copy commands, applications must invalidate the
    /// texture cache.  If the texture memory is written by non-copy GPU
    /// commands, applications must also ensure that the GPU commands
    /// producing the new data have completed, either by setting an ORDER
    /// bit in the barrier or by waiting on a NVNsync object.
    NVN_BARRIER_INVALIDATE_TEXTURE_BIT = 0x00000010,

    /// \brief Invalidate caches used to hold shader code and data from
    /// uniform or shader storage blocks.
    ///
    /// When shader code, uniform blocks, or shader storage blocks are
    /// written to by either the CPU or the GPU commands other than copy
    /// commands, applications must invalidate the shader caches.  If the
    /// memory is written by non-copy GPU commands, applications must also
    /// ensure that the GPU commands producing the new data have completed,
    /// either by setting an ORDER bit in the barrier or by waiting on a
    /// NVNsync object.
    ///
    /// In particular, before executing new shader code used by
    /// nvnProgramSetShaders, the shader caches must be invalidated to
    /// ensure it they are not holding old instructions using the same
    /// memory as the new shader code.
    NVN_BARRIER_INVALIDATE_SHADER_BIT = 0x00000020,

    /// \brief Invalidate caches used to hold texture and sampler
    /// descriptors.
    ///
    /// When new texture, image, or sampler descriptors are registered in
    /// the texture or sampler pools using nvnTexturePoolRegisterTexture,
    /// nvnTexturePoolRegisterImage, nvnSamplerPoolRegisterSampler, or
    /// nvnSamplerPoolRegisterSamplerBuilder, the texture descriptor cache
    /// must be invalidated to avoid the possibility of fetching stale
    /// texture or sampler pool entries.
    ///
    /// note The texture and sampler descriptor caches are automatically
    /// invalidated when command buffers are flushed, as well as when
    /// switching to a new texture or sampler pool.
    NVN_BARRIER_INVALIDATE_TEXTURE_DESCRIPTOR_BIT = 0x00000040,

    /// \brief Invalidate GPU ZCull storage after direct depth texture
    /// updates.
    ///
    /// For depth textures, the GPU maintains internal _ZCull_ storage that
    /// is used to accelerate depth and stencil testing.  The ZCull
    /// hardware tracks updates to the depth buffer when processing clears
    /// and depth buffer updates from the depth test, and maintains a
    /// low-resolution representation of the depth buffer.  ZCull data can
    /// often be used to quickly determine if fragments will pass or fail
    /// the depth test.  When a depth texture is updated via copy commands
    /// such as nvnCommandBufferCopyTextureToTexture, the ZCull storage
    /// will be out of date and require invalidation.  When a depth buffer
    /// is updated only by clears or normal rendering, barriers with
    /// INVALIDATE_ZCULL will unnecessarily reduce performance.
    ///
    /// \note The ZCull storage is automatically invalidated when switching
    /// to a new depth render target using
    /// nvnCommandBufferSetRenderTargets.
    NVN_BARRIER_INVALIDATE_ZCULL_BIT = 0x00000080,

    /// \brief Invalidates the L2 cache.
    ///
    /// Invalidating the L2 cache may be necessary when writing different
    /// RAW formats to the same physical memory location.For more details,
    /// see the Memory Pool Aliasing section of the NVN Programming Guide.
    ///
    /// \warning in addition to invalidating the cache, this flag performs
    /// a wait-for-idle. This may negatively impact performance.
    NVN_BARRIER_INVALIDATE_L2_CACHE_BIT = 0x00000100,

    NVN_ENUM_32BIT(BARRIER_BITS),
} NVNbarrierBits;

/// \brief Flags controlling the operation of copy commands.
typedef enum NVNcopyFlags {

    /// \brief No special operations should be performed on the copy.
    NVN_COPY_FLAGS_NONE = 0x00000000,

    /// \brief Enable linear filtering for texture-to-texture copies.
    ///
    /// Linear filtering is only supported for
    /// nvnCommandBufferCopyTextureToTexture.  This flag should not be set
    /// for any other copy command.
    NVN_COPY_FLAGS_LINEAR_FILTER_BIT = 0x00000001,

    /// \brief Enable use of the 2D engine copy path.
    ///
    /// ENGINE_2D is only supported for
    /// nvnCommandBufferCopyBufferToTexture. This flag should not be set
    /// for any other copy command.
    ///
    /// This flag directs nvnCommandBufferCopyBufferToTexture to use the
    /// same hardware engine that nvnCommandBufferCopyTextureToTexture
    /// uses, which is capable of transparently compressing compressible
    /// textures. See #NVN_TEXTURE_FLAGS_COMPRESSIBLE_BIT. This flag can be
    /// used with all of the color formats that
    /// nvnCommandBufferCopyTextureToTexture supports, except for
    /// block-based compressed texture formats. Only compressible formats
    /// will have their data compressed.
    NVN_COPY_FLAGS_ENGINE_2D_BIT = 0x00000002,

    /// \brief Mirrors data in the X direction while copying.
    ///
    /// MIRROR_X is only supported by nvnCommandBufferCopyTextureToTexture,
    /// or by nvnCommandBufferCopyBufferToTexture when using ENGINE_2D.
    /// This flag should not be set for any other copy command.
    NVN_COPY_FLAGS_MIRROR_X_BIT = 0x00000004,

    /// \brief Mirrors data in the Y direction while copying.
    NVN_COPY_FLAGS_MIRROR_Y_BIT = 0x00000008,

    /// \brief Mirrors data in the Z direction while copying.
    NVN_COPY_FLAGS_MIRROR_Z_BIT = 0x00000010,

    NVN_ENUM_32BIT(COPY_FLAGS),
} NVNcopyFlags;

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
typedef enum NVNviewportSwizzle {

    /// \brief Select the X component of the input position vector.
    NVN_VIEWPORT_SWIZZLE_POSITIVE_X = 0,

    /// \brief Select and negate the X component of the input position
    /// vector.
    NVN_VIEWPORT_SWIZZLE_NEGATIVE_X = 1,

    /// \brief Select the Y component of the input position vector.
    NVN_VIEWPORT_SWIZZLE_POSITIVE_Y = 2,

    /// \brief Select and negate the Y component of the input position
    /// vector.
    NVN_VIEWPORT_SWIZZLE_NEGATIVE_Y = 3,

    /// \brief Select the Z component of the input position vector.
    NVN_VIEWPORT_SWIZZLE_POSITIVE_Z = 4,

    /// \brief Select and negate the Z component of the input position
    /// vector.
    NVN_VIEWPORT_SWIZZLE_NEGATIVE_Z = 5,

    /// \brief Select the W component of the input position vector.
    NVN_VIEWPORT_SWIZZLE_POSITIVE_W = 6,

    /// \brief Select and negate the W component of the input position
    /// vector.
    NVN_VIEWPORT_SWIZZLE_NEGATIVE_W = 7,

    NVN_ENUM_32BIT(VIEWPORT_SWIZZLE),
} NVNviewportSwizzle;

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
typedef enum NVNcoverageModulationMode {

    /// \brief Do not modify color (RGB) or alpha components (default).
    NVN_COVERAGE_MODULATION_MODE_NONE = 0x00000000,

    /// \brief Multiply color (RGB) components by the modulation factor,
    /// but do not modify alpha.
    NVN_COVERAGE_MODULATION_MODE_RGB = 0x00000001,

    /// \brief Multiply alpha components by the modulation factor, but do
    /// not modify color (RGB).
    NVN_COVERAGE_MODULATION_MODE_ALPHA = 0x00000002,

    /// \brief Multiply color (RGB) and alpha components by the modulation
    /// factor.
    NVN_COVERAGE_MODULATION_MODE_RGBA = 0x00000003,

    NVN_ENUM_32BIT(COVERAGE_MODULATION_MODE),
} NVNcoverageModulationMode;

/// \brief Flags controlling the behavior of NVNqueue objects.
typedef enum NVNqueueFlags {

    /// \brief Disable support for fragment shader interlocks.
    ///
    /// If set, this flag indicates that the application will not be using
    /// fragment shader interlocks (e.g., the NV_fragment_shader_interlock
    /// GLSL extension) and that no per-queue memory needs to be allocated
    /// to support the feature.
    ///
    /// \note If this bit is set, it is illegal to bind a NVNprogram object
    /// using the fragment shader interlock feature.
    NVN_QUEUE_FLAGS_NO_FRAGMENT_INTERLOCK_BIT = 0x00000001,

    /// \brief Disable ZCull.
    ///
    /// Setting this flag reduces the overhead of switching between NVN
    /// queues, but significantly reduces the performance of rendering with
    /// depth testing enabled. Due to the rendering performance cost, we
    /// recommend only setting this flag for queues that perform relatively
    /// little rendering work or do not require depth testing.
    ///
    /// \note If this bit is set, ZCull related functionality like
    /// nvnCommandBufferSaveZCullData() and CommandBuffer::LoadZCullData()
    /// will be treated as no-ops by the hardware.
    NVN_QUEUE_FLAGS_NO_ZCULL_BIT = 0x00000002,

    /// \brief No special operation behavior is required by the queue.
    NVN_QUEUE_FLAGS_NONE = 0x00000000,

    NVN_ENUM_32BIT(QUEUE_FLAGS),
} NVNqueueFlags;

/// \brief Debug layer object type.
typedef enum NVNdebugObjectType {

    /// \brief NVNwindow object type.
    NVN_DEBUG_OBJECT_TYPE_WINDOW = 0,

    /// \brief NVNqueue object type.
    NVN_DEBUG_OBJECT_TYPE_QUEUE = 1,

    /// \brief NVNcommandBuffer object type.
    NVN_DEBUG_OBJECT_TYPE_COMMAND_BUFFER = 2,

    /// \brief NVNmemoryPool object type.
    NVN_DEBUG_OBJECT_TYPE_MEMORY_POOL = 3,

    /// \brief NVNsync object type.
    NVN_DEBUG_OBJECT_TYPE_SYNC = 7,

    /// \brief NVNprogram object type.
    NVN_DEBUG_OBJECT_TYPE_PROGRAM = 9,

    /// \brief NVNtexturePool object type.
    NVN_DEBUG_OBJECT_TYPE_TEXTURE_POOL = 10,

    /// \brief NVNsamplerPool object type.
    NVN_DEBUG_OBJECT_TYPE_SAMPLER_POOL = 11,

    NVN_ENUM_32BIT(DEBUG_OBJECT_TYPE),
} NVNdebugObjectType;

/// \brief Method used by nvnCommandBufferSignalEvent to update event
/// memory. When nvnCommandBufferSignalEvent is called, this mode specifies
/// how memory associated with the NVNevent object is updated.
typedef enum NVNeventSignalMode {

    /// \brief Store the value provided by the SignalEvent command to event
    /// memory.
    ///
    /// This mode is supported on all platforms.
    NVN_EVENT_SIGNAL_MODE_WRITE = 0,

    /// \brief Atomically add the value provided by the SignalEvent command
    /// to event memory.
    ///
    /// This mode is not supported on all platforms. Support is determined
    /// by querying the EVENTS_SUPPORT_REDUCTION_OPERATIONS device
    /// property.
    NVN_EVENT_SIGNAL_MODE_ADD = 1,

    NVN_ENUM_32BIT(EVENT_SIGNAL_MODE),
} NVNeventSignalMode;

/// \brief Method used by nvnCommandBufferWaitEvent to wait for the
/// completion of a previous signal. When nvnCommandBufferWaitEvent is
/// executed by the GPU, the value in the memory associated with the
/// NVNevent object is compared to a provided value. This mode specifies
/// whether this comparison is considered to pass or fail.
typedef enum NVNeventWaitMode {

    /// \brief The test passes if the value in event memory is exactly
    /// equal to the value provided to WaitEvent.
    NVN_EVENT_WAIT_MODE_EQUAL = 0,

    /// \brief The test passes if the value in event memory is greater than
    /// or equal to the value provided to WaitEvent, where the comparison
    /// uses a wrapped comparison:
    ///
    ///     uint32_t(*memory_value - wait_value) < 0x80000000U;
    ///
    /// This comparison allows for an incrementing event value that
    /// requires no special-case wrapping logic.  When a counter wraps, a
    /// wrapped value like 0x00000001 will be considered greater than a
    /// pre-wrapped value like 0xFFFFFFFE.
    NVN_EVENT_WAIT_MODE_GEQUAL_WRAP = 1,

    NVN_ENUM_32BIT(EVENT_WAIT_MODE),
} NVNeventWaitMode;

/// \brief Mode determining when nvnCommandBufferSignalEvent signals the
/// event.
typedef enum NVNeventSignalLocation {

    /// \brief Signal the event when the command is processed by the top of
    /// the GPU pipeline.
    NVN_EVENT_SIGNAL_LOCATION_TOP = 0,

    /// \brief Signal the event when vertex processing pipeline has
    /// completed work on all previously submitted draw calls. Does not
    /// wait for rasterization and fragment processing.
    NVN_EVENT_SIGNAL_LOCATION_VERTEX_PIPE = 1,

    /// \brief Signal the event when work for all previously submitted GPU
    /// commands has completed.
    NVN_EVENT_SIGNAL_LOCATION_BOTTOM = 2,

    NVN_ENUM_32BIT(EVENT_SIGNAL_LOCATION),
} NVNeventSignalLocation;

/// \brief Return values for GL/NVN sync conversion interop functions.
typedef enum NVNsyncInteropStatus {

    /// \brief The call was sucessful.
    NVN_SYNC_INTEROP_STATUS_OK = 0,

    /// \brief The sync was not ready to be converted yet. Please make sure
    /// it's flushed and try again.
    NVN_SYNC_INTEROP_STATUS_AGAIN = 1,

    /// \brief The sync was already signaled.
    NVN_SYNC_INTEROP_STATUS_ALREADY = 2,

    /// \brief The GLsync was not found. Was it flushed?
    NVN_SYNC_INTEROP_STATUS_NOT_FOUND = 3,

    /// \brief On the Windows reference platform, OpenGL cannot use sync
    /// interop with both OpenCL and NVN within the same application.
    NVN_SYNC_INTEROP_STATUS_INVALID = 4,

    NVN_ENUM_32BIT(SYNC_INTEROP_STATUS),
} NVNsyncInteropStatus;

/// \brief Flags controlling the behavior of nvnCommandBufferSignalEvent.
typedef enum NVNeventSignalFlags {

    /// \brief Do not wait for completion of memory transactions from
    /// commands prior to the SignalEvent command.
    ///
    /// By default, an event is not signaled until all GPU memory
    /// transactions from prior commands have completed. When this bit is
    /// set, the event may be signaled without waiting for all memory
    /// transactions.
    NVN_EVENT_SIGNAL_FLAGS_NO_WAIT_MEMORY_BIT = 1,

    NVN_ENUM_32BIT(EVENT_SIGNAL_FLAGS),
} NVNeventSignalFlags;

/// \brief Class of a color format.
///
/// This enum is used only for color reduction to describe a set of formats
/// that are supported for color reduction. The programming guide has a
/// table of formats that enumerates the mapping from color format to
/// format class.
typedef enum NVNformatClass {

    /// \brief UNORM8 class, for instance R8, RGBA8.
    NVN_FORMAT_CLASS_UNORM8 = 0,

    /// \brief UNORM10 class, for instance R10B10G10A2.
    NVN_FORMAT_CLASS_UNORM10 = 1,

    /// \brief UNORM16 class, for instance RGB16.
    NVN_FORMAT_CLASS_UNORM16 = 2,

    /// \brief FP11 class, for instance R11G11B10F.
    NVN_FORMAT_CLASS_FP11 = 3,

    /// \brief FP16 class, for instance RGBA16F.
    NVN_FORMAT_CLASS_FP16 = 4,

    /// \brief SRGB8 class, for instance BGRA8_SRGB.
    NVN_FORMAT_CLASS_SRGB8 = 5,

    NVN_ENUM_32BIT(FORMAT_CLASS),
} NVNformatClass;

/// \brief Priority of a NVNqueue object.
///
/// Priority govern the order in which Queues are run on the GPU. See
/// nvnQueueBuilderSetQueuePriority for more information.
typedef enum NVNqueuePriority {

    /// \brief This gives a NVNqueue the lowest Priority possible.
    NVN_QUEUE_PRIORITY_LOW = 10,

    /// \brief The default setting.
    NVN_QUEUE_PRIORITY_NORMAL = 100,

    /// \brief This gives a NVNqueue the highest Priority possible.
    NVN_QUEUE_PRIORITY_HIGH = 1000,

    NVN_ENUM_32BIT(QUEUE_PRIORITY),
} NVNqueuePriority;

/// \brief Raw storage class used when accessing buffers and textures in
/// main memory on NX.
///
/// On the NX device, each storage class (StorageClass) also has an
/// associated raw storage class used to perform format-specific
/// optimizations when the GPU's L2 cache reads data from or writes data to
/// main memory.  When a physical memory page in a NVNmemoryPool object is
/// first used with one raw storage class and then used with a different
/// raw storage class, applications must ensure that the GPU's L2 cache is
/// invalidated before it executes commands using the new raw storage
/// class.  This is done automatically if the NVNqueue used to submit
/// commands is flushed between commands using the new and old raw storage
/// classes.  The L2 cache can be invalidated manually using
/// nvnCommandBufferBarrier with the Barrier::INVALIDATE_L2_CACHE bit set.
///
/// \note The raw storage class has no effect on the Windows reference
/// implementation.
typedef enum NVNrawStorageClass {

    /// \brief No raw storage class.
    NVN_RAW_STORAGE_CLASS_NONE = 0,

    /// \brief Raw storage class used for linear and buffer textures and
    /// NVNbuffer objects.
    NVN_RAW_STORAGE_CLASS_LINEAR = 1,

    /// \brief Raw storage class used for textures (other than linear and
    /// buffer textures) using any color texture format or
    /// #NVN_FORMAT_DEPTH32F.
    NVN_RAW_STORAGE_CLASS_COLOR_AND_ZF32 = 2,

    /// \brief Raw storage class used for textures (other than linear and
    /// buffer textures) using #NVN_FORMAT_STENCIL8 or #NVN_FORMAT_DEPTH16.
    NVN_RAW_STORAGE_CLASS_S8_AND_Z16 = 3,

    /// \brief Raw storage class used for textures (other than linear and
    /// buffer textures) using #NVN_FORMAT_DEPTH24 or
    /// #NVN_FORMAT_DEPTH24_STENCIL8.
    NVN_RAW_STORAGE_CLASS_Z24_AND_Z24S8 = 4,

    /// \brief Raw storage class used for textures (other than linear and
    /// buffer textures) using #NVN_FORMAT_DEPTH32F_STENCIL8.
    NVN_RAW_STORAGE_CLASS_ZF32S8 = 5,

    NVN_ENUM_32BIT(RAW_STORAGE_CLASS),
} NVNrawStorageClass;

/// \brief Sample shading mode used when executing a fragment shader.
typedef enum NVNsampleShadingMode {

    /// \brief Default mode. The fragment shader is evaluated either by
    /// fragment or by sample, depending on whether a keyword such as
    /// gl_SamplePosition is specified within the fragment shader.
    NVN_SAMPLE_SHADING_MODE_DEFAULT_FRAGMENT = 0,

    /// \brief Force the fragment shader to be evaluated per sample.
    NVN_SAMPLE_SHADING_MODE_FORCE_SAMPLE_SHADING_ON = 1,

    /// \brief Force the fragment shader to be evaluated per fragment,
    /// overriding any behavior from any keywords in the fragment shader.
    NVN_SAMPLE_SHADING_MODE_FORCE_SAMPLE_SHADING_OFF = 2,

    NVN_ENUM_32BIT(SAMPLE_SHADING_MODE),
} NVNsampleShadingMode;

/// \}
/// \addtogroup nvn_c_struct
/// \{

/// \brief Data structure representing the shader data.
///
/// The data structure is for one shader stage.  The contents of this
/// struct contain everything NVN needs to set up the program for use with
/// nvnProgramSetShaders
typedef struct NVNshaderData {

    /// \brief Address within a memory pool where the program resides.
    NVNbufferAddress    data;

    /// \brief Control section from the offline compiler. This address must
    /// be aligned to 8 bytes.
    const void*         control;

} NVNshaderData;

/// \brief Data structure containing hashes to replace the hashes built
/// into the shader control sections.
///
/// The data structure is for one shader stage and is meant to be used with
/// NVNshaderDataExt. The values in this structure will replace the values
/// built into the stage's control section. This data structure is binary
/// identical to the GLSLC data structure GLSLCdebugDataHash and can be
/// used in place of references to that structure.
typedef struct NVNdebugDataHash {

    /// \brief The build ID from the GLSLCdebugDataHash section for this
    /// shader stage.
    uint32_t    buildId[4];

    /// \brief The low 32 bits of the hash value from the
    /// GLSLCdebugDataHash section for this shader stage.
    uint32_t    debugHashLo32;

    /// \brief The high 32 bits of the hash value from the
    /// GLSLCdebugDataHash section for this shader stage
    uint32_t    debugHashHi32;

} NVNdebugDataHash;

/// \brief Data structure representing the extended shader data with
/// optional replacement debug hashes.
///
/// The data structure is for one shader stage.  The contents of this
/// struct contain everything NVN needs to set up the program for use with
/// nvnProgramSetShaders. This is an extended version of NVNshaderData
/// which includes optional debug hashes that the driver uses to replace
/// the debug hashes in the control binary.
typedef struct NVNshaderDataExt {

    /// \brief Address within a memory pool where the program resides.
    NVNbufferAddress            data;

    /// \brief Control section from the offline compiler. This address must
    /// be aligned to 8 bytes.
    const void*                 control;

    /// \brief Debug data hash override for the debug data hash embedded in
    /// the control section. The definition of this data comes from
    /// NvnGlslc headers. If NULL, nvnProgramSetShadersExt will behave the
    /// same as nvnProgramSetShaders.
    const NVNdebugDataHash*     debugDataHash;

} NVNshaderDataExt;

/// \}
/// \addtogroup nvn_c_enum
/// \{

/// \brief Error status returned by _GetError_
typedef enum NVNqueueGetErrorResult {

    /// \brief No GPU error was detected for queue.
    NVN_QUEUE_GET_ERROR_RESULT_GPU_NO_ERROR = 0x00000000,

    /// \brief Unrecognized GPU error.
    NVN_QUEUE_GET_ERROR_RESULT_GPU_ERROR_UNKNOWN = 0x00000001,

    /// \brief GPU encountered MMU fault.
    NVN_QUEUE_GET_ERROR_RESULT_GPU_ERROR_MMU_FAULT = 0x00000002,

    /// \brief GPU encountered pushbuffer DMA error.
    NVN_QUEUE_GET_ERROR_RESULT_GPU_ERROR_PBDMA_EXCEPTION = 0x00000003,

    /// \brief GPU encountered graphics engine exception.
    NVN_QUEUE_GET_ERROR_RESULT_GPU_ERROR_ENGINE_EXCEPTION = 0x00000004,

    /// \brief GPU encountered timeout.
    NVN_QUEUE_GET_ERROR_RESULT_GPU_ERROR_TIMEOUT = 0x00000005,

    NVN_ENUM_32BIT(QUEUE_GET_ERROR_RESULT),
} NVNqueueGetErrorResult;

/// \brief Read or write access.
typedef enum NVNmemoryAccess {

    /// \brief Read access.
    NVN_MEMORY_ACCESS_MEMORY_ACCESS_READ = 0x00000000,

    /// \brief Write access.
    NVN_MEMORY_ACCESS_MEMORY_ACCESS_WRITE = 0x00000001,

    NVN_ENUM_32BIT(MEMORY_ACCESS),
} NVNmemoryAccess;

/// \}
/// \addtogroup nvn_c_struct
/// \{

/// \brief Data structure representing GPU error info.
typedef union NVNqueueErrorInfo {

    struct {
        /// \brief Fault address.
        uint64_t            faultAddress;

        /// \brief Specifies if fault was caused by read or write memory
        /// access.
        NVNmemoryAccess     accessType;

    } mmuFault;

    /// \brief Reserved for future use.
    int     reserved[16];

} NVNqueueErrorInfo;

/// \brief Data structure representing values used by indirect DrawArrays
/// commands.
///
/// This data structure is not used by any API command, but instead
/// represents the layout of GPU-accessible memory pointed to by
/// nvnCommandBufferDrawArraysIndirect.
typedef struct NVNdrawArraysIndirectData {

    /// \brief Number of vertices in each instance (copy) of the primitives
    /// to be drawn.
    int     count;

    /// \brief Number of instances (copies) of the primitives to be drawn.
    int     instanceCount;

    /// \brief Index number of the first vertex in each drawn instance.
    int     first;

    /// \brief Instance number of the first instance drawn.
    int     baseInstance;

} NVNdrawArraysIndirectData;

/// \brief Data structure representing values used by indirect DrawElements
/// commands.
///
/// This data structure is not used by any API command, but instead
/// represents the layout of GPU-accessible memory pointed to by
/// nvnCommandBufferDrawElementsIndirect.
typedef struct NVNdrawElementsIndirectData {

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

} NVNdrawElementsIndirectData;

/// \brief Data structure representing values used by indirect
/// DispatchCompute commands.
///
/// This data structure is not used by any API command, but instead
/// represents the layout of GPU-accessible memory pointed to by
/// nvnCommandBufferDispatchComputeIndirect.
typedef struct NVNdispatchComputeIndirectData {

    /// \brief The number of compute work groups in the X dimension.
    int     groupsX;

    /// \brief The number of compute work groups in the Y dimension.
    int     groupsY;

    /// \brief The number of compute work groups in the Z dimension.
    int     groupsZ;

} NVNdispatchComputeIndirectData;

/// \brief Data structure describing how counter values are written to
/// buffer memory.
///
/// This structure can be used to interpret values written in reports from
/// nvnCommandBufferReportCounter for all counters types except for
/// #NVN_COUNTER_TYPE_ZCULL_STATS.
typedef struct NVNcounterData {

    /// \brief Value of requested counter.
    uint64_t    counter;

    /// \brief GPU timestamp indicating when the counter was written.
    ///
    /// Please refer to the \ref nvn_guide_counter_resolution_of_timestamp
    /// "NVN Programming Guide" for details.
    uint64_t    timestamp;

} NVNcounterData;

/// \brief Data structure describing device error condition.
typedef struct NVNdeviceErrorData {

    /// \brief Type of the error.
    NVNdeviceError  error;

    /// \brief NVNqueue object that generated the error.
    NVNqueue*       queue;

    /// \brief Reserved for future use.
    int             reserved[16];

} NVNdeviceErrorData;

/// \brief Data structure describing a rectangle in integer coordinates.
typedef struct NVNrectangle {

    /// \brief X coordinate.
    int     x;

    /// \brief Y coordinate.
    int     y;

    /// \brief Width of rectangle.
    int     width;

    /// \brief Height of rectangle.
    int     height;

} NVNrectangle;

/// \brief Data structure describing a virtual memory pool mapping request.
///
/// nvnMemoryPoolMapVirtual can be used to map memory from a physical
/// memory pool (one using #NVN_MEMORY_POOL_FLAGS_PHYSICAL_BIT) into a
/// virtual memory pool (one using #NVN_MEMORY_POOL_FLAGS_VIRTUAL_BIT).
/// Each individual mapping request will map a contiguous set of pages in
/// the physical pool into a contiguous range of the virtual pool, using a
/// specified storage class for the virtual memory mapping.
typedef struct NVNmappingRequest {

    /// \brief Memory pool providing the physical memory for the virtual
    /// memory mapping.
    ///
    /// This parameter must be NULL or a pointer to a physical memory pool
    /// (initialized with #NVN_MEMORY_POOL_FLAGS_PHYSICAL_BIT).  If NULL,
    /// the range of the virtual memory pool will be unmapped, where GPU
    /// accesses will return zero values (NX and second-generation Maxwell
    /// GPUs) or undefined values.
    NVNmemoryPool*      physicalPool;

    /// \brief Offset (in bytes) of the first mapped page from the physical
    /// pool.
    ///
    /// Must be zero if _physicalPool_ is NULL.
    /// Must be a multiple of the value of
    /// #NVN_DEVICE_INFO_MEMORY_POOL_PAGE_SIZE.
    int64_t             physicalOffset;

    /// \brief Offset (in bytes) of the first mapped page in the virtual
    /// pool.
    /// Must be a multiple of the value of
    /// #NVN_DEVICE_INFO_MEMORY_POOL_PAGE_SIZE.
    int64_t             virtualOffset;

    /// \brief Size (in bytes) of the requested virtual memory mapping.
    /// Must be a multiple of the value of
    /// #NVN_DEVICE_INFO_MEMORY_POOL_PAGE_SIZE.
    uint64_t            size;

    /// \brief Storage class used for the virtual memory mapping.
    ///
    /// When memory from a physical memory pool is mapped into a virtual
    /// memory pool, the mapping must be established with a storage class
    /// appropriate for the texture or buffer resources using the virtual
    /// memory.  For textures, the required storage class can be determined
    /// by calling nvnTextureBuilderGetStorageClass or
    /// nvnTextureGetStorageClass.  For buffers, NVN_STORAGE_CLASS_BUFFER
    /// should be used.
    NVNstorageClass     storageClass;

} NVNmappingRequest;

/// \brief Data structure describing a region of a texture used in copy
/// commands.
///
/// \note Copy commands accessing a texture accept one or more pointers to
/// copy region structures.  NULL pointers are not supported.
typedef struct NVNcopyRegion {

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

} NVNcopyRegion;

/// \brief Data structure used by queries of the memory layout used by
/// sparse textures.
///
/// This structure is used to return properties of the layout of a sparse
/// texture in memory.  In a sparse texture, some mipmap levels can be
/// decomposed into a collection of fixed-size tiles, where each tile is
/// aligned on a virtual memory pool page boundary.  The set of smaller
/// mipmap levels that are not split into tiles is referred to as the
/// mipmap _tail_.
typedef struct NVNtextureSparseTileLayout {

    /// \brief The number of mipmap levels that can be split into
    /// fixed-size tiles aligned on memory page boundaries.
    ///
    /// This value will always be less than or equal to the number of
    /// mipmap levels in the texture.  Zero is returned if the texture does
    /// not have #NVN_TEXTURE_FLAGS_SPARSE_BIT or if none of the mipmap
    /// levels support fully aligned tiles.
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

} NVNtextureSparseTileLayout;

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
typedef struct NVNpackagedTextureLayout {

    /// \brief Data produced by the NVN texture packager indicating the
    /// layout of the packaged texture data in memory.
    char    layout[8];

} NVNpackagedTextureLayout;

/// \brief Data structure specifying a range of buffer memory.
///
/// This structure specifies a range of buffer memory. It is used in the
/// API calls that bind multiple buffer ranges in a single call.
typedef struct NVNbufferRange {

    /// \brief Memory address of the beginning of the buffer range.
    NVNbufferAddress    address;

    /// \brief Size of the buffer range in bytes.
    uint64_t            size;

} NVNbufferRange;

/// \brief Data structure describing a region used by the DrawTexture
/// command.
typedef struct NVNdrawTextureRegion {

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

} NVNdrawTextureRegion;

/// \}
/// \addtogroup nvn_c_defines
/// \{

/// \brief Boolean condition with a value of false.
#define NVN_FALSE                                                   0x0000

/// \brief Boolean condition with a value of true.
#define NVN_TRUE                                                    0x0001

/// \brief When waiting on NVNsync objects, do not wait if the sync object
/// has not yet been signaled.
#define NVN_WAIT_TIMEOUT_NONE                                       0x0ULL

/// \brief When waiting on NVNsync objects, wait indefinitely until the
/// sync object is signaled.
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
/// \addtogroup nvn_c_funcptrs
/// \{

/// \brief Generic function pointer type returned when querying API entry
/// points.
typedef void (NVNAPIENTRYP PFNNVNGENERICFUNCPTRPROC)(void);

/// \brief Function pointer type used to retreive main API entry points
/// such as nvnDeviceInitialize and nvnDeviceGetProcAddress.
///
/// \param name Name of the C entry point to query.
typedef PFNNVNGENERICFUNCPTRPROC (NVNAPIENTRYP PFNNVNBOOTSTRAPLOADERPROC)(const char * name);

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
typedef void (NVNAPIENTRYP PFNNVNDEBUGCALLBACKPROC)(NVNdebugCallbackSource source,
                                                    NVNdebugCallbackType type,
                                                    int id,
                                                    NVNdebugCallbackSeverity severity,
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
typedef void (NVNAPIENTRYP PFNNVNCOMMANDBUFFERMEMORYCALLBACKPROC)(NVNcommandBuffer *cmdBuf,
                                                                  NVNcommandBufferMemoryEvent event,
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
typedef void (NVNAPIENTRYP PFNNVNWALKDEBUGDATABASECALLBACKPROC)(void* object,
                                                                void *userParam);

/// \}

#ifdef NVN_PROTOTYPES

/// \addtogroup nvn_c_functions
/// \{

/// \brief Set default state for the NVNdeviceBuilder object.
///
/// \param builder NVNdeviceBuilder object to reset.
NVNAPI void NVNAPIENTRY
    nvnDeviceBuilderSetDefaults(NVNdeviceBuilder *builder);

/// \brief Set flags used for during device initialzation.
///
/// \param builder NVNdeviceBuilder object to update.
///
/// \param flags Flags to use during device initialization. See
/// #NVNdeviceFlagBits.
NVNAPI void NVNAPIENTRY
    nvnDeviceBuilderSetFlags(NVNdeviceBuilder *builder,
                             int flags);

/// \brief Get flags associated with this NVNdeviceBuilder object.
///
/// \param builder NVNdeviceBuilder object to query.
NVNAPI NVNdeviceFlagBits NVNAPIENTRY
    nvnDeviceBuilderGetFlags(const NVNdeviceBuilder *builder);

/// \brief Initialize a NVNdevice object from a device builder.
///
/// \param device NVNdevice object to initialize.
///
/// \param builder NVNdeviceBuilder object holding properties of the new
/// NVNdevice object.
NVNAPI NVNboolean NVNAPIENTRY
    nvnDeviceInitialize(NVNdevice *device,
                        const NVNdeviceBuilder *builder);

/// \brief Finalize a NVNdevice object.
///
/// \param device NVNdevice object to finalize.
NVNAPI void NVNAPIENTRY
    nvnDeviceFinalize(NVNdevice *device);

/// \brief Sets the debug label string for a NVNdevice object.
///
/// Annotates a NVNdevice object with a descriptive text label. This label
/// may then be used by the debug layer, an external debugger, or a
/// profiler tool.
///
/// \param device NVNdevice object to set debug label.
///
/// \param label Text string used to annotate the NVNdevice object.
NVNAPI void NVNAPIENTRY
    nvnDeviceSetDebugLabel(NVNdevice *device,
                           const char * label);

/// \brief Query a function pointer for a specified C interface entry point
/// for a device.
///
/// \param device NVNdevice object to query.
///
/// \param name Name of the C entry point to query.
NVNAPI PFNNVNGENERICFUNCPTRPROC NVNAPIENTRY
    nvnDeviceGetProcAddress(const NVNdevice *device,
                            const char * name);

/// \brief Query properties of a device.
///
/// \note _device_ may be NULL when querying supported API or GPU shader
/// code versions prior to creating a device object.  For all other
/// queries, _device_ must point at a valid NVN device object.
///
/// \param device NVNdevice to query.
///
/// \param pname Property to query.
///
/// \param[out] v Value associated with the property, returned to the
/// application.
NVNAPI void NVNAPIENTRY
    nvnDeviceGetInteger(const NVNdevice *device,
                        NVNdeviceInfo pname,
                        int *v);

/// \brief Get timestamp from the device, without queuing like the
/// ReportCounter functions. See the \ref nvn_guide_counters "Counters"
/// section of the NVN Programming Guide for more detail.
///
/// \note _device_ must point at a valid NVN device object.
///
/// \param device NVNdevice to query.
NVNAPI uint64_t NVNAPIENTRY
    nvnDeviceGetCurrentTimestampInNanoseconds(const NVNdevice *device);

/// \brief Configure the shader cache for the Windows reference platform to
/// store fat binary intermediate results. If the NVN platform is NX, this
/// function does nothing.
///
/// On the Windows reference implemenetation, nvnProgramSetShaders will
/// compile machine code that can run on the attached GPU, using
/// intermediate shader code and specialization information stored in the
/// provided control data. If the same shader is compiled with different
/// specialization information (via glslcCompileSpecialized) using this
/// cache can avoid re-processing the same shader code on each call to
/// nvnProgramSetShaders.
///
/// \param device The device owning the programs to cache.
///
/// \param numMaxEntries Maximum number of entries that the intermediate
/// shader cache can hold. If this is 0, the cache will be disabled and any
/// previous cache resources will be cleaned up.  If a non-zero value is
/// used, the cache will hold intermediate code for up to _numMaxEntries_
/// unique shaders.  Programs containing multiple shader stages will
/// consume multiple entries in this cache.
NVNAPI void NVNAPIENTRY
    nvnDeviceSetIntermediateShaderCache(NVNdevice *device,
                                        int numMaxEntries);

/// \brief Get a handle for a combined pair of NVNtexture and NVNsampler
/// objects.
///
/// \param device The device owning the texture and sampler objects.
///
/// \param textureID Registered ID of the NVNtexture object.
/// Must be greater than or equal to 256 (the value of
/// #NVN_DEVICE_INFO_RESERVED_TEXTURE_DESCRIPTORS).
///
/// \param samplerID Registered ID of the NVNsampler object.
/// Must be greater than or equal to 256 (the value of
/// #NVN_DEVICE_INFO_RESERVED_SAMPLER_DESCRIPTORS).
NVNAPI NVNtextureHandle NVNAPIENTRY
    nvnDeviceGetTextureHandle(const NVNdevice *device,
                              int textureID,
                              int samplerID);

/// \brief Get a handle for a NVNtexture object for use in texel fetches.
///
/// A handle produced by this entry point can be used for texel fetches via
/// the texelFetch() GLSL built-in function.  Using such handles in texture
/// lookups will use undefined sampler state.
///
/// \param device The device owning the texture object.
///
/// \param textureID Registered ID of the NVNtexture object.
/// Must be greater than or equal to 256 (the value of
/// #NVN_DEVICE_INFO_RESERVED_TEXTURE_DESCRIPTORS).
NVNAPI NVNtextureHandle NVNAPIENTRY
    nvnDeviceGetTexelFetchHandle(const NVNdevice *device,
                                 int textureID);

/// \brief Get a handle to use for image loads and stores to a NVNtexture
/// object.
///
/// \param device The device owning the texture and sampler objects.
///
/// \param imageID Registered image ID of the NVNtexture object.
/// Must be greater than or equal to 256 (the value of
/// #NVN_DEVICE_INFO_RESERVED_TEXTURE_DESCRIPTORS).
NVNAPI NVNimageHandle NVNAPIENTRY
    nvnDeviceGetImageHandle(const NVNdevice *device,
                            int imageID);

/// \brief Install or remove a callback function to be called whenever the
/// debug layer generates a message.
///
/// Multiple callback functions (each with separate callback data) are
/// supported.
///
/// \param device NVNdevice used for the callback.
///
/// \param callback Pointer to a callback function to call.
///
/// \param[in,out] callbackData Pointer to arbitrary data to pass in each
/// call to the callback function.
///
/// \param enable Enable (TRUE) or disable (FALSE) the specified callback
/// function.
NVNAPI void NVNAPIENTRY
    nvnDeviceInstallDebugCallback(NVNdevice *device,
                                  const PFNNVNDEBUGCALLBACKPROC callback,
                                  void *callbackData,
                                  NVNboolean enable);

/// \brief This returns a unique ID by atomically incrementing a global
/// counter on each request.
///
/// \param device NVNdevice used.
///
/// \param name Name to associate to new domain id.
NVNAPI NVNdebugDomainId NVNAPIENTRY
    nvnDeviceGenerateDebugDomainId(const NVNdevice *device,
                                   const char * name);

/// \brief Set the global location for window coordinates (0,0).
///
/// Applications should set the window origin mode in the device before
/// creating any queues or command buffers, and should use that mode for
/// the lifetime of the application. Rendering results are undefined if
/// there are any inconsistencies between the window origin mode (a)
/// currently set for the device, (b) set when creating the command buffer
/// used to create rendering commands, or (c) set when creating the queue
/// used to submit rendering commands.
///
/// \param device The device to be modified.
///
/// \param windowOriginMode New location for the window origin.
NVNAPI void NVNAPIENTRY
    nvnDeviceSetWindowOriginMode(NVNdevice *device,
                                 NVNwindowOriginMode windowOriginMode);

/// \brief Set the global clipping range for normalized depth in clip
/// space.
///
/// Applications should set the depth mode in the device before creating
/// any queues or command buffers, and should use that mode for the
/// lifetime of the application. Rendering results are undefined if there
/// are any inconsistencies between the depth mode (a) currently set for
/// the device, (b) set when creating the command buffer used to create
/// rendering commands, or (c) set when creating the queue used to submit
/// rendering commands.
///
/// \param device The device to be modified.
///
/// \param depthMode New policy for depth clipping.
NVNAPI void NVNAPIENTRY
    nvnDeviceSetDepthMode(NVNdevice *device,
                          NVNdepthMode depthMode);

/// \brief Register a fast clear color value.
///
/// Applications can register specific combinations of clear values and
/// formats that it expects to use for future framebuffer clears. Clearing
/// the framebuffer using one of these registered clear colors will result
/// in reduced memory bandwidth usage and increased performance when used
/// with compressible textures.
///
/// The number of clear values that can be registered is limited.
/// Additionally, this command is supported only for formats with more than
/// 16 bits per pixel and floating-point or normalized components.  If a
/// clear value or format can not be registered, NVN_FALSE is returned.  If
/// registration is successful, NVN_TRUE is returned.
///
/// \param device The device to be modified.
///
/// \param color Array of four floating-point (RGBA) clear color values.
///
/// \param format NVNtexture format to encode the value into.
NVNAPI NVNboolean NVNAPIENTRY
    nvnDeviceRegisterFastClearColor(NVNdevice *device,
                                    const float *color,
                                    NVNformat format);

/// \brief Register a signed integer fast clear color value.
///
/// Applications can register specific combinations of clear values and
/// formats that it expects to use for future framebuffer clears. Clearing
/// the framebuffer using one of these registered clear colors will result
/// in reduced memory bandwidth usage and increased performance when used
/// with compressible textures.
///
/// The number of clear values that can be registered is limited.
/// Additionally, this command is supported only for formats with more than
/// 16 bits per pixel and signed integer components.  If a clear value or
/// format can not be registered, NVN_FALSE is returned.  If registration
/// is successful, NVN_TRUE is returned.
///
/// \param device The device to be modified.
///
/// \param color Array of four signed integer (RGBA) clear color values.
///
/// \param format NVNtexture format to encode the value into.
NVNAPI NVNboolean NVNAPIENTRY
    nvnDeviceRegisterFastClearColori(NVNdevice *device,
                                     const int *color,
                                     NVNformat format);

/// \brief Register an unsigned integer fast clear color value.
///
/// Applications can register specific combinations of clear values and
/// formats that it expects to use for future framebuffer clears. Clearing
/// the framebuffer using one of these registered clear colors will result
/// in reduced memory bandwidth usage and increased performance when used
/// with compressible textures.
///
/// The number of clear values that can be registered is limited.
/// Additionally, this command is supported only for formats with more than
/// 16 bits per pixel and unsigned integer components.  If a clear value or
/// format can not be registered, NVN_FALSE is returned.  If registration
/// is successful, NVN_TRUE is returned.
///
/// \param device The device to be modified.
///
/// \param color Array of four unsigned integer (RGBA) clear color values.
///
/// \param format NVNtexture format to encode the value into.
NVNAPI NVNboolean NVNAPIENTRY
    nvnDeviceRegisterFastClearColorui(NVNdevice *device,
                                      const uint32_t *color,
                                      NVNformat format);

/// \brief Register a fast clear depth value.
///
/// Applications can register specific clear values that it expects to use
/// for future depth buffer clears. Clearing the depth buffer using one of
/// these registered clear depth values will result in reduced memory
/// bandwidth usage and increased performance when used with compressible
/// textures.
///
/// The number of clear values that can be registered is finite. If a clear
/// value can not be registered, NVN_FALSE is returned. If registration is
/// successful, NVN_TRUE is returned.
/// \note 16-bit depth textures (#NVN_FORMAT_DEPTH16) have limited support
/// for fast clears.  See documentation on
/// #NVN_TEXTURE_FLAGS_DEPTH16_PREFER_FAST_CLEAR_BIT for more information.
///
/// \param device The device to be modified.
///
/// \param depth Depth value.
NVNAPI NVNboolean NVNAPIENTRY
    nvnDeviceRegisterFastClearDepth(NVNdevice *device,
                                    float depth);

/// \brief Query the window origin mode for a device.
///
/// \param device The device to query.
NVNAPI NVNwindowOriginMode NVNAPIENTRY
    nvnDeviceGetWindowOriginMode(const NVNdevice *device);

/// \brief Query the depth mode for a device.
///
/// \param device The device to query.
NVNAPI NVNdepthMode NVNAPIENTRY
    nvnDeviceGetDepthMode(const NVNdevice *device);

/// \brief Returns the timestamp contained in a counter report, with time
/// values converted from the platform-specific timestamp resolution to
/// nanoseconds.
///
/// \param device NVNdevice used to perform the counter report operation.
///
/// \param counterData Pointer to a full NVNcounterData structure
/// containing the timestamp value to convert.
NVNAPI uint64_t NVNAPIENTRY
    nvnDeviceGetTimestampInNanoseconds(const NVNdevice *device,
                                       const NVNcounterData *counterData);

/// \brief Finalize buffers, textures, and samplers previously scheduled
/// for finalization.
///
/// When a device is initialized with
/// #NVN_DEVICE_FLAG_DEFERRED_FINALIZE_BIT set, calls to nvnBufferFinalize,
/// nvnTextureFinalize, and nvnSamplerFinalize don't immediately finalize
/// buffer, texture, and sampler objects, but instead schedule them for
/// future finalization.  This command finalizes such objects, where the
/// set of objects finalized by this command depends on the _age_
/// parameter.  This command has no effect if deferred finalization is not
/// enabled in _device_.
///
/// \param device NVNdevice used to apply deferred finalizes.
///
/// \param age Minimum age for deferred finalizations.
/// Objects scheduled for finalization have an 'age' that is initially set
/// to zero and incremented each time nvnDeviceApplyDeferredFinalizes is
/// called.  Each object scheduled for finalization will be finalized by
/// this call if and only if its age (before incrementing) is greater than
/// or equal to _age_.
NVNAPI void NVNAPIENTRY
    nvnDeviceApplyDeferredFinalizes(NVNdevice *device,
                                    int age);

/// \brief Finalize a command handle.
///
/// This command can be used to free any memory associated with a command
/// handle that may have been allocated by the debug layer or other
/// developer tools that track the contents of a command set.  If command
/// handles are not explicitly finalized, associated memory will be freed
/// only when the command or control memory associated with the command set
/// is freed or reused.
///
/// This command has no effect if the debug layer and other developer tools
/// are disabled.
///
/// \param device NVNdevice associated with the command handle.
///
/// \param handles Command handle to finalize.
NVNAPI void NVNAPIENTRY
    nvnDeviceFinalizeCommandHandle(NVNdevice *device,
                                   NVNcommandHandle handles);

/// \brief Walk through the debug layer object database. Debug layer must
/// be enabled.
///
/// Reports the debug layer's list of objects of the given type through the
/// callback function.  At least Level 1 debug layer must be enabled, as
/// object tracking is not done at debug layer level 0.
/// See \ref nvn_guide_api_object_tracking "API Object Tracking" for
/// detail.
///
/// \param device NVNdevice associated with list of objects to be reported.
///
/// \param type Type of objects to walk through.
///
/// \param callback Callback function to be called for every object
/// matching the given device and object type.
///
/// \param[in,out] userParam Value to be passed to the callback holding a
/// 'user pointer'
NVNAPI void NVNAPIENTRY
    nvnDeviceWalkDebugDatabase(const NVNdevice *device,
                               NVNdebugObjectType type,
                               PFNNVNWALKDEBUGDATABASECALLBACKPROC callback,
                               void *userParam);

/// \brief Get a handle for a separate NVNtexture object.
///
/// \param device The device owning the texture object.
///
/// \param textureID Registered ID of the NVNtexture object.
/// Must be greater than or equal to 256 (the value of
/// #NVN_DEVICE_INFO_RESERVED_TEXTURE_DESCRIPTORS).
NVNAPI NVNseparateTextureHandle NVNAPIENTRY
    nvnDeviceGetSeparateTextureHandle(const NVNdevice *device,
                                      int textureID);

/// \brief Get a handle for a separate NVNsampler object.
///
/// \param device The device owning the sampler object.
///
/// \param samplerID Registered ID of the NVNsampler object.
/// Must be greater than or equal to 256 (the value of
/// #NVN_DEVICE_INFO_RESERVED_SAMPLER_DESCRIPTORS).
NVNAPI NVNseparateSamplerHandle NVNAPIENTRY
    nvnDeviceGetSeparateSamplerHandle(const NVNdevice *device,
                                      int samplerID);

/// \brief Determines if an external debugger is attached when running an
/// NVN application.
///
/// \param device The device to query.
NVNAPI NVNboolean NVNAPIENTRY
    nvnDeviceIsExternalDebuggerAttached(const NVNdevice *device);

/// \brief Get GPU error status of the queue.
///
/// Applications can use this function to check if queue encountered
/// unrecoverable GPU error such as invalid GPU memory accesses, invalid
/// command, timeout or other errors. After GPU error queue is in faulted
/// state and following behavior is expected:
/// 1. nvnQueueFinish() returns immediately.
/// 2. NVNsync objects placed into faulted queue signal completion.
/// 3. Faulted queue can be finalized to free resources.
/// 4. Remaining nvnQueue API calls for faulted queue are ignored or return
/// error.
/// 5. Other device queues are not affected or blocked by faulted queue.
///
/// \param queue NVNqueue whose status should be checked for GPU error.
///
/// \param[out] errorInfo Pointer to structure to save information about
/// error. If set to NULL, information will not be retrieved.
NVNAPI NVNqueueGetErrorResult NVNAPIENTRY
    nvnQueueGetError(NVNqueue *queue,
                     NVNqueueErrorInfo *errorInfo);

/// \brief Query total queue command memory usage.
///
/// This command returns the total amount of queue command memory (in
/// bytes) used by queue commands since the queue was initialized or since
/// usage counts were last reset by nvnQueueResetMemoryUsageCounts.  The
/// total memory usage count can be used by application developers to
/// determine the amount of memory to provide when initializing a queue.
/// Note that memory usage counts may exceed the total amount of memory
/// provided to the queue since the NVN driver will automatically reuse
/// queue memory when required.
///
/// \param queue NVNqueue object to query.
NVNAPI size_t NVNAPIENTRY
    nvnQueueGetTotalCommandMemoryUsed(NVNqueue *queue);

/// \brief Query total queue control memory usage.
///
/// This command returns the total amount of queue control memory (in
/// bytes) used by queue commands since the queue was initialized or since
/// usage counts were last reset by nvnQueueResetMemoryUsageCounts.  The
/// total memory usage count can be used by application developers to
/// determine the amount of memory to provide when initializing a queue.
/// Note that memory usage counts may exceed the total amount of memory
/// provided to the queue since the NVN driver will automatically reuse
/// queue memory when required.
///
/// \param queue NVNqueue object to query.
NVNAPI size_t NVNAPIENTRY
    nvnQueueGetTotalControlMemoryUsed(NVNqueue *queue);

/// \brief Query total queue compute memory usage.
///
/// This command returns the total amount of queue compute memory (in
/// bytes) used by queue commands since the queue was initialized or since
/// usage counts were last reset by nvnQueueResetMemoryUsageCounts.  The
/// total memory usage count can be used by application developers to
/// determine the amount of memory to provide when initializing a queue.
/// Note that memory usage counts may exceed the total amount of memory
/// provided to the queue since the NVN driver will automatically reuse
/// queue memory when required.
///
/// \param queue NVNqueue object to query.
NVNAPI size_t NVNAPIENTRY
    nvnQueueGetTotalComputeMemoryUsed(NVNqueue *queue);

/// \brief Reset usage counters for command, control and compute memory
///
/// This command resets the accumulated usage counters for queue command,
/// control, and compute memory.
///
/// \note NVNqueue memory consumed by GPU commands may not be added to the
/// usage counters reset by this command until the queue is next flushed.
/// When total queue memory usage is queried, any memory used by unflushed
/// commands will be added to the total usage counters.  As a result,
/// querying memory usage immediately after resetting counters may return
/// non-zero values if the queue has unflushed commands.
///
/// \param queue NVNqueue object with counters to reset.
NVNAPI void NVNAPIENTRY
    nvnQueueResetMemoryUsageCounts(NVNqueue *queue);

/// \brief Set the NVNdevice associated with a NVNqueueBuilder object.
///
/// \param builder NVNqueueBuilder object to update.
///
/// \param device NVNdevice owning NVNqueue objects created by the
/// NVNqueueBuilder.
NVNAPI void NVNAPIENTRY
    nvnQueueBuilderSetDevice(NVNqueueBuilder *builder,
                             NVNdevice *device);

/// \brief Set default state for the NVNqueueBuilder object.
///
/// This method does not modify the device associated with the builder,
/// which is programmed by SetDevice.
///
/// \param builder NVNqueueBuilder object to reset.
NVNAPI void NVNAPIENTRY
    nvnQueueBuilderSetDefaults(NVNqueueBuilder *builder);

/// \brief Set properties of NVNqueue objects created from the builder.
///
/// The default value for queue flags is #NVN_QUEUE_FLAGS_NONE.
///
/// \param builder NVNqueueBuilder object to update.
///
/// \param flags Flags controlling the behavior of queues initialized using
/// the builder. See #NVNqueueFlags.
NVNAPI void NVNAPIENTRY
    nvnQueueBuilderSetFlags(NVNqueueBuilder *builder,
                            int flags);

/// \brief Specify the size of the internal command buffer used by a
/// NVNqueue object.
///
/// Each NVN queue has an internal command buffer used to send commands to
/// the GPU.  This command buffer is used by some NVNqueue class methods
/// and is also used for any required submit-time processing of command
/// sets sent by nvnQueueSubmitCommands.  Most queue commands do not
/// require a large amount of command memory.  However, compute dispatches
/// and shader subroutine uniform updates can require a substantial amount
/// of per-queue command memory.  The internal queue command buffer memory
/// is managed automatically by the NVN driver, and internal queue flushes
/// may be required if a significant amount of command memory is used since
/// the last explicit queue flush.  For workloads requiring a significant
/// amount of command memory, applications can specify a command memory
/// size larger than the 64KB default size.
///
/// The default command memory size can be queried using the property
/// #NVN_DEVICE_INFO_QUEUE_COMMAND_MEMORY_DEFAULT_SIZE.
///
/// \param builder NVNqueueBuilder object to update.
///
/// \param size Size (in bytes) of the queue's internal command memory.
/// Must be greater than or equal to 65536 (the value of
/// #NVN_DEVICE_INFO_QUEUE_COMMAND_MEMORY_MIN_SIZE).
/// Must be a multiple of 4096 (the value of
/// #NVN_DEVICE_INFO_QUEUE_COMMAND_MEMORY_GRANULARITY).
NVNAPI void NVNAPIENTRY
    nvnQueueBuilderSetCommandMemorySize(NVNqueueBuilder *builder,
                                        size_t size);

/// \brief Specify the size of the internal buffers used for compute
/// dispatches in a NVNqueue object.
///
/// Each NVN queue may have an internal ring buffer used to hold
/// information on compute dispatches that have been submitted to the queue
/// but have not completed execution.  The queue compute memory is managed
/// automatically by the NVN driver, but the total size of this ring buffer
/// can limit the number of compute dispatches in flight.  Applications
/// using a large number of compute dispatches per frame may benefit from
/// increasing the size of this buffer.  Applications not using compute
/// shaders at all may set the size to zero to save memory.
///
/// The default compute memory size can be queried using the property
/// #NVN_DEVICE_INFO_QUEUE_COMPUTE_MEMORY_DEFAULT_SIZE.
///
/// \note If the queue compute memory size is zero, it is illegal to submit
/// command sets containing compute work, such as
/// nvnCommandBufferDispatchCompute.
///
/// \param builder NVNqueueBuilder object to update.
///
/// \param size Size (in bytes) of the embedded queue command buffer.
/// Must be a multiple of 256 (the value of
/// #NVN_DEVICE_INFO_QUEUE_COMPUTE_MEMORY_GRANULARITY).
NVNAPI void NVNAPIENTRY
    nvnQueueBuilderSetComputeMemorySize(NVNqueueBuilder *builder,
                                        size_t size);

/// \brief Specify the size of the control memory buffer used by a NVNqueue
/// object.
///
/// Each NVN queue has an internal control memory buffer used for gathering
/// command buffer references consumed by the GPU. For example
/// nvnQueueSubmitCommands and nvnCommandBufferCallCommands will add these
/// references. The control buffer needs to be large enough so that NVN
/// doesn't need to stall due to waiting for the buffer to drain. On the
/// other hand, unnecessarily large buffer wastes memory. The default size
/// of 16KB should be enough for most applications, but an application may
/// want to tune this value for its needs. NVN will issue a debug layer
/// performance warning in case the buffer is too small.
///
/// The default queue control memory size can be queried using the property
/// #NVN_DEVICE_INFO_QUEUE_CONTROL_MEMORY_DEFAULT_SIZE.
///
/// \param builder NVNqueueBuilder object to update.
///
/// \param size Size (in bytes) of the queue control memory.
/// Must be greater than or equal to 4096 (the value of
/// #NVN_DEVICE_INFO_QUEUE_CONTROL_MEMORY_MIN_SIZE).
/// Must be a multiple of 4096 (the value of
/// #NVN_DEVICE_INFO_QUEUE_CONTROL_MEMORY_GRANULARITY).
NVNAPI void NVNAPIENTRY
    nvnQueueBuilderSetControlMemorySize(NVNqueueBuilder *builder,
                                        size_t size);

/// \brief Query the amount of extra memory needed for a queue initialized
/// using a NVNqueueBuilder.
///
/// Each NVN queue requires a separate block of per-queue memory for the
/// GPU to use, which can be provided by nvnQueueBuilderSetQueueMemory.
/// This command allows applications to determine the size of the provided
/// memory, given the current NVNqueueBuilder state. For correct operation,
/// the NVNqueueBuilder must have a device set via
/// nvnQueueBuilderSetDevice.
///
/// \param builder NVNqueueBuilder object to query.
NVNAPI size_t NVNAPIENTRY
    nvnQueueBuilderGetQueueMemorySize(const NVNqueueBuilder *builder);

/// \brief Provides memory to be used by queues pools initialized using a
/// NVNqueueBuilder.
///
/// Each NVN queue created from a queue builder requires per-queue memory
/// for several uses:
///
/// \li Embedded command buffer memory, with a size provided by
/// nvnQueueBuilderSetCommandMemorySize.
/// \li Embedded control memory, with a size provided by
/// nvnQueueBuilderSetControlMemorySize.
/// \li Compute shader ring buffers, with a size provided by
/// nvnQueueBuilderSetComputeMemorySize.
/// \li Scratch memory for fragment shader interlocks, which can be
/// disabled by setting #NVN_QUEUE_FLAGS_NO_FRAGMENT_INTERLOCK_BIT.
/// \li Scratch memory for other internal GPU operations.
///
/// When a queue is initialized using a queue builder on NX, _size_ bytes
/// of memory pointed to by _memory_ will be owned by the queue until the
/// queue is finalized.  Applications must not access _memory_ directly
/// after calling nvnQueueInitialize.  _size_ must be sufficiently large to
/// satisfy the requirements described above; the minimum size can be
/// queried using nvnQueueBuilderGetQueueMemorySize.  On the Windows
/// reference implementation, per-queue memory is allocated by the driver,
/// and the memory provided by _memory_ (if any) is ignored.
///
/// The default queue memory size is zero, indicating that no memory is
/// provided by the application.  In that case, the NVN driver will
/// allocate a buffer of sufficient size when the queue is initialized.
///
/// \param builder NVNqueueBuilder object to update.
///
/// \param[in,out] memory Storage used for the queue memory.
/// Must be aligned to a multiple of 4KB
/// (NVN_MEMORY_POOL_STORAGE_ALIGNMENT).
///
/// \param size Size (in bytes) of the queue memory.
/// Must be aligned to a multiple of 4KB
/// (NVN_MEMORY_POOL_STORAGE_GRANULARITY).
NVNAPI void NVNAPIENTRY
    nvnQueueBuilderSetQueueMemory(NVNqueueBuilder *builder,
                                  void *memory,
                                  size_t size);

/// \brief Specify a flush threshold for a new NVNqueue object.
///
/// NVNqueue objects include an embedded command buffer that is
/// automatically managed by the NVN driver.  This command buffer is used
/// to send commands to the GPU for various NVNqueue methods or for command
/// sets that require additional processing when nvnQueueSubmitCommands is
/// called.  Most commands require very little command memory, but certain
/// commands like compute dispatches or shader subroutine uniform updates
/// may require significant amounts of command memory.  The NVN driver will
/// automatically flush the queue when the amount of command memory used
/// since the last queue flush approaches a specified flush threshold.
///
/// By default, the flush threshold is 1/8 of the total available command
/// memory.  Setting a higher threshold may reduce the number of automatic
/// flushes performed, but less frequent flushes may cause the driver to
/// have to wait for the completion of previous flushes before reusing
/// command memory.
///
/// \param builder NVNqueueBuilder object to update.
///
/// \param threshold Threshold (in bytes) of queue command memory usage
/// triggering automatic flushes.
/// Must be greater than or equal to 4096 (the value of
/// #NVN_DEVICE_INFO_QUEUE_COMMAND_MEMORY_MIN_FLUSH_THRESHOLD).
NVNAPI void NVNAPIENTRY
    nvnQueueBuilderSetCommandFlushThreshold(NVNqueueBuilder *builder,
                                            size_t threshold);

/// \brief Set a priority on a NVNqueueBuilder object.
///
/// Setting a priority on a NVNqueueBuilder object allows for additional
/// control of the relative execution timing between Queues. NVNqueue
/// Priority affects both the execution order of the Queues in their
/// timeslice as well as the (best-effort, round-robin) timesharing of the
/// GPU resources. GPU-side scheduling of Queues is a function of method
/// pipelining and shader run-time, and behavior is difficult to describe
/// in a correct and precise fashion. This function has no effect on the
/// Windows reference implementation. A few rules help guide the use of
/// Priorities: \li Mixing Normal and Low should be avoided, as this
/// increases the number of context switches on the GPU: prefer High and
/// Low.
/// \li Content that should render in the frame should be submitted on High
/// priority Queues, while Low priority is well suited for Compute content
/// that can run for multiple frames.
/// \li Queues that have equal priorities will almost surely run in order
/// of creation.
///
/// \param builder NVNqueueBuilder object to query.
///
/// \param priority The priority to set on the queue.
NVNAPI void NVNAPIENTRY
    nvnQueueBuilderSetQueuePriority(NVNqueueBuilder *builder,
                                    const NVNqueuePriority priority);

/// \brief Get the priority associated with this NVNqueueBuilder object.
///
/// \param builder NVNqueueBuilder object to query.
NVNAPI NVNqueuePriority NVNAPIENTRY
    nvnQueueBuilderGetQueuePriority(const NVNqueueBuilder *builder);

/// \brief Get the NVNdevice associated with this NVNqueueBuilder object.
///
/// \param builder NVNqueueBuilder object to query.
NVNAPI const NVNdevice * NVNAPIENTRY
    nvnQueueBuilderGetDevice(const NVNqueueBuilder *builder);

/// \brief Get queue flags associated with this NVNqueueBuilder object.
///
/// \param builder NVNqueueBuilder object to query.
NVNAPI int NVNAPIENTRY
    nvnQueueBuilderGetFlags(const NVNqueueBuilder *builder);

/// \brief Get the command memory size associated with this NVNqueueBuilder
/// object.
///
/// The function will return true if the command memory size was explicitly
/// set.
///
/// \param builder NVNqueueBuilder object to query.
///
/// \param[out] size Command memory size.
NVNAPI NVNboolean NVNAPIENTRY
    nvnQueueBuilderGetCommandMemorySize(const NVNqueueBuilder *builder,
                                        int *size);

/// \brief Get the compute memory size associated with this NVNqueueBuilder
/// object.
///
/// The function will return true if the compute memory size was explicitly
/// set.
///
/// \param builder NVNqueueBuilder object to query.
///
/// \param[out] size Compute memory size.
NVNAPI NVNboolean NVNAPIENTRY
    nvnQueueBuilderGetComputeMemorySize(const NVNqueueBuilder *builder,
                                        int *size);

/// \brief Get the control memory size associated with this NVNqueueBuilder
/// object.
///
/// The function will return true if the control memory size was explicitly
/// set.
///
/// \param builder NVNqueueBuilder object to query.
///
/// \param[out] size Control memory size.
NVNAPI NVNboolean NVNAPIENTRY
    nvnQueueBuilderGetControlMemorySize(const NVNqueueBuilder *builder,
                                        int *size);

/// \brief Get the command flush threshold associated with this
/// NVNqueueBuilder object.
///
/// The function will return true if the flush threshold was explicitly
/// set.
///
/// \param builder NVNqueueBuilder object to query.
///
/// \param[out] flushThreshold Flush threshold.
NVNAPI NVNboolean NVNAPIENTRY
    nvnQueueBuilderGetCommandFlushThreshold(const NVNqueueBuilder *builder,
                                            int *flushThreshold);

/// \brief Get the size of the memory associated with this NVNqueueBuilder
/// object.
///
/// \param builder NVNqueueBuilder object to query.
NVNAPI int NVNAPIENTRY
    nvnQueueBuilderGetMemorySize(const NVNqueueBuilder *builder);

/// \brief Get the memory associated with this NVNqueueBuilder object.
///
/// \param builder NVNqueueBuilder object to query.
NVNAPI void* NVNAPIENTRY
    nvnQueueBuilderGetMemory(const NVNqueueBuilder *builder);

/// \brief Initialize a NVNqueue object from a queue builder.
///
/// \param queue NVNqueue object to initialize.
///
/// \param builder NVNqueueBuilder object holding properties of the new
/// NVNqueue object.
NVNAPI NVNboolean NVNAPIENTRY
    nvnQueueInitialize(NVNqueue *queue,
                       const NVNqueueBuilder *builder);

/// \brief Finalize a NVNqueue object.
///
/// \note Finalizing a queue will wait for the completion of all commands
/// previously submitted to the queue, including those sent by the driver
/// during queue initialization.
///
/// \param queue NVNqueue object to finalize.
NVNAPI void NVNAPIENTRY
    nvnQueueFinalize(NVNqueue *queue);

/// \brief Sets the debug label string for a NVNqueue object.
///
/// Annotates a NVNqueue object with a descriptive text label. This label
/// may then be used by the debug layer, an external debugger, or a
/// profiler tool.
///
/// \param queue NVNqueue object to set debug label.
///
/// \param label Text string used to annotate the NVNqueue object.
NVNAPI void NVNAPIENTRY
    nvnQueueSetDebugLabel(NVNqueue *queue,
                          const char * label);

/// \brief Submit commands encapsulated in command buffers to a queue for
/// processing.
///
/// \param queue NVNqueue receiving the commands stored in the command
/// buffers.
///
/// \param numCommands Number of command buffers to send.
///
/// \param handles Array of numCommands handles for the command buffers to
/// send.
NVNAPI void NVNAPIENTRY
    nvnQueueSubmitCommands(NVNqueue *queue,
                           int numCommands,
                           const NVNcommandHandle *handles);

/// \brief Flush commands previously submitted to a queue for processing.
///
/// Commands submitted to NVNqueue objects may be batched up by the
/// implementation to avoid the overhead that would be involved in flushing
/// each command immediately after it is submitted.  Flush can be used to
/// manually flush commands that were previously batched up.
///
/// \param queue NVNqueue whose commands should be flushed for further
/// processing.
NVNAPI void NVNAPIENTRY
    nvnQueueFlush(NVNqueue *queue);

/// \brief Wait until all commands previously submitted to a queue have
/// completed execution.
///
/// This command will not return to the caller until the previously
/// submitted commands have completed.  To determine if submitted commands
/// have completed without blocking, applications can use NVNsync objects
/// and Sync::WaitSync().  Finish will automatically flush the previously
/// submitted commands as part of its operation.
///
/// \param queue NVNqueue whose commands should be waited on for
/// completion.
NVNAPI void NVNAPIENTRY
    nvnQueueFinish(NVNqueue *queue);

/// \brief Display the contents of a texture into the window.
///
/// This command presents the texture specified by _textureIndex_ in the
/// native window associated with _window_. It also triggers an implicit
/// call to nvnQueueFlush so that the texture is guaranteed to be shown on
/// the screen in finite time.
///
/// \param queue NVNqueue used for previous commands rendering to texture.
///
/// \param window The window where the texture should be presented.
///
/// \param textureIndex The index of the texture to be displayed on-screen.
/// The texture must be previously acquired from _window_ using
/// nvnWindowAcquireTexture.  nvnQueuePresentTexture transfers the
/// ownership of the texture back to the platform windowing system, and the
/// application must re-acquire it with nvnWindowAcquireTexture before
/// using it again.
NVNAPI void NVNAPIENTRY
    nvnQueuePresentTexture(NVNqueue *queue,
                           NVNwindow *window,
                           int textureIndex);

/// \brief Acquire a texture from the window.  Deprecated in favor of
/// nvnWindowAcquireTexture.
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
/// \param queue NVNqueue object that will be used to render to the
/// texture.
///
/// \param window NVNwindow object to acquire a texture from.
///
/// \param[out] textureIndex An index into the texture array that was
/// passed in when _window_ was initialized, which identifies the texture
/// that is now released from the platform windowing system to the
/// application.
NVN_PRE_DEPRECATED
NVNAPI NVNqueueAcquireTextureResult NVNAPIENTRY
    nvnQueueAcquireTexture(NVNqueue *queue,
                           NVNwindow *window,
                           int *textureIndex)
NVN_POST_DEPRECATED;

/// \brief Set the NVNdevice associated with a NVNwindowBuilder object.
///
/// \param builder NVNwindowBuilder object to update.
///
/// \param device NVNdevice owning NVNwindow objects created by the
/// NVNwindowBuilder.
NVNAPI void NVNAPIENTRY
    nvnWindowBuilderSetDevice(NVNwindowBuilder *builder,
                              NVNdevice *device);

/// \brief Set default state for the NVNwindowBuilder object.
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
///
/// \param builder NVNwindowBuilder object to reset.
NVNAPI void NVNAPIENTRY
    nvnWindowBuilderSetDefaults(NVNwindowBuilder *builder);

/// \brief Set the native window where the textures of this window will be
/// presented to.
///
/// On NX, the native window should be of type nn::vi::NativeWindowHandle.
///
/// On Windows, the native window (if provided) should be of type HWND and
/// applications must have set a pixel format using the Windows
/// `SetPixelFormat` API. The pixel format must be an RGBA format that
/// supports NVIDIA hardware acceleration and has the `PFD_DRAW_TO_WINDOW`,
/// `PFD_SUPPORT_OPENGL`, and `PFD_DOUBLEBUFFER` properties set. The window
/// class that was registered using the Windows `RegisterClass` function,
/// needs to have the CS_OWNDC flag set. If no native window is provided on
/// Windows, applications must bind a "donor" OpenGL context and drawable
/// using `wglMakeCurrent` before calling nvnQueuePresentTexture.
///
/// \param builder NVNwindowBuilder object to update.
///
/// \param nativeWindow NativeWindow where the textures of this window will
/// be presented to.
NVNAPI void NVNAPIENTRY
    nvnWindowBuilderSetNativeWindow(NVNwindowBuilder *builder,
                                    NVNnativeWindow nativeWindow);

/// \brief Sets the textures used for presenting to this window.
///
/// The textures must specify #NVN_TEXTURE_FLAGS_DISPLAY_BIT, and must not
/// be finalized before the NVNwindow is finalized.
///
/// For additional restrictions such as a list of supported formats, refer
/// to the NVN programming guide.
///
/// \param builder NVNwindowBuilder object to update.
///
/// \param numTextures Number of color textures to use.
/// Must be greater than or equal to 2 (the value of
/// #NVN_DEVICE_INFO_MIN_TEXTURES_PER_WINDOW).
/// Must be less than or equal to 4 (the value of
/// #NVN_DEVICE_INFO_MAX_TEXTURES_PER_WINDOW).
///
/// \param textures Array of numTextures pointers for color textures.
/// The pointer must remain valid until the window is initialized with
/// nvnWindowInitialize.
NVNAPI void NVNAPIENTRY
    nvnWindowBuilderSetTextures(NVNwindowBuilder *builder,
                                int numTextures,
                                NVNtexture * const *textures);

/// \brief Sets the present interval used when presenting to this window.
///
/// Sets the present interval for the window object. The present interval
/// specifies the minimum number of display refresh between frames when
/// presenting to a NVNwindow object.  Legal values are in the range
/// [0,MAX_PRESENT_INTERVAL]:
///
/// * 0:  Update the display without waiting for the next refresh.  This
/// will display frames as fast as possible but will likely result in
/// visual tearing.
/// * 1:  Update the display on the next refresh.  This limits the frame
/// rate to the display's refresh rate.
/// * _n_ (> 1):  Update the display on the next refresh, but no sooner
/// than _n_ refreshes since the previous update. This limits the frame
/// rate to 1 / _n_ of the display's refresh rate.
///
/// \note The production environment will not support a present interval of
/// 0. In that environment, setting a present interval of 0 will behave as
/// though 1 were specified.
///
/// \note On the Windows reference implementation, calling
/// SetPresentInterval with an interval different than 1 is only supported
/// if the application provides a native window to the NVNwindowBuilder.
///
/// \note On NX, the display refresh rate will always be 60Hz. On the
/// Windows reference implementation, the refresh rate depend on the
/// capabilities of the connected monitor and might be in some cases
/// different than 60Hz.
///
/// \param builder NVNwindowBuilder object to update.
///
/// \param presentInterval Present interval to use.
/// Must be greater than or equal to zero.
/// Must be less than or equal to 5 (the value of
/// #NVN_DEVICE_INFO_MAX_PRESENT_INTERVAL).
NVNAPI void NVNAPIENTRY
    nvnWindowBuilderSetPresentInterval(NVNwindowBuilder *builder,
                                       int presentInterval);

/// \brief Set the number of textures for presenting to this window.
///
/// \param builder NVNwindowBuilder object to update.
///
/// \param numActiveTextures Number of color textures to use. Must be less
/// than or equal the number of textures set by
/// nvnWindowBuilderSetTextures.
/// Must be greater than or equal to 2 (the value of
/// #NVN_DEVICE_INFO_MIN_TEXTURES_PER_WINDOW).
/// Must be less than or equal to 4 (the value of
/// #NVN_DEVICE_INFO_MAX_TEXTURES_PER_WINDOW).
NVNAPI void NVNAPIENTRY
    nvnWindowBuilderSetNumActiveTextures(NVNwindowBuilder *builder,
                                         int numActiveTextures);

/// \brief Query the device associated with this NVNwindowBuilder object.
///
/// \param builder NVNwindowBuilder object to query.
NVNAPI const NVNdevice * NVNAPIENTRY
    nvnWindowBuilderGetDevice(const NVNwindowBuilder *builder);

/// \brief Query the number of textures associated with this
/// NVNwindowBuilder object.
///
/// \param builder NVNwindowBuilder object to query.
NVNAPI int NVNAPIENTRY
    nvnWindowBuilderGetNumTextures(const NVNwindowBuilder *builder);

/// \brief Returns the texture with index textureId associated with this
/// NVNwindowBuilder object.
///
/// The texture array stored in the NVNwindowBuilder object is supposed to
/// remain valid until the window is initialized with nvnWindowInitialize.
/// Therefore GetTexture should be called before the window is initialized
/// to make sure it returns a valid pointer.
///
/// \param builder NVNwindowBuilder object to query.
///
/// \param textureId Index of the texture to be returned.
NVNAPI const NVNtexture * NVNAPIENTRY
    nvnWindowBuilderGetTexture(const NVNwindowBuilder *builder,
                               int textureId);

/// \brief Query the native window for a NVNwindowBuilder object.
///
/// \param builder NVNwindowBuilder object to query.
NVNAPI NVNnativeWindow NVNAPIENTRY
    nvnWindowBuilderGetNativeWindow(const NVNwindowBuilder *builder);

/// \brief Query the present interval for a NVNwindowBuilder object.
///
/// \param builder NVNwindowBuilder object to query.
NVNAPI int NVNAPIENTRY
    nvnWindowBuilderGetPresentInterval(const NVNwindowBuilder *builder);

/// \brief Query the number of active textures for presenting to this
/// window
///
/// \param builder NVNwindowBuilder object to query.
NVNAPI int NVNAPIENTRY
    nvnWindowBuilderGetNumActiveTextures(const NVNwindowBuilder *builder);

/// \brief Initialize a new window object from a window builder.
///
/// \param window NVNwindow object to initialize.
///
/// \param builder NVNwindowBuilder object holding properties of the new
/// NVNwindow object.
NVNAPI NVNboolean NVNAPIENTRY
    nvnWindowInitialize(NVNwindow *window,
                        const NVNwindowBuilder *builder);

/// \brief Finalize a NVNwindow object.
///
/// \param window NVNwindow object to finalize.
NVNAPI void NVNAPIENTRY
    nvnWindowFinalize(NVNwindow *window);

/// \brief Sets the debug label string for a NVNwindow object.
///
/// Annotates a NVNwindow object with a descriptive text label. This label
/// may then be used by the debug layer, an external debugger, or a
/// profiler tool.
///
/// \param window NVNwindow object to set debug label.
///
/// \param label Text string used to annotate the NVNwindow object.
NVNAPI void NVNAPIENTRY
    nvnWindowSetDebugLabel(NVNwindow *window,
                           const char * label);

/// \brief Acquire a texture from the window.
///
/// This function will block until the GPU has completed rendering the
/// previous frame to the acquired texture, but may return before the
/// acquired texture is ready for rendering the next frame.  The sync
/// object _textureAvailableSync_ can be used to determine when the
/// acquired texture can be rendered to.
///
/// The caller is responsible for ensuring that it doesn't render to the
/// acquired texture prematurely.  Passing _textureAvailableSync_ to
/// nvnQueueWaitSync ensures that any subsequent commands sent to a queue
/// are not processed by the GPU until the acquired texture is ready for
/// use.
///
/// Applications should also use _textureAvailableSync_ to match their
/// rendering rate to the presentation rate.  This can be done by calling
/// nvnSyncWait on _textureAvailableSync_ before calling
/// nvnQueuePresentTexture.
///
/// \note Waiting for _textureAvailableSync_ does not provide any
/// guarantees regarding GPU work completion.  For example, when double
/// buffering between textures A and B, GPU may still be rendering texture
/// B when A has become available for rendering (i.e.
/// _textureAvailableSync_ associated with acquiring texture A has been
/// signaled).
///
/// \param window NVNwindow object to acquire a texture from.
///
/// \param textureAvailableSync NVNsync object that will be signaled when
/// the texture is available for rendering a new frame.
/// If the texture has not been presented before with
/// nvnQueuePresentTexture, _textureAvailableSync_ will be signaled
/// immediately.  If nvnWindowAcquireTexture returns an error,
/// _textureAvailableSync_ will not be modified.
///
/// \param[out] textureIndex An index into the texture array that was
/// passed in when _window_ was initialized, which identifies the texture
/// that is now released from the platform windowing system to the
/// application.
NVNAPI NVNwindowAcquireTextureResult NVNAPIENTRY
    nvnWindowAcquireTexture(NVNwindow *window,
                            NVNsync *textureAvailableSync,
                            int *textureIndex);

/// \brief Query the native window for a NVNwindow object.
///
/// \param window NVNwindow object to query.
NVNAPI NVNnativeWindow NVNAPIENTRY
    nvnWindowGetNativeWindow(const NVNwindow *window);

/// \brief Query the present interval for a NVNwindow object.
///
/// \param window NVNwindow object to query.
NVNAPI int NVNAPIENTRY
    nvnWindowGetPresentInterval(const NVNwindow *window);

/// \brief Sets the present interval for a NVNwindow object.
///
/// Sets the present interval for the window object. The present interval
/// specifies the minimum number of display refresh between frames when
/// presenting to a NVNwindow object.  Legal values are in the range
/// [0,MAX_PRESENT_INTERVAL]:
///
/// * 0:  Update the display without waiting for the next refresh.  This
/// will display frames as fast as possible but will likely result in
/// visual tearing.
/// * 1:  Update the display on the next refresh.  This limits the frame
/// rate to the display's refresh rate.
/// * _n_ (> 1):  Update the display on the next refresh, but no sooner
/// than _n_ refreshes since the previous update. This limits the frame
/// rate to 1 / _n_ of the display's refresh rate.
///
/// \note The production environment will not support a present interval of
/// 0. In that environment, setting a present interval of 0 will behave as
/// though 1 were specified.
///
/// \note On the Windows reference implementation, calling
/// SetPresentInterval with an interval different than 1 is only supported
/// if the application provides a native window to the NVNwindowBuilder.
///
/// \note On NX, the display refresh rate will always be 60Hz. On the
/// Windows reference implementation, the refresh rate depend on the
/// capabilities of the connected monitor and might be in some cases
/// different than 60Hz.
///
/// \param window NVNwindow object.
///
/// \param presentInterval Present interval to use.
/// Must be greater than or equal to zero.
/// Must be less than or equal to 5 (the value of
/// #NVN_DEVICE_INFO_MAX_PRESENT_INTERVAL).
NVNAPI void NVNAPIENTRY
    nvnWindowSetPresentInterval(NVNwindow *window,
                                int presentInterval);

/// \brief Defines the rectangle inside the window texture that gets
/// displayed.
///
/// This method sets the rectangle inside a texture that gets displayed. If
/// the rectangle size does not match the window size, the texture
/// rectangle gets scaled to fit the window size. Passing a width or height
/// of zero will reset the crop rectangle to match the dimension of the
/// window texture.
///
/// \param window NVNwindow object.
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
NVNAPI void NVNAPIENTRY
    nvnWindowSetCrop(NVNwindow *window,
                     int x,
                     int y,
                     int w,
                     int h);

/// \brief Returns the dimensions of the crop rectangle that defines the
/// region inside the window texture which gets displayed.
///
/// \param window NVNwindow object to query.
///
/// \param[out] crop The crop rectangle that is used for presenting.
NVNAPI void NVNAPIENTRY
    nvnWindowGetCrop(const NVNwindow *window,
                     NVNrectangle *crop);

/// \brief Set the number of textures for presenting to this window.
///
/// SetNumActiveTextures takes affect after numTextures - 1 frames in
/// latest case.
/// \warning This function must not be called while the app is holding any
/// texture which is before presented to the window after acquiring it.
///
/// \param window NVNwindow object.
///
/// \param numActiveTextures Number of color textures to use. Must be less
/// than or equal to the number of textures set by
/// nvnWindowBuilderSetTextures.
/// Must be greater than or equal to 2 (the value of
/// #NVN_DEVICE_INFO_MIN_TEXTURES_PER_WINDOW).
/// Must be less than or equal to 4 (the value of
/// #NVN_DEVICE_INFO_MAX_TEXTURES_PER_WINDOW).
NVNAPI void NVNAPIENTRY
    nvnWindowSetNumActiveTextures(NVNwindow *window,
                                  int numActiveTextures);

/// \brief Query the number of active textures for presenting to this
/// window.
///
/// This value reflects the value set by nvnWindowSetNumActiveTextures
/// immediately after calling the function but the actual change of the
/// frame buffer behavior may take more time up to numTextures - 1 frames.
///
/// \param window NVNwindow object to query.
NVNAPI int NVNAPIENTRY
    nvnWindowGetNumActiveTextures(const NVNwindow *window);

/// \brief Query the maximum available number of textures for presenting to
/// this window.
///
/// This value is the same number of preallocated textures by
/// nvnWindowBuilderSetTextures
///
/// \param window NVNwindow object to query.
NVNAPI int NVNAPIENTRY
    nvnWindowGetNumTextures(const NVNwindow *window);

/// \brief Initialize a NVNprogram object for the specified device.
///
/// \param program NVNprogram object to initialize.
///
/// \param device NVNdevice owning the program.
NVNAPI NVNboolean NVNAPIENTRY
    nvnProgramInitialize(NVNprogram *program,
                         NVNdevice *device);

/// \brief Finalize a NVNprogram object.
///
/// \param program NVNprogram object to finalize.
NVNAPI void NVNAPIENTRY
    nvnProgramFinalize(NVNprogram *program);

/// \brief Sets the debug label string for a NVNprogram object.
///
/// Annotates a NVNprogram object with a descriptive text label. This label
/// may then be used by the debug layer, an external debugger, or a
/// profiler tool.
///
/// \param program NVNprogram object to set debug label.
///
/// \param label Text string used to annotate the NVNprogram object.
NVNAPI void NVNAPIENTRY
    nvnProgramSetDebugLabel(NVNprogram *program,
                            const char * label);

/// \brief Initialize a program object to use pre-compiled GPU code from
/// one or multiple shaders
///
/// \param program NVNprogram object to set up.
///
/// \param count Number of GPU programs to load in.
///
/// \param stageData GPU control information to help set up
NVNAPI NVNboolean NVNAPIENTRY
    nvnProgramSetShaders(NVNprogram *program,
                         int count,
                         const NVNshaderData *stageData);

/// \brief Initialize a program object to use pre-compiled GPU code from
/// one or multiple shaders and optionally replaces the debug data hashes
///
/// \param program NVNprogram object to set up.
///
/// \param count Number of GPU programs to load in.
///
/// \param stageDataExt GPU control information to help set up the program
/// as well as replacement debug information
NVNAPI NVNboolean NVNAPIENTRY
    nvnProgramSetShadersExt(NVNprogram *program,
                            int count,
                            const NVNshaderDataExt *stageDataExt);

/// \brief Overrides how the program's fragment shader is evaluated.
///
/// By default a fragment shader is evaluated per sample only if a keyword
/// such as gl_SamplePosition is used. This function can be used to force a
/// specific sample shading mode.
///
/// \note: This function must be called before calling SetShaders otherwise
/// it will have no effect.
///
/// \param program NVNprogram object to modify.
///
/// \param shadingMode Sample shading mode to override.
NVNAPI void NVNAPIENTRY
    nvnProgramSetSampleShading(NVNprogram *program,
                               NVNsampleShadingMode shadingMode);

/// \brief Set the NVNdevice associated with a NVNmemoryPoolBuilder object.
///
/// \param builder NVNmemoryPoolBuilder object to update.
///
/// \param device NVNdevice owning NVNmemoryPool objects created by the
/// NVNmemoryPoolBuilder.
NVNAPI void NVNAPIENTRY
    nvnMemoryPoolBuilderSetDevice(NVNmemoryPoolBuilder *builder,
                                  NVNdevice *device);

/// \brief Set default state for the NVNmemoryPoolBuilder object.
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
///
/// \param builder NVNmemoryPoolBuilder object to reset.
NVNAPI void NVNAPIENTRY
    nvnMemoryPoolBuilderSetDefaults(NVNmemoryPoolBuilder *builder);

/// \brief Provides memory to be used by memory pools initialized using a
/// NVNmemoryPoolBuilder.
///
/// When a memory pool is initialized using a memory pool builder, _size_
/// bytes of memory pointed to by _memory_ will be owned by the memory pool
/// until the memory pool is finalized.  Applications must not access
/// _memory_ directly after calling nvnMemoryPoolInitialize.  For memory
/// pools created with #NVN_MEMORY_POOL_FLAGS_CPU_CACHED_BIT or
/// #NVN_MEMORY_POOL_FLAGS_CPU_UNCACHED_BIT, applications can retrieve a
/// CPU pointer to the pool using nvnMemoryPoolMap that can be used until
/// the pool is finalized.  The memory provided by _memory_ can be re-used
/// or freed by the application after the pool is finalized using
/// nvnMemoryPoolFinalize.
///
/// Virtual memory pools (#NVN_MEMORY_POOL_FLAGS_VIRTUAL_BIT) do not own
/// any storage and must be initialized using a NULL _memory_ parameter.
///
/// On NX, non-virtual memory pools must be initialized with
/// application-provided memory.  Initialization for non-virtual memory
/// pools will fail if a builder with a NULL _memory_ parameter is
/// provided.
///
/// On Windows, NVN allocates separate memory for non-virtual memory pools
/// and does not take ownership of the application-provided memory.  If
/// _memory_ is NULL, the contents of the allocated pool memory will be
/// uninitialized.  If _memory_ is non-NULL, the allocated pool memory will
/// be initialized by copying the _size_ bytes pointed to by _memory_.
///
/// \param builder NVNmemoryPoolBuilder object to update.
///
/// \param[in,out] memory Storage used for the memory pool.
/// Must be aligned to a multiple of 4KB
/// (NVN_MEMORY_POOL_STORAGE_ALIGNMENT).
///
/// \param size Size (in bytes) of the memory pool.
/// Must be aligned to a multiple of 4KB
/// (NVN_MEMORY_POOL_STORAGE_GRANULARITY).
/// Must be greater than zero.
NVNAPI void NVNAPIENTRY
    nvnMemoryPoolBuilderSetStorage(NVNmemoryPoolBuilder *builder,
                                   void *memory,
                                   size_t size);

/// \brief Set properties of NVNmemoryPool objects created from the
/// builder.
///
/// \param builder NVNmemoryPoolBuilder object to update.
///
/// \param flags Bitfield specifying properties of the memory pool to
/// initialize. See #NVNmemoryPoolFlags
NVNAPI void NVNAPIENTRY
    nvnMemoryPoolBuilderSetFlags(NVNmemoryPoolBuilder *builder,
                                 int flags);

/// \brief Query the device associated with a NVNmemoryPoolBuilder.
///
/// \param builder NVNmemoryPoolBuilder object to query.
NVNAPI const NVNdevice * NVNAPIENTRY
    nvnMemoryPoolBuilderGetDevice(const NVNmemoryPoolBuilder *builder);

/// \brief Query the memory allocation associated with a
/// NVNmemoryPoolBuilder.
///
/// \param builder NVNmemoryPoolBuilder object to query.
NVNAPI void* NVNAPIENTRY
    nvnMemoryPoolBuilderGetMemory(const NVNmemoryPoolBuilder *builder);

/// \brief Query the size associated with a NVNmemoryPoolBuilder.
///
/// \param builder NVNmemoryPoolBuilder object to query.
NVNAPI size_t NVNAPIENTRY
    nvnMemoryPoolBuilderGetSize(const NVNmemoryPoolBuilder *builder);

/// \brief Query the flags associated with a NVNmemoryPoolBuilder.
///
/// \param builder NVNmemoryPoolBuilder object to query.
NVNAPI NVNmemoryPoolFlags NVNAPIENTRY
    nvnMemoryPoolBuilderGetFlags(const NVNmemoryPoolBuilder *builder);

/// \brief Initialize a NVNmemoryPool object using a NVNmemoryPoolBuilder
/// object.
///
/// \note On Windows, nvnMemoryPoolInitialize may return NVN_FALSE if video
/// memory on the discrete GPU is exhausted. Reducing the size of the
/// memory pool and/or shutting down other applications that consume video
/// memory may resolve this error.
///
/// \param pool NVNmemoryPool object to initialize.
///
/// \param builder NVNmemoryPoolBuilder object holding properties of the
/// new NVNmemoryPool object.
NVNAPI NVNboolean NVNAPIENTRY
    nvnMemoryPoolInitialize(NVNmemoryPool *pool,
                            const NVNmemoryPoolBuilder *builder);

/// \brief Sets the debug label string for a NVNmemoryPool object.
///
/// Annotates a NVNmemoryPool object with a descriptive text label. This
/// label may then be used by the debug layer, an external debugger, or a
/// profiler tool.
///
/// \param pool NVNmemoryPool object to set debug label.
///
/// \param label Text string used to annotate the NVNmemoryPool object.
NVNAPI void NVNAPIENTRY
    nvnMemoryPoolSetDebugLabel(NVNmemoryPool *pool,
                               const char * label);

/// \brief Finalize a NVNmemoryPool object.
///
/// \param pool Memory pool to finalize.
NVNAPI void NVNAPIENTRY
    nvnMemoryPoolFinalize(NVNmemoryPool *pool);

/// \brief Returns a CPU pointer to the memory owned by the NVNmemoryPool
/// object.
///
/// If the memory pool does not support CPU access
/// (#NVN_MEMORY_POOL_FLAGS_CPU_NO_ACCESS_BIT), NULL is returned.
///
/// \param pool Memory pool object being accessed.
NVNAPI void* NVNAPIENTRY
    nvnMemoryPoolMap(const NVNmemoryPool *pool);

/// \brief Flush previous CPU writes to a range of the memory pool to be
/// visible to the GPU.
///
/// \note On NX, calling nn::os::FlushDataCache on an equivalent memory
/// range will have the same effect as calling this function.
///
/// \param pool NVNmemoryPool object containing the range to flush.
///
/// \param offset Offset (in bytes) of the first byte of the memory pool
/// range to flush.
///
/// \param size Size (in bytes) of the memory pool range to flush.
NVNAPI void NVNAPIENTRY
    nvnMemoryPoolFlushMappedRange(const NVNmemoryPool *pool,
                                  ptrdiff_t offset,
                                  size_t size);

/// \brief Invalidate CPU caches for a range of the memory pool that may
/// have been written by the GPU.
///
/// \note On NX, calling nn::os::FlushDataCache on an equivalent memory
/// range will have the same effect as calling this function.
///
/// \param pool NVNmemoryPool object containing the range to invalidate.
///
/// \param offset Offset (in bytes) of the first byte of the memory pool
/// range to invalidate.
///
/// \param size Size (in bytes) of the memory pool range to invalidate.
NVNAPI void NVNAPIENTRY
    nvnMemoryPoolInvalidateMappedRange(const NVNmemoryPool *pool,
                                       ptrdiff_t offset,
                                       size_t size);

/// \brief Query the GPU address of the data store of a memory pool object.
///
/// All buffer resources stored in a memory pool are accessed using a
/// single GPU address mapping created for the entire pool.  This function
/// returns the GPU address of the first byte of this mapping.
///
/// \note The address for a buffer object can be obtained by adding the
/// buffer address of the memory pool owning the buffer object and the
/// offset of the buffer object within its pool.
///
/// \param pool NVNmemoryPool object used for the address query.
NVNAPI NVNbufferAddress NVNAPIENTRY
    nvnMemoryPoolGetBufferAddress(const NVNmemoryPool *pool);

/// \brief Map memory pages from physical memory pools into a virtual
/// memory pool.
///
/// This command can be used to map memory from a physical memory pool (one
/// using #NVN_MEMORY_POOL_FLAGS_PHYSICAL_BIT) into a virtual memory pool
/// (one using #NVN_MEMORY_POOL_FLAGS_VIRTUAL_BIT).  Each individual
/// mapping request will map a contiguous set of pages in the physical pool
/// into a contiguous range of the virtual pool, using a specified storage
/// class for the virtual memory mapping.  Applications can specify one or
/// multiple mapping requests in a single call; performing multiple
/// mappings in a single call can reduce CPU overhead required to perform
/// the mappings.
///
/// Returns NVN_TRUE if all virtual mapping requests were successfully
/// processed.  If any mapping request fails, NVN_FALSE will be returned.
/// If NVN_FALSE is returned, some of the virtual mapping requests may have
/// been fully or partially satisfied prior to the failure.  There is no
/// guarantee that the mappings in _virtualPool_ are unchanged if this
/// command fails.  Virtual mappings may fail if the graphics firmware has
/// insufficient memory to set up the new mapping.
///
/// \param virtualPool Virtual memory pool to update.
///
/// \param numRequests Number of mapping requests to perform.
///
/// \param requests Array of mapping requests to perform.
NVNAPI NVNboolean NVNAPIENTRY
    nvnMemoryPoolMapVirtual(NVNmemoryPool *virtualPool,
                            int numRequests,
                            const NVNmappingRequest *requests);

/// \brief Query the size associated with a NVNmemoryPool object.
///
/// \param pool NVNmemoryPool object to query.
NVNAPI size_t NVNAPIENTRY
    nvnMemoryPoolGetSize(const NVNmemoryPool *pool);

/// \brief Query the flags associated with a NVNmemoryPool object.
///
/// \param pool NVNmemoryPool object to query.
NVNAPI NVNmemoryPoolFlags NVNAPIENTRY
    nvnMemoryPoolGetFlags(const NVNmemoryPool *pool);

/// \brief Initialize a NVNtexturePool object.
///
/// The texture pool is a contiguous block of memory where texture
/// descriptors are written. NVNtexture descriptors hold information about
/// textures and are used by the GPU when using textures in rendering
/// operations. NVNtexture descriptors are written into the pool using
/// RegisterTexture. The pool itself must be set to the queue before
/// rendering with any individual textures. A number of entries at the
/// beginning of the pool are reserved for use by the driver. The number of
/// reserved entries can be found by querying the device property
/// RESERVED_TEXTURE_DESCRIPTORS. The size of each entry can be found by
/// querying TEXTURE_DESCRIPTOR_SIZE.
///
/// \param texturePool NVNtexturePool object to initialize.
///
/// \param memoryPool NVNmemoryPool object providing storage for the
/// texture pool. The memory pool must be CPU-accessible (except on the
/// Windows reference implementation).
///
/// \param offset Offset (in bytes) of the first byte of memory pool
/// storage used by the texture pool.
/// Must be a multiple of 32 (the value of
/// #NVN_DEVICE_INFO_TEXTURE_DESCRIPTOR_SIZE).
///
/// \param numDescriptors The number of texture descriptors that can be
/// stored within the texture pool, including texture descriptors reserved
/// to the driver.
/// Must be greater than or equal to 256 (the value of
/// #NVN_DEVICE_INFO_RESERVED_TEXTURE_DESCRIPTORS).
/// Must be less than or equal to 1048576 (the value of
/// #NVN_DEVICE_INFO_MAX_TEXTURE_POOL_SIZE).
NVNAPI NVNboolean NVNAPIENTRY
    nvnTexturePoolInitialize(NVNtexturePool *texturePool,
                             const NVNmemoryPool *memoryPool,
                             ptrdiff_t offset,
                             int numDescriptors);

/// \brief Sets the debug label string for a NVNtexturePool object.
///
/// Annotates a NVNtexturePool object with a descriptive text label. This
/// label may then be used by the debug layer, an external debugger, or a
/// profiler tool.
///
/// \param pool NVNtexturePool object to set debug label.
///
/// \param label Text string used to annotate the NVNtexturePool object.
NVNAPI void NVNAPIENTRY
    nvnTexturePoolSetDebugLabel(NVNtexturePool *pool,
                                const char * label);

/// \brief Finalize a NVNtexturePool object.
///
/// \param pool NVNtexture pool to finalize.
NVNAPI void NVNAPIENTRY
    nvnTexturePoolFinalize(NVNtexturePool *pool);

/// \brief Register a texture descriptor in a texture pool for use via
/// texture handles.
///
/// The identifier *id* specifies an index into *texturePool* where the
/// descriptor is written.  The texture pool index is used together with a
/// sampler pool index to generate a texture handle used for subsequent
/// texture binding operations or bindless texture accesses.
/// \note If a texture needs to be used for both texture and image
/// accesses, it must be registered separately for both use cases unless
/// the device property
/// #NVN_DEVICE_INFO_SUPPORTS_IMAGES_USING_TEXTURE_HANDLES is true.
/// \note In case of the NVN Windows reference implementation, if the
/// texture is initialized with an ASTC format, any format specified in the
/// _view_ parameter will be ignored. For the detail, please refer
/// \ref nvn_guide_formats_restrictions_on_Windows
/// "Formats Restrictions on the NVN Windows Reference Implementation"
/// section of the Programming Guide.
///
/// \param texturePool The texture pool owning the texture descriptor.
///
/// \param id Offset (in descriptors) in the texture pool used to store the
/// texture descriptor.
/// Must be greater than or equal to 256 (the value of
/// #NVN_DEVICE_INFO_RESERVED_TEXTURE_DESCRIPTORS).
/// Must be less than or equal to 1048576 (the value of
/// #NVN_DEVICE_INFO_MAX_TEXTURE_POOL_SIZE).
///
/// \param texture NVNtexture object to register.
///
/// \param view View of the texture object to register.
NVNAPI void NVNAPIENTRY
    nvnTexturePoolRegisterTexture(const NVNtexturePool *texturePool,
                                  int id,
                                  const NVNtexture *texture,
                                  const NVNtextureView *view);

/// \brief Register an image descriptor in a texture pool for use via image
/// loads and stores.
///
/// The identifier *id* specifies an index into *texturePool* where the
/// descriptor is written.  The texture pool index is used to generate an
/// image handle used for subsequent image binding operations or bindless
/// image accesses.
///
/// \note If a texture needs to be used for both texture and image
/// accesses, it must be registered separately for both use cases unless
/// the device property
/// #NVN_DEVICE_INFO_SUPPORTS_IMAGES_USING_TEXTURE_HANDLES is true.
///
/// \param texturePool The texture pool owning the image descriptor.
///
/// \param id Offset (in descriptors) in the texture pool used to store the
/// image descriptor.
/// Must be greater than or equal to 256 (the value of
/// #NVN_DEVICE_INFO_RESERVED_TEXTURE_DESCRIPTORS).
/// Must be less than or equal to 1048576 (the value of
/// #NVN_DEVICE_INFO_MAX_TEXTURE_POOL_SIZE).
///
/// \param texture NVNtexture object to register.
///
/// \param view View of the texture object to register.
NVNAPI void NVNAPIENTRY
    nvnTexturePoolRegisterImage(const NVNtexturePool *texturePool,
                                int id,
                                const NVNtexture *texture,
                                const NVNtextureView *view);

/// \brief Query the memory pool used by a NVNtexturePool object.
///
/// \param pool NVNtexturePool object to query.
NVNAPI const NVNmemoryPool * NVNAPIENTRY
    nvnTexturePoolGetMemoryPool(const NVNtexturePool *pool);

/// \brief Query the memory pool offset used by a NVNtexturePool object.
///
/// \param pool NVNtexturePool object to query.
NVNAPI ptrdiff_t NVNAPIENTRY
    nvnTexturePoolGetMemoryOffset(const NVNtexturePool *pool);

/// \brief Query the number of descriptors in the memory of a
/// NVNtexturePool object.
///
/// \param pool NVNtexturePool object to query.
NVNAPI int NVNAPIENTRY
    nvnTexturePoolGetSize(const NVNtexturePool *pool);

/// \brief Initialize a NVNsamplerPool object.
///
/// The sampler pool is a contiguous block of memory where sampler
/// descriptors are written. NVNsampler descriptors hold information about
/// textures and are used by the GPU when using samplers in rendering
/// operations. NVNsampler descriptors are written into the pool using the
/// RegisterSampler or RegisterSamplerBuilder methods. The pool itself must
/// be set to the queue before rendering with any individual textures. A
/// number of entries at the beginning of the pool are reserved for use by
/// the driver. The number of reserved entries can be found by querying the
/// device property RESERVED_SAMPLER_DESCRIPTORS. The size of each entry
/// can be found by querying SAMPLER_DESCRIPTOR_SIZE.
///
/// \param samplerPool NVNsamplerPool object to initialize.
///
/// \param memoryPool NVNmemoryPool object providing storage for the
/// sampler pool. The memory pool must be CPU-accessible (except on the
/// Windows reference implementation).
///
/// \param offset Offset (in bytes) of the first byte of memory pool
/// storage used by the sampler pool.
/// Must be a multiple of 32 (the value of
/// #NVN_DEVICE_INFO_SAMPLER_DESCRIPTOR_SIZE).
///
/// \param numDescriptors The number of sampler descriptors that can be
/// stored within the sampler pool, including samplers reserved to the
/// driver.
/// Must be greater than or equal to 256 (the value of
/// #NVN_DEVICE_INFO_RESERVED_SAMPLER_DESCRIPTORS).
/// Must be less than or equal to 4096 (the value of
/// #NVN_DEVICE_INFO_MAX_SAMPLER_POOL_SIZE).
NVNAPI NVNboolean NVNAPIENTRY
    nvnSamplerPoolInitialize(NVNsamplerPool *samplerPool,
                             const NVNmemoryPool *memoryPool,
                             ptrdiff_t offset,
                             int numDescriptors);

/// \brief Sets the debug label string for a NVNsamplerPool object.
///
/// Annotates a NVNsamplerPool object with a descriptive text label. This
/// label may then be used by the debug layer, an external debugger, or a
/// profiler tool.
///
/// \param pool NVNsamplerPool object to set debug label.
///
/// \param label Text string used to annotate the NVNsamplerPool object.
NVNAPI void NVNAPIENTRY
    nvnSamplerPoolSetDebugLabel(NVNsamplerPool *pool,
                                const char * label);

/// \brief Finalize a NVNsamplerPool object.
///
/// \param pool NVNsampler pool to finalize.
NVNAPI void NVNAPIENTRY
    nvnSamplerPoolFinalize(NVNsamplerPool *pool);

/// \brief Register a sampler descriptor in a sampler pool for use via
/// texture handles, using a NVNsampler object.
///
/// The identifier *id* specifies an index into *samplerPool* where the
/// descriptor is written.  The sampler pool index is used together with a
/// texture pool index to generate a texture handle used for subsequent
/// texture binding operations or bindless texture accesses.
///
/// \note The nvnSamplerPoolRegisterSamplerBuilder method performs
/// identical sampler descriptor registration directly from sampler builder
/// state, without a NVNsampler object.
///
/// \param samplerPool The sampler pool owning the sampler descriptor.
///
/// \param id Offset (in descriptors) in the sampler pool used to store the
/// sampler descriptor.
/// Must be greater than or equal to 256 (the value of
/// #NVN_DEVICE_INFO_RESERVED_SAMPLER_DESCRIPTORS).
/// Must be less than or equal to 4096 (the value of
/// #NVN_DEVICE_INFO_MAX_SAMPLER_POOL_SIZE).
///
/// \param sampler NVNsampler object to register.
NVNAPI void NVNAPIENTRY
    nvnSamplerPoolRegisterSampler(const NVNsamplerPool *samplerPool,
                                  int id,
                                  const NVNsampler *sampler);

/// \brief Register a sampler descriptor in a sampler pool for use via
/// texture handles, using NVNsamplerBuilder state.
///
/// The identifier *id* specifies an index into *samplerPool* where the
/// descriptor is written.  The sampler pool index is used together with a
/// texture pool index to generate a texture handle used for subsequent
/// texture binding operations or bindless texture accesses.
///
/// \note The nvnSamplerPoolRegisterSampler method performs identical
/// sampler descriptor registration using a NVNsampler object.
///
/// \param samplerPool The sampler pool owning the sampler descriptor.
///
/// \param id Offset (in descriptors) in the sampler pool used to store the
/// sampler descriptor.
/// Must be greater than or equal to 256 (the value of
/// #NVN_DEVICE_INFO_RESERVED_SAMPLER_DESCRIPTORS).
/// Must be less than or equal to 4096 (the value of
/// #NVN_DEVICE_INFO_MAX_SAMPLER_POOL_SIZE).
///
/// \param builder NVNsampler builder state to register.
NVNAPI void NVNAPIENTRY
    nvnSamplerPoolRegisterSamplerBuilder(const NVNsamplerPool *samplerPool,
                                         int id,
                                         const NVNsamplerBuilder *builder);

/// \brief Query the memory pool used by a NVNsamplerPool object.
///
/// \param pool NVNsamplerPool object to query.
NVNAPI const NVNmemoryPool * NVNAPIENTRY
    nvnSamplerPoolGetMemoryPool(const NVNsamplerPool *pool);

/// \brief Query the memory pool offset used by a NVNsamplerPool object.
///
/// \param pool NVNsamplerPool object to query.
NVNAPI ptrdiff_t NVNAPIENTRY
    nvnSamplerPoolGetMemoryOffset(const NVNsamplerPool *pool);

/// \brief Query the number of descriptors in the memory of a
/// NVNsamplerPool object.
///
/// \param pool NVNsamplerPool object to query.
NVNAPI int NVNAPIENTRY
    nvnSamplerPoolGetSize(const NVNsamplerPool *pool);

/// \brief Set the NVNdevice associated with a NVNbufferBuilder object.
///
/// \param builder NVNbufferBuilder object to update.
///
/// \param device NVNdevice owning NVNbuffer objects created by the
/// NVNbufferBuilder.
NVNAPI void NVNAPIENTRY
    nvnBufferBuilderSetDevice(NVNbufferBuilder *builder,
                              NVNdevice *device);

/// \brief Set default state for the NVNbufferBuilder object.
///
/// This method does not modify the device associated with the builder,
/// which is programmed by SetDevice.
///
/// \note This is semantically equivalent to calling
///
/// \code{.c}
///   BufferBuilder::SetStorage(NULL, 0, 0);
/// \endcode
///
/// \param builder NVNbufferBuilder object to reset.
NVNAPI void NVNAPIENTRY
    nvnBufferBuilderSetDefaults(NVNbufferBuilder *builder);

/// \brief Set the range of a NVNmemoryPool used for a NVNbuffer object
/// created from the NVNbufferBuilder.
///
/// \param builder NVNbufferBuilder object to update.
///
/// \param pool NVNmemoryPool object providing storage for the new
/// NVNbuffer object.
///
/// \param offset Offset (in bytes) of the first byte of pool storage used
/// by the new NVNbuffer object.
///
/// \param size Size (in bytes) of the new buffer object.
NVNAPI void NVNAPIENTRY
    nvnBufferBuilderSetStorage(NVNbufferBuilder *builder,
                               NVNmemoryPool *pool,
                               ptrdiff_t offset,
                               size_t size);

/// \brief Query the device associated with this NVNbufferBuilder object.
///
/// \param builder NVNbufferBuilder object to query.
NVNAPI const NVNdevice * NVNAPIENTRY
    nvnBufferBuilderGetDevice(const NVNbufferBuilder *builder);

/// \brief Query the memory pool used by a NVNbufferBuilder object.
///
/// \param builder NVNbufferBuilder object to query.
NVNAPI NVNmemoryPool * NVNAPIENTRY
    nvnBufferBuilderGetMemoryPool(const NVNbufferBuilder *builder);

/// \brief Query the memory pool offset used by a NVNbufferBuilder object.
///
/// \param builder NVNbufferBuilder object to query.
NVNAPI ptrdiff_t NVNAPIENTRY
    nvnBufferBuilderGetMemoryOffset(const NVNbufferBuilder *builder);

/// \brief Query the size (in bytes) of the storage in a NVNbufferBuilder
/// object.
///
/// \param builder NVNbufferBuilder object to query.
NVNAPI size_t NVNAPIENTRY
    nvnBufferBuilderGetSize(const NVNbufferBuilder *builder);

/// \brief Initialize a new buffer object from a buffer builder.
///
/// NVNbuffer objects are created using the NVNbufferBuilder object, with
/// storage provided by a memory pool or a previously existing NVNbuffer
/// object.
///
/// \param buffer NVNbuffer object to initialize.
///
/// \param builder NVNbufferBuilder object holding properties of the new
/// NVNbuffer object.
NVNAPI NVNboolean NVNAPIENTRY
    nvnBufferInitialize(NVNbuffer *buffer,
                        const NVNbufferBuilder *builder);

/// \brief Sets the debug label string for a NVNbuffer object.
///
/// Annotates a NVNbuffer object with a descriptive text label. This label
/// may then be used by the debug layer, an external debugger, or a
/// profiler tool.
///
/// \param buffer NVNbuffer object to set debug label.
///
/// \param label Text string used to annotate the NVNbuffer object.
NVNAPI void NVNAPIENTRY
    nvnBufferSetDebugLabel(NVNbuffer *buffer,
                           const char * label);

/// \brief Finalize a NVNbuffer object.
///
/// \param buffer NVNbuffer object to finalize.
NVNAPI void NVNAPIENTRY
    nvnBufferFinalize(NVNbuffer *buffer);

/// \brief Returns a CPU pointer to the memory of a previously created
/// NVNbuffer object.
///
/// If the memory pool used by the buffer object does not support CPU
/// access (#NVN_MEMORY_POOL_FLAGS_CPU_NO_ACCESS_BIT), NULL is returned.
///
/// \param buffer NVNbuffer object to map.
NVNAPI void* NVNAPIENTRY
    nvnBufferMap(const NVNbuffer *buffer);

/// \brief Query the GPU address of the data store of a buffer object.
///
/// \note The address for a buffer object can also be obtained by adding
/// the buffer address of the memory pool owning the buffer object and the
/// offset of the buffer object within its pool.
///
/// \param buffer NVNbuffer object used for the address query.
NVNAPI NVNbufferAddress NVNAPIENTRY
    nvnBufferGetAddress(const NVNbuffer *buffer);

/// \brief Flush previous CPU writes to a range of a buffer to be visible
/// to the GPU.
///
/// \note On NX, calling nn::os::FlushDataCache on an equivalent memory
/// range will have the same effect as calling this function.
///
/// \param buffer NVNbuffer object containing the range to flush.
///
/// \param offset Offset (in bytes) of the first byte of the buffer range
/// to flush.
///
/// \param size Size (in bytes) of the buffer range to flush.
NVNAPI void NVNAPIENTRY
    nvnBufferFlushMappedRange(const NVNbuffer *buffer,
                              ptrdiff_t offset,
                              size_t size);

/// \brief Invalidate CPU caches for a range of a buffer that may have been
/// written by the GPU.
///
/// \note On NX, calling nn::os::FlushDataCache on an equivalent memory
/// range will have the same effect as calling this function.
///
/// \param buffer NVNbuffer object containing the range to invalidate.
///
/// \param offset Offset (in bytes) of the first byte of the buffer to
/// invalidate.
///
/// \param size Size (in bytes) of the buffer to invalidate.
NVNAPI void NVNAPIENTRY
    nvnBufferInvalidateMappedRange(const NVNbuffer *buffer,
                                   ptrdiff_t offset,
                                   size_t size);

/// \brief Query the memory pool used by a NVNbuffer object.
///
/// \param buffer NVNbuffer object to query.
NVNAPI NVNmemoryPool * NVNAPIENTRY
    nvnBufferGetMemoryPool(const NVNbuffer *buffer);

/// \brief Query the memory pool offset used by a NVNbuffer object.
///
/// \param buffer NVNbuffer object to query.
NVNAPI ptrdiff_t NVNAPIENTRY
    nvnBufferGetMemoryOffset(const NVNbuffer *buffer);

/// \brief Query the size (in bytes) of the storage of a NVNbuffer object.
///
/// \param buffer NVNbuffer object to query.
NVNAPI size_t NVNAPIENTRY
    nvnBufferGetSize(const NVNbuffer *buffer);

/// \brief Returns the debug layer unique ID of this buffer. Returns 0 if
/// debug layer disabled.
///
/// \param buffer NVNbuffer object to query.
NVNAPI uint64_t NVNAPIENTRY
    nvnBufferGetDebugID(const NVNbuffer *buffer);

/// \brief Set the NVNdevice associated with a NVNtextureBuilder object.
///
/// \param builder NVNtextureBuilder object to update.
///
/// \param device NVNdevice owning NVNtexture objects created by the
/// NVNtextureBuilder.
NVNAPI void NVNAPIENTRY
    nvnTextureBuilderSetDevice(NVNtextureBuilder *builder,
                               NVNdevice *device);

/// \brief Set default state for the NVNtextureBuilder object.
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
///
/// \param builder NVNtextureBuilder object to reset.
NVNAPI void NVNAPIENTRY
    nvnTextureBuilderSetDefaults(NVNtextureBuilder *builder);

/// \brief Set flags indentifying special properties of NVNtexture objects
/// created from the NVNtextureBuilder.
///
/// \param builder NVNtextureBuilder object to update.
///
/// \param flags #NVNtextureFlags value specifying special properties for
/// created NVNtexture objects.
NVNAPI void NVNAPIENTRY
    nvnTextureBuilderSetFlags(NVNtextureBuilder *builder,
                              int flags);

/// \brief Set the target (texture type) for NVNtexture objects created
/// from the NVNtextureBuilder.
///
/// \param builder NVNtextureBuilder object to update.
///
/// \param target NVNtexture target to use.
NVNAPI void NVNAPIENTRY
    nvnTextureBuilderSetTarget(NVNtextureBuilder *builder,
                               NVNtextureTarget target);

/// \brief Set the width (in texels) for NVNtexture objects created from
/// the NVNtextureBuilder.
///
/// \param builder NVNtextureBuilder object to update.
///
/// \param width Width (in texels) used for new textures.
NVNAPI void NVNAPIENTRY
    nvnTextureBuilderSetWidth(NVNtextureBuilder *builder,
                              int width);

/// \brief Set the height (in texels) for NVNtexture objects created from
/// the NVNtextureBuilder.
///
/// \param builder NVNtextureBuilder object to update.
///
/// \param height Height (in texels) used for new textures.
NVNAPI void NVNAPIENTRY
    nvnTextureBuilderSetHeight(NVNtextureBuilder *builder,
                               int height);

/// \brief Set the depth (in texels) for NVNtexture objects created from
/// the NVNtextureBuilder.
///
/// \param builder NVNtextureBuilder object to update.
///
/// \param depth Depth (in texels) used for new textures.
NVNAPI void NVNAPIENTRY
    nvnTextureBuilderSetDepth(NVNtextureBuilder *builder,
                              int depth);

/// \brief Set the width (in texels) for NVNtexture objects created from
/// the NVNtextureBuilder.
///
/// This command also sets the height and depth (in texels) to one.
///
/// \param builder NVNtextureBuilder object to update.
///
/// \param width Width (in texels) used for new textures.
/// Must be greater than zero.
NVNAPI void NVNAPIENTRY
    nvnTextureBuilderSetSize1D(NVNtextureBuilder *builder,
                               int width);

/// \brief Set the width and height (in texels) for NVNtexture objects
/// created from the NVNtextureBuilder.
///
/// This command also sets the depth (in texels) to one.
///
/// \param builder NVNtextureBuilder object to update.
///
/// \param width Width (in texels) used for new textures.
/// Must be greater than zero.
///
/// \param height Height (in texels) used for new textures.
/// Must be greater than zero.
NVNAPI void NVNAPIENTRY
    nvnTextureBuilderSetSize2D(NVNtextureBuilder *builder,
                               int width,
                               int height);

/// \brief Set the width, height, and depth (in texels) for NVNtexture
/// objects created from the NVNtextureBuilder.
///
/// \param builder NVNtextureBuilder object to update.
///
/// \param width Width (in texels) used for new textures.
/// Must be greater than zero.
///
/// \param height Height (in texels) used for new textures.
/// Must be greater than zero.
///
/// \param depth Depth (in texels) used for new textures.
/// Must be greater than zero.
NVNAPI void NVNAPIENTRY
    nvnTextureBuilderSetSize3D(NVNtextureBuilder *builder,
                               int width,
                               int height,
                               int depth);

/// \brief Set the number of mipmap levels for NVNtexture objects created
/// from the NVNtextureBuilder.
///
/// \param builder NVNtextureBuilder object to update.
///
/// \param numLevels Number of mipmap levels to create for the texture.
/// \li Must be greater than zero.
/// \li Must be one for targets TextureTarget::TARGET_RECTANGLE and
/// TextureTarget::TARGET_BUFFER.
/// Must be greater than zero.
/// Must be less than or equal to 15 (the value of
/// #NVN_DEVICE_INFO_MAX_TEXTURE_LEVELS).
NVNAPI void NVNAPIENTRY
    nvnTextureBuilderSetLevels(NVNtextureBuilder *builder,
                               int numLevels);

/// \brief Set the #NVNformat for NVNtexture objects created from the
/// NVNtextureBuilder.
///
/// Note that compressed formats are not supported for 1D or 1D array
/// textures.
///
/// \param builder NVNtextureBuilder object to update.
///
/// \param format #NVNformat to use for new textures.
NVNAPI void NVNAPIENTRY
    nvnTextureBuilderSetFormat(NVNtextureBuilder *builder,
                               NVNformat format);

/// \brief Set the sample count for NVNtexture objects created from the
/// NVNtextureBuilder.
///
/// \param builder NVNtextureBuilder object to update.
///
/// \param samples Sample count to use for new textures.
/// \li Must be zero for non-multisample textures (those with targets other
/// than TextureTarget::TEXTURE_MULTISAMPLE and
/// TextureTarget::TARGET_MULTISAMPLE_ARRAY).
/// \li Must be 2, 4, 8 or 16 for multisample textures.
/// \li May be 16 only for textures with a format of Format::STENCIL8.
NVNAPI void NVNAPIENTRY
    nvnTextureBuilderSetSamples(NVNtextureBuilder *builder,
                                int samples);

/// \brief Set the texture swizzle for NVNtexture objects created from the
/// NVNtextureBuilder.
///
/// Lookups from NVNtexture objects will return a four-component vector
/// with (R,G,B,A) components.  The texture #NVNformat specifies the
/// (R,G,B,A) components used for storing texel data.  The texture swizzle
/// allows applications to produce a new (R,G,B,A) vector by specifying a
/// source component value (R, G, B, A, or the constants 0 or 1) to use for
/// each component of the new output vector. For example, an application
/// can use the swizzle to remap a texture with a format of #NVN_FORMAT_R8
/// into a 'LUMINANCE8' texture by using a swizzle pattern of (R,R,R,1).
///
/// \param builder NVNtextureBuilder object to update.
///
/// \param r Input component value used for the first (R) output component.
///
/// \param g Input component value used for the second (G) output
/// component.
///
/// \param b Input component value used for the third (B) output component.
///
/// \param a Input component value used for the fourth (A) output
/// component.
NVNAPI void NVNAPIENTRY
    nvnTextureBuilderSetSwizzle(NVNtextureBuilder *builder,
                                NVNtextureSwizzle r,
                                NVNtextureSwizzle g,
                                NVNtextureSwizzle b,
                                NVNtextureSwizzle a);

/// \brief Set the values returned for depth/stencil NVNtexture objects
/// created from the NVNtextureBuilder.
///
/// If the texture has a format with no depth or stencil components, the
/// depth/stencil mode is ignored.  If depth/stencil mode selects the
/// stencil component in a texture with only a depth component or vice
/// versa, the results of texture lookups are undefined.
///
/// \param builder NVNtextureBuilder object to update.
///
/// \param mode Specifies whether to return depth or stencil values from a
/// texture with depth and/or stencil components.
NVNAPI void NVNAPIENTRY
    nvnTextureBuilderSetDepthStencilMode(NVNtextureBuilder *builder,
                                         NVNtextureDepthStencilMode mode);

/// \brief Returns the amount of memory (in bytes) required to hold a
/// texture to be created using the provided NVNtextureBuilder object. For
/// correct operation, the NVNtextureBuilder must have a device set via
/// nvnTextureBuilderSetDevice.
///
/// \param builder NVNtextureBuilder object to be used to create a new
/// texture.
NVNAPI size_t NVNAPIENTRY
    nvnTextureBuilderGetStorageSize(const NVNtextureBuilder *builder);

/// \brief Returns the minimum alignment in bytes (in bytes) required to
/// hold a texture to be created using the provided NVNtextureBuilder
/// object.
///
/// For correct operation, the NVNtextureBuilder must have a device set via
/// nvnTextureBuilderSetDevice.
///
/// \param builder NVNtextureBuilder object to be used to create a new
/// texture.
NVNAPI size_t NVNAPIENTRY
    nvnTextureBuilderGetStorageAlignment(const NVNtextureBuilder *builder);

/// \brief Set the location in a NVNmemoryPool used for a NVNtexture object
/// created from the NVNtextureBuilder.
///
/// \param builder NVNtextureBuilder object to update.
///
/// \param pool NVNmemoryPool object providing storage for the new
/// NVNtexture object.
///
/// \param offset Offset (in bytes) of the first byte of pool storage used
/// by the new NVNtexture object.
/// \li The provided offset must be a multiple of the required alignment
/// for the provided TextureBuilder object.
NVNAPI void NVNAPIENTRY
    nvnTextureBuilderSetStorage(NVNtextureBuilder *builder,
                                NVNmemoryPool *pool,
                                ptrdiff_t offset);

/// \brief Specify optional packaged texture data used to initialize
/// texture storage.
///
/// On the Windows reference implementation, texture data produced by the
/// texture packager is not in a form that can be consumed directly by the
/// GPU. When creating a texture with a non-NULL packaged texture data
/// pointer, the Windows reference implementation will convert the texture
/// data from the packaged form to the native format of the GPU, storing
/// the results in the pool memory specified in the NVNtextureBuilder used
/// to create the texture. If the packaged texture data pointer is NULL, no
/// conversion or copy will be performed.
/// On the target device, the packaged texture data pointer is not used;
/// packaged texture data may be loaded directly in the underlying pool
/// storage used by the texture.
/// For more information please refer to the
/// \ref nvn_guide_formats_restrictions_on_Windows
/// "Formats Restrictions on the NVN Windows Reference Implementation"
///  section of the Programming Guide.
///
/// \param builder NVNtextureBuilder object to update.
///
/// \param data CPU pointer to NX-optimized texture data built by the
/// texture packager; ignored on NX.
NVNAPI void NVNAPIENTRY
    nvnTextureBuilderSetPackagedTextureData(NVNtextureBuilder *builder,
                                            const void *data);

/// \brief Specify the layout of packaged texture data used for texture
/// storage.
///
/// This command specifies the memory layout of packaged texture data
/// produced by the NVN texture packager.  By default, the driver and
/// texture packager select an optimal layout for a texture based on its
/// size and format.  This command allows applications to provide the
/// driver with a layout descriptor produced by the NVN texture packager so
/// that old packaged texture assets can continue to be used even if the
/// default layout changes in a new driver.
///
/// \param builder NVNtextureBuilder object to update.
///
/// \param layout Description of the texture layout produced by the NVN
/// texture packager.
NVNAPI void NVNAPIENTRY
    nvnTextureBuilderSetPackagedTextureLayout(NVNtextureBuilder *builder,
                                              const NVNpackagedTextureLayout *layout);

/// \brief Specifies the stride for linear textures. Only used when the
/// TextureFlags::LINEAR_BIT flag is set. The stride is defined as the
/// number of bytes between consecutive rows in the linear image. If the
/// texture is to be used as a render target, then this value must be
/// aligned to at least deviceInfo::LINEAR_RENDER_TARGET_STRIDE_ALIGNMENT.
/// If it is only used as a texture, then it needs to be aligned to
/// deviceInfo::LINEAR_TEXTURE_STRIDE_ALIGNMENT Should be at least (width *
/// bpp).
///
/// Specify stride for linear textures.
///
/// \param builder NVNtextureBuilder object to update.
///
/// \param stride Stride for this linear texture.
/// \li Must be greater than zero for linear textures.
/// \li Must be aligned to either
/// deviceInfo::LINEAR_RENDER_TARGET_STRIDE_ALIGNMENT or
/// deviceInfo::LINEAR_TEXTURE_STRIDE_ALIGNMENT.
/// Must be greater than or equal to zero.
NVNAPI void NVNAPIENTRY
    nvnTextureBuilderSetStride(NVNtextureBuilder *builder,
                               ptrdiff_t stride);

/// \brief Set the name of the OpenGL texture to use as an external texture
/// source.
///
/// If a texture builder with a non-zero texture name is used to initialize
/// a new NVN texture, the texture will be initialized to use all the
/// properties, including texel storage, of the OpenGL or OpenGL ES texture
/// with the provided name.  The texture name will be used to look up
/// OpenGL or OpenGL ES texture object using the OpenGL/OpenGL ES context
/// current on the thread initializing the NVN texture.  If no
/// OpenGL/OpenGL ES context is current, or if the name does not identify a
/// valid texture object, the results of NVN texture initialization are
/// undefined.  There are a number of restrictions on the GL texture
/// object:
/// \li NVNtexture storage must be immutable.
/// \li The format and target of the texture must have NVN equivalents.
/// \li Sparse textures are unsupported.
/// \li NVNbuffer textures are unsupported.
/// Synchronization and lifetime management must be handled by the
/// application. In particular, the application must ensure that all NVN
/// commands accessing the texture have completed (e.g., by calling
/// nvnQueueFinish()) before calling either glDeleteTextures() or
/// nvnTextureFinalize().
/// By default, the texture name associated with a NVNtextureBuilder is
/// zero, which indicates that no GL texture should be used for NVN texture
/// initialization.
///
/// \param builder NVNtextureBuilder object to update.
///
/// \param name The name of the OpenGL/OpenGL ES texture to use as a
/// source.
/// \li Must be zero, or the name of a valid OpenGL or OpenGL ES texture.
/// \li If non-zero, the texture properties must be compatible with NVN.
NVNAPI void NVNAPIENTRY
    nvnTextureBuilderSetGLTextureName(NVNtextureBuilder *builder,
                                      uint32_t name);

/// \brief Returns the storage class needed for virtual memory mappings for
/// a texture to be created using the provided NVNtextureBuilder object.
///
/// When memory from a physical memory pool is mapped into a virtual memory
/// pool, the mapping must be established with a storage class appropriate
/// for texture or buffer resources using the virtual memory.  The value
/// returned by this command should be used for memory mappings of textures
/// created with this NVNtextureBuilder.
///
/// \param builder NVNtextureBuilder object to be used to create a new
/// texture.
NVNAPI NVNstorageClass NVNAPIENTRY
    nvnTextureBuilderGetStorageClass(const NVNtextureBuilder *builder);

/// \brief Query the device associated with this NVNtextureBuilder object.
///
/// \param builder NVNtextureBuilder object to query.
NVNAPI const NVNdevice * NVNAPIENTRY
    nvnTextureBuilderGetDevice(const NVNtextureBuilder *builder);

/// \brief Query the flags for a NVNtextureBuilder object.
///
/// \param builder NVNtextureBuilder object to query.
NVNAPI NVNtextureFlags NVNAPIENTRY
    nvnTextureBuilderGetFlags(const NVNtextureBuilder *builder);

/// \brief Query the target for a NVNtextureBuilder object.
///
/// \param builder NVNtextureBuilder object to query.
NVNAPI NVNtextureTarget NVNAPIENTRY
    nvnTextureBuilderGetTarget(const NVNtextureBuilder *builder);

/// \brief Query the width for a NVNtextureBuilder object.
///
/// \param builder NVNtextureBuilder object to query.
NVNAPI int NVNAPIENTRY
    nvnTextureBuilderGetWidth(const NVNtextureBuilder *builder);

/// \brief Query the height for a NVNtextureBuilder object.
///
/// \param builder NVNtextureBuilder object to query.
NVNAPI int NVNAPIENTRY
    nvnTextureBuilderGetHeight(const NVNtextureBuilder *builder);

/// \brief Query the depth for a NVNtextureBuilder object.
///
/// \param builder NVNtextureBuilder object to query.
NVNAPI int NVNAPIENTRY
    nvnTextureBuilderGetDepth(const NVNtextureBuilder *builder);

/// \brief Query the level count for a NVNtextureBuilder object.
///
/// \param builder NVNtextureBuilder object to query.
NVNAPI int NVNAPIENTRY
    nvnTextureBuilderGetLevels(const NVNtextureBuilder *builder);

/// \brief Query the format for a NVNtextureBuilder object.
///
/// \param builder NVNtextureBuilder object to query.
NVNAPI NVNformat NVNAPIENTRY
    nvnTextureBuilderGetFormat(const NVNtextureBuilder *builder);

/// \brief Query the sample count for a NVNtextureBuilder object.
///
/// \param builder NVNtextureBuilder object to query.
NVNAPI int NVNAPIENTRY
    nvnTextureBuilderGetSamples(const NVNtextureBuilder *builder);

/// \brief Query the swizzles for a NVNtextureBuilder object.
///
/// \param builder NVNtextureBuilder object to query.
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
NVNAPI void NVNAPIENTRY
    nvnTextureBuilderGetSwizzle(const NVNtextureBuilder *builder,
                                NVNtextureSwizzle *r,
                                NVNtextureSwizzle *g,
                                NVNtextureSwizzle *b,
                                NVNtextureSwizzle *a);

/// \brief Query the depth/stencil mode for a NVNtextureBuilder object.
///
/// \param builder NVNtextureBuilder object to query.
NVNAPI NVNtextureDepthStencilMode NVNAPIENTRY
    nvnTextureBuilderGetDepthStencilMode(const NVNtextureBuilder *builder);

/// \brief Query the packaged texture data pointer for a NVNtextureBuilder
/// object.
///
/// \param builder NVNtextureBuilder object to query.
NVNAPI const void * NVNAPIENTRY
    nvnTextureBuilderGetPackagedTextureData(const NVNtextureBuilder *builder);

/// \brief Query the packaged texture layout associated with this
/// NVNtextureBuilder object.
///
/// Returns true if a layout of packaged texture data is associated with
/// this NVNtextureBuilder. In this case the layout is returned in data.
///
/// \param builder NVNtextureBuilder object to query.
///
/// \param[out] data Packaged texture layout associated with this
/// NVNtextureBuilder object.
NVNAPI NVNboolean NVNAPIENTRY
    nvnTextureBuilderGetPackagedTextureLayout(const NVNtextureBuilder *builder,
                                              NVNpackagedTextureLayout *data);

/// \brief Query the stride for a NVNtextureBuilder object.
///
/// \param builder NVNtextureBuilder object to query.
NVNAPI ptrdiff_t NVNAPIENTRY
    nvnTextureBuilderGetStride(const NVNtextureBuilder *builder);

/// \brief Returns properties of the sparse memory layout for a texture to
/// be created using the provided NVNtextureBuilder object.
///
/// For correct operation, the NVNtextureBuilder must have a device set via
/// nvnTextureBuilderSetDevice.
///
/// \param builder NVNtextureBuilder object to be used to create a new
/// texture.
///
/// \param[out] layout Pointer to a NVNtextureSparseTileLayout structure to
/// receive layout information.
NVNAPI void NVNAPIENTRY
    nvnTextureBuilderGetSparseTileLayout(const NVNtextureBuilder *builder,
                                         NVNtextureSparseTileLayout *layout);

/// \brief Returns the name of the OpenGL or OpenGL ES texture to use as an
/// external texture source.
///
/// If the returned name is zero, no source texture will be used. See
/// SetGLTextureName for details.
///
/// \param builder NVNtextureBuilder object to query.
NVNAPI uint32_t NVNAPIENTRY
    nvnTextureBuilderGetGLTextureName(const NVNtextureBuilder *builder);

/// \brief Returns the amount of memory (in bytes) required to hold a ZCull
/// save/restore buffer for a depth texture. The buffer should be aligned
/// to #NVN_DEVICE_INFO_ZCULL_SAVE_RESTORE_ALIGNMENT bytes.
///
/// If the texture has a format with no depth or stencil components, this
/// function returns zero.
///
/// \param builder NVNtextureBuilder object to be used to create a new
/// texture.
NVNAPI size_t NVNAPIENTRY
    nvnTextureBuilderGetZCullStorageSize(const NVNtextureBuilder *builder);

/// \brief Query the memory pool used by a NVNtextureBuilder object.
///
/// \param builder NVNtextureBuilder object to query.
NVNAPI NVNmemoryPool * NVNAPIENTRY
    nvnTextureBuilderGetMemoryPool(const NVNtextureBuilder *builder);

/// \brief Query the memory pool offset used by a NVNtextureBuilder object.
///
/// \param builder NVNtextureBuilder object to query.
NVNAPI ptrdiff_t NVNAPIENTRY
    nvnTextureBuilderGetMemoryOffset(const NVNtextureBuilder *builder);

/// \brief Query the raw storage class associated with this
/// NVNtextureBuilder object.
///
/// \param builder NVNtextureBuilder object to query.
NVNAPI NVNrawStorageClass NVNAPIENTRY
    nvnTextureBuilderGetRawStorageClass(const NVNtextureBuilder *builder);

/// \brief Set default state for the NVNtextureView object.
///
/// A NVNtextureView object programmed with the default state will use all
/// of the properties of the original texture using the texture view
/// object.  Only those properties explicitly programmed in the
/// NVNtextureView object will affect the view of the original texture.
///
/// \param view NVNtextureView object to reset.
NVNAPI void NVNAPIENTRY
    nvnTextureViewSetDefaults(NVNtextureView *view);

/// \brief Update a texture view to select a subset of the mipmap levels in
/// a NVNtexture object.
///
/// By default, texture objects registered in the texture pool will use all
/// mipmap levels in the original texture.  Selecting levels in the texture
/// view when registering a texture will limit the registered texture to a
/// subset of the levels of the original texture object.  Other rendering
/// operations using a texture will default to using the base level
/// (numbered zero) of the original texture. Selecting levels in the
/// texture view in these cases will instead use the baseLevel parameter of
/// the texture view.
///
/// \param view NVNtextureView object to update.
///
/// \param baseLevel Specifies the minimum (base) level in a view of a
/// NVNtexture object.
/// A base level of zero selects the highest-resolution mipmap level of the
/// texture; larger values select lower-resolution mipmap levels. Must be
/// less than the number of layers in the original texture object.
/// Must be less than 15 (the value of
/// #NVN_DEVICE_INFO_MAX_TEXTURE_LEVELS).
///
/// \param numLevels Specifies the number of mipmap levels in a view of a
/// NVNtexture object.
/// If non-zero, selects the specified number of mipmap levels.  If zero,
/// selects all mipmap levels below baseLevel.  The sum of _baseLevel_ and
/// _numLevels_ in the view must be less than or equal to the number of
/// levels in the original texture object.
NVNAPI void NVNAPIENTRY
    nvnTextureViewSetLevels(NVNtextureView *view,
                            int baseLevel,
                            int numLevels);

/// \brief Update a texture view to select a subset of the layers in a
/// NVNtexture object.
///
/// By default, texture objects registered in the texture pool or used for
/// rendering will use all layers in the original texture.  Selecting
/// layers in the texture view will limit the view to a subset of the
/// layers of the original texture object.
///
/// \param view NVNtextureView object to update.
///
/// \param minLayer Specifies the minimum layer number in a view of a
/// NVNtexture object.
/// Must be less than the total number of layers in the texture.
/// Must be less than 2048 (the value of
/// #NVN_DEVICE_INFO_MAX_TEXTURE_LAYERS).
///
/// \param numLayers Specifies the number of layers to use when accessing
/// this texture.
/// If non-zero, selects the specified number of layers.  If zero, selects
/// all layers greater than or equal to minLayer.  The sum of _minLayer_
/// and *numLayers* in the view must be less than or equal to the number of
/// layers in the original texture object.
NVNAPI void NVNAPIENTRY
    nvnTextureViewSetLayers(NVNtextureView *view,
                            int minLayer,
                            int numLayers);

/// \brief Update a texture view to select an alternate texture format.
///
/// Using a format in a texture view allows applications to reinterpret the
/// storage belonging to a texture object with an alternate format.  For
/// example, a texture with a format of #NVN_FORMAT_RGBA8 can be
/// reinterpreted with a format of #NVN_FORMAT_RGBA8UI to obtain the
/// components of the texture as raw integer values instead of as
/// normalized values in [0,1].  The format selected for the texture view
/// must be compatible with the format of the original texture object.  The
/// format specified in a texture view overrides the format specified by
/// nvnTextureBuilderSetFormat.
///
/// \param view NVNtextureView object to update.
///
/// \param format #NVNformat to use in the texture view.
NVNAPI void NVNAPIENTRY
    nvnTextureViewSetFormat(NVNtextureView *view,
                            NVNformat format);

/// \brief Update a texture view to select an alternate component swizzle.
///
/// The swizzles specified in a texture view override those specified by
/// nvnTextureBuilderSetSwizzle.
///
/// \param view NVNtextureView object to update.
///
/// \param r Input component value used for the first (R) output component.
///
/// \param g Input component value used for the second (G) output
/// component.
///
/// \param b Input component value used for the third (B) output component.
///
/// \param a Input component value used for the fourth (A) output
/// component.
NVNAPI void NVNAPIENTRY
    nvnTextureViewSetSwizzle(NVNtextureView *view,
                             NVNtextureSwizzle r,
                             NVNtextureSwizzle g,
                             NVNtextureSwizzle b,
                             NVNtextureSwizzle a);

/// \brief Update a texture view to select an alternate depth/stencil mode.
///
/// The swizzles specified in a texture view override those specified by
/// nvnTextureBuilderSetDepthStencilMode.
///
/// \param view NVNtextureView object to update.
///
/// \param mode Specifies whether to return depth or stencil values from a
/// texture with depth and/or stencil components.
NVNAPI void NVNAPIENTRY
    nvnTextureViewSetDepthStencilMode(NVNtextureView *view,
                                      NVNtextureDepthStencilMode mode);

/// \brief Update a texture view to select an alternate texture target.
///
/// The swizzles specified in a texture view override those specified by
/// nvnTextureBuilderSetTarget.
///
/// \param view NVNtextureView object to update.
///
/// \param target NVNtexture target to use.
NVNAPI void NVNAPIENTRY
    nvnTextureViewSetTarget(NVNtextureView *view,
                            NVNtextureTarget target);

/// \brief Query the set of levels associated with a NVNtextureView object.
///
/// Returns NVN_TRUE if a set of levels has been specified in the view.
/// Returns NVN_FALSE if no set of levels has been specified.
///
/// \param view NVNtextureView object to query.
///
/// \param[out] baseLevel Minimum (base) level in a view of a NVNtexture
/// object.  Zero is returned if no set of levels has been specified.
///
/// \param[out] numLevels Number of mipmap levels in a view of a NVNtexture
/// object.  Zero is returned if no set of levels has been specified.
NVNAPI NVNboolean NVNAPIENTRY
    nvnTextureViewGetLevels(const NVNtextureView *view,
                            int *baseLevel,
                            int *numLevels);

/// \brief Query the set of layers associated with a NVNtextureView object.
///
/// Returns NVN_TRUE if a set of layers has been specified in the view.
/// Returns NVN_FALSE if no set of layers has been specified.
///
/// \param view NVNtextureView object to query.
///
/// \param[out] minLayer The minimum layer number in a view of a NVNtexture
/// object.  Zero is returned if no set of layers has been specified.
///
/// \param[out] numLayers The number of layers in a view of a NVNtexture
/// object.  Zero is returned if no set of layers has been specified.
NVNAPI NVNboolean NVNAPIENTRY
    nvnTextureViewGetLayers(const NVNtextureView *view,
                            int *minLayer,
                            int *numLayers);

/// \brief Query the format associated with a NVNtextureView object.
///
/// Returns NVN_TRUE if a format has been specified in the view. Returns
/// NVN_FALSE if no format has been specified.
///
/// \param view NVNtextureView object to query.
///
/// \param[out] format #NVNformat in the texture view.  #NVN_FORMAT_NONE is
/// returned if no format has been specified.
NVNAPI NVNboolean NVNAPIENTRY
    nvnTextureViewGetFormat(const NVNtextureView *view,
                            NVNformat *format);

/// \brief Query the swizzles associated with a NVNtextureView object.
///
/// Returns NVN_TRUE if swizzles have been specified in the view. Returns
/// NVN_FALSE if no swizzles have been specified.
///
/// \param view NVNtextureView object to query.
///
/// \param[out] r Input component value used for the first (R) output
/// component.  #NVN_TEXTURE_SWIZZLE_ZERO is returned if no swizzles have
/// been specified.
///
/// \param[out] g Input component value used for the second (G) output
/// component.  #NVN_TEXTURE_SWIZZLE_ZERO is returned if no swizzles have
/// been specified.
///
/// \param[out] b Input component value used for the third (B) output
/// component.  #NVN_TEXTURE_SWIZZLE_ZERO is returned if no swizzles have
/// been specified.
///
/// \param[out] a Input component value used for the fourth (A) output
/// component.  #NVN_TEXTURE_SWIZZLE_ZERO is returned if no swizzles have
/// been specified.
NVNAPI NVNboolean NVNAPIENTRY
    nvnTextureViewGetSwizzle(const NVNtextureView *view,
                             NVNtextureSwizzle *r,
                             NVNtextureSwizzle *g,
                             NVNtextureSwizzle *b,
                             NVNtextureSwizzle *a);

/// \brief Query the depth/stencil mode associated with a NVNtextureView
/// object.
///
/// Returns NVN_TRUE if a depth/stencil mode has been specified in the
/// view. Returns NVN_FALSE if no depth/stencil mode has been specified.
///
/// \param view NVNtextureView object to query.
///
/// \param[out] mode Selector to return depth or stencil values from a
/// texture with depth and/or stencil components.
/// #NVN_TEXTURE_DEPTH_STENCIL_MODE_DEPTH is returned if no depth/stencil
/// mode has been specified.
NVNAPI NVNboolean NVNAPIENTRY
    nvnTextureViewGetDepthStencilMode(const NVNtextureView *view,
                                      NVNtextureDepthStencilMode *mode);

/// \brief Query the target associated with a NVNtextureView object.
///
/// Returns NVN_TRUE if a target has been specified in the view. Returns
/// NVN_FALSE if no target has been specified.
///
/// \param view NVNtextureView object to query.
///
/// \param[out] target Target associated with the texture view.
/// #NVN_TEXTURE_TARGET_1D is returned if no target has been specified.
NVNAPI NVNboolean NVNAPIENTRY
    nvnTextureViewGetTarget(const NVNtextureView *view,
                            NVNtextureTarget *target);

/// \brief Checks if NVNtextureView objects are equivalent.
///
/// \param view First NVNtextureView object.
///
/// \param otherView NVNtextureView object to compare against.
NVNAPI NVNboolean NVNAPIENTRY
    nvnTextureViewCompare(const NVNtextureView *view,
                          const NVNtextureView *otherView);

/// \brief Initialize a texture object from a texture builder using storage
/// provided by a memory pool.
///
/// \param texture NVNtexture object to initialize.
///
/// \param builder NVNtextureBuilder object holding properties of the new
/// NVNtexture object.
NVNAPI NVNboolean NVNAPIENTRY
    nvnTextureInitialize(NVNtexture *texture,
                         const NVNtextureBuilder *builder);

/// \brief Returns the amount of memory (in bytes) required to hold a ZCull
/// save/restore buffer for a depth texture. The buffer should be aligned
/// to #NVN_DEVICE_INFO_ZCULL_SAVE_RESTORE_ALIGNMENT bytes.
///
/// If the texture has a format with no depth or stencil components, this
/// function returns zero.
///
/// \param texture NVNtexture object to query.
NVNAPI size_t NVNAPIENTRY
    nvnTextureGetZCullStorageSize(const NVNtexture *texture);

/// \brief Finalize a NVNtexture object.
///
/// \param texture NVNtexture object to finalize.
NVNAPI void NVNAPIENTRY
    nvnTextureFinalize(NVNtexture *texture);

/// \brief Sets the debug label string for a NVNtexture object.
///
/// Annotates a NVNtexture object with a descriptive text label. This label
/// may then be used by the debug layer, an external debugger, or a
/// profiler tool.
///
/// \param texture NVNtexture object to set debug label.
///
/// \param label Text string used to annotate the NVNtexture object.
NVNAPI void NVNAPIENTRY
    nvnTextureSetDebugLabel(NVNtexture *texture,
                            const char * label);

/// \brief Returns the storage class needed for virtual memory mappings for
/// a NVNtexture object.
///
/// When memory from a physical memory pool is mapped into a virtual memory
/// pool, the mapping must be established with a storage class appropriate
/// for texture or buffer resources using the virtual memory.  The value
/// returned by this command should be used for memory mappings used by
/// this texture.
///
/// \param texture NVNtexture object to query.
NVNAPI NVNstorageClass NVNAPIENTRY
    nvnTextureGetStorageClass(const NVNtexture *texture);

/// \brief Returns the offset (in bytes) of the first byte of storage used
/// by a view of a NVNtexture object.
///
/// The offset returned is a value relative to the offset of the first byte
/// of storage used by the full texture.  Textures are stored in
/// (virtually) contiguous memory.  For array and cube map textures, each
/// layer or face is stored in memory as a unit with offsets increasing by
/// layer number.  For mipmapped textures, each mipmap level in the texture
/// (or in each layer for array and cube map textures) is stored as a unit
/// with offsets increasing by mipmap level number.
///
/// \param texture NVNtexture object to query.
///
/// \param textureView View of the texture used to determine an offset.
NVNAPI ptrdiff_t NVNAPIENTRY
    nvnTextureGetViewOffset(const NVNtexture *texture,
                            const NVNtextureView *textureView);

/// \brief Query the flags for a NVNtexture object.
///
/// \param texture NVNtexture object to query.
NVNAPI NVNtextureFlags NVNAPIENTRY
    nvnTextureGetFlags(const NVNtexture *texture);

/// \brief Query the target for a NVNtexture object.
///
/// \param texture NVNtexture object to query.
NVNAPI NVNtextureTarget NVNAPIENTRY
    nvnTextureGetTarget(const NVNtexture *texture);

/// \brief Query the width for a NVNtexture object.
///
/// \param texture NVNtexture object to query.
NVNAPI int NVNAPIENTRY
    nvnTextureGetWidth(const NVNtexture *texture);

/// \brief Query the height for a NVNtexture object.
///
/// \param texture NVNtexture object to query.
NVNAPI int NVNAPIENTRY
    nvnTextureGetHeight(const NVNtexture *texture);

/// \brief Query the depth for a NVNtexture object.
///
/// \param texture NVNtexture object to query.
NVNAPI int NVNAPIENTRY
    nvnTextureGetDepth(const NVNtexture *texture);

/// \brief Query the level count for a NVNtexture object.
///
/// \param texture NVNtexture object to query.
NVNAPI int NVNAPIENTRY
    nvnTextureGetLevels(const NVNtexture *texture);

/// \brief Query the format for a NVNtexture object.
///
/// \param texture NVNtexture object to query.
NVNAPI NVNformat NVNAPIENTRY
    nvnTextureGetFormat(const NVNtexture *texture);

/// \brief Query the sample count for a NVNtexture object.
///
/// \param texture NVNtexture object to query.
NVNAPI int NVNAPIENTRY
    nvnTextureGetSamples(const NVNtexture *texture);

/// \brief Query the swizzles for a NVNtexture object.
///
/// \param texture NVNtexture object to query.
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
NVNAPI void NVNAPIENTRY
    nvnTextureGetSwizzle(const NVNtexture *texture,
                         NVNtextureSwizzle *r,
                         NVNtextureSwizzle *g,
                         NVNtextureSwizzle *b,
                         NVNtextureSwizzle *a);

/// \brief Query the depth/stencil mode for a NVNtexture object.
///
/// \param texture NVNtexture object to query.
NVNAPI NVNtextureDepthStencilMode NVNAPIENTRY
    nvnTextureGetDepthStencilMode(const NVNtexture *texture);

/// \brief Query the stride for a NVNtexture object.
///
/// \param texture NVNtexture object to query.
NVNAPI ptrdiff_t NVNAPIENTRY
    nvnTextureGetStride(const NVNtexture *texture);

/// \brief Returns the GPU address of the first byte of memory used by a
/// texture.
///
/// \warning Except for buffer and linear textures, a NVNtextureAddress
/// returned by nvnTextureGetTextureAddress corresponds to a special memory
/// mapping that can not be used to view or manipulate memory in commands
/// accepting a buffer address.
///
/// \param texture NVNtexture object to query.
NVNAPI NVNtextureAddress NVNAPIENTRY
    nvnTextureGetTextureAddress(const NVNtexture *texture);

/// \brief Returns properties of the sparse memory layout for a texture.
///
/// \param texture NVNtexture object to query.
///
/// \param[out] layout Pointer to a NVNtextureSparseTileLayout structure to
/// receive layout information.
NVNAPI void NVNAPIENTRY
    nvnTextureGetSparseTileLayout(const NVNtexture *texture,
                                  NVNtextureSparseTileLayout *layout);

/// \brief Copy texture data from memory into a region of a NVNtexture
/// object.
///
/// Copies texture data from the memory pointed to by _data_ to the
/// specified region of a texture, using the CPU.  This command behaves
/// like nvnCommandBufferCopyBufferToTexture, except that the copy is
/// performed immediately and does not involve the GPU on NX. On the
/// Windows reference implementation, this copy will use the GPU to update
/// GPU-attached video memory.  If the texture is stored in a memory pool
/// created with #NVN_MEMORY_POOL_FLAGS_CPU_CACHED_BIT, the caller must use
/// nvnTextureFlushTexels to flush relevant portions of the CPU cache prior
/// to using the updated texture in the GPU.
///
/// \warning Because the CPU does not have access to GPU compression
/// information needed to properly interpret GPU-compressed textures, it is
/// an error to use this command to operate on a texture with the
/// #NVN_TEXTURE_FLAGS_COMPRESSIBLE_BIT flag set.
///
/// \param texture NVNtexture object to update.
/// This command may not be used to update textures stored in memory pools
/// created with #NVN_MEMORY_POOL_FLAGS_CPU_NO_ACCESS_BIT.
///
/// \param textureView View of the texture object used as the destination
/// for the copy.
/// A texture view, if specified, can be used to override properties of the
/// corresponding texture _texture_. This parameter identifies a specific
/// mipmap level, set of layers or alternate format in the specified
/// texture. Other fields, such as swizzle, are ignored. If NULL is
/// specified, the base level of the texture _texture_ will be used as-is.
///
/// \param region Region of the destination texture to update.
///
/// \param data Pointer to source texel data in system memory.
NVNAPI void NVNAPIENTRY
    nvnTextureWriteTexels(const NVNtexture *texture,
                          const NVNtextureView *textureView,
                          const NVNcopyRegion *region,
                          const void *data);

/// \brief Copy texture data from memory into a region of a NVNtexture
/// object with user-specified stride.
///
/// Copies texture data from the memory pointed to by _data_ to the
/// specified region of a texture, using the CPU.  This command behaves
/// like the combination of nvnCommandBufferSetCopyRowStride,
/// nvnCommandBufferSetCopyImageStride, and
/// nvnCommandBufferCopyBufferToTexture, except that no stride state is
/// retained, and the copy is performed immediately and does not involve
/// the GPU on NX. Note this means that _rowStride_ must be at least the
/// number of bytes in a row of the image, and _imageStride_ must be at
/// least the number of bytes in one full image. On the Windows reference
/// implementation, this copy will use the GPU to update GPU-attached video
/// memory. If the texture is stored in a memory pool created with
/// #NVN_MEMORY_POOL_FLAGS_CPU_CACHED_BIT, the caller must use
/// nvnTextureFlushTexels to flush relevant portions of the CPU cache prior
/// to using the updated texture in the GPU.
///
/// \warning Because the CPU does not have access to GPU compression
/// information needed to properly interpret GPU-compressed textures, it is
/// an error to use this command to operate on a texture with the
/// #NVN_TEXTURE_FLAGS_COMPRESSIBLE_BIT flag set.
///
/// \param texture NVNtexture object to update.
/// This command may not be used to update textures stored in memory pools
/// created with #NVN_MEMORY_POOL_FLAGS_CPU_NO_ACCESS_BIT.
///
/// \param textureView View of the texture object used as the destination
/// for the copy.
/// A texture view, if specified, can be used to override properties of the
/// corresponding texture _texture_. This parameter identifies a specific
/// mipmap level, set of layers or alternate format in the specified
/// texture. Other fields, such as swizzle, are ignored. If NULL is
/// specified, the base level of the texture _texture_ will be used as-is.
///
/// \param region Region of the destination texture to update.
///
/// \param data Pointer to source texel data in system memory.
///
/// \param rowStride Stride (in bytes) between rows of texels in _data_.
/// If specified as 0, rows of texels in _data_ will be treated as densely
/// packed, where each new row in memory starts immediately after the end
/// of the previous row. For compressed texture formats, _rowStride_
/// indicates the stride between rows of blocks.
///
/// \param imageStride Stride (in bytes) between 2D arrays of texels in
/// _data_.
/// If specified as 0, 2D arrays of texels (corresponding to 2d texture
/// layers, cube faces, or slices of a 3D texture) will be treated as
/// densely packed, where each 2D array in memory starts immediately after
/// the end of the previous 2D array. If _imageStride_ is 0 and _rowStride_
/// is non-zero, each 2D array is treated as an array of rows, each
/// consuming _rowStride_ bytes.
NVNAPI void NVNAPIENTRY
    nvnTextureWriteTexelsStrided(const NVNtexture *texture,
                                 const NVNtextureView *textureView,
                                 const NVNcopyRegion *region,
                                 const void *data,
                                 ptrdiff_t rowStride,
                                 ptrdiff_t imageStride);

/// \brief Read texture data from a region of a NVNtexture object into
/// memory.
///
/// Copies texture data from the specified region of a texture to the
/// memory pointed to by _data_, using the CPU.  This command behaves like
/// nvnCommandBufferCopyTextureToBuffer, except that the copy is performed
/// immediately and does not involve the GPU on NX. On the Windows
/// reference implementation, this copy will use the GPU to read
/// GPU-attached video memory.  If the texture is stored in a memory pool
/// created with #NVN_MEMORY_POOL_FLAGS_CPU_CACHED_BIT and has been updated
/// using the GPU, the caller must use nvnTextureInvalidateTexels to
/// invalidate relevant portions of the CPU cache prior to using this
/// command to read from the texture.
///
/// \warning Because the CPU does not have access to GPU compression
/// information needed to properly interpret GPU-compressed textures, it is
/// an error to use this command to operate on a texture with the
/// #NVN_TEXTURE_FLAGS_COMPRESSIBLE_BIT flag set.
///
/// \param texture NVNtexture object to read.
/// This command may not be used to read from textures stored in memory
/// pools created with #NVN_MEMORY_POOL_FLAGS_CPU_NO_ACCESS_BIT.
///
/// \param textureView View of the texture object used as the source for
/// the copy.
/// A texture view, if specified, can be used to override properties of the
/// corresponding texture _texture_. This parameter identifies a specific
/// mipmap level, set of layers or alternate format in the specified
/// texture. Other fields, such as swizzle, are ignored. If NULL is
/// specified, the base level of the texture _texture_ will be used as-is.
///
/// \param region Region of the source texture to read.
///
/// \param[in,out] data Pointer to destination texel data in system memory.
NVNAPI void NVNAPIENTRY
    nvnTextureReadTexels(const NVNtexture *texture,
                         const NVNtextureView *textureView,
                         const NVNcopyRegion *region,
                         void *data);

/// \brief Read texture data from a region of a NVNtexture object into
/// memory.
///
/// Copies texture data from the specified region of a texture to the
/// memory pointed to by _data_, using the CPU.  This command behaves like
/// the combination of nvnCommandBufferSetCopyRowStride,
/// nvnCommandBufferSetCopyImageStride, and
/// nvnCommandBufferCopyTextureToBuffer, except that no stride state is
/// retained, and the copy is performed immediately and does not involve
/// the GPU on NX. Note this means that _rowStride_ must be at least the
/// number of bytes in a row of the image, and _imageStride_ must be at
/// least the number of bytes in one full image. On the Windows reference
/// implementation, this copy will use the GPU to read GPU-attached video
/// memory.  If the texture is stored in a memory pool created with
/// #NVN_MEMORY_POOL_FLAGS_CPU_CACHED_BIT and has been updated using the
/// GPU, the caller must use nvnTextureInvalidateTexels to invalidate
/// relevant portions of the CPU cache prior to using this command to read
/// from the texture.
///
/// \warning Because the CPU does not have access to GPU compression
/// information needed to properly interpret GPU-compressed textures, it is
/// an error to use this command to operate on a texture with the
/// #NVN_TEXTURE_FLAGS_COMPRESSIBLE_BIT flag set.
///
/// \param texture NVNtexture object to read.
/// This command may not be used to read from textures stored in memory
/// pools created with #NVN_MEMORY_POOL_FLAGS_CPU_NO_ACCESS_BIT.
///
/// \param textureView View of the texture object used as the source for
/// the copy.
/// A texture view, if specified, can be used to override properties of the
/// corresponding texture _texture_. This parameter identifies a specific
/// mipmap level, set of layers or alternate format in the specified
/// texture. Other fields, such as swizzle, are ignored. If NULL is
/// specified, the base level of the texture _texture_ will be used as-is.
///
/// \param region Region of the source texture to read.
///
/// \param[in,out] data Pointer to destination texel data in system memory.
///
/// \param rowStride Stride (in bytes) between rows of texels in _data_.
/// If specified as 0, rows of texels in _data_ will be treated as densely
/// packed, where each new row in memory starts immediately after the end
/// of the previous row. For compressed texture formats, _rowStride_
/// indicates the stride between rows of blocks.
///
/// \param imageStride Stride (in bytes) between 2D arrays of texels in
/// _data_.
/// If specified as 0, 2D arrays of texels (corresponding to 2d texture
/// layers, cube faces, or slices of a 3D texture) will be treated as
/// densely packed, where each 2D array in memory starts immediately after
/// the end of the previous 2D array. If _imageStride_ is 0 and _rowStride_
/// is non-zero, each 2D array is treated as an array of rows, each
/// consuming _rowStride_ bytes.
NVNAPI void NVNAPIENTRY
    nvnTextureReadTexelsStrided(const NVNtexture *texture,
                                const NVNtextureView *textureView,
                                const NVNcopyRegion *region,
                                void *data,
                                ptrdiff_t rowStride,
                                ptrdiff_t imageStride);

/// \brief Flush CPU cache lines holding portions of a texture.
///
/// When a texture object is stored in a memory pool created using
/// #NVN_MEMORY_POOL_FLAGS_CPU_CACHED_BIT, texel updates using
/// nvnTextureWriteTexels will use the CPU cache and may not be immediately
/// visible to the GPU.  Before using the contents of such textures on the
/// GPU, this command should be used to flush relevant portions of the CPU
/// cache to ensure that updated texels are written to main memory and will
/// be visible to the GPU.  This operation is not needed for textures
/// stored in memory pools created using
/// #NVN_MEMORY_POOL_FLAGS_CPU_UNCACHED_BIT.
///
/// \param texture NVNtexture object used for the flush operation.
///
/// \param textureView View of the texture object used for the flush
/// operation.
/// A texture view, if specified, can be used to specify the layer set of
/// the corresponding texture _texture_.  If NULL is specified, the base
/// level of the texture _texture_ will be used as-is.
///
/// \param region Region of the texture used by the flush operation.
NVNAPI void NVNAPIENTRY
    nvnTextureFlushTexels(const NVNtexture *texture,
                          const NVNtextureView *textureView,
                          const NVNcopyRegion *region);

/// \brief Invalidate CPU cache lines holding portions of a texture.
///
/// When a texture object is stored in a memory pool created using
/// #NVN_MEMORY_POOL_FLAGS_CPU_CACHED_BIT, texels accessed by the CPU may
/// be stored in CPU caches.  If texel values are updated using the GPU,
/// subsequent accesses using nvnTextureReadTexels may read old texel
/// values if any of the old data remains in the CPU cache.  Before reading
/// GPU-modified textures using nvnTextureReadTexels, this command should
/// be used to invalidate relevant portions of the CPU cache to ensure that
/// stale texel values do not remain in the CPU caches.  This operation is
/// not needed for textures stored in memory pools created using
/// #NVN_MEMORY_POOL_FLAGS_CPU_UNCACHED_BIT.
///
/// \param texture NVNtexture object used for the invalidate operation.
///
/// \param textureView View of the texture object used for the invalidate
/// operation.
/// A texture view, if specified, can be used to specify the layer set of
/// the corresponding texture _texture_.  If NULL is specified, the base
/// level of the texture _texture_ will be used as-is.
///
/// \param region Region of the texture used for the invalidate operation.
NVNAPI void NVNAPIENTRY
    nvnTextureInvalidateTexels(const NVNtexture *texture,
                               const NVNtextureView *textureView,
                               const NVNcopyRegion *region);

/// \brief Query the memory pool used by a NVNtexture object.
///
/// \param texture NVNtexture object to query.
NVNAPI NVNmemoryPool * NVNAPIENTRY
    nvnTextureGetMemoryPool(const NVNtexture *texture);

/// \brief Query the memory pool offset used by a NVNtexture object.
///
/// \param texture NVNtexture object to query.
NVNAPI ptrdiff_t NVNAPIENTRY
    nvnTextureGetMemoryOffset(const NVNtexture *texture);

/// \brief Query the storage size for a NVNtexture object.
///
/// \param texture NVNtexture object to query.
NVNAPI int NVNAPIENTRY
    nvnTextureGetStorageSize(const NVNtexture *texture);

/// \brief Checks if texture objects are equivalent.
///
/// \param texture First texture object.
///
/// \param otherTexture NVNtexture object to compare against.
NVNAPI NVNboolean NVNAPIENTRY
    nvnTextureCompare(const NVNtexture *texture,
                      const NVNtexture *otherTexture);

/// \brief Returns the debug layer unique ID of this texture. Returns 0 if
/// debug layer disabled.
///
/// \param texture NVNtexture object to query.
NVNAPI uint64_t NVNAPIENTRY
    nvnTextureGetDebugID(const NVNtexture *texture);

/// \brief Query the raw storage class associated with this NVNtexture
/// object.
///
/// \param texture NVNtexture object to query.
NVNAPI NVNrawStorageClass NVNAPIENTRY
    nvnTextureGetRawStorageClass(const NVNtexture *texture);

/// \brief Set the NVNdevice associated with a NVNsamplerBuilder object.
///
/// \param builder NVNsamplerBuilder object to update.
///
/// \param device NVNdevice owning NVNsampler objects created by the
/// NVNsamplerBuilder.
NVNAPI void NVNAPIENTRY
    nvnSamplerBuilderSetDevice(NVNsamplerBuilder *builder,
                               NVNdevice *device);

/// \brief Set default state for the NVNsamplerBuilder object.
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
///
/// \param builder NVNsamplerBuilder object to reset.
NVNAPI void NVNAPIENTRY
    nvnSamplerBuilderSetDefaults(NVNsamplerBuilder *builder);

/// \brief Set the minification and magnification filters for NVNsampler
/// objects created from the NVNsamplerBuilder.
///
/// \param builder NVNsamplerBuilder object to update.
///
/// \param min Specifies the minification filter.
///
/// \param mag Specifies the magnification filter.
NVNAPI void NVNAPIENTRY
    nvnSamplerBuilderSetMinMagFilter(NVNsamplerBuilder *builder,
                                     NVNminFilter min,
                                     NVNmagFilter mag);

/// \brief Set the coordinate wrap modes for NVNsampler objects created
/// from the NVNsamplerBuilder.
///
/// \param builder NVNsamplerBuilder object to update.
///
/// \param s Specifies the wrap mode for the first (S) texture coordinate.
///
/// \param t Specifies the wrap mode for the second (T) texture coordinate.
///
/// \param r Specifies the wrap mode for the third (R) texture coordinate.
NVNAPI void NVNAPIENTRY
    nvnSamplerBuilderSetWrapMode(NVNsamplerBuilder *builder,
                                 NVNwrapMode s,
                                 NVNwrapMode t,
                                 NVNwrapMode r);

/// \brief Set the minimum and maximum level of detail used for NVNsampler
/// objects created from the NVNsamplerBuilder.
///
/// When a sampler builder is used to initialize a sampler object, _min_
/// will be clamped to [0.0, 15.0], and _max_ will be clamped to [max(0.0,
/// _min_), 15.0].
///
/// \param builder NVNsamplerBuilder object to update.
///
/// \param min Specifies the minimum level of detail used with the sampler.
/// \li Must be less than or equal to max.
///
/// \param max Specifies the maximum level of detail used with the sampler.
/// \li Must be greater than or equal to min.
NVNAPI void NVNAPIENTRY
    nvnSamplerBuilderSetLodClamp(NVNsamplerBuilder *builder,
                                 float min,
                                 float max);

/// \brief Set the level-of-detail bias used for NVNsampler objects created
/// from the NVNsamplerBuilder.
///
/// When a sampler builder is used to initialize a sampler object, _bias_
/// will be clamped to [-16.0, 15.996].
///
/// \param builder NVNsamplerBuilder object to update.
///
/// \param bias Specifies the bias to add to the computed level of detail
/// when the sampler is used.
NVNAPI void NVNAPIENTRY
    nvnSamplerBuilderSetLodBias(NVNsamplerBuilder *builder,
                                float bias);

/// \brief Set the depth comparison mode used for NVNsampler objects
/// created from the NVNsamplerBuilder.
///
/// When used with textures without depth components, the depth comparison
/// mode is ignored.
///
/// \param builder NVNsamplerBuilder object to update.
///
/// \param mode Specifies whether depth comparisons are performed when
/// using the sampler.
///
/// \param func If depth comparisons are enabled, specifies the comparison
/// function used with the sampler.
NVNAPI void NVNAPIENTRY
    nvnSamplerBuilderSetCompare(NVNsamplerBuilder *builder,
                                NVNcompareMode mode,
                                NVNcompareFunc func);

/// \brief Set floating-point border color values used for NVNsampler
/// objects created from the NVNsamplerBuilder.
///
/// \param builder NVNsamplerBuilder object to update.
///
/// \param borderColor Specifies four floating-point components (R,G,B,A)
/// for the border color.
NVNAPI void NVNAPIENTRY
    nvnSamplerBuilderSetBorderColor(NVNsamplerBuilder *builder,
                                    const float *borderColor);

/// \brief Set signed integer border color values used for NVNsampler
/// objects created from the NVNsamplerBuilder.
///
/// \param builder NVNsamplerBuilder object to update.
///
/// \param borderColor Specifies four signed integer components (R,G,B,A)
/// for the border color.
NVNAPI void NVNAPIENTRY
    nvnSamplerBuilderSetBorderColori(NVNsamplerBuilder *builder,
                                     const int *borderColor);

/// \brief Set unsigned integer border color values used for NVNsampler
/// objects created from the NVNsamplerBuilder.
///
/// \param builder NVNsamplerBuilder object to update.
///
/// \param borderColor Specifies four unsigned integer components (R,G,B,A)
/// for the border color.
NVNAPI void NVNAPIENTRY
    nvnSamplerBuilderSetBorderColorui(NVNsamplerBuilder *builder,
                                      const uint32_t *borderColor);

/// \brief Set the maximum level of anisotropy used for NVNsampler objects
/// created from the NVNsamplerBuilder.
///
/// \param builder NVNsamplerBuilder object to update.
///
/// \param maxAniso Specifies the maximum level of anisotropy used with the
/// sampler.
/// Must be greater than zero.
/// Must be less than or equal to 16 (the value of
/// #NVN_DEVICE_INFO_MAX_TEXTURE_ANISOTROPY).
NVNAPI void NVNAPIENTRY
    nvnSamplerBuilderSetMaxAnisotropy(NVNsamplerBuilder *builder,
                                      float maxAniso);

/// \brief Set the reduction filter used for NVNsampler objects created
/// from the NVNsamplerBuilder.
///
/// \param builder NVNsamplerBuilder object to update.
///
/// \param filter Specifies the reduction filter used for the sampler.
NVNAPI void NVNAPIENTRY
    nvnSamplerBuilderSetReductionFilter(NVNsamplerBuilder *builder,
                                        NVNsamplerReduction filter);

/// \brief Set LOD snap used for NVNsampler objects created from the
/// NVNsamplerBuilder.
///
/// The texture LOD snap feature is an optimization that reduces texture
/// bandwidth consumption with minification filters that average samples in
/// multiple texture LODs. When performing texture lookups, hardware will
/// round (snap) the computed texture LOD to the nearest integer if the
/// difference between the computed LOD and the integer value is less than
/// the snap value in the sampler. LOD values that are not snapped are
/// adjusted so that the function adjusting the LOD value is continuous.
/// When snapping occurs, hardware will save bandwidth by sampling from
/// only one mipmap level instead of two.
///
/// \param builder NVNsamplerBuilder object to update.
///
/// \param lodSnap Specifies the LOD snap used for the sampler.
/// Specifies the LOD snap used for the sampler. Valid values are in the
/// range [0.0, 0.5], where 0.0 will result in no snapping and 0.5 would
/// round most computed LODs to integer values. Not all values are
/// supported by the hardware; the driver will select a supported snap
/// value close to the value provided in the sampler.
NVNAPI void NVNAPIENTRY
    nvnSamplerBuilderSetLodSnap(NVNsamplerBuilder *builder,
                                float lodSnap);

/// \brief Query the device associated with this NVNsamplerBuilder object.
///
/// \param builder NVNsamplerBuilder object to query.
NVNAPI const NVNdevice * NVNAPIENTRY
    nvnSamplerBuilderGetDevice(const NVNsamplerBuilder *builder);

/// \brief Query the minification and magnification filters for a
/// NVNsamplerBuilder object.
///
/// \param builder NVNsamplerBuilder object to query.
///
/// \param[out] min Minification filter for the NVNsamplerBuilder.
///
/// \param[out] mag Magnification filter for the NVNsamplerBuilder.
NVNAPI void NVNAPIENTRY
    nvnSamplerBuilderGetMinMagFilter(const NVNsamplerBuilder *builder,
                                     NVNminFilter *min,
                                     NVNmagFilter *mag);

/// \brief Query the coordinate wrap modes for a NVNsamplerBuilder object.
///
/// \param builder NVNsamplerBuilder object to query.
///
/// \param[out] s Wrap mode for the first (S) texture coordinate.
///
/// \param[out] t Wrap mode for the second (T) texture coordinate.
///
/// \param[out] r Wrap mode for the third (R) texture coordinate.
NVNAPI void NVNAPIENTRY
    nvnSamplerBuilderGetWrapMode(const NVNsamplerBuilder *builder,
                                 NVNwrapMode *s,
                                 NVNwrapMode *t,
                                 NVNwrapMode *r);

/// \brief Query the minimum and maximum level of detail in a
/// NVNsamplerBuilder object.
///
/// \param builder NVNsamplerBuilder object to query.
///
/// \param[out] min Minimum level of detail used with the sampler.
///
/// \param[out] max Maximum level of detail used with the sampler.
NVNAPI void NVNAPIENTRY
    nvnSamplerBuilderGetLodClamp(const NVNsamplerBuilder *builder,
                                 float *min,
                                 float *max);

/// \brief Query the level-of-detail bias for a NVNsamplerBuilder object.
///
/// \param builder NVNsamplerBuilder object to query.
NVNAPI float NVNAPIENTRY
    nvnSamplerBuilderGetLodBias(const NVNsamplerBuilder *builder);

/// \brief Query the depth comparison modes for a NVNsamplerBuilder object.
///
/// \param builder NVNsamplerBuilder object to query.
///
/// \param[out] mode Enable for depth comparisons in the NVNsamplerBuilder.
///
/// \param[out] func Comparison function in the NVNsamplerBuilder.
NVNAPI void NVNAPIENTRY
    nvnSamplerBuilderGetCompare(const NVNsamplerBuilder *builder,
                                NVNcompareMode *mode,
                                NVNcompareFunc *func);

/// \brief Query floating-point border color values for a NVNsamplerBuilder
/// object.
///
/// \param builder NVNsamplerBuilder object to query.
///
/// \param[out] borderColor Four floating-point components (R,G,B,A) for
/// the border color.
NVNAPI void NVNAPIENTRY
    nvnSamplerBuilderGetBorderColor(const NVNsamplerBuilder *builder,
                                    float *borderColor);

/// \brief Query signed integer border color values for a NVNsamplerBuilder
/// object.
///
/// \param builder NVNsamplerBuilder object to query.
///
/// \param[out] borderColor Four signed integer components (R,G,B,A) for
/// the border color.
NVNAPI void NVNAPIENTRY
    nvnSamplerBuilderGetBorderColori(const NVNsamplerBuilder *builder,
                                     int *borderColor);

/// \brief Query unsigned integer border color values for a
/// NVNsamplerBuilder object.
///
/// \param builder NVNsamplerBuilder object to query.
///
/// \param[out] borderColor Four unsigned integer components (R,G,B,A) for
/// the border color.
NVNAPI void NVNAPIENTRY
    nvnSamplerBuilderGetBorderColorui(const NVNsamplerBuilder *builder,
                                      uint32_t *borderColor);

/// \brief Query the maximum level of anisotropy for a NVNsamplerBuilder
/// object.
///
/// \param builder NVNsamplerBuilder object to query.
NVNAPI float NVNAPIENTRY
    nvnSamplerBuilderGetMaxAnisotropy(const NVNsamplerBuilder *builder);

/// \brief Query the reduction filter for a NVNsamplerBuilder object.
///
/// \param builder NVNsamplerBuilder object to query.
NVNAPI NVNsamplerReduction NVNAPIENTRY
    nvnSamplerBuilderGetReductionFilter(const NVNsamplerBuilder *builder);

/// \brief Query the LOD snap for a NVNsamplerBuilder object.
///
/// \param builder NVNsamplerBuilder object to query.
NVNAPI float NVNAPIENTRY
    nvnSamplerBuilderGetLodSnap(const NVNsamplerBuilder *builder);

/// \brief Initialize a NVNsampler object using the specified
/// NVNsamplerBuilder object.
///
/// \param sampler NVNsampler object to initialize.
///
/// \param builder NVNsamplerBuilder object specifying the state of the new
/// sampler object.
NVNAPI NVNboolean NVNAPIENTRY
    nvnSamplerInitialize(NVNsampler *sampler,
                         const NVNsamplerBuilder *builder);

/// \brief Finalize a NVNsampler object.
///
/// \param sampler NVNsampler object to finalize.
NVNAPI void NVNAPIENTRY
    nvnSamplerFinalize(NVNsampler *sampler);

/// \brief Sets the debug label string for a NVNsampler object.
///
/// Annotates a NVNsampler object with a descriptive text label. This label
/// may then be used by the debug layer, an external debugger, or a
/// profiler tool.
///
/// \param sampler NVNsampler object to set debug label.
///
/// \param label Text string used to annotate the NVNsampler object.
NVNAPI void NVNAPIENTRY
    nvnSamplerSetDebugLabel(NVNsampler *sampler,
                            const char * label);

/// \brief Query the minification and magnification filters for a
/// NVNsampler object.
///
/// \param sampler NVNsampler object to query.
///
/// \param[out] min Minification filter for the NVNsampler.
///
/// \param[out] mag Magnification filter for the NVNsampler.
NVNAPI void NVNAPIENTRY
    nvnSamplerGetMinMagFilter(const NVNsampler *sampler,
                              NVNminFilter *min,
                              NVNmagFilter *mag);

/// \brief Query the coordinate wrap modes for a NVNsampler object.
///
/// \param sampler NVNsampler object to query.
///
/// \param[out] s Wrap mode for the first (S) texture coordinate.
///
/// \param[out] t Wrap mode for the second (T) texture coordinate.
///
/// \param[out] r Wrap mode for the third (R) texture coordinate.
NVNAPI void NVNAPIENTRY
    nvnSamplerGetWrapMode(const NVNsampler *sampler,
                          NVNwrapMode *s,
                          NVNwrapMode *t,
                          NVNwrapMode *r);

/// \brief Query the minimum and maximum level of detail in a NVNsampler
/// object.
///
/// \param sampler NVNsampler object to query.
///
/// \param[out] min Minimum level of detail used with the sampler.
///
/// \param[out] max Maximum level of detail used with the sampler.
NVNAPI void NVNAPIENTRY
    nvnSamplerGetLodClamp(const NVNsampler *sampler,
                          float *min,
                          float *max);

/// \brief Query the level-of-detail bias for a NVNsampler object.
///
/// \param sampler NVNsampler object to query.
NVNAPI float NVNAPIENTRY
    nvnSamplerGetLodBias(const NVNsampler *sampler);

/// \brief Query the depth comparison modes for a NVNsampler object.
///
/// \param sampler NVNsampler object to query.
///
/// \param[out] mode Enable for depth comparisons in the NVNsampler.
///
/// \param[out] func Comparison function in the NVNsampler.
NVNAPI void NVNAPIENTRY
    nvnSamplerGetCompare(const NVNsampler *sampler,
                         NVNcompareMode *mode,
                         NVNcompareFunc *func);

/// \brief Query floating-point border color values for a NVNsampler
/// object.
///
/// \param sampler NVNsampler object to query.
///
/// \param[out] borderColor Four floating-point components (R,G,B,A) for
/// the border color.
NVNAPI void NVNAPIENTRY
    nvnSamplerGetBorderColor(const NVNsampler *sampler,
                             float *borderColor);

/// \brief Query signed integer border color values for a NVNsampler
/// object.
///
/// \param sampler NVNsampler object to query.
///
/// \param[out] borderColor Four signed integer components (R,G,B,A) for
/// the border color.
NVNAPI void NVNAPIENTRY
    nvnSamplerGetBorderColori(const NVNsampler *sampler,
                              int *borderColor);

/// \brief Query unsigned integer border color values for a NVNsampler
/// object.
///
/// \param sampler NVNsampler object to query.
///
/// \param[out] borderColor Four unsigned integer components (R,G,B,A) for
/// the border color.
NVNAPI void NVNAPIENTRY
    nvnSamplerGetBorderColorui(const NVNsampler *sampler,
                               uint32_t *borderColor);

/// \brief Query the maximum level of anisotropy for a NVNsampler object.
///
/// \param sampler NVNsampler object to query.
NVNAPI float NVNAPIENTRY
    nvnSamplerGetMaxAnisotropy(const NVNsampler *sampler);

/// \brief Query the reduction filter for a NVNsampler object.
///
/// \param sampler NVNsampler object to query.
NVNAPI NVNsamplerReduction NVNAPIENTRY
    nvnSamplerGetReductionFilter(const NVNsampler *sampler);

/// \brief Checks if sampler objects are equivalent.
///
/// \param sampler First sampler object.
///
/// \param otherSampler NVNsampler object to compare against.
NVNAPI NVNboolean NVNAPIENTRY
    nvnSamplerCompare(const NVNsampler *sampler,
                      const NVNsampler *otherSampler);

/// \brief Returns the debug layer unique ID of this sampler. Returns 0 if
/// debug layer disabled.
///
/// \param sampler NVNsampler object to query.
NVNAPI uint64_t NVNAPIENTRY
    nvnSamplerGetDebugID(const NVNsampler *sampler);

/// \brief Set default state for the NVNblendState object.
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
///
/// \param blend NVNblendState object to reset.
NVNAPI void NVNAPIENTRY
    nvnBlendStateSetDefaults(NVNblendState *blend);

/// \brief Specify the number of the color target to update via the
/// NVNblendState object.
///
/// \param blend NVNblendState object to update.
///
/// \param target Color target number to update when binding the blend
/// state object.
NVNAPI void NVNAPIENTRY
    nvnBlendStateSetBlendTarget(NVNblendState *blend,
                                int target);

/// \brief Specify blend functions for source and destination color and
/// alpha channels.
///
/// \param blend NVNblendState object to update.
///
/// \param srcFunc Blend factor used for source RGB color components.
///
/// \param dstFunc Blend factor used for destination RGB color components.
///
/// \param srcFuncAlpha Blend factor used for source alpha color
/// components.
///
/// \param dstFuncAlpha Blend factor used for destination alpha color
/// components.
NVNAPI void NVNAPIENTRY
    nvnBlendStateSetBlendFunc(NVNblendState *blend,
                              NVNblendFunc srcFunc,
                              NVNblendFunc dstFunc,
                              NVNblendFunc srcFuncAlpha,
                              NVNblendFunc dstFuncAlpha);

/// \brief Specify blend equations for color and alpha channels.
///
/// \param blend NVNblendState object to update.
///
/// \param modeRGB Blend equation used for RGB color components.
///
/// \param modeAlpha Blend equation used for alpha color components.
NVNAPI void NVNAPIENTRY
    nvnBlendStateSetBlendEquation(NVNblendState *blend,
                                  NVNblendEquation modeRGB,
                                  NVNblendEquation modeAlpha);

/// \brief Specify a mode for advanced blending equations.
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
///
/// When advanced blending is enabled (not set to
/// #NVN_BLEND_ADVANCED_MODE_NONE), only a single render target is
/// supported.  Advanced blending wtih multiple color targets has undefined
/// behavior and may result in to GPU errors.  Advanced modes other than
/// NONE are supported only for color target zero.
///
/// Advanced blending modes are supported on NX but are not supported on
/// the Windows reference implementation for GPUs older than
/// first-generation Maxwell GPUs.
///
/// \param blend NVNblendState object to update.
///
/// \param overlap Mode to use for advanced blending equations.
NVNAPI void NVNAPIENTRY
    nvnBlendStateSetAdvancedMode(NVNblendState *blend,
                                 NVNblendAdvancedMode overlap);

/// \brief Specify blend overlap mode for advanced blending equations.
///
/// Advanced blending in NVN provides capabilities similar to those
/// provided by the NV_blend_equation_advanced OpenGL extension.  The
/// overlap modes specify how coverage for partially covered sources and/or
/// destinations is treated in the blending equations.  This state has no
/// effect if the advanced blending mode is #NVN_BLEND_ADVANCED_MODE_NONE.
///
/// \param blend NVNblendState object to update.
///
/// \param overlap Overlap mode used for advanced blending equations.
NVNAPI void NVNAPIENTRY
    nvnBlendStateSetAdvancedOverlap(NVNblendState *blend,
                                    NVNblendAdvancedOverlap overlap);

/// \brief Specify whether the source color is considered premultiplied for
/// advanced blending equations.
///
/// Advanced blending in NVN provides capabilities similar to those
/// provided by the NV_blend_equation_advanced OpenGL extension.  By
/// default, blending is performed with pre-multiplied colors, where a
/// white pixel with 50% coverage has an (R,G,B,A) value of (0.5, 0.5, 0.5,
/// 0.5).  If NVN_FALSE is specified, the source color is assumed not to be
/// premultiplied and the same white pixel would be passed as (1.0, 1.0,
/// 1.0, 0.5).  Destination colors in advanced blending are always
/// considered pre-multiplied.  This state has no effect if the advanced
/// blending mode is #NVN_BLEND_ADVANCED_MODE_NONE.
///
/// \param blend NVNblendState object to update.
///
/// \param premultiplied TRUE if source color components are considered to
/// be pre-multiplied by the source alpha.
NVNAPI void NVNAPIENTRY
    nvnBlendStateSetAdvancedPremultipliedSrc(NVNblendState *blend,
                                             NVNboolean premultiplied);

/// \brief Specify whether the destination color target has unsigned
/// normalized components (values in [0,1]).
///
/// Advanced blending in NVN provides capabilities similar to those
/// provided by the NV_blend_equation_advanced OpenGL extension.  By
/// default, blending assumes that the destination values are stored with
/// unsigned normalized components (e.g., using formats like
/// #NVN_FORMAT_RGBA8).  If this value is set to NVN_FALSE, advanced
/// blending will not assume that the destination input and the output will
/// be clamped to [0,1] and will clamp values manually when required.  This
/// state has no effect if the advanced blending mode is
/// #NVN_BLEND_ADVANCED_MODE_NONE.
///
/// \param blend NVNblendState object to update.
///
/// \param normalized TRUE if destination color components are unsigned
/// normalized ([0,1]) values.
NVNAPI void NVNAPIENTRY
    nvnBlendStateSetAdvancedNormalizedDst(NVNblendState *blend,
                                          NVNboolean normalized);

/// \brief Query the number of the color target to update via the
/// NVNblendState object.
///
/// \param blend NVNblendState object to query.
NVNAPI int NVNAPIENTRY
    nvnBlendStateGetBlendTarget(const NVNblendState *blend);

/// \brief Query blend functions for source and destination color and alpha
/// channels.
///
/// \param blend NVNblendState object to query.
///
/// \param[out] srcFunc Blend factor used for source RGB color components.
///
/// \param[out] dstFunc Blend factor used for destination RGB color
/// components.
///
/// \param[out] srcFuncAlpha Blend factor used for source alpha color
/// components.
///
/// \param[out] dstFuncAlpha Blend factor used for destination alpha color
/// components.
NVNAPI void NVNAPIENTRY
    nvnBlendStateGetBlendFunc(const NVNblendState *blend,
                              NVNblendFunc *srcFunc,
                              NVNblendFunc *dstFunc,
                              NVNblendFunc *srcFuncAlpha,
                              NVNblendFunc *dstFuncAlpha);

/// \brief Query blend equations for color and alpha channels.
///
/// \param blend NVNblendState object to query.
///
/// \param[out] modeRGB Blend equation used for RGB color components.
///
/// \param[out] modeAlpha Blend equation used for alpha color components.
NVNAPI void NVNAPIENTRY
    nvnBlendStateGetBlendEquation(const NVNblendState *blend,
                                  NVNblendEquation *modeRGB,
                                  NVNblendEquation *modeAlpha);

/// \brief Query the mode for advanced blending equations.
///
/// \param blend NVNblendState object to query.
NVNAPI NVNblendAdvancedMode NVNAPIENTRY
    nvnBlendStateGetAdvancedMode(const NVNblendState *blend);

/// \brief Query the blend overlap mode for advanced blending equations.
///
/// \param blend NVNblendState object to query.
NVNAPI NVNblendAdvancedOverlap NVNAPIENTRY
    nvnBlendStateGetAdvancedOverlap(const NVNblendState *blend);

/// \brief Query whether the source color is considered premultiplied for
/// advanced blending equations.
///
/// \param blend NVNblendState object to query.
NVNAPI NVNboolean NVNAPIENTRY
    nvnBlendStateGetAdvancedPremultipliedSrc(const NVNblendState *blend);

/// \brief Query whether the destination color target has unsigned
/// normalized components (values in [0,1]).
///
/// \param blend NVNblendState object to query.
NVNAPI NVNboolean NVNAPIENTRY
    nvnBlendStateGetAdvancedNormalizedDst(const NVNblendState *blend);

/// \brief Set default state for the NVNcolorState object.
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
///
/// \param color NVNcolorState object to update.
NVNAPI void NVNAPIENTRY
    nvnColorStateSetDefaults(NVNcolorState *color);

/// \brief Specifies the blending enable for a single color target.
///
/// \param color NVNcolorState object to update.
///
/// \param index Number of the color target to update.
/// Must be less than 8 (the value of
/// #NVN_DEVICE_INFO_COLOR_BUFFER_BINDINGS).
///
/// \param enable TRUE if blending should be enabled for the specified
/// target; FALSE, otherwise.
NVNAPI void NVNAPIENTRY
    nvnColorStateSetBlendEnable(NVNcolorState *color,
                                int index,
                                NVNboolean enable);

/// \brief Specifies the logical operation to perform for all color
/// targets.
///
/// If programmed to anything but COPY, arithmetic blending has no effect.
/// Unlike blending, there is no support for separate logical operations
/// for each color target.
///
/// \param color NVNcolorState object to update.
///
/// \param logicOp Logical operation to perform on all color targets.
NVNAPI void NVNAPIENTRY
    nvnColorStateSetLogicOp(NVNcolorState *color,
                            NVNlogicOp logicOp);

/// \brief Specifies a comparison function to use for alpha testing.
///
/// Using #NVN_ALPHA_FUNC_ALWAYS disables the alpha test entirely.
///
/// \note Applications may get better performance from performing an alpha
/// test in the fragment shader and discarding fragments that fail.  Doing
/// so allows the fragment shader to skip other color calculations for
/// failing fragments.
///
/// \param color NVNcolorState object to update.
///
/// \param alphaTest Comparison function used by the alpha test.
NVNAPI void NVNAPIENTRY
    nvnColorStateSetAlphaTest(NVNcolorState *color,
                              NVNalphaFunc alphaTest);

/// \brief Query the blending enable for a single color target.
///
/// \param color NVNcolorState object to query.
///
/// \param index Number of the color target to query.
/// Must be less than 8 (the value of
/// #NVN_DEVICE_INFO_COLOR_BUFFER_BINDINGS).
NVNAPI NVNboolean NVNAPIENTRY
    nvnColorStateGetBlendEnable(const NVNcolorState *color,
                                int index);

/// \brief Query the logical operation to perform for all color targets.
///
/// \param color NVNcolorState object to query.
NVNAPI NVNlogicOp NVNAPIENTRY
    nvnColorStateGetLogicOp(const NVNcolorState *color);

/// \brief Query the comparison function to use for alpha testing.
///
/// \param color NVNcolorState object to query.
NVNAPI NVNalphaFunc NVNAPIENTRY
    nvnColorStateGetAlphaTest(const NVNcolorState *color);

/// \brief Set default state for the NVNchannelMaskState object.
///
/// \note This is semantically equivalent to calling
///
/// \code{.c}
///   for (NVNuint i = 0; i < DeviceInfo::COLOR_BUFFER_BINDINGS; ++i) {
///     ChannelMaskState::SetChannelMask(i, NVN_TRUE, NVN_TRUE,
///                                         NVN_TRUE, NVN_TRUE);
///   };
/// \endcode
///
/// \param channelMask NVNchannelMaskState object to reset.
NVNAPI void NVNAPIENTRY
    nvnChannelMaskStateSetDefaults(NVNchannelMaskState *channelMask);

/// \brief NVNprogram RGBA channel masks for a single color target.
///
/// \param channelMask NVNchannelMaskState object to update.
///
/// \param index Number of the color target to update.
/// Must be less than 8 (the value of
/// #NVN_DEVICE_INFO_COLOR_BUFFER_BINDINGS).
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
NVNAPI void NVNAPIENTRY
    nvnChannelMaskStateSetChannelMask(NVNchannelMaskState *channelMask,
                                      int index,
                                      NVNboolean r,
                                      NVNboolean g,
                                      NVNboolean b,
                                      NVNboolean a);

/// \brief Query RGBA channel masks for a single color target.
///
/// \param channelMask NVNchannelMaskState object to query.
///
/// \param index Number of the color target to query.
/// Must be less than 8 (the value of
/// #NVN_DEVICE_INFO_COLOR_BUFFER_BINDINGS).
///
/// \param[out] r Enable (TRUE) or disable (FALSE) writes to the first (R)
/// component.
///
/// \param[out] g Enable (TRUE) or disable (FALSE) writes to the second (G)
/// component.
///
/// \param[out] b Enable (TRUE) or disable (FALSE) writes to the third (B)
/// component.
///
/// \param[out] a Enable (TRUE) or disable (FALSE) writes to the fourth (A)
/// component.
NVNAPI void NVNAPIENTRY
    nvnChannelMaskStateGetChannelMask(const NVNchannelMaskState *channelMask,
                                      int index,
                                      NVNboolean *r,
                                      NVNboolean *g,
                                      NVNboolean *b,
                                      NVNboolean *a);

/// \brief Set default state for the NVNmultisampleState object.
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
/// \endcode All sample locations in the NVNmultisampleState object are
/// initialized to pixel centers (0.5).
///
/// \param multisample NVNmultisampleState object to reset.
NVNAPI void NVNAPIENTRY
    nvnMultisampleStateSetDefaults(NVNmultisampleState *multisample);

/// \brief Enable or disable multisample rasterization.
///
/// \param multisample NVNmultisampleState object to update.
///
/// \param enable Enable (TRUE) or disable (FALSE) multisample
/// rasterization.
/// When multisample rasterization is enabled, separate coverage
/// information is computed for every sample in a pixel.  When multisample
/// rasterization is disabled, a single coverage value is used for all
/// samples in a pixel.
NVNAPI void NVNAPIENTRY
    nvnMultisampleStateSetMultisampleEnable(NVNmultisampleState *multisample,
                                            NVNboolean enable);

/// \brief Specify the number of samples in the framebuffer.
///
/// \warning If the sample count in the NVNmultisampleState object and the
/// textures in the framebuffer don't match, the results of rendering are
/// undefined.
///
/// \param multisample NVNmultisampleState object to update.
///
/// \param samples Specifies the number of samples in the framebuffer.
/// If non-multisample framebuffers are used, the value should be specified
/// as 0.
NVNAPI void NVNAPIENTRY
    nvnMultisampleStateSetSamples(NVNmultisampleState *multisample,
                                  int samples);

/// \brief Enable or disable alpha-to-coverage operation.
///
/// When alpha-to-coverage operations are enabled, the fourth (A) component
/// of fragment color zero is used to produce a sample mask that is
/// combined with raster coverage to produce an updated sample mask.  An
/// alpha value of 0.0 disables all samples; an alpha value of 1.0 enables
/// all samples.
///
/// \param multisample NVNmultisampleState object to update.
///
/// \param enable Enable (TRUE) or disable (FALSE) the alpha-to-coverage
/// operation.
NVNAPI void NVNAPIENTRY
    nvnMultisampleStateSetAlphaToCoverageEnable(NVNmultisampleState *multisample,
                                                NVNboolean enable);

/// \brief Enable or disable dithering in the alpha-to-coverage operation.
///
/// When alpha-to-coverage operations are enabled via
/// MultisampleState::SetAlphaToCoverage, the fourth (A) component of
/// fragment color zero is used to produce a sample mask that is combined
/// with raster coverage to produce an updated sample mask.  An alpha value
/// of 0.0 disables all samples; an alpha value of 1.0 enables all samples.
///  If alpha-to-coverage dithering is enabled by this command, the same
/// alpha value will produce different sample masks for different pixels.
/// If dithering is disabled, the same alpha value will produce the same
/// sample mask in all pixels.  Dithering will allow for more effective
/// levels of transparency, but may result in more visible noise.
/// Dithering has no effect if alpha-to-coverage is disabled.
///
/// \param multisample NVNmultisampleState object to update.
///
/// \param dither Enable (TRUE) or disable (FALSE) the alpha-to-coverage
/// dithering operation.
NVNAPI void NVNAPIENTRY
    nvnMultisampleStateSetAlphaToCoverageDither(NVNmultisampleState *multisample,
                                                NVNboolean dither);

/// \brief Query the multisample rasterization enable.
///
/// \param multisample NVNmultisampleState object to query.
NVNAPI NVNboolean NVNAPIENTRY
    nvnMultisampleStateGetMultisampleEnable(const NVNmultisampleState *multisample);

/// \brief Query the number of samples in a NVNmultisampleState object.
///
/// \param multisample NVNmultisampleState object to query.
NVNAPI int NVNAPIENTRY
    nvnMultisampleStateGetSamples(const NVNmultisampleState *multisample);

/// \brief Query the alpha-to-coverage enable.
///
/// \param multisample NVNmultisampleState object to query.
NVNAPI NVNboolean NVNAPIENTRY
    nvnMultisampleStateGetAlphaToCoverageEnable(const NVNmultisampleState *multisample);

/// \brief Query the alpha-to-coverage dithering enable.
///
/// \param multisample NVNmultisampleState object to query.
NVNAPI NVNboolean NVNAPIENTRY
    nvnMultisampleStateGetAlphaToCoverageDither(const NVNmultisampleState *multisample);

/// \brief Specify the raster sample count for target-independent
/// rasterization.
///
/// When using a non-zero value (supported:  2, 4, 8, and 16), this command
/// enables target-independent rasterization, where the rasterizer
/// evaluates sample coverage using the specified number of sample
/// locations.  This raster sample count may differ from the number of
/// stored samples in the framebuffer.  When using a zero value (default),
/// the command disables target-independent rasterization and has the
/// rasterizer evaluate sample coverage based on the stored samples in the
/// framebuffer.
///
/// \param multisample NVNmultisampleState object to update.
///
/// \param rasterSamples Raster sample count.
/// If depth or stencil tests are performed with target-independent
/// rasterization, the raster sample count must be equal to the number of
/// samples in the depth/stencil render target.  If non-zero, the raster
/// sample count must be greater than or equal to the color sample count
/// set by nvnMultisampleStateSetSamples.
NVNAPI void NVNAPIENTRY
    nvnMultisampleStateSetRasterSamples(NVNmultisampleState *multisample,
                                        int rasterSamples);

/// \brief Query the raster sample count for target-independent
/// rasterization.
///
/// \param multisample NVNmultisampleState object to query.
NVNAPI int NVNAPIENTRY
    nvnMultisampleStateGetRasterSamples(NVNmultisampleState *multisample);

/// \brief Set the coverage modulation mode for target-independent
/// rasterization.
///
/// When target-independent rasterization is enabled, pixel or sample color
/// values in the framebuffer are updated whenever any of the associated
/// raster samples are covered by a primitive.  Applications can enable
/// coverage modulation to account for partial coverage, where color (RGB)
/// or alpha components are multiplied by a factor that reflects the
/// fraction of raster samples that are covered.  This modulation mode
/// allows applications to specify that some, all, or none of the color
/// components are multiplied by this factor.
///
/// \param multisample NVNmultisampleState object to update.
///
/// \param mode Specifies the set of color channels to multiply by the
/// coverage modulation factor.
NVNAPI void NVNAPIENTRY
    nvnMultisampleStateSetCoverageModulationMode(NVNmultisampleState *multisample,
                                                 NVNcoverageModulationMode mode);

/// \brief Query the coverage modulation mode for target-independent
/// rasterization.
///
/// \param multisample NVNmultisampleState object to query.
NVNAPI NVNcoverageModulationMode NVNAPIENTRY
    nvnMultisampleStateGetCoverageModulationMode(const NVNmultisampleState *multisample);

/// \brief Enable or disable the fragment coverage-to-color feature.
///
/// When enabled using nvnMultisampleStateSetCoverageToColorEnable, the
/// fragment coverage-to-color feature writes a coverage mask for each
/// fragment into the red component of the fragment color for one color
/// output.  The color output number recieving the coverage mask is
/// specified by nvnMultisampleStateSetCoverageToColorOutput.
///
/// The coverage mask used for coverage-to-color reflects not only raster
/// sample coverage, but also the results of per-fragment operations
/// including other sample masks, alpha to coverage, alpha test, and the
/// depth bounds, depth, and stencil tests.
///
/// \warning When coverage-to-color is enabled, the color target receiving
/// the coverage mask must have a one-component format with signed or
/// unsigned integer components (e.g., R8I, R32UI).  Coverage-to-color has
/// no effect if other color target formats are used.
///
/// \param multisample NVNmultisampleState object to set.
///
/// \param enable Fragment coverage-to-color enable.
NVNAPI void NVNAPIENTRY
    nvnMultisampleStateSetCoverageToColorEnable(NVNmultisampleState *multisample,
                                                NVNboolean enable);

/// \brief Query the enable for the fragment coverage-to-color feature.
///
/// \param multisample NVNmultisampleState object to query.
NVNAPI NVNboolean NVNAPIENTRY
    nvnMultisampleStateGetCoverageToColorEnable(const NVNmultisampleState *multisample);

/// \brief Specify the color output updated by the fragment
/// coverage-to-color feature.
///
/// When enabled using nvnMultisampleStateSetCoverageToColorEnable, the
/// fragment coverage-to-color feature writes a coverage mask for each
/// fragment into the red component of the fragment color for one color
/// output.  The color output number recieving the coverage mask is
/// specified by nvnMultisampleStateSetCoverageToColorOutput.
///
/// The coverage mask used for coverage-to-color reflects not only raster
/// sample coverage, but also the results of per-fragment operations
/// including other sample masks, alpha to coverage, alpha test, and the
/// depth bounds, depth, and stencil tests.
///
/// \warning When coverage-to-color is enabled, the color target receiving
/// the coverage mask must have a one-component format with signed or
/// unsigned integer components (e.g., R8I, R32UI).  Coverage-to-color has
/// no effect if other color target formats are used.
///
/// \param multisample NVNmultisampleState object to update.
///
/// \param color Color output number updated by the fragment
/// coverage-to-color feature.
NVNAPI void NVNAPIENTRY
    nvnMultisampleStateSetCoverageToColorOutput(NVNmultisampleState *multisample,
                                                int color);

/// \brief Query the color output updated by the fragment coverage-to-color
/// feature.
///
/// \param multisample NVNmultisampleState object to query.
NVNAPI int NVNAPIENTRY
    nvnMultisampleStateGetCoverageToColorOutput(const NVNmultisampleState *multisample);

/// \brief Enable or disable programmable sample locations.
///
/// By default, rasterization in NVN uses a set of fixed sample locations
/// based on the total number of raster samples.  When programmable sample
/// locations are enabled, rasterization instead uses a set of locations
/// from the NVNmultisampleState object programmed using
/// nvnMultisampleStateSetSampleLocations.
///
/// \param multisample NVNmultisampleState object to update.
///
/// \param enable Enable/disable programmble sample locations.
NVNAPI void NVNAPIENTRY
    nvnMultisampleStateSetSampleLocationsEnable(NVNmultisampleState *multisample,
                                                NVNboolean enable);

/// \brief Query the programmable sample locations enable.
///
/// \param multisample NVNmultisampleState object to query.
NVNAPI NVNboolean NVNAPIENTRY
    nvnMultisampleStateGetSampleLocationsEnable(const NVNmultisampleState *multisample);

/// \brief Query the size of a pixel grid used for programmable sample
/// locations.
///
/// When programmable sample locations are enabled, the rasterizer can use
/// the same set of locations for each pixel, or use a different set of
/// locations for each pixel in a small fixed-size grid, based on the
/// enable set by nvnMultisampleStateSetSampleLocationsGridEnable.  This
/// function returns the width and height of the fixed-size grid used when
/// enabled.  The grid will have a total of 16/N pixels, where N is the
/// total number of raster samples in the multisample state object.  Please
/// refer to the documentation of nvnMultisampleStateSetSampleLocations for
/// more details.
///
/// \param multisample NVNmultisampleState object to query.
///
/// \param[out] width Returns the width (in pixels) of the sample location
/// pixel grid.
///
/// \param[out] height Returns the height (in pixels) of the sample
/// location pixel grid.
NVNAPI void NVNAPIENTRY
    nvnMultisampleStateGetSampleLocationsGrid(NVNmultisampleState *multisample,
                                              int *width,
                                              int *height);

/// \brief Set the programmable sample locations grid enable.
///
/// When programmable sample locations are enabled, the rasterizer can use
/// the same set of locations for each pixel (disabled), or use a different
/// set of locations for each pixel in a small fixed-size grid.
///
/// \param multisample NVNmultisampleState object to update.
///
/// \param enable Enable or disable the programmble sample locations grid.
/// If set to NVN_TRUE, different sample locations are used for each pixel
/// in a fixed-size grid.  If set to NVN_FALSE, the same locations are used
/// for all pixels.
NVNAPI void NVNAPIENTRY
    nvnMultisampleStateSetSampleLocationsGridEnable(NVNmultisampleState *multisample,
                                                    NVNboolean enable);

/// \brief Query the programmable sample locations grid enable.
///
/// \param multisample NVNmultisampleState object to query.
NVNAPI NVNboolean NVNAPIENTRY
    nvnMultisampleStateGetSampleLocationsGridEnable(const NVNmultisampleState *multisample);

/// \brief Specifies programmable sample locations for rasterization.
///
/// When programmable sample locations are enabled using
/// nvnMultisampleStateSetSampleLocationsEnable, rasterization uses a set
/// of sample locations programmed in the multisample state object using
/// this command.  Each sample location is a pair of (x,y) values
/// identifying the location of a sample within its pixel.
///
/// The rasterizer may use the same sample locations for each pixel or
/// varying sample locations within a repeating pixel grid, depending on
/// state set using nvnMultisampleStateSetSampleLocationsGridEnable.  If
/// the pixel grid is disabled, a sample with index _s_ in the pixel with
/// coordinates (x,y) always uses entry _s_ in the array of sample
/// locations.  If the pixel grid is enabled and the grid has a width and
/// height of _gw_ and _gh_, respectively, sample _s_ in pixel (x,y) uses
/// entry _s_ + (_y_ % _gh_) * _gw_ + (_x_ % _gw_).
///
/// \param multisample NVNmultisampleState object to update.
///
/// \param start Index of the first sample location to update.
///
/// \param count Number of sample locations to update.
///
/// \param values Array of sample location value pairs.
/// _values_ is an array of 2 * _count_ floating-point values, where each
/// sample (x,y) location is specified using two values in the range [0.0,
/// 1.0).  The pair (0.5, 0.5) identifies the center of a pixel.  Sample
/// locations are converted to fixed-point values when the multisample
/// state object is updated.
NVNAPI void NVNAPIENTRY
    nvnMultisampleStateSetSampleLocations(NVNmultisampleState *multisample,
                                          int start,
                                          int count,
                                          const float *values);

/// \brief Set default state for the NVNpolygonState object.
///
/// \note This is semantically equivalent to calling
///
/// \code{.c}
///   PolygonState::SetCullFace(NVN_FACE_NONE);
///   PolygonState::SetFrontFace(NVN_FRONT_FACE_CCW);
///   PolygonState::SetPolygonMode(NVN_POLYGON_MODE_FILL);
///   PolygonState::SetPolygonOffsetEnables(0);
/// \endcode
///
/// \param polygon NVNpolygonState object to reset.
NVNAPI void NVNAPIENTRY
    nvnPolygonStateSetDefaults(NVNpolygonState *polygon);

/// \brief Specify controls for culling of front- and back-facing polygons.
///
/// \param polygon NVNpolygonState object to update.
///
/// \param face Specifies the set of faces (if any) that should be cullled.
NVNAPI void NVNAPIENTRY
    nvnPolygonStateSetCullFace(NVNpolygonState *polygon,
                               NVNface face);

/// \brief Specify whether clockwise or counter-clockwise polygons are
/// considered front-facing.
///
/// \param polygon NVNpolygonState object to update.
///
/// \param face Treat clockwise (CW) or counter-clockwise (CCW) polygons as
/// front-facing.
NVNAPI void NVNAPIENTRY
    nvnPolygonStateSetFrontFace(NVNpolygonState *polygon,
                                NVNfrontFace face);

/// \brief Specifies whether polygons are rendered as points, lines, or
/// triangles.
///
/// \param polygon NVNpolygonState object to update.
///
/// \param polygonMode Render polygons as points (POINT), lines (LINE), or
/// triangles (FILL).
NVNAPI void NVNAPIENTRY
    nvnPolygonStateSetPolygonMode(NVNpolygonState *polygon,
                                  NVNpolygonMode polygonMode);

/// \brief Specify whether polygon offset is applied for point-mode,
/// line-mode, and filled polygons.
///
/// \param polygon NVNpolygonState object to update.
///
/// \param enables Bitfield indicating whether polygon offset is applied
/// for point-mode, line-mode, and filled polygons. See
/// #NVNpolygonOffsetEnable.
NVNAPI void NVNAPIENTRY
    nvnPolygonStateSetPolygonOffsetEnables(NVNpolygonState *polygon,
                                           int enables);

/// \brief Query the control for culling of front- and back-facing
/// polygons.
///
/// \param polygon NVNpolygonState object to query.
NVNAPI NVNface NVNAPIENTRY
    nvnPolygonStateGetCullFace(const NVNpolygonState *polygon);

/// \brief Query whether clockwise or counter-clockwise polygons are
/// considered front-facing.
///
/// \param polygon NVNpolygonState object to query.
NVNAPI NVNfrontFace NVNAPIENTRY
    nvnPolygonStateGetFrontFace(const NVNpolygonState *polygon);

/// \brief Query whether polygons are rendered as points, lines, or
/// triangles.
///
/// \param polygon NVNpolygonState object to query.
NVNAPI NVNpolygonMode NVNAPIENTRY
    nvnPolygonStateGetPolygonMode(const NVNpolygonState *polygon);

/// \brief Query whether polygon offset is applied for point-mode,
/// line-mode, and filled polygons.
///
/// \param polygon NVNpolygonState object to query.
NVNAPI NVNpolygonOffsetEnable NVNAPIENTRY
    nvnPolygonStateGetPolygonOffsetEnables(const NVNpolygonState *polygon);

/// \brief Set default state for the NVNdepthStencilState object.
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
///
/// \param depthStencil NVNdepthStencilState object to reset.
NVNAPI void NVNAPIENTRY
    nvnDepthStencilStateSetDefaults(NVNdepthStencilState *depthStencil);

/// \brief Specify whether depth testing is enabled or disabled.
///
/// \param depthStencil NVNdepthStencilState object to update.
///
/// \param enable Enable (TRUE) or disable (FALSE) depth testing.
NVNAPI void NVNAPIENTRY
    nvnDepthStencilStateSetDepthTestEnable(NVNdepthStencilState *depthStencil,
                                           NVNboolean enable);

/// \brief Specify whether the depth buffer should be updated by the depth
/// test.
///
/// If depth testing is disabled, the depth buffer is not updated,
/// regardless of the depth write enable.
///
/// \param depthStencil NVNdepthStencilState object to update.
///
/// \param enable Enable (TRUE) or disable (FALSE) writes to the depth
/// buffer.
NVNAPI void NVNAPIENTRY
    nvnDepthStencilStateSetDepthWriteEnable(NVNdepthStencilState *depthStencil,
                                            NVNboolean enable);

/// \brief Specifies the comparison function used when depth testing is
/// enabled.
///
/// \param depthStencil NVNdepthStencilState object to update.
///
/// \param func Comparison function used when depth testing is enabled.
/// If depth testing is disabled, the depth function has no effect.
NVNAPI void NVNAPIENTRY
    nvnDepthStencilStateSetDepthFunc(NVNdepthStencilState *depthStencil,
                                     NVNdepthFunc func);

/// \brief Specify whether the stencil testing is enabled or disabled.
///
/// \param depthStencil NVNdepthStencilState object to update.
///
/// \param enable Enable (TRUE) or disable (FALSE) stencil testing.
NVNAPI void NVNAPIENTRY
    nvnDepthStencilStateSetStencilTestEnable(NVNdepthStencilState *depthStencil,
                                             NVNboolean enable);

/// \brief Specifies the comparison function used when stencil testing is
/// enabled.
///
/// There are separate stencil functions for front- and back-facing
/// primitives. This function can update the value of either or both
/// functions in the NVNdepthStencilState object.  If stencil testing is
/// disabled, the stencil function has no effect.
///
/// \param depthStencil NVNdepthStencilState object to update.
///
/// \param faces Specifies the face(s) (front and/or back) using the new
/// stencil function.
///
/// \param func Comparison function used when stencil testing is enabled.
NVNAPI void NVNAPIENTRY
    nvnDepthStencilStateSetStencilFunc(NVNdepthStencilState *depthStencil,
                                       NVNface faces,
                                       NVNstencilFunc func);

/// \brief Specifies operations performed on stored stencil values by the
/// stencil test.
///
/// There are separate sets of stencil operations for front- and
/// back-facing primitives. This function can update the value of either or
/// both sets of operations in the NVNdepthStencilState object.  If stencil
/// testing is disabled, the stencil operations have no effect.
///
/// \param depthStencil NVNdepthStencilState object to update.
///
/// \param faces Specifies the face(s) (front and/or back) using the new
/// stencil operations.
///
/// \param fail Operation performed on the stencil value when the stencil
/// test fails.
///
/// \param depthFail Operation performed on the stencil value when the
/// stencil test passes and the depth test fails.
///
/// \param depthPass Operation performed on the stencil value when the
/// stencil and depth tests pass.
NVNAPI void NVNAPIENTRY
    nvnDepthStencilStateSetStencilOp(NVNdepthStencilState *depthStencil,
                                     NVNface faces,
                                     NVNstencilOp fail,
                                     NVNstencilOp depthFail,
                                     NVNstencilOp depthPass);

/// \brief Query whether depth testing is enabled or disabled.
///
/// \param depthStencil NVNdepthStencilState object to query.
NVNAPI NVNboolean NVNAPIENTRY
    nvnDepthStencilStateGetDepthTestEnable(const NVNdepthStencilState *depthStencil);

/// \brief Query whether the depth buffer should be updated by the depth
/// test.
///
/// \param depthStencil NVNdepthStencilState object to query.
NVNAPI NVNboolean NVNAPIENTRY
    nvnDepthStencilStateGetDepthWriteEnable(const NVNdepthStencilState *depthStencil);

/// \brief Query the comparison function used when depth testing is
/// enabled.
///
/// \param depthStencil NVNdepthStencilState object to query.
NVNAPI NVNdepthFunc NVNAPIENTRY
    nvnDepthStencilStateGetDepthFunc(const NVNdepthStencilState *depthStencil);

/// \brief Query whether the stencil testing is enabled or disabled.
///
/// \param depthStencil NVNdepthStencilState object to query.
NVNAPI NVNboolean NVNAPIENTRY
    nvnDepthStencilStateGetStencilTestEnable(const NVNdepthStencilState *depthStencil);

/// \brief Query the comparison function used when stencil testing is
/// enabled.
///
/// \param depthStencil NVNdepthStencilState object to query.
///
/// \param face Specifies the face (front or back) to query.
NVNAPI NVNstencilFunc NVNAPIENTRY
    nvnDepthStencilStateGetStencilFunc(const NVNdepthStencilState *depthStencil,
                                       NVNface face);

/// \brief Query operations performed on stored stencil values by the
/// stencil test.
///
/// \param depthStencil NVNdepthStencilState object to query.
///
/// \param face Specifies the face (front or back) to query the stencil
/// operations.
///
/// \param[out] fail Operation performed on the stencil value when the
/// stencil test fails.
///
/// \param[out] depthFail Operation performed on the stencil value when the
/// stencil test passes and the depth test fails.
///
/// \param[out] depthPass Operation performed on the stencil value when the
/// stencil and depth tests pass.
NVNAPI void NVNAPIENTRY
    nvnDepthStencilStateGetStencilOp(const NVNdepthStencilState *depthStencil,
                                     NVNface face,
                                     NVNstencilOp *fail,
                                     NVNstencilOp *depthFail,
                                     NVNstencilOp *depthPass);

/// \brief Set default state for the NVNvertexAttribState object.
///
/// \note This is semantically equivalent to calling
///
/// \code{.c}
///   VertexAttribState::SetFormat(NVN_FORMAT_NONE, 0);
///   VertexAttribState::SetStreamIndex(0);
/// \endcode
///
/// except that the object will be disabled until the first valid call to
/// nvnVertexAttribStateSetFormat().
///
/// \param attrib NVNvertexAttribState object to reset.
NVNAPI void NVNAPIENTRY
    nvnVertexAttribStateSetDefaults(NVNvertexAttribState *attrib);

/// \brief Specifies the format (in memory) for values for a single vertex
/// attribute. Specifying format is mandatory, and doing so also enables
/// the vertex attribute for fetching.
///
/// \param attrib NVNvertexAttribState object to update.
///
/// \param format #NVNformat of the specified vertex attribute.
///
/// \param relativeOffset Offset (in bytes) of the values for the vertex
/// attribute relative to the base address from the vertex buffer binding.
NVNAPI void NVNAPIENTRY
    nvnVertexAttribStateSetFormat(NVNvertexAttribState *attrib,
                                  NVNformat format,
                                  ptrdiff_t relativeOffset);

/// \brief Specifies the vertex attribute stream binding index.
///
/// \param attrib NVNvertexAttribState object to update.
///
/// \param streamIndex Index of the stream to use for the attribute.
/// Must be less than 16 (the value of
/// #NVN_DEVICE_INFO_VERTEX_BUFFER_BINDINGS).
NVNAPI void NVNAPIENTRY
    nvnVertexAttribStateSetStreamIndex(NVNvertexAttribState *attrib,
                                       int streamIndex);

/// \brief Query the format (in memory) for values for a single vertex
/// attribute.
///
/// \param attrib NVNvertexAttribState object to query.
///
/// \param[out] format #NVNformat of the specified vertex attribute.
///
/// \param[out] relativeOffset Offset (in bytes) of the values for the
/// vertex attribute relative to the base address from the vertex buffer
/// binding.
NVNAPI void NVNAPIENTRY
    nvnVertexAttribStateGetFormat(const NVNvertexAttribState *attrib,
                                  NVNformat *format,
                                  ptrdiff_t *relativeOffset);

/// \brief Query the vertex attribute stream binding index.
///
/// \param attrib NVNvertexAttribState object to query.
NVNAPI int NVNAPIENTRY
    nvnVertexAttribStateGetStreamIndex(const NVNvertexAttribState *attrib);

/// \brief Set default state for the NVNvertexStreamState object.
///
/// \note This is semantically equivalent to calling
///
/// \code{.c}
///   VertexStreamState::SetDivisor(0);
///   VertexStreamState::SetStride(0);
/// \endcode
///
/// \param stream NVNvertexStreamState object to reset.
NVNAPI void NVNAPIENTRY
    nvnVertexStreamStateSetDefaults(NVNvertexStreamState *stream);

/// \brief Specifies the stride (in bytes) for vertex attribute records
/// fetched from a single vertex binding point.
///
/// \param stream NVNvertexStreamState object to update.
///
/// \param stride Stride (in bytes) between vertex attribute records for
/// the specified binding point.
NVNAPI void NVNAPIENTRY
    nvnVertexStreamStateSetStride(NVNvertexStreamState *stream,
                                  ptrdiff_t stride);

/// \brief Specifies the frequency divisor for vertices in vertex binding
/// N.
///
/// If zero, specifies that the vertex binding holds a separate set of
/// values for each vertex in a rendered primitive.  If set to a non-zero
/// value X, a separate set of values will be used for each group of X
/// instances, with the same set of values used for each vertex in a single
/// instance.
///
/// \param stream NVNvertexStreamState object to update.
///
/// \param divisor Frequency divisor for vertex attributes using the
/// specified binding point.
NVNAPI void NVNAPIENTRY
    nvnVertexStreamStateSetDivisor(NVNvertexStreamState *stream,
                                   int divisor);

/// \brief Query the stride (in bytes) for a NVNvertexStreamState object.
///
/// \param stream NVNvertexStreamState object to query.
NVNAPI ptrdiff_t NVNAPIENTRY
    nvnVertexStreamStateGetStride(const NVNvertexStreamState *stream);

/// \brief Query the frequency divisor in a NVNvertexStreamState object.
///
/// \param stream NVNvertexStreamState object to query.
NVNAPI int NVNAPIENTRY
    nvnVertexStreamStateGetDivisor(const NVNvertexStreamState *stream);

/// \brief Initializes a NVNcommandBuffer object for the specified device.
///
/// \param cmdBuf Commandbuffer object to initialize.
///
/// \param device NVNdevice owning the NVNcommandBuffer object.
NVNAPI NVNboolean NVNAPIENTRY
    nvnCommandBufferInitialize(NVNcommandBuffer *cmdBuf,
                               NVNdevice *device);

/// \brief Finalize a NVNcommandBuffer object.
///
/// \param cmdBuf NVNcommandBuffer object to finalize.
NVNAPI void NVNAPIENTRY
    nvnCommandBufferFinalize(NVNcommandBuffer *cmdBuf);

/// \brief Sets the debug label string for a NVNcommandBuffer object.
///
/// Annotates a NVNcommandBuffer object with a descriptive text label. This
/// label may then be used by the debug layer, an external debugger, or a
/// profiler tool.
///
/// \param cmdBuf NVNcommandBuffer object to set debug label.
///
/// \param label Text string used to annotate the NVNcommandBuffer object.
NVNAPI void NVNAPIENTRY
    nvnCommandBufferSetDebugLabel(NVNcommandBuffer *cmdBuf,
                                  const char * label);

/// \brief Set a callback function for a NVNcommandBuffer object that will
/// be called if it runs out of memory.
///
/// \param cmdBuf NVNcommandBuffer object to update.
///
/// \param callback Callback function to use on out-of-memory events.
NVNAPI void NVNAPIENTRY
    nvnCommandBufferSetMemoryCallback(NVNcommandBuffer *cmdBuf,
                                      PFNNVNCOMMANDBUFFERMEMORYCALLBACKPROC callback);

/// \brief Specify a generic pointer that will be passed to a callback
/// function if the command buffer runs out of memory.
///
/// \param cmdBuf NVNcommandBuffer object to update.
///
/// \param[in,out] callbackData Callback data to be passed in an
/// out-of-memory event callback.
NVNAPI void NVNAPIENTRY
    nvnCommandBufferSetMemoryCallbackData(NVNcommandBuffer *cmdBuf,
                                          void *callbackData);

/// \brief Enable or disable the memory callback when out of command
/// memory.
///
/// If a sequence of commands is known to fit within allocated command
/// memory, applications can prevent the driver from conservatively
/// invoking the out-of-memory callback when previously allocated command
/// memory is nearly exhausted.  This function does not affect behavior
/// related to control memory.
///
/// \param cmdBuf NVNcommandBuffer object to update.
///
/// \param enabled True (default) if the out-of-memory callback should be
/// enabled; false otherwise.
NVNAPI void NVNAPIENTRY
    nvnCommandBufferSetCommandMemoryCallbackEnabled(NVNcommandBuffer *cmdBuf,
                                                    NVNboolean enabled);

/// \brief Add command data memory for a NVNcommandBuffer object. This can
/// be called at any time, even outside an out-of-memory callback or while
/// recording or not recording. Any leftover space in the previous command
/// memory will remain unused. Only the last provided block is consumed.
///
/// \param cmdBuf NVNcommandBuffer object to update.
///
/// \param pool Memory pool used to store commands.
///
/// \param offset Offset (in bytes) relative to pool of the memory used to
/// store commands.
/// Must be a multiple of 4 (the value of
/// #NVN_DEVICE_INFO_COMMAND_BUFFER_COMMAND_ALIGNMENT).
///
/// \param size Size (in bytes) of memory used to store commands.
/// Must be a multiple of 4 (the value of
/// #NVN_DEVICE_INFO_COMMAND_BUFFER_COMMAND_ALIGNMENT).
NVNAPI void NVNAPIENTRY
    nvnCommandBufferAddCommandMemory(NVNcommandBuffer *cmdBuf,
                                     const NVNmemoryPool *pool,
                                     ptrdiff_t offset,
                                     size_t size);

/// \brief Add control memory for a NVNcommandBuffer object. This can be
/// called at any time, even outside an out-of-memory callback or while
/// recording or not recording. Any leftover space in the previous control
/// memory will remain unused. Only the last provided block is consumed.
///
/// \param cmdBuf NVNcommandBuffer object to update.
///
/// \param[in,out] memory Storage used for the command buffer control
/// information.
/// Must be a multiple of 8 (the value of
/// #NVN_DEVICE_INFO_COMMAND_BUFFER_CONTROL_ALIGNMENT).
///
/// \param size Size (in bytes) of memory used to store control
/// information.
/// Must be a multiple of 8 (the value of
/// #NVN_DEVICE_INFO_COMMAND_BUFFER_CONTROL_ALIGNMENT).
NVNAPI void NVNAPIENTRY
    nvnCommandBufferAddControlMemory(NVNcommandBuffer *cmdBuf,
                                     void *memory,
                                     size_t size);

/// \brief Get the total size in bytes of the command memory block that was
/// lastly added to the NVNcommandBuffer. Please see the
/// \ref nvn_guide_command_buffer_memory_management
/// "Command NVNbuffer Memory Management"
/// section of the NVN Programming Guide for further detail.
///
/// \param cmdBuf NVNcommandBuffer object to query.
NVNAPI size_t NVNAPIENTRY
    nvnCommandBufferGetCommandMemorySize(const NVNcommandBuffer *cmdBuf);

/// \brief Get the used size in bytes of the command memory block that was
/// lastly added to the NVNcommandBuffer. Please see the
/// \ref nvn_guide_command_buffer_memory_management
/// "Command NVNbuffer Memory Management"
/// section of the NVN Programming Guide for further detail.
///
/// \param cmdBuf NVNcommandBuffer object to query.
NVNAPI size_t NVNAPIENTRY
    nvnCommandBufferGetCommandMemoryUsed(const NVNcommandBuffer *cmdBuf);

/// \brief Get the unused size in bytes of the command memory block that
/// was lastly added to the NVNcommandBuffer. Please see the
/// \ref nvn_guide_command_buffer_memory_management
/// "Command NVNbuffer Memory Management"
/// section of the NVN Programming Guide for further detail.
///
/// \param cmdBuf NVNcommandBuffer object to query.
NVNAPI size_t NVNAPIENTRY
    nvnCommandBufferGetCommandMemoryFree(const NVNcommandBuffer *cmdBuf);

/// \brief Get the total size in bytes of the control memory block that was
/// lastly added to the NVNcommandBuffer. Please see the
/// \ref nvn_guide_command_buffer_memory_management
/// "Command NVNbuffer Memory Management"
/// section of the NVN Programming Guide for further detail.
///
/// \param cmdBuf NVNcommandBuffer object to query.
NVNAPI size_t NVNAPIENTRY
    nvnCommandBufferGetControlMemorySize(const NVNcommandBuffer *cmdBuf);

/// \brief Get the used size in bytes of the control memory block that was
/// lastly added to the NVNcommandBuffer. Please see the
/// \ref nvn_guide_command_buffer_memory_management
/// "Command NVNbuffer Memory Management"
/// section of the NVN Programming Guide for further detail.
///
/// \param cmdBuf NVNcommandBuffer object to query.
NVNAPI size_t NVNAPIENTRY
    nvnCommandBufferGetControlMemoryUsed(const NVNcommandBuffer *cmdBuf);

/// \brief Get the unused size in bytes of the control memory block that
/// was lastly added to the NVNcommandBuffer. Please see the
/// \ref nvn_guide_command_buffer_memory_management
/// "Command NVNbuffer Memory Management"
/// section of the NVN Programming Guide for further detail.
///
/// \param cmdBuf NVNcommandBuffer object to query.
NVNAPI size_t NVNAPIENTRY
    nvnCommandBufferGetControlMemoryFree(const NVNcommandBuffer *cmdBuf);

/// \brief Begin recording a new set of commands in this NVNcommandBuffer
/// object.
///
/// \param cmdBuf NVNcommandBuffer object used for command buffer
/// construction.
NVNAPI void NVNAPIENTRY
    nvnCommandBufferBeginRecording(NVNcommandBuffer *cmdBuf);

/// \brief Complete recording a new set of commands in this
/// NVNcommandBuffer object.
///
/// \param cmdBuf NVNcommandBuffer object used for command buffer
/// construction.
NVNAPI NVNcommandHandle NVNAPIENTRY
    nvnCommandBufferEndRecording(NVNcommandBuffer *cmdBuf);

/// \brief Insert calls to previously recorded command sets into the
/// command set under construction.
///
/// CallCommands will copy control data from the previously recorded
/// command sets into the new command set.  Command data from these command
/// sets are called directly.  The command buffer under construction must
/// hold sufficient memory to hold the control data.  The command data from
/// called command sets must not be freed until all command sets
/// referencing the data are no longer in use.  The command data in the
/// called command sets should be recorded in a memory pool using
/// #NVN_MEMORY_POOL_FLAGS_CPU_UNCACHED_BIT.
///
/// \param cmdBuf Command buffer recording the command set that should call
/// the previously recorded command sets.
///
/// \param numCommands Number of command sets to call.
///
/// \param handles Array of numCommands command set handles to call.
NVNAPI void NVNAPIENTRY
    nvnCommandBufferCallCommands(NVNcommandBuffer *cmdBuf,
                                 int numCommands,
                                 const NVNcommandHandle *handles);

/// \brief Insert copies of previously recorded command sets into the
/// command set under construction.
///
/// CopyCommands will copy command and control data from the previously
/// recorded command sets into the new command set.  The command buffer
/// must hold sufficient memory to hold the copied data.  The command data
/// in the called command sets should be recorded in a memory pool using
/// #NVN_MEMORY_POOL_FLAGS_CPU_CACHED_BIT.
///
/// \param cmdBuf Command buffer recording the command set that should copy
/// the previously recorded command sets.
///
/// \param numCommands Number of command sets to copy.
///
/// \param handles Array of numCommands command set handles to copy.
NVNAPI void NVNAPIENTRY
    nvnCommandBufferCopyCommands(NVNcommandBuffer *cmdBuf,
                                 int numCommands,
                                 const NVNcommandHandle *handles);

/// \brief Specify a new NVNblendState object to bind for subsequent
/// commands.
///
/// \param cmdBuf NVNcommandBuffer used to perform the bind operation.
///
/// \param blend NVNblendState object to be bound.
NVNAPI void NVNAPIENTRY
    nvnCommandBufferBindBlendState(NVNcommandBuffer *cmdBuf,
                                   const NVNblendState *blend);

/// \brief Specify a new NVNchannelMaskState object to bind for subsequent
/// commands.
///
/// \param cmdBuf NVNcommandBuffer used to perform the bind operation.
///
/// \param channelMask NVNchannelMaskState object to be bound.
NVNAPI void NVNAPIENTRY
    nvnCommandBufferBindChannelMaskState(NVNcommandBuffer *cmdBuf,
                                         const NVNchannelMaskState *channelMask);

/// \brief Specify a new NVNcolorState object to bind for subsequent
/// commands.
///
/// \param cmdBuf Command buffer used to perform the bind operation.
///
/// \param color NVNcolorState object to be bound.
NVNAPI void NVNAPIENTRY
    nvnCommandBufferBindColorState(NVNcommandBuffer *cmdBuf,
                                   const NVNcolorState *color);

/// \brief Specify a new NVNmultisampleState object to bind for subsequent
/// commands.
///
/// \param cmdBuf Command buffer used to perform the bind operation.
///
/// \param multisample NVNmultisampleState object to be bound.
NVNAPI void NVNAPIENTRY
    nvnCommandBufferBindMultisampleState(NVNcommandBuffer *cmdBuf,
                                         const NVNmultisampleState *multisample);

/// \brief Specify a new NVNpolygonState object to bind for subsequent
/// commands.
///
/// \param cmdBuf Command buffer used to perform the bind operation.
///
/// \param polygon NVNpolygonState object to be bound.
NVNAPI void NVNAPIENTRY
    nvnCommandBufferBindPolygonState(NVNcommandBuffer *cmdBuf,
                                     const NVNpolygonState *polygon);

/// \brief Specify a new NVNdepthStencilState object to bind for subsequent
/// commands.
///
/// \param cmdBuf Command buffer used to perform the bind operation.
///
/// \param depthStencil NVNdepthStencilState object to be bound.
NVNAPI void NVNAPIENTRY
    nvnCommandBufferBindDepthStencilState(NVNcommandBuffer *cmdBuf,
                                          const NVNdepthStencilState *depthStencil);

/// \brief Specify a vertex attribute layout for subsequent commands.
///
/// All attributes beyond _numAttribs_ are disabled, and fetching from them
/// in the shader will return zero.
///
/// \param cmdBuf Command buffer used to perform the bind operation.
///
/// \param numAttribs Number of vertex attributes to bind.
/// Must be less than or equal to 16 (the value of
/// #NVN_DEVICE_INFO_VERTEX_ATTRIBUTES).
///
/// \param attribs NVNvertexAttribState objects to be bound.
NVNAPI void NVNAPIENTRY
    nvnCommandBufferBindVertexAttribState(NVNcommandBuffer *cmdBuf,
                                          int numAttribs,
                                          const NVNvertexAttribState *attribs);

/// \brief Specify a vertex stream layout for subsequent commands.
///
/// \param cmdBuf Command buffer used to perform the bind operation.
///
/// \param numStreams Number of vertex streams to bind.
/// Must be less than or equal to 16 (the value of
/// #NVN_DEVICE_INFO_VERTEX_BUFFER_BINDINGS).
///
/// \param streams NVNvertexStreamState objects to be bound.
NVNAPI void NVNAPIENTRY
    nvnCommandBufferBindVertexStreamState(NVNcommandBuffer *cmdBuf,
                                          int numStreams,
                                          const NVNvertexStreamState *streams);

/// \brief Bind or unbind shaders in a NVNprogram object for subsequent
/// rendering commands.
///
/// This command binds all shaders present in _program_, and optionally
/// disables additional shader stages based on the bitfield _stages_.  If
/// _program_ is NULL, all shader stages specified in the bitfield _stages_
/// are disabled.  If _program_ is non-NULL, all shaders present in
/// _program_ will be bound and any stages in _stages_ that don't have
/// shaders in _program_ are disabled.  For example, using
/// #NVN_SHADER_STAGE_ALL_GRAPHICS_BITS with a program with only a vertex
/// and fragment shader will bind the vertex and fragment shaders in the
/// program and disable the optional tessellation control, tessellation
/// evaluation, and geometry shader stages.
///
/// \warning When _program_ is non-NULL, all stages present in _program_
/// must be enabled in the bitfield _stages_.
///
/// \warning In the C interface, it is possible to pass enums of the type
/// NVNshaderStage (NVN_VERTEX_SHADER) in _stages_.  This is not legal;
/// _stages_ requires a bitfield built from values of type
/// NVNshaderStageBits (NVN_VERTEX_SHADER_BIT).
///
/// \param cmdBuf Command buffer used to perform the bind operation.
///
/// \param program NVNprogram object to be bound.
///
/// \param stages Bitfield identifying the set of shader stages to be
/// updated. See #NVNshaderStageBits.
NVNAPI void NVNAPIENTRY
    nvnCommandBufferBindProgram(NVNcommandBuffer *cmdBuf,
                                const NVNprogram *program,
                                int stages);

/// \brief Bind a range of a NVNbuffer object to use for fetching vertex
/// attributes.
///
/// \param cmdBuf Command buffer used to perform the bind operation.
///
/// \param index Vertex buffer binding point number to update.
/// Must be less than 16 (the value of
/// #NVN_DEVICE_INFO_VERTEX_BUFFER_BINDINGS).
///
/// \param buffer GPU address of vertex buffer memory to bind.
///
/// \param size Size (in bytes) of the range of the NVNbuffer object to
/// bind.
NVNAPI void NVNAPIENTRY
    nvnCommandBufferBindVertexBuffer(NVNcommandBuffer *cmdBuf,
                                     int index,
                                     NVNbufferAddress buffer,
                                     size_t size);

/// \brief Bind one or more NVNbuffer object ranges to use for fetching
/// vertex attributes.
///
/// \param cmdBuf Command buffer used to perform the bind operation.
///
/// \param first First vertex buffer binding point number to update.
/// Must be less than 16 (the value of
/// #NVN_DEVICE_INFO_VERTEX_BUFFER_BINDINGS).
///
/// \param count Number of contiguous vertex buffer binding points to
/// update.
/// Must be greater than or equal to zero.
/// Must be less than or equal to 16 (the value of
/// #NVN_DEVICE_INFO_VERTEX_BUFFER_BINDINGS).
///
/// \param buffers Array of vertex buffer ranges to bind.
NVNAPI void NVNAPIENTRY
    nvnCommandBufferBindVertexBuffers(NVNcommandBuffer *cmdBuf,
                                      int first,
                                      int count,
                                      const NVNbufferRange *buffers);

/// \brief Bind a range of a NVNbuffer object to use for fetching shader
/// uniform values.
///
/// \param cmdBuf Command buffer used to perform the bind operation.
///
/// \param stage Shader stage owning the binding point to update.
///
/// \param index Uniform buffer binding point number to update.
/// Must be greater than or equal to zero.
/// Must be less than 14 (the value of
/// #NVN_DEVICE_INFO_UNIFORM_BUFFER_BINDINGS_PER_STAGE).
///
/// \param buffer GPU address of buffer object memory to bind.
/// Must be a multiple of 256 (the value of
/// #NVN_DEVICE_INFO_UNIFORM_BUFFER_ALIGNMENT).
///
/// \param size Size (in bytes) of the range of the NVNbuffer object to
/// bind.
/// Must be less than or equal to 65536 (the value of
/// #NVN_DEVICE_INFO_MAX_UNIFORM_BUFFER_SIZE).
NVNAPI void NVNAPIENTRY
    nvnCommandBufferBindUniformBuffer(NVNcommandBuffer *cmdBuf,
                                      NVNshaderStage stage,
                                      int index,
                                      NVNbufferAddress buffer,
                                      size_t size);

/// \brief Bind one or more NVNbuffer object ranges to use for fetching
/// shader uniform values.
///
/// \param cmdBuf Command buffer used to perform the bind operation.
///
/// \param stage Shader stage owning the binding point to update.
///
/// \param first First uniform buffer binding point number to update.
/// Must be less than 14 (the value of
/// #NVN_DEVICE_INFO_UNIFORM_BUFFER_BINDINGS_PER_STAGE).
///
/// \param count Number of contiguous uniform buffer binding points to
/// update.
/// Must be greater than or equal to zero.
/// Must be less than or equal to 14 (the value of
/// #NVN_DEVICE_INFO_UNIFORM_BUFFER_BINDINGS_PER_STAGE).
///
/// \param buffers Array of buffer object ranges to bind.
NVNAPI void NVNAPIENTRY
    nvnCommandBufferBindUniformBuffers(NVNcommandBuffer *cmdBuf,
                                       NVNshaderStage stage,
                                       int first,
                                       int count,
                                       const NVNbufferRange *buffers);

/// \brief Bind a range of a NVNbuffer object to use for capturing
/// transformed vertex data.
///
/// \param cmdBuf Command buffer used to perform the bind operation.
///
/// \param index Transform feedback buffer binding point number to update.
/// Must be less than 4 (the value of
/// #NVN_DEVICE_INFO_TRANSFORM_FEEDBACK_BUFFER_BINDINGS).
///
/// \param buffer GPU address of transform feedback buffer memory to bind.
/// If specified as zero, no data should be captured for the updated
/// binding point.
/// Must be a multiple of 4 (the value of
/// #NVN_DEVICE_INFO_TRANSFORM_FEEDBACK_BUFFER_ALIGNMENT).
///
/// \param size Size (in bytes) of the range of the NVNbuffer object to
/// bind.
/// Must be a multiple of 4 (the value of
/// #NVN_DEVICE_INFO_TRANSFORM_FEEDBACK_BUFFER_ALIGNMENT).
NVNAPI void NVNAPIENTRY
    nvnCommandBufferBindTransformFeedbackBuffer(NVNcommandBuffer *cmdBuf,
                                                int index,
                                                NVNbufferAddress buffer,
                                                size_t size);

/// \brief Bind one or more NVNbuffer object ranges to use for capturing
/// transformed vertex data.
///
/// \param cmdBuf Command buffer used to perform the bind operation.
///
/// \param first First transform feedback buffer binding point number to
/// update.
/// Must be less than 4 (the value of
/// #NVN_DEVICE_INFO_TRANSFORM_FEEDBACK_BUFFER_BINDINGS).
///
/// \param count Number of contiguous transform feedback buffer binding
/// points to update.
/// Must be greater than or equal to zero.
/// Must be less than or equal to 4 (the value of
/// #NVN_DEVICE_INFO_TRANSFORM_FEEDBACK_BUFFER_BINDINGS).
///
/// \param buffers Array of buffer object ranges to bind.
/// If specified as zero, no data should be captured for the updated
/// binding point.
NVNAPI void NVNAPIENTRY
    nvnCommandBufferBindTransformFeedbackBuffers(NVNcommandBuffer *cmdBuf,
                                                 int first,
                                                 int count,
                                                 const NVNbufferRange *buffers);

/// \brief Bind a range of a NVNbuffer object to use for random-access
/// reads and writes in shaders.
///
/// \param cmdBuf Command buffer used to perform the bind operation.
///
/// \param stage Shader stage owning the binding point to update.
///
/// \param index Shader storage buffer binding point number to update.
/// Must be less than 16 (the value of
/// #NVN_DEVICE_INFO_SHADER_STORAGE_BUFFER_BINDINGS_PER_STAGE).
///
/// \param buffer GPU address of buffer object memory to bind.
///
/// \param size Size (in bytes) of the range of the NVNbuffer object to
/// bind.
NVNAPI void NVNAPIENTRY
    nvnCommandBufferBindStorageBuffer(NVNcommandBuffer *cmdBuf,
                                      NVNshaderStage stage,
                                      int index,
                                      NVNbufferAddress buffer,
                                      size_t size);

/// \brief Bind one or more NVNbuffer object ranges to use for
/// random-access reads and writes in shaders.
///
/// \param cmdBuf Command buffer used to perform the bind operation.
///
/// \param stage Shader stage owning the binding point to update.
///
/// \param first First shader storage buffer binding point number to
/// update.
/// Must be less than 16 (the value of
/// #NVN_DEVICE_INFO_SHADER_STORAGE_BUFFER_BINDINGS_PER_STAGE).
///
/// \param count Number of contiguous shader storage buffer binding points
/// to update.
/// Must be greater than or equal to zero.
/// Must be less than or equal to 16 (the value of
/// #NVN_DEVICE_INFO_SHADER_STORAGE_BUFFER_BINDINGS_PER_STAGE).
///
/// \param buffers Array of buffer object ranges to bind.
NVNAPI void NVNAPIENTRY
    nvnCommandBufferBindStorageBuffers(NVNcommandBuffer *cmdBuf,
                                       NVNshaderStage stage,
                                       int first,
                                       int count,
                                       const NVNbufferRange *buffers);

/// \brief Bind a NVNtexture/NVNsampler pair to use for texture mapping in
/// shaders.
///
/// \param cmdBuf Command buffer used to perform the bind operation.
///
/// \param stage Shader stage owning the binding point to update.
///
/// \param index NVNtexture/sampler binding point number to update.
/// Must be less than 32 (the value of
/// #NVN_DEVICE_INFO_TEXTURE_BINDINGS_PER_STAGE).
///
/// \param texture Handle for the pair of NVNtexture and NVNsampler objects
/// to bind.
NVNAPI void NVNAPIENTRY
    nvnCommandBufferBindTexture(NVNcommandBuffer *cmdBuf,
                                NVNshaderStage stage,
                                int index,
                                NVNtextureHandle texture);

/// \brief Bind one or more NVNtexture/NVNsampler pairs to use for texture
/// mapping in shaders.
///
/// \param cmdBuf Command buffer used to perform the bind operation.
///
/// \param stage Shader stage owning the binding points to update.
///
/// \param first First texture/sampler binding point number to update.
/// Must be less than 32 (the value of
/// #NVN_DEVICE_INFO_TEXTURE_BINDINGS_PER_STAGE).
///
/// \param count Number of contiguous texture/sampler binding points to
/// update.
/// Must be less than or equal to 32 (the value of
/// #NVN_DEVICE_INFO_TEXTURE_BINDINGS_PER_STAGE).
///
/// \param textures Array of handles for pairs of NVNtexture and NVNsampler
/// objects to bind.
NVNAPI void NVNAPIENTRY
    nvnCommandBufferBindTextures(NVNcommandBuffer *cmdBuf,
                                 NVNshaderStage stage,
                                 int first,
                                 int count,
                                 const NVNtextureHandle *textures);

/// \brief Bind a NVNtexture to use for image loads and stores in shaders.
///
/// \param cmdBuf Command buffer used to perform the bind operation.
///
/// \param stage Shader stage owning the binding point to update.
///
/// \param index Image binding point number to update.
/// Must be less than 8 (the value of
/// #NVN_DEVICE_INFO_IMAGE_BINDINGS_PER_STAGE).
///
/// \param image Image handle for the NVNtexture object to bind.
NVNAPI void NVNAPIENTRY
    nvnCommandBufferBindImage(NVNcommandBuffer *cmdBuf,
                              NVNshaderStage stage,
                              int index,
                              NVNimageHandle image);

/// \brief Bind one or more Textures to use for image loads and stores in
/// shaders.
///
/// \param cmdBuf Command buffer used to perform the bind operation.
///
/// \param stage Shader stage owning the binding points to update.
///
/// \param first First image binding point number to update.
/// Must be less than 8 (the value of
/// #NVN_DEVICE_INFO_IMAGE_BINDINGS_PER_STAGE).
///
/// \param count Number of contiguous image binding points to update.
/// Must be less than or equal to 8 (the value of
/// #NVN_DEVICE_INFO_IMAGE_BINDINGS_PER_STAGE).
///
/// \param images Array of handles for NVNtexture objects to bind.
NVNAPI void NVNAPIENTRY
    nvnCommandBufferBindImages(NVNcommandBuffer *cmdBuf,
                               NVNshaderStage stage,
                               int first,
                               int count,
                               const NVNimageHandle *images);

/// \brief Specify the number of vertices in each patch when rendering
/// patch primitives.
///
/// \param cmdBuf Command buffer used to perform the update operation.
///
/// \param size Number of vertices present in each patch.
/// Must be greater than zero.
/// Must be less than or equal to 32 (the value of
/// #NVN_DEVICE_INFO_MAX_PATCH_SIZE).
NVNAPI void NVNAPIENTRY
    nvnCommandBufferSetPatchSize(NVNcommandBuffer *cmdBuf,
                                 int size);

/// \brief Specify default inner tessellation levels when rendering patch
/// primitives.
///
/// The default inner and outer tessellation levels are used when a
/// tessellation evaluation shader is bound without a tessellation control
/// shader.  When a tessellation control shader is bound, default levels
/// are ignored.
///
/// \note Tessellation levels are clamped to the range [1.0, 64.0], and may
/// be further clamped according to the tessellation spacing.
///
/// \param cmdBuf Command buffer used to perform the update operation.
///
/// \param levels Two floating-point inner tessellation levels.
NVNAPI void NVNAPIENTRY
    nvnCommandBufferSetInnerTessellationLevels(NVNcommandBuffer *cmdBuf,
                                               const float *levels);

/// \brief Specify default outer tessellation levels when rendering patch
/// primitives.
///
/// The default inner and outer tessellation levels are used when a
/// tessellation evaluation shader is bound without a tessellation control
/// shader.  When a tessellation control shader is bound, default levels
/// are ignored.
///
/// \note If any outer tessellation level is less than or equal to zero, a
/// patch will be discarded, except for the fourth level when processing
/// triangular patches.
///
/// \note Tessellation levels greater than zero are clamped to the range
/// [1.0, 64.0] and may be further clamped according to the tessellation
/// spacing.
///
/// \param cmdBuf Command buffer used to perform the update operation.
///
/// \param levels Four floating-point outer tessellation levels.
NVNAPI void NVNAPIENTRY
    nvnCommandBufferSetOuterTessellationLevels(NVNcommandBuffer *cmdBuf,
                                               const float *levels);

/// \brief Enable and disable primitive restart and specify a primitive
/// restart index.
///
/// When enabled, a primitive is restarted (with the same primitive type)
/// whenever an index value matching the restart index value is found.
///
/// \note The vertex index value must match the restart index exactly.  For
/// example, primitives rendered using the index type
/// #NVN_INDEX_TYPE_UNSIGNED_BYTE wil not be restarted if the restart index
/// is outside the range [0,255].
///
/// \param cmdBuf Command buffer used to perform the update operation.
///
/// \param enable Enable (TRUE) or disable (FALSE) primitive restart
/// operations.
///
/// \param index Index value used to indicate that a primitive should be
/// restarted.
NVNAPI void NVNAPIENTRY
    nvnCommandBufferSetPrimitiveRestart(NVNcommandBuffer *cmdBuf,
                                        NVNboolean enable,
                                        int index);

/// \brief Start a transform feedback (stream output) operation.
///
/// \param cmdBuf Command buffer used for the transform feedback commands.
///
/// \param buffer GPU address of buffer memory holding 32 bytes of control
/// information used by the transform feedback operation.
/// \warning Must point to valid buffer storage.
/// Must be a multiple of 4 (the value of
/// #NVN_DEVICE_INFO_TRANSFORM_FEEDBACK_CONTROL_ALIGNMENT).
NVNAPI void NVNAPIENTRY
    nvnCommandBufferBeginTransformFeedback(NVNcommandBuffer *cmdBuf,
                                           NVNbufferAddress buffer);

/// \brief Complete a transform feedback (stream output) operation.
///
/// \param cmdBuf Command buffer used for the transform feedback commands.
///
/// \param buffer GPU address of buffer memory holding 32 bytes of control
/// information used by the transform feedback operation.
/// \warning Must point to valid buffer storage.
/// Must be a multiple of 4 (the value of
/// #NVN_DEVICE_INFO_TRANSFORM_FEEDBACK_CONTROL_ALIGNMENT).
NVNAPI void NVNAPIENTRY
    nvnCommandBufferEndTransformFeedback(NVNcommandBuffer *cmdBuf,
                                         NVNbufferAddress buffer);

/// \brief Pause a transform feedback (stream output) operation.
///
/// \param cmdBuf Command buffer used for the transform feedback commands.
///
/// \param buffer GPU address of buffer memory holding 32 bytes of control
/// information used by the transform feedback operation.
/// The provided address may be zero, in which case, no control information
/// will be written to memory.
///
/// \note If an address of zero is used, the current state of the transform
/// feedback operation is not saved to memory.  If any other transform
/// feedback operation is performed before resuming, or if transform
/// feedback is resumed using a non-zero address, this state is lost.
/// Must be a multiple of 4 (the value of
/// #NVN_DEVICE_INFO_TRANSFORM_FEEDBACK_CONTROL_ALIGNMENT).
NVNAPI void NVNAPIENTRY
    nvnCommandBufferPauseTransformFeedback(NVNcommandBuffer *cmdBuf,
                                           NVNbufferAddress buffer);

/// \brief Resume a transform feedback (stream output) operation.
///
/// \param cmdBuf Command buffer used for the transform feedback commands.
///
/// \param buffer GPU address of buffer memory holding 32 bytes of control
/// information used by the transform feedback operation.
/// The provided address may be zero, in which case, no control information
/// will be read from memory.
///
/// \note If an address of zero is used, the state of the previously paused
/// transform feedback operation is not restored from memory.  If any other
/// transform feedback operation is performed before resuming with a zero
/// address, this state is lost.
/// Must be a multiple of 4 (the value of
/// #NVN_DEVICE_INFO_TRANSFORM_FEEDBACK_CONTROL_ALIGNMENT).
NVNAPI void NVNAPIENTRY
    nvnCommandBufferResumeTransformFeedback(NVNcommandBuffer *cmdBuf,
                                            NVNbufferAddress buffer);

/// \brief Draw primitives captured in a previous transform feedback
/// (stream output) operation.
///
/// \warning Applications must call nvnCommandBufferBarrier with the
/// #NVN_BARRIER_ORDER_INDIRECT_DATA_BIT bit set to ensure that the GPU
/// doesn't fetch the indirect data before previous commands are finished
/// writing the data.  Waiting via nvnQueueFinish, nvnQueueWaitSync, or
/// nvnCommandBufferWaitSync is not sufficient to ensure that GPU-produced
/// indirect data are not fetched prematurely.
///
/// \param cmdBuf Command buffer used for the transform feedback commands.
///
/// \param mode Type of primitive to render.
/// When rendering primitives captured by transform feedback, this
/// parameter should typically be POINTS, LINES, or TRIANGLES, since
/// transform feedback captures only independent primitives (even if the
/// original geometry were provided as strips).
///
/// \param buffer GPU address of buffer memory holding 32 bytes of control
/// information used by the transform feedback operation.
/// \warning Must point to valid buffer storage.
/// Must be a multiple of 4 (the value of
/// #NVN_DEVICE_INFO_TRANSFORM_FEEDBACK_CONTROL_ALIGNMENT).
NVNAPI void NVNAPIENTRY
    nvnCommandBufferDrawTransformFeedback(NVNcommandBuffer *cmdBuf,
                                          NVNdrawPrimitive mode,
                                          NVNbufferAddress buffer);

/// \brief Render a primitive with consecutive vertex indices.
///
/// \param cmdBuf NVNcommandBuffer used to perform the draw operation.
///
/// \param mode Type of the primitive to render.
///
/// \param first Index of the first vertex in the primitive.
///
/// \param count Number of vertices in the primitive.
NVNAPI void NVNAPIENTRY
    nvnCommandBufferDrawArrays(NVNcommandBuffer *cmdBuf,
                               NVNdrawPrimitive mode,
                               int first,
                               int count);

/// \brief Render a primitive with vertex indices fetched from an index
/// buffer.
///
/// \param cmdBuf NVNcommandBuffer used to perform the draw operation.
///
/// \param mode Type of the primitive to render.
///
/// \param type Type of index values stored in the index buffer.
///
/// \param count Number of vertices in the primitive.
///
/// \param indexBuffer GPU address of buffer object memory used as the
/// index buffer.
NVNAPI void NVNAPIENTRY
    nvnCommandBufferDrawElements(NVNcommandBuffer *cmdBuf,
                                 NVNdrawPrimitive mode,
                                 NVNindexType type,
                                 int count,
                                 NVNbufferAddress indexBuffer);

/// \brief Render a primitive with vertex indices fetched from an index
/// buffer, with a base vertex value added to each fetched index.
///
/// \param cmdBuf NVNcommandBuffer used to perform the draw operation.
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
/// \param baseVertex Base vertex value, added to each index fetched from
/// the index buffer.
NVNAPI void NVNAPIENTRY
    nvnCommandBufferDrawElementsBaseVertex(NVNcommandBuffer *cmdBuf,
                                           NVNdrawPrimitive mode,
                                           NVNindexType type,
                                           int count,
                                           NVNbufferAddress indexBuffer,
                                           int baseVertex);

/// \brief Render multiple copies (instances) of a primitive with
/// consecutive vertex indices.
///
/// \param cmdBuf NVNcommandBuffer used to perform the draw operation.
///
/// \param mode Type of the primitive to render.
///
/// \param first Index of the first vertex in the primitive.
///
/// \param count Number of vertices in the primitive.
///
/// \param baseInstance Instance number of the first primitive instance.
///
/// \param instanceCount Number of primitive instances to render.
NVNAPI void NVNAPIENTRY
    nvnCommandBufferDrawArraysInstanced(NVNcommandBuffer *cmdBuf,
                                        NVNdrawPrimitive mode,
                                        int first,
                                        int count,
                                        int baseInstance,
                                        int instanceCount);

/// \brief Render multiple copies (instances) of a primitive with vertex
/// indices fetched from an index buffer.
///
/// \param cmdBuf NVNcommandBuffer used to perform the draw operation.
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
/// \param baseVertex Base vertex value, added to each index fetched from
/// the index buffer.
///
/// \param baseInstance Instance number of the first primitive instance.
///
/// \param instanceCount Number of primitive instances to render.
NVNAPI void NVNAPIENTRY
    nvnCommandBufferDrawElementsInstanced(NVNcommandBuffer *cmdBuf,
                                          NVNdrawPrimitive mode,
                                          NVNindexType type,
                                          int count,
                                          NVNbufferAddress indexBuffer,
                                          int baseVertex,
                                          int baseInstance,
                                          int instanceCount);

/// \brief Render primitives with consecutive vertex indices, using draw
/// parameters fetched from an indirect data buffer.
///
/// The parameters of the draw are stored in NVNbuffer object memory using
/// the NVNdrawArraysIndirectData structure.
///
/// \warning If the indirect data are produced by previous commands
/// executed on the GPU, applications must call nvnCommandBufferBarrier
/// with the #NVN_BARRIER_ORDER_INDIRECT_DATA_BIT bit set to ensure that
/// the GPU doesn't fetch the indirect data before previous commands are
/// finished writing the data.  Waiting via nvnQueueFinish,
/// nvnQueueWaitSync, or nvnCommandBufferWaitSync is not sufficient to
/// ensure that GPU-produced indirect data are not fetched prematurely.
///
/// \param cmdBuf NVNcommandBuffer used to perform the draw operation.
///
/// \param mode Type of the primitive to render.
///
/// \param indirectBuffer GPU address of buffer object memory holding the
/// indirect draw data.
/// Must be a multiple of 4 (the value of
/// #NVN_DEVICE_INFO_INDIRECT_DRAW_ALIGNMENT).
NVNAPI void NVNAPIENTRY
    nvnCommandBufferDrawArraysIndirect(NVNcommandBuffer *cmdBuf,
                                       NVNdrawPrimitive mode,
                                       NVNbufferAddress indirectBuffer);

/// \brief Render primitives with vertex indices fetched from an index
/// buffer, using draw parameters fetched from an indirect data buffer.
///
/// The parameters of the draw are stored in NVNbuffer object memory using
/// the NVNdrawElementsIndirectData structure.
///
/// \warning If the indirect data are produced by previous commands
/// executed on the GPU, applications must call nvnCommandBufferBarrier
/// with the #NVN_BARRIER_ORDER_INDIRECT_DATA_BIT bit set to ensure that
/// the GPU doesn't fetch the indirect data before previous commands are
/// finished writing the data.  Waiting via nvnQueueFinish,
/// nvnQueueWaitSync, or nvnCommandBufferWaitSync is not sufficient to
/// ensure that GPU-produced indirect data are not fetched prematurely.
///
/// \param cmdBuf NVNcommandBuffer used to perform the draw operation.
///
/// \param mode Type of the primitive to render.
///
/// \param type Type of index values stored in the index buffer.
///
/// \param indexBuffer GPU address of buffer object memory used as the
/// index buffer.
///
/// \param indirectBuffer GPU address of buffer object memory holding the
/// indirect draw data.
/// Must be a multiple of 4 (the value of
/// #NVN_DEVICE_INFO_INDIRECT_DRAW_ALIGNMENT).
NVNAPI void NVNAPIENTRY
    nvnCommandBufferDrawElementsIndirect(NVNcommandBuffer *cmdBuf,
                                         NVNdrawPrimitive mode,
                                         NVNindexType type,
                                         NVNbufferAddress indexBuffer,
                                         NVNbufferAddress indirectBuffer);

/// \brief Render multiple primitives with vertices fetched from a vertex
/// buffer, using draw parameters fetched from an indirect data buffer and
/// the number of primitives fetched from a parameter buffer.
///
/// The parameters of the draw are stored in NVNbuffer object memory using
/// the NVNdrawArraysIndirectData structure.
///
/// \warning If the indirect data or the draw count data are produced by
/// previous commands executed on the GPU, applications must call
/// nvnCommandBufferBarrier with the #NVN_BARRIER_ORDER_INDIRECT_DATA_BIT
/// bit set to ensure that the GPU doesn't fetch the indirect data before
/// previous commands are finished writing the data.  Waiting via
/// nvnQueueFinish, nvnQueueWaitSync, or nvnCommandBufferWaitSync is not
/// sufficient to ensure that GPU-produced indirect data are not fetched
/// prematurely.
///
/// \param cmdBuf NVNcommandBuffer used to perform the draw operation.
///
/// \param mode Type of the primitive to render.
///
/// \param indirectBuffer GPU address of buffer object memory holding the
/// indirect draw data.
/// Must be a multiple of 4 (the value of
/// #NVN_DEVICE_INFO_INDIRECT_DRAW_ALIGNMENT).
///
/// \param parameterBuffer GPU address of buffer object memory holding the
/// draw count.
/// Must be a multiple of 4 (the value of
/// #NVN_DEVICE_INFO_INDIRECT_DRAW_ALIGNMENT).
///
/// \param maxDrawCount Specifies the maximum number of draws that are
/// allowed.
/// If the draw count in GPU memory pointed to by _parameterBuffer_ is
/// greater than _maxDrawCount_, only the first _maxDrawCount_ primitives
/// will be drawn.
///
/// \param stride Stride (in bytes) between elements in _indirectBuffer_.
NVNAPI void NVNAPIENTRY
    nvnCommandBufferMultiDrawArraysIndirectCount(NVNcommandBuffer *cmdBuf,
                                                 NVNdrawPrimitive mode,
                                                 NVNbufferAddress indirectBuffer,
                                                 NVNbufferAddress parameterBuffer,
                                                 int maxDrawCount,
                                                 ptrdiff_t stride);

/// \brief Render multiple primitives with vertex indices fetched from an
/// index buffer, using draw parameters fetched from an indirect data
/// buffer and the number of primitives fetched from a parameter buffer.
///
/// The parameters of the draw are stored in buffer object memory using the
/// NVNdrawElementsIndirectData structure.
///
/// \warning If the indirect data or the draw count data are produced by
/// previous commands executed on the GPU, applications must call
/// nvnCommandBufferBarrier with the #NVN_BARRIER_ORDER_INDIRECT_DATA_BIT
/// bit set to ensure that the GPU doesn't fetch the indirect data before
/// previous commands are finished writing the data.  Waiting via
/// nvnQueueFinish, nvnQueueWaitSync, or nvnCommandBufferWaitSync is not
/// sufficient to ensure that GPU-produced indirect data are not fetched
/// prematurely.
///
/// \param cmdBuf NVNcommandBuffer used to perform the draw operation.
///
/// \param mode Type of the primitive to render.
///
/// \param type Type of index values stored in the index buffer.
///
/// \param indexBuffer GPU address of buffer object memory used as the
/// index buffer.
///
/// \param indirectBuffer GPU address of buffer object memory holding the
/// indirect draw data.
/// Must be a multiple of 4 (the value of
/// #NVN_DEVICE_INFO_INDIRECT_DRAW_ALIGNMENT).
///
/// \param parameterBuffer GPU address of buffer object memory holding the
/// draw count.
/// Must be a multiple of 4 (the value of
/// #NVN_DEVICE_INFO_INDIRECT_DRAW_ALIGNMENT).
///
/// \param maxDrawCount Specifies the maximum number of draws that are
/// allowed.
/// If the draw count in GPU memory pointed to by _parameterBuffer_ is
/// greater than _maxDrawCount_, only the first _maxDrawCount_ primitives
/// will be drawn.
///
/// \param stride Stride (in bytes) between elements in _indirectBuffer_.
NVNAPI void NVNAPIENTRY
    nvnCommandBufferMultiDrawElementsIndirectCount(NVNcommandBuffer *cmdBuf,
                                                   NVNdrawPrimitive mode,
                                                   NVNindexType type,
                                                   NVNbufferAddress indexBuffer,
                                                   NVNbufferAddress indirectBuffer,
                                                   NVNbufferAddress parameterBuffer,
                                                   int maxDrawCount,
                                                   ptrdiff_t stride);

/// \brief Clear a color buffer using floating-point clear color values.
///
/// This command should be used only to clear color buffers with
/// floating-point or _normalized_ signed or unsigned integer components.
/// When clearing color buffers with unnormalized integer components (with
/// formats ending in "I" or "UI"), the values in `color` will be
/// re-interpreted as integers.  For example, clearing an integer color
/// buffer to the value 1.0 will instead clear to 0x3F800000, the IEEE-754
/// encoding of 1.0.
///
/// \warning When using the Windows reference implementation on Pascal and
/// newer GPUs, calling nvnCommandBufferBarrier with the barrier
/// #NVN_BARRIER_ORDER_FRAGMENTS_BIT is not sufficient to ensure that
/// previous rendering using the color buffer as a texture completes before
/// the color buffer clear starts. In such cases, a heavier barrier like
/// #NVN_BARRIER_ORDER_PRIMITIVES_BIT is necessary to ensure that the clear
/// doesn't corrupt rendering commands submitted prior to the barrier.
///
/// \param cmdBuf NVNcommandBuffer used to perform the clear.
///
/// \param index Index of the color target to clear.
/// Must be less than 8 (the value of
/// #NVN_DEVICE_INFO_COLOR_BUFFER_BINDINGS).
///
/// \param color Array of four floating-point (RGBA) clear color values.
///
/// \param mask Bitfield indicating which color channels should be cleared.
/// See #NVNclearColorMask.
NVNAPI void NVNAPIENTRY
    nvnCommandBufferClearColor(NVNcommandBuffer *cmdBuf,
                               int index,
                               const float *color,
                               int mask);

/// \brief Clear a color buffer using signed integer clear color values.
///
/// This command should be used only to clear color buffers with
/// _unnormalized_ signed integer components (with formats ending in "I").
/// When clearing color buffers with floating-point or normalized integer
/// components, the values in `color` will be re-interpreted as
/// floating-point values.  For example, clearing a floating-point color
/// buffer to the value 0x3F800000 will instead clear to 1.0 (the IEEE-754
/// floating-point equivalent). When clearing color buffers with unsigned
/// integer components, the signed clear color will be re-interpreted as
/// unsigned.
///
/// \warning When using the Windows reference implementation on Pascal and
/// newer GPUs, calling nvnCommandBufferBarrier with the barrier
/// #NVN_BARRIER_ORDER_FRAGMENTS_BIT is not sufficient to ensure that
/// previous rendering using the color buffer as a texture completes before
/// the color buffer clear starts. In such cases, a heavier barrier like
/// #NVN_BARRIER_ORDER_PRIMITIVES_BIT is necessary to ensure that the clear
/// doesn't corrupt rendering commands submitted prior to the barrier.
///
/// \param cmdBuf NVNcommandBuffer used to perform the clear.
///
/// \param index Index of the color target to clear.
/// Must be less than 8 (the value of
/// #NVN_DEVICE_INFO_COLOR_BUFFER_BINDINGS).
///
/// \param color Array of four signed integer (RGBA) clear color values.
///
/// \param mask Bitfield indicating which color channels should be cleared.
/// See #NVNclearColorMask.
NVNAPI void NVNAPIENTRY
    nvnCommandBufferClearColori(NVNcommandBuffer *cmdBuf,
                                int index,
                                const int *color,
                                int mask);

/// \brief Clear a color buffer using unsigned integer clear color values.
///
/// This command should be used only to clear color buffers with
/// _unnormalized_ unsigned integer components (with formats ending in
/// "UI").  When clearing color buffers with floating-point or normalized
/// integer components, the values in `color` will be re-interpreted as
/// floating-point values.  For example, clearing a floating-point color
/// buffer to the value 0x3F800000 will instead clear to 1.0 (the IEEE-754
/// floating-point equivalent). When clearing color buffers with signed
/// integer components, the unsigned clear color will be re-interpreted as
/// signed.
///
/// \warning When using the Windows reference implementation on Pascal and
/// newer GPUs, calling nvnCommandBufferBarrier with the barrier
/// #NVN_BARRIER_ORDER_FRAGMENTS_BIT is not sufficient to ensure that
/// previous rendering using the color buffer as a texture completes before
/// the color buffer clear starts. In such cases, a heavier barrier like
/// #NVN_BARRIER_ORDER_PRIMITIVES_BIT is necessary to ensure that the clear
/// doesn't corrupt rendering commands submitted prior to the barrier.
///
/// \param cmdBuf NVNcommandBuffer used to perform the clear.
///
/// \param index Index of the color target to clear.
/// Must be less than 8 (the value of
/// #NVN_DEVICE_INFO_COLOR_BUFFER_BINDINGS).
///
/// \param color Array of four unsigned integer (RGBA) clear color values.
///
/// \param mask Bitfield indicating which color channels should be cleared.
/// See #NVNclearColorMask.
NVNAPI void NVNAPIENTRY
    nvnCommandBufferClearColorui(NVNcommandBuffer *cmdBuf,
                                 int index,
                                 const uint32_t *color,
                                 int mask);

/// \brief Clear a depth/stencil buffer.
///
/// This command clears depth and stencil buffers of all formats.
///
/// \warning On NX and the Windows reference implementation, calling
/// nvnCommandBufferBarrier with the barrier
/// #NVN_BARRIER_ORDER_FRAGMENTS_BIT is not sufficient to ensure that
/// previous rendering using the depth/stencil buffer as a texture
/// completes before the depth/stencil buffer clear starts. In such cases,
/// a heavier barrier like #NVN_BARRIER_ORDER_PRIMITIVES_BIT is necessary
/// to ensure that the clear does not corrupt rendering commands submitted
/// prior to the barrier.
///
/// \param cmdBuf NVNcommandBuffer used to perform the clear.
///
/// \param depthValue Floating-point depth clear value (in the range [0.0,
/// 1.0]).
///
/// \param depthMask Enable (TRUE) or disable (FALSE) clearing of the depth
/// channel.
///
/// \param stencilValue Stencil clear value.
///
/// \param stencilMask Mask indicating the bits of the stencil buffer to
/// clear.
NVNAPI void NVNAPIENTRY
    nvnCommandBufferClearDepthStencil(NVNcommandBuffer *cmdBuf,
                                      float depthValue,
                                      NVNboolean depthMask,
                                      int stencilValue,
                                      int stencilMask);

/// \brief Dispatch a new grid of compute shader work groups for
/// processing.
///
/// Each compute shader dispatch spawns a three-dimensional array of work
/// groups for processing.  Each work group is processed independently,
/// with a three-dimensional array of threads whose size is specified in
/// the compute shader.
///
/// \param cmdBuf NVNcommandBuffer used to dispatch the compute work.
///
/// \param groupsX The number of compute work groups in the X dimension.
/// Must be less than or equal to 65535 (the value of
/// #NVN_DEVICE_INFO_MAX_COMPUTE_DISPATCH_WORK_GROUPS_X).
///
/// \param groupsY The number of compute work groups in the Y dimension.
/// Must be less than or equal to 65535 (the value of
/// #NVN_DEVICE_INFO_MAX_COMPUTE_DISPATCH_WORK_GROUPS_Y).
///
/// \param groupsZ The number of compute work groups in the Z dimension.
/// Must be less than or equal to 65535 (the value of
/// #NVN_DEVICE_INFO_MAX_COMPUTE_DISPATCH_WORK_GROUPS_Z).
NVNAPI void NVNAPIENTRY
    nvnCommandBufferDispatchCompute(NVNcommandBuffer *cmdBuf,
                                    int groupsX,
                                    int groupsY,
                                    int groupsZ);

/// \brief Dispatch a new grid of compute shader work groups for
/// processing.
///
/// Each compute shader dispatch spawns a three-dimensional array of work
/// groups for processing.  Each work group is processed independently,
/// with a three-dimensional array of threads whose size is specified in
/// the compute shader.  The number of compute workgroups in each dimension
/// is read from buffer memory.
///
/// \warning If the indirect data are produced by previous commands
/// executed on the GPU, applications must call nvnCommandBufferBarrier
/// with the #NVN_BARRIER_ORDER_INDIRECT_DATA_BIT bit set to ensure that
/// the GPU doesn't fetch the indirect data before previous commands are
/// finished writing the data.  Waiting via nvnQueueFinish,
/// nvnQueueWaitSync, or nvnCommandBufferWaitSync is not sufficient to
/// ensure that GPU-produced indirect data are not fetched prematurely.
///
/// \param cmdBuf NVNcommandBuffer used to dispatch the compute work.
///
/// \param indirectBuffer GPU address of buffer object memory holding the
/// indirect dispatch data.
/// Must be a multiple of 4 (the value of
/// #NVN_DEVICE_INFO_INDIRECT_DISPATCH_ALIGNMENT).
NVNAPI void NVNAPIENTRY
    nvnCommandBufferDispatchComputeIndirect(NVNcommandBuffer *cmdBuf,
                                            NVNbufferAddress indirectBuffer);

/// \brief Specify the rectangle for viewport 0.
///
/// \param cmdBuf Command buffer used to perform the update.
///
/// \param x Smallest X coordinate of the viewport rectangle.
///
/// \param y Smallest Y coordinate of the viewport rectangle.
///
/// \param w Width of the viewport rectangle.
///
/// \param h Height of the viewport rectangle.
NVNAPI void NVNAPIENTRY
    nvnCommandBufferSetViewport(NVNcommandBuffer *cmdBuf,
                                int x,
                                int y,
                                int w,
                                int h);

/// \brief Specify one or more viewport rectangles.
///
/// \param cmdBuf Command buffer used to perform the update.
///
/// \param first Index of the first viewport to modify.
///
/// \param count Number of contiguous viewports to set.
///
/// \param ranges An array of values to use for the viewports.
/// The array must contain 4 * _count_ values corresponding to the range of
/// viewports specified by _first_ and _count_. Each quartet of values
/// corresponds to, in order: the smallest X coordinate of the viewport,
/// the smallest Y coordinate of the viewport, the width of the viewport,
/// and the height of the viewport.
NVNAPI void NVNAPIENTRY
    nvnCommandBufferSetViewports(NVNcommandBuffer *cmdBuf,
                                 int first,
                                 int count,
                                 const float *ranges);

/// \brief Specify a set of viewport swizzle selectors for one or more
/// viewport rectangles.
///
/// As part of the viewport transformation, each vertex is transformed by
/// the viewport swizzle operation before the vertex is transformed to
/// screen space.  The viewport swizzle produces a four-component swizzled
/// position, where each component is produced by selecting and optionally
/// negating one of the four compenents of the original vertex position.
/// This command specifies sets of four swizzle selectors used to produce
/// the transformed position.  The default swizzles for each viewport are
/// POSITIVE_X, POSITIVE_Y, POSITIVE_Z, and POSITIVE_W, which specify no
/// change to the incoming position.
///
/// \note There is a separate set of four viewport swizzle values for each
/// viewport.  A primitive broadcast to multiple viewports will have a
/// separate swizzle applied for each viewport.
///
/// \note Viewport swizzles rasterization is only supported on NX and
/// second-generation Maxwell and later GPUs. Query the
/// SUPPORTS_VIEWPORT_SWIZZLE device property to determine if this command
/// is supported.
///
/// \param cmdBuf Command buffer used to perform the update.
///
/// \param first Index of the first viewport to modify.
///
/// \param count Number of consecutive viewports to update.
///
/// \param swizzles An array of swizzle values to use for the viewports.
/// This array must contain 4 * _count_ values corresponding to the range
/// of viewports specified by _first_ and _count_.  Each set of four values
/// identifies the input position component used for the X, Y, Z, and W
/// components, respectively, of the position vector produced by the
/// viewport swizzle operation.
NVNAPI void NVNAPIENTRY
    nvnCommandBufferSetViewportSwizzles(NVNcommandBuffer *cmdBuf,
                                        int first,
                                        int count,
                                        const NVNviewportSwizzle *swizzles);

/// \brief Specify the scissor rectangle used for clipping primitives and
/// clears.
///
/// \param cmdBuf Command buffer used to perform the update.
///
/// \param x Smallest X coordinate of the scissor rectangle.
///
/// \param y Smallest Y coordinate of the scissor rectangle.
///
/// \param w Width of the scissor rectangle.
///
/// \param h Height of the scissor rectangle.
NVNAPI void NVNAPIENTRY
    nvnCommandBufferSetScissor(NVNcommandBuffer *cmdBuf,
                               int x,
                               int y,
                               int w,
                               int h);

/// \brief Specify one or more scissor rectangles.
///
/// \param cmdBuf Command buffer used to perform the update.
///
/// \param first Index of the first scissor rectangle to modify.
///
/// \param count Number of contiguous scissor rectangles to set.
///
/// \param rects An array of values to use for the scissor rectangles.
/// The array must contain 4 * _count_ values corresponding to the range of
/// scissor rectangles specified by _first_ and _count_. Each quartet of
/// values corresponds to, in order: the smallest X coordinate of the
/// rectangle, the smallest Y coordinate of the rectangle, the width of the
/// rectangle, and the height of the rectangle.
NVNAPI void NVNAPIENTRY
    nvnCommandBufferSetScissors(NVNcommandBuffer *cmdBuf,
                                int first,
                                int count,
                                const int *rects);

/// \brief Set the range of depth values for rendered primitives.
///
/// \param cmdBuf Command buffer used to change the depth range.
///
/// \param n Depth value corresponding to the near clip plane.
///
/// \param f Depth value corresponding to the far clip plane.
NVNAPI void NVNAPIENTRY
    nvnCommandBufferSetDepthRange(NVNcommandBuffer *cmdBuf,
                                  float n,
                                  float f);

/// \brief Control the operation of the depth bounds test.
///
/// This command lets applications enable or disable the depth bounds test.
///  When enabled, the depth bounds test will discard any fragments where
/// the stored Z value in the depth buffer is outside the range [_n_, _f_].
///  By default, the depth bounds test is disabled.
///
/// \warning When the depth bounds test is enabled, _n_ must be less than
/// or equal to _f_.
///
/// \param cmdBuf Command buffer used to change the depth bounds test.
///
/// \param enable Enable or disable the depth bounds test.
///
/// \param n Minimum value of the depth bounds test range.
///
/// \param f Maximum value of the depth bounds test range.
NVNAPI void NVNAPIENTRY
    nvnCommandBufferSetDepthBounds(NVNcommandBuffer *cmdBuf,
                                   NVNboolean enable,
                                   float n,
                                   float f);

/// \brief Sets multiple ranges of depth values for rendered primitives.
///
/// \param cmdBuf Command buffer used to perform the update.
///
/// \param first Index of the first depth range to modify.
///
/// \param count Number of contiguous depth ranges to set.
///
/// \param ranges An array of values to use for the depth ranges. It must
/// contain _count_ pairs of values corresponding to the depth ranges
/// specified by _first_ and _count_.
NVNAPI void NVNAPIENTRY
    nvnCommandBufferSetDepthRanges(NVNcommandBuffer *cmdBuf,
                                   int first,
                                   int count,
                                   const float *ranges);

/// \brief Perform a specified operation related to tiled caching.
///
/// \param cmdBuf Command buffer used to perform the update.
///
/// \param action Tiled caching operation to perform.
/// Actions include enabling, disabling, or flushing the tiled cache
/// binner.
NVNAPI void NVNAPIENTRY
    nvnCommandBufferSetTiledCacheAction(NVNcommandBuffer *cmdBuf,
                                        NVNtiledCacheAction action);

/// \brief Sets the tile size used for tiled caching.
///
/// If the tile size has changed, the hardware will automatically flush the
/// tiled cache binner.  The tile size is clamped to [16, 16384] in either
/// dimension.
///
/// \param cmdBuf Command buffer used to perform the update.
///
/// \param tileWidth Sets the tile width, in pixels.
///
/// \param tileHeight Sets the tile height, in pixels.
NVNAPI void NVNAPIENTRY
    nvnCommandBufferSetTiledCacheTileSize(NVNcommandBuffer *cmdBuf,
                                          int tileWidth,
                                          int tileHeight);

/// \brief Bind a separate NVNtexture to use for texture mapping in
/// shaders.
///
/// This command binds a separate texture handle (without sampler
/// information) for use with shaders using separate texture types.  These
/// bindings are not used for variables with combined sampler types (such
/// as "sampler2D" in GLSL shaders).
///
/// \param cmdBuf Command buffer used to perform the update.
///
/// \param stage Shader stage owning the separate NVNtexture binding point
/// to update.
///
/// \param index Separate NVNtexture binding point number to update.
/// Must be less than 128 (the value of
/// #NVN_DEVICE_INFO_SEPARATE_TEXTURE_BINDINGS_PER_STAGE).
///
/// \param textureHandle Separate NVNtexture handle for the separate
/// NVNtexture object to bind.
NVNAPI void NVNAPIENTRY
    nvnCommandBufferBindSeparateTexture(NVNcommandBuffer *cmdBuf,
                                        NVNshaderStage stage,
                                        int index,
                                        NVNseparateTextureHandle textureHandle);

/// \brief Bind a separate NVNsampler to use for texture mapping in
/// shaders.
///
/// This command binds a separate sampler handle (without texture
/// information) for use with shaders using separate sampler types.  These
/// bindings are not used for variables with combined sampler types (such
/// as "sampler2D" in GLSL shaders).
///
/// \param cmdBuf Command buffer used to perform the update.
///
/// \param stage Shader stage owning the separate NVNsampler binding point
/// to update.
///
/// \param index Separate NVNsampler binding point number to update.
/// Must be less than 32 (the value of
/// #NVN_DEVICE_INFO_SEPARATE_SAMPLER_BINDINGS_PER_STAGE).
///
/// \param samplerHandle Separate NVNsampler handle for the separate
/// NVNsampler object to bind.
NVNAPI void NVNAPIENTRY
    nvnCommandBufferBindSeparateSampler(NVNcommandBuffer *cmdBuf,
                                        NVNshaderStage stage,
                                        int index,
                                        NVNseparateSamplerHandle samplerHandle);

/// \brief Bind one or more separate textures to use for texture mapping in
/// shaders.
///
/// This command binds one or more separate texture handles (without
/// sampler information) for use with shaders using separate texture types.
///  These bindings are not used for variables with combined sampler types
/// (such as "sampler2D" in GLSL shaders).  This command is equivalent to
/// making multiple calls to BindSeparateTexture.
///
/// \param cmdBuf Command buffer used to perform the bind operation.
///
/// \param stage Shader stage owning the binding points to update.
///
/// \param first First separate texture binding point number to update.
/// Must be less than 128 (the value of
/// #NVN_DEVICE_INFO_SEPARATE_TEXTURE_BINDINGS_PER_STAGE).
///
/// \param count Number of contiguous separate texture binding points to
/// update.
/// Must be less than or equal to 128 (the value of
/// #NVN_DEVICE_INFO_SEPARATE_TEXTURE_BINDINGS_PER_STAGE).
///
/// \param textureHandles Array of handles of separate NVNtexture objects
/// to bind.
NVNAPI void NVNAPIENTRY
    nvnCommandBufferBindSeparateTextures(NVNcommandBuffer *cmdBuf,
                                         NVNshaderStage stage,
                                         int first,
                                         int count,
                                         const NVNseparateTextureHandle *textureHandles);

/// \brief Bind one or more Samplers to use for texture mapping in shaders.
///
/// This command binds one or more separate sampler handles (without
/// texture information) for use with shaders using separate sampler types.
///  These bindings are not used for variables with combined sampler types
/// (such as "sampler2D" in GLSL shaders).  This command is equivalent to
/// making multiple calls to BindSeparateSampler.
///
/// \param cmdBuf Command buffer used to perform the bind operation.
///
/// \param stage Shader stage owning the binding points to update.
///
/// \param first First separate sampler binding point number to update.
/// Must be less than 32 (the value of
/// #NVN_DEVICE_INFO_SEPARATE_SAMPLER_BINDINGS_PER_STAGE).
///
/// \param count Number of contiguous separate sampler binding points to
/// update.
/// Must be less than or equal to 32 (the value of
/// #NVN_DEVICE_INFO_SEPARATE_SAMPLER_BINDINGS_PER_STAGE).
///
/// \param samplerHandles Array of handles of separate NVNsampler objects
/// to bind.
NVNAPI void NVNAPIENTRY
    nvnCommandBufferBindSeparateSamplers(NVNcommandBuffer *cmdBuf,
                                         NVNshaderStage stage,
                                         int first,
                                         int count,
                                         const NVNseparateSamplerHandle *samplerHandles);

/// \brief Set a mask that will be applied to the stencil index and
/// reference values prior to performing the stencil test.
///
/// There are separate stencil value masks for front- and back-facing
/// primitives. This function can update the value of either or both masks.
///  If stencil testing is disabled, the stencil value mask has no effect.
/// The default value for both front and back masks is 0xFF (all bits set).
///
/// \note In API versions prior to 53.8, the default mask was
/// unintentionally set to zero.
///
/// \param cmdBuf Command buffer used to perform the update.
///
/// \param faces Specifies the face(s) (front and/or back) using the new
/// stencil value mask.
///
/// \param mask Mask to be applied to stencil index and reference values
/// prior to the stencil test
NVNAPI void NVNAPIENTRY
    nvnCommandBufferSetStencilValueMask(NVNcommandBuffer *cmdBuf,
                                        NVNface faces,
                                        int mask);

/// \brief Specify a mask to apply to stencil values written by the stencil
/// test.
///
/// Bits in the existing stencil index corresponding to zero bits in the
/// stencil mask will not be updated by the stencil test. There are
/// separate stencil masks for front- and back-facing primitives. This
/// function can update the value of either or both masks.  If stencil
/// testing is disabled, the stencil mask has no effect.  The default value
/// for both front and back masks is 0xFF (all bits set).
///
/// \note In API versions prior to 53.8, the default mask was
/// unintentionally set to zero.
///
/// \param cmdBuf Command buffer used to perform the update.
///
/// \param faces Specifies the face(s) (front and/or back) using the new
/// stencil mask.
///
/// \param mask Mask to apply to stencil values written by the stencil
/// test.
NVNAPI void NVNAPIENTRY
    nvnCommandBufferSetStencilMask(NVNcommandBuffer *cmdBuf,
                                   NVNface faces,
                                   int mask);

/// \brief Specify a stencil reference value used by the stencil test.
///
/// There are separate reference values for front- and back-facing
/// primitives. This function can update the value of either or both
/// values.  If stencil testing is disabled, the stencil reference value
/// has no effect.  The default reference value for both front and back is
/// zero.
///
/// \param cmdBuf Command buffer used to perform the update.
///
/// \param faces Specifies the face(s) (front and/or back) using the new
/// stencil reference value.
///
/// \param ref Stencil reference value used by the stencil test.
NVNAPI void NVNAPIENTRY
    nvnCommandBufferSetStencilRef(NVNcommandBuffer *cmdBuf,
                                  NVNface faces,
                                  int ref);

/// \brief Specifies a constant color that may be used by blending
/// functions.
///
/// \param cmdBuf Command buffer used to perform the update.
///
/// \param blendColor Array of four floating-point (RGBA) blend color
/// values.
NVNAPI void NVNAPIENTRY
    nvnCommandBufferSetBlendColor(NVNcommandBuffer *cmdBuf,
                                  const float *blendColor);

/// \brief Specifies the size (in pixels) of point primitives to render.
///
/// If the last enabled vertex, tessellation control, tessellation
/// evaluation, or geometry shader writes to gl_PointSize, this value is
/// ignored and point size written in that shader will be used.
///
/// \param cmdBuf Command buffer used to perform the update.
///
/// \param pointSize Size (in pixels) of point primitives to render.
NVNAPI void NVNAPIENTRY
    nvnCommandBufferSetPointSize(NVNcommandBuffer *cmdBuf,
                                 float pointSize);

/// \brief Specifies the width (in pixels) of line primitives to render.
///
/// \param cmdBuf Command buffer used to perform the update.
///
/// \param lineWidth Width (in pixels) of line primitives to render.
NVNAPI void NVNAPIENTRY
    nvnCommandBufferSetLineWidth(NVNcommandBuffer *cmdBuf,
                                 float lineWidth);

/// \brief Specifies values used to compute a depth offset value when
/// polygon offset is enabled.
///
/// The offset is computed by multiplying the maximum depth slope of the
/// primitive [sqrt((dz/dx)^2 + (dz/dy)^2)] by _factor_, adding in the
/// minimum resolvable difference in depth values multiplied by _units_,
/// and clamping the result as described in the documentation for _clamp_.
///
/// \param cmdBuf Command buffer used to perform the update.
///
/// \param factor Value multiplied by the maximum depth slope when
/// computing offset.
///
/// \param units Value multiplied by the minimum resolvable depth
/// difference when computing offset.
///
/// \param clamp Value used to clamp the computed offset.
/// If `clamp` is positive, the computed depth offset will be clamped to a
/// value less than or equal to `clamp`. If `clamp` is negative, the
/// computed depth offset will be clamped to a value greater than or equal
/// to `clamp`. If `clamp` is zero, the computed depth offset will not be
/// clamped.
NVNAPI void NVNAPIENTRY
    nvnCommandBufferSetPolygonOffsetClamp(NVNcommandBuffer *cmdBuf,
                                          float factor,
                                          float units,
                                          float clamp);

/// \brief Specifies a reference value to use for alpha testing.
///
/// \param cmdBuf Command buffer used to perform the update.
///
/// \param ref Reference value for the alpha test.
NVNAPI void NVNAPIENTRY
    nvnCommandBufferSetAlphaRef(NVNcommandBuffer *cmdBuf,
                                float ref);

/// \brief Specifies a mask of samples that may be covered by rendered
/// primitives.
///
/// If bit N is zero in the provided mask, sample N will never be
/// considered to be covered when rasterizing primitives.
///
/// The default sample mask is ~0 (all bits enabled).
///
/// \param cmdBuf Command buffer used to perform the update.
///
/// \param mask Mask of samples that may be covered by rendered primitives.
NVNAPI void NVNAPIENTRY
    nvnCommandBufferSetSampleMask(NVNcommandBuffer *cmdBuf,
                                  int mask);

/// \brief Enable or disable rasterizer discard.
///
/// \param cmdBuf Command buffer used to change the rasterizer discard
/// state.
/// When enabled, rasterizer discard instructs the GPU to discard all
/// primitives produced by Draw* commands prior to rasterization and
/// fragment processing.  Such primitives are still processed by vertex,
/// tessellation, and geometry shaders and will be captured by transform
/// feedback, if enabled.  Additionally, when enabled, clears of color,
/// depth, and stencil buffers will be discarded.
///
/// \param discard True if primitives should be discarded; false (default)
/// otherwise.
NVNAPI void NVNAPIENTRY
    nvnCommandBufferSetRasterizerDiscard(NVNcommandBuffer *cmdBuf,
                                         NVNboolean discard);

/// \brief Enable/disable depth clamping of rendered primitives.
///
/// When enabled, primitives are not clipped to the near and far clip
/// planes.  Instead, primitives are rasterized without clipping in Z, and
/// interpolated Z values are clamped to the extents of the depth range.
///
/// \param cmdBuf Command buffer used to change the depth range.
///
/// \param clamp TRUE if depth clamping should be enabled; FALSE otherwise.
NVNAPI void NVNAPIENTRY
    nvnCommandBufferSetDepthClamp(NVNcommandBuffer *cmdBuf,
                                  NVNboolean clamp);

/// \brief Enable/disable conservative rasterization of rendered
/// primitives.
///
/// Conservative rasterization is only supported on second-generation
/// Maxwell and later GPUs. Query the SUPPORTS_CONSERVATIVE_RASTER device
/// property to determine support.
///
/// By default, conservative rasterization is disabled.
///
/// \param cmdBuf Command buffer used to toggle conservative rasterization.
///
/// \param enable TRUE if conservative rasterization should be enabled;
/// FALSE, otherwise.
NVNAPI void NVNAPIENTRY
    nvnCommandBufferSetConservativeRasterEnable(NVNcommandBuffer *cmdBuf,
                                                NVNboolean enable);

/// \brief Specifies the degree of primitive dilation when conservative
/// rasterization is enabled.
///
/// This provides a mechanism for "over-conservative" rasterization by
/// dilating (expanding) primitives prior to rasterization. *dilate* is
/// automatically clamped to the range [0.0, 0.75] and rounded up to a
/// multiple of 0.25. When *dilate* is 0.0, ordinary conservative
/// rasterization is performed. When dilate is greater than 0.0, rasterized
/// primitives are dilated by *dilate* pixels. If conservative
/// rasterization is disabled, dilation has no effect on rasterization.
/// This function is only supported on second-generation Maxwell and later
/// GPUs. Query the SUPPORTS_CONSERVATIVE_RASTER device property to
/// determine support.
///
/// By default, conservative rasterization is disabled.
///
/// \warning Changing the dilation setting requires idling the GPU and
/// results in reduced performance. Applications should avoid changing the
/// dilation factor frequently.
///
/// \param cmdBuf Command buffer used to update conservative raster
/// dilation.
///
/// \param dilate The fraction of a pixel by which to dilate rasterized
/// primitives.
NVNAPI void NVNAPIENTRY
    nvnCommandBufferSetConservativeRasterDilate(NVNcommandBuffer *cmdBuf,
                                                float dilate);

/// \brief Specifies the number of additional bits of subpixel precision to
/// be used for primitive rasterization when conservative rasterization is
/// enabled.
///
/// When vertices are transformed to window coordinates, they are snapped
/// to fixed-point positions. By default, the number of bits of fractional
/// precision within a pixel is given by the device property SUBPIXEL_BITS.
/// This function enables applications to increase the subpixel precision
/// for snapped vertex positions. If conservative rasterization is
/// disabled, additional bias bits have no effect on rasterization.
///
/// Additional subpixel precision allows for consistent conservative
/// rasterization when rendering the same geometry at different
/// resolutions. Rasterizing a scene at 64x64 with 3 extra subpixel bits in
/// X and Y will have the same snapping behavior as rasterizing the same
/// scene at 512x512 with no extra subpixel bits.
///
/// By default, the subpixel precision bias is 0.
///
/// \param cmdBuf Command buffer used to update conservative raster bias
/// bits.
///
/// \param xBits The number of additional bits of subpixel precision in the
/// horizontal direction.
/// Must be less than or equal to 8 (the value of
/// #NVN_DEVICE_INFO_MAX_SUBPIXEL_BIAS_BITS).
///
/// \param yBits The number of additional bits of subpixel precision in the
/// vertical direction.
/// Must be less than or equal to 8 (the value of
/// #NVN_DEVICE_INFO_MAX_SUBPIXEL_BIAS_BITS).
NVNAPI void NVNAPIENTRY
    nvnCommandBufferSetSubpixelPrecisionBias(NVNcommandBuffer *cmdBuf,
                                             int xBits,
                                             int yBits);

/// \brief Copy texture data from buffer memory into a region of a
/// NVNtexture object.
///
/// \param cmdBuf NVNcommandBuffer used to perform the copy.
///
/// \param src Address of the buffer holding the source data for the copy.
///
/// \param dstTexture NVNtexture object used as the destination for the
/// copy.
///
/// \param dstView View of the texture object used as the destination for
/// the copy.
/// A texture view, if specified, can be used to override properties of the
/// corresponding texture _dstTexture_. This parameter identifies a
/// specific mipmap level, set of layers or alternate format in the
/// specified texture. Other fields, such as swizzle, are ignored. If NULL
/// is specified, the base level of the texture _dstTexture_ will be used
/// as-is.
///
/// \param dstRegion Region of the destination texture to update for the
/// copy.
///
/// \param flags Specifies options used when performing the copy. See
/// #NVNcopyFlags.
NVNAPI void NVNAPIENTRY
    nvnCommandBufferCopyBufferToTexture(NVNcommandBuffer *cmdBuf,
                                        NVNbufferAddress src,
                                        const NVNtexture *dstTexture,
                                        const NVNtextureView *dstView,
                                        const NVNcopyRegion *dstRegion,
                                        int flags);

/// \brief Copy texture data from a region of a NVNtexture object into
/// buffer memory.
///
/// \param cmdBuf NVNcommandBuffer used to perform the copy.
///
/// \param srcTexture NVNtexture object used as the source for the copy.
///
/// \param srcView View of the texture object used as the source for the
/// copy.
/// A texture view, if specified, can be used to override properties of the
/// corresponding texture _srcTexture_. This parameter identifies a
/// specific mipmap level, set of layers or alternate format in the
/// specified texture. Other fields, such as swizzle, are ignored. If NULL
/// is specified, the base level of the texture _srcTexture_ will be used
/// as-is.
///
/// \param srcRegion Region of the source texture to copy from.
///
/// \param dst Address of the buffer holding the destination data for the
/// copy.
///
/// \param flags Specifies options used when performing the copy. See
/// #NVNcopyFlags.
NVNAPI void NVNAPIENTRY
    nvnCommandBufferCopyTextureToBuffer(NVNcommandBuffer *cmdBuf,
                                        const NVNtexture *srcTexture,
                                        const NVNtextureView *srcView,
                                        const NVNcopyRegion *srcRegion,
                                        NVNbufferAddress dst,
                                        int flags);

/// \brief Copy texture data from a region of one NVNtexture object into a
/// region of another NVNtexture object.
///
/// Copies texture data from the region _srcRegion_ of a view of the source
/// texture to the region _dstRegion_ of a view of the destination texture.
///  If _srcRegion_ and _dstRegion_ are of different sizes, the source
/// region will be scaled when copying.  If
/// #NVN_COPY_FLAGS_LINEAR_FILTER_BIT is set in _flags_ when scaling, the
/// copy will average texels from the source.  Otherwise, the copy will
/// select the nearest source texel for each destination texel.
///
/// \note If the source and destination regions overlap in memory, the
/// results of the copy will be undefined.
///
/// \note Filtering across layers of a source texture is not supported; the
/// number of layers in _srcRegion_ and _dstRegion_ must match.
///
/// \note Linear filtering is supported for a limited subset of source and
/// destination formats, as documented in the Programming Guide.
///
/// \note Copies involving multisample textures are not supported.
///
/// \note Copies of compressed formats must be aligned on compression block
/// boundaries and do not support scaling.
///
/// \param cmdBuf NVNcommandBuffer used to perform the copy.
///
/// \param srcTexture NVNtexture object holding the source data for the
/// copy.
///
/// \param srcView View of the texture object used as the source for the
/// copy.
/// A texture view, if specified, can be used to override properties of the
/// corresponding texture _srcTexture_. This parameter identifies a
/// specific mipmap level, set of layers or alternate format in the
/// specified texture. Other fields, such as swizzle, are ignored. If NULL
/// is specified, the base level of the texture _srcTexture_ will be used
/// as-is.
///
/// \param srcRegion Region of the source texture to read from.
///
/// \param dstTexture NVNtexture object used as the destination for the
/// copy.
///
/// \param dstView View of the texture object used as the destination for
/// the copy.
/// A texture view, if specified, can be used to override properties of the
/// corresponding texture _dstTexture_. This parameter identifies a
/// specific mipmap level, set of layers or alternate format in the
/// specified texture. Other fields, such as swizzle, are ignored. If NULL
/// is specified, the base level of the texture _dstTexture_ will be used
/// as-is.
///
/// \param dstRegion Region of the destination texture to write to.
///
/// \param flags Specifies options used when performing the copy. See
/// #NVNcopyFlags.
NVNAPI void NVNAPIENTRY
    nvnCommandBufferCopyTextureToTexture(NVNcommandBuffer *cmdBuf,
                                         const NVNtexture *srcTexture,
                                         const NVNtextureView *srcView,
                                         const NVNcopyRegion *srcRegion,
                                         const NVNtexture *dstTexture,
                                         const NVNtextureView *dstView,
                                         const NVNcopyRegion *dstRegion,
                                         int flags);

/// \brief Schedule a copy of data from one NVNbuffer object into another
/// NVNbuffer object.
///
/// \param cmdBuf NVNcommandBuffer used to perform the copy.
///
/// \param src Address of the buffer holding the source data for the copy.
///
/// \param dst Address of the buffer holding the destination data for the
/// copy.
///
/// \param size Number of bytes to copy between source and destination
/// buffers.
///
/// \param flags Specifies options used when performing the copy. See of
/// #NVNcopyFlags.
NVNAPI void NVNAPIENTRY
    nvnCommandBufferCopyBufferToBuffer(NVNcommandBuffer *cmdBuf,
                                       NVNbufferAddress src,
                                       NVNbufferAddress dst,
                                       size_t size,
                                       int flags);

/// \brief Clear a buffer to a 32-bit integer value.
///
/// \param cmdBuf NVNcommandBuffer used to perform the clear.
///
/// \param dst Address of the buffer holding the destination data for the
/// clear.  This address should be aligned to 4 bytes.
///
/// \param size Number of bytes to clear.
/// Size must be a multiple of four.  Clears with zero size are treated as
/// no-ops.
///
/// \param value 32-bit integer value to clear with.
NVNAPI void NVNAPIENTRY
    nvnCommandBufferClearBuffer(NVNcommandBuffer *cmdBuf,
                                NVNbufferAddress dst,
                                size_t size,
                                uint32_t value);

/// \brief Clear a color buffer using floating point clear color values.
///
/// Clears texture data from the region _region_ of a view of the
/// destination texture.
///
/// This command supports only texture formats that can be bound as a
/// render target.  Please refer to the 'Render' column in the programming
/// guide's \ref nvn_guide_formats_table "formats table" for supported
/// formats.
///
/// \warning Unlike Copy commands, ClearTexure renders to the texture using
/// the 3D Engine. Before accessing the cleared texture, applications must
/// ensure that the GPU commands producing the new data have completed with
/// #NVNbarrierBits of ORDER_FRAGMENTS and INVALIDATE_TEXTURE or by waiting
/// on a NVNsync object.
///
/// \param cmdBuf NVNcommandBuffer used to perform the clear.
///
/// \param dstTexture NVNtexture object used as the destination for the
/// clear.
///
/// \param dstView View of the texture object used as the destination for
/// the clear.
/// A texture view, if specified, can be used to override properties of the
/// corresponding texture _dstTexture_.  If NULL is specified, the base
/// level of the texture _dstTexture_ will be used as-is.
///
/// \param dstRegion Region of the destination texture to write to.
///
/// \param color Array of four floating-point (RGBA) clear color values.
///
/// \param mask Bitfield indicating which color channels should be cleared.
/// See of #NVNclearColorMask.
NVNAPI void NVNAPIENTRY
    nvnCommandBufferClearTexture(NVNcommandBuffer *cmdBuf,
                                 const NVNtexture *dstTexture,
                                 const NVNtextureView *dstView,
                                 const NVNcopyRegion *dstRegion,
                                 const float *color,
                                 int mask);

/// \brief Clear a color buffer using signed integer clear color values.
///
/// Clears texture data from the region _region_ of a view of the
/// destination texture.
///
/// This command supports only texture formats that can be bound as a
/// render target.  Please refer to the 'Render' column in the programming
/// guide's \ref nvn_guide_formats_table "formats table" for supported
/// formats.
///
/// \warning Unlike Copy commands, ClearTexure renders to the texture using
/// the 3D Engine. Before accessing the cleared texture, applications must
/// ensure that the GPU commands producing the new data have completed with
/// #NVNbarrierBits of ORDER_FRAGMENTS and INVALIDATE_TEXTURE or by waiting
/// on a NVNsync object.
///
/// \param cmdBuf NVNcommandBuffer used to perform the clear.
///
/// \param dstTexture NVNtexture object used as the destination for the
/// clear.
///
/// \param dstView View of the texture object used as the destination for
/// the clear.
/// A texture view, if specified, can be used to override properties of the
/// corresponding texture _dstTexture_.  If NULL is specified, the base
/// level of the texture _dstTexture_ will be used as-is.
///
/// \param dstRegion Region of the destination texture to write to.
///
/// \param color Array of four signed integer (RGBA) clear color values.
///
/// \param mask Bitfield indicating which color channels should be cleared.
/// See #NVNclearColorMask.
NVNAPI void NVNAPIENTRY
    nvnCommandBufferClearTexturei(NVNcommandBuffer *cmdBuf,
                                  const NVNtexture *dstTexture,
                                  const NVNtextureView *dstView,
                                  const NVNcopyRegion *dstRegion,
                                  const int *color,
                                  int mask);

/// \brief Clear a color buffer using unsigned integer clear color values.
///
/// Clears texture data from the region _region_ of a view of the
/// destination texture.
///
/// This command supports only texture formats that can be bound as a
/// render target.  Please refer to the 'Render' column in the programming
/// guide's \ref nvn_guide_formats_table "formats table" for supported
/// formats.
///
/// \warning Unlike Copy commands, ClearTexure renders to the texture using
/// the 3D Engine. Before accessing the cleared texture, applications must
/// ensure that the GPU commands producing the new data have completed with
/// #NVNbarrierBits of ORDER_FRAGMENTS and INVALIDATE_TEXTURE or by waiting
/// on a NVNsync object.
///
/// \param cmdBuf NVNcommandBuffer used to perform the clear.
///
/// \param dstTexture NVNtexture object used as the destination for the
/// clear.
///
/// \param dstView View of the texture object used as the destination for
/// the clear.
/// A texture view, if specified, can be used to override properties of the
/// corresponding texture _dstTexture_.  If NULL is specified, the base
/// level of the texture _dstTexture_ will be used as-is.
///
/// \param dstRegion Region of the destination texture to write to.
///
/// \param color Array of four unsigned integer (RGBA) clear color values.
///
/// \param mask Bitfield indicating which color channels should be cleared.
/// See of #NVNclearColorMask.
NVNAPI void NVNAPIENTRY
    nvnCommandBufferClearTextureui(NVNcommandBuffer *cmdBuf,
                                   const NVNtexture *dstTexture,
                                   const NVNtextureView *dstView,
                                   const NVNcopyRegion *dstRegion,
                                   const uint32_t *color,
                                   int mask);

/// \brief Update the contents of a uniform buffer.
///
/// This command schedules an update of the contents of a uniform buffer,
/// which will execute without waiting for previous draw calls prior to the
/// update.  The update will wait on the completion of previous compute
/// dispatches.  Draw calls and compute dispatches issued after the update
/// will use the new uniform buffer values.  Calls before the update will
/// use the old values.
///
/// To get correct ordering of updates, the range of buffer memory
/// identified by _buffer_ and _bufferSize_ must exactly match the memory
/// range bound via nvnCommandBufferBindUniformBuffer.  If any other
/// uniform buffer binding (using a different base address or size)
/// overlaps the range of memory updated by this command, shader accesses
/// using that binding (from commands before or after the update) will
/// return undefined values.
///
/// \param cmdBuf NVNcommandBuffer used to perform the uniform buffer
/// update.
///
/// \param buffer GPU address of the full uniform buffer to update.
/// Must be a multiple of 256 (the value of
/// #NVN_DEVICE_INFO_UNIFORM_BUFFER_ALIGNMENT).
///
/// \param bufferSize Size (in bytes) of the full uniform buffer to update.
/// Must be less than or equal to 65536 (the value of
/// #NVN_DEVICE_INFO_MAX_UNIFORM_BUFFER_SIZE).
///
/// \param updateOffset Offset (in bytes) of the first byte to update
/// inside the uniform buffer.
/// Must be a multiple of 4 (the value of
/// #NVN_DEVICE_INFO_UNIFORM_BUFFER_UPDATE_ALIGNMENT).
///
/// \param updateSize Number of bytes to update inside the uniform buffer.
/// Must be less than or equal to 65536 (the value of
/// #NVN_DEVICE_INFO_MAX_UNIFORM_BUFFER_SIZE).
/// Must be a multiple of 4 (the value of
/// #NVN_DEVICE_INFO_UNIFORM_BUFFER_UPDATE_ALIGNMENT).
///
/// \param data New data to write into the uniform buffer.  The data is
/// consumed before the call returns.
NVNAPI void NVNAPIENTRY
    nvnCommandBufferUpdateUniformBuffer(NVNcommandBuffer *cmdBuf,
                                        NVNbufferAddress buffer,
                                        size_t bufferSize,
                                        ptrdiff_t updateOffset,
                                        size_t updateSize,
                                        const void *data);

/// \brief Write a GPU counter and timestamp into buffer object memory.
///
/// The GPU maintains a set of counters for each queue that will be
/// incremented on certain events.  This function requests that the current
/// value of the counter be written to a specified offset in the specified
/// buffer object.  16 bytes will be written to the buffer object.
///
/// Except for the #NVN_COUNTER_TYPE_ZCULL_STATS counter, the first 8 bytes
/// hold the 64-bit counter value and the second 8 bytes hold a timestamp
/// when the counter value was reported.  #NVN_COUNTER_TYPE_ZCULL_STATS
/// reports four 32-bit counter values.
///
/// \note If tiled caching is enabled, this command will trigger an
/// implicit tiled cache flush.
///
/// \note For timestamp, please refer to the \ref
/// nvn_guide_counter_resolution_of_timestamp "NVN Programming Guide" for
/// details.
///
/// \param cmdBuf NVNcommandBuffer used to perform the counter report
/// operation.
///
/// \param counter Type of counter that will be written.
///
/// \param buffer GPU address of buffer memory where the counter will be
/// written.
/// Must be a multiple of 16 (the value of
/// #NVN_DEVICE_INFO_COUNTER_ALIGNMENT).
NVNAPI void NVNAPIENTRY
    nvnCommandBufferReportCounter(NVNcommandBuffer *cmdBuf,
                                  NVNcounterType counter,
                                  NVNbufferAddress buffer);

/// \brief Reset a GPU counter to zero.
///
/// The GPU maintains a set of counters for each queue that will be
/// incremented on certain events.  This function resets the current value
/// of the counter to zero.
///
/// \note If tiled caching is enabled, this command will trigger an
/// implicit tiled cache flush.
///
/// \param cmdBuf NVNcommandBuffer used to perform the counter reset
/// operation.
///
/// \param counter Type of counter that will be written.
NVNAPI void NVNAPIENTRY
    nvnCommandBufferResetCounter(NVNcommandBuffer *cmdBuf,
                                 NVNcounterType counter);

/// \brief Write a GPU counter report and timestamp into buffer object
/// memory using an application-provided value.
///
/// This command will cause the GPU to write a counter report (of type
/// NVNcounterData) to buffer object memory, with the counter value
/// replaced by an unsigned 32-bit value provided in the command.  In the
/// report, the first 8 bytes hold a 64-bit integer set to the provided
/// report value and the second 8 bytes hold a timestamp when the report
/// was performed.
///
/// \note If tiled caching is enabled, this command will trigger an
/// implicit tiled cache flush.
///
/// \note For timestamp, please refer to the \ref
/// nvn_guide_counter_resolution_of_timestamp "NVN Programming Guide" for
/// the details.
///
/// \param cmdBuf NVNcommandBuffer used to perform the counter report
/// operation.
///
/// \param value Value that be written in the counter report.
///
/// \param buffer GPU address of buffer memory where the counter will be
/// written.
/// Must be a multiple of 16 (the value of
/// #NVN_DEVICE_INFO_COUNTER_ALIGNMENT).
NVNAPI void NVNAPIENTRY
    nvnCommandBufferReportValue(NVNcommandBuffer *cmdBuf,
                                uint32_t value,
                                NVNbufferAddress buffer);

/// \brief Sets the render enable for a queue.
///
/// The render enable allows applications to disable all rendering to a
/// queue unconditionally.  If rendering is disabled for a queue, all
/// rendering commands (framebuffer clears, Draw commands) will be
/// discarded without any processing.  Commands changing queue state will
/// be executed even if rendering is disabled and will affect subsequent
/// rendering commands if/when rendering is re-enabled. SetRenderEnable and
/// SetRenderEnableConditional affect the same state in the queue.
///
/// \param cmdBuf NVNcommandBuffer used to enable or disable rendering.
///
/// \param enable NVN_TRUE if rendering should be enabled; NVN_FALSE if
/// rendering should be disabled.
NVNAPI void NVNAPIENTRY
    nvnCommandBufferSetRenderEnable(NVNcommandBuffer *cmdBuf,
                                    NVNboolean enable);

/// \brief Enables rendering for a queue, depending on the equality of
/// counters.
///
/// Enable or disable rendering based on whether two 64-bit values in
/// buffer memory are equal. If the values are written using the
/// SAMPLES_PASSED counter, this function can be used in an implementation
/// of occlusion culling. SetRenderEnable and SetRenderEnableConditional
/// affect the same state in the queue.
///
/// \param cmdBuf NVNcommandBuffer for which rendering should be enabled or
/// disabled.
///
/// \param mode NVN_CONDITIONAL_RENDER_MODE_RENDER_IF_EQUAL if rendering
/// should be enabled if the buffer values are equal,
/// NVN_CONDITIONAL_RENDER_MODE_RENDER_IF_NOT_EQUAL otherwise.
///
/// \param counters NVNbuffer address of the two values to be compared.
/// There must be at least 32 bytes of buffer memory starting at this
/// address. The first value is contained in the first eight bytes starting
/// at this address, and the second value is contained in the eight bytes
/// starting at _bufferAddress_ + 16. Note that this layout is consistent
/// with two counter query results stored consecutively in memory.
/// Must be a multiple of 16 (the value of
/// #NVN_DEVICE_INFO_COUNTER_ALIGNMENT).
NVNAPI void NVNAPIENTRY
    nvnCommandBufferSetRenderEnableConditional(NVNcommandBuffer *cmdBuf,
                                               NVNconditionalRenderMode mode,
                                               NVNbufferAddress counters);

/// \brief Sets color and depth/stencil textures in the framebuffer.
///
/// \param cmdBuf NVNcommandBuffer used to program the new render targets.
///
/// \param numColors Number of color textures in the framebuffer.
/// If zero is specified, the framebuffer will be programmed with no color
/// textures.  Any color binding point numbered _numColors_ or higher will
/// be programmed with no color texture.
/// Must be less than or equal to 8 (the value of
/// #NVN_DEVICE_INFO_COLOR_BUFFER_BINDINGS).
///
/// \param colors Array of _numColors_ color textures to program in the
/// framebuffer.
/// If any element in the array is set to NULL, the corresponding color
/// target in the framebuffer will be programmed with no texture.
///
/// \param colorViews Array of _numColors_ color texture views to program
/// in the framebuffer.
/// The texture views, if specified, can be used to override properties of
/// the corresponding texture in the array _colors_.  If NULL is specified,
/// all textures in _colors_ will be used as-is.  If a non-NULL array of
/// views is specified but one or more entries in that array is NULL, the
/// corresponding textures in _colors_ will be used as-is.
///
/// \param depthStencil Depth/stencil texture to program in the
/// framebuffer.
/// If NULL is specified, the framebuffer will be programmed with no
/// depth/stencil buffer.
///
/// \param depthStencilView Depth/stencil texture view to program in the
/// framebuffer.
/// A texture view, if specified, can be used to override properties of the
/// corresponding texture _depthStencil_.  If NULL is specified, the
/// texture _depthStencil_ will be used as-is.
NVNAPI void NVNAPIENTRY
    nvnCommandBufferSetRenderTargets(NVNcommandBuffer *cmdBuf,
                                     int numColors,
                                     const NVNtexture * const *colors,
                                     const NVNtextureView * const *colorViews,
                                     const NVNtexture *depthStencil,
                                     const NVNtextureView *depthStencilView);

/// \brief Discard the contents of the specified bound color render target.
///
/// After discarding the contents of a color render target, its values will
/// become undefined. This command can be useful for saving memory
/// bandwidth for 'transient' rendering operations such as rendering to a
/// multisample texture.  After rendering to a multisample texture, the
/// contents are often filtered to produce single-sample texels using
/// nvnCommandBufferDownsample.  Discarding the contents of a multisample
/// texture after the downsample allows the GPU to throw away its contents
/// before writing values to memory.
///
/// Only the portion of the render target contained inside the current
/// scissor rectangle will be discarded.
///
/// \param cmdBuf NVNcommandBuffer used to perform the discard operation.
///
/// \param index Number of the color target whose contents should be
/// discarded.
/// Must be less than 8 (the value of
/// #NVN_DEVICE_INFO_COLOR_BUFFER_BINDINGS).
NVNAPI void NVNAPIENTRY
    nvnCommandBufferDiscardColor(NVNcommandBuffer *cmdBuf,
                                 int index);

/// \brief Discard the contents of the currently bound depth/stencil render
/// target.
///
/// After discarding the contents of a depth/stencil render target, its
/// values will become undefined. This command can be useful for saving
/// memory bandwidth for 'transient' rendering operations such as rendering
/// to a depth/stencil buffer that is only used to generate a color buffer
/// image in the same frame.  Discarding the contents of a texture allows
/// the GPU to throw away its contents before writing values to main
/// memory.
///
/// Only the portion of the render target contained inside the current
/// scissor rectangle will be discarded.
///
/// \param cmdBuf NVNcommandBuffer used to perform the discard operation.
NVNAPI void NVNAPIENTRY
    nvnCommandBufferDiscardDepthStencil(NVNcommandBuffer *cmdBuf);

/// \brief Computes a filtered version of a multisample source texture to
/// produce a single-sample destination texture.
///
/// This command completes the execution of all previous rendering commands
/// before performing the downsample operation.  In particular, it will
/// flush all primitives accumulated by tiled caching logic.  When using
/// tiled caching, nvnCommandBufferTiledDownsample will perform better.
/// This command is performed by the RSTR2D unit.
///
/// \param cmdBuf NVNcommandBuffer used to perform the downsample
/// operation.
/// When performing the downsample operation, any commands previously
/// submitted to the queue are completed prior to the downsample.  The
/// downsample does not synchronize with commands submitted to any other
/// queue.
///
/// \param src Source texture for the downsample operation.
///
/// \param dst Destination texture for the downsample operation.
NVNAPI void NVNAPIENTRY
    nvnCommandBufferDownsample(NVNcommandBuffer *cmdBuf,
                               const NVNtexture *src,
                               const NVNtexture *dst);

/// \brief Computes a filtered version of a multisample source texture to
/// produce a single-sample destination texture.
///
/// When tiled caching is enabled, the downsample operation performed by
/// this command will be accumulated along with other rendered primitives.
/// The actual downsample will be performed on a tile-by-tile basis, and
/// may complete on one tile before regular rendering for another tile
/// begins.  When tiled caching is disabled, this command will behave
/// similarly to nvnCommandBufferDownsample, which fully flushes previous
/// work before starting the downsample operation.  This command is
/// performed by the 3D pipeline.
///
/// \param cmdBuf NVNcommandBuffer used to perform the downsample
/// operation.
/// When performing the downsample operation, any commands previously
/// submitted to the queue are completed prior to the downsample.  The
/// downsample does not synchronize with commands submitted to any other
/// queue.
///
/// \param src Source texture for the downsample operation.
///
/// \param dst Destination texture for the downsample operation.
NVNAPI void NVNAPIENTRY
    nvnCommandBufferTiledDownsample(NVNcommandBuffer *cmdBuf,
                                    const NVNtexture *src,
                                    const NVNtexture *dst);

/// \brief Computes a filtered version of a multisample source texture and
/// texture view to produce a single-sample destination texture through a
/// texture view.
///
/// This command completes the execution of all previous rendering commands
/// before performing the downsample operation.  In particular, it will
/// flush all primitives accumulated by tiled caching logic.  When using
/// tiled caching, nvnCommandBufferTiledDownsample will perform better.
/// This command is performed by the RSTR2D unit. The texture views may
/// only describe one single level/layer of a texture.
///
/// \param cmdBuf NVNcommandBuffer used to perform the downsample
/// operation.
/// When performing the downsample operation, any commands previously
/// submitted to the queue are completed prior to the downsample.  The
/// downsample does not synchronize with commands submitted to any other
/// queue.
///
/// \param src Source texture for the downsample operation.
///
/// \param srcView Source texture view for the downsample operation.
/// Selects one level/layer.
///
/// \param dst Destination texture for the downsample operation.
///
/// \param dstView Destination texture view for the downsample operation.
/// Selects one level/layer
NVNAPI void NVNAPIENTRY
    nvnCommandBufferDownsampleTextureView(NVNcommandBuffer *cmdBuf,
                                          const NVNtexture *src,
                                          const NVNtextureView *srcView,
                                          const NVNtexture *dst,
                                          const NVNtextureView *dstView);

/// \brief Computes a filtered version of a multisample source texture
/// texture view to produce a single-sample destination texture view.
///
/// When tiled caching is enabled, the downsample operation performed by
/// this command will be accumulated along with other rendered primitives.
/// The actual downsample will be performed on a tile-by-tile basis, and
/// may complete on one tile before regular rendering for another tile
/// begins.  When tiled caching is disabled, this command will behave
/// similarly to nvnCommandBufferDownsample, which fully flushes previous
/// work before starting the downsample operation.  This command is
/// performed by the 3D pipeline. The texture views must each describe only
/// a single level/layer of a texture.
///
/// \param cmdBuf NVNcommandBuffer used to perform the downsample
/// operation.
/// When performing the downsample operation, any commands previously
/// submitted to the queue are completed prior to the downsample.  The
/// downsample does not synchronize with commands submitted to any other
/// queue.
///
/// \param src Source texture for the downsample operation.
///
/// \param srcView Source texture view for the downsample operation.
/// Selects one level/layer.
///
/// \param dst Destination texture for the downsample operation.
///
/// \param dstView Destination texture view for the downsample operation.
/// Selects one level/layer.
NVNAPI void NVNAPIENTRY
    nvnCommandBufferTiledDownsampleTextureView(NVNcommandBuffer *cmdBuf,
                                               const NVNtexture *src,
                                               const NVNtextureView *srcView,
                                               const NVNtexture *dst,
                                               const NVNtextureView *dstView);

/// \brief Specify a barrier ordering execution of GPU commands and
/// invalidating internal GPU caches.
///
/// Rendering and compute dispatch commands sent to NVN queues are executed
/// on the GPU using a hardware pipeline, with no guarantee that work for
/// one draw call or dispatch completes before work on the next draw call
/// or dispatch starts.  Additionally, some units of the GPU maintain
/// caches that are not automatically invalidated when cached data are
/// overwritten by the CPU or other units of the GPU.  This command allows
/// applications to order the processing of commands sent before and after
/// the barrier and to invalidate GPU caches before processing commands
/// sent after the barrier.
///
/// \param cmdBuf NVNcommandBuffer used to insert the barrier.
///
/// \param barrier Bitfield controlling the order of commands before and
/// after the barrier and the invalidation of GPU caches for commands after
/// the barrier. See #NVNbarrierBits.
NVNAPI void NVNAPIENTRY
    nvnCommandBufferBarrier(NVNcommandBuffer *cmdBuf,
                            int barrier);

/// \brief Wait for a NVNsync object to be signaled on the GPU before
/// processing any further commands in the specified NVNcommandBuffer
/// object.
///
/// \param cmdBuf NVNcommandBuffer object whose subsequent commands wait on
/// the NVNsync object.
///
/// \param sync NVNsync object to wait on.
NVNAPI void NVNAPIENTRY
    nvnCommandBufferWaitSync(NVNcommandBuffer *cmdBuf,
                             const NVNsync *sync);

/// \brief Mark a NVNsync object as signaled when all previous commands
/// have completed in the NVNcommandBuffer object.
///
/// \warning FenceSync commands are not automatically flushed for
/// processing by the GPU.  Before waiting on a FenceSync call with the CPU
/// using nvnSyncWait or from another NVNqueue using nvnQueueWaitSync,
/// applications must ensure that the previous FenceSync call was flushed
/// using nvnQueueFlush.  Waiting on a FenceSync call using
/// nvnQueueWaitSync in the same queue requires no manual flush.
///
/// \note If tiled caching is enabled, this command will trigger an
/// implicit tiled cache flush.
///
/// \param cmdBuf NVNcommandBuffer object with commands to wait on before
/// signaling completion.
///
/// \param sync NVNsync object to mark as signaled when commands complete.
///
/// \param condition Type of command completion to wait for.
///
/// \param flags Bitfield of additional operations to perform before
/// signaling. See #NVNsyncFlagBits.
NVNAPI void NVNAPIENTRY
    nvnCommandBufferFenceSync(NVNcommandBuffer *cmdBuf,
                              NVNsync *sync,
                              NVNsyncCondition condition,
                              int flags);

/// \brief Specify a new texture pool used for subsequent rendering
/// operations.
///
/// \param cmdBuf NVNcommandBuffer used for programming new texture pool.
///
/// \param texturePool NVNtexturePool object to be programmed.
NVNAPI void NVNAPIENTRY
    nvnCommandBufferSetTexturePool(NVNcommandBuffer *cmdBuf,
                                   const NVNtexturePool *texturePool);

/// \brief Specify a new sampler pool used for subsequent rendering
/// operations.
///
/// \param cmdBuf NVNcommandBuffer used for programming new sampler pool.
///
/// \param samplerPool NVNsamplerPool object to be programmed.
NVNAPI void NVNAPIENTRY
    nvnCommandBufferSetSamplerPool(NVNcommandBuffer *cmdBuf,
                                   const NVNsamplerPool *samplerPool);

/// \brief Specify scratch memory required for shader execution.
///
/// The scratch memory provided by this function allows shaders to require
/// temporary (scratch) memory during execution to save the results of
/// intermediate computations or flow control state.  Each queue has its
/// own block of scratch memory, programmed via command buffers.  Scratch
/// memory will be shared by all active shaders running on the queue.
/// Multiple queues can share the same block of scratch memory.
///
/// Each compiled shader will report a minimum and recommended amount of
/// scratch memory required.  We recommend that applications provide the
/// largest recommended amount of scratch memory for optimal performance.
/// Providing less memory may resolve in lower shader performance, and
/// providing less than the minimum amount of memory will result in
/// crashes.  For more information, consult the programming guide.
///
/// \param cmdBuf NVNcommandBuffer used for programming new scratch memory.
///
/// \param pool Memory pool providing storage for the shader scratch
/// memory.
///
/// \param offset Offset (in bytes) of the first byte of shader scratch
/// memory.
/// Must be a multiple of 4096 (the value of
/// #NVN_DEVICE_INFO_SHADER_SCRATCH_MEMORY_ALIGNMENT).
///
/// \param size Size (in bytes) of the scratch memory.
/// Must be a multiple of 131072 (the value of
/// #NVN_DEVICE_INFO_SHADER_SCRATCH_MEMORY_GRANULARITY).
NVNAPI void NVNAPIENTRY
    nvnCommandBufferSetShaderScratchMemory(NVNcommandBuffer *cmdBuf,
                                           const NVNmemoryPool *pool,
                                           ptrdiff_t offset,
                                           size_t size);

/// \brief Save the current ZCull state for the current depth render
/// target.
///
/// The ZCull unit of the GPU tracks the contents of the currently bound
/// depth render target and uses this information to perform high-speed
/// depth and stencil testing.  When applications switch between depth
/// render targets, ZCull discards tracking information for the old render
/// target. This command allows applications to instead save the ZCull
/// state to buffer memory so that it can be later restored when switching
/// back to the old target.  When saving information for a depth render
/// target, SaveZCullData should be called immediately before switching
/// render targets.
///
/// \param cmdBuf NVNcommandBuffer used to perform the operation.
///
/// \param dst GPU address of buffer object memory where the ZCull contents
/// will be stored.
/// Must be a multiple of 32 (the value of
/// #NVN_DEVICE_INFO_ZCULL_SAVE_RESTORE_ALIGNMENT).
///
/// \param size Size (in bytes) of the destination buffer.
/// To determine the amount of memory required to save and restore ZCull
/// data, call nvnTextureGetZCullStorageSize() or
/// nvnTextureBuilderGetZCullStorageSize().
NVNAPI void NVNAPIENTRY
    nvnCommandBufferSaveZCullData(NVNcommandBuffer *cmdBuf,
                                  NVNbufferAddress dst,
                                  size_t size);

/// \brief Update the current ZCull state to use saved state from a
/// previous render target.
///
/// The ZCull unit of the GPU tracks the contents of the currently bound
/// depth render target and uses this information to perform high-speed
/// depth and stencil testing.  When applications switch between depth
/// render targets, ZCull discards tracking information for the old render
/// target.  This command allows applications restore previously saved
/// ZCull state from buffer memory so that it can be used when continuing
/// to render to the new target.  When restoring information for a depth
/// render target, RestoreZCullData should be called immediately after
/// switching render targets.
///
/// \warning The data restored by this command must have been previously
/// saved by nvnCommandBufferSaveZCullData.  Additionally, the same
/// depth/stencil texture and texture view must be bound as a render target
/// at the time of the restore and the previous save.  In case of a
/// mismatch, the results of the restore are undefined and may result in
/// GPU exceptions.
///
/// \warning If the current contents of the depth render target are
/// inconsistent with the contents of the render target when SaveZCullData
/// was called, the state written by RestoreZCullData is invalid and the
/// results of depth and stencil testing are undefined.
///
/// \param cmdBuf NVNcommandBuffer used to perform the operation.
///
/// \param src GPU address of buffer object memory where the ZCull contents
/// will be loaded from.
/// Must be a multiple of 32 (the value of
/// #NVN_DEVICE_INFO_ZCULL_SAVE_RESTORE_ALIGNMENT).
///
/// \param size Size (in bytes) of the source buffer.
/// To determine the amount of memory required to save and restore ZCull
/// data, call nvnTextureGetZCullStorageSize() or
/// nvnTextureBuilderGetZCullStorageSize().
NVNAPI void NVNAPIENTRY
    nvnCommandBufferRestoreZCullData(NVNcommandBuffer *cmdBuf,
                                     NVNbufferAddress src,
                                     size_t size);

/// \brief Set the row stride used for copies between buffer and texture
/// memory.
///
/// This command sets the stride (in bytes) between rows of texels in
/// buffer memory when copying texels between buffer and texture memory.
/// With the default stride of zero (default), texel data in buffer memory
/// will be treated as densely packed, where each new row in memory starts
/// immediately after the end of the previous row.
///
/// \note Setting a stride affects any subsequent calls to
/// nvnCommandBufferCopyBufferToTexture and
/// nvnCommandBufferCopyTextureToBuffer using this command buffer.  Unlike
/// most NVNcommandBuffer calls, this command does not store the stride in
/// command or control memory and does not affect future submissions of
/// previously recorded copy commands or copy commands recorded using other
/// command buffers.
///
/// \param cmdBuf NVNcommandBuffer object used to store row stride.
///
/// \param stride Stride (in bytes) between rows of texels in buffer
/// memory.
NVNAPI void NVNAPIENTRY
    nvnCommandBufferSetCopyRowStride(NVNcommandBuffer *cmdBuf,
                                     ptrdiff_t stride);

/// \brief Set the image stride used for copies between buffer and texture
/// memory.
///
/// This command sets the stride (in bytes) between layers of texels in
/// buffer memory when copying texels between buffer and texture memory.
/// With the default stride of zero (default), texel data in buffer memory
/// will be treated as densely packed, where each new layer in memory
/// starts immediately after the end of the previous layer.
///
/// \note Setting a stride affects any subsequent calls to
/// nvnCommandBufferCopyBufferToTexture and
/// nvnCommandBufferCopyTextureToBuffer using this command buffer.  Unlike
/// most NVNcommandBuffer calls, this command does not store the stride in
/// command or control memory and does not affect future submissions of
/// previously recorded copy commands or copy commands recorded using other
/// command buffers.
///
/// \param cmdBuf NVNcommandBuffer object used to store image stride.
///
/// \param stride Stride (in bytes) between 2D arrays of texels in buffer
/// memory.
NVNAPI void NVNAPIENTRY
    nvnCommandBufferSetCopyImageStride(NVNcommandBuffer *cmdBuf,
                                       ptrdiff_t stride);

/// \brief Get the current copy row stride stored in a NVNcommandBuffer
/// object.
///
/// \param cmdBuf NVNcommandBuffer object to query.
NVNAPI ptrdiff_t NVNAPIENTRY
    nvnCommandBufferGetCopyRowStride(const NVNcommandBuffer *cmdBuf);

/// \brief Get the current copy image stride stored in a NVNcommandBuffer
/// object.
///
/// \param cmdBuf NVNcommandBuffer object to query.
NVNAPI ptrdiff_t NVNAPIENTRY
    nvnCommandBufferGetCopyImageStride(const NVNcommandBuffer *cmdBuf);

/// \brief Draws an on-screen rectangle displaying the contents of a
/// provided texture.
///
/// This command renders an axis-aligned rectangle into color target zero
/// of the framebuffer, displaying some or all of the contents of a
/// two-dimensional texture or rectangle texture.  The caller specifies a
/// texture handle identifying the source texture, a destination region in
/// window coordinates, and a source region in non-normalized texture
/// coordinates.  For each fragment produced by the rectangle, DrawTexture
/// computes coordinates in the source region, performs a texture lookup,
/// and uses the texture result as the fragment color.  Most per-fragment
/// operations (including blending) are performed, and the results are
/// written into color target zero.
///
/// DrawTexture has a number of limitations.  Using illegal state may
/// result in undefined behavior or GPU errors:
/// - The source texture must be two-dimensional (or rectangle).
/// - Level of detail selection is not supported; level zero of the
/// original texture is used.
/// - Calling DrawTexture with multiple color targets is illegal.
/// - Calling DrawTexture with blend modes selecting a second source color
/// is illegal.
/// - Conservative rasterization is treated as disabled.
/// - Depth, depth bounds, and stencil tests are treated as disabled.
/// - Anisotropic filtering and wrap modes that mirror or repeat are
/// unsupported.
///
/// \param cmdBuf NVNcommandBuffer used to perform the operation.
///
/// \param texture Handle for the pair of NVNtexture and NVNsampler objects
/// to use.
///
/// \param dstRegion Region identifying the portion of the color target to
/// update.
/// Coordinates (x0,y0) and (x1,y1) identify opposite corners of the
/// region, in screen coordinates.  All coordinates must be in the range
/// [-65536, +65535], and x1-x0 and y1-y0 must be in the range [0, 65535].
///
/// \param srcRegion Region identifying the portion of _texture_ to access.
/// Coordinates (x0,y0) and (x1,y1) identify opposite corners of the
/// region, in non-normalized texel coordinates.  All coordinates must be
/// in the range [-65536, +65535].  The texture region will be mirrored
/// horizontally and/or vertically if x0 > x1 or y0 > y1, respectively.
NVNAPI void NVNAPIENTRY
    nvnCommandBufferDrawTexture(NVNcommandBuffer *cmdBuf,
                                NVNtextureHandle texture,
                                const NVNdrawTextureRegion *dstRegion,
                                const NVNdrawTextureRegion *srcRegion);

/// \brief Sets the program's subroutine linkage maps for future use with
/// nvnCommandBufferSetProgramSubroutines.
///
/// When using shader subroutines, applications update the values of
/// subroutine uniforms using nvnCommandBufferSetProgramSubroutines. Such
/// updates require the driver to adjust function pointers and related data
/// to reflect the new uniform values. This linkage mapping must be set
/// before submitting any commands from a
/// nvnCommandBufferSetProgramSubroutines call. For each shader stage with
/// subroutines, the compiler provides a _linkage map_ that applications
/// must provide to the driver to support subroutine updates.  The linkage
/// maps for a program must be set before binding the program or attempting
/// to update subroutine uniforms. The linkage maps must be exactly what
/// the compiler outputs for the input program in order for correctness. If
/// the driver detects any inconsistencies between the input linkage maps
/// and what the program requires, such as not providing linkage maps for
/// all the stages of the program that use subroutines, NVN_FALSE is
/// returned. Additionally, the data may be modified internally by the
/// driver, so the data backed by this pointer must remain live for the
/// lifetime of the program after calling SetSubroutineLinkage, and must
/// not be modified by the application. The driver might also write to the
/// contents of the linkage map during run-time.
///
/// \param program NVNprogram object to set up.
///
/// \param count Number of linkage maps to use, one for each shader stage
/// that uses subroutines.
///
/// \param linkageMapPtrs An array of subroutine linkage map pointers where
/// each subroutine linkage map was produced by the offline compiler.
NVNAPI NVNboolean NVNAPIENTRY
    nvnProgramSetSubroutineLinkage(NVNprogram *program,
                                   int count,
                                   const NVNsubroutineLinkageMapPtr *linkageMapPtrs);

/// \brief Sets the subroutine uniform parameters
///
/// Sets subroutine uniform parameters for program subroutine, similar to
/// the ARB_shader_subroutine functionality in OpenGL.  Subroutines allow a
/// application to dynamically switch between using different sets of
/// functions instead without having to recompile the program, similar to
/// setting function pointers in C-based languages.  In order to use this
/// feature, some linkage information from the offline compiler needs to be
/// set in the program first via a call to SetSubroutineLinkage.  This
/// function allows a program to set a range of uniform locations to point
/// to the input subroutines denoted by the index values in the array
/// _values_.  See the offline compiler documentation and the NVN
/// documentation for more information on how to use this feature.
///
/// \param cmdBuf NVNcommandBuffer used to perform the operation.
///
/// \param program NVNprogram object to perform the operation on.
///
/// \param stage Shader stage to update.
///
/// \param first First subroutine uniform location in the program to
/// update, and corresponds to the first array entry in the _values_ input.
///
/// \param count Number of subroutine uniform locations to update, starting
/// with uniform with location _first_, and corresponds to the number of
/// entries in the input array _values_.
///
/// \param values An array of _count_ subroutine index values to be
/// assigned to successive subroutine uniform locations, where the first
/// entry in the array corresponds to subroutine uniform location _first_.
NVNAPI void NVNAPIENTRY
    nvnCommandBufferSetProgramSubroutines(NVNcommandBuffer *cmdBuf,
                                          NVNprogram *program,
                                          NVNshaderStage stage,
                                          const int first,
                                          const int count,
                                          const int *values);

/// \brief Binds or unbinds a coverage modulation table used for
/// target-independent rasterization.
///
/// By default, the color modulation feature of target-independent
/// rasterization scales color using a factor based on the fraction of
/// covered samples.  For example, if three out of four samples for a
/// fragment are covered, the default modulation factor will be 0.75.  This
/// function allows applications to override this behavior by specifying a
/// 16-element table of modulation factors.  If the modulation table is
/// used with a render target with N raster samples per color sample and M
/// samples are covered in a fragment, the modulation factor will be
/// _entries_[(M-1) * 16 / N].  If no modulation table is bound (i.e.,
/// _entries_ is NULL), the modulation factor will be M / N.
///
/// By default, no modulation table is bound.
///
/// \param cmdBuf NVNcommandBuffer used to perform the operation.
///
/// \param entries Table of color modulation factors.
/// If NULL, color modulation will use the fraction of covered raster
/// samples as the color modulation factor.
NVNAPI void NVNAPIENTRY
    nvnCommandBufferBindCoverageModulationTable(NVNcommandBuffer *cmdBuf,
                                                const float *entries);

/// \brief Resolve depth values in a compressible depth render target.
///
/// Compressible depth render targets may store depth values in a
/// compressed form, where individual values are reconstructed using
/// compressed depth data and the current set of sample locations.  This
/// command decompresses the depth buffer, resolving individual depth
/// values based on the current sample locations.  This command should be
/// used in the unlikely event that an application needs to change sample
/// locations for a render target but use depth values that reflect the old
/// locations.  It is not necessary to decompress depth render targets for
/// any other reason, including to use them for texture mapping.  If there
/// is no current depth render target, this command will have no effect.
///
/// \param cmdBuf NVNcommandBuffer used to perform the operation.
NVNAPI void NVNAPIENTRY
    nvnCommandBufferResolveDepthBuffer(NVNcommandBuffer *cmdBuf);

/// \brief Enable or disable color reduction.
///
/// \param cmdBuf Command buffer used to perform the enable or disable
/// operation.
///
/// \param enable If NVN_TRUE, enable color reduction. Otherwise disable.
NVNAPI void NVNAPIENTRY
    nvnCommandBufferSetColorReductionEnable(NVNcommandBuffer *cmdBuf,
                                            NVNboolean enable);

/// \brief Specify new color reduction thresholds for a format class.
///
/// \param cmdBuf Command buffer used to perform the Set operation.
///
/// \param formatClass #NVNformat class for which to set thresholds
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
NVNAPI void NVNAPIENTRY
    nvnCommandBufferSetColorReductionThresholds(NVNcommandBuffer *cmdBuf,
                                                NVNformatClass formatClass,
                                                int thresholdConservative,
                                                int thresholdAggressive);

/// \brief Pushes a debug group into the command stream.
///
/// Debug groups provide a method for annotating a command stream with
/// discrete groups of commands using a descriptive name. New debug groups
/// are pushed to the top of the debug group stack. Debug groups are
/// strictly hierarchical and their sequences may be nested within other
/// debug groups but can not overlap. These groups may then be used by the
/// debug layer, an external debugger, or a profiler tool.
///
/// This entry point is for use with debug groups whose description is
/// comprised of a static string.
///
/// \param cmdBuf NVNcommandBuffer used to push the new debug group.
///
/// \param domainId Debug domain ID.
///
/// \param description Text string used to annotate the debug group.
NVNAPI void NVNAPIENTRY
    nvnCommandBufferPushDebugGroupStatic(NVNcommandBuffer *cmdBuf,
                                         uint32_t domainId,
                                         const char * description);

/// \brief Pushes a debug group into the command stream.
///
/// Debug groups provide a method for annotating a command stream with
/// discrete groups of commands using a descriptive name. New debug groups
/// are pushed to the top of the debug group stack. Debug groups are
/// strictly hierarchical and their sequences may be nested within other
/// debug groups but can not overlap. These groups may then be used by the
/// debug layer, an external debugger, or a profiler tool.
///
/// This entry point is for use with debug groups whose description is
/// comprised of a dynamic string.
///
/// \param cmdBuf NVNcommandBuffer used to push the new debug group.
///
/// \param domainId Debug domain ID.
///
/// \param description Text string used to annotate the debug group.
NVNAPI void NVNAPIENTRY
    nvnCommandBufferPushDebugGroupDynamic(NVNcommandBuffer *cmdBuf,
                                          uint32_t domainId,
                                          const char * description);

/// \brief Pushes a debug group into the command stream.
///
/// Debug groups provide a method for annotating a command stream with
/// discrete groups of commands using a descriptive name. New debug groups
/// are pushed to the top of the debug group stack. Debug groups are
/// strictly hierarchical and their sequences may be nested within other
/// debug groups but can not overlap. These groups may then be used by the
/// debug layer, an external debugger, or a profiler tool.
///
/// This entry point is equivalent to calling
/// nvnCommandBufferPushDebugGroupStatic with a debug domain ID of 0.
///
/// \param cmdBuf NVNcommandBuffer used to push the new debug group.
///
/// \param description Text string used to annotate the debug group.
NVNAPI void NVNAPIENTRY
    nvnCommandBufferPushDebugGroup(NVNcommandBuffer *cmdBuf,
                                   const char * description);

/// \brief Pops the active debug group off the command stream.
///
/// This entry point is equivalent to calling
/// nvnCommandBufferPopDebugGroupId with a debug domain ID of 0.
///
/// \param cmdBuf NVNcommandBuffer used to pop the debug group.
NVNAPI void NVNAPIENTRY
    nvnCommandBufferPopDebugGroup(NVNcommandBuffer *cmdBuf);

/// \brief Pops the active debug group off the command stream.
///
/// \param cmdBuf NVNcommandBuffer used to pop the debug group.
///
/// \param domainId Debug domain ID.
NVNAPI void NVNAPIENTRY
    nvnCommandBufferPopDebugGroupId(NVNcommandBuffer *cmdBuf,
                                    uint32_t domainId);

/// \brief Inserts a debug event marker to the command stream.
///
/// Inserts a debug marker to annotate a command stream with a descriptive
/// text marker. These event markers may then be used by the debug layer,
/// an external debugger, or a profiler tool.
///
/// This entry point is for use with debug markers whose description is
/// comprised of a static string.
///
/// \param cmdBuf NVNcommandBuffer used to add the event marker.
///
/// \param domainId Debug domain ID.
///
/// \param description Text string used to annotate the event marker.
NVNAPI void NVNAPIENTRY
    nvnCommandBufferInsertDebugMarkerStatic(NVNcommandBuffer *cmdBuf,
                                            uint32_t domainId,
                                            const char * description);

/// \brief Inserts a debug event marker to the command stream.
///
/// Inserts a debug marker to annotate a command stream with a descriptive
/// text marker. These event markers may then be used by the debug layer,
/// an external debugger, or a profiler tool.
///
/// This entry point can be used with dynamically allocated strings, as it
/// copies the data onto the command buffer itself.
///
/// \param cmdBuf NVNcommandBuffer used to add the event marker.
///
/// \param domainId Debug domain ID.
///
/// \param description Text string used to annotate the event marker.
NVNAPI void NVNAPIENTRY
    nvnCommandBufferInsertDebugMarkerDynamic(NVNcommandBuffer *cmdBuf,
                                             uint32_t domainId,
                                             const char * description);

/// \brief Inserts a debug event marker to the command stream.
///
/// Inserts a debug marker to annotate a command stream with a descriptive
/// text marker. These event markers may then be used by the debug layer,
/// an external debugger, or a profiler tool.
///
/// This entry point is identical to
/// nvnCommandBufferInsertDebugMarkerStatic with a domainId of 0.
///
/// \param cmdBuf NVNcommandBuffer used to add the event marker.
///
/// \param description Text string used to annotate the event marker.
NVNAPI void NVNAPIENTRY
    nvnCommandBufferInsertDebugMarker(NVNcommandBuffer *cmdBuf,
                                      const char * description);

/// \brief Query a callback function for a NVNcommandBuffer object that
/// will be called if it runs out of memory.
///
/// \param cmdBuf NVNcommandBuffer object to query.
NVNAPI PFNNVNCOMMANDBUFFERMEMORYCALLBACKPROC NVNAPIENTRY
    nvnCommandBufferGetMemoryCallback(const NVNcommandBuffer *cmdBuf);

/// \brief Query a generic pointer that will be passed to a callback
/// function if the command buffer runs out of memory.
///
/// \param cmdBuf NVNcommandBuffer object to update.
NVNAPI void* NVNAPIENTRY
    nvnCommandBufferGetMemoryCallbackData(const NVNcommandBuffer *cmdBuf);

/// \brief Query whether the command buffer is currently recording.
///
/// \param cmdBuf NVNcommandBuffer object to query.
NVNAPI NVNboolean NVNAPIENTRY
    nvnCommandBufferIsRecording(const NVNcommandBuffer *cmdBuf);

/// \brief Initialize a NVNsync object for the specified device.
///
/// \param sync NVNsync object to initialize.
///
/// \param device NVNdevice owning the NVNsync object.
NVNAPI NVNboolean NVNAPIENTRY
    nvnSyncInitialize(NVNsync *sync,
                      NVNdevice *device);

/// \brief Finalize a NVNsync object.
///
/// \param sync NVNsync object to finalize.
NVNAPI void NVNAPIENTRY
    nvnSyncFinalize(NVNsync *sync);

/// \brief Sets the debug label string for a NVNsync object.
///
/// Annotates a NVNsync object with a descriptive text label. This label
/// may then be used by the debug layer, an external debugger, or a
/// profiler tool.
///
/// \param sync NVNsync object to set debug label.
///
/// \param label Text string used to annotate the NVNsync object.
NVNAPI void NVNAPIENTRY
    nvnSyncSetDebugLabel(NVNsync *sync,
                         const char * label);

/// \brief Mark a NVNsync object as signaled when all previous commands
/// have completed in the NVNqueue object.
///
/// \warning FenceSync commands are not automatically flushed for
/// processing by the GPU.  Before waiting on a FenceSync call with the CPU
/// using nvnSyncWait or from another NVNqueue using nvnQueueWaitSync,
/// applications must ensure that the previous FenceSync call was flushed
/// using nvnQueueFlush.  Waiting on a FenceSync call using
/// nvnQueueWaitSync in the same queue requires no manual flush.
///
/// \note If tiled caching is enabled, this command will trigger an
/// implicit tiled cache flush.
///
/// \param queue NVNqueue object with commands to wait on before signaling
/// completion.
///
/// \param sync NVNsync object to mark as signaled when commands complete.
///
/// \param condition Type of command completion to wait for.
///
/// \param flags Bitfield of additional operations to perform before
/// signaling. See #NVNsyncFlagBits.
NVNAPI void NVNAPIENTRY
    nvnQueueFenceSync(NVNqueue *queue,
                      NVNsync *sync,
                      NVNsyncCondition condition,
                      int flags);

/// \brief Wait for a NVNsync object to be signaled on the CPU before
/// returning.
///
/// The value returned indicates the status of the NVNsync object when the
/// command returns.
///
/// \param sync NVNsync object to wait on.
///
/// \param timeoutNs Minimum time (in nanoseconds) to wait for the NVNsync
/// object to be signaled.
NVNAPI NVNsyncWaitResult NVNAPIENTRY
    nvnSyncWait(const NVNsync *sync,
                uint64_t timeoutNs);

/// \brief Wait for a NVNsync object to be signaled on the GPU before
/// processing any further commands in the specified NVNqueue object.
///
/// \param queue NVNqueue whose subsequent commands wait on the NVNsync
/// object.
///
/// \param sync NVNsync object to wait on.
NVNAPI NVNboolean NVNAPIENTRY
    nvnQueueWaitSync(NVNqueue *queue,
                     const NVNsync *sync);

/// \brief Use a fenced and flushed GLsync object to initialize a NVN
/// NVNsync object.
///
/// Use this function instead of Initialize for NVN NVNsync objects created
/// from GLsync objects. This function initializes a NVNsync object that
/// signals after a corresponding GLsync object. This enables OpenGL to NVN
/// synchronization (fencing is done on the OpenGL object and waiting on
/// its NVN counterpart). The value returned indicates the status of the
/// NVNsync object. See nvnSyncCreateGLSync.
///
/// \param sync The NVNsync object to initialize.
///
/// \param device NVNdevice owning the NVNsync object.
///
/// \param glSync The name of the OpenGL or OpenGL ES sync to use as a
/// source.
/// \li The GLsync object must be a valid OpenGL or OpenGL ES GLsync
/// object.
/// \li The GLsync object must have already been flushed in the GL command
/// stream.
/// \li It needs to be valid for the lifetime of the NVNsync object.
///
/// \param[out] status Detailed error status.
NVNAPI NVNboolean NVNAPIENTRY
    nvnSyncInitializeFromFencedGLSync(NVNsync *sync,
                                      NVNdevice *device,
                                      const void *glSync,
                                      NVNsyncInteropStatus *status);

/// \brief Use a fenced and flushed NVN NVNsync object to create a GLsync.
///
/// This function initializes a GLsync object that signals after a
/// corresponding NVN sync object. This enables NVN to OpenGL
/// synchronization (fencing is done on the NVN NVNsync object and waiting
/// on its GLsync counterpart).
/// The value returned indicates the status of the NVNsync object. See
/// nvnSyncInitializeFromFencedGLSync.
///
/// \param sync The NVN NVNsync object to use as a source.
/// \li Must be a valid NVN NVNsync object.
/// \li Must be fenced and flushed before calling this.
/// \li Needs to be valid for the lifetime of the GLsync object, i.e. the
/// source NVNsync object must not be finalized while there is any GL work
/// pending on the GLsync object. After the source NVN NVNsync object is
/// finalized, the GLsync object should not be used in any API function,
/// and its state is undefined.
///
/// \param[out] glSync The GLsync to initialize, cast to uint64_t*.
NVNAPI NVNsyncInteropStatus NVNAPIENTRY
    nvnSyncCreateGLSync(const NVNsync *sync,
                        uint64_t *glSync);

/// \brief Set default state for the NVNeventBuilder object.
///
/// \param builder NVNeventBuilder object to reset.
NVNAPI void NVNAPIENTRY
    nvnEventBuilderSetDefaults(NVNeventBuilder *builder);

/// \brief Set the location in a NVNmemoryPool used to hold the state of an
/// NVNevent created from the NVNeventBuilder.
///
/// Each NVNevent object created by an event builder has a 32-bit unsigned
/// integer value stored in GPU-accessible memory in a memory pool.  API
/// commands are provided to read and write to this value and to wait for
/// it to reach a specified value.  When an NVNevent object is initialized,
/// its initial value comes from the current contents of the associated
/// storage.
///
/// \param builder NVNeventBuilder object to modify.
///
/// \param pool Memory pool used to hold the state of the event.
/// If the event is signaled or queried by the CPU, a CPU_UNCACHED memory
/// pool is recommended.  If a CPU_CACHED memory pool is used, the event
/// object's memory must be flushed from the CPU data cache using
/// nvnMemoryPoolFlushMappedRange after calling nvnEventSignal and must be
/// invalidated from the CPU data cache using
/// nvnMemoryPoolInvalidateMappedRange before calling nvnEventGetValue.
/// Memory pools created with the VIRTUAL, PHYSICAL, and GPU_NO_ACCESS
/// flags are not supported. Memory pools created with GPU_CACHED are only
/// supported if the pool also uses CPU_NO_ACCESS. Memory pools created
/// with CPU_CACHED are not supported on the Windows reference
/// implementation.
///
/// \param offset offset in _pool_ for event write back location. _offset_
/// must be aligned to the size of an event which is a 32-bit unsigned
/// integer.
NVNAPI void NVNAPIENTRY
    nvnEventBuilderSetStorage(NVNeventBuilder *builder,
                              const NVNmemoryPool *pool,
                              int64_t offset);

/// \brief Get the memory pool and the offset associated with this
/// NVNeventBuilder object.
///
/// \param builder NVNeventBuilder object to query.
///
/// \param[out] offset Offset into the memory pool.
NVNAPI const NVNmemoryPool * NVNAPIENTRY
    nvnEventBuilderGetStorage(const NVNeventBuilder *builder,
                              int64_t *offset);

/// \brief Initialize an NVNevent object.
///
/// \param event NVNevent object to initialize.
///
/// \param builder NVNeventBuilder object to use.
NVNAPI NVNboolean NVNAPIENTRY
    nvnEventInitialize(NVNevent *event,
                       const NVNeventBuilder *builder);

/// \brief Finalize a NVNevent object.
///
/// \param event NVNevent object to finalize.
NVNAPI void NVNAPIENTRY
    nvnEventFinalize(NVNevent *event);

/// \brief Get the value associated with an NVNevent object.
///
/// \param event NVNevent object to query.
NVNAPI uint32_t NVNAPIENTRY
    nvnEventGetValue(const NVNevent *event);

/// \brief Use the CPU to signal an NVNevent object.
///
/// \param event NVNevent object to initialize.
///
/// \param mode NVNevent signal mode.
///
/// \param value Signal value.
NVNAPI void NVNAPIENTRY
    nvnEventSignal(NVNevent *event,
                   NVNeventSignalMode mode,
                   uint32_t value);

/// \brief Pause work on the GPU until an event is signaled.
///
/// When applications use this command, they are required to ensure that
/// the memory associated with the event will eventually contain a value
/// that passes the test specified by _waitMode_. If not, a GPU timeout
/// error will occur.
///
/// \warning On the Windows reference implementation, NVNevent objects do
/// not support synchronization between queues.  Submitting a command set
/// containing a WaitEvent command on one queue to wait on an event
/// signaled by a nvnCommandBufferSignalEvent command submitted to another
/// queue may result in GPU timeout errors.  Applications can use NVNsync
/// objects to reliably synchronize between queues on all platforms.
///
/// \param cmdBuf NVNcommandBuffer object to use.
///
/// \param event NVNevent object to wait on.
///
/// \param waitMode NVNevent wait mode.
/// The wait mode specifies a test use to compare the value in memory
/// associated with the NVNevent object and the _value_ provided in this
/// command.  The event is considered to be signaled when this test passes.
///
/// \param value Value to compare to the value in the NVNevent object's
/// memory.
NVNAPI void NVNAPIENTRY
    nvnCommandBufferWaitEvent(NVNcommandBuffer *cmdBuf,
                              const NVNevent *event,
                              NVNeventWaitMode waitMode,
                              uint32_t value);

/// \brief Use the GPU to signal an NVNevent object.
///
/// \param cmdBuf NVNcommandBuffer object to use.
///
/// \param event NVNevent object to signal.
///
/// \param mode Method used to update the memory associated with the
/// NVNevent object.
///
/// \param location Location in the GPU pipeline that the command must
/// reach before the signal operation is performed.
///
/// \param flags Flags used to control the signal operation. See
/// #NVNeventSignalFlags.
///
/// \param value Value used to update the memory associated with the
/// NVNevent object.
NVNAPI void NVNAPIENTRY
    nvnCommandBufferSignalEvent(NVNcommandBuffer *cmdBuf,
                                const NVNevent *event,
                                NVNeventSignalMode mode,
                                NVNeventSignalLocation location,
                                int flags,
                                uint32_t value);

/// \brief Sets the criteria that is used by the ZCull HW for stencil
/// culling.
///
/// This function sets the criteria that is used by the ZCull HW for
/// stencil culling. ZCull will only try to discard pixels/fragments if the
/// cull criteria matches the current stencil state and if the depth
/// texture of the current render target was created using the
/// ZCULL_SUPPORT_STENCIL flag.
/// Note that calling this function will make the current ZCull region
/// invalid until the next clear. This might have a negative impact on the
/// depth culling performance.
///
/// \param cmdBuf NVNcommandBuffer object to use.
///
/// \param func Stencil function to be used for stencil culling.
///
/// \param ref Specify a stencil reference value used for stencil culling.
///
/// \param mask Mask used for stencil culling.
NVNAPI void NVNAPIENTRY
    nvnCommandBufferSetStencilCullCriteria(NVNcommandBuffer *cmdBuf,
                                           NVNstencilFunc func,
                                           int ref,
                                           int mask);

/// \}

#endif /* NVN_PROTOTYPES */

/// \addtogroup nvn_c_entryfuncptrs
/// \{

/// \brief Function pointer type to call nvnDeviceBuilderSetDefaults().
typedef void (NVNAPIENTRYP PFNNVNDEVICEBUILDERSETDEFAULTSPROC) (NVNdeviceBuilder *builder);

/// \brief Function pointer type to call nvnDeviceBuilderSetFlags().
typedef void (NVNAPIENTRYP PFNNVNDEVICEBUILDERSETFLAGSPROC) (NVNdeviceBuilder *builder, int flags);

/// \brief Function pointer type to call nvnDeviceBuilderGetFlags().
typedef NVNdeviceFlagBits (NVNAPIENTRYP PFNNVNDEVICEBUILDERGETFLAGSPROC) (const NVNdeviceBuilder *builder);

/// \brief Function pointer type to call nvnDeviceInitialize().
typedef NVNboolean (NVNAPIENTRYP PFNNVNDEVICEINITIALIZEPROC) (NVNdevice *device, const NVNdeviceBuilder *builder);

/// \brief Function pointer type to call nvnDeviceFinalize().
typedef void (NVNAPIENTRYP PFNNVNDEVICEFINALIZEPROC) (NVNdevice *device);

/// \brief Function pointer type to call nvnDeviceSetDebugLabel().
typedef void (NVNAPIENTRYP PFNNVNDEVICESETDEBUGLABELPROC) (NVNdevice *device, const char * label);

/// \brief Function pointer type to call nvnDeviceGetProcAddress().
typedef PFNNVNGENERICFUNCPTRPROC (NVNAPIENTRYP PFNNVNDEVICEGETPROCADDRESSPROC) (const NVNdevice *device, const char * name);

/// \brief Function pointer type to call nvnDeviceGetInteger().
typedef void (NVNAPIENTRYP PFNNVNDEVICEGETINTEGERPROC) (const NVNdevice *device, NVNdeviceInfo pname, int *v);

/// \brief Function pointer type to call nvnDeviceGetCurrentTimestampInNanoseconds().
typedef uint64_t (NVNAPIENTRYP PFNNVNDEVICEGETCURRENTTIMESTAMPINNANOSECONDSPROC) (const NVNdevice *device);

/// \brief Function pointer type to call nvnDeviceSetIntermediateShaderCache().
typedef void (NVNAPIENTRYP PFNNVNDEVICESETINTERMEDIATESHADERCACHEPROC) (NVNdevice *device, int numMaxEntries);

/// \brief Function pointer type to call nvnDeviceGetTextureHandle().
typedef NVNtextureHandle (NVNAPIENTRYP PFNNVNDEVICEGETTEXTUREHANDLEPROC) (const NVNdevice *device, int textureID, int samplerID);

/// \brief Function pointer type to call nvnDeviceGetTexelFetchHandle().
typedef NVNtextureHandle (NVNAPIENTRYP PFNNVNDEVICEGETTEXELFETCHHANDLEPROC) (const NVNdevice *device, int textureID);

/// \brief Function pointer type to call nvnDeviceGetImageHandle().
typedef NVNimageHandle (NVNAPIENTRYP PFNNVNDEVICEGETIMAGEHANDLEPROC) (const NVNdevice *device, int imageID);

/// \brief Function pointer type to call nvnDeviceInstallDebugCallback().
typedef void (NVNAPIENTRYP PFNNVNDEVICEINSTALLDEBUGCALLBACKPROC) (NVNdevice *device, const PFNNVNDEBUGCALLBACKPROC callback, void *callbackData, NVNboolean enable);

/// \brief Function pointer type to call nvnDeviceGenerateDebugDomainId().
typedef NVNdebugDomainId (NVNAPIENTRYP PFNNVNDEVICEGENERATEDEBUGDOMAINIDPROC) (const NVNdevice *device, const char * name);

/// \brief Function pointer type to call nvnDeviceSetWindowOriginMode().
typedef void (NVNAPIENTRYP PFNNVNDEVICESETWINDOWORIGINMODEPROC) (NVNdevice *device, NVNwindowOriginMode windowOriginMode);

/// \brief Function pointer type to call nvnDeviceSetDepthMode().
typedef void (NVNAPIENTRYP PFNNVNDEVICESETDEPTHMODEPROC) (NVNdevice *device, NVNdepthMode depthMode);

/// \brief Function pointer type to call nvnDeviceRegisterFastClearColor().
typedef NVNboolean (NVNAPIENTRYP PFNNVNDEVICEREGISTERFASTCLEARCOLORPROC) (NVNdevice *device, const float *color, NVNformat format);

/// \brief Function pointer type to call nvnDeviceRegisterFastClearColori().
typedef NVNboolean (NVNAPIENTRYP PFNNVNDEVICEREGISTERFASTCLEARCOLORIPROC) (NVNdevice *device, const int *color, NVNformat format);

/// \brief Function pointer type to call nvnDeviceRegisterFastClearColorui().
typedef NVNboolean (NVNAPIENTRYP PFNNVNDEVICEREGISTERFASTCLEARCOLORUIPROC) (NVNdevice *device, const uint32_t *color, NVNformat format);

/// \brief Function pointer type to call nvnDeviceRegisterFastClearDepth().
typedef NVNboolean (NVNAPIENTRYP PFNNVNDEVICEREGISTERFASTCLEARDEPTHPROC) (NVNdevice *device, float depth);

/// \brief Function pointer type to call nvnDeviceGetWindowOriginMode().
typedef NVNwindowOriginMode (NVNAPIENTRYP PFNNVNDEVICEGETWINDOWORIGINMODEPROC) (const NVNdevice *device);

/// \brief Function pointer type to call nvnDeviceGetDepthMode().
typedef NVNdepthMode (NVNAPIENTRYP PFNNVNDEVICEGETDEPTHMODEPROC) (const NVNdevice *device);

/// \brief Function pointer type to call nvnDeviceGetTimestampInNanoseconds().
typedef uint64_t (NVNAPIENTRYP PFNNVNDEVICEGETTIMESTAMPINNANOSECONDSPROC) (const NVNdevice *device, const NVNcounterData *counterData);

/// \brief Function pointer type to call nvnDeviceApplyDeferredFinalizes().
typedef void (NVNAPIENTRYP PFNNVNDEVICEAPPLYDEFERREDFINALIZESPROC) (NVNdevice *device, int age);

/// \brief Function pointer type to call nvnDeviceFinalizeCommandHandle().
typedef void (NVNAPIENTRYP PFNNVNDEVICEFINALIZECOMMANDHANDLEPROC) (NVNdevice *device, NVNcommandHandle handles);

/// \brief Function pointer type to call nvnDeviceWalkDebugDatabase().
typedef void (NVNAPIENTRYP PFNNVNDEVICEWALKDEBUGDATABASEPROC) (const NVNdevice *device, NVNdebugObjectType type, PFNNVNWALKDEBUGDATABASECALLBACKPROC callback, void *userParam);

/// \brief Function pointer type to call nvnDeviceGetSeparateTextureHandle().
typedef NVNseparateTextureHandle (NVNAPIENTRYP PFNNVNDEVICEGETSEPARATETEXTUREHANDLEPROC) (const NVNdevice *device, int textureID);

/// \brief Function pointer type to call nvnDeviceGetSeparateSamplerHandle().
typedef NVNseparateSamplerHandle (NVNAPIENTRYP PFNNVNDEVICEGETSEPARATESAMPLERHANDLEPROC) (const NVNdevice *device, int samplerID);

/// \brief Function pointer type to call nvnDeviceIsExternalDebuggerAttached().
typedef NVNboolean (NVNAPIENTRYP PFNNVNDEVICEISEXTERNALDEBUGGERATTACHEDPROC) (const NVNdevice *device);

/// \brief Function pointer type to call nvnQueueGetError().
typedef NVNqueueGetErrorResult (NVNAPIENTRYP PFNNVNQUEUEGETERRORPROC) (NVNqueue *queue, NVNqueueErrorInfo *errorInfo);

/// \brief Function pointer type to call nvnQueueGetTotalCommandMemoryUsed().
typedef size_t (NVNAPIENTRYP PFNNVNQUEUEGETTOTALCOMMANDMEMORYUSEDPROC) (NVNqueue *queue);

/// \brief Function pointer type to call nvnQueueGetTotalControlMemoryUsed().
typedef size_t (NVNAPIENTRYP PFNNVNQUEUEGETTOTALCONTROLMEMORYUSEDPROC) (NVNqueue *queue);

/// \brief Function pointer type to call nvnQueueGetTotalComputeMemoryUsed().
typedef size_t (NVNAPIENTRYP PFNNVNQUEUEGETTOTALCOMPUTEMEMORYUSEDPROC) (NVNqueue *queue);

/// \brief Function pointer type to call nvnQueueResetMemoryUsageCounts().
typedef void (NVNAPIENTRYP PFNNVNQUEUERESETMEMORYUSAGECOUNTSPROC) (NVNqueue *queue);

/// \brief Function pointer type to call nvnQueueBuilderSetDevice().
typedef void (NVNAPIENTRYP PFNNVNQUEUEBUILDERSETDEVICEPROC) (NVNqueueBuilder *builder, NVNdevice *device);

/// \brief Function pointer type to call nvnQueueBuilderSetDefaults().
typedef void (NVNAPIENTRYP PFNNVNQUEUEBUILDERSETDEFAULTSPROC) (NVNqueueBuilder *builder);

/// \brief Function pointer type to call nvnQueueBuilderSetFlags().
typedef void (NVNAPIENTRYP PFNNVNQUEUEBUILDERSETFLAGSPROC) (NVNqueueBuilder *builder, int flags);

/// \brief Function pointer type to call nvnQueueBuilderSetCommandMemorySize().
typedef void (NVNAPIENTRYP PFNNVNQUEUEBUILDERSETCOMMANDMEMORYSIZEPROC) (NVNqueueBuilder *builder, size_t size);

/// \brief Function pointer type to call nvnQueueBuilderSetComputeMemorySize().
typedef void (NVNAPIENTRYP PFNNVNQUEUEBUILDERSETCOMPUTEMEMORYSIZEPROC) (NVNqueueBuilder *builder, size_t size);

/// \brief Function pointer type to call nvnQueueBuilderSetControlMemorySize().
typedef void (NVNAPIENTRYP PFNNVNQUEUEBUILDERSETCONTROLMEMORYSIZEPROC) (NVNqueueBuilder *builder, size_t size);

/// \brief Function pointer type to call nvnQueueBuilderGetQueueMemorySize().
typedef size_t (NVNAPIENTRYP PFNNVNQUEUEBUILDERGETQUEUEMEMORYSIZEPROC) (const NVNqueueBuilder *builder);

/// \brief Function pointer type to call nvnQueueBuilderSetQueueMemory().
typedef void (NVNAPIENTRYP PFNNVNQUEUEBUILDERSETQUEUEMEMORYPROC) (NVNqueueBuilder *builder, void *memory, size_t size);

/// \brief Function pointer type to call nvnQueueBuilderSetCommandFlushThreshold().
typedef void (NVNAPIENTRYP PFNNVNQUEUEBUILDERSETCOMMANDFLUSHTHRESHOLDPROC) (NVNqueueBuilder *builder, size_t threshold);

/// \brief Function pointer type to call nvnQueueBuilderSetQueuePriority().
typedef void (NVNAPIENTRYP PFNNVNQUEUEBUILDERSETQUEUEPRIORITYPROC) (NVNqueueBuilder *builder, const NVNqueuePriority priority);

/// \brief Function pointer type to call nvnQueueBuilderGetQueuePriority().
typedef NVNqueuePriority (NVNAPIENTRYP PFNNVNQUEUEBUILDERGETQUEUEPRIORITYPROC) (const NVNqueueBuilder *builder);

/// \brief Function pointer type to call nvnQueueBuilderGetDevice().
typedef const NVNdevice * (NVNAPIENTRYP PFNNVNQUEUEBUILDERGETDEVICEPROC) (const NVNqueueBuilder *builder);

/// \brief Function pointer type to call nvnQueueBuilderGetFlags().
typedef int (NVNAPIENTRYP PFNNVNQUEUEBUILDERGETFLAGSPROC) (const NVNqueueBuilder *builder);

/// \brief Function pointer type to call nvnQueueBuilderGetCommandMemorySize().
typedef NVNboolean (NVNAPIENTRYP PFNNVNQUEUEBUILDERGETCOMMANDMEMORYSIZEPROC) (const NVNqueueBuilder *builder, int *size);

/// \brief Function pointer type to call nvnQueueBuilderGetComputeMemorySize().
typedef NVNboolean (NVNAPIENTRYP PFNNVNQUEUEBUILDERGETCOMPUTEMEMORYSIZEPROC) (const NVNqueueBuilder *builder, int *size);

/// \brief Function pointer type to call nvnQueueBuilderGetControlMemorySize().
typedef NVNboolean (NVNAPIENTRYP PFNNVNQUEUEBUILDERGETCONTROLMEMORYSIZEPROC) (const NVNqueueBuilder *builder, int *size);

/// \brief Function pointer type to call nvnQueueBuilderGetCommandFlushThreshold().
typedef NVNboolean (NVNAPIENTRYP PFNNVNQUEUEBUILDERGETCOMMANDFLUSHTHRESHOLDPROC) (const NVNqueueBuilder *builder, int *flushThreshold);

/// \brief Function pointer type to call nvnQueueBuilderGetMemorySize().
typedef int (NVNAPIENTRYP PFNNVNQUEUEBUILDERGETMEMORYSIZEPROC) (const NVNqueueBuilder *builder);

/// \brief Function pointer type to call nvnQueueBuilderGetMemory().
typedef void* (NVNAPIENTRYP PFNNVNQUEUEBUILDERGETMEMORYPROC) (const NVNqueueBuilder *builder);

/// \brief Function pointer type to call nvnQueueInitialize().
typedef NVNboolean (NVNAPIENTRYP PFNNVNQUEUEINITIALIZEPROC) (NVNqueue *queue, const NVNqueueBuilder *builder);

/// \brief Function pointer type to call nvnQueueFinalize().
typedef void (NVNAPIENTRYP PFNNVNQUEUEFINALIZEPROC) (NVNqueue *queue);

/// \brief Function pointer type to call nvnQueueSetDebugLabel().
typedef void (NVNAPIENTRYP PFNNVNQUEUESETDEBUGLABELPROC) (NVNqueue *queue, const char * label);

/// \brief Function pointer type to call nvnQueueSubmitCommands().
typedef void (NVNAPIENTRYP PFNNVNQUEUESUBMITCOMMANDSPROC) (NVNqueue *queue, int numCommands, const NVNcommandHandle *handles);

/// \brief Function pointer type to call nvnQueueFlush().
typedef void (NVNAPIENTRYP PFNNVNQUEUEFLUSHPROC) (NVNqueue *queue);

/// \brief Function pointer type to call nvnQueueFinish().
typedef void (NVNAPIENTRYP PFNNVNQUEUEFINISHPROC) (NVNqueue *queue);

/// \brief Function pointer type to call nvnQueuePresentTexture().
typedef void (NVNAPIENTRYP PFNNVNQUEUEPRESENTTEXTUREPROC) (NVNqueue *queue, NVNwindow *window, int textureIndex);

/// \brief Function pointer type to call nvnQueueAcquireTexture().
typedef NVNqueueAcquireTextureResult (NVNAPIENTRYP PFNNVNQUEUEACQUIRETEXTUREPROC) (NVNqueue *queue, NVNwindow *window, int *textureIndex);

/// \brief Function pointer type to call nvnWindowBuilderSetDevice().
typedef void (NVNAPIENTRYP PFNNVNWINDOWBUILDERSETDEVICEPROC) (NVNwindowBuilder *builder, NVNdevice *device);

/// \brief Function pointer type to call nvnWindowBuilderSetDefaults().
typedef void (NVNAPIENTRYP PFNNVNWINDOWBUILDERSETDEFAULTSPROC) (NVNwindowBuilder *builder);

/// \brief Function pointer type to call nvnWindowBuilderSetNativeWindow().
typedef void (NVNAPIENTRYP PFNNVNWINDOWBUILDERSETNATIVEWINDOWPROC) (NVNwindowBuilder *builder, NVNnativeWindow nativeWindow);

/// \brief Function pointer type to call nvnWindowBuilderSetTextures().
typedef void (NVNAPIENTRYP PFNNVNWINDOWBUILDERSETTEXTURESPROC) (NVNwindowBuilder *builder, int numTextures, NVNtexture * const *textures);

/// \brief Function pointer type to call nvnWindowBuilderSetPresentInterval().
typedef void (NVNAPIENTRYP PFNNVNWINDOWBUILDERSETPRESENTINTERVALPROC) (NVNwindowBuilder *builder, int presentInterval);

/// \brief Function pointer type to call nvnWindowBuilderSetNumActiveTextures().
typedef void (NVNAPIENTRYP PFNNVNWINDOWBUILDERSETNUMACTIVETEXTURESPROC) (NVNwindowBuilder *builder, int numActiveTextures);

/// \brief Function pointer type to call nvnWindowBuilderGetDevice().
typedef const NVNdevice * (NVNAPIENTRYP PFNNVNWINDOWBUILDERGETDEVICEPROC) (const NVNwindowBuilder *builder);

/// \brief Function pointer type to call nvnWindowBuilderGetNumTextures().
typedef int (NVNAPIENTRYP PFNNVNWINDOWBUILDERGETNUMTEXTURESPROC) (const NVNwindowBuilder *builder);

/// \brief Function pointer type to call nvnWindowBuilderGetTexture().
typedef const NVNtexture * (NVNAPIENTRYP PFNNVNWINDOWBUILDERGETTEXTUREPROC) (const NVNwindowBuilder *builder, int textureId);

/// \brief Function pointer type to call nvnWindowBuilderGetNativeWindow().
typedef NVNnativeWindow (NVNAPIENTRYP PFNNVNWINDOWBUILDERGETNATIVEWINDOWPROC) (const NVNwindowBuilder *builder);

/// \brief Function pointer type to call nvnWindowBuilderGetPresentInterval().
typedef int (NVNAPIENTRYP PFNNVNWINDOWBUILDERGETPRESENTINTERVALPROC) (const NVNwindowBuilder *builder);

/// \brief Function pointer type to call nvnWindowBuilderGetNumActiveTextures().
typedef int (NVNAPIENTRYP PFNNVNWINDOWBUILDERGETNUMACTIVETEXTURESPROC) (const NVNwindowBuilder *builder);

/// \brief Function pointer type to call nvnWindowInitialize().
typedef NVNboolean (NVNAPIENTRYP PFNNVNWINDOWINITIALIZEPROC) (NVNwindow *window, const NVNwindowBuilder *builder);

/// \brief Function pointer type to call nvnWindowFinalize().
typedef void (NVNAPIENTRYP PFNNVNWINDOWFINALIZEPROC) (NVNwindow *window);

/// \brief Function pointer type to call nvnWindowSetDebugLabel().
typedef void (NVNAPIENTRYP PFNNVNWINDOWSETDEBUGLABELPROC) (NVNwindow *window, const char * label);

/// \brief Function pointer type to call nvnWindowAcquireTexture().
typedef NVNwindowAcquireTextureResult (NVNAPIENTRYP PFNNVNWINDOWACQUIRETEXTUREPROC) (NVNwindow *window, NVNsync *textureAvailableSync, int *textureIndex);

/// \brief Function pointer type to call nvnWindowGetNativeWindow().
typedef NVNnativeWindow (NVNAPIENTRYP PFNNVNWINDOWGETNATIVEWINDOWPROC) (const NVNwindow *window);

/// \brief Function pointer type to call nvnWindowGetPresentInterval().
typedef int (NVNAPIENTRYP PFNNVNWINDOWGETPRESENTINTERVALPROC) (const NVNwindow *window);

/// \brief Function pointer type to call nvnWindowSetPresentInterval().
typedef void (NVNAPIENTRYP PFNNVNWINDOWSETPRESENTINTERVALPROC) (NVNwindow *window, int presentInterval);

/// \brief Function pointer type to call nvnWindowSetCrop().
typedef void (NVNAPIENTRYP PFNNVNWINDOWSETCROPPROC) (NVNwindow *window, int x, int y, int w, int h);

/// \brief Function pointer type to call nvnWindowGetCrop().
typedef void (NVNAPIENTRYP PFNNVNWINDOWGETCROPPROC) (const NVNwindow *window, NVNrectangle *crop);

/// \brief Function pointer type to call nvnWindowSetNumActiveTextures().
typedef void (NVNAPIENTRYP PFNNVNWINDOWSETNUMACTIVETEXTURESPROC) (NVNwindow *window, int numActiveTextures);

/// \brief Function pointer type to call nvnWindowGetNumActiveTextures().
typedef int (NVNAPIENTRYP PFNNVNWINDOWGETNUMACTIVETEXTURESPROC) (const NVNwindow *window);

/// \brief Function pointer type to call nvnWindowGetNumTextures().
typedef int (NVNAPIENTRYP PFNNVNWINDOWGETNUMTEXTURESPROC) (const NVNwindow *window);

/// \brief Function pointer type to call nvnProgramInitialize().
typedef NVNboolean (NVNAPIENTRYP PFNNVNPROGRAMINITIALIZEPROC) (NVNprogram *program, NVNdevice *device);

/// \brief Function pointer type to call nvnProgramFinalize().
typedef void (NVNAPIENTRYP PFNNVNPROGRAMFINALIZEPROC) (NVNprogram *program);

/// \brief Function pointer type to call nvnProgramSetDebugLabel().
typedef void (NVNAPIENTRYP PFNNVNPROGRAMSETDEBUGLABELPROC) (NVNprogram *program, const char * label);

/// \brief Function pointer type to call nvnProgramSetShaders().
typedef NVNboolean (NVNAPIENTRYP PFNNVNPROGRAMSETSHADERSPROC) (NVNprogram *program, int count, const NVNshaderData *stageData);

/// \brief Function pointer type to call nvnProgramSetShadersExt().
typedef NVNboolean (NVNAPIENTRYP PFNNVNPROGRAMSETSHADERSEXTPROC) (NVNprogram *program, int count, const NVNshaderDataExt *stageDataExt);

/// \brief Function pointer type to call nvnProgramSetSampleShading().
typedef void (NVNAPIENTRYP PFNNVNPROGRAMSETSAMPLESHADINGPROC) (NVNprogram *program, NVNsampleShadingMode shadingMode);

/// \brief Function pointer type to call nvnMemoryPoolBuilderSetDevice().
typedef void (NVNAPIENTRYP PFNNVNMEMORYPOOLBUILDERSETDEVICEPROC) (NVNmemoryPoolBuilder *builder, NVNdevice *device);

/// \brief Function pointer type to call nvnMemoryPoolBuilderSetDefaults().
typedef void (NVNAPIENTRYP PFNNVNMEMORYPOOLBUILDERSETDEFAULTSPROC) (NVNmemoryPoolBuilder *builder);

/// \brief Function pointer type to call nvnMemoryPoolBuilderSetStorage().
typedef void (NVNAPIENTRYP PFNNVNMEMORYPOOLBUILDERSETSTORAGEPROC) (NVNmemoryPoolBuilder *builder, void *memory, size_t size);

/// \brief Function pointer type to call nvnMemoryPoolBuilderSetFlags().
typedef void (NVNAPIENTRYP PFNNVNMEMORYPOOLBUILDERSETFLAGSPROC) (NVNmemoryPoolBuilder *builder, int flags);

/// \brief Function pointer type to call nvnMemoryPoolBuilderGetDevice().
typedef const NVNdevice * (NVNAPIENTRYP PFNNVNMEMORYPOOLBUILDERGETDEVICEPROC) (const NVNmemoryPoolBuilder *builder);

/// \brief Function pointer type to call nvnMemoryPoolBuilderGetMemory().
typedef void* (NVNAPIENTRYP PFNNVNMEMORYPOOLBUILDERGETMEMORYPROC) (const NVNmemoryPoolBuilder *builder);

/// \brief Function pointer type to call nvnMemoryPoolBuilderGetSize().
typedef size_t (NVNAPIENTRYP PFNNVNMEMORYPOOLBUILDERGETSIZEPROC) (const NVNmemoryPoolBuilder *builder);

/// \brief Function pointer type to call nvnMemoryPoolBuilderGetFlags().
typedef NVNmemoryPoolFlags (NVNAPIENTRYP PFNNVNMEMORYPOOLBUILDERGETFLAGSPROC) (const NVNmemoryPoolBuilder *builder);

/// \brief Function pointer type to call nvnMemoryPoolInitialize().
typedef NVNboolean (NVNAPIENTRYP PFNNVNMEMORYPOOLINITIALIZEPROC) (NVNmemoryPool *pool, const NVNmemoryPoolBuilder *builder);

/// \brief Function pointer type to call nvnMemoryPoolSetDebugLabel().
typedef void (NVNAPIENTRYP PFNNVNMEMORYPOOLSETDEBUGLABELPROC) (NVNmemoryPool *pool, const char * label);

/// \brief Function pointer type to call nvnMemoryPoolFinalize().
typedef void (NVNAPIENTRYP PFNNVNMEMORYPOOLFINALIZEPROC) (NVNmemoryPool *pool);

/// \brief Function pointer type to call nvnMemoryPoolMap().
typedef void* (NVNAPIENTRYP PFNNVNMEMORYPOOLMAPPROC) (const NVNmemoryPool *pool);

/// \brief Function pointer type to call nvnMemoryPoolFlushMappedRange().
typedef void (NVNAPIENTRYP PFNNVNMEMORYPOOLFLUSHMAPPEDRANGEPROC) (const NVNmemoryPool *pool, ptrdiff_t offset, size_t size);

/// \brief Function pointer type to call nvnMemoryPoolInvalidateMappedRange().
typedef void (NVNAPIENTRYP PFNNVNMEMORYPOOLINVALIDATEMAPPEDRANGEPROC) (const NVNmemoryPool *pool, ptrdiff_t offset, size_t size);

/// \brief Function pointer type to call nvnMemoryPoolGetBufferAddress().
typedef NVNbufferAddress (NVNAPIENTRYP PFNNVNMEMORYPOOLGETBUFFERADDRESSPROC) (const NVNmemoryPool *pool);

/// \brief Function pointer type to call nvnMemoryPoolMapVirtual().
typedef NVNboolean (NVNAPIENTRYP PFNNVNMEMORYPOOLMAPVIRTUALPROC) (NVNmemoryPool *virtualPool, int numRequests, const NVNmappingRequest *requests);

/// \brief Function pointer type to call nvnMemoryPoolGetSize().
typedef size_t (NVNAPIENTRYP PFNNVNMEMORYPOOLGETSIZEPROC) (const NVNmemoryPool *pool);

/// \brief Function pointer type to call nvnMemoryPoolGetFlags().
typedef NVNmemoryPoolFlags (NVNAPIENTRYP PFNNVNMEMORYPOOLGETFLAGSPROC) (const NVNmemoryPool *pool);

/// \brief Function pointer type to call nvnTexturePoolInitialize().
typedef NVNboolean (NVNAPIENTRYP PFNNVNTEXTUREPOOLINITIALIZEPROC) (NVNtexturePool *texturePool, const NVNmemoryPool *memoryPool, ptrdiff_t offset, int numDescriptors);

/// \brief Function pointer type to call nvnTexturePoolSetDebugLabel().
typedef void (NVNAPIENTRYP PFNNVNTEXTUREPOOLSETDEBUGLABELPROC) (NVNtexturePool *pool, const char * label);

/// \brief Function pointer type to call nvnTexturePoolFinalize().
typedef void (NVNAPIENTRYP PFNNVNTEXTUREPOOLFINALIZEPROC) (NVNtexturePool *pool);

/// \brief Function pointer type to call nvnTexturePoolRegisterTexture().
typedef void (NVNAPIENTRYP PFNNVNTEXTUREPOOLREGISTERTEXTUREPROC) (const NVNtexturePool *texturePool, int id, const NVNtexture *texture, const NVNtextureView *view);

/// \brief Function pointer type to call nvnTexturePoolRegisterImage().
typedef void (NVNAPIENTRYP PFNNVNTEXTUREPOOLREGISTERIMAGEPROC) (const NVNtexturePool *texturePool, int id, const NVNtexture *texture, const NVNtextureView *view);

/// \brief Function pointer type to call nvnTexturePoolGetMemoryPool().
typedef const NVNmemoryPool * (NVNAPIENTRYP PFNNVNTEXTUREPOOLGETMEMORYPOOLPROC) (const NVNtexturePool *pool);

/// \brief Function pointer type to call nvnTexturePoolGetMemoryOffset().
typedef ptrdiff_t (NVNAPIENTRYP PFNNVNTEXTUREPOOLGETMEMORYOFFSETPROC) (const NVNtexturePool *pool);

/// \brief Function pointer type to call nvnTexturePoolGetSize().
typedef int (NVNAPIENTRYP PFNNVNTEXTUREPOOLGETSIZEPROC) (const NVNtexturePool *pool);

/// \brief Function pointer type to call nvnSamplerPoolInitialize().
typedef NVNboolean (NVNAPIENTRYP PFNNVNSAMPLERPOOLINITIALIZEPROC) (NVNsamplerPool *samplerPool, const NVNmemoryPool *memoryPool, ptrdiff_t offset, int numDescriptors);

/// \brief Function pointer type to call nvnSamplerPoolSetDebugLabel().
typedef void (NVNAPIENTRYP PFNNVNSAMPLERPOOLSETDEBUGLABELPROC) (NVNsamplerPool *pool, const char * label);

/// \brief Function pointer type to call nvnSamplerPoolFinalize().
typedef void (NVNAPIENTRYP PFNNVNSAMPLERPOOLFINALIZEPROC) (NVNsamplerPool *pool);

/// \brief Function pointer type to call nvnSamplerPoolRegisterSampler().
typedef void (NVNAPIENTRYP PFNNVNSAMPLERPOOLREGISTERSAMPLERPROC) (const NVNsamplerPool *samplerPool, int id, const NVNsampler *sampler);

/// \brief Function pointer type to call nvnSamplerPoolRegisterSamplerBuilder().
typedef void (NVNAPIENTRYP PFNNVNSAMPLERPOOLREGISTERSAMPLERBUILDERPROC) (const NVNsamplerPool *samplerPool, int id, const NVNsamplerBuilder *builder);

/// \brief Function pointer type to call nvnSamplerPoolGetMemoryPool().
typedef const NVNmemoryPool * (NVNAPIENTRYP PFNNVNSAMPLERPOOLGETMEMORYPOOLPROC) (const NVNsamplerPool *pool);

/// \brief Function pointer type to call nvnSamplerPoolGetMemoryOffset().
typedef ptrdiff_t (NVNAPIENTRYP PFNNVNSAMPLERPOOLGETMEMORYOFFSETPROC) (const NVNsamplerPool *pool);

/// \brief Function pointer type to call nvnSamplerPoolGetSize().
typedef int (NVNAPIENTRYP PFNNVNSAMPLERPOOLGETSIZEPROC) (const NVNsamplerPool *pool);

/// \brief Function pointer type to call nvnBufferBuilderSetDevice().
typedef void (NVNAPIENTRYP PFNNVNBUFFERBUILDERSETDEVICEPROC) (NVNbufferBuilder *builder, NVNdevice *device);

/// \brief Function pointer type to call nvnBufferBuilderSetDefaults().
typedef void (NVNAPIENTRYP PFNNVNBUFFERBUILDERSETDEFAULTSPROC) (NVNbufferBuilder *builder);

/// \brief Function pointer type to call nvnBufferBuilderSetStorage().
typedef void (NVNAPIENTRYP PFNNVNBUFFERBUILDERSETSTORAGEPROC) (NVNbufferBuilder *builder, NVNmemoryPool *pool, ptrdiff_t offset, size_t size);

/// \brief Function pointer type to call nvnBufferBuilderGetDevice().
typedef const NVNdevice * (NVNAPIENTRYP PFNNVNBUFFERBUILDERGETDEVICEPROC) (const NVNbufferBuilder *builder);

/// \brief Function pointer type to call nvnBufferBuilderGetMemoryPool().
typedef NVNmemoryPool * (NVNAPIENTRYP PFNNVNBUFFERBUILDERGETMEMORYPOOLPROC) (const NVNbufferBuilder *builder);

/// \brief Function pointer type to call nvnBufferBuilderGetMemoryOffset().
typedef ptrdiff_t (NVNAPIENTRYP PFNNVNBUFFERBUILDERGETMEMORYOFFSETPROC) (const NVNbufferBuilder *builder);

/// \brief Function pointer type to call nvnBufferBuilderGetSize().
typedef size_t (NVNAPIENTRYP PFNNVNBUFFERBUILDERGETSIZEPROC) (const NVNbufferBuilder *builder);

/// \brief Function pointer type to call nvnBufferInitialize().
typedef NVNboolean (NVNAPIENTRYP PFNNVNBUFFERINITIALIZEPROC) (NVNbuffer *buffer, const NVNbufferBuilder *builder);

/// \brief Function pointer type to call nvnBufferSetDebugLabel().
typedef void (NVNAPIENTRYP PFNNVNBUFFERSETDEBUGLABELPROC) (NVNbuffer *buffer, const char * label);

/// \brief Function pointer type to call nvnBufferFinalize().
typedef void (NVNAPIENTRYP PFNNVNBUFFERFINALIZEPROC) (NVNbuffer *buffer);

/// \brief Function pointer type to call nvnBufferMap().
typedef void* (NVNAPIENTRYP PFNNVNBUFFERMAPPROC) (const NVNbuffer *buffer);

/// \brief Function pointer type to call nvnBufferGetAddress().
typedef NVNbufferAddress (NVNAPIENTRYP PFNNVNBUFFERGETADDRESSPROC) (const NVNbuffer *buffer);

/// \brief Function pointer type to call nvnBufferFlushMappedRange().
typedef void (NVNAPIENTRYP PFNNVNBUFFERFLUSHMAPPEDRANGEPROC) (const NVNbuffer *buffer, ptrdiff_t offset, size_t size);

/// \brief Function pointer type to call nvnBufferInvalidateMappedRange().
typedef void (NVNAPIENTRYP PFNNVNBUFFERINVALIDATEMAPPEDRANGEPROC) (const NVNbuffer *buffer, ptrdiff_t offset, size_t size);

/// \brief Function pointer type to call nvnBufferGetMemoryPool().
typedef NVNmemoryPool * (NVNAPIENTRYP PFNNVNBUFFERGETMEMORYPOOLPROC) (const NVNbuffer *buffer);

/// \brief Function pointer type to call nvnBufferGetMemoryOffset().
typedef ptrdiff_t (NVNAPIENTRYP PFNNVNBUFFERGETMEMORYOFFSETPROC) (const NVNbuffer *buffer);

/// \brief Function pointer type to call nvnBufferGetSize().
typedef size_t (NVNAPIENTRYP PFNNVNBUFFERGETSIZEPROC) (const NVNbuffer *buffer);

/// \brief Function pointer type to call nvnBufferGetDebugID().
typedef uint64_t (NVNAPIENTRYP PFNNVNBUFFERGETDEBUGIDPROC) (const NVNbuffer *buffer);

/// \brief Function pointer type to call nvnTextureBuilderSetDevice().
typedef void (NVNAPIENTRYP PFNNVNTEXTUREBUILDERSETDEVICEPROC) (NVNtextureBuilder *builder, NVNdevice *device);

/// \brief Function pointer type to call nvnTextureBuilderSetDefaults().
typedef void (NVNAPIENTRYP PFNNVNTEXTUREBUILDERSETDEFAULTSPROC) (NVNtextureBuilder *builder);

/// \brief Function pointer type to call nvnTextureBuilderSetFlags().
typedef void (NVNAPIENTRYP PFNNVNTEXTUREBUILDERSETFLAGSPROC) (NVNtextureBuilder *builder, int flags);

/// \brief Function pointer type to call nvnTextureBuilderSetTarget().
typedef void (NVNAPIENTRYP PFNNVNTEXTUREBUILDERSETTARGETPROC) (NVNtextureBuilder *builder, NVNtextureTarget target);

/// \brief Function pointer type to call nvnTextureBuilderSetWidth().
typedef void (NVNAPIENTRYP PFNNVNTEXTUREBUILDERSETWIDTHPROC) (NVNtextureBuilder *builder, int width);

/// \brief Function pointer type to call nvnTextureBuilderSetHeight().
typedef void (NVNAPIENTRYP PFNNVNTEXTUREBUILDERSETHEIGHTPROC) (NVNtextureBuilder *builder, int height);

/// \brief Function pointer type to call nvnTextureBuilderSetDepth().
typedef void (NVNAPIENTRYP PFNNVNTEXTUREBUILDERSETDEPTHPROC) (NVNtextureBuilder *builder, int depth);

/// \brief Function pointer type to call nvnTextureBuilderSetSize1D().
typedef void (NVNAPIENTRYP PFNNVNTEXTUREBUILDERSETSIZE1DPROC) (NVNtextureBuilder *builder, int width);

/// \brief Function pointer type to call nvnTextureBuilderSetSize2D().
typedef void (NVNAPIENTRYP PFNNVNTEXTUREBUILDERSETSIZE2DPROC) (NVNtextureBuilder *builder, int width, int height);

/// \brief Function pointer type to call nvnTextureBuilderSetSize3D().
typedef void (NVNAPIENTRYP PFNNVNTEXTUREBUILDERSETSIZE3DPROC) (NVNtextureBuilder *builder, int width, int height, int depth);

/// \brief Function pointer type to call nvnTextureBuilderSetLevels().
typedef void (NVNAPIENTRYP PFNNVNTEXTUREBUILDERSETLEVELSPROC) (NVNtextureBuilder *builder, int numLevels);

/// \brief Function pointer type to call nvnTextureBuilderSetFormat().
typedef void (NVNAPIENTRYP PFNNVNTEXTUREBUILDERSETFORMATPROC) (NVNtextureBuilder *builder, NVNformat format);

/// \brief Function pointer type to call nvnTextureBuilderSetSamples().
typedef void (NVNAPIENTRYP PFNNVNTEXTUREBUILDERSETSAMPLESPROC) (NVNtextureBuilder *builder, int samples);

/// \brief Function pointer type to call nvnTextureBuilderSetSwizzle().
typedef void (NVNAPIENTRYP PFNNVNTEXTUREBUILDERSETSWIZZLEPROC) (NVNtextureBuilder *builder, NVNtextureSwizzle r, NVNtextureSwizzle g, NVNtextureSwizzle b, NVNtextureSwizzle a);

/// \brief Function pointer type to call nvnTextureBuilderSetDepthStencilMode().
typedef void (NVNAPIENTRYP PFNNVNTEXTUREBUILDERSETDEPTHSTENCILMODEPROC) (NVNtextureBuilder *builder, NVNtextureDepthStencilMode mode);

/// \brief Function pointer type to call nvnTextureBuilderGetStorageSize().
typedef size_t (NVNAPIENTRYP PFNNVNTEXTUREBUILDERGETSTORAGESIZEPROC) (const NVNtextureBuilder *builder);

/// \brief Function pointer type to call nvnTextureBuilderGetStorageAlignment().
typedef size_t (NVNAPIENTRYP PFNNVNTEXTUREBUILDERGETSTORAGEALIGNMENTPROC) (const NVNtextureBuilder *builder);

/// \brief Function pointer type to call nvnTextureBuilderSetStorage().
typedef void (NVNAPIENTRYP PFNNVNTEXTUREBUILDERSETSTORAGEPROC) (NVNtextureBuilder *builder, NVNmemoryPool *pool, ptrdiff_t offset);

/// \brief Function pointer type to call nvnTextureBuilderSetPackagedTextureData().
typedef void (NVNAPIENTRYP PFNNVNTEXTUREBUILDERSETPACKAGEDTEXTUREDATAPROC) (NVNtextureBuilder *builder, const void *data);

/// \brief Function pointer type to call nvnTextureBuilderSetPackagedTextureLayout().
typedef void (NVNAPIENTRYP PFNNVNTEXTUREBUILDERSETPACKAGEDTEXTURELAYOUTPROC) (NVNtextureBuilder *builder, const NVNpackagedTextureLayout *layout);

/// \brief Function pointer type to call nvnTextureBuilderSetStride().
typedef void (NVNAPIENTRYP PFNNVNTEXTUREBUILDERSETSTRIDEPROC) (NVNtextureBuilder *builder, ptrdiff_t stride);

/// \brief Function pointer type to call nvnTextureBuilderSetGLTextureName().
typedef void (NVNAPIENTRYP PFNNVNTEXTUREBUILDERSETGLTEXTURENAMEPROC) (NVNtextureBuilder *builder, uint32_t name);

/// \brief Function pointer type to call nvnTextureBuilderGetStorageClass().
typedef NVNstorageClass (NVNAPIENTRYP PFNNVNTEXTUREBUILDERGETSTORAGECLASSPROC) (const NVNtextureBuilder *builder);

/// \brief Function pointer type to call nvnTextureBuilderGetDevice().
typedef const NVNdevice * (NVNAPIENTRYP PFNNVNTEXTUREBUILDERGETDEVICEPROC) (const NVNtextureBuilder *builder);

/// \brief Function pointer type to call nvnTextureBuilderGetFlags().
typedef NVNtextureFlags (NVNAPIENTRYP PFNNVNTEXTUREBUILDERGETFLAGSPROC) (const NVNtextureBuilder *builder);

/// \brief Function pointer type to call nvnTextureBuilderGetTarget().
typedef NVNtextureTarget (NVNAPIENTRYP PFNNVNTEXTUREBUILDERGETTARGETPROC) (const NVNtextureBuilder *builder);

/// \brief Function pointer type to call nvnTextureBuilderGetWidth().
typedef int (NVNAPIENTRYP PFNNVNTEXTUREBUILDERGETWIDTHPROC) (const NVNtextureBuilder *builder);

/// \brief Function pointer type to call nvnTextureBuilderGetHeight().
typedef int (NVNAPIENTRYP PFNNVNTEXTUREBUILDERGETHEIGHTPROC) (const NVNtextureBuilder *builder);

/// \brief Function pointer type to call nvnTextureBuilderGetDepth().
typedef int (NVNAPIENTRYP PFNNVNTEXTUREBUILDERGETDEPTHPROC) (const NVNtextureBuilder *builder);

/// \brief Function pointer type to call nvnTextureBuilderGetLevels().
typedef int (NVNAPIENTRYP PFNNVNTEXTUREBUILDERGETLEVELSPROC) (const NVNtextureBuilder *builder);

/// \brief Function pointer type to call nvnTextureBuilderGetFormat().
typedef NVNformat (NVNAPIENTRYP PFNNVNTEXTUREBUILDERGETFORMATPROC) (const NVNtextureBuilder *builder);

/// \brief Function pointer type to call nvnTextureBuilderGetSamples().
typedef int (NVNAPIENTRYP PFNNVNTEXTUREBUILDERGETSAMPLESPROC) (const NVNtextureBuilder *builder);

/// \brief Function pointer type to call nvnTextureBuilderGetSwizzle().
typedef void (NVNAPIENTRYP PFNNVNTEXTUREBUILDERGETSWIZZLEPROC) (const NVNtextureBuilder *builder, NVNtextureSwizzle *r, NVNtextureSwizzle *g, NVNtextureSwizzle *b, NVNtextureSwizzle *a);

/// \brief Function pointer type to call nvnTextureBuilderGetDepthStencilMode().
typedef NVNtextureDepthStencilMode (NVNAPIENTRYP PFNNVNTEXTUREBUILDERGETDEPTHSTENCILMODEPROC) (const NVNtextureBuilder *builder);

/// \brief Function pointer type to call nvnTextureBuilderGetPackagedTextureData().
typedef const void * (NVNAPIENTRYP PFNNVNTEXTUREBUILDERGETPACKAGEDTEXTUREDATAPROC) (const NVNtextureBuilder *builder);

/// \brief Function pointer type to call nvnTextureBuilderGetPackagedTextureLayout().
typedef NVNboolean (NVNAPIENTRYP PFNNVNTEXTUREBUILDERGETPACKAGEDTEXTURELAYOUTPROC) (const NVNtextureBuilder *builder, NVNpackagedTextureLayout *data);

/// \brief Function pointer type to call nvnTextureBuilderGetStride().
typedef ptrdiff_t (NVNAPIENTRYP PFNNVNTEXTUREBUILDERGETSTRIDEPROC) (const NVNtextureBuilder *builder);

/// \brief Function pointer type to call nvnTextureBuilderGetSparseTileLayout().
typedef void (NVNAPIENTRYP PFNNVNTEXTUREBUILDERGETSPARSETILELAYOUTPROC) (const NVNtextureBuilder *builder, NVNtextureSparseTileLayout *layout);

/// \brief Function pointer type to call nvnTextureBuilderGetGLTextureName().
typedef uint32_t (NVNAPIENTRYP PFNNVNTEXTUREBUILDERGETGLTEXTURENAMEPROC) (const NVNtextureBuilder *builder);

/// \brief Function pointer type to call nvnTextureBuilderGetZCullStorageSize().
typedef size_t (NVNAPIENTRYP PFNNVNTEXTUREBUILDERGETZCULLSTORAGESIZEPROC) (const NVNtextureBuilder *builder);

/// \brief Function pointer type to call nvnTextureBuilderGetMemoryPool().
typedef NVNmemoryPool * (NVNAPIENTRYP PFNNVNTEXTUREBUILDERGETMEMORYPOOLPROC) (const NVNtextureBuilder *builder);

/// \brief Function pointer type to call nvnTextureBuilderGetMemoryOffset().
typedef ptrdiff_t (NVNAPIENTRYP PFNNVNTEXTUREBUILDERGETMEMORYOFFSETPROC) (const NVNtextureBuilder *builder);

/// \brief Function pointer type to call nvnTextureBuilderGetRawStorageClass().
typedef NVNrawStorageClass (NVNAPIENTRYP PFNNVNTEXTUREBUILDERGETRAWSTORAGECLASSPROC) (const NVNtextureBuilder *builder);

/// \brief Function pointer type to call nvnTextureViewSetDefaults().
typedef void (NVNAPIENTRYP PFNNVNTEXTUREVIEWSETDEFAULTSPROC) (NVNtextureView *view);

/// \brief Function pointer type to call nvnTextureViewSetLevels().
typedef void (NVNAPIENTRYP PFNNVNTEXTUREVIEWSETLEVELSPROC) (NVNtextureView *view, int baseLevel, int numLevels);

/// \brief Function pointer type to call nvnTextureViewSetLayers().
typedef void (NVNAPIENTRYP PFNNVNTEXTUREVIEWSETLAYERSPROC) (NVNtextureView *view, int minLayer, int numLayers);

/// \brief Function pointer type to call nvnTextureViewSetFormat().
typedef void (NVNAPIENTRYP PFNNVNTEXTUREVIEWSETFORMATPROC) (NVNtextureView *view, NVNformat format);

/// \brief Function pointer type to call nvnTextureViewSetSwizzle().
typedef void (NVNAPIENTRYP PFNNVNTEXTUREVIEWSETSWIZZLEPROC) (NVNtextureView *view, NVNtextureSwizzle r, NVNtextureSwizzle g, NVNtextureSwizzle b, NVNtextureSwizzle a);

/// \brief Function pointer type to call nvnTextureViewSetDepthStencilMode().
typedef void (NVNAPIENTRYP PFNNVNTEXTUREVIEWSETDEPTHSTENCILMODEPROC) (NVNtextureView *view, NVNtextureDepthStencilMode mode);

/// \brief Function pointer type to call nvnTextureViewSetTarget().
typedef void (NVNAPIENTRYP PFNNVNTEXTUREVIEWSETTARGETPROC) (NVNtextureView *view, NVNtextureTarget target);

/// \brief Function pointer type to call nvnTextureViewGetLevels().
typedef NVNboolean (NVNAPIENTRYP PFNNVNTEXTUREVIEWGETLEVELSPROC) (const NVNtextureView *view, int *baseLevel, int *numLevels);

/// \brief Function pointer type to call nvnTextureViewGetLayers().
typedef NVNboolean (NVNAPIENTRYP PFNNVNTEXTUREVIEWGETLAYERSPROC) (const NVNtextureView *view, int *minLayer, int *numLayers);

/// \brief Function pointer type to call nvnTextureViewGetFormat().
typedef NVNboolean (NVNAPIENTRYP PFNNVNTEXTUREVIEWGETFORMATPROC) (const NVNtextureView *view, NVNformat *format);

/// \brief Function pointer type to call nvnTextureViewGetSwizzle().
typedef NVNboolean (NVNAPIENTRYP PFNNVNTEXTUREVIEWGETSWIZZLEPROC) (const NVNtextureView *view, NVNtextureSwizzle *r, NVNtextureSwizzle *g, NVNtextureSwizzle *b, NVNtextureSwizzle *a);

/// \brief Function pointer type to call nvnTextureViewGetDepthStencilMode().
typedef NVNboolean (NVNAPIENTRYP PFNNVNTEXTUREVIEWGETDEPTHSTENCILMODEPROC) (const NVNtextureView *view, NVNtextureDepthStencilMode *mode);

/// \brief Function pointer type to call nvnTextureViewGetTarget().
typedef NVNboolean (NVNAPIENTRYP PFNNVNTEXTUREVIEWGETTARGETPROC) (const NVNtextureView *view, NVNtextureTarget *target);

/// \brief Function pointer type to call nvnTextureViewCompare().
typedef NVNboolean (NVNAPIENTRYP PFNNVNTEXTUREVIEWCOMPAREPROC) (const NVNtextureView *view, const NVNtextureView *otherView);

/// \brief Function pointer type to call nvnTextureInitialize().
typedef NVNboolean (NVNAPIENTRYP PFNNVNTEXTUREINITIALIZEPROC) (NVNtexture *texture, const NVNtextureBuilder *builder);

/// \brief Function pointer type to call nvnTextureGetZCullStorageSize().
typedef size_t (NVNAPIENTRYP PFNNVNTEXTUREGETZCULLSTORAGESIZEPROC) (const NVNtexture *texture);

/// \brief Function pointer type to call nvnTextureFinalize().
typedef void (NVNAPIENTRYP PFNNVNTEXTUREFINALIZEPROC) (NVNtexture *texture);

/// \brief Function pointer type to call nvnTextureSetDebugLabel().
typedef void (NVNAPIENTRYP PFNNVNTEXTURESETDEBUGLABELPROC) (NVNtexture *texture, const char * label);

/// \brief Function pointer type to call nvnTextureGetStorageClass().
typedef NVNstorageClass (NVNAPIENTRYP PFNNVNTEXTUREGETSTORAGECLASSPROC) (const NVNtexture *texture);

/// \brief Function pointer type to call nvnTextureGetViewOffset().
typedef ptrdiff_t (NVNAPIENTRYP PFNNVNTEXTUREGETVIEWOFFSETPROC) (const NVNtexture *texture, const NVNtextureView *textureView);

/// \brief Function pointer type to call nvnTextureGetFlags().
typedef NVNtextureFlags (NVNAPIENTRYP PFNNVNTEXTUREGETFLAGSPROC) (const NVNtexture *texture);

/// \brief Function pointer type to call nvnTextureGetTarget().
typedef NVNtextureTarget (NVNAPIENTRYP PFNNVNTEXTUREGETTARGETPROC) (const NVNtexture *texture);

/// \brief Function pointer type to call nvnTextureGetWidth().
typedef int (NVNAPIENTRYP PFNNVNTEXTUREGETWIDTHPROC) (const NVNtexture *texture);

/// \brief Function pointer type to call nvnTextureGetHeight().
typedef int (NVNAPIENTRYP PFNNVNTEXTUREGETHEIGHTPROC) (const NVNtexture *texture);

/// \brief Function pointer type to call nvnTextureGetDepth().
typedef int (NVNAPIENTRYP PFNNVNTEXTUREGETDEPTHPROC) (const NVNtexture *texture);

/// \brief Function pointer type to call nvnTextureGetLevels().
typedef int (NVNAPIENTRYP PFNNVNTEXTUREGETLEVELSPROC) (const NVNtexture *texture);

/// \brief Function pointer type to call nvnTextureGetFormat().
typedef NVNformat (NVNAPIENTRYP PFNNVNTEXTUREGETFORMATPROC) (const NVNtexture *texture);

/// \brief Function pointer type to call nvnTextureGetSamples().
typedef int (NVNAPIENTRYP PFNNVNTEXTUREGETSAMPLESPROC) (const NVNtexture *texture);

/// \brief Function pointer type to call nvnTextureGetSwizzle().
typedef void (NVNAPIENTRYP PFNNVNTEXTUREGETSWIZZLEPROC) (const NVNtexture *texture, NVNtextureSwizzle *r, NVNtextureSwizzle *g, NVNtextureSwizzle *b, NVNtextureSwizzle *a);

/// \brief Function pointer type to call nvnTextureGetDepthStencilMode().
typedef NVNtextureDepthStencilMode (NVNAPIENTRYP PFNNVNTEXTUREGETDEPTHSTENCILMODEPROC) (const NVNtexture *texture);

/// \brief Function pointer type to call nvnTextureGetStride().
typedef ptrdiff_t (NVNAPIENTRYP PFNNVNTEXTUREGETSTRIDEPROC) (const NVNtexture *texture);

/// \brief Function pointer type to call nvnTextureGetTextureAddress().
typedef NVNtextureAddress (NVNAPIENTRYP PFNNVNTEXTUREGETTEXTUREADDRESSPROC) (const NVNtexture *texture);

/// \brief Function pointer type to call nvnTextureGetSparseTileLayout().
typedef void (NVNAPIENTRYP PFNNVNTEXTUREGETSPARSETILELAYOUTPROC) (const NVNtexture *texture, NVNtextureSparseTileLayout *layout);

/// \brief Function pointer type to call nvnTextureWriteTexels().
typedef void (NVNAPIENTRYP PFNNVNTEXTUREWRITETEXELSPROC) (const NVNtexture *texture, const NVNtextureView *textureView, const NVNcopyRegion *region, const void *data);

/// \brief Function pointer type to call nvnTextureWriteTexelsStrided().
typedef void (NVNAPIENTRYP PFNNVNTEXTUREWRITETEXELSSTRIDEDPROC) (const NVNtexture *texture, const NVNtextureView *textureView, const NVNcopyRegion *region, const void *data, ptrdiff_t rowStride, ptrdiff_t imageStride);

/// \brief Function pointer type to call nvnTextureReadTexels().
typedef void (NVNAPIENTRYP PFNNVNTEXTUREREADTEXELSPROC) (const NVNtexture *texture, const NVNtextureView *textureView, const NVNcopyRegion *region, void *data);

/// \brief Function pointer type to call nvnTextureReadTexelsStrided().
typedef void (NVNAPIENTRYP PFNNVNTEXTUREREADTEXELSSTRIDEDPROC) (const NVNtexture *texture, const NVNtextureView *textureView, const NVNcopyRegion *region, void *data, ptrdiff_t rowStride, ptrdiff_t imageStride);

/// \brief Function pointer type to call nvnTextureFlushTexels().
typedef void (NVNAPIENTRYP PFNNVNTEXTUREFLUSHTEXELSPROC) (const NVNtexture *texture, const NVNtextureView *textureView, const NVNcopyRegion *region);

/// \brief Function pointer type to call nvnTextureInvalidateTexels().
typedef void (NVNAPIENTRYP PFNNVNTEXTUREINVALIDATETEXELSPROC) (const NVNtexture *texture, const NVNtextureView *textureView, const NVNcopyRegion *region);

/// \brief Function pointer type to call nvnTextureGetMemoryPool().
typedef NVNmemoryPool * (NVNAPIENTRYP PFNNVNTEXTUREGETMEMORYPOOLPROC) (const NVNtexture *texture);

/// \brief Function pointer type to call nvnTextureGetMemoryOffset().
typedef ptrdiff_t (NVNAPIENTRYP PFNNVNTEXTUREGETMEMORYOFFSETPROC) (const NVNtexture *texture);

/// \brief Function pointer type to call nvnTextureGetStorageSize().
typedef int (NVNAPIENTRYP PFNNVNTEXTUREGETSTORAGESIZEPROC) (const NVNtexture *texture);

/// \brief Function pointer type to call nvnTextureCompare().
typedef NVNboolean (NVNAPIENTRYP PFNNVNTEXTURECOMPAREPROC) (const NVNtexture *texture, const NVNtexture *otherTexture);

/// \brief Function pointer type to call nvnTextureGetDebugID().
typedef uint64_t (NVNAPIENTRYP PFNNVNTEXTUREGETDEBUGIDPROC) (const NVNtexture *texture);

/// \brief Function pointer type to call nvnTextureGetRawStorageClass().
typedef NVNrawStorageClass (NVNAPIENTRYP PFNNVNTEXTUREGETRAWSTORAGECLASSPROC) (const NVNtexture *texture);

/// \brief Function pointer type to call nvnSamplerBuilderSetDevice().
typedef void (NVNAPIENTRYP PFNNVNSAMPLERBUILDERSETDEVICEPROC) (NVNsamplerBuilder *builder, NVNdevice *device);

/// \brief Function pointer type to call nvnSamplerBuilderSetDefaults().
typedef void (NVNAPIENTRYP PFNNVNSAMPLERBUILDERSETDEFAULTSPROC) (NVNsamplerBuilder *builder);

/// \brief Function pointer type to call nvnSamplerBuilderSetMinMagFilter().
typedef void (NVNAPIENTRYP PFNNVNSAMPLERBUILDERSETMINMAGFILTERPROC) (NVNsamplerBuilder *builder, NVNminFilter min, NVNmagFilter mag);

/// \brief Function pointer type to call nvnSamplerBuilderSetWrapMode().
typedef void (NVNAPIENTRYP PFNNVNSAMPLERBUILDERSETWRAPMODEPROC) (NVNsamplerBuilder *builder, NVNwrapMode s, NVNwrapMode t, NVNwrapMode r);

/// \brief Function pointer type to call nvnSamplerBuilderSetLodClamp().
typedef void (NVNAPIENTRYP PFNNVNSAMPLERBUILDERSETLODCLAMPPROC) (NVNsamplerBuilder *builder, float min, float max);

/// \brief Function pointer type to call nvnSamplerBuilderSetLodBias().
typedef void (NVNAPIENTRYP PFNNVNSAMPLERBUILDERSETLODBIASPROC) (NVNsamplerBuilder *builder, float bias);

/// \brief Function pointer type to call nvnSamplerBuilderSetCompare().
typedef void (NVNAPIENTRYP PFNNVNSAMPLERBUILDERSETCOMPAREPROC) (NVNsamplerBuilder *builder, NVNcompareMode mode, NVNcompareFunc func);

/// \brief Function pointer type to call nvnSamplerBuilderSetBorderColor().
typedef void (NVNAPIENTRYP PFNNVNSAMPLERBUILDERSETBORDERCOLORPROC) (NVNsamplerBuilder *builder, const float *borderColor);

/// \brief Function pointer type to call nvnSamplerBuilderSetBorderColori().
typedef void (NVNAPIENTRYP PFNNVNSAMPLERBUILDERSETBORDERCOLORIPROC) (NVNsamplerBuilder *builder, const int *borderColor);

/// \brief Function pointer type to call nvnSamplerBuilderSetBorderColorui().
typedef void (NVNAPIENTRYP PFNNVNSAMPLERBUILDERSETBORDERCOLORUIPROC) (NVNsamplerBuilder *builder, const uint32_t *borderColor);

/// \brief Function pointer type to call nvnSamplerBuilderSetMaxAnisotropy().
typedef void (NVNAPIENTRYP PFNNVNSAMPLERBUILDERSETMAXANISOTROPYPROC) (NVNsamplerBuilder *builder, float maxAniso);

/// \brief Function pointer type to call nvnSamplerBuilderSetReductionFilter().
typedef void (NVNAPIENTRYP PFNNVNSAMPLERBUILDERSETREDUCTIONFILTERPROC) (NVNsamplerBuilder *builder, NVNsamplerReduction filter);

/// \brief Function pointer type to call nvnSamplerBuilderSetLodSnap().
typedef void (NVNAPIENTRYP PFNNVNSAMPLERBUILDERSETLODSNAPPROC) (NVNsamplerBuilder *builder, float lodSnap);

/// \brief Function pointer type to call nvnSamplerBuilderGetDevice().
typedef const NVNdevice * (NVNAPIENTRYP PFNNVNSAMPLERBUILDERGETDEVICEPROC) (const NVNsamplerBuilder *builder);

/// \brief Function pointer type to call nvnSamplerBuilderGetMinMagFilter().
typedef void (NVNAPIENTRYP PFNNVNSAMPLERBUILDERGETMINMAGFILTERPROC) (const NVNsamplerBuilder *builder, NVNminFilter *min, NVNmagFilter *mag);

/// \brief Function pointer type to call nvnSamplerBuilderGetWrapMode().
typedef void (NVNAPIENTRYP PFNNVNSAMPLERBUILDERGETWRAPMODEPROC) (const NVNsamplerBuilder *builder, NVNwrapMode *s, NVNwrapMode *t, NVNwrapMode *r);

/// \brief Function pointer type to call nvnSamplerBuilderGetLodClamp().
typedef void (NVNAPIENTRYP PFNNVNSAMPLERBUILDERGETLODCLAMPPROC) (const NVNsamplerBuilder *builder, float *min, float *max);

/// \brief Function pointer type to call nvnSamplerBuilderGetLodBias().
typedef float (NVNAPIENTRYP PFNNVNSAMPLERBUILDERGETLODBIASPROC) (const NVNsamplerBuilder *builder);

/// \brief Function pointer type to call nvnSamplerBuilderGetCompare().
typedef void (NVNAPIENTRYP PFNNVNSAMPLERBUILDERGETCOMPAREPROC) (const NVNsamplerBuilder *builder, NVNcompareMode *mode, NVNcompareFunc *func);

/// \brief Function pointer type to call nvnSamplerBuilderGetBorderColor().
typedef void (NVNAPIENTRYP PFNNVNSAMPLERBUILDERGETBORDERCOLORPROC) (const NVNsamplerBuilder *builder, float *borderColor);

/// \brief Function pointer type to call nvnSamplerBuilderGetBorderColori().
typedef void (NVNAPIENTRYP PFNNVNSAMPLERBUILDERGETBORDERCOLORIPROC) (const NVNsamplerBuilder *builder, int *borderColor);

/// \brief Function pointer type to call nvnSamplerBuilderGetBorderColorui().
typedef void (NVNAPIENTRYP PFNNVNSAMPLERBUILDERGETBORDERCOLORUIPROC) (const NVNsamplerBuilder *builder, uint32_t *borderColor);

/// \brief Function pointer type to call nvnSamplerBuilderGetMaxAnisotropy().
typedef float (NVNAPIENTRYP PFNNVNSAMPLERBUILDERGETMAXANISOTROPYPROC) (const NVNsamplerBuilder *builder);

/// \brief Function pointer type to call nvnSamplerBuilderGetReductionFilter().
typedef NVNsamplerReduction (NVNAPIENTRYP PFNNVNSAMPLERBUILDERGETREDUCTIONFILTERPROC) (const NVNsamplerBuilder *builder);

/// \brief Function pointer type to call nvnSamplerBuilderGetLodSnap().
typedef float (NVNAPIENTRYP PFNNVNSAMPLERBUILDERGETLODSNAPPROC) (const NVNsamplerBuilder *builder);

/// \brief Function pointer type to call nvnSamplerInitialize().
typedef NVNboolean (NVNAPIENTRYP PFNNVNSAMPLERINITIALIZEPROC) (NVNsampler *sampler, const NVNsamplerBuilder *builder);

/// \brief Function pointer type to call nvnSamplerFinalize().
typedef void (NVNAPIENTRYP PFNNVNSAMPLERFINALIZEPROC) (NVNsampler *sampler);

/// \brief Function pointer type to call nvnSamplerSetDebugLabel().
typedef void (NVNAPIENTRYP PFNNVNSAMPLERSETDEBUGLABELPROC) (NVNsampler *sampler, const char * label);

/// \brief Function pointer type to call nvnSamplerGetMinMagFilter().
typedef void (NVNAPIENTRYP PFNNVNSAMPLERGETMINMAGFILTERPROC) (const NVNsampler *sampler, NVNminFilter *min, NVNmagFilter *mag);

/// \brief Function pointer type to call nvnSamplerGetWrapMode().
typedef void (NVNAPIENTRYP PFNNVNSAMPLERGETWRAPMODEPROC) (const NVNsampler *sampler, NVNwrapMode *s, NVNwrapMode *t, NVNwrapMode *r);

/// \brief Function pointer type to call nvnSamplerGetLodClamp().
typedef void (NVNAPIENTRYP PFNNVNSAMPLERGETLODCLAMPPROC) (const NVNsampler *sampler, float *min, float *max);

/// \brief Function pointer type to call nvnSamplerGetLodBias().
typedef float (NVNAPIENTRYP PFNNVNSAMPLERGETLODBIASPROC) (const NVNsampler *sampler);

/// \brief Function pointer type to call nvnSamplerGetCompare().
typedef void (NVNAPIENTRYP PFNNVNSAMPLERGETCOMPAREPROC) (const NVNsampler *sampler, NVNcompareMode *mode, NVNcompareFunc *func);

/// \brief Function pointer type to call nvnSamplerGetBorderColor().
typedef void (NVNAPIENTRYP PFNNVNSAMPLERGETBORDERCOLORPROC) (const NVNsampler *sampler, float *borderColor);

/// \brief Function pointer type to call nvnSamplerGetBorderColori().
typedef void (NVNAPIENTRYP PFNNVNSAMPLERGETBORDERCOLORIPROC) (const NVNsampler *sampler, int *borderColor);

/// \brief Function pointer type to call nvnSamplerGetBorderColorui().
typedef void (NVNAPIENTRYP PFNNVNSAMPLERGETBORDERCOLORUIPROC) (const NVNsampler *sampler, uint32_t *borderColor);

/// \brief Function pointer type to call nvnSamplerGetMaxAnisotropy().
typedef float (NVNAPIENTRYP PFNNVNSAMPLERGETMAXANISOTROPYPROC) (const NVNsampler *sampler);

/// \brief Function pointer type to call nvnSamplerGetReductionFilter().
typedef NVNsamplerReduction (NVNAPIENTRYP PFNNVNSAMPLERGETREDUCTIONFILTERPROC) (const NVNsampler *sampler);

/// \brief Function pointer type to call nvnSamplerCompare().
typedef NVNboolean (NVNAPIENTRYP PFNNVNSAMPLERCOMPAREPROC) (const NVNsampler *sampler, const NVNsampler *otherSampler);

/// \brief Function pointer type to call nvnSamplerGetDebugID().
typedef uint64_t (NVNAPIENTRYP PFNNVNSAMPLERGETDEBUGIDPROC) (const NVNsampler *sampler);

/// \brief Function pointer type to call nvnBlendStateSetDefaults().
typedef void (NVNAPIENTRYP PFNNVNBLENDSTATESETDEFAULTSPROC) (NVNblendState *blend);

/// \brief Function pointer type to call nvnBlendStateSetBlendTarget().
typedef void (NVNAPIENTRYP PFNNVNBLENDSTATESETBLENDTARGETPROC) (NVNblendState *blend, int target);

/// \brief Function pointer type to call nvnBlendStateSetBlendFunc().
typedef void (NVNAPIENTRYP PFNNVNBLENDSTATESETBLENDFUNCPROC) (NVNblendState *blend, NVNblendFunc srcFunc, NVNblendFunc dstFunc, NVNblendFunc srcFuncAlpha, NVNblendFunc dstFuncAlpha);

/// \brief Function pointer type to call nvnBlendStateSetBlendEquation().
typedef void (NVNAPIENTRYP PFNNVNBLENDSTATESETBLENDEQUATIONPROC) (NVNblendState *blend, NVNblendEquation modeRGB, NVNblendEquation modeAlpha);

/// \brief Function pointer type to call nvnBlendStateSetAdvancedMode().
typedef void (NVNAPIENTRYP PFNNVNBLENDSTATESETADVANCEDMODEPROC) (NVNblendState *blend, NVNblendAdvancedMode overlap);

/// \brief Function pointer type to call nvnBlendStateSetAdvancedOverlap().
typedef void (NVNAPIENTRYP PFNNVNBLENDSTATESETADVANCEDOVERLAPPROC) (NVNblendState *blend, NVNblendAdvancedOverlap overlap);

/// \brief Function pointer type to call nvnBlendStateSetAdvancedPremultipliedSrc().
typedef void (NVNAPIENTRYP PFNNVNBLENDSTATESETADVANCEDPREMULTIPLIEDSRCPROC) (NVNblendState *blend, NVNboolean premultiplied);

/// \brief Function pointer type to call nvnBlendStateSetAdvancedNormalizedDst().
typedef void (NVNAPIENTRYP PFNNVNBLENDSTATESETADVANCEDNORMALIZEDDSTPROC) (NVNblendState *blend, NVNboolean normalized);

/// \brief Function pointer type to call nvnBlendStateGetBlendTarget().
typedef int (NVNAPIENTRYP PFNNVNBLENDSTATEGETBLENDTARGETPROC) (const NVNblendState *blend);

/// \brief Function pointer type to call nvnBlendStateGetBlendFunc().
typedef void (NVNAPIENTRYP PFNNVNBLENDSTATEGETBLENDFUNCPROC) (const NVNblendState *blend, NVNblendFunc *srcFunc, NVNblendFunc *dstFunc, NVNblendFunc *srcFuncAlpha, NVNblendFunc *dstFuncAlpha);

/// \brief Function pointer type to call nvnBlendStateGetBlendEquation().
typedef void (NVNAPIENTRYP PFNNVNBLENDSTATEGETBLENDEQUATIONPROC) (const NVNblendState *blend, NVNblendEquation *modeRGB, NVNblendEquation *modeAlpha);

/// \brief Function pointer type to call nvnBlendStateGetAdvancedMode().
typedef NVNblendAdvancedMode (NVNAPIENTRYP PFNNVNBLENDSTATEGETADVANCEDMODEPROC) (const NVNblendState *blend);

/// \brief Function pointer type to call nvnBlendStateGetAdvancedOverlap().
typedef NVNblendAdvancedOverlap (NVNAPIENTRYP PFNNVNBLENDSTATEGETADVANCEDOVERLAPPROC) (const NVNblendState *blend);

/// \brief Function pointer type to call nvnBlendStateGetAdvancedPremultipliedSrc().
typedef NVNboolean (NVNAPIENTRYP PFNNVNBLENDSTATEGETADVANCEDPREMULTIPLIEDSRCPROC) (const NVNblendState *blend);

/// \brief Function pointer type to call nvnBlendStateGetAdvancedNormalizedDst().
typedef NVNboolean (NVNAPIENTRYP PFNNVNBLENDSTATEGETADVANCEDNORMALIZEDDSTPROC) (const NVNblendState *blend);

/// \brief Function pointer type to call nvnColorStateSetDefaults().
typedef void (NVNAPIENTRYP PFNNVNCOLORSTATESETDEFAULTSPROC) (NVNcolorState *color);

/// \brief Function pointer type to call nvnColorStateSetBlendEnable().
typedef void (NVNAPIENTRYP PFNNVNCOLORSTATESETBLENDENABLEPROC) (NVNcolorState *color, int index, NVNboolean enable);

/// \brief Function pointer type to call nvnColorStateSetLogicOp().
typedef void (NVNAPIENTRYP PFNNVNCOLORSTATESETLOGICOPPROC) (NVNcolorState *color, NVNlogicOp logicOp);

/// \brief Function pointer type to call nvnColorStateSetAlphaTest().
typedef void (NVNAPIENTRYP PFNNVNCOLORSTATESETALPHATESTPROC) (NVNcolorState *color, NVNalphaFunc alphaTest);

/// \brief Function pointer type to call nvnColorStateGetBlendEnable().
typedef NVNboolean (NVNAPIENTRYP PFNNVNCOLORSTATEGETBLENDENABLEPROC) (const NVNcolorState *color, int index);

/// \brief Function pointer type to call nvnColorStateGetLogicOp().
typedef NVNlogicOp (NVNAPIENTRYP PFNNVNCOLORSTATEGETLOGICOPPROC) (const NVNcolorState *color);

/// \brief Function pointer type to call nvnColorStateGetAlphaTest().
typedef NVNalphaFunc (NVNAPIENTRYP PFNNVNCOLORSTATEGETALPHATESTPROC) (const NVNcolorState *color);

/// \brief Function pointer type to call nvnChannelMaskStateSetDefaults().
typedef void (NVNAPIENTRYP PFNNVNCHANNELMASKSTATESETDEFAULTSPROC) (NVNchannelMaskState *channelMask);

/// \brief Function pointer type to call nvnChannelMaskStateSetChannelMask().
typedef void (NVNAPIENTRYP PFNNVNCHANNELMASKSTATESETCHANNELMASKPROC) (NVNchannelMaskState *channelMask, int index, NVNboolean r, NVNboolean g, NVNboolean b, NVNboolean a);

/// \brief Function pointer type to call nvnChannelMaskStateGetChannelMask().
typedef void (NVNAPIENTRYP PFNNVNCHANNELMASKSTATEGETCHANNELMASKPROC) (const NVNchannelMaskState *channelMask, int index, NVNboolean *r, NVNboolean *g, NVNboolean *b, NVNboolean *a);

/// \brief Function pointer type to call nvnMultisampleStateSetDefaults().
typedef void (NVNAPIENTRYP PFNNVNMULTISAMPLESTATESETDEFAULTSPROC) (NVNmultisampleState *multisample);

/// \brief Function pointer type to call nvnMultisampleStateSetMultisampleEnable().
typedef void (NVNAPIENTRYP PFNNVNMULTISAMPLESTATESETMULTISAMPLEENABLEPROC) (NVNmultisampleState *multisample, NVNboolean enable);

/// \brief Function pointer type to call nvnMultisampleStateSetSamples().
typedef void (NVNAPIENTRYP PFNNVNMULTISAMPLESTATESETSAMPLESPROC) (NVNmultisampleState *multisample, int samples);

/// \brief Function pointer type to call nvnMultisampleStateSetAlphaToCoverageEnable().
typedef void (NVNAPIENTRYP PFNNVNMULTISAMPLESTATESETALPHATOCOVERAGEENABLEPROC) (NVNmultisampleState *multisample, NVNboolean enable);

/// \brief Function pointer type to call nvnMultisampleStateSetAlphaToCoverageDither().
typedef void (NVNAPIENTRYP PFNNVNMULTISAMPLESTATESETALPHATOCOVERAGEDITHERPROC) (NVNmultisampleState *multisample, NVNboolean dither);

/// \brief Function pointer type to call nvnMultisampleStateGetMultisampleEnable().
typedef NVNboolean (NVNAPIENTRYP PFNNVNMULTISAMPLESTATEGETMULTISAMPLEENABLEPROC) (const NVNmultisampleState *multisample);

/// \brief Function pointer type to call nvnMultisampleStateGetSamples().
typedef int (NVNAPIENTRYP PFNNVNMULTISAMPLESTATEGETSAMPLESPROC) (const NVNmultisampleState *multisample);

/// \brief Function pointer type to call nvnMultisampleStateGetAlphaToCoverageEnable().
typedef NVNboolean (NVNAPIENTRYP PFNNVNMULTISAMPLESTATEGETALPHATOCOVERAGEENABLEPROC) (const NVNmultisampleState *multisample);

/// \brief Function pointer type to call nvnMultisampleStateGetAlphaToCoverageDither().
typedef NVNboolean (NVNAPIENTRYP PFNNVNMULTISAMPLESTATEGETALPHATOCOVERAGEDITHERPROC) (const NVNmultisampleState *multisample);

/// \brief Function pointer type to call nvnMultisampleStateSetRasterSamples().
typedef void (NVNAPIENTRYP PFNNVNMULTISAMPLESTATESETRASTERSAMPLESPROC) (NVNmultisampleState *multisample, int rasterSamples);

/// \brief Function pointer type to call nvnMultisampleStateGetRasterSamples().
typedef int (NVNAPIENTRYP PFNNVNMULTISAMPLESTATEGETRASTERSAMPLESPROC) (NVNmultisampleState *multisample);

/// \brief Function pointer type to call nvnMultisampleStateSetCoverageModulationMode().
typedef void (NVNAPIENTRYP PFNNVNMULTISAMPLESTATESETCOVERAGEMODULATIONMODEPROC) (NVNmultisampleState *multisample, NVNcoverageModulationMode mode);

/// \brief Function pointer type to call nvnMultisampleStateGetCoverageModulationMode().
typedef NVNcoverageModulationMode (NVNAPIENTRYP PFNNVNMULTISAMPLESTATEGETCOVERAGEMODULATIONMODEPROC) (const NVNmultisampleState *multisample);

/// \brief Function pointer type to call nvnMultisampleStateSetCoverageToColorEnable().
typedef void (NVNAPIENTRYP PFNNVNMULTISAMPLESTATESETCOVERAGETOCOLORENABLEPROC) (NVNmultisampleState *multisample, NVNboolean enable);

/// \brief Function pointer type to call nvnMultisampleStateGetCoverageToColorEnable().
typedef NVNboolean (NVNAPIENTRYP PFNNVNMULTISAMPLESTATEGETCOVERAGETOCOLORENABLEPROC) (const NVNmultisampleState *multisample);

/// \brief Function pointer type to call nvnMultisampleStateSetCoverageToColorOutput().
typedef void (NVNAPIENTRYP PFNNVNMULTISAMPLESTATESETCOVERAGETOCOLOROUTPUTPROC) (NVNmultisampleState *multisample, int color);

/// \brief Function pointer type to call nvnMultisampleStateGetCoverageToColorOutput().
typedef int (NVNAPIENTRYP PFNNVNMULTISAMPLESTATEGETCOVERAGETOCOLOROUTPUTPROC) (const NVNmultisampleState *multisample);

/// \brief Function pointer type to call nvnMultisampleStateSetSampleLocationsEnable().
typedef void (NVNAPIENTRYP PFNNVNMULTISAMPLESTATESETSAMPLELOCATIONSENABLEPROC) (NVNmultisampleState *multisample, NVNboolean enable);

/// \brief Function pointer type to call nvnMultisampleStateGetSampleLocationsEnable().
typedef NVNboolean (NVNAPIENTRYP PFNNVNMULTISAMPLESTATEGETSAMPLELOCATIONSENABLEPROC) (const NVNmultisampleState *multisample);

/// \brief Function pointer type to call nvnMultisampleStateGetSampleLocationsGrid().
typedef void (NVNAPIENTRYP PFNNVNMULTISAMPLESTATEGETSAMPLELOCATIONSGRIDPROC) (NVNmultisampleState *multisample, int *width, int *height);

/// \brief Function pointer type to call nvnMultisampleStateSetSampleLocationsGridEnable().
typedef void (NVNAPIENTRYP PFNNVNMULTISAMPLESTATESETSAMPLELOCATIONSGRIDENABLEPROC) (NVNmultisampleState *multisample, NVNboolean enable);

/// \brief Function pointer type to call nvnMultisampleStateGetSampleLocationsGridEnable().
typedef NVNboolean (NVNAPIENTRYP PFNNVNMULTISAMPLESTATEGETSAMPLELOCATIONSGRIDENABLEPROC) (const NVNmultisampleState *multisample);

/// \brief Function pointer type to call nvnMultisampleStateSetSampleLocations().
typedef void (NVNAPIENTRYP PFNNVNMULTISAMPLESTATESETSAMPLELOCATIONSPROC) (NVNmultisampleState *multisample, int start, int count, const float *values);

/// \brief Function pointer type to call nvnPolygonStateSetDefaults().
typedef void (NVNAPIENTRYP PFNNVNPOLYGONSTATESETDEFAULTSPROC) (NVNpolygonState *polygon);

/// \brief Function pointer type to call nvnPolygonStateSetCullFace().
typedef void (NVNAPIENTRYP PFNNVNPOLYGONSTATESETCULLFACEPROC) (NVNpolygonState *polygon, NVNface face);

/// \brief Function pointer type to call nvnPolygonStateSetFrontFace().
typedef void (NVNAPIENTRYP PFNNVNPOLYGONSTATESETFRONTFACEPROC) (NVNpolygonState *polygon, NVNfrontFace face);

/// \brief Function pointer type to call nvnPolygonStateSetPolygonMode().
typedef void (NVNAPIENTRYP PFNNVNPOLYGONSTATESETPOLYGONMODEPROC) (NVNpolygonState *polygon, NVNpolygonMode polygonMode);

/// \brief Function pointer type to call nvnPolygonStateSetPolygonOffsetEnables().
typedef void (NVNAPIENTRYP PFNNVNPOLYGONSTATESETPOLYGONOFFSETENABLESPROC) (NVNpolygonState *polygon, int enables);

/// \brief Function pointer type to call nvnPolygonStateGetCullFace().
typedef NVNface (NVNAPIENTRYP PFNNVNPOLYGONSTATEGETCULLFACEPROC) (const NVNpolygonState *polygon);

/// \brief Function pointer type to call nvnPolygonStateGetFrontFace().
typedef NVNfrontFace (NVNAPIENTRYP PFNNVNPOLYGONSTATEGETFRONTFACEPROC) (const NVNpolygonState *polygon);

/// \brief Function pointer type to call nvnPolygonStateGetPolygonMode().
typedef NVNpolygonMode (NVNAPIENTRYP PFNNVNPOLYGONSTATEGETPOLYGONMODEPROC) (const NVNpolygonState *polygon);

/// \brief Function pointer type to call nvnPolygonStateGetPolygonOffsetEnables().
typedef NVNpolygonOffsetEnable (NVNAPIENTRYP PFNNVNPOLYGONSTATEGETPOLYGONOFFSETENABLESPROC) (const NVNpolygonState *polygon);

/// \brief Function pointer type to call nvnDepthStencilStateSetDefaults().
typedef void (NVNAPIENTRYP PFNNVNDEPTHSTENCILSTATESETDEFAULTSPROC) (NVNdepthStencilState *depthStencil);

/// \brief Function pointer type to call nvnDepthStencilStateSetDepthTestEnable().
typedef void (NVNAPIENTRYP PFNNVNDEPTHSTENCILSTATESETDEPTHTESTENABLEPROC) (NVNdepthStencilState *depthStencil, NVNboolean enable);

/// \brief Function pointer type to call nvnDepthStencilStateSetDepthWriteEnable().
typedef void (NVNAPIENTRYP PFNNVNDEPTHSTENCILSTATESETDEPTHWRITEENABLEPROC) (NVNdepthStencilState *depthStencil, NVNboolean enable);

/// \brief Function pointer type to call nvnDepthStencilStateSetDepthFunc().
typedef void (NVNAPIENTRYP PFNNVNDEPTHSTENCILSTATESETDEPTHFUNCPROC) (NVNdepthStencilState *depthStencil, NVNdepthFunc func);

/// \brief Function pointer type to call nvnDepthStencilStateSetStencilTestEnable().
typedef void (NVNAPIENTRYP PFNNVNDEPTHSTENCILSTATESETSTENCILTESTENABLEPROC) (NVNdepthStencilState *depthStencil, NVNboolean enable);

/// \brief Function pointer type to call nvnDepthStencilStateSetStencilFunc().
typedef void (NVNAPIENTRYP PFNNVNDEPTHSTENCILSTATESETSTENCILFUNCPROC) (NVNdepthStencilState *depthStencil, NVNface faces, NVNstencilFunc func);

/// \brief Function pointer type to call nvnDepthStencilStateSetStencilOp().
typedef void (NVNAPIENTRYP PFNNVNDEPTHSTENCILSTATESETSTENCILOPPROC) (NVNdepthStencilState *depthStencil, NVNface faces, NVNstencilOp fail, NVNstencilOp depthFail, NVNstencilOp depthPass);

/// \brief Function pointer type to call nvnDepthStencilStateGetDepthTestEnable().
typedef NVNboolean (NVNAPIENTRYP PFNNVNDEPTHSTENCILSTATEGETDEPTHTESTENABLEPROC) (const NVNdepthStencilState *depthStencil);

/// \brief Function pointer type to call nvnDepthStencilStateGetDepthWriteEnable().
typedef NVNboolean (NVNAPIENTRYP PFNNVNDEPTHSTENCILSTATEGETDEPTHWRITEENABLEPROC) (const NVNdepthStencilState *depthStencil);

/// \brief Function pointer type to call nvnDepthStencilStateGetDepthFunc().
typedef NVNdepthFunc (NVNAPIENTRYP PFNNVNDEPTHSTENCILSTATEGETDEPTHFUNCPROC) (const NVNdepthStencilState *depthStencil);

/// \brief Function pointer type to call nvnDepthStencilStateGetStencilTestEnable().
typedef NVNboolean (NVNAPIENTRYP PFNNVNDEPTHSTENCILSTATEGETSTENCILTESTENABLEPROC) (const NVNdepthStencilState *depthStencil);

/// \brief Function pointer type to call nvnDepthStencilStateGetStencilFunc().
typedef NVNstencilFunc (NVNAPIENTRYP PFNNVNDEPTHSTENCILSTATEGETSTENCILFUNCPROC) (const NVNdepthStencilState *depthStencil, NVNface face);

/// \brief Function pointer type to call nvnDepthStencilStateGetStencilOp().
typedef void (NVNAPIENTRYP PFNNVNDEPTHSTENCILSTATEGETSTENCILOPPROC) (const NVNdepthStencilState *depthStencil, NVNface face, NVNstencilOp *fail, NVNstencilOp *depthFail, NVNstencilOp *depthPass);

/// \brief Function pointer type to call nvnVertexAttribStateSetDefaults().
typedef void (NVNAPIENTRYP PFNNVNVERTEXATTRIBSTATESETDEFAULTSPROC) (NVNvertexAttribState *attrib);

/// \brief Function pointer type to call nvnVertexAttribStateSetFormat().
typedef void (NVNAPIENTRYP PFNNVNVERTEXATTRIBSTATESETFORMATPROC) (NVNvertexAttribState *attrib, NVNformat format, ptrdiff_t relativeOffset);

/// \brief Function pointer type to call nvnVertexAttribStateSetStreamIndex().
typedef void (NVNAPIENTRYP PFNNVNVERTEXATTRIBSTATESETSTREAMINDEXPROC) (NVNvertexAttribState *attrib, int streamIndex);

/// \brief Function pointer type to call nvnVertexAttribStateGetFormat().
typedef void (NVNAPIENTRYP PFNNVNVERTEXATTRIBSTATEGETFORMATPROC) (const NVNvertexAttribState *attrib, NVNformat *format, ptrdiff_t *relativeOffset);

/// \brief Function pointer type to call nvnVertexAttribStateGetStreamIndex().
typedef int (NVNAPIENTRYP PFNNVNVERTEXATTRIBSTATEGETSTREAMINDEXPROC) (const NVNvertexAttribState *attrib);

/// \brief Function pointer type to call nvnVertexStreamStateSetDefaults().
typedef void (NVNAPIENTRYP PFNNVNVERTEXSTREAMSTATESETDEFAULTSPROC) (NVNvertexStreamState *stream);

/// \brief Function pointer type to call nvnVertexStreamStateSetStride().
typedef void (NVNAPIENTRYP PFNNVNVERTEXSTREAMSTATESETSTRIDEPROC) (NVNvertexStreamState *stream, ptrdiff_t stride);

/// \brief Function pointer type to call nvnVertexStreamStateSetDivisor().
typedef void (NVNAPIENTRYP PFNNVNVERTEXSTREAMSTATESETDIVISORPROC) (NVNvertexStreamState *stream, int divisor);

/// \brief Function pointer type to call nvnVertexStreamStateGetStride().
typedef ptrdiff_t (NVNAPIENTRYP PFNNVNVERTEXSTREAMSTATEGETSTRIDEPROC) (const NVNvertexStreamState *stream);

/// \brief Function pointer type to call nvnVertexStreamStateGetDivisor().
typedef int (NVNAPIENTRYP PFNNVNVERTEXSTREAMSTATEGETDIVISORPROC) (const NVNvertexStreamState *stream);

/// \brief Function pointer type to call nvnCommandBufferInitialize().
typedef NVNboolean (NVNAPIENTRYP PFNNVNCOMMANDBUFFERINITIALIZEPROC) (NVNcommandBuffer *cmdBuf, NVNdevice *device);

/// \brief Function pointer type to call nvnCommandBufferFinalize().
typedef void (NVNAPIENTRYP PFNNVNCOMMANDBUFFERFINALIZEPROC) (NVNcommandBuffer *cmdBuf);

/// \brief Function pointer type to call nvnCommandBufferSetDebugLabel().
typedef void (NVNAPIENTRYP PFNNVNCOMMANDBUFFERSETDEBUGLABELPROC) (NVNcommandBuffer *cmdBuf, const char * label);

/// \brief Function pointer type to call nvnCommandBufferSetMemoryCallback().
typedef void (NVNAPIENTRYP PFNNVNCOMMANDBUFFERSETMEMORYCALLBACKPROC) (NVNcommandBuffer *cmdBuf, PFNNVNCOMMANDBUFFERMEMORYCALLBACKPROC callback);

/// \brief Function pointer type to call nvnCommandBufferSetMemoryCallbackData().
typedef void (NVNAPIENTRYP PFNNVNCOMMANDBUFFERSETMEMORYCALLBACKDATAPROC) (NVNcommandBuffer *cmdBuf, void *callbackData);

/// \brief Function pointer type to call nvnCommandBufferSetCommandMemoryCallbackEnabled().
typedef void (NVNAPIENTRYP PFNNVNCOMMANDBUFFERSETCOMMANDMEMORYCALLBACKENABLEDPROC) (NVNcommandBuffer *cmdBuf, NVNboolean enabled);

/// \brief Function pointer type to call nvnCommandBufferAddCommandMemory().
typedef void (NVNAPIENTRYP PFNNVNCOMMANDBUFFERADDCOMMANDMEMORYPROC) (NVNcommandBuffer *cmdBuf, const NVNmemoryPool *pool, ptrdiff_t offset, size_t size);

/// \brief Function pointer type to call nvnCommandBufferAddControlMemory().
typedef void (NVNAPIENTRYP PFNNVNCOMMANDBUFFERADDCONTROLMEMORYPROC) (NVNcommandBuffer *cmdBuf, void *memory, size_t size);

/// \brief Function pointer type to call nvnCommandBufferGetCommandMemorySize().
typedef size_t (NVNAPIENTRYP PFNNVNCOMMANDBUFFERGETCOMMANDMEMORYSIZEPROC) (const NVNcommandBuffer *cmdBuf);

/// \brief Function pointer type to call nvnCommandBufferGetCommandMemoryUsed().
typedef size_t (NVNAPIENTRYP PFNNVNCOMMANDBUFFERGETCOMMANDMEMORYUSEDPROC) (const NVNcommandBuffer *cmdBuf);

/// \brief Function pointer type to call nvnCommandBufferGetCommandMemoryFree().
typedef size_t (NVNAPIENTRYP PFNNVNCOMMANDBUFFERGETCOMMANDMEMORYFREEPROC) (const NVNcommandBuffer *cmdBuf);

/// \brief Function pointer type to call nvnCommandBufferGetControlMemorySize().
typedef size_t (NVNAPIENTRYP PFNNVNCOMMANDBUFFERGETCONTROLMEMORYSIZEPROC) (const NVNcommandBuffer *cmdBuf);

/// \brief Function pointer type to call nvnCommandBufferGetControlMemoryUsed().
typedef size_t (NVNAPIENTRYP PFNNVNCOMMANDBUFFERGETCONTROLMEMORYUSEDPROC) (const NVNcommandBuffer *cmdBuf);

/// \brief Function pointer type to call nvnCommandBufferGetControlMemoryFree().
typedef size_t (NVNAPIENTRYP PFNNVNCOMMANDBUFFERGETCONTROLMEMORYFREEPROC) (const NVNcommandBuffer *cmdBuf);

/// \brief Function pointer type to call nvnCommandBufferBeginRecording().
typedef void (NVNAPIENTRYP PFNNVNCOMMANDBUFFERBEGINRECORDINGPROC) (NVNcommandBuffer *cmdBuf);

/// \brief Function pointer type to call nvnCommandBufferEndRecording().
typedef NVNcommandHandle (NVNAPIENTRYP PFNNVNCOMMANDBUFFERENDRECORDINGPROC) (NVNcommandBuffer *cmdBuf);

/// \brief Function pointer type to call nvnCommandBufferCallCommands().
typedef void (NVNAPIENTRYP PFNNVNCOMMANDBUFFERCALLCOMMANDSPROC) (NVNcommandBuffer *cmdBuf, int numCommands, const NVNcommandHandle *handles);

/// \brief Function pointer type to call nvnCommandBufferCopyCommands().
typedef void (NVNAPIENTRYP PFNNVNCOMMANDBUFFERCOPYCOMMANDSPROC) (NVNcommandBuffer *cmdBuf, int numCommands, const NVNcommandHandle *handles);

/// \brief Function pointer type to call nvnCommandBufferBindBlendState().
typedef void (NVNAPIENTRYP PFNNVNCOMMANDBUFFERBINDBLENDSTATEPROC) (NVNcommandBuffer *cmdBuf, const NVNblendState *blend);

/// \brief Function pointer type to call nvnCommandBufferBindChannelMaskState().
typedef void (NVNAPIENTRYP PFNNVNCOMMANDBUFFERBINDCHANNELMASKSTATEPROC) (NVNcommandBuffer *cmdBuf, const NVNchannelMaskState *channelMask);

/// \brief Function pointer type to call nvnCommandBufferBindColorState().
typedef void (NVNAPIENTRYP PFNNVNCOMMANDBUFFERBINDCOLORSTATEPROC) (NVNcommandBuffer *cmdBuf, const NVNcolorState *color);

/// \brief Function pointer type to call nvnCommandBufferBindMultisampleState().
typedef void (NVNAPIENTRYP PFNNVNCOMMANDBUFFERBINDMULTISAMPLESTATEPROC) (NVNcommandBuffer *cmdBuf, const NVNmultisampleState *multisample);

/// \brief Function pointer type to call nvnCommandBufferBindPolygonState().
typedef void (NVNAPIENTRYP PFNNVNCOMMANDBUFFERBINDPOLYGONSTATEPROC) (NVNcommandBuffer *cmdBuf, const NVNpolygonState *polygon);

/// \brief Function pointer type to call nvnCommandBufferBindDepthStencilState().
typedef void (NVNAPIENTRYP PFNNVNCOMMANDBUFFERBINDDEPTHSTENCILSTATEPROC) (NVNcommandBuffer *cmdBuf, const NVNdepthStencilState *depthStencil);

/// \brief Function pointer type to call nvnCommandBufferBindVertexAttribState().
typedef void (NVNAPIENTRYP PFNNVNCOMMANDBUFFERBINDVERTEXATTRIBSTATEPROC) (NVNcommandBuffer *cmdBuf, int numAttribs, const NVNvertexAttribState *attribs);

/// \brief Function pointer type to call nvnCommandBufferBindVertexStreamState().
typedef void (NVNAPIENTRYP PFNNVNCOMMANDBUFFERBINDVERTEXSTREAMSTATEPROC) (NVNcommandBuffer *cmdBuf, int numStreams, const NVNvertexStreamState *streams);

/// \brief Function pointer type to call nvnCommandBufferBindProgram().
typedef void (NVNAPIENTRYP PFNNVNCOMMANDBUFFERBINDPROGRAMPROC) (NVNcommandBuffer *cmdBuf, const NVNprogram *program, int stages);

/// \brief Function pointer type to call nvnCommandBufferBindVertexBuffer().
typedef void (NVNAPIENTRYP PFNNVNCOMMANDBUFFERBINDVERTEXBUFFERPROC) (NVNcommandBuffer *cmdBuf, int index, NVNbufferAddress buffer, size_t size);

/// \brief Function pointer type to call nvnCommandBufferBindVertexBuffers().
typedef void (NVNAPIENTRYP PFNNVNCOMMANDBUFFERBINDVERTEXBUFFERSPROC) (NVNcommandBuffer *cmdBuf, int first, int count, const NVNbufferRange *buffers);

/// \brief Function pointer type to call nvnCommandBufferBindUniformBuffer().
typedef void (NVNAPIENTRYP PFNNVNCOMMANDBUFFERBINDUNIFORMBUFFERPROC) (NVNcommandBuffer *cmdBuf, NVNshaderStage stage, int index, NVNbufferAddress buffer, size_t size);

/// \brief Function pointer type to call nvnCommandBufferBindUniformBuffers().
typedef void (NVNAPIENTRYP PFNNVNCOMMANDBUFFERBINDUNIFORMBUFFERSPROC) (NVNcommandBuffer *cmdBuf, NVNshaderStage stage, int first, int count, const NVNbufferRange *buffers);

/// \brief Function pointer type to call nvnCommandBufferBindTransformFeedbackBuffer().
typedef void (NVNAPIENTRYP PFNNVNCOMMANDBUFFERBINDTRANSFORMFEEDBACKBUFFERPROC) (NVNcommandBuffer *cmdBuf, int index, NVNbufferAddress buffer, size_t size);

/// \brief Function pointer type to call nvnCommandBufferBindTransformFeedbackBuffers().
typedef void (NVNAPIENTRYP PFNNVNCOMMANDBUFFERBINDTRANSFORMFEEDBACKBUFFERSPROC) (NVNcommandBuffer *cmdBuf, int first, int count, const NVNbufferRange *buffers);

/// \brief Function pointer type to call nvnCommandBufferBindStorageBuffer().
typedef void (NVNAPIENTRYP PFNNVNCOMMANDBUFFERBINDSTORAGEBUFFERPROC) (NVNcommandBuffer *cmdBuf, NVNshaderStage stage, int index, NVNbufferAddress buffer, size_t size);

/// \brief Function pointer type to call nvnCommandBufferBindStorageBuffers().
typedef void (NVNAPIENTRYP PFNNVNCOMMANDBUFFERBINDSTORAGEBUFFERSPROC) (NVNcommandBuffer *cmdBuf, NVNshaderStage stage, int first, int count, const NVNbufferRange *buffers);

/// \brief Function pointer type to call nvnCommandBufferBindTexture().
typedef void (NVNAPIENTRYP PFNNVNCOMMANDBUFFERBINDTEXTUREPROC) (NVNcommandBuffer *cmdBuf, NVNshaderStage stage, int index, NVNtextureHandle texture);

/// \brief Function pointer type to call nvnCommandBufferBindTextures().
typedef void (NVNAPIENTRYP PFNNVNCOMMANDBUFFERBINDTEXTURESPROC) (NVNcommandBuffer *cmdBuf, NVNshaderStage stage, int first, int count, const NVNtextureHandle *textures);

/// \brief Function pointer type to call nvnCommandBufferBindImage().
typedef void (NVNAPIENTRYP PFNNVNCOMMANDBUFFERBINDIMAGEPROC) (NVNcommandBuffer *cmdBuf, NVNshaderStage stage, int index, NVNimageHandle image);

/// \brief Function pointer type to call nvnCommandBufferBindImages().
typedef void (NVNAPIENTRYP PFNNVNCOMMANDBUFFERBINDIMAGESPROC) (NVNcommandBuffer *cmdBuf, NVNshaderStage stage, int first, int count, const NVNimageHandle *images);

/// \brief Function pointer type to call nvnCommandBufferSetPatchSize().
typedef void (NVNAPIENTRYP PFNNVNCOMMANDBUFFERSETPATCHSIZEPROC) (NVNcommandBuffer *cmdBuf, int size);

/// \brief Function pointer type to call nvnCommandBufferSetInnerTessellationLevels().
typedef void (NVNAPIENTRYP PFNNVNCOMMANDBUFFERSETINNERTESSELLATIONLEVELSPROC) (NVNcommandBuffer *cmdBuf, const float *levels);

/// \brief Function pointer type to call nvnCommandBufferSetOuterTessellationLevels().
typedef void (NVNAPIENTRYP PFNNVNCOMMANDBUFFERSETOUTERTESSELLATIONLEVELSPROC) (NVNcommandBuffer *cmdBuf, const float *levels);

/// \brief Function pointer type to call nvnCommandBufferSetPrimitiveRestart().
typedef void (NVNAPIENTRYP PFNNVNCOMMANDBUFFERSETPRIMITIVERESTARTPROC) (NVNcommandBuffer *cmdBuf, NVNboolean enable, int index);

/// \brief Function pointer type to call nvnCommandBufferBeginTransformFeedback().
typedef void (NVNAPIENTRYP PFNNVNCOMMANDBUFFERBEGINTRANSFORMFEEDBACKPROC) (NVNcommandBuffer *cmdBuf, NVNbufferAddress buffer);

/// \brief Function pointer type to call nvnCommandBufferEndTransformFeedback().
typedef void (NVNAPIENTRYP PFNNVNCOMMANDBUFFERENDTRANSFORMFEEDBACKPROC) (NVNcommandBuffer *cmdBuf, NVNbufferAddress buffer);

/// \brief Function pointer type to call nvnCommandBufferPauseTransformFeedback().
typedef void (NVNAPIENTRYP PFNNVNCOMMANDBUFFERPAUSETRANSFORMFEEDBACKPROC) (NVNcommandBuffer *cmdBuf, NVNbufferAddress buffer);

/// \brief Function pointer type to call nvnCommandBufferResumeTransformFeedback().
typedef void (NVNAPIENTRYP PFNNVNCOMMANDBUFFERRESUMETRANSFORMFEEDBACKPROC) (NVNcommandBuffer *cmdBuf, NVNbufferAddress buffer);

/// \brief Function pointer type to call nvnCommandBufferDrawTransformFeedback().
typedef void (NVNAPIENTRYP PFNNVNCOMMANDBUFFERDRAWTRANSFORMFEEDBACKPROC) (NVNcommandBuffer *cmdBuf, NVNdrawPrimitive mode, NVNbufferAddress buffer);

/// \brief Function pointer type to call nvnCommandBufferDrawArrays().
typedef void (NVNAPIENTRYP PFNNVNCOMMANDBUFFERDRAWARRAYSPROC) (NVNcommandBuffer *cmdBuf, NVNdrawPrimitive mode, int first, int count);

/// \brief Function pointer type to call nvnCommandBufferDrawElements().
typedef void (NVNAPIENTRYP PFNNVNCOMMANDBUFFERDRAWELEMENTSPROC) (NVNcommandBuffer *cmdBuf, NVNdrawPrimitive mode, NVNindexType type, int count, NVNbufferAddress indexBuffer);

/// \brief Function pointer type to call nvnCommandBufferDrawElementsBaseVertex().
typedef void (NVNAPIENTRYP PFNNVNCOMMANDBUFFERDRAWELEMENTSBASEVERTEXPROC) (NVNcommandBuffer *cmdBuf, NVNdrawPrimitive mode, NVNindexType type, int count, NVNbufferAddress indexBuffer, int baseVertex);

/// \brief Function pointer type to call nvnCommandBufferDrawArraysInstanced().
typedef void (NVNAPIENTRYP PFNNVNCOMMANDBUFFERDRAWARRAYSINSTANCEDPROC) (NVNcommandBuffer *cmdBuf, NVNdrawPrimitive mode, int first, int count, int baseInstance, int instanceCount);

/// \brief Function pointer type to call nvnCommandBufferDrawElementsInstanced().
typedef void (NVNAPIENTRYP PFNNVNCOMMANDBUFFERDRAWELEMENTSINSTANCEDPROC) (NVNcommandBuffer *cmdBuf, NVNdrawPrimitive mode, NVNindexType type, int count, NVNbufferAddress indexBuffer, int baseVertex, int baseInstance, int instanceCount);

/// \brief Function pointer type to call nvnCommandBufferDrawArraysIndirect().
typedef void (NVNAPIENTRYP PFNNVNCOMMANDBUFFERDRAWARRAYSINDIRECTPROC) (NVNcommandBuffer *cmdBuf, NVNdrawPrimitive mode, NVNbufferAddress indirectBuffer);

/// \brief Function pointer type to call nvnCommandBufferDrawElementsIndirect().
typedef void (NVNAPIENTRYP PFNNVNCOMMANDBUFFERDRAWELEMENTSINDIRECTPROC) (NVNcommandBuffer *cmdBuf, NVNdrawPrimitive mode, NVNindexType type, NVNbufferAddress indexBuffer, NVNbufferAddress indirectBuffer);

/// \brief Function pointer type to call nvnCommandBufferMultiDrawArraysIndirectCount().
typedef void (NVNAPIENTRYP PFNNVNCOMMANDBUFFERMULTIDRAWARRAYSINDIRECTCOUNTPROC) (NVNcommandBuffer *cmdBuf, NVNdrawPrimitive mode, NVNbufferAddress indirectBuffer, NVNbufferAddress parameterBuffer, int maxDrawCount, ptrdiff_t stride);

/// \brief Function pointer type to call nvnCommandBufferMultiDrawElementsIndirectCount().
typedef void (NVNAPIENTRYP PFNNVNCOMMANDBUFFERMULTIDRAWELEMENTSINDIRECTCOUNTPROC) (NVNcommandBuffer *cmdBuf, NVNdrawPrimitive mode, NVNindexType type, NVNbufferAddress indexBuffer, NVNbufferAddress indirectBuffer, NVNbufferAddress parameterBuffer, int maxDrawCount, ptrdiff_t stride);

/// \brief Function pointer type to call nvnCommandBufferClearColor().
typedef void (NVNAPIENTRYP PFNNVNCOMMANDBUFFERCLEARCOLORPROC) (NVNcommandBuffer *cmdBuf, int index, const float *color, int mask);

/// \brief Function pointer type to call nvnCommandBufferClearColori().
typedef void (NVNAPIENTRYP PFNNVNCOMMANDBUFFERCLEARCOLORIPROC) (NVNcommandBuffer *cmdBuf, int index, const int *color, int mask);

/// \brief Function pointer type to call nvnCommandBufferClearColorui().
typedef void (NVNAPIENTRYP PFNNVNCOMMANDBUFFERCLEARCOLORUIPROC) (NVNcommandBuffer *cmdBuf, int index, const uint32_t *color, int mask);

/// \brief Function pointer type to call nvnCommandBufferClearDepthStencil().
typedef void (NVNAPIENTRYP PFNNVNCOMMANDBUFFERCLEARDEPTHSTENCILPROC) (NVNcommandBuffer *cmdBuf, float depthValue, NVNboolean depthMask, int stencilValue, int stencilMask);

/// \brief Function pointer type to call nvnCommandBufferDispatchCompute().
typedef void (NVNAPIENTRYP PFNNVNCOMMANDBUFFERDISPATCHCOMPUTEPROC) (NVNcommandBuffer *cmdBuf, int groupsX, int groupsY, int groupsZ);

/// \brief Function pointer type to call nvnCommandBufferDispatchComputeIndirect().
typedef void (NVNAPIENTRYP PFNNVNCOMMANDBUFFERDISPATCHCOMPUTEINDIRECTPROC) (NVNcommandBuffer *cmdBuf, NVNbufferAddress indirectBuffer);

/// \brief Function pointer type to call nvnCommandBufferSetViewport().
typedef void (NVNAPIENTRYP PFNNVNCOMMANDBUFFERSETVIEWPORTPROC) (NVNcommandBuffer *cmdBuf, int x, int y, int w, int h);

/// \brief Function pointer type to call nvnCommandBufferSetViewports().
typedef void (NVNAPIENTRYP PFNNVNCOMMANDBUFFERSETVIEWPORTSPROC) (NVNcommandBuffer *cmdBuf, int first, int count, const float *ranges);

/// \brief Function pointer type to call nvnCommandBufferSetViewportSwizzles().
typedef void (NVNAPIENTRYP PFNNVNCOMMANDBUFFERSETVIEWPORTSWIZZLESPROC) (NVNcommandBuffer *cmdBuf, int first, int count, const NVNviewportSwizzle *swizzles);

/// \brief Function pointer type to call nvnCommandBufferSetScissor().
typedef void (NVNAPIENTRYP PFNNVNCOMMANDBUFFERSETSCISSORPROC) (NVNcommandBuffer *cmdBuf, int x, int y, int w, int h);

/// \brief Function pointer type to call nvnCommandBufferSetScissors().
typedef void (NVNAPIENTRYP PFNNVNCOMMANDBUFFERSETSCISSORSPROC) (NVNcommandBuffer *cmdBuf, int first, int count, const int *rects);

/// \brief Function pointer type to call nvnCommandBufferSetDepthRange().
typedef void (NVNAPIENTRYP PFNNVNCOMMANDBUFFERSETDEPTHRANGEPROC) (NVNcommandBuffer *cmdBuf, float n, float f);

/// \brief Function pointer type to call nvnCommandBufferSetDepthBounds().
typedef void (NVNAPIENTRYP PFNNVNCOMMANDBUFFERSETDEPTHBOUNDSPROC) (NVNcommandBuffer *cmdBuf, NVNboolean enable, float n, float f);

/// \brief Function pointer type to call nvnCommandBufferSetDepthRanges().
typedef void (NVNAPIENTRYP PFNNVNCOMMANDBUFFERSETDEPTHRANGESPROC) (NVNcommandBuffer *cmdBuf, int first, int count, const float *ranges);

/// \brief Function pointer type to call nvnCommandBufferSetTiledCacheAction().
typedef void (NVNAPIENTRYP PFNNVNCOMMANDBUFFERSETTILEDCACHEACTIONPROC) (NVNcommandBuffer *cmdBuf, NVNtiledCacheAction action);

/// \brief Function pointer type to call nvnCommandBufferSetTiledCacheTileSize().
typedef void (NVNAPIENTRYP PFNNVNCOMMANDBUFFERSETTILEDCACHETILESIZEPROC) (NVNcommandBuffer *cmdBuf, int tileWidth, int tileHeight);

/// \brief Function pointer type to call nvnCommandBufferBindSeparateTexture().
typedef void (NVNAPIENTRYP PFNNVNCOMMANDBUFFERBINDSEPARATETEXTUREPROC) (NVNcommandBuffer *cmdBuf, NVNshaderStage stage, int index, NVNseparateTextureHandle textureHandle);

/// \brief Function pointer type to call nvnCommandBufferBindSeparateSampler().
typedef void (NVNAPIENTRYP PFNNVNCOMMANDBUFFERBINDSEPARATESAMPLERPROC) (NVNcommandBuffer *cmdBuf, NVNshaderStage stage, int index, NVNseparateSamplerHandle samplerHandle);

/// \brief Function pointer type to call nvnCommandBufferBindSeparateTextures().
typedef void (NVNAPIENTRYP PFNNVNCOMMANDBUFFERBINDSEPARATETEXTURESPROC) (NVNcommandBuffer *cmdBuf, NVNshaderStage stage, int first, int count, const NVNseparateTextureHandle *textureHandles);

/// \brief Function pointer type to call nvnCommandBufferBindSeparateSamplers().
typedef void (NVNAPIENTRYP PFNNVNCOMMANDBUFFERBINDSEPARATESAMPLERSPROC) (NVNcommandBuffer *cmdBuf, NVNshaderStage stage, int first, int count, const NVNseparateSamplerHandle *samplerHandles);

/// \brief Function pointer type to call nvnCommandBufferSetStencilValueMask().
typedef void (NVNAPIENTRYP PFNNVNCOMMANDBUFFERSETSTENCILVALUEMASKPROC) (NVNcommandBuffer *cmdBuf, NVNface faces, int mask);

/// \brief Function pointer type to call nvnCommandBufferSetStencilMask().
typedef void (NVNAPIENTRYP PFNNVNCOMMANDBUFFERSETSTENCILMASKPROC) (NVNcommandBuffer *cmdBuf, NVNface faces, int mask);

/// \brief Function pointer type to call nvnCommandBufferSetStencilRef().
typedef void (NVNAPIENTRYP PFNNVNCOMMANDBUFFERSETSTENCILREFPROC) (NVNcommandBuffer *cmdBuf, NVNface faces, int ref);

/// \brief Function pointer type to call nvnCommandBufferSetBlendColor().
typedef void (NVNAPIENTRYP PFNNVNCOMMANDBUFFERSETBLENDCOLORPROC) (NVNcommandBuffer *cmdBuf, const float *blendColor);

/// \brief Function pointer type to call nvnCommandBufferSetPointSize().
typedef void (NVNAPIENTRYP PFNNVNCOMMANDBUFFERSETPOINTSIZEPROC) (NVNcommandBuffer *cmdBuf, float pointSize);

/// \brief Function pointer type to call nvnCommandBufferSetLineWidth().
typedef void (NVNAPIENTRYP PFNNVNCOMMANDBUFFERSETLINEWIDTHPROC) (NVNcommandBuffer *cmdBuf, float lineWidth);

/// \brief Function pointer type to call nvnCommandBufferSetPolygonOffsetClamp().
typedef void (NVNAPIENTRYP PFNNVNCOMMANDBUFFERSETPOLYGONOFFSETCLAMPPROC) (NVNcommandBuffer *cmdBuf, float factor, float units, float clamp);

/// \brief Function pointer type to call nvnCommandBufferSetAlphaRef().
typedef void (NVNAPIENTRYP PFNNVNCOMMANDBUFFERSETALPHAREFPROC) (NVNcommandBuffer *cmdBuf, float ref);

/// \brief Function pointer type to call nvnCommandBufferSetSampleMask().
typedef void (NVNAPIENTRYP PFNNVNCOMMANDBUFFERSETSAMPLEMASKPROC) (NVNcommandBuffer *cmdBuf, int mask);

/// \brief Function pointer type to call nvnCommandBufferSetRasterizerDiscard().
typedef void (NVNAPIENTRYP PFNNVNCOMMANDBUFFERSETRASTERIZERDISCARDPROC) (NVNcommandBuffer *cmdBuf, NVNboolean discard);

/// \brief Function pointer type to call nvnCommandBufferSetDepthClamp().
typedef void (NVNAPIENTRYP PFNNVNCOMMANDBUFFERSETDEPTHCLAMPPROC) (NVNcommandBuffer *cmdBuf, NVNboolean clamp);

/// \brief Function pointer type to call nvnCommandBufferSetConservativeRasterEnable().
typedef void (NVNAPIENTRYP PFNNVNCOMMANDBUFFERSETCONSERVATIVERASTERENABLEPROC) (NVNcommandBuffer *cmdBuf, NVNboolean enable);

/// \brief Function pointer type to call nvnCommandBufferSetConservativeRasterDilate().
typedef void (NVNAPIENTRYP PFNNVNCOMMANDBUFFERSETCONSERVATIVERASTERDILATEPROC) (NVNcommandBuffer *cmdBuf, float dilate);

/// \brief Function pointer type to call nvnCommandBufferSetSubpixelPrecisionBias().
typedef void (NVNAPIENTRYP PFNNVNCOMMANDBUFFERSETSUBPIXELPRECISIONBIASPROC) (NVNcommandBuffer *cmdBuf, int xBits, int yBits);

/// \brief Function pointer type to call nvnCommandBufferCopyBufferToTexture().
typedef void (NVNAPIENTRYP PFNNVNCOMMANDBUFFERCOPYBUFFERTOTEXTUREPROC) (NVNcommandBuffer *cmdBuf, NVNbufferAddress src, const NVNtexture *dstTexture, const NVNtextureView *dstView, const NVNcopyRegion *dstRegion, int flags);

/// \brief Function pointer type to call nvnCommandBufferCopyTextureToBuffer().
typedef void (NVNAPIENTRYP PFNNVNCOMMANDBUFFERCOPYTEXTURETOBUFFERPROC) (NVNcommandBuffer *cmdBuf, const NVNtexture *srcTexture, const NVNtextureView *srcView, const NVNcopyRegion *srcRegion, NVNbufferAddress dst, int flags);

/// \brief Function pointer type to call nvnCommandBufferCopyTextureToTexture().
typedef void (NVNAPIENTRYP PFNNVNCOMMANDBUFFERCOPYTEXTURETOTEXTUREPROC) (NVNcommandBuffer *cmdBuf, const NVNtexture *srcTexture, const NVNtextureView *srcView, const NVNcopyRegion *srcRegion, const NVNtexture *dstTexture, const NVNtextureView *dstView, const NVNcopyRegion *dstRegion, int flags);

/// \brief Function pointer type to call nvnCommandBufferCopyBufferToBuffer().
typedef void (NVNAPIENTRYP PFNNVNCOMMANDBUFFERCOPYBUFFERTOBUFFERPROC) (NVNcommandBuffer *cmdBuf, NVNbufferAddress src, NVNbufferAddress dst, size_t size, int flags);

/// \brief Function pointer type to call nvnCommandBufferClearBuffer().
typedef void (NVNAPIENTRYP PFNNVNCOMMANDBUFFERCLEARBUFFERPROC) (NVNcommandBuffer *cmdBuf, NVNbufferAddress dst, size_t size, uint32_t value);

/// \brief Function pointer type to call nvnCommandBufferClearTexture().
typedef void (NVNAPIENTRYP PFNNVNCOMMANDBUFFERCLEARTEXTUREPROC) (NVNcommandBuffer *cmdBuf, const NVNtexture *dstTexture, const NVNtextureView *dstView, const NVNcopyRegion *dstRegion, const float *color, int mask);

/// \brief Function pointer type to call nvnCommandBufferClearTexturei().
typedef void (NVNAPIENTRYP PFNNVNCOMMANDBUFFERCLEARTEXTUREIPROC) (NVNcommandBuffer *cmdBuf, const NVNtexture *dstTexture, const NVNtextureView *dstView, const NVNcopyRegion *dstRegion, const int *color, int mask);

/// \brief Function pointer type to call nvnCommandBufferClearTextureui().
typedef void (NVNAPIENTRYP PFNNVNCOMMANDBUFFERCLEARTEXTUREUIPROC) (NVNcommandBuffer *cmdBuf, const NVNtexture *dstTexture, const NVNtextureView *dstView, const NVNcopyRegion *dstRegion, const uint32_t *color, int mask);

/// \brief Function pointer type to call nvnCommandBufferUpdateUniformBuffer().
typedef void (NVNAPIENTRYP PFNNVNCOMMANDBUFFERUPDATEUNIFORMBUFFERPROC) (NVNcommandBuffer *cmdBuf, NVNbufferAddress buffer, size_t bufferSize, ptrdiff_t updateOffset, size_t updateSize, const void *data);

/// \brief Function pointer type to call nvnCommandBufferReportCounter().
typedef void (NVNAPIENTRYP PFNNVNCOMMANDBUFFERREPORTCOUNTERPROC) (NVNcommandBuffer *cmdBuf, NVNcounterType counter, NVNbufferAddress buffer);

/// \brief Function pointer type to call nvnCommandBufferResetCounter().
typedef void (NVNAPIENTRYP PFNNVNCOMMANDBUFFERRESETCOUNTERPROC) (NVNcommandBuffer *cmdBuf, NVNcounterType counter);

/// \brief Function pointer type to call nvnCommandBufferReportValue().
typedef void (NVNAPIENTRYP PFNNVNCOMMANDBUFFERREPORTVALUEPROC) (NVNcommandBuffer *cmdBuf, uint32_t value, NVNbufferAddress buffer);

/// \brief Function pointer type to call nvnCommandBufferSetRenderEnable().
typedef void (NVNAPIENTRYP PFNNVNCOMMANDBUFFERSETRENDERENABLEPROC) (NVNcommandBuffer *cmdBuf, NVNboolean enable);

/// \brief Function pointer type to call nvnCommandBufferSetRenderEnableConditional().
typedef void (NVNAPIENTRYP PFNNVNCOMMANDBUFFERSETRENDERENABLECONDITIONALPROC) (NVNcommandBuffer *cmdBuf, NVNconditionalRenderMode mode, NVNbufferAddress counters);

/// \brief Function pointer type to call nvnCommandBufferSetRenderTargets().
typedef void (NVNAPIENTRYP PFNNVNCOMMANDBUFFERSETRENDERTARGETSPROC) (NVNcommandBuffer *cmdBuf, int numColors, const NVNtexture * const *colors, const NVNtextureView * const *colorViews, const NVNtexture *depthStencil, const NVNtextureView *depthStencilView);

/// \brief Function pointer type to call nvnCommandBufferDiscardColor().
typedef void (NVNAPIENTRYP PFNNVNCOMMANDBUFFERDISCARDCOLORPROC) (NVNcommandBuffer *cmdBuf, int index);

/// \brief Function pointer type to call nvnCommandBufferDiscardDepthStencil().
typedef void (NVNAPIENTRYP PFNNVNCOMMANDBUFFERDISCARDDEPTHSTENCILPROC) (NVNcommandBuffer *cmdBuf);

/// \brief Function pointer type to call nvnCommandBufferDownsample().
typedef void (NVNAPIENTRYP PFNNVNCOMMANDBUFFERDOWNSAMPLEPROC) (NVNcommandBuffer *cmdBuf, const NVNtexture *src, const NVNtexture *dst);

/// \brief Function pointer type to call nvnCommandBufferTiledDownsample().
typedef void (NVNAPIENTRYP PFNNVNCOMMANDBUFFERTILEDDOWNSAMPLEPROC) (NVNcommandBuffer *cmdBuf, const NVNtexture *src, const NVNtexture *dst);

/// \brief Function pointer type to call nvnCommandBufferDownsampleTextureView().
typedef void (NVNAPIENTRYP PFNNVNCOMMANDBUFFERDOWNSAMPLETEXTUREVIEWPROC) (NVNcommandBuffer *cmdBuf, const NVNtexture *src, const NVNtextureView *srcView, const NVNtexture *dst, const NVNtextureView *dstView);

/// \brief Function pointer type to call nvnCommandBufferTiledDownsampleTextureView().
typedef void (NVNAPIENTRYP PFNNVNCOMMANDBUFFERTILEDDOWNSAMPLETEXTUREVIEWPROC) (NVNcommandBuffer *cmdBuf, const NVNtexture *src, const NVNtextureView *srcView, const NVNtexture *dst, const NVNtextureView *dstView);

/// \brief Function pointer type to call nvnCommandBufferBarrier().
typedef void (NVNAPIENTRYP PFNNVNCOMMANDBUFFERBARRIERPROC) (NVNcommandBuffer *cmdBuf, int barrier);

/// \brief Function pointer type to call nvnCommandBufferWaitSync().
typedef void (NVNAPIENTRYP PFNNVNCOMMANDBUFFERWAITSYNCPROC) (NVNcommandBuffer *cmdBuf, const NVNsync *sync);

/// \brief Function pointer type to call nvnCommandBufferFenceSync().
typedef void (NVNAPIENTRYP PFNNVNCOMMANDBUFFERFENCESYNCPROC) (NVNcommandBuffer *cmdBuf, NVNsync *sync, NVNsyncCondition condition, int flags);

/// \brief Function pointer type to call nvnCommandBufferSetTexturePool().
typedef void (NVNAPIENTRYP PFNNVNCOMMANDBUFFERSETTEXTUREPOOLPROC) (NVNcommandBuffer *cmdBuf, const NVNtexturePool *texturePool);

/// \brief Function pointer type to call nvnCommandBufferSetSamplerPool().
typedef void (NVNAPIENTRYP PFNNVNCOMMANDBUFFERSETSAMPLERPOOLPROC) (NVNcommandBuffer *cmdBuf, const NVNsamplerPool *samplerPool);

/// \brief Function pointer type to call nvnCommandBufferSetShaderScratchMemory().
typedef void (NVNAPIENTRYP PFNNVNCOMMANDBUFFERSETSHADERSCRATCHMEMORYPROC) (NVNcommandBuffer *cmdBuf, const NVNmemoryPool *pool, ptrdiff_t offset, size_t size);

/// \brief Function pointer type to call nvnCommandBufferSaveZCullData().
typedef void (NVNAPIENTRYP PFNNVNCOMMANDBUFFERSAVEZCULLDATAPROC) (NVNcommandBuffer *cmdBuf, NVNbufferAddress dst, size_t size);

/// \brief Function pointer type to call nvnCommandBufferRestoreZCullData().
typedef void (NVNAPIENTRYP PFNNVNCOMMANDBUFFERRESTOREZCULLDATAPROC) (NVNcommandBuffer *cmdBuf, NVNbufferAddress src, size_t size);

/// \brief Function pointer type to call nvnCommandBufferSetCopyRowStride().
typedef void (NVNAPIENTRYP PFNNVNCOMMANDBUFFERSETCOPYROWSTRIDEPROC) (NVNcommandBuffer *cmdBuf, ptrdiff_t stride);

/// \brief Function pointer type to call nvnCommandBufferSetCopyImageStride().
typedef void (NVNAPIENTRYP PFNNVNCOMMANDBUFFERSETCOPYIMAGESTRIDEPROC) (NVNcommandBuffer *cmdBuf, ptrdiff_t stride);

/// \brief Function pointer type to call nvnCommandBufferGetCopyRowStride().
typedef ptrdiff_t (NVNAPIENTRYP PFNNVNCOMMANDBUFFERGETCOPYROWSTRIDEPROC) (const NVNcommandBuffer *cmdBuf);

/// \brief Function pointer type to call nvnCommandBufferGetCopyImageStride().
typedef ptrdiff_t (NVNAPIENTRYP PFNNVNCOMMANDBUFFERGETCOPYIMAGESTRIDEPROC) (const NVNcommandBuffer *cmdBuf);

/// \brief Function pointer type to call nvnCommandBufferDrawTexture().
typedef void (NVNAPIENTRYP PFNNVNCOMMANDBUFFERDRAWTEXTUREPROC) (NVNcommandBuffer *cmdBuf, NVNtextureHandle texture, const NVNdrawTextureRegion *dstRegion, const NVNdrawTextureRegion *srcRegion);

/// \brief Function pointer type to call nvnProgramSetSubroutineLinkage().
typedef NVNboolean (NVNAPIENTRYP PFNNVNPROGRAMSETSUBROUTINELINKAGEPROC) (NVNprogram *program, int count, const NVNsubroutineLinkageMapPtr *linkageMapPtrs);

/// \brief Function pointer type to call nvnCommandBufferSetProgramSubroutines().
typedef void (NVNAPIENTRYP PFNNVNCOMMANDBUFFERSETPROGRAMSUBROUTINESPROC) (NVNcommandBuffer *cmdBuf, NVNprogram *program, NVNshaderStage stage, const int first, const int count, const int *values);

/// \brief Function pointer type to call nvnCommandBufferBindCoverageModulationTable().
typedef void (NVNAPIENTRYP PFNNVNCOMMANDBUFFERBINDCOVERAGEMODULATIONTABLEPROC) (NVNcommandBuffer *cmdBuf, const float *entries);

/// \brief Function pointer type to call nvnCommandBufferResolveDepthBuffer().
typedef void (NVNAPIENTRYP PFNNVNCOMMANDBUFFERRESOLVEDEPTHBUFFERPROC) (NVNcommandBuffer *cmdBuf);

/// \brief Function pointer type to call nvnCommandBufferSetColorReductionEnable().
typedef void (NVNAPIENTRYP PFNNVNCOMMANDBUFFERSETCOLORREDUCTIONENABLEPROC) (NVNcommandBuffer *cmdBuf, NVNboolean enable);

/// \brief Function pointer type to call nvnCommandBufferSetColorReductionThresholds().
typedef void (NVNAPIENTRYP PFNNVNCOMMANDBUFFERSETCOLORREDUCTIONTHRESHOLDSPROC) (NVNcommandBuffer *cmdBuf, NVNformatClass formatClass, int thresholdConservative, int thresholdAggressive);

/// \brief Function pointer type to call nvnCommandBufferPushDebugGroupStatic().
typedef void (NVNAPIENTRYP PFNNVNCOMMANDBUFFERPUSHDEBUGGROUPSTATICPROC) (NVNcommandBuffer *cmdBuf, uint32_t domainId, const char * description);

/// \brief Function pointer type to call nvnCommandBufferPushDebugGroupDynamic().
typedef void (NVNAPIENTRYP PFNNVNCOMMANDBUFFERPUSHDEBUGGROUPDYNAMICPROC) (NVNcommandBuffer *cmdBuf, uint32_t domainId, const char * description);

/// \brief Function pointer type to call nvnCommandBufferPushDebugGroup().
typedef void (NVNAPIENTRYP PFNNVNCOMMANDBUFFERPUSHDEBUGGROUPPROC) (NVNcommandBuffer *cmdBuf, const char * description);

/// \brief Function pointer type to call nvnCommandBufferPopDebugGroup().
typedef void (NVNAPIENTRYP PFNNVNCOMMANDBUFFERPOPDEBUGGROUPPROC) (NVNcommandBuffer *cmdBuf);

/// \brief Function pointer type to call nvnCommandBufferPopDebugGroupId().
typedef void (NVNAPIENTRYP PFNNVNCOMMANDBUFFERPOPDEBUGGROUPIDPROC) (NVNcommandBuffer *cmdBuf, uint32_t domainId);

/// \brief Function pointer type to call nvnCommandBufferInsertDebugMarkerStatic().
typedef void (NVNAPIENTRYP PFNNVNCOMMANDBUFFERINSERTDEBUGMARKERSTATICPROC) (NVNcommandBuffer *cmdBuf, uint32_t domainId, const char * description);

/// \brief Function pointer type to call nvnCommandBufferInsertDebugMarkerDynamic().
typedef void (NVNAPIENTRYP PFNNVNCOMMANDBUFFERINSERTDEBUGMARKERDYNAMICPROC) (NVNcommandBuffer *cmdBuf, uint32_t domainId, const char * description);

/// \brief Function pointer type to call nvnCommandBufferInsertDebugMarker().
typedef void (NVNAPIENTRYP PFNNVNCOMMANDBUFFERINSERTDEBUGMARKERPROC) (NVNcommandBuffer *cmdBuf, const char * description);

/// \brief Function pointer type to call nvnCommandBufferGetMemoryCallback().
typedef PFNNVNCOMMANDBUFFERMEMORYCALLBACKPROC (NVNAPIENTRYP PFNNVNCOMMANDBUFFERGETMEMORYCALLBACKPROC) (const NVNcommandBuffer *cmdBuf);

/// \brief Function pointer type to call nvnCommandBufferGetMemoryCallbackData().
typedef void* (NVNAPIENTRYP PFNNVNCOMMANDBUFFERGETMEMORYCALLBACKDATAPROC) (const NVNcommandBuffer *cmdBuf);

/// \brief Function pointer type to call nvnCommandBufferIsRecording().
typedef NVNboolean (NVNAPIENTRYP PFNNVNCOMMANDBUFFERISRECORDINGPROC) (const NVNcommandBuffer *cmdBuf);

/// \brief Function pointer type to call nvnSyncInitialize().
typedef NVNboolean (NVNAPIENTRYP PFNNVNSYNCINITIALIZEPROC) (NVNsync *sync, NVNdevice *device);

/// \brief Function pointer type to call nvnSyncFinalize().
typedef void (NVNAPIENTRYP PFNNVNSYNCFINALIZEPROC) (NVNsync *sync);

/// \brief Function pointer type to call nvnSyncSetDebugLabel().
typedef void (NVNAPIENTRYP PFNNVNSYNCSETDEBUGLABELPROC) (NVNsync *sync, const char * label);

/// \brief Function pointer type to call nvnQueueFenceSync().
typedef void (NVNAPIENTRYP PFNNVNQUEUEFENCESYNCPROC) (NVNqueue *queue, NVNsync *sync, NVNsyncCondition condition, int flags);

/// \brief Function pointer type to call nvnSyncWait().
typedef NVNsyncWaitResult (NVNAPIENTRYP PFNNVNSYNCWAITPROC) (const NVNsync *sync, uint64_t timeoutNs);

/// \brief Function pointer type to call nvnQueueWaitSync().
typedef NVNboolean (NVNAPIENTRYP PFNNVNQUEUEWAITSYNCPROC) (NVNqueue *queue, const NVNsync *sync);

/// \brief Function pointer type to call nvnSyncInitializeFromFencedGLSync().
typedef NVNboolean (NVNAPIENTRYP PFNNVNSYNCINITIALIZEFROMFENCEDGLSYNCPROC) (NVNsync *sync, NVNdevice *device, const void *glSync, NVNsyncInteropStatus *status);

/// \brief Function pointer type to call nvnSyncCreateGLSync().
typedef NVNsyncInteropStatus (NVNAPIENTRYP PFNNVNSYNCCREATEGLSYNCPROC) (const NVNsync *sync, uint64_t *glSync);

/// \brief Function pointer type to call nvnEventBuilderSetDefaults().
typedef void (NVNAPIENTRYP PFNNVNEVENTBUILDERSETDEFAULTSPROC) (NVNeventBuilder *builder);

/// \brief Function pointer type to call nvnEventBuilderSetStorage().
typedef void (NVNAPIENTRYP PFNNVNEVENTBUILDERSETSTORAGEPROC) (NVNeventBuilder *builder, const NVNmemoryPool *pool, int64_t offset);

/// \brief Function pointer type to call nvnEventBuilderGetStorage().
typedef const NVNmemoryPool * (NVNAPIENTRYP PFNNVNEVENTBUILDERGETSTORAGEPROC) (const NVNeventBuilder *builder, int64_t *offset);

/// \brief Function pointer type to call nvnEventInitialize().
typedef NVNboolean (NVNAPIENTRYP PFNNVNEVENTINITIALIZEPROC) (NVNevent *event, const NVNeventBuilder *builder);

/// \brief Function pointer type to call nvnEventFinalize().
typedef void (NVNAPIENTRYP PFNNVNEVENTFINALIZEPROC) (NVNevent *event);

/// \brief Function pointer type to call nvnEventGetValue().
typedef uint32_t (NVNAPIENTRYP PFNNVNEVENTGETVALUEPROC) (const NVNevent *event);

/// \brief Function pointer type to call nvnEventSignal().
typedef void (NVNAPIENTRYP PFNNVNEVENTSIGNALPROC) (NVNevent *event, NVNeventSignalMode mode, uint32_t value);

/// \brief Function pointer type to call nvnCommandBufferWaitEvent().
typedef void (NVNAPIENTRYP PFNNVNCOMMANDBUFFERWAITEVENTPROC) (NVNcommandBuffer *cmdBuf, const NVNevent *event, NVNeventWaitMode waitMode, uint32_t value);

/// \brief Function pointer type to call nvnCommandBufferSignalEvent().
typedef void (NVNAPIENTRYP PFNNVNCOMMANDBUFFERSIGNALEVENTPROC) (NVNcommandBuffer *cmdBuf, const NVNevent *event, NVNeventSignalMode mode, NVNeventSignalLocation location, int flags, uint32_t value);

/// \brief Function pointer type to call nvnCommandBufferSetStencilCullCriteria().
typedef void (NVNAPIENTRYP PFNNVNCOMMANDBUFFERSETSTENCILCULLCRITERIAPROC) (NVNcommandBuffer *cmdBuf, NVNstencilFunc func, int ref, int mask);

/// \}

/// \}

#if (defined(_MSC_VER) || defined(__clang__) || \
     (defined(__GNUC__) && !defined(__sun__) && !defined(__FreeBSD__)))
#pragma pack(pop)
#endif

#endif /* __nvn_h_ */
