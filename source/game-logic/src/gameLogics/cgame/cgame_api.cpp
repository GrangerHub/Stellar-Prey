////////////////////////////////////////////////////////////////////////////////////////
// Copyright(C) 2011 - 2018 Dusan Jocic <dusanjocic@msn.com>
//
// This file is part of OpenWolf.
//
// OpenWolf is free software; you can redistribute it
// and / or modify it under the terms of the GNU General Public License as
// published by the Free Software Foundation; either version 2 of the License,
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
// File name:   cgame_api.cpp
// Version:     v1.01
// Created:
// Compilers:   Visual Studio 2017, gcc 7.3.0
// Description:
// -------------------------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////////////

#include <cgame/cgame_precompiled.h>

#define PASSFLOAT(x) x

cgameImports_t* imports;
idClientGameSystem* clientGameSystem;
idSoundSystem* soundSystem;
idRenderSystem* renderSystem;
idCollisionModelManager* collisionModelManager;
idFileSystem* fileSystem;
idCVarSystem* cvarSystem;
idCmdBufferSystem* cmdBufferSystem;
idCmdSystem* cmdSystem;
idSystem* idsystem;
idClientGUISystem* idGUISystem;
idClientScreenSystem* idScreenSystem;
idParseSystem* ParseSystem;

#ifdef __LINUX__
extern "C" idCGame* dllEntry( cgameImports_t* cgimports )
#else
Q_EXPORT idCGame* dllEntry( cgameImports_t* cgimports )
#endif
{
    imports = cgimports;
    
    clientGameSystem = imports->clientGameSystem;
    soundSystem = imports->soundSystem;
    renderSystem = imports->renderSystem;
    collisionModelManager = imports->collisionModelManager;
    fileSystem = imports->fileSystem;
    cvarSystem = imports->cvarSystem;
    cmdBufferSystem = imports->cmdBufferSystem;
    cmdSystem = imports->cmdSystem;
    idsystem = imports->idsystem;
    idGUISystem = imports->idGUISystem;
    idScreenSystem = imports->clientScreenSystem;
    ParseSystem = imports->parseSystem;
    
    return cgame;
}

void trap_Print( pointer fmt )
{
    imports->Print( fmt );
}

void trap_Error( pointer fmt )
{
    imports->clientGameSystem->CgameError( fmt );
}

sint trap_Milliseconds( void )
{
    return imports->idsystem->Milliseconds();
}

void trap_Cvar_Register( vmConvar_t* vmCvar, pointer varName, pointer defaultValue, sint flags, pointer description )
{
    imports->cvarSystem->Register( vmCvar, varName, defaultValue, flags, description );
}

void trap_Cvar_Update( vmConvar_t* vmCvar )
{
    imports->cvarSystem->Update( vmCvar );
}

void trap_Cvar_Set( pointer var_name, pointer value )
{
    imports->cvarSystem->Set( var_name, value );
}

void trap_Cvar_VariableStringBuffer( pointer var_name, valueType* buffer, sint bufsize )
{
    imports->cvarSystem->VariableStringBuffer( var_name, buffer, bufsize );
}

void trap_Cvar_LatchedVariableStringBuffer( pointer var_name, valueType* buffer, sint bufsize )
{
    imports->cvarSystem->LatchedVariableStringBuffer( var_name, buffer, bufsize );
}

sint trap_Argc( void )
{
    return imports->cmdSystem->Argc();
}

void trap_Argv( sint n, valueType* buffer, sint bufferLength )
{
    imports->cmdSystem->ArgvBuffer( n, buffer, bufferLength );
}

void trap_Args( valueType* buffer, sint bufferLength )
{
    imports->cmdSystem->ArgsBuffer( buffer, bufferLength );
}

void trap_LiteralArgs( valueType* buffer, sint bufferLength )
{
    imports->cmdSystem->LiteralArgsBuffer( buffer, bufferLength );
}

sint trap_GetDemoState( void )
{
    return imports->DemoState();
}

sint trap_GetDemoPos( void )
{
    return imports->DemoPos();
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

void trap_FS_FCloseFile( fileHandle_t f )
{
    imports->fileSystem->FCloseFile( f );
}

sint trap_FS_GetFileList( pointer path, pointer extension, valueType* listbuf, sint bufsize )
{
    return imports->fileSystem->GetFileList( path, extension, listbuf, bufsize );
}

sint trap_FS_Delete( valueType* filename )
{
    return imports->fileSystem->Delete( filename );
}

void trap_SendConsoleCommand( pointer text )
{
    imports->cmdBufferSystem->AddText( text );
}

void trap_AddCommand( pointer cmdName, pointer cmdDesc )
{
    imports->clientGameSystem->AddCgameCommand( cmdName, cmdDesc );
}

void trap_RemoveCommand( pointer cmdName )
{
    imports->cmdSystem->RemoveCommand( cmdName );
}

void trap_SendClientCommand( pointer s )
{
    imports->AddReliableCommand( s );
}

void trap_UpdateScreen( void )
{
    imports->clientScreenSystem->UpdateScreen();
}

void trap_CM_LoadMap( pointer mapname )
{
    imports->clientGameSystem->LoadMap( mapname );
}

sint trap_CM_NumInlineModels( void )
{
    return imports->collisionModelManager->NumInlineModels( );
}

clipHandle_t trap_CM_InlineModel( sint index )
{
    return imports->collisionModelManager->InlineModel( index );
}

clipHandle_t trap_CM_TempBoxModel( const vec3_t mins, const vec3_t maxs )
{
    return imports->collisionModelManager->TempBoxModel( mins, maxs, false );
}

clipHandle_t trap_CM_TempCapsuleModel( const vec3_t mins, const vec3_t maxs )
{
    return imports->collisionModelManager->TempBoxModel( mins, maxs, true );
}

sint trap_CM_PointContents( const vec3_t p, clipHandle_t model )
{
    return imports->collisionModelManager->PointContents( p, model );
}

sint trap_CM_TransformedPointContents( const vec3_t p, clipHandle_t model, const vec3_t origin, const vec3_t angles )
{
    return imports->collisionModelManager->TransformedPointContents( p, model, origin, angles );
}

void trap_CM_BoxTrace( trace_t* results, const vec3_t start, const vec3_t end, const vec3_t mins, const vec3_t maxs, clipHandle_t model, sint brushmask )
{
    imports->collisionModelManager->BoxTrace( results, start, end, mins, maxs, model, brushmask, TT_AABB );
}

void trap_CM_TransformedBoxTrace( trace_t* results, const vec3_t start, const vec3_t end, const vec3_t mins, const vec3_t maxs, clipHandle_t model, sint brushmask, const vec3_t origin, const vec3_t angles )
{
    imports->collisionModelManager->TransformedBoxTrace( results, start, end, mins, maxs, model, brushmask, origin, angles, TT_AABB );
}

void trap_CM_CapsuleTrace( trace_t* results, const vec3_t start, const vec3_t end, const vec3_t mins, const vec3_t maxs, clipHandle_t model, sint brushmask )
{
    imports->collisionModelManager->BoxTrace( results, start, end, mins, maxs, model, brushmask, TT_CAPSULE );
}

void trap_CM_TransformedCapsuleTrace( trace_t* results, const vec3_t start, const vec3_t end, const vec3_t mins, const vec3_t maxs, clipHandle_t model, sint brushmask, const vec3_t origin, const vec3_t angles )
{
    imports->collisionModelManager->TransformedBoxTrace( results, start, end, mins, maxs, model, brushmask, origin, angles, TT_CAPSULE );
}

void trap_CM_BiSphereTrace( trace_t* results, const vec3_t start, const vec3_t end, float32 startRad, float32 endRad, clipHandle_t model, sint mask )
{
    imports->collisionModelManager->BiSphereTrace( results, start, end, PASSFLOAT( startRad ), PASSFLOAT( endRad ), model, mask );
}

void trap_CM_TransformedBiSphereTrace( trace_t* results, const vec3_t start, const vec3_t end, float32 startRad, float32 endRad, clipHandle_t model, sint mask, const vec3_t origin )
{
    imports->collisionModelManager->TransformedBiSphereTrace( results, start, end, PASSFLOAT( startRad ), PASSFLOAT( endRad ), model, mask, origin );
}

sint trap_CM_MarkFragments( sint numPoints, const vec3_t* points, const vec3_t projection, sint maxPoints, vec3_t pointBuffer, sint maxFragments, markFragment_t* fragmentBuffer )
{
    return imports->renderSystem->MarkFragments( numPoints, points, projection, maxPoints, pointBuffer, maxFragments, fragmentBuffer );
}

void trap_R_ProjectDecal( qhandle_t hShader, sint numPoints, vec3_t* points, vec4_t projection, vec4_t color, sint lifeTime, sint fadeTime )
{
    //imports->renderSystem->ProjectDecal( hShader, numPoints, points, projection, color, lifeTime, fadeTime );
}

void trap_R_ClearDecals( void )
{
    //imports->renderSystem->ClearDecals();
}

void trap_S_StartSound( vec3_t origin, sint entityNum, sint entchannel, sfxHandle_t sfx )
{
    imports->soundSystem->StartSound( origin, entityNum, entchannel, sfx );
}

void trap_S_StartLocalSound( sfxHandle_t sfx, sint channelNum )
{
    imports->soundSystem->StartLocalSound( sfx, channelNum );
}

void trap_S_ClearLoopingSounds( bool killall )
{
    imports->soundSystem->ClearLoopingSounds( killall );
}

void trap_S_AddLoopingSound( sint entityNum, const vec3_t origin, const vec3_t velocity, sfxHandle_t sfx )
{
    imports->soundSystem->AddLoopingSound( entityNum, origin, velocity, sfx );
}

void trap_S_AddRealLoopingSound( sint entityNum, const vec3_t origin, const vec3_t velocity, sfxHandle_t sfx )
{
    imports->soundSystem->AddRealLoopingSound( entityNum, origin, velocity, sfx );
}

void trap_S_StopLoopingSound( sint entityNum )
{
    imports->soundSystem->StopLoopingSound( entityNum );
}

void trap_S_UpdateEntityPosition( sint entityNum, const vec3_t origin )
{
    imports->soundSystem->UpdateEntityPosition( entityNum, origin );
}

void trap_S_Respatialize( sint entityNum, const vec3_t origin, vec3_t axis[3], sint inwater )
{
    imports->soundSystem->Respatialize( entityNum, origin, axis, inwater );
}

void trap_S_StartBackgroundTrack( pointer intro, pointer loop )
{
    imports->soundSystem->StartBackgroundTrack( intro, loop );
}

void trap_R_LoadWorldMap( pointer mapname )
{
    //CG_DrawInformation(true);
    
    trap_PumpEventLoop();
    imports->renderSystem->LoadWorld( mapname );
}

qhandle_t trap_R_RegisterModel( pointer name )
{
    //CG_DrawInformation(true);
    
    trap_PumpEventLoop();
    return imports->renderSystem->RegisterModel( name );
}

qhandle_t trap_R_RegisterSkin( pointer name )
{
    //CG_DrawInformation(true);
    
    //trap_PumpEventLoop();
    return imports->renderSystem->RegisterSkin( name );
}

bool trap_R_GetSkinModel( qhandle_t skinid, pointer type, valueType* name )
{
    return 0;// imports->renderSystem->GetSkinModel(skinid, type, name);
}

qhandle_t trap_R_GetShaderFromModel( qhandle_t modelid, sint surfnum, sint withlightmap )
{
    return 0;//imports->renderSystem->GetShaderFromModel( modelid, surfnum, withlightmap );
}

qhandle_t trap_R_RegisterShader( pointer name )
{
    //CG_DrawInformation(true);
    
    //trap_PumpEventLoop();
    return imports->renderSystem->RegisterShader( name );
}

void trap_R_RegisterFont( pointer fontName, sint pointSize, fontInfo_t* font )
{
    //CG_DrawInformation(true);
    
    //trap_PumpEventLoop();
    imports->renderSystem->RegisterFont( fontName, pointSize, font );
}

qhandle_t trap_R_RegisterShaderNoMip( pointer name )
{
    //CG_DrawInformation(true);
    
    //trap_PumpEventLoop();
    return imports->renderSystem->RegisterShaderNoMip( name );
}

qhandle_t trap_R_RegisterShaderLightAttenuation( pointer name )
{
    return 0;//imports->renderSystem->RegisterShaderLightAttenuation( name );
}

void trap_R_ClearScene( void )
{
    imports->renderSystem->ClearScene();
}

void trap_R_AddRefEntityToScene( const refEntity_t* re )
{
    imports->renderSystem->AddRefEntityToScene( re );
}

void trap_R_AddPolyToScene( qhandle_t hShader, sint numVerts, const polyVert_t* verts )
{
    imports->renderSystem->AddPolyToScene( hShader, numVerts, verts, 1 );
}

void trap_R_AddPolysToScene( qhandle_t hShader, sint numVerts, const polyVert_t* verts, sint numPolys )
{
    imports->renderSystem->AddPolyToScene( hShader, numVerts, verts, numPolys );
}

//void trap_R_AddPolyBufferToScene( polyBuffer_t* pPolyBuffer )
//{
//    imports->renderSystem->AddPolyBufferToScene( pPolyBuffer );
//}

void trap_R_AddLightToScene( const vec3_t org, float32 intensity, float32 r, float32 g, float32 b )
{
    imports->renderSystem->AddLightToScene( org, PASSFLOAT( intensity ), PASSFLOAT( r ), PASSFLOAT( g ), PASSFLOAT( b ) );
}

void trap_GS_FS_Seek( fileHandle_t f, sint32 offset, fsOrigin_t origin )
{
    imports->fileSystem->Seek( f, offset, origin );
}

void trap_R_AddCoronaToScene( const vec3_t org, float32 r, float32 g, float32 b, float32 scale, sint id, bool visible )
{
    //imports->renderSystem->AddCoronaToScene( org, PASSFLOAT( r ), PASSFLOAT( g ), PASSFLOAT( b ), PASSFLOAT( scale ), id, visible );
}

void trap_R_SetFog( sint fogvar, sint var1, sint var2, float32 r, float32 g, float32 b, float32 density )
{
    //imports->renderSystem->SetFog( fogvar, var1, var2, PASSFLOAT( r ), PASSFLOAT( g ), PASSFLOAT( b ), PASSFLOAT( density ) );
}

void trap_R_SetGlobalFog( bool restore, sint duration, float32 r, float32 g, float32 b, float32 depthForOpaque )
{
    //imports->renderSystem->SetGlobalFog( restore, duration, PASSFLOAT( r ), PASSFLOAT( g ), PASSFLOAT( b ), PASSFLOAT( depthForOpaque ) );
}

void trap_R_RenderScene( const refdef_t* fd )
{
    imports->renderSystem->RenderScene( fd );
}

//void trap_R_SaveViewParms()
//{
//    imports->renderSystem->SaveViewParms();
//}

void trap_R_RestoreViewParms()
{
    //imports->renderSystem->RestoreViewParms();
}

void trap_R_SetColor( const float32* rgba )
{
    imports->renderSystem->SetColor( rgba );
}

void trap_R_SetClipRegion( const float32* region )
{
    imports->renderSystem->SetClipRegion( region );
}

void trap_R_DrawStretchPic( float32 x, float32 y, float32 w, float32 h, float32 s1, float32 t1, float32 s2, float32 t2, qhandle_t hShader )
{
    imports->renderSystem->DrawStretchPic( PASSFLOAT( x ), PASSFLOAT( y ), PASSFLOAT( w ), PASSFLOAT( h ), PASSFLOAT( s1 ), PASSFLOAT( t1 ), PASSFLOAT( s2 ), PASSFLOAT( t2 ), hShader );
}

void trap_R_DrawRotatedPic( float32 x, float32 y, float32 w, float32 h, float32 s1, float32 t1, float32 s2, float32 t2, qhandle_t hShader, float32 angle )
{
    //imports->renderSystem->DrawRotatedPic( PASSFLOAT( x ), PASSFLOAT( y ), PASSFLOAT( w ), PASSFLOAT( h ), PASSFLOAT( s1 ), PASSFLOAT( t1 ), PASSFLOAT( s2 ), PASSFLOAT( t2 ), hShader, PASSFLOAT( angle ) );
}

void trap_R_DrawStretchPicGradient( float32 x, float32 y, float32 w, float32 h, float32 s1, float32 t1, float32 s2, float32 t2, qhandle_t hShader, const float32* gradientColor, sint gradientType )
{
    //imports->renderSystem->DrawStretchPicGradient( PASSFLOAT( x ), PASSFLOAT( y ), PASSFLOAT( w ), PASSFLOAT( h ), PASSFLOAT( s1 ), PASSFLOAT( t1 ), PASSFLOAT( s2 ), PASSFLOAT( t2 ), hShader, gradientColor, gradientType );
}

void trap_R_Add2dPolys( polyVert_t* verts, sint numverts, qhandle_t hShader )
{
    //imports->renderSystem->Add2dPolys( verts, numverts, hShader );
}

void trap_R_ModelBounds( clipHandle_t model, vec3_t mins, vec3_t maxs )
{
    imports->renderSystem->ModelBounds( model, mins, maxs );
}

sint trap_R_LerpTag( orientation_t* tag, clipHandle_t mod, sint startFrame, sint endFrame, float32 frac, pointer tagName )
{
    return imports->renderSystem->LerpTag( tag, mod, startFrame, endFrame, PASSFLOAT( frac ), tagName );
}

void trap_GetGlconfig( vidconfig_t* glconfig )
{
    imports->clientGameSystem->GetGlconfig( glconfig );
}

void trap_GetGameState( gameState_t* gamestate )
{
    imports->clientGameSystem->GetGameState( gamestate );
}

void trap_GetCurrentSnapshotNumber( sint* snapshotNumber, sint* serverTime )
{
    imports->clientGameSystem->GetCurrentSnapshotNumber( snapshotNumber, serverTime );
}

bool trap_GetSnapshot( sint snapshotNumber, snapshot_t* snapshot )
{
    return imports->clientGameSystem->GetSnapshot( snapshotNumber, snapshot );
}

bool trap_GetServerCommand( sint serverCommandNumber )
{
    return imports->clientGameSystem->GetServerCommand( serverCommandNumber );
}

sint trap_GetCurrentCmdNumber( void )
{
    return imports->clientGameSystem->GetCurrentCmdNumber();
}

bool trap_GetUserCmd( sint cmdNumber, usercmd_t* ucmd )
{
    return imports->clientGameSystem->GetUserCmd( cmdNumber, ucmd );
}

void trap_SetUserCmdValue( sint stateValue, sint flags, float32 sensitivityScale, sint mpIdentClient )
{
    imports->clientGameSystem->SetUserCmdValue( stateValue, flags, PASSFLOAT( sensitivityScale ), mpIdentClient );
}

void trap_SetClientLerpOrigin( float32 x, float32 y, float32 z )
{
    imports->clientGameSystem->SetClientLerpOrigin( PASSFLOAT( x ), PASSFLOAT( y ), PASSFLOAT( z ) );
}

sint trap_MemoryRemaining( void )
{
    return imports->Hunk_MemoryRemaining();
}

bool trap_Key_IsDown( sint keynum )
{
    return imports->Key_IsDown( keynum );
}

sint trap_Key_GetCatcher( void )
{
    return imports->Key_GetCatcher();
}

void trap_Key_SetCatcher( sint catcher )
{
    imports->clientGameSystem->KeySetCatcher( catcher );
}

sint trap_Key_GetKey( pointer binding )
{
    return imports->Key_GetKey( binding );
}

bool trap_Key_GetOverstrikeMode( void )
{
    return imports->Key_GetOverstrikeMode();
}

void trap_Key_SetOverstrikeMode( bool state )
{
    imports->Key_SetOverstrikeMode( state );
}

sint trap_PC_AddGlobalDefine( valueType* define )
{
    return imports->parseSystem->AddGlobalDefine( define );
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

void trap_S_StopBackgroundTrack( void )
{
    imports->soundSystem->StopBackgroundTrack();
}

sint trap_RealTime( qtime_t* qtime )
{
    return imports->RealTime( qtime );
}

void trap_SnapVector( float32* v )
{
    imports->idsystem->SysSnapVector( v );
}

sint trap_CIN_PlayCinematic( pointer arg0, sint xpos, sint ypos, sint width, sint height, sint bits )
{
    return imports->CIN_PlayCinematic( arg0, xpos, ypos, width, height, bits );
}

e_status trap_CIN_StopCinematic( sint handle )
{
    return imports->CIN_StopCinematic( handle );
}

e_status trap_CIN_RunCinematic( sint handle )
{
    return imports->CIN_RunCinematic( handle );
}

void trap_CIN_DrawCinematic( sint handle )
{
    imports->CIN_DrawCinematic( handle );
}

void trap_CIN_SetExtents( sint handle, sint x, sint y, sint w, sint h )
{
    imports->CIN_SetExtents( handle, x, y, w, h );
}

void trap_R_RemapShader( pointer oldShader, pointer newShader, pointer timeOffset )
{
    imports->renderSystem->RemapShader( oldShader, newShader, timeOffset );
}

bool trap_loadCamera( sint camNum, pointer name )
{
    return imports->loadCamera( camNum, name );
}

void trap_startCamera( sint camNum, sint time )
{
    imports->startCamera( camNum, time );
}

void trap_stopCamera( sint camNum )
{
    imports->stopCamera( camNum );
}

bool trap_getCameraInfo( sint camNum, sint time, vec3_t* origin, vec3_t* angles, float32* fov )
{
    return imports->getCameraInfo( camNum, time, origin, angles, fov );
}

bool trap_GetEntityToken( valueType* buffer, sint bufferSize )
{
    return imports->renderSystem->GetEntityToken( buffer, bufferSize );
}

void trap_UI_Popup( pointer arg0 )
{
    imports->clientGameSystem->UIPopup( arg0 );
}

void trap_UI_ClosePopup( pointer arg0 )
{
    imports->clientGameSystem->UIClosePopup( arg0 );
}

void trap_Key_GetBindingBuf( sint keynum, valueType* buf, sint buflen )
{
    imports->idGUISystem->GetBindingBuf( keynum, buf, buflen );
}

void trap_Key_SetBinding( sint keynum, pointer binding )
{
    imports->Key_SetBinding( keynum, binding );
}

void trap_Key_KeynumToStringBuf( sint keynum, valueType* buf, sint buflen )
{
    imports->idGUISystem->KeynumToStringBuf( keynum, buf, buflen );
}

void trap_Key_KeysForBinding( pointer binding, sint* key1, sint* key2 )
{
    imports->Key_GetBindingByString( binding, key1, key2 );
}

void trap_CG_TranslateString( pointer string, valueType* buf )
{
    imports->CL_TranslateString( string, buf );
}

bool trap_R_inPVS( const vec3_t p1, const vec3_t p2 )
{
    return imports->renderSystem->inPVS( p1, p2 );
}


void trap_PumpEventLoop( void )
{
    /*if(!cgs.initing) {
    	return;
    }
    syscall(CG_PUMPEVENTLOOP);*/
}

bool trap_R_LoadDynamicShader( pointer shadername, pointer shadertext )
{
    return 0;// imports->renderSystem->LoadDynamicShader(shadername, shadertext);
}

void trap_R_RenderToTexture( sint textureid, sint x, sint y, sint w, sint h )
{
    //imports->renderSystem->RenderToTexture( textureid, x, y, w, h );
}

sint trap_R_GetTextureId( pointer name )
{
    return 0;// imports->renderSystem->GetTextureId(name);
}

void trap_R_Finish( void )
{
    //imports->renderSystem->Finish();
}

void trap_GetDemoName( valueType* buffer, sint size )
{
    imports->CL_DemoName( buffer, size );
}

sint trap_R_LightForPoint( vec3_t point, vec3_t ambientLight, vec3_t directedLight, vec3_t lightDir )
{
    return imports->renderSystem->LightForPoint( point, ambientLight, directedLight, lightDir );
}

void trap_TranslateString( pointer string, valueType* buf )
{
    imports->CL_TranslateString( string, buf );
}

sfxHandle_t trap_S_RegisterSound( pointer sample )
{
    return soundSystem->RegisterSound( sample, false );
}

sint trap_S_SoundDuration( sfxHandle_t handle )
{
    return imports->soundSystem->SoundDuration( handle );
}

void trap_Cvar_SetValue( pointer var_name, float32 value )
{
    imports->SetValue( var_name, PASSFLOAT( value ) );
}
