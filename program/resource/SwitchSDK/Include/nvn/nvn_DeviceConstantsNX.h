/*--------------------------------------------------------------------------------*
  Copyright (C)Nintendo All rights reserved.

  These coded instructions, statements, and computer programs contain proprietary
  information of Nintendo and/or its licensed developers and are protected by
  national and international copyright laws. They may not be disclosed to third
  parties or copied or duplicated in any form, in whole or in part, without the
  prior written consent of Nintendo.

  The content herein is highly confidential and should be handled accordingly.
 *--------------------------------------------------------------------------------*/


#ifndef __nvn_device_info_const_nx_h__
#define __nvn_device_info_const_nx_h__

/// \file nvn_DeviceConstantsNX.h
/// Constants holding values that would be returned for queries of
/// various NVN DeviceInfo properties on the NX target platform.
/// These constants can be used directly by applications that run only
/// on the NX platform, but may not be accurate on the Windows reference
/// implementation.

/// \brief Number of uniform buffer bindings supported for each shader
/// stage.
#define NVN_DEVICE_INFO_CONSTANT_NX_UNIFORM_BUFFER_BINDINGS_PER_STAGE              14

/// \brief Maximum size (in bytes) of a bound uniform buffer.
#define NVN_DEVICE_INFO_CONSTANT_NX_MAX_UNIFORM_BUFFER_SIZE                        65536

/// \brief Minimum alignment of uniform buffer bindings.
#define NVN_DEVICE_INFO_CONSTANT_NX_UNIFORM_BUFFER_ALIGNMENT                       256

/// \brief Number of color buffer binding points.
#define NVN_DEVICE_INFO_CONSTANT_NX_COLOR_BUFFER_BINDINGS                          8

/// \brief Number of vertex buffer binding points.
#define NVN_DEVICE_INFO_CONSTANT_NX_VERTEX_BUFFER_BINDINGS                         16

/// \brief Number of transform feedback buffer binding points.
#define NVN_DEVICE_INFO_CONSTANT_NX_TRANSFORM_FEEDBACK_BUFFER_BINDINGS             4

/// \brief Number of shader storage buffer bindings supported for each
/// shader stage.
#define NVN_DEVICE_INFO_CONSTANT_NX_SHADER_STORAGE_BUFFER_BINDINGS_PER_STAGE       16

/// \brief Number of texture and sampler bindings supported for each shader
/// stage.
#define NVN_DEVICE_INFO_CONSTANT_NX_TEXTURE_BINDINGS_PER_STAGE                     32

/// \brief Minimum alignment for counter reports.
#define NVN_DEVICE_INFO_CONSTANT_NX_COUNTER_ALIGNMENT                              16

/// \brief Minimum alignment of transform feedback buffer bindings.
#define NVN_DEVICE_INFO_CONSTANT_NX_TRANSFORM_FEEDBACK_BUFFER_ALIGNMENT            4

/// \brief Minimum alignment of transform feedback control data.
#define NVN_DEVICE_INFO_CONSTANT_NX_TRANSFORM_FEEDBACK_CONTROL_ALIGNMENT           4

/// \brief Minimum alignment of indirect draw data.
#define NVN_DEVICE_INFO_CONSTANT_NX_INDIRECT_DRAW_ALIGNMENT                        4

/// \brief Number of vertex attributes.
#define NVN_DEVICE_INFO_CONSTANT_NX_VERTEX_ATTRIBUTES                              16

/// \brief Size (in bytes) of a single texture descriptor stored in the
/// pool.
#define NVN_DEVICE_INFO_CONSTANT_NX_TEXTURE_DESCRIPTOR_SIZE                        32

/// \brief Size in bytes of a single sampler descriptor.
#define NVN_DEVICE_INFO_CONSTANT_NX_SAMPLER_DESCRIPTOR_SIZE                        32

/// \brief Number of entries reserved at the beginning of the texture pool
/// for internal driver usage.
#define NVN_DEVICE_INFO_CONSTANT_NX_RESERVED_TEXTURE_DESCRIPTORS                   256

/// \brief Number of samplers that must be reserved for use by the driver.
#define NVN_DEVICE_INFO_CONSTANT_NX_RESERVED_SAMPLER_DESCRIPTORS                   256

/// \brief Minimum alignment for command data in a command builder.
#define NVN_DEVICE_INFO_CONSTANT_NX_COMMAND_BUFFER_COMMAND_ALIGNMENT               4

/// \brief Minimum alignment for control data in a command builder.
#define NVN_DEVICE_INFO_CONSTANT_NX_COMMAND_BUFFER_CONTROL_ALIGNMENT               8

/// \brief Minimum size (in bytes) recommended for command data in a
/// command builder.
#define NVN_DEVICE_INFO_CONSTANT_NX_COMMAND_BUFFER_MIN_COMMAND_SIZE                512

/// \brief Minimum size (in bytes) recommended for control data in a
/// command builder.
#define NVN_DEVICE_INFO_CONSTANT_NX_COMMAND_BUFFER_MIN_CONTROL_SIZE                256

/// \brief Minimum scale factor for providing scratch memory for
/// non-compute shaders.
#define NVN_DEVICE_INFO_CONSTANT_NX_SHADER_SCRATCH_MEMORY_SCALE_FACTOR_MINIMUM     2

/// \brief Scale factor for the recommended amount of memory
#define NVN_DEVICE_INFO_CONSTANT_NX_SHADER_SCRATCH_MEMORY_SCALE_FACTOR_RECOMMENDED 256

/// \brief Alignment required (in bytes) for the shader scratch memory.
#define NVN_DEVICE_INFO_CONSTANT_NX_SHADER_SCRATCH_MEMORY_ALIGNMENT                4096

/// \brief Required granularity for shader scratch memory provided to NVN.
#define NVN_DEVICE_INFO_CONSTANT_NX_SHADER_SCRATCH_MEMORY_GRANULARITY              131072

/// \brief Maximum sampler anisotropy.
#define NVN_DEVICE_INFO_CONSTANT_NX_MAX_TEXTURE_ANISOTROPY                         16

/// \brief Maximum number of threads in the X dimension supported in a
/// compute work group.
#define NVN_DEVICE_INFO_CONSTANT_NX_MAX_COMPUTE_WORK_GROUP_SIZE_X                  1536

/// \brief Maximum number of threads in the Y dimension supported in a
/// compute work group.
#define NVN_DEVICE_INFO_CONSTANT_NX_MAX_COMPUTE_WORK_GROUP_SIZE_Y                  1024

/// \brief Maximum number of threads in the Z dimension supported in a
/// compute work group.
#define NVN_DEVICE_INFO_CONSTANT_NX_MAX_COMPUTE_WORK_GROUP_SIZE_Z                  64

/// \brief Maximum total number of threads supported in a compute work
/// group.
#define NVN_DEVICE_INFO_CONSTANT_NX_MAX_COMPUTE_WORK_GROUP_SIZE_THREADS            1536

/// \brief Maximum number of work groups in the X dimension supported in a
/// compute dispatch.
#define NVN_DEVICE_INFO_CONSTANT_NX_MAX_COMPUTE_DISPATCH_WORK_GROUPS_X             65535

/// \brief Maximum number of work groups in the Y dimension supported in a
/// compute dispatch.
#define NVN_DEVICE_INFO_CONSTANT_NX_MAX_COMPUTE_DISPATCH_WORK_GROUPS_Y             65535

/// \brief Maximum number of work groups in the Z dimension supported in a
/// compute dispatch.
#define NVN_DEVICE_INFO_CONSTANT_NX_MAX_COMPUTE_DISPATCH_WORK_GROUPS_Z             65535

/// \brief Number of image bindings supported for each shader stage.
#define NVN_DEVICE_INFO_CONSTANT_NX_IMAGE_BINDINGS_PER_STAGE                       8

/// \brief Maximum number of entries (including reserved ones) supported in
/// a texture descriptor pool.
#define NVN_DEVICE_INFO_CONSTANT_NX_MAX_TEXTURE_POOL_SIZE                          1048576

/// \brief Maximum number of entries (including reserved ones) supported in
/// a sampler descriptor pool.
#define NVN_DEVICE_INFO_CONSTANT_NX_MAX_SAMPLER_POOL_SIZE                          4096

/// \brief Maximum number of viewports.
#define NVN_DEVICE_INFO_CONSTANT_NX_MAX_VIEWPORTS                                  16

/// \brief Granularity of memory pool regions (pages or sub-page units),
/// where each region may not contain both texture and buffer resources.
#define NVN_DEVICE_INFO_CONSTANT_NX_MEMPOOL_TEXTURE_OBJECT_PAGE_ALIGNMENT          4096

/// \brief 1 if SamplerReduction::MIN and SamplerReduction::MAX are
/// supported by the NVN implementation; 0 otherwise.
#define NVN_DEVICE_INFO_CONSTANT_NX_SUPPORTS_MIN_MAX_FILTERING                     1

/// \brief 1 if Format::STENCIL8 is supported by the NVN implementation; 0
/// otherwise.
#define NVN_DEVICE_INFO_CONSTANT_NX_SUPPORTS_STENCIL8_FORMAT                       1

/// \brief 1 if ASTC texture formats are supported by the NVN
/// implementation; 0 otherwise.
#define NVN_DEVICE_INFO_CONSTANT_NX_SUPPORTS_ASTC_FORMATS                          1

/// \brief Total size of L2 cache in the GPU core.
#define NVN_DEVICE_INFO_CONSTANT_NX_L2_SIZE                                        262144

/// \brief Maximum number of levels supported for any texture target.
#define NVN_DEVICE_INFO_CONSTANT_NX_MAX_TEXTURE_LEVELS                             15

/// \brief Maximum number of layers supported for any array texture target.
#define NVN_DEVICE_INFO_CONSTANT_NX_MAX_TEXTURE_LAYERS                             2048

/// \brief Maximum supported GLSLC GPU code section binary major version.
#define NVN_DEVICE_INFO_CONSTANT_NX_GLSLC_MAX_SUPPORTED_GPU_CODE_MAJOR_VERSION     1

/// \brief Minimum supported GLSLC binary version.
#define NVN_DEVICE_INFO_CONSTANT_NX_GLSLC_MIN_SUPPORTED_GPU_CODE_MAJOR_VERSION     1

/// \brief Maximum supported GLSLC binary version.
#define NVN_DEVICE_INFO_CONSTANT_NX_GLSLC_MAX_SUPPORTED_GPU_CODE_MINOR_VERSION     15

/// \brief Minimum supported GLSLC binary version.
#define NVN_DEVICE_INFO_CONSTANT_NX_GLSLC_MIN_SUPPORTED_GPU_CODE_MINOR_VERSION     5

/// \brief 1 if conservative rasterization is supported supported by the
/// NVN implementation; 0 otherwise.
#define NVN_DEVICE_INFO_CONSTANT_NX_SUPPORTS_CONSERVATIVE_RASTER                   1

/// \brief The number of bits of subpixel precision used to represent
/// vertex window coordinates.
#define NVN_DEVICE_INFO_CONSTANT_NX_SUBPIXEL_BITS                                  8

/// \brief The maximum number of additional bits of subpixel precision that
/// can be used to represent vertex window coordinates during conservative
/// rasterization.
#define NVN_DEVICE_INFO_CONSTANT_NX_MAX_SUBPIXEL_BIAS_BITS                         8

/// \brief Minimum alignment of indirect dispatch data.
#define NVN_DEVICE_INFO_CONSTANT_NX_INDIRECT_DISPATCH_ALIGNMENT                    4

/// \brief Minimum alignment for ZCull save/restore buffers.
#define NVN_DEVICE_INFO_CONSTANT_NX_ZCULL_SAVE_RESTORE_ALIGNMENT                   32

/// \brief Minimum scale factor for providing scratch memory for compute
/// shaders.
#define NVN_DEVICE_INFO_CONSTANT_NX_SHADER_SCRATCH_MEMORY_COMPUTE_SCALE_FACTOR_MINIMUM 128

/// \brief Minimum alignment required for stride in linear textures, in
/// bytes.
#define NVN_DEVICE_INFO_CONSTANT_NX_LINEAR_TEXTURE_STRIDE_ALIGNMENT                32

/// \brief Minimum alignment required for stride in linear render targets,
/// in bytes.
#define NVN_DEVICE_INFO_CONSTANT_NX_LINEAR_RENDER_TARGET_STRIDE_ALIGNMENT          128

/// \brief Size of memory pages used in physical and virtual memory pools.
#define NVN_DEVICE_INFO_CONSTANT_NX_MEMORY_POOL_PAGE_SIZE                          65536

/// \brief 1 if the implementation always returns zero values when reading
/// from unpopulated portions of virtual memory pools; 0 otherwise.
#define NVN_DEVICE_INFO_CONSTANT_NX_SUPPORTS_ZERO_FROM_UNMAPPED_VIRTUAL_POOL_PAGES 1

/// \brief Alignment required (in bytes) for the offset and size of uniform
/// buffer updates via CommandBuffer::UpdateUniformBuffer.
#define NVN_DEVICE_INFO_CONSTANT_NX_UNIFORM_BUFFER_UPDATE_ALIGNMENT                4

/// \brief Maximum texture size (in pixels) supported for 1D, 2D and 1D
/// array and 2D array targets.
#define NVN_DEVICE_INFO_CONSTANT_NX_MAX_TEXTURE_SIZE                               16384

/// \brief Maximum texture size (in pixels) supported for buffer textures.
#define NVN_DEVICE_INFO_CONSTANT_NX_MAX_BUFFER_TEXTURE_SIZE                        134217728

/// \brief Maximum texture size (in pixels) supported by 3D texture target.
#define NVN_DEVICE_INFO_CONSTANT_NX_MAX_3D_TEXTURE_SIZE                            2048

/// \brief Maximum texture size (in pixels) supported by cubemap texture
/// target.
#define NVN_DEVICE_INFO_CONSTANT_NX_MAX_CUBE_MAP_TEXTURE_SIZE                      16384

/// \brief Maximum texture size (in pixels) supported by rectangle texture
/// target.
#define NVN_DEVICE_INFO_CONSTANT_NX_MAX_RECTANGLE_TEXTURE_SIZE                     16384

/// \brief 1 if GLSL shaders using NV_geometry_shader_passthrough are
/// supported by the NVN implementation; 0 otherwise.
#define NVN_DEVICE_INFO_CONSTANT_NX_SUPPORTS_PASSTHROUGH_GEOMETRY_SHADERS          1

/// \brief 1 if the viewport swizzle operation is supported by the NVN
/// implementation; 0 otherwise.
#define NVN_DEVICE_INFO_CONSTANT_NX_SUPPORTS_VIEWPORT_SWIZZLE                      1

/// \brief 1 if the NVN implementation supports packaged texture data in
/// sparse tiled form; 0 otherwise.
#define NVN_DEVICE_INFO_CONSTANT_NX_SUPPORTS_SPARSE_TILED_PACKAGED_TEXTURES        1

/// \brief 1 if advanced blending modes (other than
/// BlendAdvancedMode::NONE) are supported by the NVN implementation; 0
/// otherwise.
#define NVN_DEVICE_INFO_CONSTANT_NX_SUPPORTS_ADVANCED_BLEND_MODES                  1

/// \brief Maximum value supported by
/// nvn::WindowBuilder::SetPresentInterval.
#define NVN_DEVICE_INFO_CONSTANT_NX_MAX_PRESENT_INTERVAL                           5

/// \brief 1 if DrawTexture is supported by the NVN implementation; 0
/// otherwise.
#define NVN_DEVICE_INFO_CONSTANT_NX_SUPPORTS_DRAW_TEXTURE                          1

/// \brief 1 if the NVN implementation supports target-independent
/// rasterization, 0 otherwise.
#define NVN_DEVICE_INFO_CONSTANT_NX_SUPPORTS_TARGET_INDEPENDENT_RASTERIZATION      1

/// \brief 1 if the NVN implementation supports fragment coverage to color,
/// 0 otherwise.
#define NVN_DEVICE_INFO_CONSTANT_NX_SUPPORTS_FRAGMENT_COVERAGE_TO_COLOR            1

/// \brief 1 if the NVN implementation supports post-depth coverage, 0
/// otherwise.
#define NVN_DEVICE_INFO_CONSTANT_NX_SUPPORTS_POST_DEPTH_COVERAGE                   1

/// \brief 1 if the NVN implementation supports using texture handles for
/// image accesses, 0 otherwise.
#define NVN_DEVICE_INFO_CONSTANT_NX_SUPPORTS_IMAGES_USING_TEXTURE_HANDLES          1

/// \brief 1 if the NVN implementation supports programmable sample
/// locations, 0 otherwise.
#define NVN_DEVICE_INFO_CONSTANT_NX_SUPPORTS_SAMPLE_LOCATIONS                      1

/// \brief Total number of programmable sample locations in a
/// MultisampleState object.
#define NVN_DEVICE_INFO_CONSTANT_NX_MAX_SAMPLE_LOCATION_TABLE_ENTRIES              16

/// \brief Amount of padding required at the end of a memory pool holding
/// shader code.
#define NVN_DEVICE_INFO_CONSTANT_NX_SHADER_CODE_MEMORY_POOL_PADDING_SIZE           1024

/// \brief Maximum number of vertices in each patch primitive.
#define NVN_DEVICE_INFO_CONSTANT_NX_MAX_PATCH_SIZE                                 32

/// \brief Required granularity for per-queue command memory.
#define NVN_DEVICE_INFO_CONSTANT_NX_QUEUE_COMMAND_MEMORY_GRANULARITY               4096

/// \brief Minimum allowed size for per-queue command memory.
#define NVN_DEVICE_INFO_CONSTANT_NX_QUEUE_COMMAND_MEMORY_MIN_SIZE                  65536

/// \brief Default size for per-queue command memory.
#define NVN_DEVICE_INFO_CONSTANT_NX_QUEUE_COMMAND_MEMORY_DEFAULT_SIZE              65536

/// \brief Required granularity for per-queue compute memory.
#define NVN_DEVICE_INFO_CONSTANT_NX_QUEUE_COMPUTE_MEMORY_GRANULARITY               256

/// \brief Minimum allowed size for per-queue compute memory, if non-zero.
#define NVN_DEVICE_INFO_CONSTANT_NX_QUEUE_COMPUTE_MEMORY_MIN_SIZE                  16384

/// \brief Default size for per-queue compute memory.
#define NVN_DEVICE_INFO_CONSTANT_NX_QUEUE_COMPUTE_MEMORY_DEFAULT_SIZE              262144

/// \brief Minimum flush threshold size for per-queue command memory
/// provided to NVN.
#define NVN_DEVICE_INFO_CONSTANT_NX_QUEUE_COMMAND_MEMORY_MIN_FLUSH_THRESHOLD       4096

/// \brief 1 if GLSL fragment shaders with interlocks
/// (NV_fragment_shader_interlock) are supported by the NVN implementation;
/// 0 otherwise.
#define NVN_DEVICE_INFO_CONSTANT_NX_SUPPORTS_FRAGMENT_SHADER_INTERLOCK             1

/// \brief Maximum value supported by nvn::WindowBuilder::SetTextures.
#define NVN_DEVICE_INFO_CONSTANT_NX_MAX_TEXTURES_PER_WINDOW                        4

/// \brief Minimum value supported by nvn::WindowBuilder::SetTextures.
#define NVN_DEVICE_INFO_CONSTANT_NX_MIN_TEXTURES_PER_WINDOW                        2

/// \brief Minimum allowed size for queue control memory.
#define NVN_DEVICE_INFO_CONSTANT_NX_QUEUE_CONTROL_MEMORY_MIN_SIZE                  4096

/// \brief Default size for queue control memory.
#define NVN_DEVICE_INFO_CONSTANT_NX_QUEUE_CONTROL_MEMORY_DEFAULT_SIZE              16384

/// \brief Required granularity for queue control memory.
#define NVN_DEVICE_INFO_CONSTANT_NX_QUEUE_CONTROL_MEMORY_GRANULARITY               4096

/// \brief Number of separate texture bindings supported for each shader
/// stage.
#define NVN_DEVICE_INFO_CONSTANT_NX_SEPARATE_TEXTURE_BINDINGS_PER_STAGE            128

/// \brief Number of separate sampler bindings supported for each shader
/// stage.
#define NVN_DEVICE_INFO_CONSTANT_NX_SEPARATE_SAMPLER_BINDINGS_PER_STAGE            32

/// \brief Value of the last valid debug groups domain ID.
#define NVN_DEVICE_INFO_CONSTANT_NX_DEBUG_GROUPS_MAX_DOMAIN_ID                     65535

/// \brief Does the device support reduction operations for events.
#define NVN_DEVICE_INFO_CONSTANT_NX_EVENTS_SUPPORT_REDUCTION_OPERATIONS            1

#endif // #ifndef __nvn_device_info_const_nx_h__
