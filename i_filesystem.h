/*! \file
    \brief Interface for obtaining some basic app paths
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

    std::string  normalizeFilename(const std::string  &fname) const = 0;
    std::wstring normalizeFilename(const std::wstring &fname) const = 0;

    bool nativePathNameToVirtual(const std::string  &nativeName, std::string  &vfsName) const = 0;
    bool nativePathNameToVirtual(const std::wstring &nativeName, std::wstring &vfsName) const = 0;

    // Нерекурсивный обзор содержимого каталога
    ErrorCode enumerateDirectory(const std::string  &dirPath, std::vector<DirectoryEntryInfoA> &entries) const = 0;
    ErrorCode enumerateDirectory(const std::wstring &dirPath, std::vector<DirectoryEntryInfoW> &entries ) const = 0;

    std::vector<DirectoryEntryInfoA> enumerateDirectory(const std::string  &dirPath, ErrorCode *pErr = 0) const = 0;
    std::vector<DirectoryEntryInfoW> enumerateDirectory(const std::wstring &dirPath, ErrorCode *pErr = 0) const = 0;

    bool isFileExistAndReadable(const std::string  &fName) const = 0;
    bool isFileExistAndReadable(const std::wstring &fName) const = 0;

    bool isDirectory(const std::string  &dName) const = 0;
    bool isDirectory(const std::wstring &dName) const = 0;

    // Тут автоматически работают перекодировки текста
    ErrorCode readTextFile(const std::string  &fName, std::string  &fText) const = 0;
    ErrorCode readTextFile(const std::string  &fName, std::string  &fText) const = 0;
    ErrorCode readTextFile(const std::wstring &fName, std::wstring &fText) const = 0;
    ErrorCode readTextFile(const std::wstring &fName, std::wstring &fText) const = 0;

    // Reading binary files
    ErrorCode readDataFile(const std::string  &fName, std::vector<std::uint8_t> &fData) const = 0;
    ErrorCode readDataFile(const std::wstring &fName, std::vector<std::uint8_t> &fData) const = 0;


    ErrorCode writeTextFile(const std::string  &fName, const std::string  &fText, WriteFileFlags writeFlags) const = 0;
    ErrorCode writeTextFile(const std::string  &fName, const std::string  &fText, WriteFileFlags writeFlags) const = 0;
    ErrorCode writeTextFile(const std::wstring &fName, const std::wstring &fText, WriteFileFlags writeFlags) const = 0;
    ErrorCode writeTextFile(const std::wstring &fName, const std::wstring &fText, WriteFileFlags writeFlags) const = 0;

    ErrorCode writeDataFile(const std::string  &fName, const std::vector<std::uint8_t> &fData, WriteFileFlags writeFlags) const = 0;
    ErrorCode writeDataFile(const std::wstring &fName, const std::vector<std::uint8_t> &fData, WriteFileFlags writeFlags) const = 0;


}; // struct IFileSystem


} // namespace marty_virtual_fs


#include "warnings_restore.h"

