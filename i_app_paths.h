/*! \file
    \brief Interface for obtaining some basic app paths
*/

#pragma once


#include <string>

//
#include "vfs_types.h"

//
#include "warnings_disable.h"



namespace marty_virtual_fs {


struct IAppPaths
{
    virtual ~IAppPaths() {}


    // Path, name, and extention
    virtual bool getAppExeFullPathName(std::string  &p) const = 0;
    virtual bool getAppExeFullPathName(std::wstring &p) const = 0;

    // Only name without path and extention
    virtual bool getAppExeName(std::string  &p) const = 0;
    virtual bool getAppExeName(std::wstring &p) const = 0;

    virtual bool getAppExePath(std::string  &p) const = 0;
    virtual bool getAppExePath(std::wstring &p) const = 0;

    virtual bool getAppBinRootPath(std::string  &p) const = 0;
    virtual bool getAppBinRootPath(std::wstring &p) const = 0;

    virtual bool getAppRootPath(std::string  &p) const = 0;
    virtual bool getAppRootPath(std::wstring &p) const = 0;

    virtual bool getAppHomePathEx(std::string  &p, bool bCreateFolder) const = 0;
    virtual bool getAppHomePathEx(std::wstring &p, bool bCreateFolder) const = 0;

    virtual bool getAppHomePath(std::string  &p) const = 0;
    virtual bool getAppHomePath(std::wstring &p) const = 0;

    virtual bool getAppTempPathEx(std::string  &p, bool bCreateFolder) const = 0;
    virtual bool getAppTempPathEx(std::wstring &p, bool bCreateFolder) const = 0;

    virtual bool getAppTempPath(std::string  &p) const = 0;
    virtual bool getAppTempPath(std::wstring &p) const = 0;


}; // struct IAppPaths


} // namespace marty_virtual_fs


#include "warnings_restore.h"

