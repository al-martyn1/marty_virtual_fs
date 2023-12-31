@set UINT8=--underlaying-type=std::uint8_t
@set UINT16=--underlaying-type=std::uint16_t
@set UINT32=--underlaying-type=std::uint32_t
@set INT=--underlaying-type=int

@set HEX2=--hex-width=2
@set HEX4=--hex-width=4
@set HEX6=--hex-width=6
@set HEX8=--hex-width=8

@rem set EXTRA=--enum-flags=extra
@same but more detailed
@set FLAGENUM_EXTRA=--enum-flags=serialize-set,deserialize-set

@set FLAGS=--enum-flags=flags
@set DECL=--enum-flags=type-decl
@set CLS=--enum-flags=enum-class
@set DECLCLS=--enum-flags=type-decl,enum-class
@set FMTHEX=--enum-flags=fmt-hex
@set SERIALIZE=--enum-flags=serialize,deserialize,lowercase

@set TPL_OVERRIDE=--override-template-parameter=EnumNameFormat:$(ENAMNAME)
@set GEN_OPTS=--enum-values-style=CamelStyle

@set VALUES_CAMEL=--enum-values-style=CamelStyle
@set VALUES_PASCAL=--enum-values-style=PascalStyle 
@set SERIALIZE_PASCAL=--serialize-style=PascalStyle 

@set ENDIANNESS_GEN_FLAGS=--enum-flags=0 --enum-flags=type-decl,serialize,deserialize,lowercase,enum-class,fmt-hex %VALUES_CAMEL% %SERIALIZE_PASCAL%
@set ENDIANNESS_DEF=invalid,unknown=-1;littleEndian=0;bigEndian=1

@set FILETYPEFLAGS_GEN_FLAGS=        --enum-flags=0 --enum-flags=type-decl,serialize,deserialize,lowercase,enum-class,flags,fmt-hex %VALUES_CAMEL% %SERIALIZE_PASCAL% %FLAGENUM_EXTRA%
@set FILETYPEFLAGS_DEF=invalid=-1;normalFile=0;directory=1;deviceFile=2

@set ERRORCODE_GEN_FLAGS=--enum-flags=0 --enum-flags=type-decl,serialize,deserialize,lowercase,enum-class,fmt-hex %VALUES_CAMEL% %SERIALIZE_PASCAL%
@rem set ERRORCODE_DEF=invalid,unknown=-1;ok=0;genericError=1;notFound=2;notExist=3;alreadyExist=4;accessDenied=5;invalidName;notSupported;invalidMountPoint;invalidMountTarget;notDirectory

@set WRITEFILEFLAGS_GEN_FLAGS=       --enum-flags=0 --enum-flags=type-decl,serialize,deserialize,lowercase,enum-class,flags,fmt-hex %VALUES_CAMEL% %SERIALIZE_PASCAL% %FLAGENUM_EXTRA%
@set WRITEFILEFLAGS_DEF=invalid,unknown=-1;none,normal=0;forceCreateDir=1;forceOverwrite=2

@set SORTFLAGS_GEN_FLAGS=       --enum-flags=0 --enum-flags=type-decl,serialize,deserialize,lowercase,enum-class,flags,fmt-hex %VALUES_CAMEL% %SERIALIZE_PASCAL% %FLAGENUM_EXTRA% %HEX4%
@set SORTFLAGS_DEF=invalid,unknown=-1;none,normal,orderAscending=0;orderDescending=1;ignoreCase=2;digitsAsNumber=4;byType;bySize;byTimeCreation;byTimeLastModified;byTimeLastAccess;directoriesFirst;directoriesLast

@set ENUMERATEFLAGS_GEN_FLAGS=       --enum-flags=0 --enum-flags=type-decl,serialize,deserialize,lowercase,enum-class,flags,fmt-hex %VALUES_CAMEL% %SERIALIZE_PASCAL% %FLAGENUM_EXTRA% %HEX4%
@set ENUMERATEFLAGS_DEF=invalid,unknown=-1;none=0;enumerateFiles=1;enumerateDirectories=2;enumerateBoth,enumerateAll=3

@set FILEMASKFLAGS_GEN_FLAGS=       --enum-flags=0 --enum-flags=type-decl,serialize,deserialize,lowercase,enum-class,flags,fmt-hex %VALUES_CAMEL% %SERIALIZE_PASCAL% %FLAGENUM_EXTRA% %HEX4%
@set FILEMASKFLAGS_DEF=invalid,unknown=-1;none,matchSimple=0;useAnchors=1;matchRegex=2;matchExtOnly=4


umba-enum-gen %GEN_OPTS% %HEX2% %TPL_OVERRIDE% ^
%ERRORCODE_GEN_FLAGS%                   %UINT32% -E=ErrorCode                         -F=@error_code.txt                ^
%ENDIANNESS_GEN_FLAGS%                  %UINT32% -E=Endianness                        -F=%ENDIANNESS_DEF%               ^
%FILETYPEFLAGS_GEN_FLAGS%               %UINT32% -E=FileTypeFlags                     -F=%FILETYPEFLAGS_DEF%            ^
%WRITEFILEFLAGS_GEN_FLAGS%              %UINT32% -E=WriteFileFlags                    -F=%WRITEFILEFLAGS_DEF%           ^
%SORTFLAGS_GEN_FLAGS%                   %UINT32% -E=SortFlags                         -F=%SORTFLAGS_DEF%                ^
%ENUMERATEFLAGS_GEN_FLAGS%              %UINT32% -E=EnumerateFlags                    -F=%ENUMERATEFLAGS_DEF%           ^
%FILEMASKFLAGS_GEN_FLAGS%               %UINT32% -E=FileMaskFlags                     -F=%FILEMASKFLAGS_DEF%            ^
..\vfs_enums.h

