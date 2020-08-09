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
//  -------------------------------------------------------------------------------------
// File name:   sgame_misc.cpp
// Version:     v1.01
// Created:
// Compilers:   Visual Studio 2019, gcc 7.3.0
// Description:
// -------------------------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////////////

#include <sgame/sgame_precompiled.h>

/*
===============
idSGameMissile::idSGameMissile
===============
*/
idSGameMisc::idSGameMisc( void )
{
}

/*
===============
idSGameMisc::~idSGameMisc
===============
*/
idSGameMisc::~idSGameMisc( void )
{
}

/*QUAKED func_group (0 0 0) ?
Used to group brushes together just for editor convenience.  They are turned into normal brushes by the utilities.
*/


/*QUAKED info_null (0 0.5 0) (-4 -4 -4) (4 4 4)
Used as a positional target for calculations in the utilities (spotlights, etc), but removed during gameplay.
*/
void idSGameMisc::SP_info_null( gentity_t* self )
{
    idSGameUtils::FreeEntity( self );
}

/*QUAKED info_notnull (0 0.5 0) (-4 -4 -4) (4 4 4)
Used as a positional target for in-game calculation, like jumppad targets.
target_position does the same thing
*/
void idSGameMisc::SP_info_notnull( gentity_t* self )
{
    idSGameUtils::SetOrigin( self, self->s.origin );
}

/*QUAKED light (0 1 0) (-8 -8 -8) (8 8 8) linear
Non-displayed light.
"light" overrides the default 300 intensity.
Linear checbox gives linear falloff instead of inverse square
Lights pointed at a target will be spotlights.
"radius" overrides the default 64 unit radius of a spotlight at the target point.
*/
void idSGameMisc::SP_light( gentity_t* self )
{
    idSGameUtils::FreeEntity( self );
}

/*
=================================================================================

TELEPORTERS

=================================================================================
*/

void idSGameMisc::TeleportPlayer( gentity_t* player, vec3_t origin, vec3_t angles )
{
    // unlink to make sure it can't possibly interfere with idSGameMisc::KillBox
    trap_UnlinkEntity( player );
    
    VectorCopy( origin, player->client->ps.origin );
    player->client->ps.origin[2] += 1;
    
    // spit the player out
    AngleVectors( angles, player->client->ps.velocity, nullptr, nullptr );
    VectorScale( player->client->ps.velocity, 400, player->client->ps.velocity );
    player->client->ps.pm_time = 160;   // hold time
    player->client->ps.pm_flags |= PMF_TIME_KNOCKBACK;
    
    // toggle the teleport bit so the client knows to not lerp
    player->client->ps.eFlags ^= EF_TELEPORT_BIT;
    idSGameActive::UnlaggedClear( player );
    
    // set angles
    idSGameClient::SetClientViewAngle( player, angles );
    
    // kill anything at the destination
    if( player->client->sess.spectatorState == SPECTATOR_NOT )
    {
        idSGameUtils::KillBox( player );
    }
    
    // save results of pmove
    bggame->PlayerStateToEntityState( &player->client->ps, &player->s, true );
    
    // use the precise origin for linking
    VectorCopy( player->client->ps.origin, player->r.currentOrigin );
    
    if( player->client->sess.spectatorState == SPECTATOR_NOT )
    {
        trap_LinkEntity( player );
    }
}


/*QUAKED misc_teleporter_dest (1 0 0) (-32 -32 -24) (32 32 -16)
Point teleporters at these.
Now that we don't have teleport destination pads, this is just
an info_notnull
*/
void idSGameMisc::SP_misc_teleporter_dest( gentity_t* ent )
{
}


//===========================================================

/*QUAKED misc_model (1 0 0) (-16 -16 -16) (16 16 16)
"model"   arbitrary .md3 file to display
*/
void idSGameMisc::SP_misc_model( gentity_t* ent )
{
#if 0
    ent->s.modelindex = idSGameUtils::ModelIndex( ent->model );
    VectorSet( ent->mins, -16, -16, -16 );
    VectorSet( ent->maxs, 16, 16, 16 );
    trap_LinkEntity( ent );
    
    idSGameUtils::SetOrigin( ent, ent->s.origin );
    VectorCopy( ent->s.angles, ent->s.apos.trBase );
#else
    idSGameUtils::FreeEntity( ent );
#endif
}

//===========================================================

void idSGameMisc::locateCamera( gentity_t* ent )
{
    vec3_t    dir;
    gentity_t* target;
    gentity_t* owner;
    
    owner = idSGameUtils::PickTarget( ent->target );
    if( !owner )
    {
        idSGameMain::Printf( "Couldn't find target for misc_portal_surface\n" );
        idSGameUtils::FreeEntity( ent );
        return;
    }
    ent->r.ownerNum = owner->s.number;
    
    // frame holds the rotate speed
    if( owner->spawnflags & 1 )
        ent->s.frame = 25;
    else if( owner->spawnflags & 2 )
        ent->s.frame = 75;
        
    // swing camera ?
    if( owner->spawnflags & 4 )
    {
        // set to 0 for no rotation at all
        ent->s.misc = 0;
    }
    else
        ent->s.misc = 1;
        
    // clientNum holds the rotate offset
    ent->s.clientNum = owner->s.clientNum;
    
    VectorCopy( owner->s.origin, ent->s.origin2 );
    
    // see if the portal_camera has a target
    target = idSGameUtils::PickTarget( owner->target );
    if( target )
    {
        VectorSubtract( target->s.origin, owner->s.origin, dir );
        VectorNormalize( dir );
    }
    else
        idSGameUtils::SetMovedir( owner->s.angles, dir );
        
    ent->s.eventParm = DirToByte( dir );
}

/*QUAKED misc_portal_surface (0 0 1) (-8 -8 -8) (8 8 8)
The portal surface nearest this entity will show a view from the targeted misc_portal_camera, or a mirror view if untargeted.
This must be within 64 world units of the surface!
*/
void idSGameMisc::SP_misc_portal_surface( gentity_t* ent )
{
    VectorClear( ent->r.mins );
    VectorClear( ent->r.maxs );
    trap_LinkEntity( ent );
    
    ent->r.svFlags = SVF_PORTAL;
    ent->s.eType = ET_PORTAL;
    
    if( !ent->target )
    {
        VectorCopy( ent->s.origin, ent->s.origin2 );
    }
    else
    {
        ent->think = locateCamera;
        ent->nextthink = level.time + 100;
    }
}

/*QUAKED misc_portal_camera (0 0 1) (-8 -8 -8) (8 8 8) slowrotate fastrotate noswing

The target for a misc_portal_director.  You can set either angles or target another entity to determine the direction of view.
"roll" an angle modifier to orient the camera around the target vector;
*/
void idSGameMisc::SP_misc_portal_camera( gentity_t* ent )
{
    float32 roll;
    
    VectorClear( ent->r.mins );
    VectorClear( ent->r.maxs );
    trap_LinkEntity( ent );
    
    idSGameSpawn::SpawnFloat( "roll", "0", &roll );
    
    ent->s.clientNum = roll / 360.0f * 256;
}

void idSGameMisc::SP_toggle_particle_system( gentity_t* self )
{
    //toggle EF_NODRAW
    self->s.eFlags ^= EF_NODRAW;
    
    self->nextthink = 0;
}

/*
===============
SP_use_particle_system

Use function for particle_system
===============
*/
void idSGameMisc::SP_use_particle_system( gentity_t* self, gentity_t* other, gentity_t* activator )
{
    SP_toggle_particle_system( self );
    
    if( self->wait > 0.0f )
    {
        self->think = SP_toggle_particle_system;
        self->nextthink = level.time + ( sint )( self->wait * 1000 );
    }
}

/*
===============
SP_spawn_particle_system

Spawn function for particle system
===============
*/
void idSGameMisc::SP_misc_particle_system( gentity_t* self )
{
    valueType*  s;
    
    idSGameUtils::SetOrigin( self, self->s.origin );
    
    idSGameSpawn::SpawnString( "psName", "", &s );
    idSGameSpawn::SpawnFloat( "wait", "0", &self->wait );
    
    //add the particle system to the client precache list
    self->s.modelindex = idSGameUtils::ParticleSystemIndex( s );
    
    if( self->spawnflags & 1 )
        self->s.eFlags |= EF_NODRAW;
        
    self->use = SP_use_particle_system;
    self->s.eType = ET_PARTICLE_SYSTEM;
    trap_LinkEntity( self );
}

/*
===============
SP_use_anim_model

Use function for anim model
===============
*/
void idSGameMisc::SP_use_anim_model( gentity_t* self, gentity_t* other, gentity_t* activator )
{
    if( self->spawnflags & 1 )
    {
        //if spawnflag 1 is set
        //toggle EF_NODRAW
        if( self->s.eFlags & EF_NODRAW )
            self->s.eFlags &= ~EF_NODRAW;
        else
            self->s.eFlags |= EF_NODRAW;
    }
    else
    {
        //if the animation loops then toggle the animation
        //toggle EF_MOVER_STOP
        if( self->s.eFlags & EF_MOVER_STOP )
            self->s.eFlags &= ~EF_MOVER_STOP;
        else
            self->s.eFlags |= EF_MOVER_STOP;
    }
}

/*
===============
idSGameMisc::SP_misc_anim_model

Spawn function for anim model
===============
*/
void idSGameMisc::SP_misc_anim_model( gentity_t* self )
{
    self->s.misc      = ( sint )self->animation[ 0 ];
    self->s.weapon    = ( sint )self->animation[ 1 ];
    self->s.torsoAnim = ( sint )self->animation[ 2 ];
    self->s.legsAnim  = ( sint )self->animation[ 3 ];
    
    self->s.angles2[ 0 ] = self->pos2[ 0 ];
    
    //add the model to the client precache list
    self->s.modelindex = idSGameUtils::ModelIndex( self->model );
    
    self->use = SP_use_anim_model;
    
    self->s.eType = ET_ANIMMAPOBJ;
    
    // spawn with animation stopped
    if( self->spawnflags & 2 )
        self->s.eFlags |= EF_MOVER_STOP;
        
    trap_LinkEntity( self );
}

/*
===============
SP_use_light_flare

Use function for light flare
===============
*/
void idSGameMisc::SP_use_light_flare( gentity_t* self, gentity_t* other, gentity_t* activator )
{
    self->s.eFlags ^= EF_NODRAW;
}

/*
===============
findEmptySpot

Finds an empty spot radius units from origin
==============
*/
void idSGameMisc::findEmptySpot( vec3_t origin, float32 radius, vec3_t spot )
{
    sint     i, j, k;
    vec3_t  delta, test, total;
    trace_t tr;
    
    VectorClear( total );
    
    //54(!) traces to test for empty spots
    for( i = -1; i <= 1; i++ )
    {
        for( j = -1; j <= 1; j++ )
        {
            for( k = -1; k <= 1; k++ )
            {
                VectorSet( delta, ( i * radius ),
                           ( j * radius ),
                           ( k * radius ) );
                           
                VectorAdd( origin, delta, test );
                
                trap_Trace( &tr, test, nullptr, nullptr, test, -1, MASK_SOLID );
                
                if( !tr.allsolid )
                {
                    trap_Trace( &tr, test, nullptr, nullptr, origin, -1, MASK_SOLID );
                    VectorScale( delta, tr.fraction, delta );
                    VectorAdd( total, delta, total );
                }
            }
        }
    }
    
    VectorNormalize( total );
    VectorScale( total, radius, total );
    VectorAdd( origin, total, spot );
}

/*
===============
SP_misc_light_flare

Spawn function for light flare
===============
*/
void idSGameMisc::SP_misc_light_flare( gentity_t* self )
{
    self->s.eType = ET_LIGHTFLARE;
    self->s.modelindex = idSGameUtils::ShaderIndex( self->targetShaderName );
    VectorCopy( self->pos2, self->s.origin2 );
    
    //try to find a spot near to the flare which is empty. This
    //is used to facilitate visibility testing
    findEmptySpot( self->s.origin, 8.0f, self->s.angles2 );
    
    self->use = SP_use_light_flare;
    
    idSGameSpawn::SpawnFloat( "speed", "200", &self->speed );
    self->s.time = self->speed;
    
    idSGameSpawn::SpawnInt( "mindist", "0", &self->s.generic1 );
    
    if( self->spawnflags & 1 )
        self->s.eFlags |= EF_NODRAW;
        
    trap_LinkEntity( self );
}

/*
===============
SP_misc_cubemap
===============
*/
void idSGameMisc::SP_misc_cubemap( gentity_t* ent )
{
    idSGameUtils::FreeEntity( ent );
}

/*
===================
idSGameMisc::Sound_Think
===================
*/
void idSGameMisc::Sound_Think( gentity_t* ent )
{
    trap_SendServerCommand( -1, va( "playsound %s %f %f %f\n", ent->sound, ent->s.origin[0], ent->s.origin[1], ent->s.origin[2] ) );
    ent->nextthink = level.time + 5000;
}

/*
===================
idSGameMisc::func_sound
===================
*/
void idSGameMisc::SP_func_sound( gentity_t* ent )
{
    trap_LinkEntity( ent );
    
    idSGameUtils::SetOrigin( ent, ent->s.origin );
    VectorCopy( ent->s.angles, ent->s.apos.trBase );
    
    ent->think = Sound_Think;
    ent->nextthink = level.time + 1000;
}
