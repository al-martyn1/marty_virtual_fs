/*! \file
    \brief Virtual Filesystem definitions
*/

#pragma once

//----------------------------------------------------------------------------
#ifndef MARTY_VFS_ARG_USED

    //! Подавление варнинга о неиспользованном аргументе
    #define MARTY_VFS_USED(x)                   (void)(x)

#endif

//----------------------------------------------------------------------------



#if !defined(MARTY_VFS_ARCH_LITTLE_ENDIAN) && !defined(MARTY_VFS_ARCH_BIG_ENDIAN)

    #if defined(UMBA_ARCH_LITTLE_ENDIAN)

        #define MARTY_VFS_ARCH_LITTLE_ENDIAN

    #elif defined(UMBA_ARCH_BIG_ENDIAN)

        #define MARTY_VFS_ARCH_BIG_ENDIAN

    #endif

#endif

#if !defined(MARTY_VFS_ARCH_LITTLE_ENDIAN) && !defined(MARTY_VFS_ARCH_BIG_ENDIAN)

    #define MARTY_VFS_ARCH_LITTLE_ENDIAN

#endif

#if defined(MARTY_VFS_ARCH_LITTLE_ENDIAN) && defined(MARTY_VFS_ARCH_BIG_ENDIAN)

    #error "MARTY_VFS_ARCH_LITTLE_ENDIAN macro conficts with MARTY_VFS_ARCH_BIG_ENDIAN macro"

#endif
