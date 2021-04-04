#
# CMakeLists.txt
#
# cmake required (http://www.cmake.org/)
#

cmake_minimum_required( VERSION 3.16 )

if( POLICY CMP0017 )
  cmake_policy( SET CMP0017 NEW )
endif()

include( CMakeDependentOption )

set( GAME_APP_NAME "" CACHE            STRING "The Application name")
set( GAME_APP_VERSION "" CACHE         STRING "The Application version")
set( GAME_APP_STAGE "" CACHE           STRING "The Application stage")
set( URL_MASTER_SERVER "" CACHE        STRING "The URL for the Master Server")
set( URL_MOTD_SERVER "" CACHE          STRING "The URL for the MOTD Server")
set( URL_AUTHORIZE_SERVER "" CACHE     STRING "The URL for the Authorize Server")
set( URL_AUTOUPDATE_SERVER "" CACHE    STRING "The URL for the Autoupdate Server")

if("${GAME_APP_NAME}" STREQUAL "")
	message(FATAL_ERROR "Please set GAME_APP_NAME first")
endif()

if("${GAME_APP_VERSION}" STREQUAL "")
	message(FATAL_ERROR "Please set GAME_APP_VERSION first")
endif()

if("${GAME_APP_STAGE}" STREQUAL "")
	message(FATAL_ERROR "Please set GAME_APP_STAGE first")
endif()

if("${URL_MASTER_SERVER}" STREQUAL "")
	message(FATAL_ERROR "Please set URL_MASTER_SERVER first")
endif()

if("${URL_MOTD_SERVER}" STREQUAL "")
	message(FATAL_ERROR "Please set URL_MOTD_SERVER first")
endif()

if("${URL_AUTHORIZE_SERVER}" STREQUAL "")
	message(FATAL_ERROR "Please set URL_AUTHORIZE_SERVER first")
endif()

if("${URL_AUTOUPDATE_SERVER}" STREQUAL "")
	message(FATAL_ERROR "Please set URL_AUTOUPDATE_SERVER first")
endif()

project(${GAME_APP_NAME} C CXX)

# This line has to appear before 'PROJECT' in order to be able to disable incremental linking
set( MSVC_INCREMENTAL_DEFAULT ON )

set( CMAKE_BUILD_TYPE Release CACHE STRING "Build configuration (Debug, Release, RelWithDebInfo, MinSizeRel)" )

if( MSVC )
  set( CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} /fp:precise" )
  set( CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} /LARGEADDRESSAWARE" )
  set( CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /LARGEADDRESSAWARE /SUBSYSTEM:CONSOLE" )
  set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}" )
else()
  set( CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -msse4.1 -pipe -Wno-write-strings -Wno-narrowing -fpermissive -Wsizeof-array-argument -I/usr/include/freetype2" )
  set( CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -g" )
  set( CMAKE_C_FLAGS_MINSIZEREL "${CMAKE_C_FLAGS_MINSIZEREL} -DNDEBUG" )
  set( CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DNDEBUG -ffast-math -fomit-frame-pointer -fno-strict-aliasing" )
  set( CMAKE_C_FLAGS_RELWITHDEBINFO "${CMAKE_C_FLAGS_RELWITHDEBINFO} -DNDEBUG -ffast-math -fomit-frame-pointer -fno-strict-aliasing -g" )
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
endif()

set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${CMAKE_C_FLAGS}" )
set( CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} ${CMAKE_C_FLAGS_DEBUG}" )
set( CMAKE_CXX_FLAGS_MINSIZEREL "${CMAKE_CXX_FLAGS_MINSIZEREL} ${CMAKE_C_FLAGS_MINSIZEREL}" )
set( CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} ${CMAKE_C_FLAGS_RELEASE}" )
set( CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS_RELWITHDEBINFO} ${CMAKE_C_FLAGS_RELWITHDEBINFO}" )
set( CMAKE_SHARED_LIBRARY_LINK_CXX_FLAGS "" ) # Need this to get rid of -rdynamic linker flag set by default by cmake
set( CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake" )

# Package info
set( CPACK_PACKAGE_DESCRIPTION_SUMMARY "Application client" )
set( CPACK_PACKAGE_DESCRIPTION_FILE "${CMAKE_CURRENT_SOURCE_DIR}/README.txt" )
set( CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/GPL.txt" )
include( CPack )

set( MOUNT_DIR ${CMAKE_SOURCE_DIR}/../OpenWolf-Engine/src/engine )
set( LIB_DIR ${CMAKE_SOURCE_DIR}/libs )
set( TREMGAME_DIR ${CMAKE_SOURCE_DIR}/src/gameLogics )

message(STATUS "writing ${MOUNT_DIR}/framework/appConfig.hpp")
CONFIGURE_FILE("cmake/appConfig.hpp.in" "${MOUNT_DIR}/framework/appConfig.hpp" @ONLY)

find_package( PkgConfig )

if( MSVC )
	add_definitions( -D_CRT_SECURE_NO_WARNINGS )
endif()

set( CONSOLE "tty" )

# Determine platform
if( MSVC )
	set( PLATFORM "win32" )
	set( CONSOLE "win32" )
elseif( UNIX )
	if( ${CMAKE_SYSTEM_NAME} MATCHES "Darwin" )
		add_definitions( -DMACOS_X -DMACOS_X_ACCELERATION_HACK -D USE_FILE32API -DMACOS_X_CURSOR_HACK )
	endif()
	set( PLATFORM "unix" )
endif()

# Determine arch
if( ${CMAKE_SYSTEM_PROCESSOR} MATCHES "x86_64" )
	set( BUILD_ARCH "AMD64" )
else()
	set( BUILD_ARCH ${CMAKE_SYSTEM_PROCESSOR} )
endif()

######################
# Define Build Files #
######################

set( SHAREDLIST_HEADERS
	${MOUNT_DIR}/qcommon/q_platform.hpp
	${MOUNT_DIR}/qcommon/q_shared.hpp
)

set( SHAREDLIST_SOURCES
	${MOUNT_DIR}/qcommon/q_shared.cpp
	${MOUNT_DIR}/qcommon/q_math.cpp
)

set( GUI_HEADERS 
	${MOUNT_DIR}/framework/appConfig.hpp
	${MOUNT_DIR}/framework/types.hpp
	${MOUNT_DIR}/API/cm_api.hpp
	${MOUNT_DIR}/API/CmdBuffer_api.hpp
	${MOUNT_DIR}/API/CmdSystem_api.hpp
	${MOUNT_DIR}/API/system_api.hpp
	${MOUNT_DIR}/API/clientLAN_api.hpp
	${MOUNT_DIR}/API/clientGUI_api.hpp
	${MOUNT_DIR}/API/renderer_api.hpp
	${MOUNT_DIR}/API/soundSystem_api.hpp
	${MOUNT_DIR}/API/FileSystem_api.hpp
	${MOUNT_DIR}/API/CVarSystem_api.hpp
	${MOUNT_DIR}/GPURenderer/r_types.hpp
	${MOUNT_DIR}/API/bgame_api.hpp
	${MOUNT_DIR}/API/gui_api.hpp
	${MOUNT_DIR}/API/cgame_api.hpp
	${MOUNT_DIR}/API/Parse_api.hpp
	${TREMGAME_DIR}/GUI/gui_local.hpp
	${TREMGAME_DIR}/GUI/gui_shared.hpp
	${TREMGAME_DIR}/GUI/keycodes.hpp
	${TREMGAME_DIR}/GUI/menudef.hpp
	${TREMGAME_DIR}/GUI/gui_precompiled.hpp
)

set( GUI_SOURCES
	${TREMGAME_DIR}/GUI/gui_precompiled.cpp
	${TREMGAME_DIR}/bgame/bgame_misc.cpp
	${TREMGAME_DIR}/bgame/bgame_alloc.cpp
	${TREMGAME_DIR}/bgame/bgame_pmove.cpp
	${TREMGAME_DIR}/bgame/bgame_slidemove.cpp
	${TREMGAME_DIR}/bgame/bgame_voice.cpp
	${TREMGAME_DIR}/GUI/gui_api.cpp
	${TREMGAME_DIR}/GUI/gui_atoms.cpp
	${TREMGAME_DIR}/GUI/gui_gameinfo.cpp
	${TREMGAME_DIR}/GUI/gui_main.cpp
	${TREMGAME_DIR}/GUI/gui_shared.cpp
)

set( SGAMELIST_HEADERS
	${MOUNT_DIR}/framework/appConfig.hpp
	${MOUNT_DIR}/qcommon/qfiles.hpp
	${MOUNT_DIR}/API/cm_api.hpp
	${MOUNT_DIR}/API/serverGame_api.hpp
	${MOUNT_DIR}/API/serverWorld_api.hpp
	${MOUNT_DIR}/API/serverInit_api.hpp
	${MOUNT_DIR}/API/CmdBuffer_api.hpp
	${MOUNT_DIR}/API/CmdSystem_api.hpp
	${MOUNT_DIR}/API/serverDemo_api.hpp
	${MOUNT_DIR}/API/system_api.hpp
	${MOUNT_DIR}/API/soundSystem_api.hpp
	${MOUNT_DIR}/API/FileSystem_api.hpp
	${MOUNT_DIR}/API/CVarSystem_api.hpp
	${MOUNT_DIR}/API/serverMain_api.hpp
	${MOUNT_DIR}/API/Parse_api.hpp
	${TREMGAME_DIR}/sgame/sgame_precompiled.hpp
	${TREMGAME_DIR}/bgame/bgame_local.hpp
	${MOUNT_DIR}/API/bgame_api.hpp
	${TREMGAME_DIR}/sgame/sgame_admin.hpp
	${TREMGAME_DIR}/sgame/sgame_local.hpp
	${MOUNT_DIR}/framework/SurfaceFlags_Tech3.hpp
	${MOUNT_DIR}/API/sgame_api.hpp
	${TREMGAME_DIR}/sgame/tremulous.hpp
	${TREMGAME_DIR}/sgame/sgame_active.hpp
	${TREMGAME_DIR}/sgame/sgame_buildable.hpp
	${TREMGAME_DIR}/sgame/sgame_client.hpp
	${TREMGAME_DIR}/sgame/sgame_cmds.hpp
	${TREMGAME_DIR}/sgame/sgame_combat.hpp
	${TREMGAME_DIR}/sgame/sgame_main.hpp
	${TREMGAME_DIR}/sgame/sgame_maprotation.hpp
	${TREMGAME_DIR}/sgame/sgame_misc.hpp
	${TREMGAME_DIR}/sgame/sgame_missile.hpp
	${TREMGAME_DIR}/sgame/sgame_mover.hpp
	${TREMGAME_DIR}/sgame/sgame_physics.hpp
	${TREMGAME_DIR}/sgame/sgame_ptr.hpp
	${TREMGAME_DIR}/sgame/sgame_session.hpp
	${TREMGAME_DIR}/sgame/sgame_spawn.hpp
	${TREMGAME_DIR}/sgame/sgame_svcmds.hpp
	${TREMGAME_DIR}/sgame/sgame_target.hpp
	${TREMGAME_DIR}/sgame/sgame_team.hpp
	${TREMGAME_DIR}/sgame/sgame_trigger.hpp
	${TREMGAME_DIR}/sgame/sgame_utils.hpp
	${TREMGAME_DIR}/sgame/sgame_weapon.hpp
)

set( SGAMELIST_SOURCES
	${TREMGAME_DIR}/sgame/sgame_precompiled.cpp
	${TREMGAME_DIR}/bgame/bgame_alloc.cpp
	${TREMGAME_DIR}/bgame/bgame_misc.cpp
	${TREMGAME_DIR}/bgame/bgame_pmove.cpp
	${TREMGAME_DIR}/bgame/bgame_slidemove.cpp
	${TREMGAME_DIR}/bgame/bgame_voice.cpp
	${TREMGAME_DIR}/sgame/sgame_active.cpp
	${TREMGAME_DIR}/sgame/sgame_admin.cpp
	${TREMGAME_DIR}/sgame/sgame_api.cpp
	${TREMGAME_DIR}/sgame/sgame_buildable.cpp
	${TREMGAME_DIR}/sgame/sgame_client.cpp
	${TREMGAME_DIR}/sgame/sgame_cmds.cpp
	${TREMGAME_DIR}/sgame/sgame_combat.cpp
	${TREMGAME_DIR}/sgame/sgame_main.cpp
	${TREMGAME_DIR}/sgame/sgame_maprotation.cpp
	${TREMGAME_DIR}/sgame/sgame_misc.cpp
	${TREMGAME_DIR}/sgame/sgame_missile.cpp
	${TREMGAME_DIR}/sgame/sgame_mover.cpp
	${TREMGAME_DIR}/sgame/sgame_physics.cpp
	${TREMGAME_DIR}/sgame/sgame_ptr.cpp
	${TREMGAME_DIR}/sgame/sgame_session.cpp
	${TREMGAME_DIR}/sgame/sgame_spawn.cpp
	${TREMGAME_DIR}/sgame/sgame_svcmds.cpp
	${TREMGAME_DIR}/sgame/sgame_target.cpp
	${TREMGAME_DIR}/sgame/sgame_team.cpp
	${TREMGAME_DIR}/sgame/sgame_trigger.cpp
	${TREMGAME_DIR}/sgame/sgame_utils.cpp
	${TREMGAME_DIR}/sgame/sgame_weapon.cpp
)

set( CGAMELIST_HEADERS
	${MOUNT_DIR}/framework/appConfig.hpp
	${MOUNT_DIR}/framework/types.hpp
	${MOUNT_DIR}/framework/keycodes.hpp
	${MOUNT_DIR}/framework/SurfaceFlags_Tech3.hpp
	${MOUNT_DIR}/API/cm_api.hpp
	${MOUNT_DIR}/API/CmdBuffer_api.hpp
	${MOUNT_DIR}/API/CmdSystem_api.hpp
	${MOUNT_DIR}/API/system_api.hpp
	${MOUNT_DIR}/API/clientGame_api.hpp
	${MOUNT_DIR}/API/clientLAN_api.hpp
	${MOUNT_DIR}/API/clientGUI_api.hpp
	${MOUNT_DIR}/API/renderer_api.hpp
	${MOUNT_DIR}/API/soundSystem_api.hpp
	${MOUNT_DIR}/API/FileSystem_api.hpp
	${MOUNT_DIR}/API/CVarSystem_api.hpp
	${MOUNT_DIR}/API/cgame_api.hpp
	${MOUNT_DIR}/API/Parse_api.hpp
	${TREMGAME_DIR}/GUI/menudef.hpp
	${MOUNT_DIR}/API/bgame_api.hpp
	${TREMGAME_DIR}/GUI/gui_shared.hpp
	${MOUNT_DIR}/GPURenderer/r_types.hpp
	${TREMGAME_DIR}/bgame/bgame_local.hpp
	${TREMGAME_DIR}/cgame/cgame_local.hpp
	${TREMGAME_DIR}/cgame/cgame_precompiled.hpp
	${TREMGAME_DIR}/cgame/cgame_animation.hpp
	${TREMGAME_DIR}/cgame/cgame_animmapobj.hpp
	${TREMGAME_DIR}/cgame/cgame_attachment.hpp
	${TREMGAME_DIR}/cgame/cgame_buildable.hpp
	${TREMGAME_DIR}/cgame/cgame_consolecmds.hpp
	${TREMGAME_DIR}/cgame/cgame_draw.hpp
	${TREMGAME_DIR}/cgame/cgame_drawtools.hpp
	${TREMGAME_DIR}/cgame/cgame_ents.hpp
	${TREMGAME_DIR}/cgame/cgame_event.hpp
	${TREMGAME_DIR}/cgame/cgame_main.hpp
	${TREMGAME_DIR}/cgame/cgame_marks.hpp
	${TREMGAME_DIR}/cgame/cgame_mem.hpp
	${TREMGAME_DIR}/cgame/cgame_particles.hpp
	${TREMGAME_DIR}/cgame/cgame_players.hpp
	${TREMGAME_DIR}/cgame/cgame_playerstate.hpp
	${TREMGAME_DIR}/cgame/cgame_predict.hpp
	${TREMGAME_DIR}/cgame/cgame_ptr.hpp
	${TREMGAME_DIR}/cgame/cgame_scanner.hpp
	${TREMGAME_DIR}/cgame/cgame_servercmds.hpp
	${TREMGAME_DIR}/cgame/cgame_snapshot.hpp
	${TREMGAME_DIR}/cgame/cgame_trails.hpp
	${TREMGAME_DIR}/cgame/cgame_tutorial.hpp
	${TREMGAME_DIR}/cgame/cgame_view.hpp
	${TREMGAME_DIR}/cgame/cgame_weapons.hpp
)

set( CGAMELIST_SOURCES
	${TREMGAME_DIR}/cgame/cgame_precompiled.cpp
	${TREMGAME_DIR}/bgame/bgame_alloc.cpp
	${TREMGAME_DIR}/bgame/bgame_misc.cpp
	${TREMGAME_DIR}/bgame/bgame_pmove.cpp
	${TREMGAME_DIR}/bgame/bgame_voice.cpp
	${TREMGAME_DIR}/bgame/bgame_slidemove.cpp
	${TREMGAME_DIR}/cgame/cgame_animation.cpp
	${TREMGAME_DIR}/cgame/cgame_animmapobj.cpp
	${TREMGAME_DIR}/cgame/cgame_api.cpp
	${TREMGAME_DIR}/cgame/cgame_attachment.cpp
	${TREMGAME_DIR}/cgame/cgame_buildable.cpp
	${TREMGAME_DIR}/cgame/cgame_consolecmds.cpp
	${TREMGAME_DIR}/cgame/cgame_draw.cpp
	${TREMGAME_DIR}/cgame/cgame_drawtools.cpp
	${TREMGAME_DIR}/cgame/cgame_ents.cpp
	${TREMGAME_DIR}/cgame/cgame_event.cpp
	${TREMGAME_DIR}/cgame/cgame_main.cpp
	${TREMGAME_DIR}/cgame/cgame_marks.cpp
	${TREMGAME_DIR}/cgame/cgame_mem.cpp
	${TREMGAME_DIR}/cgame/cgame_particles.cpp
	${TREMGAME_DIR}/cgame/cgame_players.cpp
	${TREMGAME_DIR}/cgame/cgame_playerstate.cpp
	${TREMGAME_DIR}/cgame/cgame_predict.cpp
	${TREMGAME_DIR}/cgame/cgame_ptr.cpp
	${TREMGAME_DIR}/cgame/cgame_scanner.cpp
	${TREMGAME_DIR}/cgame/cgame_servercmds.cpp
	${TREMGAME_DIR}/cgame/cgame_snapshot.cpp
	${TREMGAME_DIR}/cgame/cgame_trails.cpp
	${TREMGAME_DIR}/cgame/cgame_tutorial.cpp
	${TREMGAME_DIR}/cgame/cgame_view.cpp
	${TREMGAME_DIR}/cgame/cgame_weapons.cpp
	${TREMGAME_DIR}/GUI/gui_shared.cpp
)

####################
# End of file list #
####################

if( MSVC )
	add_definitions( -DWINVER=0x501 )
	#set( OS_LIBRARIES m winmm wsock32 gdi32 ole32 ws2_32 psapi Iphlpapi advapi32 shell32 user32 )
	set( OS_LIBRARIES winmm ws2_32 psapi )
	set( EXTRALIBS dinput dsound )
elseif( UNIX )
	if( CMAKE_SYSTEM_NAME STREQUAL "FreeBSD" )
		set( OS_LIBRARIES m z )
	elseif( CMAKE_SYSTEM_NAME STREQUAL "Darwin" )
		set( OS_LIBRARIES dl m z )
		# FIXME This looks wrong.
		set( OS_LIBRARIES ${OS_LIBRARIES} "-framework AGL -framework OpenGL -framework Carbon -framework IOKit" )
	else()
		set( OS_LIBRARIES ${CMAKE_DL_LIBS} m z rt )
	endif()
endif()

###############################
# Build Game/Cgame/Ui DLL/.so #
###############################

add_library( sgame SHARED ${SHAREDLIST_HEADERS} ${SHAREDLIST_SOURCES} ${SGAMELIST_HEADERS} ${SGAMELIST_SOURCES} )
if( MSVC )
	SOURCE_GROUP( Header\ Files\\SharedList                               FILES ${SHAREDLIST_HEADERS} )
	SOURCE_GROUP( Header\ Files\\GameFiles                                FILES ${SGAMELIST_HEADERS} )
	SOURCE_GROUP( Source\ Files\\SharedList                               FILES ${SHAREDLIST_SOURCES} )
	SOURCE_GROUP( Source\ Files\\GameFiles                                FILES ${SGAMELIST_SOURCES} )

	target_link_libraries( sgame winmm.lib )

	SET_TARGET_PROPERTIES( sgame PROPERTIES LINK_FLAGS "/DEF:\"${TREMGAME_DIR}/sgame/sgame.def" )
endif( MSVC )

	target_precompile_headers( sgame PRIVATE "${TREMGAME_DIR}/sgame/sgame_precompiled.hpp" )

TARGET_INCLUDE_DIRECTORIES( sgame PRIVATE ${MOUNT_DIR} ${TREMGAME_DIR} )

if(MSVC)
	target_compile_options(sgame PRIVATE "/MP")
	set_property( TARGET sgame PROPERTY COMPILE_DEFINITIONS _AMD64_ WIN32 _AMD64 _WIN64 __WIN64__ GAMEDLL )
elseif( UNIX )
	set_property( TARGET sgame PROPERTY COMPILE_DEFINITIONS LINUX __amd64__ __linux__ __x86_64__ __LINUX__ GAMEDLL )
endif()

set_target_properties( sgame PROPERTIES
	OUTPUT_NAME "sgame${BUILD_ARCH}"
	PREFIX ""
	LIBRARY_OUTPUT_DIRECTORY "main"
)

add_library( cgame SHARED ${SHAREDLIST_HEADERS} ${SHAREDLIST_SOURCES} ${CGAMELIST_HEADERS} ${CGAMELIST_SOURCES})
if( MSVC )
	SOURCE_GROUP( Header\ Files\\SharedList                               FILES ${SHAREDLIST_HEADERS} )
	SOURCE_GROUP( Header\ Files\\GameFiles                                FILES ${CGAMELIST_HEADERS} )
	SOURCE_GROUP( Source\ Files\\SharedList                               FILES ${SHAREDLIST_SOURCES} )
	SOURCE_GROUP( Source\ Files\\GameFiles                                FILES ${CGAMELIST_SOURCES} )
	
	SET_TARGET_PROPERTIES( cgame PROPERTIES LINK_FLAGS "/DEF:\"${TREMGAME_DIR}/cgame/cgame.def" )
endif( MSVC )

	target_precompile_headers( cgame PRIVATE "${TREMGAME_DIR}/cgame/cgame_precompiled.hpp" )

	TARGET_INCLUDE_DIRECTORIES( cgame PRIVATE ${MOUNT_DIR} ${TREMGAME_DIR} )
	
if(MSVC)
	target_compile_options(cgame PRIVATE "/MP")
	set_property( TARGET cgame PROPERTY COMPILE_DEFINITIONS _AMD64_ NDEBUG _WIN32 WIN32 _AMD64 _WIN64 __WIN64__ CGAMEDLL )
elseif( UNIX )
	set_property( TARGET cgame PROPERTY COMPILE_DEFINITIONS LINUX __amd64__ __linux__ __x86_64__ __LINUX__ CGAMEDLL )
endif()

set_target_properties( cgame PROPERTIES
	OUTPUT_NAME "cgame${BUILD_ARCH}"
	PREFIX ""
	LIBRARY_OUTPUT_DIRECTORY "main"
)

target_link_libraries( cgame )

add_library( gui SHARED ${SHAREDLIST_HEADERS} ${SHAREDLIST_SOURCES} ${GUI_HEADERS} ${GUI_SOURCES})
if( MSVC )
	SOURCE_GROUP( Header\ Files\\SharedList                               FILES ${SHAREDLIST_HEADERS} )
	SOURCE_GROUP( Header\ Files\\GameFiles                                FILES ${GUI_HEADERS} )
	SOURCE_GROUP( Source\ Files\\SharedList                               FILES ${SHAREDLIST_SOURCES} )
	SOURCE_GROUP( Source\ Files\\GameFiles                                FILES ${GUI_SOURCES} )
	
	SET_TARGET_PROPERTIES( gui PROPERTIES LINK_FLAGS "/DEF:\"${TREMGAME_DIR}/GUI/gui.def" )
endif( MSVC )

	target_precompile_headers( gui PRIVATE "${TREMGAME_DIR}/GUI/gui_precompiled.hpp" )

	TARGET_INCLUDE_DIRECTORIES( gui PRIVATE ${MOUNT_DIR} ${TREMGAME_DIR} )
	
if(MSVC)
	target_compile_options(gui PRIVATE "/MP")
	set_property( TARGET gui PROPERTY COMPILE_DEFINITIONS _AMD64_ NDEBUG _WIN32 WIN32 _AMD64 _WIN64 __WIN64__ GUI )
elseif( UNIX )
	set_property( TARGET gui PROPERTY COMPILE_DEFINITIONS LINUX __amd64__ __linux__ __x86_64__ __LINUX__ GUI )
endif()

set_target_properties( gui PROPERTIES
	OUTPUT_NAME "gui${BUILD_ARCH}"
	PREFIX ""
	LIBRARY_OUTPUT_DIRECTORY "main"
)

target_link_libraries( gui )

install( TARGETS sgame cgame gui
	RUNTIME DESTINATION bin
	LIBRARY DESTINATION lib
	ARCHIVE DESTINATION lib
)  
