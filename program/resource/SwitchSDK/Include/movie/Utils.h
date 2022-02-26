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
 * @brief Utils.h 
 */

#pragma once

#include <movie/Status.h>
#include <nn/nn_Common.h>

/**
 * @namespace movie
 * @brief    MultiMedia movie functions.
 * @details  Audio/Video decoding
 */
namespace movie {

/**
 * @brief 
 * Buffer utility for passing media data between decoders and extractors
 *
 * @details
 * Buffer utility class can be used for passing decoder buffers to extractors. Buffer can be constructed 
 * with buffer object allocated data memory, client passed data memory or to hold only buffer information.
 */
class Buffer 
{
public:
    
    /**
     * @brief
     * Constructor to create an empty buffer object
     *
     * @details 
     * A new buffer object is created. No data memory is allocated 
     */
    Buffer();

    /**
     * @brief 
     * Constructor to create a buffer object with memory allocation
     *
     * @param[in] capacity              The size of data memory allocation
     *
     * @details 
     * Creates a movie::Buffer object that allocates memory of size @a capacity
     * This memory is owned by the movie::Buffer and will be released when destructed.
     */
    Buffer(size_t capacity);

    /**
     * @brief 
     * Create a buffer object, data points to client provided memory pointer
     *
     * @param[in] data                  Memory data pointer from client
     * @param[in] capacity              Data size
     *
     * @details 
     * The buffer object is created. Memory does not belong to buffer object. This memory will not 
     * be de-allocated when buffer object is deleted. Client need to deallocate the client provided
     * memory.
     */
    Buffer(void* data, size_t capacity);
    
    /**
     * @brief 
     * Copy constructor.     
     *
     * @details
     * Copy constructor for movie::Buffer.
     */
    Buffer(const Buffer& otherBuffer);

    /**
     * @brief 
     * Operator overloading for assignment operator
     *
     * @details
     * Assignment operator overloading for movie::Buffer.
     */
    Buffer& operator= (const Buffer& otherBuffer);

    /**
     * @brief 
     * Return buffer base, pointer to data memory
     *
     * @retval uint8_t                  Data memory pointer
     *
     * @details 
     * Return uint8_t memory pointer. The pointer can be pointing to null, buffer object allocated 
     * memory or client provided memory.
     */
    uint8_t* Data() const;

    /**
     * @brief 
     * Return buffer capacity
     *
     * @retval size_t                   Maximum data buffer object can hold
     *
     * @details 
     * Return maximum data the object can hold. 
     */
    size_t Capacity() const;

    /**
     * @brief 
     * Return data pointer including data offset
     *
     * @retval uint8_t                  Data memory pointer, includes data offset.
     *
     * @details 
     * Return data pointer including data offset. The is basically, Data() + Offset()
     */
    uint8_t* Base() const;

    /**
     * @brief 
     * Return data offset where valid data starts.
     *
     * @retval int32_t                  Offset in bytes from data location
     *
     * @details 
     * The offset corresponds to the location in memory where the actual data starts. 
     * The offset is the distance in bytes from Data() pointer to start of actual data.
     */
    int32_t Offset() const;
    
    /**
     * @brief 
     * Return the current data size in the buffer object.
     *
     * @retval int32_t                  Data size in bytes
     *
     * @details 
     * The data size corresponds to the data present in the buffer object. The data size will be same as 
     * capacity, if the offset is zero and size is equal to capacity
     */
    int32_t Size() const;

    /**
     * @brief 
     * Sets buffer capacity and data pointer.
     *
     * @param[in]  data                 Memory data pointer from client
     * @param[in]  capacity             Buffer capacity
     *
     * @details 
     * The data memory pointer and capacity of the movie::Buffer are updated with client provided values. 
     * If the memory is owned by the movie::Buffer, it will be released before memory pointer is updated.
     */
    void SetDataAndCapacity(void* data, size_t capacity);

    /**
     * @brief 
     * Sets buffer capacity.
     *
     * @param[in]  capacity             Buffer capacity
     *
     * @details 
     * The capacity points to client provided value. This API can be used to set buffer capacity
     */
    void SetCapacity(size_t capacity);

    /**
     * @brief 
     * Sets data offset and size.
     *
     * @param[in]  offset               Offset in bytes from data location
     * @param[in]  size                 Buffer capacity
     *
     * @details 
     * The data offset and data size is updated with client provided values. 
     */
    void SetRange(int32_t offset, int32_t size);

    /**
     * @brief 
     * Sets generic int32_t data.
     *
     * @param[in]  data                 Generic int32_t data
     *
     * @details 
     * This API can be used to store generic int32_t data. Note that a second attempt to store same 
     * generic data will overwrite previously set value. Client need to use GetInt32Data() API to 
     * retrieve the data.
     */
    void SetInt32Data(int32_t data);

    /**
     * @brief 
     * Get stored generic int32_t data
     *
     * @retval  int32_t                 Return generic int32_t data
     *
     * @details 
     * This API can be used to retrieve generic int32_t data, previously stored using SetInt32Data().
     */
    int32_t GetInt32Data() const;

    /**
     * @brief 
     * Sets generic int64_t data.
     *
     * @param[in]  data                 Generic int64_t data
     *
     * @details 
     * This API can be used to store generic int64_t data. Note that a second attempt to store same 
     * generic data will overwrite previously set value. Client need to use GetInt64Data() API to 
     * retrieve the data.
     */
    void SetInt64Data(int64_t data);

    /**
     * @brief 
     * Get stored generic int64 data
     *
     * @retval  int64_t                 Return generic int64_t data
     *
     * @details 
     * This API can be used to retrieve generic int64_t data, previously stored using SetInt64Data().
     */
    int64_t GetInt64Data() const;

    /**
     * @brief 
     * Sets generic uint32_t data.
     *
     * @param[in]  data                 Generic uint32_t data
     *
     * @details 
     * This API can be used to store generic uint32_t data. Note that a second attempt to store same 
     * generic data will overwrite previously set value. Client need to use GetUint32Data() API to 
     * retrieve the data.
     */
    void SetUint32Data(uint32_t data);
  
    /**
     * @brief 
     * Get stored generic uint32_t data
     *
     * @retval  uint32_t                 Return generic uint32_t data
     *
     * @details 
     * This API can be used to retrieve generic uint32_t data, previously stored using SetUint32Data().
     */
    uint32_t GetUint32Data() const;
    /**
     * @brief 
     * Sets nvn device object.
     *
     * @param[in]  nvndev               Nvn device object
     *
     * @details 
     * This API can be used to store nvn device. Note that a second attempt to store same 
     * generic data will overwrite previously set value. Client need to use GetNvnDev() API to 
     * retrieve the data.
     */
    void SetNvnDev(void *nvndev);

    /**
     * @brief
     * Get stored nvn device
     *
     * @retval  fill stored nvn device
     *
     * @details
     * This API can be used to retrieve nvn device, previously stored using SetNvnDev().
     */
    void* GetNvnDev() const;
    /**
     * @brief 
     * Sets nvn sync point object.
     *
     * @param[in]  nvnSync              NVN sync object
     *
     * @details 
     * This API can be used to store nvn sync data. Note that a second attempt to store same
     * generic data will overwrite previously set value. Client need to use GetNvnSync() API to
     * retrieve the data.
     */
    void SetNvnSync(void* nvnSync);

    /**
     * @brief
     * Get stored nvn sync data
     *
     * @retval  fill stored nvn sync data
     *
     * @details
     * This API can be used to retrieve nvn sync point data, previously stored using SetNvnSync().
     */
    void* GetNvnSync() const;
    
    /**
     * @brief
     * Swaps this Buffer object with another
     *
     * @param[in] other               Buffer to be swapped with
     *
     * @details 
     * Swaps the values of all members of this Buffer and other, including the transfer of ownership. 
     * Note: it is safe to perform this operation on a Buffer with itself.
     */
    void Swap(movie::Buffer& other) noexcept;    

    /**
     * @brief
     * Destroys Buffer object
     *
     * @details 
     * Destroy buffer object and delete data memory if it was allocated by buffer object.
     */
    ~Buffer();

private:
    int32_t m_RangeOffset;
    int32_t m_RangeLength;
    int32_t m_Int32Data;
    uint32_t m_Uint32Data;
    bool m_OwnsData;
    size_t m_Capacity;
    int64_t m_Int64Data;
    void* m_Data;
	void* m_NvnDevice;
	void* m_NvnSync;
};


/**
 * @brief 
 * A collection of typed name/value pairs. This data structure is used to pass media data between different modules.
 *
 * @details  
 * Objects are unsorted and stored in order of insertion. 
 * Only one item of a given name may exist at any time.  
 * Setting will overwrite any existing item even if it's a different type.
 * Collection holds at most 64 different name/value pairs. 
 */

class MediaData
{
public:

    /**
     * @brief
     * Constructor to create an empty MediaData object
     *
     * @details 
     * A new MediaData object is created.
     */
    MediaData();
    
        /**
     * @brief 
     * Destroys MediaData object
     *
     * @details 
     * Destroy MediaData object and delete any data memory allocated.
     */
    ~MediaData();

    /**
     * @brief 
     * Copy constructor to perform deep copy of MediaData object.
     *
     * @details 
     * This copy constructor does a deep copy of the MediaData object.
     */
    MediaData(const MediaData& otherMediaData);

    /**
     * @brief 
     * Operator overloading for assignment operator
     *
     * @details 
     * Assignment operator is overloaded. Data is copied from other object during assignment.
     */
    MediaData& operator= (const MediaData& otherMediaData);

    /**
     * @brief 
     * Clear any memory allocated internally by MediaData object.
     *
     * @details 
     * Deletes any memory allocated by MediaData object. It is recommended that clients call this API before 
     * deleting MediaData object.
     */
    void Clear();   
    
    /**
     * @brief 
     * Stores a generic int32_t data with a name key.
     *
     * @param[in]  name                 Key name to store
     * @param[in]  value                int32_t value to store
     *
     * @details 
     * This API can be used to store generic int32_t data with a name. Note that a second attempt to store same 
     * name key will overwrite previously stored value. 
     */
    void SetInt32(const char *name, int32_t value);
    
    /**
     * @brief 
     * Stores a generic int64_t data with a name key.
     *
     * @param[in]  name                 Key name to store
     * @param[in]  value                int64_t value to store
     *
     * @details 
     * This API can be used to store generic int64_t data with a name. Note that a second attempt to store same 
     * name key will overwrite previously stored value. 
     */
    void SetInt64(const char *name, int64_t value);
    
    /**
     * @brief 
     * Stores a generic size_t data with a name key.
     *
     * @param[in]  name                 Key name to store
     * @param[in]  value                size_t value to store
     *
     * @details 
     * This API can be used to store generic size_t data with a name. Note that a second attempt to store same 
     * name key will overwrite previously stored value. 
     */
    void SetSize(const char *name, size_t value);
    
    /**
     * @brief 
     * Stores a generic float data with a name key.
     *
     * @param[in]  name                 Key name to store
     * @param[in]  value                float value to store
     *
     * @details 
     * This API can be used to store generic float data with a name. Note that a second attempt to store same 
     * name key will overwrite previously stored value. 
     */
    void SetFloat(const char *name, float value);
    
    /**
     * @brief 
     * Stores a generic double data with a name key.
     *
     * @param[in]  name                 Key name to store
     * @param[in]  value                double value to store
     *
     * @details 
     * This API can be used to store generic double data with a name. Note that a second attempt to store same 
     * name key will overwrite previously stored value. 
     */
    void SetDouble(const char *name, double value);
    
    /**
     * @brief 
     * Stores a void pointer with a name key.
     *
     * @param[in]  name                 Key name to store
     * @param[in]  value                void pointer value to store
     *
     * @details 
     * This API can be used to store generic void pointer with a name. Note that a second attempt to store same 
     * name key will overwrite previously stored value. 
     */
    void SetPointer(const char *name, void *value);
    
    /**
     * @brief 
     * Stores a generic char string with a name key.
     *
     * @param[in]  name                 Key name to store
     * @param[in]  str                  char string to store
     *
     * @details 
     * This API can be used to store generic char string with a name. Note that a second attempt to store same 
     * name key will overwrite previously stored value. 
     */
    void SetString(const char* name, const char* str);
    
    /**
     * @brief 
     * Stores a buffer with a name key.
     *
     * @param[in]  name                 Key name to store
     * @param[in]  buffer               char string to store
     *
     * @details 
     * This API can be used to store buffer with a name. Note that a second attempt to store same name key will
     * overwrite previously stored value. 
     */
    void SetBuffer(const char *name, Buffer *buffer);
    
    /**
     * @brief 
     * Stores rectangle coordinates with a name key.
     *
     * @param[in] name                  Key name to store
     * @param[in] left                  Left coordinate value
     * @param[in] top                   Top coordinate value
     * @param[in] right                 Right coordinate value
     * @param[in] bottom                Bottom coordinate value
     *
     * @details 
     * This API can be used to store rectangle coordinates with a name. Note that a second attempt to store same 
     * name key will overwrite previously stored value. 
     */
    void SetRect(const char *name, int32_t left, int32_t top, int32_t right, int32_t bottom);
    
    /**
     * @brief 
     * Find whether the MediaData container has a given string key
     *
     * @param[in]  name                 Key name to search.
     *
     * @retval  true                    If @a name key is found
     * @retval  false                   If no @a name key is found
     *
     * @details 
     * Search for a key in MediaData container. API returns true is it is found, false otherwise.
     */ 
    bool Contains(const char *name) const;
    
    /**
     * @brief 
     * Search for a key with a name and retrieve int32_t value stored
     *
     * @param[in]  name                 Key name to find
     * @param[out] value                Found key value
     *
     * @retval  true                    If @a name key is found
     * @retval  false                   If no @a name key is found
     *
     * @details 
     * Search for a key in MediaData container. API returns true is it is found, false otherwise. If key is found
     * corresponding int32_t data stored is retrieved.
     */ 
    bool FindInt32(const char *name, int32_t *value) const;
    
    /**
     * @brief 
     * Search for a key with a name and retrieve int64_t value stored
     *
     * @param[in]  name                 Key name to find
     * @param[out] value                Found key value
     *
     * @retval  true                    If @a name key is found
     * @retval  false                   If no @a name key is found
     *
     * @details 
     * Search for a key in MediaData container. API returns true is it is found, false otherwise. If key is found
     * corresponding int64_t data stored is retrieved.
     */ 
    bool FindInt64(const char* name, int64_t* value) const;
    
    /**
     * @brief 
     * Search for a key with a name and retrieve size_t value stored
     *
     * @param[in]  name                 Key name to find
     * @param[out] value                Found key value
     *
     * @retval  true                    If @a name key is found
     * @retval  false                   If no @a name key is found
     *
     * @details 
     * Search for a key in MediaData container. API returns true is it is found, false otherwise. If key is found
     * corresponding size_t data stored is retrieved.
     */ 
    bool FindSize(const char* name, size_t* value) const;
    
    /**
     * @brief 
     * Search for a key with a name and retrieve float value stored
     *
     * @param[in]  name                 Key name to find
     * @param[out] value
     *
     * @retval  true                    If @a name key is found
     * @retval  false                   If no @a name key is found
     *
     * @details 
     * Search for a key in MediaData container. API returns true is it is found, false otherwise. If key is found
     * corresponding float data stored is retrieved.
     */ 
    bool FindFloat(const char* name, float* value) const;
    
    /**
     * @brief 
     * Search for a key with a name and retrieve double value stored
     * 
     * @param[in]  name                 Key name to find
     * @param[out] value                Found key value
     *
     * @retval  true                    If @a name key is found
     * @retval  false                   If no @a name key is found
     *
     * @details 
     * Search for a key in MediaData container. API returns true is it is found, false otherwise. If key is found
     * corresponding double data stored is retrieved.
     */ 
    bool FindDouble(const char* name, double* value) const;
    
    /**
     * @brief 
     * Search for a key with a name and retrieve void pointer stored
     * 
     * @param[in]  name                 Key name to find
     * @param[out] pValue               Found key value
     *
     * @retval  true                    If @a name key is found
     * @retval  false                   If no @a name key is found
     *
     * @details 
     * Search for a key in MediaData container. API returns true is it is found, false otherwise. If key is found
     * corresponding void pointer stored is retrieved.
     */ 
    bool FindPointer(const char* name, void** pValue) const;
    
    /**
     * @brief 
     * Search for a key with a name and retreive char string stored
     * 
     * @param[in]  name                 Key name to find
     * @param[out] value                Found key value
     *
     * @retval  true                    If @a name key is found
     * @retval  false                   If no @a name key is found
     *
     * @details 
     * Search for a key in MediaData container. API returns true is it is found, false otherwise. If key is found
     * corresponding char string stored is retrieved.
     */ 
    bool FindString(const char* name, const char** value) const;
    
    /**
     * @brief 
     * Search for a key with a name and retrieve buffer stored
     * 
     * @param[in]  name                 Key name to find
     * @param[out]  pBuffer             Return buffer address
     *
     * @retval  true                    If @a name key is found
     * @retval  false                   If no @a name key is found
     *
     * @details 
     * Search for a key in MediaData container. API returns true is it is found, false otherwise. If key is found
     * corresponding buffer stored is retrieved.
     */ 
    bool FindBuffer(const char* name, Buffer** pBuffer) const;
    
    /**
     * @brief 
     * Search for a key with a name and retrieve rectangle coordinates stored
     * 
     * @param[in]  name                 Key name to find
     * @param[out] left                 Left coordinate value
     * @param[out] top                  Top coordinate value
     * @param[out] right                Right coordinate value
     * @param[out] bottom               Bottom coordinate value
     *
     * @retval  true                    If @a name key is found
     * @retval  false                   If no @a name key is found
     *
     * @details 
     * Search for a key in MediaData container. API returns true is it is found, false otherwise. If key is found
     * corresponding rectangle coordinates stored are retrieved.
     */ 
    bool FindRect(const char* name, int32_t* left, int32_t* top, int32_t* right, int32_t* bottom) const;

    /**
     * @brief 
     * Count number of entries in in MediaData container
     *
     * @retval  size_t                  Returns number of elements in the container
     *
     * @details 
     * Returns number of elements in the MediaData container. 
     */ 
    size_t CountEntries() const;


    /**
     * @brief 
     * A virtual interface type that can be customized for use with the Visit API         
     *
     * @details 
     * The Visitor class defines a virtual interface that is used by the Visit API to
     * report the entries stored in the MediaData instance. When reporting, the MediaData
     * instance will, for each entry it contains, call into the Visitor with the name of 
     * the entry (const char*) and the entry's value (movie::Buffer entries are passed to 
     * the Visitor by const&). 
     */     
    class Visitor {                  
    public:                
        virtual void accept(const char*, int32_t) const noexcept {}
        virtual void accept(const char*, int64_t) const noexcept {}
        virtual void accept(const char*, size_t ) const noexcept {}
        virtual void accept(const char*, double ) const noexcept {}
        virtual void accept(const char*, float  ) const noexcept {}        
        virtual void accept(const char*, void*  ) const noexcept {}
        virtual void accept(const char*, const char*) const noexcept {}
        virtual void accept(const char*, movie::Buffer const&) const noexcept {}
        virtual void accept(const char*, int32_t left,  int32_t top, 
                                         int32_t right, int32_t bottom) const noexcept
                                         {
                                             (void)left;
                                             (void)top;
                                             (void)right;
                                             (void)bottom;
                                         }
        virtual ~Visitor() = 0;
    };

    /**
     * @brief
     * The MediaData instance calls back into the Visitor with each of its entries
     *
     * @details 
     * The Visit API provides a customizable way to query the entries of a MediaData instance. 
     * By implementing a type with the Visitor interface, users can specify a behavior for 
     * the various kinds of types that may be stored in a MediaData.
     * See the MovieDecoderPlayer sample for some example Visitors.
     */    
    void Visit(Visitor const&) const noexcept;
   
private:
      void* m_MediaDataImpl;
};

/**
 * @brief 
 * Create H264 decoder configuration data using a sequence parameter set and picture parameter set
 *
 * @param[in] seqParam                  H264 Sequence parameter set data pointer
 * @param[in] seqParamSize              H264 Sequence parameter set data size in bytes
 * @param[in] picParam                  H264 Picture parameter set data pointer
 * @param[in] picParamSize              H264 Picture parameter set data size in bytes
 * @param[out] config                   Created decoder configuration
 *
 * @retval  bool                        Returns true if configuration is created successfully
 * @retval  bool                        Returns false if configuration creation failed
 *
 * @details 
 * Create H264 decoder configuration data. This data is needed to initialize the H264 decoder.
 */ 
bool CreateH264DecoderConfigurationData(const uint8_t* seqParam, size_t seqParamSize, const uint8_t* picParam, size_t picParamSize, MediaData* config);

/**
* @brief
* Get a valid H264 NAL start position and its size.
*
* @param[in, out] inputData            Input H264 byte stream data
* @param[in, out] inputDataSize        Input H264 byte stream data size
* @param[out] nalStart                 Start position of NAL unit, without start code
* @param[out] nalSize                  NAL size in bytes, without start code size
*
* @retval  bool                        Returns true if successful
* @retval  bool                        Returns false if unsuccessful
*
* @details
* This API can be used to find NAL units and the client can copy the NAL data using NAL start and size information.
* If next NAL unit is not found @a inputData is set to NULL and @a inputDataSize is set to 0.
* If next NAL unit is found @a inputData points to end of NAL unit and @a inputDataSize is set to remaing data
* in the input buffer. If a complete NAL unit is not found(i.e. when next NAL unit start code is not found)
* @a nalSize returns data size of partial NAL unit. This size may include the partial start code of next NAL.
*/
bool GetH264NalUnitStartPositionAndNalSize(uint8_t** inputData, size_t* inputDataSize, uint8_t** nalStart, size_t* nalSize);

/**
* @brief
* Get H264 stream properties from sequence parameter set (SPS) data
*
* @param[in] seqParamSetData           H264 Sequence parameter set data pointer
* @param[in] seqParamSetSize           H264 Sequence parameter set data size in bytes
* @param[out] width                    Video width in number of lines
* @param[out] height                   Video height in number of lines
* @param[out] frameRate                Video presentation frame rate, frames per seconds.
*
* @retval  bool                        Returns true if successful
* @retval  bool                        Returns false if unsuccessful
*
* @details
*  This API can be used to get basic stream properties like video height, width and frame rate.
*/
bool GetH264PropertiresFromSeqParamSetData(const uint8_t* seqParamSetData, size_t seqParamSetSize, int32_t* width, int32_t* height, double* frameRate);

/**
 * @brief 
 * Create AAC decoder configuration data
 *
 * @param[in] profile                   AAC decoder profile to use
 * @param[in] samplingFrequency         Audio sample rate in hertz (eg: 48000)
 * @param[in] channelCount              Number of channels present in AAC audio
 * @param[out] config                   Created decoder configuration
 *
 * @retval  bool                        Returns true if configuration is created successfully
 * @retval  bool                        Returns false if configuration creation failed
 *
 * @details 
 * Create AAC audio decoder configuration data. This data is needed to initialize the AAC decoder.
 */ 
bool CreateAacDecoderConfigurationData(uint32_t profile, uint32_t samplingFrequency, uint32_t channelCount, MediaData* config);

/**
* @brief
* Create VP8 decoder configuration data 
*
* @param[in] width                     Video width in number of lines
* @param[in] height                    Video height in number of lines
* @param[in] frameRate                 Video presentation frame rate, frames per seconds
* @param[out] config                   Created decoder configuration
*
* @retval  bool                        Returns true if configuration is created successfully
* @retval  bool                        Returns false if configuration creation failed
*
* @details
* Create VP8 video decoder configuration data. This data is needed to initialize the VP8 decoder.
*/
bool CreateVp8DecoderConfigurationData(int32_t width, int32_t height, int32_t frameRate, MediaData* config);

/**
* @brief
* Create VP9 decoder configuration data
*
* @param[in] width                     Video width in number of lines
* @param[in] height                    Video height in number of lines
* @param[in] frameRate                 Video presentation frame rate, frames per seconds
* @param[out] config                   Created decoder configuration
*
* @retval  bool                        Returns true if configuration is created successfully
* @retval  bool                        Returns false if configuration creation failed
*
* @details
* Create VP9 video decoder configuration data. This data is needed to initialize the VP9 decoder.
*/
bool CreateVp9DecoderConfigurationData(int32_t width, int32_t height, int32_t frameRate, MediaData* config);

/**
 * @brief      
 * A function pointer for allocating memory.
 *
 * @details    
 * Can be passed to SetAllocator().
 */
typedef void* (*AllocateFunction)(size_t size, size_t alignment, void *userPtr);

/**
 * @brief      
 * A function pointer for freeing memory.
 * 
 * @details    
 * Can be passed to SetAllocator().
 */
typedef void  (*FreeFunction)(void *addr, void *userPtr);

/**
 * @brief      
 * A function pointer for reallocating memory.
 *
 * @details    
 * Can be passed to SetAllocator().
 */
typedef void* (*ReallocateFunction)(void* addr, size_t newSize, void *userPtr);

/**
 * @brief Set memory allocator for multimedia subsystem.
 *
 * @param[in]  allocate     A function pointer for allocating memory.
 * @param[in]  free         A function pointer for freeing memory.
 * @param[in]  reallocate   A function pointer for reallocating memory.
 * @param[in]  userPtr      User pointer that will be passed to callbacks.
 *
 * @details This function must be called before using any graphics APIs.
 */
void SetAllocator(AllocateFunction allocate, FreeFunction free, ReallocateFunction reallocate, void *userPtr);

/**
 * @brief Set core mask for movie libraries.
 *
 * @param[in]  coreAffinityMask     Specify usable cores in coreAffinityMask. If bit n in coreAffinityMask is set to 1, the thread can run on core n. 
 * 
 * @details This function must be called before the creation of Extractor and Decoder.
 */
void SetCoreMask(uint64_t coreAffinityMask);

/**
 * @brief 
 * This API can be used to set priorities for all the threads in movie library.
 *
 * @param[in]  threadPriority     Allowed range is 0 to 31. If value set beyond the range default thread setting will be applicable.
 *
 * @details
 * This API must be called before the creation of Extractor or Decoder.
 */
void SetThreadPriority(int threadPriority);

/**
 * @brief 
 * This API can be used to query the priority specified by SetThreadPriority(). If SetThreadPriority() has not been called, 
 * nn::os::DefaultThreadPriority will be returned.
 *
 * @retval int                         Returns a value in the range of [nn::os::HighestThreadPriority, nn::os::LowestThreadPriority]
 */
int GetThreadPriority();
}
