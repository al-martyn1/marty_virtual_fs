#pragma once

#include "defs.h"
//
#include "vfs_enums.h"
//
#include "umba/filesys.h"
//
#include "umba/regex_helpers.h"
#include "umba/string_plus.h"


//----------------------------------------------------------------------------
namespace marty_virtual_fs{




//----------------------------------------------------------------------------
typedef umba::filesys::filesize_t  FileSize;
typedef umba::filesys::filetime_t  FileTime;

//----------------------------------------------------------------------------



//----------------------------------------------------------------------------
template<typename StringType>
struct DirectoryEntryInfoT
{
    FileTypeFlags    fileTypeFlags    = FileTypeFlags::normalFile;
    FileSize         fileSize         = 0;
    FileTime         timeCreation     = 0;
    FileTime         timeLastModified = 0;
    FileTime         timeLastAccess   = 0;

    StringType       entryName       ;
    StringType       entryExt        ;
    StringType       path            ; // Имеет ли смысл сделать shared_ptr<StringType>, чтобы память экономить и лишнее не копировать тудым сюдым?

};

//------------------------------
typedef DirectoryEntryInfoT<std::string>   DirectoryEntryInfoA;
typedef DirectoryEntryInfoT<std::wstring>  DirectoryEntryInfoW;

//------------------------------
template<typename StringType> inline
void fillDirectoryEntryInfoFromUmbaFilesysFileStat(const umba::filesys::FileStat &fileStat, DirectoryEntryInfoT<StringType> &dirInfo)
{
    dirInfo.fileTypeFlags    = (fileStat.fileType==umba::filesys::FileType::FileTypeDir) ? FileTypeFlags::directory : FileTypeFlags::normalFile;
    dirInfo.fileSize         = fileStat.fileSize         ;
    dirInfo.timeCreation     = fileStat.timeCreation     ;
    dirInfo.timeLastModified = fileStat.timeLastModified ;
    dirInfo.timeLastAccess   = fileStat.timeLastAccess   ;

}

//------------------------------
inline
DirectoryEntryInfoA fromOppositeDirectoryEntryInfo(const DirectoryEntryInfoW &infoW)
{
    DirectoryEntryInfoA infoA;
    infoA.fileTypeFlags    = infoW.fileTypeFlags   ;
    infoA.fileSize         = infoW.fileSize        ;
    infoA.timeCreation     = infoW.timeCreation    ;
    infoA.timeLastModified = infoW.timeLastModified;
    infoA.timeLastAccess   = infoW.timeLastAccess  ;
    return infoA;
}

//------------------------------
inline
DirectoryEntryInfoW fromOppositeDirectoryEntryInfo(const DirectoryEntryInfoA &infoA)
{
    DirectoryEntryInfoW infoW;
    infoW.fileTypeFlags    = infoA.fileTypeFlags   ;
    infoW.fileSize         = infoA.fileSize        ;
    infoW.timeCreation     = infoA.timeCreation    ;
    infoW.timeLastModified = infoA.timeLastModified;
    infoW.timeLastAccess   = infoA.timeLastAccess  ;
    return infoW;
}

//----------------------------------------------------------------------------



//----------------------------------------------------------------------------
template<typename StringType>
struct CompiledFileMaskInfoT
{
    FileMaskFlags                                         fileMaskFlags = FileMaskFlags::matchSimple;
    std::basic_regex<typename StringType::value_type>     compiledMask  ;

}; // struct FileMaskInfoT

//------------------------------
typedef CompiledFileMaskInfoT<std::string>   CompiledFileMaskInfoA;
typedef CompiledFileMaskInfoT<std::wstring>  CompiledFileMaskInfoW;

//----------------------------------------------------------------------------



//----------------------------------------------------------------------------
template<typename StringType>
struct FileMaskInfoT
{
    FileMaskFlags      fileMaskFlags = FileMaskFlags::matchSimple;
    StringType         mask;


    // requires wrapping into try/catch
    CompiledFileMaskInfoT<StringType> compileRegex() const
    {
        CompiledFileMaskInfoT<StringType> compiledFileMaskInfoMask;
        compiledFileMaskInfoMask.fileMaskFlags = fileMaskFlags;

        StringType expandedRegexStr;
        if ((fileMaskFlags&FileMaskFlags::matchRegex)!=0)
        {
            expandedRegexStr = mask;
        }
        else
        {
            expandedRegexStr = umba::regex_helpers::expandSimpleMaskToEcmaRegex(mask, (fileMaskFlags&FileMaskFlags::useAnchors)!=0, false);
        }

        compiledFileMaskInfoMask.compiledMask = std::basic_regex<typename StringType::value_type>(expandedRegexStr);

        return compiledFileMaskInfoMask;
    }

}; // struct FileMaskInfoT

//------------------------------
typedef FileMaskInfoT<std::string>   FileMaskInfoA;
typedef FileMaskInfoT<std::wstring>  FileMaskInfoW;

//----------------------------------------------------------------------------



//----------------------------------------------------------------------------

} // namespace marty_virtual_fs

