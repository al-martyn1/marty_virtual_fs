/*! \file
    \brief IAppPaths base implementation
*/

#pragma once

#include "umba/filename.h"
#include "umba/string_plus.h"

// 
#include "filename_encoder_impl.h"
#include "i_app_paths.h"


namespace marty_virtual_fs {


class AppPathsBaseImpl : public         IAppPaths
                       , public virtual FilenameEncoderImpl // нужно для перекодировки имен файлов
{

protected:

public:

    AppPathsBaseImpl()                           = default;
    AppPathsBaseImpl(const AppPathsBaseImpl &) = default;
    AppPathsBaseImpl(AppPathsBaseImpl &&)      = default;
    AppPathsBaseImpl& operator=(const AppPathsBaseImpl &) = default;
    AppPathsBaseImpl& operator=(AppPathsBaseImpl &&)      = default;


    // virtual std::string  encodeFilename( const std::wstring &str ) const override
    // virtual std::wstring decodeFilename( const std::string  &str ) const override

protected:


#if defined(WIN32) || defined(_WIN32)

    std::wstring getExeName() const
    {
        wchar_t buf[4096];
        const std::size_t bufSize = sizeof(buf)/sizeof(buf[0]);

        DWORD res = GetModuleFileNameW( 0, buf, bufSize );
        if (res==bufSize)
            buf[bufSize-1] = 0;
    
        return std::wstring(buf);
    }

#else // Generic POSIX - Linups etc

    std::string getExeName() const
    {
        // #error "Not implemented"
        #pragma message("marty_virtual_fs::AppPathsBaseImpl::getExeName(): Not implemented")
    }

#endif


public:

#if defined(WIN32) || defined(_WIN32)

    // Path, name, and extention
    virtual bool getAppExeFullPathName(std::string  &p) const override
    {
        p = encodeFilename(getExeName());
        return true;
    }

    virtual bool getAppExeFullPathName(std::wstring &p) const override
    {
        p = getExeName();
        return true;
    }

#else // Generic POSIX - Linups etc

    // Path, name, and extention
    virtual bool getAppExeFullPathName(std::string  &p) const override
    {
        p = getExeName();
        return true;
    }

    virtual bool getAppExeFullPathName(std::wstring &p) const override
    {
        p = decodeFilename(getExeName());
        return true;
    }

#endif


private:

    template<typename StringType> StringType getAppExeNameImpl() const
    {
        StringType exeName;
        getAppExeFullPathName(exeName);
        return umba::filename::getName(exeName);
    }

    template<typename StringType> StringType getAppExePathImpl() const
    {
        StringType exeName;
        getAppExeFullPathName(exeName);
        return umba::filename::getPath(exeName);
    }

    template<typename StringType> std::vector<StringType> makeNames(const std::vector<std::string> &names) const
    {
        std::vector<StringType> res; res.reserve(names.size());
        for(const auto &n : names)
        {
            res.emplace_back(umba::string_plus::make_string<StringType>(n));
        }
    
        return res;
    }

    template<typename StringType> std::vector<StringType> makeBinNames() const
    {
        return makeNames<StringType>(std::vector<std::string>{ "bin" });
    }

    template<typename StringType> const std::vector<StringType>& getBinNames() const
    {
        static std::vector<StringType> names = makeBinNames<StringType>();
        return names;
    }

    template<typename StringType> std::vector<StringType> makeReleaseDebugNames() const
    {
        return makeNames<StringType>(std::vector<std::string>{ "Debug", "Release", "MinSizeRel", "RelWithDebInfo", "RelWithDebugInfo", "Unicode Debug", "Unicode Release" });
    }

    template<typename StringType> const std::vector<StringType>& getReleaseDebugNames() const
    {
        static std::vector<StringType> names = makeReleaseDebugNames<StringType>();
        return names;
    }

    template<typename StringType> std::vector<StringType> makeOutNames() const
    {
        return makeNames<StringType>(std::vector<std::string>{ ".out", "out" });
    }

    template<typename StringType> const std::vector<StringType>& getOutNames() const
    {
        static std::vector<StringType> names = makeOutNames<StringType>();
        return names;
    }

    template<typename StringType> StringType nativePathPartPrepareForCompare(const StringType &s) const
    {
        // Или похеру, везде будем в этом месте сравнивать без учета регистра?
        #if 0
            #if defined(WIN32) || defined(_WIN32)
            #else
            #endif
        #endif

        return umba::string_plus::toupper_copy(s);
    }

    template<typename StringType> bool isNativePathPartOneOf(StringType s, const std::vector<StringType> &names) const
    {
        s = nativePathPartPrepareForCompare(s);

        for(const auto &n : names)
        {
            if (s==nativePathPartPrepareForCompare(n))
            {
                return true;
            }
        }

        return false;
    }

    template<typename StringType>
    StringType getAppBinRootPathImpl() const
    {
        StringType exePathOrg = getAppExePathImpl<StringType>();
        auto pathSep       = (typename StringType::value_type)'/';
        StringType exePath = umba::filename::makeCanonical(exePathOrg, pathSep);
        auto npfi          = umba::filename::stripNativePrefixes(exePath, pathSep);

        //bool lastPathSep  = 
        umba::filename::stripLastPathSep(exePath);
        bool firstPathSep = umba::filename::stripFirstPathSep(exePath);

        std::vector< StringType > parts = umba::filename::makeCanonicalSimpleParts( exePath
                                                                                  , umba::filename::getNativeCurrentDirAlias<StringType>()
                                                                                  , umba::filename::getNativeParentDirAlias<StringType>()
                                                                                  , pathSep
                                                                                  );

        if (parts.empty())
        {
            return exePathOrg;
        }

        std::size_t removeNumParts = 0;

        if (isNativePathPartOneOf(parts.back(), getBinNames<StringType>()))
        {
            removeNumParts = 1;
        }
        else if (isNativePathPartOneOf(parts.back(), getReleaseDebugNames<StringType>()))
        {
            // .out/msvc2019/x64/Debug - 4 max

            std::size_t idx  = parts.size()-1;
            std::size_t maxN = parts.size();
            if (maxN>4)
            {
                maxN = 4;
            }

            auto outNames = getOutNames<StringType>();
            for(std::size_t i=0u; i!=maxN; ++i, --idx)
            {
                if (isNativePathPartOneOf(parts[idx], outNames))
                {
                    removeNumParts = i+1;
                    break;
                }
            }

            if (removeNumParts==0)
            {
                removeNumParts = 1;
            }
            
        }
        else
        {
            // not in BIN and not in OUT under Debug/Release or not in Debug/Release
        }

        if (removeNumParts)
        {
            --removeNumParts;
        }

        parts.erase( parts.end()-(std::ptrdiff_t)removeNumParts, parts.end() );

        exePath = umba::string_plus::merge(parts,pathSep);

        if (firstPathSep)
        {
            exePath = StringType(1, pathSep) + exePath;
        }

        exePath = addNativePrefixes(exePath, npfi, pathSep);

        return umba::filename::makeCanonical( exePath );
    }

    template<typename StringType>
    StringType getAppRootPathImpl() const
    {
        StringType exePathOrg = getAppExePathImpl<StringType>();
        auto pathSep       = (typename StringType::value_type)'/';
        StringType exePath = umba::filename::makeCanonical(exePathOrg, pathSep);
        auto npfi          = umba::filename::stripNativePrefixes(exePath, pathSep);

        //bool lastPathSep  = 
        umba::filename::stripLastPathSep(exePath);
        bool firstPathSep = umba::filename::stripFirstPathSep(exePath);

        std::vector< StringType > parts = umba::filename::makeCanonicalSimpleParts( exePath
                                                                                  , umba::filename::getNativeCurrentDirAlias<StringType>()
                                                                                  , umba::filename::getNativeParentDirAlias<StringType>()
                                                                                  , pathSep
                                                                                  );

        if (parts.empty())
        {
            return exePathOrg;
        }

        std::size_t removeNumParts = 0;

        if (isNativePathPartOneOf(parts.back(), getBinNames<StringType>()))
        {
            removeNumParts = 1;
        }
        else if (isNativePathPartOneOf(parts.back(), getReleaseDebugNames<StringType>()))
        {
            // .out/msvc2019/x64/Debug - 4 max

            std::size_t idx  = parts.size()-1;
            std::size_t maxN = parts.size();
            if (maxN>4)
            {
                maxN = 4;
            }

            auto outNames = getOutNames<StringType>();
            for(std::size_t i=0u; i!=maxN; ++i, --idx)
            {
                if (isNativePathPartOneOf(parts[idx], outNames))
                {
                    removeNumParts = i+1;
                    break;
                }
            }

            if (removeNumParts==0)
            {
                removeNumParts = 1;
            }
            
        }
        else
        {
            // not in BIN and not in OUT under Debug/Release or not in Debug/Release
        }

        parts.erase( parts.end()-(std::ptrdiff_t)removeNumParts, parts.end() );

        exePath = umba::string_plus::merge(parts,pathSep);

        if (firstPathSep)
        {
            exePath = StringType(1, pathSep) + exePath;
        }

        exePath = addNativePrefixes(exePath, npfi, pathSep);

        //return umba::filename::makeCanonicalSimpleParts(exePath);
        return umba::filename::makeCanonical( exePath );

    }




// return umba::string_plus::make_string<StringType>("..");

public:

    // Only name without path and extention
    virtual bool getAppExeName(std::string  &p) const override
    {
        p = getAppExeNameImpl<std::string >();
        return true;
    }

    virtual bool getAppExeName(std::wstring &p) const override
    {
        p = getAppExeNameImpl<std::wstring>();
        return true;
    }


    virtual bool getAppExePath(std::string  &p) const override
    {
        p = getAppExePathImpl<std::string >();
        return true;
    }

    virtual bool getAppExePath(std::wstring &p) const override
    {
        p = getAppExePathImpl<std::wstring>();
        return true;
    }


    virtual bool getAppBinRootPath(std::string  &p) const override
    {
        p = getAppBinRootPathImpl<std::string >();
        return true;
    }

    virtual bool getAppBinRootPath(std::wstring &p) const override
    {
        p = getAppBinRootPathImpl<std::wstring>();
        return true;
    }


    virtual bool getAppRootPath(std::string  &p) const override
    {
        p = getAppRootPathImpl<std::string >();
        return true;
    }

    virtual bool getAppRootPath(std::wstring &p) const override
    {
        p = getAppRootPathImpl<std::wstring>();
        return true;
    }


}; // struct AppPathsBaseImpl


} // namespace marty_virtual_fs


