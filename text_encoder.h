/*! \file
    \brief File name and data encoder interface
*/

#pragma once


#include <exception>
#include <stdexcept>
#include <cstddef>
#include <cstring>
#include <memory>
#include <string>
#include <vector>
#include <utility>

//
#include "encoding/encoding.h"

namespace marty_virtual_fs {


struct TextEncoder
{

private: // ибо нехрен

    std::string                                 encName       = "UTF-8"; // "CP866"; // UTF-8 65001
    unsigned                                    encCodepageId = 65001  ; // 866;

public:

    TextEncoder()                               = default;
    TextEncoder(const TextEncoder &)            = default;
    TextEncoder& operator=(const TextEncoder &) = default;
    TextEncoder(TextEncoder &&)                 = default;
    TextEncoder& operator=(TextEncoder &&)      = default;



    virtual bool setEncoding(const std::string &eName) override
    {
        encoding::EncodingsApi* pApi = encoding::getEncodingsApi();
        unsigned cp = pApi->getCodePageByName(eName);
        if (!cp)
        {
            return false;
        }

        encName       = eName;
        encCodepageId = cp;

        return true;
    }

    virtual std::string getEncoding() const override
    {
        return encName;
    }

    virtual std::string  encodeText( const std::wstring &str ) const override
    {
        encoding::EncodingsApi* pApi = encoding::getEncodingsApi();
        return pApi->encode(str, encCodepageId);
    }

    virtual std::wstring decodeText( const std::string  &str ) const override
    {
        encoding::EncodingsApi* pApi = encoding::getEncodingsApi();
        return pApi->decode(str, encCodepageId);
    }


}; // struct TextEncoder





} // namespace marty_virtual_fs


