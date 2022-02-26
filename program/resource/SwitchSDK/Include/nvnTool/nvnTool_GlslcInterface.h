/*--------------------------------------------------------------------------------*
  Copyright (C)Nintendo All rights reserved.

  These coded instructions, statements, and computer programs contain proprietary
  information of Nintendo and/or its licensed developers and are protected by
  national and international copyright laws. They may not be disclosed to third
  parties or copied or duplicated in any form, in whole or in part, without the
  prior written consent of Nintendo.

  The content herein is highly confidential and should be handled accordingly.
 *--------------------------------------------------------------------------------*/


#ifndef NVNTOOL_GLSLCINTERFACE_H_
#define NVNTOOL_GLSLCINTERFACE_H_

/// \file nvnTool_GlslcInterface.h
///      API data type and function definitions for the GLSLC shader compiler
///      tool.

// Clients using older MSVC versions without support for stdint.h need
// to supply their own versions.
#if (defined(_MSC_VER) && (_MSC_VER >= 1600)) || defined(__GNUC__)
#include "stdint.h"
#endif

#include "nvn/nvn.h"

#ifndef GLSLCAPI
# if defined(__GNUC__)
#  define GLSLCAPI extern __attribute__ ((visibility ("default")))
# elif defined(WIN32) || defined(_WIN32)
#  define GLSLCAPI __declspec(dllexport)
# else
#  define GLSLCAPI
# endif
#endif

#define GLSLC_API_VERSION_MAJOR    17
#define GLSLC_API_VERSION_MINOR    20

#define GLSLC_GPU_CODE_VERSION_MAJOR 1
#define GLSLC_GPU_CODE_VERSION_MINOR 15

#if defined(_MSC_VER) || (defined(__GNUC__) && !defined(__sun__))
#pragma pack(push, 8)
#endif

/// A number indicating the start of a GLSLCoutput structure block.
#define GLSLC_MAGIC_NUMBER 0x19866891U

/// A number indicating the start of a GPU program control section.
#define GLSLC_GPU_CODE_SECTION_CONTROL_MAGIC_NUMBER 0x98761234U

/// A number indicating the start of a GPU program data section.
#define GLSLC_GPU_CODE_SECTION_DATA_MAGIC_NUMBER 0x12345678U

/// A number indicating the start of a perf stats data section.
#define GLSLC_PERF_STATS_SECTION_MAGIC_NUMBER 0x12898888U

/// A number indicating the start of a program reflection data section.
#define GLSLC_REFLECTION_SECTION_MAGIC_NUMBER 0x19292919U

/// A number indicating the start of a debug data section
#define GLSLC_DEBUG_SECTION_MAGIC_NUMBER 0x65040891

#define GLSLC_NUM_SHADER_STAGES 6

/// Maximum number of uniforms that can be specialized.
#define GLSLC_MAX_SPECIALIZATION_UNIFORMS 512

/// A helper macro to define a struct member pointer guaranteed to be 64 bits wide using an
/// anonymous union.
#define GLSLC_POINTER_MEMBER(type,name) union { type name ; uint64_t ptr##name ; }

/// Reserves _num_ bytes.  Typically this is used at the end of structure definitions to
/// allow for future expansion.
#define GLSLC_RESERVED_BYTES(num) uint8_t reserved[num]
#define GLSLC_RESERVED_BYTES2(num) uint8_t reserved2[num]

#ifdef __cplusplus
extern "C" {
#endif

/// Used to indicate whether the uniform is a sampler, image, separate texture, or normal uniform.
enum GLSLCpiqUniformKind {
    /// Normal uniform part of UBOs
    GLSLC_PIQ_UNIFORM_KIND_PLAIN = 0,

    /// Sampler uniform (either separate sampler or combined sampler and texture,
    /// the GLSLCpiqType will distinguish between the two). 
    GLSLC_PIQ_UNIFORM_KIND_SAMPLER,

    /// Image uniform
    GLSLC_PIQ_UNIFORM_KIND_IMAGE,

    /// Separate texture uniform
    GLSLC_PIQ_UNIFORM_KIND_TEXTURE,

    GLSLC_PIQ_UNIFORM_KIND_INVALID = 0x7FFFFFFF,
};

/// \brief For program interface query support, data types reported for
/// shader elements will have types that correspond to the types
/// in this enumeration structure.
enum GLSLCpiqTypeEnum {
    GLSLC_PIQ_TYPE_BOOL = 0,
    GLSLC_PIQ_TYPE_BVEC2,
    GLSLC_PIQ_TYPE_BVEC3,
    GLSLC_PIQ_TYPE_BVEC4,
    GLSLC_PIQ_TYPE_INT,
    GLSLC_PIQ_TYPE_INT_VEC2,
    GLSLC_PIQ_TYPE_INT_VEC3,
    GLSLC_PIQ_TYPE_INT_VEC4,
    GLSLC_PIQ_TYPE_INT8,
    GLSLC_PIQ_TYPE_INT8_VEC2,
    GLSLC_PIQ_TYPE_INT8_VEC3,
    GLSLC_PIQ_TYPE_INT8_VEC4,
    GLSLC_PIQ_TYPE_INT16,
    GLSLC_PIQ_TYPE_INT16_VEC2,
    GLSLC_PIQ_TYPE_INT16_VEC3,
    GLSLC_PIQ_TYPE_INT16_VEC4,
    GLSLC_PIQ_TYPE_INT64,
    GLSLC_PIQ_TYPE_INT64_VEC2,
    GLSLC_PIQ_TYPE_INT64_VEC3,
    GLSLC_PIQ_TYPE_INT64_VEC4,
    GLSLC_PIQ_TYPE_UINT,
    GLSLC_PIQ_TYPE_UINT_VEC2,
    GLSLC_PIQ_TYPE_UINT_VEC3,
    GLSLC_PIQ_TYPE_UINT_VEC4,
    GLSLC_PIQ_TYPE_UINT8,
    GLSLC_PIQ_TYPE_UINT8_VEC2,
    GLSLC_PIQ_TYPE_UINT8_VEC3,
    GLSLC_PIQ_TYPE_UINT8_VEC4,
    GLSLC_PIQ_TYPE_UINT16,
    GLSLC_PIQ_TYPE_UINT16_VEC2,
    GLSLC_PIQ_TYPE_UINT16_VEC3,
    GLSLC_PIQ_TYPE_UINT16_VEC4,
    GLSLC_PIQ_TYPE_UINT64,
    GLSLC_PIQ_TYPE_UINT64_VEC2,
    GLSLC_PIQ_TYPE_UINT64_VEC3,
    GLSLC_PIQ_TYPE_UINT64_VEC4,
    GLSLC_PIQ_TYPE_FLOAT,
    GLSLC_PIQ_TYPE_FLOAT_VEC2,
    GLSLC_PIQ_TYPE_FLOAT_VEC3,
    GLSLC_PIQ_TYPE_FLOAT_VEC4,
    GLSLC_PIQ_TYPE_FLOAT16,
    GLSLC_PIQ_TYPE_FLOAT16_VEC2,
    GLSLC_PIQ_TYPE_FLOAT16_VEC3,
    GLSLC_PIQ_TYPE_FLOAT16_VEC4,
    GLSLC_PIQ_TYPE_DOUBLE,
    GLSLC_PIQ_TYPE_DOUBLE_VEC2,
    GLSLC_PIQ_TYPE_DOUBLE_VEC3,
    GLSLC_PIQ_TYPE_DOUBLE_VEC4,
    GLSLC_PIQ_TYPE_MAT2,
    GLSLC_PIQ_TYPE_MAT3,
    GLSLC_PIQ_TYPE_MAT4,
    GLSLC_PIQ_TYPE_MAT2X3,
    GLSLC_PIQ_TYPE_MAT2X4,
    GLSLC_PIQ_TYPE_MAT3X2,
    GLSLC_PIQ_TYPE_MAT3X4,
    GLSLC_PIQ_TYPE_MAT4X2,
    GLSLC_PIQ_TYPE_MAT4X3,
    GLSLC_PIQ_TYPE_DMAT2,
    GLSLC_PIQ_TYPE_DMAT3,
    GLSLC_PIQ_TYPE_DMAT4,
    GLSLC_PIQ_TYPE_DMAT2X3,
    GLSLC_PIQ_TYPE_DMAT2X4,
    GLSLC_PIQ_TYPE_DMAT3X2,
    GLSLC_PIQ_TYPE_DMAT3X4,
    GLSLC_PIQ_TYPE_DMAT4X2,
    GLSLC_PIQ_TYPE_DMAT4X3,
    GLSLC_PIQ_TYPE_SAMPLER_1D,
    GLSLC_PIQ_TYPE_SAMPLER_2D,
    GLSLC_PIQ_TYPE_SAMPLER_3D,
    GLSLC_PIQ_TYPE_SAMPLER_CUBE,
    GLSLC_PIQ_TYPE_SAMPLER_1D_SHADOW,
    GLSLC_PIQ_TYPE_SAMPLER_2D_SHADOW,
    GLSLC_PIQ_TYPE_SAMPLER_1D_ARRAY,
    GLSLC_PIQ_TYPE_SAMPLER_2D_ARRAY,
    GLSLC_PIQ_TYPE_SAMPLER_1D_ARRAY_SHADOW,
    GLSLC_PIQ_TYPE_SAMPLER_2D_ARRAY_SHADOW,
    GLSLC_PIQ_TYPE_SAMPLER_2D_MULTISAMPLE,
    GLSLC_PIQ_TYPE_SAMPLER_2D_MULTISAMPLE_ARRAY,
    GLSLC_PIQ_TYPE_SAMPLER_CUBE_SHADOW,
    GLSLC_PIQ_TYPE_SAMPLER_BUFFER,
    GLSLC_PIQ_TYPE_SAMPLER_2D_RECT,
    GLSLC_PIQ_TYPE_SAMPLER_2D_RECT_SHADOW,
    GLSLC_PIQ_TYPE_INT_SAMPLER_1D,
    GLSLC_PIQ_TYPE_INT_SAMPLER_2D,
    GLSLC_PIQ_TYPE_INT_SAMPLER_3D,
    GLSLC_PIQ_TYPE_INT_SAMPLER_CUBE,
    GLSLC_PIQ_TYPE_INT_SAMPLER_1D_ARRAY,
    GLSLC_PIQ_TYPE_INT_SAMPLER_2D_ARRAY,
    GLSLC_PIQ_TYPE_INT_SAMPLER_2D_MULTISAMPLE,
    GLSLC_PIQ_TYPE_INT_SAMPLER_2D_MULTISAMPLE_ARRAY,
    GLSLC_PIQ_TYPE_INT_SAMPLER_BUFFER,
    GLSLC_PIQ_TYPE_INT_SAMPLER_2D_RECT,
    GLSLC_PIQ_TYPE_UINT_SAMPLER_1D,
    GLSLC_PIQ_TYPE_UINT_SAMPLER_2D,
    GLSLC_PIQ_TYPE_UINT_SAMPLER_3D,
    GLSLC_PIQ_TYPE_UINT_SAMPLER_CUBE,
    GLSLC_PIQ_TYPE_UINT_SAMPLER_1D_ARRAY,
    GLSLC_PIQ_TYPE_UINT_SAMPLER_2D_ARRAY,
    GLSLC_PIQ_TYPE_UINT_SAMPLER_2D_MULTISAMPLE,
    GLSLC_PIQ_TYPE_UINT_SAMPLER_2D_MULTISAMPLE_ARRAY,
    GLSLC_PIQ_TYPE_UINT_SAMPLER_BUFFER,
    GLSLC_PIQ_TYPE_UINT_SAMPLER_2D_RECT,
    GLSLC_PIQ_TYPE_IMAGE_1D,
    GLSLC_PIQ_TYPE_IMAGE_2D,
    GLSLC_PIQ_TYPE_IMAGE_3D,
    GLSLC_PIQ_TYPE_IMAGE_2D_RECT,
    GLSLC_PIQ_TYPE_IMAGE_CUBE,
    GLSLC_PIQ_TYPE_IMAGE_BUFFER,
    GLSLC_PIQ_TYPE_IMAGE_1D_ARRAY,
    GLSLC_PIQ_TYPE_IMAGE_2D_ARRAY,
    GLSLC_PIQ_TYPE_IMAGE_CUBE_MAP_ARRAY,
    GLSLC_PIQ_TYPE_IMAGE_2D_MULTISAMPLE,
    GLSLC_PIQ_TYPE_IMAGE_2D_MULTISAMPLE_ARRAY,
    GLSLC_PIQ_TYPE_INT_IMAGE_1D,
    GLSLC_PIQ_TYPE_INT_IMAGE_2D,
    GLSLC_PIQ_TYPE_INT_IMAGE_3D,
    GLSLC_PIQ_TYPE_INT_IMAGE_2D_RECT,
    GLSLC_PIQ_TYPE_INT_IMAGE_CUBE,
    GLSLC_PIQ_TYPE_INT_IMAGE_BUFFER,
    GLSLC_PIQ_TYPE_INT_IMAGE_1D_ARRAY,
    GLSLC_PIQ_TYPE_INT_IMAGE_2D_ARRAY,
    GLSLC_PIQ_TYPE_INT_IMAGE_CUBE_MAP_ARRAY,
    GLSLC_PIQ_TYPE_INT_IMAGE_2D_MULTISAMPLE,
    GLSLC_PIQ_TYPE_INT_IMAGE_2D_MULTISAMPLE_ARRAY,
    GLSLC_PIQ_TYPE_UINT_IMAGE_1D,
    GLSLC_PIQ_TYPE_UINT_IMAGE_2D,
    GLSLC_PIQ_TYPE_UINT_IMAGE_3D,
    GLSLC_PIQ_TYPE_UINT_IMAGE_2D_RECT,
    GLSLC_PIQ_TYPE_UINT_IMAGE_CUBE,
    GLSLC_PIQ_TYPE_UINT_IMAGE_BUFFER,
    GLSLC_PIQ_TYPE_UINT_IMAGE_1D_ARRAY,
    GLSLC_PIQ_TYPE_UINT_IMAGE_2D_ARRAY,
    GLSLC_PIQ_TYPE_UINT_IMAGE_CUBE_MAP_ARRAY,
    GLSLC_PIQ_TYPE_UINT_IMAGE_2D_MULTISAMPLE,
    GLSLC_PIQ_TYPE_UINT_IMAGE_2D_MULTISAMPLE_ARRAY,
    GLSLC_PIQ_TYPE_SAMPLER_CUBE_MAP_ARRAY,
    GLSLC_PIQ_TYPE_INT_SAMPLER_CUBE_MAP_ARRAY,
    GLSLC_PIQ_TYPE_UINT_SAMPLER_CUBE_MAP_ARRAY,
    GLSLC_PIQ_TYPE_SAMPLER_CUBE_MAP_ARRAY_SHADOW,
    GLSLC_PIQ_TYPE_SAMPLER,
    GLSLC_PIQ_TYPE_TEXTURE_1D,
    GLSLC_PIQ_TYPE_TEXTURE_2D,
    GLSLC_PIQ_TYPE_TEXTURE_3D,
    GLSLC_PIQ_TYPE_TEXTURE_CUBE,
    GLSLC_PIQ_TYPE_TEXTURE_1D_SHADOW,
    GLSLC_PIQ_TYPE_TEXTURE_2D_SHADOW,
    GLSLC_PIQ_TYPE_TEXTURE_1D_ARRAY,
    GLSLC_PIQ_TYPE_TEXTURE_2D_ARRAY,
    GLSLC_PIQ_TYPE_TEXTURE_1D_ARRAY_SHADOW,
    GLSLC_PIQ_TYPE_TEXTURE_2D_ARRAY_SHADOW,
    GLSLC_PIQ_TYPE_TEXTURE_2D_MULTISAMPLE,
    GLSLC_PIQ_TYPE_TEXTURE_2D_MULTISAMPLE_ARRAY,
    GLSLC_PIQ_TYPE_TEXTURE_CUBE_SHADOW,
    GLSLC_PIQ_TYPE_TEXTURE_BUFFER,
    GLSLC_PIQ_TYPE_TEXTURE_2D_RECT,
    GLSLC_PIQ_TYPE_TEXTURE_2D_RECT_SHADOW,
    GLSLC_PIQ_TYPE_TEXTURE_CUBE_MAP_ARRAY,
    GLSLC_PIQ_TYPE_TEXTURE_CUBE_MAP_ARRAY_SHADOW,
    GLSLC_PIQ_TYPE_INT_TEXTURE_1D,
    GLSLC_PIQ_TYPE_INT_TEXTURE_2D,
    GLSLC_PIQ_TYPE_INT_TEXTURE_3D,
    GLSLC_PIQ_TYPE_INT_TEXTURE_CUBE,
    GLSLC_PIQ_TYPE_INT_TEXTURE_1D_ARRAY,
    GLSLC_PIQ_TYPE_INT_TEXTURE_2D_ARRAY,
    GLSLC_PIQ_TYPE_INT_TEXTURE_2D_MULTISAMPLE,
    GLSLC_PIQ_TYPE_INT_TEXTURE_2D_MULTISAMPLE_ARRAY,
    GLSLC_PIQ_TYPE_INT_TEXTURE_BUFFER,
    GLSLC_PIQ_TYPE_INT_TEXTURE_2D_RECT,
    GLSLC_PIQ_TYPE_INT_TEXTURE_CUBE_MAP_ARRAY,
    GLSLC_PIQ_TYPE_UINT_TEXTURE_1D,
    GLSLC_PIQ_TYPE_UINT_TEXTURE_2D,
    GLSLC_PIQ_TYPE_UINT_TEXTURE_3D,
    GLSLC_PIQ_TYPE_UINT_TEXTURE_CUBE,
    GLSLC_PIQ_TYPE_UINT_TEXTURE_1D_ARRAY,
    GLSLC_PIQ_TYPE_UINT_TEXTURE_2D_ARRAY,
    GLSLC_PIQ_TYPE_UINT_TEXTURE_2D_MULTISAMPLE,
    GLSLC_PIQ_TYPE_UINT_TEXTURE_2D_MULTISAMPLE_ARRAY,
    GLSLC_PIQ_TYPE_UINT_TEXTURE_BUFFER,
    GLSLC_PIQ_TYPE_UINT_TEXTURE_2D_RECT,
    GLSLC_PIQ_TYPE_UINT_TEXTURE_CUBE_MAP_ARRAY,

    GLSLC_PIQ_INVALID_TYPE = 0x7FFFFFFF,
};

/// \brief Controls the amount of debug information to generate.
typedef enum GLSLCdebugInfoLevel_enum {
    /// No debug information generated.
    GLSLC_DEBUG_LEVEL_NONE = 0,

    /// Minimal debug information required for GLSL source debugging.
    GLSLC_DEBUG_LEVEL_G0   = 1,

    /// Minimal debug information for GLSL profiling.
    GLSLC_DEBUG_LEVEL_G1   = 2,

    /// Full debug information, no optimizations.
    GLSLC_DEBUG_LEVEL_G2   = 3,
} GLSLCdebugInfoLevelEnum;

/// \brief Language the shaders should be compiled with.
typedef enum GLSLClanguageTypeEnum_enum {
    /// OpenGL shading language (default)
    GLSLC_LANGUAGE_GLSL = 0,

    /// OpenGL ES shading language
    GLSLC_LANGUAGE_GLES = 1,

    /// SPIR-V compilation
    GLSLC_LANGUAGE_SPIRV = 2,
} GLSLClanguageTypeEnum;


/// \brief Spill control options.
typedef enum SpillControlEnum_enum {
    /// Default spill behavior
    DEFAULT_SPILL = 0,

    /// Disable spilling
    NO_SPILL = 1,
} SpillControlEnum;

/// \brief Compiler optimization levels.
typedef enum GLSLCoptLevel_enum {
    /// Default optimization level.
    GLSLC_OPTLEVEL_DEFAULT = 0,

    /// No optimization will be performed.
    GLSLC_OPTLEVEL_NONE = 1,
} GLSLCoptLevelEnum;

/// \brief Compiler loop unrolling control
typedef enum GLSLCunrollControl_enum {

    /// Default optimizations related to loop unrolling.
    GLSLC_LOOP_UNROLL_DEFAULT = 0,

    /// Don't unroll loops.
    GLSLC_LOOP_UNROLL_NONE = 1,

    /// Force all possible unrolling of loops.
    GLSLC_LOOP_UNROLL_ALL = 2
} GLSLCunrollControlEnum;

/// \brief Uninitialized variable warning control
typedef enum GLSLCwarnUninitControl_enum {

    /// Default level, warns for some uninitialized variables
    GLSLC_WARN_UNINIT_DEFAULT = 0,

    /// Don't generate warning for uninitialized variables
    GLSLC_WARN_UNINIT_NONE = 1,

    /// Warns on all possible uninitialized variables, higher
    /// compile time cost
    GLSLC_WARN_UNINIT_ALL = 2

} GLSLCwarnUninitControlEnum;

/// \brief Types for individual sections.
typedef enum GLSLCsectionTypeEnum_enum {
    /// GPU code section.
    GLSLC_SECTION_TYPE_GPU_CODE,

    /// Assembly dump section.
    GLSLC_SECTION_TYPE_ASM_DUMP,

    /// Perf statistics section
    GLSLC_SECTION_TYPE_PERF_STATS,

    /// Reflection section
    GLSLC_SECTION_TYPE_REFLECTION,

    /// Debug info section.
    GLSLC_SECTION_TYPE_DEBUG_INFO,

    /// Number of section types.
    GLSLC_NUM_SECTION_TYPES,

    /// Ensure the size of this enum is 32 bits.
    GLSLC_SECTION_TYPE_ENUM_LARGE = 0X7FFFFFFF
} GLSLCsectionTypeEnum;

/// \brief Structure used to access the name for the each variable.
/// _nameOffset_ is used to index into the string buffer.
typedef struct GLSLCpiqName_rec {
    /// Offset of the name into the data string pool.
    uint32_t nameOffset;

    /// Name length
    uint32_t nameLength;

    GLSLC_RESERVED_BYTES(32);

} GLSLCpiqName;

/// \brief An information data section corresponding to uniform blocks.
typedef struct GLSLCuniformBlockInfo_rec {
    /// Info about the name in the data string pool.
    GLSLCpiqName nameInfo;

    /// Data size (in bytes) of this uniform block.
    uint32_t size;

    /// Number of active variables defined in this uniform block.
    uint32_t numActiveVariables;

    /// Stage mask representing which stages this variable is active in.
    NVNshaderStageBits stagesReferencedIn;

    /// Bindings associated with this block in each shader stage, where
    /// array index _n_ corresponds to NVNshaderStage _n_.
    /// If the block is unused in a particular shader stage, -1 is
    /// output for that array entry.
    /// The _stagesReferencedIn_ member can be used to determine which
    /// array entries contain valid binding values.
    int32_t bindings[GLSLC_NUM_SHADER_STAGES];

    GLSLC_RESERVED_BYTES(32);
} GLSLCuniformBlockInfo;


/// \brief An information data section corresponding to individual uniforms
typedef struct GLSLCuniform_rec {
    /// Info about the name in the data string pool.
    GLSLCpiqName nameInfo;

    /// The data type of this uniform.
    GLSLCpiqTypeEnum type;

    /// If part of a uniform block, the ndx to the corresponding uniform
    /// block.  -1 if not defined in a uniform block.
    int32_t  blockNdx;

    /// Byte offset into the uniform block. -1 if not defined in a uniform block.
    int32_t  blockOffset;

    /// Number of array elements if the uniform is an array, 1 otherwise.
    uint32_t sizeOfArray;

    /// Stride in bytes between uniforms in the array.
    uint32_t arrayStride;

    /// Specifies stride between columns/rows of matrix depending on _isRowMajor_.
    /// -1 if not backed by a buffer.
    int32_t matrixStride;

    /// Specifies whether row major or column major.
    uint32_t isRowMajor;

    /// Stage mask representing which stages this variable is active in.
    NVNshaderStageBits stagesReferencedIn;

    /// Bindings associated with this uniform in each shader stage, where
    /// array ndx _n_ corresponds to NVNshaderStage _n_.
    /// If the unifom is unused in a particular shader stage, or the uniform
    /// is part of a uniform block, -1 is output for that array entry.
    /// Valid binding entries _= 0 will only be available if the uniform is an
    /// image or sampler which is not in a uniform block, indicated by the _isInUBO_
    /// member.
    /// The _stagesReferencedIn_ member can be used to determine which
    /// array entries contain valid binding values for sampler/image type uniforms.
    int32_t  bindings[GLSLC_NUM_SHADER_STAGES];

    /// Indicates if the uniform is a sampler, image, or normal uniform.
    GLSLCpiqUniformKind kind;

    /// 1 if this uniform is in a uniform block, 0 othersize.
    uint8_t isInUBO;

    /// 1 if this uniform is an array, 0 if otherwise.
    uint8_t isArray;

    GLSLC_RESERVED_BYTES(62);
} GLSLCuniformInfo;

/// \brief An information data section corresponding to individual subroutines.
typedef struct GLSLCsubroutineInfo_rec {
    /// Info about the name in the data string pool.
    GLSLCpiqName nameInfo;

    /// Index value for the subroutine.
    int32_t index;

    /// Shader stage this subroutine corresponds to.
    NVNshaderStage stage;

    GLSLC_RESERVED_BYTES(16);
} GLSLCsubroutineInfo;

/// \brief An information data section corresponding to individual subroutine uniforms.
typedef struct GLSLCsubroutineUniformInfo_rec {
    /// Info about the name in the data string pool.
    GLSLCpiqName nameInfo;

    /// Number of compatible subroutines for this subroutine uniform.
    uint32_t numCompatibleSubroutines;

    /// Offset (in bytes) of the compatible subroutines into the compatible subroutine index pool.
    /// The total number of NvU32 values in the pool for this subroutine uniform is
    /// denoted by _numCompatibleSubroutines_
    uint32_t compatibleSubroutineInfoOffset;

    /// Locations for the subroutine uniform.
    int32_t location;

    /// Size of the array in number of entries, or 1 if not an array.
    uint32_t sizeOfArray;

    /// Shader stage this subroutine uniform corresponds to.
    NVNshaderStage stage;

    /// 1 if this uniform is an array, 0 if otherwise
    uint8_t isArray;

    GLSLC_RESERVED_BYTES(35);
} GLSLCsubroutineUniformInfo;

/// \brief An information data section corresponding to individual attributes.
typedef struct GLSLCProgramInputInfo_rec {
    /// Info about the name in the data string pool.
    GLSLCpiqName nameInfo;

    /// The data type of this input.
    GLSLCpiqTypeEnum type;

    /// Number of array elements if the input is an array, 1 otherwise.
    uint32_t sizeOfArray;

    /// Location this input is assigned to.  -1 if not a vert/frag program
    /// input.
    int32_t location;

    /// Stage mask representing which stages this variable is active in.
    NVNshaderStageBits stagesReferencedIn;

    /// 1 if this input is an array, 0 if otherwise.
    uint8_t isArray;

    /// 1 if this input is per patch, 0 otherwise.
    uint8_t isPerPatch;

    GLSLC_RESERVED_BYTES(34);
} GLSLCprogramInputInfo;


/// \brief An information data section corresponding to program output variables.
typedef struct GLSLCProgramOutput_rec {
    /// Info about the name in the data string pool.
    GLSLCpiqName nameInfo;

    /// The data type of this program output.
    GLSLCpiqTypeEnum type;

    /// Number of array elements if the program output is an array, 1 otherwise.
    uint32_t sizeOfArray;

    /// Location this output is assigned to.  -1 if not a user-assigned or a vertex/fragment
    /// input/output.
    int32_t location;

    /// Identifies the fragment color index of the output variable.  If this is
    /// for a non-fragment shader, -1 is written.
    int32_t locationNdx;

    /// Stage mask representing which stages this variable is active in.
    NVNshaderStageBits stagesReferencedIn;

    /// 1 if this output is an array, 0 if otherwise.
    uint8_t isArray;

    /// 1 if this output is per patch, 0 otherwise.
    uint8_t isPerPatch;

    GLSLC_RESERVED_BYTES(30);
} GLSLCprogramOutputInfo;

/// \brief An information data section corresponding to individual shader storage blocks.
typedef struct GLSLCssboInfo_rec {
    /// Info about the name in the data string pool.
    GLSLCpiqName nameInfo;

    /// Size (in bytes) of the shader storage block.
    uint32_t size;

    /// Number of active variables assigned to the shader storage block.
    uint32_t numActiveVariables;

    /// Bindings associated with this SSBO in each shader stage, where
    /// array ndx _n_ corresponds to NVNshaderStage _n_.
    /// If the SSBO is unused in a particular shader stage, -1 is
    /// output for that array entry.
    /// The _stagesReferencedIn_ member can be used to determine which
    /// array entries contain valid binding values.
    int32_t bindings[GLSLC_NUM_SHADER_STAGES];

    /// Stage mask representing which stages this variable is active in.
    NVNshaderStageBits stagesReferencedIn;

    GLSLC_RESERVED_BYTES(32);
} GLSLCssboInfo;


/// \brief An information data section corresponding to individual buffer variables.
typedef struct GLSLCbufferVariableInfo_rec {
    /// Info about the name in the data string pool.
    GLSLCpiqName nameInfo;

    /// The data type of this buffer variable.
    GLSLCpiqTypeEnum type;

    /// The index into the corresponding buffer.
    int32_t  blockNdx;

    /// The data offset (in bytes) of this variable inside it's owning buffer.
    int32_t  blockOffset;


    /// Size of the array if the buffer variable is an array, otherwise 1.
    uint32_t sizeOfArray;

    uint32_t arrayStride;

    /// Specifies stride between columns/rows of matrix depending on _isRowMajor_.
    /// -1 if not backed by a buffer.
    int32_t matrixStride;

    /// Specifies whether row major or column major.
    uint32_t isRowMajor;

    /// Number of active elements of the top-level shader storage block
    /// member containing this variable.  If the top-level shader storage block
    /// is not an array, 1 is output.  If top-level block member is an array
    /// with no declared size, the value 0 is written.
    uint32_t topLevelArraySize;

    /// Stride between elements of the top-level shader storage block member
    /// containing this active variable. For top-level shader storage blocks
    /// delcared as arrays, value is the differnce in basic machine units between
    /// the offset of the active variable for consecutive elements in the top-level array.
    /// For top-level block members not declared as an array, 0 is assigned.
    uint32_t topLevelArrayStride;

    /// Stage mask representing which stages this variable is active in.
    NVNshaderStageBits stagesReferencedIn;

    /// 1 if this buffer variable is an array, otherwise 0
    uint8_t isArray;

    GLSLC_RESERVED_BYTES(31);
} GLSLCbufferVariableInfo;


/// \brief An information data section corresponding to an individual transform feedback varying.
typedef struct GLSLCxfbVaryingInfo_rec {
    /// Info about the name in the data string pool.
    GLSLCpiqName nameInfo;

    /// The data type of this transform feedback varying.
    GLSLCpiqTypeEnum type;

    /// Size of the array if the transform feedback varying is an array, otherwise 1.
    uint32_t sizeOfArray;

    /// 1 If this transform feedback varying is an array, 0 otherwise.
    uint8_t isArray;

    GLSLC_RESERVED_BYTES(31);
} GLSLCxfbVaryingInfo;

/// \brief Options for specifying a set of shader outputs to capture for transform feedback.
///
/// Each string must specify the name of an output in the last vertex,
/// tessellation, or geometry shader in the program.  For each vertex
/// capture, values are written to transform feedback buffer(s) in the
/// order varyings are specified in the array.  The special strings
/// 'gl_SkipComponents[1234]' can be used to specify that one to four
/// words in the output buffer should be skipped for each vertex prior
/// to writing the next output.  The special string 'gl_NextBuffer' can
/// be used to specify that subsequent variables should be captured to
/// the next buffer binding point.
typedef struct GLSLCxfbInfo_rec {

    /// Array of strings, one for each varying.
    GLSLC_POINTER_MEMBER(const char * const *, varyings);

    /// Number of varyings that are included in the _varyings_ variable.
    uint32_t numVaryings;

    GLSLC_RESERVED_BYTES(32);
} GLSLCxfbInfo;



/// \brief Options for setting the include search paths.
typedef struct GLSLCincludeInfo_rec {
    /// Array of strings, one for each include path.
    /// NOTE: Only UTF-8 encoded path strings are supported.
    GLSLC_POINTER_MEMBER(const char * const *, paths);

    /// Number of include paths that are included in the _paths_ variable.
    uint32_t numPaths;

    // if set to 1, the search paths <paths> will be used first
    // to look for files before looking in the current working directory.
    // if set to 0, the current working directory will be searched
    // first before looking through the provided include paths.
    uint32_t useIncludePathsFirst : 1;

    GLSLC_RESERVED_BYTES(28);
} GLSLCincludeInfo;


/// \brief Each element corresponds to a single uniform. The layout of the data should
/// correspond to how the shader expects uniform values to be stored in the
/// uniform buffer.
///
/// For example: if the array represents an array of uniforms of type "vec3", and the
/// array is called "ary" in the shader with a layout using std140 rules, the data
/// would look like the following:
///
/// uniformName = "ary"
/// val = {uni[0].x, uni[0].y, uni[0].z, _unused_, uni[1].x, uni[1].y, uni[1].z, _unused_}
/// numElements = 2
/// elementSize = 16
typedef struct GLSLCspecializationUniform_rec {
    /// A pointer to the application-provided data
    ///
    /// The data should be laid out as NVN expects (either by using known layouts like std140, or by querying the reflection information)
    ///
    /// NOTE: The number of elements in the array is determined by the reflection information about this
    /// uniform from GLSLC after calling "glslcCompilePreSpecialized", or from the shader itself.  It is an error to specify an array with more
    /// elements than what the shader is actually coded with.
    GLSLC_POINTER_MEMBER(void *, values);

    /// Name of the uniform
    /// Array base name if specifying an array.  The naming scheme follows the same naming scheme
    /// as specified in ARB_program_interface_query
    ///
    /// It is an error to specify uniforms that don't exist or are not active in any stage UBOs
    /// (this information is available through the reflection section).
    ///
    /// Specifying the same uniform name more than once will result in the last uniform definition
    /// being used.
    GLSLC_POINTER_MEMBER(const char *, uniformName);

    /// Number of uniforms in the array.  For single components (ex vec2, vec4, mat4, etc), this should always
    /// be 1.  For arrays, it denotes the number of uniforms in the array.  This information must match the reflection
    /// data or else an error will be generated.
    uint32_t numElements;

    /// Size of each uniform in the array in bytes.  Note, GLSLC expects the memory layout as specified by the shader, and what NVN would expect for the
    /// input buffer.
    /// For instance, if "layout std140" is used on a block, and a uniform of type vec3 is specified, the size will be 16 bytes as denoted
    /// by the std140 rules.  In other words, this size should match the parameters of the buffer backing the uniform object.
    uint8_t elementSize;

    GLSLC_RESERVED_BYTES(31);
} GLSLCspecializationUniform;

/// \brief Used to specify a set of uniforms to be specialized.
typedef struct GLSLCspecializationSet_rec {
    /// An array of GLSLCspeciailzationUniform entries.
    GLSLC_POINTER_MEMBER(const GLSLCspecializationUniform *, uniforms);

    /// The number of uniforms in the array _uniforms.
    uint32_t numUniforms;

    GLSLC_RESERVED_BYTES(32);
} GLSLCspecializationSet;

/// \brief Used for "batch" mode processing, contains multiple specialization
/// sets.
typedef struct GLSLCspecializationBatch_rec {
    /// An array of specialization entry sets.
    GLSLC_POINTER_MEMBER(const GLSLCspecializationSet *, entries);

    /// Number of sets in the array _entries_
    uint32_t numEntries;

    GLSLC_RESERVED_BYTES(32);
} GLSLCspecializationBatch;


/// \brief Option flags that control compilation and GLSLC output.
///
/// This struct is used as part of the GLSLCoptions struct.
typedef struct GLSLCoptionFlags_rec {
    /// Treat the GLSLC object as separable
    uint32_t glslSeparable: 1;

    /// Include an assembly dump section in the GLSLCoutput.  There will
    /// be a separate assembly dump section for each shader stage.
    uint32_t outputAssembly: 1;

    /// Output individual program binary sections in the GLSLCoutput.
    /// There will be one binary per stage.
    uint32_t outputGpuBinaries: 1;

    /// Output individual perf statistics for each compiled binary.
    uint32_t outputPerfStats: 1;

    /// Output shader reflection data.
    uint32_t outputShaderReflection: 1;

    /// Specify the language of the input shaders.
    GLSLClanguageTypeEnum language: 4;

    /// Level of debug information to generate.
    GLSLCdebugInfoLevelEnum outputDebugInfo: 4;

    /// Control amount of spilling.
    SpillControlEnum spillControl : 4;

    /// Controls whether the resulting output contains a GPU program that is a thin binary (only contains
    /// program code for NX platforms).
    uint32_t outputThinGpuBinaries : 1;

    /// Compiles tessellation evaluation and passthrough geometry shaders
    /// (NV_geometry_shader_passthrough) to work together as a pair.  When
    /// a tessellation evaluation or passthrough geometry shader compiled
    /// with this option is bound for rendering, the other stage must also
    /// have a shader compiled with this option bound.  Using such shaders
    /// without a similarly compiled shader in the other stage will produce
    /// undefined results.
    uint32_t tessellationAndPassthroughGS : 1;

    /// Optimize texture batches for intra-warp cache locality in compute shaders.
    /// Does not affect non-compute shaders.
    uint32_t prioritizeConsecutiveTextureInstructions : 1;

    /// A mask indicate which shader stages to enable fast math optimizations.  Fast math optimizations
    /// allow the compiler to make decisions which might not strictly adhere to IEEE standards.  The mask
    /// corresponds to an NVNshaderStageBits type.
    /// 
    /// NOTE: The function glslcGetDefaultOptions will enable fast math optimizations in fragment shaders
    /// by default (i.e. will initialize enableFastMathMask with a value of NVN_SHADER_STAGE_FRAGMENT_BIT).
    ///
    /// NOTE: It is recommended that fast math be enabled for fragment shaders only, while other stages
    /// such as vertex/geometry/tessellation/compute might be more sensitive to precision differences in
    /// the presence of fast math optimizations and might produce incorrect results.
    uint32_t enableFastMathMask : 6;

    /// Optimization level for which to compile the shaders.
    GLSLCoptLevelEnum optLevel : 3;

    /// Option for controlling loop unrolling behavior.
    GLSLCunrollControlEnum unrollControl : 3;

    /// Option to error out if the shader uses scratch memory to store data
    uint32_t errorOnScratchMemUsage : 1;

    /// Option to compile the shader using the cull-before-fetch optimization
    /// Refer to the GLSLC programming guide for more details
    uint32_t enableCBFOptimization : 1;

    /// Option to compile the shader using the warp culling optimization
    /// Refer to the GLSLC programming guide for more details
    uint32_t enableWarpCulling : 1;

    /// Option to enable multithreaded compilation using separate GLSLCcompileObject.
    /// **As of package version 50, up to 64 concurrent GLSLCcompileObjects can have this enabled.
    /// Prior to package version 50, up to 16 concurrent GLSLCcompileObjects can have this enabled.
    /// Use the GLSLC API function glslcGetVersion to query the library's package version.**
    uint32_t enableMultithreadCompilation : 1;

    /// Option to control warning when compiling shaders with uninitialized variables
    /// Refer to GLSLC programming guide for more details
    GLSLCwarnUninitControlEnum warnUninitControl : 3;

} GLSLCoptionFlags;

/// \brief A structure to enable and use debug labels when boolprinting shader
/// compilation warning/error logs for semantic compilation errors.  The
/// per-stage debug labels will prefix the warning/error log in the output
/// log for each particular shader.
/// If _enableErrorLabels_ is 1, and _labels_ is NULL, the shaders will
/// be prefixed with their pointer hexidecimal value consisting of 16 characters.
/// If _enableErrorLabels_ is 1, and _labels_ is non-NULL, the shaders
/// will be prefixed with the label string for the corresponding stage in
/// _labels_.
/// If _enableErrorLabels_ is 0, no labels will be prefixed on the compiler
/// warnings/error log.
typedef struct GLSLCshaderLabels_rec {
    /// Labels for the shaders to use when outputting compiler errors/warnings.
    /// The array size and label entries corresponds to
    /// GLSLCinput::sources entries (one label per input shader stage).
    /// The label will be printed out once and pre-pend the compiler semantic
    /// errors and warnings.
    /// Note: If this is NULL, the GLSLCinput::sources pointer values will
    /// be used as labels instead printed as 16 character hexadecimal strings.
    GLSLC_POINTER_MEMBER(const char * const *, compileLogLabels);

    // Set to true to enable labeling the output compile log.  If this is false (default),
    // no labeling will be used.
    uint32_t enableCompileLogLabels: 1;

    GLSLC_RESERVED_BYTES(12);

} GLSLCshaderLabels;

/// \brief Options structure to control compilation phase, what gets output
/// in the GLSLCOutput structure
typedef struct GLSLCoptions_rec {
    /// Include standard header file. If this is NULL, no standard header will be included.
    GLSLC_POINTER_MEMBER(const char *, forceIncludeStdHeader);

    /// A structure used for controlling which outputs are emitted in the GLSLCoutput
    /// section and for specifying which options should be used for compilation.
    GLSLCoptionFlags optionFlags;

    /// A structure used for setting include paths.
    GLSLCincludeInfo includeInfo;

    /// A structure used for specifying transform feedback information.
    GLSLCxfbInfo xfbVaryingInfo;

    /// Optional debug labels to prefix compiler errors/warnings.
    GLSLCshaderLabels debugLabels;

    GLSLC_RESERVED_BYTES(8);
} GLSLCoptions;


/// \brief Status of the compilation.
///
/// This struct is used to contain the info log which any compilation errors
/// or warnings. All characters returned through the info log are in ASCII
/// encoding.
typedef struct GLSLCcompilationStatus_rec {
    /// A pointer returned by GLSLC pointing to the info log.

    GLSLC_POINTER_MEMBER(const char *, infoLog);

    /// The number of characters that GLSLC outputs to the info log,
    /// excluding the terminating null character.
    uint32_t infoLogLength;

    /// Indicates whether the compilation was successful or not.  A value of 1 indicates success,
    /// a value of 0 indicates failure.
    uint8_t success;

    /// Indicates whether an allocation error ocurred during execution of GLSLC.
    uint8_t allocError;

    /// Indicates whether the results were from specialization using the multi-threaded variant
    /// glslcCompileSpecializedMT function.
    uint8_t usedMTSpecialization;

    GLSLC_RESERVED_BYTES(1);

    /// Number of entries in the batch compilation these results are from.  If specialization wasn't used,
    /// this number should be 1.  If specialization was used, the number indicates the total entries specified
    /// through the glslcCompileSpecialized or glslcCompileSpecializedMT functions.
    uint32_t numEntriesInBatch;

    GLSLC_RESERVED_BYTES2(24);
} GLSLCcompilationStatus;

/// \brief A structure used for specifying a shader stage's SPIR-V constant specialization
/// data.
typedef struct GLSLCspirvSpecializationInfo_rec {
    /// Array of specialization constant IDs.  Each entry in this array corresponds to
    /// the entries in the array _data_.
    GLSLC_POINTER_MEMBER(const uint32_t *, constantIDs);

    /// Specialization data array.  Each entry should be a numeric value represented by
    /// 4 bytes.
    GLSLC_POINTER_MEMBER(const void *, data);

    /// Number of specialization constants in the arrays _constantIDs_ and _data_.
    uint32_t numEntries;

    GLSLC_RESERVED_BYTES(20);
} GLSLCspirvSpecializationInfo;


/// \brief A structure used as input into GLSLC functions.
///
/// This structure is for handling input data.  All strings should be null-terminated.
typedef struct GLSLCinput_rec {
    /// An array of shader strings.  GLSLC will use these strings as source for compilation.
    /// If the GLSLCoptionFlags::language flag is GLSLC_LANGUAGE_GLSL or GLSL_LANGUAGE_GLES,
    /// the sources are interpreted as UTF-8 strings.
    /// If the GLSLCoptionFlags::language flag is GLSLC_LANGUAGE_SPIRV, the sources are
    /// interpreted as binary data containing SPIR-V modules.
    GLSLC_POINTER_MEMBER(const char * const *, sources);

    /// Shader stage array, such that stages[i] corresponds to sources[i].
    GLSLC_POINTER_MEMBER(const NVNshaderStage *, stages);

    /// Number of entries in both the _sources_ and _stages_ arrays.
    uint8_t count;

    GLSLC_RESERVED_BYTES(7);

    /// Names for entry points corresponding to each SPIR-V module.  Each entry in the
    /// array corresponds to the SPIR-V module pointed to by the corresponding entry in _sources_.
    /// If the GLSLCoptionFlags::language flag is not GLSLC_LANGUAGE_SPIRV, this field is ignored.
    GLSLC_POINTER_MEMBER(const char * const *, spirvEntryPointNames);

    /// An array of sizes (in bytes) for each input SPIRV module binary data pointed
    /// to by the corresponding entry in _sources_.
    /// If the GLSLCoptionFlags::language flag is not GLSLC_LANGUAGE_SPIRV, this field is ignored.
    GLSLC_POINTER_MEMBER(const uint32_t *, spirvModuleSizes);

    /// An array of SPIR-V specialization information structures.  Each entry in the array corresponds to
    /// each SPIR-V module stage.  If this is NULL, then it will be ignored by the compiler.
    GLSLC_POINTER_MEMBER(const GLSLCspirvSpecializationInfo * const *, spirvSpecInfo);
} GLSLCinput;

/// \brief Common section header included at the beginning of each section
typedef struct GLSLCsectionHeaderCommon_rec {
    /// Total size of this section's data (in bytes).
    uint32_t size;

    /// Offset of the data section, in bytes, relative to the top-level structure containing the section header
    /// and data. This is typically the GLSLCoutput structure, but could also be the _reflectionSection_ member
    /// in the compile object.
    uint32_t dataOffset;

    /// Type of section.
    GLSLCsectionTypeEnum type;

    GLSLC_RESERVED_BYTES(32);
} GLSLCsectionHeaderCommon;

/// \brief Info specific to compute shaders.
typedef struct GLSLCshaderInfoCompute_rec {
    /// The work group size in 3 dimensions.  If variable workgroup sizes are used, the output
    /// for each dimension is 0.
    uint32_t workGroupSize[3];

    uint8_t reserved[32];
} GLSLCshaderInfoCompute;

/// \brief Aunion containing individual shader stage info structures.
typedef union GLSLCshaderInfoUnion_rec {
    /// Info for compute shaders.
    GLSLCshaderInfoCompute compute;
} GLSLCshaderInfoUnion;


/// \brief Contains entries for outputting shader info for each shader stage.
///
/// There will be one of these structures output per reflection section.  Entries
/// are only filled in for active stages while inactive stage entries will just
/// contain the value 0 for all entries.
typedef struct GLSLCperStageShaderInfo_rec {

    /// Shader info containing stage-specific information.
    GLSLCshaderInfoUnion shaderInfo[GLSLC_NUM_SHADER_STAGES];
} GLSLCperStageShaderInfo;

/// \brief Program reflection data header.
///
/// This section contains reflection information about
/// the compiled programs.  Each type of variable has an associated offset into
/// the data section where the start variable can be found.
/// NOTE: The common.dataOffset member of this structure corresponds to the beginning
/// of a GLSLCoutput structure if it is part of a GLSLCoutput block, but if the reflection
/// header is from the compile object, this offset is from the beginning of the header
/// itself.
typedef struct GLSLCprogramReflectionHeader_rec {
    GLSLCsectionHeaderCommon common;

    /// Number of uniform blocks
    uint32_t numUniformBlocks;

    /// Offset into data section for the uniform block reflection data.
    uint32_t uniformBlockOffset;

    /// Number of uniforms.
    uint32_t numUniforms;

    /// Offset into data section for the uniform reflection data.
    uint32_t uniformOffset;

    /// Number of program inputs.
    uint32_t numProgramInputs;

    /// Offset into data section for the program input.
    uint32_t programInputsOffset;

    /// Number of program outputs
    uint32_t numProgramOutputs;

    /// Offset into data section for the program output
    uint32_t programOutputsOffset;

    /// Number of shader storage buffers.
    uint32_t numSsbo;

    /// Offset into data section for the shader storage block data.
    uint32_t ssboOffset;

    /// Number of buffer variables.
    uint32_t numBufferVariables;

    /// Offset into data section for the buffer variable data.
    uint32_t bufferVariableOffset;

    /// Number of transform feedback varyings.
    uint32_t numXfbVaryings;

    /// Offset into data section for the transform feedback varying data.
    uint32_t xfbVaryingsOffset;

    /// Size of the string pool
    uint32_t stringPoolSize;

    /// Offset into data section for the string pool data.
    uint32_t stringPoolOffset;

    /// Offset into data section for the per-stage shader information.
    uint32_t shaderInfoOffset;

    /// Number of subroutines in the program in all stages.
    uint32_t numSubroutines;

    /// Offset into the data section for the GLSLCsubroutineInfo headers.
    uint32_t subroutineOffset;

    /// Number of subroutine uniforms in the program in all stages.
    uint32_t numSubroutineUniforms;

    /// Offset into the data section for the GLSLCsubroutineUniformInfo headers.
    uint32_t subroutineUniformOffset;

    /// Size of the pool (in bytes) containing index values for subroutine uniforms corresponding
    /// to the subroutines they are compatible with.  The actual offset into this pool for
    /// a specific subroutine uniform should be determined from the
    /// GLSLCsubroutineUniformInfo::compatibleSubroutineInfoOffset. Each index in the pool
    /// is a uint32_t value.
    uint32_t subroutineCompatibleIndexPoolSize;

    /// Offset into the data section to the start of the compatible index pool data.
    uint32_t subroutineCompatibleIndexPoolOffset;

    GLSLC_RESERVED_BYTES(4);
} GLSLCprogramReflectionHeader;

/// \brief GPU code section header containing GPU code for an individual shader stage.
typedef struct GLSLCgpuCodeHeader_rec {
    GLSLCsectionHeaderCommon common;

    /// The shader stage this corresponds to.
    NVNshaderStage stage;

    /// The offset within the data of the control section, in bytes.
    uint32_t controlOffset;

    /// The offset within the data of the GPU code data section in bytes.
    uint32_t dataOffset;

    /// The size of the data portion of the stream in bytes.
    uint32_t dataSize;

    /// The size of the control portion of the stream in bytes.
    uint32_t controlSize;

    /// The amount of local memory (in bytes per warp) required for this program.  This is used to compute
    /// the total amount of scratch memory needed for a program depending on the platform.  See the NVN programming
    /// guide's section on shader scratch memory to see how this value is used by the application.
    uint32_t scratchMemBytesPerWarp;

    /// The total amount of local memory recommended for this program on a
    /// NX device.
    ///
    /// \warning This value _SHOULD NOT_ be used on the Windows reference NVN platform.
    ///
    /// GLSLC calculates this value using the formula documented in the NVN
    /// programming guide for computing scratch memory requirements from queried
    /// device properties and the scratchMemBytesPerWarp variable, where the
    /// device property values used in the formula are the same device property
    /// values that NVN would return if running on a NX device.  The value
    /// is only included in this output for convenience.
    uint32_t scratchMemBytesRecommended;

    /// Section index for the corresponding assembly dump (if requested).
    uint32_t asmDumpSectionIdx;

    /// Section index for the corresponding perf stats (if requested).
    uint32_t perfStatsSectionNdx;

    /// Subroutine linkage map offset (in bytes) into the data section.  The
    /// subroutine linkage map is an opaque structure that is provided to the NVN
    /// driver by the application via a call to nvnProgramSetSubroutineLinkage.
    /// The driver uses the information stored in this map to determine which
    /// entries in an internal jump table stored in constant memory need to be
    /// patched, and also which values they should be patched with by mapping
    /// from subroutine uniform binding inputs in a
    /// nvnCommandBufferSetProgramSubroutines call to the actual code locations
    /// for the subroutines in the program.
    uint32_t subroutineLinkageMapOffset;

    /// Subroutine linkage map size (in bytes).  This is 0 if there are no active subroutines used by the
    /// program.
    uint32_t subroutineLinkageMapSize;

    GLSLC_RESERVED_BYTES(56);
} GLSLCgpuCodeHeader;


/// \brief Assembly dump section header corresponding to the textual assembly of a single
/// stage's GPU code.
typedef struct GLSLCasmDumpHeader_rec {
    GLSLCsectionHeaderCommon common;

    // Shader stage this ASM dump corresponds to
    NVNshaderStage stage;

    GLSLC_RESERVED_BYTES(60);
} GLSLCasmDumpHeader;

/// \brief Generic header allowing access to the common data structure.
typedef struct GLSLCgenericHeader_rec {
    GLSLCsectionHeaderCommon common;

    GLSLC_RESERVED_BYTES(32);
} GLSLCgenericHeader;

/// \brief Header for perf statistics.
typedef struct GLSLCperfStatsHeader_rec {
    GLSLCsectionHeaderCommon common;

    GLSLC_RESERVED_BYTES(32);
} GLSLCperfStatsHeader;


/// \brief Set of unique identifiers used to identify debug info data and the associated source data.
///
/// This includes a numeric hash computed from the debug data and a build identifier.
typedef struct GLSLCdebugDataHash_rec
{
    /// Unique build identifier to match up source input with the debug data.
    /// Note: Currently this is unsupported.  Future versions of GLSLC will support a buildId
    /// and an interface to set this ID, but currently buildId will always be an array of 0 values.
    uint32_t buildId[4];

    /// Unique identifier for the debug data.  The hash is 64-bits and
    /// debugHashLo32 represents the bits 0..31, and debugHashHi32 represents
    /// the bits 32..63.
    uint32_t debugHashLo32;
    uint32_t debugHashHi32;
} GLSLCdebugDataHash;

/// \brief A debug section header.
typedef struct GLSLCdebugInfoHeader_rec {
    GLSLCsectionHeaderCommon common;

    /// Debug data unique identifying information.
    GLSLCdebugDataHash debugDataHash;

    GLSLC_RESERVED_BYTES(40);
} GLSLCdebugInfoHeader;

/// \brief A union consisting of the possible headers that can be used as input.
///
/// The genericHeader entry should be used for any generic operation
/// on a GLSLCsectionHeaderUnion data type.
typedef union GLSLCsectionHeaderUnion_rec {
    GLSLCgenericHeader   genericHeader;
    GLSLCgpuCodeHeader   gpuCodeHeader;
    GLSLCasmDumpHeader   asmDumpHeader;
    GLSLCperfStatsHeader perfStatsHeader;
    GLSLCprogramReflectionHeader programReflectionHeader;
    GLSLCdebugInfoHeader debugInfoHeader;
} GLSLCsectionHeaderUnion;


/// \brief Spill mem data
typedef struct GLSLCspillLMem_rec {
    /// Amount of local memory written due to spills.
    ///  execution count.
    uint32_t numLmemSpillBytes;

    /// Amount of local memory read due to refills.
    uint32_t numLmemRefillBytes;

    /// Amount of shared memory written due to spills.
    uint32_t numSmemSpillBytes;

    /// Amount of shared memory read due to refills.
    uint32_t numSmemRefillBytes;

    /// Amount of local memory used in the shader not including extra memory required
    /// for divergence handling.
    uint32_t size;
    GLSLC_RESERVED_BYTES(32);
} GLSLCspillLMem;

/// \brief Non-spill memory.
typedef struct GLSLCnonSpillLMem_rec {
    /// Amount of local memory read by loads other than refill loads.
    uint32_t loadBytes;

    /// Amount of local memory written by stores other than spill sotres.
    uint32_t storeBytes;

    /// Amount of local memory used not including extra memory required for divergence handling.
    uint32_t size;

    GLSLC_RESERVED_BYTES(32);
} GLSLCnonSpillLMem;

/// \brief Throughput limiter information.  Values are in terms of warps per cycle.
typedef struct GLSLCthroughputLimiter_rec {
    /// Instruction issue limited throughput
    float issue;

    /// Single precision floating point operation limited throughput
    float fp;

    /// Half operation limited throughput
    float half;

    /// Transcendental, Conversion operation limited throughput
    float trancedental;

    /// Attribute interpolation limited throughput
    float ipa;

    /// Attribute load/store, shared memory load/store limited throughput
    float shared;

    /// Control flow operation limited throughput
    float controlFlow;

    /// Texture fetch, global/local load/store limited throughput
    float texLoadStore;

    /// Register limited throughput
    float reg;

    /// Warp id limited throughput
    float warp;

    /// Shared memory limited throughput
    float sharedMemResource;

    GLSLC_RESERVED_BYTES(28);
} GLSLCthroughputLimiter;

/// \brief Loop unroll information
typedef struct GLSLCloopData_rec {
    /// Outputs the number of loops unrolled partially by the compiler
    /// because the bounds are either unknown at compile time or are too large.
    uint32_t partiallyUnrolled;

    /// Outputs the number of loops the compiler was unable to either fully or partially unroll.
    uint32_t nonUnrolled;
    GLSLC_RESERVED_BYTES(32);
} GLSLCloopData;

/// \brief Data containing the perf statistics returned from GLSLC.
typedef struct GLSLCperfStatsData_rec {
    /// Indicates the presence of perf stats data for sanity checking.  This should
    /// be equal to the define GLSLC_PERF_STATS_SECTION_MAGIC_NUMBER
    uint32_t magic;

    /// Spill memory data.
    GLSLCspillLMem spillMem;

    /// Non-spill memory data.
    GLSLCnonSpillLMem nonSpillLMem;

    /// Throughput limiter information.
    GLSLCthroughputLimiter throughputLimiter;

    /// Information about unrolled loops.
    GLSLCloopData loopData;

    /// Cycles per pixel
    uint32_t latency;

    /// Occupancy is the ratio of active warps to maximum number of warps supported
    float occupancy;

    /// Number of divergent branches.
    uint32_t numDivergentBranches;

    /// Attribute memory usage in bytes
    uint32_t attributeMemUsage;

    /// Program size in bytes
    uint32_t programSize;

    /// Indicates whether any compute shader uniform buffer accesses are emulated with global
    /// loads.
    /// This flag will be set to 1 if using a compute program that contains an access to
    /// uniform blocks with a binding greater than 4.  This flag will never be set for
    /// graphics programs (non-compute).
    /// In general, if this flag is set to 1 it indicates a performance reduction.  If possible,
    /// compute shaders should limit any uniform blocks to be bound to bindings 0 through 4 to
    /// avoid this.
    /// Please see the architecture-specific performance documentation for more information.
    uint8_t requiresGlobalLoadUniformEmulation;

    /// Padding.
    GLSLC_RESERVED_BYTES2(3);

    // Percentage of FP16 instruction vectorized.
    float fp16VectPercent;

    GLSLC_RESERVED_BYTES(24);
} GLSLCperfStatsData;


/// \brief GLSLC versioning information
typedef struct GLSLCversion_rec {
    /// GLSLC API major version number.
    uint32_t apiMajor;

    /// GLSLC API minor version number.
    uint32_t apiMinor;

    /// GLSLC gpu code section binary major version number.
    uint32_t gpuCodeVersionMajor;

    /// GLSLC gpu code section binary minor version number.
    uint32_t gpuCodeVersionMinor;

    /// GLSLC package version.
    uint32_t package;

    GLSLC_RESERVED_BYTES(32);
} GLSLCversion;

/// \brief Main GLSLC output structure.
///
/// This structure contains information about the overall GLSLCoutput
/// followed by _numSections_ section headers.
///
/// Data for each section is stored following the last header, with the section
/// header indicating the size of the data and its offset relative to the
/// beginning of the GLSLCoutput structure.
///
/// The size parameter contains the total size of the GLSLCoutput
/// structure (including data).
///
/// The dataOffset is the offset to the first data block
/// from the beginning of the GLSLCoutput structure.
///
/// The structure is stored in contiguous memory, using the offsets in each
/// section header to denote the data location in the GLSLCoutput
/// structure.
typedef struct GLSLCoutput_rec {
    /// GLSLC magic number to identify the beginning of a GLSLCoutput data
    /// block.  This should be equal to the define GLSLC_MAGIC_NUMBER.
    uint32_t magic;

    /// Reserved to keep sizeof the GLSLCoutput struct the same as previous version 14 due to padding.
    uint32_t reservedBits;

    /// Specific GLSLC option flags that were used to produce this output
    /// structure.
    GLSLCoptionFlags optionFlags;

    /// GLSLC version.
    GLSLCversion versionInfo;

    /// Total size of the output (in bytes).
    uint32_t size;

    /// Offset to the data section starting from the beginning address of
    /// the GLSLCoutput structure, in bytes.
    uint32_t dataOffset;

    /// Number of sections contained in the binary.
    uint32_t numSections;

    GLSLC_RESERVED_BYTES(64);

    /// Array of section headers for this output structure.
    GLSLCsectionHeaderUnion headers[1];
} GLSLCoutput;


/// \brief A section that will contain the results of a compile.
///
/// This section includes pointers to any internally allocated data
/// structures by GLSLC that includes the serialized GLSLCoutput structure
/// as well as additional information such as the compilation info log.
///
/// Important: These entries in this structure are internally allocated by GLSLC.
/// When either glslcFinalize is called or a new compilation is made with the
/// corresponding glslcCompileObject, the memory pointed to by these entries will
/// be invalid.  If the application wishes to retain these, they should be copied
/// out to application managed memory after a compilation procedure.
typedef struct GLSLCresults_rec {
    /// The GLSLC compilation status including the infolog that is associated
    /// with the compilation.
    GLSLC_POINTER_MEMBER(GLSLCcompilationStatus *, compilationStatus);

    /// The GLSLCoutput containing the serialized output for the compilation.
    /// This pointer is only non-NULL if the compilation operations were successful.
    GLSLC_POINTER_MEMBER(GLSLCoutput *, glslcOutput);

    GLSLC_RESERVED_BYTES(32);
} GLSLCresults;

/// \brief Initialization status flags indicating any errors during glslcInitialize.
typedef enum GLSLCinitializationStatus_enum {
    /// Default value indicating glslcInitialize has not been called yet.
    GLSLC_INIT_ERROR_UNINITIALIZED = 0,

    /// Flag indicating a successful call to glslcInitialize.
    GLSLC_INIT_SUCCESS = 1,

    /// Flag indicating that a memory allocation failure occurred during glslcInitialize.
    GLSLC_INIT_ERROR_ALLOC_FAILURE = 2,

    /// Flag indicating that the memory allocation callbacks were not set before trying
    /// to use the GLSLC library.  This error only occurs on the NX version of the
    /// GLSLC library.
    GLSLC_INIT_ERROR_NO_ALLOC_CALLBACKS_SET = 3,
} GLSLCinitializationStatus;

/// \brief Enumeration indicating the return value of the API function glslcWriteOutputsToDiskForDebugger
typedef enum GLSLCdebuggerOutputErrorEnum_enum {
    /// Indicates that one or more of the GLSLCoutputs used as input to glslcWriteOutputsToDiskForDebugger do
    /// not contain debug sections (i.e. sections having type GLSLC_SECTION_TYPE_DEBUG_INFO).
    GLSLC_DEBUGGER_OUTPUT_ERROR_NO_DEBUG_SECTION = 0,

    /// Indicates glslcWriteOutputsToDiskForDebugger was successful and all outputs were written to the
    /// specified directory.
    GLSLC_DEBUGGER_OUTPUT_SUCCESS = 1,

    /// A call to glslcWriteOutputsToDiskForDebugger failed due to GLSLC not being able to open one or more
    /// of the output file paths for writing.
    GLSLC_DEBUGGER_OUTPUT_ERROR_CANT_OPEN_FILE = 2,

    /// A call to glslcWriteOutputsToDiskForDebugger failed due to GLSLC not being able to write to one or more
    /// of the opened output file paths.
    GLSLC_DEBUGGER_OUTPUT_ERROR_CANT_WRITE_FILE = 3,

    /// Allocation callbacks not set.  This error will happen only on NX versions of the GLSLC library.
    GLSLC_DEBUGGER_OUTPUT_ERROR_NO_ALLOC_CALLBACKS_SET = 4,

    /// An allocation error occurred.
    GLSLC_DEBUGGER_OUTPUT_ERROR_ALLOCATION = 5
} GLSLCdebuggerOutputError;

/// \brief The main object that holds input/output variables for compilation.
///
/// This structure is responsible for holding the input and the output of
/// compile operations.  An instance of this object should be initialized before
/// use via glslcInitialize, and finalized to clean up memory after use via
/// glslcFinalize.
typedef struct GLSLCcompileObject {
    /// Results of the last glslcCompile call.  The memory pointed to by this entry
    /// is overwritten on successive glslcCompile calls.  Additionally, the memory
    /// is freed during glslcFinalize.  If the application wishes to hold on to any
    /// results, the application should copy over to application managed memory
    /// after a compilation operation.
    GLSLC_POINTER_MEMBER(GLSLCresults *, lastCompiledResults);

    /// This points to a GLSLCprogramReflectionHeader structure (equivalent to what would
    /// be inside the GLSLCoutput if a reflection information were requested).
    /// This will allow applications wishing to perform specialization to compile first with a
    /// glslcCompilePreSpecialized function call, then retrieve the uniforms from the reflection information
    /// in order to make desicions on which uniforms can be specialized. Additionally, applications need not
    /// specify the "outputShaderReflection" in the options to get this data.  This will automatically be
    /// cleaned up during "glslcFinalize", or if a new "glslcCompile" or "glslcCompilePreSpecialized" call is made.
    GLSLC_POINTER_MEMBER(GLSLCprogramReflectionHeader *, reflectionSection);

    /// Private data that GLSLC may use internally to store intermediate data that does
    /// not belong in the GLSLCoutput structure.
    /// IMPORTANT: This pointer should not be used by the application.
    GLSLC_POINTER_MEMBER(void *, privateData);

    /// Compilation options.
    GLSLCoptions options;

    /// Input shaders.
    GLSLCinput input;

    /// Status set from a glslcInitialize call.  GLSLC_INIT_SUCCESS indicates that glslcInitialize
    /// was successful, otherwise the appropriate GLSLCinitializationStatus error code will be set.
    GLSLCinitializationStatus initStatus;

    GLSLC_RESERVED_BYTES(28);
} GLSLCcompileObject;


/// \brief A function pointer for allocating memory.
typedef void* (*GLSLCallocateFunction)(size_t size, size_t alignment, void *userPtr);

/// \brief A function pointer for freeing memory.
typedef void  (*GLSLCfreeFunction)(void *addr, void *userPtr);

/// \brief A function pointer for reallocating memory.
typedef void* (*GLSLCreallocateFunction)(void* addr, size_t newSize, void *userPtr);


/// \brief Partially compiles to an intermediate representation so that future specialization compiles
/// don't need to go all the way from GLSL each time.
///
/// The compile object will hold a reference to this data until the call to glslcFinalize is performed or
/// another glslcCompilePreSpecialized call is made.
///
/// Note:  Most options can not be changed between compiles after this function call is made.
/// If new options are needed other than specialization options, then glslcFinalize will
/// need to be called to destroy the IL before reuse, or a new call to "glslcCompilePreSpecialized" will need to
/// be made.
///
/// The compileObject's "reflectionSection" member will contain reflection information
/// that can be used to obtain uniforms for specifying values after calling this function.
///
/// \param compileObject A pointer to GLSLCcompileObject to be set up.
GLSLCAPI bool glslcCompilePreSpecialized(GLSLCcompileObject * compileObject);

/// \brief Compiles a specialized version of the binary using the input specialization information.
///
/// The number of GLSLCoutput objects returned corresponds to the parameter GLSLCspecializationBatch::numEntries
/// set inside the GLSLCoptions structure.  The memory for the returned GLSLCoutputs is tracked internally by
/// GLSLC.  The memory for the returned GLSLCoutputs will be freed by calls to glslcFinalize,
/// or by additional compiles using glslcCompileSpecialized or glslcCompile.  The application is responsible for
/// copying the binary data before the objects are freed by GLSLC.
///
/// NOTE: This function is not thread-safe.  If a thread-safe version of glslcCompileSpecialized is required (where
/// each thread is sharing the same compile object), then use the function glslcCompileSpecializedMT.
///
/// NOTE: It is an error to change any compilation options other than specialization options
/// between calls to glslcCompileSpecialized if the function glslcCopmilePreSpecialized were used previously.
///
/// NOTE: Since this is purely for specialization, the member GLSLCcompileObject::lastCompiledResults will only contain
/// information if an error in compilation occurred.
///
/// NOTE: If glslcCompilePreSpecialized was previously used, then the compilation will start with the intermediate compile data.
/// If it has not been called, then this function will do a full compile.
///
/// \param compileObject The previously initialized GLSLCcompileObject.
/// \param specEntries A pointer to the GLSLCspecializationBatch which contains 1 or more GLSLCspecializationSets
/// \return NULL if an error occurred, otherwise a pointer to GLSLCoutput structures where _n_ equals the specEntries->numEntries field.
GLSLCAPI const GLSLCoutput * const * glslcCompileSpecialized(GLSLCcompileObject * compileObject, const GLSLCspecializationBatch * specEntries);

/// \brief Initializes the compile objects, setting up internal structures. This must be called before
/// using the object anywhere.
///
/// \return 1 if the operation succeeded, 0 if there was an error or if compileObject is NULL.
/// If compileObject is not NULL, the member GLSLCcompileObject::initStatus will contain an
/// GLSLCinitializationStatus enumeration indicating the initialization status of the compile object.
/// If this function returns 1, then the compile object's initStatus flag is expected to be GLSLC_INIT_SUCCESS.
///
/// Note: With the NX version of the GLSLC library, glslcSetAllocator must be called prior to calling
/// glslcInitialize, otherwise glslcInitialize will return 0 and the compile object's initStatus flag will
/// be set to GLSLC_INIT_ERROR_NO_ALLOC_CALLBACKS_SET.
GLSLCAPI uint8_t glslcInitialize( GLSLCcompileObject * compileObject );

/// \brief Cleans up any internal memory associated with a GLSLC compile.
///
/// After this function returns, the passed-in GLSLCcompileObject should not be used until
/// glslcInitialize is called with it.
///
/// \param compileObject A pointer to GLSLCcompileObject to be set up.
GLSLCAPI void glslcFinalize( GLSLCcompileObject * compileObject );

/// \brief Compile source shaders using a previously initialized GLSLCcompileObject.
///
/// The function will return false if any errors occurred, including compilation errors.
/// The GLSLCcompileObject passed in will be modified based on the results of the
/// compilation.  This function will NOT perform any specialization.
///
/// \param compileObject The previously initialized GLSLCcompileObject.
/// \return 1 if the operation succeeded, 0 if otherwise.
GLSLCAPI uint8_t glslcCompile( GLSLCcompileObject * compileObject );

/// \brief Reports the GLSLC version from the library.  Note that there could be
/// a difference between the reported version and the version in this header file.
///
/// \return A GLSLCversion structure filled in with the DLL-reported version.
GLSLCAPI GLSLCversion glslcGetVersion();

/// \brief Set memory allocator for GLSLC library.
///
/// With the NX version of GLSLC, this function should be called once, before calling
/// any other functions in the GLSLC API. If the function is not called, a warning will be
/// emitted during compilation.  In future releases, glslcInitialize will fail if the allocators
/// are not set. This function does not need to be called on the Windows version of GLSLC and will
/// result in a noop.
///
/// \param allocate A function pointer that GLSLC library should call when it needs more working memory.
/// \param free A function pointer that the GLSLC library can call when it can release working memory.
/// \param reallocate A function pointer that the GLSLC library can call when it can release working memory.
/// \param userPtr User pointer that will be passed to callbacks.
GLSLCAPI void glslcSetAllocator(GLSLCallocateFunction allocate, GLSLCfreeFunction free, GLSLCreallocateFunction reallocate, void *userPtr);

/// \brief Return a default GLSLCoptions structure.
///
/// This functions initializes and returns a default GLSLCoptions structure.  This is the same
/// initialization performed during a glslcInitialize call for the GLSLCoptions member of the
/// GLSLC compile object.
GLSLCAPI GLSLCoptions glslcGetDefaultOptions();

/// \brief Writes the serialized GLSLCoutput structures to disk.
///
/// Writes the output as "[8-byte hexidecimal debug hash].glslcoutput".  The function returns 0 upon
/// failure, and 1 upon success.  If outStatus != NULL, it will be filled with the status code which can
/// be used in cases of failures to determine what the failure was.  All GLSLCoutputs specified in the outputArray
/// must contain debug sections of the type GLSLC_SECTION_TYPE_DEBUG_INFO.
GLSLCAPI uint8_t glslcWriteOutputsToDiskForDebugger(const char * directoryPath, const GLSLCoutput * const * outputArray, uint32_t numOutputs, GLSLCdebuggerOutputError * outStatus);

/// \brief Compiles a specialized version of the binary using the input specialization information and is thread-safe.
///
/// This function may be called on multiple threads using the same compile object.  The number of GLSLCoutput objects
/// returned corresponds to the parameter GLSLCspecializationBatch::numEntries
/// set inside the GLSLCoptions structure.  The memory for the returned GLSLCoutputs is NOT tracked internally by
/// GLSLC and needs to be explicitly freed by the application by calling glslcFreeSpecializedResultsMT.  Additionally,
/// the member GLSLCcompileObject::lastCompiledResults will not be modified by calls to this function, and the error
/// logs need to be taken from the GLSLCresults structures returned directly from this function.
///
/// NOTE: It is an error to change any compilation options other than specialization options
/// between calls to glslcCompileSpecialized if the function glslcCopmilePreSpecialized were used previously.
///
/// NOTE: If glslcCompilePreSpecialized was previously used, then the compilation will start with the intermediate compile data.
/// If it has not been called, then this function will do a full compile.
///
/// \param compileObject The previously initialized GLSLCcompileObject.
/// \param specEntries A pointer to the GLSLCspecializationBatch which contains 1 or more GLSLCspecializationSets
/// \return NULL if an error occurred, otherwise a pointer to GLSLCresults structures where _n_ equals the specEntries->numEntries field.
GLSLCAPI GLSLCresults const * const * glslcCompileSpecializedMT(const GLSLCcompileObject * compileObject, const GLSLCspecializationBatch * specEntries);

/// \brief Free the results returned from glslcCompileSpecializedMT.
///
/// NOTE: Calling this function on any pointer not returned by glslcCompileSpecializedMT has undefined behavior.
///
/// NOTE: The memory pointed to by specResults will be freed and invalid after this function call.
///
/// \param specResults A pointer to the results returned from a glslcCompileSpecializedMT call.
GLSLCAPI void glslcFreeSpecializedResultsMT(GLSLCresults const * const * specResults);

/// \brief Compare two control section binaries for functional equality using
//         the rules defined in the GLSLC programming guide.  This does NOT test for byte-wise equality.
///
/// \param pControlSectionA A pointer to the first control section pointer.
/// \param pControlSectionB A pointer to the second control section pointer.
/// \return 0 if the sections are not equal or invalid, 1 if they are equal (ignoring bytes used for functional equality)
GLSLCAPI uint8_t glslcCompareControlSections(const void* pControlSectionA, const void* pControlSectionB);

/// \brief Extract the debug data hash of the GLSLC binary
///
/// \param pControlSection A pointer to a control section pointer.
/// \param pDebugDataHash A pointer to a debug hash section which will be filled in upon successful completion.
/// \return 1 if if the operation succeeded, 0 otherwise
GLSLCAPI uint8_t glslcGetDebugDataHash(const void* pControlSection, GLSLCdebugDataHash* pDebugDataHash);

/// \brief Insert the specified debug data hash into the control section binary
///
/// \param pControlSection A pointer to a control section pointer which will be modified with input debug data hash.
/// \param pDebugDataHash A pointer to a debug hash section.
/// \return 1 if if the operation succeeded, 0 otherwise
GLSLCAPI uint8_t glslcSetDebugDataHash(void* pControlSection, const GLSLCdebugDataHash* pDebugDataHash);

#ifdef __cplusplus
}
#endif

#if defined(_MSC_VER) || (defined(__GNUC__) && !defined(__sun__))
#pragma pack(pop)
#endif

#endif //NVNTOOL_GLSLCINTERFACE_H_
