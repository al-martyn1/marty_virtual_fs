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


struct IAppPathsCommon
{

    virtual ~IAppPathsCommon() {}

    // Глобально на всё приложение для всех экземпляров приложения
    // Установка "группового" подпути в home каталоге для нескольких приложений одной группы
    // По умолчанию не задано и никакой группировки нет
    void setAppCommonHomeSubPath(const std::string  &p) = 0;
    void setAppCommonHomeSubPath(const std::wstring &p) = 0;

    void getAppCommonHomeSubPath(std::string  &p) const = 0;
    void getAppCommonHomeSubPath(std::wstring &p) const = 0;

    // Глобально на всё приложение для всех экземпляров приложения
    // Используется для получения полного имени каталога приложения в home каталоге пользователя
    // По умолчанию используется имя исполняемого файла
    void setAppName(const std::string  &p) = 0;
    void setAppName(const std::wstring &p) = 0;

    void getAppName(std::string  &p) const = 0;
    void getAppName(std::wstring &p) const = 0;


}; // struct IAppPathsCommon


} // namespace marty_virtual_fs


#include "warnings_restore.h"

