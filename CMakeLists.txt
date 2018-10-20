cmake_minimum_required (VERSION 2.6)
project (xb-lua C)

include(CheckLibraryExists)
include(CheckTypeSize)
include(CheckCSourceCompiles)
include(CheckIncludeFiles)
include(CheckFunctionExists)
include(CheckSymbolExists)

include_directories("${CMAKE_CURRENT_SOURCE_DIR}/lua")
include_directories("${CMAKE_CURRENT_BINARY_DIR}/lua")

CHECK_LIBRARY_EXISTS(rt clock_gettime "" HAVE_LIB_RT)
CHECK_LIBRARY_EXISTS(pthread pthread_create "" HAVE_LIB_PTHREAD)
CHECK_LIBRARY_EXISTS(m sin "" HAVE_LIB_M)
#
if (HAVE_LIB_RT)
    set(EXTRA_LIBS ${EXTRA_LIBS} rt)
endif (HAVE_LIB_RT)
if (HAVE_LIB_PTHREAD)
    set(EXTRA_LIBS ${EXTRA_LIBS} pthread)
endif (HAVE_LIB_PTHREAD)
if (HAVE_LIB_M)
    set(EXTRA_LIBS ${EXTRA_LIBS} m)
endif (HAVE_LIB_M)

CHECK_LIBRARY_EXISTS(dl dlopen "" HAVE_LIB_DL)
CHECK_LIBRARY_EXISTS(readline rl_instream "" HAVE_LIB_RL)
CHECK_LIBRARY_EXISTS(history history_search "" HAVE_LIB_HISTORY)
CHECK_LIBRARY_EXISTS(ncurses getcurx "" HAVE_LIB_NCURSES)
#
if (HAVE_LIB_DL)
    set(GOODI_LIBS ${GOODI_LIBS} dl)
endif (HAVE_LIB_DL)
if (HAVE_LIB_RL)
    set(GOODI_LIBS ${GOODI_LIBS} readline)
endif (HAVE_LIB_RL)
if (HAVE_LIB_HISTORY)
    set(GOODI_LIBS ${GOODI_LIBS} history)
endif (HAVE_LIB_HISTORY)
if (HAVE_LIB_NCURSES)
    set(GOODI_LIBS ${GOODI_LIBS} ncurses)
endif (HAVE_LIB_NCURSES)

message (STATUS "Checking for (p)Threads")
find_package(Threads)

# Functions that might be missing and need worka-arounding
# Both of these are needed for either of the test to work (strange...)
set(CMAKE_REQUIRED_INCLUDES pthread.h)
set(CMAKE_REQUIRED_LIBRARIES pthread)

CHECK_FUNCTION_EXISTS("pthread_cancel"  HAVE_PTHREAD_CANCEL_F)
CHECK_SYMBOL_EXISTS("pthread_cancel" "pthread.h" HAVE_PTHREAD_CANCEL_S)

CHECK_FUNCTION_EXISTS("pthread_create"  HAVE_PTHREAD_CREATE_F)
CHECK_SYMBOL_EXISTS("pthread_create" "pthread.h" HAVE_PTHREAD_CREATE_S)

################################################################################
# Configuration options
################################################################################
# Normal project options (src defines)
#-------------------------------------------------------------------------------

#-------------------------------------------------------------------------------
# X buildtool options (gcc)
#-------------------------------------------------------------------------------
set(SYSROOT
	${CMAKE_SYSROOT}
	CACHE STRING
	"System path (--system=)")

# Check built-in preprocessor for tool-chain by run a test-compile but
# without a header file.
# Method is independent of CMake version and cmake x-dev tools plugin.
# Note that this method is not fool-proof if x-tools change after first
# cmake configuration Method is independent of CMake version and cmake x-dev
# tools plugin.
CHECK_SYMBOL_EXISTS(__ANDROID__ "" HAVE_CC_TARGET_CAN_ANDROID)

if (HAVE_CC_TARGET_CAN_ANDROID)
	message( "** Compiler can build for Android" )
	set(XB-LUA_DFLT_AF_UNIX_DIR "/data/local/tmp")
else()
	set(XB-LUA_DFLT_AF_UNIX_DIR "/tmp")
endif()

set(XB-LUA_AF_UNIX_DIR
	${XB-LUA_DFLT_AF_UNIX_DIR}
	CACHE STRING
	"Domain-sockets directory")


### Lua examples (tbd) ########################
#option(BUILD_XB-LUA_EXAMPLES
#	"Build test/example-program(s)" ON)

#-------------------------------------------------------------------------------
# System (project supported) options

#-------------------------------------------------------------------------------
# Configuration options (optional)
#-------------------------------------------------------------------------------
#set(XB-LUA_BUFF_SZ
#	"${XB-LUA_BUFF_SZ}0x400"
#	CACHE STRING
#	"Buffer sizes for all IPC interacton")

#-------------------------------------------------------------------------------
# Build-tool options
#-------------------------------------------------------------------------------
set(CMAKE_EXTRA_C_FLAGS
	"${CMAKE_EXTRA_C_FLAGS}"
	CACHE STRING
	"Compiler options appended to CMAKE_C_FLAGS")

if (NOT GOODI_LIBS STREQUAL "")
	# Offer option if there is a chance it would build
	option(ENABLE_LINUX_GOODIES
		"Build with linux goodies" ON)
else ()
	SET(ENABLE_LINUX_GOODIES OFF)
endif ()

if (ENABLE_LINUX_GOODIES)
	SET(CMAKE_EXTRA_C_FLAGS "${CMAKE_EXTRA_C_FLAGS} -DLUA_USE_LINUX")
endif ()

#-------------------------------------------------------------------------------
# X build-tool options (this project's x-tool handling)
#-------------------------------------------------------------------------------
if (NOT SYSROOT STREQUAL "")
	message( "** INFO: SYSROOT was either set or defaulted from toolchain file" )
	set(CMAKE_SYSROOT "${SYSROOT}")
	set(CMAKE_EXTRA_C_FLAGS "${CMAKE_EXTRA_C_FLAGS} --sysroot=${CMAKE_SYSROOT}")
endif (NOT SYSROOT STREQUAL "")

set(CMAKE_C_FLAGS "${CMAKE_EXTRA_C_FLAGS} -no-integrated-cpp -Wno-unused-function -Wall")
set(CMAKE_C_FLAGS_DEBUG "${CMAKE_EXTRA_C_FLAGS} -g3 -ggdb3 -O0")
set(CMAKE_C_FLAGS_RELEASE "${CMAKE_EXTRA_C_FLAGS} -no-integrated-cpp -Wno-unused-function -O2 -Wall")

################################################################################
# Libraries (src binding)
################################################################################

################################################################################
# Libraries (system installed)
################################################################################
#set (EXTRA_LIBS ${EXTRA_LIBS} sotcptap)
#set (EXTRA_LIBS ${EXTRA_LIBS} log)

################################################################################
# Project
################################################################################
# The version number.
set (XB-LUA_VERSION_MAJOR 3)
set (XB-LUA_VERSION_MINOR 5)
set (XB-LUA_VERSION_PATCH 5)

# configure a header file to pass some of the CMake settings
# to the source code
configure_file (
	"${PROJECT_SOURCE_DIR}/xb_lua_config.h.in"
	"${PROJECT_BINARY_DIR}/xb_lua_config.h"
)

# add the binary tree to the search path for include files
# so that we will find Config.h
include_directories("${PROJECT_BINARY_DIR}")


### core library #############################
set(XB-LUA_CORE_O
	lua/lapi.c
	lua/lcode.c
	lua/lctype.c
	lua/ldebug.c
	lua/ldo.c
	lua/ldump.c
	lua/lfunc.c
	lua/lgc.c
	lua/llex.c
	lua/lmem.c
	lua/lobject.c
	lua/lopcodes.c
	lua/lparser.c
	lua/lstate.c
	lua/lstring.c
	lua/ltable.c
	lua/ltm.c
	lua/lundump.c
	lua/lvm.c
	lua/lzio.c
	lua/ltests.c
)
add_library(libxb_core_lua SHARED ${XB-LUA_CORE_O})
target_link_libraries (libxb_core_lua ${EXTRA_LIBS})

add_library(libxb_core_lua_static STATIC ${XB-LUA_CORE_O})
target_link_libraries (libxb_core_lua_static ${EXTRA_LIBS})

set_target_properties(
  libxb_core_lua PROPERTIES PUBLIC_HEADER
  "lua/lua.h")

### aux library #############################
set(XB-LUA_AUX_O
	lua/lauxlib.c
)
add_library(libxb_aux_lua SHARED ${XB-LUA_AUX_O})
target_link_libraries (libxb_aux_lua ${EXTRA_LIBS})

add_library(libxb_aux_lua_static STATIC ${XB-LUA_AUX_O})
target_link_libraries (libxb_aux_lua_static ${EXTRA_LIBS})

set_target_properties(
  libxb_aux_lua PROPERTIES PUBLIC_HEADER
  "lua/lauxlib.h")

### "lualib" library #############################
set(XB-LUA_LIB_O
	lua/lbaselib.c
	lua/ldblib.c
	lua/liolib.c
	lua/lmathlib.c
	lua/loslib.c
	lua/ltablib.c
	lua/lstrlib.c
	lua/lutf8lib.c
	lua/lbitlib.c
	lua/loadlib.c
	lua/lcorolib.c
	lua/linit.c
)
add_library(libxb_lualib SHARED ${XB-LUA_LIB_O})
target_link_libraries (libxb_lualib ${EXTRA_LIBS})

add_library(libxb_lualib_static STATIC ${XB-LUA_LIB_O})
target_link_libraries (libxb_lualib_static ${EXTRA_LIBS})

set_target_properties(
  libxb_lualib PROPERTIES PUBLIC_HEADER
  "lua/lualib.h")


### Lua executable #############################
set(XB-LUA_SOURCE
	lua/lua.c
)
add_executable(xb_lua ${XB-LUA_SOURCE})
target_link_libraries (xb_lua
	libxb_core_lua
	libxb_aux_lua
	libxb_lualib
	${EXTRA_LIBS}
	${GOODI_LIBS}
)

### Lua examples (tbd) ########################
#if (BUILD_XB-LUA_EXAMPLES)
#	add_subdirectory (examples)
#endif (BUILD_XB-LUA_EXAMPLES)

#------------------------------------------------------

# Installation
# Shells (ignore for now)
#install(PROGRAMS ${PROJECT_BINARY_DIR}/somescript.sh DESTINATION ${CMAKE_INSTALL_PREFIX}/bin)


# Build-targets
set(XB_LUA_TARGETS
	libxb_core_lua
	libxb_core_lua_static
	libxb_aux_lua
	libxb_aux_lua_static
	libxb_lualib
	libxb_lualib_static
	xb_lua
)

# Targets:
install(TARGETS       ${XB_LUA_TARGETS}
        RUNTIME       DESTINATION ${CMAKE_INSTALL_PREFIX}/bin
        LIBRARY       DESTINATION ${CMAKE_INSTALL_PREFIX}/lib
        ARCHIVE       DESTINATION ${CMAKE_INSTALL_PREFIX}/lib
        PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_PREFIX}/include/${PROJECT_NAME})

# Install package generation
# --------------------------
SET(CPACK_PACKAGE_CONTACT "Michael Ambrus")
SET(CPACK_PACKAGE_VERSION_MAJOR ${XB-LUA_VERSION_MAJOR})
SET(CPACK_PACKAGE_VERSION_MINOR ${XB-LUA_VERSION_MINOR})
SET(CPACK_PACKAGE_VERSION_PATCH ${XB-LUA_VERSION_PATCH})
SET(CPACK_PACKAGE_DESCRIPTION_SUMMARY
	"XB-LUA: lua & luac for x-band")

set(SPECIFIED_CPACK
	${SPECIFIED_CPACK}
	CACHE STRING
	"Specify cpack generator: [NSIS|ZIP|DEB|TGZ|RPM]")

if (NOT SPECIFIED_CPACK STREQUAL "")
	SET(CPACK_GENERATOR ${SPECIFIED_CPACK})
endif ()

INCLUDE(CPack)

