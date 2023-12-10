/*! \file
    \brief Interface for common options for IAppPaths
*/

#pragma once


#include <string>

//
#include "vfs_types.h"

//
#include "warnings_disable.h"



namespace marty_virtual_fs {


struct IAppPathsCommon
{

    virtual ~IAppPathsCommon() {}

    // // Необходимо сделать под линупсами - или, может, будет найден другой способ получения имени EXE файла
    // void setArgv0(const std::string  &p) = 0;
    // void setArgv0(const std::wstring &p) = 0;


    // Глобально на всё приложение для всех экземпляров приложения
    // Установка "группового" подпути в home каталоге для нескольких приложений одной группы
    // По умолчанию не задано и никакой группировки нет
    virtual void setAppCommonHomeSubPath(const std::string  &p) = 0;
    virtual void setAppCommonHomeSubPath(const std::wstring &p) = 0;

    virtual void getAppCommonHomeSubPath(std::string  &p) const = 0;
    virtual void getAppCommonHomeSubPath(std::wstring &p) const = 0;

    // Глобально на всё приложение для всех экземпляров приложения
    // Используется для получения полного имени каталога приложения в home каталоге пользователя
    // По умолчанию используется имя исполняемого файла
    virtual void setAppName(const std::string  &a) = 0;
    virtual void setAppName(const std::wstring &a) = 0;

    virtual void getAppName(std::string  &a) const = 0;
    virtual void getAppName(std::wstring &a) const = 0;


}; // struct IAppPathsCommon


} // namespace marty_virtual_fs


#include "warnings_restore.h"

