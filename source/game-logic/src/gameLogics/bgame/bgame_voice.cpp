////////////////////////////////////////////////////////////////////////////////////////
// Copyright(C) 1999 - 2005 Id Software, Inc.
// Copyright(C) 2000 - 2006 Tim Angus
// Copyright(C) 2008 Tony J. White
// Copyright(C) 2018 - 2021 Dusan Jocic <dusanjocic@msn.com>
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
// File name:   bgame_alloc.cpp
// Created:
// Compilers:   Microsoft (R) C/C++ Optimizing Compiler Version 19.26.28806 for x64,
//              gcc (Ubuntu 9.3.0-10ubuntu2) 9.3.0
// Description: both games voice functions
// -------------------------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////////////

#ifdef CGAMEDLL
#include <cgame/cgame_precompiled.hpp>
#elif GAMEDLL
#include <sgame/sgame_precompiled.hpp>
#else
#include <GUI/gui_precompiled.hpp>
#endif // !GAMEDLL

sint trap_FS_FOpenFile( pointer qpath, fileHandle_t* f, fsMode_t mode );
sint trap_FS_GetFileList( pointer path, pointer extension, valueType* listbuf, sint bufsize );
sint trap_PC_LoadSource( pointer filename );
sint trap_PC_FreeSource( sint handle );
sint trap_PC_ReadToken( sint handle, pc_token_t* pc_token );
sint trap_PC_SourceFileAndLine( sint handle, valueType* filename, sint* line );

#ifdef CGAMEDLL
sfxHandle_t trap_S_RegisterSound( pointer sample );
sint trap_S_SoundDuration( sfxHandle_t handle );
#endif

/*
============
idBothGamesLocal::VoiceParseError
============
*/
void idBothGamesLocal::VoiceParseError( fileHandle_t handle, valueType* err )
{
    sint line;
    valueType filename[ MAX_QPATH ];
    
    trap_PC_SourceFileAndLine( handle, filename, &line );
    trap_PC_FreeSource( handle );
    Com_Error( ERR_FATAL, "%s on line %d of %s\n", err, line, filename );
}

/*
============
idBothGamesLocal::VoiceList
============
*/
voice_t* idBothGamesLocal::VoiceList( void )
{
    valueType fileList[ MAX_VOICES * ( MAX_VOICE_NAME_LEN + 8 ) ] = {""};
    sint numFiles, i, fileLen = 0;
    sint count = 0;
    valueType* filePtr;
    voice_t* voices = nullptr;
    voice_t* top = nullptr;
    
    numFiles = trap_FS_GetFileList( "voice", ".voice", fileList, sizeof( fileList ) );
    
    if( numFiles < 1 )
    {
        return nullptr;
    }
    
    // special case for default.voice.  this file is REQUIRED and will
    // always be loaded first in the event of overflow of voice definitions
    if( !trap_FS_FOpenFile( "voice/default.voice", nullptr, FS_READ ) )
    {
        Com_Printf( "voice/default.voice missing, voice system disabled." );
        return nullptr;
    }
    
    voices = ( voice_t* )Alloc( sizeof( voice_t ) );
    Q_strncpyz( voices->name, "default", sizeof( voices->name ) );
    voices->cmds = nullptr;
    voices->next = nullptr;
    count = 1;
    
    top = voices;
    
    filePtr = fileList;
    for( i = 0; i < numFiles; i++, filePtr += fileLen + 1 )
    {
        fileLen = strlen( filePtr );
        
        // accounted for above
        if( !Q_stricmp( filePtr, "default.voice" ) )
        {
            continue;
        }
        
        if( fileLen > MAX_VOICE_NAME_LEN + 8 )
        {
            Com_Printf( S_COLOR_YELLOW "WARNING: MAX_VOICE_NAME_LEN is %d. skipping \"%s\", filename too long", MAX_VOICE_NAME_LEN, filePtr );
            continue;
        }
        
        // trap_FS_GetFileList() buffer has overflowed
        if( !trap_FS_FOpenFile( va( "voice/%s", filePtr ), nullptr, FS_READ ) )
        {
            Com_Printf( S_COLOR_YELLOW "WARNING: BG_VoiceList(): detected "
                        " an invalid .voice file \"%s\" in directory listing.  You have"
                        "probably named one or more .voice files with outrageously long "
                        "names.  gjbs", filePtr );
            break;
        }
        
        if( count >= MAX_VOICES )
        {
            Com_Printf( S_COLOR_YELLOW "WARNING: .voice file overflow.  "
                        "%d of %d .voice files loaded.  MAX_VOICES is %d",
                        count, numFiles, MAX_VOICES );
            break;
        }
        
        voices->next = ( voice_t* )Alloc( sizeof( voice_t ) );
        voices = voices->next;
        
        Q_strncpyz( voices->name, filePtr, sizeof( voices->name ) );
        
        // strip extension
        voices->name[ fileLen - 6 ] = '\0';
        voices->cmds = nullptr;
        voices->next = nullptr;
        count++;
    }
    
    return top;
}

/*
============
idBothGamesLocal::VoiceParseTrack
============
*/
bool idBothGamesLocal::VoiceParseTrack( sint handle, voiceTrack_t* voiceTrack )
{
    pc_token_t token;
    bool found = false;
    bool foundText = false;
    bool foundToken = false;
    
    foundToken = trap_PC_ReadToken( handle, &token );
    while( foundToken )
    {
        if( token.string[ 0 ] == '}' )
        {
            if( foundText )
                return true;
            else
            {
                VoiceParseError( handle, "idBothGamesLocal::VoiceParseTrack(): missing text attribute for track" );
            }
        }
        else if( !Q_stricmp( token.string, "team" ) )
        {
            foundToken = trap_PC_ReadToken( handle, &token );
            found = false;
            while( foundToken && token.type == TT_NUMBER )
            {
                found = true;
                
                if( voiceTrack->team < 0 )
                {
                    voiceTrack->team = 0;
                }
                
                voiceTrack->team |= ( 1 << token.intvalue );
                foundToken = trap_PC_ReadToken( handle, &token );
            }
            if( !found )
            {
                VoiceParseError( handle, "idBothGamesLocal::VoiceParseTrack(): missing \"team\" value" );
            }
            continue;
        }
        else if( !Q_stricmp( token.string, "class" ) )
        {
            foundToken = trap_PC_ReadToken( handle, &token );
            found = false;
            
            while( foundToken && token.type == TT_NUMBER )
            {
                found = true;
                
                if( voiceTrack->_class < 0 )
                {
                    voiceTrack->_class = 0;
                }
                
                voiceTrack->_class |= ( 1 << token.intvalue );
                foundToken = trap_PC_ReadToken( handle, &token );
            }
            if( !found )
            {
                VoiceParseError( handle, "idBothGamesLocal::VoiceParseTrack(): missing \"class\" value" );
            }
            continue;
        }
        else if( !Q_stricmp( token.string, "weapon" ) )
        {
            foundToken = trap_PC_ReadToken( handle, &token );
            found = false;
            
            while( foundToken && token.type == TT_NUMBER )
            {
                found = true;
                
                if( voiceTrack->weapon < 0 )
                {
                    voiceTrack->weapon = 0;
                }
                
                voiceTrack->weapon |= ( 1 << token.intvalue );
                foundToken = trap_PC_ReadToken( handle, &token );
            }
            
            if( !found )
            {
                VoiceParseError( handle, "idBothGamesLocal::VoiceParseTrack(): missing \"weapon\" value" );
            }
            
            continue;
        }
        else if( !Q_stricmp( token.string, "text" ) )
        {
            if( foundText )
            {
                VoiceParseError( handle, "idBothGamesLocal::VoiceParseTrack(): duplicate \"text\" definition for track" );
            }
            foundToken = trap_PC_ReadToken( handle, &token );
            
            if( !foundToken )
            {
                VoiceParseError( handle, "idBothGamesLocal::VoiceParseTrack(): " "missing \"text\" value" );
            }
            foundText = true;
            
            if( strlen( token.string ) >= MAX_SAY_TEXT )
            {
                VoiceParseError( handle, va( "idBothGamesLocal::VoiceParseTrack(): "
                                             "\"text\" value " "\"%s\" exceeds MAX_SAY_TEXT length", token.string ) );
            }
            
            voiceTrack->text = ( valueType* )Alloc( strlen( token.string ) + 1 );
            Q_strncpyz( voiceTrack->text, token.string, strlen( token.string ) + 1 );
            foundToken = trap_PC_ReadToken( handle, &token );
            continue;
        }
        else if( !Q_stricmp( token.string, "enthusiasm" ) )
        {
            foundToken = trap_PC_ReadToken( handle, &token );
            if( token.type == TT_NUMBER )
            {
                voiceTrack->enthusiasm = token.intvalue;
            }
            else
            {
                VoiceParseError( handle, "idBothGamesLocal::VoiceParseTrack(): missing \"enthusiasm\" value" );
            }
            foundToken = trap_PC_ReadToken( handle, &token );
            continue;
        }
        else
        {
            VoiceParseError( handle, va( "idBothGamesLocal::VoiceParseTrack(): unknown token \"%s\"", token.string ) );
        }
    }
    return false;
}

/*
============
idBothGamesLocal::VoiceParseCommand
============
*/
voiceTrack_t* idBothGamesLocal::VoiceParseCommand( sint handle )
{
    pc_token_t token;
    bool parsingTrack = false;
    voiceTrack_t* voiceTracks = nullptr;
    voiceTrack_t* top = nullptr;
    
    while( trap_PC_ReadToken( handle, &token ) )
    {
        if( !parsingTrack && token.string[0] == '}' )
        {
            return top;
        }
        
        if( parsingTrack )
        {
            if( token.string[ 0 ] == '{' )
            {
                VoiceParseTrack( handle, voiceTracks );
                parsingTrack = false;
                continue;
                
            }
            else
            {
                VoiceParseError( handle, va( "BG_VoiceParseCommand(): parse error at \"%s\"", token.string ) );
            }
        }
        
        
        if( top == nullptr )
        {
            voiceTracks = ( voiceTrack_t* )Alloc( sizeof( voiceTrack_t ) );
            top = voiceTracks;
        }
        else
        {
            voiceTracks->next = ( voiceTrack_t* )Alloc( sizeof( voiceCmd_t ) );
            voiceTracks = voiceTracks->next;
        }
        
        if( !trap_FS_FOpenFile( token.string, nullptr, FS_READ ) )
        {
            sint line;
            valueType filename[ MAX_QPATH ];
            
            trap_PC_SourceFileAndLine( handle, filename, &line );
            
            Com_Printf( S_COLOR_YELLOW "WARNING: BG_VoiceParseCommand(): "
                        "track \"%s\" referenced on line %d of %s does not exist\n", token.string, line, filename );
        }
        else
        {
#ifdef CGAMEDLL
            voiceTracks->track = trap_S_RegisterSound( token.string );
            voiceTracks->duration = trap_S_SoundDuration( voiceTracks->track );
#endif
        }
        
        voiceTracks->team = -1;
        voiceTracks->_class = -1;
        voiceTracks->weapon = -1;
        voiceTracks->enthusiasm = 0;
        voiceTracks->text = nullptr;
        voiceTracks->next = nullptr;
        parsingTrack = true;
        
    }
    return nullptr;
}

/*
============
idBothGamesLocal::VoiceParse
============
*/
voiceCmd_t* idBothGamesLocal::VoiceParse( valueType* name )
{
    voiceCmd_t* voiceCmds = nullptr;
    voiceCmd_t* top = nullptr;
    pc_token_t token;
    bool parsingCmd = false;
    sint handle;
    
    handle = trap_PC_LoadSource( va( "voice/%s.voice", name ) );
    
    if( !handle )
    {
        return nullptr;
    }
    
    while( trap_PC_ReadToken( handle, &token ) )
    {
        if( parsingCmd )
        {
            if( token.string[ 0 ] == '{' )
            {
                voiceCmds->tracks = VoiceParseCommand( handle );
                parsingCmd = false;
                continue;
            }
            else
            {
                sint line;
                valueType filename[ MAX_QPATH ];
                
                trap_PC_SourceFileAndLine( handle, filename, &line );
                Com_Error( ERR_FATAL, "idBothGamesLocal::VoiceParse(): parse error on line %d of %s\n", line, filename );
            }
        }
        
        if( strlen( token.string ) >= MAX_VOICE_CMD_LEN )
        {
            sint line;
            valueType filename[ MAX_QPATH ];
            
            trap_PC_SourceFileAndLine( handle, filename, &line );
            Com_Error( ERR_FATAL, "idBothGamesLocal::VoiceParse(): command \"%s\" exceeds MAX_VOICE_CMD_LEN (%d) on line %d of %s\n", token.string, MAX_VOICE_CMD_LEN, line, filename );
        }
        
        if( top == nullptr )
        {
            voiceCmds = ( voiceCmd_t* )Alloc( sizeof( voiceCmd_t ) );
            top = voiceCmds;
        }
        else
        {
            voiceCmds->next = ( voiceCmd_t* )Alloc( sizeof( voiceCmd_t ) );
            voiceCmds = voiceCmds->next;
        }
        
        Q_strncpyz( voiceCmds->cmd, token.string, sizeof( voiceCmds->cmd ) );
        voiceCmds->next = nullptr;
        parsingCmd = true;
        
    }
    
    trap_PC_FreeSource( handle );
    
    return top;
}

/*
============
idBothGamesLocal::VoiceInit
============
*/
voice_t* idBothGamesLocal::VoiceInit( void )
{
    voice_t* voices;
    voice_t* voice;
    
    voices = VoiceList();
    
    voice = voices;
    
    while( voice )
    {
        voice->cmds = VoiceParse( voice->name );
        voice = voice->next;
    }
    
    return voices;
}

/*
============
idBothGamesLocal::PrintVoices
============
*/
void idBothGamesLocal::PrintVoices( voice_t* voices, sint debugLevel )
{
    voice_t* voice = voices;
    voiceCmd_t* voiceCmd;
    voiceTrack_t* voiceTrack;
    
    sint cmdCount, trackCount;
    
    if( voice == nullptr )
    {
        Com_Printf( "voice list is empty\n" );
        return;
    }
    
    while( voice != nullptr )
    {
        if( debugLevel > 0 )
        {
            Com_Printf( "voice \"%s\"\n", voice->name );
        }
        
        voiceCmd = voice->cmds;
        cmdCount = 0;
        trackCount = 0;
        
        while( voiceCmd != nullptr )
        {
            if( debugLevel > 0 )
            {
                Com_Printf( "  %s\n", voiceCmd->cmd );
            }
            
            voiceTrack = voiceCmd->tracks;
            cmdCount++;
            
            while( voiceTrack != nullptr )
            {
                if( debugLevel > 1 )
                {
                    Com_Printf( "    text -> %s\n", voiceTrack->text );
                }
                
                if( debugLevel > 2 )
                {
                    Com_Printf( "    team -> %d\n", voiceTrack->team );
                    Com_Printf( "    class -> %d\n", voiceTrack->_class );
                    Com_Printf( "    weapon -> %d\n", voiceTrack->weapon );
                    Com_Printf( "    enthusiasm -> %d\n", voiceTrack->enthusiasm );
#ifdef CGAMEDLL
                    Com_Printf( "    duration -> %d\n", voiceTrack->duration );
#endif
                }
                
                if( debugLevel > 1 )
                {
                    Com_Printf( "\n" );
                }
                
                trackCount++;
                voiceTrack = voiceTrack->next;
            }
            
            voiceCmd = voiceCmd->next;
        }
        
        if( !debugLevel )
        {
            Com_Printf( "voice \"%s\": %d commands, %d tracks\n", voice->name, cmdCount, trackCount );
        }
        voice = voice->next;
    }
}

/*
============
idBothGamesLocal::VoiceByName
============
*/
voice_t* idBothGamesLocal::VoiceByName( voice_t* head, valueType* name )
{
    voice_t* v = head;
    
    while( v )
    {
        if( !Q_stricmp( v->name, name ) )
        {
            return v;
        }
        
        v = v->next;
    }
    return nullptr;
}

/*
============
idBothGamesLocal::VoiceCmdFind
============
*/
voiceCmd_t* idBothGamesLocal::VoiceCmdFind( voiceCmd_t* head, valueType* name, sint* cmdNum )
{
    voiceCmd_t* vc = head;
    sint i = 0;
    
    while( vc )
    {
        i++;
        
        if( !Q_stricmp( vc->cmd, name ) )
        {
            *cmdNum = i;
            return vc;
        }
        
        vc = vc->next;
    }
    return nullptr;
}

/*
============
idBothGamesLocal::VoiceCmdByNum
============
*/
voiceCmd_t* idBothGamesLocal::VoiceCmdByNum( voiceCmd_t* head, sint num )
{
    voiceCmd_t* vc = head;
    sint i = 0;
    
    while( vc )
    {
        i++;
        
        if( i == num )
        {
            return vc;
        }
        
        vc = vc->next;
    }
    return nullptr;
}

/*
============
idBothGamesLocal::VoiceTrackByNum
============
*/
voiceTrack_t* idBothGamesLocal::VoiceTrackByNum( voiceTrack_t* head, sint num )
{
    voiceTrack_t* vt = head;
    sint i = 0;
    
    while( vt )
    {
        i++;
        
        if( i == num )
        {
            return vt;
        }
        
        vt = vt->next;
    }
    return nullptr;
}

/*
============
BG_VoiceTrackFind
============
*/
voiceTrack_t* idBothGamesLocal::VoiceTrackFind( voiceTrack_t* head, team_t team, class_t _class, weapon_t weapon, sint enthusiasm, sint* trackNum )
{
    voiceTrack_t* vt = head;
    sint highestMatch = 0;
    sint matchCount = 0;
    sint selectedMatch = 0;
    sint i = 0, j = 0;
    
    // find highest enthusiasm without going over
    while( vt )
    {
        if( ( vt->team >= 0 && !( vt->team  & ( 1 << team ) ) ) || ( vt->_class >= 0 && !( vt->_class & ( 1 << _class ) ) ) || ( vt->weapon >= 0 && !( vt->weapon & ( 1 << weapon ) ) ) || vt->enthusiasm > enthusiasm )
        {
            vt = vt->next;
            continue;
        }
        
        if( vt->enthusiasm > highestMatch )
        {
            matchCount = 0;
            highestMatch = vt->enthusiasm;
        }
        
        if( vt->enthusiasm == highestMatch )
        {
            matchCount++;
        }
        
        vt = vt->next;
    }
    
    if( !matchCount )
    {
        return nullptr;
    }
    
    // return randomly selected match
    selectedMatch = rand() % matchCount;
    vt = head;
    i = 0;
    j = 0;
    while( vt )
    {
        j++;
        
        if( ( vt->team >= 0 && !( vt->team  & ( 1 << team ) ) ) || ( vt->_class >= 0 && !( vt->_class & ( 1 << _class ) ) ) || ( vt->weapon >= 0 && !( vt->weapon & ( 1 << weapon ) ) ) || vt->enthusiasm != highestMatch )
        {
            vt = vt->next;
            continue;
        }
        
        if( i == selectedMatch )
        {
            *trackNum = j;
            return vt;
        }
        
        i++;
        vt = vt->next;
    }
    
    return nullptr;
}
