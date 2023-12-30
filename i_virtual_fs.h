/*! \file
    \brief File system interface
*/

#pragma once


#include <exception>
#include <stdexcept>
#include <cstddef>
#include <cstring>
#include <memory>
#include <string>
#include <vector>
#include <utility>

//
#include "vfs_types.h"

//
#include "warnings_disable.h"



namespace marty_virtual_fs {

//! Интерфейс для настройки IFileSystem
/*! IFileSystem предназначен доступа к файлам, а IVirtualFs - для его настройки
    IFileSystem предоставляет доступ к файловой системе через единственный корень.
    Если у нас производится доступ к реальной файловой системе:
      1) дело происходит под Windows - добавляются "mount points", соответствующие дискам Windows
      2) дело происходит под *nix - добавляются "mount points", соответствующие каталогам в "/"
    Чтение/запись файлов в корне - недоступна. Или таки нет?
    А если у нас архив, и там кроме папок, есть в корне и файлы? Их надо читать же?
    Окей, тогда "mount points" могут быть ссылками как на папки, так и на файлы

 */
struct IVirtualFs
{
    virtual ~IVirtualFs() {}

    // Возвращает текстовую строку, соответствующую коду ошибки
    virtual bool getErrorCodeString(ErrorCode e, std::string  &errStr) const = 0; // static
    virtual bool getErrorCodeString(ErrorCode e, std::wstring &errStr) const = 0; // static

    virtual std::string  normalizeFilename(const std::string  &fname) const = 0; // static
    virtual std::wstring normalizeFilename(const std::wstring &fname) const = 0; // static

    virtual void setVfsGlobalReadonly(bool bReadonly) = 0;
    virtual bool getVfsGlobalReadonly() const = 0;

    // "Статические" методы для извлечения частей пути
    // Возвращаемые значения (в основном это касается путей) будут "нормализованы"

    //! Возвращает путь
    virtual std::string  getPath(const std::string  &fullName) const = 0; // static
    virtual std::wstring getPath(const std::wstring &fullName) const = 0; // static

    //! Возвращает имя и расширение
    virtual std::string  getFileName(const std::string  &fullName) const = 0; // static
    virtual std::wstring getFileName(const std::wstring &fullName) const = 0; // static

    //! Возвращает путь и имя
    virtual std::string  getPathFile(const std::string  &fullName) const = 0; // static
    virtual std::wstring getPathFile(const std::wstring &fullName) const = 0; // static

    //! Возвращает расширение
    virtual std::string  getExt(const std::string  &fullName) const = 0; // static
    virtual std::wstring getExt(const std::wstring &fullName) const = 0; // static

    //! Возвращает имя файла без пути и расширения
    virtual std::string  getName(const std::string  &fullName) const = 0; // static
    virtual std::wstring getName(const std::wstring &fullName) const = 0; // static

    //! Конкатенация путей
    virtual std::string  appendPath(const std::string  &pathAppendTo, const std::string  &appendPath) const = 0; // static
    virtual std::wstring appendPath(const std::wstring &pathAppendTo, const std::wstring &appendPath) const = 0; // static

    //! Добавление расширения
    virtual std::string  appendExt(const std::string  &nameAppendTo, const std::string  &appendExt) const = 0; // static
    virtual std::wstring appendExt(const std::wstring &nameAppendTo, const std::wstring &appendExt) const = 0; // static


    virtual ErrorCode clearMounts( ) = 0;

    virtual ErrorCode removeMountPoint( const std::string  &mntPointName ) = 0;
    virtual ErrorCode removeMountPoint( const std::wstring &mntPointName ) = 0;

    virtual ErrorCode addMountPoint( const std::string  &mntPointName, const std::string  &mntPointTarget ) = 0;
    virtual ErrorCode addMountPoint( const std::wstring &mntPointName, const std::wstring &mntPointTarget ) = 0;

    // Only FileTypeFlags::directory flag allowed
    virtual ErrorCode addMountPointEx( const std::string  &mntPointName, const std::string  &mntPointTarget, FileTypeFlags flags ) = 0;
    virtual ErrorCode addMountPointEx( const std::wstring &mntPointName, const std::wstring &mntPointTarget, FileTypeFlags flags ) = 0;

    virtual ErrorCode mapVirtualPath( const std::string  &vPath, std::string  &realPath) const = 0;
    virtual ErrorCode mapVirtualPath( const std::wstring &vPath, std::wstring &realPath) const = 0;

    virtual ErrorCode virtualizeRealPath( const std::string  &realPath, std::string  &vPath) const = 0;
    virtual ErrorCode virtualizeRealPath( const std::wstring &realPath, std::wstring &vPath) const = 0;

    virtual std::string  makePathCanonical(const std::string &p) const = 0;
    virtual std::wstring makePathCanonical(const std::wstring &p) const = 0;

    virtual std::string  makeNativePathCanonical(const std::string &p) const = 0;
    virtual std::wstring makeNativePathCanonical(const std::wstring &p) const = 0;

    // Предыдущие удаляются. Это надо делать один раз при создании объекта, и при подключении/отключении съёмных носителей
    // Сами объекты IFileSystem/IVirtualFs не следят за событиями подключения/отключения съёмных носителей
    virtual ErrorCode createMachineFilesystemMountPoints() = 0;


}; // struct IVirtualFs


} // namespace marty_virtual_fs



#include "warnings_restore.h"