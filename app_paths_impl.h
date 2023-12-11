/*! \file
    \brief Interface for common options for IAppPaths
*/

#pragma once

#include "app_paths_base_impl.h"
#include "app_paths_common_impl.h"

#include "umba/filesys.h"
#include "utils.h"

namespace marty_virtual_fs {


class AppPathsImpl : public AppPathsCommonImpl
                   , public AppPathsBaseImpl
{

public:

    AppPathsImpl()                                = default;
    AppPathsImpl(const AppPathsImpl &)            = default;
    AppPathsImpl(AppPathsImpl &&)                 = default;
    AppPathsImpl& operator=(const AppPathsImpl &) = default;
    AppPathsImpl& operator=(AppPathsImpl &&)      = default;


protected:

    //TODO: !!! Заменить все невалидные в пути символы на символ почеркивания
    template<typename StringType>
    StringType getSubPath(bool makeDot) const
    {
        StringType resPath;
        getAppCommonHomeSubPath(resPath);
        resPath = filterFileNameInvalidChars(resPath);
        if (makeDot && !resPath.empty())
        {
            resPath = StringType(1, (typename StringType::value_type)'.') + resPath;
        }

        StringType appName;
        getAppName(appName);
        if (appName.empty())
        {
            getAppExeName(appName);
        }
        appName = filterFileNameInvalidChars(appName);

        if (makeDot && !appName.empty())
        {
            appName = StringType(1, (typename StringType::value_type)'.') + appName;
        }

        if (!appName.empty())
        {
            if (resPath.empty())
            {
                resPath = appName;
            }
            else
            {
                resPath = umba::filename::appendPath(resPath, appName);
            }
        }

        return resPath;
    }

    template<typename StringType>
    StringType getFullPath(StringType basePath, bool makeDot) const
    {
        if (basePath.empty())
        {
            return basePath;
        }

        StringType subPath = getSubPath<StringType>(makeDot);
        if (!subPath.empty())
        {
            basePath = umba::filename::appendPath(basePath, subPath);
        }

        return basePath;
    }

    

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

    //TODO: !!! Под линупсом тут может не быть wide версий getCurrentUserHomeDirectory/getTempFolderPath
    template<typename StringType>
    bool getAppHomePathExImpl( StringType &p, bool bCreateFolder ) const
    {
        p = getFullPath<StringType>(umba::filesys::getCurrentUserHomeDirectory<StringType>(), true); // makeDot
        return bCreateFolder ? forceCreateDirectory(p) : true;
    }

    template<typename StringType>
    bool getAppTempPathExImpl( StringType &p, bool bCreateFolder ) const
    {
        p = getFullPath<StringType>(umba::filesys::getTempFolderPath<StringType>(), false); // no dot prefix - temp folders are not hidden
        return bCreateFolder ? forceCreateDirectory(p) : true;
    }


public:

    // virtual bool getAppExeName(std::string  &p) const = 0;
    // virtual bool getAppExeName(std::wstring &p) const = 0;

    void getAppName(std::string &a) const override
    {
        AppPathsCommonImpl::getAppName(a);

        if (a.empty())
        {
            getAppExeName(a);
        }
    }

    void getAppName(std::wstring &a) const override
    {
        AppPathsCommonImpl::getAppName(a);

        if (a.empty())
        {
            getAppExeName(a);
        }
    }


    virtual bool getAppHomePathEx(std::string  &p, bool bCreateFolder) const override
    {
        return getAppHomePathExImpl(p, bCreateFolder);
    }

    virtual bool getAppHomePathEx(std::wstring &p, bool bCreateFolder) const override
    {
        return getAppHomePathExImpl(p, bCreateFolder);
    }



    virtual bool getAppHomePath(std::string  &p) const override
    {
        return getAppHomePathExImpl(p, false);
    }

    virtual bool getAppHomePath(std::wstring &p) const override
    {
        return getAppHomePathExImpl(p, false);
    }


    virtual bool getAppTempPathEx(std::string  &p, bool bCreateFolder) const override
    {
        return getAppTempPathExImpl(p, bCreateFolder);
    }

    virtual bool getAppTempPathEx(std::wstring &p, bool bCreateFolder) const override
    {
        return getAppTempPathExImpl(p, bCreateFolder);
    }


    virtual bool getAppTempPath(std::string  &p) const override
    {
        return getAppTempPathExImpl(p, false);
    }

    virtual bool getAppTempPath(std::wstring &p) const override
    {
        return getAppTempPathExImpl(p, false);
    }


}; // struct AppPathsBaseImpl


} // namespace marty_virtual_fs


