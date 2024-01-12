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
#include "i_virtual_fs.h"

//
#include "utils.h"

namespace marty_virtual_fs {


class VirtualFsImpl : public         IVirtualFs
                    , public virtual FilenameEncoderImpl // нужно для перекодировки имен файлов
                    //, public virtual FileDataEncoderImpl // нужно для перекодировки текста файлов
{

protected:

    bool        m_readOnly = false;

public:

    VirtualFsImpl()                                 = default;
    VirtualFsImpl(const VirtualFsImpl &)            = default;
    VirtualFsImpl(VirtualFsImpl &&)                 = default;
    VirtualFsImpl& operator=(const VirtualFsImpl &) = default;
    VirtualFsImpl& operator=(VirtualFsImpl &&)      = default;


protected:

    template<typename StringType>
    StringType makePathCanonicalImpl(StringType p) const
    {
        typedef typename StringType::value_type CharType;

        // Хвостовой слэш не должен нас путать, но только если он не единственный символ, а то скушаем корневой слэш
        if (p.size()>1)
        {
            umba::filename::stripLastPathSep(p);
        }

        return umba::filename::makeCanonicalSimple(p, StringType(1, (CharType)'.'), StringType(2, (CharType)'.'), (CharType)'/');
    }

    template<typename StringType>
    StringType makeNativePathCanonicalImpl(StringType p) const
    {
        return umba::filename::makeCanonical(p);
    }

    

    virtual std::string  makePathCanonical(const std::string &p) const override
    {
        return makePathCanonicalImpl(p);
    }

    virtual std::wstring makePathCanonical(const std::wstring &p) const override
    {
        return makePathCanonicalImpl(p);
    }

    virtual std::string  makeNativePathCanonical(const std::string &p) const override
    {
        return makeNativePathCanonicalImpl(p);
    }

    virtual std::wstring makeNativePathCanonical(const std::wstring &p) const override
    {
        return makeNativePathCanonicalImpl(p);
    }


protected:

    std::wstring prepareMountPointKey(const std::wstring &k) const
    {
        #if defined(WIN32) || defined(_WIN32)

            return umba::string_plus::toupper_copy( k );

        #else // Generic POSIX - Linups etc

            return k;

        #endif
    }

    std::wstring prepareMountPointKey(const std::string &k) const
    {
        return prepareMountPointKey(decodeFilename(k));
    }

    template<typename StringType>
    StringType prepareMountTarget(StringType mt)
    {
        // Пути могут даваться с с разделителем пути в конце
        umba::filename::stripLastPathSep(mt);
        return mt;
    }


    struct MountPointInfo
    {
        std::wstring name  ;
        std::wstring target;
        FileTypeFlags flags;

    }; // struct MountPointInfo


    // Это конечно медленнее, чем unordered_map, но сортировка автоматом. Да и mount points будут в таком количестве, что вряд ли будут сильно влиять на что-то
    std::map<std::wstring, MountPointInfo> m_mountPoints;


    template<typename StringType>
    ErrorCode removeMountPointImpl(const StringType &k)
    {
        std::wstring key = prepareMountPointKey(k);
        std::map<std::wstring, MountPointInfo>::const_iterator it = m_mountPoints.find(key);
        if (it==m_mountPoints.end())
        {
            return ErrorCode::invalidMountPoint;
        }

        m_mountPoints.erase(it);

        return ErrorCode::ok;
    }

    template<typename StringType>
    ErrorCode addMountPointExImpl( const StringType &mntPointName, const StringType &mntPointTarget, FileTypeFlags flags )
    {
        auto mntPointNameFiltered = filterFileNameInvalidChars(mntPointName);
        if (mntPointNameFiltered!=mntPointName)
        {
            return ErrorCode::invalidMountPoint; // invalidMountPoint or invalidName?
        }

        std::wstring key = prepareMountPointKey(mntPointName);
        std::map<std::wstring, MountPointInfo>::const_iterator it = m_mountPoints.find(key);
        if (it!=m_mountPoints.end())
        {
            return ErrorCode::alreadyExist;
        }

        //TODO: !!! Надо проверить, ссылается ли mntPointTarget на что-то реальное

        MountPointInfo mntInfo;
        mntInfo.name   = decodeFilename(mntPointName);
        mntInfo.target = prepareMountTarget(decodeFilename(mntPointTarget));
        mntInfo.flags  = flags;

        m_mountPoints[key] = mntInfo;

        return ErrorCode::ok;
    }

    template<typename StringType>
    std::vector< StringType > splitVirtualPath(const StringType &vp) const
    {
        typedef typename StringType::value_type CharType;
        return umba::filename::makeCanonicalSimpleParts( vp, StringType(1, (CharType)'.'), StringType(2, (CharType)'.'), (CharType)'/');
    }

    template<typename StringType, typename StringTypeSrc> StringType filenameToStringType(const StringTypeSrc &src) const;
    template<> std::string  filenameToStringType<std::string ,std::string >(const std::string  &src) const { return src; }
    template<> std::string  filenameToStringType<std::string ,std::wstring>(const std::wstring &src) const { return encodeFilename(src); }
    template<> std::wstring filenameToStringType<std::wstring,std::string >(const std::string  &src) const { return decodeFilename(src); }
    template<> std::wstring filenameToStringType<std::wstring,std::wstring>(const std::wstring &src) const { return src; }

    
    template<typename StringType>
    ErrorCode mapVirtualPathImpl( const StringType &vPath, StringType &realPath) const
    {
        typedef typename StringType::value_type CharType;

        std::vector< StringType > vpParts = splitVirtualPath(vPath);
        if (vpParts.empty())
        {
            return ErrorCode::invalidName; // invalidMountPoint?
        }


        std::vector<StringType>::const_iterator vpIt = vpParts.begin();

        std::wstring key = prepareMountPointKey(*vpIt++);
        std::map<std::wstring, MountPointInfo>::const_iterator mit = m_mountPoints.find(key);
        if (mit==m_mountPoints.end())
        {
            return ErrorCode::notFound;
        }

        
        const MountPointInfo &mntInfo = mit->second;
        if ((mntInfo.flags&FileTypeFlags::directory)==0)
        {
            // Mount point is a file entry
            //if (vpParts.size()>1)
            if (vpIt!=vpParts.end())
            {
                // У нас файл задан как точка монтирования, но почему-то в виртуальном пути задан дополнительный путь
                return ErrorCode::invalidMountTarget;
            }

            realPath = filenameToStringType<StringType,std::wstring>(mntInfo.target);

            return ErrorCode::ok;
        }
        else
        {
            //using namespace umba::filename;

            //std::vector<StringType>::const_iterator vpEnd = vpParts.end(); // cend чот не работает
            StringType vPathRestMerged = umba::string_plus::merge<StringType,std::vector<StringType>::const_iterator>(vpIt, vpParts.end(), (CharType)'/' /* , [](const StringType &str) { return str; } */ );
            StringType mntTargetPath   = filenameToStringType<StringType,std::wstring>(mntInfo.target);
            //realPath = makeCanonical( appendPath(mntTargetPath, vPathRestMerged) );
            realPath = this->makeNativePathCanonical(appendPath(mntTargetPath, vPathRestMerged));
            return ErrorCode::ok;
        }
    }

    std::wstring prepareVirtualizeCmp(const std::wstring &k) const
    {
        #if defined(WIN32) || defined(_WIN32)

            return umba::string_plus::toupper_copy( this->makeNativePathCanonical /* umba::filename::makeCanonical */ (k) );

        #else // Generic POSIX - Linups etc

            return k;

        #endif
    }

    std::wstring prepareVirtualizeCmp(const std::string &k) const
    {
        return prepareVirtualizeCmp(decodeFilename(k));
    }


    std::string vrpImplAppendPath(const std::wstring &mntName, const std::string &restPath) const
    {
        return umba::filename::appendPath(encodeFilename(mntName), restPath);
    }

    std::wstring vrpImplAppendPath(const std::wstring &mntName, const std::wstring &restPath) const
    {
        return umba::filename::appendPath(mntName, restPath);
    }


    template<typename StringType>
    ErrorCode virtualizeRealPathImpl( StringType realPath_, StringType &vPath) const
    {
        //using namespace umba::filename;
        typedef typename StringType::value_type CharType;

        std::wstring realPath   = this->makeNativePathCanonical /* umba::filename::makeCanonical */ (decodeFilename(realPath_));

        // StringType 
        std::wstring realPathCmp = prepareVirtualizeCmp(realPath);

        std::map<std::wstring, MountPointInfo>::const_iterator mit = m_mountPoints.begin();
        for(; mit!=m_mountPoints.end(); ++mit)
        {
            // auto 
            std::wstring cmp = prepareVirtualizeCmp(mit->second.target);
            if (!umba::string_plus::starts_with(realPathCmp, cmp ))
            {
                continue;
            }

            // Тут есть нюанс. Если у нас есть несколько mount points, некоторые из которых указывают куда-то 
            // вглубь других, то будет выбрана первая найденная, а не самая длинная версия

            realPath.erase(0, cmp.size()); // Удаляем из исходного имени кусок, соответствующий по длине таргету текущей mount point
            umba::filename::stripFirstPathSep(realPath);

            auto vpCandy = this->makePathCanonical(vrpImplAppendPath(mit->second.name, realPath)); //umba::filename::makeCanonical( vrpImplAppendPath(mit->second.name, realPath), L'/' );
            
            umba::filename::stripFirstPathSep(vpCandy);
            vPath = filenameToStringType<StringType,std::wstring>(std::wstring(1, L'/') + vpCandy);

            return ErrorCode::ok;

        } // for

        return ErrorCode::notFound;

    }


    template<typename StringType>
    StringType normalizeFilenameImpl(StringType fname) const
    {
        typedef typename StringType::value_type CharType;

        // Хвостовой слэш не должен нас путать, но только если он не единственный символ, а то скушаем корневой слэш
        if (fname.size()>1)
        {
            umba::filename::stripLastPathSep(fname);
        }

        return this->makePathCanonical(fname); // umba::filename::makeCanonicalSimple(fname, StringType(1, (CharType)'.'), StringType(2, (CharType)'.'), (CharType)'/');
    }


    //! Возвращает путь
    template<typename StringType>
    StringType getPathImpl(StringType fullName) const
    {
        fullName = normalizeFilenameImpl(fullName);
        return umba::filename::getPath(fullName);
    }

    //! Возвращает имя и расширение
    template<typename StringType>
    StringType getFileNameImpl(StringType fullName) const
    {
        fullName = normalizeFilenameImpl(fullName);
        return umba::filename::getFileName(fullName);
    }

    //! Возвращает путь и имя
    template<typename StringType>
    StringType getPathFileImpl(StringType fullName) const
    {
        fullName = normalizeFilenameImpl(fullName);
        return umba::filename::getPathFile(fullName);
    }

    //! Возвращает расширение
    template<typename StringType>
    StringType getExtImpl(StringType fullName) const
    {
        fullName = normalizeFilenameImpl(fullName);
        return umba::filename::getExt(fullName);
    }

    //! Возвращает имя файла без пути и расширения
    template<typename StringType>
    StringType getNameImpl(StringType fullName) const
    {
        fullName = normalizeFilenameImpl(fullName);
        return umba::filename::getName(fullName);
    }

    //! Конкатенация путей
    template<typename StringType>
    StringType appendPathImpl(StringType pathAppendTo, StringType appendPath) const
    {
        pathAppendTo = normalizeFilenameImpl(pathAppendTo);
        appendPath   = normalizeFilenameImpl(appendPath);
        return umba::filename::appendPath(pathAppendTo, appendPath, (typename StringType::value_type)'/');
    }

    //! Добавление расширения
    template<typename StringType>
    StringType appendExtImpl(StringType nameAppendTo, StringType appendExt) const
    {
        nameAppendTo = normalizeFilenameImpl(nameAppendTo);
        appendExt    = normalizeFilenameImpl(appendExt);
        return umba::filename::appendExt(nameAppendTo, appendExt, (typename StringType::value_type)'.');
    }

public:

    virtual bool getErrorCodeString(ErrorCode e, std::string  &errStr) const override
    {
        try
        {
            errStr = enum_serialize(e);
            return !errStr.empty();
        }
        catch(...)
        {
            return false;
        }
    }

    virtual bool getErrorCodeString(ErrorCode e, std::wstring &errStr) const override
    {
        try
        {
            std::string str;
            str = enum_serialize(e);
            errStr = decodeFilename(str); // Тут на кодировку ваще пофиг, всё равно только латиница
            return !errStr.empty();
        }
        catch(...)
        {
            return false;
        }
    }


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



    //bool        m_readOnly = false;
    virtual void setVfsGlobalReadonly(bool bReadonly) override
    {
        m_readOnly = bReadonly;
    }

    virtual bool getVfsGlobalReadonly() const override
    {
        return m_readOnly;
    }


    virtual ErrorCode clearMounts( ) override
    {
        m_mountPoints.clear();
        return ErrorCode::ok;
    }

    virtual ErrorCode removeMountPoint( const std::string  &mntPointName ) override
    {
        return removeMountPointImpl(mntPointName);
    }

    virtual ErrorCode removeMountPoint( const std::wstring &mntPointName ) override
    {
        return removeMountPointImpl(mntPointName);
    }


    virtual ErrorCode addMountPoint( const std::string  &mntPointName, const std::string  &mntPointTarget ) override
    {
        return addMountPointExImpl( mntPointName, mntPointTarget, FileTypeFlags::directory );
    }

    virtual ErrorCode addMountPoint( const std::wstring &mntPointName, const std::wstring &mntPointTarget ) override
    {
        return addMountPointExImpl( mntPointName, mntPointTarget, FileTypeFlags::directory );
    }


    // Only FileTypeFlags::directory flag allowed
    virtual ErrorCode addMountPointEx( const std::string  &mntPointName, const std::string  &mntPointTarget, FileTypeFlags flags ) override
    {
        return addMountPointExImpl( mntPointName, mntPointTarget, flags & FileTypeFlags::directory );
    }

    virtual ErrorCode addMountPointEx( const std::wstring &mntPointName, const std::wstring &mntPointTarget, FileTypeFlags flags ) override
    {
        return addMountPointExImpl( mntPointName, mntPointTarget, flags & FileTypeFlags::directory );
    }


    virtual ErrorCode mapVirtualPath( const std::string  &vPath, std::string  &realPath) const override
    {
        return mapVirtualPathImpl(vPath, realPath);
    }

    virtual ErrorCode mapVirtualPath( const std::wstring &vPath, std::wstring &realPath) const override
    {
        return mapVirtualPathImpl(vPath, realPath);
    }


    virtual ErrorCode virtualizeRealPath( const std::string  &realPath, std::string  &vPath) const override
    {
        return virtualizeRealPathImpl( realPath, vPath);
    }

    virtual ErrorCode virtualizeRealPath( const std::wstring &realPath, std::wstring &vPath) const override
    {
        return virtualizeRealPathImpl( realPath, vPath);
    }


    // Предыдущие удаляются. Это надо делать один раз при создании объекта, и при подключении/отключении съёмных носителей
    // Сами объекты IFileSystem/IVirtualFs не следят за событиями подключения/отключения съёмных носителей

    // https://learn.microsoft.com/en-us/windows/win32/devio/dbt-devicequeryremove
    // WM_DEVICECHANGE - https://learn.microsoft.com/en-us/windows/win32/devio/wm-devicechange

    // Редирект STDOUT - https://stackoverflow.com/questions/54094127/redirecting-stdout-in-win32-does-not-redirect-stdout

    virtual ErrorCode createMachineFilesystemMountPoints() override
    {
        #if defined(WIN32) || defined(_WIN32)

            clearMounts();

            // https://learn.microsoft.com/en-us/windows/win32/fileio/enumerating-volumes
            // https://learn.microsoft.com/en-us/windows/win32/api/fileapi/nf-fileapi-findfirstvolumew

            wchar_t mntPoint     [2] = L"X";
            wchar_t mntTargetPath[4] = L"X:\\";
            wchar_t drvLetter        = L'A';

            DWORD drivesMask = GetLogicalDrives();

            for(; drivesMask; drivesMask>>=1, drvLetter++)
            {
                if (drivesMask&1)
                {
                    mntPoint[0] = drvLetter;
                    mntTargetPath[0] = drvLetter;
                    addMountPointEx( std::wstring(&mntPoint[0]), std::wstring(&mntTargetPath[0]), FileTypeFlags::directory );
                }
            }

            return ErrorCode::ok;

        #else // Generic POSIX - Linups etc

            return ErrorCode::ok;

        #endif
    
    }


}; // struct VirtualFsImpl


} // namespace marty_virtual_fs


