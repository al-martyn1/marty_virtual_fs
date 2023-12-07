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


    ErrorCode addMountPoint( const std::string  &mntPointName, const std::string  &mntPointTarget ) const = 0;
    ErrorCode addMountPoint( const std::wstring &mntPointName, const std::wstring &mntPointTarget ) const = 0;

    // Only FileTypeFlags::directory flag allowed
    ErrorCode addMountPointEx( const std::string  &mntPointName, const std::string  &mntPointTarget, FileTypeFlags flags ) const = 0;
    ErrorCode addMountPointEx( const std::wstring &mntPointName, const std::wstring &mntPointTarget, FileTypeFlags flags ) const = 0;

    // Предыдущие удаляются. Это надо делать один раз при создании объекта, и при подключении/отключении съёмных носителей
    // Сами объекты IFileSystem/IVirtualFs не следят за событиями подключения/отключения съёмных носителей
    ErrorCode createMachineFilesystemMountPoints() const = 0;


}; // struct IVirtualFs


} // namespace marty_virtual_fs



#include "warnings_restore.h"