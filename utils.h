#pragma once

#include <algorithm>
#include <string>

namespace marty_virtual_fs {


template<typename StringType> inline
StringType filterFileNameInvalidChars(StringType str)
{
    auto isInvalidChar = [&](typename StringType::value_type ch)
    {
        // https://en.wikipedia.org/wiki/Filename
        // Win32 - https://learn.microsoft.com/en-us/windows/win32/fileio/naming-a-file
        // https://stackoverflow.com/questions/1976007/what-characters-are-forbidden-in-windows-and-linux-directory-names
        // https://stackoverflow.com/questions/4814040/allowed-characters-in-filename
        switch(ch)
        {
            case (typename StringType::value_type)'<' : [[fallthrough]];
            case (typename StringType::value_type)'>' : [[fallthrough]];
            case (typename StringType::value_type)':' : [[fallthrough]];
            case (typename StringType::value_type)'\"': [[fallthrough]];
            case (typename StringType::value_type)'/' : [[fallthrough]];
            case (typename StringType::value_type)'\\': [[fallthrough]];
            case (typename StringType::value_type)'|' : [[fallthrough]];
            case (typename StringType::value_type)'?' : [[fallthrough]];
            case (typename StringType::value_type)'*' : [[fallthrough]];
            case (typename StringType::value_type)'%' :
            //case (typename StringType::value_type)'' : [[fallthrough]];
                return true;

            default:
                if (ch<(typename StringType::value_type)' ')
                    return true;
                else
                    return false;
        }
    };

    std::replace_if( str.begin(), str.end()
                   , isInvalidChar
                   , (typename StringType::value_type)'_'
                   );
    return str;
}


} // namespace marty_virtual_fs

