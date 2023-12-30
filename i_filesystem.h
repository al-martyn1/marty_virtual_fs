/*! \file
    \brief Interface for filesystem like access to something
*/

#pragma once


#include <string>
#include <vector>

//
#include "vfs_types.h"

//
#include "warnings_disable.h"



namespace marty_virtual_fs {


// Тут у нас нет понятия текущий каталог, путь начинается с корня - '/', если корень отсутствует явно, то считается, что все равно путь начинается с корня
struct IFileSystem
{
    virtual ~IFileSystem() {}

    virtual std::uint8_t* swapByteOrder(std::uint8_t *pData, std::size_t dataSize) const = 0;

    virtual Endianness    getHostEndianness() const = 0;
    virtual std::uint8_t* convertEndiannessToHost  (std::uint8_t *pData, std::size_t dataSize, Endianness srcEndianness) const = 0;
    virtual std::uint8_t* convertEndiannessFromHost(std::uint8_t *pData, std::size_t dataSize, Endianness dstEndianness) const = 0;

    virtual std::string  encodeText( const std::wstring &str ) const = 0;
    virtual std::string  encodeText( const std::string  &str ) const = 0;
    virtual std::wstring decodeText( const std::wstring &str ) const = 0;
    virtual std::wstring decodeText( const std::string  &str ) const = 0;

    virtual std::string  encodeFilename( const std::wstring &str ) const = 0;
    virtual std::string  encodeFilename( const std::string  &str ) const = 0;
    virtual std::wstring decodeFilename( const std::wstring &str ) const = 0;
    virtual std::wstring decodeFilename( const std::string  &str ) const = 0;


    // Нормализует виртуальное имя файла, нормализует разделители пути, и схлопывает спец пути типа "."/"..", 
    // чтобы мамкины "хакеры" из скрипта не могли вылезти за пределы песочницы
    // Выставляем наружу, чтобы в скрипте можно было проверить, как выглядит путь, который будет реально использоваться

    virtual std::string  normalizeFilename(const std::string  &fname) const = 0; // static
    virtual std::wstring normalizeFilename(const std::wstring &fname) const = 0; // static

    // Возвращает текстовую строку, соответствующую коду ошибки
    virtual bool getErrorCodeString(ErrorCode e, std::string  &errStr) const = 0; // static
    virtual bool getErrorCodeString(ErrorCode e, std::wstring &errStr) const = 0; // static


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


    // Конвертация в/из нативных путей. Если путь вне вирт системы - ErrorCode::notFound. 
    // Если текущая ф. система не поддерживает конвертацию в нативное имя - ErrorCode::notSupported
    virtual ErrorCode fromNativePathName(const std::string  &nativeName, std::string  &vfsName) const = 0;
    virtual ErrorCode fromNativePathName(const std::wstring &nativeName, std::wstring &vfsName) const = 0;

    virtual ErrorCode toNativePathName(const std::string  &vfsName, std::string  &nativeName) const = 0;
    virtual ErrorCode toNativePathName(const std::wstring &vfsName, std::wstring &nativeName) const = 0;

    virtual ErrorCode mapVirtualPath( const std::string  &vPath, std::string  &realPath) const = 0;
    virtual ErrorCode mapVirtualPath( const std::wstring &vPath, std::wstring &realPath) const = 0;

    virtual ErrorCode virtualizeRealPath( const std::string  &realPath, std::string  &vPath) const = 0;
    virtual ErrorCode virtualizeRealPath( const std::wstring &realPath, std::wstring &vPath) const = 0;

    virtual ErrorCode createDirectory(const std::string  &dirPath, bool bForce ) const = 0;
    virtual ErrorCode createDirectory(const std::wstring &dirPath, bool bForce ) const = 0;


    // Нерекурсивный обзор содержимого каталога
    virtual ErrorCode enumerateDirectory(const std::string  &dirPath, std::vector<DirectoryEntryInfoA> &entries) const = 0;
    virtual ErrorCode enumerateDirectory(const std::wstring &dirPath, std::vector<DirectoryEntryInfoW> &entries ) const = 0;

    virtual std::vector<DirectoryEntryInfoA> enumerateDirectory(const std::string  &dirPath, ErrorCode *pErr = 0) const = 0;
    virtual std::vector<DirectoryEntryInfoW> enumerateDirectory(const std::wstring &dirPath, ErrorCode *pErr = 0) const = 0;


    // Возвращает 0, если совпадения не найдено, >0 - индекс маски, по которой найдено совпадение, <0 - индекс маски, на которой произошла какая-то ошибка (например, корявый regex)
    virtual bool testMaskMatch(const DirectoryEntryInfoA &entry, const FileMaskInfoA &mask) const = 0;
    virtual bool testMaskMatch(const DirectoryEntryInfoW &entry, const FileMaskInfoW &mask) const = 0;

    // Нерекурсивный обзор содержимого каталога, расширенная версия
    virtual ErrorCode enumerateDirectoryEx(const std::string  &dirPath, EnumerateFlags enumerateFlags, SortFlags sortFlags, const std::vector<FileMaskInfoA> &masks, std::vector<DirectoryEntryInfoA> &entries) const = 0;
    virtual ErrorCode enumerateDirectoryEx(const std::wstring &dirPath, EnumerateFlags enumerateFlags, SortFlags sortFlags, const std::vector<FileMaskInfoW> &masks, std::vector<DirectoryEntryInfoW> &entries) const = 0;

    virtual std::vector<DirectoryEntryInfoA> enumerateDirectoryEx(const std::string  &dirPath, EnumerateFlags enumerateFlags, SortFlags sortFlags, const std::vector<FileMaskInfoA> &masks, ErrorCode *pErr = 0) const = 0;
    virtual std::vector<DirectoryEntryInfoW> enumerateDirectoryEx(const std::wstring &dirPath, EnumerateFlags enumerateFlags, SortFlags sortFlags, const std::vector<FileMaskInfoW> &masks, ErrorCode *pErr = 0) const = 0;


    // std::string formatFiletime<std::string>( filetime_t t, const std::string &fmt )
    // Описание форматной строки тут - https://man7.org/linux/man-pages/man3/strftime.3.html
    virtual std::string  formatFiletime(FileTime ft, const std::string  &fmt) const = 0;
    virtual std::wstring formatFiletime(FileTime ft, const std::wstring &fmt) const = 0;


    virtual std::uint32_t getFileSizeLo(FileSize sz) const = 0;
    virtual std::uint32_t getFileSizeHi(FileSize sz) const = 0;


    virtual int compareFilenames(const std::string  &n1, const std::string  &n2, SortFlags sortFlags) const = 0;
    virtual int compareFilenames(const std::wstring &n1, const std::wstring &n2, SortFlags sortFlags) const = 0;

    virtual int compareDirectoryEntries(const DirectoryEntryInfoA &e1, const DirectoryEntryInfoA &e2, SortFlags sortFlags) const = 0;
    virtual int compareDirectoryEntries(const DirectoryEntryInfoW &e1, const DirectoryEntryInfoW &e2, SortFlags sortFlags) const = 0;


    virtual bool isFileExistAndReadable(const std::string  &fName) const = 0;
    virtual bool isFileExistAndReadable(const std::wstring &fName) const = 0;

    virtual bool isDirectory(const std::string  &dName) const = 0;
    virtual bool isDirectory(const std::wstring &dName) const = 0;


    virtual std::string  makePathCanonical(const std::string &p) const = 0;
    virtual std::wstring makePathCanonical(const std::wstring &p) const = 0;

    virtual std::string  makeNativePathCanonical(const std::string &p) const = 0;
    virtual std::wstring makeNativePathCanonical(const std::wstring &p) const = 0;


    // Тут автоматически работают перекодировки текста
    virtual ErrorCode readTextFile(const std::string  &fName, std::string  &fText) const = 0;
    virtual ErrorCode readTextFile(const std::string  &fName, std::wstring &fText) const = 0;
    virtual ErrorCode readTextFile(const std::wstring &fName, std::string  &fText) const = 0;
    virtual ErrorCode readTextFile(const std::wstring &fName, std::wstring &fText) const = 0;

    // Reading binary files
    virtual ErrorCode readDataFile(const std::string  &fName, std::vector<std::uint8_t> &fData) const = 0;
    virtual ErrorCode readDataFile(const std::wstring &fName, std::vector<std::uint8_t> &fData) const = 0;


    virtual ErrorCode writeTextFile(const std::string  &fName, const std::string  &fText, WriteFileFlags writeFlags) const = 0;
    virtual ErrorCode writeTextFile(const std::string  &fName, const std::wstring &fText, WriteFileFlags writeFlags) const = 0;
    virtual ErrorCode writeTextFile(const std::wstring &fName, const std::string  &fText, WriteFileFlags writeFlags) const = 0;
    virtual ErrorCode writeTextFile(const std::wstring &fName, const std::wstring &fText, WriteFileFlags writeFlags) const = 0;

    virtual ErrorCode writeDataFile(const std::string  &fName, const std::vector<std::uint8_t> &fData, WriteFileFlags writeFlags) const = 0;
    virtual ErrorCode writeDataFile(const std::wstring &fName, const std::vector<std::uint8_t> &fData, WriteFileFlags writeFlags) const = 0;


}; // struct IFileSystem


} // namespace marty_virtual_fs


#include "warnings_restore.h"

