////////////////////////////////////////////////////////////////////////////////////////
// Copyright(C) 1999 - 2005 Id Software, Inc.
// Copyright(C) 2000 - 2006 Tim Angus
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
// File name:   sgame_cmds.cpp
// Created:
// Compilers:   Microsoft (R) C/C++ Optimizing Compiler Version 19.26.28806 for x64,
//              gcc (Ubuntu 9.3.0-10ubuntu2) 9.3.0
// Description:
// -------------------------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////////////

#include <sgame/sgame_precompiled.hpp>

/*
===============
idSGameCmds::idSGameCmds
===============
*/
idSGameCmds::idSGameCmds( void )
{
}

/*
===============
idSGameCmds::~idSGameCmds
===============
*/
idSGameCmds::~idSGameCmds( void )
{
}

/*
==================
idSGameCmds::SanitiseString

Remove case and control characters from a string
==================
*/
void idSGameCmds::SanitiseString( valueType* in, valueType* out, sint len )
{
    bool skip = true;
    sint spaces = 0;
    
    len--;
    
    while( *in && len > 0 )
    {
        // strip leading white space
        if( *in == ' ' )
        {
            if( skip )
            {
                in++;
                continue;
            }
            spaces++;
        }
        else
        {
            spaces = 0;
            skip = false;
        }
        
        if( Q_IsColorString( in ) )
        {
            in += 2;    // skip color code
            continue;
        }
        
        if( *in < 32 )
        {
            in++;
            continue;
        }
        
        *out++ = tolower( *in++ );
        len--;
    }
    out -= spaces;
    *out = 0;
}

/*
==================
idSGameCmds::ClientNumberFromString

Returns a player number for either a number or name string
Returns -1 if invalid
==================
*/
sint idSGameCmds::ClientNumberFromString( valueType* s )
{
    gclient_t* cl;
    sint       i;
    valueType      s2[ MAX_NAME_LENGTH ];
    valueType      n2[ MAX_NAME_LENGTH ];
    
    // numeric values are just slot numbers
    for( i = 0; s[ i ] && isdigit( s[ i ] ); i++ );
    if( !s[ i ] )
    {
        i = atoi( s );
        
        if( i < 0 || i >= level.maxclients )
            return -1;
            
        cl = &level.clients[ i ];
        
        if( cl->pers.connected == CON_DISCONNECTED )
            return -1;
            
        return i;
    }
    
    // check for a name match
    SanitiseString( s, s2, sizeof( s2 ) );
    
    for( i = 0, cl = level.clients; i < level.maxclients; i++, cl++ )
    {
        if( cl->pers.connected == CON_DISCONNECTED )
            continue;
            
        SanitiseString( cl->pers.netname, n2, sizeof( n2 ) );
        
        if( !strcmp( n2, s2 ) )
            return i;
    }
    
    return -1;
}


/*
==================
idSGameCmds::MatchOnePlayer

This is a companion function to idSGameCmds::ClientNumbersFromString()

err will be populated with an error message.
==================
*/
void idSGameCmds::MatchOnePlayer( sint* plist, sint num, valueType* err, sint len )
{
    gclient_t* cl;
    sint i;
    valueType line[ MAX_NAME_LENGTH + 10 ] = {""};
    
    err[ 0 ] = '\0';
    if( num == 0 )
    {
        Q_strcat( err, len, "no connected player by that name or slot #" );
    }
    else if( num > 1 )
    {
        Q_strcat( err, len, "more than one player name matches. "
                  "be more specific or use the slot #:\n" );
        for( i = 0; i < num; i++ )
        {
            cl = &level.clients[ plist[ i ] ];
            if( cl->pers.connected == CON_DISCONNECTED )
                continue;
            Q_vsprintf_s( line, sizeof( line ), sizeof( line ), "%2i - %s^7\n",
                          plist[ i ], cl->pers.netname );
            if( strlen( err ) + strlen( line ) > len )
                break;
            Q_strcat( err, len, line );
        }
    }
}

/*
==================
idSGameCmds::ClientNumbersFromString

Sets plist to an array of integers that represent client numbers that have
names that are a partial match for s.

Returns number of matching clientids up to max.
==================
*/
sint idSGameCmds::ClientNumbersFromString( valueType* s, sint* plist, sint max )
{
    gclient_t* p;
    sint i, found = 0;
    valueType n2[ MAX_NAME_LENGTH ] = {""};
    valueType s2[ MAX_NAME_LENGTH ] = {""};
    
    if( max == 0 )
        return 0;
        
    // if a number is provided, it is a clientnum
    for( i = 0; s[ i ] && isdigit( s[ i ] ); i++ );
    if( !s[ i ] )
    {
        i = atoi( s );
        if( i >= 0 && i < level.maxclients )
        {
            p = &level.clients[ i ];
            if( p->pers.connected != CON_DISCONNECTED )
            {
                *plist = i;
                return 1;
            }
        }
        // we must assume that if only a number is provided, it is a clientNum
        return 0;
    }
    
    // now look for name matches
    SanitiseString( s, s2, sizeof( s2 ) );
    if( strlen( s2 ) < 1 )
        return 0;
    for( i = 0; i < level.maxclients && found < max; i++ )
    {
        p = &level.clients[ i ];
        if( p->pers.connected == CON_DISCONNECTED )
        {
            continue;
        }
        SanitiseString( p->pers.netname, n2, sizeof( n2 ) );
        if( strstr( n2, s2 ) )
        {
            *plist++ = i;
            found++;
        }
    }
    return found;
}

/*
==================
idSGameCmds::ScoreboardMessage
==================
*/
void idSGameCmds::ScoreboardMessage( gentity_t* ent )
{
    valueType      entry[ 1024 ];
    valueType      string[ 1400 ];
    sint       stringlength;
    sint       i, j;
    gclient_t* cl;
    sint       numSorted;
    weapon_t  weapon = WP_NONE;
    upgrade_t upgrade = UP_NONE;
    
    // send the latest information on all clients
    string[ 0 ] = 0;
    stringlength = 0;
    
    numSorted = level.numConnectedClients;
    
    for( i = 0; i < numSorted; i++ )
    {
        sint   ping;
        
        cl = &level.clients[ level.sortedClients[ i ] ];
        
        if( cl->pers.connected == CON_CONNECTING )
            ping = -1;
        else
            ping = cl->ps.ping < 999 ? cl->ps.ping : 999;
            
        if( cl->sess.spectatorState == SPECTATOR_NOT &&
                ( ent->client->pers.teamSelection == TEAM_NONE ||
                  cl->pers.teamSelection == ent->client->pers.teamSelection ) )
        {
            weapon = ( weapon_t )cl->ps.weapon;
            
            if( bggame->InventoryContainsUpgrade( UP_BATTLESUIT, cl->ps.stats ) )
                upgrade = UP_BATTLESUIT;
            else if( bggame->InventoryContainsUpgrade( UP_JETPACK, cl->ps.stats ) )
                upgrade = UP_JETPACK;
            else if( bggame->InventoryContainsUpgrade( UP_BATTPACK, cl->ps.stats ) )
                upgrade = UP_BATTPACK;
            else if( bggame->InventoryContainsUpgrade( UP_HELMET, cl->ps.stats ) )
                upgrade = UP_HELMET;
            else if( bggame->InventoryContainsUpgrade( UP_LIGHTARMOUR, cl->ps.stats ) )
                upgrade = UP_LIGHTARMOUR;
            else
                upgrade = UP_NONE;
        }
        else
        {
            weapon = WP_NONE;
            upgrade = UP_NONE;
        }
        
        Q_vsprintf_s( entry, sizeof( entry ), sizeof( entry ),
                      " %d %d %d %d %d %d", level.sortedClients[ i ], cl->ps.persistant[ PERS_SCORE ],
                      ping, ( level.time - cl->pers.enterTime ) / 60000, weapon, upgrade );
                     
        j = strlen( entry );
        
        if( stringlength + j > 1024 )
            break;
            
        strcpy( string + stringlength, entry );
        stringlength += j;
    }
    
    trap_SendServerCommand( ent - g_entities, va( "scores %i %i %i%s", i,
                            level.alienKills, level.humanKills, string ) );
}


/*
==================
ConcatArgs
==================
*/
valueType* idSGameCmds::ConcatArgs( sint start )
{
    sint         i, c, tlen;
    static valueType line[ MAX_STRING_CHARS ];
    sint         len;
    valueType        arg[ MAX_STRING_CHARS ];
    
    len = 0;
    c = trap_Argc( );
    
    for( i = start; i < c; i++ )
    {
        trap_Argv( i, arg, sizeof( arg ) );
        tlen = strlen( arg );
        
        if( len + tlen >= MAX_STRING_CHARS - 1 )
            break;
            
        memcpy( line + len, arg, tlen );
        len += tlen;
        
        if( len == MAX_STRING_CHARS - 1 )
            break;
            
        if( i != c - 1 )
        {
            line[ len ] = ' ';
            len++;
        }
    }
    
    line[ len ] = 0;
    
    return line;
}


/*
==================
Cmd_Give_f

Give items to a client
==================
*/
void idSGameCmds::Cmd_Give_f( gentity_t* ent )
{
    valueType*      name;
    bool  give_all = false;
    
    name = ConcatArgs( 1 );
    if( Q_stricmp( name, "all" ) == 0 )
        give_all = true;
        
    if( give_all || Q_stricmp( name, "health" ) == 0 )
    {
        ent->health = ent->client->ps.stats[ STAT_MAX_HEALTH ];
        bggame->AddUpgradeToInventory( UP_MEDKIT, ent->client->ps.stats );
    }
    
    if( give_all || Q_stricmpn( name, "funds", 5 ) == 0 )
    {
        sint credits = atoi( name + 6 );
        
        if( ent->client->pers.teamSelection == TEAM_ALIENS )
            credits *= ALIEN_CREDITS_PER_FRAG;
            
        if( give_all )
            credits = ALIEN_MAX_CREDITS;
            
        idSGameClient::AddCreditToClient( ent->client, credits, true );
    }
    
    if( give_all || Q_stricmp( name, "stamina" ) == 0 )
        ent->client->ps.stats[ STAT_STAMINA ] = MAX_STAMINA;
        
    if( Q_stricmp( name, "poison" ) == 0 )
    {
        if( ent->client->pers.teamSelection == TEAM_HUMANS )
        {
            ent->client->ps.stats[ STAT_STATE ] |= SS_POISONED;
            ent->client->lastPoisonTime = level.time;
            ent->client->lastPoisonClient = ent;
        }
        else
        {
            ent->client->ps.stats[ STAT_STATE ] |= SS_BOOSTED;
            ent->client->boostedTime = level.time;
        }
    }
    
    if( give_all || Q_stricmp( name, "ammo" ) == 0 )
    {
        gclient_t* client = ent->client;
        
        if( client->ps.weapon != WP_ALEVEL3_UPG &&
                bggame->Weapon( ( weapon_t )client->ps.weapon )->infiniteAmmo )
            return;
            
        client->ps.ammo = bggame->Weapon( ( weapon_t )client->ps.weapon )->maxAmmo;
        client->ps.clips = bggame->Weapon( ( weapon_t )client->ps.weapon )->maxClips;
        
        if( bggame->Weapon( ( weapon_t )client->ps.weapon )->usesEnergy &&
                bggame->InventoryContainsUpgrade( UP_BATTPACK, client->ps.stats ) )
            client->ps.ammo = ( sint )( ( float32 )client->ps.ammo * BATTPACK_MODIFIER );
    }
}


/*
==================
Cmd_God_f

Sets client to godmode

argv(0) god
==================
*/
void idSGameCmds::Cmd_God_f( gentity_t* ent )
{
    valueType* msg;
    
    ent->flags ^= FL_GODMODE;
    
    if( !( ent->flags & FL_GODMODE ) )
        msg = "godmode OFF\n";
    else
        msg = "godmode ON\n";
        
    trap_SendServerCommand( ent - g_entities, va( "print \"%s\"", msg ) );
}


/*
==================
Cmd_Notarget_f

Sets client to notarget

argv(0) notarget
==================
*/
void idSGameCmds::Cmd_Notarget_f( gentity_t* ent )
{
    valueType*  msg;
    
    ent->flags ^= FL_NOTARGET;
    
    if( !( ent->flags & FL_NOTARGET ) )
        msg = "notarget OFF\n";
    else
        msg = "notarget ON\n";
        
    trap_SendServerCommand( ent - g_entities, va( "print \"%s\"", msg ) );
}


/*
==================
Cmd_Noclip_f

argv(0) noclip
==================
*/
void idSGameCmds::Cmd_Noclip_f( gentity_t* ent )
{
    valueType*  msg;
    
    if( ent->client->noclip )
        msg = "noclip OFF\n";
    else
        msg = "noclip ON\n";
        
    ent->client->noclip = !ent->client->noclip;
    
    trap_SendServerCommand( ent - g_entities, va( "print \"%s\"", msg ) );
}


/*
==================
Cmd_LevelShot_f

This is just to help generate the level pictures
for the menus.  It goes to the intermission immediately
and sends over a command to the client to resize the view,
hide the scoreboard, and take a special screenshot
==================
*/
void idSGameCmds::Cmd_LevelShot_f( gentity_t* ent )
{
    idSGameMain::BeginIntermission( );
    trap_SendServerCommand( ent - g_entities, "clientLevelShot" );
}

/*
=================
Cmd_Kill_f
=================
*/
void idSGameCmds::Cmd_Kill_f( gentity_t* ent )
{
    if( ent->client->ps.stats[ STAT_STATE ] & SS_HOVELING )
    {
        trap_SendServerCommand( ent - g_entities, "print \"Leave the hovel first (use your destroy key)\n\"" );
        return;
    }
    
    if( g_cheats.integer )
    {
        ent->flags &= ~FL_GODMODE;
        ent->client->ps.stats[ STAT_HEALTH ] = ent->health = 0;
        idSGameCombat::player_die( ent, ent, ent, 100000, MOD_SUICIDE );
    }
    else
    {
        if( ent->suicideTime == 0 )
        {
            trap_SendServerCommand( ent - g_entities, "print \"You will suicide in 20 seconds\n\"" );
            ent->suicideTime = level.time + 20000;
        }
        else if( ent->suicideTime > level.time )
        {
            trap_SendServerCommand( ent - g_entities, "print \"Suicide canceled\n\"" );
            ent->suicideTime = 0;
        }
    }
}

/*
=================
Cmd_Team_f
=================
*/
void idSGameCmds::Cmd_Team_f( gentity_t* ent )
{
    team_t    team, oldteam = ent->client->pers.teamSelection;
    valueType      s[ MAX_TOKEN_CHARS ];
    bool      force = adminLocal.AdminPermission( ent, ADMF_FORCETEAMCHANGE );
    sint       aliens = level.numAlienClients;
    sint       humans = level.numHumanClients;
    
    // stop team join spam
    if( level.time - ent->client->pers.teamChangeTime < 1000 )
        return;
        
    if( oldteam == TEAM_ALIENS )
        aliens--;
    else if( oldteam == TEAM_HUMANS )
        humans--;
        
    trap_Argv( 1, s, sizeof( s ) );
    
    if( !s[ 0 ] )
    {
        trap_SendServerCommand( ent - g_entities, va( "print \"team: %i\n\"",
                                oldteam ) );
        return;
    }
    
    if( !Q_stricmp( s, "auto" ) )
    {
        if( level.humanTeamLocked && level.alienTeamLocked )
            team = TEAM_NONE;
        else if( level.humanTeamLocked || humans > aliens )
            team = TEAM_ALIENS;
        else if( level.alienTeamLocked || aliens > humans )
            team = TEAM_HUMANS;
        else
            team = ( team_t )( TEAM_ALIENS + ( rand( ) % 2 ) );
    }
    else switch( idSGameTeam::TeamFromString( s ) )
        {
            case TEAM_NONE:
                team = TEAM_NONE;
                break;
                
            case TEAM_ALIENS:
                if( level.alienTeamLocked )
                {
                    trap_SendServerCommand( ent - g_entities,
                                            "print \"Alien team has been ^1LOCKED\n\"" );
                    return;
                }
                if( level.humanTeamLocked )
                    force = true;
                    
                if( !force && g_teamForceBalance.integer && aliens > humans )
                {
                    idSGameUtils::TriggerMenu( ent - g_entities, MN_A_TEAMFULL );
                    return;
                }
                
                team = TEAM_ALIENS;
                break;
                
            case TEAM_HUMANS:
                if( level.humanTeamLocked )
                {
                    trap_SendServerCommand( ent - g_entities,
                                            "print \"Human team has been ^1LOCKED\n\"" );
                    return;
                }
                if( level.alienTeamLocked )
                    force = true;
                    
                if( !force && g_teamForceBalance.integer && humans > aliens )
                {
                    idSGameUtils::TriggerMenu( ent - g_entities, MN_H_TEAMFULL );
                    return;
                }
                
                team = TEAM_HUMANS;
                break;
                
            default:
                trap_SendServerCommand( ent - g_entities,
                                        va( "print \"Unknown team: %s\n\"", s ) );
                return;
        }
        
    // stop team join spam
    if( oldteam == team )
        return;
        
    if( team != TEAM_NONE && g_maxGameClients.integer &&
            level.numPlayingClients >= g_maxGameClients.integer )
    {
        trap_SendServerCommand( ent - g_entities, va( "print \"The maximum number of "
                                "playing clients has been reached (g_maxGameClients = %d)\n\"",
                                g_maxGameClients.integer ) );
        return;
    }
    
    // guard against build timer exploit
    if( oldteam != TEAM_NONE && ent->client->sess.spectatorState == SPECTATOR_NOT &&
            ( ent->client->ps.stats[ STAT_CLASS ] == PCL_ALIEN_BUILDER0 ||
              ent->client->ps.stats[ STAT_CLASS ] == PCL_ALIEN_BUILDER0_UPG ||
              bggame->InventoryContainsWeapon( WP_HBUILD, ent->client->ps.stats ) ) &&
            ent->client->ps.stats[ STAT_MISC ] > 0 )
    {
        if( ent->client->pers.teamSelection == TEAM_ALIENS )
            idSGameUtils::TriggerMenu( ent->client->ps.clientNum, MN_A_TEAMCHANGEBUILDTIMER );
        else
            idSGameUtils::TriggerMenu( ent->client->ps.clientNum, MN_H_TEAMCHANGEBUILDTIMER );
        return;
    }
    
    // Apply the change
    idSGameTeam::ChangeTeam( ent, team );
}


/*
==================
G_Say
==================
*/
void idSGameCmds::SayTo( gentity_t* ent, gentity_t* other, sint mode, sint color, pointer name, pointer message )
{
    bool ignore = false;
    
    if( !other )
        return;
        
    if( !other->inuse )
        return;
        
    if( !other->client )
        return;
        
    if( other->client->pers.connected != CON_CONNECTED )
        return;
        
    if( mode == SAY_TEAM && !idSGameTeam::OnSameTeam( ent, other ) )
    {
        if( other->client->pers.teamSelection != TEAM_NONE )
            return;
            
        if( !adminLocal.AdminPermission( other, ADMF_SPEC_ALLCHAT ) )
            return;
            
        // specs with ADMF_SPEC_ALLCHAT flag can see team chat
    }
    
    if( bggame->ClientListTest( &other->client->sess.ignoreList, ent - g_entities ) )
        ignore = true;
        
    trap_SendServerCommand( other - g_entities, va( "%s \"%s%s%c%c%s%s\"",
                            mode == SAY_TEAM ? "tchat" : "chat",
                            ( ignore ) ? "[skipnotify]" : "",
                            name, Q_COLOR_ESCAPE, color, message, S_COLOR_WHITE ) );
}

void idSGameCmds::Say( gentity_t* ent, gentity_t* target, sint mode, pointer chatText )
{
    sint         j;
    gentity_t*   other;
    sint         color;
    pointer  prefix;
    valueType        name[ 64 ];
    // don't let text be too long for malicious reasons
    valueType        text[ MAX_SAY_TEXT ];
    valueType        location[ 64 ];
    
    if( g_chatTeamPrefix.integer )
    {
        prefix = bggame->TeamName( ent->client->pers.teamSelection );
        prefix = va( "[%c] ", toupper( *prefix ) );
    }
    else
        prefix = "";
        
    switch( mode )
    {
        default:
        case SAY_ALL:
            idSGameMain::LogPrintf( "say: %s^7: %s\n", ent->client->pers.netname, chatText );
            Q_vsprintf_s( name, sizeof( name ), sizeof( name ), "%s%s" S_COLOR_WHITE ": ", prefix,
                          ent->client->pers.netname );
            color = COLOR_GREEN;
            break;
            
        case SAY_TEAM:
            idSGameMain::LogPrintf( "sayteam: %s^7: %s\n", ent->client->pers.netname, chatText );
            if( idSGameTeam::Team_GetLocationMsg( ent, location, sizeof( location ) ) )
                Q_vsprintf_s( name, sizeof( name ), sizeof( name ), "(%s" S_COLOR_WHITE ") (%s): ",
                              ent->client->pers.netname, location );
            else
                Q_vsprintf_s( name, sizeof( name ), sizeof( name ), "(%s" S_COLOR_WHITE "): ",
                              ent->client->pers.netname );
            color = COLOR_CYAN;
            break;
            
        case SAY_TELL:
            if( target && idSGameTeam::OnSameTeam( target, ent ) &&
                    idSGameTeam::Team_GetLocationMsg( ent, location, sizeof( location ) ) )
                Q_vsprintf_s( name, sizeof( name ), sizeof( name ), "[%s" S_COLOR_WHITE "] (%s): ",
                              ent->client->pers.netname, location );
            else
                Q_vsprintf_s( name, sizeof( name ), sizeof( name ), "[%s" S_COLOR_WHITE "]: ",
                              ent->client->pers.netname );
            color = COLOR_MAGENTA;
            break;
    }
    
    Q_strncpyz( text, chatText, sizeof( text ) );
    
    if( target )
    {
        SayTo( ent, target, mode, color, name, text );
        return;
    }
    
    if( g_adminParseSay.integer )
    {
        if( adminLocal.AdminCmdCheck( ent, true ) )
        {
            return;
        }
    }
    
    // send it to all the apropriate clients
    for( j = 0; j < level.maxclients; j++ )
    {
        other = &g_entities[ j ];
        SayTo( ent, other, mode, color, name, text );
    }
}

void idSGameCmds::Cmd_SayArea_f( gentity_t* ent )
{
    sint    entityList[ MAX_GENTITIES ];
    sint    num, i;
    sint    color = COLOR_BLUE;
    pointer  prefix;
    vec3_t range = { 1000.0f, 1000.0f, 1000.0f };
    vec3_t mins, maxs;
    valueType*   msg = ConcatArgs( 1 );
    valueType   name[ 64 ];
    
    for( i = 0; i < 3; i++ )
        range[ i ] = g_sayAreaRange.value;
        
    if( g_chatTeamPrefix.integer )
    {
        prefix = bggame->TeamName( ent->client->pers.teamSelection );
        prefix = va( "[%c] ", toupper( *prefix ) );
    }
    else
        prefix = "";
        
    idSGameMain::LogPrintf( "sayarea: %s%s^7: %s\n", prefix, ent->client->pers.netname, msg );
    Q_vsprintf_s( name, sizeof( name ), sizeof( name ), "%s<%s%c%c> ", prefix, ent->client->pers.netname, Q_COLOR_ESCAPE, COLOR_WHITE );
    
    VectorAdd( ent->s.origin, range, maxs );
    VectorSubtract( ent->s.origin, range, mins );
    
    num = trap_EntitiesInBox( mins, maxs, entityList, MAX_GENTITIES );
    for( i = 0; i < num; i++ )
        SayTo( ent, &g_entities[ entityList[ i ] ], SAY_TEAM, color, name, msg );
        
    //Send to ADMF_SPEC_ALLCHAT candidates
    for( i = 0; i < level.maxclients; i++ )
    {
        if( ( &g_entities[ i ] )->client->pers.teamSelection == TEAM_NONE && adminLocal.AdminPermission( &g_entities[ i ], ADMF_SPEC_ALLCHAT ) )
        {
            SayTo( ent, &g_entities[ i ], SAY_TEAM, color, name, msg );
        }
    }
}

/*
==================
Cmd_Say_f
==================
*/
void idSGameCmds::Cmd_Say_f( gentity_t* ent )
{
    valueType*    p;
    valueType*    args;
    sint     mode = SAY_ALL;
    
    args = SayConcatArgs( 0 );
    if( Q_stricmpn( args, "say_team ", 9 ) == 0 )
        mode = SAY_TEAM;
        
    // support parsing /m out of say text since some people have a hard
    // time figuring out what the console is.
    if( !Q_stricmpn( args, "say /m ", 7 ) ||
            !Q_stricmpn( args, "say_team /m ", 12 ) ||
            !Q_stricmpn( args, "say /mt ", 8 ) ||
            !Q_stricmpn( args, "say_team /mt ", 13 ) )
    {
        PrivateMessage_f( ent );
        return;
    }
    
    // support parsing /a out of say text for the same reason
    if( !Q_stricmpn( args, "say /a ", 7 ) || !Q_stricmpn( args, "say_team /a ", 12 ) )
    {
        AdminMessage_f( ent );
        return;
    }
    
    if( g_allowShare.integer )
    {
        args = SayConcatArgs( 0 );
        if( !Q_stricmpn( args, "say /share", 10 ) ||
                !Q_stricmpn( args, "say_team /share", 15 ) )
        {
            Share_f( ent );
            return;
        }
    }
    
    if( trap_Argc( ) < 2 )
        return;
        
    p = ConcatArgs( 1 );
    
    if( strlen( p ) >= MAX_SAY_TEXT )
    {
        p[MAX_SAY_TEXT - 1] = '\0';
        idSGameMain::LogPrintf( "Cmd_Say_f from %d (%s) has been truncated: %s\n", ent->s.number, ent->client->pers.netname, p );
        return;
    }
    
    Say( ent, nullptr, mode, p );
}

/*
==================
Cmd_Tell_f
==================
*/
void idSGameCmds::Cmd_Tell_f( gentity_t* ent )
{
    sint     targetNum;
    gentity_t* target;
    valueType*    p;
    valueType    arg[MAX_TOKEN_CHARS];
    
    if( trap_Argc( ) < 2 )
        return;
        
    trap_Argv( 1, arg, sizeof( arg ) );
    targetNum = atoi( arg );
    
    if( targetNum < 0 || targetNum >= level.maxclients )
        return;
        
    target = &g_entities[ targetNum ];
    if( !target || !target->inuse || !target->client )
        return;
        
    p = ConcatArgs( 2 );
    
    idSGameMain::LogPrintf( "tell: %s^7 to %s^7: %s\n", ent->client->pers.netname, target->client->pers.netname, p );
    Say( ent, target, SAY_TELL, p );
}

/*
==================
Cmd_VSay_f
==================
*/
void idSGameCmds::Cmd_VSay_f( gentity_t* ent )
{
    valueType            arg[MAX_TOKEN_CHARS];
    voiceChannel_t  vchan;
    voice_t*         voice;
    voiceCmd_t*      cmd;
    voiceTrack_t*    track;
    sint             cmdNum = 0;
    sint             trackNum = 0;
    valueType            voiceName[ MAX_VOICE_NAME_LEN ] = {"default"};
    valueType            voiceCmd[ MAX_VOICE_CMD_LEN ] = {""};
    valueType            vsay[ 12 ] = {""};
    weapon_t        weapon;
    
    if( !ent || !ent->client )
        Com_Error( ERR_FATAL, "Cmd_VSay_f() called by non-client entity\n" );
        
    trap_Argv( 0, arg, sizeof( arg ) );
    if( trap_Argc( ) < 2 )
    {
        trap_SendServerCommand( ent - g_entities, va( "print \"usage: %s command [text] \n\"", arg ) );
        return;
    }
    if( !level.voices )
    {
        trap_SendServerCommand( ent - g_entities, va( "print \"%s: voice system is not installed on this server\n\"", arg ) );
        return;
    }
    if( !g_voiceChats.integer )
    {
        trap_SendServerCommand( ent - g_entities, va( "print \"%s: voice system administratively disabled on this server\n\"", arg ) );
        return;
    }
    if( !Q_stricmp( arg, "vsay" ) )
        vchan = VOICE_CHAN_ALL;
    else if( !Q_stricmp( arg, "vsay_team" ) )
        vchan = VOICE_CHAN_TEAM;
    else if( !Q_stricmp( arg, "vsay_local" ) )
        vchan = VOICE_CHAN_LOCAL;
    else
        return;
    Q_strncpyz( vsay, arg, sizeof( vsay ) );
    
    if( ent->client->pers.voice[ 0 ] )
        Q_strncpyz( voiceName, ent->client->pers.voice, sizeof( voiceName ) );
    voice = bggame->VoiceByName( level.voices, voiceName );
    if( !voice )
    {
        trap_SendServerCommand( ent - g_entities, va(
                                    "print \"%s: voice '%s' not found\n\"", vsay, voiceName ) );
        return;
    }
    
    trap_Argv( 1, voiceCmd, sizeof( voiceCmd ) ) ;
    cmd = bggame->VoiceCmdFind( voice->cmds, voiceCmd, &cmdNum );
    if( !cmd )
    {
        trap_SendServerCommand( ent - g_entities, va(
                                    "print \"%s: command '%s' not found in voice '%s'\n\"",
                                    vsay, voiceCmd, voiceName ) );
        return;
    }
    
    // filter non-spec humans by their primary weapon as well
    weapon = WP_NONE;
    if( ent->client->sess.spectatorState == SPECTATOR_NOT )
    {
        weapon = bggame->PrimaryWeapon( ent->client->ps.stats );
    }
    
    track = bggame->VoiceTrackFind( cmd->tracks, ent->client->pers.teamSelection,
                                    ent->client->pers.classSelection, weapon, ( sint )ent->client->voiceEnthusiasm,
                                    &trackNum );
    if( !track )
    {
        trap_SendServerCommand( ent - g_entities, va(
                                    "print \"%s: no available track for command '%s', team %d, "
                                    "class %d, weapon %d, and enthusiasm %d in voice '%s'\n\"",
                                    vsay, voiceCmd, ent->client->pers.teamSelection,
                                    ent->client->pers.classSelection, weapon,
                                    ( sint )ent->client->voiceEnthusiasm, voiceName ) );
        return;
    }
    
    if( !Q_stricmp( ent->client->lastVoiceCmd, cmd->cmd ) )
        ent->client->voiceEnthusiasm++;
        
    Q_strncpyz( ent->client->lastVoiceCmd, cmd->cmd,
                sizeof( ent->client->lastVoiceCmd ) );
                
    // optional user supplied text
    trap_Argv( 2, arg, sizeof( arg ) );
    
    switch( vchan )
    {
        case VOICE_CHAN_ALL:
        case VOICE_CHAN_LOCAL:
            trap_SendServerCommand( -1, va(
                                        "voice %d %d %d %d \"%s\"\n",
                                        ( sint )( ent - g_entities ), vchan, cmdNum, trackNum, arg ) );
            break;
        case VOICE_CHAN_TEAM:
            idSGameUtils::TeamCommand( ent->client->pers.teamSelection, va(
                                           "voice %d %d %d %d \"%s\"\n",
                                           ( sint )( ent - g_entities ), vchan, cmdNum, trackNum, arg ) );
            break;
        default:
            break;
    }
}

/*
==================
Cmd_Where_f
==================
*/
void idSGameCmds::Cmd_Where_f( gentity_t* ent )
{
    if( !ent->client )
        return;
    trap_SendServerCommand( ent - g_entities, va( "print \"origin: %f %f %f\n\"", ent->s.origin[ 0 ], ent->s.origin[ 1 ], ent->s.origin[ 2 ] ) );
}

/*
==================
Cmd_CallVote_f
==================
*/
void idSGameCmds::Cmd_CallVote_f( gentity_t* ent )
{
    sint   i;
    valueType  arg1[ MAX_STRING_TOKENS ];
    valueType  arg2[ MAX_NAME_LENGTH ];
    sint   clientNum = -1;
    valueType  name[ MAX_NAME_LENGTH ];
    
    if( !g_allowVote.integer )
    {
        trap_SendServerCommand( ent - g_entities, "print \"Voting not allowed here\n\"" );
        return;
    }
    
    if( level.voteTime )
    {
        trap_SendServerCommand( ent - g_entities, "print \"A vote is already in progress\n\"" );
        return;
    }
    
    if( g_voteLimit.integer > 0 && ent->client->pers.voteCount >= g_voteLimit.integer && !adminLocal.AdminPermission( ent, ADMF_NO_VOTE_LIMIT ) )
    {
        trap_SendServerCommand( ent - g_entities, va( "print \"You have already called the maximum number of votes (%d)\n\"", g_voteLimit.integer ) );
        return;
    }
    
    // make sure it is a valid command to vote on
    trap_Argv( 1, arg1, sizeof( arg1 ) );
    trap_Argv( 2, arg2, sizeof( arg2 ) );
    
    if( strchr( arg1, ';' ) || strchr( arg2, ';' ) )
    {
        trap_SendServerCommand( ent - g_entities, "print \"Invalid vote string\n\"" );
        return;
    }
    
    // if there is still a vote to be executed
    if( level.voteExecuteTime )
    {
        level.voteExecuteTime = 0;
        trap_SendConsoleCommand( EXEC_APPEND, va( "%s\n", level.voteString ) );
    }
    
    level.votePassThreshold = 50;
    
    // detect clientNum for partial name match votes
    if( !Q_stricmp( arg1, "kick" ) ||
            !Q_stricmp( arg1, "mute" ) ||
            !Q_stricmp( arg1, "unmute" ) )
    {
        sint clientNums[ MAX_CLIENTS ];
        
        if( !arg2[ 0 ] )
        {
            trap_SendServerCommand( ent - g_entities, "print \"callvote: no target\n\"" );
            return;
        }
        
        if( ClientNumbersFromString( arg2, clientNums, MAX_CLIENTS ) == 1 )
        {
            // there was only one partial name match
            clientNum = clientNums[ 0 ];
        }
        else
        {
            // look for an exact name match (sets clientNum to -1 if it fails)
            clientNum = ClientNumberFromString( arg2 );
        }
        
        if( clientNum != -1 )
        {
            Q_strncpyz( name, level.clients[ clientNum ].pers.netname,
                        sizeof( name ) );
            Q_CleanStr( name );
        }
        else
        {
            trap_SendServerCommand( ent - g_entities,
                                    "print \"callvote: invalid player\n\"" );
            return;
        }
    }
    
    if( !Q_stricmp( arg1, "kick" ) )
    {
        if( adminLocal.AdminPermission( &g_entities[ clientNum ], ADMF_IMMUNITY ) )
        {
            trap_SendServerCommand( ent - g_entities, "print \"callvote: admin is immune from vote kick\n\"" );
            return;
        }
        if( level.clients[ clientNum ].pers.localClient )
        {
            trap_SendServerCommand( ent - g_entities, "print \"callvote: host is immute from vote kick\n\"" );
            return;
        }
        
        // use ip in case this player disconnects before the vote ends
        Q_vsprintf_s( level.voteString, sizeof( level.voteString ), sizeof( level.voteString ),
                      "!ban %s \"1s%s\" vote kick", level.clients[ clientNum ].pers.ip,
                      g_adminTempBan.string );
        Q_vsprintf_s( level.voteDisplayString, sizeof( level.voteDisplayString ), sizeof( level.voteDisplayString ),
                      "Kick player \'%s\'", name );
    }
    else if( !Q_stricmp( arg1, "mute" ) )
    {
        if( level.clients[ clientNum ].pers.muted )
        {
            trap_SendServerCommand( ent - g_entities,
                                    "print \"callvote: player is already muted\n\"" );
            return;
        }
        
        if( adminLocal.AdminPermission( &g_entities[ clientNum ], ADMF_IMMUNITY ) )
        {
            trap_SendServerCommand( ent - g_entities, "print \"callvote: admin is immune from vote mute\n\"" );
            return;
        }
        Q_vsprintf_s( level.voteString, sizeof( level.voteString ), sizeof( level.voteString ),
                      "!mute %i", clientNum );
        Q_vsprintf_s( level.voteDisplayString, sizeof( level.voteDisplayString ), sizeof( level.voteDisplayString ),
                      "Mute player \'%s\'", name );
    }
    else if( !Q_stricmp( arg1, "unmute" ) )
    {
        if( !level.clients[ clientNum ].pers.muted )
        {
            trap_SendServerCommand( ent - g_entities,
                                    "print \"callvote: player is not currently muted\n\"" );
            return;
        }
        Q_vsprintf_s( level.voteString, sizeof( level.voteString ), sizeof( level.voteString ),
                      "!unmute %i", clientNum );
        Q_vsprintf_s( level.voteDisplayString, sizeof( level.voteDisplayString ), sizeof( level.voteDisplayString ),
                      "Un-Mute player \'%s\'", name );
    }
    else if( !Q_stricmp( arg1, "map_restart" ) )
    {
        Q_vsprintf_s( level.voteString, sizeof( level.voteString ), sizeof( level.voteString ), "%s", arg1 );
        Q_vsprintf_s( level.voteDisplayString, sizeof( level.voteDisplayString ),
                      sizeof( level.voteDisplayString ), "Restart current map" );
    }
    else if( !Q_stricmp( arg1, "map" ) )
    {
        if( !trap_FS_FOpenFile( va( "maps/%s.bsp", arg2 ), nullptr, FS_READ ) )
        {
            trap_SendServerCommand( ent - g_entities, va( "print \"callvote: "
                                    "'maps/%s.bsp' could not be found on the server\n\"", arg2 ) );
            return;
        }
        
        Q_vsprintf_s( level.voteString, sizeof( level.voteString ), sizeof( level.voteString ), "%s %s", arg1, arg2 );
        Q_vsprintf_s( level.voteDisplayString, sizeof( level.voteDisplayString ),
                      sizeof( level.voteDisplayString ), "Change to map '%s'", arg2 );
    }
    else if( !Q_stricmp( arg1, "draw" ) )
    {
        Q_vsprintf_s( level.voteString, sizeof( level.voteString ), sizeof( level.voteString ), "evacuation" );
        Q_vsprintf_s( level.voteDisplayString, sizeof( level.voteDisplayString ), sizeof( level.voteDisplayString ),
                      "End match in a draw" );
    }
    else if( !Q_stricmp( arg1, "sudden_death" ) ||
             !Q_stricmp( arg1, "suddendeath" ) )
    {
        if( !g_suddenDeathVotePercent.integer )
        {
            trap_SendServerCommand( ent - g_entities,
                                    "print \"Sudden Death votes have been disabled\n\"" );
            return;
        }
        else if( g_suddenDeath.integer )
        {
            trap_SendServerCommand( ent - g_entities,
                                    va( "print \"callvote: Sudden Death has already begun\n\"" ) );
            return;
        }
        else if( idSGameMain::TimeTilSuddenDeath() <= g_suddenDeathVoteDelay.integer * 1000 )
        {
            trap_SendServerCommand( ent - g_entities,
                                    va( "print \"callvote: Sudden Death is already immenent\n\"" ) );
            return;
        }
        else
        {
            level.votePassThreshold = g_suddenDeathVotePercent.integer;
            Q_vsprintf_s( level.voteString, sizeof( level.voteString ), sizeof( level.voteString ), "suddendeath" );
            Q_vsprintf_s( level.voteDisplayString, sizeof( level.voteDisplayString ),
                          sizeof( level.voteDisplayString ), "Begin sudden death" );
                         
            if( g_suddenDeathVoteDelay.integer )
                Q_strcat( level.voteDisplayString, sizeof( level.voteDisplayString ),
                          va( " in %d seconds", g_suddenDeathVoteDelay.integer ) );
                          
        }
    }
    else
    {
        trap_SendServerCommand( ent - g_entities, "print \"Invalid vote string\n\"" );
        trap_SendServerCommand( ent - g_entities, "print \"Valid vote commands are: "
                                "map, map_restart, sudden_death, draw, kick, mute and unmute\n" );
        return;
    }
    
    if( level.votePassThreshold != 50 )
    {
        Q_strcat( level.voteDisplayString, sizeof( level.voteDisplayString ),
                  va( " (Needs > %d percent)", level.votePassThreshold ) );
    }
    
    trap_SendServerCommand( -1, va( "print \"%s" S_COLOR_WHITE
                                    " called a vote\n\"", ent->client->pers.netname ) );
    idSGameMain::Printf( "'%s' called a vote for '%s'\n", ent->client->pers.netname,
                         level.voteString ) ;
                         
    ent->client->pers.voteCount++;
    
    // start the voting, the caller autoamtically votes yes
    level.voteTime = level.time;
    level.voteYes = 1;
    level.voteNo = 0;
    ent->client->pers.vote = true;
    
    for( i = 0; i < level.maxclients; i++ )
        level.clients[i].ps.eFlags &= ~EF_VOTED;
        
    ent->client->ps.eFlags |= EF_VOTED;
    
    trap_SetConfigstring( CS_VOTE_TIME, va( "%i", level.voteTime ) );
    trap_SetConfigstring( CS_VOTE_STRING, level.voteDisplayString );
    trap_SetConfigstring( CS_VOTE_YES, "1" );
    trap_SetConfigstring( CS_VOTE_NO, "0" );
}

/*
==================
Cmd_Vote_f
==================
*/
void idSGameCmds::Cmd_Vote_f( gentity_t* ent )
{
    valueType msg[ 64 ];
    
    if( !level.voteTime )
    {
        trap_SendServerCommand( ent - g_entities, "print \"No vote in progress\n\"" );
        return;
    }
    
    if( ent->client->ps.eFlags & EF_VOTED )
    {
        trap_SendServerCommand( ent - g_entities, "print \"Vote already cast\n\"" );
        return;
    }
    
    trap_SendServerCommand( ent - g_entities, "print \"Vote cast\n\"" );
    
    trap_Argv( 1, msg, sizeof( msg ) );
    ent->client->pers.vote = ( tolower( msg[ 0 ] ) == 'y' || msg[ 0 ] == '1' );
    idSGameMain::Vote( ent, true );
    
    // a majority will be determined in idSGameCmds::CheckVote, which will also account
    // for players entering or leaving
}

/*
==================
Cmd_CallTeamVote_f
==================
*/
void idSGameCmds::Cmd_CallTeamVote_f( gentity_t* ent )
{
    sint   i, team, cs_offset = 0;
    valueType  arg1[ MAX_STRING_TOKENS ];
    valueType  arg2[ MAX_NAME_LENGTH ];
    sint   clientNum = -1;
    valueType  name[ MAX_NAME_LENGTH ];
    
    team = ent->client->pers.teamSelection;
    
    if( team == TEAM_ALIENS )
        cs_offset = 1;
        
    if( !g_allowVote.integer )
    {
        trap_SendServerCommand( ent - g_entities, "print \"Voting not allowed here\n\"" );
        return;
    }
    
    if( level.teamVoteTime[ cs_offset ] )
    {
        trap_SendServerCommand( ent - g_entities, "print \"A team vote is already in progress\n\"" );
        return;
    }
    
    if( g_voteLimit.integer > 0 && ent->client->pers.voteCount >= g_voteLimit.integer && !adminLocal.AdminPermission( ent, ADMF_NO_VOTE_LIMIT ) )
    {
        trap_SendServerCommand( ent - g_entities, va( "print \"You have already called the maximum number of votes (%d)\n\"", g_voteLimit.integer ) );
        return;
    }
    
    // make sure it is a valid command to vote on
    trap_Argv( 1, arg1, sizeof( arg1 ) );
    trap_Argv( 2, arg2, sizeof( arg2 ) );
    
    if( strchr( arg1, ';' ) || strchr( arg2, ';' ) )
    {
        trap_SendServerCommand( ent - g_entities, "print \"Invalid team vote string\n\"" );
        return;
    }
    
    // detect clientNum for partial name match votes
    if( !Q_stricmp( arg1, "kick" ) ||
            !Q_stricmp( arg1, "denybuild" ) ||
            !Q_stricmp( arg1, "allowbuild" ) )
    {
        sint clientNums[ MAX_CLIENTS ];
        
        if( !arg2[ 0 ] )
        {
            trap_SendServerCommand( ent - g_entities,
                                    "print \"callteamvote: no target\n\"" );
            return;
        }
        
        if( ClientNumbersFromString( arg2, clientNums, MAX_CLIENTS ) == 1 )
        {
            // there was only one partial name match
            clientNum = clientNums[ 0 ];
        }
        else
        {
            // look for an exact name match (sets clientNum to -1 if it fails)
            clientNum = ClientNumberFromString( arg2 );
        }
        
        // make sure this player is on the same team
        if( clientNum != -1 && level.clients[ clientNum ].pers.teamSelection !=
                team )
        {
            clientNum = -1;
        }
        
        if( clientNum != -1 )
        {
            Q_strncpyz( name, level.clients[ clientNum ].pers.netname,
                        sizeof( name ) );
            Q_CleanStr( name );
        }
        else
        {
            trap_SendServerCommand( ent - g_entities, "print \"callteamvote: invalid player\n\"" );
            return;
        }
    }
    
    if( !Q_stricmp( arg1, "kick" ) )
    {
        if( adminLocal.AdminPermission( &g_entities[ clientNum ], ADMF_IMMUNITY ) )
        {
            trap_SendServerCommand( ent - g_entities, "print \"callteamvote: admin is immune from vote kick\n\"" );
            return;
        }
        if( level.clients[ clientNum ].pers.localClient )
        {
            trap_SendServerCommand( ent - g_entities, "print \"callteamvote: host is immune from vote kick\n\"" );
            return;
        }
        
        // use ip in case this player disconnects before the vote ends
        Q_vsprintf_s( level.teamVoteString[ cs_offset ], sizeof( level.teamVoteString[cs_offset] ),
                      sizeof( level.teamVoteString[ cs_offset ] ),
                      "!ban %s \"1s%s\" team vote kick", level.clients[ clientNum ].pers.ip,
                      g_adminTempBan.string );
        Q_vsprintf_s( level.teamVoteDisplayString[ cs_offset ], sizeof( level.teamVoteDisplayString[cs_offset] ),
                      sizeof( level.teamVoteDisplayString[ cs_offset ] ),
                      "Kick player '%s'", name );
    }
    else if( !Q_stricmp( arg1, "denybuild" ) )
    {
        if( level.clients[ clientNum ].pers.denyBuild )
        {
            trap_SendServerCommand( ent - g_entities, "print \"callteamvote: player already lost building rights\n\"" );
            return;
        }
        
        if( adminLocal.AdminPermission( &g_entities[ clientNum ], ADMF_IMMUNITY ) )
        {
            trap_SendServerCommand( ent - g_entities, "print \"callteamvote: admin is immune from denybuild\n\"" );
            return;
        }
        
        Q_vsprintf_s( level.teamVoteString[ cs_offset ], sizeof( level.teamVoteString[cs_offset] ),
                      sizeof( level.teamVoteString[ cs_offset ] ), "!denybuild %i", clientNum );
        Q_vsprintf_s( level.teamVoteDisplayString[ cs_offset ], sizeof( level.teamVoteDisplayString[cs_offset] ),
                      sizeof( level.teamVoteDisplayString[ cs_offset ] ),
                      "Take away building rights from '%s'", name );
    }
    else if( !Q_stricmp( arg1, "allowbuild" ) )
    {
        if( !level.clients[ clientNum ].pers.denyBuild )
        {
            trap_SendServerCommand( ent - g_entities,
                                    "print \"callteamvote: player already has building rights\n\"" );
            return;
        }
        
        Q_vsprintf_s( level.teamVoteString[ cs_offset ], sizeof( level.teamVoteString[cs_offset] ),
                      sizeof( level.teamVoteString[ cs_offset ] ), "!allowbuild %i", clientNum );
        Q_vsprintf_s( level.teamVoteDisplayString[ cs_offset ], sizeof( level.teamVoteDisplayString[cs_offset] ),
                      sizeof( level.teamVoteDisplayString[ cs_offset ] ),
                      "Allow '%s' to build", name );
    }
    else if( !Q_stricmp( arg1, "admitdefeat" ) )
    {
        if( team == level.surrenderTeam )
        {
            trap_SendServerCommand( ent - g_entities, "print \"You have already surrendered\n\"" );
            return;
        }
        Q_vsprintf_s( level.teamVoteString[ cs_offset ], sizeof( level.teamVoteString[cs_offset] ),
                      sizeof( level.teamVoteString[ cs_offset ] ), "admitdefeat %i", team );
        Q_vsprintf_s( level.teamVoteDisplayString[ cs_offset ], sizeof( level.teamVoteDisplayString[cs_offset] ),
                      sizeof( level.teamVoteDisplayString[ cs_offset ] ),
                      "Admit Defeat" );
    }
    else
    {
        trap_SendServerCommand( ent - g_entities, "print \"Invalid vote string\n\"" );
        trap_SendServerCommand( ent - g_entities,
                                "print \"Valid team vote commands are: "
                                "kick, denybuild, allowbuild and admitdefeat\n\"" );
        return;
    }
    ent->client->pers.voteCount++;
    
    idSGameUtils::TeamCommand( ( team_t )team, va( "print \"%s " S_COLOR_WHITE "called a team vote\n\"",
                               ent->client->pers.netname ) );
                               
    idSGameMain::Printf( "'%s' called a teamvote for '%s'\n", ent->client->pers.netname, level.teamVoteString[ cs_offset ] ) ;
    
    // start the voting, the caller autoamtically votes yes
    level.teamVoteTime[ cs_offset ] = level.time;
    level.teamVoteYes[ cs_offset ] = 1;
    level.teamVoteNo[ cs_offset ] = 0;
    ent->client->pers.teamVote = true;
    
    for( i = 0; i < level.maxclients; i++ )
    {
        if( level.clients[ i ].ps.stats[ STAT_TEAM ] == team )
            level.clients[ i ].ps.eFlags &= ~EF_TEAMVOTED;
    }
    
    ent->client->ps.eFlags |= EF_TEAMVOTED;
    
    trap_SetConfigstring( CS_TEAMVOTE_TIME + cs_offset,
                          va( "%i", level.teamVoteTime[ cs_offset ] ) );
    trap_SetConfigstring( CS_TEAMVOTE_STRING + cs_offset,
                          level.teamVoteDisplayString[ cs_offset ] );
    trap_SetConfigstring( CS_TEAMVOTE_YES + cs_offset, "1" );
    trap_SetConfigstring( CS_TEAMVOTE_NO + cs_offset, "0" );
}


/*
==================
Cmd_TeamVote_f
==================
*/
void idSGameCmds::Cmd_TeamVote_f( gentity_t* ent )
{
    sint     cs_offset = 0;
    valueType    msg[ 64 ];
    
    if( ent->client->pers.teamSelection == TEAM_ALIENS )
        cs_offset = 1;
        
    if( !level.teamVoteTime[ cs_offset ] )
    {
        trap_SendServerCommand( ent - g_entities, "print \"No team vote in progress\n\"" );
        return;
    }
    
    if( ent->client->ps.eFlags & EF_TEAMVOTED )
    {
        trap_SendServerCommand( ent - g_entities, "print \"Team vote already cast\n\"" );
        return;
    }
    
    trap_SendServerCommand( ent - g_entities, "print \"Team vote cast\n\"" );
    
    trap_Argv( 1, msg, sizeof( msg ) );
    ent->client->pers.teamVote = ( tolower( msg[ 0 ] ) == 'y' || msg[ 0 ] == '1' );
    idSGameMain::TeamVote( ent, true );
    
    // a majority will be determined in idSGameCmds::CheckTeamVote, which will also account
    // for players entering or leaving
}

/*
=================
Cmd_SetViewpos_f
=================
*/
void idSGameCmds::Cmd_SetViewpos_f( gentity_t* ent )
{
    vec3_t  origin, angles;
    valueType    buffer[ MAX_TOKEN_CHARS ];
    sint     i;
    
    if( trap_Argc( ) != 5 )
    {
        trap_SendServerCommand( ent - g_entities, "print \"usage: setviewpos x y z yaw\n\"" );
        return;
    }
    
    VectorClear( angles );
    
    for( i = 0; i < 3; i++ )
    {
        trap_Argv( i + 1, buffer, sizeof( buffer ) );
        origin[ i ] = atof( buffer );
    }
    
    trap_Argv( 4, buffer, sizeof( buffer ) );
    angles[ YAW ] = atof( buffer );
    
    idSGameMisc::TeleportPlayer( ent, origin, angles );
}

/*
=================
idSGameCmds::Cmd_Stats_f
=================
*/
void idSGameCmds::Cmd_Stats_f( gentity_t* ent )
{
}

#define AS_OVER_RT3         ((ALIENSENSE_RANGE*0.5f)/M_ROOT3)

bool idSGameCmds::RoomForClassChange( gentity_t* ent, class_t _class, vec3_t newOrigin )
{
    vec3_t    fromMins, fromMaxs;
    vec3_t    toMins, toMaxs;
    vec3_t    temp;
    trace_t   tr;
    float32     nudgeHeight;
    float32     maxHorizGrowth;
    class_t   oldClass = ( class_t )ent->client->ps.stats[ STAT_CLASS ];
    
    bggame->ClassBoundingBox( oldClass, fromMins, fromMaxs, nullptr, nullptr, nullptr );
    bggame->ClassBoundingBox( _class, toMins, toMaxs, nullptr, nullptr, nullptr );
    
    VectorCopy( ent->s.origin, newOrigin );
    
    // find max x/y diff
    maxHorizGrowth = toMaxs[ 0 ] - fromMaxs[ 0 ];
    if( toMaxs[ 1 ] - fromMaxs[ 1 ] > maxHorizGrowth )
        maxHorizGrowth = toMaxs[ 1 ] - fromMaxs[ 1 ];
    if( toMins[ 0 ] - fromMins[ 0 ] > -maxHorizGrowth )
        maxHorizGrowth = -( toMins[ 0 ] - fromMins[ 0 ] );
    if( toMins[ 1 ] - fromMins[ 1 ] > -maxHorizGrowth )
        maxHorizGrowth = -( toMins[ 1 ] - fromMins[ 1 ] );
        
    if( maxHorizGrowth > 0.0f )
    {
        // test by moving the player up the max required on a 60 degree slope
        nudgeHeight = maxHorizGrowth * 2.0f;
    }
    else
    {
        // player is shrinking, so there's no need to nudge them upwards
        nudgeHeight = 0.0f;
    }
    
    // find what the new origin would be on a level surface
    newOrigin[ 2 ] -= toMins[ 2 ] - fromMins[ 2 ];
    
    //compute a place up in the air to start the real trace
    VectorCopy( newOrigin, temp );
    temp[ 2 ] += nudgeHeight;
    trap_Trace( &tr, newOrigin, toMins, toMaxs, temp, ent->s.number, MASK_PLAYERSOLID );
    
    //trace down to the ground so that we can evolve on slopes
    VectorCopy( newOrigin, temp );
    temp[ 2 ] += ( nudgeHeight * tr.fraction );
    trap_Trace( &tr, temp, toMins, toMaxs, newOrigin, ent->s.number, MASK_PLAYERSOLID );
    VectorCopy( tr.endpos, newOrigin );
    
    //make REALLY sure
    trap_Trace( &tr, newOrigin, toMins, toMaxs, newOrigin,
                ent->s.number, MASK_PLAYERSOLID );
                
    //check there is room to evolve
    return ( !tr.startsolid && tr.fraction == 1.0f );
}

/*
=================
Cmd_Class_f
=================
*/
void idSGameCmds::Cmd_Class_f( gentity_t* ent )
{
    valueType      s[ MAX_TOKEN_CHARS ];
    sint       clientNum;
    sint       i;
    vec3_t    infestOrigin;
    class_t   currentClass = ent->client->pers.classSelection;
    class_t   newClass;
    sint       entityList[ MAX_GENTITIES ];
    vec3_t    range = { AS_OVER_RT3, AS_OVER_RT3, AS_OVER_RT3 };
    vec3_t    mins, maxs;
    sint       num;
    gentity_t* other;
    sint       oldBoostTime = -1;
    vec3_t    oldVel;
    
    clientNum = ent->client - level.clients;
    trap_Argv( 1, s, sizeof( s ) );
    newClass = bggame->ClassByName( s )->number;
    
    if( ent->client->sess.spectatorState != SPECTATOR_NOT )
    {
        if( ent->client->sess.spectatorState == SPECTATOR_FOLLOW )
            StopFollowing( ent );
        if( ent->client->pers.teamSelection == TEAM_ALIENS )
        {
            if( newClass != PCL_ALIEN_BUILDER0 &&
                    newClass != PCL_ALIEN_BUILDER0_UPG &&
                    newClass != PCL_ALIEN_LEVEL0 )
            {
                idSGameUtils::TriggerMenu2( ent->client->ps.clientNum, MN_A_CLASSNOTSPAWN, newClass );
                return;
            }
            
            if( !bggame->ClassIsAllowed( newClass ) )
            {
                idSGameUtils::TriggerMenu2( ent->client->ps.clientNum, MN_A_CLASSNOTALLOWED, newClass );
                return;
            }
            
            if( !bggame->ClassAllowedInStage( newClass, ( stage_t )g_alienStage.integer ) )
            {
                idSGameUtils::TriggerMenu2( ent->client->ps.clientNum, MN_A_CLASSNOTATSTAGE, newClass );
                return;
            }
            
            // spawn from an egg
            if( idSGameMain::PushSpawnQueue( &level.alienSpawnQueue, clientNum ) )
            {
                ent->client->pers.classSelection = newClass;
                ent->client->ps.stats[ STAT_CLASS ] = newClass;
            }
        }
        else if( ent->client->pers.teamSelection == TEAM_HUMANS )
        {
            //set the item to spawn with
            if( !Q_stricmp( s, bggame->Weapon( WP_MACHINEGUN )->name ) &&
                    bggame->WeaponIsAllowed( WP_MACHINEGUN ) )
            {
                ent->client->pers.humanItemSelection = WP_MACHINEGUN;
            }
            else if( !Q_stricmp( s, bggame->Weapon( WP_HBUILD )->name ) &&
                     bggame->WeaponIsAllowed( WP_HBUILD ) )
            {
                ent->client->pers.humanItemSelection = WP_HBUILD;
            }
            else
            {
                idSGameUtils::TriggerMenu( ent->client->ps.clientNum, MN_H_UNKNOWNSPAWNITEM );
                return;
            }
            // spawn from a telenode
            if( idSGameMain::PushSpawnQueue( &level.humanSpawnQueue, clientNum ) )
            {
                ent->client->pers.classSelection = PCL_HUMAN;
                ent->client->ps.stats[ STAT_CLASS ] = PCL_HUMAN;
            }
        }
        return;
    }
    
    if( ent->health <= 0 )
        return;
        
    if( ent->client->pers.teamSelection == TEAM_ALIENS )
    {
        if( newClass == PCL_NONE )
        {
            idSGameUtils::TriggerMenu( ent->client->ps.clientNum, MN_A_UNKNOWNCLASS );
            return;
        }
        
        //if we are not currently spectating, we are attempting evolution
        if( ent->client->pers.classSelection != PCL_NONE )
        {
            sint cost;
            
            if( !level.overmindPresent )
            {
                idSGameUtils::TriggerMenu( clientNum, MN_A_NOOVMND_EVOLVE );
                return;
            }
            
            //check there are no humans nearby
            VectorAdd( ent->client->ps.origin, range, maxs );
            VectorSubtract( ent->client->ps.origin, range, mins );
            
            num = trap_EntitiesInBox( mins, maxs, entityList, MAX_GENTITIES );
            for( i = 0; i < num; i++ )
            {
                other = &g_entities[ entityList[ i ] ];
                
                //LEPE: only trigger if near Human base, not near humans
                //if( ( other->client && other->client->ps.stats[ STAT_TEAM ] == TEAM_HUMANS ) ||
                if( other->s.eType == ET_BUILDABLE && other->buildableTeam == TEAM_HUMANS &&
                        other->powered )
                {
                    idSGameUtils::TriggerMenu( clientNum, MN_A_TOOCLOSE );
                    return;
                }
            }
            
            //check that we are not wallwalking
            if( ent->client->ps.eFlags & EF_WALLCLIMB )
            {
                idSGameUtils::TriggerMenu( clientNum, MN_A_EVOLVEWALLWALK );
                return;
            }
            
            //guard against selling the HBUILD weapons exploit
            if( ent->client->sess.spectatorState == SPECTATOR_NOT &&
                    ( currentClass == PCL_ALIEN_BUILDER0 ||
                      currentClass == PCL_ALIEN_BUILDER0_UPG ) &&
                    ent->client->ps.stats[ STAT_MISC ] > 0 )
            {
                idSGameUtils::TriggerMenu( ent->client->ps.clientNum, MN_A_EVOLVEBUILDTIMER );
                return;
            }
            
            cost = bggame->ClassCanEvolveFromTo( currentClass, newClass,
                                                 ent->client->ps.persistant[ PERS_CREDIT ],
                                                 g_alienStage.integer, 0 );
                                                 
            if( RoomForClassChange( ent, newClass, infestOrigin ) )
            {
                if( cost >= 0 )
                {
                    ent->client->pers.evolveHealthFraction = ( float32 )ent->client->ps.stats[ STAT_HEALTH ] /
                            ( float32 )bggame->Class( currentClass )->health;
                            
                    if( ent->client->pers.evolveHealthFraction < 0.0f )
                        ent->client->pers.evolveHealthFraction = 0.0f;
                    else if( ent->client->pers.evolveHealthFraction > 1.0f )
                        ent->client->pers.evolveHealthFraction = 1.0f;
                        
                    //remove credit
                    idSGameClient::AddCreditToClient( ent->client, -cost, true );
                    ent->client->pers.classSelection = newClass;
                    sgameLocal.ClientUserinfoChanged( clientNum );
                    VectorCopy( infestOrigin, ent->s.pos.trBase );
                    VectorCopy( ent->client->ps.velocity, oldVel );
                    if( ent->client->ps.stats[ STAT_STATE ] & SS_BOOSTED )
                        oldBoostTime = ent->client->boostedTime;
                    idSGameClient::ClientSpawn( ent, ent, ent->s.pos.trBase, ent->s.apos.trBase );
                    VectorCopy( oldVel, ent->client->ps.velocity );
                    if( oldBoostTime > 0 )
                    {
                        ent->client->boostedTime = oldBoostTime;
                        ent->client->ps.stats[ STAT_STATE ] |= SS_BOOSTED;
                    }
                    return;
                }
                else
                {
                    idSGameUtils::TriggerMenu2( clientNum, MN_A_CANTEVOLVE, newClass );
                    return;
                }
            }
            else
            {
                idSGameUtils::TriggerMenu( clientNum, MN_A_NOEROOM );
                return;
            }
        }
    }
    else if( ent->client->pers.teamSelection == TEAM_HUMANS )
    {
        idSGameUtils::TriggerMenu( clientNum, MN_H_DEADTOCLASS );
        return;
    }
}


/*
=================
Cmd_Destroy_f
=================
*/
void idSGameCmds::Cmd_Destroy_f( gentity_t* ent )
{
    vec3_t      forward, end;
    trace_t     tr;
    gentity_t*   traceEnt;
    valueType        cmd[ 12 ];
    bool    deconstruct = true;
    
    if( ent->client->pers.denyBuild )
    {
        idSGameUtils::TriggerMenu( ent->client->ps.clientNum, MN_B_REVOKED );
        return;
    }
    
    trap_Argv( 0, cmd, sizeof( cmd ) );
    if( Q_stricmp( cmd, "destroy" ) == 0 )
        deconstruct = false;
        
    if( ent->client->ps.stats[ STAT_STATE ] & SS_HOVELING )
        idSGameCombat::Damage( ent->client->hovel, ent, ent, forward, ent->s.origin, 10000, 0, MOD_SUICIDE );
        
    AngleVectors( ent->client->ps.viewangles, forward, nullptr, nullptr );
    VectorMA( ent->client->ps.origin, 100, forward, end );
    
    trap_Trace( &tr, ent->client->ps.origin, nullptr, nullptr, end, ent->s.number, MASK_PLAYERSOLID );
    traceEnt = &g_entities[ tr.entityNum ];
    
    if( tr.fraction < 1.0f &&
            ( traceEnt->s.eType == ET_BUILDABLE ) &&
            ( traceEnt->buildableTeam == ent->client->pers.teamSelection ) &&
            ( ent->client->ps.weapon == WP_ABUILD ||
              ent->client->ps.weapon == WP_ABUILD2 ||
              ent->client->ps.weapon == WP_HBUILD ) )
    {
        // Always let the builder prevent the explosion
        if( traceEnt->health <= 0 )
        {
            idSGameBuildable::QueueBuildPoints( traceEnt );
            idSGameUtils::FreeEntity( traceEnt );
            return;
        }
        
        // Cancel deconstruction
        if( g_markDeconstruct.integer && traceEnt->deconstruct )
        {
            traceEnt->deconstruct = false;
            return;
        }
        
        // Prevent destruction of the last spawn
        if( !g_markDeconstruct.integer && !g_cheats.integer )
        {
            if( ent->client->pers.teamSelection == TEAM_ALIENS &&
                    traceEnt->s.modelindex == BA_A_SPAWN )
            {
                if( level.numAlienSpawns <= 1 )
                {
                    idSGameUtils::TriggerMenu( ent->client->ps.clientNum, MN_B_LASTSPAWN );
                    return;
                }
            }
            else if( ent->client->pers.teamSelection == TEAM_HUMANS &&
                     traceEnt->s.modelindex == BA_H_SPAWN )
            {
                if( level.numHumanSpawns <= 1 )
                {
                    idSGameUtils::TriggerMenu( ent->client->ps.clientNum, MN_B_LASTSPAWN );
                    return;
                }
            }
        }
        
        // Don't allow destruction of hovel with granger inside
        if( traceEnt->s.modelindex == BA_A_HOVEL && traceEnt->active )
            return;
            
        // Don't allow destruction of buildables that cannot be rebuilt
        if( idSGameMain::TimeTilSuddenDeath( ) <= 0 )
            return;
            
        if( !g_markDeconstruct.integer && ent->client->ps.stats[ STAT_MISC ] > 0 )
        {
            idSGameUtils::AddEvent( ent, EV_BUILD_DELAY, ent->client->ps.clientNum );
            return;
        }
        
        if( traceEnt->health > 0 )
        {
            if( !deconstruct )
                idSGameCombat::Damage( traceEnt, ent, ent, forward, tr.endpos, traceEnt->health, 0, MOD_SUICIDE );
            else if( g_markDeconstruct.integer )
            {
                traceEnt->deconstruct     = true; // Mark buildable for deconstruction
                traceEnt->deconstructTime = level.time;
            }
            else
            {
                idSGameCombat::LogDestruction( traceEnt, ent, MOD_DECONSTRUCT );
                idSGameUtils::FreeEntity( traceEnt );
                
                if( !g_cheats.integer )
                    ent->client->ps.stats[ STAT_MISC ] +=
                        bggame->Buildable( ( buildable_t )traceEnt->s.modelindex )->buildTime >> 2;
            }
        }
    }
}


/*
=================
Cmd_ActivateItem_f

Activate an item
=================
*/
void idSGameCmds::Cmd_ActivateItem_f( gentity_t* ent )
{
    valueType      s[ MAX_TOKEN_CHARS ];
    sint   upgrade, weapon;
    
    trap_Argv( 1, s, sizeof( s ) );
    
    // "weapon" aliased to whatever weapon you have
    if( !Q_stricmp( "weapon", s ) )
    {
        if( ent->client->ps.weapon == WP_BLASTER && bggame->PlayerCanChangeWeapon( &ent->client->ps ) )
            idSGameWeapons::ForceWeaponChange( ent, WP_NONE );
        return;
    }
    
    upgrade = bggame->UpgradeByName( s )->number;
    weapon = bggame->WeaponByName( s )->number;
    
    if( upgrade != UP_NONE && bggame->InventoryContainsUpgrade( upgrade, ent->client->ps.stats ) )
        bggame->ActivateUpgrade( upgrade, ent->client->ps.stats );
    else if( weapon != WP_NONE &&
             bggame->InventoryContainsWeapon( weapon, ent->client->ps.stats ) )
    {
        if( ent->client->ps.weapon != weapon && bggame->PlayerCanChangeWeapon( &ent->client->ps ) )
            idSGameWeapons::ForceWeaponChange( ent, ( weapon_t )weapon );
    }
    else
        trap_SendServerCommand( ent - g_entities, va( "print \"You don't have the %s\n\"", s ) );
}


/*
=================
Cmd_DeActivateItem_f

Deactivate an item
=================
*/
void idSGameCmds::Cmd_DeActivateItem_f( gentity_t* ent )
{
    valueType      s[ MAX_TOKEN_CHARS ];
    sint   upgrade;
    
    trap_Argv( 1, s, sizeof( s ) );
    upgrade = bggame->UpgradeByName( s )->number;
    
    if( bggame->InventoryContainsUpgrade( upgrade, ent->client->ps.stats ) )
        bggame->DeactivateUpgrade( upgrade, ent->client->ps.stats );
    else
        trap_SendServerCommand( ent - g_entities, va( "print \"You don't have the %s\n\"", s ) );
}


/*
=================
Cmd_ToggleItem_f
=================
*/
void idSGameCmds::Cmd_ToggleItem_f( gentity_t* ent )
{
    valueType      s[ MAX_TOKEN_CHARS ];
    sint   upgrade, weapon;
    
    trap_Argv( 1, s, sizeof( s ) );
    upgrade = bggame->UpgradeByName( s )->number;
    weapon = bggame->WeaponByName( s )->number;
    
    if( weapon != WP_NONE )
    {
        if( !bggame->PlayerCanChangeWeapon( &ent->client->ps ) )
            return;
            
        //special case to allow switching between
        //the blaster and the primary weapon
        if( ent->client->ps.weapon != WP_BLASTER )
            weapon = WP_BLASTER;
        else
            weapon = WP_NONE;
            
        idSGameWeapons::ForceWeaponChange( ent, ( weapon_t )weapon );
    }
    else if( bggame->InventoryContainsUpgrade( upgrade, ent->client->ps.stats ) )
    {
        if( bggame->UpgradeIsActive( upgrade, ent->client->ps.stats ) )
            bggame->DeactivateUpgrade( upgrade, ent->client->ps.stats );
        else
            bggame->ActivateUpgrade( upgrade, ent->client->ps.stats );
    }
    else
        trap_SendServerCommand( ent - g_entities, va( "print \"You don't have the %s\n\"", s ) );
}

/*
=================
Cmd_Buy_f
=================
*/
void idSGameCmds::Cmd_Buy_f( gentity_t* ent )
{
    valueType      s[ MAX_TOKEN_CHARS ];
    weapon_t  weapon;
    upgrade_t upgrade;
    bool  energyOnly;
    
    trap_Argv( 1, s, sizeof( s ) );
    
    weapon = bggame->WeaponByName( s )->number;
    upgrade = bggame->UpgradeByName( s )->number;
    
    // Only give energy from reactors or repeaters
    if( idSGameBuildable::BuildableRange( ent->client->ps.origin, 100, BA_H_ARMOURY ) )
        energyOnly = false;
    else if( upgrade == UP_AMMO &&
             bggame->Weapon( ( weapon_t )ent->client->ps.stats[ STAT_WEAPON ] )->usesEnergy &&
             ( idSGameBuildable::BuildableRange( ent->client->ps.origin, 100, BA_H_REACTOR ) ||
               idSGameBuildable::BuildableRange( ent->client->ps.origin, 100, BA_H_REPEATER ) ) )
        energyOnly = true;
    else
    {
        if( upgrade == UP_AMMO &&
                bggame->Weapon( ( weapon_t )ent->client->ps.weapon )->usesEnergy )
            idSGameUtils::TriggerMenu( ent->client->ps.clientNum, MN_H_NOENERGYAMMOHERE );
        else
            idSGameUtils::TriggerMenu( ent->client->ps.clientNum, MN_H_NOARMOURYHERE );
        return;
    }
    
    if( weapon != WP_NONE )
    {
        //already got this?
        if( bggame->InventoryContainsWeapon( weapon, ent->client->ps.stats ) )
        {
            idSGameUtils::TriggerMenu( ent->client->ps.clientNum, MN_H_ITEMHELD );
            return;
        }
        
        if( bggame->Weapon( weapon )->team != TEAM_HUMANS )
        {
            //shouldn't need a fancy dialog
            trap_SendServerCommand( ent - g_entities, "print \"You can't buy alien items\n\"" );
            return;
        }
        
        //are we /allowed/ to buy this?
        if( !bggame->Weapon( weapon )->purchasable )
        {
            trap_SendServerCommand( ent - g_entities, "print \"You can't buy this item\n\"" );
            return;
        }
        
        //are we /allowed/ to buy this?
        if( !bggame->WeaponAllowedInStage( weapon, ( stage_t )g_humanStage.integer ) || !bggame->WeaponIsAllowed( weapon ) )
        {
            trap_SendServerCommand( ent - g_entities, "print \"You can't buy this item\n\"" );
            return;
        }
        
        //can afford this?
        if( bggame->Weapon( weapon )->price > ( schar16 )ent->client->ps.persistant[ PERS_CREDIT ] )
        {
            idSGameUtils::TriggerMenu( ent->client->ps.clientNum, MN_H_NOFUNDS );
            return;
        }
        
        //have space to carry this?
        if( bggame->Weapon( weapon )->slots & bggame->CalculateSlotsForInventory( ent->client->ps.stats ) )
        {
            idSGameUtils::TriggerMenu( ent->client->ps.clientNum, MN_H_NOSLOTS );
            return;
        }
        
        // In some instances, weapons can't be changed
        if( !bggame->PlayerCanChangeWeapon( &ent->client->ps ) )
            return;
            
        ent->client->ps.stats[ STAT_WEAPON ] = weapon;
        ent->client->ps.ammo = bggame->Weapon( weapon )->maxAmmo;
        ent->client->ps.clips = bggame->Weapon( weapon )->maxClips;
        
        if( bggame->Weapon( weapon )->usesEnergy &&
                bggame->InventoryContainsUpgrade( UP_BATTPACK, ent->client->ps.stats ) )
            ent->client->ps.ammo *= BATTPACK_MODIFIER;
            
        idSGameWeapons::ForceWeaponChange( ent, weapon );
        
        //set build delay/pounce etc to 0
        ent->client->ps.stats[ STAT_MISC ] = 0;
        
        //subtract from funds
        idSGameClient::AddCreditToClient( ent->client, -( schar16 )bggame->Weapon( weapon )->price, false );
    }
    else if( upgrade != UP_NONE )
    {
        //already got this?
        if( bggame->InventoryContainsUpgrade( upgrade, ent->client->ps.stats ) )
        {
            idSGameUtils::TriggerMenu( ent->client->ps.clientNum, MN_H_ITEMHELD );
            return;
        }
        
        //can afford this?
        if( bggame->Upgrade( upgrade )->price > ( schar16 )ent->client->ps.persistant[ PERS_CREDIT ] )
        {
            idSGameUtils::TriggerMenu( ent->client->ps.clientNum, MN_H_NOFUNDS );
            return;
        }
        
        //have space to carry this?
        if( bggame->Upgrade( upgrade )->slots & bggame->CalculateSlotsForInventory( ent->client->ps.stats ) )
        {
            idSGameUtils::TriggerMenu( ent->client->ps.clientNum, MN_H_NOSLOTS );
            return;
        }
        
        // Only humans can buy stuff
        if( bggame->Upgrade( upgrade )->team != TEAM_HUMANS )
        {
            trap_SendServerCommand( ent - g_entities, va( "print \"You can't buy alien items\n\"" ) );
            return;
        }
        
        //are we /allowed/ to buy this?
        if( !bggame->Upgrade( upgrade )->purchasable )
        {
            trap_SendServerCommand( ent - g_entities, va( "print \"You can't buy this item\n\"" ) );
            return;
        }
        
        //are we /allowed/ to buy this?
        if( !bggame->UpgradeAllowedInStage( upgrade, ( stage_t )g_humanStage.integer ) || !bggame->UpgradeIsAllowed( upgrade ) )
        {
            trap_SendServerCommand( ent - g_entities, va( "print \"You can't buy this item\n\"" ) );
            return;
        }
        
        if( upgrade == UP_AMMO )
            idSGameWeapons::GiveClientMaxAmmo( ent, energyOnly );
        else
        {
            if( upgrade == UP_BATTLESUIT )
            {
                vec3_t newOrigin;
                
                if( !RoomForClassChange( ent, PCL_HUMAN_BSUIT, newOrigin ) )
                {
                    idSGameUtils::TriggerMenu( ent->client->ps.clientNum, MN_H_NOROOMBSUITON );
                    return;
                }
                VectorCopy( newOrigin, ent->client->ps.origin );
                ent->client->ps.stats[ STAT_CLASS ] = PCL_HUMAN_BSUIT;
                ent->client->pers.classSelection = PCL_HUMAN_BSUIT;
                ent->client->ps.eFlags ^= EF_TELEPORT_BIT;
            }
            
            //add to inventory
            bggame->AddUpgradeToInventory( upgrade, ent->client->ps.stats );
        }
        
        if( upgrade == UP_BATTPACK )
            idSGameWeapons::GiveClientMaxAmmo( ent, true );
            
        //subtract from funds
        idSGameClient::AddCreditToClient( ent->client, -( schar16 )bggame->Upgrade( upgrade )->price, false );
    }
    else
        idSGameUtils::TriggerMenu( ent->client->ps.clientNum, MN_H_UNKNOWNITEM );
        
    //update ClientInfo
    sgameLocal.ClientUserinfoChanged( ent->client->ps.clientNum );
}


/*
=================
Cmd_Sell_f
=================
*/
void idSGameCmds::Cmd_Sell_f( gentity_t* ent )
{
    valueType      s[ MAX_TOKEN_CHARS ];
    sint       i;
    sint       weapon, upgrade;
    
    trap_Argv( 1, s, sizeof( s ) );
    
    //no armoury nearby
    if( !idSGameBuildable::BuildableRange( ent->client->ps.origin, 100, BA_H_ARMOURY ) )
    {
        idSGameUtils::TriggerMenu( ent->client->ps.clientNum, MN_H_NOARMOURYHERE );
        return;
    }
    
    weapon = bggame->WeaponByName( s )->number;
    upgrade = bggame->UpgradeByName( s )->number;
    
    if( !Q_stricmpn( s, "weapon", 6 ) )
        weapon = ent->client->ps.stats[ STAT_WEAPON ];
        
    if( weapon != WP_NONE )
    {
        weapon_t selected = bggame->GetPlayerWeapon( &ent->client->ps );
        
        if( !bggame->PlayerCanChangeWeapon( &ent->client->ps ) )
            return;
            
        //are we /allowed/ to sell this?
        if( !bggame->Weapon( ( weapon_t )weapon )->purchasable )
        {
            trap_SendServerCommand( ent - g_entities, "print \"You can't sell this weapon\n\"" );
            return;
        }
        
        //remove weapon if carried
        if( bggame->InventoryContainsWeapon( weapon, ent->client->ps.stats ) )
        {
            //guard against selling the HBUILD weapons exploit
            if( weapon == WP_HBUILD && ent->client->ps.stats[ STAT_MISC ] > 0 )
            {
                idSGameUtils::TriggerMenu( ent->client->ps.clientNum, MN_H_ARMOURYBUILDTIMER );
                return;
            }
            
            ent->client->ps.stats[ STAT_WEAPON ] = WP_NONE;
            // Cancel ghost buildables
            ent->client->ps.stats[STAT_BUILDABLE] = BA_NONE;
            
            //add to funds
            idSGameClient::AddCreditToClient( ent->client, ( schar16 )bggame->Weapon( ( weapon_t )weapon )->price, false );
        }
        
        //if we have this weapon selected, force a new selection
        if( weapon == selected )
            idSGameWeapons::ForceWeaponChange( ent, WP_BLASTER );
    }
    else if( upgrade != UP_NONE )
    {
        //are we /allowed/ to sell this?
        if( !bggame->Upgrade( ( upgrade_t )upgrade )->purchasable )
        {
            trap_SendServerCommand( ent - g_entities, "print \"You can't sell this item\n\"" );
            return;
        }
        //remove upgrade if carried
        if( bggame->InventoryContainsUpgrade( upgrade, ent->client->ps.stats ) )
        {
            // shouldn't really need to test for this, but just to be safe
            if( upgrade == UP_BATTLESUIT )
            {
                vec3_t newOrigin;
                
                if( !RoomForClassChange( ent, PCL_HUMAN, newOrigin ) )
                {
                    idSGameUtils::TriggerMenu( ent->client->ps.clientNum, MN_H_NOROOMBSUITOFF );
                    return;
                }
                VectorCopy( newOrigin, ent->client->ps.origin );
                ent->client->ps.stats[ STAT_CLASS ] = PCL_HUMAN;
                ent->client->pers.classSelection = PCL_HUMAN;
                ent->client->ps.eFlags ^= EF_TELEPORT_BIT;
            }
            
            //add to inventory
            bggame->RemoveUpgradeFromInventory( upgrade, ent->client->ps.stats );
            
            if( upgrade == UP_BATTPACK )
                idSGameWeapons::GiveClientMaxAmmo( ent, true );
                
            //add to funds
            idSGameClient::AddCreditToClient( ent->client, ( schar16 )bggame->Upgrade( ( upgrade_t )upgrade )->price, false );
        }
    }
    else if( !Q_stricmp( s, "weapons" ) )
    {
        weapon_t selected = bggame->GetPlayerWeapon( &ent->client->ps );
        
        if( !bggame->PlayerCanChangeWeapon( &ent->client->ps ) )
            return;
            
        for( i = WP_NONE + 1; i < WP_NUM_WEAPONS; i++ )
        {
            //guard against selling the HBUILD weapons exploit
            if( i == WP_HBUILD && ent->client->ps.stats[ STAT_MISC ] > 0 )
            {
                idSGameUtils::TriggerMenu( ent->client->ps.clientNum, MN_H_ARMOURYBUILDTIMER );
                continue;
            }
            
            if( bggame->InventoryContainsWeapon( i, ent->client->ps.stats ) &&
                    bggame->Weapon( ( weapon_t )i )->purchasable )
            {
                ent->client->ps.stats[ STAT_WEAPON ] = WP_NONE;
                
                //add to funds
                idSGameClient::AddCreditToClient( ent->client, ( schar16 )bggame->Weapon( ( weapon_t )i )->price, false );
            }
            
            //if we have this weapon selected, force a new selection
            if( i == selected )
                idSGameWeapons::ForceWeaponChange( ent, WP_NONE );
        }
    }
    else if( !Q_stricmp( s, "upgrades" ) )
    {
        for( i = UP_NONE + 1; i < UP_NUM_UPGRADES; i++ )
        {
            //remove upgrade if carried
            if( bggame->InventoryContainsUpgrade( i, ent->client->ps.stats ) &&
                    bggame->Upgrade( ( upgrade_t )i )->purchasable )
            {
            
                // shouldn't really need to test for this, but just to be safe
                if( i == UP_BATTLESUIT )
                {
                    vec3_t newOrigin;
                    
                    if( !RoomForClassChange( ent, PCL_HUMAN, newOrigin ) )
                    {
                        idSGameUtils::TriggerMenu( ent->client->ps.clientNum, MN_H_NOROOMBSUITOFF );
                        continue;
                    }
                    VectorCopy( newOrigin, ent->client->ps.origin );
                    ent->client->ps.stats[ STAT_CLASS ] = PCL_HUMAN;
                    ent->client->pers.classSelection = PCL_HUMAN;
                    ent->client->ps.eFlags ^= EF_TELEPORT_BIT;
                }
                
                bggame->RemoveUpgradeFromInventory( i, ent->client->ps.stats );
                
                if( i == UP_BATTPACK )
                    idSGameWeapons::GiveClientMaxAmmo( ent, true );
                    
                //add to funds
                idSGameClient::AddCreditToClient( ent->client, ( schar16 )bggame->Upgrade( ( upgrade_t )i )->price, false );
            }
        }
    }
    else
        idSGameUtils::TriggerMenu( ent->client->ps.clientNum, MN_H_UNKNOWNITEM );
        
    //update ClientInfo
    sgameLocal.ClientUserinfoChanged( ent->client->ps.clientNum );
}


/*
=================
Cmd_Build_f
=================
*/
void idSGameCmds::Cmd_Build_f( gentity_t* ent )
{
    valueType          s[ MAX_TOKEN_CHARS ];
    buildable_t   buildable;
    float32         dist;
    vec3_t        origin;
    team_t        team;
    
    if( ent->client->pers.denyBuild )
    {
        idSGameUtils::TriggerMenu( ent->client->ps.clientNum, MN_B_REVOKED );
        return;
    }
    
    if( ent->client->pers.teamSelection == level.surrenderTeam )
    {
        idSGameUtils::TriggerMenu( ent->client->ps.clientNum, MN_B_SURRENDER );
        return;
    }
    
    trap_Argv( 1, s, sizeof( s ) );
    
    buildable = bggame->BuildableByName( s )->number;
    
    if( idSGameMain::TimeTilSuddenDeath( ) <= 0 )
    {
        idSGameUtils::TriggerMenu( ent->client->ps.clientNum, MN_B_SUDDENDEATH );
        return;
    }
    
    team = ( team_t )ent->client->ps.stats[ STAT_TEAM ];
    
    if( buildable != BA_NONE &&
            ( ( 1 << ent->client->ps.weapon ) & bggame->Buildable( buildable )->buildWeapon ) &&
            !( ent->client->ps.stats[ STAT_STATE ] & SS_HOVELING ) &&
            bggame->BuildableIsAllowed( buildable ) &&
            ( ( team == TEAM_ALIENS && bggame->BuildableAllowedInStage( buildable, ( stage_t )g_alienStage.integer ) ) ||
              ( team == TEAM_HUMANS && bggame->BuildableAllowedInStage( buildable, ( stage_t )g_humanStage.integer ) ) ) )
    {
        dist = bggame->Class( ( class_t )ent->client->ps.stats[ STAT_CLASS ] )->buildDist;
        
        //these are the errors displayed when the builder first selects something to use
        switch( idSGameBuildable::CanBuild( ent, buildable, dist, origin ) )
        {
                // can place right away, set the blueprint and the valid togglebit
            case IBE_NONE:
            case IBE_TNODEWARN:
            case IBE_RPTNOREAC:
            case IBE_RPTPOWERHERE:
            case IBE_SPWNWARN:
                ent->client->ps.stats[ STAT_BUILDABLE ] = ( buildable | SB_VALID_TOGGLEBIT );
                break;
                
                // can't place yet but maybe soon: start with valid togglebit off
            case IBE_NORMAL:
            case IBE_HOVELEXIT:
            case IBE_NOCREEP:
            case IBE_NOROOM:
            case IBE_NOOVERMIND:
            case IBE_NOPOWERHERE:
                ent->client->ps.stats[ STAT_BUILDABLE ] = buildable;
                break;
                
                // more serious errors just pop a menu
            case IBE_NOALIENBP:
                idSGameUtils::TriggerMenu( ent->client->ps.clientNum, MN_A_NOBP );
                break;
                
            case IBE_ONEOVERMIND:
                idSGameUtils::TriggerMenu( ent->client->ps.clientNum, MN_A_ONEOVERMIND );
                break;
                
            case IBE_ONEHOVEL:
                idSGameUtils::TriggerMenu( ent->client->ps.clientNum, MN_A_ONEHOVEL );
                break;
                
            case IBE_ONEREACTOR:
                idSGameUtils::TriggerMenu( ent->client->ps.clientNum, MN_H_ONEREACTOR );
                break;
                
            case IBE_NOHUMANBP:
                idSGameUtils::TriggerMenu( ent->client->ps.clientNum, MN_H_NOBP );
                break;
                
            case IBE_NODCC:
                idSGameUtils::TriggerMenu( ent->client->ps.clientNum, MN_H_NODCC );
                break;
                
            default:
                break;
        }
    }
    else
        idSGameUtils::TriggerMenu( ent->client->ps.clientNum, MN_B_CANNOT );
}

/*
=================
Cmd_Reload_f
=================
*/
void idSGameCmds::Cmd_Reload_f( gentity_t* ent )
{
    playerState_t* ps = &ent->client->ps;
    sint ammo;
    
    // weapon doesn't ever need reloading
    if( bggame->Weapon( ( weapon_t )ps->weapon )->infiniteAmmo )
        return;
        
    if( ps->clips <= 0 )
        return;
        
    if( bggame->Weapon( ( weapon_t )ps->weapon )->usesEnergy &&
            bggame->InventoryContainsUpgrade( UP_BATTPACK, ps->stats ) )
        ammo = bggame->Weapon( ( weapon_t )ps->weapon )->maxAmmo * BATTPACK_MODIFIER;
    else
        ammo = bggame->Weapon( ( weapon_t )ps->weapon )->maxAmmo;
        
    // don't reload when full
    if( ps->ammo >= ammo )
        return;
        
    // the animation, ammo refilling etc. is handled by PM_Weapon
    if( ent->client->ps.weaponstate != WEAPON_RELOADING )
        ent->client->ps.pm_flags |= PMF_WEAPON_RELOAD;
}

/*
=================
idSGameCmds::StopFromFollowing

stops any other clients from following this one
called when a player leaves a team or dies
=================
*/
void idSGameCmds::StopFromFollowing( gentity_t* ent )
{
    sint i;
    
    for( i = 0; i < level.maxclients; i++ )
    {
        if( level.clients[ i ].sess.spectatorState != SPECTATOR_NOT && level.clients[ i ].sess.spectatorState ==
                SPECTATOR_FOLLOW && level.clients[ i ].sess.spectatorClient == ent->client->ps.clientNum )
        {
            if( !FollowNewClient( &g_entities[ i ], 1 ) )
                StopFollowing( &g_entities[ i ] );
        }
    }
}

/*
=================
idSGameCmds::StopFollowing

If the client being followed leaves the game, or you just want to drop
to free floating spectator mode
=================
*/
void idSGameCmds::StopFollowing( gentity_t* ent )
{
    ent->client->ps.stats[ STAT_TEAM ] = ent->client->pers.teamSelection;
    
    if( ent->client->pers.teamSelection == TEAM_NONE )
    {
        ent->client->sess.spectatorState = ( spectatorState_t )( ent->client->ps.persistant[ PERS_SPECSTATE ] = SPECTATOR_FREE );
    }
    else
    {
        vec3_t spawn_origin, spawn_angles;
        
        ent->client->sess.spectatorState = ( spectatorState_t )( ent->client->ps.persistant[ PERS_SPECSTATE ] = SPECTATOR_LOCKED );
        if( ent->client->pers.teamSelection == TEAM_ALIENS )
            idSGameClient::SelectAlienLockSpawnPoint( spawn_origin, spawn_angles );
        else if( ent->client->pers.teamSelection == TEAM_HUMANS )
            idSGameClient::SelectHumanLockSpawnPoint( spawn_origin, spawn_angles );
        idSGameUtils::SetOrigin( ent, spawn_origin );
        VectorCopy( spawn_origin, ent->client->ps.origin );
        idSGameClient::SetClientViewAngle( ent, spawn_angles );
    }
    ent->client->sess.spectatorClient = -1;
    ent->client->ps.pm_flags &= ~PMF_FOLLOW;
    ent->client->ps.stats[ STAT_STATE ] &= ~SS_WALLCLIMBING;
    ent->client->ps.eFlags &= ~( EF_WALLCLIMB | EF_WALLCLIMBCEILING );
    ent->client->ps.stats[ STAT_VIEWLOCK ] = 0;
    ent->client->ps.viewangles[ PITCH ] = 0.0f;
    //  ent->r.svFlags &= ~SVF_BOT;
    ent->client->ps.clientNum = ent - g_entities;
    
    idSGameMain::CalculateRanks( );
}

/*
=================
idSGameCmds::FollowLockView

Client is still following a player, but that player has gone to spectator
mode and cannot be followed for the moment
=================
*/
void idSGameCmds::FollowLockView( gentity_t* ent )
{
    vec3_t spawn_origin, spawn_angles;
    sint clientNum;
    
    clientNum = ent->client->sess.spectatorClient;
    ent->client->sess.spectatorState = ( spectatorState_t )( ent->client->ps.persistant[ PERS_SPECSTATE ] = SPECTATOR_FOLLOW );
    ent->client->ps.clientNum = clientNum;
    ent->client->ps.pm_flags &= ~PMF_FOLLOW;
    ent->client->ps.stats[ STAT_TEAM ] = ent->client->pers.teamSelection;
    ent->client->ps.stats[ STAT_STATE ] &= ~SS_WALLCLIMBING;
    ent->client->ps.eFlags &= ~( EF_WALLCLIMB | EF_WALLCLIMBCEILING );
    ent->client->ps.eFlags ^= EF_TELEPORT_BIT;
    ent->client->ps.stats[ STAT_VIEWLOCK ] = 0;
    ent->client->ps.viewangles[ PITCH ] = 0.0f;
    
    // Put the view at the team spectator lock position
    if( level.clients[ clientNum ].pers.teamSelection == TEAM_ALIENS )
        idSGameClient::SelectAlienLockSpawnPoint( spawn_origin, spawn_angles );
    else if( level.clients[ clientNum ].pers.teamSelection == TEAM_HUMANS )
        idSGameClient::SelectHumanLockSpawnPoint( spawn_origin, spawn_angles );
    idSGameUtils::SetOrigin( ent, spawn_origin );
    VectorCopy( spawn_origin, ent->client->ps.origin );
    idSGameClient::SetClientViewAngle( ent, spawn_angles );
}

/*
=================
idSGameCmds::FollowNewClient

This was a really nice, elegant function. Then I fucked it up.
=================
*/
bool idSGameCmds::FollowNewClient( gentity_t* ent, sint dir )
{
    sint clientnum = ent->client->sess.spectatorClient;
    sint original = clientnum;
    bool selectAny = false;
    
    if( dir > 1 )
        dir = 1;
    else if( dir < -1 )
        dir = -1;
    else if( dir == 0 )
        return true;
        
    if( ent->client->sess.spectatorState == SPECTATOR_NOT )
        return false;
        
    // select any if no target exists
    if( clientnum < 0 || clientnum >= level.maxclients )
    {
        clientnum = original = 0;
        selectAny = true;
    }
    
    do
    {
        clientnum += dir;
        
        if( clientnum >= level.maxclients )
            clientnum = 0;
            
        if( clientnum < 0 )
            clientnum = level.maxclients - 1;
            
        // can't follow self
        if( &g_entities[ clientnum ] == ent )
            continue;
            
        // avoid selecting existing follow target
        if( clientnum == original && !selectAny )
            continue; //effectively break;
            
        // can only follow connected clients
        if( level.clients[ clientnum ].pers.connected != CON_CONNECTED &&
                !level.clients[ clientnum ].pers.demoClient )
            continue;
            
        // can't follow a spectator
        if( level.clients[ clientnum ].pers.teamSelection == TEAM_NONE &&
                !level.clients[ clientnum ].pers.demoClient )
            continue;
            
        // if stickyspec is disabled, can't follow someone in queue either
        if( !ent->client->pers.stickySpec &&
                level.clients[ clientnum ].sess.spectatorState != SPECTATOR_NOT )
            continue;
            
        // can only follow teammates when dead and on a team
        if( ent->client->pers.teamSelection != TEAM_NONE &&
                ( level.clients[ clientnum ].pers.teamSelection !=
                  ent->client->pers.teamSelection ) )
            continue;
            
        // this is good, we can use it
        ent->client->sess.spectatorClient = clientnum;
        ent->client->sess.spectatorState = SPECTATOR_FOLLOW;
        
        // if this client is in the spawn queue, we need to do something special
        if( level.clients[ clientnum ].sess.spectatorState != SPECTATOR_NOT )
            FollowLockView( ent );
            
        return true;
        
    }
    while( clientnum != original );
    
    return false;
}

/*
=================
idSGameCmds::ToggleFollow
=================
*/
void idSGameCmds::ToggleFollow( gentity_t* ent )
{
    if( ent->client->sess.spectatorState == SPECTATOR_FOLLOW )
        StopFollowing( ent );
    else
        FollowNewClient( ent, 1 );
}

/*
=================
Cmd_Follow_f
=================
*/
void idSGameCmds::Cmd_Follow_f( gentity_t* ent )
{
    sint   i;
    sint   pids[ MAX_CLIENTS ];
    valueType  arg[ MAX_NAME_LENGTH ];
    
    if( trap_Argc( ) != 2 )
    {
        ToggleFollow( ent );
    }
    else
    {
        trap_Argv( 1, arg, sizeof( arg ) );
        if( ClientNumbersFromString( arg, pids, MAX_CLIENTS ) == 1 )
        {
            i = pids[ 0 ];
        }
        else
        {
            i = ClientNumberFromString( arg );
            
            if( i == -1 )
            {
                trap_SendServerCommand( ent - g_entities,
                                        "print \"follow: invalid player\n\"" );
                return;
            }
        }
        
        // can't follow self
        if( &level.clients[ i ] == ent->client )
            return;
            
        // can't follow another spectator if sticky spec is off
        if( !ent->client->pers.stickySpec &&
                level.clients[ i ].sess.spectatorState != SPECTATOR_NOT )
            return;
            
        // can only follow teammates when dead and on a team
        if( ent->client->pers.teamSelection != TEAM_NONE &&
                ( level.clients[ i ].pers.teamSelection !=
                  ent->client->pers.teamSelection ) )
            return;
            
        ent->client->sess.spectatorState = SPECTATOR_FOLLOW;
        ent->client->sess.spectatorClient = i;
    }
}

/*
=================
Cmd_FollowCycle_f
=================
*/
void idSGameCmds::Cmd_FollowCycle_f( gentity_t* ent )
{
    valueType args[ 11 ];
    sint  dir = 1;
    
    trap_Argv( 0, args, sizeof( args ) );
    if( Q_stricmp( args, "followprev" ) == 0 )
        dir = -1;
        
    // won't work unless spectating
    if( ent->client->sess.spectatorState == SPECTATOR_NOT )
        return;
        
    FollowNewClient( ent, dir );
}

/*
=================
Cmd_PTRCVerify_f

Check a PTR code is valid
=================
*/
void idSGameCmds::Cmd_PTRCVerify_f( gentity_t* ent )
{
    connectionRecord_t*  connection;
    valueType                s[ MAX_TOKEN_CHARS ] = { 0 };
    sint                 code;
    
    if( ent->client->pers.connection )
        return;
        
    trap_Argv( 1, s, sizeof( s ) );
    
    if( !s[ 0 ] )
        return;
        
    code = atoi( s );
    
    connection = idSGamePtr::FindConnectionForCode( code );
    if( connection )
    {
        // valid code
        if( connection->clientTeam != TEAM_NONE )
            trap_SendServerCommand( ent->client->ps.clientNum, "ptrcconfirm" );
            
        // restore mapping
        ent->client->pers.connection = connection;
        connection->clientNum = ent->client->ps.clientNum;
    }
    else
    {
        // invalid code -- generate a new one
        connection = idSGamePtr::GenerateNewConnection( ent->client );
        
        if( connection )
        {
            trap_SendServerCommand( ent->client->ps.clientNum, va( "ptrcissue %d", connection->ptrCode ) );
        }
    }
}

/*
=================
Cmd_PTRCRestore_f

Restore against a PTR code
=================
*/
void idSGameCmds::Cmd_PTRCRestore_f( gentity_t* ent )
{
    valueType                s[ MAX_TOKEN_CHARS ] = { 0 };
    sint                 code;
    connectionRecord_t*  connection;
    
    if( ent->client->pers.joinedATeam )
    {
        trap_SendServerCommand( ent - g_entities,
                                "print \"You cannot use a PTR code after joining a team\n\"" );
        return;
    }
    
    trap_Argv( 1, s, sizeof( s ) );
    
    if( !s[ 0 ] )
        return;
        
    code = atoi( s );
    
    connection = ent->client->pers.connection;
    if( connection && connection->ptrCode == code )
    {
        // set the correct team
        idSGameTeam::ChangeTeam( ent, connection->clientTeam );
        
        // set the correct credit
        ent->client->ps.persistant[ PERS_CREDIT ] = 0;
        idSGameClient::AddCreditToClient( ent->client, connection->clientCredit, true );
        if( connection->oldClient != ent - g_entities )
            idSGameClient::AddCreditToClient( &level.clients[ connection->oldClient ], -connection->clientCredit, true );
        connection->oldClient = ent - g_entities;
    }
    else
    {
        trap_SendServerCommand( ent - g_entities,
                                va( "print \"'%d' is not a valid PTR code\n\"", code ) );
    }
}

void idSGameCmds::Cmd_Ignore_f( gentity_t* ent )
{
    sint pids[ MAX_CLIENTS ];
    valueType name[ MAX_NAME_LENGTH ];
    valueType cmd[ 9 ];
    sint matches = 0;
    sint i;
    bool ignore = false;
    
    trap_Argv( 0, cmd, sizeof( cmd ) );
    if( Q_stricmp( cmd, "ignore" ) == 0 )
        ignore = true;
        
    if( trap_Argc() < 2 )
    {
        trap_SendServerCommand( ent - g_entities, va( "print \"[skipnotify]"
                                "usage: %s [clientNum | partial name match]\n\"", cmd ) );
        return;
    }
    
    Q_strncpyz( name, ConcatArgs( 1 ), sizeof( name ) );
    matches = ClientNumbersFromString( name, pids, MAX_CLIENTS );
    if( matches < 1 )
    {
        trap_SendServerCommand( ent - g_entities, va( "print \"[skipnotify]"
                                "%s: no clients match the name '%s'\n\"", cmd, name ) );
        return;
    }
    
    for( i = 0; i < matches; i++ )
    {
        if( ignore )
        {
            if( !bggame->ClientListTest( &ent->client->sess.ignoreList, pids[ i ] ) )
            {
                bggame->ClientListAdd( &ent->client->sess.ignoreList, pids[ i ] );
                sgameLocal.ClientUserinfoChanged( ent->client->ps.clientNum );
                trap_SendServerCommand( ent - g_entities, va( "print \"[skipnotify]"
                                        "ignore: added %s^7 to your ignore list\n\"",
                                        level.clients[ pids[ i ] ].pers.netname ) );
            }
            else
            {
                trap_SendServerCommand( ent - g_entities, va( "print \"[skipnotify]"
                                        "ignore: %s^7 is already on your ignore list\n\"",
                                        level.clients[ pids[ i ] ].pers.netname ) );
            }
        }
        else
        {
            if( bggame->ClientListTest( &ent->client->sess.ignoreList, pids[ i ] ) )
            {
                bggame->ClientListRemove( &ent->client->sess.ignoreList, pids[ i ] );
                sgameLocal.ClientUserinfoChanged( ent->client->ps.clientNum );
                trap_SendServerCommand( ent - g_entities, va( "print \"[skipnotify]"
                                        "unignore: removed %s^7 from your ignore list\n\"",
                                        level.clients[ pids[ i ] ].pers.netname ) );
            }
            else
            {
                trap_SendServerCommand( ent - g_entities, va( "print \"[skipnotify]"
                                        "unignore: %s^7 is not on your ignore list\n\"",
                                        level.clients[ pids[ i ] ].pers.netname ) );
            }
        }
    }
}

/*
=================
Cmd_Test_f

Hopefully will be able to catch that bizarro turret bug with this one
=================
*/
void idSGameCmds::Cmd_Test_f( gentity_t* ent )
{
    trap_SendServerCommand( ent - g_entities, va( "print \""
                            "  pointcontents = %x\n  r.contents = %x\n  targeted = %d\n\"",
                            trap_PointContents( ent->s.origin, ent - g_entities ),
                            ent->r.contents, ( ent->targeted ) ? ( sint )( ent->targeted - g_entities ) : 0 ) );
}

/*
=================
Cmd_Damage_f

Deals damage to you (for testing), arguments: [damage] [dx] [dy] [dz]
The dx/dy arguments describe the damage point's offset from the entity origin
=================
*/
void idSGameCmds::Cmd_Damage_f( gentity_t* ent )
{
    vec3_t point;
    valueType arg[ 16 ];
    float32 dx = 0.f, dy = 0.f, dz = 100.f;
    sint damage = 100;
    bool nonloc = true;
    
    if( trap_Argc() > 1 )
    {
        trap_Argv( 1, arg, sizeof( arg ) );
        damage = atoi( arg );
    }
    if( trap_Argc() > 4 )
    {
        trap_Argv( 2, arg, sizeof( arg ) );
        dx = atof( arg );
        trap_Argv( 3, arg, sizeof( arg ) );
        dy = atof( arg );
        trap_Argv( 4, arg, sizeof( arg ) );
        dz = atof( arg );
        nonloc = false;
    }
    VectorCopy( ent->s.origin, point );
    point[ 0 ] += dx;
    point[ 1 ] += dy;
    point[ 2 ] += dz;
    idSGameCombat::Damage( ent, nullptr, nullptr, nullptr, point, damage, ( nonloc ? DAMAGE_NO_LOCDAMAGE : 0 ), MOD_TARGET_LASER );
}

void idSGameCmds::EditPlayerInventory( gentity_t* ent )
{
    valueType itstr[MAX_TOKEN_CHARS];
    valueType arg[MAX_TOKEN_CHARS];
    trap_Argv( 1, itstr, sizeof( itstr ) );
    
    if( Q_stricmp( itstr, "dmg" ) == 0 )
    {
        trap_Argv( 2, arg, sizeof( arg ) );
        idSGameCombat::Damage( ent, nullptr, nullptr, nullptr, nullptr, atoi( arg ), 0, MOD_UNKNOWN );
    }
    else if( Q_stricmp( itstr, "cash" ) == 0 )
    {
        trap_Argv( 2, arg, sizeof( arg ) );
        ent->client->ps.persistant[ PERS_CREDIT ] += atoi( arg );
        // sanity check
        if( ent->client->ps.persistant[ PERS_CREDIT ] < 0 )
            ent->client->ps.persistant[ PERS_CREDIT ] = 0;
        else if( ent->client->ps.persistant[ PERS_CREDIT ] > HUMAN_MAX_CREDITS )
            ent->client->ps.persistant[ PERS_CREDIT ] = HUMAN_MAX_CREDITS;
    }
    else
        trap_SendServerCommand( ent - g_entities, va( "print \"unknown parameter\n\"" ) );
}

/*
==================
idSGameCmds::FloodLimited

Determine whether a user is flood limited, and adjust their flood demerits
Print them a warning message if they are over the limit
Return is time in msec until the user can speak again
==================
*/
sint idSGameCmds::FloodLimited( gentity_t* ent )
{
    sint deltatime, ms;
    
    if( g_floodMinTime.integer <= 0 )
        return 0;
        
    // handles !ent
    if( adminLocal.AdminPermission( ent, ADMF_NOCENSORFLOOD ) )
        return 0;
        
    deltatime = level.time - ent->client->pers.floodTime;
    
    ent->client->pers.floodDemerits += g_floodMinTime.integer - deltatime;
    if( ent->client->pers.floodDemerits < 0 )
        ent->client->pers.floodDemerits = 0;
    ent->client->pers.floodTime = level.time;
    
    ms = ent->client->pers.floodDemerits - g_floodMaxDemerits.integer;
    if( ms <= 0 )
        return 0;
    trap_SendServerCommand( ent - g_entities, va( "print \"You are flooding: "
                            "please wait %d second%s before trying again\n",
                            ( ms + 999 ) / 1000, ( ms > 1000 ) ? "s" : "" ) );
    return ms;
}

extern vmConvar_t bot_developer;
void idSGameCmds::Cmd_EditBotInv_f( gentity_t* ent )
{
    //gentity_t	*ent2;
    //sint i;
    gentity_t*	spec_ent;
    
    if( !bot_developer.integer )
    {
        trap_SendServerCommand( ent - g_entities, va( "print \"this cmd is only for bot testing\n\"" ) );
        return;
    }
    
    spec_ent = &g_entities[ ent->client->sess.spectatorClient ];
    
    if( spec_ent != ent )
    {
        EditPlayerInventory( spec_ent );
    }
    
}

/*
=================
idSGameCmds::Share_f
=================
*/
void idSGameCmds::Share_f( gentity_t* ent )
{
    sint   i, clientNum = 0, creds = 0, skipargs = 0;
    sint   clientNums[MAX_CLIENTS] = { -1 };
    valueType  cmd[12];
    valueType  arg1[MAX_STRING_TOKENS];
    valueType  arg2[MAX_STRING_TOKENS];
    team_t team;
    valueType data[255];
    
    if( !ent || !ent->client || ( ent->client->pers.teamSelection == TEAM_NONE ) )
    {
        return;
    }
    
    if( !g_allowShare.integer )
    {
        trap_SendServerCommand( ent - g_entities, "print \"Share has been disabled.\n\"" );
        return;
    }
    
    team = ent->client->pers.teamSelection;
    
    SayArgv( 0, cmd, sizeof( cmd ) );
    
    if( !Q_stricmp( cmd, "say" ) || !Q_stricmp( cmd, "say_team" ) )
    {
        skipargs = 1;
        SayArgv( 1, cmd, sizeof( cmd ) );
    }
    
    // target player name is in arg1
    SayArgv( 1 + skipargs, arg1, sizeof( arg1 ) );
    // amount to be shared is in arg2
    SayArgv( 2 + skipargs, arg2, sizeof( arg2 ) );
    
    if( arg1[0] && !strchr( arg1, ';' ) && Q_stricmp( arg1, "target_in_aim" ) )
    {
        //check arg1 is a number
        for( i = 0; arg1[i]; i++ )
        {
            if( arg1[i] < '0' || arg1[i] > '9' )
            {
                clientNum = -1;
                break;
            }
        }
        
        if( clientNum >= 0 )
        {
            clientNum = atoi( arg1 );
        }
        else if( ClientNumbersFromString( arg1, clientNums, MAX_CLIENTS ) == 1 )
        {
            // there was one partial name match
            clientNum = clientNums[0];
        }
        else
        {
            // look for an exact name match before bailing out
            clientNum = ClientNumberFromString( arg1 );
            if( clientNum == -1 )
            {
                trap_SendServerCommand( ent - g_entities, "print \"share: invalid player name specified.\n\"" );
                return;
            }
        }
    }
    else // arg1 not set
    {
        vec3_t      forward, end;
        trace_t     tr;
        gentity_t*   traceEnt;
        
        
        // trace a teammate
        AngleVectors( ent->client->ps.viewangles, forward, nullptr, nullptr );
        VectorMA( ent->client->ps.origin, 8192 * 16, forward, end );
        
        trap_Trace( &tr, ent->client->ps.origin, nullptr, nullptr, end, ent->s.number, MASK_PLAYERSOLID );
        traceEnt = &g_entities[tr.entityNum];
        
        if( tr.fraction < 1.0f && traceEnt->client &&
                ( traceEnt->client->pers.teamSelection == team ) )
        {
            clientNum = traceEnt - g_entities;
        }
        else
        {
            trap_SendServerCommand( ent - g_entities, va( "print \"share: aim at a teammate to share %s.\n\"", ( team == TEAM_HUMANS ) ? "credits" : "evolvepoints" ) );
            return;
        }
    }
    
    // verify target player team
    if( ( clientNum < 0 ) || ( clientNum >= level.maxclients ) ||
            ( level.clients[clientNum].pers.teamSelection != team ) )
    {
        trap_SendServerCommand( ent - g_entities, "print \"share: not a valid player of your team.\n\"" );
        return;
    }
    
    if( !arg2[0] || strchr( arg2, ';' ) )
    {
        // default credit count
        if( team == TEAM_HUMANS )
        {
            creds = FREEKILL_HUMAN;
        }
        else if( team == TEAM_ALIENS )
        {
            creds = FREEKILL_ALIEN;
        }
    }
    else
    {
        //check arg2 is a number
        for( i = 0; arg2[i]; i++ )
        {
            if( arg2[i] < '0' || arg2[i] > '9' )
            {
                trap_SendServerCommand( ent - g_entities, "print \"usage: share [name|slot#] [amount]\n\"" );
                break;
            }
        }
        
        // credit count from parameter
        creds = atoi( arg2 );
    }
    
    // player specified "0" to transfer
    if( creds <= 0 )
    {
        trap_SendServerCommand( ent - g_entities, "print \"Ooh, you are a generous one, indeed!\n\"" );
        return;
    }
    
    // transfer only credits the player really has
    if( creds > ent->client->ps.persistant[PERS_CREDIT] )
    {
        creds = ent->client->ps.persistant[PERS_CREDIT];
    }
    
    // player has no credits
    if( creds <= 0 )
    {
        trap_SendServerCommand( ent - g_entities, "print \"Earn some first, lazy gal!\n\"" );
        return;
    }
    
    // allow transfers only up to the credit/evo limit
    if( ( team == TEAM_HUMANS ) &&
            ( creds > HUMAN_MAX_CREDITS - level.clients[clientNum].ps.persistant[PERS_CREDIT] ) )
    {
        creds = HUMAN_MAX_CREDITS - level.clients[clientNum].ps.persistant[PERS_CREDIT];
    }
    else if( ( team == TEAM_ALIENS ) &&
             ( creds > ALIEN_MAX_KILLS - level.clients[clientNum].ps.persistant[PERS_CREDIT] ) )
    {
        creds = ALIEN_MAX_KILLS - level.clients[clientNum].ps.persistant[PERS_CREDIT];
    }
    
    // target cannot take any more credits
    if( creds <= 0 )
    {
        trap_SendServerCommand( ent - g_entities,
                                va( "print \"share: player cannot receive any more %s.\n\"",
                                    ( team == TEAM_HUMANS ) ? "credits" : "evolvepoints" ) );
        return;
    }
    
    // transfer credits
    ent->client->ps.persistant[PERS_CREDIT] -= creds;
    trap_SendServerCommand( ent - g_entities,
                            va( "print \"share: transferred %d %s to %s^7.\n\"", creds,
                                ( team == TEAM_HUMANS ) ? "credits" : "evolvepoints",
                                level.clients[clientNum].pers.netname ) );
    level.clients[clientNum].ps.persistant[PERS_CREDIT] += creds;
    trap_SendServerCommand( clientNum,
                            va( "print \"You have received %d %s from %s^7.\n\"", creds,
                                ( team == TEAM_HUMANS ) ? "credits" : "evolvepoints",
                                ent->client->pers.netname ) );
                                
    idSGameMain::LogPrintf( "Share: %i %i %i %d: %s^7 transferred %d%s to %s^7\n",
                            ent->client->ps.clientNum,
                            clientNum,
                            team,
                            creds,
                            ent->client->pers.netname,
                            creds,
                            ( team == TEAM_HUMANS ) ? "c" : "e",
                            level.clients[clientNum].pers.netname );
}

commands_t cmds[ ] =
{
    // normal commands
    { "team", 0, &idSGameCmds::Cmd_Team_f },
    { "vote", 0, &idSGameCmds::Cmd_Vote_f },
    { "ignore", 0, &idSGameCmds::Cmd_Ignore_f },
    { "unignore", 0, &idSGameCmds::Cmd_Ignore_f },
    
    // communication commands
    { "tell", CMD_MESSAGE, &idSGameCmds::Cmd_Tell_f },
    { "callvote", CMD_MESSAGE, &idSGameCmds::Cmd_CallVote_f },
    { "callteamvote", CMD_MESSAGE | CMD_TEAM, &idSGameCmds::Cmd_CallTeamVote_f },
    { "say_area", CMD_MESSAGE | CMD_TEAM, &idSGameCmds::Cmd_SayArea_f },
    // can be used even during intermission
    { "say", CMD_MESSAGE | CMD_INTERMISSION, &idSGameCmds::Cmd_Say_f },
    { "say_team", CMD_MESSAGE | CMD_INTERMISSION, &idSGameCmds::Cmd_Say_f },
    { "vsay", CMD_MESSAGE | CMD_INTERMISSION, &idSGameCmds::Cmd_VSay_f },
    { "vsay_team", CMD_MESSAGE | CMD_INTERMISSION, &idSGameCmds::Cmd_VSay_f },
    { "vsay_local", CMD_MESSAGE | CMD_INTERMISSION, &idSGameCmds::Cmd_VSay_f },
    { "m", CMD_MESSAGE | CMD_INTERMISSION, &idSGameCmds::PrivateMessage_f },
    { "mt", CMD_MESSAGE | CMD_INTERMISSION, &idSGameCmds::PrivateMessage_f },
    { "a", CMD_MESSAGE | CMD_INTERMISSION, &idSGameCmds::AdminMessage_f },
    
    { "score", CMD_INTERMISSION, &idSGameCmds::ScoreboardMessage },
    
    // cheats
    { "give", CMD_CHEAT | CMD_TEAM | CMD_LIVING, &idSGameCmds::Cmd_Give_f },
    { "god", CMD_CHEAT | CMD_TEAM | CMD_LIVING, &idSGameCmds::Cmd_God_f },
    { "notarget", CMD_CHEAT | CMD_TEAM | CMD_LIVING, &idSGameCmds::Cmd_Notarget_f },
    { "levelshot", CMD_CHEAT, &idSGameCmds::Cmd_LevelShot_f },
    { "setviewpos", CMD_CHEAT_TEAM, &idSGameCmds::Cmd_SetViewpos_f },
    { "stats", 0, &idSGameCmds::Cmd_Stats_f },
    { "noclip", CMD_CHEAT_TEAM, &idSGameCmds::Cmd_Noclip_f },
    { "destroy", CMD_CHEAT | CMD_TEAM | CMD_LIVING, &idSGameCmds::Cmd_Destroy_f },
    { "test", CMD_CHEAT, &idSGameCmds::Cmd_Test_f },
    { "damage", CMD_CHEAT | CMD_LIVING, &idSGameCmds::Cmd_Damage_f },
    { "botgod", CMD_CHEAT, &idSGameCmds::Cmd_EditBotInv_f },
    { "where", 0, &idSGameCmds::Cmd_Where_f },
    
    // game commands
    { "ptrcverify", 0, &idSGameCmds::Cmd_PTRCVerify_f },
    { "ptrcrestore", 0, &idSGameCmds::Cmd_PTRCRestore_f },
    
    { "follow", 0, &idSGameCmds::Cmd_Follow_f },
    { "follownext", 0, &idSGameCmds::Cmd_FollowCycle_f },
    { "followprev", 0, &idSGameCmds::Cmd_FollowCycle_f },
    
    { "teamvote", CMD_TEAM, &idSGameCmds::Cmd_TeamVote_f },
    { "class", CMD_TEAM, &idSGameCmds::Cmd_Class_f },
    { "kill", CMD_TEAM | CMD_LIVING, &idSGameCmds::Cmd_Kill_f },
    
    { "build", CMD_TEAM | CMD_LIVING, &idSGameCmds::Cmd_Build_f },
    { "deconstruct", CMD_TEAM | CMD_LIVING, &idSGameCmds::Cmd_Destroy_f },
    { "share", CMD_TEAM, &idSGameCmds::Share_f },
    { "buy", CMD_HUMAN | CMD_LIVING, &idSGameCmds::Cmd_Buy_f },
    { "sell", CMD_HUMAN | CMD_LIVING, &idSGameCmds::Cmd_Sell_f },
    { "itemact", CMD_HUMAN | CMD_LIVING, &idSGameCmds::Cmd_ActivateItem_f },
    { "itemdeact", CMD_HUMAN | CMD_LIVING, &idSGameCmds::Cmd_DeActivateItem_f },
    { "itemtoggle", CMD_HUMAN | CMD_LIVING, &idSGameCmds::Cmd_ToggleItem_f },
    { "reload", CMD_HUMAN | CMD_LIVING, &idSGameCmds::Cmd_Reload_f },
};
static sint numCmds = sizeof( cmds ) / sizeof( cmds[ 0 ] );

/*
=================
idSGameLocal::ClientCommand
=================
*/
void idSGameLocal::ClientCommand( sint clientNum )
{
    gentity_t* ent;
    valueType      cmd[ MAX_TOKEN_CHARS ];
    sint       i;
    
    ent = g_entities + clientNum;
    if( !ent->client )
        return;   // not fully in game yet
        
    trap_Argv( 0, cmd, sizeof( cmd ) );
    
    for( i = 0; i < numCmds; i++ )
    {
        if( Q_stricmp( cmd, cmds[ i ].cmdName ) == 0 )
            break;
    }
    
    if( i == numCmds )
    {
        if( !adminLocal.AdminCmdCheck( ent, false ) )
        {
            trap_SendServerCommand( clientNum, va( "print \"Unknown command %s\n\"", cmd ) );
        }
        return;
    }
    
    // do tests here to reduce the amount of repeated code
    
    if( !( cmds[ i ].cmdFlags & CMD_INTERMISSION ) && level.intermissiontime )
        return;
        
    if( cmds[ i ].cmdFlags & CMD_CHEAT && !g_cheats.integer )
    {
        idSGameUtils::TriggerMenu( clientNum, MN_CMD_CHEAT );
        return;
    }
    
    if( cmds[ i ].cmdFlags & CMD_MESSAGE && ( ent->client->pers.muted ||
            idSGameCmds::FloodLimited( ent ) ) )
        return;
        
    if( cmds[ i ].cmdFlags & CMD_TEAM &&
            ent->client->pers.teamSelection == TEAM_NONE )
    {
        idSGameUtils::TriggerMenu( clientNum, MN_CMD_TEAM );
        return;
    }
    
    if( cmds[ i ].cmdFlags & CMD_CHEAT_TEAM && !g_cheats.integer &&
            ent->client->pers.teamSelection != TEAM_NONE )
    {
        idSGameUtils::TriggerMenu( clientNum, MN_CMD_CHEAT_TEAM );
        return;
    }
    
    if( cmds[ i ].cmdFlags & CMD_SPEC &&
            ent->client->sess.spectatorState == SPECTATOR_NOT )
    {
        idSGameUtils::TriggerMenu( clientNum, MN_CMD_SPEC );
        return;
    }
    
    if( cmds[ i ].cmdFlags & CMD_ALIEN &&
            ent->client->pers.teamSelection != TEAM_ALIENS )
    {
        idSGameUtils::TriggerMenu( clientNum, MN_CMD_ALIEN );
        return;
    }
    
    if( cmds[ i ].cmdFlags & CMD_HUMAN &&
            ent->client->pers.teamSelection != TEAM_HUMANS )
    {
        idSGameUtils::TriggerMenu( clientNum, MN_CMD_HUMAN );
        return;
    }
    
    if( cmds[ i ].cmdFlags & CMD_LIVING &&
            ( ent->client->ps.stats[ STAT_HEALTH ] <= 0 ||
              ent->client->sess.spectatorState != SPECTATOR_NOT ) )
    {
        idSGameUtils::TriggerMenu( clientNum, MN_CMD_LIVING );
        return;
    }
    
    cmds[ i ].cmdHandler( ent );
}

/*
=================
idSGameCmds::SayArgc
idSGameCmds::SayArgv
idSGameCmds::SayConcatArgs

trap_Argc, trap_Argv, and ConcatArgs consider say text as a single argument
These functions assemble the text and re-parse it on word boundaries
=================
*/
sint idSGameCmds::SayArgc( void )
{
    sint c = 0;
    valueType* s;
    
    s = ConcatArgs( 0 );
    while( 1 )
    {
        while( *s == ' ' )
            s++;
        if( !*s )
            break;
        c++;
        while( *s && *s != ' ' )
            s++;
    }
    return c;
}

bool idSGameCmds::SayArgv( sint n, valueType* buffer, sint bufferLength )
{
    valueType* s;
    
    if( bufferLength < 1 )
        return false;
    if( n < 0 )
        return false;
    s = ConcatArgs( 0 );
    while( 1 )
    {
        while( *s == ' ' )
            s++;
        if( !*s || n == 0 )
            break;
        n--;
        while( *s && *s != ' ' )
            s++;
    }
    if( n > 0 )
        return false;
    //memccpy( buffer, s, ' ', bufferLength );
    while( *s && *s != ' ' && bufferLength > 1 )
    {
        *buffer++ = *s++;
        bufferLength--;
    }
    *buffer = 0;
    return true;
}

valueType* idSGameCmds::SayConcatArgs( sint start )
{
    valueType* s;
    
    s = ConcatArgs( 0 );
    while( 1 )
    {
        while( *s == ' ' )
            s++;
        if( !*s || start == 0 )
            break;
        start--;
        while( *s && *s != ' ' )
            s++;
    }
    return s;
}

void idSGameCmds::DecolorString( valueType* in, valueType* out, sint len )
{
    len--;
    
    while( *in && len > 0 )
    {
        if( Q_IsColorString( in ) )
        {
            in++;
            if( *in )
                in++;
            continue;
        }
        *out++ = *in++;
        len--;
    }
    *out = '\0';
}

void idSGameCmds::PrivateMessage_f( gentity_t* ent )
{
    sint pids[ MAX_CLIENTS ];
    sint ignoreids[ MAX_CLIENTS ];
    valueType name[ MAX_NAME_LENGTH ];
    valueType cmd[ 12 ];
    valueType str[ MAX_STRING_CHARS ];
    valueType* msg;
    valueType color;
    sint pcount, matches, ignored = 0;
    sint i;
    sint skipargs = 0;
    bool teamonly = false;
    gentity_t* tmpent;
    
    if( !g_privateMessages.integer && ent )
    {
        adminLocal.ADMP( "Sorry, but private messages have been disabled\n" );
        return;
    }
    
    SayArgv( 0, cmd, sizeof( cmd ) );
    if( !Q_stricmp( cmd, "say" ) || !Q_stricmp( cmd, "say_team" ) )
    {
        skipargs = 1;
        SayArgv( 1, cmd, sizeof( cmd ) );
    }
    if( SayArgc( ) < 3 + skipargs )
    {
        adminLocal.ADMP( va( "usage: %s [name|slot#] [message]\n", cmd ) );
        return;
    }
    
    if( !Q_stricmp( cmd, "mt" ) || !Q_stricmp( cmd, "/mt" ) )
    {
        teamonly = true;
    }
    
    SayArgv( 1 + skipargs, name, sizeof( name ) );
    msg = SayConcatArgs( 2 + skipargs );
    pcount = ClientNumbersFromString( name, pids, MAX_CLIENTS );
    
    if( ent )
    {
        sint count = 0;
        
        for( i = 0; i < pcount; i++ )
        {
            tmpent = &g_entities[ pids[ i ] ];
            
            if( teamonly && !idSGameTeam::OnSameTeam( ent, tmpent ) )
            {
                continue;
            }
            
            if( bggame->ClientListTest( &tmpent->client->sess.ignoreList, ent - g_entities ) )
            {
                ignoreids[ ignored++ ] = pids[ i ];
                continue;
            }
            
            pids[ count ] = pids[ i ];
            count++;
        }
        matches = count;
    }
    else
    {
        matches = pcount;
    }
    
    color = teamonly ? COLOR_CYAN : COLOR_YELLOW;
    
    Q_vsprintf_s( str, sizeof( str ), sizeof( str ), "^%csent to %i player%s: ^7", color, matches, ( matches == 1 ) ? "" : "s" );
    
    for( i = 0; i < matches; i++ )
    {
        tmpent = &g_entities[ pids[ i ] ];
        
        if( i > 0 )
        {
            Q_strcat( str, sizeof( str ), "^7, " );
        }
        
        Q_strcat( str, sizeof( str ), tmpent->client->pers.netname );
        trap_SendServerCommand( pids[ i ], va(
                                    "chat \"%s^%c -> ^7%s^7: (%d recipient%s): ^%c%s^7\" %i",
                                    ( ent ) ? ent->client->pers.netname : "console",
                                    color,
                                    name,
                                    matches,
                                    ( matches == 1 ) ? "" : "s",
                                    color,
                                    msg,
                                    ent ? ( sint )( ent - g_entities ) : -1 ) );
        if( ent )
        {
            trap_SendServerCommand( pids[ i ], va(
                                        "print \">> to reply, say: /m %d [your message] <<\n\"",
                                        ( sint )( ent - g_entities ) ) );
        }
        trap_SendServerCommand( pids[ i ], va(
                                    "cp \"^%cprivate message from ^7%s^7\"", color,
                                    ( ent ) ? ent->client->pers.netname : "console" ) );
    }
    
    if( !matches )
        adminLocal.ADMP( va( "^3No player matching ^7\'%s^7\' ^3to send message to.\n",
                             name ) );
    else
    {
        adminLocal.ADMP( va( "^%cPrivate message: ^7%s\n", color, msg ) );
        adminLocal.ADMP( va( "%s\n", str ) );
        
        idSGameMain::LogPrintf( "%s: %s^7: %s^7: %s\n",
                                ( teamonly ) ? "tprivmsg" : "privmsg",
                                ( ent ) ? ent->client->pers.netname : "console",
                                name, msg );
    }
    
    if( ignored )
    {
        Q_vsprintf_s( str, sizeof( str ), sizeof( str ), "^%cignored by %i player%s: ^7", color,
                      ignored, ( ignored == 1 ) ? "" : "s" );
        for( i = 0; i < ignored; i++ )
        {
            tmpent = &g_entities[ ignoreids[ i ] ];
            if( i > 0 )
                Q_strcat( str, sizeof( str ), "^7, " );
            Q_strcat( str, sizeof( str ), tmpent->client->pers.netname );
        }
        adminLocal.ADMP( va( "%s\n", str ) );
    }
}

/*
=================
idSGameCmds::AdminMessage_f

Send a message to all active admins
=================
*/
void idSGameCmds::AdminMessage_f( gentity_t* ent )
{
    valueType cmd[ sizeof( "say_team" ) ];
    valueType prefix[ 50 ];
    valueType* msg;
    sint skiparg = 0;
    
    // Check permissions and add the appropriate user [prefix]
    if( !ent )
    {
        Q_vsprintf_s( prefix, sizeof( prefix ), sizeof( prefix ), "[CONSOLE]:" );
    }
    else if( !adminLocal.AdminPermission( ent, ADMF_ADMINCHAT ) )
    {
        if( !g_publicAdminMessages.integer )
        {
            adminLocal.ADMP( "Sorry, but use of /a by non-admins has been disabled.\n" );
            return;
        }
        else
        {
            Q_vsprintf_s( prefix, sizeof( prefix ), sizeof( prefix ), "[PLAYER] %s" S_COLOR_WHITE ":",
                          ent->client->pers.netname );
            adminLocal.ADMP( "Your message has been sent to any available admins and to the server logs.\n" );
        }
    }
    else
    {
        Q_vsprintf_s( prefix, sizeof( prefix ), sizeof( prefix ), "[ADMIN] %s" S_COLOR_WHITE ":",
                      ent->client->pers.netname );
    }
    
    // Skip say/say_team if this was used from one of those
    SayArgv( 0, cmd, sizeof( cmd ) );
    if( !Q_stricmp( cmd, "say" ) || !Q_stricmp( cmd, "say_team" ) )
    {
        skiparg = 1;
        SayArgv( 1, cmd, sizeof( cmd ) );
    }
    if( SayArgc( ) < 2 + skiparg )
    {
        adminLocal.ADMP( va( "usage: %s [message]\n", cmd ) );
        return;
    }
    
    msg = SayConcatArgs( 1 + skiparg );
    
    // Send it
    idSGameMain::AdminMessage( prefix, "%s", msg );
}
