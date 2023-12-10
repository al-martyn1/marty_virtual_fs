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


namespace marty_virtual_fs {


class VirtualFsImpl : public         IVirtualFs
                    , public virtual FilenameEncoderImpl // нужно для перекодировки имен файлов
                    //, public virtual FileDataEncoderImpl // нужно для перекодировки текста файлов
{

protected:

public:

    VirtualFsImpl()                                 = default;
    VirtualFsImpl(const VirtualFsImpl &)            = default;
    VirtualFsImpl(VirtualFsImpl &&)                 = default;
    VirtualFsImpl& operator=(const VirtualFsImpl &) = default;
    VirtualFsImpl& operator=(VirtualFsImpl &&)      = default;


    // virtual std::string  encodeFilename( const std::wstring &str ) const override
    // virtual std::wstring decodeFilename( const std::string  &str ) const override

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
            using namespace umba::filename;

            //std::vector<StringType>::const_iterator vpEnd = vpParts.end(); // cend чот не работает
            StringType vPathRestMerged = umba::string_plus::merge<StringType,std::vector<StringType>::const_iterator>(vpIt, vpParts.end(), (CharType)'/' /* , [](const StringType &str) { return str; } */ );
            StringType mntTargetPath   = filenameToStringType<StringType,std::wstring>(mntInfo.target);
            realPath = makeCanonical( appendPath(mntTargetPath, vPathRestMerged) );
            return ErrorCode::ok;
        }
    }

    std::wstring prepareVirtualizeCmp(const std::wstring &k) const
    {
        #if defined(WIN32) || defined(_WIN32)

            return umba::string_plus::toupper_copy( umba::filename::makeCanonical(k) );

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
        using namespace umba::filename;
        typedef typename StringType::value_type CharType;

        std::wstring realPath   = makeCanonical(decodeFilename(realPath_));

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
            stripFirstPathSep(realPath);

            auto vpCandy = makeCanonical( vrpImplAppendPath(mit->second.name, realPath), L'/' );
            
            stripFirstPathSep(vpCandy);
            vPath = filenameToStringType<StringType,std::wstring>(std::wstring(1, L'/') + vpCandy);

            return ErrorCode::ok;

        } // for

        return ErrorCode::notFound;

    }

        // MountPointInfo mntInfo;
        // mntInfo.name   = mntPointName;
        // mntInfo.target = prepareMountTarget(mntPointTarget);
        // mntInfo.flags  = flags;


public:

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


