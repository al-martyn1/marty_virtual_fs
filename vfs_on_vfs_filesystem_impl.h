/*! \file
    \brief IFileSystem implementation
*/

#pragma once

#include "umba/filename.h"
//#include "umba/filesys.h"
#include "umba/string_plus.h"
#include "umba/regex_helpers.h"

// 
#include "filedata_encoder_impl.h"
#include "filename_encoder_impl.h"
#include "i_filesystem.h"
#include "virtual_fs_impl.h"

//
#include "filesystem_impl.h"

//
#include <algorithm>


namespace marty_virtual_fs {

class VfsOnVfsFileSystemImpl : public         IFileSystem
                             , public         VirtualFsImpl
{

protected:

    std::shared_ptr<IFileSystem>   pParentFs;

    void checkParentFs() const
    {
        if (!pParentFs)
        {
            throw std::runtime_error("VfsOnVfsFileSystemImpl: no parent filesystem assigned");
        }
    }

    IFileSystem* checkedPfs() const
    {
        checkParentFs();
        return pParentFs.get();
    }
    


public:


    VfsOnVfsFileSystemImpl(std::shared_ptr<IFileSystem> pfs) : pParentFs(pfs) {};
    VfsOnVfsFileSystemImpl()                                  = default;
    VfsOnVfsFileSystemImpl(const VfsOnVfsFileSystemImpl &)            = default;
    VfsOnVfsFileSystemImpl(VfsOnVfsFileSystemImpl &&)                 = default;
    VfsOnVfsFileSystemImpl& operator=(const VfsOnVfsFileSystemImpl &) = default;
    VfsOnVfsFileSystemImpl& operator=(VfsOnVfsFileSystemImpl &&)      = default;


    virtual std::uint8_t* swapByteOrder(std::uint8_t *pData, std::size_t dataSize) const override
    {
        return checkedPfs()->swapByteOrder(pData, dataSize);
    }

    virtual Endianness    getHostEndianness() const override
    {
        return checkedPfs()->getHostEndianness();
    }

    virtual std::uint8_t* convertEndiannessToHost  (std::uint8_t *pData, std::size_t dataSize, Endianness srcEndianness) const override
    {
        return checkedPfs()->convertEndiannessToHost(pData, dataSize, srcEndianness);
    }

    virtual std::uint8_t* convertEndiannessFromHost(std::uint8_t *pData, std::size_t dataSize, Endianness dstEndianness) const override
    {
        return checkedPfs()->convertEndiannessFromHost(pData, dataSize, dstEndianness);
    }

    virtual std::string  encodeText( const std::wstring &str ) const override
    {
        return checkedPfs()->encodeText(str);
    }

    virtual std::string  encodeText( const std::string  &str ) const override
    {
        return checkedPfs()->encodeText(str);
    }

    virtual std::wstring decodeText( const std::wstring &str ) const override
    {
        return checkedPfs()->decodeText(str);
    }

    virtual std::wstring decodeText( const std::string  &str ) const override
    {
        return checkedPfs()->decodeText(str);
    }


    virtual std::string  encodeFilename( const std::wstring &str ) const override
    {
        return checkedPfs()->encodeFilename(str);
    }

    virtual std::string  encodeFilename( const std::string  &str ) const override
    {
        return checkedPfs()->encodeFilename(str);
    }

    virtual std::wstring decodeFilename( const std::wstring &str ) const override
    {
        return checkedPfs()->decodeFilename(str);
    }

    virtual std::wstring decodeFilename( const std::string  &str ) const override
    {
        return checkedPfs()->decodeFilename(str);
    }


    virtual std::string  makePathCanonical(const std::string &p) const override
    {
        return VirtualFsImpl::makePathCanonical(p);
    }

    virtual std::wstring makePathCanonical(const std::wstring &p) const override
    {
        return VirtualFsImpl::makePathCanonical(p);
    }

    virtual std::string  makeNativePathCanonical(const std::string &p) const override
    {
        return checkedPfs()->makePathCanonical(p);
    }

    virtual std::wstring makeNativePathCanonical(const std::wstring &p) const override
    {
        return checkedPfs()->makePathCanonical(p);
    }


protected:


    template<typename StringType>
    StringType normalizeFilenameImpl(StringType fname) const
    {
        return checkedPfs()->normalizeFilename(fname);
    }

    template<typename StringType>
    bool forceCreateDirectory(const StringType &dirname) const
    {
        return checkedPfs()->createDirectory(dirname, true);
    }


    template<typename StringType>
    bool isVirtualRoot(StringType p) const
    {
        typedef typename StringType::value_type CharType;

        p = normalizeFilenameImpl(p);

        if (p.empty())
        {
            return true;
        }

        if (p.size()==1 && p[0]==(CharType)'/')
        {
            return true;
        }

        return false;
    }



    template<typename StringType>
    ErrorCode readDataFileImpl2(StringType fName, std::vector<std::uint8_t> &fData) const
    {
        fName = normalizeFilenameImpl(fName);

        if (isVirtualRoot(fName))
        {
            return ErrorCode::notFound;
        }

        StringType nativePath;
        ErrorCode err = toNativePathName(fName, nativePath);
        if (err!=ErrorCode::ok)
        {
            return err;
        }

        return checkedPfs()->readDataFile(nativePath, fData);
    }


    template<typename StringType>
    ErrorCode readTextFileImpl2(StringType fName, std::wstring &fText) const
    {
        fName = normalizeFilenameImpl(fName);

        if (isVirtualRoot(fName))
        {
            return ErrorCode::notFound;
        }

        StringType nativePath;
        ErrorCode err = toNativePathName(fName, nativePath);
        if (err!=ErrorCode::ok)
        {
            return err;
        }

        return checkedPfs()->readTextFile(nativePath, fText);
    }

    template<typename StringType>
    ErrorCode readTextFileImpl2(StringType fName, std::string &fText) const
    {
        std::wstring wText;
        ErrorCode e = readTextFileImpl2(fName, wText);
        if (e!=ErrorCode::ok)
        {
            return e;
        }

        fText = encodeText(wText);

        return ErrorCode::ok;
    }

    template<typename StringType>
    ErrorCode writeTextFileImpl2(StringType fName, const std::string &fText, WriteFileFlags writeFlags) const
    {
        if (getVfsGlobalReadonly())
        {
            return ErrorCode::accessDenied;
        }

        fName = normalizeFilenameImpl(fName);

        if (isVirtualRoot(fName))
        {
            return ErrorCode::notFound;
        }

        StringType nativePath;
        ErrorCode err = toNativePathName(fName, nativePath);
        if (err!=ErrorCode::ok)
        {
            return err;
        }

        return checkedPfs()->writeTextFile(nativePath, fText, writeFlags);
    }

    template<typename StringType>
    ErrorCode writeTextFileImpl2(StringType fName, const std::wstring &fText, WriteFileFlags writeFlags) const
    {
        return writeTextFileImpl(fName, encodeText(fText), writeFlags);
    }

    template<typename StringType>
    ErrorCode writeDataFileImpl2(StringType fName, const std::vector<std::uint8_t> &fData, WriteFileFlags writeFlags) const
    {
        if (getVfsGlobalReadonly())
        {
            return ErrorCode::accessDenied;
        }

        fName = normalizeFilenameImpl(fName);

        if (isVirtualRoot(fName))
        {
            return ErrorCode::notFound;
        }

        StringType nativePath;
        ErrorCode err = toNativePathName(fName, nativePath);
        if (err!=ErrorCode::ok)
        {
            return err;
        }

        return checkedPfs()->writeDataFile(nativePath, fData, writeFlags);
    }

    template<typename StringType>
    bool isFileExistAndReadableImpl(StringType fName) const
    {
        fName = normalizeFilenameImpl(fName);

        if (isVirtualRoot(fName))
        {
            return false;
        }

        StringType nativePath;
        ErrorCode err = toNativePathName(fName, nativePath);
        if (err!=ErrorCode::ok)
        {
            return false;
        }

        return checkedPfs()->isFileExistAndReadable(nativePath);
    }

    template<typename StringType>
    bool isDirectoryImpl(StringType dName) const
    {
        dName = normalizeFilenameImpl(dName);

        if (isVirtualRoot(dName))
        {
            return true;
        }

        StringType nativePath;
        ErrorCode err = toNativePathName(dName, nativePath);
        if (err!=ErrorCode::ok)
        {
            return false;
        }

        return checkedPfs()->isDirectory(nativePath);
    }

    //------------------------------
    template<typename StringType>
    ErrorCode enumerateDirectoryImpl(StringType dirPath, std::vector< DirectoryEntryInfoT<StringType> > &entries) const
    {
        entries.clear();

        dirPath = normalizeFilenameImpl(dirPath);

        if (isVirtualRoot(dirPath)) // Корень
        {
            // Перечисляем mount points

            std::map<std::wstring, MountPointInfo>::const_iterator mit = m_mountPoints.begin();
            for(; mit!=m_mountPoints.end(); ++mit)
            {
                DirectoryEntryInfoT<StringType> dirEntryInfo;
                dirEntryInfo.entryName     = filenameToStringType<StringType, std::wstring>(mit->second.name);
                dirEntryInfo.path          = dirPath;
                dirEntryInfo.fileTypeFlags = mit->second.flags;
                entries.emplace_back(dirEntryInfo);
            }

            return ErrorCode::ok;

        }

        StringType nativePath;
        ErrorCode err = toNativePathName(dirPath, nativePath);
        if (err!=ErrorCode::ok)
        {
            return err;
        }

        //if (!umba::filesys::isPathDirectory(nativePath))
        if (!checkedPfs()->isDirectory(nativePath))
        {
            return ErrorCode::notDirectory;
        }

        // enumerate native directory
        return checkedPfs()->enumerateDirectory(nativePath, entries);

    }

    template<typename StringType>
    ErrorCode enumerateDirectoryExImpl(const StringType &dirPath, EnumerateFlags enumerateFlags, SortFlags sortFlags, const std::vector<FileMaskInfoT<StringType> > &masks, std::vector<DirectoryEntryInfoT<StringType> > &entries) const
    {
        std::vector< DirectoryEntryInfoT<StringType> > entriesTmp;
        ErrorCode err = enumerateDirectoryImpl(dirPath, entriesTmp);
        if (err!=ErrorCode::ok)
        {
            return err;
        }

        std::vector< CompiledFileMaskInfoT<StringType> > compiledMasks;
        for(const auto &m: masks)
        {
            try
            {
                compiledMasks.emplace_back(m.compileRegex());
            }
            catch(...)
            {
            }
        }

        entries.clear();


        for(const auto &e : entriesTmp)
        {
            if ((enumerateFlags&EnumerateFlags::enumerateFiles)!=0) // Перечисляем файлы
            {
                if ((e.fileTypeFlags&FileTypeFlags::directory)!=0) 
                {
                    // Найден каталог - пропускаем
                    continue;
                }
            }

            if ((enumerateFlags&EnumerateFlags::enumerateDirectories)!=0) // Перечисляем каталоги
            {
                if ((e.fileTypeFlags&FileTypeFlags::directory)==0) 
                {
                    // Найден файл - пропускаем
                    continue;
                }
            }


            for(const auto &cm: compiledMasks)
            {
                try
                {
                    if (umba::regex_helpers::regexMatch( ((cm.fileMaskFlags&FileMaskFlags::matchExtOnly)!=0) ? e.entryExt : e.entryName
                                                       , cm.compiledMask
                                                       , std::regex_constants::match_default
                                                       ))
                    {
                        entries.emplace_back(e);
                        break; // маска сработала, продолжать не нужно
                    }
                }
                catch(...)
                {
                }
            }
        }
        //entries

        std::stable_sort( entries.begin(), entries.end()
                        , [&](const DirectoryEntryInfoT<StringType> &e1, const DirectoryEntryInfoT<StringType> &e2)
                          {
                              return compareDirectoryEntries(e1, e2, sortFlags) > 0;
                          }
                        );

        return ErrorCode::ok;
    }


    //------------------------------
    template<typename StringType>
    std::vector<DirectoryEntryInfoT<StringType> > enumerateDirectoryExImpl(const StringType &dirPath, EnumerateFlags enumerateFlags, SortFlags sortFlags, const std::vector<FileMaskInfoT<StringType> > &masks, ErrorCode *pErr) const
    {
        std::vector<DirectoryEntryInfoT<StringType> > entries;
        ErrorCode err = enumerateDirectoryExImpl(dirPath, enumerateFlags, sortFlags, masks, entries);
        if (pErr)
        {
            *pErr = err;
        }

        return entries;
    }


    //------------------------------
    ErrorCode readDataFileImpl(const std::wstring &fName, std::vector<std::uint8_t> &fData) const
    {
        return readDataFileImpl2(fName, fData);
    }

    ErrorCode readDataFileImpl(const std::string  &fName, std::vector<std::uint8_t> &fData) const
    {
        return readDataFileImpl2(fName, fData);
    }

    //------------------------------
    ErrorCode readTextFileImpl(const std::wstring & fName, std::wstring &fText) const
    {
        return readTextFileImpl2(fName, fText);
    }

    ErrorCode readTextFileImpl(const std::string & fName, std::wstring &fText) const
    {
        return readTextFileImpl2(fName, fText);
    }

    //------------------------------
    ErrorCode readTextFileImpl(const std::wstring &fName, std::string &fText) const
    {
        return readTextFileImpl2(fName, fText);
    }

    ErrorCode readTextFileImpl(const std::string &fName, std::string &fText) const
    {
        return readTextFileImpl2(fName, fText);
    }

    //------------------------------
    ErrorCode writeTextFileImpl(const std::wstring &fName, const std::string &fText, WriteFileFlags writeFlags) const
    {
        return writeTextFileImpl2(fName, fText, writeFlags);
    }

    ErrorCode writeTextFileImpl(const std::string &fName, const std::string &fText, WriteFileFlags writeFlags) const
    {
        return writeTextFileImpl2(fName, fText, writeFlags);
    }

    //------------------------------
    ErrorCode writeTextFileImpl(const std::wstring &fName, const std::wstring &fText, WriteFileFlags writeFlags) const
    {
        return writeTextFileImpl2(fName, fText, writeFlags);
    }

    ErrorCode writeTextFileImpl(const std::string &fName, const std::wstring &fText, WriteFileFlags writeFlags) const
    {
        return writeTextFileImpl2(fName, fText, writeFlags);
    }

    //------------------------------
    ErrorCode writeDataFileImpl(const std::wstring &fName, const std::vector<std::uint8_t> &fData, WriteFileFlags writeFlags) const
    {
        return writeDataFileImpl2(fName, fData, writeFlags);
    }

    ErrorCode writeDataFileImpl(const std::string &fName, const std::vector<std::uint8_t> &fData, WriteFileFlags writeFlags) const
    {
        return writeDataFileImpl2(fName, fData, writeFlags);
    }

    virtual int compareFilenames(const std::string  &n1, const std::string  &n2, SortFlags sortFlags) const override
    {
        return checkedPfs()->compareFilenames(n1, n2, sortFlags);
    }

    virtual int compareFilenames(const std::wstring &n1, const std::wstring &n2, SortFlags sortFlags) const override
    {
        return checkedPfs()->compareFilenames(n1, n2, sortFlags);
    }

    template<typename StringType>
    int compareDirectoryEntriesImpl(const DirectoryEntryInfoT<StringType> &e1, const DirectoryEntryInfoT<StringType> &e2, SortFlags sortFlags) const
    {
        return checkedPfs()->compareDirectoryEntries(e1, e2, sortFlags);
    }


    template<typename StringType>
    bool testMaskMatchImpl(const DirectoryEntryInfoT<StringType> &entry, const FileMaskInfoT<StringType> &mask) const
    {
        if (mask.fileMaskFlags==FileMaskFlags::invalid)
        {
            return false;
        }

        try
        {
            CompiledFileMaskInfoT<StringType>  compiledFileMaskInfo = mask.compileRegex();
            return umba::regex_helpers::regexMatch( ((compiledFileMaskInfo.fileMaskFlags&FileMaskFlags::matchExtOnly)!=0) ? entry.entryExt : entry.entryName
                                                  , compiledFileMaskInfo.compiledMask
                                                  , std::regex_constants::match_default
                                                  );
        }
        catch(...)
        {
            return false;
        }

    }

    template<typename StringType>
    ErrorCode createDirectoryImpl(StringType dirPath, bool bForce ) const
    {
        if (getVfsGlobalReadonly())
        {
            return ErrorCode::accessDenied;
        }

        dirPath = normalizeFilenameImpl(dirPath);

        if (isVirtualRoot(dirPath))
        {
            return ErrorCode::accessDenied;
        }

        StringType nativePath;
        ErrorCode err = toNativePathName(dirPath, nativePath);
        if (err!=ErrorCode::ok)
        {
            return err;
        }

        return checkedPfs()->createDirectory(nativePath, bForce);
    }


public:


    // Нормализует виртуальное имя файла, нормализует разделители пути, и схлопывает спец пути типа "."/"..", 
    // чтобы мамкины "хакеры" из скрипта не могли вылезти за пределы песочницы
    // Выставляем наружу, чтобы в скрипте можно было проверить, как выглядит путь, который будет реально использоваться
    std::string  normalizeFilename(const std::string  &fname) const override
    {
        return normalizeFilenameImpl(fname);
    }

    std::wstring normalizeFilename(const std::wstring &fname) const override
    {
        return normalizeFilenameImpl(fname);
    }


    virtual bool getErrorCodeString(ErrorCode e, std::string  &errStr) const override
    {
        errStr = enum_serialize(e);
        return !errStr.empty();
    }

    virtual bool getErrorCodeString(ErrorCode e, std::wstring &errStr) const override
    {
        std::string str;
        str = enum_serialize(e);
        errStr = decodeFilename(str); // Тут на кодировку ваще пофиг, всё равно только латиница
        return !errStr.empty();
    }


    //! Возвращает путь
    virtual std::string  getPath(const std::string  &fullName) const override // static
    {
        return getPathImpl(fullName);
    }

    virtual std::wstring getPath(const std::wstring &fullName) const override // static
    {
        return getPathImpl(fullName);
    }

    //! Возвращает имя и расширение
    virtual std::string  getFileName(const std::string  &fullName) const override // static
    {
        return getFileNameImpl(fullName);
    }

    virtual std::wstring getFileName(const std::wstring &fullName) const override // static
    {
        return getFileNameImpl(fullName);
    }


    //! Возвращает путь и имя
    virtual std::string  getPathFile(const std::string  &fullName) const override // static
    {
        return getPathFileImpl(fullName);
    }

    virtual std::wstring getPathFile(const std::wstring &fullName) const override // static
    {
        return getPathFileImpl(fullName);
    }


    //! Возвращает расширение
    virtual std::string  getExt(const std::string  &fullName) const override // static
    {
        return getExtImpl(fullName);
    }

    virtual std::wstring getExt(const std::wstring &fullName) const override // static
    {
        return getExtImpl(fullName);
    }


    //! Возвращает имя файла без пути и расширения
    virtual std::string  getName(const std::string  &fullName) const override // static
    {
        return getNameImpl(fullName);
    }

    virtual std::wstring getName(const std::wstring &fullName) const override // static
    {
        return getNameImpl(fullName);
    }


    //! Конкатенация путей
    virtual std::string  appendPath(const std::string  &pathAppendTo, const std::string  &appendPath) const override // static
    {
        return appendPathImpl(pathAppendTo, appendPath);
    }

    virtual std::wstring appendPath(const std::wstring &pathAppendTo, const std::wstring &appendPath) const override // static
    {
        return appendPathImpl(pathAppendTo, appendPath);
    }


    //! Добавление расширения
    virtual std::string  appendExt(const std::string  &nameAppendTo, const std::string  &appendExt) const override // static
    {
        return appendExtImpl(nameAppendTo, appendExt);
    }

    virtual std::wstring appendExt(const std::wstring &nameAppendTo, const std::wstring &appendExt) const override // static
    {
        return appendExtImpl(nameAppendTo, appendExt);
    }



    // Конвертация в/из нативных путей. Если путь вне вирт системы - ErrorCode::notFound. 
    // Если текущая ф. система не поддерживает конвертацию в нативное имя - ErrorCode::notSupported
    ErrorCode fromNativePathName(const std::string  &nativeName, std::string  &vfsName) const override
    {
        return virtualizeRealPathImpl( nativeName, vfsName);
    }

    ErrorCode fromNativePathName(const std::wstring &nativeName, std::wstring &vfsName) const override
    {
        return virtualizeRealPathImpl( nativeName, vfsName);
    }


    ErrorCode toNativePathName(const std::string  &vfsName, std::string  &nativeName) const override
    {
        return mapVirtualPathImpl( vfsName, nativeName);
    }

    ErrorCode toNativePathName(const std::wstring &vfsName, std::wstring &nativeName) const override
    {
        return mapVirtualPathImpl( vfsName, nativeName);
    }

    // Нерекурсивный обзор содержимого каталога
    ErrorCode enumerateDirectory(const std::string  &dirPath, std::vector<DirectoryEntryInfoA> &entries) const override
    {
        return enumerateDirectoryImpl(dirPath, entries);
    }

    ErrorCode enumerateDirectory(const std::wstring &dirPath, std::vector<DirectoryEntryInfoW> &entries ) const override
    {
        return enumerateDirectoryImpl(dirPath, entries);
    }


    std::vector<DirectoryEntryInfoA> enumerateDirectory(const std::string  &dirPath, ErrorCode *pErr = 0) const override
    {
        std::vector<DirectoryEntryInfoA> entries;
        ErrorCode e = enumerateDirectory(dirPath, entries);
        if (pErr)
        {
            *pErr = e;
        }

        return entries;
    }

    std::vector<DirectoryEntryInfoW> enumerateDirectory(const std::wstring &dirPath, ErrorCode *pErr = 0) const override
    {
        std::vector<DirectoryEntryInfoW> entries;
        ErrorCode e = enumerateDirectory(dirPath, entries);
        if (pErr)
        {
            *pErr = e;
        }

        return entries;
    }


    bool isFileExistAndReadable(const std::string  &fName) const override
    {
        return isFileExistAndReadableImpl(fName);
    }

    bool isFileExistAndReadable(const std::wstring &fName) const override
    {
        return isFileExistAndReadableImpl(fName);
    }


    bool isDirectory(const std::string  &dName) const override
    {
        return isDirectoryImpl(dName);
    }

    bool isDirectory(const std::wstring &dName) const override
    {
        return isDirectoryImpl(dName);
    }


    // Тут автоматически работают перекодировки текста
    ErrorCode readTextFile(const std::string  &fName, std::string  &fText) const override
    {
        return readTextFileImpl(fName, fText);
    }

    ErrorCode readTextFile(const std::string  &fName, std::wstring &fText) const override
    {
        return readTextFileImpl(fName, fText);
    }

    ErrorCode readTextFile(const std::wstring &fName, std::string  &fText) const override
    {
        return readTextFileImpl(fName, fText);
    }

    ErrorCode readTextFile(const std::wstring &fName, std::wstring &fText) const override
    {
        return readTextFileImpl(fName, fText);
    }


    // Reading binary files
    ErrorCode readDataFile(const std::string  &fName, std::vector<std::uint8_t> &fData) const override
    {
        return readDataFileImpl(fName, fData);
    }

    ErrorCode readDataFile(const std::wstring &fName, std::vector<std::uint8_t> &fData) const override
    {
        return readDataFileImpl(fName, fData);
    }


    ErrorCode writeTextFile(const std::string  &fName, const std::string  &fText, WriteFileFlags writeFlags) const override
    {
        return writeTextFileImpl(fName, fText, writeFlags);
    }

    ErrorCode writeTextFile(const std::string  &fName, const std::wstring &fText, WriteFileFlags writeFlags) const override
    {
        return writeTextFileImpl(fName, fText, writeFlags);
    }

    ErrorCode writeTextFile(const std::wstring &fName, const std::string  &fText, WriteFileFlags writeFlags) const override
    {
        return writeTextFileImpl(fName, fText, writeFlags);
    }

    ErrorCode writeTextFile(const std::wstring &fName, const std::wstring &fText, WriteFileFlags writeFlags) const override
    {
        return writeTextFileImpl(fName, fText, writeFlags);
    }


    ErrorCode writeDataFile(const std::string  &fName, const std::vector<std::uint8_t> &fData, WriteFileFlags writeFlags) const override
    {
        return writeDataFileImpl(fName, fData, writeFlags);
    }

    ErrorCode writeDataFile(const std::wstring &fName, const std::vector<std::uint8_t> &fData, WriteFileFlags writeFlags) const override
    {
        return writeDataFileImpl(fName, fData, writeFlags);
    }


    // std::string formatFiletime<std::string>( filetime_t t, const std::string &fmt )
    // Описание форматной строки тут - https://man7.org/linux/man-pages/man3/strftime.3.html
    virtual std::string  formatFiletime(FileTime ft, const std::string  &fmt) const override
    {
        return checkedPfs()->formatFiletime(ft, fmt);
    }

    virtual std::wstring formatFiletime(FileTime ft, const std::wstring &fmt) const override
    {
        return checkedPfs()->formatFiletime(ft, fmt);
    }


    virtual std::uint32_t getFileSizeLo(FileSize sz) const override
    {
        return (std::uint32_t)(sz&0xFFFFFFFull);
    }

    virtual std::uint32_t getFileSizeHi(FileSize sz) const override
    {
        return (std::uint32_t)(sz>>32);
    }

    virtual int compareDirectoryEntries(const DirectoryEntryInfoA &e1, const DirectoryEntryInfoA &e2, SortFlags sortFlags) const override
    {
        return compareDirectoryEntriesImpl(e1, e2, sortFlags);
    }

    virtual int compareDirectoryEntries(const DirectoryEntryInfoW &e1, const DirectoryEntryInfoW &e2, SortFlags sortFlags) const override
    {
        return compareDirectoryEntriesImpl(e1, e2, sortFlags);
    }


    // Возвращает 0, если совпадения не найдено, >0 - индекс маски, по которой найдено совпадение, <0 - индекс маски, на которой произошла какая-то ошибка (например, корявый regex)
    virtual bool testMaskMatch(const DirectoryEntryInfoA &entry, const FileMaskInfoA &mask) const override
    {
        return testMaskMatchImpl(entry, mask);
    }

    virtual bool testMaskMatch(const DirectoryEntryInfoW &entry, const FileMaskInfoW &mask) const override
    {
        return testMaskMatchImpl(entry, mask);
    }


    // Нерекурсивный обзор содержимого каталога, расширенная версия
    virtual ErrorCode enumerateDirectoryEx(const std::string  &dirPath, EnumerateFlags enumerateFlags, SortFlags sortFlags, const std::vector<FileMaskInfoA> &masks, std::vector<DirectoryEntryInfoA> &entries) const override
    {
        return enumerateDirectoryExImpl(dirPath, enumerateFlags, sortFlags, masks, entries);
    }

    virtual ErrorCode enumerateDirectoryEx(const std::wstring &dirPath, EnumerateFlags enumerateFlags, SortFlags sortFlags, const std::vector<FileMaskInfoW> &masks, std::vector<DirectoryEntryInfoW> &entries) const override
    {
        return enumerateDirectoryExImpl(dirPath, enumerateFlags, sortFlags, masks, entries);
    }


    virtual std::vector<DirectoryEntryInfoA> enumerateDirectoryEx(const std::string  &dirPath, EnumerateFlags enumerateFlags, SortFlags sortFlags, const std::vector<FileMaskInfoA> &masks, ErrorCode *pErr = 0) const override
    {
        return enumerateDirectoryExImpl(dirPath, enumerateFlags, sortFlags, masks, pErr);
    }

    virtual std::vector<DirectoryEntryInfoW> enumerateDirectoryEx(const std::wstring &dirPath, EnumerateFlags enumerateFlags, SortFlags sortFlags, const std::vector<FileMaskInfoW> &masks, ErrorCode *pErr = 0) const override
    {
        return enumerateDirectoryExImpl(dirPath, enumerateFlags, sortFlags, masks, pErr);
    }


    virtual ErrorCode createDirectory(const std::string  &dirPath, bool bForce ) const override
    {
        return createDirectoryImpl(dirPath, bForce );
    }

    virtual ErrorCode createDirectory(const std::wstring &dirPath, bool bForce ) const override
    {
        return createDirectoryImpl(dirPath, bForce );
    }


    virtual ErrorCode createMachineFilesystemMountPoints() override
    {
        return ErrorCode::notSupported;
    }


}; // struct FileSystemImpl


} // namespace marty_virtual_fs


