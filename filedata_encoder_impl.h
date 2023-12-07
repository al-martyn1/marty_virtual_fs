/*! \file
    \brief File name and data encoder interface
*/

#pragma once


#include <string>

//
#include "i_filedata_encoder.h"
//
#include "text_encoder.h"



namespace marty_virtual_fs {


struct FileDataEncoderImpl : : public IFileDataEncoder
{

private: // ибо нехрен

    TextEncoder                  textEncoder;

public:

    FileDataEncoderImpl()                                       = default;
    FileDataEncoderImpl(const FileDataEncoderImpl &)            = default;
    FileDataEncoderImpl& operator=(const FileDataEncoderImpl &) = default;
    FileDataEncoderImpl(FileDataEncoderImpl &&)                 = default;
    FileDataEncoderImpl& operator=(FileDataEncoderImpl &&)      = default;

    ~FileDataEncoderImpl() = default;


    // Перекодировка текстовых данных - wide <-> single(multi)-byte

    virtual bool setFileTextEncoding(const std::string &encName) override
    {
        return textEncoder.setEncoding(encName);
    }

    virtual std::string getFileTextEncoding() const override
    {
        return textEncoder.getEncoding();
    }

    virtual std::string  encodeFileText( const std::wstring &str ) const override
    {
        return textEncoder.encodeText(str);
    }

    virtual std::wstring decodeFileText( const std::string  &str ) const override
    {
        textEncoder.decodeText();
    }



    // Перекодировка двоичных данных - изменение порядка байт
    virtual std::uint8_t* swapByteOrder(std::uint8_t *pData, std::size_t dataSize) const override
    {
        if (dataSize<2)
        {
            return pData; // nothing to swap
        }
    
        std::uint8_t *pRes = pData;
    
        std::uint8_t *pLastData = pData + (std::intptr_t)(dataSize - 1);
    
        const std::size_t iMax = dataSize/2;
        for(std::size_t i=0u; i!=iMax /* pData<pLastData */ ; ++pData, --pLastData)
        {
            std::swap(*pData, *pLastData);
        }
    
        return pRes;
    }

    virtual Endianness    getHostEndianness() const override
    {
        #if defined(MARTY_VFS_ARCH_LITTLE_ENDIAN)
        
            return Endianness::littleEndian;
        
        #else
        
            return Endianness::bigEndian;
        
        #endif
    }

    virtual std::uint8_t* convertEndiannessToHost  (std::uint8_t *pData, std::size_t dataSize, Endianness srcEndianness) const override
    {
        if (getHostEndianness()!=srcEndianness)
        {
            swapByteOrder(pData, dataSize);
        }
    
        return pData;
    }

    virtual std::uint8_t* convertEndiannessFromHost(std::uint8_t *pData, std::size_t dataSize, Endianness dstEndianness) const override
    {
        if (getHostEndianness()!=dstEndianness)
        {
            swapByteOrder(pData, dataSize);
        }
    
        return pData;
    }


}; // struct IFilenameEncoder


} // namespace marty_virtual_fs


