////////////////////////////////////////////////////////////////////////////////////////
// Copyright(C) 1999 - 2005 Id Software, Inc.
// Copyright(C) 2000 - 2006 Tim Angus
// Copyright(C) 2011 - 2019 Dusan Jocic <dusanjocic@msn.com>
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
// File name:   sgame_session.cpp
// Version:     v1.01
// Created:
// Compilers:   Visual Studio 2019, gcc 7.3.0
// Description: Session data is the only data that stays persistant across level loads
//              and tournament restarts.
// -------------------------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////////////

#include <sgame/sgame_precompiled.h>

/*
===============
idSGameSession::idSGameSession
===============
*/
idSGameSession::idSGameSession( void )
{
}

/*
===============
idSGameSession::~idSGameSession
===============
*/
idSGameSession::~idSGameSession( void )
{
}

/*
================
idSGameSession::WriteClientSessionData

Called on game shutdown
================
*/
void idSGameSession::WriteClientSessionData( gclient_t* client )
{
    pointer  s;
    pointer  var;
    
    s = va( "%i %i %i %i %s",
            client->sess.spectatorTime,
            client->sess.spectatorState,
            client->sess.spectatorClient,
            client->sess.restartTeam,
            Com_ClientListString( &client->sess.ignoreList )
          );
          
    var = va( "session%i", ( sint )( client - level.clients ) );
    
    trap_Cvar_Set( var, s );
}

/*
================
idSGameSession::ReadSessionData

Called on a reconnect
================
*/
void idSGameSession::ReadSessionData( gclient_t* client )
{
    valueType        s[MAX_STRING_CHARS];
    pointer  var;
    sint         spectatorState;
    sint         restartTeam;
    valueType        ignorelist[17];
    
    var = va( "session%i", ( sint )( client - level.clients ) );
    trap_Cvar_VariableStringBuffer( var, s, sizeof( s ) );
    
    sscanf( s, "%i %i %i %i %16s",
            &client->sess.spectatorTime,
            &spectatorState,
            &client->sess.spectatorClient,
            &restartTeam,
            ignorelist
          );
          
    client->sess.spectatorState = ( spectatorState_t )spectatorState;
    client->sess.restartTeam = ( team_t )restartTeam;
    Com_ClientListParse( &client->sess.ignoreList, ignorelist );
}


/*
================
idSGameSession::InitSessionData

Called on a first-time connect
================
*/
void idSGameSession::InitSessionData( gclient_t* client, valueType* userinfo )
{
    clientSession_t* sess;
    pointer value;
    
    sess = &client->sess;
    
    // initial team determination
    value = Info_ValueForKey( userinfo, "team" );
    if( value[0] == 's' )
    {
        // a willing spectator, not a waiting-in-line
        sess->spectatorState = SPECTATOR_FREE;
    }
    else
    {
        if( g_maxGameClients.integer > 0 && level.numNonSpectatorClients >= g_maxGameClients.integer )
        {
            sess->spectatorState = SPECTATOR_FREE;
        }
        else
        {
            sess->spectatorState = SPECTATOR_NOT;
        }
    }
    
    sess->restartTeam = TEAM_NONE;
    sess->spectatorState = SPECTATOR_FREE;
    sess->spectatorTime = level.time;
    sess->spectatorClient = -1;
    ::memset( &sess->ignoreList, 0, sizeof( sess->ignoreList ) );
    
    WriteClientSessionData( client );
}


/*
==================
idSGameSession::WriteSessionData
==================
*/
void idSGameSession::WriteSessionData( void )
{
    sint i;
    
    for( i = 0 ; i < level.maxclients ; i++ )
    {
        if( level.clients[i].pers.connected == CON_CONNECTED )
        {
            WriteClientSessionData( &level.clients[i] );
        }
    }
    
    // write values for sv_maxclients and sv_democlients because they invalidate session data
    trap_Cvar_Set( "session", va( "%i %i", trap_Cvar_VariableIntegerValue( "sv_maxclients" ), trap_Cvar_VariableIntegerValue( "sv_democlients" ) ) );
}
