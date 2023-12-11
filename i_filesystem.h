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

    // Нормализует виртуальное имя файла, нормализует разделители пути, и схлопывает спец пути типа "."/"..", 
    // чтобы мамкины "хакеры" из скрипта не могли вылезти за пределы песочницы
    // Выставляем наружу, чтобы в скрипте можно было проверить, как выглядит путь, который будет реально использоваться
    virtual std::string  normalizeFilename(const std::string  &fname) const = 0;
    virtual std::wstring normalizeFilename(const std::wstring &fname) const = 0;

    virtual void getErrorCodeString(ErrorCode e, std::string  &errStr) const = 0;
    virtual void getErrorCodeString(ErrorCode e, std::wstring &errStr) const = 0;


    // Конвертация в/из нативных путей. Если путь вне вирт системы - ErrorCode::notFound. 
    // Если текущая ф. система не поддерживает конвертацию в нативное имя - ErrorCode::notSupported
    virtual ErrorCode fromNativePathName(const std::string  &nativeName, std::string  &vfsName) const = 0;
    virtual ErrorCode fromNativePathName(const std::wstring &nativeName, std::wstring &vfsName) const = 0;

    virtual ErrorCode toNativePathName(const std::string  &vfsName, std::string  &nativeName) const = 0;
    virtual ErrorCode toNativePathName(const std::wstring &vfsName, std::wstring &nativeName) const = 0;

    // Нерекурсивный обзор содержимого каталога
    virtual ErrorCode enumerateDirectory(const std::string  &dirPath, std::vector<DirectoryEntryInfoA> &entries) const = 0;
    virtual ErrorCode enumerateDirectory(const std::wstring &dirPath, std::vector<DirectoryEntryInfoW> &entries ) const = 0;

    virtual std::vector<DirectoryEntryInfoA> enumerateDirectory(const std::string  &dirPath, ErrorCode *pErr = 0) const = 0;
    virtual std::vector<DirectoryEntryInfoW> enumerateDirectory(const std::wstring &dirPath, ErrorCode *pErr = 0) const = 0;

    virtual bool isFileExistAndReadable(const std::string  &fName) const = 0;
    virtual bool isFileExistAndReadable(const std::wstring &fName) const = 0;

    virtual bool isDirectory(const std::string  &dName) const = 0;
    virtual bool isDirectory(const std::wstring &dName) const = 0;

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

