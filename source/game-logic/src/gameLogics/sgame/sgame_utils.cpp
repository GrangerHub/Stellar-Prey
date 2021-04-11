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
// File name:   sgame_utils.cpp
// Created:
// Compilers:   Microsoft (R) C/C++ Optimizing Compiler Version 19.26.28806 for x64,
//              gcc (Ubuntu 9.3.0-10ubuntu2) 9.3.0
// Description: misc utility functions for game module
// -------------------------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////////////

#include <sgame/sgame_precompiled.hpp>

/*
===============
idSGameUtils::idSGameUtils
===============
*/
idSGameUtils::idSGameUtils( void )
{
}

/*
===============
idSGameWeapons::~idSGameWeapons
===============
*/
idSGameUtils::~idSGameUtils( void )
{
}

/*
================
idSGameUtils::AddRemap
================
*/
void idSGameUtils::AddRemap( pointer oldShader, pointer newShader, float32 timeOffset )
{
    sint i;
    
    for( i = 0; i < remapCount; i++ )
    {
        if( Q_stricmp( oldShader, remappedShaders[ i ].oldShader ) == 0 )
        {
            // found it, just update this one
            strcpy( remappedShaders[ i ].newShader, newShader );
            remappedShaders[ i ].timeOffset = timeOffset;
            return;
        }
    }
    
    if( remapCount < MAX_SHADER_REMAPS )
    {
        strcpy( remappedShaders[ remapCount ].newShader, newShader );
        strcpy( remappedShaders[ remapCount ].oldShader, oldShader );
        remappedShaders[ remapCount ].timeOffset = timeOffset;
        remapCount++;
    }
}

/*
================
idSGameUtils::BuildShaderStateConfig
================
*/
pointer idSGameUtils::BuildShaderStateConfig( void )
{
    static valueType buff[ MAX_STRING_CHARS * 4 ];
    valueType out[( MAX_QPATH * 2 ) + 5 ];
    sint i;
    
    ::memset( buff, 0, MAX_STRING_CHARS );
    
    for( i = 0; i < remapCount; i++ )
    {
        Q_vsprintf_s( out, ( MAX_QPATH * 2 ) + 5, ( MAX_QPATH * 2 ) + 5, "%s=%s:%5.2f@", remappedShaders[ i ].oldShader, remappedShaders[ i ].newShader, remappedShaders[ i ].timeOffset );
        Q_strcat( buff, sizeof( buff ), out );
    }
    return buff;
}

/*
=========================================================================
Model / Sound configstring indexes
=========================================================================
*/

/*
================
idSGameUtils::FindConfigstringIndex
================
*/
sint idSGameUtils::FindConfigstringIndex( valueType* name, sint start, sint max, bool create )
{
    sint i;
    valueType s[ MAX_STRING_CHARS ];
    
    if( !name || !name[0] )
    {
        return 0;
    }
    
    for( i = 1; i < max; i++ )
    {
        trap_GetConfigstring( start + i, s, sizeof( s ) );
        if( !s[0] )
        {
            break;
        }
        
        if( !strcmp( s, name ) )
        {
            return i;
        }
    }
    
    if( !create )
    {
        return 0;
    }
    
    if( i == max )
    {
        idSGameMain::Error( "idSGameUtils::FindConfigstringIndex: overflow" );
    }
    
    trap_SetConfigstring( start + i, name );
    
    return i;
}

/*
===============
idSGameUtils::ParticleSystemIndex
===============
*/
sint idSGameUtils::ParticleSystemIndex( valueType* name )
{
    return FindConfigstringIndex( name, CS_PARTICLE_SYSTEMS, MAX_GAME_PARTICLE_SYSTEMS, true );
}

/*
===============
idSGameUtils::ShaderIndex
===============
*/
sint idSGameUtils::ShaderIndex( valueType* name )
{
    return FindConfigstringIndex( name, CS_SHADERS, MAX_GAME_SHADERS, true );
}

/*
===============
idSGameUtils::ModelIndex
===============
*/
sint idSGameUtils::ModelIndex( valueType* name )
{
    return FindConfigstringIndex( name, CS_MODELS, MAX_MODELS, true );
}

/*
===============
idSGameUtils::SoundIndex
===============
*/
sint idSGameUtils::SoundIndex( valueType* name )
{
    return FindConfigstringIndex( name, CS_SOUNDS, MAX_SOUNDS, true );
}

/*
================
idSGameUtils::TeamCommand

Broadcasts a command to only a specific team
================
*/
void idSGameUtils::TeamCommand( team_t team, valueType* cmd )
{
    sint   i;
    
    for( i = 0 ; i < level.maxclients ; i++ )
    {
        if( level.clients[ i ].pers.connected == CON_CONNECTED )
        {
            if( level.clients[ i ].pers.teamSelection == team ||
                    ( level.clients[ i ].pers.teamSelection == TEAM_NONE && adminLocal.AdminPermission( &g_entities[ i ], ADMF_SPEC_ALLCHAT ) ) ||
                    ( level.clients[ i ].pers.teamSelection == TEAM_NONE && level.clients[ i ].sess.spectatorState == SPECTATOR_FOLLOW &&
                      level.clients[ i ].sess.spectatorClient >= 0 && level.clients[ level.clients[ i ].sess.spectatorClient ].pers.teamSelection == team ) )
                trap_SendServerCommand( i, cmd );
        }
    }
}


/*
=============
idSGameUtils::Find

Searches all active entities for the next one that holds
the matching string at fieldofs (use the FOFS() macro) in the structure.

Searches beginning at the entity after from, or the beginning if nullptr
nullptr will be returned if the end of the list is reached.

=============
*/
gentity_t* idSGameUtils::Find( gentity_t* from, sint fieldofs, pointer match )
{
    valueType*  s;
    
    if( !from )
    {
        from = g_entities;
    }
    else
    {
        from++;
    }
    
    for( ; from < &g_entities[ level.num_entities ]; from++ )
    {
        if( !from->inuse )
        {
            continue;
        }
        s = *( valueType** )( ( uchar8* )from + fieldofs );
        
        if( !s )
        {
            continue;
        }
        
        if( !Q_stricmp( s, match ) )
        {
            return from;
        }
    }
    
    return nullptr;
}


/*
=============
idSGameUtils::PickTarget

Selects a random entity from among the targets
=============
*/
#define MAXCHOICES  32

gentity_t* idSGameUtils::PickTarget( valueType* targetname )
{
    gentity_t* ent = nullptr;
    sint num_choices = 0;
    gentity_t* choice[ MAXCHOICES ];
    
    if( !targetname )
    {
        idSGameMain::Printf( "idSGameUtils::PickTarget: Called with nullptr targetname\n" );
        return nullptr;
    }
    
    while( 1 )
    {
        ent = Find( ent, FOFS( targetname ), targetname );
        
        if( !ent )
        {
            break;
        }
        
        choice[ num_choices++ ] = ent;
        
        if( num_choices == MAXCHOICES )
        {
            break;
        }
    }
    
    if( !num_choices )
    {
        idSGameMain::Printf( "idSGameUtils::PickTarget: Target %s not found\n", targetname );
        return nullptr;
    }
    
    return choice[ rand( ) % num_choices ];
}


/*
==============================
idSGameUtils::UseTargets

"activator" should be set to the entity that initiated the firing.

Search for (string)targetname in all entities that
match (string)self.target and call their .use function
==============================
*/
void idSGameUtils::UseTargets( gentity_t* ent, gentity_t* activator )
{
    gentity_t* t;
    
    if( !ent )
    {
        return;
    }
    
    if( ent->targetShaderName && ent->targetShaderNewName )
    {
        float32 f = level.time * 0.001;
        AddRemap( ent->targetShaderName, ent->targetShaderNewName, f );
        trap_SetConfigstring( CS_SHADERSTATE, BuildShaderStateConfig( ) );
    }
    
    if( !ent->target )
    {
        return;
    }
    
    t = nullptr;
    while( ( t = Find( t, FOFS( targetname ), ent->target ) ) != nullptr )
    {
        if( t == ent )
        {
            idSGameMain::Printf( "idSGameUtils::UseTargets: Entity used itself.\n" );
        }
        else
        {
            if( t->use )
            {
                t->use( t, ent, activator );
            }
        }
        
        if( !ent->inuse )
        {
            idSGameMain::Printf( "idSGameUtils::UseTargets: Entity was removed while using targets\n" );
            return;
        }
    }
}

/*
=============
idSGameUtils::VectorToString

This is just a convenience function
for printing vectors
=============
*/
valueType* idSGameUtils::vtos( const vec3_t v )
{
    static sint index;
    static valueType str[ 8 ][ 32 ];
    valueType* s;
    
    // use an array so that multiple vtos won't collide
    s = str[ index ];
    index = ( index + 1 ) & 7;
    
    Q_vsprintf_s( s, 32, 32, "(%i %i %i)", ( sint )v[ 0 ], ( sint )v[ 1 ], ( sint )v[ 2 ] );
    
    return s;
}


/*
===============
idSGameUtils::SetMovedir

The editor only specifies a single value for angles (yaw),
but we have special constants to generate an up or down direction.
Angles will be cleared, because it is being used to represent a direction
instead of an orientation.
===============
*/
void idSGameUtils::SetMovedir( vec3_t angles, vec3_t movedir )
{
    static vec3_t VEC_UP = { 0, -1, 0 };
    static vec3_t MOVEDIR_UP = { 0, 0, 1 };
    static vec3_t VEC_DOWN = { 0, -2, 0 };
    static vec3_t MOVEDIR_DOWN = { 0, 0, -1 };
    
    if( VectorCompare( angles, VEC_UP ) )
    {
        VectorCopy( MOVEDIR_UP, movedir );
    }
    else if( VectorCompare( angles, VEC_DOWN ) )
    {
        VectorCopy( MOVEDIR_DOWN, movedir );
    }
    else
    {
        AngleVectors( angles, movedir, nullptr, nullptr );
    }
    
    VectorClear( angles );
}

/*
===============
idSGameUtils::InitGentity
===============
*/
void idSGameUtils::InitGentity( gentity_t* e )
{
    e->inuse = true;
    e->classname = "noclass";
    e->s.number = e - g_entities;
    e->r.ownerNum = ENTITYNUM_NONE;
}

/*
=================
idSGameUtils::Spawn

Either finds a free entity, or allocates a new one.

The slots from 0 to MAX_CLIENTS-1 are always reserved for clients, and will
never be used by anything else.

Try to avoid reusing an entity that was recently freed, because it
can cause the client to think the entity morphed into something else
instead of being removed and recreated, which can cause interpolated
angles and bad trails.
=================
*/
gentity_t* idSGameUtils::Spawn( void )
{
    sint i, force;
    gentity_t* e;
    
    e = nullptr; // shut up warning
    i = 0;    // shut up warning
    
    for( force = 0; force < 2; force++ )
    {
        // if we go through all entities and can't find one to free,
        // override the normal minimum times before use
        e = &g_entities[ MAX_CLIENTS ];
        
        for( i = MAX_CLIENTS; i < level.num_entities; i++, e++ )
        {
            if( e->inuse )
            {
                continue;
            }
            
            // the first couple seconds of server time can involve a lot of
            // freeing and allocating, so relax the replacement policy
            if( !force && e->freetime > level.startTime + 2000 && level.time - e->freetime < 1000 )
            {
                continue;
            }
            
            // reuse this slot
            InitGentity( e );
            return e;
        }
        
        if( i != MAX_GENTITIES )
        {
            break;
        }
    }
    
    if( i == ENTITYNUM_MAX_NORMAL )
    {
        for( i = 0; i < MAX_GENTITIES; i++ )
        {
            idSGameMain::Printf( "%4i: %s\n", i, g_entities[i].classname );
        }
        
        idSGameMain::Error( "idSGameUtils::Spawn: no free entities" );
    }
    
    // open up a new slot
    level.num_entities++;
    
    // let the server system know that there are more entities
    trap_LocateGameData( level.gentities, level.num_entities, sizeof( gentity_t ),
                         &level.clients[ 0 ].ps, sizeof( level.clients[ 0 ] ) );
                         
    InitGentity( e );
    return e;
}

/*
=================
idSGameUtils::FreeEntity

Marks the entity as free
=================
*/
void idSGameUtils::FreeEntity( gentity_t* ent )
{
    trap_UnlinkEntity( ent );   // unlink from world
    
    if( ent->neverFree )
    {
        return;
    }
    
    ::memset( ent, 0, sizeof( *ent ) );
    ent->classname = "freent";
    ent->freetime = level.time;
    ent->inuse = false;
}

/*
=================
idSGameUtils::TempEntity

Spawns an event entity that will be auto-removed
The origin will be snapped to save net bandwidth, so care
must be taken if the origin is right on a surface (snap towards start vector first)
=================
*/
gentity_t* idSGameUtils::TempEntity( vec3_t origin, sint _event )
{
    gentity_t* e;
    vec3_t    snapped;
    
    e = Spawn( );
    e->s.eType = ( entityType_t )( ET_EVENTS + _event );
    
    e->classname = "tempEntity";
    e->eventTime = level.time;
    e->freeAfterEvent = true;
    
    VectorCopy( origin, snapped );
    SnapVector( snapped );    // save network bandwidth
    idSGameUtils::SetOrigin( e, snapped );
    
    // find cluster for PVS
    trap_LinkEntity( e );
    
    return e;
}

/*
=================
idSGameUtils::KillBox

Kills all entities that would touch the proposed new positioning
of ent.  Ent should be unlinked before calling this!
=================
*/
void idSGameUtils::KillBox( gentity_t* ent )
{
    sint i, num, touch[ MAX_GENTITIES ];
    gentity_t* hit;
    vec3_t mins, maxs;
    
    VectorAdd( ent->client->ps.origin, ent->r.mins, mins );
    VectorAdd( ent->client->ps.origin, ent->r.maxs, maxs );
    num = trap_EntitiesInBox( mins, maxs, touch, MAX_GENTITIES );
    
    for( i = 0; i < num; i++ )
    {
        hit = &g_entities[ touch[ i ] ];
        
        if( !hit->client )
        {
            continue;
        }
        
        // impossible to telefrag self
        if( ent == hit )
        {
            continue;
        }
        
        // nail it
        idSGameCombat::Damage( hit, ent, ent, nullptr, nullptr, 100000, DAMAGE_NO_PROTECTION, MOD_TELEFRAG );
    }
    
}

/*
===============
idSGameUtils::AddPredictableEvent

Use for non-pmove events that would also be predicted on the
client side: jumppads and item pickups
Adds an event+parm and twiddles the event counter
===============
*/
void idSGameUtils::AddPredictableEvent( gentity_t* ent, sint _event, sint eventParm )
{
    if( !ent->client )
    {
        return;
    }
    
    bggame->AddPredictableEventToPlayerstate( _event, eventParm, &ent->client->ps );
}


/*
===============
idSGameUtils::AddEvent

Adds an event+parm and twiddles the event counter
===============
*/
void idSGameUtils::AddEvent( gentity_t* ent, sint _event, sint eventParm )
{
    sint bits;
    
    if( !_event )
    {
        idSGameMain::Printf( "idSGameUtils::AddEvent: zero event added for entity %i\n", ent->s.number );
        return;
    }
    
    // eventParm is converted to uint8_t (0 - 255) in msg.c
    if( eventParm & ~0xFF )
    {
        idSGameMain::Printf( S_COLOR_YELLOW "WARNING: G_AddEvent( %s ) has eventParm %d, "
                             "which will overflow\n", bggame->EventName( _event ), eventParm );
    }
    
    // clients need to add the event in playerState_t instead of entityState_t
    if( ent->client )
    {
        bits = ent->client->ps.externalEvent & EV_EVENT_BITS;
        bits = ( bits + EV_EVENT_BIT1 ) & EV_EVENT_BITS;
        ent->client->ps.externalEvent = _event | bits;
        ent->client->ps.externalEventParm = eventParm;
        ent->client->ps.externalEventTime = level.time;
    }
    else
    {
        bits = ent->s._event & EV_EVENT_BITS;
        bits = ( bits + EV_EVENT_BIT1 ) & EV_EVENT_BITS;
        ent->s._event = _event | bits;
        ent->s.eventParm = eventParm;
    }
    
    ent->eventTime = level.time;
}

/*
===============
idSGameUtils::BroadcastEvent

Sends an event to every client
===============
*/
void idSGameUtils::BroadcastEvent( sint _event, sint eventParm )
{
    gentity_t* ent;
    
    ent = TempEntity( vec3_origin, _event );
    ent->s.eventParm = eventParm;
    ent->r.svFlags = SVF_BROADCAST; // send to everyone
}

/*
=============
idSGameUtils::Sound
=============
*/
void idSGameUtils::Sound( gentity_t* ent, sint channel, sint soundIndex )
{
    gentity_t* te;
    
    te = TempEntity( ent->r.currentOrigin, EV_GENERAL_SOUND );
    te->s.eventParm = soundIndex;
}

/*
================
idSGameUtils::SetOrigin

Sets the pos trajectory for a fixed position
================
*/
void idSGameUtils::SetOrigin( gentity_t* ent, vec3_t origin )
{
    VectorCopy( origin, ent->s.pos.trBase );
    ent->s.pos.trType = TR_STATIONARY;
    ent->s.pos.trTime = 0;
    ent->s.pos.trDuration = 0;
    VectorClear( ent->s.pos.trDelta );
    
    VectorCopy( origin, ent->r.currentOrigin );
    VectorCopy( origin, ent->s.origin );
}

/*
==============
idSGameUtils::SetOrigin
==============
*/
void idSGameUtils::SetAngle( gentity_t* ent, vec3_t angle )
{

    VectorCopy( angle, ent->s.apos.trBase );
    ent->s.apos.trType = TR_STATIONARY;
    ent->s.apos.trTime = 0;
    ent->s.apos.trDuration = 0;
    VectorClear( ent->s.apos.trDelta );
    
    VectorCopy( angle, ent->r.currentAngles );
    
    //	VectorCopy (ent->s.angles, ent->s.apos.trDelta );
    
}

/*
===============
idSGameUtils::Visible

Test for a LOS between two entities
===============
*/
bool idSGameUtils::Visible( gentity_t* ent1, gentity_t* ent2, sint contents )
{
    trace_t trace;
    
    trap_Trace( &trace, ent1->s.pos.trBase, nullptr, nullptr, ent2->s.pos.trBase, ent1->s.number, contents );
    return trace.fraction >= 1.0f || trace.entityNum == ent2 - g_entities;
}

/*
===============
idSGameUtils::ClosestEnt

Test a list of entities for the closest to a particular point
===============
*/
gentity_t* idSGameUtils::ClosestEnt( vec3_t origin, gentity_t** entities, sint numEntities )
{
    sint i;
    float32 nd, d = 1000000.0f;
    gentity_t* closestEnt = nullptr;
    
    for( i = 0; i < numEntities; i++ )
    {
        gentity_t* ent = entities[ i ];
        
        nd = DistanceSquared( origin, ent->s.origin );
        
        if( i == 0 || nd < d )
        {
            d = nd;
            closestEnt = ent;
        }
    }
    
    return closestEnt;
}

/*
===============
idSGameUtils::TriggerMenu

Trigger a menu on some client
===============
*/
void idSGameUtils::TriggerMenu( sint clientNum, dynMenu_t menu )
{
    valueType buffer[ 32 ];
    
    Q_vsprintf_s( buffer, sizeof( buffer ), sizeof( buffer ), "servermenu %d", menu );
    trap_SendServerCommand( clientNum, buffer );
}

/*
===============
idSGameUtils::TriggerMenu2

Trigger a menu on some client and passes an argument
===============
*/
void idSGameUtils::TriggerMenu2( sint clientNum, dynMenu_t menu, sint arg )
{
    valueType buffer[ 64 ];
    
    Q_vsprintf_s( buffer, sizeof( buffer ), sizeof( buffer ), "servermenu %d %d", menu, arg );
    trap_SendServerCommand( clientNum, buffer );
}

/*
===============
idSGameUtils::CloseMenus

Close all open menus on some client
===============
*/
void idSGameUtils::CloseMenus( sint clientNum )
{
    valueType buffer[ 32 ];
    
    Q_vsprintf_s( buffer, 32, 32, "serverclosemenus" );
    trap_SendServerCommand( clientNum, buffer );
}
