/*! \file
    \brief IAppPathsCommon implementation
*/

#pragma once

#include "filename_encoder_impl.h"
#include "i_app_paths_common.h"


namespace marty_virtual_fs {


class AppPathsCommonImpl : public         IAppPathsCommon
                         , public virtual FilenameEncoderImpl // нужно для перекодировки имен файлов
{

protected:

    // Не будем возится с защитой для многопоточки - это всё настраивается один раз при запуске приложения/создании объекта, и больше не трогается
    static inline std::wstring    m_homeSubPath;
    static inline std::wstring    m_appName    ;


public:

    AppPathsCommonImpl()                           = default;
    AppPathsCommonImpl(const AppPathsCommonImpl &) = default;
    AppPathsCommonImpl(AppPathsCommonImpl &&)      = default;
    AppPathsCommonImpl& operator=(const AppPathsCommonImpl &) = default;
    AppPathsCommonImpl& operator=(AppPathsCommonImpl &&)      = default;


    // virtual std::string  encodeFilename( const std::wstring &str ) const override
    // virtual std::wstring decodeFilename( const std::string  &str ) const override


    // Глобально на всё приложение для всех экземпляров приложения
    // Установка "группового" подпути в home каталоге для нескольких приложений одной группы
    // По умолчанию не задано и никакой группировки нет

#if defined(WIN32) || defined(_WIN32)

    void setAppCommonHomeSubPath(const std::string  &p) override
    {
        m_homeSubPath = decodeFilename(p);
    }

    void setAppCommonHomeSubPath(const std::wstring &p) override
    {
        m_homeSubPath = p;
    }

    void getAppCommonHomeSubPath(std::string &p) const override
    {
        p = encodeFilename(m_homeSubPath);
    }

    void getAppCommonHomeSubPath(std::wstring &p) const override
    {
        p = m_homeSubPath;
    }

    // Глобально на всё приложение для всех экземпляров приложения
    // Используется для получения полного имени каталога приложения в home каталоге пользователя
    // По умолчанию используется имя исполняемого файла
    void setAppName(const std::string  &a) override
    {
        m_appName = decodeFilename(a);
    }

    void setAppName(const std::wstring &a) override
    {
        m_appName = a;
    }

    void getAppName(std::string &a) const override
    {
        a = encodeFilename(m_appName);
    }

    void getAppName(std::wstring &a) const override
    {
        a = m_appName;
    }

#else // Generic POSIX - Linups etc

    // В линупсе в ходу UTF-8 мультибайт кодровка, поэтому там надо наоборот wide версии конвертить в string
    #error "Not implemented"

#endif

}; // struct AppPathsCommonImpl


} // namespace marty_virtual_fs



