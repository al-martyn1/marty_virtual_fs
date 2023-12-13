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


    template<typename StringType>
    StringType normalizeFilenameImpl(StringType fname) const
    {
        typedef typename StringType::value_type CharType;

        // Хвостовой слэш не должен нас путать, но только если он не единственный символ, а то скушаем корневой слэш
        if (fname.size()>1)
        {
            umba::filename::stripLastPathSep(fname);
        }

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

        if (!umba::filesys::readFile(nativePath, fData))
        {
            return ErrorCode::genericError;
        }

        return ErrorCode::ok;
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

        std::string tmpStr;
        if (!umba::filesys::readFile(nativePath, tmpStr))
        {
            return ErrorCode::genericError;
        }

        fText = autoDecodeText(tmpStr);

        return ErrorCode::ok;
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


#if defined(WIN32) || defined(_WIN32)

    // Под виндой юникодное апи первично

    ErrorCode enumerateNativeDirectoryImpl(const std::wstring &vPath, const std::wstring &path, std::vector< DirectoryEntryInfoW > &entries) const
    {
        if (!umba::filesys::enumerateDirectory( path
                                              , [&](const std::wstring &name, const umba::filesys::FileStat &fileStat)
                                                {
                                                    DirectoryEntryInfoW e;
                                                    e.entryName     = name;
                                                    e.path          = vPath;
                                                    fillDirectoryEntryInfoFromUmbaFilesysFileStat(fileStat, e);
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

    ErrorCode enumerateNativeDirectoryImpl(const std::string &vPath, const std::string &path, std::vector< DirectoryEntryInfoA > &entries) const
    {
        std::vector< DirectoryEntryInfoW > entriesW;
        ErrorCode e = enumerateNativeDirectoryImpl(decodeFilename(vPath), decodeFilename(path), entriesW);
        if (e!=ErrorCode::ok)
        {
            return e;
        }

        for(const auto& ew: entriesW)
        {
            DirectoryEntryInfoA ea = fromOppositeDirectoryEntryInfo(ew);
            ea.entryName           = encodeFilename(ew.entryName);
            ea.path                = encodeFilename(ew.path);
            entries.emplace_back(ea);
        }

        return e;
    }

    bool isFileExistAndReadableImpl(std::wstring fName) const
    {
        fName = normalizeFilenameImpl(fName);

        if (isVirtualRoot(fName))
        {
            return false;
        }

        std::wstring nativePath;
        ErrorCode err = toNativePathName(fName, nativePath);
        if (err!=ErrorCode::ok)
        {
            return false;
        }

        return umba::filesys::isFileReadable(nativePath);
    }

    bool isFileExistAndReadableImpl(std::string fName) const
    {
        return isFileExistAndReadableImpl(decodeFilename(fName));
    }

    bool isDirectoryImpl(std::wstring dName) const
    {
        dName = normalizeFilenameImpl(dName);

        if (isVirtualRoot(dName))
        {
            return true;
        }

        std::wstring nativePath;
        ErrorCode err = toNativePathName(dName, nativePath);
        if (err!=ErrorCode::ok)
        {
            return false;
        }

        return umba::filesys::isPathDirectory(nativePath);
    }

    bool isDirectoryImpl(std::string dName) const
    {
        return isDirectoryImpl(decodeFilename(dName));
    }

    bool forceCreateDirectory(const std::wstring &dirname) const
    {
        return umba::filesys::createDirectoryEx(dirname, true);
    }

    bool forceCreateDirectory(const std::string &dirname) const
    {
        // перекодируем локальным местечковым энкодером в юникодное имя
        return forceCreateDirectory(decodeFilename(dirname));
    }


#else // Generic POSIX - Linups etc

    ErrorCode enumerateNativeDirectoryImpl(const std::string &vPath, const std::string &path, std::vector< DirectoryEntryInfoA > &entries) const
    {
        if (!umba::filesys::enumerateDirectory( path
                                              , [&](const std::string &name, const umba::filesys::FileStat &fileStat)
                                                {
                                                    DirectoryEntryInfoA e;
                                                    e.entryName     = name;
                                                    e.path          = vPath;
                                                    fillDirectoryEntryInfoFromUmbaFilesysFileStat(fileStat, e);
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

    ErrorCode enumerateNativeDirectoryImpl(const std::wstring &vPath, const std::wstring &path, std::vector< DirectoryEntryInfoW > &entries) const
    {
        std::vector< DirectoryEntryInfoA > entriesA;
        ErrorCode e = enumerateNativeDirectoryImpl(decodeFilename(path), entriesA);
        if (e!=ErrorCode::ok)
        {
            return e;
        }

        for(const auto& ea: entriesA)
        {
            DirectoryEntryInfoW ew = fromOppositeDirectoryEntryInfo(ea);
            ew.entryName           = encodeFilename(ea.entryName);
            ew.path                = encodeFilename(ea.path);
            entries.emplace_back(ew);
        }

        return e;
    }

    bool isFileExistAndReadableImpl(std::string fName) const
    {
        fName = normalizeFilenameImpl(fName);

        if (isVirtualRoot(fName))
        {
            return false;
        }

        std::string nativePath;
        ErrorCode err = toNativePathName(fName, nativePath);
        if (err!=ErrorCode::ok)
        {
            return false;
        }

        return umba::filesys::isFileReadable(nativePath);
    }

    bool isFileExistAndReadableImpl(std::wstring fName) const
    {
        return isFileExistAndReadableImpl(encodeFilename(fName));
    }

    bool isDirectoryImpl(std::string dName) const
    {
        dName = normalizeFilenameImpl(dName);

        if (isVirtualRoot(dName))
        {
            return true;
        }

        std::string nativePath;
        ErrorCode err = toNativePathName(dName, nativePath);
        if (err!=ErrorCode::ok)
        {
            return false;
        }

        return umba::filesys::isPathDirectory(nativePath);
    }

    bool isDirectoryImpl(std::wstring dName) const
    {
        return isDirectoryImpl(encodeFilename(dName));
    }

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


#if defined(WIN32) || defined(_WIN32)

    // Под виндой юникодное апи первично

    //------------------------------
    ErrorCode readDataFileImpl(const std::wstring &fName, std::vector<std::uint8_t> &fData) const
    {
        return readDataFileImpl2(fName, fData);
    }

    ErrorCode readDataFileImpl(const std::string  &fName, std::vector<std::uint8_t> &fData) const
    {
        return readDataFileImpl2(decodeFilename(fName), fData);
    }

    //------------------------------
    
    //------------------------------
    ErrorCode readTextFileImpl(const std::wstring & fName, std::wstring &fText) const
    {
        return readTextFileImpl2(fName, fText);
    }

    ErrorCode readTextFileImpl(const std::string & fName, std::wstring &fText) const
    {
        return readTextFileImpl2(decodeFilename(fName), fText);
    }

    //------------------------------
    ErrorCode readTextFileImpl(const std::wstring &fName, std::string &fText) const
    {
        return readTextFileImpl2(fName, fText);
    }

    ErrorCode readTextFileImpl(const std::string &fName, std::string &fText) const
    {
        return readTextFileImpl2(decodeFilename(fName), fText);
    }

    //------------------------------
    ErrorCode writeTextFileImpl(const std::wstring &fName, const std::string &fText, WriteFileFlags writeFlags) const
    {
        return writeTextFileImpl2(fName, fText, writeFlags);
    }

    ErrorCode writeTextFileImpl(const std::string &fName, const std::string &fText, WriteFileFlags writeFlags) const
    {
        return writeTextFileImpl2(decodeFilename(fName), fText, writeFlags);
    }

    //------------------------------
    ErrorCode writeTextFileImpl(const std::wstring &fName, const std::wstring &fText, WriteFileFlags writeFlags) const
    {
        return writeTextFileImpl2(fName, fText, writeFlags);
    }

    ErrorCode writeTextFileImpl(const std::string &fName, const std::wstring &fText, WriteFileFlags writeFlags) const
    {
        return writeTextFileImpl2(decodeFilename(fName), fText, writeFlags);
    }

    //------------------------------
    ErrorCode writeDataFileImpl(const std::wstring &fName, const std::vector<std::uint8_t> &fData, WriteFileFlags writeFlags) const
    {
        return writeDataFileImpl2(fName, fData, writeFlags);
    }

    ErrorCode writeDataFileImpl(const std::string &fName, const std::vector<std::uint8_t> &fData, WriteFileFlags writeFlags) const
    {
        return writeDataFileImpl2(decodeFilename(fName), fData, writeFlags);
    }


#else // Generic POSIX - Linups etc

    // Под лиеупом ascii апи первично

    ErrorCode readDataFileImpl(const std::string &fName, std::vector<std::uint8_t> &fData) const
    {
        return readDataFileImpl2(fName, fData)
    }

    ErrorCode readDataFileImpl(const std::wstring &fName, std::vector<std::uint8_t> &fData) const
    {
        return readDataFileImpl2(encodeFilename(fName), fData)
    }

    ErrorCode readTextFileImpl(const std::string &fName, std::wstring &fText) const
    {
        return readTextFileImpl2(fName, fText);
    }

    ErrorCode readTextFileImpl(const std::wstring &fName, std::wstring &fText) const
    {
        return readTextFileImpl2(encodeFilename(fName), fText);
    }


    ErrorCode readTextFileImpl(const std::string &fName, std::string &fText) const
    {
        readTextFileImpl2(fName, fText);
    }

    ErrorCode readTextFileImpl(const std::wstring &fName, std::string &fText) const
    {
        readTextFileImpl2(encodeFilename(fName), fText);
    }


    ErrorCode writeTextFileImpl(const std::string  &fName, const std::string &fText, WriteFileFlags writeFlags) const
    {
        return writeTextFileImpl2(fName, fText, writeFlags);
    }

    ErrorCode writeTextFileImpl(const std::wstring &fName, const std::string &fText, WriteFileFlags writeFlags) const
    {
        return writeTextFileImpl2(encodeFilename(fName), fText, writeFlags);
    }


    ErrorCode writeTextFileImpl(const std::string  &fName, const std::wstring &fText, WriteFileFlags writeFlags) const
    {
        return writeTextFileImpl2(fName, fText, writeFlags);
    }

    ErrorCode writeTextFileImpl(const std::wstring &fName, const std::wstring &fText, WriteFileFlags writeFlags) const
    {
        return writeTextFileImpl2(encodeFilename(fName), fText, writeFlags);
    }


    ErrorCode writeDataFileImpl(const std::string  &fName, const std::vector<std::uint8_t> &fData, WriteFileFlags writeFlags) const
    {
        return writeTextFileImpl2(fName, fData, writeFlags);
    }

    ErrorCode writeDataFileImpl(const std::wstring &fName, const std::vector<std::uint8_t> &fData, WriteFileFlags writeFlags) const
    {
        return writeTextFileImpl2(encodeFilename(fName), fData, writeFlags);
    }



#endif



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

        if (!umba::filesys::isPathDirectory(nativePath))
        {
            return ErrorCode::notDirectory;
        }

        // enumerate native directory
        return enumerateNativeDirectoryImpl(dirPath, nativePath, entries);

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


    virtual void getErrorCodeString(ErrorCode e, std::string  &errStr) const override
    {
        errStr = enum_serialize(e);
    }

    virtual void getErrorCodeString(ErrorCode e, std::wstring &errStr) const override
    {
        std::string str;
        str = enum_serialize(e);
        errStr = decodeFilename(str); // Тут на кодировку ваще пофиг, всё равно только латиница
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
        return appendPathImpl(nameAppendTo, appendExt);
    }

    virtual std::wstring appendExt(const std::wstring &nameAppendTo, const std::wstring &appendExt) const override // static
    {
        return appendPathImpl(nameAppendTo, appendExt);
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
        return umba::filesys::formatFiletime(ft, fmt);
    }

    virtual std::wstring formatFiletime(FileTime ft, const std::wstring &fmt) const override
    {
        return decodeFilename(umba::filesys::formatFiletime(ft, encodeFilename(fmt)));
    }


    virtual std::uint32_t getFileSizeLo(FileSize sz) const override
    {
        return (std::uint32_t)(sz&0xFFFFFFFull);
    }

    virtual std::uint32_t getFileSizeHi(FileSize sz) const override
    {
        return (std::uint32_t)(sz>>32);
    }



}; // struct FileSystemImpl


} // namespace marty_virtual_fs


