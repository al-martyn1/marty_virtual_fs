/*! \file
    \brief File names encoder interface
*/

#pragma once


#include <string>

//
#include "i_filename_encoder.h"

//
#include "text_encoder.h"


namespace marty_virtual_fs {


struct FilenameEncoderImpl : public IFilenameEncoder
{

private: // ибо нехрен

    TextEncoder                  filenameEncoder;

public:

    FilenameEncoderImpl()                                       = default;
    FilenameEncoderImpl(const FilenameEncoderImpl &)            = default;
    FilenameEncoderImpl& operator=(const FilenameEncoderImpl &) = default;
    FilenameEncoderImpl(FilenameEncoderImpl &&)                 = default;
    FilenameEncoderImpl& operator=(FilenameEncoderImpl &&)      = default;

    ~FilenameEncoderImpl() = default;


    virtual bool setFilenameEncoding(const std::string &encName) override
    {
        return filenameEncoder.setEncoding(encName);
    }

    virtual std::string getFilenameEncoding() const override
    {
        return filenameEncoder.getEncoding();
    }

    virtual std::string  encodeFilename( const std::wstring &str ) const override
    {
        return filenameEncoder.encodeText(str);
    }

    virtual std::string  encodeFilename( const std::string &str ) const override
    {
        return str;
    }

    virtual std::wstring decodeFilename( const std::string  &str ) const override
    {
        return filenameEncoder.decodeText(str);
    }

    virtual std::wstring decodeFilename( const std::wstring &str ) const override
    {
        return str;
    }


}; // struct FilenameEncoderImpl


} // namespace marty_virtual_fs



