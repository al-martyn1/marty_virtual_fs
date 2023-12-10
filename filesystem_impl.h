/*! \file
    \brief IFileSystem implementation
*/

#pragma once

#include "umba/filename.h"
#include "umba/filesys.h"
#include "umba/string_plus.h"

// 
#include "filedata_encoder_impl.h"
#include "filename_encoder_impl.h"
#include "i_filesystem.h"
#include "virtual_fs_impl.h"


namespace marty_virtual_fs {

class FileSystemImpl : public         IFileSystem
                     , public         VirtualFsImpl
                     , public virtual FileDataEncoderImpl // нужно для перекодировки текста файлов
                     , public virtual FilenameEncoderImpl // нужно для перекодировки имен файлов
{

protected:

public:

    FileSystemImpl()                                  = default;
    FileSystemImpl(const FileSystemImpl &)            = default;
    FileSystemImpl(FileSystemImpl &&)                 = default;
    FileSystemImpl& operator=(const FileSystemImpl &) = default;
    FileSystemImpl& operator=(FileSystemImpl &&)      = default;


    // virtual std::string  encodeFilename( const std::wstring &str ) const override
    // virtual std::wstring decodeFilename( const std::string  &str ) const override

protected:


#if defined(WIN32) || defined(_WIN32)

    template<typename StringType>
    ErrorCode enumerateNativeDirectoryImpl(const StringType &path, std::vector< DirectoryEntryInfoT<StringType> > &entries) const
    {
        if (!umba::filesys::enumerateDirectory( path
                                              , [&](const StringType &name, bool isDir)
                                                {
                                                    DirectoryEntryInfoT<StringType> e;
                                                    e.entryName     = name;
                                                    e.fileTypeFlags = isDir ? FileTypeFlags::directory : FileTypeFlags::normalFile;
                                                    entries.emplace_back(e);
                                                    return true;
                                                }
                                              )
           )
        {
            return ErrorCode::genericError;
        }

        return ErrorCode::ok;
    }

#else // Generic POSIX - Linups etc

    ErrorCode enumerateNativeDirectoryImpl(const std::string &path, std::vector< DirectoryEntryInfoA > &entries) const
    {
        if (!umba::filesys::enumerateDirectory( path
                                              , [&](const std::string &name, bool isDir)
                                                {
                                                    DirectoryEntryInfo<StringType> e;
                                                    e.entryName     = name;
                                                    e.fileTypeFlags = isDir ? FileTypeFlags::directory : FileTypeFlags::normalFile;
                                                    entries.emplace_back(e);
                                                    return true;
                                                }
                                              )
           )
        {
            return ErrorCode::genericError;
        }

        return ErrorCode::ok;
    
    }

    ErrorCode enumerateNativeDirectoryImpl(const std::wstring &path, std::vector< DirectoryEntryInfoW > &entries) const
    {
        std::vector< DirectoryEntryInfoA > entriesA;
        ErrorCode e = enumerateNativeDirectoryImpl(decodeFilename(path), entriesA);
        if (e!=ErrorCode::ok)
        {
            return e;
        }

        for(const auto& ea: entriesA)
        {
            DirectoryEntryInfoW ew;
            ew.entryName     = encodeFilename(ea.entryName);
            ew.fileTypeFlags = ea.fileTypeFlags;
            entries.emplace_back(ew);
        }

        return e;
    }

#endif


#if defined(WIN32) || defined(_WIN32)

    bool forceCreateDirectory(const std::wstring &dirname) const
    {
        return umba::filesys::createDirectoryEx(dirname, true);
    }

    bool forceCreateDirectory(const std::string &dirname) const
    {
        return forceCreateDirectory(decodeFilename(dirname));
    }

#else // Generic POSIX - Linups etc

    // В линупсе всё работает через char* API - используем его как основное

    bool forceCreateDirectory(const std::wstring &dirname) const
    {
        return forceCreateDirectory(encodeFilename(dirname));
    }

    bool forceCreateDirectory(const std::string &dirname) const
    {
        return umba::filesys::createDirectoryEx(dirname, true);
    }

#endif


    template<typename StringType>
    StringType normalizeFilenameImpl(const StringType &fname) const
    {
        typedef typename StringType::value_type CharType;
        return umba::filename::makeCanonicalSimple(fname, StringType(1, (CharType)'.'), StringType(2, (CharType)'.'), (CharType)'/');
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

        if (!umba::filesys::isPathDirectory(nativePath))
        {
            return ErrorCode::notDirectory;
        }

        // enumerate native directory
        return enumerateNativeDirectoryImpl(nativePath, entries);

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

        return umba::filesys::isFileReadable(nativePath);
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

        return umba::filesys::isPathDirectory(nativePath);
    }

    template<typename StringType>
    ErrorCode readDataFileImpl(StringType fName, std::vector<std::uint8_t> &fData) const
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

        if (!umba::filesys::readFile(nativePath, fData))
        {
            return ErrorCode::genericError;
        }

        return ErrorCode::ok;
    }


    template<typename StringType>
    ErrorCode readTextFileImpl(StringType fName, std::wstring &fText) const
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

        std::string tmpStr;
        if (!umba::filesys::readFile(nativePath, tmpStr))
        {
            return ErrorCode::genericError;
        }

        fText = autoDecodeText(tmpStr);

        return ErrorCode::ok;
    }

    template<typename StringType>
    ErrorCode readTextFileImpl(StringType fName, std::string &fText) const
    {
        std::wstring wText;
        ErrorCode e = readTextFileImpl(fName, wText);
        if (e!=ErrorCode::ok)
        {
            return e;
        }

        fText = encodeText(wText);

        return ErrorCode::ok;
    }

    template<typename StringType>
    ErrorCode writeTextFileImpl(StringType fName, const std::string &fText, WriteFileFlags writeFlags) const
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

        if ((writeFlags&WriteFileFlags::forceCreateDir)!=0)
        {
            auto path = umba::filename::getPath(nativePath);
            forceCreateDirectory(path);
        }

        bool fOverwrite = (writeFlags&WriteFileFlags::forceOverwrite)!=0;

        if (!umba::filesys::writeFile(nativePath, fText.data(), fText.size(), fOverwrite))
        {
            return ErrorCode::genericError;
        }

        return ErrorCode::ok;
    }

    template<typename StringType>
    ErrorCode writeTextFileImpl(StringType fName, const std::wstring &fText, WriteFileFlags writeFlags) const
    {
        return writeTextFileImpl(fName, encodeText(fText), writeFlags);
    }

    template<typename StringType>
    ErrorCode writeDataFileImpl(StringType fName, const std::vector<std::uint8_t> &fData, WriteFileFlags writeFlags) const
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

        if ((writeFlags&WriteFileFlags::forceCreateDir)!=0)
        {
            auto path = umba::filename::getPath(nativePath);
            forceCreateDirectory(path);
        }

        bool fOverwrite = (writeFlags&WriteFileFlags::forceOverwrite)!=0;

        if (fData.empty())
        {
            std::uint8_t tmp = 0;
            if (!umba::filesys::writeFile(nativePath, &tmp, 0, fOverwrite))
            {
                return ErrorCode::genericError;
            }
        }
        else
        {
            if (!umba::filesys::writeFile(nativePath, fData.data(), fData.size(), fOverwrite))
            {
                return ErrorCode::genericError;
            }
        }

        return ErrorCode::ok;
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


}; // struct FileSystemImpl


} // namespace marty_virtual_fs


