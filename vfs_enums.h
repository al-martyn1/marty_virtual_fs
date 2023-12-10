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

enum class Endianness : std::uint32_t
{
    invalid        = (std::uint32_t)(-1),
    unknown        = (std::uint32_t)(-1),
    littleEndian   = 0x00,
    bigEndian      = 0x01

}; // enum class Endianness : std::uint32_t

MARTY_CPP_ENUM_CLASS_SERIALIZE_BEGIN( Endianness, std::map, 1 )
    MARTY_CPP_ENUM_CLASS_SERIALIZE_ITEM( Endianness::invalid        , "Invalid"      );
    MARTY_CPP_ENUM_CLASS_SERIALIZE_ITEM( Endianness::bigEndian      , "BigEndian"    );
    MARTY_CPP_ENUM_CLASS_SERIALIZE_ITEM( Endianness::littleEndian   , "LittleEndian" );
MARTY_CPP_ENUM_CLASS_SERIALIZE_END( Endianness, std::map, 1 )

MARTY_CPP_ENUM_CLASS_DESERIALIZE_BEGIN( Endianness, std::map, 1 )
    MARTY_CPP_ENUM_CLASS_DESERIALIZE_ITEM( Endianness::invalid        , "invalid"      );
    MARTY_CPP_ENUM_CLASS_DESERIALIZE_ITEM( Endianness::invalid        , "unknown"      );
    MARTY_CPP_ENUM_CLASS_DESERIALIZE_ITEM( Endianness::bigEndian      , "bigendian"    );
    MARTY_CPP_ENUM_CLASS_DESERIALIZE_ITEM( Endianness::littleEndian   , "littleendian" );
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
    notDirectory         = 0x0A

}; // enum class ErrorCode : std::uint32_t

MARTY_CPP_ENUM_CLASS_SERIALIZE_BEGIN( ErrorCode, std::map, 1 )
    MARTY_CPP_ENUM_CLASS_SERIALIZE_ITEM( ErrorCode::accessDenied         , "AccessDenied"       );
    MARTY_CPP_ENUM_CLASS_SERIALIZE_ITEM( ErrorCode::invalid              , "Invalid"            );
    MARTY_CPP_ENUM_CLASS_SERIALIZE_ITEM( ErrorCode::ok                   , "Ok"                 );
    MARTY_CPP_ENUM_CLASS_SERIALIZE_ITEM( ErrorCode::genericError         , "GenericError"       );
    MARTY_CPP_ENUM_CLASS_SERIALIZE_ITEM( ErrorCode::alreadyExist         , "AlreadyExist"       );
    MARTY_CPP_ENUM_CLASS_SERIALIZE_ITEM( ErrorCode::notFound             , "NotFound"           );
    MARTY_CPP_ENUM_CLASS_SERIALIZE_ITEM( ErrorCode::notExist             , "NotExist"           );
    MARTY_CPP_ENUM_CLASS_SERIALIZE_ITEM( ErrorCode::invalidName          , "InvalidName"        );
    MARTY_CPP_ENUM_CLASS_SERIALIZE_ITEM( ErrorCode::invalidMountTarget   , "InvalidMountTarget" );
    MARTY_CPP_ENUM_CLASS_SERIALIZE_ITEM( ErrorCode::notSupported         , "NotSupported"       );
    MARTY_CPP_ENUM_CLASS_SERIALIZE_ITEM( ErrorCode::notDirectory         , "NotDirectory"       );
    MARTY_CPP_ENUM_CLASS_SERIALIZE_ITEM( ErrorCode::invalidMountPoint    , "InvalidMountPoint"  );
MARTY_CPP_ENUM_CLASS_SERIALIZE_END( ErrorCode, std::map, 1 )

MARTY_CPP_ENUM_CLASS_DESERIALIZE_BEGIN( ErrorCode, std::map, 1 )
    MARTY_CPP_ENUM_CLASS_DESERIALIZE_ITEM( ErrorCode::accessDenied         , "accessdenied"       );
    MARTY_CPP_ENUM_CLASS_DESERIALIZE_ITEM( ErrorCode::invalid              , "invalid"            );
    MARTY_CPP_ENUM_CLASS_DESERIALIZE_ITEM( ErrorCode::invalid              , "unknown"            );
    MARTY_CPP_ENUM_CLASS_DESERIALIZE_ITEM( ErrorCode::ok                   , "ok"                 );
    MARTY_CPP_ENUM_CLASS_DESERIALIZE_ITEM( ErrorCode::genericError         , "genericerror"       );
    MARTY_CPP_ENUM_CLASS_DESERIALIZE_ITEM( ErrorCode::alreadyExist         , "alreadyexist"       );
    MARTY_CPP_ENUM_CLASS_DESERIALIZE_ITEM( ErrorCode::notFound             , "notfound"           );
    MARTY_CPP_ENUM_CLASS_DESERIALIZE_ITEM( ErrorCode::notExist             , "notexist"           );
    MARTY_CPP_ENUM_CLASS_DESERIALIZE_ITEM( ErrorCode::invalidName          , "invalidname"        );
    MARTY_CPP_ENUM_CLASS_DESERIALIZE_ITEM( ErrorCode::invalidMountTarget   , "invalidmounttarget" );
    MARTY_CPP_ENUM_CLASS_DESERIALIZE_ITEM( ErrorCode::notSupported         , "notsupported"       );
    MARTY_CPP_ENUM_CLASS_DESERIALIZE_ITEM( ErrorCode::notDirectory         , "notdirectory"       );
    MARTY_CPP_ENUM_CLASS_DESERIALIZE_ITEM( ErrorCode::invalidMountPoint    , "invalidmountpoint"  );
MARTY_CPP_ENUM_CLASS_DESERIALIZE_END( ErrorCode, std::map, 1 )


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

} // namespace marty_virtual_fs

