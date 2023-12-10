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


    bool setEncoding(const std::string &eName)
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

    std::string getEncoding() const
    {
        return encName;
    }

    std::string  encodeText( const std::wstring &str ) const
    {
        encoding::EncodingsApi* pApi = encoding::getEncodingsApi();
        return pApi->encode(str, encCodepageId);
    }

    std::string  encodeText( const std::string &str ) const
    {
        return str;
    }

    std::wstring decodeText( const std::string  &str ) const
    {
        encoding::EncodingsApi* pApi = encoding::getEncodingsApi();
        return pApi->decode(str, encCodepageId);
    }

    std::wstring decodeText( const std::wstring  &str ) const
    {
        return str;
    }

    std::wstring autoDecodeText(std::string str) const
    {
        encoding::EncodingsApi* pApi = encoding::getEncodingsApi();

        size_t bomSize = 0;
        std::string detectRes = pApi->detect( str, bomSize );

        if (bomSize)
            str.erase(0,bomSize);

        auto cpId = pApi->getCodePageByName(detectRes);

        // return 
        //pApi->convert( str, cpId, encCodepageId /* encoding::EncodingsApi::cpid_UTF8 */ );

        return pApi->decode(str, cpId);
    }


}; // struct TextEncoder





} // namespace marty_virtual_fs


