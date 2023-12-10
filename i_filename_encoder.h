/*! \file
    \brief File name and data encoder interface
*/

#pragma once


#include <string>

//
#include "vfs_types.h"

//
#include "warnings_disable.h"



namespace marty_virtual_fs {


struct IFilenameEncoder
{

    virtual ~IFilenameEncoder() {}

    virtual bool setFilenameEncoding(const std::string &encName) = 0;
    virtual std::string getFilenameEncoding() const = 0;

    virtual std::string  encodeFilename( const std::wstring &str ) const = 0;
    virtual std::string  encodeFilename( const std::string  &str ) const = 0;
    virtual std::wstring decodeFilename( const std::wstring &str ) const = 0;
    virtual std::wstring decodeFilename( const std::string  &str ) const = 0;

}; // struct IFilenameEncoder


} // namespace marty_virtual_fs


#include "warnings_restore.h"

