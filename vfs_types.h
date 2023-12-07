#pragma once

#include "defs.h"
//
#include "vfs_enums.h"


namespace marty_virtual_fs{


template<typename StringType>
struct DirectoryEntryInfoT
{
    FileTypeFlags    fileTypeFlags;
    StringType       entryName    ;
};

typedef DirectoryEntryInfoT<std::string>   DirectoryEntryInfoA;
typedef DirectoryEntryInfoT<std::wstring>  DirectoryEntryInfoW;




} // namespace marty_virtual_fs

