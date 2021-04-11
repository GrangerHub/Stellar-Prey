////////////////////////////////////////////////////////////////////////////////////////
// Copyright(C) 2011 - 2021 Dusan Jocic <dusanjocic@msn.com>
//
// This file is part of OpenWolf.
//
// OpenWolf is free software; you can redistribute it
// and / or modify it under the terms of the GNU General Public License as
// published by the Free Software Foundation; either version 3 of the License,
// or (at your option) any later version.
//
// OpenWolf is distributed in the hope that it will be
// useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with OpenWolf; if not, write to the Free Software
// Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110 - 1301  USA
//
// -------------------------------------------------------------------------------------
// File name:   sgame_api.cpp
// Created:
// Compilers:   Microsoft (R) C/C++ Optimizing Compiler Version 19.26.28806 for x64,
//              gcc (Ubuntu 9.3.0-10ubuntu2) 9.3.0
// Description:
// -------------------------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////////////

#include <sgame/sgame_precompiled.hpp>

gameImports_t* imports;
idSoundSystem* soundSystem;
idCollisionModelManager* collisionModelManager;
idFileSystem* fileSystem;
idCVarSystem* cvarSystem;
idServerGameSystem* serverGameSystem;
idServerWorldSystem* serverWorldSystem;
idServerInitSystem* serverInitSystem;
idServerMainSystem* serverMainSystem;
idCmdBufferSystem* cmdBufferSystem;
idCmdSystem* cmdSystem;
idSystem* idsystem;

#ifdef GAMEDLL
typedef struct gclient_s gclient_t;
typedef struct gentity_s gentity_t;
#else
#define gentity_t sharedEntity_t
#endif

#ifdef __LINUX__
extern "C" idSGame* gameEntry( gameImports_t* gimports )
#else
Q_EXPORT idSGame* gameEntry( gameImports_t* gimports )
#endif
{
    imports = gimports;
    
    soundSystem = imports->soundSystem;
    collisionModelManager = imports->collisionModelManager;
    fileSystem = imports->fileSystem;
    cvarSystem = imports->cvarSystem;
    serverGameSystem = imports->serverGameSystem;
    serverWorldSystem = imports->serverWorldSystem;
    serverInitSystem = imports->serverInitSystem;
    serverMainSystem = imports->serverMainSystem;
    cmdBufferSystem = imports->cmdBufferSystem;
    cmdSystem = imports->cmdSystem;
    idsystem = imports->idsystem;
    
    return sgame;
}

#define PASSFLOAT(x) x

void trap_Print( pointer fmt )
{
    imports->Printf( fmt );
}

void trap_Error( pointer fmt )
{
    imports->Error( ERR_DROP, fmt );
}

sint trap_Milliseconds( void )
{
    return imports->idsystem->Milliseconds();
}

void trap_Cvar_Register( vmConvar_t* cvar, pointer var_name, pointer value, sint flags, pointer description )
{
    imports->cvarSystem->Register( cvar, var_name, value, flags, description );
}

void trap_Cvar_Update( vmConvar_t* cvar )
{
    imports->cvarSystem->Update( cvar );
}

void trap_Cvar_Set( pointer var_name, pointer value )
{
    imports->cvarSystem->Set( var_name, value );
}

sint trap_Cvar_VariableIntegerValue( pointer var_name )
{
    return imports->cvarSystem->VariableIntegerValue( var_name );
}

void trap_Cvar_VariableStringBuffer( pointer var_name, valueType* buffer, uint64 bufsize )
{
    imports->cvarSystem->VariableStringBuffer( var_name, buffer, bufsize );
}

void trap_Cvar_LatchedVariableStringBuffer( pointer var_name, valueType* buffer, uint64 bufsize )
{
    imports->cvarSystem->VariableStringBuffer( var_name, buffer, bufsize );
}

sint trap_Argc( void )
{
    return imports->cmdSystem->Argc();
}

void trap_Argv( sint n, valueType* buffer, sint bufferLength )
{
    imports->cmdSystem->ArgvBuffer( n, buffer, bufferLength );
}

void trap_SendConsoleCommand( sint exec_when, pointer text )
{
    imports->cmdBufferSystem->ExecuteText( exec_when, text );
}

sint trap_FS_FOpenFile( pointer qpath, fileHandle_t* f, fsMode_t mode )
{
    return imports->fileSystem->FOpenFileByMode( qpath, f, mode );
}

void trap_FS_Read( void* buffer, sint len, fileHandle_t f )
{
    imports->fileSystem->Read( buffer, len, f );
}

sint trap_FS_Write( const void* buffer, sint len, fileHandle_t f )
{
    return imports->fileSystem->Write( buffer, len, f );
}

sint trap_FS_Rename( pointer from, pointer to )
{
    imports->fileSystem->Rename( from, to );
    return 0;
}

void trap_FS_FCloseFile( fileHandle_t f )
{
    imports->fileSystem->FCloseFile( f );
}

sint trap_FS_GetFileList( pointer path, pointer extension, valueType* listbuf, sint bufsize )
{
    return imports->fileSystem->GetFileList( path, extension, listbuf, bufsize );
}

void trap_LocateGameData( gentity_t* gEnts, uint64 numGEntities, sint sizeofGEntity_t, playerState_t* clients, uint64 sizeofGClient )
{
    imports->serverGameSystem->LocateGameData( gEnts, numGEntities, sizeofGEntity_t, clients, sizeofGClient );
}

void trap_DropClient( sint clientNum, pointer reason, sint length )
{
    imports->serverGameSystem->GameDropClient( clientNum, reason, length );
}

void trap_SendServerCommand( sint clientNum, pointer text )
{
    if( strlen( text ) > 1022 )
    {
        idSGameMain::LogPrintf( "%s: trap_SendServerCommand( %d, ... ) length exceeds 1022.\n", PRODUCT_VERSION, clientNum );
        idSGameMain::LogPrintf( "%s: text [%.950s]... truncated\n", PRODUCT_VERSION, text );
        return;
    }
    
    imports->serverGameSystem->GameSendServerCommand( clientNum, text );
}

void trap_LinkEntity( gentity_t* ent )
{
    imports->serverWorldSystem->LinkEntity( ent );
}

void trap_UnlinkEntity( gentity_t* ent )
{
    imports->serverWorldSystem->UnlinkEntity( ent );
}

sint trap_EntitiesInBox( const vec3_t mins, const vec3_t maxs, sint* list, sint maxcount )
{
    return imports->serverWorldSystem->AreaEntities( mins, maxs, list, maxcount );
}

bool trap_EntityContact( const vec3_t mins, const vec3_t maxs, const gentity_t* ent )
{
    return imports->serverGameSystem->EntityContact( mins, maxs, ( sharedEntity_t* )ent, TT_AABB );
}

bool trap_EntityContactCapsule( const vec3_t mins, const vec3_t maxs, const gentity_t* ent )
{
    return imports->serverGameSystem->EntityContact( mins, maxs, ( sharedEntity_t* )ent, TT_CAPSULE );
}

void trap_Trace( trace_t* results, const vec3_t start, const vec3_t mins, const vec3_t maxs, const vec3_t end, sint passEntityNum, sint contentmask )
{
    imports->serverWorldSystem->Trace( results, start, mins, maxs, end, passEntityNum, contentmask, TT_AABB );
}

void trap_TraceNoEnts( trace_t* results, const vec3_t start, const vec3_t mins, const vec3_t maxs, const vec3_t end, sint passEntityNum, sint contentmask )
{
    imports->serverWorldSystem->Trace( results, start, mins, maxs, end, -2, contentmask, TT_AABB );
}

void trap_TraceCapsule( trace_t* results, const vec3_t start, const vec3_t mins, const vec3_t maxs, const vec3_t end, sint passEntityNum, sint contentmask )
{
    imports->serverWorldSystem->Trace( results, start, mins, maxs, end, passEntityNum, contentmask, TT_CAPSULE );
}

void trap_TraceCapsuleNoEnts( trace_t* results, const vec3_t start, const vec3_t mins, const vec3_t maxs, const vec3_t end, sint passEntityNum, sint contentmask )
{
    imports->serverWorldSystem->Trace( results, start, mins, maxs, end, -2, contentmask, TT_CAPSULE );
}

sint trap_PointContents( const vec3_t point, sint passEntityNum )
{
    return imports->serverWorldSystem->PointContents( point, passEntityNum );
}

void trap_SetBrushModel( gentity_t* ent, pointer name )
{
    imports->serverGameSystem->SetBrushModel( ent, name );
}

bool trap_InPVS( const vec3_t p1, const vec3_t p2 )
{
    return imports->serverGameSystem->inPVS( p1, p2 );
}

bool trap_InPVSIgnorePortals( const vec3_t p1, const vec3_t p2 )
{
    return imports->serverGameSystem->inPVSIgnorePortals( p1, p2 );
}

void trap_SetConfigstring( sint num, pointer string )
{
    imports->serverInitSystem->SetConfigstring( num, string );
}

void trap_GetConfigstring( sint num, valueType* buffer, uint64 bufferSize )
{
    imports->serverInitSystem->GetConfigstring( num, buffer, bufferSize );
}

void trap_SetConfigstringRestrictions( sint num, const clientList_t* clientList )
{
    imports->serverInitSystem->SetConfigstringRestrictions( num, clientList );
}

void trap_SetUserinfo( sint num, pointer buffer )
{
    imports->serverInitSystem->SetUserinfo( num, buffer );
}

void trap_GetUserinfo( sint num, valueType* buffer, uint64 bufferSize )
{
    imports->serverInitSystem->GetUserinfo( num, buffer, bufferSize );
}

void trap_GetServerinfo( valueType* buffer, uint64 bufferSize )
{
    imports->serverGameSystem->GetServerinfo( buffer, bufferSize );
}

void trap_AdjustAreaPortalState( gentity_t* ent, bool open )
{
    imports->serverGameSystem->AdjustAreaPortalState( ent, open );
}

bool trap_AreasConnected( sint area1, sint area2 )
{
    return imports->collisionModelManager->AreasConnected( area1, area2 );
}

void trap_UpdateSharedConfig( uint port, pointer rconpass )
{
    imports->serverGameSystem->UpdateSharedConfig( port, rconpass );
}

sint trap_S_SoundDuration( sfxHandle_t handle )
{
    return imports->soundSystem->SoundDuration( handle );
}

sfxHandle_t trap_S_RegisterSound( pointer sample )
{
    return soundSystem->RegisterSound( sample, false );
}

void trap_GetUsercmd( sint clientNum, usercmd_t* cmd )
{
    imports->serverGameSystem->GetUsercmd( clientNum, cmd );
}

bool trap_GetEntityToken( valueType* buffer, uint64 bufferSize )
{
    return imports->serverGameSystem->GetEntityToken( buffer, bufferSize );
}

sint trap_RealTime( qtime_t* qtime )
{
    return imports->RealTime( qtime );
}

void trap_SnapVector( float32* v )
{
    imports->idsystem->SysSnapVector( v );
}

void trap_SendGameStat( pointer data )
{
    imports->serverMainSystem->MasterGameStat( data );
}

void trap_AddCommand( pointer cmdName, pointer cmdDesc )
{
    imports->cmdSystem->AddCommand( cmdName, nullptr, cmdDesc );
}

void trap_RemoveCommand( pointer cmdName )
{
    imports->cmdSystem->RemoveCommand( cmdName );
}

bool trap_GetTag( sint clientNum, sint tagFileNumber, valueType* tagname, orientation_t* _or )
{
    return imports->serverGameSystem->GetTag( clientNum, tagFileNumber, tagname, _or );
}

bool trap_LoadTag( pointer filename )
{
    return imports->serverMainSystem->LoadTag( filename );
}

sfxHandle_t trap_RegisterSound( pointer sample )
{
    return imports->soundSystem->RegisterSound( sample, false );
}

sint trap_GetSoundLength( sfxHandle_t sfxHandle )
{
    return imports->soundSystem->GetSoundLength( sfxHandle );
}

sint trap_PC_LoadSource( pointer filename )
{
    return imports->parseSystem->LoadSourceHandle( filename );
}

sint trap_PC_FreeSource( sint handle )
{
    return imports->parseSystem->FreeSourceHandle( handle );
}

sint trap_PC_ReadToken( sint handle, pc_token_t* pc_token )
{
    return imports->parseSystem->ReadTokenHandle( handle, pc_token );
}

sint trap_PC_SourceFileAndLine( sint handle, valueType* filename, sint* line )
{
    return imports->parseSystem->SourceFileAndLine( handle, filename, line );
}
