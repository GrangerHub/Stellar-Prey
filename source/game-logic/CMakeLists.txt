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

string( REPLACE " " "" GAME_APP_NAME_UPPER ${GAME_APP_NAME} )
string( TOLOWER ${GAME_APP_NAME} GAME_APP_NAME_LOWER )

if("${GAME_APP_NAME}" STREQUAL "")
	message(FATAL_ERROR "Please set GAME_APP_NAME first")
endif()

if("${GAME_APP_VERSION}" STREQUAL "")
	message(FATAL_ERROR "Please set GAME_APP_VERSION first")
endif()

if("${GAME_APP_STAGE}" STREQUAL "")
	message(FATAL_ERROR "Please set GAME_APP_STAGE first")
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

message(STATUS "writing ${MOUNT_DIR}/framework/appConfig.h")
CONFIGURE_FILE("cmake/appConfig.h.in" "${MOUNT_DIR}/framework/appConfig.h" @ONLY)

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
	${MOUNT_DIR}/qcommon/q_platform.h
	${MOUNT_DIR}/qcommon/q_shared.h
)

set( SHAREDLIST_SOURCES
	${MOUNT_DIR}/qcommon/q_shared.cpp
	${MOUNT_DIR}/qcommon/q_math.cpp
)

set( GUI_HEADERS 
	${MOUNT_DIR}/framework/appConfig.h
	${MOUNT_DIR}/framework/types.h
	${MOUNT_DIR}/API/cm_api.h
	${MOUNT_DIR}/API/CmdBuffer_api.h
	${MOUNT_DIR}/API/CmdSystem_api.h
	${MOUNT_DIR}/API/system_api.h
	${MOUNT_DIR}/API/clientLAN_api.h
	${MOUNT_DIR}/API/clientGUI_api.h
	${MOUNT_DIR}/API/renderer_api.h
	${MOUNT_DIR}/API/sound_api.h
	${MOUNT_DIR}/API/FileSystem_api.h
	${MOUNT_DIR}/API/CVarSystem_api.h
	${MOUNT_DIR}/GPURenderer/r_types.h
	${MOUNT_DIR}/API/bgame_api.h
	${MOUNT_DIR}/API/gui_api.h
	${MOUNT_DIR}/API/cgame_api.h
	${MOUNT_DIR}/API/Parse_api.h
	${TREMGAME_DIR}/GUI/gui_local.h
	${TREMGAME_DIR}/GUI/gui_shared.h
	${TREMGAME_DIR}/GUI/keycodes.h
	${TREMGAME_DIR}/GUI/menudef.h
	${TREMGAME_DIR}/GUI/gui_precompiled.h
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
	${MOUNT_DIR}/framework/appConfig.h
	${MOUNT_DIR}/qcommon/qfiles.h
	${MOUNT_DIR}/API/cm_api.h
	${MOUNT_DIR}/API/serverGame_api.h
	${MOUNT_DIR}/API/serverWorld_api.h
	${MOUNT_DIR}/API/serverInit_api.h
	${MOUNT_DIR}/API/CmdBuffer_api.h
	${MOUNT_DIR}/API/CmdSystem_api.h
	${MOUNT_DIR}/API/serverDemo_api.h
	${MOUNT_DIR}/API/system_api.h
	${MOUNT_DIR}/API/sound_api.h
	${MOUNT_DIR}/API/FileSystem_api.h
	${MOUNT_DIR}/API/CVarSystem_api.h
	${MOUNT_DIR}/API/serverMain_api.h
	${MOUNT_DIR}/API/Parse_api.h
	${TREMGAME_DIR}/sgame/sgame_precompiled.h
	${TREMGAME_DIR}/bgame/bgame_local.h
	${MOUNT_DIR}/API/bgame_api.h
	${TREMGAME_DIR}/sgame/sgame_admin.h
	${TREMGAME_DIR}/sgame/sgame_local.h
	${MOUNT_DIR}/framework/SurfaceFlags_Tech3.h
	${MOUNT_DIR}/API/sgame_api.h
	${TREMGAME_DIR}/sgame/tremulous.h
	${TREMGAME_DIR}/sgame/sgame_active.h
	${TREMGAME_DIR}/sgame/sgame_buildable.h
	${TREMGAME_DIR}/sgame/sgame_client.h
	${TREMGAME_DIR}/sgame/sgame_cmds.h
	${TREMGAME_DIR}/sgame/sgame_combat.h
	${TREMGAME_DIR}/sgame/sgame_main.h
	${TREMGAME_DIR}/sgame/sgame_maprotation.h
	${TREMGAME_DIR}/sgame/sgame_misc.h
	${TREMGAME_DIR}/sgame/sgame_missile.h
	${TREMGAME_DIR}/sgame/sgame_mover.h
	${TREMGAME_DIR}/sgame/sgame_physics.h
	${TREMGAME_DIR}/sgame/sgame_bulletphysics.h
	${TREMGAME_DIR}/sgame/sgame_ptr.h
	${TREMGAME_DIR}/sgame/sgame_session.h
	${TREMGAME_DIR}/sgame/sgame_spawn.h
	${TREMGAME_DIR}/sgame/sgame_svcmds.h
	${TREMGAME_DIR}/sgame/sgame_target.h
	${TREMGAME_DIR}/sgame/sgame_team.h
	${TREMGAME_DIR}/sgame/sgame_trigger.h
	${TREMGAME_DIR}/sgame/sgame_utils.h
	${TREMGAME_DIR}/sgame/sgame_weapon.h
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
	${TREMGAME_DIR}/sgame/sgame_bulletphysics.cpp
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
	${MOUNT_DIR}/framework/appConfig.h
	${MOUNT_DIR}/framework/types.h
	${MOUNT_DIR}/framework/keycodes.h
	${MOUNT_DIR}/framework/SurfaceFlags_Tech3.h
	${MOUNT_DIR}/API/cm_api.h
	${MOUNT_DIR}/API/CmdBuffer_api.h
	${MOUNT_DIR}/API/CmdSystem_api.h
	${MOUNT_DIR}/API/system_api.h
	${MOUNT_DIR}/API/clientGame_api.h
	${MOUNT_DIR}/API/clientLAN_api.h
	${MOUNT_DIR}/API/clientGUI_api.h
	${MOUNT_DIR}/API/renderer_api.h
	${MOUNT_DIR}/API/sound_api.h
	${MOUNT_DIR}/API/FileSystem_api.h
	${MOUNT_DIR}/API/CVarSystem_api.h
	${MOUNT_DIR}/API/cgame_api.h
	${MOUNT_DIR}/API/Parse_api.h
	${TREMGAME_DIR}/GUI/menudef.h
	${MOUNT_DIR}/API/bgame_api.h
	${TREMGAME_DIR}/GUI/gui_shared.h
	${MOUNT_DIR}/GPURenderer/r_types.h
	${TREMGAME_DIR}/bgame/bgame_local.h
	${TREMGAME_DIR}/cgame/cgame_local.h
	${TREMGAME_DIR}/cgame/cgame_precompiled.h
	${TREMGAME_DIR}/cgame/cgame_animation.h
	${TREMGAME_DIR}/cgame/cgame_animmapobj.h
	${TREMGAME_DIR}/cgame/cgame_attachment.h
	${TREMGAME_DIR}/cgame/cgame_buildable.h
	${TREMGAME_DIR}/cgame/cgame_consolecmds.h
	${TREMGAME_DIR}/cgame/cgame_draw.h
	${TREMGAME_DIR}/cgame/cgame_drawtools.h
	${TREMGAME_DIR}/cgame/cgame_ents.h
	${TREMGAME_DIR}/cgame/cgame_event.h
	${TREMGAME_DIR}/cgame/cgame_main.h
	${TREMGAME_DIR}/cgame/cgame_marks.h
	${TREMGAME_DIR}/cgame/cgame_mem.h
	${TREMGAME_DIR}/cgame/cgame_particles.h
	${TREMGAME_DIR}/cgame/cgame_players.h
	${TREMGAME_DIR}/cgame/cgame_playerstate.h
	${TREMGAME_DIR}/cgame/cgame_predict.h
	${TREMGAME_DIR}/cgame/cgame_ptr.h
	${TREMGAME_DIR}/cgame/cgame_scanner.h
	${TREMGAME_DIR}/cgame/cgame_servercmds.h
	${TREMGAME_DIR}/cgame/cgame_snapshot.h
	${TREMGAME_DIR}/cgame/cgame_trails.h
	${TREMGAME_DIR}/cgame/cgame_tutorial.h
	${TREMGAME_DIR}/cgame/cgame_view.h
	${TREMGAME_DIR}/cgame/cgame_weapons.h
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

	target_precompile_headers( sgame PRIVATE "${TREMGAME_DIR}/sgame/sgame_precompiled.h" )

find_package( Bullet REQUIRED )

TARGET_INCLUDE_DIRECTORIES( sgame PRIVATE ${MOUNT_DIR} ${TREMGAME_DIR} ${BULLET_INCLUDE_DIRS} ${BULLET_INCLUDE_DIR} ${BULLET_INCLUDE_COLLISION_DIR} ${BULLET_INCLUDE_DYNAMICS_DIR} ${BULLET_INCLUDE_SOFTBODY_DIR} ${BULLET_INCLUDE_LINEARMATH_DIR} ${BULLET_INCLUDE_FILELOADER_DIR} ${BULLET_INCLUDE_WORLDIMPORTER_DIR} )

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

if( MSVC )
target_link_libraries( sgame ${BULLET_LIBRARIES} )
else()
target_link_libraries( sgame ${BULLET_LIBRARIES} ${BULLET_COLLISION_LIBRARY} ${BULLET_DYNAMICS_LIBRARY} ${BULLET_LINEARMATH_LIBRARY} )
endif()

add_library( cgame SHARED ${SHAREDLIST_HEADERS} ${SHAREDLIST_SOURCES} ${CGAMELIST_HEADERS} ${CGAMELIST_SOURCES})
if( MSVC )
	SOURCE_GROUP( Header\ Files\\SharedList                               FILES ${SHAREDLIST_HEADERS} )
	SOURCE_GROUP( Header\ Files\\GameFiles                                FILES ${CGAMELIST_HEADERS} )
	SOURCE_GROUP( Source\ Files\\SharedList                               FILES ${SHAREDLIST_SOURCES} )
	SOURCE_GROUP( Source\ Files\\GameFiles                                FILES ${CGAMELIST_SOURCES} )
	
	SET_TARGET_PROPERTIES( cgame PROPERTIES LINK_FLAGS "/DEF:\"${TREMGAME_DIR}/cgame/cgame.def" )
endif( MSVC )

	target_precompile_headers( cgame PRIVATE "${TREMGAME_DIR}/cgame/cgame_precompiled.h" )

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

	target_precompile_headers( gui PRIVATE "${TREMGAME_DIR}/GUI/gui_precompiled.h" )

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
