/*! \file
    \brief File name and data encoder interface
*/

#pragma once


#include <string>

//
#include "vfs_types.h"

//
#include "i_filename_encoder.h"

//
#include "warnings_disable.h"


namespace marty_virtual_fs {


struct IFileDataEncoder
{

    virtual ~IFileDataEncoder() {}

    // Перекодировка двоичных данных - изменение порядка байт
    virtual std::uint8_t* swapByteOrder(std::uint8_t *pData, std::size_t dataSize) const = 0;

    virtual Endianness    getHostEndianness() const = 0;
    virtual std::uint8_t* convertEndiannessToHost  (std::uint8_t *pData, std::size_t dataSize, Endianness srcEndianness) const = 0;
    virtual std::uint8_t* convertEndiannessFromHost(std::uint8_t *pData, std::size_t dataSize, Endianness dstEndianness) const = 0;


    // Перекодировка текстовых данных - wide <-> single(multi)-byte
    virtual bool setTextEncoding(const std::string &encName) = 0;
    virtual std::string getTextEncoding() const = 0;

    virtual std::wstring autoDecodeText( const std::string  &str ) const = 0;

    virtual std::string  encodeText( const std::wstring &str ) const = 0;
    virtual std::string  encodeText( const std::string  &str ) const = 0;
    virtual std::wstring decodeText( const std::wstring &str ) const = 0;
    virtual std::wstring decodeText( const std::string  &str ) const = 0;

}; // struct IFileDataEncoder


} // namespace marty_virtual_fs


#include "warnings_restore.h"

