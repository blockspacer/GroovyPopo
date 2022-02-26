/*--------------------------------------------------------------------------------*
  Copyright (C)Nintendo All rights reserved.

  These coded instructions, statements, and computer programs contain proprietary
  information of Nintendo and/or its licensed developers and are protected by
  national and international copyright laws. They may not be disclosed to third
  parties or copied or duplicated in any form, in whole or in part, without the
  prior written consent of Nintendo.

  The content herein is highly confidential and should be handled accordingly.
 *--------------------------------------------------------------------------------*/


/// \file nvnTool_Image.h
///       This module defines classes to represent and operate on raw generic
///       pitch-linear images used by the NVN texture packager tool.

#ifndef NVNTOOL_IMAGE_H
#define NVNTOOL_IMAGE_H

#include <nvn/nvn.h>
#include "nvnTool_TexpkgCommon.h"

namespace nvnTool {
namespace texpkg {

// Forward declaration to avoid cyclic header dependency.
struct RawImageLib;

#define NVN_RAW_IMAGE_VERSION 0x0020

/// A set of flags indicating cube-map faces.
enum RawTextureCubeFace {
    CUBE_FACE_None        = 0x00, ///< No cube-map faces.
    CUBE_FACE_PositiveX   = 0x01, ///< The positive-X cube-map face.
    CUBE_FACE_NegativeX   = 0x02, ///< The negative-X cube-map face.
    CUBE_FACE_PositiveY   = 0x04, ///< The positive-Y cube-map face.
    CUBE_FACE_NegativeY   = 0x08, ///< The negative-Y cube-map face.
    CUBE_FACE_PositiveZ   = 0x10, ///< The positive-Z cube-map face.
    CUBE_FACE_NegativeZ   = 0x20, ///< The negative-Z cube-map face.
    CUBE_FACE_All         = 0x3f, ///< All the cube-map faces.
};

/// \brief Custom alignment function callback.
///
/// Custom alignment function callback. Format loaders will need to implement this for
/// format-specific alignments. Alternatively the format loader can re-align at load,
/// and just use the default (which assumes packed no align).
///
typedef uint32_t (*GetImageSliceDataSizeFunction)(RawImageLib* imglib,
                                                  uint32_t width, uint32_t height, uint32_t depth,
                                                  NVNformat format, uint32_t *pitch);

/// \brief  Reference to the data in one texture mipmap level in an image.
///
/// A reference to the data in one texture mipmap level in an image, which is a simple
/// W x H x D image. The offset into the data for each 3D depth slice may be queried, taking into
/// account alignment requirements for different formats.
/// This structure does NOT own any memory. It is merely a weak pointer struct with some helper
/// functions.
///
struct RawMipMapLevel {
    //! Width(x) dimension of the mipmap level. Width > 0.
    uint32_t width;
    //! Height(y) dimension of the mipmap level. Height > 0.
    uint32_t height;
    //! Depth(z) dimension of the mipmap level. Depth > 0.
    uint32_t depth;
    //! The difference, in bytes, between the first texels in adjacent rows.
    //  Equal to ALIGN(Width * BPP / blockSize).
    uint32_t pitch;

    //! Size (in bytes) of one slice of data.
    uint32_t sliceSize;
    //! Size (in bytes) of the entire mipmap level.
    uint32_t dataSize;
    //! Format of the pixels stored in 'data'.
    NVNformat format;

    //! The mipmap level pixel data. Weak reference; do not delete.
    uint8_t *data;

    RawMipMapLevel()
    {
        width = height = depth = pitch = sliceSize = dataSize = 0;
        format = NVN_FORMAT_NONE;
        data = NULL;
    }
};

#ifdef NVNIMG_BUILD
/// \brief Get one single 2D data slice in the 3D mipmap level image.
///
/// \param[in] slice     The 3D depth slice index to retrieve.
///
/// \return Weak pointer to the beginning of the 2D image at the given slice depth ie.
///         data + offset.
///
uint8_t* RawMipMapLevelDataSlice(RawMipMapLevel *lp, uint32_t slice);

/// \brief Get the offset into the pixel of a data slice. Takes into account pitch and BPP so forth.
/// Eg. DataSlice(3) + GetSlicePixelOffset(1, 2) will be the pixel address of pixel[1][2][3].
///
/// \param[in] x       The X (width) co-ordinate of the pixel offset.
/// \param[in] y       The Y (height) co-ordinate of the pixel offset.
///
/// \return Offset into a dataslice at which the pixel is at, in bytes.
///
uint32_t RawMipMapLevelSlicePixelOffset(RawMipMapLevel *lp, uint32_t x, uint32_t y);
#endif

/// \brief A generic pitch-linear image.
///
/// A generic pitch-linear image. Note that this class does not do any modification of the image
/// data; it's simply a container. Pixels are stored per-subimage, then per-mipmap-level, then by
/// depth, height, and finally width.
///
/// 2D texture images have depth = 1 and sub-images = 1.
/// Cube textures have up to 6 subimages; CubeFaceMask() returns a bit-mask of which subimage
/// map to which cube face. 3D textures have 1 sub-image, and slices are stored as depth. 2D texture
/// arrays have depth = 1, and slices are stored as N sub-images.
///
/// This structure will allocate and own the pixel data. Thus, the data pointer is no longer valid
/// if this object falls out of scope.
///
class RawImage {
public:
    virtual ~RawImage() {}

    virtual uint8_t Initialized(void) const = 0;

    virtual uint32_t MipmapLevels(void) const = 0;

    virtual uint32_t SubImages(void) const = 0;

    virtual uint32_t DataSize(void) const = 0;

    virtual NVNformat Format(void) const  = 0;

    virtual NVNtextureTarget Target(void) const = 0;

    virtual uint32_t CubeFaceMask(void) const = 0;

    virtual void SetCubeFaceMask(uint32_t cubeFaceMask) = 0;

    /// Release data and reset object back to its default uninitialized state.
    virtual void Release(void) = 0;

    /// \brief Get the image data pointer, or NULL if image hasn't been initialized.
    /// NOTE: Do not delete the contents of the returned pointer.
    virtual uint8_t* GetData(void) = 0;

    /// Base level width of the image.
    virtual uint32_t Width(void) const = 0;

    /// Base level height of the image.
    virtual uint32_t Height(void) const = 0;
    /// Base level depth of the image.
    virtual uint32_t Depth(void) const = 0;

    /// \brief Set custom callback function to calculate the data size.
    /// Set a custom callback function to calculate the data size given image dimensions and format,
    /// used for custom alignment / padding requirements. Defaults to no padding.
    /// Used by image format loaders to set the data block to be able to read in the entire file
    /// chunk all at once, rather than having to re-align every row.
    /// NOTE: This must be done before calling Create() below.
    ///
    /// \param[in] func       The custom alignment callback function to set.
    /// \param[in] imglib     Image library passed into callback function. Optional, may be NULL.
    ///
    virtual TPError SetImageSliceDataSizeFunction(GetImageSliceDataSizeFunction func, RawImageLib *imglib) = 0;

    /// Get the currently set data size calculation function.
    virtual GetImageSliceDataSizeFunction CurrentImageSliceDataSizeFunction(void) const = 0;

    /// Get the currently set RawImage library.
    virtual RawImageLib* CurrentImageSliceDataSizeFunctionImgLib(void) const = 0;

    /// \brief Create the image and allocate its storage. Images may not be re-allocated.
    /// One of the Create* methods must be called to initialize the image.
    ///
    /// \param[in] target       The intended image texture target.
    /// \param[in] width        The width of the image (at mip level 0).
    /// \param[in] height       The height of the image (at mip level 0).
    /// \param[in] depth        The depth of the image (at mip level 0).
    /// \param[in] format       The NVN format of the image.
    /// \param[in] mipLevels    Number of mipmap levels in the image
    /// \param[in] subimages    Number of sub-images in the image. This should be the number of
    ///                         layers in array images, or number of faces in cubemaps.
    ///
    /// \return TP_OK if image creation and allocation succeeded, error code otherwise.
    ///
    virtual TPError Create(NVNtextureTarget target, uint32_t width, uint32_t height, uint32_t depth,
                           NVNformat format, uint32_t mipLevels, uint32_t subimages = 1) = 0;

    /// \brief Create a 1D image and allocate its storage. Images may not be re-allocated.
    /// One of the Create* methods must be called to initialize the image.
    ///
    /// \param[in] width        The width of the image (at mip level 0).
    /// \param[in] format       The NVN format of the image.
    /// \param[in] mipLevels    Num of mipmap levels in the image
    ///
    /// \return TP_OK if image creation and allocation succeeded, error code otherwise.
    ///
    inline TPError Create1D(uint32_t width, NVNformat format, uint32_t mipLevels)
    {
        return Create(NVN_TEXTURE_TARGET_1D, width, 1, 1, format, mipLevels);
    }

    /// \brief Create a 2D image and allocate its storage. Images may not be re-allocated.
    /// One of the Create* methods must be called to initialize the image.
    ///
    /// \param[in] width        The width of the image (at mip level 0).
    /// \param[in] height       The height of the image (at mip level 0).
    /// \param[in] format       The NVN format of the image.
    /// \param[in] mipLevels    Num of mipmap levels in the image
    ///
    /// \return TP_OK if image creation and allocation succeeded, error code otherwise.
    ///
    inline TPError Create2D(uint32_t width, uint32_t height, NVNformat format,
                            uint32_t mipLevels)
    {
        return Create(NVN_TEXTURE_TARGET_2D, width, height, 1, format, mipLevels);
    }

    /// \brief Create a 3D image and allocate its storage. Images may not be re-allocated.
    /// One of the Create* methods must be called to initialize the image.
    ///
    /// \param[in] width        The width of the image (at mip level 0).
    /// \param[in] height       The height of the image (at mip level 0).
    /// \param[in] depth        The depth of the image (at mip level 0).
    /// \param[in] format       The NVN format of the image.
    /// \param[in] mipLevels    Num of mipmap levels in the image
    ///
    /// \return TP_OK if image creation and allocation succeeded, error code otherwise.
    ///
    inline TPError Create3D(uint32_t width, uint32_t height, uint32_t depth, NVNformat format,
                            uint32_t mipLevels)
    {
        return Create(NVN_TEXTURE_TARGET_3D, width, height, depth, format, mipLevels);
    }

    /// \brief Create a 1D array image and allocate its storage. Images may not be re-allocated.
    /// One of the Create* methods must be called to initialize the image.
    ///
    /// \param[in] width        The width of the image (at mip level 0).
    /// \param[in] format       The NVN format of the image.
    /// \param[in] mipLevels    Num of mipmap levels in the image
    /// \param[in] layers       The number of array layers.
    ///
    /// \return TP_OK if image creation and allocation succeeded, error code otherwise.
    ///
    inline TPError Create1DArray(uint32_t width, NVNformat format, uint32_t mipLevels,
                                 uint32_t layers)
    {
        return Create(NVN_TEXTURE_TARGET_1D_ARRAY, width, 1, 1, format, mipLevels, layers);
    }

    /// \brief Create a 2D array image and allocate its storage. Images may not be re-allocated.
    /// One of the Create* methods must be called to initialize the image.
    ///
    /// \param[in] width        The width of the image (at mip level 0).
    /// \param[in] height       The height of the image (at mip level 0).
    /// \param[in] format       The NVN format of the image.
    /// \param[in] mipLevels    Num of mipmap levels in the image
    /// \param[in] layers       The number of array layers.
    ///
    /// \return TP_OK if image creation and allocation succeeded, error code otherwise.
    ///
    inline TPError Create2DArray(uint32_t width, uint32_t height, NVNformat format,
                                 uint32_t mipLevels, uint32_t layers)
    {
        return Create(NVN_TEXTURE_TARGET_2D_ARRAY, width, height, 1, format, mipLevels, layers);
    }

    /// \brief Create a rectangle image and allocate its storage. Images may not be re-allocated.
    /// One of the Create* methods must be called to initialize the image.
    ///
    /// \param[in] width        The width of the image (at mip level 0).
    /// \param[in] height       The height of the image (at mip level 0).
    /// \param[in] format       The NVN format of the image.
    /// \param[in] mipLevels    Num of mipmap levels in the image
    ///
    /// \return TP_OK if image creation and allocation succeeded, error code otherwise.
    ///
    inline TPError CreateRect(uint32_t width, uint32_t height, NVNformat format, uint32_t mipLevels)
    {
        return Create(NVN_TEXTURE_TARGET_RECTANGLE, width, height, 1, format, mipLevels);
    }

    /// \brief Create a cubemap image and allocate its storage. Images may not be re-allocated.
    /// One of the Create* methods must be called to initialize the image.
    ///
    /// \param[in] size         The size (width & height) of the cubemap (at mip level 0).
    /// \param[in] format       The NVN format of the image.
    /// \param[in] mipLevels    Num of mipmap levels in the image
    ///
    /// \return TP_OK if image creation and allocation succeeded, error code otherwise.
    ///
    inline TPError CreateCubemap(uint32_t size, NVNformat format, uint32_t mipLevels)
    {
        return Create(NVN_TEXTURE_TARGET_CUBEMAP, size, size, 1, format, mipLevels, 6);
    }

    /// \brief Create a cubemap array image and allocate its storage. Images may not be re-allocated.
    /// One of the Create* methods must be called to initialize the image.
    ///
    /// \param[in] size         The size (width & height) of the cubemap (at mip level 0).
    /// \param[in] format       The NVN format of the image.
    /// \param[in] mipLevels    Num of mipmap levels in the image
    /// \param[in] layers       The number of array layers. Each array layer is to 6 faces
    ///                         (ie. 6 subimages).
    ///
    /// \return TP_OK if image creation and allocation succeeded, error code otherwise.
    ///
    inline TPError CreateCubemapArray(uint32_t size, NVNformat format, uint32_t mipLevels, uint32_t layers)
    {
        return Create(NVN_TEXTURE_TARGET_CUBEMAP_ARRAY, size, size, 1, format, mipLevels,
                      6 * layers);
    }

    ///
    /// \brief Retrieve the reference to a single mipmap level.
    ///
    /// \param[in] levelIdx   The mipmap level index ( must be < MipmapLevels() ).
    /// \param[in] subimage   The subimage index ( must be < SubImages() ).
    ///
    /// \return Reference to a mipmap level structure. The returned structure is a reference only,
    ///         do not free the returned structure.
    ///
    virtual RawMipMapLevel* MipMapLevel(uint32_t levelIdx, uint32_t subimage = 0) = 0;

    ///
    /// \brief Retrieve the reference to the smallest mipmap level.
    ///
    /// \param[in] subimage       The subimage index ( must be < SubImages() ).
    ///
    /// \return Reference to last mipmap level structure. The returned structure is a reference only,
    ///         do not free the returned structure.
    ///
    virtual RawMipMapLevel* LastMipMapLevel(uint32_t subimage = 0) = 0;

    ///
    /// \brief Re-interpret source image data as a new format.
    ///
    /// Re-interpret source image data as a new format. All data and alignment left unchanged.
    /// NOTE: Only safe to do this if BPP matches between old and new formats.
    /// Do not perform on compressed images.
    ///
    /// \param[in] fmt        The new data format.
    ///
    virtual void ReinterpretSourceFormatAs(NVNformat fmt) = 0;

    ///
    /// \brief Dump a mipmap level's data to file. DEBUG BUILD ONLY.
    ///
    /// \param[in] fileName   The dump output file name.
    /// \param[in] lp         The mipmap level structure to dump. Must belong to this image class!
    ///
    /// \return True if dump succeeded, false otherwise.
    ///
    virtual bool DebugDump(const char* fileName, RawMipMapLevel* lp) = 0;

    ///
    /// \brief Dump a mipmap level's data to file. DEBUG BUILD ONLY.
    ///
    /// \param[in] fileName   The dump output file name.
    /// \param[in] levelIdx   The mipmap level index ( must be < MipmapLevels() ).
    /// \param[in] subimage       The subimage index ( must be < SubImages() ).
    ///
    /// \return True if dump succeeded, false otherwise.
    ///
    virtual bool DebugDump(const char* fileName, uint32_t levelIdx = 0, uint32_t subimage = 0) = 0;

    /// Hash the contents of the image.
    virtual uint64_t Hash(void) = 0;
};

} // namespace texpkg
} // namespace nvnTool

extern "C" {

    /// \brief Create a RawImage object.
    ///
    /// \return Created RawImage object. This must be released using texpkgReleaseRawImage().
    /// \see texpkgReleaseRawImage.
    ///
    DLL_API nvnTool::texpkg::RawImage* texpkgCreateRawImage(void);

    TP_DLL_EXPORTED_FUNC (
        nvnTool::texpkg::RawImage*,
        texpkgCreateRawImage,
        (void),
        CREATE_RAWIMAGE_FUNC
    );

    /// \brief Release a RawImage object.
    ///
    /// \param[in] ri       RawImage object created by texpkgCreateRawImage().
    /// \see texpkgCreateRawImage.
    ///
    DLL_API void texpkgReleaseRawImage(nvnTool::texpkg::RawImage* ri);

    TP_DLL_EXPORTED_FUNC (
        void,
        texpkgReleaseRawImage,
        (nvnTool::texpkg::RawImage*),
        RELEASE_RAWIMAGE_FUNC
    );

    /// \brief Get one single 2D data slice in the 3D mipmap level image.
    ///
    /// \param[in] lp        The mipmap level.
    /// \param[in] slice     The 3D depth slice index to retrieve.
    ///
    /// \return Weak pointer to the beginning of the 2D image at the given slice depth ie.
    ///         data + offset.
    ///
    DLL_API uint8_t* texpkgRawMipMapLevelDataSlice(nvnTool::texpkg::RawMipMapLevel* lp, uint32_t slice);

    TP_DLL_EXPORTED_FUNC (
        uint8_t*,
        texpkgRawMipMapLevelDataSlice,
        (nvnTool::texpkg::RawMipMapLevel*, uint32_t),
        RAWMIPMAP_LEVEL_DATASLICE_FUNC
    );

    /// \brief Get the offset into the pixel of a data slice. Takes into account pitch and BPP so forth.
    /// Eg. DataSlice(3) + GetSlicePixelOffset(1, 2) will be the pixel address of pixel[1][2][3].
    ///
    /// \param[in] lp      The mipmap level.
    /// \param[in] x       The X (width) co-ordinate of the pixel offset.
    /// \param[in] y       The Y (height) co-ordinate of the pixel offset.
    ///
    /// \return Offset into a dataslice at which the pixel is at, in bytes.
    ///
    DLL_API uint32_t texpkgRawMipMapLevelSlicePixelOffset(nvnTool::texpkg::RawMipMapLevel* lp, uint32_t x, uint32_t y);

    TP_DLL_EXPORTED_FUNC (
        uint32_t,
        texpkgRawMipMapLevelSlicePixelOffset,
        (nvnTool::texpkg::RawMipMapLevel*, uint32_t, uint32_t),
        RAWMIPMAP_LEVEL_SLICE_PIXEL_OFFSET_FUNC
    );

} // extern "C"

#endif // NVNTOOL_IMAGE_H
