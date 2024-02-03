#pragma once

#include "marty_cpp/marty_enum.h"
#include "marty_cpp/marty_flag_ops.h"
#include "marty_cpp/marty_flags.h"

#include <exception>
#include <map>
#include <set>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <unordered_set>



namespace marty_virtual_fs{

enum class ErrorCode : std::uint32_t
{
    invalid              = (std::uint32_t)(-1),
    unknown              = (std::uint32_t)(-1),
    ok                   = 0x00,
    genericError         = 0x01,
    notFound             = 0x02,
    notExist             = 0x03,
    alreadyExist         = 0x04,
    accessDenied         = 0x05,
    invalidName          = 0x06,
    notSupported         = 0x07,
    invalidMountPoint    = 0x08,
    invalidMountTarget   = 0x09,
    notDirectory         = 0x0A,
    invalidFormat        = 0x0B,
    unknownFormat        = 0x0C,
    missingFiles         = 0x0D,
    notImplemented       = 0x0E,
    convertError         = 0x0F,
    unknownUnits         = 0x10,
    invalidArgument      = 0x11,
    outOfRange           = 0x12,
    noMemory             = 0x13

}; // enum class ErrorCode : std::uint32_t

MARTY_CPP_MAKE_ENUM_IS_FLAGS_FOR_NON_FLAGS_ENUM(ErrorCode)

MARTY_CPP_ENUM_CLASS_SERIALIZE_BEGIN( ErrorCode, std::map, 1 )
    MARTY_CPP_ENUM_CLASS_SERIALIZE_ITEM( ErrorCode::accessDenied         , "AccessDenied"       );
    MARTY_CPP_ENUM_CLASS_SERIALIZE_ITEM( ErrorCode::invalid              , "Invalid"            );
    MARTY_CPP_ENUM_CLASS_SERIALIZE_ITEM( ErrorCode::ok                   , "Ok"                 );
    MARTY_CPP_ENUM_CLASS_SERIALIZE_ITEM( ErrorCode::genericError         , "GenericError"       );
    MARTY_CPP_ENUM_CLASS_SERIALIZE_ITEM( ErrorCode::missingFiles         , "MissingFiles"       );
    MARTY_CPP_ENUM_CLASS_SERIALIZE_ITEM( ErrorCode::alreadyExist         , "AlreadyExist"       );
    MARTY_CPP_ENUM_CLASS_SERIALIZE_ITEM( ErrorCode::notFound             , "NotFound"           );
    MARTY_CPP_ENUM_CLASS_SERIALIZE_ITEM( ErrorCode::notExist             , "NotExist"           );
    MARTY_CPP_ENUM_CLASS_SERIALIZE_ITEM( ErrorCode::invalidName          , "InvalidName"        );
    MARTY_CPP_ENUM_CLASS_SERIALIZE_ITEM( ErrorCode::unknownUnits         , "UnknownUnits"       );
    MARTY_CPP_ENUM_CLASS_SERIALIZE_ITEM( ErrorCode::invalidMountTarget   , "InvalidMountTarget" );
    MARTY_CPP_ENUM_CLASS_SERIALIZE_ITEM( ErrorCode::notSupported         , "NotSupported"       );
    MARTY_CPP_ENUM_CLASS_SERIALIZE_ITEM( ErrorCode::notDirectory         , "NotDirectory"       );
    MARTY_CPP_ENUM_CLASS_SERIALIZE_ITEM( ErrorCode::invalidMountPoint    , "InvalidMountPoint"  );
    MARTY_CPP_ENUM_CLASS_SERIALIZE_ITEM( ErrorCode::invalidFormat        , "InvalidFormat"      );
    MARTY_CPP_ENUM_CLASS_SERIALIZE_ITEM( ErrorCode::unknownFormat        , "UnknownFormat"      );
    MARTY_CPP_ENUM_CLASS_SERIALIZE_ITEM( ErrorCode::notImplemented       , "NotImplemented"     );
    MARTY_CPP_ENUM_CLASS_SERIALIZE_ITEM( ErrorCode::convertError         , "ConvertError"       );
    MARTY_CPP_ENUM_CLASS_SERIALIZE_ITEM( ErrorCode::invalidArgument      , "InvalidArgument"    );
    MARTY_CPP_ENUM_CLASS_SERIALIZE_ITEM( ErrorCode::outOfRange           , "OutOfRange"         );
    MARTY_CPP_ENUM_CLASS_SERIALIZE_ITEM( ErrorCode::noMemory             , "NoMemory"           );
MARTY_CPP_ENUM_CLASS_SERIALIZE_END( ErrorCode, std::map, 1 )

MARTY_CPP_ENUM_CLASS_DESERIALIZE_BEGIN( ErrorCode, std::map, 1 )
    MARTY_CPP_ENUM_CLASS_DESERIALIZE_ITEM( ErrorCode::accessDenied         , "accessdenied"       );
    MARTY_CPP_ENUM_CLASS_DESERIALIZE_ITEM( ErrorCode::invalid              , "invalid"            );
    MARTY_CPP_ENUM_CLASS_DESERIALIZE_ITEM( ErrorCode::invalid              , "unknown"            );
    MARTY_CPP_ENUM_CLASS_DESERIALIZE_ITEM( ErrorCode::ok                   , "ok"                 );
    MARTY_CPP_ENUM_CLASS_DESERIALIZE_ITEM( ErrorCode::genericError         , "genericerror"       );
    MARTY_CPP_ENUM_CLASS_DESERIALIZE_ITEM( ErrorCode::missingFiles         , "missingfiles"       );
    MARTY_CPP_ENUM_CLASS_DESERIALIZE_ITEM( ErrorCode::alreadyExist         , "alreadyexist"       );
    MARTY_CPP_ENUM_CLASS_DESERIALIZE_ITEM( ErrorCode::notFound             , "notfound"           );
    MARTY_CPP_ENUM_CLASS_DESERIALIZE_ITEM( ErrorCode::notExist             , "notexist"           );
    MARTY_CPP_ENUM_CLASS_DESERIALIZE_ITEM( ErrorCode::invalidName          , "invalidname"        );
    MARTY_CPP_ENUM_CLASS_DESERIALIZE_ITEM( ErrorCode::unknownUnits         , "unknownunits"       );
    MARTY_CPP_ENUM_CLASS_DESERIALIZE_ITEM( ErrorCode::invalidMountTarget   , "invalidmounttarget" );
    MARTY_CPP_ENUM_CLASS_DESERIALIZE_ITEM( ErrorCode::notSupported         , "notsupported"       );
    MARTY_CPP_ENUM_CLASS_DESERIALIZE_ITEM( ErrorCode::notDirectory         , "notdirectory"       );
    MARTY_CPP_ENUM_CLASS_DESERIALIZE_ITEM( ErrorCode::invalidMountPoint    , "invalidmountpoint"  );
    MARTY_CPP_ENUM_CLASS_DESERIALIZE_ITEM( ErrorCode::invalidFormat        , "invalidformat"      );
    MARTY_CPP_ENUM_CLASS_DESERIALIZE_ITEM( ErrorCode::unknownFormat        , "unknownformat"      );
    MARTY_CPP_ENUM_CLASS_DESERIALIZE_ITEM( ErrorCode::notImplemented       , "notimplemented"     );
    MARTY_CPP_ENUM_CLASS_DESERIALIZE_ITEM( ErrorCode::convertError         , "converterror"       );
    MARTY_CPP_ENUM_CLASS_DESERIALIZE_ITEM( ErrorCode::invalidArgument      , "invalidargument"    );
    MARTY_CPP_ENUM_CLASS_DESERIALIZE_ITEM( ErrorCode::outOfRange           , "outofrange"         );
    MARTY_CPP_ENUM_CLASS_DESERIALIZE_ITEM( ErrorCode::noMemory             , "nomemory"           );
MARTY_CPP_ENUM_CLASS_DESERIALIZE_END( ErrorCode, std::map, 1 )


enum class Endianness : std::uint32_t
{
    invalid            = (std::uint32_t)(-1),
    unknown            = (std::uint32_t)(-1),
    littleEndian       = 0x00,
    bigEndian          = 0x01,
    networkByteOrder   = 0x01

}; // enum class Endianness : std::uint32_t

MARTY_CPP_MAKE_ENUM_IS_FLAGS_FOR_NON_FLAGS_ENUM(Endianness)

MARTY_CPP_ENUM_CLASS_SERIALIZE_BEGIN( Endianness, std::map, 1 )
    MARTY_CPP_ENUM_CLASS_SERIALIZE_ITEM( Endianness::invalid        , "Invalid"      );
    MARTY_CPP_ENUM_CLASS_SERIALIZE_ITEM( Endianness::bigEndian      , "BigEndian"    );
    MARTY_CPP_ENUM_CLASS_SERIALIZE_ITEM( Endianness::littleEndian   , "LittleEndian" );
MARTY_CPP_ENUM_CLASS_SERIALIZE_END( Endianness, std::map, 1 )

MARTY_CPP_ENUM_CLASS_DESERIALIZE_BEGIN( Endianness, std::map, 1 )
    MARTY_CPP_ENUM_CLASS_DESERIALIZE_ITEM( Endianness::invalid        , "invalid"          );
    MARTY_CPP_ENUM_CLASS_DESERIALIZE_ITEM( Endianness::invalid        , "unknown"          );
    MARTY_CPP_ENUM_CLASS_DESERIALIZE_ITEM( Endianness::bigEndian      , "bigendian"        );
    MARTY_CPP_ENUM_CLASS_DESERIALIZE_ITEM( Endianness::bigEndian      , "networkbyteorder" );
    MARTY_CPP_ENUM_CLASS_DESERIALIZE_ITEM( Endianness::littleEndian   , "littleendian"     );
MARTY_CPP_ENUM_CLASS_DESERIALIZE_END( Endianness, std::map, 1 )


enum class FileTypeFlags : std::uint32_t
{
    invalid      = (std::uint32_t)(-1),
    normalFile   = 0x00,
    directory    = 0x01,
    deviceFile   = 0x02

}; // enum class FileTypeFlags : std::uint32_t

MARTY_CPP_MAKE_ENUM_FLAGS(FileTypeFlags)

MARTY_CPP_ENUM_FLAGS_SERIALIZE_BEGIN( FileTypeFlags, std::map, 1 )
    MARTY_CPP_ENUM_FLAGS_SERIALIZE_ITEM( FileTypeFlags::directory    , "Directory"  );
    MARTY_CPP_ENUM_FLAGS_SERIALIZE_ITEM( FileTypeFlags::invalid      , "Invalid"    );
    MARTY_CPP_ENUM_FLAGS_SERIALIZE_ITEM( FileTypeFlags::normalFile   , "NormalFile" );
    MARTY_CPP_ENUM_FLAGS_SERIALIZE_ITEM( FileTypeFlags::deviceFile   , "DeviceFile" );
MARTY_CPP_ENUM_FLAGS_SERIALIZE_END( FileTypeFlags, std::map, 1 )

MARTY_CPP_ENUM_FLAGS_DESERIALIZE_BEGIN( FileTypeFlags, std::map, 1 )
    MARTY_CPP_ENUM_FLAGS_DESERIALIZE_ITEM( FileTypeFlags::directory    , "directory"  );
    MARTY_CPP_ENUM_FLAGS_DESERIALIZE_ITEM( FileTypeFlags::invalid      , "invalid"    );
    MARTY_CPP_ENUM_FLAGS_DESERIALIZE_ITEM( FileTypeFlags::normalFile   , "normalfile" );
    MARTY_CPP_ENUM_FLAGS_DESERIALIZE_ITEM( FileTypeFlags::deviceFile   , "devicefile" );
MARTY_CPP_ENUM_FLAGS_DESERIALIZE_END( FileTypeFlags, std::map, 1 )

MARTY_CPP_ENUM_FLAGS_SERIALIZE_SET(FileTypeFlags, std::set)

MARTY_CPP_ENUM_FLAGS_DESERIALIZE_SET(FileTypeFlags, std::set)


enum class WriteFileFlags : std::uint32_t
{
    invalid          = (std::uint32_t)(-1),
    unknown          = (std::uint32_t)(-1),
    none             = 0x00,
    normal           = 0x00,
    forceCreateDir   = 0x01,
    forceOverwrite   = 0x02

}; // enum class WriteFileFlags : std::uint32_t

MARTY_CPP_MAKE_ENUM_FLAGS(WriteFileFlags)

MARTY_CPP_ENUM_FLAGS_SERIALIZE_BEGIN( WriteFileFlags, std::map, 1 )
    MARTY_CPP_ENUM_FLAGS_SERIALIZE_ITEM( WriteFileFlags::invalid          , "Invalid"        );
    MARTY_CPP_ENUM_FLAGS_SERIALIZE_ITEM( WriteFileFlags::none             , "None"           );
    MARTY_CPP_ENUM_FLAGS_SERIALIZE_ITEM( WriteFileFlags::forceCreateDir   , "ForceCreateDir" );
    MARTY_CPP_ENUM_FLAGS_SERIALIZE_ITEM( WriteFileFlags::forceOverwrite   , "ForceOverwrite" );
MARTY_CPP_ENUM_FLAGS_SERIALIZE_END( WriteFileFlags, std::map, 1 )

MARTY_CPP_ENUM_FLAGS_DESERIALIZE_BEGIN( WriteFileFlags, std::map, 1 )
    MARTY_CPP_ENUM_FLAGS_DESERIALIZE_ITEM( WriteFileFlags::invalid          , "invalid"        );
    MARTY_CPP_ENUM_FLAGS_DESERIALIZE_ITEM( WriteFileFlags::invalid          , "unknown"        );
    MARTY_CPP_ENUM_FLAGS_DESERIALIZE_ITEM( WriteFileFlags::none             , "none"           );
    MARTY_CPP_ENUM_FLAGS_DESERIALIZE_ITEM( WriteFileFlags::none             , "normal"         );
    MARTY_CPP_ENUM_FLAGS_DESERIALIZE_ITEM( WriteFileFlags::forceCreateDir   , "forcecreatedir" );
    MARTY_CPP_ENUM_FLAGS_DESERIALIZE_ITEM( WriteFileFlags::forceOverwrite   , "forceoverwrite" );
MARTY_CPP_ENUM_FLAGS_DESERIALIZE_END( WriteFileFlags, std::map, 1 )

MARTY_CPP_ENUM_FLAGS_SERIALIZE_SET(WriteFileFlags, std::set)

MARTY_CPP_ENUM_FLAGS_DESERIALIZE_SET(WriteFileFlags, std::set)


enum class SortFlags : std::uint32_t
{
    invalid              = (std::uint32_t)(-1),
    unknown              = (std::uint32_t)(-1),
    none                 = 0x0000,
    normal               = 0x0000,
    orderAscending       = 0x0000,
    orderDescending      = 0x0001,
    ignoreCase           = 0x0002,
    digitsAsNumber       = 0x0004,
    byType               = 0x0008,
    bySize               = 0x0010,
    byTimeCreation       = 0x0020,
    byTimeLastModified   = 0x0040,
    byTimeLastAccess     = 0x0080,
    directoriesFirst     = 0x0100,
    directoriesLast      = 0x0200

}; // enum class SortFlags : std::uint32_t

MARTY_CPP_MAKE_ENUM_FLAGS(SortFlags)

MARTY_CPP_ENUM_FLAGS_SERIALIZE_BEGIN( SortFlags, std::map, 1 )
    MARTY_CPP_ENUM_FLAGS_SERIALIZE_ITEM( SortFlags::invalid              , "Invalid"            );
    MARTY_CPP_ENUM_FLAGS_SERIALIZE_ITEM( SortFlags::none                 , "None"               );
    MARTY_CPP_ENUM_FLAGS_SERIALIZE_ITEM( SortFlags::orderDescending      , "OrderDescending"    );
    MARTY_CPP_ENUM_FLAGS_SERIALIZE_ITEM( SortFlags::ignoreCase           , "IgnoreCase"         );
    MARTY_CPP_ENUM_FLAGS_SERIALIZE_ITEM( SortFlags::digitsAsNumber       , "DigitsAsNumber"     );
    MARTY_CPP_ENUM_FLAGS_SERIALIZE_ITEM( SortFlags::byTimeCreation       , "ByTimeCreation"     );
    MARTY_CPP_ENUM_FLAGS_SERIALIZE_ITEM( SortFlags::byType               , "ByType"             );
    MARTY_CPP_ENUM_FLAGS_SERIALIZE_ITEM( SortFlags::bySize               , "BySize"             );
    MARTY_CPP_ENUM_FLAGS_SERIALIZE_ITEM( SortFlags::byTimeLastModified   , "ByTimeLastModified" );
    MARTY_CPP_ENUM_FLAGS_SERIALIZE_ITEM( SortFlags::byTimeLastAccess     , "ByTimeLastAccess"   );
    MARTY_CPP_ENUM_FLAGS_SERIALIZE_ITEM( SortFlags::directoriesFirst     , "DirectoriesFirst"   );
    MARTY_CPP_ENUM_FLAGS_SERIALIZE_ITEM( SortFlags::directoriesLast      , "DirectoriesLast"    );
MARTY_CPP_ENUM_FLAGS_SERIALIZE_END( SortFlags, std::map, 1 )

MARTY_CPP_ENUM_FLAGS_DESERIALIZE_BEGIN( SortFlags, std::map, 1 )
    MARTY_CPP_ENUM_FLAGS_DESERIALIZE_ITEM( SortFlags::invalid              , "invalid"            );
    MARTY_CPP_ENUM_FLAGS_DESERIALIZE_ITEM( SortFlags::invalid              , "unknown"            );
    MARTY_CPP_ENUM_FLAGS_DESERIALIZE_ITEM( SortFlags::none                 , "orderascending"     );
    MARTY_CPP_ENUM_FLAGS_DESERIALIZE_ITEM( SortFlags::none                 , "none"               );
    MARTY_CPP_ENUM_FLAGS_DESERIALIZE_ITEM( SortFlags::none                 , "normal"             );
    MARTY_CPP_ENUM_FLAGS_DESERIALIZE_ITEM( SortFlags::orderDescending      , "orderdescending"    );
    MARTY_CPP_ENUM_FLAGS_DESERIALIZE_ITEM( SortFlags::ignoreCase           , "ignorecase"         );
    MARTY_CPP_ENUM_FLAGS_DESERIALIZE_ITEM( SortFlags::digitsAsNumber       , "digitsasnumber"     );
    MARTY_CPP_ENUM_FLAGS_DESERIALIZE_ITEM( SortFlags::byTimeCreation       , "bytimecreation"     );
    MARTY_CPP_ENUM_FLAGS_DESERIALIZE_ITEM( SortFlags::byType               , "bytype"             );
    MARTY_CPP_ENUM_FLAGS_DESERIALIZE_ITEM( SortFlags::bySize               , "bysize"             );
    MARTY_CPP_ENUM_FLAGS_DESERIALIZE_ITEM( SortFlags::byTimeLastModified   , "bytimelastmodified" );
    MARTY_CPP_ENUM_FLAGS_DESERIALIZE_ITEM( SortFlags::byTimeLastAccess     , "bytimelastaccess"   );
    MARTY_CPP_ENUM_FLAGS_DESERIALIZE_ITEM( SortFlags::directoriesFirst     , "directoriesfirst"   );
    MARTY_CPP_ENUM_FLAGS_DESERIALIZE_ITEM( SortFlags::directoriesLast      , "directorieslast"    );
MARTY_CPP_ENUM_FLAGS_DESERIALIZE_END( SortFlags, std::map, 1 )

MARTY_CPP_ENUM_FLAGS_SERIALIZE_SET(SortFlags, std::set)

MARTY_CPP_ENUM_FLAGS_DESERIALIZE_SET(SortFlags, std::set)


enum class EnumerateFlags : std::uint32_t
{
    invalid                = (std::uint32_t)(-1),
    unknown                = (std::uint32_t)(-1),
    none                   = 0x0000,
    enumerateFiles         = 0x0001,
    enumerateDirectories   = 0x0002,
    enumerateBoth          = 0x0003,
    enumerateAll           = 0x0003

}; // enum class EnumerateFlags : std::uint32_t

MARTY_CPP_MAKE_ENUM_FLAGS(EnumerateFlags)

MARTY_CPP_ENUM_FLAGS_SERIALIZE_BEGIN( EnumerateFlags, std::map, 1 )
    MARTY_CPP_ENUM_FLAGS_SERIALIZE_ITEM( EnumerateFlags::enumerateFiles         , "EnumerateFiles"       );
    MARTY_CPP_ENUM_FLAGS_SERIALIZE_ITEM( EnumerateFlags::invalid                , "Invalid"              );
    MARTY_CPP_ENUM_FLAGS_SERIALIZE_ITEM( EnumerateFlags::none                   , "None"                 );
    MARTY_CPP_ENUM_FLAGS_SERIALIZE_ITEM( EnumerateFlags::enumerateDirectories   , "EnumerateDirectories" );
    MARTY_CPP_ENUM_FLAGS_SERIALIZE_ITEM( EnumerateFlags::enumerateBoth          , "EnumerateBoth"        );
MARTY_CPP_ENUM_FLAGS_SERIALIZE_END( EnumerateFlags, std::map, 1 )

MARTY_CPP_ENUM_FLAGS_DESERIALIZE_BEGIN( EnumerateFlags, std::map, 1 )
    MARTY_CPP_ENUM_FLAGS_DESERIALIZE_ITEM( EnumerateFlags::enumerateFiles         , "enumeratefiles"       );
    MARTY_CPP_ENUM_FLAGS_DESERIALIZE_ITEM( EnumerateFlags::invalid                , "invalid"              );
    MARTY_CPP_ENUM_FLAGS_DESERIALIZE_ITEM( EnumerateFlags::invalid                , "unknown"              );
    MARTY_CPP_ENUM_FLAGS_DESERIALIZE_ITEM( EnumerateFlags::none                   , "none"                 );
    MARTY_CPP_ENUM_FLAGS_DESERIALIZE_ITEM( EnumerateFlags::enumerateDirectories   , "enumeratedirectories" );
    MARTY_CPP_ENUM_FLAGS_DESERIALIZE_ITEM( EnumerateFlags::enumerateBoth          , "enumerateboth"        );
    MARTY_CPP_ENUM_FLAGS_DESERIALIZE_ITEM( EnumerateFlags::enumerateBoth          , "enumerateall"         );
MARTY_CPP_ENUM_FLAGS_DESERIALIZE_END( EnumerateFlags, std::map, 1 )

MARTY_CPP_ENUM_FLAGS_SERIALIZE_SET(EnumerateFlags, std::set)

MARTY_CPP_ENUM_FLAGS_DESERIALIZE_SET(EnumerateFlags, std::set)


enum class FileMaskFlags : std::uint32_t
{
    invalid        = (std::uint32_t)(-1),
    unknown        = (std::uint32_t)(-1),
    none           = 0x0000,
    matchSimple    = 0x0000,
    useAnchors     = 0x0001,
    matchRegex     = 0x0002,
    matchExtOnly   = 0x0004

}; // enum class FileMaskFlags : std::uint32_t

MARTY_CPP_MAKE_ENUM_FLAGS(FileMaskFlags)

MARTY_CPP_ENUM_FLAGS_SERIALIZE_BEGIN( FileMaskFlags, std::map, 1 )
    MARTY_CPP_ENUM_FLAGS_SERIALIZE_ITEM( FileMaskFlags::invalid        , "Invalid"      );
    MARTY_CPP_ENUM_FLAGS_SERIALIZE_ITEM( FileMaskFlags::none           , "None"         );
    MARTY_CPP_ENUM_FLAGS_SERIALIZE_ITEM( FileMaskFlags::useAnchors     , "UseAnchors"   );
    MARTY_CPP_ENUM_FLAGS_SERIALIZE_ITEM( FileMaskFlags::matchRegex     , "MatchRegex"   );
    MARTY_CPP_ENUM_FLAGS_SERIALIZE_ITEM( FileMaskFlags::matchExtOnly   , "MatchExtOnly" );
MARTY_CPP_ENUM_FLAGS_SERIALIZE_END( FileMaskFlags, std::map, 1 )

MARTY_CPP_ENUM_FLAGS_DESERIALIZE_BEGIN( FileMaskFlags, std::map, 1 )
    MARTY_CPP_ENUM_FLAGS_DESERIALIZE_ITEM( FileMaskFlags::invalid        , "invalid"      );
    MARTY_CPP_ENUM_FLAGS_DESERIALIZE_ITEM( FileMaskFlags::invalid        , "unknown"      );
    MARTY_CPP_ENUM_FLAGS_DESERIALIZE_ITEM( FileMaskFlags::none           , "none"         );
    MARTY_CPP_ENUM_FLAGS_DESERIALIZE_ITEM( FileMaskFlags::none           , "matchsimple"  );
    MARTY_CPP_ENUM_FLAGS_DESERIALIZE_ITEM( FileMaskFlags::useAnchors     , "useanchors"   );
    MARTY_CPP_ENUM_FLAGS_DESERIALIZE_ITEM( FileMaskFlags::matchRegex     , "matchregex"   );
    MARTY_CPP_ENUM_FLAGS_DESERIALIZE_ITEM( FileMaskFlags::matchExtOnly   , "matchextonly" );
MARTY_CPP_ENUM_FLAGS_DESERIALIZE_END( FileMaskFlags, std::map, 1 )

MARTY_CPP_ENUM_FLAGS_SERIALIZE_SET(FileMaskFlags, std::set)

MARTY_CPP_ENUM_FLAGS_DESERIALIZE_SET(FileMaskFlags, std::set)

} // namespace marty_virtual_fs

