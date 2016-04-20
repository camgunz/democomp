#==============================================================================
############################################################# License #########
#==============================================================================
#
#  Copyright (c) 2010 Jamie Jones <jamie_jones_au@yahoo.com.au>
#  Copyright (C) 2013 David Hill
#  Copyright (C) 2014 Charles Gunyon <charles.gunyon@gmail.com>
#
#  This software is free software; you can redistribute it and/or modify it
#  under the terms of the GNU General Public License as published by the Free
#  Software Foundation; either version 2 of the License, or (at your option)
#  any later version.
#
#  This software is distributed in the hope that it will be useful, but WITHOUT
#  ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
#  FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
#  more details.
#
#  You should have received a copy of the GNU General Public License along with
#  this program; if not, write to the Free Software Foundation, Inc., 51
#  Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
#
#==============================================================================

#==============================================================================
################################################# CMake Configuration #########
#==============================================================================
PROJECT("DEMOCOMP" C)
CMAKE_MINIMUM_REQUIRED(VERSION 2.8.0 FATAL_ERROR)
IF(${CMAKE_BINARY_DIR} STREQUAL ${CMAKE_SOURCE_DIR})
  MESSAGE(${CMAKE_SOURCE_DIR})
  MESSAGE(${CMAKE_BINARY_DIR})
  MESSAGE(FATAL_ERROR "In-tree Builds are NOT supported.")
ENDIF()

IF(WIN32)
  SET(CMAKE_MFC_FLAG 2)
  SET(CMAKE_INSTALL_MFC_LIBRARIES 1)
  INCLUDE(InstallRequiredSystemLibraries)
ENDIF()

###
# Supported Build Types are:
#  * Debug (CMAKE_C_FLAGS_DEBUG)
#  * Release (CMAKE_C_FLAGS_RELEASE)
# If no build type requested, default to Release
###

IF(NOT DEFINED CMAKE_BUILD_TYPE)
  SET(CMAKE_BUILD_TYPE Release)
ENDIF()

# Default to not building shared libraries.
IF(NOT DEFINED BUILD_SHARED_LIBS)
  SET(BUILD_SHARED_LIBS OFF)
ENDIF()

# Default to not installing any sub-projects.
IF(NOT DEFINED SKIP_INSTALL_ALL)
  SET(SKIP_INSTALL_ALL ON)
ENDIF()

INCLUDE(CheckIncludeFiles)
INCLUDE(CheckFunctionExists)
INCLUDE(CheckSymbolExists)
INCLUDE(CheckLibraryExists)
INCLUDE(CheckTypeSize)
INCLUDE(CheckCSourceCompiles)
INCLUDE(CheckCCompilerFlag)
INCLUDE(CMakeDependentOption)

SET(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake/modules)
SET(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

#==============================================================================
########################################################### Functions #########
#==============================================================================
FUNCTION(TRY_C_COMPILER_FLAG flag name)
  CHECK_C_COMPILER_FLAG("${flag}" ${name})
  IF(${name})
    SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${flag}" PARENT_SCOPE)
  ENDIF()
ENDFUNCTION(TRY_C_COMPILER_FLAG flag name)

#==============================================================================
##################################################### Package Details #########
#==============================================================================
SET(VERSION "0")
SET(PACKAGE "DEMOCOMP")
SET(PACKAGE_NAME "DEMOCOMP")
SET(PACKAGE_VERSION "${VERSION}")
# SET(PACKAGE_BUGREPORT "huh")
SET(PACKAGE_STRING "${PACKAGE_NAME} v${PACKAGE_VERSION}")
SET(PACKAGE_TARNAME "democomp")
SET(PACKAGE_URL "http://github.com/camgunz/democomp")

SET(CPACK_GENERATOR "DEB;RPM;STGZ;ZIP")
SET(CPACK_PACKAGE_VENDOR "Team TotalTrash")
SET(CPACK_PACKAGE_CONTACT "Team TotalTrash <ladna@totaltrash.org>")
SET(CPACK_PACKAGE_VERSION "${VERSION}")
SET(CPACK_PACKAGE_NAME "DEMOCOMP")
SET(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_SOURCE_DIR}/COPYING")
SET(CPACK_PACKAGE_DESCRIPTION_SUMMARY
  "DEMOCOMP is a tool to compare demo playback state dumps"
)
SET(CPACK_DEBIAN_PACKAGE_SECTION "games")
SET(CPACK_DEBIAN_PACKAGE_DEPENDS
  "libc6 (>= 2.11.0),
   libgcc1 (>= 1:4.4.3),
   libxdiff (>= 0.23),
   bash"
)
SET(CPACK_DEBIAN_PACKAGE_RECOMMENDS
  "boom-wad or doom-wad,
   timidity,
   game-data-packager"
)

SET(BUILD_ARCH "${CMAKE_SYSTEM_PROCESSOR}")

IF(${CMAKE_SYSTEM_PROCESSOR} MATCHES "i686")
  SET(CPACK_DEBIAN_PACKAGE_ARCHITECTURE "i386")
  SET(CPACK_RPM_PACKAGE_ARCHITECTURE "i686" )
  SET(BUILD_ARCH "${CPACK_DEBIAN_PACKAGE_ARCHITECTURE}")
ENDIF()

IF(${CMAKE_SYSTEM_PROCESSOR} MATCHES "x86_64")
  SET(CPACK_DEBIAN_PACKAGE_ARCHITECTURE "amd64")
  SET(CPACK_RPM_PACKAGE_ARCHITECTURE "x86_64" )
  SET(BUILD_ARCH "${CPACK_DEBIAN_PACKAGE_ARCHITECTURE}")
ENDIF()

IF(WIN32)
  SET(BUILD_ARCH "windows-x86")
  IF("$ENV{PROCESSOR_ARCHITECTURE}" STREQUAL "AMD64")
    SET(BUILD_ARCH "windows-$ENV{PROCESSOR_ARCHITECTURE}")
  ENDIF()
ENDIF()

SET(
  CPACK_PACKAGE_FILE_NAME
  "${CPACK_PACKAGE_NAME}-v${CPACK_PACKAGE_VERSION}_${BUILD_ARCH}"
)
SET(CPACK_DEBIAN_PACKAGE_NAME "${CPACK_PACKAGE_FILE_NAME}")
SET(CPACK_RPM_PACKAGE_NAME "${CPACK_PACKAGE_FILE_NAME}")

INCLUDE(CPack)

IF(CMAKE_C_COMPILER_ID STREQUAL "Clang")
  SET(CLANG TRUE)
ELSE()
  SET(CLANG FALSE)
ENDIF()

IF(CMAKE_C_COMPILER_ID STREQUAL "GNU")
  SET(GCC TRUE)
ELSE()
  SET(GCC FALSE)
ENDIF()

IF(CLANG OR GCC)
  SET(GCC_OR_CLANG TRUE)
ELSE()
  SET(GCC_OR_CLANG FALSE)
ENDIF()

#==============================================================================
##################################################### Platform Checks #########
#==============================================================================

IF(MINGW)
  SET(CMAKE_REQUIRED_DEFINITIONS
    ${CMAKE_REQUIRED_DEFINITIONS} "-D__USE_MINGW_ANSI_STDIO=1"
  )
ENDIF()

CHECK_INCLUDE_FILES("asm/byteorder.h" HAVE_ASM_BYTEORDER_H)
CHECK_INCLUDE_FILES("inttypes.h" HAVE_INTTYPES_H)
CHECK_INCLUDE_FILES("stdbool.h" HAVE_STDBOOL_H)
CHECK_INCLUDE_FILES("stdint.h" HAVE_STDINT_H)
CHECK_INCLUDE_FILES("sys/types.h" HAVE_SYS_TYPES_H)
CHECK_INCLUDE_FILES("sys/wait.h" HAVE_SYS_WAIT_H)
CHECK_INCLUDE_FILES("unistd.h" HAVE_UNISTD_H)

CHECK_SYMBOL_EXISTS(sys_siglist signal.h HAVE_DECL_SYS_SIGLIST)

CHECK_C_SOURCE_COMPILES(
  "struct __attribute__((packed)) foo { int a; } b; int main() { return 0; }"
  HAVE_ATTRIBUTE_PACKED
)
IF(HAVE_ATTRIBUTE_PACKED)
  SET(PACKEDATTR "__attribute__((packed))")
ELSE()
  MESSAGE(FATAL_ERROR "Can't figure out how to pack structs with your compiler")
ENDIF()

CHECK_FUNCTION_EXISTS("max" MAX)
CHECK_FUNCTION_EXISTS("min" MIN)

CHECK_SYMBOL_EXISTS(_stricmp string.h HAVE__STRICMP)
CHECK_SYMBOL_EXISTS(stricmp string.h HAVE_STRICMP)
CHECK_SYMBOL_EXISTS(strcasecmp string.h HAVE_STRCASECMP)
CHECK_SYMBOL_EXISTS(_strnicmp string.h HAVE__STRNICMP)
CHECK_SYMBOL_EXISTS(strnicmp string.h HAVE_STRNICMP)
CHECK_SYMBOL_EXISTS(strncasecmp string.h HAVE_STRNCASECMP)

IF(HAVE_STRICMP)
  SET(stricmp stricmp)
ELSE()
  IF(HAVE__STRICMP)
    SET(stricmp _stricmp)
  ELSE()
    IF(HAVE_STRCASECMP)
      SET(stricmp strcasecmp)
    ELSE()
      MESSAGE(FATAL_ERROR "Neither _stricmp, stricmp, or strcasecmp found")
    ENDIF()
  ENDIF()
ENDIF()

IF(HAVE_STRNICMP)
  SET(strnicmp strnicmp)
ELSE()
  IF(HAVE__STRNICMP)
    SET(strnicmp _strnicmp)
  ELSE()
    IF(HAVE_STRNCASECMP)
      SET(strnicmp strncasecmp)
    ELSE()
      MESSAGE(FATAL_ERROR "Neither _strnicmp, strnicmp, or strncasecmp found")
    ENDIF()
  ENDIF()
ENDIF()

CHECK_SYMBOL_EXISTS(_snprintf string.h FOUND_string__SNPRINTF)
IF(FOUND_string_SNPRINTF)
  SET(snprintf _snprintf)
ELSE()
  CHECK_SYMBOL_EXISTS(snprintf string.h FOUND_string_SNPRINTF)
  IF(FOUND_string_SNPRINTF)
    SET(snprintf snprintf)
  ELSE()
    CHECK_SYMBOL_EXISTS(_snprintf stdio.h FOUND_stdio__SNPRINTF)
    IF(FOUND_stdio_SNPRINTF)
      SET(snprintf _snprintf)
    ELSE()
      CHECK_SYMBOL_EXISTS(snprintf stdio.h FOUND_stdio_SNPRINTF)
      IF(FOUND_stdio_SNPRINTF)
        SET(snprintf snprintf)
      ELSE()
        MESSAGE(FATAL_ERROR "Neither snprintf nor _snprintf found")
      ENDIF()
    ENDIF()
  ENDIF()
ENDIF()

CHECK_SYMBOL_EXISTS(_vsnprintf string.h FOUND_string__VSNPRINTF)
IF(FOUND_string_VSNPRINTF)
  SET(vsnprintf _vsnprintf)
ELSE()
  CHECK_SYMBOL_EXISTS(vsnprintf string.h FOUND_string_VSNPRINTF)
  IF(FOUND_string_VSNPRINTF)
    SET(vsnprintf vsnprintf)
  ELSE()
    CHECK_SYMBOL_EXISTS(_vsnprintf stdio.h FOUND_stdio__VSNPRINTF)
    IF(FOUND_stdio_VSNPRINTF)
      SET(vsnprintf _vsnprintf)
    ELSE()
      CHECK_SYMBOL_EXISTS(vsnprintf stdio.h FOUND_stdio_VSNPRINTF)
      IF(FOUND_stdio_VSNPRINTF)
        SET(vsnprintf vsnprintf)
      ELSE()
        MESSAGE(FATAL_ERROR "Neither vsnprintf nor _vsnprintf found")
      ENDIF()
    ENDIF()
  ENDIF()
ENDIF()

#==============================================================================
################################################## Compiler: Features #########
#==============================================================================
IF(CMAKE_C_COMPILER_ID STREQUAL "GNU")
  TRY_C_COMPILER_FLAG(--std=c99 FLAG_C_stdc99)
  IF(PROFILE)
    TRY_C_COMPILER_FLAG(-pg FLAG_C_profile)
  ENDIF()
ENDIF()

#==============================================================================
################################################## Compiler: Warnings #########
#==============================================================================
IF(GCC_OR_CLANG)
  TRY_C_COMPILER_FLAG(-Wall FLAG_C_Wall)
ENDIF()

IF(CMAKE_C_COMPILER_ID STREQUAL "Intel")
  TRY_C_COMPILER_FLAG(-Wall              FLAG_C_Wall)
  TRY_C_COMPILER_FLAG(-Wcheck            FLAG_C_Wcheck)
  TRY_C_COMPILER_FLAG(-Wp64              FLAG_C_Wp64)
  TRY_C_COMPILER_FLAG(-Wshorten-64-to-32 FLAG_C_Wshorten_64_to_32)
ENDIF()

IF(MSVC)
  ADD_DEFINITIONS(-D_CRT_SECURE_NO_WARNINGS)
  ADD_DEFINITIONS(-D_CRT_NONSTDC_NO_WARNINGS)
ENDIF()

#==============================================================================
################################################# Compiler: Hardening #########
#==============================================================================
IF(CMAKE_BUILD_TYPE STREQUAL "Debug")
  ADD_DEFINITIONS(-DDEBUG)
  IF(CLANG AND SANITIZE_ADDRESSES)
    SET(CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} -fsanitize=address")
    SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fsanitize=address")
    SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O0")
    SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fno-omit-frame-pointer")
    SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fno-optimize-sibling-calls")
    TRY_C_COMPILER_FLAG(-fno-omit-frame-pointer FLAG_C_NO_OMIT_FRAME_POINTER)
  ELSEIF(GCC)
    TRY_C_COMPILER_FLAG(-fstack-protector-strong FLAG_C_fstackprotector)
  ENDIF()
ELSE()
  IF(GCC_OR_CLANG)
    TRY_C_COMPILER_FLAG(-fstack-protector FLAG_C_fstack_protector)

    IF(CMAKE_SYSTEM_NAME STREQUAL "Linux")
      # TRY_C_COMPILER_FLAG(-fPIC              FLAG_C_fPIC)
      # TRY_C_COMPILER_FLAG(-pie               FLAG_C_pie)
      # TRY_C_COMPILER_FLAG(-Wl,-z,relro       FLAG_C_Wl_z_relro)
      # TRY_C_COMPILER_FLAG(-Wl,-z,now         FLAG_C_Wl_z_now)
      # TRY_C_COMPILER_FLAG(-Wl,--as-needed    FLAG_C_Wl_as_needed)
      # TRY_C_COMPILER_FLAG(-Wl,-z,noexecstack FLAG_C_Wl_z_noexecstack)
    ENDIF()
  ENDIF()

  IF(CMAKE_C_COMPILER_ID STREQUAL "Intel")
    TRY_C_COMPILER_FLAG(-fstack-protector  FLAG_C_fstackprotector)
    TRY_C_COMPILER_FLAG(-fPIC              FLAG_C_fPIC)
    TRY_C_COMPILER_FLAG(-pie               FLAG_C_pie)
    TRY_C_COMPILER_FLAG(-Wl,-z,relro       FLAG_C_Wl_z_relro)
    TRY_C_COMPILER_FLAG(-Wl,-z,now         FLAG_C_Wl_z_now)
    TRY_C_COMPILER_FLAG(-Wl,--as-needed    FLAG_C_Wl_as_needed)
    TRY_C_COMPILER_FLAG(-Wl,-z,noexecstack FLAG_C_Wl_z_noexecstack)
  ENDIF()

  IF(MSVC)
    TRY_C_COMPILER_FLAG(/GS FLAG_C_GS)
  ENDIF()
ENDIF()

#==============================================================================
############################################## Compiler: Optimization #########
#==============================================================================
IF(CMAKE_BUILD_TYPE STREQUAL "MinSizeRel" OR
   CMAKE_BUILD_TYPE STREQUAL "Release")
  IF(GCC_OR_CLANG)
    TRY_C_COMPILER_FLAG(-fomit-frame-pointer FLAG_C_fomit_frame_pointer)
    TRY_C_COMPILER_FLAG(-O3 FLAG_C_optimizations)
  ENDIF()

  IF(CMAKE_C_COMPILER_ID STREQUAL "Intel")
    TRY_C_COMPILER_FLAG(-fomit-frame-pointer FLAG_C_fomit_frame_pointer)
    TRY_C_COMPILER_FLAG(-ipo                 FLAG_C_ipo)
  ENDIF()

ENDIF()

#==============================================================================
################################################### Find Dependencies #########
#==============================================================================
FIND_PACKAGE(XDiff REQUIRED)
INCLUDE_DIRECTORIES(${XDIFF_INCLUDE_DIR})

#==============================================================================
################################################## Set Project Source #########
#==============================================================================

IF(GCC_OR_CLANG)
  ADD_DEFINITIONS(-D_GNU_SOURCE)
ENDIF()

IF(MINGW)
  ADD_DEFINITIONS(-D_GNU_SOURCE)
  ADD_DEFINITIONS(-D__USE_MINGW_ANSI_STDIO=1)
  TRY_C_COMPILER_FLAG(-mno-ms-bitfields FLAG_C_nomsbitfields)
ENDIF()

INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/src)

SET(DEMOCOMP_SOURCES
  ${CMAKE_SOURCE_DIR}/src/buf.c
  ${CMAKE_SOURCE_DIR}/src/cmp.c
  ${CMAKE_SOURCE_DIR}/src/delta.c
  ${CMAKE_SOURCE_DIR}/src/comp.c
  ${CMAKE_SOURCE_DIR}/src/main.c
  ${CMAKE_SOURCE_DIR}/src/pbuf.c
  ${CMAKE_SOURCE_DIR}/src/utils.c
)

SET(DEMOCOMP_LIBRARIES "-lm")
SET(DEMOCOMP_LIBRARIES ${DEMOCOMP_LIBRARIES} ${XDIFF_LIBRARIES})

IF(LINUX)
  SET(DEMOCOMP_LIBRARIES ${DEMOCOMP_LIBRARIES} dl)
ENDIF()

ADD_EXECUTABLE(democomp ${DEMOCOMP_SOURCES})
TARGET_LINK_LIBRARIES(democomp ${DEMOCOMP_LIBRARIES})

SET(BIN_DIR "${PREFIX}/bin")
SET(LIB_DIR "${PREFIX}/lib")
SET(INCLUDE_DIR "${PREFIX}/include")
SET(SHARE_DIR "${PREFIX}/share")

INSTALL(
  TARGETS democomp
  RUNTIME DESTINATION ${BIN_DIR}
  LIBRARY DESTINATION ${LIB_DIR}
  ARCHIVE DESTINATION ${LIB_DIR}
)

# vi: ai:ts=2:sw=2:et

